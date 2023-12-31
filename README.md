# Socket-Programming-using-CPP
I was given the task of socket programming. I have a structure as follows:
```
typedef struct user_info {
		int user_id;
		char first_name[100];
		char last_name[100];
		char email[100];
		char subscription_time[50];
		char last_entry_time[50];
		unsigned long view_time;
	} user_info;
```
Database Table of MySQL format as follows:
```
CREATE TABLE `UserInfo` (
	  `id` int(11) NOT NULL AUTO_INCREMENT,
	  `user_id` int(10) DEFAULT NULL,
	  `first_name` varchar(100) DEFAULT NULL,
	  `last_name` varchar(100) DEFAULT NULL,
	  `email` varchar(100) DEFAULT NULL,
	  `subscription_time` datetime DEFAULT NULL,
	  `last_entry_time` datetime DEFAULT NULL,
	  `view_time` int(20) DEFAULT NULL,
	  PRIMARY KEY (`id`)
	) ENGINE=InnoDB AUTO_INCREMENT=64 DEFAULT CHARSET=latin1;

```
 Now, the First Task is to send multiple instances of the user as user_info to a server at PORT= 2222;
 Then, A SERVER at that port will listen and receive the data given by the Client.
 After this, SERVER will save that info in a Database only if the Given User Email is Valid and contains "test" as a substring.

# Instructions to Run:
I have used [CLION IDE](https://www.jetbrains.com/clion/download/#section=windows) and Windows 11 as OS for this project. Some files and directories need to be adjusted to run the project. [MySQL](https://dev.mysql.com/downloads/installer/) and [MySQL server](https://dev.mysql.com/downloads/mysql/) need to be installed. I have used [XAMPP](https://www.apachefriends.org/download.html) as a Local Server.

1. **Server_Using_MySQL**
   	 - Adding **#pragma comment(lib, "ws2_32.lib")** in serverMysql.cpp in case of using Visual C++. or Manually Linking **"ws2_32.lib"**  file for other IDEs.
  	 - Updating the directories of the following in **CMakeLists.txt** file...
	  	include_directories("path/to/MySQL Server/include/")
		include_directories("path/to/MySQL Server/include/mysql")
		link_directories("path/to/MySQL Server/lib")
	- Updating Database Connection Parameters in **dbOperation.cpp** as follows
		mysql_real_connect(conn, "Host_Name", "User", "Password", "Database_Name"...)

2. **Client**
   	- Adding **#pragma comment(lib, "ws2_32.lib")** in Client.cpp in case of using Visual C++. or Manually Linking **"ws2_32.lib"**  file for other IDEs.
