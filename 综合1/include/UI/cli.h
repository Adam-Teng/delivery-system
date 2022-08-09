#pragma once

#include <conio.h>

#include <iostream>

#include "../misc/delivery.h"
#include "../misc/user.h"

using namespace std;

class CLI {
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
	/*utils*/
	void alert(string s);
	void editorInput(string* s);
	string to_string(const double val, const int n = 2);
	/*list*/
	void listUser();
	void listExpress();
	void listSendExpress(User* user);
	void listReceiveExpress(User* user);
	/*delivery*/
	void sendExpress(User* user, User* manager);
	void receiveExpress(User* user);
	void show(Delivery* d);
	void changeState(Delivery* d);
	/*print*/
	void print(Delivery* d);
	void printState(Delivery* d);
};
