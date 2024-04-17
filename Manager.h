#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>

//header files
#include "SQLAccess.h"

using namespace std;

class Manager
{
public:

	Manager(vector<string>);
	~Manager();

	void managerMenu();
	void DisplayAllCustomers();
	void DeleteUser();
	void LookUpUser();

private:

	string firstName;
	string lastName;
	string userType;
	string email;
	string phone;
	string username;
	string password;
};

//Constructor
Manager::Manager(vector<string> info)
{
	firstName = info.at(0);
	lastName = info.at(1);
	email = info.at(2);
	phone = info.at(3);
	username = info.at(4);
	password = info.at(5);

}

//Destructor
Manager::~Manager()
{

}

//Dashboard for manager
void Manager::managerMenu()
{
	cout << "Welcome " << firstName << " " << lastName << "!" << endl;
	cout << "----------------------" << endl;
	cout << "\nWhat would you like to do? Please enter a number" << endl;

	cout << "1. Display all customers" << endl;
	cout << "2. Delete Customer" << endl;
	cout << "3. Search for a Customer" << endl;
	cout << "4. Sign Off" << endl;
}


//Display all the customers in the database
void Manager::DisplayAllCustomers()
{
	system("cls");
	Database* manager = new Database();
	manager->DisplayCustomers();
	delete manager;
}


//Delete a user from the database
void Manager::DeleteUser()
{
	
	Database* manager = new Database();

	string choice = "";
	bool inLoop = true;
	while (inLoop)
	{
		system("cls");
		manager->DisplayCustomers();
		cout << "Which customer do you want to delete? Enter their ID, First Name and Last Name" << endl;

		int delID = 0;
		string delFirst = "";
		string delLast = "";

		cout << "ID: ";
		cin >> delID;

		cout << "First Name: ";
		cin >> delFirst;

		cout << "Last Name: ";
		cin >> delLast;

		//if deleteuser returns true, break out of loop by making inLoop not true
		inLoop = !manager->DeleteUser(delID, delFirst, delLast);

		if (inLoop == true)
		{
			cout << "Account not found! Be sure to enter the credentials correctly!" << endl;
			cout << "Would you like to try again? Press (Y/y) to try again: ";
			cin >> choice;
			if (choice != "Y" && choice != "y")
			{
				inLoop = false;
			}
		}
	}
}


//Display all details of the user you want to lookup
void Manager::LookUpUser()
{

	Database* manager = new Database();

	vector<string> customerInfo;
	string choice = "";
	bool inLoop = true;
	while (inLoop)
	{
		system("cls");
		manager->DisplayCustomers();
		cout << "Which customer do you want to search for? Enter their ID, First Name and Last Name" << endl;

		int searchID = 0;
		string searchFirst = "";
		string searchLast = "";

		cout << "ID: ";
		cin >> searchID;

		cout << "First Name: ";
		cin >> searchFirst;

		cout << "Last Name: ";
		cin >> searchLast;

		//get customer information into a vector
		customerInfo = manager->SearchUser(searchID, searchFirst, searchLast);

		if (customerInfo.empty())
		{
			inLoop = true;
		}
		else
		{
			inLoop = false;
		}

		//verify that the account wasn't found
		if (inLoop == true)
		{
			cout << "Account not found! Be sure to enter the credentials correctly!" << endl;
			cout << "Would you like to try again? Press (Y/y) to try again: ";
			cin >> choice;
			if (choice != "Y" && choice != "y")
			{
				inLoop = false;
			}
		}
		else
		{
			cout << "\nCustomer Details:" << endl;

			cout << "Name: " << customerInfo.at(0) << " " << customerInfo.at(1) << endl;
			cout << "E-mail: " << customerInfo.at(2) << endl;
			cout << "Phone #: " << customerInfo.at(3) << endl;
			cout << "Username: " << customerInfo.at(4) << endl;
			cout << "Bank Account #: " << customerInfo.at(5) << endl;
			double customerBalance = stod(customerInfo.at(6));
			cout << fixed << setprecision(2);
			cout << "Account Balance: $" << customerBalance << endl;
		}
	}
}