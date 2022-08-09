#include "include/UI/cli.h"
#include "include/data.h"
#include <sstream>
#include <codecvt>
#include <locale>

using namespace std;

// 命令行界面
CLI::CLI() {
	while (this->mainMenu())
	{

	}
}

bool CLI::mainMenu()
{
	system("cls");
	cout << "Delivery-Manage by Teng: " << endl;
	User* currentUser = Data::getInstance()->user->currentUser;
	User* manager = NULL;
	auto m = Data::getInstance()->user->listUser();
	for (auto& kv : m)
	{
		if (kv.first == "manager")
		{
			manager = kv.second;
			break;
		}
	}
	if (currentUser == NULL)
	{
		cout << "您还没有登录，请登录！" << endl;
	}
	else
	{
		cout << "当前用户：";
		cout << (currentUser->getUserType() == MANAGER ? "[管理员]" : (currentUser
			->getUserType() == COURIER ? "[快递员]" : "[普通用户]"));
		cout << currentUser->username << " ";
		cout << "姓名：" << currentUser->name;
		cout << " 余额：" << ((float)currentUser->balance) / 100 << "元";
		if (currentUser->getUserType() == USER)
		{
			cout << endl;
			cout << "电话号码：" << currentUser->phoneNum << endl;
			cout << "地址：" << currentUser->address << endl;
		}
	}
	if (currentUser == NULL)
	{
		cout << "1. 登录" << endl;
		cout << "2. 注册" << endl;
	}
	else
	{
		cout << endl;
		cout << "1. 余额充值" << endl;
		if (currentUser->getUserType() == MANAGER)
		{
			cout << "2. 查询用户信息" << endl;
			cout << "3. 查询快递信息" << endl;
			cout << "4. 添加快递员" << endl;
			cout << "5. 删除快递员" << endl;
			cout << "6. 分配快递员" << endl;
		}
		else if (currentUser->getUserType() == COURIER)
		{
			cout << "2. 查询未签收快递信息" << endl;
			cout << "3. 查询已签收快递信息" << endl;
			cout << "4. 揽收快递" << endl;
		}
		else
		{
			cout << "2. 查询发送快递" << endl;
			cout << "3. 查询接收快递" << endl;
			cout << "4. 发送快递" << endl;
			cout << "5. 接收快递" << endl;
		}
		cout << "8. 修改密码" << endl;
		cout << "9. 退出登录" << endl;

	}
	cout << "0. 退出程序" << endl;
	cout << "请选择操作并回车以确认 >> ";
	string strOp;
	cin >> strOp;
	if (strOp.find_first_not_of("1234567890") != string::npos)
	{
		this->alert("错误输入");
		return true;
	}
	int op = -1;
	try
	{
		op = stoi(strOp);
	}
	catch (exception e)
	{
		this->alert("输入错误，请重新输入");
		system("pause");
		return true;
	}
	if (currentUser == NULL && op > 0)
	{
		op += 10;
	}
	if (currentUser != NULL && currentUser->getUserType() == USER && op > 0)
	{
		op += 20;
	}
	if (currentUser != NULL && currentUser->getUserType() == COURIER && op > 0)
	{
		op += 30;
	}
	switch (op)
	{
	case 1: //余额充值
	case 21:
	case 31:
		this->recharge();
		break;
	case 8:
	case 28:
	case 38:
		this->passwd();
		break;
	case 9:
	case 29:
	case 39:
		if (Data::getInstance()->user->logout())
		{
			this->alert("您已退出登录");
		}
		else
		{
			this->alert("系统错误");
		}
		break;
	case 11:
		this->login();
		break;
	case 12:
		this->reg();
		break;
	case 2:
		if (currentUser->getUserType() == MANAGER)
		{
			this->listUser();
		}
		break;
	case 3:
		if (currentUser->getUserType() == MANAGER)
		{
			this->listExpress();
		}
		break;
	case 4:
		if (currentUser->getUserType() == MANAGER)
		{
			this->addCourier();
		}
		break;
	case 5:
		if (currentUser->getUserType() == MANAGER)
		{
			this->deleteCourier();
		}
		break;
	case 6:
		if (currentUser->getUserType() == MANAGER)
		{
			this->takeExpress(currentUser);
		}
		break;
	case 22:
		if (currentUser->getUserType() == USER)
		{
			this->listSendExpress(currentUser);
		}
		break;
	case 23:
		if (currentUser->getUserType() == USER)
		{
			this->listReceiveExpress(currentUser);
		}
		break;
	case 25:
		if (currentUser->getUserType() == USER)
		{
			this->receiveExpress(currentUser);
		}
		break;
	case 24:
		if (currentUser->getUserType() == USER)
		{
			this->sendExpress(currentUser, manager);
		}
		break;
	case 32:
		if (currentUser->getUserType() == COURIER)
		{
			this->listSendCourierExpress(currentUser);
		}
		break;
	case 33:
		if (currentUser->getUserType() == COURIER)
		{
			this->listHaveSCourierExpress(currentUser);
		}
		break;
	case 34:
		if (currentUser->getUserType() == COURIER)
		{
			this->getExpress(currentUser);
		}
		break;
	case 0:
		return false;
	default:
		this->alert("输入错误，请重新输入");
	}
	return true;
}
