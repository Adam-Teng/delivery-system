#include "include/client/deliverymanager.h"
#include "include/data.h"

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
#include <QUuid>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Client {
	DeliveryManager::DeliveryManager(xHttpClient* client)
	{
		this->client = client;
		this->load();
	}
	DeliveryManager::~DeliveryManager()
	{
		for (auto& v : this->deliveryList)
		{
			delete v.second;
		}
		this->deliveryList.empty();
	}
	void DeliveryManager::load() {
		this->client->request.method = HTTP_GET;
		this->client->request.url = "/delivery";
		this->client->request.body = "";
		if (this->client->sendRequest() > 0) {
			cout << "GetDeliveryList failed" << endl;
			exit(2);
		}
		if (this->client->response.code != 200) {
			cout << "GetDeliveryList failed with code " << this->client->response.code << "." << endl;
			exit(2);
		}
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->client->response.body), &jsonError);
		if (QJsonParseError::NoError != jsonError.error) {
			// 格式解析失败
			// 错误时直接return userList就会是空的
			cout << jsonError.errorString().toStdString() << endl;
			return;
		}
		QJsonArray jsonArray = jsonDoc.array();
		for (auto v : jsonArray) {
			QJsonObject obj = v.toObject();
			Delivery* delivery = NULL;
			switch ((DELIVERYTYPE)obj["type"].toInt())
			{
			case BOOK:
				delivery = new DeliveryBook;
				break;
			case NORMAL:
				delivery = new DeliveryNormal;
				break;
			case FRAGILE:
				delivery = new DeliveryFragile;
				break;
			}
			delivery->id = obj["id"].toString().toStdString();
			delivery->name = obj["name"].toString().toStdString();
			delivery->sendUser = obj["sendUser"].toString().toStdString();
			delivery->receiveUser = obj["receiveUser"].toString().toStdString();
			delivery->sendTime = obj["sendTime"].toString().toStdString();
			delivery->receiveTime = obj["receiveTime"].toString().toStdString();
			delivery->description = obj["description"].toString().toStdString();
			delivery->courier = obj["courier"].toString().toStdString();
			delivery->pricing = obj["pricing"].toInt();
			delivery->state = obj["state"].toInt();
			if (this->_getDeliveryById(delivery->id)) {
				delete this->_getDeliveryById(delivery->id);
			}
			this->deliveryList.insert_or_assign(delivery->id, delivery);
		}
	}

	map<string, Delivery*> DeliveryManager::listDelivery() const {
		return this->deliveryList;
	}
	//用户添加快递，在服务端设置快递为自己的
	bool DeliveryManager::addDelivery(Delivery* v) {
		User* currentUser = (Data::getInstance())->user->currentUser;
		if (currentUser == NULL || currentUser->getUserType() != USER) {
			return false;
		}
		QJsonObject obj;
		obj["name"] = QString::fromStdString(v->name);
		obj["sendUser"] = QJsonValue(QString::fromStdString(v->sendUser));
		obj["receiveUser"] = QJsonValue(QString::fromStdString(v->receiveUser));
		obj["sendTime"] = QJsonValue(QString::fromStdString(v->sendTime));
		obj["receiveTime"] = QJsonValue(QString::fromStdString(v->receiveTime));
		obj["description"] = QJsonValue(QString::fromStdString(v->description));
		obj["courier"] = QJsonValue(QString::fromStdString(v->courier));
		obj["pricing"] = QJsonValue(v->pricing);
		obj["state"] = QJsonValue(v->state);
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_POST;
		this->client->request.url = "/delivery";
		if (this->client->sendRequest() > 0) {
			cout << "HTTPError" << endl;
			return false;
		}
		if (this->client->response.code != 201) {
			return false;
		}
		this->load();
		return true;
	}
	//用户修改自己的快递
	bool DeliveryManager::replaceDelivery(Delivery* delivery) {
		User* currentUser = (Data::getInstance())->user->currentUser;
		if (currentUser == NULL) {
			return false;
		}
		Delivery* origDelivery = this->getDeliveryById(delivery->id);
		if (origDelivery == NULL) {
			return false;
		}
		delete origDelivery;
		QJsonObject obj;
		obj["name"] = QJsonValue(QString::fromStdString(delivery->name));
		obj["sendUser"] = QJsonValue(QString::fromStdString(delivery->sendUser));
		obj["receiveUser"] = QJsonValue(QString::fromStdString(delivery->receiveUser));
		obj["sendTime"] = QJsonValue(QString::fromStdString(delivery->sendTime));
		obj["receiveTime"] = QJsonValue(QString::fromStdString(delivery->receiveTime));
		obj["description"] = QJsonValue(QString::fromStdString(delivery->description));
		obj["courier"] = QJsonValue(QString::fromStdString(delivery->courier));
		obj["pricing"] = QJsonValue(delivery->pricing);
		obj["state"] = QJsonValue(delivery->state);
		this->client->request.body = QJsonDocument(obj).toJson();
		this->client->request.method = HTTP_PUT;
		this->client->request.url = "/delivery/" + delivery->id;
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
	//计算快递价格
	int DeliveryManager::getPrice(const Delivery* Delivery) const {
		return Delivery->pricing;
	}
	Delivery* DeliveryManager::_getDeliveryById(string id) const {
		auto it = this->deliveryList.find(id);
		if (it == this->deliveryList.end()) {
			return NULL;
		}
		else {
			return it->second;
		}
	}
	Delivery* DeliveryManager::getDeliveryById(string id) const {
		auto it = this->deliveryList.find(id);
		if (it == this->deliveryList.end()) {
			return NULL;
		}
		else {
			return this->copy(it->second);
		}
	}
	Delivery* DeliveryManager::copy(Delivery* delivery) const {
		Delivery* copyDelivery = NULL;
		switch (delivery->getDeliveryType())
		{
		case BOOK:
			copyDelivery = new DeliveryBook;
			break;
		case FRAGILE:
			copyDelivery = new DeliveryFragile;
			break;
		case NORMAL:
			copyDelivery = new DeliveryNormal;
			break;
		}
		copyDelivery->id = delivery->id;
		copyDelivery->name = delivery->name;
		copyDelivery->sendUser = delivery->sendUser;
		copyDelivery->receiveUser = delivery->receiveUser;
		copyDelivery->sendTime = delivery->sendTime;
		copyDelivery->receiveTime = delivery->receiveTime;
		copyDelivery->description = delivery->description;
		copyDelivery->courier = delivery->courier;
		copyDelivery->pricing = delivery->pricing;
		copyDelivery->state = delivery->state;
		return copyDelivery;
	}
}
