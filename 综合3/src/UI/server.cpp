#include "include/UI/server.h"
#include "include/UI/handler.h"
#include "include/misc/http.h"
#include <QTcpSocket>
#include <iostream>
#include <regex>
#include <string>
using namespace std;
deliveryServer::deliveryServer(QObject* parent)
	: QTcpServer(parent) {
	connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}
deliveryServer::~deliveryServer() {
}
void deliveryServer::onNewConnection() {
	while (hasPendingConnections()) {
		QTcpSocket* sock = nextPendingConnection();
		connect(sock, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
		if (sock->bytesAvailable() > 0) {
			processData(sock);
		}
	}
}
void deliveryServer::onReadyRead() {
	QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender()); //取得当前Socket对象
	processData(sock);
}
void deliveryServer::processData(QTcpSocket* socket) {
	string data = socket->readAll().toStdString();
	xHttp packet = xHttp::fromString(data);
	Handler handler(packet);
	xHttp response = handler.getResponse();
	char* str[8] = { "UNKNOWN", "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH" };
	cout << "HTTP: " << str[packet.method] << " " << packet.url << " -> " << response.code << endl;
	string resStr = response.toString();
	socket->write(QByteArray::fromStdString(resStr));
	socket->flush();
	socket->waitForBytesWritten((int)resStr.size());
	socket->close();
}