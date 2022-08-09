#ifndef SERVER_H
#define SERVER_H

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

class deliveryServer : public QTcpServer {
	Q_OBJECT
public:
	deliveryServer(QObject* parent = 0);
	~deliveryServer();
protected slots:
	void onNewConnection();
	void onReadyRead();

private:
	void processData(QTcpSocket* sock);
};

#endif