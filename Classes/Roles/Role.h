#ifndef __ROLE_H__
#define __ROLE_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


class GameArmtr;
class Monster;
class RoleDialog;


class Role : public Node
{
public:
	Role();
	virtual ~Role();
//	static Role *create(int id);

	virtual bool init(int id);
	virtual void update(float delta);

	void initFightData();
//	void resetFightData();

	void addAttInfo(CrushArr_T *pCrushInfo);

	void playMove();
	void playIdle();
	void playHurt();
	void playStateAnim(const std::string& animName);
	void playHitAnim(const std::string& animName);
	void playAnim(const std::string& animName);
	void playStartDefAnim();
	void playEndDefAnim();
	void playDefAnim();
	void playCheer();

	void startTalk(const std::string &talkId, int contentType = 0);	//0.xml 1.text
	void stopTalk();

	void playBuffAttAnim();
	void stopBuffAttAnim();

	virtual void changeWeapon(int weaponId) = 0;

	bool isAttFinish();
//	void eventHandler(cocos2d::EventCustom *event);

	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void animationSkillEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	void onSkillFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void animationStateEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	CC_SYNTHESIZE_READONLY(int, _roleId, RoleId);
	CC_SYNTHESIZE_READONLY(bool, _isCurAttFinish, IsCurAttFinish);
	CC_SYNTHESIZE(int, _rolePosIndex, RolePosIndex);
protected:
	virtual void initAttData() = 0;
	virtual void initAttSkillData() = 0;
	virtual void initAttMonsterData() = 0;
	virtual void startAttAnim();
	virtual void doAttPer() = 0;
	virtual void endAttAnim();
	virtual void playSkillAnim();

	virtual bool isCanAtt();

	void throwDart();
	void endThrowDart();

	Monster *getTargetMonster();

	GameArmtr *_pArmtr;
	std::queue<CrushArr_T> _attInfoQueue;
	RoleCurAttInfo_T _curAttInfo;
	Skill_T _norAttInfo; 
	const PlayerInfo_T *_pRoleData;
	RoleDialog *_pRoleDialog; 
	GameArmtr *_pBuffAtt;
};

#endif