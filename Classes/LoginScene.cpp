#include "LoginScene.h"
#include "ResourceManager.h"
#include "MessageQueue.h"
#include "CommDelegate.h"
#include "HttpQueue.h"
#include "HallSceneEx.h"
using namespace ui;

CLoginScene::CLoginScene()
{
}


CLoginScene::~CLoginScene()
{
}

cocos2d::Scene* CLoginScene::createScene()
{
	auto _scene = Scene::create();
	auto layer = CLoginScene::create();
	_scene->addChild(layer);
	return _scene;
}

bool CLoginScene::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//±³¾°Í¼Æ¬
	Sprite* bj = Sprite::create();
	bj->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("loginbeijing"));
	bj->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bj, -1);

	//ÓÃ»§Ãû£¬ÃÜÂë¿ò
	_editBoxUser = ui::EditBox::create(Size(350, 55), ui::Scale9Sprite::create());
	_editBoxUser->setPosition(Point(visibleSize.width / 2+75, 292));
	_editBoxUser->setFontSize(40);
	_editBoxUser->setMaxLength(11);
	_editBoxUser->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_WORD);
	_editBoxUser->setInputMode(EditBox::InputMode::NUMERIC);
	_editBoxUser->setInputMode(EditBox::InputMode::SINGLE_LINE);
	_editBoxUser->setFontColor(Color3B::WHITE);
	_editBoxUser->setDelegate(this);
	string user = UserDefault::getInstance()->getStringForKey("loginuser");
	if (!user.empty())
		_editBoxUser->setText(user.c_str());
	this->addChild(_editBoxUser, 2);
	

	_editBoxPass = ui::EditBox::create(Size(350, 55), ui::Scale9Sprite::create());
	_editBoxPass->setPosition(Point(visibleSize.width/2+ 75, 210));
	_editBoxUser->setFontSize(40);
	_editBoxPass->setMaxLength(16);
	_editBoxPass->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_WORD);
	_editBoxPass->setInputMode(EditBox::InputMode::SINGLE_LINE);
	_editBoxPass->setInputFlag(EditBox::InputFlag::PASSWORD);
	_editBoxPass->setFontColor(Color3B::WHITE);
	_editBoxPass->setDelegate(this);
	string pass = UserDefault::getInstance()->getStringForKey("loginpass");
	if (!pass.empty())
		_editBoxPass->setText(pass.c_str());
	this->addChild(_editBoxPass, 2);

	//µÇÂ½£¬×¢²á°´Å¥
	_btOnRegist = ui::Button::create("pic/btlogin.png");
	_btOnRegist->setPosition(Vec2(618, 138));
	_btOnRegist->addTouchEventListener(CC_CALLBACK_2(CLoginScene::OnLogin, this));
	_btOnRegist->setEnabled(true);
	this->addChild(_btOnRegist, 1);

	_btOnLogin = ui::Button::create("pic/btregist.png");
	_btOnLogin->setPosition(Vec2(795, 138));
	_btOnLogin->addTouchEventListener(CC_CALLBACK_2(CLoginScene::OnRegist, this));
	_btOnLogin->setEnabled(true);
	this->addChild(_btOnLogin, 1);

	//ÌáÊ¾ÐÅÏ¢
	_infoLabel = Label::createWithTTF("", "fonts/msyhbd.ttc", 30);
	_infoLabel->setColor(Color3B::WHITE);
	_infoLabel->setString("Ver: 6.1.1008");
	_infoLabel->setPosition(visibleSize.width / 2,49);
	this->addChild(_infoLabel, 2);
	
	//¿ªÆôµ÷¶È
	this->schedule(CC_SCHEDULE_SELECTOR(CLoginScene::LoginSchedule), float(0.01));
	httpQuery::instance()->Version();
	return true;
}

void CLoginScene::editBoxReturn(ui::EditBox* editBox)
{
	_infoLabel->setString("");
}

void CLoginScene::OnLogin(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex> ul(_muxLogin);
	if (type == ui::Widget::TouchEventType::ENDED) {
		_btOnLogin->setEnabled(false);
		_btOnRegist->setEnabled(false);
		_editBoxUser->setEnabled(false);
		_editBoxPass->setEnabled(false);

		string user = _editBoxUser->getText();
		string pass = _editBoxUser->getText();
		if (CheckUserPass(user, pass)) {
			//±£´æÕËºÅÃÜÂë  
			UserDefault::getInstance()->setStringForKey("loginuser", user);
			UserDefault::getInstance()->setStringForKey("loginpass", pass);
			_infoLabel->setString(Configuration::getInstance()->getValue("login").asString());
			httpQuery::instance()->Login(commDele::instance()->ToLogin(user, pass));
			return;
		}
		else
			_infoLabel->setString(Configuration::getInstance()->getValue("user").asString());
	}
	_btOnLogin->setEnabled(true);
	_btOnRegist->setEnabled(true);
	_editBoxUser->setEnabled(true);
	_editBoxPass->setEnabled(true);
}

void CLoginScene::OnRegist(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex> ul(_muxLogin);
	if (type == ui::Widget::TouchEventType::ENDED) {
		_btOnRegist->setEnabled(false);
		_btOnLogin->setEnabled(false);
		_infoLabel->setString(Configuration::getInstance()->getValue("regist").asString());
		_btOnRegist->setEnabled(true);
		_btOnLogin->setEnabled(true);
	}
}

bool CLoginScene::CheckUserPass(const string& user, const string& pass)
{	
	if (user.length() != 11 || pass.length() < 6 || pass.length() > 16)
		return false;

	for (size_t i=0; i<user.length(); i++){
		if (!isalnum(user[i]))
			return false;
	}
	return true;
}

void CLoginScene::LoginSchedule(float dt)
{
	auto ptr = messageQueue::instance()->getMessage();
	//µÇÂ½ÓÎÏ·
	if (ptr && ptr->opt == "login") {
		do 
		{
			if (ptr->message == "error") {
				_infoLabel->setString(Configuration::getInstance()->getValue("net_error").asString());
				break;
			}
			if (ptr->message == "login_error") {
				_infoLabel->setString(Configuration::getInstance()->getValue("login_error").asString());
				break;
			}
			messageQueue::instance()->start();

			// ÇÐ»»µ½´óÌü³¡¾°
			Scene *pScene = CHallSceneEx::createScene(ptr->message);
			Director::getInstance()->replaceScene(pScene);

		} while (false);
		_btOnLogin->setEnabled(true);
		_btOnRegist->setEnabled(true);
		_editBoxUser->setEnabled(true);
		_editBoxPass->setEnabled(true);
	}
}
