#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

using namespace std;

void CLI::print(Delivery *d) {
  cout << d->name << endl;
  cout << "接收人：" << d->receiveUser << endl;
  cout << "寄件时间" << d->sendTime << endl;
  cout << d->description << endl;
}

void CLI::printState(Delivery *d) {
  if (d->state == 0) {
    cout << "待签收" << endl;
  } else {
    cout << "已签收" << endl;
  }
}
