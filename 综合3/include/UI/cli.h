#pragma once

#include "../misc/user.h"
#include "../misc/delivery.h"
#include <conio.h>
#include <iostream>

using namespace std;

class CLI
{
public:
	CLI();
	string passwordInput();

private:
	bool mainMenu();
	/*Users*/
	void reg();
	void login();
	void passwd();
	void recharge();
	void addCourier();
	void deleteCourier();
	void deleteUser(User* user);
	/*utils*/
	void alert(string s);
	void editorInput(string* s);
	string to_string(const double val, const int n = 2);
	/*list*/
	void listUser();
	void listExpress();
	void listSendExpress(User* user);
	void listReceiveExpress(User* user);
	void listSendCourierExpress(User* user);
	void listHaveSCourierExpress(User* user);
	/*delivery*/
	void sendExpress(User* user, User* manager);
	void receiveExpress(User* user);
	void takeExpress(User* user);
	void getExpress(User* user);
	void show(User* user, Delivery* d);
	void changeState(User* user, Delivery* d);
	/*print*/
	void print(Delivery* d);
	void printState(Delivery* d);
};