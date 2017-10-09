#include "CommDelegate.h"
#include <external/md5/md5.c>
#include <sstream>

#define cmd_error			0x1000 //命令出错
#define cmd_query_version   0x1001 //查询版本
#define cmd_add_desk		0x1002 //加入桌子
#define cmd_query_rank		0x1003 //查询排行
#define cmd_desk_ready		0x1004 //举手准备
#define cmd_desk_leave		0x1005 //离开桌子
#define cmd_heart			0x1006 //心跳
#define cmd_put_cards		0x1007 //玩家出牌
#define cmd_query_online	0x1008 //查询在线人数
#define cmd_add_hall		0x1009 //加入大厅
#define	cmd_query_self		0x1010 //查询自己信息
#define	cmd_exit			0x1011 //退出

CCommDelegate::CCommDelegate()
{
}


CCommDelegate::~CCommDelegate()
{
}

string CCommDelegate::ToLogin(const string& user, const string& pass)
{
	md5_state_t pms;
	md5_init(&pms);
	md5_append(&pms, (const md5_byte_t*)pass.c_str(), pass.length());
	md5_byte_t buf[17] = {0};
	md5_finish(&pms, buf);
	char hexbuf[33] = { 0 };
	for (int i=0; i<16; i++)
		sprintf(hexbuf+2*i, "%02x", buf[i]);

	ostringstream os;
	os << "user=" << user << "&pass=" << string(hexbuf, 32);
	return os.str().c_str();
}

pair<int, string> CCommDelegate::ToHeart()
{
	return make_pair(cmd_heart, "");
}

pair<int, string> CCommDelegate::ToQueryCounts()
{
	return make_pair(cmd_query_online, "");
}

pair<int, string> CCommDelegate::ToGame(const string& loginkey)
{
	string rpy = loginkey;
	return make_pair(cmd_add_hall, rpy);
}

pair<int, string> CCommDelegate::ToFastAdd()
{
	string rpy = "{\"opt\":\"add\", \"desk\":0}";
	return make_pair(cmd_add_desk, rpy);
}

pair<int, string> CCommDelegate::ToAdd(int num)
{
	string rpy = "{\"opt\":\"add\", \"desk\":" + to_string(num) + "}";
	return make_pair(cmd_add_desk, rpy);
}

pair<int, string> CCommDelegate::ToCreate()
{
	string rpy = "{\"opt\":\"create\"}";
	return make_pair(cmd_add_desk, rpy);
}

std::pair<int, std::string> CCommDelegate::ToQueryRank()
{
	return make_pair(cmd_query_rank, "");
}

std::pair<int, std::string> CCommDelegate::ToQueryVer(const string& ver)
{
	string rpy = "{\"message\":\""+ ver +"\"}";
	return make_pair(cmd_query_version, rpy);
}

std::pair<int, std::string> CCommDelegate::ToQueryInfo()
{
	return make_pair(cmd_query_self, "");
}

std::pair<int, std::string> CCommDelegate::ToLeave(int deskId, int siteId)
{
	return make_pair(cmd_desk_leave, "");
}

std::pair<int, std::string> CCommDelegate::ToReady(int deskId, int siteId)
{

	return make_pair(cmd_desk_ready, "");
}

std::pair<int, std::string> CCommDelegate::ToPut(int deskId, int siteId, const vector<int>& vecCards)
{
	ostringstream os;
	os << "{\"opt\":\"game\",\"desk\":" << deskId << ",\"site\":" << siteId << ",\"cards\":\"";
	for (const auto& it : vecCards)
		os << it << ",";
	os << "\"}";
	return make_pair(cmd_put_cards, os.str());
}

std::pair<int, std::string> CCommDelegate::ToExit()
{
	return make_pair(cmd_exit, "");
}
