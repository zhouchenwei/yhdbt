#ifndef __CDeskScene_H__
#define __CDeskScene_H__

#include "cocos2d.h"
#include "UI/UIButton.h"
#include "CardSprite.h"
#include "MessageQueue.h"
USING_NS_CC;

// typedef struct
// {
// 	Label* _nickName;
// 	Label* _score;
// 	Label* _total;
// 	Label* _surplus;
// 	Sprite*_ready;
// 	LabelAtlas* _time;
// 	Label* _gone;
// 	Label* _buchu;
// 	int 	_x;
// 	int		_y;
// 	int		id;
// 	chrono::time_point<chrono::system_clock> timepoint;
// 	vector<CardSprite*> perCards;
// 
// }st_player_info;
// typedef shared_ptr<st_player_info> playerPtr;
// 
// bool inline mycompair(CardSprite* c1, CardSprite* c2) {
// 	return c1->getSeq() > c2->getSeq();
// }
// 


//�����
typedef struct playerExtensionInfo{
	stplayinfo playerInfo;

	Label*	mheaderPic;	//ͷ��
	Label*	mnick;		//�ǳ�
	Label*	mrecord;	//ս��
	Label*	msurplus;	//ʣ������
	Label*	mtimer;		//����ʱ
	Sprite* mready;		//�Ƿ�׼��
	Label*	mgone;		//�Ƿ����
	Label*	mpass;		//����
	Point	pos;		//���Ƶ�����

	chrono::time_point<chrono::system_clock> timepoint; //����ʱ
	vector<CardSprite*> perCards;	//֮ǰ������

}stPlayerExtensionInfo;
typedef shared_ptr<stPlayerExtensionInfo> playerPtr;


class CDeskScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int desk = 0, int site = 0, const stplayinfo& pinfo = stplayinfo{});

    virtual bool init();
   
    CREATE_FUNC(CDeskScene);

	void onReturn(Ref *pSender, ui::Widget::TouchEventType type);
	void onReady(Ref *pSender, ui::Widget::TouchEventType type);
	void onNoput(Ref *pSender, ui::Widget::TouchEventType type);
	void onPut(Ref *pSender, ui::Widget::TouchEventType type);
 
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);

	void deskSchedule(float dt);
	void timeSchedule(float dt);

	inline void	setDeskInfo(int desk, int site, const stplayinfo& pinfo) {
		_deskId = desk;
		_siteId = site; 
		_playinfo = pinfo;
		_labelDeskNum->setString(to_string(_deskId));
		//������λ��
		for (size_t i = 0; i < 4; i++)
			_mapPlayers[(_siteId + i) % 4] = _vecPlayers[i];
	};

protected:
	void	buttonEnable(bool enable);
	void	clearDesk(int site);

	void	gameSatrt();
	void	gameOver();

	void	getSelectedCards(vector<int>& vecSel);
	void	sortCards(vector<int>& vec); 

	void    perPutCards(msgptr ptr); //������һ��
	void	nowPutCards(msgptr ptr); //����ǰ��

	int		calcuLen(int counts); //������Ƶ�λ��

private:
	//�߼����
	int			_deskId;			//����
	int			_siteId;			//�������λ��
	int			_baseScore = 10;	//������
	bool		_isSatrting = false;//��Ϸ�Ƿ������
	bool		_exit = false;		//�Ƿ�ǿ���˳�����
	bool		_must = false;		//�Ƿ�������
	Size		_visibleSize;		//���ڳߴ�
	mutex		_muxButton;			//��ť������
	stplayinfo	_playinfo;
	int			_nowPut = -1;		//��ǰ������λ��
	CardSprite*	_perSelectedCard = nullptr;	//֮ǰѡ�е��ƣ���ֹѡ��ʱ�ƶ�����

	map<int, playerPtr>	_mapPlayers;//��λ��->���
	vector<playerPtr>	_vecPlayers;//��ʼ����ʱ���������
	vector<CardSprite*>	_vecCards;	//�Լ�����(ֻ���Լ���)
	vector<int>			_vecPerCards; //��һ�γ�����

	//ͼƬ���
	ui::Button* _btReturn;
	ui::Button* _btReady;
	ui::Button* _btNoput;
	ui::Button* _btPut;

	Label*	_labelDeskNum;		//����
	Label*  _labelDeskScore;	//�������
	Label*	_labelBaseScore;	//�׷�
	Label*	_label0Score;		//0���÷�
	Label*	_label1Score;		//1���÷�
	Label*	_labelInfo;			//���,��ʾ��
	Label*	_labelOnline;		//��������
};

#endif // __HELLOWORLD_SCENE_H__
