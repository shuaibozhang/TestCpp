#include "GameButton.h"
#include "GameUtils.h"


IMPLEMENT_CLASS_GUI_INFO(GameButton)


GameButton::GameButton()
{
}


GameButton::~GameButton()
{

}

#if 0
GameButton* GameButton::create()
{
	GameButton* widget = new GameButton();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}


GameButton* GameButton::create(const std::string &normalImage,
					   const std::string& selectedImage ,
					   const std::string& disableImage,
					   TextureResType texType)
{
	GameButton *btn = new GameButton;
	if (btn && btn->init(normalImage,selectedImage,disableImage,texType)) {
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}
#endif

GameButton* GameButton::create()
{
	GameButton* widget = new (std::nothrow) GameButton();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

GameButton* GameButton::create(const std::string &normalImage,
					   const std::string& selectedImage ,
					   const std::string& disableImage,
					   TextureResType texType)
{
	GameButton *btn = new (std::nothrow) GameButton;
	if (btn && btn->init(normalImage,selectedImage,disableImage,texType))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}


void GameButton::pushDownEvent()
{
	if (_touchEventCallback) {
		_touchEventCallback(this, TouchEventType::BEGAN);
	}

	if (_touchEventListener && _touchEventSelector)
	{
		(_touchEventListener->*_touchEventSelector)(this,TOUCH_EVENT_BEGAN);
	}
}


void GameButton::moveEvent()
{
	if (_touchEventCallback) {
		_touchEventCallback(this, TouchEventType::MOVED);
	}

	if (_touchEventListener && _touchEventSelector)
	{
		(_touchEventListener->*_touchEventSelector)(this,TOUCH_EVENT_MOVED);
	}
}


void GameButton::releaseUpEvent()
{
	if (_touchEventCallback) 
	{
		GameUtils::playEffect("anniu.ogg");
		_touchEventCallback(this, TouchEventType::ENDED);
		return;
	} 
	
	if (_touchEventListener && _touchEventSelector)	//modify by sb
	{
		GameUtils::playEffect("anniu.ogg");
		(_touchEventListener->*_touchEventSelector)(this,TOUCH_EVENT_ENDED);
		return;
	}
}


void GameButton::cancelUpEvent()
{
	if (_touchEventCallback)
	{
		_touchEventCallback(this, TouchEventType::CANCELED);
	}

	if (_touchEventListener && _touchEventSelector)
	{
		(_touchEventListener->*_touchEventSelector)(this,TOUCH_EVENT_CANCELED);
	}
}