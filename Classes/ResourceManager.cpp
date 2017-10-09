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

	//���ػ�ɫ
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

	//�����ƺ�
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

	//��������
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
	//���س��ư�ť
	SpriteFrame* button = nullptr;
	button = SpriteFrame::create("pic/ready.png", Rect(0, 0, 79, 96));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "ready");
	//����
	button = SpriteFrame::create("pic/desk.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "deskbeijing");
	button = SpriteFrame::create("pic/hall.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "hallbeijing");
	button = SpriteFrame::create("pic/login.png", Rect(0, 0, 1300, 700));
	SpriteFrameCache::getInstance()->addSpriteFrame(button, "loginbeijing");
}

void CResourceManager::loadSoundResource()
{
	//��������
	auto audioengine = SimpleAudioEngine::getInstance();
	audioengine->preloadBackgroundMusic("sound/bj.mp3"); //����
	audioengine->preloadEffect("sound/3.mp3"); //���ͬ
	audioengine->preloadEffect("sound/cp.wav"); //����
	audioengine->preloadEffect("sound/at.wav"); //ͬ��
	audioengine->preloadEffect("sound/boom.mp3"); //ը��
	audioengine->preloadEffect("sound/bg.wav"); //��ʼ
	audioengine->preloadEffect("sound/by.mp3"); //��Ҫ
	audioengine->preloadEffect("sound/jg.wav"); //����
	audioengine->preloadEffect("sound/win.mp3"); //Ӯ
	audioengine->preloadEffect("sound/lose.mp3");//��
	audioengine->preloadEffect("sound/zb.mp3");//׼��
	audioengine->preloadEffect("sound/cw.wav");//��׼����
}
