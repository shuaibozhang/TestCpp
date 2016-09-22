#ifndef __GAME_RES_POOL_H__
#define __GAME_RES_POOL_H__

#include "cocos2d.h"
#include "Armtr/GameArmtr.h"
#include "ParamData.h"

USING_NS_CC;
using namespace std;

typedef enum GameResId
{
	RES_ROLE = GameArmtrType_E::TYPE_ROLE,
	RES_MONSTER,
	RES_SKILL,
	RES_BUFF,
	RES_FB,
	RES_HIT,
	RES_ELE_ICON,
	RES_CRUSH_EFFECT,
	RES_CREATE_EFFECT,
	RES_ICON_SKILL,
	RES_ITEM_EFFECT,
	RES_SPECIAL,
	RES_SPECIAL_EX,
	RES_GOLD_0,	//yuanbao
	RES_GOLD_1,
	RES_GOLD_2,
	RES_FLY_ICON_START,
	RES_FLY_ICON_END = RES_FLY_ICON_START + ParamData::ROLE_COUNT * 3,
	//keep it
	RES_COUNT
}GameResId_E;


class GameResPool
{
public:
	static GameResPool *getInstance();
	~GameResPool();

	Node *getResById(int resId);

	void loadRes(int resId, int resCount);
	void recycleRes(int resId, Node *pRes);
	void recycleRes(Node *pRes);

private:
	static GameResPool *s_pInstance;
	GameResPool();
	
	bool init();
	Node *createRes(int resId);

	Vector<Node *> _arrResPool[GameResId_E::RES_COUNT];
};



#endif // !__GAME_RES_POOL_H__
