#ifndef __GAME_ARMTR_H__
#define __GAME_ARMTR_H__

#include "cocos2d.h"
#include "ParamData.h"
//#include "DragonBonesHeaders.h"
//#include "DBCCRenderHeaders.h"

USING_NS_CC;
//USING_NS_DB;


class ArmtrName
{
public:
	static const std::string ROLE_WALK;
	static const std::string ROLE_IDLE;
	static const std::string ROLE_CHEER;
	static const std::string ROLE_NOR_ATT;
	static const std::string ROLE_NOR_DEF;
	static const std::string ROLE_ATT_BASE;
	static const std::string SKILL_NOR_ATT;

	static const std::string ROLE_STATE_ADD_DEF;
	static const std::string ROLE_STATE_ADD_HP;
	static const std::string ROLE_STATE_CLEAN;
	static const std::string ROLE_STATE_ADD_LV;
	static const std::string ROLE_STATE_RELIVE;
	static const std::string ROLE_STATE_NARROW;
	static const std::string ROLE_STATE_CONFUSE; 
	static const std::string ROLE_STATE_ABSORB_HP;
	static const std::string ROLE_STATE_ADD_ATT;

	static const std::string MONSTER_IDLE;
	static const std::string MONSTER_HURT;
	static const std::string MONSTER_CLOSE_ATT;
	static const std::string MONSTER_REMOTE_ATT;
	static const std::string MONSTER_DEATH;

	static const std::string ROLE_EVT_HURT;
	static const std::string ROLE_EVT_SKILL;
	static const std::string ROLE_EVT_SKILL_HURT;

	static const std::string MONSTER_EVT_HURT;
	static const std::string MONSTER_EVT_SKILL;
	static const std::string MONSTER_EVT_SKILL_HURT;
};


typedef enum _GameArmtrType
{
	TYPE_ROLE = 0,
	TYPE_MONSTER,
	TYPE_SKILL,
	TYPE_BUFF,
	TYPE_FB,
	TYPE_HIT,
	TYPE_ELE_ICON,
	TYPE_CRUSH_EFFECT,
	TYPE_CREATE_EFFECT,
	TYPE_ICON_SKILL,
	TYPE_ITEM_EFFECT,
	TYPE_SPECIAL,
	TYPE_SPECIAL_EX,
}GameArmtrType_E;


class GameArmtr : public Node
{
public:
	static const float ARMTR_SCALE_VALE;
	GameArmtr();
	virtual ~GameArmtr();

	static GameArmtr *createRole(const DragonBoneInfo_T *pInfo);	//0.role 1.monster 2.skill 3.skill state(buff) 4.feibiao 5.hit
	static GameArmtr *createSkill(const std::string& skillName);
	static GameArmtr *createDart();
	static GameArmtr *createMonster(const std::string& animName);
	static GameArmtr *createBuff(const std::string& buffName);
	static GameArmtr *createHit(const std::string& hitName);
	static GameArmtr *createEleIcon();
	static GameArmtr *createCrushEffect();
	static GameArmtr *createCreateEffect();
	static GameArmtr *createIconSkill();
	static GameArmtr *createItemEffect();
	static GameArmtr *createSpecial();
	static GameArmtr *createSpecialEx();
	static GameArmtr *create(const std::string &pathName, const std::string &dragonBoneName, const std::string &armtrName, int type);

	bool init(const std::string &pathName, const std::string &dragonBoneName, const std::string &armtrName, int type);

//	void playByIndex(int index);
	void play(const std::string& animationName, int durationTo = -1, int loop = -1);

	CC_SYNTHESIZE_READONLY(Armature *, _pArmtr, Armtr);
//	CC_SYNTHESIZE_READONLY(DBCCArmatureNode *, _pArmtr, Armtr);
private:
//	DBCCArmatureNode *_pArmtr;
	int _type;
};

#endif