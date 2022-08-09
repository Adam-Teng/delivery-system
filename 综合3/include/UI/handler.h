#ifndef HANDLER_H
#define HANDLER_H
#include "include/misc/http.h"
#include <vector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMap>
#include <QString>
using namespace std;
class Handler
{
public:
	Handler(xHttp request);
	xHttp getResponse();
	xHttp request;
	vector<string> paths;
private:
	QJsonArray getDelivery();
	QJsonArray getUsers();
	QJsonObject getCurrentUser();
	QJsonObject login();
	bool patchUser();
	bool checkSession();
	int sendExpress();
	int editExpress(string id);
};
#endif