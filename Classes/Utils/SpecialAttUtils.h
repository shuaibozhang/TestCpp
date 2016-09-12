#ifndef __SPECIAL_ATT_UTILS_H__
#define __SPECIAL_ATT_UTILS_H__

#include "cocos2d.h"
#include "ParamData.h"

class EleIcon;

class SpecialAttUtils
{
public:
	static void attPlayer(float damage);
	static void createWeak(int weakId, int count, int posType, CrushIndex_T posIndex);	//0:pos fixed 1:pos random
	static void createSpecEle(EleSpecialType specType, int eleId, int count, CrushIndex_T posIndex);
	static void addMonsterHp(float hp);
	static void openBox(int count);
	static int createStoneState();
	static void cleanStoneState(int eleId);
	static void createRandomS();
	static void createRandomKey(int round);

private:
	static void travelseThorn(int row, int col, int arrTag[ParamData::CRUSH_ROW][ParamData::CRUSH_COL], EleIcon *arrEle[ParamData::CRUSH_ROW][ParamData::CRUSH_COL]);
};


#endif