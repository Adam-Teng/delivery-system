#include "include/misc/http.h"
#include "include/client/httpclient.h"
#include "include/data.h"
#include "include/UI/cli.h"
#include "include/UI/server.h"
#include <WinSock2.h>
#include <QCommandLineParser>
#include <QApplication>
#include <memory>

std::shared_ptr<Data> Data::instance(new Data(), Data::destroy);

int main(int argc, char* argv[])
{
	/*命令行设置*/
	QCoreApplication a(argc, argv);
	QCoreApplication::setApplicationName("deliverySys");
	QCoreApplication::setApplicationVersion("1.0");
	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	parser.setApplicationDescription("\n快递任务管理系统");
	parser.addOptions({
		{{"s", "server"}, "server mode"},
		{{"c", "client"}, "client mode"},
		{{"l", "local"}, "local mode"},
		{{"p", "port"}, "set port for server. Default 3010", "port"},
		});
	parser.process(a);
	/*导入数据*/
	if (parser.isSet("s") || parser.isSet("l"))
	{
		shared_ptr<Data> d = Data::getInstance();
		d->init("../../../file/user.json", "../../../file/delivery.json");
	}
	else if (parser.isSet("c"))
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		shared_ptr<Data> d = Data::getInstance();
		d->init("http://127.0.0.1:3010/");
	}
	/*开始执行代码*/
	if (parser.isSet("l") || parser.isSet("c"))
	{
		new CLI;
		return 0;
	}
	else if (parser.isSet("s"))
	{
		int port = parser.isSet("p") ? parser.value("p").toInt() : 3010;
		deliveryServer server;
		cout << "server start, listening on http://0.0.0.0:" << port << endl;
		server.listen(QHostAddress::Any, port);
		return a.exec();
	}
	else
	{
		parser.showHelp();
		return 0;
	}
	//new CLI;
	//return 0;
}
