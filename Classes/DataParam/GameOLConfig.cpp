#include "GameOLConfig.h"
#include "GameUtils.h"
#include "platBridge/cocos2dx_analyze.h"
#include "platBridge/cocos2dx_plat.h"
#include "JRCommon.h"


using namespace std;


const char *STR_OL_CLOSE_2ND = "close_2nd_";
const char *STR_OL_POP_PAY = "pop_pay_";
const char *STR_OL_GUIDE_PAY = "guide_pay_";
const char *STR_OL_FINGER = "finger_";
const char *STR_OL_DRAW = "can_draw_money";
const char *STR_OL_CLOSE_VER = "close_";

GameOLConfig *GameOLConfig::s_pInstance = nullptr;


GameOLConfig::GameOLConfig()
{
}


GameOLConfig *GameOLConfig::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new GameOLConfig();
	}
	
	return s_pInstance;
}