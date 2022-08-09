#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "../misc/user.h"
#include <iostream>
#include <map>

using namespace std;
namespace Common {
class UserManager {
public:
  virtual ~UserManager(){};
  virtual bool reg(User *user) = 0;
  virtual bool login(string username, string password) = 0;
  virtual bool logout() = 0;
  virtual bool addFund(int amount) = 0;
  virtual bool changePassword(string Password) = 0;
  virtual User *getUserByName(string username) const = 0;
  virtual map<string, User *> listUser() const = 0; //列出用户列表返回map
  User *currentUser;
};
}; // namespace Common
#endif
