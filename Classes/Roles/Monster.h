#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "ParamData.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GameArmtr;
class RoundTag;
class RoleDialog;


typedef struct _MonsterWeakInfo 
{
	int attribute;
	int round;
	int curRound;
	RoundTag *pTag;
	float param;
}MonsterWeakInfo_T;



class Monster : public Node
{
public:
	Monster();
	virtual ~Monster();
	static Monster *create(int id, int posIndex);

	virtual bool init(int id, int posIndex);

	void doWeakPer();
	bool doRound();
	void startAtt();
//	void doAtt(int attIndex);
#if (CC_PLATFORM_IOS  == CC_TARGET_PLATFORM)
	bool doHurt(float damage, int attribute, long round, float attrParam);	//type:0 100 normal att
	bool doHurtByRole(float damage, int attribute, long round, float attrParam);	//type:0 100 normal att
#else
	bool doHurt(float damage, int attribute, int round, float attrParam);	//type:0 100 normal att
	bool doHurtByRole(float damage, int attribute, int round, float attrParam);	//type:0 100 normal att
#endif
	void doDead();
//	void doHurt(CrushArr_T *pCrushInfo, int roleId);
	void addHP(float addValue);

	void updateWeakIcon(float dt);

//	void eventHandler(cocos2d::EventCustom *event);
	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void animationSkillEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	void onSkillFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void playStateAnim(const std::string & animName);

	void animationStateEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	void startTalk(const std::string &talkId);
	void stopTalk();

//	void onFightEvent();
	void openBox();
	void dropBoxItem();

	CC_PROPERTY(bool, _isActive, IsActive);
	CC_SYNTHESIZE_READONLY(int, _monsterId, MonsterId);
	CC_SYNTHESIZE(bool, _isDead, IsDead);
	CC_SYNTHESIZE(bool, _isAttFinish, IsAttFinish);
	CC_SYNTHESIZE_READONLY(int, _posIndex, PosIndex);
	CC_SYNTHESIZE(int, _waveCount, WaveCount);
	CC_SYNTHESIZE(bool, _isLastMonster, IsLastMonster);
	CC_SYNTHESIZE(bool, _isBoxChange, IsBoxChange);
	CC_SYNTHESIZE_READONLY(const MonsterInfo_T *, _pMonsterInfo, MonsterInfo);
private:
	const DefAttrbt_T *getDefAttrbtInfo(int skillAttrbtId);

	void initAttData();
	void startAttAnim();
	void doAttPer();
	void endAttAnim();
	void playSkillAnim();
	void doEndAtt();

	bool isAttNear(int attId);
	bool isDrug();

	int getAnimId(int attId);

	GameArmtr *_pArmtr;
	int _curAttIndex;
	int _curRound;
	RoundTag *_pRound;
	float _hp;
	float _totalHp;
//	LoadingBar *_pHpBar;
	Node *_pHpBarNode;
	const std::map<int, MonsterAttArmInfo_T> *_pMonsterAttArmInfo;
	int _curAttCount;
	bool _isWithSkillAnim;
	Vec2 _oldPos;
//	MonsterWeakInfo_T _arrWeakInfo[AttAttrbt_E::ATT_COUNT];
	std::vector<MonsterWeakInfo_T> _arrWeakInfo;
	int _curShowWeakIndex;
	RoleDialog *_pDialog;
	float _attDamage;
};


#endif