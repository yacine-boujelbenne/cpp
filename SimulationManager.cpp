#include <iostream>
#include <ostream>
#include <thread>
#include "CanTp.hpp"
#include "CanBus.hpp"

void receiverThread(CanTp* receiver) {
    std::cout << "Receiver started\n";
    while (true) {
        std::string message = receiver->receiveMessage();
        if (!message.empty()) {
            std::cout << "\nReceived message (" << message.size() << " bytes):\n" 
                      << message << "\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    try {
        CanTp sender(0x700, 0x701);


        // Corrected ID configuration:
        // Sender: sends on 0x700, expects responses on 0x701
        
        
        // Receiver: listens on 0x701, sends responses on 0x700
        CanTp receiver(0x701, 0x700, sender.getbus());

        // Test with a long message
        std::string longMessage = "This is a long message that requires multiple frames to be transmitted over CAN-TP. "
                                 "It should be longer than 7 bytes to trigger multi-frame transmission. "
                                 "CAN-TP is specified in ISO 15765-2 and allows for transmission of up to 4095 bytes "
                                 "of data over a CAN network using multiple frames.";
        
        std::cout << "Sending message (" << longMessage.size() << " bytes)\n";
        
        // Start receiver in separate thread
        //std::thread receiverWorker(receiverThread, &receiver);
        
        // Give receiver time to start
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Send the message
        int rep=0;
        std::cout<<"if you want to start sending press 1 and if you want to receive press 2"<<std::endl;
        std::cin>>rep;
        if (rep==1){
            sender.sendMessageP(longMessage);
        }
        else if (rep == 2){
            std::cout<<receiver.receiveMessage()<<std::endl;
            std::thread receiverWorker(receiverThread, &receiver);
            receiverWorker.join();
        }
        
        
        // Wait for receiver to finish
        //receiverWorker.join();
        
        std::cout << "Test complete\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}