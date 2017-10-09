#ifndef _H_CCOMMDELEGATE_H_
#define _H_CCOMMDELEGATE_H_
#include <string>
#include <map>
#include <vector>
#include "Singleton.h"
using namespace std;

class CCommDelegate
{
public:
	CCommDelegate();
	~CCommDelegate();

	string ToLogin(const string& user, const string& pass);//登录游戏
	pair<int, string> ToHeart();//心跳
	pair<int, string> ToQueryCounts();//查询在线人数
	pair<int, string> ToGame(const string& loginkey);//登录游戏大厅
	pair<int, string> ToFastAdd();//快速开始
	pair<int, string> ToAdd(int num);//加入桌子
	pair<int, string> ToCreate();	//创建桌子
	pair<int, string> ToQueryRank();//排行榜
	pair<int, string> ToQueryVer(const string& ver);  //版本
	pair<int, string> ToQueryInfo(); //个人信息
	pair<int, string> ToLeave(int deskId, int siteId);//离开桌子
	pair<int, string> ToReady(int deskId, int siteId);//准备
	pair<int, string> ToPut(int deskId, int siteId, const vector<int>& vecCards);//出牌/不出
	pair<int, string> ToExit();//退出
};

typedef CSingleton<CCommDelegate> commDele;
#endif