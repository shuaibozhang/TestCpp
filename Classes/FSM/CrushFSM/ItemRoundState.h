#ifndef __ITEM_ROUND_STATE_H__
#define __ITEM_ROUND_STATE_H__

#include "State.h"

class CrushLayer;

class ItemRoundState : public State<CrushLayer>
{
public:
	static ItemRoundState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static ItemRoundState *s_pInstance;
};
#endif
