#ifndef __GAME_OL_CONFIG_H__
#define __GAME_OL_CONFIG_H__

#include "cocos2d.h"
#include "ParamData.h"
#include <iostream>

USING_NS_CC;


class GameOLConfig 
{
public:

	static GameOLConfig *getInstance();


private:
	static GameOLConfig *s_pInstance;
	GameOLConfig();
};

#endif /* defined(__GAME_OL_CONFIG_H__) */
