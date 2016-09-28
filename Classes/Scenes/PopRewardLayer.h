#ifndef __SETTLE_REWARD_LAYER_H__
#define __SETTLE_REWARD_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ParamData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum PopRewardBoxId
{
	POP_BOX_0 = 20000,
	POP_BOX_1,
	POP_BOX_2,
	POP_BOX_3
};

class PopRewardLayer : public Layer
{
public:
	PopRewardLayer();
	~PopRewardLayer();

	static PopRewardLayer *getInstance(void);

	static PopRewardLayer *create(int itemId, int itemCount, int titleType = 0);//0.nor 1.settle reward 2.exchange 
	static PopRewardLayer *create(const std::vector<PopItemInfo_T> &arrItems, int titleType = 0);//0.nor 1.settle reward 2.exchange 
	static PopRewardLayer *openBox(int boxId, int boxLv, int param1 = 0);	//1. 2.tili count 6 3.no tili	4.need resume
	static PopRewardLayer *createRoleReward(int roleId);

	bool init(const std::vector<PopItemInfo_T> &arrItems, int titleType, bool isNeedOpen);
	bool initOpenBox(int boxId, int boxLv, int param1);
	bool initRoleReward(int roleId);
	
	virtual void onEnter(void);
	virtual void update(float delta);

	void setRankIdx(int randIdx);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	
private:
	static PopRewardLayer *s_pInstance;

	void changeAniPic(cocostudio::Armature* ani, const std::vector<PopItemInfo_T> &arrItems);
	Node* getItemDisplayerNode(int itemid, int num);

	void giveItemToUser();
	void showRewardTitle(int titleType);	//0.nor 1.settle reward 2.exchange 

	void addBoxLvTille(int idx, int lv, Vec2 pos);

	int getBoxAnimId(int boxId);

	std::vector<PopItemInfo_T> _arrItems;
	int _titleType;

	int _curPopIdx;
	Armature *_pBox;
	Armature *_pBoxPop;
	bool _isNeedOpen;
	int _boxId;
	int _boxLv;
	bool _isCanTouch;
	float _topItemPosY;
	int _boxAnimId;
	bool _isFirstTouch;
	int _boxParam;
};

#endif