#include "MainScene.h"
#include "effects/EffectAction.h"
#include "effects/ShaderSprite.h"
#include "GameMap.h"
#include "VisibleRect.h"
#include "GLCommon/UI/GLSprite.h"
#include "DataParam/ParamData.h"
#include "DataParam/BagItemControl.h"
#include "DataParam/SkillControl.h"
#include "DataParam/WeaponControl.h"
#include "DataParam/UserData.h"
#include "../Story/Commond.h"
#include "PauseLayer.h"
#include "ParamMgr.h"
#include "../Armtr/GameArmtr.h"
#include "JRCommon.h"
#include "GameUtils.h"
#include "GameLayer.h"
#include "Player.h"
#include "CrushUtil.h"
#include "TimeCountDownNode.h"
#include "ShopLayer.h"
#include "BoxLayer.h"
#include "ui/CocosGUI.h"
#include "platBridge/cocos2dx_plat.h"
#include "PopRewardLayer.h"
#include "AchieveMgr.h"
#include "AchieveLayer.h"
#include "BoxDataMgr.h"
#include "EndlessMainLayer.h"
#include "GameGuideLayer.h"
#include "StoreBridge/StoreAssets.h"
#include "Store/PurchaseLayer.h"
#include "BGLLayer.h"
#include "DailyRewardLayer.h"
#include "StartScene.h"
#include "AnncLayer.h"
#include "MagPieMgr.h"
#include "MagPieLayer.h"
#include "DayActiveMgr.h"
#include "DayActivityLayer.h"
#include "NewMapOpenMgr.h"
#include "TimeLimitActivityMgr.h"
#include "NewWorldInfoLayer.h"
#include "DungeonLayer.h"
#include "Player.h"
#include "Role.h"

USING_NS_CC;
using namespace glui;
using namespace ui;

cocos2d::Scene * MainLayer::scene()
{
	auto scene = Scene::create();
	auto layer = MainLayer::create();
	scene->addChild(layer);
	return scene;
}

MainLayer * MainLayer::create()
{
	auto pRef = new MainLayer();
	if (pRef && pRef->init())
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
}

MainLayer* MainLayer::s_curMainLayer = nullptr;

MainLayer::MainLayer():_mainMenu(nullptr),
	_secondMenu(nullptr),
	_isShowMainMenu(true),
	_playersLayer(nullptr),
	_playerbg(nullptr),
	_curMenuIdx(-1),
	_equipPageView(nullptr),
	_skillPageView(nullptr),
	_pageViewFinalPos(0.f, VisibleRect::top().y / 2 - 350.f),
	_pageViewFinalPos2(640.f, VisibleRect::top().y / 2 - 350.f),
	_pageSize(640.f, 550.f),
	_pageViewFinalPos3(-640.f, VisibleRect::top().y / 2 - 350.f),
	_curShowPageView(nullptr),
	_touchListenerMarkNode(nullptr),
	_touchOneByOneListener(nullptr),
	_itemsLayer(nullptr),
	_bagLayer(nullptr),
	_isMenuInProtect(false),
	_topGoldNode(nullptr),
	_goldNumAtlas(nullptr),
	_topHpInfoNode(nullptr),
	_markNode(nullptr),
	_menuUp(true),
	_boxtimenode(nullptr),
	_settleRankState(0),
	_giftBtn(nullptr),
	_isNeedDaily(false),
	_isNeedWeekReward(false),
	_showNewGetCallBack(nullptr),
	_7xiActivityBtn(nullptr),
	_timeLimitActivityBtn(nullptr),
	_timeLimitActivityText(nullptr),
	_dayAcitvityBtn(nullptr),
	_pTiliMaxIcon(nullptr),
	_commondActivityBtn(nullptr)
{
	memset(_arrMenuItems, 0, sizeof(_arrMenuItems));
	memset(_lightArr, 0, sizeof(_lightArr));
	s_curMainLayer = this;
}

MainLayer::~MainLayer()
{
	s_curMainLayer = nullptr;

	_eventDispatcher->removeEventListenersForTarget(this);
	_eventDispatcher->removeCustomEventListeners("guide_end");
}

bool MainLayer::init()
{
	Layer::init();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/mainui.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/comui.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/skillicon.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/comitems.plist");

	initMapLayer();

	/*_playerbg = ui::ImageView::create("bgs/main_dark_bg.jpg");
	this->addChild(_playerbg, BG_Z);
	_playerbg->setScale9Enabled(true);
	_playerbg->setContentSize(Size(VisibleRect::rightTop()));
	_playerbg->setPosition(VisibleRect::center());
	_playerbg->setCapInsets(Rect(0.f, 0.f, 640.f, 10.f));
	_playerbg->setOpacity(0);*/

	_playerbg = ImageView::create("bgs/main_darkness_bg.png");
	this->addChild(_playerbg, BG_Z);
	_playerbg->setScale9Enabled(true);
	_playerbg->setContentSize(Size(VisibleRect::rightTop()));
	_playerbg->setPosition(VisibleRect::center());
	_playerbg->setOpacity(0);

	_logo = Sprite::create("bgs/main_dark_logo.png");
	this->addChild(_logo, BG_Z);
	_logo->setPosition(Vec2(320.f, VisibleRect::top().y / 2));
	_logo->setOpacity(0);

	_itemsLayer = Layer::create();
	this->addChild(_itemsLayer, PLAYERINFOLAYER_Z);
	_itemsLayer->setVisible(false);

	auto offy = VisibleRect::top().y - 960.f;
	_bagLayer = BagLayer::createLayer();
	this->addChild(_bagLayer, BAGLAYER_Z);
	_bagLayer->setPositionY(offy * 0.5f);

	_bagLayer->setVisible(false);
	_bagLayer->setCanTouch(false);

	_topGoldNode = Node::create();
	this->addChild(_topGoldNode, MENU_Z);

	_numInfoNode = Node::create();
	_topGoldNode->addChild(_numInfoNode,2);

	auto numbg = ImageView::create("com_goldnumbg.png", Widget::TextureResType::PLIST);
	_topGoldNode->addChild(numbg);
	numbg->setAnchorPoint(Vec2(0.f, 0.5f));
	numbg->setPosition(Vec2(40.f, VisibleRect::top().y - 40.f));
	numbg->setContentSize(Size(220.f, 42.f));
	numbg->setScale9Enabled(true);

	auto spritebag = Sprite::createWithSpriteFrameName("com_goldbag.png");
	_topGoldNode->addChild(spritebag);
	auto pos = Vec2(40.f, VisibleRect::top().y - 40.f);
	spritebag->setPosition(pos);

	_goldNumAtlas = ui::TextAtlas::create("110", "fonts/font_goldnum.png", 24.f, 30.f, ".");
	_topGoldNode->addChild(_goldNumAtlas);
	_goldNumAtlas->setPosition(numbg->getPosition() + Vec2(90.f, 0.f));

	auto addbtngold = GameButton::create("com_addbtn_0.png", "com_addbtn_1.png", "", Widget::TextureResType::PLIST);
	_numInfoNode->addChild(addbtngold);
	addbtngold->setPosition(numbg->getPosition() + Vec2(205.f, -2.f));
	addbtngold->addTouchEventListener(CC_CALLBACK_2(MainLayer::touchAddBtn, this));
	addbtngold->setName("gold");

	auto numbg2 = ImageView::create("com_goldnumbg.png", Widget::TextureResType::PLIST);
	_numInfoNode->addChild(numbg2);
	numbg2->setAnchorPoint(Vec2(0.f, 0.5f));
	numbg2->setPosition(Vec2(295.f, VisibleRect::top().y - 40.f));
	numbg2->setContentSize(Size(125.f, 42.f));
	numbg2->setScale9Enabled(true);

	auto spritebag2 = Sprite::createWithSpriteFrameName("com_crystal.png");
	_numInfoNode->addChild(spritebag2);
	auto pos2 = Vec2(290.f, VisibleRect::top().y - 40.f);
	spritebag2->setPosition(pos2);

	_crystalNumAtlas = ui::TextAtlas::create("110", "fonts/font_goldnum.png", 24.f, 30.f, ".");
	_numInfoNode->addChild(_crystalNumAtlas);
	//_crystalNumAtlas->setAnchorPoint(Vec2(0.f, 0.5f));
	_crystalNumAtlas->setPosition(numbg2->getPosition() + Vec2(50.f, 0.f));

	auto addbtncry = GameButton::create("com_addbtn_0.png", "com_addbtn_1.png", "", Widget::TextureResType::PLIST);
	_numInfoNode->addChild(addbtncry);
	addbtncry->setPosition(numbg2->getPosition() + Vec2(110.f, -2.f));
	addbtncry->addTouchEventListener(CC_CALLBACK_2(MainLayer::touchAddBtn, this));
	addbtncry->setName("cry");
	//tili
	auto numbg3 = ImageView::create("com_goldnumbg.png", Widget::TextureResType::PLIST);
	_numInfoNode->addChild(numbg3);
	numbg3->setAnchorPoint(Vec2(0.f, 0.5f));
	numbg3->setPosition(Vec2(440.f, VisibleRect::top().y - 40.f));
	numbg3->setContentSize(Size(120.f, 42.f));
	numbg3->setScale9Enabled(true);

	auto spritebag3 = Sprite::createWithSpriteFrameName("com_icon_jingqi.png");
	_numInfoNode->addChild(spritebag3);
	auto pos3 = Vec2(445.f, VisibleRect::top().y - 40.f);
	spritebag3->setPosition(pos3);
	spritebag3->setScale(0.7f);

	_pTiliMaxIcon = Sprite::createWithSpriteFrameName("com_tilimax.png");
	_numInfoNode->addChild(_pTiliMaxIcon);
	auto posTiliMax = Vec2(445.f, VisibleRect::top().y - 50.f);
	_pTiliMaxIcon->setPosition(posTiliMax);
	_pTiliMaxIcon->setVisible(false);

	_tiliNumAtlas = ui::TextAtlas::create("110", "fonts/font_goldnum.png", 24.f, 30.f, ".");
	_numInfoNode->addChild(_tiliNumAtlas);
	_tiliNumAtlas->setPosition(numbg3->getPosition() + Vec2(60.f, 0.f));

	auto addbtntili = GameButton::create("com_addbtn_0.png", "com_addbtn_1.png", "", Widget::TextureResType::PLIST);
	_numInfoNode->addChild(addbtntili);
	addbtntili->setName("tili");
	addbtntili->setPosition(numbg3->getPosition() + Vec2(100.f, -2.f));
	addbtntili->addTouchEventListener(CC_CALLBACK_2(MainLayer::touchAddBtn, this));

	_topHpInfoNode = GameCSLoader::createNode("csb/topinfonode.csb");
	_topHpInfoNode->setPosition(Vec2(320.f, pos2.y - 70.f));
	this->addChild(_topHpInfoNode, MENU_Z);
	_topHpInfoNode->setVisible(false);

	_topHpAtlas = static_cast<ui::TextAtlas*>(_topHpInfoNode->getChildByName("AtlasLabel_1"));
	_topDpAtlas = static_cast<ui::TextAtlas*>(_topHpInfoNode->getChildByName("AtlasLabel_1_0"));

	_markNode = GameCSLoader::createNode("csb/pagemarknode.csb");
	this->addChild(_markNode, MENU_Z);
	_markNode->setPosition(Vec2(320.f, 120.f));
	_markNode->setVisible(false);

	initSkillAndEpuipLayer();
	initPlayersLayer();
	initMenus();
	initTouchLsitener();
	initTimeCounrdown();

	_btnPause = ui::Button::create("com_pause_0.png", "", "", ui::Widget::TextureResType::PLIST);
	this->addChild(_btnPause, MainLayer_Z::POP_Z);
	_btnPause->setPosition(Vec2(600.f, VisibleRect::top().y - 40.f));
	_btnPause->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto pauselayer = PauseLayer::create();
			this->addChild(pauselayer, POP_Z);
		}
	});

	updataTopInfo();
	updataGold();

	NetDataMgr::getInstance()->updateRole(CC_CALLBACK_2(MainLayer::onUpdateSelfInfoCallback, this));

	return true;
}

void MainLayer::onEnter()
{
	Layer::onEnter();
	this->enterMainLayer();

	_leftBtnsNode->setPositionX(-200.f);
	auto actionMoveInL = EaseSineOut::create(MoveTo::create(0.5f, Vec2(0.f, 0.f)));
	_leftBtnsNode->runAction(actionMoveInL);

	auto tempbgposy = _menubg->getPosition();
	auto tempbgposy2 = _mainMenu->getPosition();

	_menubg->setPositionY(tempbgposy.y - 100.f);
	_mainMenu->setPositionY(tempbgposy2.y - 100.f);

	auto action = EaseSineOut::create(MoveTo::create(0.5f, tempbgposy));
	auto action2 = EaseSineOut::create(MoveTo::create(0.5f, tempbgposy2));
	_menubg->runAction(action);
	_mainMenu->runAction(action2);

	updataGiftBtns();
	updataDayActivityBtn();
}

void MainLayer::initMenus()
{
	_menubg = Sprite::createWithSpriteFrameName("mainui_menubg.png");
	this->addChild(_menubg, MENU_Z);
	_menubg->setPosition(Vec2(320.f, _menubg->getContentSize().height / 2.f  - 30.f));

	_selectLight = Sprite::createWithSpriteFrameName("mainui_item_bg.png");
	this->addChild(_selectLight, MENU_Z);
	_selectLight->setPosition(Vec2(110.f, _menubg->getPositionY() + 30.f));
	_selectLight->setVisible(false);

	_mainMenu = Menu::create();
	this->addChild(_mainMenu, MENU_Z);
	_mainMenu->setPosition(Vec2(0.f, 0.f));


	_secondMenu = Menu::create();
	this->addChild(_secondMenu, MENU_Z);
	_secondMenu->setPosition(Vec2(0.f, 0.f));
	_secondMenu->setVisible(false);

	for (int i = 0; i < MenuItem_E::ItemNums; i++)
	{
		if (i < 1)
		{
			//auto itemsprite = Sprite::createWithSpriteFrameName(String::createWithFormat("mainui_item_%d.png", i)->getCString());
			auto itemsprite = Sprite::createWithSpriteFrameName("mainui_item_sp.png");
			_arrMenuItems[i] = GLMenuItemSprite::create(itemsprite, CC_CALLBACK_1(MainLayer::menuCallback, this));
			_arrMenuItems[i]->setUserData((void*)(i));

			_arrMenuItems[i]->setPosition(Vec2(320.f, _menubg->getPositionY() + 15.f));
			_mainMenu->addChild(_arrMenuItems[i]);
		}
		else
		{
			//auto itembg = Sprite::createWithSpriteFrameName("mainui_item_bg.png");

			auto itemsprite = Sprite::createWithSpriteFrameName(String::createWithFormat("mainui_item_%d.png", i)->getCString());
			_arrMenuItems[i] = GLMenuItemSprite::create(itemsprite, CC_CALLBACK_1(MainLayer::menuCallback, this));
			_arrMenuItems[i]->setUserData((void*)(i));
			//_arrMenuItems[i]->setBgSprite(itembg);
			//itembg->setPosition(Vec2(_arrMenuItems[i]->getContentSize().width / 2, _arrMenuItems[i]->getContentSize().height / 2) + Vec2(0.f, -20.f));

			//auto text = Sprite::createWithSpriteFrameName(String::createWithFormat("mainui_itemtext_%d.png", i)->getCString());
			//text->setPosition(Vec2(_arrMenuItems[i]->getContentSize().width / 2, _arrMenuItems[i]->getContentSize().height / 2) + Vec2(0.f, -40.f));
			//_arrMenuItems[i]->addChild(text);
			//text->setLocalZOrder(2);
			//_arrSpriteMenuText[i] = text;

			_arrMenuItems[i]->setPosition(Vec2(110.f + 105 * i - 105, _menubg->getPositionY() + 20.f));
			_secondMenu->addChild(_arrMenuItems[i]);
		}
	}


}

void MainLayer::initPlayersLayer()
{
	_playersLayer = PlayersLayer::createPlayerLayer();
	_itemsLayer->addChild(_playersLayer, 0);
}

void MainLayer::initSkillAndEpuipLayer()
{
	_equipPageView = glui::GLPageView::create();
	_equipPageView->setPosition(_pageViewFinalPos);
	_itemsLayer->addChild(_equipPageView, 1);
	_equipPageView->setContentSize(_pageSize);
	_equipPageView->setVisible(false);
	_equipPageView->setCanScroll(false);
	_equipPageView->setClippingEnabled(true);
	_equipPageView->setCustomScrollThreshold(150.f);

	_equipPageView->addEventListener([=](cocos2d::Ref* target, ui::PageView::EventType type) {
		if (type == ui::PageView::EventType::TURNING)
		{
			for (int i = 0; i < 4; i++)
			{
				int idx = static_cast<glui::GLPageView*>(target)->getCurPageIndex();
				auto sprite = static_cast<Sprite*>(_markNode->getChildByName(String::createWithFormat("mark_%d", i)->getCString()));
				if (idx == i)
				{
					sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_1.png"));
				}
				else
				{
					sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_0.png"));
				}
			}
		}
	});

	_skillPageView = glui::GLPageView::create();
	_skillPageView->setPosition(_pageViewFinalPos2);
	_itemsLayer->addChild(_skillPageView, 2);
	_skillPageView->setContentSize(_pageSize);
	_skillPageView->setVisible(false);
	_skillPageView->setCanScroll(false);
	_skillPageView->setClippingEnabled(true);
	_skillPageView->setCustomScrollThreshold(150.f);

	_skillPageView->addEventListener([=](cocos2d::Ref* target, ui::PageView::EventType type) {
		if (type == ui::PageView::EventType::TURNING)
		{
			for (int i = 0; i < 4; i++)
			{
				int idx = static_cast<glui::GLPageView*>(target)->getCurPageIndex();
				auto sprite = static_cast<Sprite*>(_markNode->getChildByName(String::createWithFormat("mark_%d", i)->getCString()));
				if (idx == i)
				{
					sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_1.png"));
				}
				else
				{
					sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_0.png"));
				}
			}
		}
	});

	for (int i = 0; i < 4; i++)
	{
		auto layout = ui::Layout::create();
		auto layer = EquipLayer::createLayer(i);
		layout->addChild(layer);
		_equipPageView->addPage(layout);
		_arrEquipLayer[i] = layer;

		auto layout2 = ui::Layout::create();
		auto layer2 = SkillLayer::createLayer(i);
		layout2->addChild(layer2);
		_skillPageView->addPage(layout2);
		_arrSkillLayer[i] = layer2;
	}
}

void MainLayer::initTouchLsitener()
{
	_touchListenerMarkNode = Node::create();
	this->addChild(_touchListenerMarkNode, MainLayer_Z::TOUCH_Z);

	_touchOneByOneListener = EventListenerTouchOneByOne::create();
	_touchOneByOneListener->setSwallowTouches(false);

	_touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegan, this);
	_touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMoved, this);
	_touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnded, this);
	_touchOneByOneListener->onTouchCancelled = CC_CALLBACK_2(MainLayer::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchOneByOneListener, _touchListenerMarkNode);
	
}

void MainLayer::initMapLayer()
{
	_worldLayer = WordMap::createWordMap();
	this->addChild(_worldLayer, WORLDMAP_Z);

	_mapLayer = GameMap::createWithIdx(_worldLayer->getSceneId(UserData::getInstance()->getPlayerPos()));
	this->addChild(_mapLayer, GUANQIAMAP_Z);
}

void MainLayer::menuCallback(cocos2d::Ref * pSender)
{
	auto menuitem = static_cast<GLMenuItemSprite*>(pSender);
	auto idx = int(menuitem->getUserData());
	float actiondur = 0.4f;

	if (idx == _curMenuIdx)
	{
		return;
	}

	if (_isMenuInProtect)
	{
		return;
	}
	else
	{
		startMenuProtect(0.6f);
	}

	for (int i = 0; i < 4; i++)
	{
		MainLayer::getCurMainLayer()->getCurPlayersLayer()->updataPlayerInfo(i);
	}

	if (idx == MenuItem_E::MainItem)
	{
		_bagLayer->setCanTouch(false);
		_btnPause->setVisible(false);
		_worldLayer->setCanTouchMove(false);
		_markNode->setVisible(false);
		_mainMenu->setEnabled(false);
		_secondMenu->setEnabled(false);
		setStartState();
		meunEffectMainOut(actiondur);
		_equipPageView->setVisible(false);
		_skillPageView->setVisible(false);

		_topHpInfoNode->setVisible(true);
		_topHpInfoNode->setPositionX(960.f);
		auto actionMove = MoveBy::create(actiondur, Vec2(-640.f, 0.f));
		_topHpInfoNode->runAction(actionMove);

		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepEnd();
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {GameGuideLayer::getInstance()->guideStepStart(1, _arrMenuItems[2]->convertToWorldSpace(Vec2(48.f, 38.f))); })));
		}

		showNewGetItem();
		
	}
	else if (idx == MenuItem_E::MapItem)
	{
		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepEnd();
		}
		_bagLayer->setCanTouch(false);
		_markNode->setVisible(false);
		_worldLayer->setCanTouchMove(true);
		_mainMenu->setEnabled(false);
		_secondMenu->setEnabled(false);
		_btnPause->setVisible(true);
		menuEffectSecondOut();

		_itemsLayer->runAction(Sequence::createWithTwoActions(MoveBy::create(actiondur, Vec2(-640.f, 0.f)), Hide::create()));

		if (_curMenuIdx == MenuItem_E::BagItem)
		{
			_bagLayer->runAction(Sequence::createWithTwoActions(MoveBy::create(actiondur, Vec2(-640.f, 0.f)), Hide::create()));

			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = Sequence::createWithTwoActions(MoveBy::create(actiondur, Vec2(-640.f, 0.f)), Hide::create());
			_topHpInfoNode->runAction(actionMove);
		}
		else if (_curMenuIdx == MenuItem_E::CharacterItem)
		{
			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = Sequence::createWithTwoActions(MoveBy::create(actiondur, Vec2(-640.f, 0.f)), Hide::create());
			_topHpInfoNode->runAction(actionMove);
		}
		
	}
	else if (idx == MenuItem_E::CharacterItem)
	{
		_bagLayer->setCanTouch(false);
		_markNode->setVisible(false);
		
		_worldLayer->setCanTouchMove(false);

		if (_curMenuIdx == MenuItem_E::WeaponsItem)
		{
			_playersLayer->showChangeBackTo(actiondur, _equipPageView->getCurPageIndex());
			auto action = Sequence::create(DelayTime::create(actiondur/2.f), MoveTo::create(actiondur/2.f, Vec2(0.f, -_pageSize.height)), Hide::create(), nullptr);
			_equipPageView->runAction(action);

			_topHpInfoNode->setPositionX(-320.f);
			auto actionMove = Sequence::createWithTwoActions(DelayTime::create(actiondur/2.f),MoveBy::create(actiondur / 2.f, Vec2(640.f, 0.f)));
			_topHpInfoNode->runAction(actionMove);

			_skillPageView->setVisible(false);
		}
		else if (_curMenuIdx == MenuItem_E::SkillItem)
		{
			_playersLayer->showChangeBackTo(actiondur, _skillPageView->getCurPageIndex());
			auto action = Sequence::create(DelayTime::create(actiondur/2.f), MoveTo::create(actiondur/2.f, Vec2(0.f, -_pageSize.height)), Hide::create(), nullptr);
			_skillPageView->runAction(action);

			_topHpInfoNode->setPositionX(-320.f);
			auto actionMove = Sequence::createWithTwoActions(DelayTime::create(actiondur / 2.f), MoveBy::create(actiondur / 2.f, Vec2(640.f, 0.f)));
			_topHpInfoNode->runAction(actionMove);

			_equipPageView->setVisible(false);
		}
		else
		{
			_playersLayer->resetPos();
			_equipPageView->setVisible(false);
			_skillPageView->setVisible(false);
		}

		auto action = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([this]() {
			_skillPageView->setCurPageIndex(0);
			_equipPageView->setCurPageIndex(0);

			resetMarkPoint();
			//_playersLayer->resetPos();
		}));

		this->runAction(action);
	}
	else if (idx == MenuItem_E::WeaponsItem)
	{
		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepEnd();
//			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {GameGuideLayer::getInstance()->guideStepStart(0, Vec2::ZERO); })));
		}
		_bagLayer->setCanTouch(false);
		_markNode->setVisible(true);

		_worldLayer->setCanTouchMove(false);

		if (_curMenuIdx == MenuItem_E::SkillItem)
		{
			_equipPageView->setCurPageIndex(_skillPageView->getCurPageIndex());
			_equipPageView->setPosition(_pageViewFinalPos3);
			_equipPageView->setVisible(true);

			auto action = MoveTo::create(actiondur, _pageViewFinalPos);
			_equipPageView->runAction(action);

			auto action2 = Sequence::createWithTwoActions(MoveTo::create(actiondur, _pageViewFinalPos2), Hide::create());
			_skillPageView->runAction(action2);
		}
		else if (_curMenuIdx == MenuItem_E::BagItem)
		{
			_playersLayer->showChangeToOtherLayer(0.f);
			_equipPageView->setPosition(_pageViewFinalPos);
			_equipPageView->setVisible(true);
			_equipPageView->setCurPageIndex(0);
			_skillPageView->setVisible(false);

			resetMarkPoint();

			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = MoveBy::create(actiondur, Vec2(640.f, 0.f));
			_topHpInfoNode->runAction(actionMove);
		}
		else
		{
			_playersLayer->showChangeToOtherLayer(actiondur);
			_equipPageView->setPosition(Vec2(0.f, -_pageSize.height));
			_equipPageView->setVisible(true);
			_equipPageView->setCurPageIndex(0);
			
			resetMarkPoint();

			auto action = MoveTo::create(actiondur, _pageViewFinalPos);
			_equipPageView->runAction(action);

			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = MoveBy::create(actiondur / 2.f, Vec2(-640.f, 0.f));
			_topHpInfoNode->runAction(actionMove);

			_skillPageView->setVisible(false);
		}
		_curShowPageView = _equipPageView;
	}
	else if (idx == MenuItem_E::SkillItem)
	{
		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepEnd();
		}
		_bagLayer->setCanTouch(false);
		_worldLayer->setCanTouchMove(false);
		_markNode->setVisible(true);
		if (_curMenuIdx == MenuItem_E::WeaponsItem)
		{
			_skillPageView->setCurPageIndex(_equipPageView->getCurPageIndex());
			_skillPageView->setPosition(_pageViewFinalPos2);
			_skillPageView->setVisible(true);

			auto action = MoveTo::create(actiondur, _pageViewFinalPos);
			_skillPageView->runAction(action);

			auto action2 = Sequence::createWithTwoActions(MoveTo::create(actiondur, _pageViewFinalPos3), Hide::create());
			_equipPageView->runAction(action2);
		}
		else if (_curMenuIdx == MenuItem_E::BagItem)
		{
			_playersLayer->showChangeToOtherLayer(0.f);
			_skillPageView->setPosition(_pageViewFinalPos);
			_skillPageView->setVisible(true);
			_skillPageView->setCurPageIndex(0);
			_equipPageView->setVisible(false);

			resetMarkPoint();

			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = MoveBy::create(actiondur, Vec2(640.f, 0.f));
			_topHpInfoNode->runAction(actionMove);
		}
		else
		{
			_playersLayer->showChangeToOtherLayer(actiondur);
			_skillPageView->setPosition(Vec2(0.f, -_pageSize.height));
			_skillPageView->setVisible(true);
			auto action = MoveTo::create(actiondur, _pageViewFinalPos);
			_skillPageView->runAction(action);

			_topHpInfoNode->setPositionX(320.f);
			auto actionMove = MoveBy::create(actiondur/2.f, Vec2(-640.f, 0.f));
			_topHpInfoNode->runAction(actionMove);

			_equipPageView->setVisible(false);
		}

		_curShowPageView = _skillPageView;
	}
	else if (idx == MenuItem_E::BagItem)
	{
		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepEnd();
		}
		//_bagLayer->setCanTouch(false);
		_worldLayer->setCanTouchMove(false);
		_markNode->setVisible(false);
		_itemsLayer->setPositionX(0.f);
		_itemsLayer->runAction(Sequence::createWithTwoActions(MoveTo::create(actiondur, Vec2(-640.f, 0.f)), Hide::create()));

		_bagLayer->setPositionX(640.f);
		_bagLayer->setVisible(true);
		_bagLayer->setCanTouch(true);
		_bagLayer->runAction(MoveTo::create(actiondur, Vec2(0.f, (VisibleRect::top().y - 960.f) / 2.f)));

		if (_curMenuIdx == MenuItem_E::CharacterItem)
		{

		}
		else
		{
			_topHpInfoNode->setPositionX(960.f);
			auto actionMove = MoveBy::create(actiondur, Vec2(-640.f, 0.f));
			_topHpInfoNode->runAction(actionMove);
		}
	}

	if (_curMenuIdx == MenuItem_E::BagItem && idx != MenuItem_E::MapItem)
	{
		_itemsLayer->setVisible(true);
		_itemsLayer->setPositionX(-640.f);
		_itemsLayer->runAction(MoveTo::create(actiondur, Vec2(0.f, 0.f)));

		_bagLayer->setPositionX(0.f);
		_bagLayer->runAction(Sequence::createWithTwoActions(MoveTo::create(actiondur, Vec2(640.f, (VisibleRect::top().y - 960.f)/2.f)), Hide::create()));
	}

	if (idx == 0)
	{
		idx = 1;
	}

	if (idx >=1 && idx <5)
	{
		_selectLight->setPositionX(110.f + 105 * idx - 105);
	}
	

	_curMenuIdx = idx;
}

void MainLayer::initTimeCounrdown()
{	
	_leftBtnsNode = Node::create();
	this->addChild(_leftBtnsNode, MAPUI_Z);

	_rightBtnsNode = Node::create();
	this->addChild(_rightBtnsNode, MAPUI_Z);
	_rightBtnsNode->setPositionX(530.f);
	_rightBtnsNode->setName("right_menu_node");

	_boxtimenode = TimeCountDownNode::createWithNode( 20, TimeTypeEnum::TIME_OPENBOXCOUNTDOWN);
	this->addChild(_boxtimenode);
	_boxtimenode->startCountDown();

	auto pos = Vec2(150.f, VisibleRect::top().y - 200.f) + Vec2(-100.f, 60.f);

	_countdownNode[1] = TimeCountDownNode::createWithNode(12*60*60, TimeTypeEnum::TIME_FREEGOLDEBOX);
	this->addChild(_countdownNode[1]);

	_countdownNode[1]->setTimeCallBack([=](int click) {
		clickAction(click);
	});

	_countdownNode[0] = nullptr;
	_countdownNode[1]->startCountDown();

	//auto giftoffy = 0.f;

	/*auto giftcurname = "store_daylimit_gift.png";
	if (UserData::getInstance()->getCurGiftType() == 0)
	{
		giftcurname = "store_timeslimits_gift.png";
	}

	_daylimitGift = GameButton::create(giftcurname, "", "", Widget::TextureResType::PLIST);
	_rightBtnsNode->addChild(_daylimitGift, MAPUI_Z);
	_daylimitGift->setPosition(pos + Vec2(0.f, -300.f + giftoffy));
	_daylimitGift->setVisible(false);
	_daylimitGift->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
			
					std::string arritmeid[] = { StoreAssetMgr::ITEMID_GOOD_DAYGIFT ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1 ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2 };
					int idx = checkBuyGiftType();
					if (idx == -1)
					{
						return;
					}
					auto layer = PurchaseLayer::create(arritmeid[idx], "", this);
					this->addChild(layer, POP_Z);
			}
		}
	});*/

	
	//init left btns
	auto posleft = pos + Vec2(0.f, 20.f);
	_dayAcitvityBtn = GameButton::create("store_dayactivity.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_dayAcitvityBtn, MAPUI_Z);
	_dayAcitvityBtn->setPosition(posleft + Vec2(0.f, 0.f));
	_dayAcitvityBtn->setVisible(true);
	_dayAcitvityBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				auto pLayer = DayActivityLayer::create();
				this->addChild(pLayer, POP_Z);
			}
		}
	});
	auto dayredpoint = Sprite::createWithSpriteFrameName("box_red_bg.png");
	_dayAcitvityBtn->addChild(dayredpoint, MAPUI_Z);
	dayredpoint->setPosition(Vec2(60.f, 60.f));
	dayredpoint->setVisible(false);
	dayredpoint->setName("dayredpoint");

	_wujingBtn = GameButton::create("main_wujing_btn.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_wujingBtn, MAPUI_Z);
	_wujingBtn->setPosition(posleft + Vec2(0.f, -100.f));
	_wujingBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				//debug
#if (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)
				if (true)
#else
				if (UserData::getInstance()->getStoryFileIdx() > 4)
#endif
				{
					if (NetDataMgr::getInstance()->getHasSelfInfo())
					{
						auto pLayer = EndlessMainLayer::create();
						this->addChild(pLayer, POP_Z);
						if (!UserData::getInstance()->getEndlessGuided())
						{
							UserData::getInstance()->setEndlessGuided(true);
							auto pArm = _wujingBtn->getChildByName("guangquan");
							if (nullptr != pArm)
							{
								pArm->removeFromParent();
							}
						}
					}
					else
					{
						//toast offline
						//debug
						GameUtils::toastTip(1);
					}
				}
				else
				{
					GameUtils::toastTip(7);
				}
			}
		}
	});

	auto _dungeonBtn = GameButton::create("store_dungeon.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_dungeonBtn, MAPUI_Z);
	_dungeonBtn->setPosition(posleft + Vec2(0.f, -200.f));
	_dungeonBtn->setVisible(true);
	_dungeonBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				if (UserData::getInstance()->getIsBossPass(5) > 0)
				{
					auto pLayer = DungeonLayer::create();
					this->addChild(pLayer, POP_Z);
				}
				else
				{
					GameUtils::toastTip("dungeon_luck");
				}
				
			}
		}
	});



	_bglBtn = GameButton::create("main_bgl_btn.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_bglBtn, MAPUI_Z);
	_bglBtn->setPosition(posleft + Vec2(0.f, -300.f));
	_bglBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
				if (UserData::getInstance()->getIsBossPass(19) == 0)
				{
					GameUtils::toastTip("bgl_luck");
					return;
				}
#endif				
				auto layer = BGLLayer::create();
				this->addChild(layer, POP_Z);
			}
		}
	});


	_chengjiuBtn = GameButton::create("mian_chenjiu_btn.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_chengjiuBtn, MAPUI_Z);
	_chengjiuBtn->setPosition(posleft + Vec2(0.f, -400.f));
	_chengjiuBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				auto pLayer = AchieveLayer::create();
				this->addChild(pLayer, POP_Z);
			}
		}
	});

	_achCountBg = Sprite::createWithSpriteFrameName("box_red_bg.png");
	_chengjiuBtn->addChild(_achCountBg, MAPUI_POP);
	//	_achCountBg->setPosition(pos + Vec2(10.f, 24.f) + Vec2(0.f, -100.f) - _chengjiuBtn->getPosition() + Vec2(35.f, 30.f));
	_achCountBg->setPosition(Vec2(62.f, 62.f));

	_achCountText = Text::create();
	_achCountText->setFontSize(24);
	_achCountBg->addChild(_achCountText);
	_achCountText->setPosition(Vec2(_achCountBg->getContentSize().width / 2, _achCountBg->getContentSize().height / 2));


	auto _storyBtn = GameButton::create("main_juqing_btn.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_storyBtn, MAPUI_Z);
	_storyBtn->setPosition(posleft + Vec2(0.f, -500.f));
	_storyBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				auto layer = StoryInfoLayer::create();
				this->addChild(layer, MainLayer_Z::POP_Z);
			}
		}
	});

	auto _gonggaoBtn = GameButton::create("mian_gonggao_btn.png", "", "", Widget::TextureResType::PLIST);
	_leftBtnsNode->addChild(_gonggaoBtn, MAPUI_Z);
	_gonggaoBtn->setPosition(posleft + Vec2(0.f, -600.f));
	_gonggaoBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				auto pLayer = AnncLayer::create();
				this->addChild(pLayer, POP_Z);
			}
		}
	});

	

	

	auto hongdianstory = Sprite::createWithSpriteFrameName("box_red_bg.png");
	_leftBtnsNode->addChild(hongdianstory, MAPUI_Z);
	hongdianstory->setPosition(_storyBtn->getPosition() + Vec2(30.f, 30.f));
	hongdianstory->setVisible(false);
	hongdianstory->setName("hongdianleft");
	
	
	//init right btns
		float _offyRight = -100.f;
		_boxStoreBtn = GameButton::create("box_shop_btn.png", "", "", Widget::TextureResType::PLIST);
		_rightBtnsNode->addChild(_boxStoreBtn, MAPUI_Z);
		_boxStoreBtn->setPosition(pos + Vec2(0.f, _offyRight));
		_boxStoreBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
				{
					auto layer = StoreLayer2::create();

					this->addChild(layer, MainLayer_Z::POP_Z);
				}

			}
		});

		_arrrRightBnts.push_back(_boxStoreBtn);
		_offyRight -= 100.f;

		_rewardBtn = GameButton::create("main_box_btn.png", "", "", Widget::TextureResType::PLIST);
		_rightBtnsNode->addChild(_rewardBtn, 4);
		_rewardBtn->setPosition(pos + Vec2(0.f, _offyRight));
		_rewardBtn->getRendererNormal()->setOpacity(0);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/btnani/cangbaoge.ExportJson");
		_rewardBtnani = Armature::create("cangbaoge");
		_rewardBtnani->getAnimation()->play("daiji");
		_rightBtnsNode->addChild(_rewardBtnani, 3);
		_rewardBtnani->setPosition(_rewardBtn->getPosition() + Vec2(0.f, -20.f));
		_boxCountBg = _rewardBtnani->getBone("Layer8")->getDisplayRenderNode();

		_rewardBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::BEGAN)
			{
				_rewardBtnani->setScale(1.1f);
			}
			else if (type == Widget::TouchEventType::CANCELED)
			{
				_rewardBtnani->setScale(1.0f);
			}
			else if (type == Widget::TouchEventType::ENDED)
			{
				_rewardBtnani->setScale(1.0f);
				if (!_getOnlineTime)
				{
					GameUtils::toastTip("box_net_err");
					return;
				}

				if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
				{
					if (nullptr != GameGuideLayer::getInstance())
					{
						GameGuideLayer::getInstance()->guideStepEnd();
					}
					auto layer = BoxLayer::create();
					this->addChild(layer, POP_Z);
				}
			}
		});

		auto offfpos = _rewardBtn->getPosition() - Vec2(35.f, 30.f);
		_boxCountText = Text::create();
		_boxCountText->setFontSize(24);
		_boxCountBg->addChild(_boxCountText, MAPUI_POP);
		_boxCountText->setPosition(Vec2(_boxCountBg->getContentSize().width / 2, _boxCountBg->getContentSize().height / 2));

		_countdownNum[0] = ui::TextAtlas::create("110", "fonts/num_boxcountout.png", 12.f, 20.f, ".");
		_rewardBtn->addChild(_countdownNum[0], MAPUI_POP);
		_countdownNum[0]->setPosition(Vec2(35.f,-5.f));

		_boxCountBg->setVisible(false);
		_countdownNum[0]->setVisible(false);

		_offyRight -= 100.f;
		_arrrRightBnts.push_back(_rewardBtn);

		auto _allGift = GameButton::create("store_gifts.png", "", "", Widget::TextureResType::PLIST);
		_rightBtnsNode->addChild(_allGift, MAPUI_Z);
		_allGift->setPosition(pos + Vec2(0.f, _offyRight));
		_allGift->setVisible(true);
		_allGift->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
				{
					//auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_REBORN_2, "", this);
					//auto layer = DungeonLayer::create();
					auto layer = GiftLayer::create();
					this->addChild(layer, MainLayer_Z::POP_Z);
				}
			}
		});
		_arrrRightBnts.push_back(_allGift);

		if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT) == 0)
		{
			//
			_offyRight -= 100.f;

			_giftBtn = GameButton::create("store_icon_firstbuy.png", "", "", Widget::TextureResType::PLIST);
			_rightBtnsNode->addChild(_giftBtn, MAPUI_POP);
			_giftBtn->setName("firstgift");
			_giftBtn->setPosition(pos + Vec2(0.f, _offyRight));

			_giftBtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
					{
						auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT, "", this);
						this->addChild(layer, MainLayer_Z::POP_Z);
					}
				}
			});

			//libao.ExportJson
			ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
			auto pArmtr = Armature::create("libao");
			_giftBtn->addChild(pArmtr);
			pArmtr->getAnimation()->play("libao_2");
			pArmtr->setPosition(Vec2(35.f, 35.f));

			_arrrRightBnts.push_back(_giftBtn);
		}
		else
		{

		}

		_offyRight -= 100.f;

		_7xiActivityBtn = GameButton::create("store_icon_qixi.png", "", "", Widget::TextureResType::PLIST);
		_rightBtnsNode->addChild(_7xiActivityBtn, MAPUI_Z);
		_7xiActivityBtn->setPosition(pos + Vec2(0.f, _offyRight));
		_7xiActivityBtn->setVisible(true);
		_7xiActivityBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
				{
					auto pLayer = MagPieLayer::create();
					this->addChild(pLayer, POP_Z);
				}
			}
		});
		_arrrRightBnts.push_back(_7xiActivityBtn);
	
	_backToForegroundlistener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[this](EventCustom*)
	{
		getCurOnlineTime();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_backToForegroundlistener, this);
	getCurOnlineTime();
}

void MainLayer::initActivityInfo(int idx)
{
	CCLOG("init activity %d", idx);

	auto pos = Vec2(150.f, VisibleRect::top().y - 200.f) + Vec2(-100.f, 60.f);

	_timeLimitActivityBtn = GameButton::create("store_icon_jrjl.png","","", Widget::TextureResType::PLIST);
	_rightBtnsNode->addChild(_timeLimitActivityBtn);
	_timeLimitActivityBtn->setPosition(pos);

	_arrrRightBnts.push_back(_timeLimitActivityBtn);

	_timeLimitActivityText = ui::TextAtlas::create("", "fonts/num_boxcountout.png", 12.f, 20.f, ".");
	_timeLimitActivityBtn->addChild(_timeLimitActivityText, MAPUI_POP);
	_timeLimitActivityText->setPosition(Vec2(45.f, -10.f));

	Sprite* redpoing = Sprite::createWithSpriteFrameName("box_red_bg.png");
	_timeLimitActivityBtn->addChild(redpoing, 1, "redpoint");
	redpoing->setVisible(false);
	redpoing->setPosition(Vec2(60.f, 50.f));

	Text* textnum = Text::create();
	textnum->setFontSize(24);
	redpoing->addChild(textnum, 2, "textnum");
	textnum->setPosition(Vec2(15.f, 15.f));

	if (TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum() > 0)
	{
		_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(true);
		static_cast<Text*>(_timeLimitActivityBtn->getChildByName("redpoint")->getChildByName("textnum"))->setString(String::createWithFormat("%d", TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum())->getCString());
	}
	else
	{
		_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(false);
	}

	//_timeActivityNode->startCountDown();
	if (UserData::getInstance()->getOnlineCountdownTime() == -1)
	{
		TimeLimitActivityMgr::getInstance()->resetOnlineTimeReward();
	}

	_timeLimitActivityBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
			{
				if (TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum() <= 0)
				{
					GameUtils::toastTip("online_reward_end");
				}

				TimeLimitActivityMgr::getInstance()->getReward(TimeLimitActivityMgr::s_CurActivityName);
				if (TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum() > 0)
				{
					_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(true);
				}
				else
				{
					_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(false);
				}

				static_cast<Text*>(_timeLimitActivityBtn->getChildByName("redpoint")->getChildByName("textnum"))->setString(String::createWithFormat("%d", TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum())->getCString());
			}
		}
	});
	updataGiftBtns();

	this->schedule(CC_CALLBACK_1(MainLayer::offlineClickAction, this), 1, CC_REPEAT_FOREVER, 0, "offlineClickAction");
}

void MainLayer::resetMarkPoint()
{
	for (int i = 0; i < 4; i++)
	{
		auto sprite = static_cast<Sprite*>(_markNode->getChildByName(String::createWithFormat("mark_%d", i)->getCString()));
		if (0 == i)
		{
			sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_1.png"));
		}
		else
		{
			sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_0.png"));
		}
	}
}

void MainLayer::showNewGetItem()
{
	int cur = BagItemControl::getInstace()->getNeedShowItemOne();
	if (cur  != -1)
	{
		if (_showNewGetCallBack)
		{
			_showNewGetCallBack();
		}

		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer, POP_Z + 1);

		auto textbg = Sprite::createWithSpriteFrameName("mainui_wenpontext_bg.png");
		layer->addChild(textbg);
		textbg->setPosition(Vec2(320.f, VisibleRect::top().y / 2));
		textbg->setScale(0.7f);

		auto effectsp = Sprite::createWithSpriteFrameName("effect_com_light.png");
		layer->addChild(effectsp);
		effectsp->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f + 180.f));
		auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
		effectsp->runAction(actionrotate);

		int playeridx = 0;
		std::string name;
		std::string get;
		std::string itemname;

		std::string title;
		if (BagItemControl::getInstace()->checkItemType(cur) == 1)
		{
			playeridx = SkillControl::getInstance()->getSkillOwnerBySkillid(cur);
			get = ResMgr::getInstance()->getString("getnewskill")->getCString();
			title = ResMgr::getInstance()->getString("getnewskilltitle")->getCString();
		}
		else
		{
			//warming 500 is the start id of wenponid
			playeridx = (cur - 500) / 10;
			get = ResMgr::getInstance()->getString("getneww")->getCString();
			title = ResMgr::getInstance()->getString("getnewwtitle")->getCString();
		}
		
		name = ResMgr::getInstance()->getString(String::createWithFormat("player_%d", playeridx)->getCString())->getCString();

		ComInfo_T temp;
		ParamMgr::getInstance()->getComGameItemInfo(cur, temp);

		itemname = temp.name;

		auto richtextCur = RichText::create();
		auto temp0 = RichElementText::create(0, Color3B(145, 80, 39), 255, name, "", 24);
		auto temp1 = RichElementText::create(0, Color3B(), 255, get, "", 24);
		auto temp2 = RichElementText::create(0, Color3B(145, 80, 39), 255, itemname, "", 24);

		richtextCur->pushBackElement(temp0);
		richtextCur->pushBackElement(temp1);
		richtextCur->pushBackElement(temp2);

		richtextCur->setPosition(Vec2(320.f, VisibleRect::top().y / 2 - 15.f));
		layer->addChild(richtextCur);

		auto texttitle = Text::create();
		layer->addChild(texttitle);
		texttitle->setString(title);
		texttitle->setFontSize(30);
		texttitle->setPosition(Vec2(320.f, VisibleRect::top().y / 2 + 45.f));
		texttitle->setTextColor(Color4B(129, 77, 0, 255));

		auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
		layer->addChild(sprite);
		sprite->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f + 180.f));

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch * touch, Event * unusedEvent) {
			int nextidx = BagItemControl::getInstace()->getNeedShowItemOne();

			if (nextidx != -1)
			{
				if (_showNewGetCallBack)
				{
					_showNewGetCallBack();
				}

				ComInfo_T tempnext;
				ParamMgr::getInstance()->getComGameItemInfo(nextidx, tempnext);
				sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(tempnext.picname));

				int playeridx = 0;
				std::string name;
				std::string get;
				std::string itemname;
				std::string title;
				if (BagItemControl::getInstace()->checkItemType(nextidx) == 1)
				{
					playeridx = SkillControl::getInstance()->getSkillOwnerBySkillid(nextidx);
					get = ResMgr::getInstance()->getString("getnewskill")->getCString();
					title = ResMgr::getInstance()->getString("getnewskilltitle")->getCString();
				}
				else
				{
					//warming 500 is the start id of wenponid
					playeridx = (nextidx - 500) / 10;
					get = ResMgr::getInstance()->getString("getneww")->getCString();
					title = ResMgr::getInstance()->getString("getnewwtitle")->getCString();
				}

				name = ResMgr::getInstance()->getString(String::createWithFormat("player_%d", playeridx)->getCString())->getCString();
				texttitle->setString(title);

				itemname = tempnext.name;

				auto temp0 = RichElementText::create(0, Color3B(145, 80, 39), 255, name, "", 24);
				auto temp1 = RichElementText::create(0, Color3B(), 255, get, "", 24);
				auto temp2 = RichElementText::create(0, Color3B(145, 80, 39), 255, itemname, "", 24);
				
				richtextCur->removeElement(2);
				richtextCur->removeElement(1);

				richtextCur->pushBackElement(temp0);
				richtextCur->pushBackElement(temp1);
				richtextCur->pushBackElement(temp2);

				richtextCur->removeElement(0);
			}
			else
			{
				layer->removeFromParent();
			}

			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
	}

}

void MainLayer::meunEffectMainOut(float dur)
{
	float actiondur = dur;
	//_arrMenuItems[0]->getBgSprite()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));
	_arrMenuItems[0]->getNormalImage()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));
	//_arrSpriteMenuText[0]->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));

	_secondMenu->setVisible(true);
	for (int i = 1; i < MenuItem_E::ItemNums; i++)
	{
		//_arrMenuItems[i]->getBgSprite()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
		_arrMenuItems[i]->getNormalImage()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
		//_arrSpriteMenuText[i]->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
	}

	auto menuAction = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([=]() {
		_mainMenu->setVisible(false);
		_secondMenu->setEnabled(true);
	}));

	this->runAction(menuAction);

	auto fadeout = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([this]() {
		_mapLayer->setVisible(false);
	}));
	_mapLayer->runAction(fadeout);

	auto fadeout2 = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([this]() {
		_worldLayer->setVisible(false);
	}));

	_worldLayer->runAction(fadeout2);

	auto action = FadeIn::create(actiondur);
	_playerbg->runAction(action);
	_logo->runAction(action->clone());

	_itemsLayer->setPositionX(640.f);
	_itemsLayer->setVisible(true);
	_itemsLayer->runAction(MoveTo::create(actiondur, Vec2(0.f, 0.f)));
	_selectLight->setVisible(true);
}

void MainLayer::menuEffectSecondOut()
{
	float actiondur = 0.5f;
	_mainMenu->setVisible(true);


	//_arrMenuItems[0]->getBgSprite()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
	_arrMenuItems[0]->getNormalImage()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
	//_arrSpriteMenuText[0]->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_SHOW));
	for (int i = 1; i < MenuItem_E::ItemNums; i++)
	{
		//_arrMenuItems[i]->getBgSprite()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));
		_arrMenuItems[i]->getNormalImage()->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));
		//_arrSpriteMenuText[i]->runAction(EffectAction::create(actiondur, EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT));
	}

	auto menuAction = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([=]() {
		_secondMenu->setVisible(false);
		_mainMenu->setEnabled(true);
	}));
	this->runAction(menuAction);

	auto action = FadeOut::create(actiondur);
	_playerbg->runAction(action);
	_logo->runAction(action->clone());

	//auto fadeout = Sequence::createWithTwoActions(DelayTime::create(actiondur), CallFunc::create([this]() {
		_mapLayer->setVisible(true);
		_worldLayer->setVisible(true);
		_selectLight->setVisible(false);
	//}));

	//this->runAction(fadeout);
}

void MainLayer::setStartState()
{
	_equipPageView->setCurPageIndex(0);
	_skillPageView->setCurPageIndex(0);
	resetMarkPoint();
	_equipPageView->setPosition(_pageViewFinalPos);
	_skillPageView->setPosition(_pageViewFinalPos2);
	_playersLayer->resetPos();
	_itemsLayer->setPosition(Vec2(0.f, 0.f));
}

bool MainLayer::onTouchBegan(Touch * touch, Event * unusedEvent)
{
	if (_curMenuIdx == MenuItem_E::WeaponsItem)
	{
		getCurDisplayEquipLayer()->touchBegan(touch->getLocation());
		return true;
	}
	else if (_curMenuIdx == MenuItem_E::SkillItem)
	{
		getCurDisPlaySkillLayer()->touchBegan(touch->getLocation());
		return true;
	}

	return false;
}

void MainLayer::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	if (_equipPageView->isVisible())
	{
		_equipPageView->onTouchMoved(touch, unusedEvent);
	}
	if (_skillPageView->isVisible())
	{
		_skillPageView->onTouchMoved(touch, unusedEvent);
	}
	
	if ((_playersLayer->getPlayersPages())->isVisible())
	{
		(_playersLayer->getPlayersPages())->onTouchMoved(touch, unusedEvent);
	}
	
}

void MainLayer::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	if (_equipPageView->isVisible())
	{
		_equipPageView->onTouchEnded(touch, unusedEvent);
	}
	if (_skillPageView->isVisible())
	{
		_skillPageView->onTouchEnded(touch, unusedEvent);
	}

	if ((_playersLayer->getPlayersPages())->isVisible())
	{
		(_playersLayer->getPlayersPages())->onTouchEnded(touch, unusedEvent);
	}
}

void MainLayer::onTouchCancelled(Touch * touch, Event * unusedEvent)
{
	if (_equipPageView->isVisible())
	{
		_equipPageView->onTouchCancelled(touch, unusedEvent);
	}
	if (_skillPageView->isVisible())
	{
		_skillPageView->onTouchCancelled(touch, unusedEvent);
	}

	if ((_playersLayer->getPlayersPages())->isVisible())
	{
		(_playersLayer->getPlayersPages())->onTouchCancelled(touch, unusedEvent);
	}
}

void MainLayer::startMenuProtect(float time)
{
	_isMenuInProtect = true;
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]() {
		_isMenuInProtect = false;
	}));
	this->runAction(action);
}

void MainLayer::enterStory()
{
	_mainMenu->setVisible(false);
	_menubg->setVisible(false);

	_worldLayer->setVisible(false);
}

void MainLayer::leaveStory(bool win)
{
	auto action = Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create([=]() {
		_mainMenu->setVisible(true);
		_menubg->setVisible(true);
		_worldLayer->setVisible(true);
	}));

	this->runAction(action);


	auto action2 = Sequence::createWithTwoActions(DelayTime::create(2.5f), CallFunc::create([=]() {
		if (win == false)
		{
			popBossGuide();
		}
	}));
	this->runAction(action2);
}

void MainLayer::enterFight()
{
	_numInfoNode->setVisible(false);

	/*_boxStoreBtn->setVisible(false);
	_wujingBtn->setVisible(false);
	_chengjiuBtn->setVisible(false);
	_rewardBtn->setVisible(false);
	_bglBtn->setVisible(false);*/
	_leftBtnsNode->setVisible(false);
	_rightBtnsNode->setVisible(false);

	if (nullptr != _giftBtn && UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT) == 0)
	{
		_giftBtn->setVisible(false);
	}
}

void MainLayer::leaveFight()
{
	this->enterMainLayer();

	auto act = Sequence::createWithTwoActions(DelayTime::create(1.5f), Show::create());
	_numInfoNode->runAction(act);

	/*_boxStoreBtn->runAction(act->clone());
	_wujingBtn->runAction(act->clone());
	_chengjiuBtn->runAction(act->clone());
	_rewardBtn->runAction(act->clone());
	_bglBtn->runAction(act->clone());*/
	_leftBtnsNode->runAction(act->clone());
	_rightBtnsNode->runAction(act->clone());
	
	if (nullptr != _giftBtn && UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT) == 0)
	{
		_giftBtn->runAction(act->clone());
	}
	
	if (StoryInfoLayer::getCurShowTextIdx() != UserData::getInstance()->getShowTextIdx())
	{
		_leftBtnsNode->getChildByName("hongdianleft")->setVisible(true);
	}
	else
	{
		_leftBtnsNode->getChildByName("hongdianleft")->setVisible(false);
	}
// 	_numInfoNode->setVisible(true);
// 	_boxStoreBtn->setVisible(true);
// 	_wujingBtn->setVisible(true);
// 	_chengjiuBtn->setVisible(true);
// 	_rewardBtn->setVisible(true);
}

void MainLayer::enterMainLayer()
{
	if (this->checkGuide())
	{
		//avoid can't touch
		return;
	}
	BagItemControl::getInstace()->autoEquipItems();
	GameLayer::getInstance()->updateEquipItems();

	bool isCanPop = true;
	if (NetDataMgr::getInstance()->getHasSelfInfo())
	{
		if (2 == _settleRankState)
		{
			//show reward
			//after reward save settle day,_settleRankState = 0
			if (isCanPop)
			{
				std::vector<PopItemInfo_T> arrReward;
				CrushUtil::getSettleReward(NetDataMgr::getInstance()->getSelfSettleScoreRank(), arrReward);
				auto pLayer = PopRewardLayer::create(arrReward, 1);
				this->addChild(pLayer, POP_REWARD_Z);
				pLayer->setRankIdx(NetDataMgr::getInstance()->getSelfSettleScoreRank());
				UserData::getInstance()->setLastSettleDay(NetDataMgr::getInstance()->getOnlineDay());

				_settleRankState = 0;
				isCanPop = false;
			}
		}
	}
	else
	{
		//		NetDataMgr::getInstance()->queryRole(CC_CALLBACK_2(MainLayer::onGetSelfInfoCallback, this));
	}

	if (AchieveMgr::getInstance()->checkFinishAchieve() > 0)
	{
		//modify achieve icon
		_achCountBg->setVisible(true);
		auto strText = __String::createWithFormat("%d", AchieveMgr::getInstance()->checkFinishAchieve());
		_achCountText->setString(strText->getCString());
	}
	else
	{
		//reset achieve icon
		_achCountBg->setVisible(false);
	}

	if (_isNeedDaily)
	{
		auto pLayer = DailyRewardLayer::create();
		this->addChild(pLayer, POP_Z);

		_isNeedDaily = false;
	}
	else if (_isNeedWeekReward)
	{
		auto pLayer = DailyRewardLayer::create(true);
		this->addChild(pLayer, POP_Z);

		_isNeedWeekReward = false;
	}

	if (cocos2dx_plat::getGameValue(AnncLayer::ANNC_KEY_SHOW) == "1")
	{
		if (isCanPop)
		{
			auto pLayer = AnncLayer::create();
			this->addChild(pLayer, POP_Z);
			isCanPop = false;
		}
		else
		{
			//show red point
		}
	}
	else
	{
		//hide red point
	}
}

void MainLayer::moveDownMenu(float time)
{
	if (_menuUp)
	{
		auto action = EaseSineOut::create(MoveBy::create(time, Vec2(0.f, -150.f)));
		_menubg->runAction(action);
		_mainMenu->runAction(action->clone());

		auto action2 = EaseSineOut::create(MoveBy::create(time * 0.6, Vec2(-100.f, 0.f)));
		auto action3 = EaseSineOut::create(MoveBy::create(time * 0.6, Vec2(130.f, 0.f)));
		/*_boxStoreBtn->runAction(action2);
		_wujingBtn->runAction(action2->clone());
		_chengjiuBtn->runAction(action2->clone());
		_rewardBtn->runAction(action2->clone());
		_bglBtn->runAction(action2->clone());*/
		_leftBtnsNode->runAction(action2);
		
		_rightBtnsNode->runAction(action3);

		_menuUp = false;
	}
}

void MainLayer::moveUpMenu(float time)
{
	if (!_menuUp)
	{
		auto action = EaseSineOut::create(MoveBy::create(time, Vec2(0.f, 150.f)));
		_menubg->runAction(action);
		_mainMenu->runAction(action->clone());

		auto action2 = EaseSineOut::create(MoveBy::create(time * 0.6, Vec2(100.f, 0.f)));
		auto action3 = EaseSineOut::create(MoveBy::create(time * 0.6, Vec2(-130.f, 0.f)));
		/* _boxStoreBtn->runAction(action2);
		 _wujingBtn->runAction(action2->clone());
		 _chengjiuBtn->runAction(action2->clone());
		 _rewardBtn->runAction(action2->clone());
		 _bglBtn->runAction(action2->clone());*/
		_leftBtnsNode->runAction(action2);

		_rightBtnsNode->runAction(action3);
		_menuUp = true;


	}
}

void MainLayer::updataTopInfo()
{
	int hp = 0;
	int dp = 0;
	for (int i = 0; i < 4; i++)
	{
		auto& temp  = ParamMgr::getInstance()->getPlayerInfo(i, UserData::getInstance()->getPlayerCurLv(i));
		hp += temp.hp;
		dp += temp.dp;
		dp += WeaponControl::getInstance()->getEquipWenpon(i).def;
	}

	_topHpAtlas->setString(String::createWithFormat("%d", hp)->getCString());
	_topDpAtlas->setString(String::createWithFormat("%d", dp)->getCString());
}

void MainLayer::updataGold()
{
	_goldNumAtlas->setString(String::createWithFormat("%d", UserData::getInstance()->getGoldNum())->getCString());
	_crystalNumAtlas->setString(String::createWithFormat("%d", UserData::getInstance()->getCrystalNum())->getCString());
	_tiliNumAtlas->setString(String::createWithFormat("%d", UserData::getInstance()->getTili())->getCString());

	if (UserData::getInstance()->getTili() == ParamData::MAX_TILI)
	{
		getTiliMaxIcon()->setVisible(true);
	}
}

void MainLayer::updataPlayersInfo(int idx)
{
	_playersLayer->updataPlayerInfo(idx);
}

SkillLayer * MainLayer::getCurDisPlaySkillLayer()
{
	if (_curMenuIdx == MenuItem_E::SkillItem)
	{
		int idx =_curShowPageView->getCurPageIndex();
		return _arrSkillLayer[idx];
	}
	return nullptr;
}

EquipLayer * MainLayer::getCurDisplayEquipLayer()
{
	if (_curMenuIdx == MenuItem_E::WeaponsItem)
	{
		int idx = _curShowPageView->getCurPageIndex();
		return _arrEquipLayer[idx];
	}
	return nullptr;
}

MainLayer * MainLayer::getCurMainLayer()
{
	return s_curMainLayer;
}

void MainLayer::pauseGame()
{
	_worldLayer->pauseGame();
	_mapLayer->pauseGame();
}

void MainLayer::resumGame()
{
	_worldLayer->resumGame();
	_mapLayer->resumGame();
}

void MainLayer::getCurOnlineTime()
{
	_getOnlineTime = false;

	std::thread t1([&]() {
		std::string timestring = cocos2dx_plat::getonlineTime();
		if (timestring.compare("error") == 0)
		{

		}
		else
		{
			std::vector<std::string > vectorNumString;
			ToolsUtil::split(timestring, "-", vectorNumString);
			int year = atoi(vectorNumString[0].c_str());
			int day = atoi(vectorNumString[1].c_str());
			int hour = atoi(vectorNumString[2].c_str());
			int min = atoi(vectorNumString[3].c_str());
			int sec = atoi(vectorNumString[4].c_str());

			_curTime = hour * 60 * 60 + min * 60 + sec;
			_getOnlineTime = true;
		}
	});

	t1.detach();
}

bool MainLayer::checkCanFreeGetBox()
{
	if (!_getOnlineTime)
	{
		return false;
	}
	bool can = false;

	int curhour = MainLayer::getCurMainLayer()->getCurTime() / 3600;

	auto _freeGetTimeIdx = -1;
	if (curhour >= 7 && curhour < 9)
	{
		_freeGetTimeIdx = 0;
	}
	else if (curhour >= 12 && curhour < 14)
	{
		_freeGetTimeIdx = 1;
	}
	else if (curhour >= 18 && curhour < 20)
	{
		_freeGetTimeIdx = 2;
	}
	else
	{
		_freeGetTimeIdx = -1;
	}

	if (_freeGetTimeIdx != -1 && UserData::getInstance()->getTiliRewardTimes(_freeGetTimeIdx) == 0)
	{
		can = true;
	}
	else
	{
		can = false;
	}

	return can;
}

void MainLayer::firstGiftBuyCallBack()
{
	auto gift = MainLayer::getCurMainLayer()->getChildByName("right_menu_node")->getChildByName("firstgift");
	if (nullptr != gift)
	{
		gift->setVisible(false);
	}

	/*auto light = _lightArr[2];
	light->setVisible(false);
	light->stopAllActions();*/

	//_daylimitGift->setPositionY(_daylimitGift->getPositionY() + 100.f);
	/*auto light3 = _lightArr[3];
	light3->setPositionY(light3->getPositionY() + 100.f);*/

	updataGiftBtns();
}

int MainLayer::checkBuyGiftType()
{
	if (UserData::getInstance()->getCurGiftType() == 1)
	{
		return 0;
	}
	else
	{
		if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT) == 0 && UserData::getInstance()->getPlayerCurLv(0) < 15)
		{
			return 1;
		}
		else if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1) == 0 && UserData::getInstance()->getPlayerCurLv(0) >= 15  && UserData::getInstance()->getPlayerCurLv(0) < 20)
		{
			return 2;
		}
		else if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2) == 0 && UserData::getInstance()->getPlayerCurLv(0) >= 20 )
		{
			return 3;
		}

	}
	
	return -1;
}

int MainLayer::checkTimeLimitGift()
{
	if (UserData::getInstance()->getCurGiftType() == 1)
	{
		return -1;
	}
	else
	{
		if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT) == 0 && UserData::getInstance()->getPlayerCurLv(0) < 15)
		{
			return 1;
		}
		else if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1) == 0 && UserData::getInstance()->getPlayerCurLv(0) >= 15 && UserData::getInstance()->getPlayerCurLv(0) < 20)
		{
			return 2;
		}
		else if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2) == 0 && UserData::getInstance()->getPlayerCurLv(0) >= 20)
		{
			return 3;
		}

	}

	return -1;
}

void MainLayer::updataGiftBtns()
{
	int idx = checkBuyGiftType();
	/*if (idx == -1)
	{
		_daylimitGift->setVisible(false);

		if (_7xiActivityBtn)
		{
			_7xiActivityBtn->setPosition(_daylimitGift->getPosition());
		}
	}
	else
	{
		_daylimitGift->setVisible(true);

		if (_7xiActivityBtn)
		{
			_7xiActivityBtn->setPosition(_daylimitGift->getPosition() + Vec2(0.f, -100.f));
		}
	}*/

	if (nullptr != _7xiActivityBtn)
	{
		int dayIdx = MagPieMgr::getInstance()->getDayIdx();
		if (dayIdx > 6)
		{
			_7xiActivityBtn->setVisible(false);
		}
		else
		{
			_7xiActivityBtn->setVisible(true);
		}
	}

	auto pos = Vec2(150.f, VisibleRect::top().y - 200.f) + Vec2(-100.f, 60.f);
	int idx2 = 0;
	if (_timeLimitActivityBtn == nullptr || (_timeLimitActivityBtn != nullptr && _timeLimitActivityBtn->isVisible() == false))
	{
		//pos += Vec2(0.f, 100.f);
	}
	for (auto& temp : _arrrRightBnts)
	{
		temp->setPosition(pos + Vec2(0.f, -100.f * idx2));
		if (temp == _rewardBtn)
		{
			_rewardBtnani->setPosition(_rewardBtn->getPosition() + Vec2(0.f, -20.f));
		}
		if (temp && temp->isVisible() == true)
		{
			idx2++;
		}		
	}
}


void MainLayer::updataDayActivityBtn()
{
	if (_dayAcitvityBtn)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (DayActivityMgr::getInstance()->checkCanGet(i) == 1)
			{
				_dayAcitvityBtn->getChildByName("dayredpoint")->setVisible(true);
				return;
			}
		}
		
		_dayAcitvityBtn->getChildByName("dayredpoint")->setVisible(false);
	}
}

void MainLayer::touchAddBtn(Ref * btn, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		auto node = static_cast<Node*>(btn);
		if (node->getName().compare("gold") == 0)
		{
			auto layer = StoreLayer2::create();
			this->addChild(layer, MainLayer_Z::POP_Z);
			layer->jumpToPage(2);
		}
		else if (node->getName().compare("cry") == 0)
		{
			auto layer = StoreLayer2::create();
			this->addChild(layer, MainLayer_Z::POP_Z);
			layer->jumpToPage(3);
		}
		else if (node->getName().compare("tili") == 0)
		{
			auto layer = StoreLayer2::create();
			this->addChild(layer, MainLayer_Z::POP_Z);
			layer->jumpToPage(1);
		}
	}
}

void MainLayer::updataDayAndTimeGiftBtn()
{
	/*auto giftcurname = "store_daylimit_gift.png";
	if (UserData::getInstance()->getCurGiftType() == 0)
	{
		giftcurname = "store_timeslimits_gift.png";
	}

	if (_daylimitGift)
	{
		_daylimitGift->setVisible(true);
		_daylimitGift->loadTextures(giftcurname, "", "", Widget::TextureResType::PLIST);
	}*/

	updataGiftBtns();
}

void MainLayer::popBuyItemOrLayer(int idx)
{
	if (idx == 0)
	{
		std::string arritmeid[] = { StoreAssetMgr::ITEMID_GOOD_DAYGIFT ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1 ,StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2 };
		int idx = checkTimeLimitGift();
		if (idx == -1)
		{
			return;
		}
		auto layer = PurchaseLayer::create(arritmeid[idx], "", this);
		this->addChild(layer, POP_Z);
	}
	else if (idx == 1)
	{
		auto layer = StoreLayer2::create();
		this->addChild(layer, MainLayer_Z::POP_Z);
		layer->jumpToPage(1);
	}
	else if (idx == 2)
	{
		auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_DAYGIFT, "", this);
		this->addChild(layer, POP_Z);
	}
	else if (idx == 3)
	{
		auto layer = StoreLayer2::create();
		this->addChild(layer, MainLayer_Z::POP_Z);
		layer->jumpToPage(0);
	}
}

void MainLayer::popBossGuide()
{
	if (UserData::getInstance()->getIsBossPass(10) > 0 && UserData::getInstance()->getPopGuideBoss() == 1)
	{
		UserData::getInstance()->setPopGuideBoss(0);

		WordMap::getInstance()->popNearyBoss();
		auto pRole = Player::getInstance()->getRole(2);
		pRole->startTalk("guide_pop_boss");
		Player::getInstance()->runAction(Sequence::create(DelayTime::create(3.f), CallFunc::create([=] {pRole->stopTalk(); }), nullptr));
	}
}

void MainLayer::initCommondActivity(int curday)
{
	_commondActivityBtn = GameButton::create("store_icon_midautumn.png", "", "", Widget::TextureResType::PLIST);
	_rightBtnsNode->addChild(_commondActivityBtn);
	_commondActivityBtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED)
		{
			return;
		}

		if (_curMenuIdx == -1 || _curMenuIdx == MenuItem_E::MapItem)
		{
			auto pLayer = MidAutumnLayer::create();
			this->addChild(pLayer, POP_Z);
		}
	});
	_arrrRightBnts.push_back(_commondActivityBtn);
	updataGiftBtns();
}

void MainLayer::clickAction(int click)
{
	bool opentimeneed = true;
	bool freegetneed = false;
	

	if (_boxtimenode && UserData::getInstance()->getIsBoxUnlock() != -1)
	{
		click = _boxtimenode->getDur();
		if (click <= 0)
		{
			opentimeneed = false;
			_countdownNum[0]->setVisible(false);
		}
		else
		{
			opentimeneed = true;
			_countdownNum[0]->setVisible(true);

			int hour = click / 3600;
			int min = click % 3600 / 60;
			int sec = click % (60);
			_countdownNum[0]->setString(String::createWithFormat("%02d.%02d.%02d", hour, min, sec)->getCString());
		}
	}
	else
	{
		_countdownNum[0]->setVisible(false);
	}

	for (int i = 0; i < 4; i++)
	{
		auto stage = UserData::getInstance()->getBoxLayerStage(i);
		if (stage == 2)
		{
			opentimeneed = false;
			break;
		}

		auto& configbox = UserData::getInstance()->getBoxLayerConfig(i);
		if (configbox.boxidx != -1 && stage == 0 && UserData::getInstance()->getIsBoxUnlock() == -1)
		{
			opentimeneed = false;
			break;
		}
	}

	if (_getOnlineTime)
	{
		_curTime++;
		if (checkCanFreeGetBox())
		{
			freegetneed = true;
		}
	}

	if (freegetneed || !opentimeneed)
	{
		if (_boxCountBg->isVisible() == false)
		{
			_rewardBtnani->getAnimation()->play("huangdong");
		}
		_boxCountBg->setVisible(true);
	}
	else
	{
		if (_boxCountBg->isVisible() == true)
		{
			_rewardBtnani->getAnimation()->play("daiji");
		}
		_boxCountBg->setVisible(false);
	}
}

void MainLayer::offlineClickAction(float dt)
{
	int time = UserData::getInstance()->getOnlineCountdownTime();
	if (UserData::getInstance()->getOnlineCountdownTime() > 0)
	{
		//int time = UserData::getInstance()->getOnlineCountdownTime();
		UserData::getInstance()->setOnlineCountdownTime(time - 1);

		if (_timeLimitActivityText)
		{
			int hour = time / 3600;
			int min = time % 3600 / 60;
			int sec = time % (60);
			_timeLimitActivityText->setString(String::createWithFormat("%02d.%02d.%02d", hour, min, sec)->getCString());
		}
	}
	else
	{
	
			int curRewardIdx = ParamData::ONLINE_TEACHER_REWARD_NUM;
			for (int i = 0; i < ParamData::ONLINE_TEACHER_REWARD_NUM; i++)
			{
				int stage = UserData::getInstance()->getOnlineTimeReward(i);
				if (stage == 0)
				{
					curRewardIdx = i;
					break;
				}
			}

			if ( curRewardIdx != ParamData::ONLINE_TEACHER_REWARD_NUM)
			{
				UserData::getInstance()->setOnlineReward(curRewardIdx, 1);

				if (curRewardIdx == ParamData::ONLINE_TEACHER_REWARD_NUM - 1)
				{
					_timeLimitActivityText->setVisible(false);
					_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(true);
					static_cast<Text*>(_timeLimitActivityBtn->getChildByName("redpoint")->getChildByName("textnum"))->setString(String::createWithFormat("%d", TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum())->getCString());
					this->unschedule("offlineClickAction");					
					updataGiftBtns();
				}
				else
				{
					_timeLimitActivityBtn->getChildByName("redpoint")->setVisible(true);
					static_cast<Text*>(_timeLimitActivityBtn->getChildByName("redpoint")->getChildByName("textnum"))->setString(String::createWithFormat("%d", TimeLimitActivityMgr::getInstance()->checkCanGetRewardNum())->getCString());
						
					UserData::getInstance()->setOnlineCountdownTime((TimeLimitActivityMgr::getInstance()->getRewardInfo(TimeLimitActivityMgr::s_CurActivityName,curRewardIdx + 1).timemin * 60));
				}
			}
			else
			{
				_timeLimitActivityText->setVisible(false);
				this->unschedule("offlineClickAction");				
				updataGiftBtns();
			}
		}
	
}


void MainLayer::removeBoxTimeNode()
{
	if (_boxtimenode)
	{
		_boxtimenode->removeFromParent();
		_boxtimenode = nullptr;
	}
}

void MainLayer::addBoxTimeNode(int time)
{
	//removeBoxTimeNode();

	//_boxtimenode = TimeCountDownNode::createWithNode(time, 3);
	_boxtimenode->setDur(time);
	//this->addChild(_boxtimenode);

	//_boxtimenode->startCountDown();

	
}
/**********************************************************************************************************************************************/

PlayersLayer::PlayersLayer():_wenPonNodePos(Vec2(298.f, -45.f)),
	_wenPonNodeStartPos(Vec2(-1.f, -230.f))
{
	
}

PlayersLayer::~PlayersLayer()
{
	WeaponControl::getInstance()->removeWeaponListener(this);
}

PlayersLayer* PlayersLayer::createPlayerLayer()
{
	auto pRef = new PlayersLayer();
	if (pRef->init())
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
}


bool PlayersLayer::init()
{
	Layout::init();
	
	_scrollPlayersPageView = glui::GLPageView::create();
	this->addChild(_scrollPlayersPageView);
	_scrollPlayersPageView->setContentSize(Size(VisibleRect::rightTop()));
	_scrollPlayersPageView->setTouchEnabled(false);
	_scrollPlayersPageView->setVisible(false);
	_scrollPlayersPageView->setCanScroll(false);
	_scrollPlayersPageView->setCustomScrollThreshold(150.f);

	auto offy = VisibleRect::top().y - 980.f;

	Vec2 pos[] = { Vec2(165.f, 800.f + offy), Vec2(480.f, 800.f + offy), Vec2(165.f, 390.f + offy/2.f), Vec2(480.f, 390.f + offy/2.f) };
	for (int i = 0; i < 4; i++)
	{
		auto node = GameCSLoader::createNode(String::createWithFormat("csb/player_%d.csb", i)->getCString());
		this->addChild(node);
		node->setPosition(pos[i]);
		_arrPlayers.pushBack(node);
		_arrStartPos[i] = node->getPosition();

		auto pArmInfo = ParamMgr::getInstance()->getRoleArmtrInfo(i);
		auto _pArmtr = GameArmtr::createRole(pArmInfo);
		node->addChild(_pArmtr);
		_pArmtr->setPosition(Vec2(-80.f, -110.f));
		_pArmtr->play(ArmtrName::ROLE_IDLE);
		_playerArm[i] = _pArmtr;
//		_pArmtr->setScale(0.8f);


		_vectorPlayerNode.push_back(node);

		auto layout = ui::Layout::create();
		auto tempnode = GameCSLoader::createNode(String::createWithFormat("csb/player_%d.csb", i)->getCString());
		auto wenpon = tempnode->getChildByName("wenpon_node");
		wenpon->setPosition(Vec2(298.f, -45.f));
		layout->addChild(tempnode);

		auto pArmInfo2 = ParamMgr::getInstance()->getRoleArmtrInfo(i);
		auto _pArmtr2 = GameArmtr::createRole(pArmInfo2);
		tempnode->addChild(_pArmtr2);
		_pArmtr2->setPosition(Vec2(-80.f, -110.f));
		_pArmtr2->play(ArmtrName::ROLE_IDLE);
		_playerArm2[i] = _pArmtr2;
		tempnode->setPosition(pos[0] + Vec2(0.f, 100.f));
		_scrollPlayersPageView->addPage(layout);
		_vectorPlayerNode.push_back(tempnode);
	}

	updataInfo();

	this->setName("playerlayer");
	WeaponControl::getInstance()->addWeaponListener(this, CC_CALLBACK_3(PlayersLayer::equipCallBack, this));
	return true;
}

void PlayersLayer::updataInfo()
{
	for (int i = 0; i < 4; i++)
	{
		updataPlayerInfo(i);
	}

	NewMapOpenMgr::getInstance()->tryGet(WENPON20_0, false);	
	NewMapOpenMgr::getInstance()->tryGet(WENPON20_1, false);
	NewMapOpenMgr::getInstance()->tryGet(WENPON20_2, false);
	NewMapOpenMgr::getInstance()->tryGet(WENPON20_3, false);

	NewMapOpenMgr::getInstance()->popGet();	
}

void PlayersLayer::updataPlayerInfo(int i)
{
	auto temp = _vectorPlayerNode[2 * i];
	auto temp2 = _vectorPlayerNode[2 * i + 1];

	auto wenponnode = temp->getChildByName("wenpon_node");
	auto wenponnode2 = temp2->getChildByName("wenpon_node");

	int lv = UserData::getInstance()->getPlayerCurLv(i);
	const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, lv);
	auto& wenpon = WeaponControl::getInstance()->getEquipWenpon(i);
	int posidx = WeaponControl::getInstance()->getEquipPosIdx(i);
	auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(i * 10 + posidx);

	CrushUtil::changeWeapon(_playerArm[i], i, wenpon.id);
	CrushUtil::changeWeapon(_playerArm2[i], i, wenpon.id);

	if (lv >= ParamMgr::getInstance()->getPlayerMaxLv())
	{
		temp->getChildByName("player_nextexp")->setVisible(false);
		temp2->getChildByName("player_nextexp")->setVisible(false);
	}

	if (i == 0)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
	else if (i == 1)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(wenponnode->getChildByName("defens_cur")))->setString(String::createWithFormat("%d", info.def)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("defens_next")))->setString(String::createWithFormat("%d", info.def + weaponinfo.def)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("hpadd_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("hpadd_next")))->setString(String::createWithFormat("%d", info.dpadd + weaponinfo.dpAdd)->getCString());

		((ui::TextAtlas*)(wenponnode2->getChildByName("defens_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("defens_next")))->setString(String::createWithFormat("%d", info.def + weaponinfo.def)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("hpadd_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("hpadd_next")))->setString(String::createWithFormat("%d", info.dpadd + weaponinfo.dpAdd)->getCString());
	}
	else if (i == 2)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
	else if (i == 3)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.hpadd)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.hpadd + weaponinfo.hpAdd)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.hpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.hpadd + weaponinfo.hpAdd)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
}

void PlayersLayer::showWenponInfo(int playerid, int wenponid)
{
	int i = playerid;
	auto temp = _vectorPlayerNode[2 * i];
	auto temp2 = _vectorPlayerNode[2 * i + 1];

	auto wenponnode = temp->getChildByName("wenpon_node");
	auto wenponnode2 = temp2->getChildByName("wenpon_node");

	int lv = UserData::getInstance()->getPlayerCurLv(i);
	const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, lv);
	auto& wenpon = ParamMgr::getInstance()->getWeaponByID(wenponid);

	int posidx = WeaponControl::getInstance()->getPosIdxByWeaponId(wenponid);
	auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(i * 10 + posidx);

	CrushUtil::changeWeapon(_playerArm[i], i, wenpon.id);
	CrushUtil::changeWeapon(_playerArm2[i], i, wenpon.id);

	if (lv >= ParamMgr::getInstance()->getPlayerMaxLv())
	{
		temp->getChildByName("player_nextexp")->setVisible(false);
		temp2->getChildByName("player_nextexp")->setVisible(false);
	}

	if (i == 0)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
	else if (i == 1)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(wenponnode->getChildByName("defens_cur")))->setString(String::createWithFormat("%d", info.def)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("defens_next")))->setString(String::createWithFormat("%d", info.def + weaponinfo.def)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("hpadd_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("hpadd_next")))->setString(String::createWithFormat("%d", info.dpadd + weaponinfo.dpAdd)->getCString());

		((ui::TextAtlas*)(wenponnode2->getChildByName("defens_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("defens_next")))->setString(String::createWithFormat("%d", info.dpadd + weaponinfo.def)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("hpadd_cur")))->setString(String::createWithFormat("%d", info.dpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("hpadd_next")))->setString(String::createWithFormat("%d", info.dpadd + weaponinfo.dpAdd)->getCString());
	}
	else if (i == 2)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.attack)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.attack + weaponinfo.attack)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
	else if (i == 3)
	{
		((ui::TextAtlas*)(temp->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(temp->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.hpadd)->getCString());
		((ui::TextAtlas*)(wenponnode->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.hpadd + weaponinfo.hpAdd)->getCString());
		((ui::LoadingBar*)(temp->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));

		((ui::TextAtlas*)(temp2->getChildByName("player_hp")))->setString(String::createWithFormat("%d", info.hp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_defens")))->setString(String::createWithFormat("%d", info.dp)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("text_lv")))->setString(String::createWithFormat("%d", lv)->getCString());
		((ui::TextAtlas*)(temp2->getChildByName("player_nextexp")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_cur")))->setString(String::createWithFormat("%d", info.hpadd)->getCString());
		((ui::TextAtlas*)(wenponnode2->getChildByName("attack_next")))->setString(String::createWithFormat("%d", info.hpadd + weaponinfo.hpAdd)->getCString());
		((ui::LoadingBar*)(temp2->getChildByName("expbar")))->setPercent(UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp);
		((Sprite*)(wenponnode2->getChildByName("weapon")))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(wenpon.picname));
	}
}

void PlayersLayer::equipCallBack(int i, int skillid, int action)
{
	updataPlayerInfo(i);

	if (i == 0)
	{
		NewMapOpenMgr::getInstance()->tryGet(WENPON20_0);
	}
	else if (i == 1)
	{
		NewMapOpenMgr::getInstance()->tryGet(WENPON20_1);
	}
	else if (i == 2)
	{
		NewMapOpenMgr::getInstance()->tryGet(WENPON20_2);
	}
	else if (i == 3)
	{
		NewMapOpenMgr::getInstance()->tryGet(WENPON20_3);
	}
}

void PlayersLayer::resetPos()
{
	for (int i = 0; i < 4; i++)
	{
		_arrPlayers.at(i)->setPosition(_arrStartPos[i]);
		auto wenpon = _arrPlayers.at(i)->getChildByName("wenpon_node");
		wenpon->setPosition(Vec2(-2.f, -230.f));
		_arrPlayers.at(i)->setVisible(true);
		_scrollPlayersPageView->setVisible(false);
	}
	_scrollPlayersPageView->setCurPageIndex(0);
}


void PlayersLayer::showChangeToOtherLayer(float dur)
{
	for (int i = 1; i < 4; i++)
	{
		auto temp = _arrPlayers.at(i)->getPosition() - _arrStartPos[0];
		auto action = MoveBy::create(dur, temp * 3.f);
		_arrPlayers.at(i)->runAction(Sequence::create(action, Hide::create(), nullptr));
	}
	_arrPlayers.at(0)->setPosition(_arrStartPos[0]);
	auto action = MoveTo::create(dur/2.f, _arrStartPos[0] + Vec2(0.f, 100.f));
	_arrPlayers.at(0)->runAction(Sequence::create(action, DelayTime::create(dur / 2.f), Hide::create(),CallFunc::create([this]() {
		_scrollPlayersPageView->setCurPageIndex(0);
		_scrollPlayersPageView->setVisible(true);
	}), nullptr));

	auto wenpon = _arrPlayers.at(0)->getChildByName("wenpon_node");
	wenpon->setPosition(Vec2(-2.f, -230.f));

	auto actionright = EaseSineOut::create(MoveBy::create(dur / 2.f, Vec2(300.f, 0.f)));
	auto actionup = EaseSineIn::create(MoveBy::create(dur / 2.f, Vec2(0.f, 185.f)));
	wenpon->runAction(Sequence::createWithTwoActions(actionright, actionup));
}

void PlayersLayer::showChangeBackTo(float dur, int idx)
{
	auto wenpon = _arrPlayers.at(idx)->getChildByName("wenpon_node");
	wenpon->setPosition(_wenPonNodePos);

	//auto actionright = EaseSineIn::create(MoveBy::create(dur / 2.f, Vec2(-300.f, 0.f)));
	auto actionright =MoveBy::create(dur / 2.f, Vec2(-300.f, 0.f));
	auto actionup = EaseSineOut::create(MoveBy::create(dur / 2.f, Vec2(0.f, -185.f)));
	wenpon->runAction(Sequence::createWithTwoActions(actionup, actionright));

	for (int i = 0; i < 4; i++)
	{
		if (idx == i)
		{
			_arrPlayers.at(idx)->setPosition(_arrStartPos[0] + Vec2(0.f, 100.f));
		}
		else
		{
			auto wenpon = _arrPlayers.at(i)->getChildByName("wenpon_node");
			wenpon->setPosition(_wenPonNodeStartPos);

			auto temp = _arrStartPos[i] - _arrStartPos[idx];
			auto pos = _arrStartPos[idx] + 3 * temp;
			_arrPlayers.at(i)->setPosition(pos);
		}

		auto action =MoveTo::create(dur / 2.f, _arrStartPos[i]);
		_arrPlayers.at(i)->runAction(Sequence::createWithTwoActions(DelayTime::create(dur/2.f), action));
		_arrPlayers.at(i)->setVisible(true);
	}
	_scrollPlayersPageView->setVisible(false);
	_scrollPlayersPageView->setCurPageIndex(0);
}

void PlayersLayer::setScoreShow(bool show)
{
	_scrollPlayersPageView->setVisible(show);
	for (auto temp : _arrPlayers)
	{
		temp->setVisible(!show);
	}
}


const char* g_iconbgname[] = { "wup_itembg_0.png", "wup_itembg_0.png", "wup_itembg_0.png", "wup_itembg_1.png", "wup_itembg_1.png", "wup_itembg_1.png", "wup_itembg_2.png", "wup_itembg_2.png", "wup_itembg_2.png", "wup_itembg_3.png" };


EquipLayer::~EquipLayer()
{
	WeaponControl::getInstance()->removeWeaponListener(this);
	_eventDispatcher->removeEventListenersForTarget(this);
	_eventDispatcher->removeCustomEventListeners("guide_end");
}

EquipLayer * EquipLayer::createLayer(int idx)
{
	auto pRef = new EquipLayer();
	if (pRef && pRef->initWithIdx(idx))
	{
		pRef->autorelease();
	}
	else
	{
		delete pRef;
		pRef = nullptr;
	}
	return pRef;
}

bool EquipLayer::initWithIdx(int idx)
{
	_idx = idx;

	if (0 == idx)
	{
		_eventDispatcher->addCustomEventListener("guide_end", CC_CALLBACK_1(EquipLayer::guideEvent, this));
	}

	this->setName(String::createWithFormat("weaponlayer_%d", idx)->getCString());

	_layer = GameCSLoader::createNode("csb/layer_0.csb");
	this->addChild(_layer);

	_selectMark = Sprite::createWithSpriteFrameName("wup_icon_select.png");
	_layer->addChild(_selectMark, 10);
	_selectMark->setVisible(false);

	_textDes = static_cast<ui::Text*>(_layer->getChildByName("text_des"));
	_textTitle = static_cast<ui::Text*>(_layer->getChildByName("text_title"));
	_textWenponRare = static_cast<ui::Text*>(_layer->getChildByName("text_wenpon_rare"));
	_textWenponLv = static_cast<ui::Text*>(_layer->getChildByName("text_wenpon_lv"));

	_btnNode = _layer->getChildByName("btn_node");
	_btnNode->setVisible(false);
	_btnok = static_cast<ui::Button*>(_btnNode->getChildByName("btn_ok"));
	_btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_selectIdx != -1)
			{
				int wenlv = ParamMgr::getInstance()->getWeaponByID(_arrWeaponid[_selectIdx]).equipMinLv;

				if (UserData::getInstance()->getPlayerCurLv(_idx) >= wenlv)
				{
					WeaponControl::getInstance()->equipWeapon(_idx, _arrWeaponid[_selectIdx]);
				}
				else
				{
					cocos2dx_plat::showToast(String::createWithFormat(ResMgr::getInstance()->getString("lv_limit")->getCString(), wenlv)->getCString());
				}
			}
		}
	});

	_btnWUP = GameButton::create("com_ok_bg.png", "", "", Widget::TextureResType::PLIST);
	_layer->addChild(_btnWUP, 2);
	_btnWUP->setPosition(_btnNode->getPosition());
	_btnWUP->addTouchEventListener(CC_CALLBACK_2(EquipLayer::menuOnWUP, this));

	_jiantou = Sprite::createWithSpriteFrameName("wup_jiantou_down.png");
	_btnWUP->addChild(_jiantou);
	_jiantou->setPosition(Vec2(_btnWUP->getContentSize().width / 2, _btnWUP->getContentSize().height / 2) + Vec2(0.f, 10.f));

	_tipText = Sprite::createWithSpriteFrameName("wup_text_hc.png");
	_btnWUP->addChild(_tipText);
	_tipText->setPosition(Vec2(_btnWUP->getContentSize().width / 2, _btnWUP->getContentSize().height / 2) + Vec2(0.f, -17.f));

	//jiantou action
	auto jiantouAction = Sequence::createWithTwoActions(MoveBy::create(0.5f, Vec2(0.f, 10.f)), MoveBy::create(0.5f, Vec2(0.f, -10.f)));
	_jiantou->runAction(RepeatForever::create(jiantouAction));

	_selectIdx = -1;

	auto pos_0 = _layer->getChildByName("btn_0")->getPosition();
	auto pos_5 = _layer->getChildByName("btn_5")->getPosition();

	for (int i = 0; i < 10; i++)
	{
		auto pos = i >= 5 ? pos_5 : pos_0;
		_arrPos[i] = pos + Vec2(128.f * (i % 5), 0.f);
		
		auto sprite = Sprite::createWithSpriteFrameName(g_iconbgname[i]);
		_layer->addChild(sprite, 1);
		sprite->setPosition(_arrPos[i]);

		_arrWeaponid[i] =_idx * 10 + 500 + i;

		auto infonode = Node::create();
		_layer->addChild(infonode, 3);

		_infoNode[i] = infonode;

		auto barbg = Sprite::createWithSpriteFrameName("wup_bar_bg.png");
		infonode->addChild(barbg, 3);
		barbg->setPosition(_arrPos[i] + Vec2(0.f, -37.f));

		_arrparsBar[i] = LoadingBar::create("wup_bar_0.png", Widget::TextureResType::PLIST, 50.f);
		infonode->addChild(_arrparsBar[i], 4);
		_arrparsBar[i]->setPosition(_arrPos[i] + Vec2(0.f, -37.f));

		_textparts[i] = Text::create();
		_textparts[i]->setString("30/60");
		infonode->addChild(_textparts[i], 5);
		_textparts[i]->setPosition(_arrPos[i] + Vec2(0.f, -37.f));
		_textparts[i]->setFontSize(14);

		_arrLvNode[i] = Node::create();
		infonode->addChild(_arrLvNode[i], 6);
		_arrLvNode[i]->setPosition(_arrPos[i] + Vec2(-10.f, 40.f));

		auto lvtext = Sprite::createWithSpriteFrameName("wup_text_lv.png");
		_arrLvNode[i]->addChild(lvtext);
		lvtext->setPositionX(-20.f);

		auto lablv = ui::TextAtlas::create(String::createWithFormat("%d", UserData::getInstance()->getWeaponLv(_idx * 10 + i))->getCString(), "fonts/font_wuplv.png", 13.f, 18.f, ".");
		_arrLvNode[i]->addChild(lablv);
		lablv->setAnchorPoint(Vec2(0.f, 0.5f));
		lablv->setPosition(Vec2(-5.f, 0.f));
		lablv->setName("lablv");

		if (checkHaveWenpon(i))
		{
			ComInfo_T item;
			ParamMgr::getInstance()->getComGameItemInfo(_arrWeaponid[i], item);
			auto sprite = Sprite::createWithSpriteFrameName(item.picname);
			_layer->addChild(sprite, 2);

			sprite->setPosition(_arrPos[i]);

			_arrWeaponIcon[i] = sprite;
		}
		else
		{
			_arrLvNode[i]->setVisible(false);

			ComInfo_T item;
			ParamMgr::getInstance()->getComGameItemInfo(_arrWeaponid[i], item);
			auto sprite = Sprite::createWithSpriteFrameName(item.picname);
			_layer->addChild(sprite, 2);
			sprite->setPosition(_arrPos[i]);

			_arrWeaponIcon[i] = sprite;
			//_arrWeaponIcon[i] = nullptr;
		}

		auto partsicon = Sprite::createWithSpriteFrameName("store_spicon.png");
		_layer->addChild(partsicon, 8);
		partsicon->setPosition(_arrPos[i] + Vec2(-25.f, 25.f) + Vec2(-15.f, 16.f) + Vec2(5.f, -5.f));
		partsicon->setName(String::createWithFormat("parts_sprite_%d", i)->getCString());

		_arrPartsIcon[i] = partsicon;

		if (checkHaveEquipWenpon(_arrWeaponid[i]))
		{
			_equipIcon = Sprite::createWithSpriteFrameName("skill_gou.png");
			_layer->addChild(_equipIcon, 7);
			_equipIcon->setPosition(_arrPos[i] + Vec2(35.f, -35.f));
			//auto sprite = Sprite::createWithSpriteFrameName("skill_gou.png");
			//_arrWeaponIcon[i]->addChild(sprite);
			//sprite->setPosition(Vec2(70.f, 0.f));
			//sprite->setName("equipicon");
		}
	}

	_attackNode[0] = _layer->getChildByName("attacknode_0");
	_attackNode[1] = _layer->getChildByName("attacknode_1");
	_attackNode[2] = _layer->getChildByName("attacknode_2");
	_attackNode[0]->setVisible(false);
	_attackNode[1]->setVisible(false);
	_attackNode[2]->setVisible(false);
	_textDes->setString("");
	_textTitle->setString("");

	//gold show node
	_wupGoldNode = Node::create();
	_layer->addChild(_wupGoldNode);
	_wupGoldNode->setPosition(_btnWUP->getPosition() + Vec2(-40.f, 75.f));

	auto bg = Sprite::createWithSpriteFrameName("com_wup_goldbg.png");
	_wupGoldNode->addChild(bg);

	auto text = Sprite::createWithSpriteFrameName("wup_text_hf.png");
	_wupGoldNode->addChild(text);
	text->setPositionX(-50.f);

	auto textnum = Text::create();
	textnum->setName("goldnum");
	_wupGoldNode->addChild(textnum);
	textnum->setString("1000");
	textnum->setFontSize(24);

	auto icon = Sprite::createWithSpriteFrameName("com_goldbag.png");
	_wupGoldNode->addChild(icon);
	icon->setPositionX(50.f);
	icon->setScale(0.5f);
	

	WeaponControl::getInstance()->addWeaponListener(this, CC_CALLBACK_3(EquipLayer::equipCallBack, this));

	for (int i = 0; i < 10; i++)
	{
		updateWenponInfo(i);
	}
	updateWUPBtn();

	return true;
}

void EquipLayer::onEnter()
{
	Layer::onEnter();
	auto offy = VisibleRect::top().y / 2 - 350.f;
	_startPos0 = _layer->getPosition()+ Vec2(64.f, 435.f + offy) + Vec2(-64.f, -63.f);
	_startPos1 = _layer->getPosition() + Vec2(64.f, 307.f + offy) + Vec2(-64.f, -63.f);
}

bool EquipLayer::checkHaveWenpon(int id)
{
	int weaponid = _arrWeaponid[id];
	return WeaponControl::getInstance()->isWeaponUnLock(weaponid);
}

bool EquipLayer::checkHaveEquipWenpon(int skillid)
{
	return WeaponControl::getInstance()->checkIsEquipWeapon(_idx, skillid);
}

bool EquipLayer::checkCanHeCheng(int posidx)
{
	auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + posidx);

	if (curparts > 0)
	{
		return true;
	}

	return false;
}

int EquipLayer::checkTouchBtn(cocos2d::Vec2 pos)
{
	if (pos.y >= _startPos1.y && pos.y <= _startPos1.y + 126.f)
	{
		
		return 5 + pos.x / 128.f;
		
	}
	else if (pos.y >= _startPos0.y && pos.y <= _startPos0.y + 126.f)
	{
		
		return pos.x / 128.f;	
	}
	return -1;
}

void EquipLayer::touchBegan(Vec2 pos)
{
	int touchidx = checkTouchBtn(pos);
	
	if (touchidx != -1 && (checkCanHeCheng(touchidx) || checkHaveWenpon(touchidx)))
	{
		if (0 == _idx && 0 == touchidx)
		{
			if (nullptr != GameGuideLayer::getInstance())
			{
				GameGuideLayer::getInstance()->guideStepEnd();
				this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {GameGuideLayer::getInstance()->guideStepStart(1, _btnWUP->convertToWorldSpace(Vec2(47.f, 47.f))); })));
			}
		}

		if (touchidx != _selectIdx)
		{
			auto& info = ParamMgr::getInstance()->getWeaponByID(_arrWeaponid[touchidx]);
			_textTitle->setString(info.name);
			_textDes->setString(info.des);
			
			_textWenponLv->setString(String::createWithFormat("LV.%d", UserData::getInstance()->getWeaponLv(_idx * 10 + touchidx))->getCString());
			_textWenponRare->setString(ResMgr::getInstance()->getString(String::createWithFormat("wenpon_lv_pos_%d", touchidx)->getCString())->getCString());

			_textWenponLv->setPositionX(_textTitle->getPositionX() + _textTitle->getContentSize().width + 10.f);

			auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(_idx * 10 + touchidx);

			_selectMark->setLocalZOrder(3);
			_selectMark->setVisible(true);
			_selectMark->setPosition(_arrPos[touchidx]);

			if (checkHaveEquipWenpon(_arrWeaponid[touchidx]))
			{
				_btnNode->setVisible(false);
				_textWenponLv->setVisible(true);
			}
			else if(checkHaveWenpon(touchidx))
			{
				_btnNode->setVisible(true);
				_textWenponLv->setVisible(true);
			}
			else if (checkHaveWenpon(touchidx) == false)
			{
				_btnNode->setVisible(false);
				_textWenponLv->setVisible(false);
			}

			if (_idx == 1)
			{
				((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
				((ui::TextAtlas*)(_attackNode[1]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.def)->getCString());
				((ui::TextAtlas*)(_attackNode[2]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.dpAdd)->getCString());
			}
			else if (_idx == 3)
			{
				((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.hpAdd)->getCString());
				static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_10.png"));
			}
			else if (_idx == 0)
			{
				((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
				static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_3.png"));
			}
			else
			{			
				((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
				static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_2.png"));
			}

			_attackNode[0]->setVisible(true);
			_attackNode[1]->setVisible(false);
			_attackNode[2]->setVisible(false);
			if (_idx == 1)
			{
				_attackNode[1]->setVisible(true);
				_attackNode[2]->setVisible(true);
			}
			_selectIdx = touchidx;

			MainLayer::getCurMainLayer()->getCurPlayersLayer()->showWenponInfo(_idx, _arrWeaponid[_selectIdx]);
		}

		updateWUPBtn();
	}
	else
	{
		//if (touchidx != _selectIdx)
		//{
			//MainLayer::getCurMainLayer()->getCurPlayersLayer()->updataPlayerInfo(_idx);
		//}	
	}
}

void EquipLayer::updataWenponDes(int posidx)
{
	auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(_idx * 10 + posidx);
	if (_idx == 1)
	{
		((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
		((ui::TextAtlas*)(_attackNode[1]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.def)->getCString());
		((ui::TextAtlas*)(_attackNode[2]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.dpAdd)->getCString());
	}
	else if (_idx == 3)
	{
		((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.hpAdd)->getCString());
		static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_10.png"));
	}
	else if (_idx == 0)
	{
		((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
		static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_3.png"));
	}
	else
	{
		((ui::TextAtlas*)(_attackNode[0]->getChildByName("attack_next")))->setString(String::createWithFormat("%d", weaponinfo.attack)->getCString());
		static_cast<Sprite*>(_attackNode[0]->getChildByName("mainui_shuxinicon_0_24"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_2.png"));
	}
}

void EquipLayer::equipCallBack(int playeridx, int skillid, int action)
{
	if (playeridx == _idx)
	{
		for (int i = 0; i < 10; i++)
		{
			if (_arrWeaponid[i] == skillid)
			{
				if (action == 1)
				{
					_equipIcon = Sprite::createWithSpriteFrameName("skill_gou.png");
					_layer->addChild(_equipIcon, 7);
					_equipIcon->setPosition(_arrPos[i] + Vec2(35.f, -35.f));
					/*auto sprite = Sprite::createWithSpriteFrameName("skill_gou.png");
					_arrWeaponIcon[i]->addChild(sprite);
					sprite->setName("equipicon");
					sprite->setPosition(Vec2(70.f, 0.f));*/
					_btnNode->setVisible(false);

					auto pArm = MainLayer::getCurMainLayer()->getCurPlayersLayer()->getPlayerArm(playeridx);
					CrushUtil::changeWeapon(pArm, playeridx, skillid);

					pArm = MainLayer::getCurMainLayer()->getCurPlayersLayer()->getPlayerArm2(playeridx);
					CrushUtil::changeWeapon(pArm, playeridx, skillid);

					Player::getInstance()->changeWeapon(playeridx, skillid);
				}
				else if (action == 0)
				{
					_equipIcon->removeFromParent();
					_equipIcon = nullptr;
				}
				else if (action == 2)
				{
					if (_arrWeaponIcon[i]  == nullptr)
					{
						auto& item = ParamMgr::getInstance()->getWeaponByID(_arrWeaponid[i]);
						auto sprite = Sprite::createWithSpriteFrameName(item.picname);
						_layer->addChild(sprite, 2);
						sprite->setPosition(_arrPos[i]);
						_arrWeaponIcon[i] = sprite;
					}

					updateWenponInfo(i);
				}
				break;
			}
		}
	}
}


void EquipLayer::freshenUpadtaLayer()
{
	Node* layer = nullptr;
	if (_layercolor)
	{
		layer = _layercolor->getChildByName("wuplayer");
	}

	if (layer == nullptr)
	{
		return;
	}

	int type = WeaponControl::getInstance()->getWenponType(_selectIdx);
	auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
	auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + _selectIdx);
	auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx);

	int lvidx = curlv >= info.size() ? (int)info.size() - 1 : curlv;

	ComInfo_T item;
	ParamMgr::getInstance()->getComGameItemInfo(_arrWeaponid[_selectIdx], item);

	//icon bg 
	auto iconbg = static_cast<Sprite*>(layer->getChildByName("wup_itembg_0"));
	iconbg->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_iconbgname[_selectIdx]));

	//lv in icon
	auto lvInIcon = static_cast<TextAtlas*>(iconbg->getChildByName("lv_num"));
	lvInIcon->setString(String::createWithFormat("%d", curlv)->getCString());

	//icon wenpon
	auto iconWeapon = static_cast<Sprite*>(iconbg->getChildByName("weapon"));
	iconWeapon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(item.picname));

	//partsnum
	TextAtlas* parts = static_cast<TextAtlas*>(iconbg->getChildByName("num"));
	parts->setString(String::createWithFormat("%d/%d", curparts, info[lvidx].costparts)->getCString());

	//loadbar
	auto bar = static_cast<LoadingBar*>(iconbg->getChildByName("wup_bar_0"));
	bar->setPercent(curparts / info[lvidx].costparts * 100);

	if (curparts >= info[lvidx].costparts)
	{
		bar->loadTexture("wup_bar_1.png", Widget::TextureResType::PLIST);
	}
	else
	{
		bar->loadTexture("wup_bar_0.png", Widget::TextureResType::PLIST);
	}

	auto root = layer->getChildByName("panel_propety");
	int weaponInfo[4];
	int weaponMinInfo[4];
	int weaponMaxInfo[4];
	auto& curInfo = UserData::getInstance()->getWeaponAttack(_idx * 10 + _selectIdx);

	weaponInfo[0] = curInfo.attack;
	weaponInfo[1] = curInfo.def;
	weaponInfo[2] = curInfo.hpAdd;
	weaponInfo[3] = curInfo.dpAdd;

	auto& weapon = (ParamMgr::getInstance()->getWeaponVector())[_idx * 10 + _selectIdx];

	weaponMinInfo[0] = curInfo.attack + weapon.lvMinAttack;
	weaponMinInfo[1] = curInfo.def + weapon.lvMinDef;
	weaponMinInfo[2] = curInfo.hpAdd + weapon.lvMinHpadd;
	weaponMinInfo[3] = curInfo.dpAdd + weapon.lvMinDpadd;

	weaponMaxInfo[0] = curInfo.attack + weapon.lvMaxAttack;
	weaponMaxInfo[1] = curInfo.def + weapon.lvMaxDef;
	weaponMaxInfo[2] = curInfo.hpAdd + weapon.lvMaxHpadd;
	weaponMaxInfo[3] = curInfo.dpAdd + weapon.lvMaxDpadd;

	//textcost
	auto textcost = static_cast<Text*>(layer->getChildByName("cost_num"));
	textcost->setString(String::createWithFormat("%d", info[lvidx].costnum)->getCString());

	for (int i = 0; i < 6; i++)
	{
		auto itemroot = root->getChildByName(String::createWithFormat("panel_property_%d", i)->getCString());

		if (i == 0)
		{
			auto lv = static_cast<TextAtlas*>(itemroot->getChildByName("num_0"));
			auto lvto = static_cast<TextAtlas*>(itemroot->getChildByName("num_1"));

			lv->setString(String::createWithFormat("%d", curlv)->getCString());
			lvto->setString(String::createWithFormat("%d", curlv + 1)->getCString());
		}
		else if (i == 5)
		{
			auto test = static_cast<Text*>(itemroot->getChildByName("chance"));
			int type = WeaponControl::getInstance()->getWenponType(_selectIdx);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
			auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + _selectIdx);
			auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx);

			int lvidx = curlv >= info.size() ? (int)info.size() - 1 : curlv;
			auto p = info[lvidx].upp;
			p = p > 1.0f ? 1.0f : p;

			int pnameidx = 0;
			float parr[] = {1.f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f, 0.01f};
			for (int i = 0; i < sizeof(parr) / sizeof(parr[0]); i++)
			{
				if (p >= parr[i])
				{
					pnameidx = i;
					break;
				}
			}

			test->setString(ResMgr::getInstance()->getString(String::createWithFormat("p_name_%d", pnameidx)->getCString())->getCString());
		}
		else
		{
			auto cur = static_cast<TextAtlas*>(itemroot->getChildByName("num_0"));
			auto nextmin = static_cast<TextAtlas*>(itemroot->getChildByName("num_1"));
			auto nextman = static_cast<TextAtlas*>(itemroot->getChildByName("num_2"));

			cur->setString(String::createWithFormat("%d", weaponInfo[i - 1])->getCString());
			nextmin->setString(String::createWithFormat("%d", weaponMinInfo[i - 1])->getCString());
			nextman->setString(String::createWithFormat("%d", weaponMaxInfo[i - 1])->getCString());
		}
	}
}

void EquipLayer::guideEvent(EventCustom * evt)
{
	if (nullptr != GameGuideLayer::getInstance())
	{
		int guideType = GameGuideLayer::getInstance()->getGuideType();
		int step = GameGuideLayer::getInstance()->getGuideStep();

		if (1 == guideType && 3 == step)
		{
			Vec2 pos = this->convertToWorldSpace(_arrPos[0]);
			GameGuideLayer::getInstance()->guideStepStart(1, pos);
		}
	}
}


void EquipLayer::showUpdataWeapon()
{
	_layercolor = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	MainLayer::getCurMainLayer()->addChild(_layercolor, MainLayer_Z::POP_Z);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch * touch, Event * unusedEvent) {
		return true;
	};

	listener->onTouchEnded = [=](Touch * touch, Event * unusedEvent) {};


	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _layercolor);

	auto layer = GameCSLoader::createNode("csb/wupui.csb");	
	layer->setPositionY(VisibleRect::top().y * 0.5f - 480.f);
	_layercolor->addChild(layer);
	layer->setName("wuplayer");

	//btnclose
	auto close = static_cast<Button*>(layer->getChildByName("bt_close"));
	close->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (0 == _idx)
			{
				if (nullptr != GameGuideLayer::getInstance())
				{
					GameGuideLayer::getInstance()->guideStepEnd();
				}
			}
			_layercolor->removeFromParent();
		}
	});

	//btnwup
	auto btnwup = static_cast<Button*>(layer->getChildByName("bt_lv_up"));
	btnwup->addTouchEventListener([=](Ref*, Widget::TouchEventType ttype) {
		if (ttype == Widget::TouchEventType::ENDED)
		{
			int type = WeaponControl::getInstance()->getWenponType(_selectIdx);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
			auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + _selectIdx);
			auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx);

			int lvidx = curlv >= info.size() ? (int)info.size() - 1 : curlv;

			bool isenoughGold = false;
			bool isenoughParts = false;

			if (UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx) >= info[lvidx].costparts)
			{
				isenoughParts = true;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_parts")->getCString());
			}

			if (nullptr != GameGuideLayer::getInstance())
			{
				UserData::getInstance()->giveGold(info[lvidx].costnum);
			}

			if (UserData::getInstance()->getGoldNum() >= info[lvidx].costnum)
			{
				isenoughGold = true;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_gold")->getCString());
			}		
			

			if (isenoughGold && isenoughParts)
			{
				if (0 == _idx)
				{
					if (nullptr != GameGuideLayer::getInstance())
					{
						GameGuideLayer::getInstance()->guideStepEnd();
						GameGuideLayer::getInstance()->guideStepStart(1, close->convertToWorldSpace(Vec2(35.f, 29.f)));
					}
				}

				bool succeed = false;

				if (ToolsUtil::getRandomFloat(0.f, 1.f) <= info[lvidx].upp)
				{
					succeed = true;
					cocos2dx_plat::showToast(ResMgr::getInstance()->getString("up_succeed")->getCString());
				}
				else
				{
					cocos2dx_plat::showToast(ResMgr::getInstance()->getString("up_failed")->getCString());
				}

				UserData::getInstance()->giveGold(-info[lvidx].costnum);
				UserData::getInstance()->setWeaponPartsNum(_idx * 10 + _selectIdx, curparts - info[lvidx].costparts);

				if (succeed)
				{
					ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/wuqishengji.ExportJson");
					auto pArmtr = Armature::create("wuqishengji");
					layer->addChild(pArmtr);
					pArmtr->getAnimation()->play("hecheng");
					pArmtr->setPosition(Vec2(320.f, 767.f));

					auto actionremove = Sequence::createWithTwoActions(DelayTime::create(2.0f), RemoveSelf::create());
					pArmtr->runAction(actionremove);

					UserData::getInstance()->setWeaponLv(_idx * 10 + _selectIdx, curlv + 1);

					auto& weapon = (ParamMgr::getInstance()->getWeaponVector())[_idx * 10 + _selectIdx];

					int weaponMinInfo[4];
					int weaponMaxInfo[4];

					auto& curInfo = UserData::getInstance()->getWeaponAttack(_idx * 10 + _selectIdx);

					weaponMinInfo[0] = curInfo.attack + weapon.lvMinAttack;
					weaponMinInfo[1] = curInfo.def + weapon.lvMinDef;
					weaponMinInfo[2] = curInfo.hpAdd + weapon.lvMinHpadd;
					weaponMinInfo[3] = curInfo.dpAdd + weapon.lvMinDpadd;

					weaponMaxInfo[0] = curInfo.attack + weapon.lvMaxAttack;
					weaponMaxInfo[1] = curInfo.def + weapon.lvMaxDef;
					weaponMaxInfo[2] = curInfo.hpAdd + weapon.lvMaxHpadd;
					weaponMaxInfo[3] = curInfo.dpAdd + weapon.lvMaxDpadd;

					AttactInfo_T curattackadd;
					curattackadd.attack = ToolsUtil::getRandomInt(weaponMinInfo[0], weaponMaxInfo[0]);
					curattackadd.def = ToolsUtil::getRandomInt(weaponMinInfo[1], weaponMaxInfo[1]);
					curattackadd.hpAdd = ToolsUtil::getRandomInt(weaponMinInfo[2], weaponMaxInfo[2]);
					curattackadd.dpAdd = ToolsUtil::getRandomInt(weaponMinInfo[3], weaponMaxInfo[3]);

					UserData::getInstance()->setWeaponAttack(_idx * 10 + _selectIdx, curattackadd);

					AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::FORGE, 1);
					MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::WEAPON, 1);
				}

				UserData::getInstance()->saveUserData();

				freshenUpadtaLayer();

				updateWenponInfo(_selectIdx);
				updataWenponDes(_selectIdx);
				
				MainLayer::getCurMainLayer()->getCurPlayersLayer()->showWenponInfo(_idx, _arrWeaponid[_selectIdx]);

				DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYWENPONUP);
			}
			else
			{
				MainLayer::getCurMainLayer()->getCurDisplayEquipLayer()->updateWenponInfo(_selectIdx);
			}
		}	
	});


	if (0 == _idx)
	{
		if (nullptr != GameGuideLayer::getInstance())
		{
			GameGuideLayer::getInstance()->guideStepStart(1, btnwup->convertToWorldSpace(Vec2(114, 22)));
		}
	}


	freshenUpadtaLayer();
}

void EquipLayer::menuOnWUP(Ref* ref, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (checkHaveWenpon(_selectIdx))
		{
			if (0 == _idx)
			{
				if (nullptr != GameGuideLayer::getInstance())
				{
					GameGuideLayer::getInstance()->guideStepEnd();
				}
			}
			showUpdataWeapon();

		}
		else
		{
			int type = WeaponControl::getInstance()->getWenponType(_selectIdx);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
			auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + _selectIdx);
			auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx);

			int lvidx = curlv >= info.size() ? (int)(info.size() - 1) : curlv;

			if (UserData::getInstance()->getGoldNum() <  info[lvidx].costnum)
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_gold")->getCString());
				return;
			}

			UserData::getInstance()->setWeaponPartsNum(_idx * 10 + _selectIdx, curparts - info[lvidx].costparts);
			UserData::getInstance()->giveGold(-info[lvidx].costnum);
			UserData::getInstance()->saveUserData();

			UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", _arrWeaponid[ _selectIdx])->getCString(), 1);
			WeaponControl::getInstance()->addNewWeapon(_arrWeaponid[_selectIdx]);
		
			for (int i = 0; i < 10; i++)
			{
				updateWenponInfo(i);
			}

			ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/wuqishengji.ExportJson");
			auto pArmtr = Armature::create("wuqishengji");
			_layer->addChild(pArmtr);
			pArmtr->getAnimation()->play("hecheng");
			pArmtr->setPosition(_arrPos[_selectIdx]);

			auto actionremove = Sequence::createWithTwoActions(DelayTime::create(2.0f), RemoveSelf::create());
			pArmtr->runAction(actionremove);

			updateWUPBtn();

			_btnNode->setVisible(true);		
		}
		
	}
}

void EquipLayer::updateWenponInfo(int idx)
{
	int type = WeaponControl::getInstance()->getWenponType(idx);
	auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
	auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + idx);
	auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + idx);

	int lvidx = curlv >= info.size() ?(int) info.size() - 1 : curlv;
	
	_textparts[idx]->setString(String::createWithFormat("%d/%d", curparts, info[lvidx].costparts)->getCString());
	
	_arrparsBar[idx]->setPercent(float(curparts) / info[lvidx].costparts * 100);

	if (curparts >= info[lvidx].costparts)
	{
		_arrparsBar[idx]->loadTexture("wup_bar_1.png", Widget::TextureResType::PLIST);

		if (_layer->getChildByName(String::createWithFormat("point_%d", idx)->getCString()) == nullptr)
		{
			auto point = Sprite::createWithSpriteFrameName("box_red_bg.png");
			_layer->addChild(point, 9);
			point->setName(String::createWithFormat("point_%d", idx)->getCString());
			point->setPosition(_arrPos[idx] + Vec2(45.f, 45.f));
			point->setScale(0.7f);
		}
	}
	else
	{
		auto node = _layer->getChildByName(String::createWithFormat("point_%d", idx)->getCString());
		if (node)
		{
			node->removeFromParent();
		}

		_arrparsBar[idx]->loadTexture("wup_bar_0.png", Widget::TextureResType::PLIST);
	}

	if (checkHaveWenpon(idx))
	{
		_arrLvNode[idx]->setVisible(true);
		_arrPartsIcon[idx]->setVisible(false);
		_arrWeaponIcon[idx]->setVisible(true);
		_infoNode[idx]->setVisible(true);
		static_cast<TextAtlas*>(_arrLvNode[idx]->getChildByName("lablv"))->setString(String::createWithFormat("%d", UserData::getInstance()->getWeaponLv(_idx * 10 + idx))->getCString());
	}
	else if (curparts > 0)
	{
		_infoNode[idx]->setVisible(true);
		_arrPartsIcon[idx]->setVisible(true);
		_arrWeaponIcon[idx]->setVisible(true);
	}
	else
	{
		_arrPartsIcon[idx]->setVisible(false);
		_arrWeaponIcon[idx]->setVisible(false);
		_infoNode[idx]->setVisible(false);
	}
}

void EquipLayer::updateWUPBtn()
{
	if (_selectIdx != -1)
	{
		int type = WeaponControl::getInstance()->getWenponType(_selectIdx);
		auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);
		auto curlv = UserData::getInstance()->getWeaponLv(_idx * 10 + _selectIdx);
		auto curparts = UserData::getInstance()->getWeaponParts(_idx * 10 + _selectIdx);

		int lvidx = curlv >= info.size() ? (int)info.size() - 1 : curlv;

		if (checkHaveWenpon(_selectIdx))
		{
			_wupGoldNode->setVisible(false);
			if (curparts >= info[lvidx].costparts)
			{
				_btnWUP->setVisible(true);
				_btnWUP->setPosition(_btnNode->getPosition() + Vec2(-90.f, 0.f));
				_jiantou->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_jiantou_up.png"));
				_tipText->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_text_sj0.png"));
			}
			else
			{
				_btnWUP->setVisible(true);
				_btnWUP->setPosition(_btnNode->getPosition() + Vec2(-90.f, 0.f));
				_jiantou->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_jiantou_up.png"));
				_tipText->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_text_sj0.png"));
			}
		}
		else
		{
			if (curparts >= info[lvidx].costparts)
			{
				_btnWUP->setVisible(true);
				_btnWUP->setPosition(_btnNode->getPosition());
				_jiantou->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_jiantou_down.png"));
				_tipText->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("wup_text_hc.png"));

				_wupGoldNode->setVisible(true);
				static_cast<Text*>(_wupGoldNode->getChildByName("goldnum"))->setString(String::createWithFormat("%d", info[lvidx].costnum)->getCString());
			}
			else
			{
				_btnWUP->setVisible(false);
				_wupGoldNode->setVisible(false);
			}
		}
	}
	else
	{
		_btnWUP->setVisible(false);
		_wupGoldNode->setVisible(false);
	}
}

SkillLayer::~SkillLayer()
{
	SkillControl::getInstance()->removeSkillListener(this);
}

SkillLayer * SkillLayer::createLayer(int idx)
{
	auto pRef = new SkillLayer();
	if (pRef && pRef->initWithIdx(idx))
	{
		pRef->autorelease();
	}
	else
	{
		delete pRef;
		pRef = nullptr;
	}
	return pRef;
}

bool SkillLayer::initWithIdx(int idx)
{
	_idx = idx;

	this->setName(String::createWithFormat("skilllayer_%d", idx)->getCString());

	_layer = GameCSLoader::createNode("csb/layer_1.csb");
	this->addChild(_layer);

	_selectMark = Sprite::createWithSpriteFrameName("skill_select.png");
	_layer->addChild(_selectMark, 3);
	_selectMark->setVisible(false);

	_textDes = static_cast<ui::Text*>(_layer->getChildByName("text_des"));
	_textTitle = static_cast<ui::Text*>(_layer->getChildByName("text_title"));
	_textDes->setString("");
	_textTitle->setString("");

	_btnNode = _layer->getChildByName("btn_node");
	_btnNode->setVisible(false);
	_btnok = static_cast<ui::Button*>(_btnNode->getChildByName("btn_ok"));
	_btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_selectIdx != -1)
			{
				SkillControl::getInstance()->equipSkill(_idx, SkillControl::getInstance()->_arrSkillId[_idx * 12 + _selectIdx]);
			}
		}
	});

	_selectIdx = -1;

	auto pos_0 = _layer->getChildByName("btn_0")->getPosition();
	auto pos_6 = _layer->getChildByName("btn_6")->getPosition();

	for (int i = 0; i < 12; i++)
	{
		auto pos = i > 5 ? pos_6 : pos_0;
		_arrPos[i] = pos + Vec2(103 * (i % 6), 0.f);

		auto spritebg = Sprite::createWithSpriteFrameName("mainui_skill_itembg_0.png");
		_layer->addChild(spritebg, 1);
		spritebg->setPosition(_arrPos[i]);

		if (SkillControl::getInstance()->_arrSkillId[_idx * 12 + i] != -1)
		{
			ComInfo_T item;
			ParamMgr::getInstance()->getComGameItemInfo(SkillControl::getInstance()->_arrSkillId[_idx * 12 + i], item);
			auto sprite = Sprite::createWithSpriteFrameName(item.picname);
			_layer->addChild(sprite, 2);

			sprite->setPosition(_arrPos[i]);

			_arrSkillIcon[i] = sprite;

			if (checkHaveEquipSkill(SkillControl::getInstance()->_arrSkillId[_idx * 12 + i]))
			{
				auto spritegou = Sprite::createWithSpriteFrameName("skill_gou.png");
				sprite->addChild(spritegou);
				spritegou->setPosition(Vec2(60.f, 0.f));
				spritegou->setName("equipicon");
			}
		}
		else
		{		
			_arrSkillIcon[i] = nullptr;
		}
	}
	
	SkillControl::getInstance()->addSkillListener(this, CC_CALLBACK_3(SkillLayer::skillCallBack, this));

	return true;
}

void SkillLayer::onEnter()
{
	Layer::onEnter();
	auto offy = VisibleRect::top().y / 2 - 350.f;

	_startPos0 = _layer->getChildByName("btn_0")->getPosition() + _layer->getPosition() + Vec2(-51.5f, -51.5f + offy);
	_startPos1 = _layer->getChildByName("btn_6")->getPosition() + _layer->getPosition() + Vec2(-51.5f, -51.5f + offy);
}

int SkillLayer::checkTouchBtn(cocos2d::Vec2 pos)
{
	if (pos.y >= _startPos1.y && pos.y <= _startPos1.y + 103.f)
	{
		if (pos.x >= 8.f && pos.x <= 632.f)
		{
			return 6 + (pos.x - 8.f) / 103.f;
		}
	}
	else if (pos.y >= _startPos0.y && pos.y <= _startPos0.y + 103.f)
	{
		if (pos.x >= 8.f && pos.x <= 632.f)
		{
			return (pos.x - 8.f) / 103.f;
		}
	}
	return -1;
}

void SkillLayer::touchBegan(Vec2 pos)
{
	int touchidx = checkTouchBtn(pos);
	if (touchidx != -1)
	{
		if (touchidx != _selectIdx && checkHaveSkill(touchidx))
		{
			ComInfo_T info;
			ParamMgr::getInstance()->getComGameItemInfo(SkillControl::getInstance()->_arrSkillId[_idx * 12 + touchidx], info);
			_textTitle->setString(info.name);
			_textDes->setString(info.des);

			_selectMark->setVisible(true);
			_selectMark->setPosition(_arrPos[touchidx]);

			if (checkHaveEquipSkill(SkillControl::getInstance()->_arrSkillId[_idx * 12 + touchidx]))
			{
				_btnNode->setVisible(false);
			}
			else
			{
				_btnNode->setVisible(true);
			}

			_selectIdx = touchidx;	
		}
	}
}

bool SkillLayer::checkHaveSkill(int skillposid)
{
	int skillid = SkillControl::getInstance()->_arrSkillId[skillposid  + _idx * 12];
	if (skillid != -1)
	{
		return true;
	}

	return false;
}

bool SkillLayer::checkHaveEquipSkill(int skillid)
{
	return SkillControl::getInstance()->checkIsEquipSkill(_idx, skillid);
}

void SkillLayer::skillCallBack(int playeridx, int skillid, int isEquipOn)
{
	if (playeridx == _idx)
	{
		if (isEquipOn == 2)
		{
			auto pos = SkillControl::getInstance()->getSkillPosByPlayerIdx(playeridx, skillid);
			auto item = ParamMgr::getInstance()->getSkillByID(skillid);
			
			if (_arrSkillIcon[pos])
			{
				_arrSkillIcon[pos]->removeFromParent();
			}

			auto sprite = Sprite::createWithSpriteFrameName(item.icon);
			_layer->addChild(sprite, 2);
			sprite->setPosition(_arrPos[pos]);
			_arrSkillIcon[pos] = sprite;

		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				if (SkillControl::getInstance()->_arrSkillId[_idx * 12 + i] == skillid)
				{
					if (isEquipOn == 1)
					{
						auto sprite = Sprite::createWithSpriteFrameName("skill_gou.png");
						_arrSkillIcon[i]->addChild(sprite);
						sprite->setPosition(Vec2(60.f, 0.f));
						sprite->setName("equipicon");
						_btnNode->setVisible(false);

						CrushUtil::loadSkillImage(skillid);
					}
					else if (isEquipOn == 0)
					{
						_arrSkillIcon[i]->getChildByName("equipicon")->removeFromParent();

						CrushUtil::releaseSkillImage(skillid);
					}
					break;
				}
			}
		}
	}
}


BagLayer::BagLayer():_touchItemsIdx(-1),
	_isMove(false),
	_itemsStartPos(Vec2(0.f, 0.f)),
	_touchOneByOneListener(nullptr),
	_textTitle(nullptr),
	_textDes(nullptr),
	_selectMark(nullptr),
	_curpageidx(0),
	_itemStartId(0)
{
	memset(_flagBag, 0, sizeof(_flagBag));
	memset(_vectorPlayerItems, 0, sizeof(_vectorPlayerItems));
}

BagLayer::~BagLayer()
{
	BagItemControl::getInstace()->_callback = nullptr;
	BagItemControl::getInstace()->_posCallback = nullptr;
}

BagLayer * BagLayer::createLayer()
{
	auto pRef = new BagLayer();
	if (pRef && pRef->init())
	{
		pRef->autorelease();
	}
	else
	{
		delete pRef;
		pRef = nullptr;
	}
	return pRef;
}

bool BagLayer::init()
{
//	SoundsMgr::getInstance()->setSoundFilePath("sounds");
	GameUtils::playBackgroundMusic("daiji.mp3", true);

	_rootLayer = GameCSLoader::createNode("csb/layer_3.csb");
	this->addChild(_rootLayer);

	_textTitle = static_cast<ui::Text*>(_rootLayer->getChildByName("text_title"));
	_textDes = static_cast<ui::Text*>(_rootLayer->getChildByName("text_des"));
	_textTitle->setString("");
	_textDes->setString("");
	_textDes->setTextColor(Color4B(0, 0, 0, 255));

	auto posmark = Vec2(52.f, 540.f);

	auto layout = ui::Layout::create();
	_rootLayer->addChild(layout);
	layout->setContentSize(Size(640.f, VisibleRect::top().y));
	layout->setClippingEnabled(true);

	for (int j = 0; j < g_pagecount; j++)
	{
		_pageNode[j] = Node::create();
		layout->addChild(_pageNode[j], 1);
		_pageNode[j]->setPositionX(j * 640.f);

		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto truepos = Vec2(posmark.x + (i % 7) * 90.f, posmark.y + 90.f - 90.f * (i / 7));
			_itemsPos[i] = truepos;

			auto spritbg = Sprite::createWithSpriteFrameName("mainui_bag_combg_.png");
			_pageNode[j]->addChild(spritbg, 1);
			spritbg->setPosition(truepos);
		}
	}


	_selectMark = Sprite::createWithSpriteFrameName("mainui_bag_select_select.png");
	_rootLayer->addChild(_selectMark, 3);
	_selectMark->setVisible(false);

	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto itemid = BagItemControl::getInstace()->getItemConfig(i, j);
			if (itemid != -1)
			{
				ComInfo_T temp;
				ParamMgr::getInstance()->getComGameItemInfo(itemid, temp);
				auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
				_pageNode[j]->addChild(sprite, 2);

				//_vectorPlayerItems[i].item = &temp;
				_vectorPlayerItems[j][i].itemid = itemid;
				_vectorPlayerItems[j][i].posidx = i;
				_vectorPlayerItems[j][i].sprite = sprite;
				_vectorPlayerItems[j][i].id = _itemStartId;
				_flagBag[j][i] = 1;
				_itemStartId++;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		auto itemid = BagItemControl::getInstace()->getEquipItemsByIdx(i);
		if (itemid != -1)
		{
			ComInfo_T temp;
			ParamMgr::getInstance()->getComGameItemInfo(itemid, temp);
			auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
			_rootLayer->addChild(sprite, 2);
			//_vectorPlayerItems[i].item = &temp;
			_vectorEquipPlayerItems[i].itemid = itemid;
			_vectorEquipPlayerItems[i].posidx = i;
			_vectorEquipPlayerItems[i].sprite = sprite;
			_vectorEquipPlayerItems[i].id = _itemStartId;
			_itemStartId++;
			_flagEquip[i] = 1;
		}
		else
		{
			_flagEquip[i] = 0;
		}
	}
	_touchOneByOneListener = EventListenerTouchOneByOne::create();
	_touchOneByOneListener->setSwallowTouches(true);
	_touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(BagLayer::onTouchBegan, this);
	_touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(BagLayer::onTouchMoved, this);
	_touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(BagLayer::onTouchEnded, this);
	_touchOneByOneListener->onTouchCancelled = CC_CALLBACK_2(BagLayer::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchOneByOneListener, this);

	auto resortBtn = static_cast<ui::Button*>(_rootLayer->getChildByName("Button_2"));
	resortBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			reSortItems();
		}
	});

	 _delBtn = static_cast<ui::Button*>(_rootLayer->getChildByName("Button_3"));
	 _delBtn->setVisible(false);
	_delBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_touchItemsIdx != -1 && _touchItemsIdx < g_onepagetnum &&_flagBag[_curpageidx][_touchItemsIdx] == 1)
			{
				showDelDialog(_touchItemsIdx);
				//BagItemControl::getInstace()->delBagItem(_touchItemsIdx);
			}
			else if (_touchItemsIdx != -1 && _touchItemsIdx >= g_onepagetnum && _flagEquip[_touchItemsIdx - g_onepagetnum] == 1)
			{
				showDelDialog(_touchItemsIdx);
			}
		}
	});
	auto textpagechange = ui::TextAtlas::create("110", "fonts/num_pageselect.png", 16.f, 26.f, ".");
	auto pageselect = PageSelecter::create(g_pagecount, "com_pagejt_0.png", "com_pagejt_1.png", "com_pagejt_2.png", textpagechange, [=](int start, int end) {
		for (auto& temp : _pageNode)
		{
			auto actionmove = MoveBy::create(0.5f, Vec2(640.f * (start - end), 0.f));
			temp->runAction(actionmove);
			_curpageidx = end - 1;
		}
	});
	_rootLayer->addChild(pageselect);
	pageselect->setPosition(Vec2(320.f, _itemsPos[0].y - 160.f));
	/*_btnChangePageR = GameButton::create("jiantou_0.png", "", "", Widget::TextureResType::PLIST);
	_rootLayer->addChild(_btnChangePageR);
	_btnChangePageR->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_curpageidx != 2)
			{
				_curpageidx++;
				for (auto& temp : _pageNode)
				{
					auto actionmove = MoveBy::create(0.5f, Vec2(-640.f, 0.f));
					temp->runAction(actionmove);
				}
			}
		}
	});

	_btnChangePageL = GameButton::create("jiantou_0.png", "", "", Widget::TextureResType::PLIST);
	_rootLayer->addChild(_btnChangePageL);
	_btnChangePageL->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_curpageidx != 0)
			{
				_curpageidx--;
				for (auto& temp : _pageNode)
				{
					auto actionmove = MoveBy::create(0.5f, Vec2(640.f, 0.f));
					temp->runAction(actionmove);
				}
			}
		}
	});*/

	for (int i = 0; i < 4; i++)
	{
		_arrArrows[i] = (Sprite*)_rootLayer->getChildByName(String::createWithFormat("arrow_%d", i)->getCString());
		_arrArrows[i]->setVisible(false);
		_arrArrows[i]->setLocalZOrder(5);
	}

	_actionArrow = GameCSLoader::createTimeline("csb/layer_3.csb");
	_rootLayer->runAction(_actionArrow);
	_actionArrow->gotoFrameAndPlay(0,true);
	_actionArrow->pause();

	BagItemControl::getInstace()->_callback = std::bind(&BagLayer::ItemChanges, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	BagItemControl::getInstace()->_posCallback = std::bind(&BagLayer::PosChanges, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	return true;
}

void BagLayer::onEnter()
{
	Layer::onEnter();
	auto offy = VisibleRect::top().y - 960.f;
	auto trueoffy = (offy * 0.5f);
	for (int i = 0; i < 4; i++)
	{
		auto posNode = _rootLayer->getChildByName(String::createWithFormat("equip_%d", i)->getCString());
		_equipRectInWorld[i].origin = posNode->getPosition() + Vec2(-45.f, -45.f + trueoffy);
		_equipRectInWorld[i].size = Size(90.f, 90.f);

		_itemsPos[g_onepagetnum + i] = posNode->getPosition();
	}

	_itemsStartPos = Vec2(52.f, 540.f) + Vec2(0.f, -45.f + trueoffy) ;
	auto posmark =  Vec2(52.f, 540.f);
	for (int i = 0; i < g_onepagetnum; i ++)
	{
		auto truepos = Vec2(posmark.x + (i % 7) * 90.f, posmark.y + 90.f - 90.f * (i / 7));
		_itemsPos[i] = truepos;
	}
	
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto& item = _vectorPlayerItems[j][i];
			if (_flagBag[j][i] != 0)
			{
				item.sprite->setPosition(_itemsPos[i]);
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		auto &temp = _vectorEquipPlayerItems[i];
		if (_flagEquip[i] == 1)
		{
			temp.sprite->setPosition(_itemsPos[i + g_onepagetnum]);
		}
	}

	//_btnChangePageR->setPosition(_itemsPos[6] + Vec2(0.f, 50.f));
	//_btnChangePageL->setPosition(_itemsPos[0] + Vec2(0.f, 50.f));
}

bool BagLayer::onTouchBegan(Touch * touch, Event * unusedEvent)
{
	if (_isMove)
	{
		return false;
	}
	auto pos = touch->getLocation();
	_touchItemsIdx = checkIsTouchItem(pos);

	if (_touchItemsIdx != -1)
	{
		if (_touchItemsIdx < g_onepagetnum)
		{
			if (_touchItemsIdx != -1 && _flagBag[_curpageidx][_touchItemsIdx] == 1)
			{
				_delBtn->setVisible(true);
			}
			else
			{
				_delBtn->setVisible(false);
			}

			_selectMark->setPosition(_itemsPos[_touchItemsIdx]);
			_selectMark->setVisible(true);

			if (_flagBag[_curpageidx][_touchItemsIdx] == 0)
			{
				_textTitle->setString("");
				_textDes->setString("");
			}
			else
			{
				ComInfo_T temp;
				ParamMgr::getInstance()->getComGameItemInfo(_vectorPlayerItems[_curpageidx][_touchItemsIdx].itemid, temp);
				_textTitle->setString(temp.name);
				_textDes->setString(temp.des);
			}

			if (_isMove || _touchItemsIdx == -1 || _flagBag[_curpageidx][_touchItemsIdx] == 0)
			{
				return false;
			}
			if(BagItemControl::getInstace()->checkItemType(_vectorPlayerItems[_curpageidx][_touchItemsIdx].itemid) == 0)
			//if (_vectorPlayerItems[_curpageidx][_touchItemsIdx].itemid >= 1000)
			{
				for (int i = 0; i < 4; i++)
				{
					_arrArrows[i]->setVisible(true);
				}
			}

			_actionArrow->resume();
		}
		else
		{
			if (_touchItemsIdx != -1 && _flagEquip[_touchItemsIdx - g_onepagetnum] == 1)
			{
				_delBtn->setVisible(true);
			}
			else
			{
				_delBtn->setVisible(false);
			}

			_selectMark->setPosition(_itemsPos[_touchItemsIdx]);
			_selectMark->setVisible(true);

			if (_flagEquip[_touchItemsIdx - g_onepagetnum] == 0)
			{
				_textTitle->setString("");
				_textDes->setString("");
			}
			else
			{
				ComInfo_T temp;
				ParamMgr::getInstance()->getComGameItemInfo(_vectorEquipPlayerItems[_touchItemsIdx - 14].itemid, temp);
				_textTitle->setString(temp.name);
				_textDes->setString(temp.des);
			}

			if (_isMove || _touchItemsIdx == -1 || _flagEquip[_touchItemsIdx - g_onepagetnum] == 0)
			{
				return false;
			}
			if(BagItemControl::getInstace()->checkItemType(_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].itemid) == 0)
			//if (_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].itemid >= 1000)
			{
				for (int i = 0; i < 4; i++)
				{
					_arrArrows[i]->setVisible(true);
				}
			}

			_actionArrow->resume();
		}
		
	}
	else
	{
		_selectMark->setVisible(false);
		_textTitle->setString("");
		_textDes->setString("");
	}

	CCLOG("baglayer: %d item touched", _touchItemsIdx);
	return true;
}

void BagLayer::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	if (_touchItemsIdx < g_onepagetnum)
	{
		if (_touchItemsIdx != -1 && _flagBag[_curpageidx][_touchItemsIdx] == 1)
		{
			auto off = touch->getDelta();
			_vectorPlayerItems[_curpageidx][_touchItemsIdx].sprite->setPosition(_vectorPlayerItems[_curpageidx][_touchItemsIdx].sprite->getPosition() + off);

			auto Idx = checkIsTouchItem(touch->getLocation());
			if (Idx != -1)
			{
				_selectMark->setVisible(true);
				_selectMark->setPosition(_itemsPos[Idx]);
			}
			else
			{
				_selectMark->setVisible(false);
			}
		}
	}
	else
	{
		if (_touchItemsIdx != -1 && _flagEquip[_touchItemsIdx - g_onepagetnum] == 1)
		{
			auto off = touch->getDelta();
			_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].sprite->setPosition(_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].sprite->getPosition() + off);

			auto Idx = checkIsTouchItem(touch->getLocation());
			if (Idx != -1)
			{
				_selectMark->setVisible(true);
				_selectMark->setPosition(_itemsPos[Idx]);
			}
			else
			{
				_selectMark->setVisible(false);
			}
		}
	}
}

void BagLayer::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	if (_touchItemsIdx < g_onepagetnum && _touchItemsIdx >=0)
	{
		for (int i = 0; i < 4; i++)
		{
			_arrArrows[i]->setVisible(false);
		}
		_actionArrow->pause();

		auto pos = touch->getLocation();
		int endidx = checkIsTouchItem(pos);

		if (endidx >= g_onepagetnum && _vectorPlayerItems[_curpageidx][_touchItemsIdx].itemid < 1000)
		{
			endidx = -1;
		}

		if (endidx != -1 && endidx < g_onepagetnum)
		{
			if (_flagBag[_curpageidx][endidx] == 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}
			else if (endidx == _touchItemsIdx)
			{
				auto truepos = _itemsPos[_touchItemsIdx];
				_vectorPlayerItems[_curpageidx][_touchItemsIdx].sprite->setPosition(truepos);
			}
			else if (_flagBag[_curpageidx][endidx] != 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}

			GameLayer::getInstance()->updateEquipItems();
		}
		if (endidx != -1 && endidx >= g_onepagetnum)
		{
			if (_flagEquip[endidx - g_onepagetnum] == 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}
			else if (_flagEquip[endidx - g_onepagetnum] != 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}

			GameLayer::getInstance()->updateEquipItems();
		}
		else if (endidx == -1)
		{
			_isMove = true;
			auto truepos = _itemsPos[_touchItemsIdx];

			auto time = _vectorPlayerItems[_curpageidx][_touchItemsIdx].sprite->getPosition().distance(truepos) / 800.f;
			auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; _selectMark->setVisible(true); _selectMark->setPosition(_itemsPos[_touchItemsIdx]); }));
			_vectorPlayerItems[_curpageidx][_touchItemsIdx].sprite->runAction(action);
		}
	}
	else if (_touchItemsIdx >= g_onepagetnum)
	{
		for (int i = 0; i < 4; i++)
		{
			_arrArrows[i]->setVisible(false);
		}
		_actionArrow->pause();

		auto pos = touch->getLocation();
		int endidx = checkIsTouchItem(pos);

		if (endidx >= g_onepagetnum && _vectorEquipPlayerItems[_touchItemsIdx - 14].itemid < 1000)
		{
			endidx = -1;
		}
		else if (endidx != -1 && endidx < g_onepagetnum)
		{
			if (_flagBag[_curpageidx][endidx] != 0 && _vectorPlayerItems[_curpageidx][endidx].itemid < 1000)
			{
				endidx = -1;
			}
		}

		if (endidx != -1 && endidx < g_onepagetnum)
		{
			if (_flagBag[_curpageidx][endidx] == 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}
			else if (_flagBag[_curpageidx][endidx] != 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}

			GameLayer::getInstance()->updateEquipItems();
		}
		else if (endidx != -1 && endidx >= g_onepagetnum)
		{
			if (_flagEquip[endidx - g_onepagetnum] == 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}
			else if (endidx == _touchItemsIdx)
			{
				auto truepos = _itemsPos[_touchItemsIdx];
				_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].sprite->setPosition(truepos);
			}
			else if (_flagEquip[endidx - g_onepagetnum] != 0)
			{
				BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpageidx);
			}

			GameLayer::getInstance()->updateEquipItems();
		}
		else if (endidx == -1)
		{
			_isMove = true;
			auto truepos = _itemsPos[_touchItemsIdx];

			auto time = _vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].sprite->getPosition().distance(truepos) / 800.f;
			auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; _selectMark->setVisible(true); _selectMark->setPosition(_itemsPos[_touchItemsIdx]); }));
			_vectorEquipPlayerItems[_touchItemsIdx - g_onepagetnum].sprite->runAction(action);
		}
	}
	
}

void BagLayer::onTouchCancelled(Touch * touch, Event * unusedEvent)
{
	
}

void BagLayer::setCanTouch(bool cantouch)
{
	if (cantouch)
	{
		if (_touchOneByOneListener == nullptr)
		{
			_touchOneByOneListener = EventListenerTouchOneByOne::create();
			_touchOneByOneListener->setSwallowTouches(true);
			_touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(BagLayer::onTouchBegan, this);
			_touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(BagLayer::onTouchMoved, this);
			_touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(BagLayer::onTouchEnded, this);
			_touchOneByOneListener->onTouchCancelled = CC_CALLBACK_2(BagLayer::onTouchCancelled, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchOneByOneListener, this);
		}
	}
	else
	{
		if (_touchOneByOneListener != nullptr)
		{
			_eventDispatcher->removeEventListener(_touchOneByOneListener);
			_touchOneByOneListener = nullptr;
		}
	}
}

int BagLayer::checkIsTouchItem(cocos2d::Vec2 touchPos)
{
	/*if (_itemsStartPos.y == 0.f)
	{
		_itemsStartPos = _rootLayer->getChildByName("Button_1_7")->convertToWorldSpaceAR(Vec2(0.f, 0.f)) + Vec2(0.f, -45.f);
	}*/

	if (touchPos.y >= _itemsStartPos.y && touchPos.y <= _itemsStartPos.y + 180.f && touchPos.x >= 5.f && touchPos.x <= 635.f)
	{
		int idx = (touchPos.x - 5.f) / 90.f;
		int idy = (touchPos.y - _itemsStartPos.y) / 90.f;
		CCASSERT((idx + 7 - idy * 7) < 14, "error");
		return (idx + 7 - idy * 7);
	}
	else
	{
		int idx = 0;
		for (auto& rect : _equipRectInWorld)
		{
			if (rect.containsPoint(touchPos))
			{
				return g_onepagetnum + idx;
			}
			idx++;
		}
	}
	
	return -1;
}

void BagLayer::ItemChanges(int itemid, int numchange, int bagpos, int page)
{
	if (bagpos >= g_onepagetnum)
	{
		if (numchange > 0)
		{
			ComInfo_T temp;
			ParamMgr::getInstance()->getComGameItemInfo(itemid, temp);
			auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
			_pageNode[page]->addChild(sprite, 2);
			sprite->setPosition(_itemsPos[bagpos]);

			CCASSERT(_flagEquip[bagpos - g_onepagetnum] == 0, "this pos already have items");
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].itemid = itemid;
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].posidx = bagpos;
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].sprite = sprite;
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].id = _itemStartId;
			_itemStartId++;
			_flagEquip[bagpos - g_onepagetnum] = 1;
		}
		else if (numchange < 0)
		{
			CCASSERT(_flagEquip[bagpos - g_onepagetnum] == 1, "this pos have no items");
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].itemid = -1;
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].posidx = -1;
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].sprite->removeFromParent();
			_vectorEquipPlayerItems[bagpos - g_onepagetnum].sprite = nullptr;
			_flagEquip[bagpos - g_onepagetnum] = 0;
		}
	}
	else
	{
		if (numchange > 0)
		{
			ComInfo_T temp;
			ParamMgr::getInstance()->getComGameItemInfo(itemid, temp);
			auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
			_pageNode[page]->addChild(sprite, 2);
			sprite->setPosition(_itemsPos[bagpos]);

			CCASSERT(_flagBag[page][bagpos] == 0, "this pos already have items");
			_vectorPlayerItems[page][bagpos].itemid = itemid;
			//_vectorPlayerItems[bagpos].item = &temp;
			_vectorPlayerItems[page][bagpos].posidx = bagpos;
			_vectorPlayerItems[page][bagpos].sprite = sprite;
			_vectorPlayerItems[page][bagpos].id = _itemStartId;
			_itemStartId++;
			_flagBag[page][bagpos] = 1;
		}
		else if (numchange < 0)
		{
			CCASSERT(_flagBag[page][bagpos] == 1, "this pos have no items");
			_vectorPlayerItems[page][bagpos].itemid = -1;
			//_vectorPlayerItems[bagpos].item = nullptr;
			_vectorPlayerItems[page][bagpos].posidx = -1;
			_vectorPlayerItems[page][bagpos].sprite->removeFromParent();
			_vectorPlayerItems[page][bagpos].sprite = nullptr;
			_flagBag[page][bagpos] = 0;
		}
	}
	
}

void BagLayer::PosChanges(int start, int endidx, int page, int type)
{
	if (type == 0)// norpos->norpos;
	{
		if (_flagBag[page][endidx] == 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorPlayerItems[page][start].sprite->setPosition(truepos);

			_flagBag[page][endidx] = 1;
			_flagBag[page][start] = 0;

			_vectorPlayerItems[page][endidx] = _vectorPlayerItems[page][start];
			_vectorPlayerItems[page][endidx].posidx = endidx;
			_vectorPlayerItems[page][start].itemid = -1;
			_vectorPlayerItems[page][start].posidx = -1;
			_vectorPlayerItems[page][start].sprite = nullptr;

			_touchItemsIdx = endidx;
		}
		else if (_flagBag[page][endidx] != 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorPlayerItems[page][start].sprite->setPosition(truepos);

			_isMove = true;
			auto startpos = _itemsPos[start];
			auto time = _itemsPos[endidx].distance(startpos) / 800.f;
			auto action = Sequence::createWithTwoActions(MoveTo::create(time, startpos), CallFunc::create([=]() {_isMove = false; }));
			_vectorPlayerItems[page][endidx].sprite->runAction(action);

			auto bak = _vectorPlayerItems[page][endidx];
			_vectorPlayerItems[page][endidx] = _vectorPlayerItems[page][start];
			_vectorPlayerItems[page][start] = bak;

			_touchItemsIdx = endidx;
		}
	}
	else if (type == 1)// norpos->equippos;
	{
		auto equipidx =_flagEquip[endidx - g_onepagetnum];
		if (equipidx == 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorPlayerItems[page][start].sprite->setPosition(truepos);

			_vectorPlayerItems[page][start].sprite->retain();
			_vectorPlayerItems[page][start].sprite->removeFromParent();
			_rootLayer->addChild(_vectorPlayerItems[page][start].sprite);
			_vectorPlayerItems[page][start].sprite->release();

			_flagBag[page][start] = 0;
			_flagEquip[endidx - g_onepagetnum] = 1;

			_vectorEquipPlayerItems[endidx - g_onepagetnum] = _vectorPlayerItems[page][start];
			_vectorEquipPlayerItems[endidx - g_onepagetnum].posidx = endidx;
			_vectorPlayerItems[page][start].itemid = -1;
			_vectorPlayerItems[page][start].posidx = -1;
			_vectorPlayerItems[page][start].sprite = nullptr;

			_touchItemsIdx = endidx;
		}
		else if (equipidx != 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorPlayerItems[page][start].sprite->setPosition(truepos);

			_vectorPlayerItems[page][start].sprite->retain();
			_vectorPlayerItems[page][start].sprite->removeFromParent();
			_rootLayer->addChild(_vectorPlayerItems[page][start].sprite);
			_vectorPlayerItems[page][start].sprite->release();

			_isMove = true;
			auto startpos = _itemsPos[start];
			auto time = _itemsPos[endidx].distance(startpos) / 800.f;

			_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite->retain();
			_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite->removeFromParent();
			_pageNode[page]->addChild(_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite);
			_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite->release();

			auto action = Sequence::createWithTwoActions(MoveTo::create(time, startpos), CallFunc::create([=]() {_isMove = false; }));
			_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite->runAction(action);

			auto bak = _vectorEquipPlayerItems[endidx - g_onepagetnum];
			_vectorEquipPlayerItems[endidx - g_onepagetnum] = _vectorPlayerItems[page][start];
			_vectorPlayerItems[page][start] = bak;

			_touchItemsIdx = endidx;
		}
	}
	else if (type == 2)// norpos<-equippos;
	{
		if (_flagBag[page][endidx] == 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->setPosition(truepos);

			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->retain();
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->removeFromParent();
			_pageNode[page]->addChild(_vectorEquipPlayerItems[start - g_onepagetnum].sprite);
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->release();

			_flagBag[page][endidx] = 1;
			_flagEquip[start - g_onepagetnum] = 0;

			_vectorPlayerItems[page][endidx] = _vectorEquipPlayerItems[start - g_onepagetnum];
			_vectorPlayerItems[page][endidx].posidx = endidx;
			_vectorEquipPlayerItems[start - g_onepagetnum].itemid = -1;
			_vectorEquipPlayerItems[start - g_onepagetnum].posidx = -1;
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite = nullptr;

			_touchItemsIdx = endidx;
		}
		else if (_flagBag[page][endidx] != 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->setPosition(truepos);

			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->retain();
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->removeFromParent();
			_pageNode[_curpageidx]->addChild(_vectorEquipPlayerItems[start - g_onepagetnum].sprite);
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->release();

			_isMove = true;
			auto startpos = _itemsPos[start];
			auto time = _itemsPos[endidx].distance(startpos) / 800.f;

			_vectorPlayerItems[page][endidx].sprite->retain();
			_vectorPlayerItems[page][endidx].sprite->removeFromParent();
			_rootLayer->addChild(_vectorPlayerItems[page][endidx].sprite);
			_vectorPlayerItems[page][endidx].sprite->release();

			auto action = Sequence::createWithTwoActions(MoveTo::create(time, startpos), CallFunc::create([=]() {_isMove = false; }));
			_vectorPlayerItems[page][endidx].sprite->runAction(action);

			auto bak = _vectorPlayerItems[page][endidx];
			_vectorPlayerItems[page][endidx] = _vectorEquipPlayerItems[start - g_onepagetnum];
			_vectorEquipPlayerItems[start - g_onepagetnum] = bak;

			_touchItemsIdx = endidx;
		}
	}
	else if (type == 3)// equippos<-equippos;
	{
		auto equipidx = _flagEquip[endidx - g_onepagetnum];
		if (equipidx == 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->setPosition(truepos);

			_flagEquip[start - g_onepagetnum] = 0;
			_flagEquip[endidx - g_onepagetnum] = 1;

			_vectorEquipPlayerItems[endidx - g_onepagetnum] = _vectorEquipPlayerItems[start - g_onepagetnum];
			_vectorEquipPlayerItems[endidx - g_onepagetnum].posidx = endidx;
			_vectorEquipPlayerItems[start - g_onepagetnum].itemid = -1;
			_vectorEquipPlayerItems[start - g_onepagetnum].posidx = -1;
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite = nullptr;

			_touchItemsIdx = endidx;
		}
		else if (equipidx != 0)
		{
			auto truepos = _itemsPos[endidx];
			_vectorEquipPlayerItems[start - g_onepagetnum].sprite->setPosition(truepos);

			_isMove = true;
			auto startpos = _itemsPos[start];
			auto time = _itemsPos[endidx].distance(startpos) / 800.f;
			auto action = Sequence::createWithTwoActions(MoveTo::create(time, startpos), CallFunc::create([=]() {_isMove = false; }));
			_vectorEquipPlayerItems[endidx - g_onepagetnum].sprite->runAction(action);

			auto bak = _vectorEquipPlayerItems[endidx - g_onepagetnum];
			_vectorEquipPlayerItems[endidx - g_onepagetnum] = _vectorEquipPlayerItems[start - g_onepagetnum];
			_vectorEquipPlayerItems[start - g_onepagetnum] = bak;

			_touchItemsIdx = endidx;
		}
	}
}

void BagLayer::reSortItems()
{
	if (_isMove)
	{
		return;
	}
	std::set< ItemWithSprite_T, mycomp> tempset;
	int count = 0;
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (_flagBag[j][i])
			{
				tempset.insert(_vectorPlayerItems[j][i]);
				count++;
			}
			_flagBag[j][i] = 0;
			
		}
	}
	
	
	int idx = (int)tempset.size();
	auto itr = tempset.begin();
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (i + j*g_onepagetnum< idx)
			{
				_flagBag[j][i] = 1;
				int pos = (*itr).posidx;
				_vectorPlayerItems[j][i].sprite = (*itr).sprite;
				_vectorPlayerItems[j][i].itemid = (*itr).itemid;
				//_vectorPlayerItems[i].item = (*itr).item;
				_vectorPlayerItems[j][i].posidx = i;

				(*itr).sprite->retain();
				(*itr).sprite->removeFromParent();
				_pageNode[j]->addChild((*itr).sprite);
				(*itr).sprite->release();

				(*itr).sprite->setPosition(_itemsPos[i]);
				itr++;
				BagItemControl::getInstace()->setItemConfig(i, _vectorPlayerItems[j][i].itemid, j);
				//BagItemControl::getInstace()->setItemConfig(i, _vectorPlayerItems[i].item->id);
			}
			else
			{
				_vectorPlayerItems[j][i].itemid = -1;
				//_vectorPlayerItems[i].item = nullptr;
				_vectorPlayerItems[j][i].posidx = -1;
				_vectorPlayerItems[j][i].sprite = nullptr;

				BagItemControl::getInstace()->setItemConfig(i, -1, j);
			}
	}
	
		
		
	}
	
	BagItemControl::getInstace()->saveItemConfig();
}

void BagLayer::showDelDialog(int posidx)
{
	if (_isMove)
	{
		return;
	}

	if (BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpageidx) == -1)
	{
		return;
	}
	auto delDialog = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	auto node = GameCSLoader::createNode("csb/itemdelnode.csb");
	node->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f - 100.f));
	delDialog->addChild(node);
	this->addChild(delDialog, 1000);

	auto itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpageidx);
	ComInfo_T iteminfo;
	ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);
	auto text = static_cast<ui::Text*>(node->getChildByName("text_title"));
	text->setString(iteminfo.name);

	auto textdel = Sprite::createWithSpriteFrameName("mainui_text_dqwp.png");
	node->addChild(textdel);
	textdel->setPosition(text->getPosition());
	text->setVisible(false);

	auto textname = static_cast<ui::Text*>(node->getChildByName("text_name"));
	textname->setString(iteminfo.name);

	auto icon = static_cast<Sprite*>(node->getChildByName("item_icon"));
	icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iteminfo.picname));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, delDialog);

	auto btnok = static_cast<ui::Button*>(node->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
				BagItemControl::getInstace()->delBagItem(posidx, _curpageidx);
				delDialog->removeFromParent();

				GameLayer::getInstance()->updateEquipItems();
			}
		});

	auto btncancle = static_cast<ui::Button*>(node->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			delDialog->removeFromParent();
		}
	});	
}

bool BagLayer::mycomp::operator()(const ItemWithSprite_T & a, const ItemWithSprite_T & b)
{
	{
		if (a.itemid == b.itemid &&  a.posidx == b.posidx)
		{
			return a.id < b.id;
		}
		if (a.itemid == b.itemid)
		{
			return a.posidx < b.posidx;
		}
		return a.itemid< b.itemid;
	}
}

PageSelecter * PageSelecter::create(int maxidx, std::string pic, std::string pic2, std::string pic3, ui::TextAtlas* font, ccPageChangeCallBack pagecall)
{
	auto pRef = new PageSelecter();
	if (pRef && pRef->init(maxidx,pic, pic2, pic3, font, pagecall))
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
	return nullptr;
}

bool PageSelecter::init(int maxidx, std::string pic, std::string pic2, std::string pic3, ui::TextAtlas* font, ccPageChangeCallBack pagecall)
{
	Node::init();
	_protectTime = 0.5f;
	_inProtect = false;

	_maxidx = maxidx;
	_curidx = 1;
	_textPage = font;
	_callback = pagecall;

	this->addChild(_textPage);

	_textPage->setString(String::createWithFormat("%d/%d", _curidx, _maxidx)->getCString());

	_btnL = GameButton::create(pic, pic2, pic3, Widget::TextureResType::PLIST);
	this->addChild(_btnL);
	_btnL->setScaleX(-1);

	_btnR = GameButton::create(pic, pic2, pic3, Widget::TextureResType::PLIST);
	this->addChild(_btnR);

	_btnL->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_inProtect)
			{
				return;
			}

			if (_curidx != 1)
			{
				auto bakidx = _curidx;
				_curidx--;
				_callback(bakidx, _curidx);
				_textPage->setString(String::createWithFormat("%d/%d", _curidx, _maxidx)->getCString());

				_inProtect = true;

				auto action = Sequence::createWithTwoActions(DelayTime::create(_protectTime), CallFunc::create([=]() {_inProtect = false; }));
				this->runAction(action);
			}

			if (_curidx == 1)
			{
				_btnL->setVisible(false);
				_btnR->setVisible(true);
			}
			else if (_curidx == _maxidx)
			{
				_btnL->setVisible(true);
				_btnR->setVisible(false);
			}
			else
			{
				_btnL->setVisible(true);
				_btnR->setVisible(true);
			}
		}
	});



	_btnR->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_inProtect)
			{
				return;
			}

			if (_curidx != _maxidx)
			{
				auto bakidx = _curidx;
				_curidx++;
				_callback(bakidx, _curidx);
				_textPage->setString(String::createWithFormat("%d/%d", _curidx, _maxidx)->getCString());

				_inProtect = true;

				auto action = Sequence::createWithTwoActions(DelayTime::create(_protectTime), CallFunc::create([=]() {_inProtect = false; }));
					this->runAction(action);
			}
			
			if (_curidx == 1)
			{
				_btnL->setVisible(false);
				_btnR->setVisible(true);
			}
			else if (_curidx == _maxidx)
			{
				_btnL->setVisible(true);
				_btnR->setVisible(false);
			}
			else
			{
				_btnL->setVisible(true);
				_btnR->setVisible(true);
			}
		}
	});

	_btnL->setPositionX(-60.f);
	_btnR->setPositionX(60.f);

	
	_btnL->setVisible(false);
	_btnR->setVisible(true);

	return true;
}


#if 0
void MainLayer::onGetTopDataCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba 
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}


	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	CCLOG(statusString);

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	//	NetDataMgr::getInstance()->saveSettleRankInfo(client, response);
	//	NetDataMgr::getInstance()->saveRealRankInfo(client, response);
	RankBaseData_T *p = NetDataMgr::getInstance()->analyzeTmpSort(client, response);
	return;
	//	NetDataMgr::getInstance()->saveSelfInfo(client, response);
	//	NetDataMgr::getInstance()->saveUpdateRole(client, response);
	/*
	std::vector<char> *buffer = response->getResponseData();
	CCLOG("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
	CCLOG("%c", (*buffer)[i]);//这里打印从服务器返回的数据

	}
	CCLOG("\n");

	string strRepData(buffer->begin(), buffer->end());
	Json *pJson = Json_create(strRepData.c_str());
	if (NULL != pJson)
	{
	Json *pItem = Json_getItem(pJson, "nick");
	string accInfo = Json_getString(pJson, "account", "");
	pItem = Json_getItemAt(pJson, 0);
	pItem = Json_getItem(pJson, "DATA");
	Json *pSubItem = Json_getItemAt(pItem, 2);
	int index = Json_getInt(pSubItem, "index", -1);
	}*/
}
void MainLayer::onGetSelfInfoCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	NetDataMgr::getInstance()->saveSelfInfo(client, response);

	int onlineDay = NetDataMgr::getInstance()->getOnlineDay();
	int lastLoginDay = UserData::getInstance()->getLastLoginDay();
	int lastSettleDay = UserData::getInstance()->getLastSettleDay();

	UserData::getInstance()->setLastLoginDay(onlineDay);

	if (!JRTime::isTheSameWeek(onlineDay, lastLoginDay))
	{
		UserData::getInstance()->setWeeklyScore(0);
	}

	if (-1 == lastSettleDay)
	{
		UserData::getInstance()->setLastSettleDay(onlineDay);
		lastSettleDay = onlineDay;
	}

	if (!JRTime::isTheSameWeek(onlineDay, lastSettleDay) && 0 == _settleRankState)
	{
		_settleRankState = 1;
		NetDataMgr::getInstance()->querySettleRank(CC_CALLBACK_2(MainLayer::onGetSettleTopCallback, this));
	}

// 	if (SCENEID_MAIN == CCDirector::sharedDirector()->getRunningScene()->getTag())
// 	{
//		this->updateSelfData();
//		this->updateSelfInfo();
// 	}


	return;
}
#endif
void MainLayer::onUpdateSelfInfoCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	/*auto pLayer = DailyRewardLayer::create();
	this->addChild(pLayer, POP_Z);*/

	NetDataMgr::getInstance()->saveUpdateRole(client, response);

	int onlineDay = NetDataMgr::getInstance()->getOnlineDay();
	int lastLoginDay = UserData::getInstance()->getLastLoginDay();
	int lastSettleDay = UserData::getInstance()->getLastSettleDay();

	CommondActivityMagr::getInstance()->intiAndShowActivitys(onlineDay);

	//debug zsb
	if (onlineDay != lastLoginDay)
	//if (true)
	{
		UserData::getInstance()->setEndlessCount(0);
		UserData::getInstance()->setTiliBuyTimes(3);
		UserData::getInstance()->setPopGuideBoss(1);
		UserData::getInstance()->setPayRmb(0);

		DayActivityMgr::getInstance()->resetTimes();
		TimeLimitActivityMgr::getInstance()->resetOnlineTimeReward();
		
		for (int i = 0; i < 3; i ++)
		{
			UserData::getInstance()->setDungeonTime(i, 0);
		}

		if (0 != lastLoginDay)
		{
			int cur = UserData::getInstance()->getCurGiftType();
			if (0 != cur)
			{
				UserData::getInstance()->setCurGiftType(0);
			}
			else
			{
				UserData::getInstance()->setCurGiftType(1);
			}
			updataDayAndTimeGiftBtn();
		}

		if (!UserData::getInstance()->getIsGotDailyReward(6))
		{
			if (!GameMap::getCurGameMap()->isInFight())//not in fight
			{
				auto pLayer = DailyRewardLayer::create();
				this->addChild(pLayer, POP_Z);
			}
			else
			{
				_isNeedDaily = true;
			}
		}
		else
		{
			if (!GameMap::getCurGameMap()->isInFight())//not in fight
			{
				auto pLayer = DailyRewardLayer::create(true);
				this->addChild(pLayer, POP_Z);
			}
			else
			{
				_isNeedWeekReward = true;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			UserData::getInstance()->setTiliRewardTimes(i, 0);
		}
		UserData::getInstance()->setRefashTimes(0);

		UserData::getInstance()->setCurDayGiftTimes(0);

		CommondActivityMagr::getInstance()->resetReward();
	}

	UserData::getInstance()->setLastLoginDay(onlineDay);
	MagPieMgr::getInstance()->setDayIdx(onlineDay - MagPieMgr::MAGPIE_START_DAY);
	TimeLimitActivityMgr::getInstance()->intiAndShowActivitys(onlineDay);
	

	this->updataGiftBtns();

	if (!JRTime::isTheSameWeek(onlineDay, lastLoginDay))
	{
		UserData::getInstance()->setWeeklyScore(0);
	}

	if (0 == lastSettleDay)
	{
		UserData::getInstance()->setLastSettleDay(onlineDay);
		lastSettleDay = onlineDay;
	}

	if (!JRTime::isTheSameWeek(onlineDay, lastSettleDay) && 0 == _settleRankState)
	{
		_settleRankState = 1;
		NetDataMgr::getInstance()->querySettleRank(CC_CALLBACK_2(MainLayer::onGetSettleTopCallback, this));
	}

	if ("" == UserData::getInstance()->getAccount())
	{
		UserData::getInstance()->setAccount(NetDataMgr::getInstance()->getAccount().c_str());
	}	 
	UserData::getInstance()->saveUserData(true);

	if (nullptr != EndlessMainLayer::getInstance())
	{
		EndlessMainLayer::getInstance()->updateSelfInfoUI();
		EndlessMainLayer::getInstance()->updateRankTableView();
	}


// 	if (SCENEID_MAIN == CCDirector::sharedDirector()->getRunningScene()->getTag())
// 	{
//		this->updateSelfData();
//		this->updateSelfInfo();
// 		this->updateRankTableView();
// 	}
}
#if 0

void MainLayer::onGetRealTopCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	NetDataMgr::getInstance()->saveRealRankInfo(client, response);
	/*updata rank table view*/
// 	if (SCENEID_MAIN == CCDirector::sharedDirector()->getRunningScene()->getTag())
// 	{
// 		this->updateRankTableView();
// 	}
}
#endif
void MainLayer::onGetSettleTopCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	NetDataMgr::getInstance()->saveSettleRankInfo(client, response);

	if (NetDataMgr::getInstance()->getSelfSettleScoreRank() <= 1000 && NetDataMgr::getInstance()->getSelfSettleScoreRank() > 0)
	{
		if (!GameMap::getCurGameMap()->isInFight())//not in fight
		{
			_settleRankState = 0;
			//show settle reward
			std::vector<PopItemInfo_T> arrReward;
			CrushUtil::getSettleReward(NetDataMgr::getInstance()->getSelfSettleScoreRank(), arrReward);
			auto pLayer = PopRewardLayer::create(arrReward, 1);
			this->addChild(pLayer, POP_REWARD_Z);
			pLayer->setRankIdx(NetDataMgr::getInstance()->getSelfSettleScoreRank());

			UserData::getInstance()->setLastSettleDay(NetDataMgr::getInstance()->getOnlineDay());
		}
		else
		{
			_settleRankState = 2;
		}
	}
	else
	{
		_settleRankState = 0;
		UserData::getInstance()->setLastSettleDay(NetDataMgr::getInstance()->getOnlineDay());
	}
}

#if 0
void MainLayer::onPostTmpSortCallback(HttpClient* client, HttpResponse* response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);
	//simba
	if (statusCode != 200)
	{
		GameUtils::toastTip(5);
		return;
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	RankBaseData_T *arrRank = NetDataMgr::getInstance()->analyzeTmpSort(client, response);
	/*
	if (NULL != PurchaseLayer::getCurPurchaseLayer())
	{
		PurchaseLayer *pPurchaseLayer = PurchaseLayer::getCurPurchaseLayer();
		int nRankIndex = arrRank[0].nIndex;

		if (0 == pPurchaseLayer->getGoodItemId().compare("itemid_good_revive_1")
			|| 0 == pPurchaseLayer->getGoodItemId().compare("itemid_good_revive")
			|| 0 == pPurchaseLayer->getGoodItemId().compare("itemid_good_revive_2")
			|| 0 == pPurchaseLayer->getGoodItemId().compare("itemid_good_revive_3"))
		{
			//string strRankInfo = ResMgr::getInstance()->getString("revive_rank")->getCString();
			//ToolsUtil::stringFindReplace(strRankInfo, "N", CCString::createWithFormat("%d", nRankIndex)->getCString());
			CCLabelTTF *pLbl = pPurchaseLayer->getItemText();
			//pLbl->setString(strRankInfo.c_str());
			pLbl->setString(CCString::createWithFormat("%d", nRankIndex)->getCString());
			pLbl->setColor(ccc3(0xff, 0xf6, 0x00));
		}
	}*/

	delete[]arrRank;
}
#endif

void MainLayer::onCheckCodeCallback(HttpClient * client, HttpResponse * response)
{
	HttpClient *pClient = client;

	if (!response)
	{
		return;
	}

//	auto s = response->getHttpRequest()->getRequestType();
//	CCLOG("request type %d", s);


	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	CCLOG("response code: %d", statusCode);

	if (!response->isSucceed())
	{
		GameUtils::toastTip(5);
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	Json *pJson = Json_create(strRepData.c_str());

	int checkRet = Json_getInt(pJson, "status", 0);
	int codeType = Json_getInt(pJson, "type", 1);

	Json_dispose(pJson);

	if (checkRet == 200)
	{
		if (codeType >= 1 && codeType < 4 || 5 == codeType || 6 == codeType)
		{
			if (UserData::getInstance()->getIsExchange(codeType-1))
			{
				GameUtils::toastTip(3);
			}
			else
			{
				std::vector<PopItemInfo_T> arrItems;
				CrushUtil::getExchangeReward(codeType, arrItems);
				auto pPop = PopRewardLayer::create(arrItems, 2);
				this->addChild(pPop, 100);

				UserData::getInstance()->setIsExchange(codeType - 1, true);
				UserData::getInstance()->saveUserData(true);
			}
		}
		else if (4 == codeType)
		{
			std::vector<PopItemInfo_T> arrItems;
			CrushUtil::getExchangeReward(codeType, arrItems);
			auto pPop = PopRewardLayer::create(arrItems, 2);
			this->addChild(pPop, 100);
		}
	}
	else
	{
		GameUtils::toastTip(4);
	}
}

void MainLayer::updataEquipLayer(int playeridx)
{
	for (int i = 0; i < 10; i++)
	{
		_arrEquipLayer[playeridx]->updateWenponInfo(i);
	}
	_arrEquipLayer[playeridx]->updateWUPBtn();
}

bool MainLayer::checkGuide()
{
	//debug
	if (UserData::getInstance()->getMaxPos() > 0)
//	if (UserData::getInstance()->getMaxPos() > -1)
	{
		if (!UserData::getInstance()->getIsBoxGuided())
//		if (false)
		{
			if (nullptr == GameGuideLayer::getInstance())
			{
				auto pLayer = GameGuideLayer::create(0);
				this->addChild(pLayer, 999);

				pLayer->guideStepStart(1, _rewardBtn->convertToWorldSpace(Vec2(30.f, 36.f)));
				return true;
			}
		}
		else if (!UserData::getInstance()->getWeaponGuided())
		{
			if (AchieveMgr::getInstance()->getCurAchieveCount(AchieveMgr::_AchieveIdx::FORGE) == 0)
			{
				if (nullptr == GameGuideLayer::getInstance())
				{
					_eventDispatcher->addCustomEventListener("guide_end", CC_CALLBACK_1(MainLayer::guideEvent, this));

					auto pLayer = GameGuideLayer::create(1);
					this->addChild(pLayer, 999);

					pLayer->guideStepStart(1, _arrMenuItems[0]->convertToWorldSpace(Vec2(140.f, 20.f)));
					return true;
				}
			}
			else
			{
				UserData::getInstance()->setWeaponGuided(true);
			}
		}
		else
		{			
			if (!UserData::getInstance()->getEndlessGuided() && UserData::getInstance()->getStoryFileIdx() > 4)
			{
				if (nullptr == _wujingBtn->getChildByName("guangquan"))
				{
					ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/guangquan_dianji/guangquan_dianji.ExportJson");

					auto pArmtr = Armature::create("guangquan_dianji");
					_wujingBtn->addChild(pArmtr, 1, "guangquan");
					pArmtr->getAnimation()->play("dianji");
					pArmtr->setPosition(Vec2(30.f, 36.f));
				}
			}
		}
	}

	return false;
}

void MainLayer::startGuideStep()
{
	if (nullptr != GameGuideLayer::getInstance())
	{
		auto pGuideLayer = GameGuideLayer::getInstance();
		int guideType = GameGuideLayer::getInstance()->getGuideType();
		int guideStep = GameGuideLayer::getInstance()->getGuideStep();

		if (0 == guideType)
		{
			switch (guideStep)
			{
			case 0:
			{
				Vec2 pos = pGuideLayer->convertToNodeSpace(_rewardBtn->convertToWorldSpace(Vec2::ZERO));
				pGuideLayer->setTouchInfo(1, pos);
			}
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
		else if (1 == guideType)
		{

		}
	}
}

void MainLayer::endGuideStep()
{
}

void MainLayer::guideEvent(EventCustom * evt)
{
	if (nullptr != GameGuideLayer::getInstance())
	{
		int guideType = GameGuideLayer::getInstance()->getGuideType();
		int guideStep = GameGuideLayer::getInstance()->getGuideStep();

		if (1 == guideType)
		{
			if (7 == guideStep)
			{
				GameGuideLayer::getInstance()->guideStepStart(1, _arrMenuItems[3]->convertToWorldSpace(Vec2(48.f, 38.f)));
			}
			else if (9 == guideStep)
			{
				GameGuideLayer::getInstance()->guideStepStart(1, _arrMenuItems[4]->convertToWorldSpace(Vec2(48.f, 38.f)));
			}
			else if (11 == guideStep)
			{
				GameGuideLayer::getInstance()->guideStepStart(1, _arrMenuItems[5]->convertToWorldSpace(Vec2(48.f, 38.f)));
			}
		}
	}
}
