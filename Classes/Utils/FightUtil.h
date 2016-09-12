#ifndef __FIGHT_UTIL_H__
#define __FIGHT_UTIL_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


class FightUtil
{
public:
	static const float DESIGN_NOR_MONSTER_TIME;
	static const float DESIGN_ELITE_MONSTER_TIME;
	static const float DESIGN_BOSS_TIME;
	static const float DESIGN_DUNGEON_TIME;

	static float calcMonsterHp(const DesignRoleInfo_T &designRoleInfo, int rAttMCount);
	static float calcMonsterAtt(const DesignRoleInfo_T &designRoleInfo, int mAttRCount);
	static int calcMonsterGold(int designType, int perGold);
	static int calcMonsterExp(int designType, int perExp);

	static float calcWeakDamageValue(int weakId, const DesignRoleInfo_T &designRoleInfo);
	static int getDungeonReward(int dungeonType, std::vector<PopItemInfo_T>& arrReward, const DesignRoleInfo_T &designRoleInfo);
	static int getDesignEliteSceneId(int sceneId);
};


#endif