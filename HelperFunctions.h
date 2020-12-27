#include <string>
#include <vector>
using namespace std;

bool validateUsername(const string& username);
bool validatePassword(const string& password);

bool charIsSymbol(const char character);
bool stringContainsSymbol(const string& input);
bool stringContainsUppercaseLetter(const string& input);
bool stringContainsLowercaseLetter(const string& input);
bool stringContainsIllegalCharacters(const string& input);
bool characterIsAllowed(const vector<char>& allowedCharacters, const char character);
char toUpperCase(char character);

char getCommand(const vector<char>& allowedCharacters);
double getValidDoubleInput();

bool isNumeric(const string& input);
double roundDown(const double number);