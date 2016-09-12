#ifndef __SWAP_ANIM_STATE_H__
#define __SWAP_ANIM_STATE_H__

#include "State.h"

class CrushLayer;

class SwapAnimState : public State<CrushLayer>
{
public:
	static SwapAnimState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static SwapAnimState *s_pInstance;
};
#endif
