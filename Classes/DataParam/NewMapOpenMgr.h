#ifndef __MAPOPEN_MGR_H__
#define __MAPOPEN_MGR_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

enum MapOpenType
{
	PASS68 = 0,
	PASS77,
	PASS80,
	WENPON20_0,
	WENPON20_1,
	WENPON20_2,
	WENPON20_3,
	TA200,
	BGL5000,
	TA500,
};

class NewMapOpenMgr
{
public:
	static NewMapOpenMgr *getInstance();
	~NewMapOpenMgr();

	bool isGet(MapOpenType type);
	bool tryGet(MapOpenType type, bool needPop = true);

	void popGet();
private:
	void init();
	NewMapOpenMgr() {};

	static NewMapOpenMgr *s_pInstance;
	int _getnum;
};



#endif 
