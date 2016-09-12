#ifndef __GAME_MENU_ITEM_SPRITE_H__
#define __GAME_MENU_ITEM_SPRITE_H__

#include "cocos2d.h"

USING_NS_CC;

//CC_DLL
class GameMenuItemSprite : public MenuItemSprite
{
public:
	/** creates a menu item with a normal, selected and disabled image*/
	static GameMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite = nullptr);
	/** creates a menu item with a normal and selected image with target/selector */
	//CC_DEPRECATED_ATTRIBUTE 
	static GameMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Ref* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	//CC_DEPRECATED_ATTRIBUTE 
	static GameMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, Ref* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal and selected image with a callable object */
	static GameMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static GameMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);

	virtual void activate();
	virtual void selected();
	virtual void unselected();
};

#endif