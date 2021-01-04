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
* <main strutcure used in project>
*
*/

#pragma once
#include <string>
using namespace std;

struct user
{
    string name;
    unsigned long hashedPassword;
    double balance;
};