#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
using namespace std;

typedef enum USERTYPE { MANAGER, USER } USERTYPE;

class User {
public:
	User();
	string username;      //用户名
	string passwordHash;//密码
	string name;//姓名
	string phoneNum;//电话
	string address;//地址
	int balance;//账户余额

	virtual USERTYPE getUserType() = 0;
	void setPassword(string password);
};

#endif
