#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

//header files
#include "Customer.h"
#include "Manager.h"
#include "SQLAccess.h"

using namespace std;

class Login
{
public:

	Login();
	~Login();

	void LoginInformation();
	void CheckUserType(string);
	void CurrentUser();

private:

	string firstName;
	string lastName;
	string username;
	string password;
	string userType;
	vector<string> UserInfo;

};

//Constructor
Login::Login()
{
	firstName = "";
	lastName = "";
	username = "";
	password = "";
}

//Destructor
Login::~Login()
{

}


//Methods
//asks the user to enter their information, validation to make sure they enter a correct type
void Login::LoginInformation() {
	bool invalid = true;

	while (invalid) {
		system("cls");
		cout << "LOGIN TO SYSTEM" << endl;
		cout << "\nUser Login:\n" << endl;


		/*-------------ENTER YOUR ACCOUNT TYPE-------------------*/
		bool invalidType = true;
		while (invalidType) {
			cout << "\nEnter what type of user you are as a number(1.Customer or 2.Manager): ";
			int choice = 0;
			
			while (true)
			{
				cout << "\nSelection: ";
				// Read user input
				if (!(cin >> choice)) {
					// Input is not an integer
					cout << "Invalid input! Please enter a valid integer." << endl;
					cin.clear(); // Clear the fail state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
				}
				else {
					// Input is an integer
					break; // Exit the loop
				}
			}


			if (choice == 1 || choice == 2) {
				invalidType = false;
				if (choice == 1)
				{
					userType = "Customer";
				}
				else
				{
					userType = "Manager";
				}
			}
			else {
				cout << "Invalid User Type! Please enter a valid user type" << endl;
			}
		}

		/*-------------ENTER YOUR USERNAME-------------------*/
		cout << "Enter your username: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, username);

		/*-------------ENTER YOUR PASSWORD-------------------*/
		cout << "Enter your password: ";
		cin >> password;

		

		//check if user exists - this is to see if user does not exist
		//if empty vector, invalid is true, else false
		CheckUserType(userType);

		if (UserInfo.empty())
		{
			invalid = true;
		}
		else
		{
			invalid = false;
		}

		//if user does not exist ask to try again
		if (invalid == true) {
			cout << "Incorrect Login sorry!" << endl;
			cout << "Press y to try again..." << endl;
			char again;
			cin >> again;
			if (again != 'y' && again != 'Y') {
				invalid = false;
			}
		}
		//user does exist, continue to corresponding type
		else if (invalid == false) {
			cout << "Login Successful!" << endl;
			system("pause");
			CurrentUser();
		}
	}
}

//method to call a specific reading method depending on user type
void Login::CheckUserType(string type) {
	Database* data = new Database();
	UserInfo = data->Login(type, username, password);
	delete data;
}


//send control message to a user class upon successful login
//revamp this into a menu and let user class display the options
void Login::CurrentUser() {
	if (userType == "Customer") {
		Customer* customer = new Customer(UserInfo);

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		bool menu = true;
		while (menu) {
			int choice;


			system("cls");
			customer->customerMenu();

			while (true)
			{
				cout << "\nSelection: ";
				// Read user input
				if (!(cin >> choice)) {
					// Input is not an integer
					cout << "Invalid input! Please enter a valid integer." << endl;
					cin.clear(); // Clear the fail state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
				}
				else {
					// Input is an integer
					break; // Exit the loop
				}
			}

			switch (choice) {
			case 0:
				customer->showTransactions();
				system("pause");
				break;
			case 1:
				customer->DepositMoney();
				system("pause");
				break;
			case 2:
				customer->WithdrawMoney();
				system("pause");
				break;
			case 3:
				//add a friend
				customer->AddFriend();
				system("pause");
				break;
			case 4:
				//send money to a friend
				customer->SendMoney();
				system("pause");
				break;
			case 5:
				//request money
				customer->RequestMoney();
				system("pause");
				break;
			case 6:
				cout << "Goodbye!" << endl;
				menu = false;
				delete customer;
				system("pause");
				break;
			default:
				cout << "Invalid choice detected try again please" << endl;
				system("pause");
				break;
			}
		}
	}
	else if (userType == "Manager") {
		Manager* manager = new Manager(UserInfo);

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		bool menu = true;
		while (menu) {
			int choice;

			system("cls");
			manager->managerMenu();

			while (true)
			{
				cout << "\nSelection: ";
				// Read user input
				if (!(cin >> choice)) {
					// Input is not an integer
					cout << "Invalid input! Please enter a valid integer." << endl;
					cin.clear(); // Clear the fail state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
				}
				else {
					// Input is an integer
					break; // Exit the loop
				}
			}

			switch (choice) {
			case 1:
				manager->DisplayAllCustomers();
				system("pause");
				break;
			case 2:
				manager->DeleteUser();
				system("pause");
				break;
			case 3:
				manager->LookUpUser();
				system("pause");
				break;
			case 4:
				cout << "Goodbye!" << endl;
				menu = false;
				delete manager;
				system("pause");
				break;
			default:
				cout << "Invalid choice detected try again please" << endl;
				system("pause");
				break;
			}
		}
	}
}