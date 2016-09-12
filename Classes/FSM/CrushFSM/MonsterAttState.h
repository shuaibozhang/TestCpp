#ifndef __MONSTER_ATT_STATE_H__
#define __MONSTER_ATT_STATE_H__

#include "State.h"

class CrushLayer;

class MonsterAttState : public State<CrushLayer>
{
public:
	static MonsterAttState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static MonsterAttState *s_pInstance;
};
#endif
