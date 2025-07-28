#include <cstdint>
#include "CanTp.hpp"
#include <iostream>
#include <iomanip>
#include "Can.hpp"
#include "TransportProtocol.hpp"
#include <typeinfo>
#include "Receiver.hpp"
#include "Sender.hpp"
#include "CanBus.hpp"
#include "Ecu.hpp"
const size_t CanTp::MAX_SF_DATA = 7; // Maximum data length for Single Frame
const size_t CanTp::MAX_CF_DATA = 6; // Maximum data length for Consecutive Frame

// Constructor updated to include BusManager reference
CanTp::CanTp(uint32_t txId, uint32_t rxId)
    : txId_(txId), rxId_(rxId), busManager_(*new CanBus()) {}

// Constructor with BusManager reference
CanTp::CanTp(uint32_t txId, uint32_t rxId, BusManager &busManager)
    : txId_(txId), rxId_(rxId), busManager_(busManager) {}

void CanTp::sendMessageP(const std::string &message)
{
    std::cout << "now the cantp start sending the data";
    this->sendMessage(message);
}

void CanTp::sendMessage(const std::string &message)
{
    if (message.size() <= MAX_SF_DATA)
    {
        sendSingleFrame(message);
    }
    else
    {
        sendingMessage = message;
        sendingOffset = 0;
        sendingSequence = 1;
        sendingMultiFrame = true;
        sendMultiFrame(message);
    }
}

void CanTp::sendSingleFrame(const std::string &message)
{
    std::vector<uint8_t> data;
    uint8_t pci = (0x00 << 4) | (message.size() & 0x0F); // Single Frame PCI
    data.push_back(pci);
    data.insert(data.end(), message.begin(), message.end());
    Can frame(txId_, data);
    if (txId_ <= 0x7FF)
    {
        frame.setFrameType(false); // Standard frame
    }
    else
    {
        frame.setFrameType(true); // Extended frame
    }
    busManager_.send(frame); // Use BusManager instead of print
}

void CanTp::sendMultiFrame(const std::string &message)
{
    // Send First Frame
    std::vector<uint8_t> data;
    uint16_t length = message.size();
    uint8_t pci1 = (0x10) | ((length >> 8) & 0x0F); // First Frame PCI, high length
    uint8_t pci2 = length & 0xFF;                   // Low length
    data.push_back(pci1);
    data.push_back(pci2);
    size_t dataBytes = std::min<size_t>(6, message.size());
    data.insert(data.end(), message.begin(), message.begin() + dataBytes);
    Can frame(txId_, data);
    busManager_.send(frame); // Send via BusManager
    sendingOffset = dataBytes;
    // Wait for Flow Control frame
    busManager_.receive();
    CanManager *fcFrame1 = busManager_.getcanMan(); // Replace with actual FC ID

    auto fcFrame = dynamic_cast<Can *>(fcFrame1);
    while (fcFrame->getId() != rxId_ || (fcFrame->getData().size() > 0 && (fcFrame->getData()[0] >> 4) != 0x30))
    {
        busManager_.receive();
        fcFrame1 = busManager_.getcanMan();
    }

    // Process Flow Control
    const auto &fcData = fcFrame->getData();
    if (fcData.size() >= 3)
    {
        uint8_t flowStatus = fcData[1];
        uint8_t blockSize = fcData[2];
        uint8_t separationTime = fcData[3]; // Not used in this simple sim

        if (flowStatus == 0)
        { // ContinueToSend
            sendNextBlock(blockSize);
        }
        else if (flowStatus == 2)
        { // Overflow
            sendingMultiFrame = false;
            std::cerr << "Transmission aborted due to overflow" << std::endl;
        }
        // Wait status (1) would loop back to receive another FC frame if implemented
    }
}

void CanTp::sendNextBlock(uint8_t blockSize)
{
    size_t remaining = sendingMessage.size() - sendingOffset;
    for (uint8_t i = 0; i < blockSize && remaining > 0; ++i)
    {
        std::vector<uint8_t> data;
        uint8_t pci = (0x20) | (sendingSequence & 0x0F); // Consecutive Frame PCI
        data.push_back(pci);
        size_t bytesToSend = std::min<size_t>(MAX_CF_DATA, remaining);
        data.insert(data.end(), sendingMessage.begin() + sendingOffset,
                    sendingMessage.begin() + sendingOffset + bytesToSend);
        Can cf(txId_, data);
        busManager_.send(cf); // Send via BusManager
        sendingOffset += bytesToSend;
        sendingSequence = (sendingSequence + 1) & 0x0F;
        remaining -= bytesToSend;
    }
    if (sendingOffset >= sendingMessage.size())
    {
        sendingMultiFrame = false;
    }
    else
    {
        // Wait for next Flow Control frame
        busManager_.receive();
        CanManager *fcFrame1 = busManager_.getcanMan();
        auto fcFrame = dynamic_cast<Can *>(fcFrame1);
        while (fcFrame->getId() != rxId_ || (fcFrame->getData().size() > 0 && (fcFrame->getData()[0] >> 4) != 0x30))
        {
            busManager_.receive();

            fcFrame1 = busManager_.getcanMan();
        }
        const auto &fcData = fcFrame->getData();
        if (fcData.size() >= 3 && fcData[1] == 0)
        { // ContinueToSend
            sendNextBlock(fcData[2]);
        }
    }
}

void CanTp::sendFlowControl(uint8_t flowStatus, uint8_t blockSize, uint8_t separationTime)
{
    std::vector<uint8_t> data;
    uint8_t pci = 0x30; // Flow Control PCI
    data.push_back(pci);
    data.push_back(flowStatus);
    data.push_back(blockSize);
    data.push_back(separationTime);
    Can frame(rxId_, data);  // Send on rxId_ as it's from receiver to sender
    busManager_.send(frame); // Send via BusManager
}

std::string CanTp::receiveMessage(const std::vector<Can> &frames)
{
    if (frames.empty())
    {
        std::cerr << "No frames received" << std::endl;
        return "";
    }

    std::string message;
    bool isFirstFrame = false;
    size_t expectedLength = 0;
    uint8_t expectedSequence = 1;
    size_t receivedFrames = 0;

    for (const auto &frame : frames)
    {
        if (frame.getId() != rxId_)
        {
            std::cerr << "Invalid CAN ID: " << std::hex << frame.getId() << std::endl;
            continue;
        }

        const auto &data = frame.getData();
        if (data.empty())
        {
            std::cerr << "Empty frame data" << std::endl;
            continue;
        }

        uint8_t pci = data[0];
        uint8_t frameType = (pci >> 4) & 0x0F;

        if (frameType == 0x00) // Single Frame
        {
            uint8_t length = pci & 0x0F;
            if (data.size() < length + 1)
            {
                std::cerr << "Invalid Single Frame length" << std::endl;
                return "";
            }
            message.assign(data.begin() + 1, data.begin() + 1 + length);
            return message;
        }
        else if (frameType == 0x10 && !isFirstFrame) // First Frame
        {
            if (data.size() < 2)
            {
                std::cerr << "Invalid First Frame" << std::endl;
                return "";
            }
            expectedLength = ((data[0] & 0x0F) << 8) | data[1];
            message.assign(data.begin() + 2, data.end());
            isFirstFrame = true;
            sendFlowControl(0, 5, 0); // Send FC: ContinueToSend, blockSize=5
        }
        else if (frameType == 0x20 && isFirstFrame) // Consecutive Frame
        {
            uint8_t sequence = pci & 0x0F;
            if (sequence != expectedSequence)
            {
                std::cerr << "Out-of-sequence Consecutive Frame: expected " << (int)expectedSequence
                          << ", got " << (int)sequence << std::endl;
                return "";
            }
            message.insert(message.end(), data.begin() + 1, data.end());
            expectedSequence = (expectedSequence + 1) & 0x0F;
            receivedFrames++;
            if (receivedFrames % 5 == 0 && message.size() < expectedLength)
            {
                sendFlowControl(0, 5, 0); // Send FC after each block
            }
        }
    }

    if (message.size() != expectedLength)
    {
        std::cerr << "Incomplete message: expected " << expectedLength << ", got " << message.size() << std::endl;
        return "";
    }

    return message;
}
std::string CanTp::receiveMessageP(TransportProtocol &tp, CanManager *cm)
{
    std::vector<Can> frames;
    std::string msg = cm->receiveFrame(tp, cm);
    while (msg != "")

    {
        Ecu::encoder(msg);
    }
    return receiveMessage(frames);
}