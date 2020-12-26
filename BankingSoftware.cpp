/**
*
* Solution to course project # 3
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Momchil Zanev
* @idnumber 62558
* @compiler VC
*
* <UserStruct.h>
* <HelperFunctions.h>
* <HelperFunctions.cpp>
*
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "UserStruct.h"
#include "HelperFunctions.h"
using namespace std;

const char dbSeparator = ':';
const double maxOverdraft = 10000.0;
const double minimumTransaction = 0.0;

void MainMenu(vector<user>& users);
int Login(vector<user>& users);
int Register(vector<user>& users);
void Quit(vector<user>& users);

void UserMenu(vector<user>& users, const int userId);
bool CancelAccount(vector<user>& users, const int userId);
void Deposit(vector<user>& users, const int userId);
void Transfer(vector<user>& users, const int userId);
void Withdraw(vector<user>& users, const int userId);

void loadUsersFromDb(vector<user>& users);
user getUserFromString(const string input);

int getUserId(vector<user>& users, const string username);
bool passwordsMatch(vector<user>& users, const int userId, const string passwordGuess);
bool usernameExists(vector<user>& users, const string username);

int main()
{
    vector<user> users;

    loadUsersFromDb(users);

    MainMenu(users);

    return 0;
}

void loadUsersFromDb(vector<user>& users)
{
    fstream userDb;
    userDb.open("users.txt", fstream::in);
    string currentLine;
    while (getline(userDb, currentLine))
    {
        user currentUser = getUserFromString(currentLine);
        users.push_back(currentUser);
    }
    userDb.close();
}

void MainMenu(vector<user>& users)
{
    int currentUserId;
    while (true)
    {
        const string menuArt =  
            "    /|    //| |                                     /|    //| |                                  \n"
            "   //|   // | |     ___       ( )       __         //|   // | |     ___         __               \n"
            "  // |  //  | |   //   ) )   / /     //   ) )     // |  //  | |   //___) )   //   ) )   //   / / \n"
            " //  | //   | |  //   / /   / /     //   / /     //  | //   | |  //         //   / /   //   / /  \n"
            "//   |//    | | ((___( (   / /     //   / /     //   |//    | | ((____     //   / /   ((___( (   \n";
        cout << menuArt;
        cout << "Commands:\nL - login:\nR - Register:\nQ - Quit:" << endl;

        const vector<char> commandCharacters = { 'L', 'R', 'Q' };
        char command = getCommand(commandCharacters);
        switch (command)
        {
        case 'L':
            currentUserId = Login(users);
            UserMenu(users, currentUserId);
            break;
        case 'R':
            currentUserId = Register(users);
            UserMenu(users, currentUserId);
            break;
        case 'Q':
            Quit(users);
            return;
        default:
            break;
        }
    }
}

int Login(vector<user>& users)
{
    cout << "Username:" << endl;
    string username;
    cin >> username;
    while (!usernameExists(users, username))
    {
        cout << "User doesn't exist\nUsername:" << endl;
        cin >> username;
    }
    int userId = getUserId(users, username);

    cout << "Password:" << endl;
    string password;
    cin >> password;
    while (!passwordsMatch(users, userId, password))
    {
        cout << "Incorrect password\nPassword:" << endl;
        cin >> password;
    }

    return userId;
}

int Register(vector<user>& users)
{
    cout << "Choose Username:" << endl;
    string username;
    cin >> username;
    while (!validateUsername(username))
    {
        cout << "Username must contain only latin letters, choose another Username:" << endl;
        cin >> username;
    }
    while (usernameExists(users, username))
    {
        cout << "Username is already taken, choose another Username:" << endl;
        cin >> username;
    }

    cout << "Choose a Password:" << endl;
    string password;
    cin >> password;
    while (!validatePassword(password))
    {
        cout << "Password must be at least 5 characters long and contain at least 1 lowercase, 1 uppercase letter and 1 symbol(!@#$%^&*), choose another Password:" << endl;
        cin >> password;
    }

    cout << "Confirm Password:" << endl;
    string confirmPassword;
    cin >> confirmPassword;
    while (confirmPassword != password)
    {
        cout << "Passwords don't match, confirm password:" << endl;
        cin >> confirmPassword;
    }

    hash<string> hashString;
    unsigned long hashedPassword = hashString(password);

    user newUser;
    newUser.name = username;
    newUser.hashedPassword = hashedPassword;
    newUser.balance = 0;

    users.push_back(newUser);

    return getUserId(users, username);
}

void Quit(vector<user>& users)
{
    fstream userDb;
    userDb.open("users.txt", fstream::out);

    for (int i = 0; i < users.size(); i++)
    {
        user currentUser = users[i];
        userDb << fixed << setprecision(2) << currentUser.name << dbSeparator << currentUser.hashedPassword << dbSeparator << currentUser.balance << endl;
    }

    userDb.close();
}

void UserMenu(vector<user>& users, const int userId)
{
    while (true)
    {
        user currentUser = users[userId];
        cout << fixed << setprecision(2) << "You have " << currentUser.balance << " BGN. Choose one of the following options:" << endl;
        cout << "C - Cancel account\nD - Deposit\nL - Logout\nT - Transfer\nW - Withdraw" << endl;

        const vector<char> commandCharacters = { 'C', 'D', 'L', 'T', 'W' };
        char command = getCommand(commandCharacters);
        switch (command)
        {
        case 'C':            
            if (CancelAccount(users, userId))
            {
                //If account was removed successfully go to Main menu
                return;
            }
            break;
        case 'D':
            Deposit(users, userId);
            break;
        case 'L':
            return;
        case 'T':
            Transfer(users, userId);
            break;
        case 'W':
            Withdraw(users, userId);
            break;
        default:
            break;
        }
    }    
}

bool CancelAccount(vector<user>& users, const int userId)
{
    cout << "Password:" << endl;
    string password;
    cin >> password;
    while (!passwordsMatch(users, userId, password))
    {
        cout << "Incorrect password\nPassword:" << endl;
        cin >> password;
    }

    if (users[userId].balance != 0)
    {
        cout << "Account can be canceled only if balance is equal to 0" << endl;
        return false;
    }

    users.erase(users.begin() + userId);
    return true;
}

void Deposit(vector<user>& users, const int userId)
{
    cout << "Choose amount to deposit:" << endl;
    double depositAmount = getValidDoubleInput();
    while (depositAmount <= minimumTransaction)
    {
        cout << "Amount must be greater than " << minimumTransaction << "\nChoose amount to deposit:" << endl;
        depositAmount = getValidDoubleInput();
    }
    users[userId].balance += depositAmount;
}

void Transfer(vector<user>& users, const int userId)
{
    cout << "Choose destination account name:" << endl;
    string destinationName;
    cin >> destinationName;
    int destinationId = getUserId(users, destinationName);
    while (!(usernameExists(users, destinationName) && destinationId != userId))
    {
        cout << "Invalid account\nChoose destination account name:" << endl;
        cin >> destinationName;
        destinationId = getUserId(users, destinationName);
    }

    double userBalance = users[userId].balance;

    cout << "Choose amount to transfer:" << endl;
    double transferAmount = getValidDoubleInput();
    double userBalanceAfterTransfer = userBalance - transferAmount;
    while (transferAmount <= minimumTransaction || userBalanceAfterTransfer < -maxOverdraft)
    {
        cout << "Amount must be greater than " << minimumTransaction << " and maximum overdraft is " << maxOverdraft << " BGN\nChoose amount to transfer:" << endl;
        transferAmount = getValidDoubleInput();
        userBalanceAfterTransfer = userBalance - transferAmount;
    }

    users[userId].balance = userBalanceAfterTransfer;
    users[destinationId].balance += transferAmount;
}

void Withdraw(vector<user>& users, const int userId)
{
    double userBalance = users[userId].balance;

    cout << "Choose amount to withdraw:" << endl;
    double withdrawAmount = getValidDoubleInput();
    double userBalanceAfterWithdraw = userBalance - withdrawAmount;
    while (withdrawAmount <= minimumTransaction || userBalanceAfterWithdraw < -maxOverdraft)
    {
        cout << "Amount must be greater than " << minimumTransaction << " and maximum overdraft is " << maxOverdraft << " BGN\nChoose amount to withdraw:" << endl;
        withdrawAmount = getValidDoubleInput();
        userBalanceAfterWithdraw = userBalance - withdrawAmount;
    }

    users[userId].balance -= withdrawAmount;
}

int getUserId(vector<user>& users, const string username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].name == username)
        {
            return i;
        }
    }
    return -1;
}

bool passwordsMatch(vector<user>& users, const int userId, const string passwordGuess)
{
    hash<string> hashString;
    unsigned long hashedPasswordGuess = hashString(passwordGuess);

    if (users[userId].hashedPassword == hashedPasswordGuess)
    {
        return true;
    }
    return false;
}

bool usernameExists(vector<user>& users, const string username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].name == username)
        {
            return true;
        }
    }
    return false;
}

user getUserFromString(const string input)
{
    int length = input.size();

    int index = 0;
    char tmp = input[index];
    string name;
    while (tmp != dbSeparator && index < length)
    {
        name += tmp;
        index++;
        tmp = input[index];
    }

    index++;
    tmp = input[index];
    string hashedPasswordStr;
    while (tmp != dbSeparator && index < length)
    {
        hashedPasswordStr += tmp;
        index++;
        tmp = input[index];
    }
    unsigned long hashedPassword = stoul(hashedPasswordStr);

    index++;
    tmp = input[index];
    string balanceStr;
    while (tmp != dbSeparator && index < length)
    {
        balanceStr += tmp;
        index++;
        tmp = input[index];
    }

    double balance = stod(balanceStr);

    user currentUser;
    currentUser.name = name;
    currentUser.hashedPassword = hashedPassword;
    currentUser.balance = balance;

    return currentUser;
}