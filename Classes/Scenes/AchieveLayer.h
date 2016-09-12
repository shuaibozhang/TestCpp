#ifndef __ACHIEVE_LAYER_H__
#define __ACHIEVE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;


class AchieveLayer : public cocos2d::LayerColor
{
public:
	AchieveLayer();
	~AchieveLayer();

	CREATE_FUNC(AchieveLayer);

	virtual bool init();

	void updateAchieveItem(int idx);

private:
	Node *_pRoot;
	std::string _arrStrContent[6];
	bool _isInit;
};


#endif
