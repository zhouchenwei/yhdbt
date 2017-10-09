#include "DeskScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "ui/UIButton.h"
#include "HallSceneEx.h"
#include "DBTRule.h"
#include "CommonFunction.h"
#include "CommDelegate.h"
#include "TcpConnect.h"
#include <chrono>
using namespace CocosDenshion;
using namespace ui;
USING_NS_CC;

#define	def_cardsep  22
#define def_max_cardsep 32
#define def_selected_y 70
#define def_no_selected_y 45
#define def_selected_y_len 25

double gSelfDEF_cardsep = 22;

Scene* CDeskScene::createScene(int desk, int site, const stplayinfo& info)
{
	auto _scene = Scene::create();
    auto layer = CDeskScene::create();
	layer->setDeskInfo(desk, site, info);
	_scene->addChild(layer);
    return _scene;
}

bool CDeskScene::init()
{
    if (!Layer::init())
        return false;

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	_visibleSize = Director::getInstance()->getVisibleSize();
	
	//背景
	Sprite* bj = Sprite::create();
	bj->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("deskbeijing"));
	bj->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));
	this->addChild(bj);

	//返回按钮
	_btReturn = ui::Button::create("pic/close.png");
	_btReturn->setPosition(Point(_visibleSize.width - 66, _visibleSize.height - 66));
	_btReturn->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReturn, this));
	_btReturn->setEnabled(true);
	this->addChild(_btReturn);
	
	//出牌按钮
	_btPut = ui::Button::create("pic/chupai.png");
	_btPut->setPosition(Vec2(_visibleSize.width / 2 + 200, 180));
	_btPut->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onPut, this));
	_btPut->setVisible(false);
	_btPut->setEnabled(true);
	this->addChild(_btPut, 2);

	//准备按钮
	_btReady = ui::Button::create("pic/zhunbei.png");
	_btReady->setPosition(Vec2(_visibleSize.width / 2, 250));
	_btReady->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReady, this));
	_btReady->setEnabled(true);
	this->addChild(_btReady, 2);

	//不出按钮
	_btNoput = ui::Button::create("pic/buchu.png");
	_btNoput->setPosition(Vec2(_visibleSize.width / 2+350, 180));
	_btNoput->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onNoput, this));
	_btNoput->setVisible(false);
	_btNoput->setEnabled(true);
	this->addChild(_btNoput, 2);

	//桌号标签
	_labelDeskNum = Label::createWithTTF(to_string(_deskId), "fonts/msyhbd.ttc", 26);
	_labelDeskNum->setPosition(Vec2(94, _visibleSize.height-29));
	_labelDeskNum->setColor(Color3B::WHITE);
	this->addChild(_labelDeskNum);
	
	//底分标签
	_labelBaseScore = Label::createWithTTF(to_string(_baseScore), "fonts/msyhbd.ttc", 26);
	_labelBaseScore->setPosition(Vec2(205, _visibleSize.height - 29));
	_labelBaseScore->setColor(Color3B::WHITE);
	this->addChild(_labelBaseScore);

	//我方得分标签
	_label0Score = Label::createWithTTF("0", "fonts/msyhbd.ttc", 26);
	_label0Score->setPosition(Vec2(330, _visibleSize.height - 29));
	_label0Score->setColor(Color3B::WHITE);
	this->addChild(_label0Score);

	//对方得分标签
	_label1Score = Label::createWithTTF("0", "fonts/msyhbd.ttc", 26);
	_label1Score->setPosition(Vec2(460, _visibleSize.height - 29));
	_label1Score->setColor(Color3B::WHITE);
	this->addChild(_label1Score);

	//桌面分标签
	_labelDeskScore = Label::createWithTTF("0", "fonts/msyhbd.ttc", 26);
	_labelDeskScore->setPosition(Vec2(_visibleSize.width/2-16, _visibleSize.height/2+30));
	_labelDeskScore->setColor(Color3B::WHITE);
	this->addChild(_labelDeskScore);

	//提示标签
	_labelInfo = Label::createWithTTF(Configuration::getInstance()->getValue("desk_welcom").asString(), "fonts/msyhbd.ttc", 26);
	_labelInfo->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2-30));
	_labelInfo->setColor(Color3B::WHITE);
	this->addChild(_labelInfo);

	//在线人数
	_labelOnline = Label::createWithTTF("0", "fonts/msyhbd.ttc", 28);
	_labelOnline->setPosition(Vec2(_visibleSize.width-170, _visibleSize.height - 28));
	_labelOnline->setColor(Color3B::WHITE);
	this->addChild(_labelOnline);

	_vecPlayers.clear();
	//玩家信息-自己
	playerPtr ptr = make_shared<stPlayerExtensionInfo>();
	ptr->mnick = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mnick->setPosition(Point(_visibleSize.width/2 - 200, 35));
	ptr->mnick->setColor(Color3B::BLACK);
	this->addChild(ptr->mnick, 30);
	ptr->msurplus = Label::createWithTTF("0", "fonts/msyhbd.ttc", 40);
	ptr->msurplus->setVisible(false);
	this->addChild(ptr->msurplus, 30);
	ptr->mrecord = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mrecord->setPosition(Point(_visibleSize.width / 2 + 200, 35));
	ptr->mrecord->setColor(Color3B(78, 185, 211));
	this->addChild(ptr->mrecord, 30);
	ptr->mtimer = Label::createWithTTF("30", "fonts/msyhbd.ttc", 30);
	ptr->mtimer->setPosition(Point(_visibleSize.width / 2 + 300, 250));
	ptr->mtimer->setVisible(false);
	this->addChild(ptr->mtimer, 2);
	ptr->mready = Sprite::create();
	ptr->mready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ready"));
	ptr->mready->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 - 120));
	ptr->mready->setVisible(false);
	ptr->mready->setScale(0.5);
	this->addChild(ptr->mready);
	ptr->mgone = Label::createWithTTF(Configuration::getInstance()->getValue("over").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mgone->setColor(Color3B(255, 162, 56));
	ptr->mgone->setPosition(Vec2(_visibleSize.width/2, _visibleSize.height/2 - 120));
	ptr->mgone->setVisible(false);
	this->addChild(ptr->mgone, 20);
	ptr->mpass = Label::createWithTTF(Configuration::getInstance()->getValue("pass").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mpass->setColor(Color3B(255, 162, 56));
	ptr->mpass->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 - 120));
	ptr->mpass->setVisible(false);
	this->addChild(ptr->mpass);
	ptr->pos = Point(_visibleSize.width / 2, 200);
	_vecPlayers.emplace_back(ptr);
	
	//玩家信息-下家
	ptr = make_shared<stPlayerExtensionInfo>();
	ptr->mnick = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mnick->setPosition(Point(_visibleSize.width-140, _visibleSize.height/2));
	ptr->mnick->setColor(Color3B::BLACK);
	this->addChild(ptr->mnick, 30);
	ptr->mrecord = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mrecord->setPosition(Point(_visibleSize.width-140, _visibleSize.height/2-40));
	ptr->mrecord->setColor(Color3B(78, 185, 211));
	this->addChild(ptr->mrecord, 30);
	ptr->msurplus = Label::createWithTTF("0", "fonts/msyhbd.ttc", 40);
	ptr->msurplus->setPosition(Vec2(_visibleSize.width-120, _visibleSize.height/2+70));
	ptr->msurplus->setColor(Color3B::WHITE);
	ptr->msurplus->setVisible(false);
	this->addChild(ptr->msurplus, 30);
	ptr->mtimer = Label::createWithTTF("30", "fonts/msyhbd.ttc", 30);
	ptr->mtimer->setPosition(Point(_visibleSize.width-120, _visibleSize.height/2+120));
	ptr->mtimer->setVisible(false);
	this->addChild(ptr->mtimer, 30);
	ptr->mready = Sprite::create();
	ptr->mready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ready"));
	ptr->mready->setPosition(Vec2(_visibleSize.width-300, _visibleSize.height/2+70));
	ptr->mready->setVisible(false);
	ptr->mready->setScale(0.5);
	this->addChild(ptr->mready);
	ptr->mgone = Label::createWithTTF(Configuration::getInstance()->getValue("over").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mgone->setColor(Color3B(255, 162, 56));
	ptr->mgone->setPosition(Vec2(_visibleSize.width - 300, _visibleSize.height / 2 + 70));
	ptr->mgone->setVisible(false);
	this->addChild(ptr->mgone, 20);
	ptr->mpass = Label::createWithTTF(Configuration::getInstance()->getValue("pass").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mpass->setColor(Color3B(255, 162, 56));
	ptr->mpass->setPosition(Vec2(_visibleSize.width - 300, _visibleSize.height / 2 + 70));
	ptr->mpass->setVisible(false);
	this->addChild(ptr->mpass);
	ptr->pos = Point(_visibleSize.width - 400, _visibleSize.height / 2 + 50);
	_vecPlayers.emplace_back(ptr);

	//玩家信息-对家
	ptr = make_shared<stPlayerExtensionInfo>();
	ptr->mnick = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mnick->setPosition(Point(_visibleSize.width / 2+80, _visibleSize.height-35));
	ptr->mnick->setColor(Color3B::BLACK);
	this->addChild(ptr->mnick, 30);
	ptr->mrecord = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mrecord->setPosition(Point(_visibleSize.width / 2 + 80, _visibleSize.height - 75));
	ptr->mrecord->setColor(Color3B(78, 185, 211));
	this->addChild(ptr->mrecord, 30);
	ptr->msurplus = Label::createWithTTF("0", "fonts/msyhbd.ttc", 40);
	ptr->msurplus->setPosition(Vec2(_visibleSize.width/2-20, _visibleSize.height-120));
	ptr->msurplus->setColor(Color3B::WHITE);
	ptr->msurplus->setVisible(false);
	this->addChild(ptr->msurplus, 30);
	ptr->mtimer = Label::createWithTTF("30", "fonts/msyhbd.ttc", 30);
	ptr->mtimer->setPosition(Point(_visibleSize.width / 2+100, _visibleSize.height - 120));
	ptr->mtimer->setVisible(false);
	this->addChild(ptr->mtimer, 30);
	ptr->mready = Sprite::create();
	ptr->mready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ready"));
	ptr->mready->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2+ 150));
	ptr->mready->setVisible(false);
	ptr->mready->setScale(0.5);
	this->addChild(ptr->mready);
	ptr->mgone = Label::createWithTTF(Configuration::getInstance()->getValue("over").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mgone->setColor(Color3B(255, 162, 56));
	ptr->mgone->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 + 150));
	ptr->mgone->setVisible(false);
	this->addChild(ptr->mgone, 20);
	ptr->mpass = Label::createWithTTF(Configuration::getInstance()->getValue("pass").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mpass->setColor(Color3B(255, 162, 56));
	ptr->mpass->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 + 150));
	ptr->mpass->setVisible(false);
	this->addChild(ptr->mpass);
	ptr->pos = Point(_visibleSize.width / 2, _visibleSize.height / 2 + 160);
	_vecPlayers.emplace_back(ptr);

	//玩家信息-下家
	ptr = make_shared<stPlayerExtensionInfo>();
	ptr->mnick = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mnick->setPosition(Point(140, _visibleSize.height/2));
	ptr->mnick->setColor(Color3B::BLACK);
	this->addChild(ptr->mnick, 30);
	ptr->mrecord = Label::createWithTTF("", "fonts/msyhbd.ttc", 26);
	ptr->mrecord->setPosition(Point(140, _visibleSize.height / 2 - 40));
	ptr->mrecord->setColor(Color3B(78, 185, 211));
	this->addChild(ptr->mrecord, 30);
	ptr->msurplus = Label::createWithTTF("0", "fonts/msyhbd.ttc", 40);
	ptr->msurplus->setPosition(Vec2(120, _visibleSize.height / 2 + 70));
	ptr->msurplus->setColor(Color3B::WHITE);
	ptr->msurplus->setVisible(false);
	this->addChild(ptr->msurplus, 30);
	ptr->mtimer = Label::createWithTTF("30", "fonts/msyhbd.ttc", 30);
	ptr->mtimer->setPosition(Point(120, _visibleSize.height / 2 + 120));
	ptr->mtimer->setVisible(false);
	this->addChild(ptr->mtimer, 30);
	ptr->mready = Sprite::create();
	ptr->mready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ready"));
	ptr->mready->setPosition(Vec2(300, _visibleSize.height / 2 + 70));
	ptr->mready->setVisible(false);
	ptr->mready->setScale(0.5);
	this->addChild(ptr->mready);
	ptr->mgone = Label::createWithTTF(Configuration::getInstance()->getValue("over").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mgone->setColor(Color3B(255, 162, 56));
	ptr->mgone->setPosition(Vec2(300, _visibleSize.height / 2 + 70));
	ptr->mgone->setVisible(false);
	this->addChild(ptr->mgone, 20);
	ptr->mpass = Label::createWithTTF(Configuration::getInstance()->getValue("pass").asString(), "fonts/msyhbd.ttc", 50);
	ptr->mpass->setColor(Color3B(255, 162, 56));
	ptr->mpass->setPosition(Vec2(300, _visibleSize.height / 2 + 70));
	ptr->mpass->setVisible(false);
	this->addChild(ptr->mpass);
	ptr->pos = Point(400, _visibleSize.height / 2 + 50);
	_vecPlayers.emplace_back(ptr);

	//注册触摸事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CDeskScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CDeskScene::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//调度处理
	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::deskSchedule), float(0.01));
	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::timeSchedule), 1.0);

    return true;
}

void CDeskScene::onReturn(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex>	lk(_muxButton);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);
		if (_isSatrting && !_exit){
			_exit = true;
			_labelInfo->setString(Configuration::getInstance()->getValue("exit_message").asString());
			buttonEnable(true);
			return;
		}
		tcpConn::instance()->SendQuery(commDele::instance()->ToLeave(_deskId, _siteId));
		Scene *pScene = CHallSceneEx::createScene();
		Director::getInstance()->replaceScene(pScene);
	}
}

void CDeskScene::onReady(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex>	lk(_muxButton);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);
		_labelInfo->setString("");
		SimpleAudioEngine::getInstance()->playEffect("sound/zb.mp3");
		_btReady->setVisible(false);
		_mapPlayers[_siteId]->mready->setVisible(true);
		_label0Score->setString("0");
		_label1Score->setString("0");
		_labelDeskScore->setString("0");
		clearDesk(-1);
		tcpConn::instance()->SendQuery(commDele::instance()->ToReady(_deskId, _siteId));
	}
}

void CDeskScene::onNoput(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex>	lk(_muxButton);
	if (type == ui::Widget::TouchEventType::ENDED) {
		if (_must) {
			_labelInfo->setString(Configuration::getInstance()->getValue("mustput").asString());
			return;
		}
		_btPut->setVisible(false);
		_btNoput->setVisible(false);
		buttonEnable(false);
		tcpConn::instance()->SendQuery(commDele::instance()->ToPut(_deskId, _siteId, vector<int>()));
	}
}

void CDeskScene::onPut(Ref *pSender, ui::Widget::TouchEventType type)
{
	unique_lock<mutex>	lk(_muxButton);
	if (type == ui::Widget::TouchEventType::ENDED) {
		buttonEnable(false);

		vector<int> vecSels;
		getSelectedCards(vecSels);
		if (vecSels.empty()) {
			buttonEnable(true);
			return;
		}

		auto r = CDBTRule::isBigger(_vecPerCards, vecSels);
		if (r.first) {
			_btPut->setVisible(false);
			_btNoput->setVisible(false);
			_vecPerCards = vecSels;
			//整理手牌
			for (const auto&it : vecSels) {
				auto itper = _vecCards.begin();
				while (itper != _vecCards.end()) {
					if ((*itper)->getSeq() == it) {
						this->removeChild(*itper);
						_vecCards.erase(itper);
						break;
					}
					++itper;
				}
			}
			if (_vecCards.size() > 0 && (_visibleSize.width-50) / _vecCards.size() < def_max_cardsep)
				gSelfDEF_cardsep = (_visibleSize.width - 50) / _vecCards.size();
			int i = 0;
			for (auto &it : _vecCards) {
				it->setPosition(Vec2(_visibleSize.width / 2 - (_vecCards.size() - 1) * gSelfDEF_cardsep / 2 + i * gSelfDEF_cardsep + it->getContentSize().width / 4, def_no_selected_y));
				i++;
			}
			//发送信息
			tcpConn::instance()->SendQuery(commDele::instance()->ToPut(_deskId, _siteId, vecSels));
		}
		else {
			SimpleAudioEngine::getInstance()->playEffect("sound/cw.wav");
			buttonEnable(true);
		}
	}
}

bool CDeskScene::onTouchBegan(Touch *touch, Event *event)
{
	Point p = touch->getLocation();
	for (int i = _vecCards.size(); i > 0; i--) {
		CardSprite* card = _vecCards[i - 1];
		Rect recv = card->getBoundingBox();
		if (recv.containsPoint(p)) {
			_perSelectedCard = card;
			if (card->getSelect()) {
				card->setSelect(false);
				card->setPosition(Vec2(card->getPositionX(), card->getPositionY() - def_selected_y_len));
			}
			else {
				card->setSelect(true);
				card->setPosition(Vec2(card->getPositionX(), card->getPositionY() + def_selected_y_len));
			}
			break;
		}
	}
	return true;
}

void CDeskScene::onTouchMoved(Touch *touch, Event *event)
{
	unique_lock<mutex>	lk(_muxButton);
	Point p = touch->getLocation();
	
	for (size_t i = _vecCards.size(); i > 0; i--) {
		CardSprite* card = _vecCards[i - 1];
		Rect recv = card->getBoundingBox();
		if (recv.containsPoint(p)) {
			if (_perSelectedCard != card){
				_perSelectedCard = card;
				if (card->getSelect()) {
					card->setSelect(false);
					card->setPosition(Vec2(card->getPositionX(), card->getPositionY() - def_selected_y_len));
				}
				else {
					card->setSelect(true);
					card->setPosition(Vec2(card->getPositionX(), card->getPositionY() + def_selected_y_len));
				}
			}
			break;
		}
	}
}

void CDeskScene::deskSchedule(float dt)
{
	static int counts = 0;
	if (counts % 2000 == 100) {
		tcpConn::instance()->SendQuery(commDele::instance()->ToQueryCounts());
	}
	
	counts++;
	auto ptr = messageQueue::instance()->getMessage();

	if (ptr == nullptr)
		return;

	buttonEnable(true);
	//在线人数
	if (ptr && ptr->opt == "online")
		_labelOnline->setString(ptr->hallmsg.oninle);
	//人员变更
	if (ptr && ptr->opt == "change" && !_isSatrting) {
		for (size_t i = 0; i < 4; i++) {
			//走了
			if (ptr->deskmsg.mapPlayers[i]->nick.empty()) {
				clearDesk(i);
				_mapPlayers[i]->mrecord->setString("");
				_mapPlayers[i]->mnick->setString("");
			}
			else {
				//昵称
				char buf[256] = { 0 };
				int n = sprintf(buf, Configuration::getInstance()->getValue("plyaerinfo").asString().c_str(),
					ptr->deskmsg.mapPlayers[i]->nick.c_str(), ptr->deskmsg.mapPlayers[i]->score);
				_mapPlayers[i]->mnick->setString(string(buf, n));
				//得分
				memset(buf, 0, 256);
				n = sprintf(buf, Configuration::getInstance()->getValue("plyaerzj").asString().c_str(),
					ptr->deskmsg.mapPlayers[i]->win, ptr->deskmsg.mapPlayers[i]->lose, 
					ptr->deskmsg.mapPlayers[i]->he, ptr->deskmsg.mapPlayers[i]->run);
				_mapPlayers[i]->mrecord->setString(string(buf, n));
				if (ptr->deskmsg.mapPlayers[i]->ready == 1)
					_mapPlayers[i]->mready->setVisible(true);
				else
					_mapPlayers[i]->mready->setVisible(false);
			}
		}
	}
	//开始
	if (ptr && ptr->opt == "start" && !_isSatrting){
		gameSatrt();
		SimpleAudioEngine::getInstance()->playEffect("sound/bg.wav");
		//显示手牌
		vector<string> ctmp;
		stringToVector(ptr->deskmsg.cards, ctmp, ",");
		vector<int> _vecStartCards;
		for (auto it : ctmp)
			_vecStartCards.emplace_back(atoi(it.c_str()));
		sort(_vecStartCards.begin(), _vecStartCards.end());
		//规整手牌
		sortCards(_vecStartCards);
		for (size_t i = 0; i < _vecStartCards.size(); i++) {
			auto cardSprite = CardSprite::createCardSprite(_vecStartCards[i]);
			cardSprite->setPosition(Vec2(_visibleSize.width / 2 - (_vecStartCards.size() - 1) * gSelfDEF_cardsep / 2 + i * gSelfDEF_cardsep + cardSprite->getContentSize().width / 4, def_no_selected_y));
			this->addChild(cardSprite, 1);
			_vecCards.emplace_back(cardSprite);
		}

	}
	//游戏结束
	if (ptr && ptr->opt == "over" && _isSatrting){
		int rst = atoi(ptr->message.c_str());
		char buf[512] = { 0 };
		if (rst > 0) {
			SimpleAudioEngine::getInstance()->playEffect("sound/win.mp3");
			int n = sprintf(buf, Configuration::getInstance()->getValue("win_msg").asString().c_str(), rst, rst*_baseScore);
			_labelInfo->setString(string(buf, n));
		}else if (rst < 0) {
			SimpleAudioEngine::getInstance()->playEffect("sound/lose.mp3");
			int n = sprintf(buf, Configuration::getInstance()->getValue("lose_msg").asString().c_str(), rst, rst*_baseScore);
			_labelInfo->setString(string(buf, n));
		}
		else {
			SimpleAudioEngine::getInstance()->playEffect("sound/win.mp3");
			int n = sprintf(buf, Configuration::getInstance()->getValue("he_msg").asString().c_str(), rst, rst*_baseScore);
			_labelInfo->setString(string(buf, n));
		}
		gameOver();
	}
	//玩家逃跑
	if (ptr && ptr->opt == "run") {
		char buf[256] = { 0 };
		int n = sprintf(buf, Configuration::getInstance()->getValue("player_run").asString().c_str(),
			ptr->deskmsg.self.nick.c_str(), ptr->deskmsg.self.score);
		_labelInfo->setString(string(buf, n));
		_mapPlayers[ptr->deskmsg.site]->mnick->setString("");
		_mapPlayers[ptr->deskmsg.site]->mrecord->setString("");
		gameOver();
	}
	//得分
	if (ptr && ptr->opt == "score" && _isSatrting) {
		if (_siteId % 2 == 0) {
			_label0Score->setString(ptr->deskmsg.p0score);
			_label1Score->setString(ptr->deskmsg.p1score);
		}
		else {
			_label0Score->setString(ptr->deskmsg.p1score);
			_label1Score->setString(ptr->deskmsg.p0score);
		}
	}
	//长时间不准备
	if (ptr && ptr->opt == "timeout" && !_isSatrting) {
		Scene *hScene = CHallSceneEx::createScene();
		Director::getInstance()->replaceScene(hScene);
	}
	//主流程(游戏进行种)
	if (ptr && ptr->opt == "game" && _isSatrting) {
		//桌面分
		if (ptr->deskmsg.deskscore < 80)
			_labelDeskScore->setColor(Color3B::WHITE);
		else
			_labelDeskScore->setColor(Color3B::RED);
		_labelDeskScore->setString(to_string(ptr->deskmsg.deskscore));
		if (ptr->deskmsg.per != -1)//前一个玩家出牌
			perPutCards(ptr);
		if (ptr->deskmsg.now != -1)//当前玩家出牌
			nowPutCards(ptr);
		if (ptr->deskmsg.per != -1 && ptr->deskmsg.now != -1 && ptr->deskmsg.must != 1) {
			//判断是否出完
			int i = 1;
			while ((ptr->deskmsg.per + i) % 4 != ptr->deskmsg.now) {
				playerPtr nowptr = _mapPlayers[(ptr->deskmsg.per + i) % 4];
				if (nowptr->mgone->isVisible()) {
					for (const auto &it : nowptr->perCards)
						this->removeChild(it);
					nowptr->perCards.clear();
				}
				i++;
			}
		}
	}
}

//用于倒计时
void CDeskScene::timeSchedule(float dt)
{
	if (_isSatrting && _nowPut != -1) {
		auto ptr = _mapPlayers[_nowPut];
		int sitme = atoi(ptr->mtimer->getString().c_str());
		sitme--;
		if (sitme > 0)
			ptr->mtimer->setString(to_string(sitme));
		else
			ptr->mtimer->setString("0");
		//告警
		if (sitme < 10 && sitme % 2 == 1)
			SimpleAudioEngine::getInstance()->playEffect("sound/jg.wav");

		//若自己超时，pass或则强制出牌
		if (sitme < 0 && _nowPut == _siteId) {
			ptr->mtimer->setVisible(false);
			_btPut->setVisible(false);
			_btNoput->setVisible(false);
			
			//强制出牌
			if (_must) {
				tcpConn::instance()->SendQuery(commDele::instance()->ToPut(_deskId, _siteId, vector<int>(1, _vecCards[0]->getSeq())));
				this->removeChild(_vecCards[0]);
				_vecCards.erase(_vecCards.begin());
				//调整手牌
				int i = 0;
				for (auto &it : _vecCards) {
					it->setPosition(Vec2(_visibleSize.width / 2 - (_vecCards.size() - 1) * gSelfDEF_cardsep / 2 + i * gSelfDEF_cardsep + it->getContentSize().width / 4, def_no_selected_y));
					i++;
				}
			}
			//不出
			else
				tcpConn::instance()->SendQuery(commDele::instance()->ToPut(_deskId, _siteId, vector<int>()));
			_nowPut = -1;
		}
	}
}

void CDeskScene::buttonEnable(bool enable)
{
	_btPut->setEnabled(enable);
	_btNoput->setEnabled(enable);
	_btReady->setEnabled(enable);
	_btReturn->setEnabled(enable);
}

//清理桌子
void CDeskScene::clearDesk(int site)
{	
	//全清
	if (site == -1) {
		for (size_t i = 0; i < _vecCards.size(); i++)
			this->removeChild(_vecCards[i]);
		_vecCards.clear();
		_btNoput->setVisible(false);
		_btPut->setVisible(false);
		for (int i = 0; i < 4; i++) {
			playerPtr ptr = _mapPlayers[i];
			ptr->mtimer->setVisible(false);
			ptr->mready->setVisible(false);
			ptr->mgone->setVisible(false);
			ptr->mpass->setVisible(false);
			ptr->msurplus->setString("0");
			for (size_t i = 0; i < ptr->perCards.size(); i++)
				this->removeChild(ptr->perCards[i]);
			ptr->perCards.clear();
		}
	}
	else {
		playerPtr ptr = _mapPlayers[site];
		ptr->mtimer->setVisible(false);
		ptr->mready->setVisible(false);
		ptr->mgone->setVisible(false);
		ptr->mpass->setVisible(false);
	}
}

void CDeskScene::gameSatrt()
{
	_isSatrting = true;
	for (const auto& it : _mapPlayers) {
		it.second->mready->setVisible(false);
		it.second->msurplus->setString("54");
	}
	_labelInfo->setString("");
}

void CDeskScene::gameOver()
{
	clearDesk(_siteId);
	_nowPut = -1;
	_isSatrting = false;
	_exit = false;
	_btReady->setVisible(true);
	_btReady->setEnabled(true);
	_btPut->setVisible(false);
	_btNoput->setVisible(false);
	_vecPerCards.clear();

}

void CDeskScene::getSelectedCards(vector<int>& vecSel)
{
	vecSel.clear();
	for (const auto &it : _vecCards) {
		if (it->getSelect())
			vecSel.emplace_back(it->getSeq());
	}
}

//整理手牌
void CDeskScene::sortCards(vector<int>& vec)
{
	vector<int> tmp;
	vector<int> dbttmp;
	vector<int> zhatmp;
	//大板同
	for (size_t i = 0; i < vec.size() - 3; i++) {
		if (vec[i] != -1 && vec[i] == vec[i + 1] && vec[i] == vec[i + 2] && vec[i] == vec[i + 3]) {
			dbttmp.emplace_back(vec[i]);
			vec[i] = -1;
			vec[i + 1] = -1;
			vec[i + 2] = -1;
			vec[i + 3] = -1;
		}
	}
	//三板同
	for (size_t i = 0; i < vec.size() - 2; i++) {
		if (vec[i] != -1 && vec[i] == vec[i + 1] && vec[i] == vec[i + 2]) {
			tmp.emplace_back(vec[i]);
			vec[i] = -1;
			vec[i + 1] = -1;
			vec[i + 2] = -1;
		}
	}
	vector<int> tttp;
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] != -1)
			tttp.emplace_back(vec[i]);
	}
	vec = tttp;
	//炸弹
	size_t j = 0;
	while (j < vec.size()) {
		if (j < vec.size() - 3 && vec[j] != -1 && vec[j + 1] != -1 && vec[j + 2] != -1 && vec[j + 3] != -1
			&& !CDBTRule::isJoker(vec[j])
			&& CDBTRule::getValue(vec[j]) == CDBTRule::getValue(vec[j + 1])
			&& CDBTRule::getValue(vec[j]) == CDBTRule::getValue(vec[j + 2])
			&& CDBTRule::getValue(vec[j]) == CDBTRule::getValue(vec[j + 3])) {
			int ztmp = CDBTRule::getValue(vec[j]);
			while (j < vec.size()) {
				if (CDBTRule::getValue(vec[j]) == ztmp) {
					zhatmp.emplace_back(vec[j]);
					vec[j] = -1;
					j++;
				}
				else
					break;
			}
		}
		else
			j++;
	}
	//其他
	vector<int> rst;
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] != -1)
			rst.emplace_back(vec[i]);
	}
	for (size_t i = 0; i < zhatmp.size(); i++)
		rst.emplace_back(zhatmp[i]);
	for (size_t i = 0; i < tmp.size(); i++) {
		rst.emplace_back(tmp[i]);
		rst.emplace_back(tmp[i]);
		rst.emplace_back(tmp[i]);
	}
	for (size_t i = 0; i < dbttmp.size(); i++) {
		rst.emplace_back(dbttmp[i]);
		rst.emplace_back(dbttmp[i]);
		rst.emplace_back(dbttmp[i]);
		rst.emplace_back(dbttmp[i]);
	}
	swap(rst, vec);
}

//处理前一位玩家
void CDeskScene::perPutCards(msgptr ptr)
{
	playerPtr perptr = _mapPlayers[ptr->deskmsg.per];
	perptr->msurplus->setString(ptr->deskmsg.surplus);
	//出完
	if (ptr->deskmsg.surplus == "0") 
		perptr->mgone->setVisible(true);
	perptr->mtimer->setVisible(false);
	//不出
	if (ptr->deskmsg.putCards.empty()) {
		SimpleAudioEngine::getInstance()->playEffect("sound/by.mp3");
		perptr->mpass->setVisible(true);
		return;
	}
	//出牌
	vector<string> vecPer;
	stringToVector(ptr->deskmsg.putCards, vecPer, ",");
	int i = 0;
	//重新记录上一次出的牌
	_vecPerCards.clear();
	for (const auto &it : vecPer) {
		int seq = atoi(it.c_str());
		auto cardSprite = CardSprite::createCardSprite(seq);
		_vecPerCards.emplace_back(seq);
		perptr->perCards.emplace_back(cardSprite);

		//计算出牌的位置
		int pos = calcuLen(vecPer.size());
		cardSprite->setPosition(Point(perptr->pos.x-pos+i*def_cardsep, perptr->pos.y));
		this->addChild(cardSprite);
		i++;
	}
	//获取牌型，播放对应音效
	auto ty = CDBTRule::getType(_vecPerCards);
	switch (ty.first)
	{
	case  CDBTRule::type_boom:
		SimpleAudioEngine::getInstance()->playEffect("sound/boom.mp3");
		break;
	case  CDBTRule::type_atom:
		SimpleAudioEngine::getInstance()->playEffect("sound/at.wav");
		if (_vecPerCards.size() == 4)
			SimpleAudioEngine::getInstance()->playEffect("sound/3.mp3");
		break;
	default:
		SimpleAudioEngine::getInstance()->playEffect("sound/cp.wav");
		break;
	}
}

void CDeskScene::nowPutCards(msgptr ptr)
{
	playerPtr nowptr = _mapPlayers[ptr->deskmsg.now];
	nowptr->mpass->setVisible(false);
	//上一轮出的牌清理掉
	for (const auto &it : nowptr->perCards)
		this->removeChild(it);
	nowptr->perCards.clear();
	_nowPut = ptr->deskmsg.now;
	//是否需要30秒倒计时
	bool b30 = true;
	if (_vecPerCards.size()>0 && _vecPerCards[0] == 19 && !ptr->deskmsg.must){
		b30 = false;
		for (size_t i=0; i<_vecPerCards.size()-1; i++){
			if (_vecPerCards[i] != _vecPerCards[i + 1]) {
				b30 = true;
				break;
			}
		}
	}
	if (b30)
		nowptr->mtimer->setString("30");
	else
		nowptr->mtimer->setString("1");
	nowptr->mtimer->setVisible(true);
	//必须出牌(上轮最大，清桌)
	_must = false;
	if (ptr->deskmsg.must) {
		_must = true;
		for (int i = 0; i < 4; i++) {
			playerPtr ptr = _mapPlayers[i];
			ptr->mpass->setVisible(false);
			for (size_t j = 0; j < ptr->perCards.size(); j++)
				this->removeChild(ptr->perCards[j]);
			ptr->perCards.clear();
		}
	}
	//自己
	if (_nowPut == _siteId){
		if (ptr->deskmsg.must) 
			_vecPerCards.clear();
		else 
			_btNoput->setVisible(true);
		_btPut->setVisible(true);
	}
}

int CDeskScene::calcuLen(int counts)
{
	double total = card_weight + def_cardsep*(counts - 1);
	return int((total - card_weight) / 2);
}
