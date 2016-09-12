#ifndef __ROUND_TAG_H__
#define __ROUND_TAG_H__

#include "cocos2d.h"

USING_NS_CC;


class RoundTag : public Node
{
public:
	RoundTag();
	virtual ~RoundTag();

	static RoundTag *create(int tagType, int roundCount);	//type:0.eleicon 1.monster 2.monster weak icon 3.role weak icon
	static RoundTag *createWeakIcon(int tagType, int roundCount, int weakId);	//2.monster 3.role
	static RoundTag *createRoleBuff(int roundCount, int buffId);	//4.role buff		buffId:0.hp 1.def 2.relive 3.shield

	virtual bool init(int tagType, int roundCount, int paramId);

	bool doRound();
	bool setCurRound(int curRound);

	CC_SYNTHESIZE_READONLY(int, _curRound, CurRound);
	CC_SYNTHESIZE_READONLY(int, _tagType, TagType);
	CC_SYNTHESIZE_READONLY(int, _paramId, ParamId);

private:
	Sprite *_pBg;
	Label *_pLbl;

};

#endif