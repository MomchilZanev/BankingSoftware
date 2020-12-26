#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct user
{
    string name;
    unsigned long hashedPassword;
    double balance;
};

user getUserFromString(string input);

int Login(vector<user>& users);
int Register(vector<user>& users);
void Quit(vector<user>& users);

void userMenu(vector<user>& users, int userId);
void cancelAccount(vector<user>& users, int userId);
void deposit(vector<user>& users, int userId);
void transfer(vector<user>& users, int userId);
void withdraw(vector<user>& users, int userId);

int getUserId(vector<user>& users, string username);
bool passwordsMatch(vector<user>& users, int userId, string passwordGuess);

bool usernameExists(vector<user>& users, string username);
bool validateUsername(string username);

bool validatePassword(string password);
bool charIsSymbol(char character);
bool stringContainsSymbol(string input);
bool stringContainsUppercaseLetter(string input);
bool stringContainsLowercaseLetter(string input);
bool stringContainsIllegalCharacters(string input);

char getCommand(vector<char> allowedCharacters);
bool characterIsAllowed(vector<char> allowedCharacters, char character);
char toUpperCase(char character);

int main()
{
    vector<user> users;

    fstream userDb;
    userDb.open("users.txt", fstream::in);
    string currentLine;
    while (getline(userDb, currentLine))
    {
        user currentUser = getUserFromString(currentLine);
        users.push_back(currentUser);
    }
    userDb.close();

    int currentUserId;
    while (true)
    {
        cout << "   ---   Main Menu   ---" << endl;
        cout << "Commands:" << endl;
        cout << "L - login:" << endl;
        cout << "R - Register:" << endl;
        cout << "Q - Quit:" << endl;
        
        vector<char> commandCharacters = { 'L', 'R', 'Q' };
        char command = getCommand(commandCharacters);
        switch (command)
        {
        case 'L':
            currentUserId = Login(users);
            userMenu(users, currentUserId);
            break;
        case 'R':
            currentUserId = Register(users);
            userMenu(users, currentUserId);
            break;
        case 'Q':
            Quit(users);
            return 0;
        default:
            break;
        }
    }

    return 0;
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
        cout << "Passwords dont match, confirm password:" << endl;
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
        userDb << fixed << setprecision(2) << currentUser.name << ':' << currentUser.hashedPassword << ':' << currentUser.balance << endl;
    }

    userDb.close();
}

void userMenu(vector<user>& users, int userId)
{
    while (true)
    {
        user currentUser = users[userId];
        cout << "You have " << currentUser.balance << " BGN. Choose one of the following options:" << endl;
        cout << "C - Cancel account\nD - Deposit\nL - Logout\nT - Transfer\nW - Withdraw" << endl;

        vector<char> commandCharacters = { 'C', 'D', 'L', 'T', 'W' };
        char command = getCommand(commandCharacters);
        switch (command)
        {
        case 'C':
            cancelAccount(users, userId);
            return;
        case 'D':
            deposit(users, userId);
            break;
        case 'L':
            return;
        case 'T':
            transfer(users, userId);
            break;
        case 'W':
            withdraw(users, userId);
            break;
        default:
            break;
        }
    }    
}

void cancelAccount(vector<user>& users, int userId)
{
    cout << "Password:" << endl;
    string password;
    cin >> password;
    while (!passwordsMatch(users, userId, password))
    {
        cout << "Incorrect password\nPassword:" << endl;
        cin >> password;
    }

    users.erase(users.begin() + userId);
}

void deposit(vector<user>& users, int userId)
{
    cout << "Choose amount to deposit:" << endl;
    double amount;
    cin >> amount;
    while (amount <= 0)
    {
        cout << "Amount must be greater than 0\nChoose amount to deposit:" << endl;
        cin >> amount;
    }

    //Round down to 2 decimal places
    amount = floor(amount * 100.0) / 100.0;
    users[userId].balance += amount;
}

void transfer(vector<user>& users, int userId)
{
    cout << "Choose destination account name:" << endl;
    string destinationName;
    cin >> destinationName;
    while (!usernameExists(users, destinationName))
    {
        cout << "Account doesn't exist\nChoose destination account name:" << endl;
        cin >> destinationName;
    }

    int destinationId = getUserId(users, destinationName);
    while (destinationId == userId)
    {
        cout << "Account must be different from current account\nChoose destination account name:" << endl;
        cin >> destinationName;
        destinationId = getUserId(users, destinationName);
    }

    double userBalance = users[userId].balance;

    cout << "Choose amount to transfer:" << endl;
    double transferAmount;
    cin >> transferAmount;    
    //Round down to 2 decimal places
    transferAmount = floor(transferAmount * 100.0) / 100.0;
    double userBalanceAfterTransfer = userBalance - transferAmount;
    while (transferAmount <= 0 || userBalanceAfterTransfer < -10000)
    {
        cout << "Amount must be greater than 0 and maximum overdraft is 10 000 BGN\nChoose amount to transfer:" << endl;
        cin >> transferAmount;
        //Round down to 2 decimal places
        transferAmount = floor(transferAmount * 100.0) / 100.0;
        userBalanceAfterTransfer = userBalance - transferAmount;
    }

    users[userId].balance = userBalanceAfterTransfer;
    users[destinationId].balance += transferAmount;
}

void withdraw(vector<user>& users, int userId)
{
    double userBalance = users[userId].balance;

    cout << "Choose amount to withdraw:" << endl;
    double withdrawAmount;
    cin >> withdrawAmount;
    //Round down to 2 decimal places
    withdrawAmount = floor(withdrawAmount * 100.0) / 100.0;
    double userBalanceAfterWithdraw = userBalance - withdrawAmount;
    while (withdrawAmount <= 0 || userBalanceAfterWithdraw < -10000)
    {
        cout << "Amount must be greater than 0 and maximum overdraft is 10 000 BGN\nChoose amount to withdraw:" << endl;
        cin >> withdrawAmount;
        //Round down to 2 decimal places
        withdrawAmount = floor(withdrawAmount * 100.0) / 100.0;
        userBalanceAfterWithdraw = userBalance - withdrawAmount;
    }

    users[userId].balance -= withdrawAmount;
}

user getUserFromString(string input)
{
    int length = input.size();

    int index = 0;
    char tmp = input[index];
    string name;
    while (tmp != ':' && index < length)
    {
        name += tmp;
        index++;
        tmp = input[index];
    }

    index++;
    tmp = input[index];
    string hashedPasswordStr;
    while (tmp != ':' && index < length)
    {
        hashedPasswordStr += tmp;
        index++;
        tmp = input[index];
    }
    unsigned long hashedPassword = stoul(hashedPasswordStr);

    index++;
    tmp = input[index];
    string balanceStr;
    while (tmp != ':' && index < length)
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

int getUserId(vector<user>& users, string username)
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

bool passwordsMatch(vector<user>& users, int userId, string passwordGuess)
{
    hash<string> hashString;
    unsigned long hashedPasswordGuess = hashString(passwordGuess);

    if (users[userId].hashedPassword == hashedPasswordGuess)
    {
        return true;
    }
    return false;
}

bool validateUsername(string username)
{
    for (int i = 0; i < username.size(); i++)
    {
        if (!((username[i] >= 'a' && username[i] <= 'z') || (username[i] >= 'A' && username[i] <= 'Z')))
        {
            return false;
        }
    }
    return true;
}

bool usernameExists(vector<user>& users, string username)
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

bool validatePassword(string password)
{
    int length = password.size();
    if (length < 5 || !stringContainsLowercaseLetter(password) || !stringContainsUppercaseLetter(password) || !stringContainsSymbol(password))
    {
        return false;
    }

    
    for (int i = 0; i < password.size(); i++)
    {
        if (!((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z') || (password[i] >= '1' && password[i] <= '9') || (charIsSymbol(password[i]))))
        {
            return false;
        }
    }
    return true;
}

bool stringContainsIllegalCharacters(string input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= '1' && input[i] <= '9') || (charIsSymbol(input[i]))))
        {
            return false;
        }
    }
    return true;
}

bool charIsSymbol(char character)
{
    const vector<char> allowedSymbols = { '!', '@', '#', '$', '%', '^', '&', '*' };
    for (int i = 0; i < allowedSymbols.size(); i++)
    {
        if (character == allowedSymbols[i])
        {
            return true;
        }
    }
    return false;
}

bool stringContainsSymbol(string input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (charIsSymbol(input[i]))
        {
            return true;
        }
    }
    return false;
}

bool stringContainsLowercaseLetter(string input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            return true;
        }
    }
    return false;
}

bool stringContainsUppercaseLetter(string input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            return true;
        }
    }
    return false;
}

char getCommand(vector<char> allowedCharacters)
{
    string input;
    cin >> input;
    char commandChar = toUpperCase(input[0]);
    while (input.size() > 1 || !characterIsAllowed(allowedCharacters, commandChar))
    {
        cout << "Invalid command" << endl;
        cin >> input;
        commandChar = toUpperCase(input[0]);
    }

    return commandChar;
}

bool characterIsAllowed(vector<char> allowedCharacters, char character)
{
    for (int i = 0; i < allowedCharacters.size(); i++)
    {
        if (allowedCharacters[i] == character)
        {
            return true;
        }
    }
    return false;
}

char toUpperCase(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        const int asciiDiffBetweenUpperAndLowerCaseLetter = 32;
        character -= asciiDiffBetweenUpperAndLowerCaseLetter;
    }

    return character;
}