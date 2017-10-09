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


//玩家类
typedef struct playerExtensionInfo{
	stplayinfo playerInfo;

	Label*	mheaderPic;	//头像
	Label*	mnick;		//昵称
	Label*	mrecord;	//战绩
	Label*	msurplus;	//剩余手牌
	Label*	mtimer;		//倒计时
	Sprite* mready;		//是否准备
	Label*	mgone;		//是否出完
	Label*	mpass;		//不出
	Point	pos;		//出牌的中心

	chrono::time_point<chrono::system_clock> timepoint; //倒计时
	vector<CardSprite*> perCards;	//之前出的牌

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
		//调整座位号
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

	void    perPutCards(msgptr ptr); //处理上一家
	void	nowPutCards(msgptr ptr); //处理当前家

	int		calcuLen(int counts); //计算出牌的位置

private:
	//逻辑相关
	int			_deskId;			//桌号
	int			_siteId;			//主玩家座位号
	int			_baseScore = 10;	//基础分
	bool		_isSatrting = false;//游戏是否进行中
	bool		_exit = false;		//是否强制退出桌子
	bool		_must = false;		//是否必须出牌
	Size		_visibleSize;		//窗口尺寸
	mutex		_muxButton;			//按钮互斥锁
	stplayinfo	_playinfo;
	int			_nowPut = -1;		//当前出牌座位号
	CardSprite*	_perSelectedCard = nullptr;	//之前选中的牌（防止选择时牌抖动）

	map<int, playerPtr>	_mapPlayers;//座位号->玩家
	vector<playerPtr>	_vecPlayers;//初始化的时候放在这里
	vector<CardSprite*>	_vecCards;	//自己手牌(只有自己有)
	vector<int>			_vecPerCards; //上一次出的牌

	//图片相关
	ui::Button* _btReturn;
	ui::Button* _btReady;
	ui::Button* _btNoput;
	ui::Button* _btPut;

	Label*	_labelDeskNum;		//桌号
	Label*  _labelDeskScore;	//桌面分数
	Label*	_labelBaseScore;	//底分
	Label*	_label0Score;		//0方得分
	Label*	_label1Score;		//1方得分
	Label*	_labelInfo;			//结果,提示等
	Label*	_labelOnline;		//在线人数
};

#endif // __HELLOWORLD_SCENE_H__
