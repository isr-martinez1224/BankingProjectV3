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

class Customer
{
public:

	Customer(vector<string>);
	~Customer();

	void customerMenu();
	void showTransactions();
	void DepositMoney();
	void WithdrawMoney();
	void AddFriend();
	void SendMoney();
	void RequestMoney();

private:

	string firstName;
	string lastName;
	string userType;
	string email;
	string phone;
	string username;
	string password;
	string accountNumber;
	double money;

	vector<string> UserInfo;
};

//Constructor
Customer::Customer(vector<string> info)
{
	firstName = info.at(0);
	lastName = info.at(1);
	email = info.at(2);
	phone = info.at(3);
	username = info.at(4);
	password = info.at(5);
	accountNumber = info.at(6);
	money = stod(info.at(7));
}

//Destructor
Customer::~Customer()
{

}

//Dashboard for customer
void Customer::customerMenu()
{
	cout << "Welcome " << firstName << " " << lastName << "!" << endl;
	cout << "----------------------" << endl;
	cout << "Current Balance: $" << fixed << setprecision(2) << money << endl;
	cout << "----------------------\n" << endl;
	cout << "\nWhat would you like to do? Please enter a number" << endl;

	cout << "0. View Recent Transactions" << endl;
	cout << "1. Deposit Money" << endl;
	cout << "2. Withdraw Money" << endl;
	cout << "3. Add a Friend" << endl;
	cout << "4. Send Money" << endl;
	cout << "5. Request Money" << endl;
	cout << "6. Sign Off" << endl;
}



void Customer::showTransactions()
{
	Database* customer = new Database();
	customer->viewTransactions(firstName, lastName);
	delete customer;
}


//Add money to your account
void Customer::DepositMoney()
{
	cout << "\nGetting balance information...\n" << endl;
	cout << "Current Balance: $" << money << endl; //insert the number of the user's balance here

	bool nonValidNum = true;
	string deposit;
	while (nonValidNum)
	{
		//loop until user enters valid amount
		cout << "\nEnter the amount of money you want to deposit: $"; //ask for input
		cin >> deposit;

		size_t decimalPoint = deposit.find('.');
		if (decimalPoint != std::string::npos)
		{
			size_t decimalLength = deposit.substr(decimalPoint + 1).length();
			if (decimalLength == 2)
			{

				money += stod(deposit); 
				cout << fixed << setprecision(2);

				Database* customer = new Database();
				customer->EditBalance(firstName, lastName, accountNumber, money, "Deposit", stod(deposit));
				delete customer;
				cout << "Updated Balance after deposit: $" << money << endl;
				nonValidNum = false;
			}
			else
			{
				cout << "Invalid input, deposit must have only 2 decimal places\n" << endl;
			}
		}
		else
		{
			cout << "Invalid input, there must be a decimal point\n" << endl;
		}
	}

	//close file when done with deposit

}



//Subtract money to your account
void Customer::WithdrawMoney()
{
	cout << "\nGetting balance information...\n" << endl;
	cout << "Current Balance: $" << money << endl; //insert the number of the user's balance here

	bool nonValidNum = true;
	string withdraw;
	while (nonValidNum)
	{
		//loop until user enters valid amount
		cout << "\nEnter the amount of money you want to withdraw: $"; //ask for input
		cin >> withdraw;

		size_t decimalPoint = withdraw.find('.');
		if (decimalPoint != std::string::npos)
		{
			size_t decimalLength = withdraw.substr(decimalPoint + 1).length();
			if (decimalLength == 2)
			{
				if (money < stod(withdraw))
				{
					cout << "Your entered withdraw is more than your balance! Try again" << endl;
				}
				else
				{
					money -= stod(withdraw);
					cout << fixed << setprecision(2);

					Database* customer = new Database();
					customer->EditBalance(firstName, lastName, accountNumber, money, "Withdrawal", stod(withdraw));
					delete customer;
					cout << "Updated Balance after withdraw: $" << money << endl;
					nonValidNum = false;
				}
			}
			else
			{
				cout << "Invalid input, deposit must have only 2 decimal places\n" << endl;
			}
		}
		else
		{
			cout << "Invalid input, there must be a decimal point\n" << endl;
		}
	}

	//need validation that you have enough money in the bank

}


//Add a friend to your contact list
void Customer::AddFriend()
{
	vector<string> friendInfo;

	cout << "\nWho would you like to add? Please enter their name and contact information" << endl;
	
	cout << "\nFirst Name: ";
	string friendFirstName;
	cin >> friendFirstName;
	friendInfo.push_back(friendFirstName);

	cout << "\nLast Name: ";
	string friendLastName;
	cin >> friendLastName;
	friendInfo.push_back(friendLastName);

	cout << "\nE-mail: ";
	string friendEmail;
	cin >> friendEmail;
	friendInfo.push_back(friendEmail);
	
	cout << "\nPhone #: ";
	string friendPhone;
	cin >> friendPhone;
	friendInfo.push_back(friendPhone);


	vector<string> myInfo = {accountNumber, firstName, lastName, email, phone};

	Database* customer = new Database();
	customer->AddFriend(myInfo, friendInfo);
	delete customer;
	friendInfo.clear();
}


//Send money to a friend
void Customer::SendMoney()
{
	Database* customer = new Database();
	string fullName = firstName + lastName;
	
	bool notFound = true;
	while (notFound)
	{
		system("cls");

		customer->DisplayFriendList(fullName);

		//ask for which friend you want to send money to
		cout << "\nWho would you like to send money to? Please enter their name, email, and phone number" << endl;

		string firstFriendName;
		cout << "First Name: ";
		cin >> firstFriendName;

		string lastFriendName;
		cout << "\nLast Name: ";
		cin >> lastFriendName;

		string friendMail;
		cout << "\nEmail Address: ";
		cin >> friendMail;

		string friendPhone;
		cout << "\nPhone #: ";
		cin >> friendPhone;

		//get information from friend list
		vector<string> friendInfo = customer->searchFriend(fullName, firstFriendName, lastFriendName, friendMail, friendPhone);

		if (friendInfo.empty() == true)
		{
			friendInfo.clear();
			cout << "The information you entered is invalid or your contact no longer has an account!" << endl;
			cout << "\nWould you like to try again? (Y/y for yes)" << endl;

			string choice;
			cin >> choice;

			if (choice == "Y" || choice == "y")
			{
				continue;
			}
			else
			{
				notFound = false;
			}
		}
		else if (friendInfo.empty() != true)
		{

			//method 1 - like adding money
			bool nonValidNum = true;
			string sendMoney;
			while (nonValidNum)
			{
				//loop until user enters valid amount
				cout << "\nHow much money would you like to send?" << endl;
				cout << "$";
				cin >> sendMoney;

				size_t decimalPoint = sendMoney.find('.');
				if (decimalPoint != std::string::npos)
				{
					size_t decimalLength = sendMoney.substr(decimalPoint + 1).length();
					if (decimalLength == 2)
					{
						if (money < stod(sendMoney))
						{
							cout << "You are sending money that is more than what you have in your balance! Try again" << endl;
						}
						else
						{
							
							//verify friend in main customer database
							double friendBalance = customer->verifyFriend(friendInfo.at(0), friendInfo.at(1), friendInfo.at(2), friendInfo.at(3), friendInfo.at(4));


							if (friendBalance == -1)
							{
								cout << "Your contact no longer has an account! Cannot send funds..." << endl;
								friendBalance = 0;

								//break out of both loops
								notFound = false;
								nonValidNum = false;

							}
							else if (friendBalance >= 0.0)
							{
								vector<string> myInfo = { firstName, lastName, accountNumber };//0,1,2
								money = customer->SendMoney(myInfo, friendInfo, sendMoney, money, friendBalance);


								
								nonValidNum = false;
								notFound = false;
							}
						}
					}
					else
					{
						cout << "Invalid input, deposit must have only 2 decimal places\n" << endl;
					}
				}
				else
				{
					cout << "Invalid input, there must be a decimal point\n" << endl;
				}
			}
		}

	}

	delete customer;
}


//Request Money from a friend
void Customer::RequestMoney()
{
	Database* customer = new Database();
	string fullName = firstName + lastName;

	bool notFound = true;
	while (notFound)
	{
		system("cls");

		customer->DisplayFriendList(fullName);

		//ask for which friend you want to request money from
		cout << "\nWho would you like to request money from? Please enter their name, email, and phone number" << endl;

		string firstFriendName;
		cout << "First Name: ";
		cin >> firstFriendName;

		string lastFriendName;
		cout << "\nLast Name: ";
		cin >> lastFriendName;

		string friendMail;
		cout << "\nEmail Address: ";
		cin >> friendMail;

		string friendPhone;
		cout << "\nPhone #: ";
		cin >> friendPhone;

		//get information from friend list
		vector<string> friendInfo = customer->searchFriend(fullName, firstFriendName, lastFriendName, friendMail, friendPhone);

		if (friendInfo.empty() == true)
		{
			friendInfo.clear();
			cout << "The information you entered is invalid or your contact no longer has an account!" << endl;
			cout << "\nWould you like to try again? (Y/y for yes)" << endl;

			string choice;
			cin >> choice;

			if (choice == "Y" || choice == "y")
			{
				continue;
			}
			else
			{
				notFound = false;
			}
		}
		else if (friendInfo.empty() != true)
		{

			bool nonValidNum = true;
			string requestMoney;
			while (nonValidNum)
			{
				//loop until user enters valid amount
				cout << "\nHow much money would you like to request?" << endl;
				cout << "$";
				cin >> requestMoney;

				size_t decimalPoint = requestMoney.find('.');
				if (decimalPoint != std::string::npos)
				{
					size_t decimalLength = requestMoney.substr(decimalPoint + 1).length();
					if (decimalLength == 2)
					{
						//verify friend in main customer database
						double friendBalance = customer->verifyFriend(friendInfo.at(0), friendInfo.at(1), friendInfo.at(2), friendInfo.at(3), friendInfo.at(4));

						if (friendBalance == -1)
						{
							cout << "Your contact no longer has an account! Cannot request funds..." << endl;
							friendBalance = 0;

							//break out of both loops
							notFound = false;
							nonValidNum = false;

						}
						else if (friendBalance >= 0.0)
						{
							//update friend balance
							double requesting = stod(requestMoney);
							cout << fixed << setprecision(2);
							cout << "\nRequesting $" << requesting << " from " << friendInfo.at(1) << " " << friendInfo.at(2) << endl;

							//add requested funds to already remaining request
							requesting += stod(friendInfo.at(6));
							cout << fixed << setprecision(2);

							vector<string> myInfo = { firstName, lastName, accountNumber };//0,1,2
							//update your table and friend table
							customer->RequestMoney(myInfo, friendInfo, requesting);
						
							nonValidNum = false;
							notFound = false;
							myInfo.clear();
						}
						
					}
					else
					{
						cout << "Invalid input, deposit must have only 2 decimal places\n" << endl;
					}
				}
				else
				{
					cout << "Invalid input, there must be a decimal point\n" << endl;
				}
			}
		}

	}

	delete customer;
}