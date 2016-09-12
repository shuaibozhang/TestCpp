#ifndef __CHECK_CRUSH_STATE_H__
#define __CHECK_CRUSH_STATE_H__

#include "State.h"

class CrushLayer;

class CheckCrushState : public State<CrushLayer>
{
public:
	static CheckCrushState *getInstance();

	virtual void enter(CrushLayer *pOwner);
	virtual void execute(CrushLayer *pOwner);
	virtual void exit(CrushLayer *pOwner);
	virtual void handleMessage(CrushLayer *pOwner, EventCustom *event);
private:
	static CheckCrushState *s_pInstance;
};
#endif
