#ifndef __GAME_BUTTON_READER_H__
#define __GAME_BUTTON_READER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocostudio/WidgetReader/ButtonReader/ButtonReader.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
//using namespace cocostudio::timeline;

class GameButtonReader : public ButtonReader
{
	DECLARE_CLASS_NODE_READER_INFO
public:
	static GameButtonReader* getInstance();
	/** @deprecated Use method destroyInstance() instead */
	CC_DEPRECATED_ATTRIBUTE static void purge();
	static void destroyInstance();

	Node* createNodeWithFlatBuffers(const flatbuffers::Table *buttonOptions);
};



#endif // !__GAME_BUTTON_READER_H__