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
* <main project file>
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "UserStruct.h"
#include "HelperFunctions.h"
#include "PrimaryFunctions.h"
using namespace std;

int main()
{
    vector<user> users;

    loadUsersFromDb(users);

    MainMenu(users);

    return 0;
}