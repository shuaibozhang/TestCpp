#ifndef __FALL_ANIM_STATE_H__
#define __FALL_ANIM_STATE_H__

#include "State.h"

class CrushLayer;

class FallAnimState : public State<CrushLayer>
{
public:
	static FallAnimState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static FallAnimState *s_pInstance;
};
#endif
