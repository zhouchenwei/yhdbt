#include "DBTRule.h"


CDBTRule::CDBTRule()
{
}


CDBTRule::~CDBTRule()
{
}

void CDBTRule::shuffleCards(std::array<int, 216>& arrCards, int times)
{
	
}

bool CDBTRule::checkCards(std::vector<int>& cards)
{
	
	return true;
}

int CDBTRule::getWeight(int card)
{
	return 0;
}

int CDBTRule::getAtomWeight(int card)
{
	
	return 0;
}

int CDBTRule::getWeightNoRedFive(int card)
{
	

	return card;
}

bool CDBTRule::isNormal(const cards_type ty)
{
	return true;
}

pair<bool, int> CDBTRule::isSingle(const std::vector<int>& cards)
{
	
	return pair<bool, int>(false, 0);
}

pair<bool, int> CDBTRule::isPairs(const std::vector<int>& cards)
{
	
	return pair<bool, int>(true, 1);
}

pair<bool, int> CDBTRule::isthree(const std::vector<int>& cards)
{
	
	return pair<bool, int>(true, 1);
}

pair<bool, int> CDBTRule::isAtom(const std::vector<int>& cards)
{
	
	return pair<bool, int>(false, 0);
}

pair<bool, int> CDBTRule::isBoom(const std::vector<int>& cards)
{

	return pair<bool, int>(true, 1);
}

pair<bool, int> CDBTRule::isSister(const std::vector<int>& cards)
{
	
	return pair<bool, int>(true, 1);
}

pair<bool, int> CDBTRule::isPlane(const std::vector<int>& cards)
{
	
	return pair<bool, int>(true, 1);
}

pair<bool, int> CDBTRule::isThreetwo(const std::vector<int>& cards)
{
	return pair<bool, int>(false, 0);
}

std::pair<CDBTRule::cards_type, int> CDBTRule::getType(const std::vector<int>& cards)
{
	return std::make_pair(type_unknow, 0);
}

pair<bool, CDBTRule::cards_type> CDBTRule::isBigger(std::vector<int>& cards_per, std::vector<int>& cards_now)
{
	
	return pair<bool, cards_type>(true, type_unknow);
}

int CDBTRule::getScore(const std::vector<int>& cards)
{
	return 0;
}

