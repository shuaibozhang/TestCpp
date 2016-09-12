#ifndef __GRADUALALPHA_H__
#define  __GRADUALALPHA_H__

#include "cocos2d.h"

USING_NS_CC;

class GradualAlphaSprite : public Sprite
{public:
	static GradualAlphaSprite* create(const std::string& name, bool isPlist = false);
	bool init(const std::string& name, bool isPlist);
};
#endif
