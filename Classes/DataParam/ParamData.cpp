#include "ParamData.h"
#include "VisibleRect.h"


const float ParamData::ELE_BG_WIDTH = 76.f;
const float ParamData::ELE_BG_HEIGHT = 76.f;
const float ParamData::ELE_SWAP_DUR = 0.2f;
float ParamData::CRUSH_BG_HEIGHT = 0.f;// 562.f;
const float ParamData::POS_X_INTERVAL = 400.f;// 640.f;
const float ParamData::ROLE_START_POS_X = 136.f;// 126.f;
//const float ParamData::ROLE_START_POS_Y = VisibleRect::getVisibleRect().getMaxY() - 318.f;////642.f;// 743.f;
float ParamData::ROLE_START_POS_Y;////642.f;// 743.f;
const float ParamData::MONSTER_START_POS_X = 510.f;
//const float ParamData::MONSTER_START_POS_Y = ParamData::ROLE_START_POS_Y;
float ParamData::MONSTER_START_POS_Y;
float ParamData::FIGHT_BG_HEIGHT = 0.f;
const float ParamData::WEAK_ROUND_DAMAGE_PARAM = 0.2f;
float ParamData::FIGHT_LAYER_POS_Y = 0.f;


const float ParamData::ELE_CRUSH_DUR = 0.5f;
const float ParamData::ELE_CRUSH_WAIT = 0.5f;
//const float ParamData::ELE_CREATE_DUR;
const float ParamData::ELE_CREATE_WAIT = 0.3f;
const float ParamData::ELE_FALL_DUR = 0.14f;////0.16f;
const float ParamData::ELE_FLY_DUR = ParamData::ELE_CRUSH_WAIT - 0.1f;

bool _AttactInfo::operator==(const _AttactInfo & other)
{
	if (attack != other.attack)
	{
		return false;
	}
	
	if (def != other.def)
	{
		return false;
	}

	if (dpAdd != other.dpAdd)
	{
		return false;
	}

	if (hpAdd != other.hpAdd)
	{
		return false;
	}

	return true;
}
