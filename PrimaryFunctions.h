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
* <header file for primary functions file>
*
*/

#include "HelperFunctions.h"
#include "UserStruct.h"
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

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