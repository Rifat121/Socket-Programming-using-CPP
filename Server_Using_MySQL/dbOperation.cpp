//
// Created by Lenovo on 12/5/2023.
//
#include <iostream>
#include <mysql.h>
MYSQL *conn;


typedef struct user_info {
    int user_id;
    char first_name[100];
    char last_name[100];
    char email[100];
    char subscription_time[50];
    char last_entry_time[50];
    unsigned long view_time;
} user_info;

int initDB(){
    conn = mysql_init(nullptr);

    // Check if initialization was successful
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed." << std::endl;
        return 1;
    }

    // Connect to the MySQL database
    if (mysql_real_connect(conn, "localhost", "root", "", "test", 3306, NULL, 0) == NULL) {
        std::cerr << "Failed to connect to database: Error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 2;
    }

    // Connection successful
    std::cout << "Connected to MySQL database!" << std::endl;
    return 0;

}

void createTable(){
    std::string query = "CREATE TABLE IF NOT EXISTS UserInfo ("
                        "id int(11) NOT NULL AUTO_INCREMENT, "
                        "user_id int(10) DEFAULT NULL, "
                        "first_name varchar(100) DEFAULT NULL, "
                        "last_name varchar(100) DEFAULT NULL, "
                        "email varchar(100) DEFAULT NULL, "
                        "subscription_time datetime DEFAULT NULL, "
                        "last_entry_time datetime DEFAULT NULL, "
                        "view_time int(20) DEFAULT NULL, "
                        "PRIMARY KEY (id)"
                        ") ENGINE=InnoDB AUTO_INCREMENT=64 DEFAULT CHARSET=latin1";
    if(mysql_query(conn, query.c_str())!=0)
    {
        std::cerr << "Failed to create table: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
    }
}

static int insertData(user_info user){
    if(validateUserEmail(user.email))
    {// INSERT command
        std::string insertData = "INSERT INTO UserInfo (user_id, first_name, last_name, email, subscription_time, last_entry_time, view_time) VALUES (" +
                                 std::to_string(user.user_id) + ", '" +
                                 user.first_name + "', '" +
                                 user.last_name + "', '" +
                                 user.email + "', '" +
                                 user.subscription_time + "', '" +
                                 user.last_entry_time + "', " +
                                 std::to_string(user.view_time) + ");";
//        std::cout<<insertData<<"\n";
        if(mysql_query(conn, insertData.c_str()) != 0)
        {
            std::cerr << "Failed to Insert Data: " << mysql_error(conn) << std::endl;
            mysql_close(conn);
        }

        std::cout << "Data inserted successfully." << std::endl;
    }
    else
        std::cout << "Email: "<<user.email<<" is not Valid" << std::endl;
    return 0;

}

void viewTableData() {
    // Construct the SELECT command
    std::string selectCommand = "SELECT * FROM UserInfo";

    // Execute the SELECT command
    if (mysql_query(conn, selectCommand.c_str()) != 0) {
        std::cerr << "Failed to execute SELECT command: " << mysql_error(conn) << std::endl;
        return;
    }

    // Get the result set
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == nullptr) {
        std::cerr << "Failed to get result set: " << mysql_error(conn) << std::endl;
        return;
    }

    MYSQL_FIELD *fields = mysql_fetch_fields(res);
    for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
        std::cout << fields[i].name << "\t";
    }
    std::cout << std::endl;

    // Fetch and display the rows
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
            std::cout << row[i] << "\t";
        }
        std::cout << std::endl;
    }

    // Free the result set
    mysql_free_result(res);
}