#ifndef __DAYACTIVITY_LAYER_H__
#define __DAYACTIVITY_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;


class DayActivityLayer : public cocos2d::LayerColor
{
public:
	DayActivityLayer();
	~DayActivityLayer();

	CREATE_FUNC(DayActivityLayer);

	virtual bool init();
	void updateBtn();
private:
	Node *_pRoot;
};


#endif
