/********************************************************************
	created:	2013/10/29
	created:	29:10:2013   15:48
	file base:	GameUtils
	file ext:	h
	author:		Dragon.Zhou
	
	purpose:	
*********************************************************************/
#ifndef __GAME_UTILS_H__
#define __GAME_UTILS_H__

#include "cocos2d.h"
#include "DataParam/ParamData.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace std;
using namespace network;

#define GAME_AUDIO_API_OLD	0
#define GAME_AUDIO_API_NEW	1

#ifndef GAME_AUDIO_TARGET_API
#define GAME_AUDIO_TARGET_API GAME_AUDIO_API_OLD
#endif // !GAME_AUDIO_API



class GameEntity;


class GameUtils
{
public:
	static void preloadEffect(const char* pszFilePath);
	static void unloadEffect(const char* pszFilePath);
	static unsigned int playEffect(const char* pszFilePath, bool bLoop = false);
	static void stopEffect(unsigned int nSoundId);
	static void stopAllEffect();
	
	static void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);

	static void stopBackgroundMusic(bool bReleaseData = false);

	static int getOnlineValue(const char *strKey, int defaultValue = 0);

//	static void getOnlineTime(tm& time);

	static void getRectPosInWorld(Node *pNode, const cocos2d::Rect &boundingBox, Vec2 *arrPos);
	static bool isNodeRectCollision(Node *pNode1, const cocos2d::Rect &boundingBox1, Node *pNode2, const cocos2d::Rect &boundingBox2);
	
	static float getMoveDur(Vec2 offset, float speed);

	static bool isAdsLoaded();

	static void getDataFromServicer(const char *strUrl, const char *strReqDesc, const char *strReqData, const char *strTag, const ccHttpRequestCallback& callback);
	static void postDataToServicer(const char *strUrl, const char *strReqDesc, const char *strReqData, const char *strTag, const ccHttpRequestCallback& callback);

	static void toastTip(int tipId);	//0.crystal unenough	
	static void toastTip(const std::string &strKey);	//0.crystal unenough	
private:
	static int s_curBgmIdx;
	static string s_strBgmName;
	static float s_effectVolume;
	static float s_bgmVolume;
};

#endif