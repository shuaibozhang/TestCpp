#ifndef __ACHIEVE_MGR_H__
#define __ACHIEVE_MGR_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


typedef struct _AchGoal
{
	int count;
	int reward;
}AchGoal_T;


class AchieveMgr
{
public:
	enum _AchieveIdx
	{
		GOLD = 0,
		FLOOR,
		WEAPON,
		STAGE,
		BOSS,
		FORGE,
	}AchieveIdx_E;

	static AchieveMgr *getInstance();

	~AchieveMgr();


	int checkFinishAchieve();
	bool checkIsFinishAchieve(int idx);

	int getCurAchieveLv(int idx);
	int getCurAchieveTotalCount(int idx);
	int getCurAchieveCount(int idx);
	int getCurAchieveReward(int idx);

	void addAchieveCount(int idx, int addCount, bool isSave = true);
	void finishAchieceLv(int idx);

private:
	AchieveMgr();

	static AchieveMgr *s_pInstance;
};



#endif // __ACHIEVE_MGR_H__
