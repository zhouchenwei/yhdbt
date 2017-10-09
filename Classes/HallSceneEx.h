#ifndef _H_HALLSCENEEX_H_
#define _H_HALLSCENEEX_H_
#include <string>
#include <sstream>
#include "cocos2d.h"
#include "UI/UIButton.h"
#include "UI/UIEditBox/UIEditBox.h"
#include "MessageQueue.h"
USING_NS_CC;
using namespace std;
using namespace ui;

class CHallSceneEx : public cocos2d::Layer, public EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene(const string& loginkey="");

    virtual bool init();
   
    CREATE_FUNC(CHallSceneEx);

	void	SetHallInfo(const string& loginkey);

	void	OnFastAddDesk(Ref *pSender, ui::Widget::TouchEventType type);
	void	OnAddDesk(Ref *pSender, ui::Widget::TouchEventType type);
	void	OnCreateDesk(Ref *pSender, ui::Widget::TouchEventType type);
	void	OnClose(Ref *pSender, ui::Widget::TouchEventType type);

	void	HallSchedule(float dt);

protected:
	void	editBoxReturn(ui::EditBox* editBox);
	void	buttonEnable(bool enable);

private:
	string		_LoginKey;		//登陆密钥
	EditBox*	_editDesk;		//桌子
	bool		_exit = false;	//是否想退出
	mutex		_muxHall;

	ui::Button* _btAdd;		//加入桌子
	ui::Button* _btFastAdd;	//快速加入
	ui::Button* _btCreate;	//创建桌子
	ui::Button*	_btClose;	//关闭游戏

	Label*		_infoOnlien;  //在线人数
	Label*		_infoMessage; //提示信息
	Label*		_infoRank;	//排行信息
	Label*		_infoSelf;	//自己信息

	int			_nCounts = 0;
};

#endif // __HELLOWORLD_SCENE_H__
