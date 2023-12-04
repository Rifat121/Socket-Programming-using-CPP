////
//// Created by Lenovo on 12/1/2023.
////
#include <iostream>
#include <cstring>
#include <winsock2.h>


typedef struct user_info {
    int user_id;
    char first_name[100];
    char last_name[100];
    char email[100];
    char subscription_time[50];
    char last_entry_time[50];
    unsigned long view_time;
} user_info;

user_info takeFromUser(){
    user_info user;
    int userId = 1;
    std::cout<<"Enter First Name: ";
    std::cin>>user.first_name;
    std::cout<<"Enter Last Name: ";
    std::cin>>user.last_name;
    std::cout<<"Enter Your Email (including 'test'): ";
    std::cin>>user.email;
    std::cout<<"Enter Subscription Time: ";
    std::cin>>user.subscription_time;
    std::cout<<"Enter Last Entry Time: ";
    std::cin>>user.last_entry_time;
    std::cout<<"Enter View Time: ";
    std::cin>>user.view_time;
    return user;
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }
//    else
//        std::cout << "Initialization Done" << "\n";

    // Create a UDP socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }
//    else
//        std::cout << "Created Socket" << "\n";

    // Server address information
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2222); // Replace with the desired UDP port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Create multiple instances of user_info structure
    user_info users[100] = {
            {1,"John", "Doe", "john@test.com","2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {2, "John", "Test","john@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {3,"Alice", "Test", "alice@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {4,"Bob", " ", "bob@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {5, " ", "Doe","john@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {6,"John", "Smith", "john@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {7,"Alice", "Bob", "alice@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {8,"Mr", "Karim", "karim@test.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {9,"Abdur", "Rahim", "rahim.test@tst.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
            {10,"Shakib", "Hasan", "shakib.com", "2023-25-12 23:50:55",
                    "2023-25-13 11:22:33",
                    111112222},
    };

    //if we want to take input
//    user_info user = takeFromUser();
//    int bytesSent = sendto(sockfd, reinterpret_cast<const char*>(&user), sizeof(user_info), 0,
//                           reinterpret_cast<sockaddr*>(&serverAddr) , sizeof(serverAddr));
//
//    if (bytesSent == SOCKET_ERROR) {
//        std::cerr << "Error sending data: " << WSAGetLastError() << "\n";
//        closesocket(sockfd);
//        WSACleanup();
//        return 1;
//    }
//    else
//        std::cout << "Sent Successfully. \n";

    // Send each user_info structure to the UDP port
    for (const auto& user : users) {
        int bytesSent = sendto(sockfd, reinterpret_cast<const char*>(&user), sizeof(user_info), 0,
                               reinterpret_cast<sockaddr*>(&serverAddr) , sizeof(serverAddr));

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Error sending data: " << WSAGetLastError() << "\n";
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        else
            std::cout << "Sent Successfully. \n";// << user.first_name << " - " << user.email << "\n";
    }

    // Close the socket and clean up Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
