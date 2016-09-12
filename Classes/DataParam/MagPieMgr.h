#ifndef __MagPieMgr_H__
#define __MagPieMgr_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

typedef struct _MagPieTask
{
	int goalNum;
	int rewardCount;
	PopItemInfo_T arrReward[2];
}MagPieTask_T;


class MagPieMgr
{
public:
	static const int MAGPIE_START_DAY = 57;
	enum _MagPieTaskNameIdx
	{
		LUSANJIN = 0,
		SHENGQIUYUE,	
		PROPUSE,
		BOX,
		WEAPON,
		STAGE,
		SHOPING,
	}MagPieTaskNameIdx_E;

	~MagPieMgr();

	static MagPieMgr *getInstance();

	//return how many MagPie task has been finished
	int getFinishMagPieTasksNum();
	//check if finish MagPie task at "idx"
	bool checkIsFinishMagPieTask(int idx);
	bool checkIsGotReward(int idx);

	//	return goal number of MagPie task at "idx" 
	int getMagPieTaskGoalNum(int idx);

	// return how many goal number has been finishen now.
	int getCurFinishedMagPieGoalNum(int idx);

	// return numer of reward at "idx";
	void getMagPieReward(int idx, std::vector<PopItemInfo_T> &arrReward);

	// add Finished MagPieGoalNum at Task "idx"
	void addFinishedMagPieGoalNum(int idx, int addCount);
		
	//set which reward has been take at "idx" 
	void setIsGotMagPieReward(int idx);

	int getMissionState(int idx);	//0.unfinish 1.finish 2.is got reward

	CC_SYNTHESIZE(int, _dayIdx, DayIdx);
private:
	MagPieMgr();

	static MagPieMgr *s_pInstance;
};
#endif


