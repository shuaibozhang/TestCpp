#ifndef __SETTLE_STATE_H__
#define __SETTLE_STATE_H__

#include "State.h"

class CrushLayer;

class SettleState : public State<CrushLayer>
{
public:
	static SettleState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static SettleState *s_pInstance;
};


#endif