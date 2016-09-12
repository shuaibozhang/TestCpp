#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GLCommon/UI/TouchesScaleNode.h"

class RTTNode;
class ShaderSprite;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	Node* rttnode;
	ShaderSprite* sprite;
	ShaderSprite* sprite2;
};

class TestLayer : public cocos2d::Layer
{public:
	virtual bool init();
	CREATE_FUNC(TestLayer);
private:
	cocos2d::Vector<cocos2d::ui::Slider*> _vectorSlider;
	cocos2d::Vector<cocos2d::Sprite*> _vectorImage;
	cocos2d::Vector<cocos2d::ui::Text*> _vectorText;
};

#endif // __HELLOWORLD_SCENE_H__
