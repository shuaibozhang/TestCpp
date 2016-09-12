#include "GameMenuItem.h"
//#include "../GameUtils.h"

/*
GameMenuItem::GameMenuItem()
{

}

GameMenuItem::~GameMenuItem()
{

}*/
#if 0

GameMenuItem* GameMenuItem::create()
{
	return GameMenuItem::create((const ccMenuCallback&)nullptr);
}

// XXX deprecated
GameMenuItem* GameMenuItem::create(Ref *target, SEL_MenuHandler selector)
{
	GameMenuItem *ret = new GameMenuItem();
	ret->initWithTarget(target, selector);
	ret->autorelease();
	return ret;
}

GameMenuItem* GameMenuItem::create( const ccMenuCallback& callback)
{
	GameMenuItem *ret = new GameMenuItem();
	ret->initWithCallback(callback);
	ret->autorelease();
	return ret;
}
#endif


GameMenuItem* GameMenuItem::create()
{
	return GameMenuItem::create((const ccMenuCallback&)nullptr);
}

// FIXME: deprecated
GameMenuItem* GameMenuItem::create(Ref *target, SEL_MenuHandler selector)
{
	GameMenuItem *ret = new (std::nothrow) GameMenuItem();
	ret->initWithCallback(std::bind(selector, target, std::placeholders::_1));
	ret->autorelease();
	return ret;
}

GameMenuItem* GameMenuItem::create( const ccMenuCallback& callback)
{
	GameMenuItem *ret = new (std::nothrow) GameMenuItem();
	ret->initWithCallback(callback);
	ret->autorelease();
	return ret;
}


void GameMenuItem::selected()
{
	MenuItem::selected();
}


void GameMenuItem::unselected()
{
	MenuItem::unselected();
}


void GameMenuItem::activate()
{
	if (_enabled)
	{
		//GameUtils::playEffect("anniu.ogg");
		if( _callback)
		{
			_callback(this);
		}
#if CC_ENABLE_SCRIPT_BINDING
		if (kScriptTypeNone != _scriptType)
		{
			BasicScriptData data(this);
			ScriptEvent scriptEvent(kMenuClickedEvent,&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
		}
#endif
	}
}