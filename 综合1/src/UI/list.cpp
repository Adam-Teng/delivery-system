#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

void CLI::listExpress() {
  bool loop = true;
  string search = "";
  while (loop) {
    system("cls");
    int i = 1;
    map<int, Delivery *> displayDeliveryMap;
    auto m = Data::getInstance()->delivery->listDelivery();
    for (auto &v : m) {
      Delivery *d = v.second;
      if (search.length() > 0 && (d->name.find(search) == string::npos &&
                                  d->receiveTime.find(search) == string::npos &&
                                  d->sendTime.find(search) == string::npos &&
                                  d->id.find(search) == string::npos)) {
        continue;
      }
      displayDeliveryMap.insert_or_assign(i, d);
      cout << i << ".\t" << d->name << endl;
      cout << "\t寄件人：" << d->sendUser << endl;
      cout << "\t收件人：" << d->receiveUser << endl;
      cout << "\t寄件时间：" << d->sendTime << endl;
      cout << "\t收件时间：" << d->receiveTime << endl;
      cout << "\t状态：";
      this->printState(d);
      cout << endl;
      i++;
    }
    if (i == 1) {
      cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
    } else {
      cout << "[q]返回上级，[s]搜索， 并回车确认 >> ";
    }
    string op;
    cin >> op;
    if (op == "q") {
      return;
    } else if (op == "s") {
      cout << "请输入搜索关键字，并回车确认 >> ";
      this->editorInput(&search);
    } else {
      this->alert("错误输入");
    }
  }
}
void CLI::listUser() {
  bool loop = true;
  while (loop) {
    system("cls");
    map<int, User *> displayUserMap;
    auto m = Data::getInstance()->user->listUser();
    int i = 1;
    for (auto &v : m) {
      User *d = v.second;
      displayUserMap.insert_or_assign(i, d);
      cout << i << ".\t" << d->name << endl;
      cout << "\t用户名：" << d->username << endl;
      cout << "\t地址：" << d->address << endl;
      cout << "\t电话号码：" << d->phoneNum << endl;
      i++;
    }
    cout << "[q]返回上级，并回车确认 >> ";
    string op;
    cin >> op;
    if (op == "q") {
      return;
    } else {
      this->alert("错误输入");
    }
  }
}
void CLI::listSendExpress(User *user) {
  bool loop = true;
  string temp = user->username;
  string search = "";
  while (loop) {
    system("cls");
    int i = 1;
    map<int, Delivery *> displayDeliveryMap;
    auto m = Data::getInstance()->delivery->listDelivery();
    for (auto &v : m) {
      Delivery *d = v.second;
      if (d->sendUser.find(temp) == string::npos ||
          (search.length() > 0 && d->receiveUser.find(search) == string::npos &&
           d->receiveTime.find(search) == string::npos &&
           d->sendTime.find(search) == string::npos &&
           d->id.find(search) == string::npos)) {
        continue;
      }
      displayDeliveryMap.insert_or_assign(i, d);
      cout << i << ".\t" << d->name << endl;
      cout << "\t寄件人：" << d->sendUser << endl;
      cout << "\t收件人：" << d->receiveUser << endl;
      cout << "\t寄件时间：" << d->sendTime << endl;
      cout << "\t收件时间：" << d->receiveTime << endl;
      cout << "\t状态：";
      this->printState(d);
      cout << endl;
      i++;
    }
    if (i == 1) {
      cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
    } else {
      cout << "[q]返回上级，[s]搜索， 并回车确认 >> ";
    }
    string op;
    cin >> op;
    if (op == "q") {
      return;
    } else if (op == "s") {
      cout << "请输入搜索关键字，并回车确认 >> ";
      this->editorInput(&search);
    } else {
      this->alert("错误输入");
    }
  }
}

void CLI::listReceiveExpress(User *user) {
  string temp = user->username;
  bool loop = true;
  string search = "";
  while (loop) {
    system("cls");
    int i = 1;
    map<int, Delivery *> displayDeliveryMap;
    auto m = Data::getInstance()->delivery->listDelivery();
    for (auto &v : m) {
      Delivery *d = v.second;
      bool a = d->receiveUser.find(temp) == string::npos;
      if ((search.length() > 0 &&
           (d->sendUser.find(search) == string::npos &&
            d->receiveTime.find(search) == string::npos &&
            d->sendTime.find(search) == string::npos &&
            d->id.find(search) == string::npos)) ||
          a) {
        continue;
      }
      displayDeliveryMap.insert_or_assign(i, d);
      cout << i << ".\t" << d->name << endl;
      cout << "\t寄件人：" << d->sendUser << endl;
      cout << "\t收件人：" << d->receiveUser << endl;
      cout << "\t寄件时间：" << d->sendTime << endl;
      cout << "\t收件时间：" << d->receiveTime << endl;
      cout << "\t状态：";
      this->printState(d);
      cout << endl;
      i++;
    }
    if (i == 1) {
      cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
    } else {
      cout << "[q]返回上级，[s]搜索， 并回车确认 >> ";
    }
    string op;
    cin >> op;
    if (op == "q") {
      return;
    } else if (op == "s") {
      cout << "请输入搜索关键字，并回车确认 >> ";
      this->editorInput(&search);
    } else {
      this->alert("错误输入");
    }
  }
}
