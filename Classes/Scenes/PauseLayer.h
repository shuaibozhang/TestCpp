#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;

class PauseLayer : public Layer
{
public:
	CREATE_FUNC(PauseLayer);

	virtual bool init()override;

	void updateUi();
	Button* _btn0;
	Button* _btn1;
	Button* _btn2;
	Button* _btn3;
};
#endif
