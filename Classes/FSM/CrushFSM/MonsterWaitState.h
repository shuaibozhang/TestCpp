#ifndef __MONSTER_WAIT_STATE_H__
#define __MONSTER_WAIT_STATE_H__

#include "State.h"

class CrushLayer;

class MonsterWaitState : public State<CrushLayer>
{
public:
	static MonsterWaitState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static MonsterWaitState *s_pInstance;
};
#endif
