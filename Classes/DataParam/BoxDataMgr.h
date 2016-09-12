#ifndef __BOX_DATA_MGR_H__
#define __BOX_DATA_MGR_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


class BoxDataMgr
{
public:
	static BoxDataMgr *getInstance();
	~BoxDataMgr();

	void getBoxReward(int boxIdx, int boxLv, std::vector<PopItemInfo_T> &arrReward);
	int getBoxOpenCost(int boxId, int surplusTime);
	int getBoxBuyCost(int boxId);
//	int getBoxDescGoldCount(int boxIdx, int boxLv);
//	int getBoxDescChipCount(int boxIdx, int boxLv);
//	int getBoxDescRareChipCount(int boxIdx, int boxLv);
	int getWaitTime(int boxIdx);
	int getCurGetBoxLv(int sceneId = -1);

	int getBoxGoldCount(int boxIdx, int boxLv, int countType);	//countType:0.min 1.max
	int getBoxChipCount(int boxIdx, int boxLv, int chipType, int countType);	//chipType 0.putong 1.jinglian 2.xiyou 3.chuanqi	countType:0.min 1.max
	int getBoxAboveChipCount(int boxIdx, int boxLv, int chipType, int countType);	//chipType 0.putong 1.jinglian 2.xiyou 3.chuanqi	countType:0.min 1.max


	bool isBoxInBagFull();
private:
	static BoxDataMgr *s_pInstance;
	BoxDataMgr();

//	BoxDetailInfo_T _arrBoxInfo[ParamData::BOX_COUNT];

	int getItemId(int categoryIdx, int itemIdx);
};


#endif