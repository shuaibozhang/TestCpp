#ifndef __CRUSH_UTIL_H__
#define __CRUSH_UTIL_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

class EleIcon;
class GameArmtr;

class CrushUtil
{
public:
	//travelType:1:horizontal 2:vertical 0:both
//	static bool travelseEle(int row, int column, TravelseInfo_T *pCrushInfo, bool isCanCrush, int travelType);
	static bool travelseEle(int row, int column, TravelseInfo_T *pCrushInfo, bool isCanCrush, int travelType);
	static int productEleId(int arrEleCount[4], int row, int col, int *arrForbidInf);
	static void simulateFall(bool (*arrIsCanFall)[ParamData::CRUSH_COL], CrushIndex_T (*arrEleSrcIndex)[ParamData::CRUSH_COL]);
	static void swapEle(EleIcon *pEle1, EleIcon *pEle2, EleIcon *(*arrEle)[ParamData::CRUSH_COL]);
	static void swapEle(CrushIndex_T pos1, CrushIndex_T pos2, EleIcon *(*arrEle)[ParamData::CRUSH_COL]);
	static Vec2 getCrushAreaStartPos();
	static Vec2 getElePos(int row, int column);
	static CrushIndex_T getCrushIndex(const Vec2 &pos);
	static void setPosIndex(CrushIndex_T &posIndex, int row=-1, int column=-1);
	static bool isEleCanTouch(EleIcon *pEle);


	static Vec2 getMonsterPos(int index, int count, int waveIndex);
	static Vec2 getRoleOffsetPos(int roleId);
	static Vec2 getMonsterCenterPos(int waveIndex);
	static Vec2 getMonsterFrontPos(int waveIndex);
	static Vec2 getRoleCenterPos(int posId);
//	static Vec2 getRoleFrontPos(int posId);
	static Vec2 getRolePos(int roleIndex, int posId);
	static Vec2 getMonsterAttPos(int posId, int monsterIndex, int waveCount);
	static Vec2 getMonsterSkillPos(int posId, int monsterIndex, int waveCount);
	static Vec2 getRoleDefPos(int posId);
	static Vec2 getShieldPos(int posId);

	static float getDamageN(int maxLv);

	static void changeWeapon(GameArmtr *pArm, int roleId, int weaponId);

	static void loadSkillImage(int skillId);
	static void releaseSkillImage(int skillId);

	static float getMonsterScale(int monsterId);

	static float getMonsterAttValue(int monsterLv, int roleLv, float attValue, int monsterType, bool isGrow = true);
	static float getMonsterHpValue(int monsterLv, int roleLv, float hpValue, int monsterType, bool isGrow = true);
	static float getWeakDamageValue(int weakLv, int roleLv, float damage, bool isGrow = true);
	static int getMonsterExpValue(int monsterLv, int roleLv, int exp, bool isGrow = true);
	static int getMonsterGoldValue(int monsterLv, int roleLv, int gold, bool isGrow = true);

	static int calcCrushWeight(const CrushArr_T *pCrushInfo, bool isSwap);
	static void initTravelseInfo(TravelseInfo_T *pInfo);
	static void copyTravelseInfo(TravelseInfo_T *pDesInfo, TravelseInfo_T *pSrcInfo);

	static int calcEleLayoutWeight(int row, int col, int(*arrEleId)[ParamData::CRUSH_COL]);

	static void addPopItem(std::vector<PopItemInfo_T>& arrReward, int itemId, int itemCount);

	static void getSettleReward(int rankIdx, std::vector<PopItemInfo_T>& arrReward);
	static void getExchangeReward(int codeId, std::vector<PopItemInfo_T>& arrReward);

	static int getEndlessBaseLv(int roleLv);
	static int getEndlessAddLv(int floorIdx);
	static int getEliteAddLv(int sceneId);

	static bool isNeedSkillBg(int skillId);

	static void giveUserItem(int itemId, int itemCount, int itemLv, bool isNeedSave);
	static string getItemPicName(int itemId);

	static int getDungeonReward(int dungeonType, int lv, std::vector<PopItemInfo_T>& arrReward);

	static __String *getEleAnimName(int eleId, int eleLv, bool isStone, int animId);	//animId:0.idle 1.miss 2.touch
};


#endif