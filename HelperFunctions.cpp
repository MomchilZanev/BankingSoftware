#include "HelperFunctions.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool validateUsername(const string& username)
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

bool validatePassword(const string& password)
{
    const int passwordMinLength = 5;
    int length = password.size();
    if (length < passwordMinLength || !stringContainsLowercaseLetter(password) || !stringContainsUppercaseLetter(password) || !stringContainsSymbol(password) || stringContainsIllegalCharacters(password))
    {
        return false;
    }
    return true;
}

bool stringContainsIllegalCharacters(const string& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= '1' && input[i] <= '9') || (charIsSymbol(input[i]))))
        {
            return true;
        }
    }
    return false;
}

bool charIsSymbol(const char character)
{
    const vector<char> allowedSymbols = { '!', '@', '#', '$', '%', '^', '&', '*' };
    return characterIsAllowed(allowedSymbols, character);
}

bool stringContainsSymbol(const string& input)
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

bool stringContainsLowercaseLetter(const string& input)
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

bool stringContainsUppercaseLetter(const string& input)
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

char getCommand(const vector<char>& allowedCharacters)
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

bool characterIsAllowed(const vector<char>& allowedCharacters, const char character)
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

double getValidDoubleInput()
{
    string number;
    cin >> number;
    while (!isNumeric(number))
    {
        cout << "This is not a valid number, try again" << endl;
        cin >> number;
    }

    return roundDown(stod(number));
}

bool isNumeric(const string& input)
{
    const char decimalPoint = '.';
    const char plusChar = '+';
    const char minusChar = '-';
    for (int i = 0; i < input.size(); i++)
    {
        if (!isdigit(input[i]) && input[i] != decimalPoint && input[i] != plusChar && input[i] != minusChar)
        {
            return false;
        }
    }

    return true;
}

double roundDown(const double number)
{
    //Round down to 2 decimal places
    return floor(number * 100.0) / 100.0;
}