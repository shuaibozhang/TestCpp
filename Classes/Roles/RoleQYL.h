#ifndef __ROLE_QYL_H__
#define __ROLE_QYL_H__

#include "cocos2d.h"
#include "Role.h"

USING_NS_CC;


class RoleQyl : public Role
{
public:
	static RoleQyl *create(int id);

	virtual bool init(int id) override;
	virtual void update(float delta) override;
	virtual void changeWeapon(int weaponId) override;

protected:
	virtual void initAttData();
	virtual void initAttSkillData();
	virtual void initAttMonsterData();
	virtual void doAttPer();
	virtual bool isCanAtt();
};


#endif
