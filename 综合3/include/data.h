#ifndef DATA_H__
#define DATA_H__

#include <iostream>
#include <memory>
#include "backend/usermanager.h"
#include "client/usermanager.h"
#include "common/usermanager.h"
#include "backend/deliverymanager.h"
#include "client/deliverymanager.h"
#include "common/deliverymanager.h"

using namespace std;
using namespace Common;

typedef enum DATATYPE
{
	UNDEFINED,
	STAND,
	ONLINE
} DATATYPE;

class Data
{
private:
	Data() {
		this->user = NULL;
		this->delivery = NULL;
	};
	Data(const Data&) {};
	Data& operator=(const Data&) {};
	~Data()
	{
		if (this->user) delete this->user;
		if (this->delivery) delete this->delivery;
	}
	static void destroy(Data* p)
	{
		delete p;
	}
	static shared_ptr<Data> instance;
	xHttpClient client;

public:
	DATATYPE type = UNDEFINED;
	UserManager* user;
	DeliveryManager* delivery;
	void* conn;
	static shared_ptr<Data> getInstance()
	{
		return instance;
	}
	/*初始化为独立模式或服务端*/
	bool init(string userFile, string deliveryFile)
	{
		if (type != UNDEFINED)
		{
			return false;
		}
		this->user = (UserManager*)new Backend::UserManager(userFile);
		this->delivery = (DeliveryManager*)new Backend::DeliveryManager(deliveryFile);
		return true;
	}
	/*初始化为联网模式客户端*/
	bool init(string serverAddr) {
		if (type != UNDEFINED)
		{
			return false;
		}
		this->client.base = serverAddr;
		this->user = (UserManager*)new Client::UserManager(&this->client);
		this->delivery = (DeliveryManager*)new Client::DeliveryManager(&this->client);
		return true;
	}
};

#endif