#include "FightUtil.h"
#include "UserData.h"
#include "ParamMgr.h"
#include "CrushUtil.h"
#include "GLCommon/JRCommon.h"


const float FightUtil::DESIGN_NOR_MONSTER_TIME = 25.f;
const float FightUtil::DESIGN_ELITE_MONSTER_TIME = 40.f;
const float FightUtil::DESIGN_BOSS_TIME = 260.f;
const float FightUtil::DESIGN_DUNGEON_TIME = 150.f;


float FightUtil::calcMonsterHp(const DesignRoleInfo_T & designRoleInfo, int rAttMCount)
{
	float hp = designRoleInfo.att * rAttMCount;
	return hp;
}

float FightUtil::calcMonsterAtt(const DesignRoleInfo_T & designRoleInfo, int mAttRCount)
{
	float a = mAttRCount;
	float b = -(designRoleInfo.hp + designRoleInfo.hpAdd * mAttRCount * 0.25f);
	float c = -designRoleInfo.def * (designRoleInfo.dp + designRoleInfo.dpAdd * mAttRCount * 0.25f);

//	float aaa = sqrt(b*b - 4 * a * c);
	float att = (-b + sqrt(b*b - 4 * a * c)) / (2 * a);
//	float aaaaaa = a * att * att + b * att + c;

	return att;
}

int FightUtil::calcMonsterGold(int designType, int perGold)
{
	int ret = 0;

	switch (designType)
	{
	case 0:
		ret = perGold * DESIGN_NOR_MONSTER_TIME / 60.f;
		break;
	case 1:
		ret = perGold * DESIGN_ELITE_MONSTER_TIME / 60.f;
		break;
	case 2:
		ret = perGold * DESIGN_BOSS_TIME / 60.f;
		break;
	}

	return ret;
}

int FightUtil::calcMonsterExp(int designType, int perExp)
{
	int ret = 0;

	switch (designType)
	{
	case 0:
		ret = perExp * DESIGN_NOR_MONSTER_TIME / 60.f;
		break;
	case 1:
		ret = perExp * DESIGN_ELITE_MONSTER_TIME / 60.f;
		break;
	case 2:
		ret = perExp * DESIGN_BOSS_TIME / 60.f;
		break;
	}

	return ret;
}

float FightUtil::calcWeakDamageValue(int weakId, const DesignRoleInfo_T & designRoleInfo)
{
	float ret = 0.f;

	switch (weakId)
	{
	case 0:
		//fire
		ret = designRoleInfo.hp / 20.f;
		break;
	case 1:
		//boom
		ret = designRoleInfo.hp / 5.f;
		break;
	case 2:
		//thunder
		ret = designRoleInfo.hp / 12.f;
		break;
	case 3:
		//add hp
		break;
	}

	return ret;
}


int FightUtil::getDungeonReward(int dungeonType, std::vector<PopItemInfo_T>& arrReward, const DesignRoleInfo_T &designRoleInfo)
{
	int maxPos = UserData::getInstance()->getMaxPos();
//	auto pStageInfo = ParamMgr::getInstance()->getStageInfo(maxPos);
	auto pReward = ParamMgr::getInstance()->getDungeonReward(maxPos);

	int ret = -1;

	if (0 == dungeonType)
	{
		CrushUtil::addPopItem(arrReward, -100, designRoleInfo.perGold * DESIGN_DUNGEON_TIME / 60.f);
	}
	else if (1 == dungeonType)
	{
		ret = designRoleInfo.perExp * DESIGN_DUNGEON_TIME / 60.f;
	}
	else if (2 == dungeonType)
	{
		int itemId = 1500 + ToolsUtil::getRandomInt(0, 4) * 10 + pReward->equipLv - 1;
		CrushUtil::addPopItem(arrReward, itemId, pReward->equip);
	}

	return ret;
}

int FightUtil::getDesignEliteSceneId(int sceneId)
{
	int ret = -1;
	switch (sceneId)
	{
	case 10:
		ret = 17;
		break;
	case 19:
		ret = 30;
		break;
	case 32:
		ret = 39;
		break;
	case 41:
		ret = 47;
		break;
	case 48:
		ret = 56;
		break;
	case 58:
		ret = 59;
		break;
	case 61:
		ret = 63;
		break;
	case 64:
		ret = 67;
		break;
	case 68:
		ret = 75;
		break;
	case 77:
		ret = 79;
		break;
	case 80:
		ret = 81;
		break;
	default:
		CCASSERT(false, __FUNCTION__);
		break;
	}
	return ret;
}
