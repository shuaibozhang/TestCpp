#ifndef __WAIT_TOUCH_STATE_H__
#define __WAIT_TOUCH_STATE_H__

#include "State.h"

class CrushLayer;

class WaitTouchState : public State<CrushLayer>
{
public:
	static WaitTouchState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static WaitTouchState *s_pInstance;
	bool _isCanSwapCrush;
};

#endif
