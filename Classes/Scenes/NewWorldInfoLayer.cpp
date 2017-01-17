#include "NewWorldInfoLayer.h"
#include "VisibleRect.h"
#include "Widgets/GameCSLoader.h"
#include "NewMapOpenMgr.h"
#include "GLCommon/Utils/ResMgr.h"
#include "GameUtils.h"
#include "UserData.h"
#include "GameMap.h"

bool NewWorldInfo::init()
{
	LayerColor::initWithColor(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_mainroot = CSLoader::createNode("csb/newworldinfo.csb");
	this->addChild(_mainroot);
	_mainroot->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	auto root = _mainroot->getChildByName("node_0");
	auto root2 = _mainroot->getChildByName("node_1");

	for (int i = 0; i < 10; i++)
	{
		auto btn = static_cast<Button*>(root->getChildByName(String::createWithFormat("btn_%d", i)->getCString()));
		if (!NewMapOpenMgr::getInstance()->isGet(MapOpenType(i)))
		{
			btn->loadTextures("newworld_icon_colorstone_1.png", "", "", Widget::TextureResType::PLIST);
		}
		btn->setUserData((void*)(i));
		btn->addTouchEventListener(CC_CALLBACK_2(NewWorldInfo::menuOnColorStone, this));
	}


	_infoText_0 = static_cast<Text*>(root->getChildByName("info_1"));
	_infoText_1 = static_cast<Text*>(root->getChildByName("info_2"));

	_infoText_0->setString(ResMgr::getInstance()->getString(String::createWithFormat("newmap_tip_%d_0", 0)->getCString())->getCString());
	_infoText_1->setString(ResMgr::getInstance()->getString(String::createWithFormat("newmap_tip_%d_1", 0)->getCString())->getCString());

	_selectIcon = Sprite::createWithSpriteFrameName("mainui_bag_select_select.png");
	root->addChild(_selectIcon);
	_selectIcon->setPosition(Vec2(-220.f, 38.f));

	auto btnexit = static_cast<Button*>(root->getChildByName("btn_exit"));
	btnexit->addTouchEventListener([=](Ref * ref, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	auto btngo = static_cast<Button*>(root->getChildByName("btn_ok"));
	btngo->addTouchEventListener([=](Ref * ref, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
#if (1 != CC_ENABLE_NEW_WORLD)
			for (int i = 0; i < 10; i++)
			{
				if (NewMapOpenMgr::getInstance()->isGet(MapOpenType(i)) == false)
				{
					GameUtils::toastTip("no_enough_color_stone");
					return;
				}
			}
#endif
			showInfo();
		}
	});

	auto btnexit2 = static_cast<Button*>(root2->getChildByName("btn_exit"));
	btnexit2->addTouchEventListener([=](Ref * ref, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			root2->setVisible(false);
			root->setVisible(true);
		}
	});

	auto btngo2 = static_cast<Button*>(root2->getChildByName("btn_ok"));
	btngo2->addTouchEventListener([=](Ref * ref, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			root2->setVisible(false);
			root->setVisible(true);
		}
	});


	if (UserData::getInstance()->getFirstOpenNewMap() == 0)
	{
		_mainroot->setVisible(false);
		auto desroot = CSLoader::createNode("csb/newWorldDes.csb");
		this->addChild(desroot);
		desroot->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

		auto btngo2 = static_cast<Button*>(desroot->getChildByName("btn_ok"));
		btngo2->addTouchEventListener([=](Ref * ref, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				desroot->removeFromParent();
				_mainroot->setVisible(true);
			}
		});

		UserData::getInstance()->setFirstOpenNewMap(1);
		UserData::getInstance()->saveUserData();
	}
	

	return true;
}

void NewWorldInfo::menuOnColorStone(Ref * ref, Widget::TouchEventType type)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int idx = long(((Node*)(ref))->getUserData());
#else
	int idx = int(((Node*)(ref))->getUserData());
#endif
	_infoText_0->setString(ResMgr::getInstance()->getString(String::createWithFormat("newmap_tip_%d_0", idx)->getCString())->getCString());
	_infoText_1->setString(ResMgr::getInstance()->getString(String::createWithFormat("newmap_tip_%d_1", idx)->getCString())->getCString());
	_selectIcon->setPosition(((Node*)(ref))->getPosition());
}

void NewWorldInfo::showInfo()
{
	auto root2 = _mainroot->getChildByName("node_1");
	root2->setVisible(true);

	auto root = _mainroot->getChildByName("node_0");
	root->setVisible(false);
#if (1 == CC_ENABLE_NEW_WORLD)
	if (UserData::getInstance()->getPlayerPos() >= 100)
	{
		WordMap::getInstance()->toOldMap();
	}
	else
	{
		WordMap::getInstance()->toNewMap();
	}
#endif
}
