#ifndef __ELE_WEAK_H__
#define __ELE_WEAK_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;


class EleWeak : public Node
{
public:
	EleWeak();
	virtual ~EleWeak();

	static EleWeak *create(int id);
	virtual bool init(int id);

private:
	EleWeakInfo_T _info;
};

#endif