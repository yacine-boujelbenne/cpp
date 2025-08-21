#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include "./headers/CanTp.hpp"
#include "./headers/Receiver.hpp"


#include "./headers/Sender.hpp"

void idSettings(CanTp &tp, int &idT, int &idR){
    std::cout<<"   Please ener the Tx and Rx configuration (the both values need to be between 0x7FF and 0x001, between 2047 and 1 in decimal)"<<std::endl;
    std::cout<<"        Tx: ";
    std::cin>>idT;
    std::cout<<"        Rx: ";
    std::cin>>idR;
    tp.setRx(idR);
    tp.setTx(idT);

}
void busSettings(CanTp &tp){
    while (true) {
    
    

        std::cout<<"    ===================================================="<<std::endl;
        std::cout<<"    =================  Bus settings   ================"<<std::endl;
        std::cout<<"    ==================================================="<<std::endl;
        std::cout<<"    ====        (1) > Seperation Time               ==="<<std::endl;
        std::cout<<"    ====        (2) > Block Size                    ==="<<std::endl;
        std::cout<<"    ====        (0) > back                         ==="<<std::endl;
        std::cout<<"    ==================================================="<<std::endl;
        std::cout<<"    ==================================================="<<std::endl;
    
        int rep;
        std::cin>>rep;
        switch(rep){
            case 1:
            {
                std::cout<<"enter the new Seperation time !"<<std::endl;
                int s;
                std::cin>>s;
                tp.setst(static_cast<uint8_t>(s));
                if (s > 0 && static_cast<uint8_t>(s) >= tp.staticST){
                    tp.setbs(static_cast<uint8_t>(s));
                }
                else{
                    std::cout<<"Invalid Seperation Time ! please try again ."<<std::endl;
                }
               
                break;
    
            }
            case 2:
            {
                std::cout<<"enter the new Block Size !"<<std::endl;
                int b;
                std::cin>>b;
                if (b > 0 && static_cast<uint8_t>(b) >= tp.staticBS){
                    tp.setbs(static_cast<uint8_t>(b));
                }
                else{
                    std::cout<<"Invalid Block Size ! please try again ."<<std::endl;
                }
                break;
            }
            case 0:
            {
                return;
            }
    
        }
    }

}

void setting(CanTp &tp, int &idT, int &idR){
    while (true) {
    
    

    std::cout<<"    ===================================================="<<std::endl;
    std::cout<<"    ==================    Settings   ==================="<<std::endl;
    std::cout<<"    ===================================================="<<std::endl;
    std::cout<<"    ====        (1) > Id setup                      ==="<<std::endl;
    std::cout<<"    ====        (2) > Bus settings                  ==="<<std::endl;
    std::cout<<"    ====        (0) > back                          ==="<<std::endl;
    std::cout<<"    ===================================================="<<std::endl;
    std::cout<<"    ===================================================="<<std::endl;

    int rep;
    std::cin>>rep;
    switch(rep){
        case 1:
        {
            idSettings(tp, idT, idR);
            break;

        }
        case 2:
        {
            busSettings(tp);
            break;
        }
        case 0:
        {
            return;
        }

    }
}
}




int main() {
    try {
        int idR, idT;
        std::cout<<"                              ========================================"<<std::endl;
        std::cout<<"                      =========================================================="<<std::endl;
        std::cout<<"                      ========== Welcome to Virtual CAN Simulator app =========="<<std::endl;
        std::cout<<"                      =========================================================="<<std::endl;
        std::cout<<"   Please ener the Tx and Rx configuration (the both values need to be between 0x7FF and 0x001, between 2047 and 1 in decimal)"<<std::endl;
        std::cout<<"   Tx: ";
        std::cin>>idT;
        std::cout<<"   Rx: ";
        std::cin>>idR;



        CanTp tp(static_cast<uint16_t>(idT),static_cast<uint16_t>(idR));
        std::cout<<"   =========================================================="<<std::endl;
        std::cout<<"                     Creating a new Ecu...."<<std::endl;
        std::cout<<"   =========================================================="<<std::endl;

        std::cout<<"        Please enter the Ecu name: ";
        std::string name;
        std::cin>>name;

        bool availability= true;

        // Send the message
        int rep=-1;
        while (true) {
        
        
    
            std::cout<<"    ===================================================="<<std::endl;
            std::cout<<"    ========== The envirement is ready to go! =========="<<std::endl;
            std::cout<<"    ===================================================="<<std::endl;
            std::cout<<"    ====        (1) > Send a message.                ==="<<std::endl;
            std::cout<<"    ====        (2) > receive a message.             ==="<<std::endl;
            std::cout<<"    ====        (3) > Settings.                      ==="<<std::endl;
            std::cout<<"    ====        (0) > Quit.                          ==="<<std::endl;
            std::cout<<"    ===================================================="<<std::endl;
            std::cout<<"    ===================================================="<<std::endl;



            std::cin>>rep;
            switch (rep)
                {
                case 1:
                    {
                        Sender s(name, availability);
                        std::cout<<"              Enter the message to send :  ";



                        std::string message;
                        std::cin.ignore();
                        std::getline(std::cin, message);
                        std::cout<<"   =========================================================="<<std::endl;
                        std::cout<<"   =========================================================="<<std::endl;
                        s.sendEcuData(message, tp);
                        break;

                    }
                case 2:
                    {
                        Receiver r(name, availability);

                        r.receiveEcuData(tp);
                        std::cout<<r.getmessage()<<std::endl;
                        break;

                    }
                case 3:
                    {
                        setting(tp, idT, idR);
                        break;
                    }
                case 0:
                    {
                        std::cout<<"   =========================================================="<<std::endl;
                        std::cout << "              Closing the app..." << std::endl;
                        std::cout<<"   =========================================================="<<std::endl;
                        return 0;


                    }
                default:
                    std::cout<<"   =========================================================="<<std::endl;
                    std::cout << "                Invalid choice!" << std::endl;
                    std::cout<<"   =========================================================="<<std::endl;
                    break;

            
                }
        }
        
        std::cout << "Test complete\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}