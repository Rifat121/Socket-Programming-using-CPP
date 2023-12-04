# Socket-Programming-using-CPP
I was given a task of socket programming. I have a structure as follows:
typedef struct user_info {
		int user_id;
		char first_name[100];
		char last_name[100];
		char email[100];
		char subscription_time[50];
		char last_entry_time[50];
		unsigned long view_time;
	} user_info;

 Now, First Task is to send multiple instances of user as user_info to a server at PORT= 2222;
 Then, A SERVER at that port will listen and receive the data given by the Client.
 After this, SERVER will save that info in a Database only if Given User Email is Valid and contains "test" as substring.
