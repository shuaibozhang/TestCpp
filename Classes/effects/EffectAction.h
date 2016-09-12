#ifndef __EFFECTACTION_H__
#define __EFFECTACTION_H__

#include "cocos2d.h"
USING_NS_CC;

enum EffectActionType_E
{
	EFFECTACTION_GRADUAL_DISPPEAR_OUT = 0,
	EFFECTACTION_GRADUAL_SHOW,
	EFFECTACTION_ARCHING_SHOW_OUT,
	EFFECTACTION_ARCHING_SHOW,
	EFFECTACTION_DEFAULT,
};
class EffectAction : public ActionInterval
{
public:
	~EffectAction();

	static EffectAction* create(float fDuration, EffectActionType_E type, bool keep = false);
	bool initWithDuration(float fDuration, EffectActionType_E type, bool keep);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(float time);

protected:
	/*before run effectaction, if target's shader is same with the action, we can use _bkeep = true to keep the state of glprogam*/
	bool _bKeep;
	EffectActionType_E _effectType;
	Sprite* _target;
};


//class EffectMoveBy : public MoveBy
//{
//public:
//	static EffectMoveBy*  create(float dut, const Vec2& pos);
//	virtual void update(float time)override;
//};
#endif
