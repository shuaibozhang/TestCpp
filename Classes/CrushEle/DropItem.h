#ifndef __DROP_ITEM_H__
#define __DROP_ITEM_H__

#include "cocos2d.h"

USING_NS_CC;


class DropItem : public Node
{
public:
	DropItem();
	~DropItem();

	static DropItem *create(int itemId);

	virtual bool init(int itemId);
	virtual void setPosition(float x, float y);
	virtual void setPosition(const Vec2 &position);

	void update(float delta) override;

	CC_SYNTHESIZE_READONLY(int, _posId, PosId);
private:
	int _itemId;
//	int _posId; //0.lsj 1.cbd 2.lqc 3.sqy
	Sprite *_pShadow;
	Sprite *_pModel;
};


#endif