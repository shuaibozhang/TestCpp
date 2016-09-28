#ifndef __DAILY_REWARD_LAYER_H__
#define __DAILY_REWARD_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ParamData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class DailyRewardLayer : public Layer
{
public:
	DailyRewardLayer();
	~DailyRewardLayer();

	static DailyRewardLayer *getInstance(void);

	/*
	*@param type = 0:is defult daily, 
	type = 1: is repeat week reward,
	type = 2: is guoqing activity reward
	*/
	static DailyRewardLayer *create(int type, int onlineday);

	static DailyRewardLayer *create(bool isweek = false);
	bool init(bool isWeekReward);

	/*
	*@param type = 0:is defult daily,
	type = 1: is repeat week reward,
	type = 2: is guoqing activity reward
	*/
	bool init(int type, int onlineday);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	
private:
	static DailyRewardLayer *s_pInstance;
	int _curDayIdx;
	Node *_pRoot;
	int _rewardType;
};

#endif