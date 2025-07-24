

#include "CanTp.hpp"
#include <iostream>
#include <iomanip>
#include "Can.hpp"
#include "TransportProtocol.hpp"

void TransportProtocol::sendMessageP(const std::string &message, TransportProtocol &transportProtocol)
{
    CanTp cantp = dynamic_cast<CanTp &>(transportProtocol);
    cantp.sendMessage(message);
}

CanTp::CanTp(uint32_t txId, uint32_t rxId) : txId_(txId), rxId_(rxId) {}

void CanTp::sendMessage(const std::string &message)
{
    if (message.size() <= MAX_SF_DATA)
    {
        sendSingleFrame(message);
    }
    else
    {
        sendMultiFrame(message);
    }
}

void CanTp::sendSingleFrame(const std::string &message)
{
    // Single Frame: [PCI (1 byte) | Data]
    // PCI: 0x00 | Length (4 bits)
    std::vector<uint8_t> data;
    uint8_t pci = (0x00 << 4) | (message.size() & 0x0F); // Single Frame PCI
    data.push_back(pci);
    data.insert(data.end(), message.begin(), message.end()); // Append string bytes
    Can frame(txId_, data);

    std::cout << "Sending Single Frame:" << std::endl;
    frame.print();
}

void CanTp::sendMultiFrame(const std::string &message)
{
    // First Frame: [PCI (2 bytes) | Data]
    // PCI: 0x10 | Length (12 bits)
    std::vector<uint8_t> data;
    uint16_t length = message.size();
    uint8_t pci1 = (0x10) | ((length >> 8) & 0x0F); // First Frame PCI, high length
    uint8_t pci2 = length & 0xFF;                   // Low length
    data.push_back(pci1);
    data.push_back(pci2);
    size_t dataBytes = std::min<size_t>(6, message.size()); // Up to 6 bytes in FF
    data.insert(data.end(), message.begin(), message.begin() + dataBytes);
    Can frame(txId_, data);
    std::cout << "Sending First Frame:" << std::endl;
    frame.print(); // Houni Bch yod5l el BusManager

    // Consecutive Frames: [PCI (1 byte) | Data]
    // PCI: 0x20 | Sequence Number (4 bits)
    size_t remaining = message.size() - dataBytes;
    size_t offset = dataBytes;
    uint8_t sequence = 1;
    while (remaining > 0)
    {
        data.clear();
        uint8_t pci = (0x20) | (sequence & 0x0F); // Consecutive Frame PCI
        data.push_back(pci);
        size_t bytesToSend = std::min<size_t>(MAX_CF_DATA, remaining);
        data.insert(data.end(), message.begin() + offset, message.begin() + offset + bytesToSend);
        Can cf(txId_, data);
        std::cout << "Sending Consecutive Frame (SN=" << (int)sequence << "):" << std::endl;
        cf.print(); // houni bch yod5l el BusManager
        remaining -= bytesToSend;
        offset += bytesToSend;
        sequence = (sequence + 1) & 0x0F; // Wrap sequence number
    }
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

        if (frameType == 0x00)
        { // Single Frame
            uint8_t length = pci & 0x0F;
            if (data.size() < length + 1)
            {
                std::cerr << "Invalid Single Frame length" << std::endl;
                return "";
            }
            message.assign(data.begin() + 1, data.begin() + 1 + length);
            return message;
        }
        else if (frameType == 0x10 && !isFirstFrame)
        { // First Frame
            if (data.size() < 2)
            {
                std::cerr << "Invalid First Frame" << std::endl;
                return "";
            }
            expectedLength = ((data[0] & 0x0F) << 8) | data[1];
            message.assign(data.begin() + 2, data.end());
            isFirstFrame = true;
        }
        else if (frameType == 0x20 && isFirstFrame)
        { // Consecutive Frame
            uint8_t sequence = pci & 0x0F;
            if (sequence != expectedSequence)
            {
                std::cerr << "Out-of-sequence Consecutive Frame: expected " << (int)expectedSequence
                          << ", got " << (int)sequence << std::endl;
                return "";
            }
            message.insert(message.end(), data.begin() + 1, data.end());
            expectedSequence = (expectedSequence + 1) & 0x0F;
        }
        else
        {
            return "";
        }
    }

    if (message.size() != expectedLength)
    {
        std::cerr << "Incomplete message: expected " << expectedLength << ", got " << message.size() << std::endl;
        return "";
    }

    return message;
}
