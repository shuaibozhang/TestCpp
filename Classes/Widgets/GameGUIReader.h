#ifndef __GAME_GUI_READER_H__
#define __GAME_GUI_READER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;


class GameGUIReader : public GUIReader
{
public:
	CC_DEPRECATED_ATTRIBUTE static GameGUIReader* shareReader() { return GameGUIReader::getInstance(); };
	CC_DEPRECATED_ATTRIBUTE static void purgeGUIReader() { GameGUIReader::destroyInstance(); };

	static GameGUIReader* getInstance();
	static void destroyInstance();

	virtual cocos2d::ui::Widget* widgetFromJsonFile(const char* fileName);


protected:
	GameGUIReader();
};


class GameWidgetPropertiesReader0300 : public WidgetPropertiesReader0300
{
protected:
	std::string getGUIClassName(const std::string& name);
};
#endif // !__GAME_GUI_READER_H__