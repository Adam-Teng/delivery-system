#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include <map>
#include "../misc/delivery.h"

using namespace std;

namespace Common
{
	class DeliveryManager
	{
	public:
		virtual ~DeliveryManager()
		{

		};
		virtual map<string, Delivery*> listDelivery() const = 0; //列出快递列表返回map
		virtual Delivery* getDeliveryById(string id) const = 0; //根据id获取快递
		virtual bool addDelivery(Delivery* delivery) = 0; //发送快递
		virtual bool replaceDelivery(Delivery* delivery) = 0; //修改快递
		virtual int getPrice(const Delivery* delivery) const = 0; //获取快递价格
	};
}

#endif
