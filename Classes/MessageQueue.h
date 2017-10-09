#ifndef _CMessageQueue_h_
#define _CMessageQueue_h_

#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include "cocos2d.h"
#include "Singleton.h"
#include "TcpConnect.h"
using namespace std;
USING_NS_CC;

//玩家信息
typedef struct playinfo {
	string nick;
	int score = 0;
	int win = 0;
	int lose = 0;
	int run = 0;
	int sex = 0;
	int he = 0;

	int site = -1;
	int ready = 0;
	int result = 0;
	string	info;
}stplayinfo;
typedef shared_ptr<stplayinfo> playinfoptr;

//桌子相关信息
typedef struct deskinfo {
	int desk = -1;
	int site = -1;
	int per = -1;
	int now = -1;
	int must = 0;

	string	surplus;
	string p0score;
	string p1score;
	int deskscore = 0;
	
	string		cards;	//游戏开始时的手牌
	playinfo	self;	//自己信息
	string 		putCards;	//出的牌
	map<int,playinfoptr> mapPlayers; //玩家信息
}stdeskinfo;

//大厅相关
typedef struct hallinfo{
	string  oninle; //在线人数
	string  ver;	//版本号
	playinfo self;	//自己信息
}sthallinfo;

//游戏消息结构
typedef struct stGameMessage {
	string opt;		//操作类型
	string message; //消息

	sthallinfo	hallmsg; //大厅相关
	stdeskinfo  deskmsg; //桌子相关

}stmsg;
typedef shared_ptr<stmsg> msgptr;

class CMessageQueue
{
public:
	CMessageQueue();
	~CMessageQueue();

	bool	start();
	void	addMessage(msgptr msg);
	msgptr	getMessage();

protected:
	void	threadWork();
	void	threadHeart();

	msgptr	prcessQuery(const string& msg);

private:
	list<msgptr>	_lstMessage;
	mutex			_muxMsg;
};

typedef CSingleton<CMessageQueue>	messageQueue;

#endif 