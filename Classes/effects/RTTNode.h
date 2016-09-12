#ifndef __RTTNODE_H__
#define __RTTNODE_H__
#include "cocos2d.h"
USING_NS_CC;

enum RTTEffect_E
{
	RTTEFFECT_DEFAULT = 0,
	RTTEFFECT_LOWHP,
	RTTEFFECT_GRADUALALPHA
};

class RTTNode : public RenderTexture
{
public:
	~RTTNode();
	static RTTNode* create(int w, int h);
	bool init(int w, int h);

	void setRTTEffect(RTTEffect_E effect, bool fauseupdata = false);
	virtual void setPosition(float x, float y);
	Action* runAction(Action* action);
	void stopAction(Action* action);
private:
	GLProgramState* _programState;
	RTTEffect_E _effectType;
	EventListenerCustom * _backToForegroundlistener;
};

#endif
