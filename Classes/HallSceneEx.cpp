#include "HallSceneEx.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "DeskScene.h"
#include "MessageQueue.h"
#include "CommonFunction.h"
#include "TcpConnect.h"
#include "CommDelegate.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* CHallSceneEx::createScene(const string& loginkey)
{	
    auto _scene = Scene::create();
    auto layer = CHallSceneEx::create();
	layer->SetHallInfo(loginkey);		
	_scene->addChild(layer);
    return _scene;
}

bool CHallSceneEx::init()
{
    if (!Layer::init())
        return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//背景音乐 背景图片
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bj.mp3", true);

	Sprite* bjpic = Sprite::create();
	bjpic->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hallbeijing"));
	bjpic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bjpic,-1);

	//关闭按钮
	_btClose = ui::Button::create("pic/close.png");
	_btClose->setPosition(Vec2(visibleSize.width-66, visibleSize.height-66));
	_btClose->addTouchEventListener(CC_CALLBACK_2(CHallSceneEx::OnClose, this));
	_btClose->setEnabled(true);
	this->addChild(_btClose);

	//输入桌号
	_editDesk = EditBox::create(Size(200, 55), ui::Scale9Sprite::create());
	_editDesk->setPosition(Vec2(643, 455));
	_editDesk->setMaxLength(2);
	_editDesk->setInputMode(EditBox::InputMode::NUMERIC);
	_editDesk->setInputMode(EditBox::InputMode::SINGLE_LINE);
	_editDesk->setFontColor(Color3B::WHITE);
	_editDesk->setDelegate(this);
	this->addChild(_editDesk, 2);

	//加入按钮
	_btAdd = ui::Button::create("pic/add.png");
	_btAdd->setPosition(Vec2(831, 452));
	_btAdd->addTouchEventListener(CC_CALLBACK_2(CHallSceneEx::OnAddDesk, this));
	_btAdd->setEnabled(true);
	this->addChild(_btAdd);

	//创建桌子按钮
	_btCreate = ui::Button::create("pic/create.png");
	_btCreate->setPosition(Vec2(998, 451));
	_btCreate->addTouchEventListener(CC_CALLBACK_2(CHallSceneEx::OnCreateDesk, this));
	_btCreate->setEnabled(true);
	this->addChild(_btCreate);

	//快速加入按钮 
	_btFastAdd = ui::Button::create("pic/fastadd.png");
	_btFastAdd->setPosition(Vec2(1167, 451));
	_btFastAdd->addTouchEventListener(CC_CALLBACK_2(CHallSceneEx::OnFastAddDesk, this));
	_btFastAdd->setEnabled(true);
	this->addChild(_btFastAdd);

	//标签-在线人数
	_infoOnlien = Label::createWithTTF(Configuration::getInstance()->getValue("online").asString(), "fonts/msyhbd.ttc", 30);
	_infoOnlien->setColor(Color3B::WHITE);
	_infoOnlien->setPosition(Vec2(visibleSize.width-115, 138));
	this->addChild(_infoOnlien);
	//标签-提示信息
	_infoMessage = Label::createWithTTF(Configuration::getInstance()->getValue("hall_info").asString(), "fonts/msyhbd.ttc", 30);
	_infoMessage->setColor(Color3B::WHITE);
	_infoMessage->setPosition(Vec2(840, 57));
	this->addChild(_infoMessage);
	//标签-排行信息
	_infoRank = Label::createWithTTF(Configuration::getInstance()->getValue("test_rank").asString(), "fonts/msyhbd.ttc", 30);
	_infoRank->setColor(Color3B::WHITE);
	_infoRank->setPosition(Vec2(800, 100));
	this->addChild(_infoRank);
	//标签-自己信息
	_infoSelf = Label::createWithTTF("", "fonts/msyhbd.ttc", 30);
	_infoSelf->setColor(Color3B::WHITE);
	_infoSelf->setPosition(Vec2(365, 600));
	this->addChild(_infoSelf);

	//开启调度
	this->schedule(CC_SCHEDULE_SELECTOR(CHallSceneEx::HallSchedule), float(0.1));
    return true;
}

void CHallSceneEx::SetHallInfo(const string& loginkey)
{
	_LoginKey = loginkey;
}

void CHallSceneEx::OnFastAddDesk(Ref *pSender, ui::Widget::TouchEventType type)
{	
	unique_lock<mutex> lk(_muxHall);
	if (type == ui::Widget::TouchEventType::ENDED) {
		//不允许再加入其他
		buttonEnable(false);
		tcpConn::instance()->SendQuery(commDele::instance()->ToFastAdd());
	}
}

void CHallSceneEx::OnAddDesk(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex> lk(_muxHall);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);
		const char* pinfo = _editDesk->getText();
		if (strlen(pinfo) == 0){
			_infoMessage->setString(Configuration::getInstance()->getValue("desk_error").asString());
			buttonEnable(true);
			return;
		}
		_infoMessage->setString(Configuration::getInstance()->getValue("add_desk").asString());
		int deskId = atoi(_editDesk->getText());
		tcpConn::instance()->SendQuery(commDele::instance()->ToAdd(deskId));
	}
}

void CHallSceneEx::OnCreateDesk(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex> lk(_muxHall);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);
		tcpConn::instance()->SendQuery(commDele::instance()->ToCreate());
	}
}

void CHallSceneEx::OnClose(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex> lk(_muxHall);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);
		if (_exit) {
			exit(0);
		}
		_exit = true;
		_infoMessage->setString(Configuration::getInstance()->getValue("exit_message").asString());
	}
	buttonEnable(true);
}

void CHallSceneEx::HallSchedule(float dt)
{
	if (_nCounts == 0){
		if (!_LoginKey.empty())
			tcpConn::instance()->SendQuery(commDele::instance()->ToGame(_LoginKey));
		else
			tcpConn::instance()->SendQuery(commDele::instance()->ToQueryInfo());

		tcpConn::instance()->SendQuery(commDele::instance()->ToQueryRank());
	}
	if (_nCounts % 300 == 0)
		tcpConn::instance()->SendQuery(commDele::instance()->ToQueryCounts());
	_nCounts++;
	//在线人数
	auto ptr = messageQueue::instance()->getMessage();
	if (ptr && ptr->opt == "online") 
		_infoOnlien->setString(ptr->hallmsg.oninle);
	//自己信息
	if (ptr && ptr->opt == "login"){
		char buf[512] = { 0 };
		int n = sprintf(buf, Configuration::getInstance()->getValue("fmt_self").asString().c_str(),
			ptr->hallmsg.self.nick.c_str(), ptr->hallmsg.self.score, ptr->hallmsg.self.win,
			ptr->hallmsg.self.lose, ptr->hallmsg.self.he, ptr->hallmsg.self.run);
		_infoSelf->setString(string(buf, n));
	}
	//排行
	if (ptr && ptr->opt == "rank") {
		_infoRank->setString(ptr->message);
	}
	//加入桌子
	if (ptr && ptr->opt == "add") {
		if (ptr->message != "error"){
			Scene *pScene = CDeskScene::createScene(
				ptr->deskmsg.desk, ptr->deskmsg.site, ptr->deskmsg.self);
			Director::getInstance()->replaceScene(pScene);
		}
		_infoMessage->setString(Configuration::getInstance()->getValue("desk_error").asString().c_str());
		buttonEnable(true);
	}
}

void CHallSceneEx::editBoxReturn(ui::EditBox* editBox){}

void CHallSceneEx::buttonEnable(bool enable)
{
	_btFastAdd->setEnabled(enable);
	_btAdd->setEnabled(enable);
	_btCreate->setEnabled(enable);
	_btClose->setEnabled(enable);
}
