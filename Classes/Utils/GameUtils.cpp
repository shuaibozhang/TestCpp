#include "GameUtils.h"
#include "Defines.h"
#include "GLCommon/JRCommon.h"
#include "platBridge/cocos2dx_plat.h"
#include "platBridge/cocos2dx_analyze.h"
#include "DataParam/GameConfig.h"
#include "DataParam/UserData.h"
#include "GLCommon/utils/SoundsMgr.h"
#include "StoreBridge/cocos2dx_StoreInventory.h"
#include "VisibleRect.h"
#include "Widgets/GameCSLoader.h"

using namespace std;
USING_NS_CC;
using namespace cocos2d::experimental;


int GameUtils::s_curBgmIdx = AudioEngine::INVALID_AUDIO_ID;
string GameUtils::s_strBgmName = "";
float GameUtils::s_bgmVolume = 1.f;
float GameUtils::s_effectVolume = 1.f;

/*
CCRect GameUtils::getActualRect(CCNode *pContainer, const CCRect &objRect)
{
	CCPoint conatinerPos = pContainer->getPosition();

	return CCRectMake(conatinerPos.x + objRect.getMinX(), conatinerPos.y + objRect.getMinY(), objRect.size.width, objRect.size.height);
}


bool GameUtils::isNodeIntersectsRect(CCNode *pNode1, const CCRect &rBox1, CCNode *pNode2, const CCRect &rBox2)
{
	CCRect rActualBox1 = GameUtils::getActualRect(pNode1, rBox1);
	CCRect rActualBox2 = GameUtils::getActualRect(pNode2, rBox2);

	return rActualBox1.intersectsRect(rActualBox2);
}


bool GameUtils::isLeftOutScreen(CCNode *pContainer, CCNode *pObj)
{
	bool bRet = false;

	if (pContainer->getPositionX() + pObj->getPositionX() < 0.f)
	{
		bRet = true;
	}

	return bRet;
}*/


void GameUtils::preloadEffect(const char* pszFilePath)
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	SoundsMgr::getInstance()->preloadEffect(pszFilePath);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	NewSoundsMgr::getInstance()->preload(pszFilePath);
#endif
}


void GameUtils::unloadEffect(const char* pszFilePath)
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	SoundsMgr::getInstance()->unloadEffect(pszFilePath);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	NewSoundsMgr::getInstance()->unload(pszFilePath);
#endif
}


unsigned int GameUtils::playEffect(const char* pszFilePath, bool bLoop)
{
	unsigned int ret = 0;

	if (GameConfig::isEffectOn())
	{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
		ret = SoundsMgr::getInstance()->playEffect(pszFilePath, bLoop);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
		ret = NewSoundsMgr::getInstance()->play(pszFilePath, bLoop, s_effectVolume);
#endif
	}	

	return ret;
}


void GameUtils::stopEffect(unsigned int nSoundId)
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	SoundsMgr::getInstance()->stopEffect(nSoundId);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	NewSoundsMgr::getInstance()->stop(nSoundId);
#endif
}


void GameUtils::stopAllEffect()
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	SoundsMgr::getInstance()->stopAllEffect();
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
//	NewSoundsMgr::getInstance()->stopAllEffect();
#endif
}


void GameUtils::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if (GameConfig::isMusicOn())
	{
		if (nullptr != pszFilePath)
		{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
			SoundsMgr::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
			s_curBgmIdx = NewSoundsMgr::getInstance()->play(pszFilePath, bLoop, s_bgmVolume);
#endif
		}
		else if (0 != s_strBgmName.compare(""))
		{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
			SoundsMgr::getInstance()->playBackgroundMusic(s_strBgmName.c_str(), bLoop);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
			s_curBgmIdx = NewSoundsMgr::getInstance()->play(s_strBgmName.c_str(), bLoop, s_bgmVolume);
#endif
		}
	}

	if (nullptr != pszFilePath)
	{
		GameUtils::s_strBgmName = pszFilePath;
	}
}


void GameUtils::stopBackgroundMusic(bool bReleaseData)
{
#if GAME_AUDIO_TARGET_API == GAME_AUDIO_API_OLD
	SoundsMgr::getInstance()->stopBackgroundMusic(bReleaseData);
#elif GAME_AUDIO_TARGET_API == GAME_AUDIO_API_NEW
	if (s_curBgmIdx != AudioEngine::INVALID_AUDIO_ID)
	{
		NewSoundsMgr::getInstance()->stop(s_curBgmIdx);
		s_curBgmIdx = AudioEngine::INVALID_AUDIO_ID;
	}
#endif
}


/*CCString *GameUtils::getRegisterItemId(void)
{
	int onlineValue = 0;
	CCString *strItemId = NULL;

	onlineValue = GameUtils::getOnlineValue("register_price", 2);

	switch (onlineValue)
	{
	case 0:
	case 2:
		strItemId = CCString::createWithFormat("itemid_good_register");
		break;
	case 1:
		strItemId = CCString::createWithFormat("itemid_good_register");
		break;
	default:
		strItemId = CCString::createWithFormat("itemid_good_register");
		break;
	}

	return strItemId;
}*/



int GameUtils::getOnlineValue(const char *strKey, int defaultValue)
{
	int onlineValue = defaultValue;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

//	char *pStrChannel = (char *)cocos2dx_plat::getChannelId();
	string strChannel = cocos2dx_plat::getChannelId();
	string strChannelId(strChannel);
	string strKeyWithChannel(strKey);
	strKeyWithChannel.append("_");
	strKeyWithChannel.append(strChannelId);
//	char *pStrValue = (char *)cocos2dx_analyze::getOnlineValue(strKeyWithChannel.c_str());
//	onlineValue = CCString::create(pStrValue)->intValue();
	string strValue = cocos2dx_analyze::getOnlineValue(strKeyWithChannel.c_str());
	onlineValue = __String::create(strValue)->intValue();

	if (onlineValue <= 0)
	{
//		char *pStrDefault = (char *)cocos2dx_analyze::getOnlineValue(strKey);
//		int defaultOnlineValue = CCString::create(pStrDefault)->intValue();
		string strDefault = cocos2dx_analyze::getOnlineValue(strKey);
		int defaultOnlineValue = __String::create(strDefault)->intValue();

		if (defaultOnlineValue <= 0)
		{
			defaultOnlineValue = defaultValue;
		}

		onlineValue = defaultOnlineValue;

//		CC_SAFE_FREE(pStrDefault);
	}

//	CC_SAFE_FREE(pStrChannel);

//	CC_SAFE_FREE(pStrValue);
#endif

	return onlineValue;
}
		

void GameUtils::getRectPosInWorld(Node *pNode, const Rect &boundingBox, Vec2 *arrPos)
{
	arrPos[0] = pNode->convertToWorldSpaceAR(Vec2(boundingBox.getMinX(), boundingBox.getMaxY()));
	arrPos[1] = pNode->convertToWorldSpaceAR(Vec2(boundingBox.getMaxX(), boundingBox.getMaxY()));
	arrPos[2] = pNode->convertToWorldSpaceAR(Vec2(boundingBox.getMaxX(), boundingBox.getMinY()));
	arrPos[3] = pNode->convertToWorldSpaceAR(Vec2(boundingBox.getMinX(), boundingBox.getMinY()));
}

/*
bool GameUtils::isEntityRectCollision(GameEntity *pEntity1, GameEntity *pEntity2)
{
	return isNodeRectCollision(pEntity1, pEntity1->getBoundingRect(), pEntity2, pEntity2->getBoundingRect());
}*/


bool GameUtils::isNodeRectCollision(Node *pNode1, const Rect &boundingBox1, Node *pNode2, const Rect &boundingBox2)
{
	Vec2 arrPos1[4];
	Vec2 arrPos2[4];

	getRectPosInWorld(pNode1, boundingBox1, arrPos1);
	getRectPosInWorld(pNode2, boundingBox2, arrPos2);

	return ToolsUtil::checkRotateRectColision(arrPos1, arrPos2);
}


float GameUtils::getMoveDur(Vec2 offset, float speed)
{
	float ret = 0.f;

	ret = offset.length() / speed;

	return ret;
}


bool GameUtils::isAdsLoaded()
{
#if (CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
    return MainLayer::getCurMainLayer()->getAdBtn()->isVisible();
#else
	return false;
#endif
}

void GameUtils::getDataFromServicer(const char *strUrl, const char *strReqDesc, const char *strReqData, const char *strTag, const ccHttpRequestCallback& callback)
{
	string strFullUrl(strUrl);
	strFullUrl.append("/");
	strFullUrl.append(strReqDesc);
	strFullUrl.append("?");
	//	strFullUrl.append("?req=");
	strFullUrl.append(strReqData);

	HttpRequest* request = new HttpRequest();
	request->setUrl(strFullUrl.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(callback);
//	request->setResponseCallback(pTarget, pSelector);
	request->setTag(strTag);

	HttpClient::getInstance()->send(request);
	request->release();
}


void GameUtils::postDataToServicer(const char *strUrl, const char *strReqDesc, const char *strReqData, const char *strTag, const ccHttpRequestCallback& callback)
{
	string strFullUrl(strUrl);
	strFullUrl.append("/");
	strFullUrl.append(strReqDesc);

	HttpRequest* request = new HttpRequest();
	request->setUrl(strFullUrl.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setRequestData(strReqData, strlen(strReqData));
	request->setResponseCallback(callback);
//	request->setResponseCallback(pTarget, pSelector);
	request->setTag(strTag);

	HttpClient::getInstance()->send(request);
	request->release();
}

void GameUtils::toastTip(int tipId)
{
	auto strKey = __String::createWithFormat("toast_tip_%d", tipId);

	GameUtils::toastTip(strKey->getCString());
}

void GameUtils::toastTip(const std::string & strKey)
{
	auto pContent = ResMgr::getInstance()->getString(strKey.c_str());

	cocos2dx_plat::showToast(pContent->getCString());
}
