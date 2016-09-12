#ifndef __NEW_WORLD_INFO_LAYER_H__
#define __NEW_WORLD_INFO_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class NewWorldInfo : public LayerColor
{
public:
	CREATE_FUNC(NewWorldInfo);

	bool init();
	void menuOnColorStone(Ref*, Widget::TouchEventType type);
	void showInfo();
private:
	Text* _infoText_0;
	Text* _infoText_1;
	Sprite* _selectIcon;
	Node* _mainroot;
};
#endif