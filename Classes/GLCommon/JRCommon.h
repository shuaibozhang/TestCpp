
#ifndef __JR_COMMON_H__
#define __JR_COMMON_H__

#include "cocos2d.h"

#include "Utils/ResMgr.h"
#include "Utils/ToolsUtil.h"
#include "Utils/JRTime.h"
#include "Utils/SoundsMgr.h"
#include "Utils/ParticleManager.h"
#include "Utils/NewSoundsMgr.h"

USING_NS_CC;


#define SCREENSIZE		(CCDirector::sharedDirector()->getWinSize())
#define SCREENCENTER	(ccp(SCREENSIZE.width / 2, SCREENSIZE.height / 2))

#define MAX_VALID_ATTACK_DEPTH (60)


typedef enum _ActionState {
	kActionStateNone = 0,
	kActionStateIdle,
	kActionStateAttack,
	kActionStateWalk,
    kActionStateHurt,
	kActionStateKnockedOut
} ActionState;



typedef struct _BoundingBox {
	Rect actual;
	Rect original;
} BoundingBox;


/*CCPoint operator*/
// inline cocos2d::CCPoint operator + (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2)
// {
// 	return ccp(v1.x + v2.x, v1.y + v2.y);
// }
// 
// inline cocos2d::CCPoint operator - (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2)
// {
// 	return ccp(v1.x - v2.x, v1.y - v2.y);
// }
// 
// inline cocos2d::CCPoint operator - (const cocos2d::CCPoint& v)
// {
// 	return ccp(-v.x, -v.y);
// }
// 
// inline cocos2d::CCPoint operator * (const cocos2d::CCPoint& v1, float scale)
// {
// 	return ccp(v1.x * scale, v1.y * scale);        
// }
// 
// inline cocos2d::CCPoint operator * (float scale, const cocos2d::CCPoint& v1)
// {
// 	return ccp(v1.x * scale, v1.y * scale);        
// }
// 
// inline cocos2d::CCPoint operator / (const cocos2d::CCPoint& v1, float scale)
// {
// 	return ccp(v1.x / scale, v1.y / scale);        
// }
// 
// inline bool operator == (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2)
// {
// 	return (v1.x == v2.x) && (v1.y == v2.y);
// }
// 
// inline bool operator != (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2)
// {
// 	return (v1.x != v2.x) || (v1.y != v2.y);
// }
/*CCPoint operator end*/

/*
 *
 */
/*
inline float getTileMapWidth(CCTMXTiledMap *pTileMap)
{
	return (pTileMap->getMapSize().width) * (pTileMap->getTileSize().width);
}

inline float getTileMapHeight(CCTMXTiledMap *pTileMap)
{
	return (pTileMap->getMapSize().height) * (pTileMap->getTileSize().height);
}*/


#endif