<div align="center">
<h1>Banking Project V3</h1>
</div>

This is a class project I made using C++ and SQL programming languages in Spring 2024. Visual Studio 2022 and MySQL were used to build the program and the databases that contain the information that are used during runtime. This project simulates a Banking Application where customers can manage their money and add other members as contacts in order to request & send funds. The manager role can see who is enrolled, pull information, and delete invalid users.


## Main.cpp
<p align="center">
  <img src="docs/showcase/console_menu.JPG"/>
</p>


The program launches in a console window, which is best displayed in fullscreen for the most efficient experience. 
The user is presented with 3 options:
1. **Quit**

   Terminates the program
   
2. **Login**
   
   User can log into the system with their credentials

3. **Sign Up**

   User can choose this option to create an account in the system


## CreateAccount.h
<table>
    <tr>
        <th>
            Sign Up Process
            <img src="docs/showcase/console_create1.JPG"/>
        </th>
        <th>
            Database After Sign Up
            <img src="docs/showcase/console_create2.JPG"/>
        </th>
    </tr>
</table>

Account creation operations are a part of the CreateAccount class and also interacts with the Database class which contains access to the MySQL databases. When the user wants to create an account, they need to go through a sign up process and fill out information. First the user must enter their first and last names.
Next, they need to choose what account they are signing up for: 
- Customer
- Manager

Next, the user will need to enter an email and phone number that will be connected to their account. If the user is signing up for a customer account, then the program will generate a random bank account number for them. This will not happen for users signing up for a manager account. Finally, the user will come up with a username and secure password as their login credentials.

That is the end of the sign up process and then the program will write all the information and save it into the customer or manager database respectively. Now the user can log in!


## UserLogin.h
<p align="center">
  <img src="docs/showcase/console_login.JPG"/>
</p>

Login operations are a part of the CreateAccount class and also interacts with the Database class which contains access to the MySQL databases. Customer and Manager classes are also involved in order to provide the correct functions based on the account type. First the user needs to enter what account type they are trying to log into (customer/manager). Next, the user needs to enter their username and password. The program will send this entered information into a function called <ins>**CheckUserType**</ins>. The Database class will then access the MySQL databases and search for the user infomation using queries that the entered information will help build and other predetermined queries. If the user is a customer, the program will search in the Customer database and if the user is a manager, then the program will search in the Manager database instead. The program will go row by row to find the matching login credentials and if a match is found, all of the important information of the row will be pushed onto a vector. The vector is then checked to see if it is empty or not empty. If it is empty, that means matching information was not found and therefore the login is invalid, the user must try again. Otherwise, the vector contains matching user information and the login attempt was successful.

The user will then be redirected to their respective dashboard according to their account type. All their available options are presented to them on the screen.


## Customer.h
<p align="center">
  <img src="docs/showcase/console_customer1.JPG"/>
</p>

When the customer successfully logs into their account, they are greeted on the dashboard with their name and available balance. A customer account has 7 possible choices:

0. **View Recent Transactions**

   Customer can see all recent transactions made on their account
   
1. **Deposit Money**
   
   Customer can add money to their account

2. **Withdraw Money**

   Customer can take money out of their account

3. **Add a Friend**

   Customer can add another customer user as a friend in their contact list
   
4. **Send Money**
   
   Customer can send money to a friend in their contact list

5. **Request Money**

   Customer can request to recieve money from a friend in their contact list

6. **Sign Off**

   Customer signs out of their account, program returns to main menu


### Deposit Money
The customer is first presented with their current balance. Then they are asked to enter how much money they would like to deposit and must enter a valid monetary amount. The balance is updated on the screen and in the database.
<p align="center">
  <img src="docs/showcase/console_customer2.JPG"/>
</p>


### Withdraw Money
The customer is first presented with their current balance. Then they are asked to enter how much money they would like to withdraw and must enter a valid monetary amount. They must also enter an amount that does not exceed their available funds or else they will be asked to try again. The balance is updated on the screen and in the database.
<p align="center">
  <img src="docs/showcase/console_customer3.JPG"/>
</p>


### Add a Friend
The customer can add another customer as a friend in their contact list. They need to fill out the information requested and the program will check if that person exists in the database. If they do, then that person's basic information is added to the customer's contact list, in this case they have their own small database of contacts.
<table>
    <tr>
        <th>
            Adding a friend
            <img src="docs/showcase/console_customer4.JPG"/>
        </th>
        <th>
            Friend is an existing user
            <img src="docs/showcase/console_customer5.JPG"/>
        </th>
    </tr>
    <tr>
        <th>
            Customer's Contacts in own table
            <img src="docs/showcase/console_.JPG"/>
        </th>
        <th>
        </th>
    </tr>
</table>


### Send Money
The customer can send a friend money. Their contact list will display on the screen and the customer will use that information to specify who to send money to. If that person exists in the list AND is still a user in the datatbase, then money will be properly transferred. Funds are properply updated in th main database and will be reflected on both accounts.
<p align="center">
  <img src="docs/showcase/console_customer6.JPG"/>
</p>


### Request Money
The customer can request money from a friend. Their contact list will display on the screen and the customer will use that information to specify who to request money from. If that person exists in the list AND is still a user in the datatbase, then the request is updated on the friend's contact list. The amount requested is displayed and will be updated as the user sends money to the recipiant. Funds are properply updated in th main database and will be reflected on both accounts.
<table>
    <tr>
        <th>
            Requesting Money as Justin
            <img src="docs/showcase/console_customer7.JPG"/>
        </th>
        <th>
            Updated Request from Cory's view
            <img src="docs/showcase/console_customer11.JPG"/>
        </th>
    </tr>
</table>


### Sign Off
The customer can sign out of their account once they are finished with their tasks. User returns to the main menu and now a new user can log in.
<p align="center">
  <img src="docs/showcase/console_customer9.JPG"/>
</p>


### View Recent Transactions
The customer can view all transactions that have occured on their account including the date and time which they occurred.
<p align="center">
  <img src="docs/showcase/console_customer8.JPG"/>
</p>
