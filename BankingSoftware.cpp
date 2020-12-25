#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
struct user
{
    string name;
    string hashedPassword;
    double balance;
};

user getUserFromString(string input);

int main()
{
    fstream userDb;
    userDb.open("users.txt", fstream::in | fstream::out | fstream::app);

    vector<user> users;    

    string currentLine;
    while (getline(userDb, currentLine))
    {
        user currentUser = getUserFromString(currentLine);
        users.push_back(currentUser);
    }

    userDb.close();
    return 0;
}

user getUserFromString(string input)
{
    int length = input.size();
    int index = 0;
    char tmp = input[index];
    string username;
    while (tmp != ':' && index < length)
    {
        username += tmp;
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

    //save only up to 2 decimal places
    double balance = ceil(stod(balanceStr) * 100.0) / 100.0;

    user currentUser = {
        username = username,
        hashedPassword = hashedPassword,
        balance = balance
    };

    return currentUser;
}