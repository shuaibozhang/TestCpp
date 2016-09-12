#ifndef __BUFF_ROUND_STATE_H__
#define __BUFF_ROUND_STATE_H__

#include "State.h"

class CrushLayer;

class BuffRoundState : public State<CrushLayer>
{
public:
	static BuffRoundState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static BuffRoundState *s_pInstance;
};
#endif
