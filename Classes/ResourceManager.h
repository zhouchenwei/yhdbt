#ifndef  _CRESOURCEMANAGER_H_
#define  _CRESOURCEMANAGER_H_

#include "cocos2d.h"
#include "Singleton.h"
USING_NS_CC;
using namespace std;


#define card_weight	98
#define card_height	138

class CResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	void	loadResource();

private:
	void	loadCardResource();
	void	loadLoginResource();
	void	loadDeskResource();
	void	loadSoundResource();

};

typedef CSingleton<CResourceManager>	resourceMgr;
#endif