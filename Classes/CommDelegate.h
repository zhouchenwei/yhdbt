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

	string ToLogin(const string& user, const string& pass);//��¼��Ϸ
	pair<int, string> ToHeart();//����
	pair<int, string> ToQueryCounts();//��ѯ��������
	pair<int, string> ToGame(const string& loginkey);//��¼��Ϸ����
	pair<int, string> ToFastAdd();//���ٿ�ʼ
	pair<int, string> ToAdd(int num);//��������
	pair<int, string> ToCreate();	//��������
	pair<int, string> ToQueryRank();//���а�
	pair<int, string> ToQueryVer(const string& ver);  //�汾
	pair<int, string> ToQueryInfo(); //������Ϣ
	pair<int, string> ToLeave(int deskId, int siteId);//�뿪����
	pair<int, string> ToReady(int deskId, int siteId);//׼��
	pair<int, string> ToPut(int deskId, int siteId, const vector<int>& vecCards);//����/����
	pair<int, string> ToExit();//�˳�
};

typedef CSingleton<CCommDelegate> commDele;
#endif