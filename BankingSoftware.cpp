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
        char command;
        cin >> command;
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
    user currentUser = users[userId];
    cout << "You have " << currentUser.balance << " BGN. Choose one of the following options:" << endl;
    cout << "C - Cancel account\nD - Deposit\nL - Logout\nT - Transfer\nW - Withdraw" << endl;

    char command;
    cin >> command;
    switch (command)
    {
    case 'C':
        cancelAccount(users, userId);
        break;
    default:
        break;
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