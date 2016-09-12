#ifndef __GAME_GUIDE_LAYER_H__
#define __GAME_GUIDE_LAYER_H__

#include "cocos2d.h"
#include "Armtr/GameArmtr.h"

USING_NS_CC;


class GameGuideLayer : public LayerColor
{
public:
	virtual ~GameGuideLayer();

	static GameGuideLayer *getInstance();

	static GameGuideLayer *create(int guideType);
	virtual bool init(int guideType);

	bool onTouchBegan(Touch *touch, Event *evt);
	void onTouchEnded(Touch *touch, Event *evt);

	void guideStepStart(int touchType, const Vec2 &pos);
	void guideStepEnd();

	void setTouchInfo(int touchType, const Vec2 &pos);//0.all screen 1.touch guangquan
	void removeAllChildrenByTag(int tag);

	CC_SYNTHESIZE(int, _guideStep, GuideStep);
	CC_SYNTHESIZE(int, _guideType, GuideType);	//0.open box 1.weapon
	CC_SYNTHESIZE(int, _touchType, TouchType);	//0.all screen 1.touch guangquan
private:
	GameGuideLayer();
	static GameGuideLayer *s_pInstance;
	Vec2 _guidePos;
	Armature *_pArmtr;
	bool _isStart;
	
};


#endif // !__GAME_GUIDE_LAYER_H__

