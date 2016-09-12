#ifndef __GAME_CSLOADER_H__
#define __GAME_CSLOADER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace cocostudio::timeline;

class GameCSLoader : public CSLoader
{
public:
	static GameCSLoader* getInstance();
	static void destroyInstance();

	GameCSLoader();

	static cocos2d::Node* createNode(const std::string& filename);
	static cocos2d::Node* createNode(const std::string& filename, const ccNodeLoadCallback& callback);
	static cocostudio::timeline::ActionTimeline* createTimeline(const std::string& filename);

protected:
	std::string getGUIClassName(const std::string &name);
	//cocos2d::Node* loadSprite(const rapidjson::Value& json);
};

#endif