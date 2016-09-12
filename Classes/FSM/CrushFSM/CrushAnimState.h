#ifndef __CRUSH_ANIM_STATE_H__
#define __CRUSH_ANIM_STATE_H__

#include "State.h"

class CrushLayer;

class CrushAnimState : public State<CrushLayer>
{
public:
	static CrushAnimState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static CrushAnimState *s_pInstance;
};
#endif
