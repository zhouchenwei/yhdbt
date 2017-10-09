#include "HttpQueue.h" 
#include <sstream>
#include <functional>
#include "MessageQueue.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
using rapidjson::Document;
#include "cocos2d.h"
USING_NS_CC;
using namespace std;


CHttpQueue::CHttpQueue()
{
}


CHttpQueue::~CHttpQueue()
{
}

void CHttpQueue::Version()
{
	HttpClient* client = HttpClient::getInstance();
	ostringstream os;
	os << "http://" << Configuration::getInstance()->getValue("host").asString().c_str() << ":51888/version";
	request.setUrl(os.str().c_str());
	request.setResponseCallback(bind(&CHttpQueue::VersionCB, this,
		std::placeholders::_1, std::placeholders::_2));
	request.setRequestType(HttpRequest::Type::GET);
	client->send(&request);
}

void CHttpQueue::VersionCB(HttpClient* client, HttpResponse* response)
{
	if (!response->isSucceed()) {
		MessageBox(Configuration::getInstance()->getValue("game_error").asString().c_str(), "error");
		exit(0);
		return;
	}
	auto vecContent = response->getResponseData();
	string content = string(vecContent->data(), vecContent->size());

	if (content.find("version\":\"61") == string::npos){
		MessageBox(Configuration::getInstance()->getValue("ver_error").asString().c_str(), "error");
		exit(0);
	}
}

void CHttpQueue::Login(const string& info)
{
	// 连接服务器
	if (!tcpConn::instance()->ConnectServer(
		Configuration::getInstance()->getValue("host").asString(), 9999)) {
		msgptr ptr = make_shared<stmsg>();
		ptr->opt = "login";
		ptr->message = "error";
		messageQueue::instance()->addMessage(ptr);
		return;
	}
	// 登陆
	HttpClient* client = HttpClient::getInstance();
	ostringstream os;
	os << "http://" << Configuration::getInstance()->getValue("host").asString().c_str()
		<< ":51888/login?" << info;
	request.setUrl(os.str().c_str());
	request.setResponseCallback(bind(&CHttpQueue::LoginCB, this, 
		std::placeholders::_1, std::placeholders::_2));
	request.setRequestType(HttpRequest::Type::GET);
	client->send(&request);
}

void CHttpQueue::LoginCB(HttpClient* client, HttpResponse* response)
{	
	msgptr ptr = make_shared<stmsg>();
	ptr->opt = "login";
	if (!response->isSucceed()){
		ptr->message = "error";
		messageQueue::instance()->addMessage(ptr);
		return;
	}
	auto vecContent = response->getResponseData();
	string content = string(vecContent->data(), vecContent->size());

	Document doc;
	doc.Parse<0>(content.c_str());
	if (doc.HasParseError()) {
		ptr->message = "error";
		messageQueue::instance()->addMessage(ptr);
		return;
	}
	
	if (doc.HasMember("loginkey"))
		ptr->message = doc["loginkey"].GetString();
	
	if (ptr->message.empty())
		ptr->message = "login_error";
	messageQueue::instance()->addMessage(ptr);
}
