#include "CardSprite.h"
#include "DBTRule.h"


CardSprite::CardSprite()
{
}


CardSprite::~CardSprite()
{
}
CardSprite* CardSprite::createCardSprite(int seq)
{
	ostringstream os;
	CardSprite *cardptr = CardSprite::create();
	cardptr->setSeq(seq);
	if (seq == 52)
		cardptr->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("b_joker"));
	else if (seq == 53)
		cardptr->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("r_joker"));
	else {
		cardptr->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("front"));
		//�������
		Size cardSize = cardptr->getContentSize();
		int val = CDBTRule::getValue(seq);
		int col = CDBTRule::getColor(seq);
		if (col < 2)
			os << "b_num" << val;
		else
			os << "r_num" << val;
		auto num = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(os.str()));
		num->setScale(float(0.4));
		num->setPosition(Vec2(12, cardSize.height - 20));
		cardptr->addChild(num);
		//��ӻ�ɫ
		os.str("");
		os << "flower" << col;
		Sprite *color = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(os.str()));
		color->setScale(float(0.4));
		color->setPosition(Vec2(12, cardSize.height - 45));
		cardptr->addChild(color);
	}
	return cardptr;
}

//�ѳ����ƴ�����һ������
cocos2d::Sprite* CardSprite::createPutsSprite(const vector<int>& vec)
{
	return nullptr;
}
