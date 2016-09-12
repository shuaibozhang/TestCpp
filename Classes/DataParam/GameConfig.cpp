#include "GameConfig.h"
#include <string>
#include "platBridge/cocos2dx_plat.h"
#include "Defines.h"
#include "GameUtils.h"
#include "UserData.h"
#include "GameOLConfig.h"

using namespace std;

static bool s_bIsSoundOn = true;
static bool s_bIsMusicOn = true;
static bool s_bIsFirstPlay = true;
static bool s_bIsAuthorized = false;


void GameConfig::loadGameConfig(void)
{
	s_bIsSoundOn = UserDefault::getInstance()->getBoolForKey("sound", true);
	s_bIsMusicOn = UserDefault::getInstance()->getBoolForKey("music", true);
	s_bIsFirstPlay = UserDefault::getInstance()->getBoolForKey("firstplay", true);

	setMusicValue(getMusicValue());
	setEffectValue(getEffectValue());	
	/*
#if(CC_PAY_SDK == PAY_SDK_JIDI)
	if(cocos2dx_plat::getIsSoundOn().compare("true") == 0)
	{
		s_bIsMusicOn = true;
		s_bIsSoundOn = true;
	}
	else
	{
		s_bIsMusicOn = false;
		s_bIsSoundOn = false;
	}
#endif*/

#if (0 == CC_ENABLE_CHECK_SIGN)
	s_bIsAuthorized = true;
#else
//	char *pStrSign = (char *)cocos2dx_plat::getSignCode();
	string strSign = cocos2dx_plat::getSignCode();
	int nSignCode = __String::create(strSign)->intValue();

	if (-1920181079 == nSignCode)
	{
		s_bIsAuthorized = true;
	}
//	CC_SAFE_FREE(pStrSign);

#if (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)
	s_bIsAuthorized = true;
#endif

#endif	
}

bool GameConfig::isEffectOn()
{
	return s_bIsSoundOn;
}

void GameConfig::switchEffectOn()
{
	s_bIsSoundOn = true;
    UserDefault::getInstance()->setBoolForKey("sound", true);
    UserDefault::getInstance()->flush();
}

void GameConfig::switchEffectOff()
{
	s_bIsSoundOn = false;
    UserDefault::getInstance()->setBoolForKey("sound", false);
    UserDefault::getInstance()->flush();
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}


void GameConfig::setEffectValue(int value)
{
	UserDefault::getInstance()->setFloatForKey("effect_value", value);
//	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value / 100.f);
}


int GameConfig::getEffectValue()
{
	return UserDefault::getInstance()->getFloatForKey("effect_value", 100);
}


void GameConfig::setMusicValue(int value)
{
	UserDefault::getInstance()->setFloatForKey("music_value", value);
//	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value / 100.f);
}


int GameConfig::getMusicValue()
{
	return UserDefault::getInstance()->getFloatForKey("music_value", 100);
}


bool GameConfig::isMusicOn()
{
    return s_bIsMusicOn;
}

void GameConfig::switchMusicOn()
{
	s_bIsMusicOn = true;
    UserDefault::getInstance()->setBoolForKey("music", true);
    UserDefault::getInstance()->flush();
}

void GameConfig::switchMusicOff()
{
	s_bIsMusicOn = false;
    UserDefault::getInstance()->setBoolForKey("music", false);
    UserDefault::getInstance()->flush();
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}


bool GameConfig::isFirstPlay()
{
    return s_bIsFirstPlay;
}


void GameConfig::clearFirsPlay()
{
	s_bIsFirstPlay = false;
    UserDefault::getInstance()->setBoolForKey("firstplay", false);
    UserDefault::getInstance()->flush();
}


bool GameConfig::isAuthorized(void)
{
	return s_bIsAuthorized;
}