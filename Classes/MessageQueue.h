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

//�����Ϣ
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

//���������Ϣ
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
	
	string		cards;	//��Ϸ��ʼʱ������
	playinfo	self;	//�Լ���Ϣ
	string 		putCards;	//������
	map<int,playinfoptr> mapPlayers; //�����Ϣ
}stdeskinfo;

//�������
typedef struct hallinfo{
	string  oninle; //��������
	string  ver;	//�汾��
	playinfo self;	//�Լ���Ϣ
}sthallinfo;

//��Ϸ��Ϣ�ṹ
typedef struct stGameMessage {
	string opt;		//��������
	string message; //��Ϣ

	sthallinfo	hallmsg; //�������
	stdeskinfo  deskmsg; //�������

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