//
// Created by Lenovo on 12/1/2023.
//
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <regex>

// Define the structure
struct user_info {
    char user_name[50];
    char user_email[50];
};



bool isValidEmail(const std::string& email) {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    return std::regex_match(email, emailRegex);
}

bool searchTest(const std::string& email){
    const std::string key = "test";
    for(int i=0, j=0; i<email.size(); i++)
    {
        if(email[i]==key[j])
        {
            while(j<5 && email[i]==key[j])
            {
                i++,j++;
            }
            if(j>=4)
                return true;
            else
                j=0;
        }
        else
            j=0;
    }
    return false;
}
bool validateUserInfo(const user_info& user) {

    bool haveTest = searchTest(user.user_email);
    if(haveTest)
        return isValidEmail(user.user_email);
    return false;
}

int main() {
    // Initialize Winsock
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

        // Validate the received user information
        bool isValid = validateUserInfo(receivedUser);

        // Print validation result
        std::cout << "Received user from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port)
                  << " - Name: " << receivedUser.user_name << ", Email: " << receivedUser.user_email
                  << ", Validation: " << (isValid ? "Valid" : "Invalid") << "\n";
    }

    // Close the socket and clean up Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
