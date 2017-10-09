#include "ResourceManager.h"
#include <array>
#include <sstream>
#include <SimpleAudioEngine.h>
using namespace CocosDenshion;

CResourceManager::CResourceManager()
{
}


CResourceManager::~CResourceManager()
{
}

void CResourceManager::loadResource()
{
	loadCardResource();
	loadLoginResource();
	loadDeskResource();
	loadSoundResource();
}

void CResourceManager::loadCardResource()
{
	ostringstream	os;

	//加载花色
#define flower_weight 44
#define flower_height 42
	SpriteFrame *flowerframe = nullptr;
	flowerframe = SpriteFrame::create("pic/colors.png", Rect(0 * flower_weight, 0, flower_weight, flower_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(flowerframe, "flower2");
	flowerframe = SpriteFrame::create("pic/colors.png", Rect(1 * flower_weight, 0, flower_weight, flower_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(flowerframe, "flower0");
	flowerframe = SpriteFrame::create("pic/colors.png", Rect(2 * flower_weight, 0, flower_weight, flower_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(flowerframe, "flower3");
	flowerframe = SpriteFrame::create("pic/colors.png", Rect(3 * flower_weight, 0, flower_weight, flower_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(flowerframe, "flower1");

	//加载牌号
#define number_size	39
	SpriteFrame* numframe = nullptr;
	for (int i = 0; i < 13; i++) {
		os.str("");
		os << "r_num" << i;
		numframe = SpriteFrame::create("pic/poker_num.png", Rect(i * number_size, number_size, number_size, number_size));
		SpriteFrameCache::getInstance()->addSpriteFrame(numframe, os.str());
		os.str("");
		os << "b_num" << i;
		numframe = SpriteFrame::create("pic/poker_num.png", Rect(i*number_size, 0, number_size, number_size));
		SpriteFrameCache::getInstance()->addSpriteFrame(numframe, os.str());
	}

	//加载牌面
	SpriteFrame* puke = nullptr;
	puke = SpriteFrame::create("pic/poker_surface.png", Rect(0 * card_weight, 0, card_weight, card_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(puke, "front");
	puke = SpriteFrame::create("pic/poker_surface.png", Rect(1 + 1 * card_weight, 0, card_weight, card_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(puke, "r_joker");
	puke = SpriteFrame::create("pic/poker_surface.png", Rect(1 + 2 * card_weight, 0, card_weight, card_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(puke, "b_joker");
	puke = SpriteFrame::create("pic/poker_surface.png", Rect(1 + 3 * card_weight, 0, card_weight, card_height));
	SpriteFrameCache::getInstance()->addSpriteFrame(puke, "back");
}

void CResourceManager::loadLoginResource()
{

}

void CResourceManager::loadDeskResource()
{
	//加载出牌按钮
	SpriteFrame* button = nullptr;
	button = SpriteFrame::create("pic/ready.png", Rect(0, 0, 79, 96));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "ready");
	//背景
	button = SpriteFrame::create("pic/desk.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "deskbeijing");
	button = SpriteFrame::create("pic/hall.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "hallbeijing");
	button = SpriteFrame::create("pic/login.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "loginbeijing");
}

void CResourceManager::loadSoundResource()
{
	//加载配音
	auto audioengine = SimpleAudioEngine::getInstance();
	audioengine->preloadBackgroundMusic("sound/bj.mp3"); //背景
	audioengine->preloadEffect("sound/3.mp3"); //大板同
	audioengine->preloadEffect("sound/cp.wav"); //出牌
	audioengine->preloadEffect("sound/at.wav"); //同点
	audioengine->preloadEffect("sound/boom.mp3"); //炸弹
	audioengine->preloadEffect("sound/bg.wav"); //开始
	audioengine->preloadEffect("sound/by.mp3"); //不要
	audioengine->preloadEffect("sound/jg.wav"); //警告
	audioengine->preloadEffect("sound/win.mp3"); //赢
	audioengine->preloadEffect("sound/lose.mp3");//输
	audioengine->preloadEffect("sound/zb.mp3");//准备
	audioengine->preloadEffect("sound/cw.wav");//不准出牌
}
