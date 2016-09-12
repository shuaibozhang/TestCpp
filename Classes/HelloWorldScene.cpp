#include "HelloWorldScene.h"
#include "effects/GradualAlpha.h"
#include "effects/ensShatterNode.h"
#include "effects/EffectAction.h"
#include "effects/ShaderSprite.h"
#include "effects/RTTNode.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "GLCommon/UI/GLMenuItemSprite.h"
#include "GLCommon/UI/ScrollBg.h"
#include "GLCommon/UI/TouchesScaleNode.h"

USING_NS_CC;

using namespace ui;
using namespace glui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto item = Sprite::create("CloseNormal.png");
	//this->addChild(item, 1, "test");

	auto item2 = Sprite::create("HelloWorld.png");
	this->addChild(item2, 1, "test");

	auto closeItem = glui::GLMenuItemSprite::create(
		item,
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	//closeItem->setScale(1.5f);
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);




    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	this->removeChildByName("test");
	SpriteFrameCache::getInstance()->removeSpriteFrameByName("HelloWorld.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("HelloWorld.png");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


bool TestLayer::init()
{
	Layer::init();
	auto root = CSLoader::createNode("Layer.csb");
	this->addChild(root);

	for (int i = 0; i < 5; i++)
	{
		auto name = String::createWithFormat("test_%d", i + 1);
		auto temp = dynamic_cast<Sprite*>(root->getChildByName(name->getCString()));
		_vectorImage.pushBack(temp);

		auto glprogram = GLProgram::createWithFilenames("shader/test.vsh", "shader/test.fsh");
		auto glprogramstate = GLProgramState::create(glprogram);
		temp->setGLProgramState(glprogramstate);

		glprogramstate->setUniformVec3("colorglow", Vec3(0.f, 0.f, 0.f));
		glprogramstate->setUniformVec3("colorout", Vec3(1.f, 1.f, 1.f));
		glprogramstate->setUniformFloat("time", 1.f);
		glprogramstate->setUniformFloat("time2", 1.f);
	}

	for (int i = 0; i < 8; i++)
	{
		auto name = String::createWithFormat("Slider_%d", i + 1);
		auto temp = dynamic_cast<Slider*>(root->getChildByName(name->getCString()));
		_vectorSlider.pushBack(temp);

		auto name2 = String::createWithFormat("text_%d", i + 1);
		auto text = dynamic_cast<Text*>(root->getChildByName(name2->getCString()));
		_vectorText.pushBack(text);

		if (i < 3)
		{
			temp->setPercent(0);
		}
		else if (i == 3)
		{
			temp->setPercent(100);
		}
		else
		{
			temp->setPercent(100);
		}
		temp->addEventListener([=](Ref* target, Slider::EventType type) {
			if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				for (auto& tempimage : _vectorImage)
				{
					auto gl = tempimage->getGLProgramState();
					
					Vec3 glow;
					glow.x = _vectorSlider.at(0)->getPercent() / 100.f;
					glow.y = _vectorSlider.at(1)->getPercent() / 100.f;
					glow.z = _vectorSlider.at(2)->getPercent() / 100.f;
					Vec3 out;
					out.x = _vectorSlider.at(4)->getPercent() / 100.f;
					out.y = _vectorSlider.at(5)->getPercent() / 100.f;
					out.z = _vectorSlider.at(6)->getPercent() / 100.f;

					float time = _vectorSlider.at(3)->getPercent() / 100.f;
					float time1 = _vectorSlider.at(7)->getPercent() / 100.f;

					_vectorText.at(0)->setText(String::createWithFormat("%.3f", glow.x)->getCString());
					_vectorText.at(1)->setText(String::createWithFormat("%.3f", glow.y)->getCString());
					_vectorText.at(2)->setText(String::createWithFormat("%.3f", glow.z)->getCString());
					_vectorText.at(3)->setText(String::createWithFormat("%.3f", time)->getCString());

					_vectorText.at(4)->setText(String::createWithFormat("%.3f", out.x)->getCString());
					_vectorText.at(5)->setText(String::createWithFormat("%.3f", out.y)->getCString());
					_vectorText.at(6)->setText(String::createWithFormat("%.3f", out.z)->getCString());
					_vectorText.at(7)->setText(String::createWithFormat("%.3f", time1)->getCString());
					
					gl->setUniformVec3("colorglow", glow);
					gl->setUniformVec3("colorout", out);
					gl->setUniformFloat("time", time);
					gl->setUniformFloat("time2", time1);
				}
			}
		});
	}

	return true;
}
