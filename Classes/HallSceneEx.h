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
	string		_LoginKey;		//��½��Կ
	EditBox*	_editDesk;		//����
	bool		_exit = false;	//�Ƿ����˳�
	mutex		_muxHall;

	ui::Button* _btAdd;		//��������
	ui::Button* _btFastAdd;	//���ټ���
	ui::Button* _btCreate;	//��������
	ui::Button*	_btClose;	//�ر���Ϸ

	Label*		_infoOnlien;  //��������
	Label*		_infoMessage; //��ʾ��Ϣ
	Label*		_infoRank;	//������Ϣ
	Label*		_infoSelf;	//�Լ���Ϣ

	int			_nCounts = 0;
};

#endif // __HELLOWORLD_SCENE_H__
