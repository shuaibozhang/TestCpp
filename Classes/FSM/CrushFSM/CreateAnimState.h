#ifndef __CREATE_ANIM_STATE_H__
#define __CREATE_ANIM_STATE_H__

#include "State.h"

class CrushLayer;

class CreateAnimState : public State<CrushLayer>
{
public:
	static CreateAnimState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static CreateAnimState *s_pInstance;
};
#endif
