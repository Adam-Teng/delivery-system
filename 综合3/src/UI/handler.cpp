#include "include/UI/handler.h"

#include <QByteArray>
#include <QMessageAuthenticationCode>

#include "include/data.h"
Handler::Handler(xHttp request) {
	this->request = request;
	std::regex rgx("/");
	std::sregex_token_iterator iter(request.url.begin(), request.url.end(), rgx,
		-1);
	std::sregex_token_iterator end;
	while (iter != end) {
		this->paths.push_back(*iter);
		++iter;
	}
}

xHttp Handler::getResponse() {
	QJsonDocument d;
	xHttp response;
	response.code = 404;
	if (this->paths.size() <= 1) {
		response.code = 200;
		response.body = "{\"msg\":\"delivery API server\"}";

	}
	else if (this->paths[1] == "user" && this->request.method == HTTP_GET) {
		// GET /product
		response.code = 200;
		d = QJsonDocument(this->getUsers());
	}
	else if (this->paths[1] == "delivery" && this->request.method == HTTP_GET) {
		response.code = 200;
		d = QJsonDocument(this->getDelivery());
	}
	else if (this->paths[1] == "session" && this->request.method == HTTP_POST) {
		// POST /login
		QJsonObject obj = this->login();
		response.code = obj["token"].isString() ? 200 : 403;
		d = QJsonDocument(obj);
	}
	else if (this->paths[1] == "user" && this->request.method == HTTP_POST) {
		response.code = 400;
	}
	else if (!this->checkSession()) {
		// Not login
		response.code = 401;
		response.body = "{\"msg\":\"Authentication required.\"}";
	}
	else if (this->paths[1] == "session" && this->request.method == HTTP_GET) {
		// GET /session
		response.code = 200;
		d = QJsonDocument(this->getCurrentUser());
	}
	else if (this->paths[1] == "session" &&
		this->request.method == HTTP_PATCH) {
		// PATCH /session
		if (this->patchUser()) {
			request.code = 200;
			d = QJsonDocument(this->getCurrentUser());
		}
		else {
			response.code = 500;
		}
	}
	else if (this->paths[1] == "delivery" && this->request.method == HTTP_POST)
	{
		response.code = this->sendExpress();
	}
	else if (this->paths[1] == "delivery" && this->paths.size() >= 3 && this->request.method == HTTP_PUT)
	{
		response.code = this->editExpress(this->paths[2]);
	}
	if (response.body.length() == 0) {
		response.body = d.toJson().toStdString();
	}
	response.headers.insert_or_assign("content-type",
		"application/json; charset=utf-8");
	response.headers.insert_or_assign("content-length",
		to_string(response.body.length()));
	response.headers.insert_or_assign("server", "delivery @Teng");
	return response;
}

//JWT身份验证
bool Handler::checkSession() {
	auto it = this->request.headers.find("authorization");
	if (it == this->request.headers.end())
	{
		return false;
	}
	string authStr = it->second;
	authStr = authStr.substr(7);
	vector<string> seg;
	std::regex rgx("\\.");
	std::sregex_token_iterator iter(authStr.begin(), authStr.end(), rgx, -1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter)
	{
		seg.push_back(*iter);
	}
	if (seg.size() != 3)
	{
		//Wrong JWT Segments
		return false;
	}
	string key = "delivery2KeyC@9Z^2ZEhkDz9Ycm";
	QByteArray sign = QMessageAuthenticationCode::hash(QByteArray::fromStdString(seg[0] + "." + seg[1]), QByteArray::fromStdString(key), QCryptographicHash::Sha256).toBase64();
	string signStr = sign.toStdString();
	replace(signStr.begin(), signStr.end(), '+', '-');
	replace(signStr.begin(), signStr.end(), '/', '_');
	signStr.erase(remove(signStr.begin(), signStr.end(), '='), signStr.end());
	if (signStr != seg[2])
	{
		//Wrong signature
		return false;
	}
	QByteArray jsonStr = QByteArray::fromBase64(QByteArray::fromStdString(seg[1]));
	QJsonDocument json = QJsonDocument::fromJson(jsonStr);
	if (!json.isObject())
	{
		//wrong json
		return false;
	}
	QJsonObject obj = json.object();
	time_t now = time(NULL);
	time_t exp = obj["exp"].toInt();
	if (exp < now)
	{
		//Expired
		return false;
	}
	return ((Backend::UserManager*)Data::getInstance()->user)->setCurrentUser(obj["sub"].toString().toStdString());
}

QJsonArray Handler::getDelivery() {
	auto m = Data::getInstance()->delivery->listDelivery();
	QJsonArray jsonArray;
	for (auto& kv : m)
	{
		Delivery* v = kv.second;
		QJsonObject obj;
		obj["id"] = QJsonValue(QString::fromStdString(v->id));
		obj["name"] = QJsonValue(QString::fromStdString(v->name));
		obj["sendUser"] = QJsonValue(QString::fromStdString(v->sendUser));
		obj["sendTime"] = QJsonValue(QString::fromStdString(v->sendTime));
		obj["receiveUser"] = QJsonValue(QString::fromStdString(v->receiveUser));
		obj["receiveTime"] = QJsonValue(QString::fromStdString(v->receiveTime));
		obj["description"] = QJsonValue(QString::fromStdString(v->description));
		obj["courier"] = QJsonValue(QString::fromStdString(v->courier));
		obj["pricing"] = QJsonValue(v->pricing);
		obj["state"] = QJsonValue(v->state);
		obj["type"] = QJsonValue((int)v->getDeliveryType());
	}
	return jsonArray;
}

QJsonArray Handler::getUsers() {
	auto m = ((Backend::UserManager*)Data::getInstance()->user)->listUser();
	QJsonArray jsonArray;
	for (auto& kv : m)
	{
		User* v = kv.second;
		if (v->getUserType() != MANAGER)
		{
			continue;
		}
		QJsonObject obj;
		obj["username"] = QJsonValue(QString::fromStdString(v->username));
		obj["name"] = QJsonValue(QString::fromStdString(v->name));
		obj["phoneNum"] = QJsonValue(QString::fromStdString(v->phoneNum));
		obj["address"] = QJsonValue(QString::fromStdString(v->address));
		obj["balance"] = QJsonValue(v->balance);
		obj["type"] = QJsonValue(v->getUserType() == MANAGER ? "MANAGER" : (v->getUserType() == COURIER ? "COURIER" : "USER"));
		jsonArray.push_back(obj);
	}
	return jsonArray;
}

QJsonObject Handler::getCurrentUser() {
	User* v = Data::getInstance()->user->currentUser;
	QJsonObject obj;
	obj["username"] = QJsonValue(QString::fromStdString(v->username));
	obj["name"] = QJsonValue(QString::fromStdString(v->name));
	obj["phoneNum"] = QJsonValue(QString::fromStdString(v->phoneNum));
	obj["address"] = QJsonValue(QString::fromStdString(v->address));
	obj["balance"] = QJsonValue(v->balance);
	obj["type"] = QJsonValue(v->getUserType() == MANAGER ? "MANAGER" : (v->getUserType() == COURIER ? "COURIER" : "USER"));
	return obj;
}

bool Handler::patchUser() {
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->request.body), &jsonError);
	if (QJsonParseError::NoError != jsonError.error || !jsonDoc.isObject())
	{
		cout << jsonError.errorString().toStdString() << endl;
		return false;
	}
	QJsonObject obj = jsonDoc.object();
	if (obj["password"].isString())
	{
		Data::getInstance()->user->changePassword(obj["password"].toString().toStdString());
	}
	if (obj["balance"].isDouble())
	{
		Data::getInstance()->user->addFund(obj["balance"].toInt());
	}
	return true;
}

QJsonObject Handler::login() {
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->request.body), &jsonError);
	if (QJsonParseError::NoError != jsonError.error || !jsonDoc.isObject())
	{
		cout << jsonError.errorString().toStdString() << endl;
		QJsonObject obj;
		return obj;
	}
	QJsonObject obj = jsonDoc.object();
	if (!obj["password"].isString() || !obj["username"].isString())
	{
		QJsonObject obj;
		return obj;
	}
	if (Data::getInstance()->user->login(obj["username"].toString().toStdString(),
		obj["password"].toString().toStdString()))
	{
		string jwtHead = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";
		QJsonObject jwt;
		jwt["sub"] = QString::fromStdString(Data::getInstance()->user->currentUser->username);
		jwt["exp"] = time(NULL) + 86400;
		string jwtBody = QJsonDocument(jwt).toJson().toBase64().toStdString();
		replace(jwtBody.begin(), jwtBody.end(), '+', '-');
		replace(jwtBody.begin(), jwtBody.end(), '/', '_');
		jwtBody.erase(remove(jwtBody.begin(), jwtBody.end(), '='), jwtBody.end());
		string key = "delivery2KeyC@9Z^2ZEhkDz9Ycm";
		QByteArray sign = QMessageAuthenticationCode::hash(QByteArray::fromStdString(jwtHead + "." + jwtBody),
			QByteArray::fromStdString(key), QCryptographicHash::Sha256).toBase64();
		string signStr = sign.toStdString();
		replace(signStr.begin(), signStr.end(), '+', '-');
		replace(signStr.begin(), signStr.end(), '/', '_');
		signStr.erase(remove(signStr.begin(), signStr.end(), '='), signStr.end());
		QJsonObject obj;
		obj["token"] = QString::fromStdString(jwtHead + "." + jwtBody + "." + signStr);
		return obj;
	}
	else
	{
		QJsonObject obj;
		return obj;
	}
}

int Handler::sendExpress() {
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->request.body), &jsonError);
	if (QJsonParseError::NoError != jsonError.error || !jsonDoc.isObject()) {
		cout << jsonError.errorString().toStdString() << endl;
		return 400;
	}
	QJsonObject obj = jsonDoc.object();
	Delivery* np = NULL;
	switch (obj["type"].toInt()) {
	case BOOK:
		np = new DeliveryBook;
		break;
	case NORMAL:
		np = new DeliveryNormal;
		break;
	case FRAGILE:
		np = new DeliveryFragile;
		break;
	default:
		return 400;
	}
	np->name = obj["name"].toString().toStdString();
	np->receiveUser = obj["receiveUser"].toString().toStdString();
	np->sendTime = obj["sendTime"].toString().toStdString();
	np->receiveTime = obj["receiveTime"].toString().toStdString();
	np->state = obj["state"].toInt();
	np->description = obj["description"].toString().toStdString();
	np->pricing = obj["pricing"].toInt();
	np->courier = obj["courier"].toString().toStdString();
	bool res = Data::getInstance()->delivery->addDelivery(np);
	delete np;
	return res ? 201 : 500;
}

int Handler::editExpress(string id) {
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(this->request.body), &jsonError);
	if (QJsonParseError::NoError != jsonError.error || !jsonDoc.isObject()) {
		cout << jsonError.errorString().toStdString() << endl;
		return 400;
	}
	QJsonObject obj = jsonDoc.object();
	Delivery* np = NULL;
	switch (obj["type"].toInt()) {
	case BOOK:
		np = new DeliveryBook;
		break;
	case NORMAL:
		np = new DeliveryNormal;
		break;
	case FRAGILE:
		np = new DeliveryFragile;
		break;
	default:
		return 400;
	}
	np->id = id;
	np->name = obj["name"].toString().toStdString();
	np->receiveUser = obj["receiveUser"].toString().toStdString();
	np->sendTime = obj["sendTime"].toString().toStdString();
	np->receiveTime = obj["receiveTime"].toString().toStdString();
	np->state = obj["state"].toInt();
	np->description = obj["description"].toString().toStdString();
	np->courier = obj["courier"].toString().toStdString();
	np->pricing = obj["pricing"].toInt();
	bool res = Data::getInstance()->delivery->replaceDelivery(np);
	delete np;
	return res ? 200 : 500;
}


