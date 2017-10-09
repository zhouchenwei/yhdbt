#include "MessageQueue.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "CommDelegate.h"
using rapidjson::Document;
#include "cocos2d.h"
USING_NS_CC;
CMessageQueue::CMessageQueue()
{
#ifdef _WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif
}


CMessageQueue::~CMessageQueue()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

bool CMessageQueue::start()
{
	//主通讯和心跳
	thread(&CMessageQueue::threadWork, this).detach();
	thread(&CMessageQueue::threadHeart, this).detach();
	return true;
}

void CMessageQueue::addMessage(msgptr msg)
{
	unique_lock<mutex> ul(_muxMsg);
	_lstMessage.emplace_back(msg);
}

msgptr CMessageQueue::getMessage() {
	unique_lock<mutex> ul(_muxMsg);
	if (_lstMessage.empty()){
		return nullptr;
	}
	msgptr ptr = _lstMessage.front();
	_lstMessage.pop_front();
	return ptr;
}

void CMessageQueue::threadWork()
{
	string strRecv;
	while (true){
		strRecv.clear();
		//接受指令
		if (!tcpConn::instance()->RecvQuery(strRecv))
			break;

		msgptr ptr = prcessQuery(strRecv);
		if (ptr == nullptr)
			break;

		_muxMsg.lock();
		_lstMessage.emplace_back(ptr);
		_muxMsg.unlock();
	}
	tcpConn::instance()->GameError();
}

void CMessageQueue::threadHeart()
{
	while (true) {
		this_thread::sleep_for(chrono::seconds(30));
		tcpConn::instance()->SendQuery(commDele::instance()->ToHeart());
	}
}

msgptr CMessageQueue::prcessQuery(const string& msg)
{
	//解析指令
	Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.HasParseError())
		return nullptr;

	msgptr ptr = make_shared<stmsg>();
	ptr->opt = doc["opt"].GetString();
	//在线人员
	if (ptr->opt == "online"){
		ptr->hallmsg.oninle = doc["count"].GetString();
	}
	//自己信息
	else if (ptr->opt == "login"){
		ptr->hallmsg.self.nick = doc["nick"].GetString();
		ptr->hallmsg.self.score = atoi(doc["score"].GetString());
		ptr->hallmsg.self.win = atoi(doc["win"].GetString());
		ptr->hallmsg.self.lose = atoi(doc["lose"].GetString());
		ptr->hallmsg.self.run = atoi(doc["run"].GetString());
		ptr->hallmsg.self.he = atoi(doc["he"].GetString());
		ptr->hallmsg.self.sex = atoi(doc["sex"].GetString());
	}
	//排行信息
	else if (ptr->opt == "rank"){
		if (!doc["info"].Empty()){
			ostringstream os;
			for (size_t i = 0; i < doc["info"].GetArray().Capacity(); i++)
				os << i << "." << doc["info"][i]["nick"].GetString() << " " << doc["info"][i]["score"].GetString() << "\r\n";
			ptr->message = os.str();
		}
	}
	//加入桌子
	else if (ptr->opt == "add") {
		int desk = atoi(doc["desk"].GetString());
		if (desk == -1)
			ptr->message = "error";
		else {
			ptr->deskmsg.desk = desk;
			ptr->deskmsg.site = atoi(doc["site"].GetString());
		}
	}
	//玩家变更
	else if (ptr->opt == "change") {
		for (size_t i = 0; i < 4; ++i) {
			playinfoptr pinfo = make_shared<stplayinfo>();
			ptr->deskmsg.mapPlayers[i] = pinfo;
			ptr->deskmsg.mapPlayers[i]->nick = string(doc["info"][i]["name"].GetString(), doc["info"][i]["name"].GetStringLength());
			if (!ptr->deskmsg.mapPlayers[i]->nick.empty()) {
				ptr->deskmsg.mapPlayers[i]->ready = atoi(doc["info"][i]["ready"].GetString());
				ptr->deskmsg.mapPlayers[i]->score = atoi(doc["info"][i]["score"].GetString());
				ptr->deskmsg.mapPlayers[i]->win = atoi(doc["info"][i]["win"].GetString());
				ptr->deskmsg.mapPlayers[i]->lose = atoi(doc["info"][i]["lose"].GetString());
				ptr->deskmsg.mapPlayers[i]->run = atoi(doc["info"][i]["run"].GetString());
				ptr->deskmsg.mapPlayers[i]->he = atoi(doc["info"][i]["he"].GetString());
				ptr->deskmsg.mapPlayers[i]->sex = atoi(doc["info"][i]["sex"].GetString());
			}
		}
	}
	//游戏开始
	else if (ptr->opt == "start") {
		ptr->deskmsg.cards = doc["cards"].GetString();
	}
	//游戏结束
	else if (ptr->opt == "over") {
		ptr->message = doc["result"].GetString();
	}
	//玩家逃跑
	else if (ptr->opt == "run") {
		ptr->deskmsg.site = atoi(doc["site"].GetString());
		ptr->deskmsg.self.nick = string(doc["name"].GetString(), doc["name"].GetStringLength());
		ptr->deskmsg.self.score = atoi(doc["score"].GetString());
	}
	//主游戏
	else if (ptr->opt == "game") {
		ptr->deskmsg.per = atoi(doc["per"].GetString());
		ptr->deskmsg.surplus = doc["surplus"].GetString();
		ptr->deskmsg.now = atoi(doc["now"].GetString());
		ptr->deskmsg.putCards = doc["cards"].GetString();
		ptr->deskmsg.deskscore = atoi(doc["score"].GetString());
		ptr->deskmsg.must = atoi(doc["must"].GetString());
	}
	//得分
	else if (ptr->opt == "score") {
		ptr->deskmsg.p0score = doc["p0"].GetString();
		ptr->deskmsg.p1score = doc["p1"].GetString();
	}
	//数据异常
	else if (ptr->opt == "error") {
		tcpConn::instance()->GameError();
	}
	//长时间不准备
	else if (ptr->opt == "timeout") {
		//
	}
	//数据异常
	else if (ptr->opt == "kicked") {
		MessageBox(Configuration::getInstance()->getValue("kicked").asString().c_str(), "error");
		exit(0);
	}
	return ptr;
}
