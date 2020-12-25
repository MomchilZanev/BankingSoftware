#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct user
{
    string name;
    string hashedPassword;
    double balance;
};

void quit( vector<user> users);
user getUserFromString(string input);

int main()
{
    fstream userDb;
    userDb.open("users.txt", fstream::in);

    vector<user> users;    

    string currentLine;
    while (getline(userDb, currentLine))
    {
        user currentUser = getUserFromString(currentLine);
        users.push_back(currentUser);
    }

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
        case 'Q':
            userDb.close();
            quit(users);
            return 0;
        default:
            break;
        }
    }

    userDb.close();
    return 0;
}

void quit(vector<user> users)
{
    fstream userDb;
    userDb.open("users.txt", fstream::out);

    for (int i = 0; i < users.size(); i++)
    {
        user currentUser = users[i];
        userDb << fixed <<setprecision(2) << currentUser.name << ':' << currentUser.hashedPassword << ':' << currentUser.balance << endl;
    }

    userDb.close();
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
    string hashedPassword;
    while (tmp != ':' && index < length)
    {
        hashedPassword += tmp;
        index++;
        tmp = input[index];
    }

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