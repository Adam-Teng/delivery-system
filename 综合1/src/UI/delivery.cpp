#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

void CLI::sendExpress(User* user, User* manager) {
	Delivery* dd = NULL;
	system("cls");
	if (user->balance < 15) {
		this->alert("余额不足");
		delete dd;
		return;
	}
	else {
		user->balance -= 1500;
		manager->balance += 1500;
	}
	cout << "创建快递";
	cout << "：以下所有输入后都需要回车确认" << endl;
	dd = new Delivery;
	cout << "快递名称 >> ";
	dd->name = "";
	this->editorInput(&dd->name);
	cout << "收件人 >> ";
	dd->receiveUser = "";
	this->editorInput(&dd->receiveUser);
	auto m = Data::getInstance()->user->listUser();
	int flag = 0;
	for (auto& kv : m) {
		if (kv.first == dd->receiveUser) {
			flag = 1;
		}
	}
	if (flag == 0) {
		this->alert("没有该用户");
		user->balance += 1500;
		manager->balance -= 1500;
		delete dd;
		return;
	}
	cout << "寄件时间 >> ";
	dd->sendTime = "";
	this->editorInput(&dd->sendTime);
	dd->receiveTime = "0";
	cout << "描述 >> ";
	dd->description = "";
	this->editorInput(&dd->description);
	dd->state = 0;
	cout << endl;
	this->print(dd);
	cout << "确认发送？(y/n) >> ";
	while (1) {
		char op = _getch();
		if (op == 'y') {
			if (Data::getInstance()->delivery->addDelivery(dd)) {
				this->alert("添加成功");
			}
			else {
				this->alert("系统错误");
			}
			delete dd;
			return;
		}
		else if (op == 'n') {
			cout << endl;
			user->balance += 1500;
			manager->balance -= 1500;
			delete dd;
			return;
		}
		else {
			this->alert("系统错误");
		}
	}
}
void CLI::receiveExpress(User* user) {
	bool loop = true;
	string temp = user->username;
	string search = "";
	while (loop) {
		system("cls");
		int i = 1;
		map<int, Delivery*> displayDeliveryMap;
		auto m = Data::getInstance()->delivery->listDelivery();
		for (auto& v : m) {
			Delivery* d = v.second;
			if (d->receiveUser.find(temp) == string::npos ||
				(search.length() > 0 &&
					(d->receiveTime.find(search) == string::npos &&
						d->sendTime.find(search) == string::npos &&
						d->id.find(search) == string::npos))) {
				continue;
			}
			if (d->state == 0) {
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
			else {
				continue;
			}
		}
		if (i == 1) {
			cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
		}
		else {
			cout << "请输入编号选择商品，或[q]返回上级，[s]搜索， 并回车确认 >> ";
		}
		string op;
		cin >> op;
		try {
			int seq = stoi(op);
			map<int, Delivery*>::iterator it = displayDeliveryMap.find(seq);
			if (it == displayDeliveryMap.end()) {
				this->alert("错误输入");
			}
			else {
				this->show(it->second);
			}
		}
		catch (...) {
			if (op == "q") {
				return;
			}
			else if (op == "s") {
				cout << "请输入搜索关键字，并回车确认 >> ";
				this->editorInput(&search);
			}
			else {
				this->alert("错误输入");
			}
		}
	}
}

void CLI::show(Delivery* d) {
	Delivery* dd = Data::getInstance()->delivery->getDeliveryById(d->id);
	User* currentUser = Data::getInstance()->user->currentUser;
	while (1) {
		system("cls");
		cout << dd->name << endl;
		cout << "状态：";
		this->printState(dd);
		this->changeState(dd);
		delete (dd);
		return;
	}
}

void CLI::changeState(Delivery* d) {
	Delivery* dd = new Delivery;
	dd->id = d->id;
	dd->name = d->name;
	dd->description = d->description;
	dd->receiveTime = "";
	cout << "签收时间 >>";
	this->editorInput(&dd->receiveTime);
	dd->receiveUser = d->receiveUser;
	dd->sendTime = d->sendTime;
	dd->sendUser = d->sendUser;
	dd->state = 1;
	if (Data::getInstance()->delivery->replaceDelivery(dd)) {
		this->alert("修改成功");
	}
	else {
		this->alert("系统错误");
	}
	delete dd;
	return;
}
