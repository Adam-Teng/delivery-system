#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

using namespace std;

void CLI::reg() {
  User *user = new Duser;
  cout << "账号 >> ";
  cin >> user->username;
  cout << "姓名 >> ";
  cin >> user->name;
  cout << "密码 >> ";
  string password = this->passwordInput();
  user->setPassword(password);
  cout << "电话号码 >>";
  cin >> user->phoneNum;
  cout << "地址 >> ";
  cin >> user->address;
  if (Data::getInstance()->user->reg(user)) {
    this->alert("注册成功");
  } else {
    this->alert("系统错误");
  }
  delete user;
}

void CLI::login() {
  string username;
  cout << "账号 >> ";
  cin >> username;
  cout << "密码 >> ";
  string password = this->passwordInput();
  if (Data::getInstance()->user->login(username, password)) {
    this->alert("登录成功");
  } else {
    this->alert("登录失败，用户名或密码错误");
  }
}

void CLI::passwd() {
  cout << "请输入新密码 >> ";
  string password = this->passwordInput();
  if (Data::getInstance()->user->changePassword(password)) {
    this->alert("修改成功");
  } else {
    this->alert("系统错误");
  }
}

void CLI::recharge() {
  cout << "请输入充值金额，如：100.00 >> ";
  float f;
  cin >> f;
  int i = (int)(f * 100);
  if (Data::getInstance()->user->addFund(i)) {
    this->alert("充值成功");
  } else {
    this->alert("系统错误");
  }
}
