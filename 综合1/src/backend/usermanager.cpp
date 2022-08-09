#include "include/backend/usermanager.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMap>
#include <QString>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
namespace Backend {
UserManager::UserManager(string userFile) {
  this->userFile = userFile;
  this->currentUser = NULL;
  this->load();
}
UserManager::~UserManager() {
  this->save();
  for (auto &v : this->userList) {
    delete v.second;
  }
  this->currentUser = NULL;
  this->userList.empty();
}
/*从文件加载所有用户数据*/
void UserManager::load() {
  string strData;
  ifstream s(this->userFile, ios_base::binary);
  if (s.fail()) {
    cout << "打开用户文件失败" << endl;
    return;
  } else {
    stringstream buffer;
    buffer << s.rdbuf();
    strData = buffer.str();
  }
  s.close();
  QJsonParseError jsonError;
  QJsonDocument jsonDoc =
      QJsonDocument::fromJson(QByteArray(strData.c_str()), &jsonError);
  if (jsonError.error != QJsonParseError::NoError) {
    cout << jsonError.errorString().toStdString() << endl;
    return;
  }
  QJsonArray jsonArray = jsonDoc.array();
  for (auto v : jsonArray) {
    QJsonObject obj = v.toObject();
    User *user = NULL;
    if (obj["type"].toString() == "MANAGER") {
      user = (User *)new manager;
    } else {
      user = (User *)new Duser;
    }
    user->username = obj["username"].toString().toStdString();
    user->passwordHash = obj["passwordHash"].toString().toStdString();
    user->balance = obj["balance"].toInt();
    user->name = obj["name"].toString().toStdString();
    user->phoneNum = obj["phoneNum"].toString().toStdString();
    user->address = obj["address"].toString().toStdString();
    this->userList.insert(pair<string, User *>(user->username, user));
  }
}
/*保存用户数据到文件*/
void UserManager::save() {
  cout << "Backend::UserManager::save()" << endl;
  QJsonArray jsonArray;
  for (auto &kv : this->userList) {
    User *v = kv.second;
    QJsonObject obj;
    obj["username"] = QJsonValue(QString::fromStdString(v->username));
    obj["passwordHash"] = QJsonValue(QString::fromStdString(v->passwordHash));
    obj["balance"] = QJsonValue(v->balance);
    obj["name"] = QJsonValue(QString::fromStdString(v->name));
    obj["phoneNum"] = QJsonValue(QString::fromStdString(v->phoneNum));
    obj["address"] = QJsonValue(QString::fromStdString(v->address));
    obj["type"] = QJsonValue(v->getUserType() == MANAGER ? "MANAGER" : "USER");
    jsonArray.push_back(obj);
  }
  string strData = QJsonDocument(jsonArray).toJson().toStdString();
  ofstream s(this->userFile, ios::out);
  if (s.fail()) {
    throw "ERROR saving user data";
  } else {
    s << strData;
  }
  s.close();
}
map<string, User *> UserManager::listUser() const { return this->userList; }
bool UserManager::reg(User *user) {
  User *foundUser = this->_getUserByName(user->username);
  if (foundUser == NULL) {
    User *copyUser = NULL;
    copyUser = new Duser;
    copyUser->username = user->username;
    copyUser->passwordHash = user->passwordHash;
    copyUser->balance = user->balance;
    copyUser->address = user->address;
    copyUser->name = user->name;
    copyUser->phoneNum = user->phoneNum;
    this->userList.insert(pair<string, User *>(user->username, copyUser));
    this->save();
    return true;
  }
  return false;
}
/*返回用户名*/
User *UserManager::getUserByName(string username) const {
  User *foundUser = this->_getUserByName(username);
  if (foundUser == NULL) {
    return NULL;
  }
  User *foundUserBrief = NULL;
  if (foundUser->getUserType() == MANAGER) {
    foundUserBrief = new manager;
  } else {
    foundUserBrief = new Duser;
  }
  foundUserBrief->username = foundUser->username;
  foundUserBrief->passwordHash = "";
  return foundUserBrief;
}
/*返回用户名和密码以及其他数据*/
User *UserManager::_getUserByName(string username) const {
  auto it = this->userList.find(username);
  if (it == this->userList.end()) {
    return NULL;
  } else {
    return it->second;
  }
}
/*登录*/
bool UserManager::login(string username, string password) {
  User *foundUser = this->_getUserByName(username);
  if (foundUser == NULL) {
    return false;
  }
  QByteArray hash = QCryptographicHash::hash(
      QByteArray::fromStdString(password), QCryptographicHash::Sha256);
  string phash = hash.toHex().toStdString();
  if (phash != foundUser->passwordHash) {
    return false;
  }
  this->currentUser = foundUser;
  return true;
}
/*退出登录*/
bool UserManager::logout() {
  this->currentUser = NULL;
  return true;
}
/*增加余额*/
bool UserManager::addFund(int amount) {
  if (!this->currentUser) {
    throw "请先登录";
  }
  this->currentUser->balance += amount;
  this->save();
  return true;
}
/*修改密码*/
bool UserManager::changePassword(string password) {
  this->currentUser->setPassword(password);
  this->save();
  return true;
}

}; // namespace Backend
