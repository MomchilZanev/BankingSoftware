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

void Register(vector<user>& users);
void Quit(vector<user>& users);

bool usernameIsTaken(vector<user>& users, string username);
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
        case 'R':
            Register(users);
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

void Register(vector<user>& users)
{
    cout << "Choose Username:" << endl;
    string username;
    cin >> username;
    while (!validateUsername(username))
    {
        cout << "Username must contain only latin letters, choose another Username:" << endl;
        cin >> username;
    }
    while (usernameIsTaken(users, username))
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

bool usernameIsTaken(vector<user>& users, string username)
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