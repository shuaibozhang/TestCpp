#include "GameMenuItemSprite.h"
//#include "../GameUtils.h"

#if 0
GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite, (const ccMenuCallback&)nullptr);
}


// XXX deprecated
GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Ref* target, SEL_MenuHandler selector)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, nullptr, target, selector);
}


GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, nullptr, callback);
}


// XXX deprecated
GameMenuItemSprite * GameMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, Ref *target, SEL_MenuHandler selector)
{
	GameMenuItemSprite *ret = new GameMenuItemSprite();
	ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector); 
	ret->autorelease();
	return ret;
}


GameMenuItemSprite * GameMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
{
	GameMenuItemSprite *ret = new GameMenuItemSprite();
	ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
	ret->autorelease();
	return ret;
}
#endif

GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite, (const ccMenuCallback&)nullptr);
}

// FIXME: deprecated
GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Ref* target, SEL_MenuHandler selector)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, nullptr, std::bind(selector, target, std::placeholders::_1));
}

GameMenuItemSprite * GameMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback)
{
	return GameMenuItemSprite::create(normalSprite, selectedSprite, nullptr, callback);
}

// FIXME: deprecated
GameMenuItemSprite * GameMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, Ref *target, SEL_MenuHandler selector)
{
	GameMenuItemSprite *ret = new (std::nothrow) GameMenuItemSprite();
	ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, std::bind(selector, target, std::placeholders::_1));
	ret->autorelease();
	return ret;
}

GameMenuItemSprite * GameMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
{
	GameMenuItemSprite *ret = new (std::nothrow) GameMenuItemSprite();
	ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
	ret->autorelease();
	return ret;
}


void GameMenuItemSprite::activate()
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


void GameMenuItemSprite::selected()
{
	MenuItemSprite::selected();
}


void GameMenuItemSprite::unselected()
{
	MenuItemSprite::unselected();
}

