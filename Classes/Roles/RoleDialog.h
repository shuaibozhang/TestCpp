#ifndef __RoleDialog_DIALOG_H__
#define __RoleDialog_DIALOG_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


class RoleDialog : public Node
{
public:
	RoleDialog();
	virtual ~RoleDialog();

	static RoleDialog *create(const std::string & talkId, int rolePosIndex, int contentType = 0);

	virtual bool init(const std::string & talkId, int rolePosIndex, int contentType);
	virtual void update(float dt);
protected:
	int _rolePosIndex;
};

#endif