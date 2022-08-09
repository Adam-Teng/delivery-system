#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>
using namespace std;

class Delivery {
public:
  Delivery();
  string id;          //快递编号
  string name;        //快递名称
  string sendUser;    //寄件用户
  string receiveUser; //收件用户
  string sendTime;    //寄件时间
  string receiveTime; //收件时间
  string description; //描述
  int state;          //快递状态，0为待签收，1为已签收
};

#endif
