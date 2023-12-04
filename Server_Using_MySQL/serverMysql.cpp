#include <iostream>
#include <mysql.h>
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include "validEmail.cpp"
#include "dbOperation.cpp"


int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    // Create a UDP socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    // Server address information
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2222); // Replace with the desired UDP port
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 2222...\n";
    int count = 0;
    bool dbCreated = false, tblCreated = false;
    while (true) {
        user_info receivedUser;
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        // Receive data from the client
        int bytesReceived = recvfrom(sockfd, reinterpret_cast<char*>(&receivedUser), sizeof(user_info), 0,
                                     reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error receiving data: " << WSAGetLastError() << "\n";
            continue; // Continue listening for the next message
        }


        if(!dbCreated){
            dbCreated = true;
            initDB();
        }
        if(!tblCreated){
            tblCreated = true;
            createTable();
        }
        insertData(receivedUser);

        count++; // Limiting the infinite loop...
        if(count>=10)
            break;
    }
    viewTableData();

    // Close the socket and clean up Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
