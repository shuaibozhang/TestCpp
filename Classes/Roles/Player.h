#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "ParamData.h"
#include "Defines.h"

USING_NS_CC;


class Role;
class RoundTag;
class GameArmtr;
class EleIcon;

typedef struct _RecoverInfo
{
	int round;
	int param1;
	float param2;
	RoundTag *pTag;
}RecoverInfo_T;


typedef struct _ShieldInfo
{
	int id;
	int round;
}ShieldInfo_T;



class Player : public Node
{
public:
	Player();
	virtual ~Player();

	static Player *getInstance();

	static Player *create();
	virtual bool init();

	void initFightData();
	void resetFightData();
	void resetBarData();
	void updateRoleId();

	void placeRole(const Vec2 &newPos);
	void moveToPos(int posIndex, float xDt, float dur, float bufferTime);
	void moveToSettlePos(bool isWin);
	void moveToOutsidePos(int posIndex, float xDt, float dur, float bufferTime);
	
	void addAttInfo(CrushArr_T *pCrushInfo);
	bool doHeartSkill(EleIcon *pEle, int skillFlag);	//flag 0.nor skill 1.super skill
	void doHurt(int attribute, float damage);
	void doHurtByMonster(int attribute, float damage, int monsterLv);
	void addDp(const RoleCurAttInfo_T *pAttInfo);
	void addDp(float value);
	void addHp(const RoleCurAttInfo_T *pAttInfo);
	void addHp(float value, int rolePosIndex = 3);
//	void doClean(const RoleCurAttInfo_T *pAttInfo);
	bool doClean(int lv, int count, int startType, bool isNeedAnim = false, Vec2 heartPos = Vec2::ZERO);	//0.front to end 1.end to front
	bool doAddEleLv(int lv, int count, bool isNeedAnim = false, Vec2 heartPos = Vec2::ZERO);
	void getShield(const RoleCurAttInfo_T *pAttInfo);
	void getRelive(const RoleCurAttInfo_T *pAttInfo);
	void addRecover(int type, int round, float param);	//type 0.hp 1.def
	void addAttBuff(int round, float param);
	void playStateAnim(const string &animName, int startIndex = 0, int count = 4);
	void doDead();
	void doRecover();
	void doRelive(int reliveType);	//0.relive by skill 1.relive by rmb

	bool isAttFinish();
	bool isCurAttFinish();

	void playMove();
	void playIdle();
	void doStartDef();
	void doEndDef();

	void changeWeapon(int roleId, int weaponId);

	const PlayerInfo_T *getRoleInfo(int roleId);
	int getRoleSkillId(int roleId, int skillFlag);	//type:0.normal skill 1.super 2.def skill
	Vec2 getCurRolePos(int roleIndex);
	Role *getRole(int roleIndex);

	void reliveCB(bool isSuccess);

	int getRoleIdByPosIndex(int posIndex);
	int getPosIndexByRoleId(int roleId);

	float getAttBuff();
	void playBuffAttAnim();
	void stopBuffAttAnim();

	CC_SYNTHESIZE_READONLY(float, _curHp, CurHp);
	CC_SYNTHESIZE_READONLY(float, _curDp, CurDp);
//	CC_SYNTHESIZE_READONLY(float, _armorRecover, ArmorRecover);
//	CC_SYNTHESIZE_READONLY(float, _hpRecover, HpRecover);
//	CC_SYNTHESIZE_READONLY(float, _attValue, AttValue);
//	CC_SYNTHESIZE_READONLY(float, _armorValue, ArmorValue);
	CC_SYNTHESIZE(float, _totalHp, TotalHp);
	CC_SYNTHESIZE(float, _totalDp, TotalDp);
	CC_SYNTHESIZE(float, _totalDef, TotalDef);
	CC_SYNTHESIZE(bool, _isDefing, IsDefing);
	CC_SYNTHESIZE(bool, _isDead, IsDead);
	CC_SYNTHESIZE(bool, _isWaitRelive, IsWaitRelive);
#if (0 == CC_ENABLE_NEW_PARAM)
	CC_SYNTHESIZE(int, _baseLv, BaseLv);
#endif
private:
	void updateBuffTagPos();
	bool reduceBuffRound(int buffIndex);
	Role *createRole(int roleId);

	void playLinkAnim(Vec2 srcPos, Vec2 desPos);

	static Player *s_pInstance;
	std::vector<Role *> _arrRoles;
//	Vec2 _startPox;
	PlayerInfo_T _arrRoleInfo[ParamData::FIGHT_ROLE_COUNT];
	int _arrRoleSkillId[ParamData::FIGHT_ROLE_COUNT][3];	//0:normal att 1:super att 2:def
	RecoverInfo_T _arrRecoverInfo[5];	//0.hp 1.def 2.relive 3.shield 4.att buff
//	ShieldInfo_T _shieldInfo;
	GameArmtr *_pArmtrShield;
	int _reliveTimes;
	int _arrRoleId[ParamData::FIGHT_ROLE_COUNT];
};

#endif
