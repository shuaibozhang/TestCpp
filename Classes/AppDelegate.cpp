#include "AppDelegate.h"
#include "effects/GLProgramMgr.h"
#include "Scenes/MainScene.h"
#include "DataParam/UserData.h"
#include "GameLayer.h"
#include "VisibleRect.h"
#include "JRCommon.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "ParamMgr.h"
#include "GameConfig.h"
#include "Scenes/StartScene.h"
#include "GameUtils.h"
#include "AchieveMgr.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//#include<vld.h>
#endif

USING_NS_CC;
using namespace cocos2d::experimental;


static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	CocosDenshion::SimpleAudioEngine::getInstance()->end();
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	AudioEngine::end();
#endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		float scaleParam = 0.8f;
        glview = GLViewImpl::createWithRect("LongMen", Rect(0, 0, 640*scaleParam, 1100*scaleParam));
#else
        glview = GLViewImpl::create("LongMen");
#endif
        director->setOpenGLView(glview);
	}
	ZipUtils::setPvrEncryptionKeyPart(0, 0x918abce8);
	ZipUtils::setPvrEncryptionKeyPart(1, 0x9b0b320a);
	ZipUtils::setPvrEncryptionKeyPart(2, 0xea56f4f5);
	ZipUtils::setPvrEncryptionKeyPart(3, 0x2f4a0270);
	Image::setPVRImagesHavePremultipliedAlpha(true);
	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::AUTO);
    // turn on display FPS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)// || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    Size frameSize = glview->getFrameSize();

  /*  if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }

    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }

    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }*/

    register_all_packages();

	if (UserData::getInstance()->getIsFirstPlay(false) == 1)
	{
//		UserData::getInstance()->giveGold(10000);
		UserData::getInstance()->setIsFirstPlay(0, false);
		UserData::getInstance()->giveItem("itemid_good_weapon_500", 1, false);
		UserData::getInstance()->giveItem("itemid_good_weapon_510", 1, false);
		UserData::getInstance()->giveItem("itemid_good_weapon_520", 1, false);
		UserData::getInstance()->giveItem("itemid_good_weapon_530", 1, false);

		//lsj
		UserData::getInstance()->giveItem("itemid_good_skill_0", 1, false);
		UserData::getInstance()->giveItem("itemid_good_skill_6", 1, false);
		//cbd
		UserData::getInstance()->giveItem("itemid_good_skill_9", 1, false);
		UserData::getInstance()->giveItem("itemid_good_skill_8", 1, false);
		UserData::getInstance()->giveItem("itemid_good_skill_15", 1, false);
		//lqc
		UserData::getInstance()->giveItem("itemid_good_skill_20", 1, false);
		UserData::getInstance()->giveItem("itemid_good_skill_29", 1, false);
		//sqy
		UserData::getInstance()->giveItem("itemid_good_skill_32", 1, false);
		UserData::getInstance()->giveItem("itemid_good_skill_43", 1, false);

		//lsj
		UserData::getInstance()->setSkillEquip(0, 0, true, false);
		UserData::getInstance()->setSkillEquip(0, 6, true, false);
		//cbd
		UserData::getInstance()->setSkillEquip(1, 9, true, false);
		UserData::getInstance()->setSkillEquip(1, 8, true, false);
		UserData::getInstance()->setSkillEquip(1, 15, true, false);
		//lqc
		UserData::getInstance()->setSkillEquip(2, 20, true, false);
		UserData::getInstance()->setSkillEquip(2, 29, true, false);
		//sqy
		UserData::getInstance()->setSkillEquip(3, 32, true, false);
		UserData::getInstance()->setSkillEquip(3, 43, true, false);

		UserData::getInstance()->setWeaponEquip(500, true);
		UserData::getInstance()->setWeaponEquip(510, true);
		UserData::getInstance()->setWeaponEquip(520, true);
		UserData::getInstance()->setWeaponEquip(530, true);

		UserData::getInstance()->setWeaponLv(0, 1);
		UserData::getInstance()->setWeaponLv(10, 1);
		UserData::getInstance()->setWeaponLv(20, 1);
		UserData::getInstance()->setWeaponLv(30, 1);

		UserData::getInstance()->addWeaponPartsNum(1500, 20);
		UserData::getInstance()->giveTili(5);
		UserData::getInstance()->giveCrystal(10);
		UserData::getInstance()->giveGold(100);

 		UserData::getInstance()->setEquipConfig(0, 1000); 
 		UserData::getInstance()->setEquipConfig(1, 1000);

		AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::WEAPON, 4);

		UserData::getInstance()->saveUserData(false);
	}

	if (VisibleRect::getVisibleRect().getMaxY() > (580.f*2))
	{
		ParamData::FIGHT_BG_HEIGHT = 580.f;
		ParamData::CRUSH_BG_HEIGHT = VisibleRect::getVisibleRect().getMaxY() - ParamData::FIGHT_BG_HEIGHT;
//		ParamData::ROLE_START_POS_Y = ParamData::CRUSH_BG_HEIGHT + 62.f;// VisibleRect::getVisibleRect().getMaxY() - 282.f;////642.f;// 743.f;
	}
	else
	{
		ParamData::CRUSH_BG_HEIGHT = 580.f;
		ParamData::FIGHT_BG_HEIGHT = VisibleRect::getVisibleRect().getMaxY() - ParamData::CRUSH_BG_HEIGHT;
//		ParamData::ROLE_START_POS_Y = 960.f - 282.f;////642.f;// 743.f;
	}
	ParamData::ROLE_START_POS_Y = ParamData::CRUSH_BG_HEIGHT + 90.f;//82.f;//62.f;
	ParamData::MONSTER_START_POS_Y = ParamData::ROLE_START_POS_Y;
	ParamData::FIGHT_LAYER_POS_Y = -(ParamData::CRUSH_BG_HEIGHT - 50.f);

	GameConfig::loadGameConfig();

	auto scene = StartScene::scene();
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	//auto scene = Director::getInstance()->getRunningScene();		
	//auto action = Sequence::createWithTwoActions(DelayTime::create(0.0001f), CallFunc::create([=]() {
	//		GLProgramMgr::getInstance()->reloadGLProgram();
	//		CCLOG("reloadGLProgram");
	//	}));
	//	scene->runAction(action);	
    // if you use SimpleAudioEngine, it must be pause

#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	AudioEngine::pauseAll();
#endif
	//GLProgramMgr::getInstance()->reloadGLProgram();
	UserData::getInstance()->saveUserData(true);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

	if (nullptr != MainLayer::getCurMainLayer())
	{
		NetDataMgr::getInstance()->updateRole(CC_CALLBACK_2(MainLayer::onUpdateSelfInfoCallback, MainLayer::getCurMainLayer()));
	}
	// if you use SimpleAudioEngine, it must resume here
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	AudioEngine::resumeAll();
#endif
}
