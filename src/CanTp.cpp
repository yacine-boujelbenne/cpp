#include <cstdint>
#include "../headers/CanTp.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "../headers/Can.hpp"
#include "../headers/CanBus.hpp"

static const size_t MAX_SF_DATA = 7;
static const size_t MAX_CF_DATA = 6;
static bool situation = true;

CanTp::CanTp(uint32_t txId, uint32_t rxId, CanBus &bus)
    : txId_(txId), rxId_(rxId), busTp_(bus), ownsBus_(false)
{

}

CanTp::CanTp(uint32_t txId, uint32_t rxId)
    : txId_(txId), rxId_(rxId), busTp_(*new CanBus()), ownsBus_(true)
{
    busTp_.init();
}

CanTp::~CanTp() {
    if (ownsBus_) {
        delete &busTp_;
    }
}

void CanTp::sendMessageP(const std::string &message) {
    std::cout << "CAN-TP starting transmission\n";
    this->sendMessage(message);
}

void CanTp::sendMessage(const std::string &message) {
    if (message.size() <= MAX_SF_DATA) {
        sendSingleFrame(message);
    } else {
        sendingMessage = message;
        sendingOffset = 0;
        sendingSequence = 1;
        sendingMultiFrame = true;
        this->sendMultiFrame(true);
        while (situation) {
            this->sendMultiFrame(false);       
        }
        
                
        
    }
}

void CanTp::sendSingleFrame(const std::string &message) {
    std::vector<uint8_t> data;
    uint8_t pci = (0x00 << 4) | (message.size() & 0x0F);
    data.push_back(pci);
    data.insert(data.end(), message.begin(), message.end());
    Can frame(txId_, data);
    frame.setFrameType(txId_ > 0x7FF);
    busTp_.send(frame);
}

void CanTp::sendMultiFrame(bool first) {
    if (first){
        // Send First Frame
        std::vector<uint8_t> data;
        uint16_t length = sendingMessage.size();
        uint8_t pci1 = 0x10 | ((length >> 8) & 0x0F);
        uint8_t pci2 = length & 0xFF;
        data.push_back(pci1);
        data.push_back(pci2);
        size_t dataBytes = std::min<size_t>(6, sendingMessage.size());
        data.insert(data.end(), sendingMessage.begin(), sendingMessage.begin() + dataBytes);
        sendingOffset = dataBytes;
        
        Can frame(txId_, data);
        frame.setFrameType(txId_ > 0x7FF);
        busTp_.send(frame);
        std::cout << "Sent First Frame\n";
        }
    // Wait for Flow Control frame with timeout
    auto start = std::chrono::steady_clock::now();
    const std::chrono::seconds timeout(2);
    bool fcReceived = false;
    
    
    while (!fcReceived && std::chrono::steady_clock::now() - start < timeout) {
        Can fcFrame = busTp_.receiveFrame();
        std::cout << Can::decoder(fcFrame.getData())<<std::endl;
        if (fcFrame.getId() == rxId_ && !fcFrame.getData().empty()) {
            uint8_t pci = fcFrame.getData()[0];
            uint8_t frameType = (pci >> 4) & 0x0F;
            
            if (frameType == 0x3) {  // Flow Control frame
                fcReceived = true;
                const auto& fcData = fcFrame.getData();
                if (fcData.size() >= 3) {
                    uint8_t flowStatus = fcData[0] & 0x0F;
                    uint8_t blockSize = fcData[1];
                    
                    if (flowStatus == 0) {
                        std::cout << "Received Flow Control (Continue)\n";

                        sendNextBlock(blockSize);
                        
                        
                        

                    } else if (flowStatus == 1) {
                        std::cout << "Received Flow Control (Wait)\n";
                        // Implement wait if needed
                        sendNextBlock(blockSize);

                    } else {
                        std::cerr << "Flow Control: Overflow abort\n";
                        this->sendingMultiFrame = false;
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    if (!fcReceived) {
        std::cerr << "Timeout waiting for Flow Control frame\n";
        this->sendingMultiFrame = false;
    }
}

void CanTp::sendNextBlock(uint8_t blockSize) {
    if (blockSize == 0) blockSize = 1;  // Handle 0 block size
    
    size_t remaining = sendingMessage.size() - sendingOffset;
    for (uint8_t i = 0; i < blockSize && remaining > 0; ++i) {
        std::vector<uint8_t> data;
        uint8_t pci = 0x20 | (sendingSequence & 0x0F);
        data.push_back(pci);
        size_t bytesToSend = std::min<size_t>(MAX_CF_DATA, remaining);
        data.insert(data.end(), sendingMessage.begin() + sendingOffset,
                    sendingMessage.begin() + sendingOffset + bytesToSend);
        
        Can cf(txId_, data);
        cf.setFrameType(txId_ > 0x7FF);
        busTp_.send(cf);
        std::cout << "Sent Consecutive Frame #" << (int)sendingSequence << "\n";
        
        sendingOffset += bytesToSend;
        sendingSequence = (sendingSequence + 1) & 0x0F;
        remaining -= bytesToSend;
    }

    
    if (sendingOffset >= sendingMessage.size()) {
        this->sendingMultiFrame = false;
        situation = false;
        std::cout << "Multi-frame transmission complete\n";
    }
    else {
        this->sendingMultiFrame = true;
        situation = true;
        sendMultiFrame(false);


    }
}

std::string CanTp::receiveMessage() {
    const int MAX_RETRIES = 10000;  // Increased retries
    int retryCount = 0;

    while (retryCount < MAX_RETRIES) {
        Can frame = busTp_.receiveFrame();
        if (frame.getId() == 0) {  // Empty frame indicates timeout
            retryCount++;
            continue;
        }
        
        if (frame.getId() != rxId_) {
            retryCount++;
            continue;
        }
        
        const auto& data = frame.getData();
        if (data.empty()) {
            retryCount++;
            continue;
        }
        
        uint8_t pci = data[0];
        uint8_t frameType = (pci >> 4) & 0x0F;

        if (frameType == 0x0) { // Single Frame
            uint8_t length = pci & 0x0F;
            if (length > 0 && data.size() > 1) {
                return std::string(data.begin() + 1, data.begin() + 1 + length);
            }
        }
        else { 
            if (frameType == 0x1) {// First Frame
            // Extract length
            this->receivingTotalLength = ((pci & 0x0F) << 8) | data[1];
            std::cout<< receivingTotalLength<<std::endl;
            this->receivingBuffer.clear();
            this->receivingBuffer.insert(this->receivingBuffer.end(), data.begin() + 2, data.end());
            this->receivingOffset = data.size() - 2;
            this->receivingSequence = 1;
            this->receivingMultiFrame = true;
            
            std::cout << "Received First Frame, length: " << this->receivingTotalLength << "\n";
            }
            // Send Flow Control Continue to the SENDER'S RX ID
            this->sendFlowControl(0x00, 8, 0, txId_);  // FIXED: Use sender's rxId
            std::cout << "Sent Flow Control (Continue) to 0x" << std::hex << txId_ << std::dec << "\n";
            
            // Start receiving consecutive frames
            while (this->receivingOffset < this->receivingTotalLength && retryCount < MAX_RETRIES) {
                Can cf = busTp_.receiveFrame();
                if (cf.getId() == 0) {  // Timeout
                    retryCount++;
                    this->sendFlowControl(0x00, 8, 0, txId_);  // FIXED: Use sender's rxId

                    continue;
                }
                
                if (cf.getId() != rxId_) {
                    retryCount++;
                    continue;
                }
                
                const auto& cfData = cf.getData();
                if (cfData.empty()) {
                    this->sendFlowControl(0x00, 8, 0, txId_);  // FIXED: Use sender's rxId

                    retryCount++;
                    continue;
                }
                
                uint8_t cfPci = cfData[0];
                uint8_t cfFrameType = (cfPci >> 4) & 0x0F;
                
                if (cfFrameType != 0x2) { // Not Consecutive Frame
                    this->sendFlowControl(0x00, 8, 0, txId_);  // FIXED: Use sender's rxId

                    continue;
                }
                
                uint8_t sequence = cfPci & 0x0F;
                if (sequence != this->receivingSequence) {
                    std::cerr << "Sequence error: expected " 
                              << (int)receivingSequence << ", got " 
                              << (int)sequence << "\n";
                    retryCount++;
                    continue;
                }
                
                // Valid consecutive frame
                size_t bytesToCopy = std::min<size_t>(
                    cfData.size() - 1,
                    receivingTotalLength - receivingOffset
                );
                
                this->receivingBuffer.insert(receivingBuffer.end(), 
                                     cfData.begin() + 1, 
                                     cfData.begin() + 1 + bytesToCopy);
                this->receivingOffset += bytesToCopy;
                this->receivingSequence = (receivingSequence + 1) % 16;
                retryCount = 0;  // Reset retry count on successful frame
                
                std::cout << "Received Consecutive Frame #" << (int)sequence 
                          << ", progress: " << receivingOffset << "/" 
                          << receivingTotalLength << "\n";
            }
            
            if (receivingOffset >= receivingTotalLength) {
                receivingMultiFrame = false;
                return std::string(receivingBuffer.begin(), 
                                   receivingBuffer.begin() + receivingTotalLength);
            }
            else{
                return receiveMessage();
            }
        }
            return receiveMessage();
            retryCount++;
        
    }
    
    std::cerr << "Failed to receive message after " << MAX_RETRIES << " attempts\n";
    return "";
}

void CanTp::sendFlowControl(uint8_t flowStatus, uint8_t blockSize, 
                           uint8_t separationTime, uint32_t targetId) {
    std::vector<uint8_t> data;
    data.push_back(0x30 | (flowStatus & 0x0F));
    data.push_back(blockSize);
    data.push_back(separationTime);
    Can fcFrame(targetId, data);
    fcFrame.setFrameType(targetId > 0x7FF);
    busTp_.send(fcFrame);
}
/*std::string CanTp::receiveCon(int& retryCount){
                // Start receiving consecutive frames
    while (receivingOffset+10 < receivingTotalLength && retryCount < 1000) {
        std::cout<< receivingOffset<< " " << receivingTotalLength<< " "<<retryCount <<std::endl;
    
        Can cf = busTp_.receiveFrame();
        const auto& cfData = cf.getData();
        uint8_t cfPci = cfData[0];
        uint8_t cfFrameType = (cfPci >> 4) & 0x0F;
        uint8_t sequence = cfPci & 0x0F;
        std::cout<< cf.getId()<<std::endl;
        if (cf.getId() == 0) {  // Timeout
            //this->sendFlowControl(0x00, 8, 10, txId_);  // FIXED: Use sender's rxId
    
            retryCount++;
    
      
    
        }
                    
        if (cf.getId() != rxId_) {
            retryCount++;
    
            continue;
    
        }
                    
                   
        if (cfData.empty()) {
    
            retryCount++;
    
            continue;
    
        }
                    
    
                    
        if (cfFrameType != 0x2) { // Not Consecutive Frame
    
            retryCount++;
            //this->sendFlowControl(0x00, 8, 10, txId_);  // FIXED: Use sender's rxId
    
    
            continue;
    
        }
                    
                    
        if (sequence != this->receivingSequence) {
            std::cerr << "Sequence error: expected " 
                        << (int)receivingSequence << ", got " 
                            << (int)sequence << "\n";
                                
            retryCount++;
            continue;
        }
                    
        // Valid consecutive frame
        size_t bytesToCopy = std::min<size_t>(
            cfData.size() - 1,
            receivingTotalLength - receivingOffset
            );
                    
        this->receivingBuffer.insert(receivingBuffer.end(), 
                                         cfData.begin() + 1, 
                                         cfData.begin() + 1 + bytesToCopy);
        this->receivingOffset += bytesToCopy;
        this->receivingSequence = (receivingSequence + 1) % 16;
        retryCount = 0;  // Reset retry count on successful frame
                    
                    std::cout << "Received Consecutive Frame #" << (int)sequence 
                              << ", progress: " << receivingOffset << "/" 
                              << receivingTotalLength << "\n";
                }
                
        if (receivingOffset >= receivingTotalLength) {
                    this->receivingMultiFrame = false;
                    this->sendingMultiFrame = false;
                    situation = false;
                    return std::string(receivingBuffer.begin(), 
                                       receivingBuffer.begin() + receivingTotalLength);
            } else{
                
                // Wait for Flow Control frame with timeout
            auto start = std::chrono::steady_clock::now();
            const std::chrono::seconds timeout(2);   
            //this->sendFlowControl(0x00, 8, 10, txId_);  // FIXED: Use sender's rxId

            std::cout << "Sent Flow Control (Continue) to 0x" << std::hex << txId_ << std::dec << "\n";
            return receiveCon(retryCount);
        }
    
}*/