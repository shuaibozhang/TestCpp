#ifndef __FLY_ICON_H__
#define __FLY_ICON_H__

#include "cocos2d.h"

USING_NS_CC;


class FlyIcon : public Node
{
public:
	FlyIcon();
	~FlyIcon();

	static FlyIcon *create(int iconId, int iconLv);

	virtual bool init(int iconId, int iconLv);
//	virtual void setPosition(float x, float y);
//	virtual void setPosition(const Vec2 &position);

	void update(float delta) override;

	CC_SYNTHESIZE_READONLY(int, _iconId, IconId);
	CC_SYNTHESIZE_READONLY(int, _iconLv, IconLv);
private:
	Sprite *_pModel;
	float _totalTime;
};


#endif