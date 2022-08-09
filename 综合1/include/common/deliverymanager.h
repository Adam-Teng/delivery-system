#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include "../misc/delivery.h"
#include <map>

using namespace std;

namespace Common {
class DeliveryManager {
public:
  virtual ~DeliveryManager(){

  };
  virtual map<string, Delivery *>
  listDelivery() const = 0; //列出快递列表返回map
  virtual Delivery *getDeliveryById(string id) const = 0; //根据id获取快递
  virtual bool addDelivery(Delivery *delivery) = 0;       //发送快递
  virtual bool replaceDelivery(Delivery *delivery) = 0;   //修改快递
};
} // namespace Common

#endif
