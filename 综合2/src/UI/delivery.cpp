#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

void CLI::sendExpress(User* user, User* manager)
{
	Delivery* dd = NULL;
	system("cls");
	cout << "创建快递";
	cout << "：以下所有输入后都需要回车确认" << endl;

	cout << "请选择快递分类：[b]图书 [f]易碎品 [n]普通快递 [其他]图书 >> ";
	string op;
	cin >> op;
	DELIVERYTYPE dtype = BOOK;
	if (op == "b")
		dtype = BOOK;
	else if (op == "f")
		dtype = FRAGILE;
	else if (op == "n")
		dtype = NORMAL;
	else
		dtype = BOOK;
	switch (dtype)
	{
	case NORMAL:
		dd = new DeliveryNormal;
		break;
	case BOOK:
		dd = new DeliveryBook;
		break;
	case FRAGILE:
		dd = new DeliveryFragile;
		break;
	}
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
		delete dd;
		return;
	}
	cout << "寄件时间 >> ";
	dd->sendTime = "";
	this->editorInput(&dd->sendTime);
	dd->receiveTime = "0";
	dd->courier = "";
	cout << "描述 >> ";
	dd->description = "";
	this->editorInput(&dd->description);
	dd->state = 0;
	cout << "快递重量，若快递类型为图书则单位为本，其他类型单位为kg >>";
	string snum = "";
	this->editorInput(&snum);
	try
	{
		int sprice;
		int num = stoi(snum);
		switch (dtype)
		{
		case NORMAL:
			sprice = num * 500;
			break;
		case BOOK:
			sprice = num * 200;
			break;
		case FRAGILE:
			sprice = num * 800;
			break;
		}
		dd->pricing = sprice / 100;
	}
	catch (const std::exception&)
	{
		cout << "输入有误，当前值已设置为8";
		dd->pricing = 800;
	}
	if (user->balance < dd->pricing)
	{
		this->alert("余额不足");
		delete dd;
		return;
	}
	else
	{
		user->balance -= (dd->pricing * 100);
		manager->balance += (dd->pricing * 100);
	}
	cout << endl;
	this->print(dd);
	cout << "确认发送？(y/n) >> ";
	while (1)
	{
		char op = _getch();
		if (op == 'y')
		{
			if (Data::getInstance()->delivery->addDelivery(dd))
			{
				this->alert("添加成功");
			}
			else
			{
				this->alert("系统错误");
			}
			delete dd;
			return;
		}
		else if (op == 'n')
		{
			cout << endl;
			user->balance -= (dd->pricing * 100);
			manager->balance += (dd->pricing * 100);
			delete dd;
			return;
		}
		else
		{
			this->alert("系统错误");
		}
	}
}

void CLI::takeExpress(User* user)
{
	bool loop = true;
	while (loop)
	{
		system("cls");
		int i = 1;
		map<int, Delivery*> displayDeliveryMap;
		auto m = Data::getInstance()->delivery->listDelivery();
		for (auto& v : m)
		{
			Delivery* d = v.second;
			if (d->state == 0)
			{
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
			else
			{
				continue;
			}
		}
		if (i == 1)
		{
			cout << "当前条件下暂无快递，您可以[q]返回上级，并回车确认 >> ";
		}
		else
		{
			cout << "请输入编号选择商品，或[q]返回上级，并回车确认 >> ";
		}
		string op;
		cin >> op;
		try
		{
			int seq = stoi(op);
			map<int, Delivery*>::iterator it = displayDeliveryMap.find(seq);
			if (it == displayDeliveryMap.end())
			{
				this->alert("错误输入");
			}
			else
			{
				this->show(user, it->second);
			}
		}
		catch (...)
		{
			if (op == "q")
			{
				return;
			}
			else
			{
				this->alert("错误输入");
			}
		}
	}
}

void CLI::receiveExpress(User* user)
{
	bool loop = true;
	string temp = user->username;
	string search = "";
	while (loop)
	{
		system("cls");
		int i = 1;
		map<int, Delivery*> displayDeliveryMap;
		auto m = Data::getInstance()->delivery->listDelivery();
		for (auto& v : m)
		{
			Delivery* d = v.second;
			if (d->receiveUser.find(temp) == string::npos ||
				(search.length() > 0 && (
					d->receiveTime.find(search) == string::npos &&
					d->sendTime.find(search) == string::npos &&
					d->id.find(search) == string::npos
					)))
			{
				continue;
			}
			if (d->state == 2)
			{
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
			else
			{
				continue;
			}
		}
		if (i == 1)
		{
			cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
		}
		else
		{
			cout << "请输入编号选择商品，或[q]返回上级，[s]搜索， 并回车确认 >> ";
		}
		string op;
		cin >> op;
		try
		{
			int seq = stoi(op);
			map<int, Delivery*>::iterator it = displayDeliveryMap.find(seq);
			if (it == displayDeliveryMap.end())
			{
				this->alert("错误输入");
			}
			else
			{
				this->show(user, it->second);
			}
		}
		catch (...)
		{
			if (op == "q")
			{
				return;
			}
			else if (op == "s")
			{
				cout << "请输入搜索关键字，并回车确认 >> ";
				this->editorInput(&search);
			}
			else
			{
				this->alert("错误输入");
			}
		}
	}
}

void CLI::getExpress(User* user)
{
	bool loop = true;
	string temp = user->username;
	string search = "";
	while (loop)
	{
		system("cls");
		int i = 1;
		map<int, Delivery*> displayDeliveryMap;
		auto m = Data::getInstance()->delivery->listDelivery();
		for (auto& v : m)
		{
			Delivery* d = v.second;
			if (d->courier.find(temp) == string::npos ||
				(search.length() > 0 && (
					d->receiveTime.find(search) == string::npos &&
					d->sendTime.find(search) == string::npos &&
					d->id.find(search) == string::npos
					)))
			{
				continue;
			}
			if (d->state == 0)
			{
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
			else
			{
				continue;
			}
		}
		if (i == 1)
		{
			cout << "当前条件下暂无快递，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
		}
		else
		{
			cout << "请输入编号选择商品，或[q]返回上级，[s]搜索， 并回车确认 >> ";
		}
		string op;
		cin >> op;
		try
		{
			int seq = stoi(op);
			map<int, Delivery*>::iterator it = displayDeliveryMap.find(seq);
			if (it == displayDeliveryMap.end())
			{
				this->alert("错误输入");
			}
			else
			{
				this->show(user, it->second);
			}
		}
		catch (...)
		{
			if (op == "q")
			{
				return;
			}
			else if (op == "s")
			{
				cout << "请输入搜索关键字，并回车确认 >> ";
				this->editorInput(&search);
			}
			else
			{
				this->alert("错误输入");
			}
		}
	}
}


void CLI::show(User* user, Delivery* d)
{
	Delivery* dd = Data::getInstance()->delivery->getDeliveryById(d->id);
	User* currentUser = Data::getInstance()->user->currentUser;
	while (1)
	{
		system("cls");
		cout << dd->name << endl;
		cout << "状态：";
		this->printState(dd);
		this->changeState(user, dd);
		delete(dd);
		return;
	}
}

void CLI::changeState(User* user, Delivery* d)
{
	Delivery* dd = NULL;
	switch (d->getDeliveryType())
	{
	case NORMAL:
		dd = new DeliveryNormal;
		break;
	case BOOK:
		dd = new DeliveryBook;
		break;
	case FRAGILE:
		dd = new DeliveryFragile;
		break;
	}
	dd->id = d->id;
	dd->name = d->name;
	dd->description = d->description;
	dd->receiveUser = d->receiveUser;
	dd->sendTime = d->sendTime;
	dd->sendUser = d->sendUser;
	dd->pricing = d->pricing;
	dd->courier = d->courier;
	if (user->getUserType() == MANAGER)
	{
		cout << "请输入快递员账户名" << endl;
		this->editorInput(&dd->courier);
		auto m = Data::getInstance()->user->listUser();
		User* crer = new Cuser;
		int flag = 0;
		for (auto& kv : m) {
			if (kv.first == dd->courier) {
				crer = (Cuser*)Data::getInstance()->user->getUserByName(kv.second->username);
				flag = 1;
			}
		}
		if (flag == 0) {
			this->alert("没有该用户");
			delete crer;
			delete dd;
			return;
		}
		dd->receiveTime = d->receiveTime;
		dd->state = d->state;
		int price = dd->pricing / 2;
		if (user->balance < price)
		{
			this->alert("余额不足");
			return;
		}
		else
		{
			user->balance -= (price * 100);
			crer->balance += (price * 100);
		}
		delete crer;
	}
	else if (user->getUserType() == COURIER)
	{
		dd->receiveTime = d->receiveTime;
		dd->state = 2;
	}
	else
	{
		dd->receiveTime = "";
		cout << "签收时间 >>";
		this->editorInput(&dd->receiveTime);
		dd->state = 1;
	}
	if (Data::getInstance()->delivery->replaceDelivery(dd))
	{
		this->alert("修改成功");
	}
	else
	{
		this->alert("系统错误");
	}
	delete dd;
	return;
}

