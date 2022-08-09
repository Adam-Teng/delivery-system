#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>
using namespace std;

typedef enum DELIVERYTYPE
{
	NORMAL,//普通快递
	BOOK,//图书
	FRAGILE //易碎品
} DELIVERYTYPE;

class Delivery
{
public:
	Delivery();
	string id;//快递编号
	string name; //快递名称
	string sendUser;//寄件用户
	string receiveUser;//收件用户
	string sendTime;//寄件时间
	string receiveTime;//收件时间
	string description;//描述
	string courier; //负责派送的快递员用户名
	int pricing; //快递价格
	int state;//快递状态，0为待签收，1为已签收
	virtual DELIVERYTYPE getDeliveryType() const = 0;
	virtual int getPrice() const;
};

#endif
