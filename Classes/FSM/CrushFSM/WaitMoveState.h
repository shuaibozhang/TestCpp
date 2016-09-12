#ifndef __WAIT_MOVE_STATE_H__
#define __WAIT_MOVE_STATE_H__

#include "State.h"

class CrushLayer;

class WaitMoveState : public State<CrushLayer>
{
public:
	static WaitMoveState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static WaitMoveState *s_pInstance;
};
#endif
