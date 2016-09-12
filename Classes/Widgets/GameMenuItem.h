#ifndef __GAME_MENU_ITEM_H__
#define __GAME_MENU_ITEM_H__

#include "cocos2d.h"

USING_NS_CC;

//CC_DLL
class GameMenuItem : public MenuItem
{
public:
	/** Creates a MenuItem with no target/selector */
	static GameMenuItem* create();
	/** Creates a MenuItem with a target/selector */
	//CC_DEPRECATED_ATTRIBUTE 
	static GameMenuItem* create(Ref *rec, SEL_MenuHandler selector);
	/** Creates a MenuItem with a target/selector */
	static GameMenuItem* create(const ccMenuCallback& callback);

	/** Activate the item */
	virtual void activate() override;
	/** The item was selected (not activated), similar to "mouse-over" */
	virtual void selected() override;
	/** The item was unselected */
	virtual void unselected() override;

	/*
CC_CONSTRUCTOR_ACCESS:
	GameMenuItem();
	virtual ~GameMenuItem();*/
};

#endif