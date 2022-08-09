#include "include/client/usermanager.h"
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
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
namespace Client {
	UserManager::UserManager(xHttpClient* client)
	{
		this->client = client;
		this->currentUser = NULL;
		this->load();
	}
	UserManager::~UserManager()
	{
		for (auto& v : this->userList)
		{
			delete v.second;
		}
		this->userList.empty();
		if (this->currentUser == NULL)
		{
			delete this->currentUser;
		}
	}
	void UserManager::load() {
		this->client->request.method = HTTP_GET;
		this->client->request.url = "/user";
		if (this->client->sendRequest() > 0)
		{
			cout << "GetUserList failed." << endl;
			exit(2);
		}
		if (this->client->response.code != 200)
		{
			cout << "GetUserList failed with code" << this->client->response.code << "." << endl;
			exit(2);
		}
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->client->response.body), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			cout << jsonError.errorString().toStdString() << endl;
			return;
		}
		QJsonArray jsonArray = jsonDoc.array();
		for (auto v : jsonArray)
		{
			QJsonObject obj = v.toObject();
			User* user = NULL;
			if (obj["type"].toString() == "MANAGER")
			{
				user = (User*)new manager;
			}
			else if (obj["type"].toString() == "COURIER")
			{
				user = (User*)new Cuser;
			}
			else
			{
				user = (User*)new Duser;
			}
			user->username = obj["username"].toString().toStdString();
			if (this->_getUserByName(user->username))
			{
				delete this->_getUserByName(user->username);
			}
			this->userList.insert_or_assign(user->username, user);
		}


	}
	/*登录*/
	bool UserManager::login(string username, string password) {
		QJsonObject obj;
		obj["username"] = QString::fromStdString(username);
		obj["password"] = QString::fromStdString(password);
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_POST;
		this->client->request.url = "/session";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return false;
		}
		if (this->client->response.code != 200) {
			return false;
		}
		QJsonDocument v = QJsonDocument::fromJson(QByteArray::fromStdString(this->client->response.body));
		obj = v.object();
		this->client->request.headers.insert_or_assign("Authorization", "Bearer " + obj["token"].toString().toStdString());
		this->loadCurrentUser();
		return true;
	}
	void UserManager::loadCurrentUser() {
		if (this->currentUser != NULL) {
			delete this->currentUser;
			this->currentUser = NULL;
		}
		this->client->request.method = HTTP_GET;
		this->client->request.url = "/session";
		this->client->request.body = "";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return;
		}
		if (this->client->response.code != 200) {
			if (this->currentUser != NULL) {
				delete this->currentUser;
			}
			this->currentUser = NULL;
			return;
		}
		QJsonDocument v = QJsonDocument::fromJson(QByteArray::fromStdString(this->client->response.body));
		QJsonObject obj = v.object();
		User* user = NULL;
		if (obj["type"].toString() == "USER")
		{
			user = (User*)new Duser;
		}
		else if (obj["type"].toString() == "COURIER")
		{
			user = (User*)new Cuser;
		}
		else
		{
			user = (User*)new manager;
		}
		user->username = obj["username"].toString().toStdString();
		user->passwordHash = obj["passwordHash"].toString().toStdString();
		user->balance = obj["balance"].toInt();
		user->name = obj["name"].toString().toStdString();
		user->phoneNum = obj["phoneNum"].toString().toStdString();
		user->address = obj["address"].toString().toStdString();
		this->currentUser = user;
		return;
	}
	//登出
	bool UserManager::logout() {
		this->client->request.headers.erase("Authorization");
		return true;
	}
	bool UserManager::addFund(int amount) {
		QJsonObject obj;
		obj["balance"] = amount;
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_PATCH;
		this->client->request.url = "/session";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return false;
		}
		if (this->client->response.code != 200) {
			return false;
		}
		this->loadCurrentUser();
		return true;
	}
	User* UserManager::getUserByName(string username) const {
		User* foundUser = this->_getUserByName(username);
		if (foundUser == NULL) {
			return NULL;
		}
		User* foundUserBrief = NULL;
		if (foundUser->getUserType() == MANAGER)
		{
			foundUserBrief = new manager;
		}
		else if (foundUser->getUserType() == COURIER)
		{
			foundUserBrief = new Cuser;
		}
		else
		{
			foundUserBrief = new Duser;
		}
		foundUserBrief->username = foundUser->username;
		foundUserBrief->passwordHash = "";
		return foundUserBrief;
	}

	/*
	 * 通过用户名获取用户信息 - 私有
	 */
	User* UserManager::_getUserByName(string username) const {
		auto it = this->userList.find(username);
		if (it == this->userList.end()) {
			return NULL;
		}
		else {
			return it->second;
		}
	}
	map<string, User*>UserManager::listUser() const
	{
		return this->userList;
	}
	bool UserManager::reg(User* user) {
		QJsonObject obj;
		obj["username"] = QString::fromStdString(user->username);
		obj["passwordHash"] = QString::fromStdString(user->passwordHash);
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_POST;
		this->client->request.url = "/user";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return false;
		}
		if (this->client->response.code != 200) {
			return false;
		}
		this->load();
		return true;
	}
	bool UserManager::changePassword(string password) {
		QJsonObject obj;
		obj["password"] = QString::fromStdString(password);
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_PATCH;
		this->client->request.url = "/session";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return false;
		}
		if (this->client->response.code != 200) {
			return false;
		}
		return true;
	}
	bool UserManager::del(User* user)
	{
		this->userList.erase(user->username);
		return true;
	}
}

