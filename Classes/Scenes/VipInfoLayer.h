#ifndef _VIPINFOLAYER_H_
#define _VIPINFOLAYER_H_

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class VipInfoLayer : public cocos2d::Layer
{
public:
	VipInfoLayer();
	~VipInfoLayer();
	CREATE_FUNC(VipInfoLayer);
	bool init()override;
	void updateInfo();
	void touchBuyBtn(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
	void touchGetRewardBtn(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
private:
	Node* _infoNode;
	ui::Button* _buyBtn;
	ui::Button* _getRewardBtn;
};
#endif
