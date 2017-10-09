#ifndef _H_CHTTPQUEUE_H_ 
#define _H_CHTTPQUEUE_H_
#include <string>
#include "Singleton.h"
#include <network/HttpClient.h>
#include "LoginScene.h"
using namespace std;
using namespace cocos2d::network;

class CHttpQueue
{
public:
	CHttpQueue();
	~CHttpQueue();
	
	void Version();
	void VersionCB(HttpClient* client, HttpResponse* response);
	void Login(const string& info);
	void LoginCB(HttpClient* client, HttpResponse* response);

private:
	HttpRequest request;

};

typedef CSingleton<CHttpQueue> httpQuery;

#endif