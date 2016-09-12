#ifndef __FLY_WEAK_H__
#define __FLY_WEAK_H__

#include "cocos2d.h"

USING_NS_CC;


class FlyWeak : public Node
{
public:
	FlyWeak();
	~FlyWeak();

	static FlyWeak *create(int weakId, int roleId, float param);	//0.fire 1.boom 2.thunder 3.

	virtual bool init(int weakId, int roleId, float param);

	void update(float delta) override;

	CC_SYNTHESIZE_READONLY(int, _weakId, WeakId);
	CC_SYNTHESIZE_READONLY(int, _roleId, RoleId);
	CC_SYNTHESIZE_READONLY(int, _param, Param);
private:
	Sprite *_pModel;
	float _totalTime;
};


#endif