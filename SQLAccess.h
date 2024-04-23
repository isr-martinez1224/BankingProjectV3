#pragma once

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <time.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

//login sql server info for myself
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "sql212#im";

//connect to database
/*
sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* res;*/

class Database
{
public:
    Database();
    ~Database();

    void AddAccount(string, string, string, string, string, string, string, string);
    vector<string> Login(string, string, string);

    //CUSTOMER
    void EditBalance(string, string, string, double, string, double);
    void AddFriend(vector<string>, vector<string>);
    void viewTransactions(string, string);

    //MANAGER
    void DisplayCustomers();
    bool DeleteUser(int, string, string);
    vector<string> SearchUser(int, string, string);

    //Friend Functions
    void DisplayFriendList(string);
    vector<string> searchFriend(string, string, string, string, string);
    double verifyFriend(string, string, string, string, string);
    double SendMoney(vector<string>, vector<string>, string, double, double);
    void RequestMoney(vector<string>, vector<string>, double);
private:

    
};

Database::Database()
{
    
}

Database::~Database()
{
  
}


/*-------------ADD ACCOUNT TO DATABASE-------------------*/
void Database::AddAccount(string fName, string lName, string accountType, string mail, string phoneNum, string accNum, string uName, string passCode)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);



        

        con->setSchema("bankingapplication");
        if (accountType == "Customer")
        {
            stmt = con->createStatement();
            //this works to make a table if it does not exist
            stmt->execute("CREATE TABLE IF NOT EXISTS customer (USERID serial PRIMARY KEY, FirstName VARCHAR(50), LastName VARCHAR(50), AccountType VARCHAR(50), EmailAddress VARCHAR(50), PhoneNumber VARCHAR(50), Username VARCHAR(50), Password VARCHAR(50), AccountNumber VARCHAR(50), AccountBalance DECIMAL(15,2)); ");
            string fullName = fName + lName;
            //create a contact list
            stmt->execute("CREATE TABLE IF NOT EXISTS " + fullName + "Friends (AccountNumber VARCHAR(50) PRIMARY KEY, FirstName VARCHAR(50), LastName VARCHAR(50), EmailAddress VARCHAR(50), PhoneNumber VARCHAR(50), MoneySendToFriend DECIMAL(15,2), MoneyRecieveFromFriend DECIMAL(15,2)); ");
            //create a transaction list
            stmt->execute("CREATE TABLE IF NOT EXISTS " + fullName + "Transactions (TransactionNumber serial PRIMARY KEY, TransactionType VARCHAR(50), MoneyInTransaction DECIMAL(15,2), DateAndTime VARCHAR(50)); ");
            delete stmt;

            //insert customer into database
            pstmt = con->prepareStatement("INSERT INTO customer(FirstName, LastName, AccountType, EmailAddress, PhoneNumber, Username, Password, AccountNumber, AccountBalance) VALUES(?,?,?,?,?,?,?,?,?)");
            pstmt->setString(1, fName);
            pstmt->setString(2, lName);
            pstmt->setString(3, accountType);
            pstmt->setString(4, mail);
            pstmt->setString(5, phoneNum);
            pstmt->setString(6, uName);
            pstmt->setString(7, passCode);
            pstmt->setString(8, accNum);
            pstmt->setDouble(9, 0.00);
            pstmt->execute();


            //insert first trasaction
            pstmt = con->prepareStatement("INSERT INTO " + fullName + "Transactions(TransactionType, DateAndTime) VALUES(? , ? )");
            pstmt->setString(1, "Account Creation");
            time_t ct = time(0);
            string currentTime = ctime(&ct);
            pstmt->setString(2, currentTime);
            pstmt->execute();


            cout << "Information saved in the database." << endl;

            delete pstmt;
        }
        else if (accountType == "Manager")
        {
            stmt = con->createStatement();
            //this works to make a table if it does not exist
            stmt->execute("CREATE TABLE IF NOT EXISTS manager (USERID serial PRIMARY KEY, FirstName VARCHAR(50), LastName VARCHAR(50), AccountType VARCHAR(50), EmailAddress VARCHAR(50), PhoneNumber VARCHAR(50), Username VARCHAR(50), Password VARCHAR(50)); ");
            delete stmt;

            pstmt = con->prepareStatement("INSERT INTO manager(FirstName, LastName, AccountType, EmailAddress, PhoneNumber, Username, Password) VALUES(?,?,?,?,?,?,?)");
            pstmt->setString(1, fName);
            pstmt->setString(2, lName);
            pstmt->setString(3, accountType);
            pstmt->setString(4, mail);
            pstmt->setString(5, phoneNum);
            pstmt->setString(6, uName);
            pstmt->setString(7, passCode);
            pstmt->execute();
            cout << "Information saved in the database." << endl;

            delete pstmt;
        }

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}


/*-------------VERIFY LOGIN/RETURN ACCOUNT DETAILS IF VERIFIED-------------------*/
vector<string> Database::Login(string accType, string uName, string passCode)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        vector<string> userData;
        userData.clear();
        con->setSchema("bankingapplication");
        bool found = false;
        if (accType == "Customer")
        {
            stmt = con->createStatement();

            //fetch columns from table
            res = stmt->executeQuery("SELECT FirstName, LastName, AccountType, EmailAddress, PhoneNumber, Username, Password, AccountNumber, AccountBalance  FROM customer");

            while (res->next() && found == false)
            {
                //find a way to compare the res to your input
                if (uName == res->getString("Username") && passCode == res->getString("Password"))
                {
                    found = true;
                    //get the rest of the data
                    userData.push_back(res->getString("FirstName"));
                    userData.push_back(res->getString("LastName"));
                    userData.push_back(res->getString("EmailAddress"));
                    userData.push_back(res->getString("PhoneNumber"));
                    userData.push_back(res->getString("Username"));
                    userData.push_back(res->getString("Password"));
                    userData.push_back(res->getString("AccountNumber"));
                    userData.push_back(to_string(res->getDouble("AccountBalance")));
                }
            }

            delete res;
            delete stmt;
        }
        else if (accType == "Manager")
        {
            stmt = con->createStatement();

            //fetch columns from table
            res = stmt->executeQuery("SELECT FirstName, LastName, AccountType, EmailAddress, PhoneNumber, Username, Password  FROM manager");

            while (res->next() && found == false)
            {
                //find a way to compare the res to your input
                if (uName == res->getString("Username") && passCode == res->getString("Password"))
                {
                    found = true;
                    //get the rest of the data
                    userData.push_back(res->getString("FirstName"));
                    userData.push_back(res->getString("LastName"));
                    userData.push_back(res->getString("EmailAddress"));
                    userData.push_back(res->getString("PhoneNumber"));
                    userData.push_back(res->getString("Username"));
                    userData.push_back(res->getString("Password"));
                }
            }

            delete res;
            delete stmt;
        }

        delete con;
        //return vector
        return userData;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}


/********************************************************************************************************************************************************/
/****************************                                                                                     ***************************************/
/****************************                  CUSTOMER FUNCTIONS                                                 ***************************************/
/****************************                                                                                     ***************************************/
/********************************************************************************************************************************************************/

/*-------------EDIT MONEY IN DATABASE FOR A CUSTOMER-------------------*/
void Database::EditBalance(string fName, string lName, string accNum, double money, string transaction, double moneyInTransaction)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;


        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        pstmt = con->prepareStatement("UPDATE customer SET AccountBalance = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ?");

        pstmt->setDouble(1, money);
        pstmt->setString(2, fName);
        pstmt->setString(3, lName);
        pstmt->setString(4, accNum);

        pstmt->executeUpdate();

        //add transaction
        string fullName = fName + lName;
        pstmt = con->prepareStatement("INSERT INTO " + fullName + "Transactions(TransactionType, MoneyInTransaction, DateAndTime) VALUES(? , ? , ? )");
        pstmt->setString(1, transaction);
        pstmt->setDouble(2, moneyInTransaction);
        time_t ct = time(0);
        string currentTime = ctime(&ct);
        pstmt->setString(3, currentTime);
        pstmt->execute();


        delete pstmt;
        delete stmt;

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}

/*-------------CUSTOMER CAN ADD ANOTHER CUSTOMER AS A FRIEND CONTACT-------------------*/
void Database::AddFriend(vector<string> myInfo, vector<string> friendInfo)//get info from customer database first
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);





        con->setSchema("bankingapplication");

        stmt = con->createStatement();



        //fetch columns from table
        //check if your friend is an existing user in the database
        res = stmt->executeQuery("SELECT USERID, FirstName, LastName, EmailAddress, PhoneNumber, AccountNumber FROM customer");

        bool found = false;
        vector<string> friendDatabaseInfo;
        while (res->next() && found == false)
        {
            //find a way to compare the res to your input
            if (friendInfo.at(0) == res->getString("FirstName") && friendInfo.at(1) == res->getString("LastName") && friendInfo.at(2) == res->getString("EmailAddress") && friendInfo.at(3) == res->getString("PhoneNumber"))
            {
                found = true;
                friendDatabaseInfo.push_back(res->getString("AccountNumber"));
                friendDatabaseInfo.push_back(res->getString("FirstName"));
                friendDatabaseInfo.push_back(res->getString("LastName"));
                friendDatabaseInfo.push_back(res->getString("EmailAddress"));
                friendDatabaseInfo.push_back(res->getString("PhoneNumber"));
            }
        }



        if (found == false)
        {
            cout << "\nThis user does not exist or the information you entered is incorrect!" << endl;
            return;
        }
        else if (found == true)
        {
            pstmt = con->prepareStatement("INSERT INTO " + myInfo.at(1) + myInfo.at(2) + "Friends(AccountNumber, FirstName, LastName, EmailAddress, PhoneNumber, MoneySendToFriend, MoneyRecieveFromFriend) VALUES(? , ? , ? , ? , ? , ? , ? )");
            pstmt->setString(1, friendDatabaseInfo.at(0));
            pstmt->setString(2, friendDatabaseInfo.at(1));
            pstmt->setString(3, friendDatabaseInfo.at(2));
            pstmt->setString(4, friendDatabaseInfo.at(3));
            pstmt->setString(5, friendDatabaseInfo.at(4));
            pstmt->setDouble(6, 0.00);
            pstmt->setDouble(7, 0.00);
            pstmt->execute();


            pstmt = con->prepareStatement("INSERT INTO " + friendDatabaseInfo.at(1) + friendDatabaseInfo.at(2) + "Friends(AccountNumber, FirstName, LastName, EmailAddress, PhoneNumber, MoneySendToFriend, MoneyRecieveFromFriend) VALUES(? , ? , ? , ? , ? , ? , ? )");
            pstmt->setString(1, myInfo.at(0));
            pstmt->setString(2, myInfo.at(1));
            pstmt->setString(3, myInfo.at(2));
            pstmt->setString(4, myInfo.at(3));
            pstmt->setString(5, myInfo.at(4));
            pstmt->setDouble(6, 0.00);
            pstmt->setDouble(7, 0.00);
            pstmt->execute();
            delete pstmt;
            cout << "Information saved in the database." << endl;
        }

        delete res;
        delete stmt;
        
        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}



void Database::DisplayFriendList(string name)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);






        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        res = stmt->executeQuery("SELECT AccountNumber, FirstName, LastName, EmailAddress, PhoneNumber, MoneySendToFriend, MoneyRecieveFromFriend FROM " + name + "Friends ORDER BY FirstName");

        cout << "Displaying all friends...\n" << endl;
        cout << setw(26) << "First Name" << setw(31) << "Last Name" << setw(51) << "Email Address" << setw(20) << "Phone Number" << setw(35) << "Money Reqested By Friend" << setw(35) << "Money Pending From Friend" << endl;
        for (int i = 0; i < 237; i++)
        {
            cout << "-";
        }
        cout << endl;

        while (res->next()) {

            cout << setw(26) << res->getString("FirstName") << setw(31) << res->getString("LastName") << setw(51) << res->getString("EmailAddress")
                << setw(20) << res->getString("PhoneNumber") << setw(35) << res->getString("MoneySendToFriend") << setw(35) << res->getString("MoneyRecieveFromFriend") << endl;
        }

        delete res;
        delete stmt;

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}

//find friend in your contact list
vector<string> Database::searchFriend(string user, string first, string last, string email, string phone)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);





        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //GET THE FRIEND DATA FROM USER LIST 
        res = stmt->executeQuery("SELECT AccountNumber, FirstName, LastName, EmailAddress, PhoneNumber, MoneySendToFriend, MoneyRecieveFromFriend FROM " + user + "Friends ORDER BY FirstName");

        vector<string> info;
        bool found = false;
        while (res->next() && found == false)
        {
            //find a way to compare the res to your input
            if (first == res->getString("FirstName") && last == res->getString("LastName") && email == res->getString("EmailAddress") && phone == res->getString("PhoneNumber"))
            {
                found = true;
                //get the rest of the data
                info.push_back(res->getString("AccountNumber"));//0
                info.push_back(res->getString("FirstName"));//1
                info.push_back(res->getString("LastName"));//2
                info.push_back(res->getString("EmailAddress"));//3
                info.push_back(res->getString("PhoneNumber"));//4
                info.push_back(to_string(res->getDouble("MoneySendToFriend")));//5
                info.push_back(to_string(res->getDouble("MoneyRecieveFromFriend")));//6
            }
        }

        delete res;
        delete stmt;

        delete con;
        return info;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}


//verify that friend exists in main customer database         something here is crashing
double Database::verifyFriend(string accNum, string fName, string lName, string mail, string phone)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);



        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //fetch columns from table
        res = stmt->executeQuery("SELECT FirstName, LastName, EmailAddress, PhoneNumber, AccountNumber, AccountBalance FROM customer");

        double balance = 0.0;
        bool found = false;
        while (res->next() && found == false)
        {
            //find a way to compare the res to your input
            if (accNum == res->getString("AccountNumber") && fName == res->getString("FirstName") && lName == res->getString("LastName") && mail == res->getString("EmailAddress") && phone == res->getString("PhoneNumber"))
            {
                found = true;
                //get balance for friend
                balance = res->getDouble("AccountBalance");
                cout << fixed << setprecision(2);
            }
            else
            {
                balance = -1;
            }
        }

        delete res;
        delete stmt;

        delete con;
        return balance;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}



/*-------------SEND MONEY----------------------*/
double Database::SendMoney(vector<string> myDetails, vector<string> friendDetails, string moneyInput, double myBalance, double friendBalance)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        //update friend balance
        double sending = stod(moneyInput);
        cout << fixed << setprecision(2);
        friendBalance += sending;
        cout << fixed << setprecision(2);
        cout << "\nSending $" << sending << " to " << friendDetails.at(1) << " " << friendDetails.at(2) << endl;


        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //add funds to friend
        pstmt = con->prepareStatement("UPDATE customer SET AccountBalance = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ?");
        friendBalance = friendBalance;
        cout << fixed << setprecision(2);
        pstmt->setDouble(1, friendBalance);
        pstmt->setString(2, friendDetails.at(1));
        pstmt->setString(3, friendDetails.at(2));
        pstmt->setString(4, friendDetails.at(0));

        pstmt->executeUpdate();


        //update your balance
        myBalance -= stod(moneyInput);
        cout << fixed << setprecision(2);


        //subtract funds from you
        pstmt = con->prepareStatement("UPDATE customer SET AccountBalance = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ?");
        myBalance = myBalance;
        cout << fixed << setprecision(2);
        pstmt->setDouble(1, myBalance);
        pstmt->setString(2, myDetails.at(0));
        pstmt->setString(3, myDetails.at(1));
        pstmt->setString(4, myDetails.at(2));

        pstmt->executeUpdate();

        cout << "Updated Balance after sending money: $" << myBalance << endl;


        //check if you owed money, update friend list
        if (stod(friendDetails.at(5)) > 0)
        {
            double moneyRemaining = 0.0;
            if (sending >= stod(friendDetails.at(5)))
            {
                moneyRemaining = 0;
            }
            else
            {
                moneyRemaining = stod(friendDetails.at(5)) - sending;

            }

            //update your table
            pstmt = con->prepareStatement("UPDATE " + myDetails.at(0) + myDetails.at(1) + "Friends SET MoneySendToFriend = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ? ");

            moneyRemaining = moneyRemaining;
            cout << fixed << setprecision(2);
            pstmt->setDouble(1, moneyRemaining);
            pstmt->setString(2, friendDetails.at(1));
            pstmt->setString(3, friendDetails.at(2));
            pstmt->setString(4, friendDetails.at(0));

            pstmt->executeUpdate();


            //update table for friend
            pstmt = con->prepareStatement("UPDATE " + friendDetails.at(1) + friendDetails.at(2) + "Friends SET MoneyRecieveFromFriend = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ? ");

            moneyRemaining = moneyRemaining;
            cout << fixed << setprecision(2);
            pstmt->setDouble(1, moneyRemaining);
            pstmt->setString(2, myDetails.at(0));
            pstmt->setString(3, myDetails.at(1));
            pstmt->setString(4, myDetails.at(2));

            pstmt->executeUpdate();
        }

        //update your transaction sheet
        string transactionText = "Sent Money to: " + friendDetails.at(1) + " " + friendDetails.at(2);
        pstmt = con->prepareStatement("INSERT INTO " + myDetails.at(0) + myDetails.at(1) + "Transactions(TransactionType, MoneyInTransaction, DateAndTime) VALUES(? , ? , ? )");
        pstmt->setString(1, transactionText);
        pstmt->setDouble(2, sending);
        time_t ct = time(0);
        string currentTime = ctime(&ct);
        pstmt->setString(3, currentTime);
        pstmt->execute();


        //update recieving transaction for friend
        string recievingText = "Recieved Money from: " + myDetails.at(0) + " " + myDetails.at(1);
        pstmt = con->prepareStatement("INSERT INTO " + friendDetails.at(1) + friendDetails.at(2) + "Transactions(TransactionType, MoneyInTransaction, DateAndTime) VALUES(? , ? , ? )");
        pstmt->setString(1, recievingText);
        pstmt->setDouble(2, sending);
        pstmt->setString(3, currentTime);
        pstmt->execute();


        delete pstmt;
        delete stmt;

        delete con;

        return myBalance;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}

/*-------------REQUEST MONEY-------------------*/
void Database::RequestMoney(vector<string> myDetails, vector<string> friendDetails, double fundsRequested)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //update your table
        pstmt = con->prepareStatement("UPDATE " + myDetails.at(0) + myDetails.at(1) + "Friends SET MoneyRecieveFromFriend = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ? ");

        pstmt->setDouble(1, fundsRequested);
        pstmt->setString(2, friendDetails.at(1));
        pstmt->setString(3, friendDetails.at(2));
        pstmt->setString(4, friendDetails.at(0));

        pstmt->executeUpdate();


        //update table for friend
        pstmt = con->prepareStatement("UPDATE " + friendDetails.at(1) + friendDetails.at(2) + "Friends SET MoneySendToFriend = ? WHERE FirstName = ? AND LastName = ? AND AccountNumber = ? ");

        pstmt->setDouble(1, fundsRequested);
        pstmt->setString(2, myDetails.at(0));
        pstmt->setString(3, myDetails.at(1));
        pstmt->setString(4, myDetails.at(2));

        pstmt->executeUpdate();



        delete pstmt;
        delete stmt;

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}



//view your transactions
void Database::viewTransactions(string first, string last)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        system("cls");
        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        res = stmt->executeQuery("SELECT TransactionNumber, TransactionType, MoneyInTransaction, DateAndTime FROM " + first + last + "Transactions ORDER BY TransactionNumber DESC");

        cout << "Displaying all transactions...\n" << endl;
        cout << setw(32) << "TransactionNumber(Most Recent)" << setw(45) << "TransactionType" << setw(40) << "MoneyInTransaction" << setw(40) << "DateAndTime" << endl;
        for (int i = 0; i < 237; i++)
        {
            cout << "-";
        }
        cout << endl;

        while (res->next()) {

            cout << setw(32) << res->getString("TransactionNumber") << setw(45) << res->getString("TransactionType") << setw(40) << res->getString("MoneyInTransaction")
                << setw(40) << res->getString("DateAndTime") << endl;
        }

        delete res;
        delete stmt;

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}
/********************************************************************************************************************************************************/
/****************************                                                                                     ***************************************/
/****************************                  MANAGER FUNCTIONS                                                  ***************************************/
/****************************                                                                                     ***************************************/
/********************************************************************************************************************************************************/

/*-------------DISPLAY ALL CUSTOMERS FROM DATABASE-------------------*/
void Database::DisplayCustomers()
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        res = stmt->executeQuery("SELECT USERID, FirstName, LastName, EmailAddress, PhoneNumber, AccountNumber FROM customer ORDER BY USERID");

        cout << "Displaying all customers...\n" << endl;
        cout << setw(16) << "USERID" << setw(26) << "First Name" << setw(31) << "Last Name" << setw(51) << "Email Address" << setw(20) << "Phone Number" << setw(20) << "Account Number" << endl;
        for (int i = 0; i < 237; i++)
        {
            cout << "-";
        }
        cout << endl;

        while (res->next()) {

            cout << setw(16) << res->getInt("USERID") << setw(26) << res->getString("FirstName") << setw(31) << res->getString("LastName") << setw(51) << res->getString("EmailAddress")
                << setw(20) << res->getString("PhoneNumber") << setw(20) << res->getString("AccountNumber") << endl;
        }

        delete res;
        delete stmt;

        delete con;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}


//Delete customer profile from database
bool Database::DeleteUser(int id, string first, string last)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //fetch columns from table
        res = stmt->executeQuery("SELECT USERID, FirstName, LastName, AccountType, EmailAddress, PhoneNumber, Username, Password, AccountNumber, AccountBalance  FROM customer");

        bool found = false;
        while (res->next() && found == false)
        {
            //find a way to compare the res to your input
            if (to_string(id) == res->getString("USERID") && first == res->getString("FirstName") && last == res->getString("LastName"))
            {
                found = true;

                pstmt = con->prepareStatement("DELETE FROM customer WHERE USERID = ? AND FirstName = ? AND LastName = ?");


                pstmt->setInt(1, id);
                pstmt->setString(2, first);
                pstmt->setString(3, last);

                pstmt->executeUpdate();

                delete pstmt;
            }
        }

        delete res;
        
        delete stmt;

        delete con;
        return found;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}



//Look up a customer's profile details
vector<string> Database::SearchUser(int id, string first, string last)
{
    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);




        vector<string> searchData;
        searchData.clear();
        con->setSchema("bankingapplication");
        stmt = con->createStatement();

        //fetch columns from table
        res = stmt->executeQuery("SELECT USERID, FirstName, LastName, EmailAddress, PhoneNumber, Username, AccountNumber, AccountBalance  FROM customer");

        bool found = false;
        while (res->next() && found == false)
        {
            //find a way to compare the res to your input
            if (to_string(id) == res->getString("USERID") && first == res->getString("FirstName") && last == res->getString("LastName"))
            {
                found = true;
                //get the rest of the data into vector
                searchData.push_back(res->getString("FirstName"));
                searchData.push_back(res->getString("LastName"));
                searchData.push_back(res->getString("EmailAddress"));
                searchData.push_back(res->getString("PhoneNumber"));
                searchData.push_back(res->getString("Username"));
                searchData.push_back(res->getString("AccountNumber"));
                searchData.push_back(to_string(res->getDouble("AccountBalance")));
            }
        }


        delete res;
        delete stmt;

        delete con;
        return searchData;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
}

