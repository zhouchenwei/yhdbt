#ifndef _H_CLOGINSCENE_H_
#define _H_CLOGINSCENE_H_

#include "cocos2d.h"
#include "UI/UIButton.h"
#include "UI/UIEditBox/UIEditBox.h"
#include <string>
using namespace std;
USING_NS_CC;

class CLoginScene : public cocos2d::Layer, public ui::EditBoxDelegate
{
public:
	CLoginScene();
	~CLoginScene();

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CLoginScene);

private:
	void editBoxReturn(ui::EditBox* editBox);
	void OnLogin(Ref *pSender, ui::Widget::TouchEventType type);
	void OnRegist(Ref *pSender, ui::Widget::TouchEventType type);

	bool CheckUserPass(const string& user, const string& pass);
	
	void LoginSchedule(float dt);

private:
	ui::EditBox* _editBoxUser;
	ui::EditBox* _editBoxPass;
	ui::Button* _btOnLogin;
	ui::Button* _btOnRegist;
	Label*		_infoLabel;
	std::mutex	_muxLogin;
};

#endif 
