#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

#define ONLINE_DRAW_FLAG (10086)

class GameConfig {
public:
	static void loadGameConfig(void);

    // effect control
    static bool isEffectOn();
    static void switchEffectOn();
    static void switchEffectOff();

	static void setEffectValue(int value);
	static int getEffectValue();
    
    // Background music control
    static bool isMusicOn();
    static void switchMusicOn();
	static void switchMusicOff();
    
	static void setMusicValue(int valse);
	static int getMusicValue();

    static bool isFirstPlay();
    static void clearFirsPlay();

	static bool isAuthorized(void);
};

#endif /* defined(__CONFIG_H__) */
