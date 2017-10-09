#ifndef _CCommonFunction_h_
#define _CCommonFunction_h_
#include <string>
#include <map>
#include <list>
#include <vector>
using namespace std;

#define  MOD 18
int inline encrypt_v2(const unsigned char *src, unsigned char *dec, int len)
{
	

	return 0;
}

int inline decrypt_v2(const unsigned char *src, unsigned char *dec, int len)
{
	
	return 0;
}

bool checkUserIegal(const string& name);
bool checkPassIegal(const string& pass);
bool checkNickIegal(const string& name);


void stringToMap(const string& src, map<string, string>& m, const string& sep/*="\r\n"*/);
void stringToList(const string& src, list<string>& lst, const string& sep /*= "\r\n"*/);
void stringToVector(const string& src, vector<string>& lst, const string& sep /*= "\r\n"*/);
#endif

#define CN(_text_) ShowChinese(_text_)
extern std::string ShowChinese(const char * strGBK);
