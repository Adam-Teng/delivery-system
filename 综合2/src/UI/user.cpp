#include "include/UI/cli.h"
#include "include/data.h"
#include <codecvt>
#include <locale>
#include <map>
#include <sstream>

using namespace std;

void CLI::reg()
{
	User* user = new Duser;
	cout << "账号 >> ";
	cin >> user->username;
	cout << "姓名 >> ";
	cin >> user->name;
	cout << "密码 >> ";
	string password = this->passwordInput();
	user->setPassword(password);
	cout << "电话号码 >>";
	cin >> user->phoneNum;
	cout << "地址 >> ";
	cin >> user->address;
	if (Data::getInstance()->user->reg(user))
	{
		this->alert("注册成功");
	}
	else
	{
		this->alert("系统错误");
	}
	delete user;
}

void CLI::login()
{
	string username;
	cout << "账号 >> ";
	cin >> username;
	cout << "密码 >> ";
	string password = this->passwordInput();
	if (Data::getInstance()->user->login(username, password))
	{
		this->alert("登录成功");
	}
	else
	{
		this->alert("登录失败，用户名或密码错误");
	}
}

void CLI::passwd()
{
	cout << "请输入新密码 >> ";
	string password = this->passwordInput();
	if (Data::getInstance()->user->changePassword(password))
	{
		this->alert("修改成功");
	}
	else
	{
		this->alert("系统错误");
	}
}

void CLI::recharge()
{
	cout << "请输入充值金额，如：100.00 >> ";
	float f;
	cin >> f;
	int i = (int)(f * 100);
	if (Data::getInstance()->user->addFund(i))
	{
		this->alert("充值成功");
	}
	else
	{
		this->alert("系统错误");
	}
}

void CLI::addCourier()
{
	User* user = new Cuser;
	cout << "账号 >> ";
	cin >> user->username;
	cout << "姓名 >> ";
	cin >> user->name;
	cout << "密码 >> ";
	string password = this->passwordInput();
	user->setPassword(password);
	cout << "电话号码 >>";
	cin >> user->phoneNum;
	user->address = "0";
	if (Data::getInstance()->user->reg(user))
	{
		this->alert("注册成功");
	}
	else
	{
		this->alert("系统错误");
	}
	delete user;
}

void CLI::deleteCourier()
{
	bool loop = true;
	while (loop)
	{
		int i = 1;
		map<int, User*> displayUserMap;
		auto m = Data::getInstance()->user->listUser();
		for (auto &v : m)
		{
			User* d = v.second;
			if (d->getUserType() != 1)
			{
				continue;
			}
			displayUserMap.insert_or_assign(i, d);
			cout << i << ".\t" << d->name << endl;
			cout << "\t用户名：" << d->username << endl;
			cout << "\t电话号码：" << d->phoneNum << endl;
			cout << endl;
			i++;
		}
		if (i == 1)
		{
			cout << "当前条件下暂无用户，您可以[q]返回上级，[s]搜索，并回车确认 >> ";
		}
		else
		{
			cout << "请输入编号选择用户，或[q]返回上级，[s]搜索， 并回车确认 >> ";
		}
		string op;
		cin >> op;
		try
		{
			int seq = stoi(op);
			map<int, User*>::iterator it = displayUserMap.find(seq);
			if (it == displayUserMap.end())
			{
				this->alert("错误输入");
			}
			else
			{
				this->deleteUser(it->second);
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

void CLI::deleteUser(User* user)
{
	User* tempuser = Data::getInstance()->user->getUserByName(user->username);
	if (Data::getInstance()->user->del(tempuser))
	{
		this->alert("删除成功");
	}
	else
	{
		this->alert("系统错误");
	}
	delete user;
}
