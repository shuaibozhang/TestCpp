#ifndef __GAME_BUTTON_H__
#define __GAME_BUTTON_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;


class GameButton : public Button
{
	DECLARE_CLASS_GUI_INFO

public:
	GameButton();
	virtual ~GameButton();    
	
	/**
     * Allocates and initializes.
     */
    static GameButton* create();
    

    /**
     * create a button with custom textures
     * @normalImage normal state texture name
     * @selectedImage  selected state texture name
     * @disableImage disabled state texture name
     * @param texType    @see UI_TEX_TYPE_LOCAL
     */
    static GameButton* create(const std::string& normalImage,
                          const std::string& selectedImage = "",
                          const std::string& disableImage = "",
						  TextureResType texType = TextureResType::LOCAL);

protected:
	virtual void pushDownEvent();
	virtual void moveEvent();

	virtual void releaseUpEvent();
	virtual void cancelUpEvent();
};


#endif