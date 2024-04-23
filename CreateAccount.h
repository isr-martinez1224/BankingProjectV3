#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cctype>

#include "SQLAccess.h"

using namespace std;

class CreateAccount
{
public:

	CreateAccount();
	~CreateAccount();

	void CreateUserAccount();
	void WriteToLoginFile();

private:

	string firstName;
	string lastName;
	string userType;
	string email;
	string phone;
	string accountNumber;
	string username;
	string password;

};


//Constructor
CreateAccount::CreateAccount()
{
	firstName = "";
	lastName = "";
	userType = "";
	email = "";
	phone = "";
	accountNumber = "";
	username = "";
	password = "";
}

//Destructor
CreateAccount::~CreateAccount()
{

}


//methods
//ask user for information
void CreateAccount::CreateUserAccount()
{
	system("cls");

	/*-------------ENTER YOUR NAME-------------------*/

	cout << "CREATE USER LOGIN" << endl;
	cout << "\nEnter your first name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, firstName);

	cout << "\nEnter your last name: ";
	cin >> lastName;


	/*-------------SET YOUR ACCOUNT TYPE-------------------*/
	bool invalidType = true;
	while (invalidType)
	{
		cout << "\nEnter what type of user you are registering as:" << endl;
		cout << "1. Customer" << endl;
		cout << "2. Manager" << endl;
		cout << "\n(Please enter a number)" << endl;
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

		if (choice == 1 || choice == 2)
		{
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
		else
		{
			cout << "Invalid User Type! Please enter a valid user type" << endl;
		}
	}


	/*-------------ENTER YOUR EMAIL-------------------*/

	cout << "\nEnter your email address: ";
	cin >> email;

	/*-------------ENTER YOUR PHONE NUMBER-------------------*/

	invalidType = true;
	while (invalidType)
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nEnter your phone number: ";
		cin >> phone;

		if (phone.length() != 10)
		{
			cout << "\nThis is not a 10 digit phone number! Please try again..." << endl;
		}
		else if (phone.length() == 10)
		{
			invalidType = false;
		}
	}

	/*-------------CREATE BANK ACCOUNT NUMBER (if customer)-------------------*/
	if (userType == "Customer") {
		srand(time(0));
		string numString = "";
		for (int i = 0; i < 16; i++)
		{
			int num = rand() % 10;
			numString += to_string(num);
		}
		accountNumber = numString;
	}

	/*-------------CREATE A USERNAME-------------------*/

	cout << "\nCreate a username: ";
	cin >> username;

	/*-------------CREATE A PASSWORD-------------------*/
	//must be length 8
	//must have a number
	//must have a letter
	//must have a special character
	invalidType = true;
	while (invalidType)
	{
		cout << "\n(Must be 8 characters long, include a number, a letter, and a special character '!,@,#,$,%,&,*')" << endl;
		cout << "\nCreate a password: ";
		cin >> password;

		if (password.length() < 8)
		{
			cout << "\nPassword is not at least 8 characters long!" << endl;
		}
		else
		{
			string checklist = "";
			for (int i = 0; i < password.length(); i++)
			{
				if (isalpha(password.at(i)))
				{
					checklist += 'a';
				}
				else if (isdigit(password.at(i)))
				{
					checklist += 'b';
				}
				else if (password.at(i) == '!' || password.at(i) == '@' || password.at(i) == '#' || password.at(i) == '$' || password.at(i) == '%' || password.at(i) == '&' || password.at(i) == '*')
				{
					checklist += 'c';
				}
				else
				{
					checklist += '-';
				}
			}

			if (checklist.find('-') != string::npos || checklist.find('a') == string::npos || checklist.find('b') == string::npos || checklist.find('c') == string::npos)
			{
				cout << "\nYour password does not meet the required characters listed above! Try again" << endl;
			}
			else
			{
				invalidType = false;
			}
		}
	}



	//write info to database in correct table
	WriteToLoginFile();//fix sign up process , make it different for customer/manager

	cout << "\n--------------------------------" << endl;
	cout << "\nAccount successfully created!" << endl;
	cout << "User Details:" << endl;
	cout << "Name: " << firstName << " " << lastName << endl;
	cout << "User Type: " << userType << endl;
	cout << "E-mail: " << email << endl;
	cout << "Phone #: " << phone << endl;
	cout << "Username: " << username << endl;
	cout << "Password: " << password << "\n" << endl;

	if (userType == "Customer") {
		cout << "BankAccount #: " << accountNumber << endl;
		cout << "Starting Balance: $0.00" << endl;
	}

	system("pause");
}

//Write user login info to correct table
void CreateAccount::WriteToLoginFile()
{
	Database* data = new Database();
	data->AddAccount(firstName, lastName, userType, email, phone, accountNumber, username, password);
	delete data;
}