#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include "../misc/http.h"
using namespace std;
class xHttpClient {
public:
	xHttpClient() {}
	string base;
	xHttp request;
	xHttp response;
	int sendRequest();
};
#endif