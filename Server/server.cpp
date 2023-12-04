//
// Created by Lenovo on 12/1/2023.
//
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <regex>
#include "sqlite3.h"


// Define the structure
typedef struct user_info {
    int user_id;
    char first_name[100];
    char last_name[100];
    char email[100];
    char subscription_time[50];
    char last_entry_time[50];
    unsigned long view_time;
} user_info;



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
bool validateUserEmail(std::string email) {

    bool haveTest = searchTest(email);
    if(haveTest)
        return isValidEmail(email);
    return false;
}

//SQL part
sqlite3* db;
char* errMsg = 0;

static int createDB(){
    int rc = sqlite3_open("Users.db", &db);

    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }
}

static int createTable(){
    const char* query = "CREATE TABLE IF NOT EXISTS UserInfo (\n"
                        "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                        "    user_id INTEGER,\n"
                        "    first_name VARCHAR(100),\n"
                        "    last_name VARCHAR(100),\n"
                        "    email VARCHAR(100),\n"
                        "    subscription_time DATETIME,\n"
                        "    last_entry_time DATETIME,\n"
                        "    view_time INTEGER\n"
                        ");";
    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }

//    std::cout << "Table created successfully." << std::endl;

}

static int insertData(user_info user){


    if(validateUserEmail(user.email))
    {// INSERT command
        std::string insertSQL = "INSERT INTO UserInfo (user_id, first_name, last_name, email, subscription_time, last_entry_time, view_time) VALUES (" +
                                std::to_string(user.user_id) + ", '" +
                                user.first_name + "', '" +
                                user.last_name + "', '" +
                                user.email + "', '" +
                                user.subscription_time + "', '" +
                                user.last_entry_time + "', " +
                                std::to_string(user.view_time) + ");";
        int rc = sqlite3_exec(db, insertSQL.c_str(), 0, 0, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return rc;
        }

        std::cout << "Data inserted successfully." << std::endl;
    }
    else
        std::cout << "Email: "<<user.email<<" is not Valid" << std::endl;

}

void showTitle(int count,  char **columns){
    printf("There are %d column(s)\n", count);
    for(int i=0; i<count; i++)
        std::cout<<columns[i]<<"\t";
    std::cout<<"\n";
}
bool columnTitle = false;
static int displayData(void *unused, int count, char **data, char **columns){

    if(!columnTitle){
        showTitle(count, columns);
        columnTitle = true;
    }
    for(int i=0; i<count; i++)
        std::cout<<data[i]<<"\t";
    printf("\n");

    return 0;
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
    int count = 0;
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

        bool dbCreated = false, tblCreated = false;
        if(!dbCreated){
            dbCreated = true;
            createDB();
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
    const char* query = "SELECT * FROM userinfo;";
    int rc = sqlite3_exec(db, query, displayData, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }

    // Close the socket and clean up Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
