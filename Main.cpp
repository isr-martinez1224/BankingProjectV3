#include<iostream>
#include<limits>

#include "CreateAccount.h"
#include "UserLogin.h"

using namespace std;

int main() {

	
	while(true){
		int menuSelect;

		system("cls");
		cout << "BANKING APPLICATION" << endl;

		cout << "0 - Quit" << endl;
		cout << "1 - Login" << endl;
		cout << "2 - Sign Up" << endl;
		
		while (true)
		{
			cout << "\nSelection: ";
			// Read user input
			if (!(cin >> menuSelect)) {
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
	

		if (menuSelect == 0)
		{
			cout << "Goodbye!" << endl;
			exit(0);
		}
		else if (menuSelect == 1)
		{
			Login* userLogin = new Login();
			userLogin->LoginInformation();
			delete userLogin;
		}
		else if (menuSelect == 2)
		{
			CreateAccount* userCreate = new CreateAccount();
			userCreate->CreateUserAccount();
			delete userCreate;
		}
		else
		{
			cout << "Not a valid choice!" << endl;
			system("pause");
		}

	}

	return 0;
}