#ifndef C_DELIVERYMANAGER_H
#define C_DELIVERYMANAGER_H

#include "../common/deliverymanager.h"
#include "../misc/deliverybook.h"
#include "../misc/deliveryfragile.h"
#include "../misc/deliverynormal.h"
#include "httpclient.h"
#include <map>
#include <iostream>
#include <string>

using namespace std;

namespace Client {
	class DeliveryManager : Common::DeliveryManager {
	public:
		DeliveryManager(xHttpClient* client);
		~DeliveryManager();
		map<string, Delivery*> listDelivery() const;
		Delivery* getDeliveryById(string id) const;
		bool addDelivery(Delivery* delivery);
		bool replaceDelivery(Delivery* delivery);
		int getPrice(const Delivery* delivery) const;
		void load();
	private:
		xHttpClient* client;
		map<string, Delivery*> deliveryList;
		Delivery* copy(Delivery* delivery) const;
		Delivery* _getDeliveryById(string id) const;
	};
};

#endif
