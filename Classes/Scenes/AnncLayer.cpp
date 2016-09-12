#include "AnncLayer.h"
#include "../platBridge/cocos2dx_plat.h"
#include "VisibleRect.h"
#include "GameCSLoader.h"
#include "MainScene.h"


const string AnncLayer::ANNC_KEY_CONTENT = "annc_content";
const string AnncLayer::ANNC_KEY_SHOW = "annc_show";


AnncLayer::AnncLayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/storyui.plist");
}

AnncLayer::~AnncLayer()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/storyui.plist");
}

bool AnncLayer::init()
{
	LayerColor::initWithColor(Color4B(0, 0, 0, 0), 640.f, VisibleRect::top().y);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	this->setPositionY(-VisibleRect::top().y + 100.f);
	this->runAction(actionshow);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	float offsetY = (VisibleRect::top().y - 960.f) / 2.f;
	_pRoot = GameCSLoader::createNode("csb/announcement_layer.csb");
	this->addChild(_pRoot);
	_pRoot->setPositionY(offsetY);

	string strContent = cocos2dx_plat::getGameValue(ANNC_KEY_CONTENT);
	cocos2dx_plat::setGameValue(ANNC_KEY_SHOW, "0");

	auto contentTxt = dynamic_cast<ui::Text*>(_pRoot->getChildByName("main_bg")->getChildByName("sub_bg")->getChildByName("scroll")->getChildByName("content"));
	if ("" != strContent)
	{
		contentTxt->setString(strContent);
	}

	auto btnClose = dynamic_cast<ui::Button *>(_pRoot->getChildByName("bt_know"));
	btnClose->addClickEventListener([=](Ref *) {
		MainLayer::getCurMainLayer()->enterMainLayer();
		this->removeFromParent();
	});

	return true;
}
