#include "HelperFunctions.h"
#include "UserStruct.h"
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
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
user getUserFromString(const string& input);

int getUserId(vector<user>& users, const string& username);
bool passwordsMatch(vector<user>& users, const int userId, const string& passwordGuess);
bool usernameExists(vector<user>& users, const string& username);