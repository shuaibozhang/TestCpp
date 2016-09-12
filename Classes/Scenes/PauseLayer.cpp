#include "PauseLayer.h"
#include "../VisibleRect.h"
#include "GameMap.h"
#include "GameConfig.h"
#include "GameLayer.h"
#include "MainScene.h"
#include "StartScene.h"
#include "GameUtils.h"
#include "platBridge/cocos2dx_plat.h"

bool PauseLayer::init()
{
	Layer::init();

	Color4B color(0, 0, 0, 200);
	auto layerclolor = LayerColor::create(color);
	layerclolor->setContentSize(Size(640.f, VisibleRect::top().y));
	this->addChild(layerclolor);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *touch, Event *event) {return true; };
	listener->setSwallowTouches(true);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, layerclolor);

	auto layer = GameCSLoader::createNode("csb/pauselayer.csb");
	this->addChild(layer);
	layer->setPositionY((VisibleRect::top().y - 960.f) * 0.5f);

	GameLayer::getInstance()->pauseGame();

	_btn0 = ((Button*)(layer->getChildByName("btn_0")));
	_btn1 = ((Button*)(layer->getChildByName("btn_1")));
	_btn2 = ((Button*)(layer->getChildByName("btn_2")));
	_btn3 = ((Button*)(layer->getChildByName("btn_3")));

	_btn0->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{	
			if (GameMap::getCurGameMap()->isInFight())
			{
				GameMap::getCurGameMap()->quitFight();
				GameLayer::getInstance()->resumeGame();
				//debug zsb
				MainLayer::getCurMainLayer()->resumGame();
			}
			else
			{
				MainLayer::getCurMainLayer()->resumGame();
				Director::getInstance()->pushScene(StartScene::scene());
			}
			this->removeFromParent();
		}
	});

	_btn3->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			GameLayer::getInstance()->resumeGame();
			MainLayer::getCurMainLayer()->resumGame();
			this->removeFromParent();
		}
	});


	_btn1->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto music = GameConfig::isMusicOn();
			if (music)
			{
				GameConfig::switchMusicOff();
				GameUtils::stopBackgroundMusic();
			}
			else
			{
				GameConfig::switchMusicOn();
				GameUtils::playBackgroundMusic(nullptr, true);
			}

			updateUi();
		}
	});

	_btn2->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto music = GameConfig::isEffectOn();
			if (music)
			{
				GameConfig::switchEffectOff();
				GameUtils::stopAllEffect();
			}
			else
			{
				GameConfig::switchEffectOn();
			}

			updateUi();
		}
	});

	updateUi();
	cocos2dx_plat::gamePause();

	//debug zsb
	//if (!GameMap::getCurGameMap()->isInFight())
	//{
		MainLayer::getCurMainLayer()->pauseGame();
	//}
	//else
	//{

	//}
	return true;
}

void PauseLayer::updateUi()
{
	if (GameConfig::isMusicOn())
	{
		_btn1->loadTextures("pause_music_0.png", "pause_music_0.png", "", Widget::TextureResType::PLIST);
	}
	else
	{
		_btn1->loadTextures("pause_music_1.png", "pause_music_1.png", "", Widget::TextureResType::PLIST);
	}

	if (GameConfig::isEffectOn())
	{
		_btn2->loadTextures("pause_sound_0.png", "pause_sound_0.png", "", Widget::TextureResType::PLIST);
	}
	else
	{
		_btn2->loadTextures("pause_sound_1.png", "pause_sound_1.png", "", Widget::TextureResType::PLIST);
	}
}
