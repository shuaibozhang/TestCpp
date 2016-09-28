#ifndef __DAYACTIVITY_MGR_H__
#define __DAYACTIVITY_MGR_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

enum DayActivityTppe
{
	DAYPASS = 0,
	DAYWENPONUP,
	DAYUSEITEMS,
	DAYSPBOSS,
	DAYTA,
	DAYBUYITEMS,
	DAYUSEBGL,
	DAYOPENGOLDBOX,
	DAYGETTILI,
	DAYBUYZUANSHI,
	DAYDUNGEON,
	DAYALLNUMS
};

struct DayActivityRewardInfo
{
	DayActivityTppe activityType;
	int maxNum;
	int everyNumRewrad;
};

class DayActivityMgr
{
public:
	static DayActivityMgr *getInstance();
	~DayActivityMgr();

	void addTimes(DayActivityTppe type, int num = 1);
	void addTimes(DayActivityTppe type, int num,bool save);

	int getAllActivityScore();
	int getActivityScore(DayActivityTppe type);
	int getActivityTimes(DayActivityTppe type);
	int getActivityMaxTimes(DayActivityTppe type);

	int getNeedScore();

	void resetTimes();
	/*
	*return 
	0 : can not
	1: can
	2: can but have get
	*/
	int checkCanGet(int idx);

	void getReward();

	int getFinishNum();
private:
	void init();
	DayActivityMgr() {};

	static DayActivityMgr *s_pInstance;
	std::map<int, int*> _activityInfoMap;
};



#endif 
