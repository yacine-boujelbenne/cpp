#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(x) Sleep(x)
#else
#include <unistd.h>
#define SLEEP_MS(x) usleep((x) * 1000)
#endif


#include "Can.hpp"
#include "CanTp.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"


void demonstrateBasicCanCommunication()
{
    std::cout << "\n=== Démonstration Communication CAN de base ===\n"
              << std::endl;

    // Création de trames CAN de test
    std::vector<uint8_t> data1 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::vector<uint8_t> data2 = {0xAA, 0xBB, 0xCC, 0xDD};
    std::vector<uint8_t> data3 = {0x11, 0x22, 0x33};

    Can frame1(0x123, data1);
    Can frame2(0x456, data2);
    Can frame3(0x789, data3);

    // Affichage des trames avant envoi
    std::cout << "Trames à envoyer:" << std::endl;
    frame1.print();

    frame2.print();
    frame3.print();

    SLEEP_MS(500); // 500ms
}

void demonstrateCanTpCommunication()
{
    std::cout << "\n=== Démonstration Communication CAN-TP ===\n"
              << std::endl;

    // Création d'une instance CAN-TP
    CanTp canTp(0x700, 0x700); // TX ID: 0x700, RX ID: 0x700e



        // Test avec un message long (Multi Frame)
        std::string longMessage = "Ceci est un message très long qui nécessite plusieurs trames CAN pour être transmis complètement via CAN-TP";
        std::cout << "Envoi message long: \"" << longMessage << "\"" << std::endl;
        canTp.sendMessageP(longMessage);

    

    
}

void demonstrateEcuCommunication()
{
    std::cout << "\n=== Démonstration Communication ECU ===\n"
              << std::endl;


    CanTp canTp(0x700, 0x700); // TX ID: 0x700, RX ID: 0x708

    // Création des ECUs
    Sender senderEcu("Engine_ECU", true);

    // Configuration des valeurs

    // Simulation de l\"envoi de données
    std::cout << "=== ECU Sender ===" << std::endl;
    std::string msg = "hey. im a transmitter i send data.";
    senderEcu.sendEcuData(msg, canTp);
}
void runInteractiveDemo()
{
    std::cout << "\n=== Mode Interactif (Simulation Windows) ===\n"
              << std::endl;

    while (true)
    {
        std::cout << "\nChoisissez une action:" << std::endl;
        std::cout << "1. Créer et afficher une trame CAN personnalisée" << std::endl;
        std::cout << "2. Envoyer un message CAN-TP" << std::endl;
        std::cout << "3. Simuler réception de trames" << std::endl;
        std::cout << "4. Retour au menu principal" << std::endl;
        std::cout << "Choix: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            uint32_t id;
            std::cout << "Entrez l'ID CAN (décimal, ex: 291 pour 0x123): ";
            std::cin >> id;

            std::cout << "Entrez le nombre de bytes (1-8): ";
            int numBytes;
            std::cin >> numBytes;
            if (numBytes < 1)
                numBytes = 1;
            if (numBytes > 8)
                numBytes = 8;

            std::vector<uint8_t> data;
            std::cout << "Entrez " << numBytes << " valeurs (0-255):" << std::endl;
            for (int i = 0; i < numBytes; i++)
            {
                int val;
                std::cout << "Byte " << i << ": ";
                std::cin >> val;
                if (val < 0)
                    val = 0;
                if (val > 255)
                    val = 255;
                data.push_back(static_cast<uint8_t>(val));
            }

            Can customFrame(id, data);
            std::cout << "Trame créée:" << std::endl;
            customFrame.print();
            break;
        }
        case 2:
        {
            std::cout << "Entrez le message à envoyer: ";
            std::string message;
            std::cin.ignore();
            std::getline(std::cin, message);

            CanTp canTp(0x700, 0x700);
            canTp.sendMessageP(message);
            break;
        }
        case 3:
        {
            std::cout << "Simulation de réception de trames..." << std::endl;

            // Simuler quelques trames reçues
            std::vector<std::vector<uint8_t>> simulatedData = {
                {0x12, 0x34, 0x56, 0x78},
                {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                {0x01, 0x02, 0x03}};

            std::vector<uint32_t> simulatedIds = {0x200, 0x300, 0x400};

            for (size_t i = 0; i < simulatedData.size(); i++)
            {
                Can simulatedFrame(simulatedIds[i], simulatedData[i]);
                std::cout << "Trame simulée reçue: ";
                simulatedFrame.print();
                SLEEP_MS(500);
            }
            break;
        }
        case 4:
            return;
        default:
            std::cout << "Choix invalide!" << std::endl;
            break;
        }
    }
}

void receivingmode(){
    CanTp canTp(0x700, 0x700); // TX ID: 0x700, RX ID: 0x708e

    std::cout << "receiving frames..." << std::endl;
    //canTp.receiveMessageP(canTp.getbus().getcanMan());
    //std::cout<<Ecu::decoder(canTp.getbus().getcanMan()->getData());
    std::cout<<canTp.receiveMessage()<<std::endl;
}

void receiveECU(){
    Receiver rec("rec", true);
    CanTp canTp(0x700, 0x700); // TX ID: 0x700, RX ID: 0x708e
    rec.receiveEcuData(canTp);
   

}

void testCanFrameCreation()
{
    std::cout << "\n=== Test de Création de Trames CAN ===\n"
              << std::endl;

    // Test de différentes tailles de données
    std::cout << "Test avec différentes tailles de données:" << std::endl;

    // Trame avec 1 byte et avec l option 
    std::vector<uint8_t> data1 = {0xFF};
    Can frame(0x001, data1);
    frame.print();
    std::cout<<(int)frame.getId()<<std::endl;

    // Trame avec 4 bytes
    std::vector<uint8_t> data2 = {0xDE, 0xAD, 0xBE, 0xEF};
    Can frame2(0x123, data2);
    frame2.print();

    // Trame avec 8 bytes (maximum)
    std::vector<uint8_t> data3 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    Can frame3(0x7FF, data3);
    frame3.print();

    // Test de l'encodeur de chaîne
    std::cout << "\nTest de l'encodeur de chaîne:" << std::endl;
    std::string testString = "TEST";
    std::vector<uint8_t> encodedData = Can::encoder(testString);
    Can stringFrame(0x555, encodedData);
    stringFrame.print();
}

int main()
{
    std::cout << "=== Démonstration CAN Bus (Version Windows) ===\n"
              << std::endl;
    std::cout << "Note: Cette version fonctionne en mode simulation sur Windows." << std::endl;
    std::cout << "Pour utiliser vcan0 réellement, utilisez Linux.\n"
              << std::endl;

    // Menu principal
    while (true)
    {
        std::cout << "\n=== Menu Principal ===\n"
                  << std::endl;
        std::cout << "1. Démonstration Communication CAN de base" << std::endl;
        std::cout << "2. Démonstration CAN Transport Protocol (CAN-TP)" << std::endl;
        std::cout << "3. Démonstration Transmission par ECU" << std::endl;
        std::cout << "4. Mode Interactif" << std::endl;
        std::cout << "5. Test de création de trames CAN" << std::endl;
        std::cout << "6. Test de performance (simulation)" << std::endl;
        std::cout << "7. Reception avec canTp" << std::endl;
        std::cout << "8. Reception avec Ecu" << std::endl;
        std::cout << "9. Quitter" << std::endl;
        std::cout << "Choix: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            demonstrateBasicCanCommunication();
            break;
        case 2:
            demonstrateCanTpCommunication();
            break;
        case 3:
            demonstrateEcuCommunication();
            break;
        case 4:
            runInteractiveDemo();
            break;
        case 5:
            testCanFrameCreation();
            break;
        case 6:
        {
            std::cout << "\n=== Test de Performance (Simulation) ===\n"
                      << std::endl;
            std::cout << "Création et affichage de 10 trames..." << std::endl;

            for (int i = 0; i < 10; i++)
            {
                std::vector<uint8_t> data = {
                    static_cast<uint8_t>(i & 0xFF),
                    static_cast<uint8_t>((i >> 8) & 0xFF),
                    0x00, 0x01, 0x02, 0x03};
                Can frame(0x100 + i, data);
                std::cout << "Trame " << (i + 1) << "/10: ";
                frame.print();

                if (i < 9)
                    SLEEP_MS(200); // Pause entre les trames
            }

            std::cout << "10 trames créées avec succès!" << std::endl;
            break;
        }
        case 7:
            receivingmode();
            break;
        case 8:
            receiveECU();
        case 9:
            std::cout << "Fermeture du programme..." << std::endl;
            return 0;
        default:
            std::cout << "Choix invalide!" << std::endl;
            break;
        }

        std::cout << "\nAppuyez sur Entrée pour continuer...";
        std::cin.ignore();
        std::cin.get();
    }

    return 0;
}