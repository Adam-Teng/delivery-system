#include "include/backend/deliverymanager.h"
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

namespace Backend
{
	DeliveryManager::DeliveryManager(string deliveryFile)
	{
		this->deliveryFile = deliveryFile;
		this->load();
	}
	DeliveryManager::~DeliveryManager()
	{
		this->save();
		for (auto& v : this->deliveryList)
		{
			delete v.second;
		}
		this->deliveryList.empty();
	}
	/*加载所有快递*/
	void DeliveryManager::load()
	{
		string strData;
		ifstream s(this->deliveryFile, ios_base::binary);
		if (s.fail())
		{
			cout << "打开文件错误" << endl;
			return;
		}
		else
		{
			stringstream buffer;
			buffer << s.rdbuf();
			strData = buffer.str();
		}
		s.close();
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(strData), &jsonError);
		if (QJsonParseError::NoError != jsonError.error)
		{
			cout << jsonError.errorString().toStdString() << endl;
			return;
		}
		QJsonArray jsonArray = jsonDoc.array();
		for (auto& v : jsonArray)
		{
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
			this->deliveryList.insert(pair<string, Delivery*>(delivery->id, delivery));
		}
	}
	void DeliveryManager::save()
	{
		cout << "Backend::DeliveryManager::save()" << endl;
		QJsonArray jsonArray;
		for (auto& kv : this->deliveryList)
		{
			Delivery* v = kv.second;
			QJsonObject obj;
			obj["id"] = QJsonValue(QString::fromStdString(v->id));
			obj["name"] = QJsonValue(QString::fromStdString(v->name));
			obj["sendUser"] = QJsonValue(QString::fromStdString(v->sendUser));
			obj["receiveUser"] = QJsonValue(QString::fromStdString(v->receiveUser));
			obj["sendTime"] = QJsonValue(QString::fromStdString(v->sendTime));
			obj["receiveTime"] = QJsonValue(QString::fromStdString(v->receiveTime));
			obj["description"] = QJsonValue(QString::fromStdString(v->description));
			obj["courier"] = QJsonValue(QString::fromStdString(v->courier));
			obj["pricing"] = QJsonValue(v->pricing);
			obj["state"] = QJsonValue(v->state);
			jsonArray.push_back(obj);
		}
		string strData = QJsonDocument(jsonArray).toJson().toStdString();
		ofstream s(this->deliveryFile, ios::out);
		if (s.fail())
		{
			cout << "Error saving user data" << endl;
			throw "Error saving user data";
		}
		else
		{
			s << strData;
		}
		s.close();
	}

	map<string, Delivery*>DeliveryManager::listDelivery() const
	{
		return this->deliveryList;
	}
	/*快递价格*/
	int DeliveryManager::getPrice(const Delivery* delivery) const
	{
		return delivery->pricing;
	}
	/*通过id获取快递*/
	Delivery* DeliveryManager::_getDeliveryById(string id) const
	{
		auto it = this->deliveryList.find(id);
		if (it == this->deliveryList.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}
	/*通过id获取快递（复制后的对象）*/
	Delivery* DeliveryManager::getDeliveryById(string id) const
	{
		auto it = this->deliveryList.find(id);
		if (it == this->deliveryList.end())
		{
			return NULL;
		}
		else
		{
			return this->copy(it->second);
		}
	}
	/*添加快递*/
	bool DeliveryManager::addDelivery(Delivery* delivery)
	{
		User* currentUser = (Data::getInstance())->user->currentUser;
		if (currentUser == NULL || currentUser->getUserType() != USER)
		{
			return false;
		}
		delivery->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
		delivery->sendUser = currentUser->username;
		return this->addOrReplace(delivery);
	}
	/*修改快递状态*/
	bool DeliveryManager::replaceDelivery(Delivery* delivery)
	{
		User* currentUser = (Data::getInstance())->user->currentUser;
		if (currentUser == NULL)
		{
			return false;
		}
		Delivery* originDelivery = this->getDeliveryById(delivery->id);
		if (originDelivery == NULL)
		{
			return false;
		}
		delivery->receiveUser = originDelivery->receiveUser;
		delete originDelivery;
		return this->addOrReplace(delivery);
	}
	/*复制*/
	Delivery* DeliveryManager::copy(Delivery* delivery) const
	{
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
	bool DeliveryManager::addOrReplace(Delivery* delivery)
	{
		Delivery* copyDelivery = this->copy(delivery);
		Delivery* originDelivery = this->_getDeliveryById(copyDelivery->id);
		if (originDelivery != NULL)
		{
			delete originDelivery;
		}
		this->deliveryList.insert_or_assign(copyDelivery->id, copyDelivery);
		this->save();
		return true;
	}
}