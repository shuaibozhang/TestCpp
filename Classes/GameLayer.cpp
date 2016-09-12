#include "GameLayer.h"
#include "VisibleRect.h"
#include "JRCommon.h"
#include "EleIcon.h"
#include "CrushLayer.h"
#include "cocostudio/CocoStudio.h"
//#include "DragonBonesHeaders.h"
//#include "DBCCRenderHeaders.h"
#include "Armtr/GameArmtr.h"
#include "ParamMgr.h"
#include "Player.h"
#include "Monster.h"
#include "CrushUtil.h"
#include "FightLayer.h"
#include "GameCSLoader.h"
#include "UserData.h"
#include "Scenes/GameMap.h"
#include "GameUtils.h"
#include "BagItemControl.h"
#include "GLActions/GLActions.h"
#include "SkillControl.h"
#include "GuideLayer.h"
#include "CrushFSM/CrushFSM.h"
#include "GLCommon/UI/GLSlidBar.h"
#include "Story/StoryMgr.h"
#include "Scenes/MainScene.h"
#include "SpecialAttUtils.h"
#include "platBridge/cocos2dx_analyze.h"
#include "Scenes/PopRewardLayer.h"
#include "WeaponControl.h"
#include "MagPieMgr.h"
#include "DayActiveMgr.h"
#include "../StoreBridge/StoreAssets.h"
#include "Store/PurchaseLayer.h"
#include "Scenes/DungeonLayer.h"

USING_NS_CC;
using namespace cocostudio;
//USING_NS_DB;

GameLayer *GameLayer::s_pInstance = nullptr;

GameLayer *GameLayer::getInstance()
{
	return s_pInstance;
}


Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


GameLayer::GameLayer()
	:_pCrushLayer(nullptr),
	_isNeedUpdateExp(false),
	_isExpNodeShow(false),
	_fightInfoNode(nullptr),
	_lastHp(-1),
	_lastDp(-1),
	_isGuiding(false),
	_isStartFight(false),
	_touchStoryListener(nullptr),
	_storyTouchProxyNode(nullptr),
	_selectmenu(nullptr),
	_curStorytype(0),
	_fightType(0),
	_pDefBarArmtr(nullptr),
	_skillAnimCount(0),
	_pDungeonRootNode(nullptr)
{
	s_pInstance = this;
	memset(_arrEquipItems, 0, sizeof(_arrEquipItems));
	memset(_aniNode, 0, sizeof(_aniNode));
	memset(_spriteEquipBg, 0, sizeof(_spriteEquipBg));
}


GameLayer::~GameLayer()
{
	_bossHpBar = nullptr;
	_bossHpBarBg = nullptr;
	s_pInstance = nullptr;
}


// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	ToolsUtil::initRandomNumber();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/items.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/weak_items.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/fightUi.plist");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/lv_up/lv_up.ExportJson");
	
	initCombo();
	_comboNum = 0;
	_comboNode->setVisible(false);

// 	_pCrushLayer = CrushLayer::create();
// 	this->addChild(_pCrushLayer, EntityZ_E::E_Z_CRUSH_LAYER);


	float _mapOffpos = 0;
	if ((VisibleRect::top().y / VisibleRect::right().x) > (1160.f / 640.f))
	{
		_mapOffpos = VisibleRect::top().y - 1160 ;
	}
	else
	{
		_mapOffpos = 0;
	}

	_bottomUiNode = GameCSLoader::createNode("csb/fightBottomLayer.csb");
	this->addChild(_bottomUiNode, EntityZ_E::E_Z_UI);
	_bottomUiNode->setPositionY(_mapOffpos);

	_bottomUiNodeR = Node::create();
	this->addChild(_bottomUiNodeR, EntityZ_E::E_Z_UI);
	_bottomUiNodeR->setPosition(_bottomUiNode->getPosition());

	_pPlayerDefBar = static_cast<LoadingBar*>(_bottomUiNode->getChildByName("bar_def"));
//	_pPlayerDefBar->setScale9Enabled(true);
	_pPlayerDefBar->setPercent(100.f);

	_pPlayerHpBar = static_cast<LoadingBar*>(_bottomUiNode->getChildByName("bar_hp"));
//	_pPlayerHpBar->setScale9Enabled(true);
	_pPlayerHpBar->setPercent(100.f);

	_pHpLbl = static_cast<TextAtlas*>(_bottomUiNode->getChildByName("lbl_hp"));
	_pDefLbl = static_cast<TextAtlas*>(_bottomUiNode->getChildByName("lbl_def"));
	_pHpLbl->setScale(0.85f);
	_pDefLbl->setScale(0.85f);
	_pHpLbl->setPosition(_pHpLbl->getPosition() + Vec2(0.f, 6.f));
	_pDefLbl->setPosition(_pDefLbl->getPosition() + Vec2(0.f, 6.f));

	_pBarIconHp = static_cast<Sprite*>(_bottomUiNode->getChildByName("bar_icon_hp_11"));
	_pBarIconDef = static_cast<Sprite*>(_bottomUiNode->getChildByName("bar_icon_def_12"));
	
	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/huduntiao/huduntiao.ExportJson");
	_pDefBarArmtr = Armature::create("huduntiao");
	_bottomUiNode->addChild(_pDefBarArmtr);
	_pDefBarArmtr->setPosition(_pPlayerDefBar->getPosition());
	_pDefBarArmtr->getAnimation()->play("fuyu");
	_pDefBarArmtr->setVisible(false);

	_pSkillBg = LayerColor::create(Color4B::BLACK, 640.f * 6, visibleSize.height * 2.f);
	this->addChild(_pSkillBg, EntityZ_E::E_Z_BG);
	_pSkillBg->setVisible(false);

	_pFightLayer = FightLayer::create();
	this->addChild(_pFightLayer, EntityZ_E::E_Z_FIGHT_LAYER);

	/*
	auto uiNode = GameCSLoader::createNode("csb/fightUi.csb");
//	uiNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	this->addChild(uiNode, EntityZ_E::E_Z_UI);

	auto uiBottom = uiNode->getChildByName("fight_ui_bottom");
	auto barDef = static_cast<LoadingBar*>(uiBottom->getChildByName("bar_def"));
	barDef->setScale9Enabled(true);
	barDef->setPercent(100.f);

	auto barHp = static_cast<LoadingBar*>(uiBottom->getChildByName("bar_hp"));
	barHp->setScale9Enabled(true);
	barHp->setPercent(70.f);
	*/
	/*
	ParticleManager::getInstance()->addPlistData("particles/buff_huo.plist", "test");
	auto vm = ParticleManager::getInstance()->getPlistData("test");
	auto _hit = ParticleSystemQuad::create(vm);
	_hit->setBlendFunc(BlendFunc::ADDITIVE);
	this->addChild(_hit, 1);
	_hit->setScale(0.3f);
	_hit->setPositionType(ParticleSystem::PositionType::GROUPED);
	_hit->setPosition(Vec2(320.f, 480.f));*/

//	this->startFight();
		
	_bossHpBarBg = Sprite::createWithSpriteFrameName("com_bosshpbar_bg.png");
	this->addChild(_bossHpBarBg);
	_bossHpBarBg->setPosition(Vec2(320.f, VisibleRect::top().y - 100.f));

	_bossHpBar = LoadingBar::create("com_bosshpbar.png", Widget::TextureResType::PLIST, 50);
	this->addChild(_bossHpBar);
	_bossHpBar->setPosition(Vec2(320.f, VisibleRect::top().y - 100.f) + Vec2(28.f, -15.f));

	_bossHpBar->setVisible(false);
	_bossHpBarBg->setVisible(false);
	//_bossHpBar->setScaleY(1.5f);
	//_bossHpBarBg->setScaleY(1.5f);

	//countdownbar
	_timeCountdownBarBG = Sprite::createWithSpriteFrameName("mainui_fightbar_bg.png");
	_timeCountdownBar = LoadingBar::create("mainui_fightbar.png", Widget::TextureResType::PLIST);
	_timeCountdownBar->setPercent(100);

	this->addChild(_timeCountdownBarBG, EntityZ_E::E_Z_UI);
	this->addChild(_timeCountdownBar, EntityZ_E::E_Z_UI);
	
	_timeCountdownBarBG->setPosition(Vec2(320.f, ParamData::CRUSH_BG_HEIGHT));
	_timeCountdownBar->setPosition(Vec2(320.f, ParamData::CRUSH_BG_HEIGHT));

	_timeCountdownBar->setVisible(false);
	_timeCountdownBarBG->setVisible(false);

	initFightInfo();
	updateEquipItems();
	initStoryInfo();
	
	
	intDungeonInfo();
	/*std::vector<int> test;
	test.push_back(0);
	test.push_back(1);
	showDungeonInfo(test);
	getDungeonMissionNode(0)->setMissionNum(30, 30);*/

	this->scheduleUpdate();
    return true;
}

void GameLayer::onEnter()
{
	Layer::onEnter();
	showEnterAni();
}


void GameLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameLayer::update(float delta)
{
	if (nullptr != _pCrushLayer)
	{
		_pCrushLayer->update(delta);
	}

	if (_isNeedUpdateExp && _isExpNodeShow)
	{
		for (int i = 0; i < 4; i++)
		{
			int curlv = UserData::getInstance()->getPlayerCurLv(i);
			const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, curlv);

			float prerect = ((ui::LoadingBar*)(_aniNode[i]->getChildByName("bar")))->getPercent();

			((ui::TextAtlas*)(_aniNode[i]->getChildByName("expnum")))->setString(String::createWithFormat("%d", (int)(info.exp*(100 - prerect) / 100.f))->getCString());
		}
	}
}


void GameLayer::startFight()
{
	if (!GameMap::getCurGameMap()->isInFight())
	{
		return;
	}

	_skillAnimCount = 0;
	_expTimes = 1;
	_isNeedGrow = true;
	_isStartFight = true;
	//init _fightType
	auto fightType = GameMap::getCurGameMap()->getCurFightType();
	auto strLv = String::createWithFormat("stage_%d", _sceneId);

	if (FightType::NOR == fightType)
	{
		_fightType = 0;
		_sceneId = WordMap::getInstance()->getCurPosIdx();

		_isNeedGrow = false;
		//remove monster grow
		/*
		if (1 != UserData::getInstance()->getIsBossPass(_sceneId))
		{
			_isNeedGrow = false;
		}*/

		if (0 == _sceneId && UserData::getInstance()->isNeedGuide())
		{
			_isGuiding = true;

			for (int i = 0; i < ParamData::ROLE_COUNT; i++)
			{
				int roleLv = UserData::getInstance()->getPlayerCurLv(i);
				auto weaponInfo = WeaponControl::getInstance()->getEquipWenpon(i);

				if (roleLv > 0 || (weaponInfo.id - i * 10 - 500) > 0)
				{
					UserData::getInstance()->setIsNeedGuide(false);
					_isGuiding = false;
					break;
				}
			}
		}
		else
		{
			_isGuiding = false;
		}
	}
	else if (FightType::ENDLESS == fightType)
	{
		_fightType = 1;
		_sceneId = -1;
	}
	else if (FightType::HARD == fightType)
	{
		_fightType = 2;
		_sceneId = WordMap::getInstance()->getCurPosIdx(); 
		strLv = String::createWithFormat("stage_%d_%d", _fightType, _sceneId);
	}
	else if (FightType::DUNGEON == fightType)
	{
		_fightType = 3;
		_sceneId = -2;
		strLv = String::createWithFormat("stage_%d_%d", _fightType, GameMap::getCurGameMap()->getDungeonIdx());
	}

	cocos2dx_analyze::startLevel(strLv->getCString());

	_pFightLayer->startFight();
	_pDefBarArmtr->setVisible(true);

	_pCrushLayer = CrushLayer::create();
	this->addChild(_pCrushLayer, EntityZ_E::E_Z_CRUSH_LAYER);
	_pCrushLayer->setVisible(false);

	GameUtils::stopBackgroundMusic(false);

	int soundId = ToolsUtil::getRandomInt(1, 2);
	auto strSoundName = __String::createWithFormat("zhandou%d.mp3", soundId);
	GameUtils::playBackgroundMusic(strSoundName->getCString(), true);
//	_pCrushLayer->setOpacity(0);
//	_pCrushLayer->runAction(FadeIn::create(2.f));
}


void GameLayer::stopFight()
{
	if (_isStartFight)
	{
		_isStartFight = false;
		if (nullptr != GuideLayer::getInstance())
		{
			GuideLayer::getInstance()->removeFromParent();
			_isGuiding = false;
		}

		_pFightLayer->stopFight();
		_pDefBarArmtr->setVisible(false);
		_pCrushLayer->removeFromParent();
		_pCrushLayer = nullptr;

		this->setEquipBtnEnabled(true);
	}
}


void GameLayer::endStory(int index)
{
	if (0 == index)
	{
		CrushLayer::getInstance()->setVisible(true);
		FightLayer::getInstance()->setCurFightState(FightState::Fight);

		if (_isGuiding)
		{
			auto pGuideLayer = GuideLayer::create();
			this->addChild(pGuideLayer, EntityZ_E::E_Z_GUIDE);
			pGuideLayer->onTriggerEvent(0);
		}

		if (1 == GameLayer::getInstance()->getFightType())
		{
			//start round count down
			GameLayer::getInstance()->startBarCountdown();
		}
		FightLayer::getInstance()->startRound(0);
	}
	else if (1 == index)
	{
		CrushLayer::getInstance()->setVisible(true);
		FightLayer::getInstance()->setCurFightState(FightState::Fight);
	}
	else if (2 == index)
	{
		FightLayer::getInstance()->setGameState(2);
	}
}


void GameLayer::backToWorldMap()
{
	Player::getInstance()->resetBarData();
}


void GameLayer::pauseGame()
{
	ToolsUtil::pauseNodeAndChildren(this);
}


void GameLayer::resumeGame()
{
	ToolsUtil::resumeNodeAndChildren(this);
}

void GameLayer::setBossHp(float hp, float percent)
{
	if (3 == _fightType)
	{
		return;
	}

	if (hp == 0)
	{
		_bossHpBar->setVisible(false);
		_bossHpBarBg->setVisible(false);
	}

	if (percent == 100)
	{
		_bossHpBar->setVisible(true);
		_bossHpBarBg->setVisible(true);

		_bossHpBar->setPercent(100);
	}

	auto actionbar = GLProgressTo::create(0.25f, percent);
	_bossHpBar->stopAllActions();
	_bossHpBar->runAction(actionbar);
}

void GameLayer::setHp(float hp, float percent)
{
	float dur = 0.25f;
	float hpValue = hp;
	if (hpValue > 0.f && hpValue < 1.f)
	{
		hpValue = 1.f;
	}
	
	if (percent == 100)
	{
		_lastHp = hpValue;
	}

	
	auto actionNum = GLTextNumTo::create(dur, _lastHp, hpValue);

	auto strContent = __String::createWithFormat("%d", (int)(hpValue));
	//_pHpLbl->setString(strContent->getCString());
	_pHpLbl->stopAllActions();
	_pHpLbl->runAction(actionNum);

	auto actionbar = GLProgressTo::create(dur, percent);
	_pPlayerHpBar->stopAllActions();
	_pPlayerHpBar->runAction(actionbar);

	if (percent < 50)
	{
		//GameMap::getCurGameMap()->showLowHpEffect(true);
	}
	else
	{
		//GameMap::getCurGameMap()->showLowHpEffect(false);
	}

	_lastHp = hpValue;
}

void GameLayer::setDef(float def, float percent)
{
	float dur = 0.25f;
	float defValue = def;
	if (defValue > 0.f && defValue < 1.f)
	{
		defValue = 1.f;
	}
	
	if (percent == 100)
	{
		_lastDp = defValue;
	}

	if (0.f == def)
	{
		_pBarIconDef->setColor(Color3B::GRAY);
	}
	else
	{
		_pBarIconDef->setColor(Color3B::WHITE);
	}

	auto actionNum = GLTextNumTo::create(dur, _lastDp, defValue);
	auto strContent = __String::createWithFormat("%d", (int)(defValue));
	_pDefLbl->stopAllActions();
	_pDefLbl->runAction(actionNum);
	//_pDefLbl->setString(strContent->getCString());

	auto actionbar = GLProgressTo::create(dur, percent);
	_pPlayerDefBar->stopAllActions();
	_pPlayerDefBar->runAction(actionbar);

	_lastDp = defValue;
}


void GameLayer::playBarIconAnim(int animType)
{
	float dur = 0.2f;
	if (0 == animType)
	{
		_pBarIconHp->runAction(Sequence::create(ScaleTo::create(dur, 1.4f), ScaleTo::create(dur, 1.f), nullptr));
	}
	else if (1 == animType)
	{
		_pBarIconDef->runAction(Sequence::create(ScaleTo::create(dur, 1.4f), ScaleTo::create(dur, 1.f), nullptr));
	}
}


void GameLayer::switchInfoDisPlay(bool show)
{
	_bottomUiNode->setVisible(show);
}


void GameLayer::updateEquipItems()
{
	for (int i = 0; i < 4; i++)
	{
		auto idx = BagItemControl::getInstace()->getEquipItemsByIdx(i);
		if (idx != -1)
		{
			auto config = ParamMgr::getInstance()->getItemByID(idx);

			if (_spriteEquipBg[i] == nullptr)
			{
				_spriteEquipBg[i] = Sprite::createWithSpriteFrameName("com_hp_shadow.png");
				_bottomUiNodeR->addChild(_spriteEquipBg[i], 1);
				_spriteEquipBg[i]->setPosition(Vec2(390.f + 70.f * i, 588.f));
			}

			if (_arrEquipItems[i] == nullptr)
			{
				auto btn = Button::create(config.picname, "", "", Widget::TextureResType::PLIST);
				_arrEquipItems[i] = btn;

				_bottomUiNodeR->addChild(btn, 2);
				btn->setPosition(Vec2(390.f + 70.f * i, 620.f));
				btn->setUserData((void*)(i));
				btn->addTouchEventListener(CC_CALLBACK_2(GameLayer::itemsCallback, this));
			}
			else
			{
				_arrEquipItems[i]->loadTextures(config.picname, "", "", Widget::TextureResType::PLIST);
			}
		}
		else
		{
			if (_arrEquipItems[i] != nullptr)
			{
				_arrEquipItems[i]->removeFromParent();
				_arrEquipItems[i] = nullptr;
			}

			if (_spriteEquipBg[i] != nullptr)
			{
				_spriteEquipBg[i]->removeFromParent();
				_spriteEquipBg[i] = nullptr;
			}
		}

	}
}


void GameLayer::itemsCallback(cocos2d::Ref* pSender, Widget::TouchEventType type)
{	
	if (type == Widget::TouchEventType::ENDED)
	{
		if (!GameMap::getCurGameMap()->isInFight() 
			|| nullptr == CrushLayer::getInstance() 
			|| !CrushLayer::getInstance()->isVisible() 
			|| CrushLayer::getInstance()->getShieldLayer()->isVisible()
			|| !CrushLayer::getInstance()->getStateMac()->isInState(WaitTouchState::getInstance())
			|| 3 == _fightType)
		{
			return;
		}

		if (_isGuiding)
		{
			GameUtils::toastTip("guide_unable_item");
			return;
		}

		int idx = (int)(((Node*)(pSender))->getUserData());
		auto itemid = BagItemControl::getInstace()->getEquipItemsByIdx(idx);
		auto config = ParamMgr::getInstance()->getItemByID(itemid);
		CCLOG("use item: %d, type:%d, value :%d", idx, config.type, config.value);
		BagItemControl::getInstace()->delBagItem(14 + idx, 0);
		((Node*)(pSender))->removeFromParent();
		_arrEquipItems[idx] = nullptr;
		_spriteEquipBg[idx]->removeFromParent();
		_spriteEquipBg[idx] = nullptr;

		if (BagItemControl::getInstace()->checkItemType(itemid) == 0)
		{
			cocos2dx_analyze::use(String::createWithFormat("itemid_good_item_%d", itemid)->getCString(), 1, 0);
			MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::PROPUSE, 1);
		}

		Vec2 worldPos = ((Node*)(pSender))->convertToWorldSpaceAR(Vec2::ZERO);
		Vec2 actPos = this->convertToNodeSpace(worldPos);

		switch (config.type)
		{
		case 0:
			//add hp
			Player::getInstance()->addHp(config.value);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP);
			this->playItemEffect("hongsegq", actPos);
			break;
		case 1:
			//add def
			Player::getInstance()->addDef(config.value);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_DEF);
			this->playItemEffect("lvsegq", actPos);
			break;
		case 2:
			//add hp and def
			Player::getInstance()->addHp(config.value);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP);
			Player::getInstance()->addDef(config.value);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_DEF);
			this->playItemEffect("zisegq", actPos);
			break;
		case 3:
			//clean lv 0
			Player::getInstance()->doClean(0, config.value, 1);
			this->playItemEffect("xxsd", actPos);
			break;
		case 4:
			//clean lv 1
			Player::getInstance()->doClean(1, config.value, 1);
			this->playItemEffect("xxsd", actPos);
			break;
		case 5:
			//clean lv 3
			Player::getInstance()->doClean(2, config.value, 1);
			this->playItemEffect("xxsd", actPos);
			break;
		case 6:
			//random s skill
			SpecialAttUtils::createRandomS();
			this->playItemEffect("suijis", actPos);
			break;
		case 7:
			//add exp 
			_expTimes++;
			this->playItemEffect("suijis", actPos);
			break;
		}

		CrushLayer::getInstance()->getStateMac()->changeState(ItemRoundState::getInstance());
		DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYUSEITEMS);
	}	
}


void GameLayer::setEquipBtnEnabled(bool enable)
{
	for (int i = 0; i < 4; i++)
	{
		if (nullptr != _arrEquipItems[i])
		{
			_arrEquipItems[i]->setEnabled(enable);
		}
	}
}

void GameLayer::initStoryInfo()
{
	auto _mapStartY = 40.f;
	auto _mapOffpos = 0.f;
	if ((VisibleRect::top().y / VisibleRect::right().x) > (1160.f / 640.f))
	{
		_mapOffpos = VisibleRect::top().y - 580.f - _mapStartY;
	}
	else
	{
		_mapOffpos = 580.f - _mapStartY;
	}

	_storyListView = ui::ListView::create();
	_storyListView->setItemsMargin(20.f);

	this->addChild(_storyListView);
	_storyListView->setContentSize(Size(600.f, 550.f));
	_storyListView->setClippingEnabled(false);
	_storyListView->setPosition(Vec2(20.f, _mapOffpos + _mapStartY - 570.f));
	_storyListView->setVisible(false);
	_storyListView->setScrollBarEnabled(false);
}


void GameLayer::showSelectMenu(Node* menu)
{
	if (_selectmenu)
	{
		_selectmenu->removeFromParent();
	}
	_selectmenu = menu;
	_selectmenu->setPositionY(400.f);
	_selectmenu->setPositionX(320.f);

	this->addChild(_selectmenu, E_Z_CRUSH_LAYER + 2);
}


void GameLayer::removeSelectMenu()
{
	if (_selectmenu)
	{
		_selectmenu->removeFromParent();
		_selectmenu = nullptr;
	}
}


void GameLayer::contitueStoryAfterSelect(int next)
{
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.01f), CallFunc::create([=]() {
		StoryMgr::getInstance()->getCommandMgrByIdx(_curStorytype)->_pause = false;
		StoryMgr::getInstance()->getCommandMgrByIdx(_curStorytype)->_commondidx = next;
		StoryMgr::getInstance()->getCommandMgrByIdx(_curStorytype)->executeCommond();
	}));
	this->runAction(action);	
}


void GameLayer::showReward(std::vector<Reward_T> arr)
{
	auto reard = PopRewardLayer::create(-100, arr[0].num);
	MainLayer::getCurMainLayer()->addChild(reard, MainLayer_Z::POP_REWARD_Z);

	/*auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layer, E_Z_CRUSH_LAYER + 3);

	auto posnode = Node::create();
	layer->addChild(posnode);

	posnode->setPosition(320.f -130.f * (arr.size() - 1) / 2.f, VisibleRect::top().y /2.f);

	int i = 0;
	for (auto&temp : arr)
	{
		float posx = 130.f * i;

		auto effectsp = Sprite::createWithSpriteFrameName("box_bg_2.png");
		posnode->addChild(effectsp);
		effectsp->setPosition(posx, 0.f);

		auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
		effectsp->runAction(actionrotate);

		auto& item = ParamMgr::getInstance()->getItemByID(temp.itemid);

		auto  icon = Sprite::createWithSpriteFrameName(item.picname);
		posnode->addChild(icon);
		icon->setPosition(posx, 0.f);

		auto num = ui::TextAtlas::create(String::createWithFormat(".%d", temp.num)->getCString(), "fonts/num_boxreward.png", 18.f, 28.f, ".");
		posnode->addChild(num);
		num->setPosition(Vec2(posx, -40.f));

		i++;

		for (int j = 0; j < temp.num; j++)
		{
			BagItemControl::getInstace()->addBagItem(temp.itemid);
		}
	}

	BagItemControl::getInstace()->saveItemConfig();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch * touch, Event * unusedEvent) {
		return true;
	};

	listener->onTouchEnded = [=](Touch * touch, Event * unusedEvent) {layer->removeFromParent(); };

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);*/
}


void GameLayer::storyend()
{
	_storyListView->setVisible(false);
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]() {
		if (_touchStoryListener)
		{
			_eventDispatcher->removeEventListener(_touchStoryListener);
			_touchStoryListener = nullptr;
			_storyListView->setVisible(false);
		}
	}));

	this->runAction(action);


}


void GameLayer::showStory(int posidx)
{
	_curStorytype = posidx;

	if (_storyTouchProxyNode == nullptr)
	{
		_storyTouchProxyNode = Node::create();
		this->addChild(_storyTouchProxyNode, EntityZ_E::E_Z_CRUSH_LAYER + 1);
	}
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]() {
		_storyListView->removeAllItems();
		StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->executeCommond();
		_storyListView->setVisible(true);
		_touchStoryListener = EventListenerTouchOneByOne::create();
		_touchStoryListener->setSwallowTouches(true);
		_touchStoryListener->onTouchBegan = [=](Touch*, Event*) {
			return true;
		};
		_touchStoryListener->onTouchEnded = [=](Touch*, Event*) {
			if (!StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->_isExecuting)
			{
				StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->executeCommond();
			}
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchStoryListener, _storyTouchProxyNode);
	}));

	this->runAction(action);

	GameLayer::getInstance()->switchInfoDisPlay(false);
	MainLayer::getCurMainLayer()->getPauseBtn()->setVisible(false);
}


void GameLayer::initCombo()
{
	_comboNode = Node::create();
	this->addChild(_comboNode,10);
	_comboNode->setPosition(Vec2(550.f, VisibleRect::top().y - 130.f));
	//_comboNode->setVisible(false);

	auto sprite = Sprite::createWithSpriteFrameName("combo_bg.png");
	_comboNode->addChild(sprite);

	auto numAtlas = ui::TextAtlas::create("110", "fonts/num_combo.png", 50.f, 70.f, ".");
	_comboNode->addChild(numAtlas);
	numAtlas->setAnchorPoint(Vec2(0.f, 0.5f));
	numAtlas->setPosition(Vec2(-65.f, 0.f));
	numAtlas->setName("combonum");
}


void GameLayer::addCombo()
{
	_comboNum++;
	if (_comboNum >= 2)
	{
		_comboNode->setVisible(true);
		for (auto &temp : _comboNode->getChildren())
		{
			temp->stopAllActions();
			temp->setOpacity(255);
			temp->runAction(FadeOut::create(2.f));
		}
	}

	auto text = static_cast<TextAtlas*>(_comboNode->getChildByName("combonum"));
	text->setString(String::createWithFormat("%d", _comboNum)->getCString());

	auto actionsclae = ScaleTo::create(0.2f, 1.f);
	actionsclae->setTag(456);

	text->stopActionByTag(456);
	text->setScale(1.3f);
	text->runAction(actionsclae);
}


void GameLayer::resetCombo()
{
	_comboNum = 0;
	//_comboNode->setVisible(false);
	//_comboNode->setVisible(true);
}


void GameLayer::showExpAdd(int exp0, int exp1, int exp2, int exp3)
{
	initUpdateNode();
	showUpdateAni(0, exp0);
	showUpdateAni(1, exp1);
	showUpdateAni(2, exp2);
	showUpdateAni(3, exp3);
}


void GameLayer::initUpdateNode()
{
	if (_isExpNodeShow)
	{
		return;
	}

	float _mapOffpos = 0;
	if ((VisibleRect::top().y / VisibleRect::right().x) > (1160.f / 640.f))
	{
		_mapOffpos = VisibleRect::top().y - 1160 + 660.f;
	}
	else
	{
		_mapOffpos = 660;
	}

	for (int i = 0; i < 4; i++)
	{
		_aniNode[i] = GameCSLoader::createNode("csb/winupdate.csb");
		this->addChild(_aniNode[i], 10);
		_aniNode[i]->setPosition(Vec2(90.f + (3-i) * 150.f, _mapOffpos));

		int curlv = UserData::getInstance()->getPlayerCurLv(i);
		const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, curlv);
		((ui::TextAtlas*)(_aniNode[i]->getChildByName("expnum")))->setString(String::createWithFormat("%d", info.exp - UserData::getInstance()->getPlayerCurExp(i))->getCString());
		float percent = UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp;
		((ui::LoadingBar*)(_aniNode[i]->getChildByName("bar")))->setPercent(percent);

		auto textadd = ui::TextAtlas::create("/0", "fonts/num_score_exp.png", 20.f, 26.f, ".");
		_aniNode[i]->addChild(textadd);
		textadd->setPosition(Vec2(40.f, 40.f));
		textadd->setName("expadd");

		//_arrexpbak[i] = 0;
	}

	_isExpNodeShow = true;
}


void GameLayer::removeUpdateNode()
{
	if (!_isExpNodeShow)
	{
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		if (_aniNode[i])
		{
			_aniNode[i]->removeFromParent();
			_aniNode[i] = nullptr;
		}	
	}

	_isExpNodeShow = false;
}


void GameLayer::showUpdateAni(int idx, float exp)
{
	_isNeedUpdateExp = true;
	int i = idx;
	int expbak = exp;
	auto nodebar = _aniNode[i]->getChildByName("bar");
	
	static int actionCount = 1;

	float time = 4.f;
	int curlv = UserData::getInstance()->getPlayerCurLv(i);
	const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, curlv);
	float percent = UserData::getInstance()->getPlayerCurExp(i) * 100.f / info.exp;
	float needexp = info.exp - UserData::getInstance()->getPlayerCurExp(i);
	float Curneedexp = needexp;

	int lvup = 0;
	while (Curneedexp <= exp && curlv + lvup <= ParamMgr::getInstance()->getPlayerMaxLv())
	{
		lvup++;
		exp = exp - Curneedexp;
		Curneedexp = ParamMgr::getInstance()->getPlayerInfo(i, curlv+lvup).exp;
	}

	float time0 = 0.f;
	float timeend = 0.f;

	if (curlv >= ParamMgr::getInstance()->getPlayerMaxLv())
	{
		_aniNode[i]->setVisible(false);
	}
	else if (lvup == 0.f)
	{
		time0 = exp / info.exp * time;

		auto actionstart = GLProgressTo::create(time0, percent + time0 / time * 100.f);
		nodebar->runAction(Sequence::createWithTwoActions(actionstart, CallFunc::create([=]() {
			//UserData::getInstance()->addPlayerCurExp(i, exp);
			//UserData::getInstance()->saveUserData();
			((ui::TextAtlas*)(_aniNode[i]->getChildByName("expnum")))->setString(String::createWithFormat("%d", (int)(needexp-exp))->getCString());
			_isNeedUpdateExp = false; })));
	}
	else if(lvup == 1)
	{
		time0 = needexp/ info.exp * time;
		timeend = exp / Curneedexp * time;

		auto actionstart = GLProgressTo::create(time0, percent + time0 / time * 100.f);
		auto actionend = GLProgressFromTo::create(timeend, 0, timeend / time * 100.f);
		nodebar->runAction(Sequence::create(actionstart, CallFunc::create([=]() {
				showAttributeAdd(i, curlv, curlv + lvup);
				//UserData::getInstance()->addPlayerCurExp(i, needexp);
			}), actionend,CallFunc::create([=]() {
				//UserData::getInstance()->addPlayerCurExp(i, exp);
				//UserData::getInstance()->saveUserData();
				((ui::TextAtlas*)(_aniNode[i]->getChildByName("expnum")))->setString(String::createWithFormat("%d", (int)(Curneedexp - exp))->getCString());
				_isNeedUpdateExp = false; 
			}), nullptr));
	}
	else
	{
		time0 = needexp / info.exp * time;
		timeend = exp / Curneedexp * time;

		auto actionstart = GLProgressTo::create(time0, percent + time0 / time * 100.f);
		auto actionend = GLProgressFromTo::create(timeend, 0, timeend / time * 100.f);
		auto actionall = GLProgressFromTo::create(time, 0, 100.f);

		auto actiondur = Repeat::create(Sequence::createWithTwoActions(actionall, CallFunc::create([=]() {
			actionCount++;
			showAttributeAdd(i, curlv + actionCount -1, curlv + actionCount);
			const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(i, curlv + actionCount);
			//UserData::getInstance()->addPlayerCurExp(i, info.exp);
		})), lvup - 1);

		nodebar->runAction(Sequence::create(actionstart, CallFunc::create([=]() {
			showAttributeAdd(i, curlv, curlv + 1);
			//UserData::getInstance()->addPlayerCurExp(i, needexp);
		}),actiondur, actionend, CallFunc::create([=]() {
			//UserData::getInstance()->addPlayerCurExp(i, exp);
			//UserData::getInstance()->saveUserData();

			((ui::TextAtlas*)(_aniNode[i]->getChildByName("expnum")))->setString(String::createWithFormat("%d", (int)(Curneedexp - exp))->getCString());
			_isNeedUpdateExp = false;
		}), nullptr));
	}

	if (UserData::getInstance()->addPlayerCurExp(i, expbak))
	{
		int maxLv = 0;
		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int tmpLv = UserData::getInstance()->getPlayerCurLv(i);
			maxLv = MAX(maxLv, tmpLv);
		}

		auto strLv = String::createWithFormat("%d", maxLv);
		cocos2dx_analyze::setUserLevel(strLv->getCString());
	}

	UserData::getInstance()->saveUserData();

	auto timeall = time0 + timeend + ((lvup - 1)*time > 0 ? (lvup - 1)*time : 0);
	auto actiontext = GLTextNumTo::create(timeall, 0, expbak, "/%d");
	((ui::TextAtlas*)(_aniNode[i]->getChildByName("expadd")))->runAction(actiontext);
}

void GameLayer::showLvUpLight(int idx)
{
	float pox = _aniNode[idx]->getPositionX();
	float poy = _aniNode[idx]->getPositionY() + 30.f;

	auto arm = Armature::create("lv_up");
	arm->getAnimation()->play("lv_up");
	arm->setPosition(pox, poy);

	this->addChild(arm);

	auto actionremove = Sequence::createWithTwoActions(DelayTime::create(2.f), RemoveSelf::create());
	arm->runAction(actionremove);
}

void GameLayer::showAttributeAdd(int idx, int curlv, int tarlv)
{
	float pox = _aniNode[idx]->getPositionX() - 45.f;
	float poy = _aniNode[idx]->getPositionY() + 200.f;
	float offy = 100.f;
	float timeoff = 2.f;

	auto sphp = Sprite::createWithSpriteFrameName("mainui_shuxinicon_6.png");
	auto spdp = Sprite::createWithSpriteFrameName("mainui_shuxinicon_1.png");
	auto spatt = Sprite::createWithSpriteFrameName("mainui_shuxinicon_3.png");
	
	auto labhp =  ui::TextAtlas::create("/1", "fonts/num_score_exp.png", 20.f, 26.f, ".");
	sphp->addChild(labhp);
	labhp->setAnchorPoint(Vec2(0.f, 0.5f));
	labhp->setPosition(Vec2(50.f, 20.f));

	auto labdp = ui::TextAtlas::create("/1", "fonts/num_score_exp.png", 20.f, 26.f, ".");
	spdp->addChild(labdp);
	labdp->setAnchorPoint(Vec2(0.f, 0.5f));
	labdp->setPosition(Vec2(50.f, 20.f));

	TextAtlas* labatt = ui::TextAtlas::create("/1", "fonts/num_score_exp.png", 20.f, 26.f, ".");
	spatt->addChild(labatt);
	labatt->setAnchorPoint(Vec2(0.f, 0.5f));
	labatt->setPosition(Vec2(50.f, 20.f));

	const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(idx, curlv);
	const PlayerInfo_T& info2 = ParamMgr::getInstance()->getPlayerInfo(idx, tarlv);

	int offhp = info2.hp - info.hp;
	int offdp = info2.dp - info.dp;
	int offatt = 0;
	offatt = info2.attack - info.attack;
	if (idx == 2)
	{
		spatt->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_2.png"));	
	}
	else if (idx == 3)
	{
		spatt->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_10.png"));
		offatt = info2.hpadd - info.hpadd;
	}
	else if (idx == 1)
	{
		spatt->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_shuxinicon_0.png"));
		offatt = info2.attack - info.attack;

		auto sp0 = Sprite::createWithSpriteFrameName("mainui_shuxinicon_4.png");
		auto sp1 = Sprite::createWithSpriteFrameName("mainui_shuxinicon_5.png");

		TextAtlas* labsp0 = ui::TextAtlas::create("/1", "fonts/num_score_exp.png", 20.f, 26.f, ".");
		sp0->addChild(labsp0);
		labsp0->setAnchorPoint(Vec2(0.f, 0.5f));
		labsp0->setPosition(Vec2(50.f, 20.f));
		int off = info2.def - info.def;
		labsp0->setString(String::createWithFormat("/%d", off)->getCString());

		TextAtlas* labsp1 = ui::TextAtlas::create("/1", "fonts/num_score_exp.png", 20.f, 26.f, ".");
		sp1->addChild(labsp1);
		labsp1->setAnchorPoint(Vec2(0.f, 0.5f));
		labsp1->setPosition(Vec2(50.f, 20.f));
		off = info2.dpadd - info.dpadd;
		labsp1->setString(String::createWithFormat("/%d", off)->getCString());

		sp0->setPosition(Vec2(pox, poy - 120.f));
		sp0->runAction(Sequence::create(MoveBy::create(timeoff, Vec2(0.f, offy)), RemoveSelf::create(), nullptr));

		sp1->setPosition(Vec2(pox, poy - 160.f));
		sp1->runAction(Sequence::create(MoveBy::create(timeoff, Vec2(0.f, offy)), RemoveSelf::create(), nullptr));

		this->addChild(sp0, 11);
		this->addChild(sp1, 11);
	}

	labhp->setString(String::createWithFormat("/%d", offhp)->getCString());
	labdp->setString(String::createWithFormat("/%d", offdp)->getCString());
	labatt->setString(String::createWithFormat("/%d", offatt)->getCString());

	this->addChild(sphp, 11);
	this->addChild(spdp, 11);
	this->addChild(spatt, 11);

	sphp->setPosition(Vec2(pox, poy));
	sphp->runAction(Sequence::create(MoveBy::create(timeoff, Vec2(0.f, offy)),RemoveSelf::create(), nullptr));

	spdp->setPosition(Vec2(pox, poy - 40.f));
	spdp->runAction(Sequence::create(MoveBy::create(timeoff, Vec2(0.f, offy)), RemoveSelf::create(), nullptr));

	spatt->setPosition(Vec2(pox, poy - 80.f));
	spatt->runAction(Sequence::create(MoveBy::create(timeoff, Vec2(0.f, offy)), RemoveSelf::create(), nullptr));

	showLvUpLight(idx);
}

void GameLayer::initFightInfo()
{
	if (!_fightInfoNode)
	{
		_fightInfoNode = Node::create();
		this->addChild(_fightInfoNode, 10);
		_fightInfoNode->setPosition(Vec2(490.f, VisibleRect::top().y - 40.f));
		//auto bg = Sprite::createWithSpriteFrameName("fight_stage_bg.png");
		//_fightInfoNode->addChild(bg);

		//auto icon = Sprite::createWithSpriteFrameName("fight_stage_icon.png");
		//_fightInfoNode->addChild(icon);
		//icon->setPositionX(-65.f);

		/*auto text = TextAtlas::create("5/10", "fonts/font_guaqia.png", 16.f, 20.f, ".");
		_fightInfoNode->addChild(text);
		text->setName("num");*/

		auto textname = Text::create();
		textname->setAnchorPoint(Vec2(1.f, 0.5));
		textname->setFontSize(24);
		textname->setName("text");
		textname->setString("");
		_fightInfoNode->addChild(textname);
		textname->setPositionX(-250.f);

		_pFloorTitle = Label::createWithSystemFont("", "fonts/Marker Felt.ttf", 24);
//		this->addChild(_pFloorTitle, 10);
//		_pFloorTitle->setPosition(Vec2(490.f, VisibleRect::top().y - 40.f));
		_fightInfoNode->addChild(_pFloorTitle);

		auto barbg = Sprite::createWithSpriteFrameName("com_guaiqia_bg.png");
		_fightInfoNode->addChild(barbg);
		barbg->setPosition(Vec2(8.f, 5.f) + Vec2(-40.f, 0.f));
		barbg->setName("barbg");

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/jindutiao.ExportJson");

		auto logo0 = Armature::create("jindutiao");
		logo0->getAnimation()->play("jindutiao2");

		auto bar = glui::GLSlidBar::create("com_guanqia.png", logo0, Widget::TextureResType::PLIST);
		bar->setName("glbar");
		_fightInfoNode->addChild(bar);
		bar->setPositionX(-40.f);

		bar->setPercent(50);
		bar->getPrecentMarkNode()->setPositionY(bar->getPrecentMarkNode()->getPositionY() + 8.f);

		//auto actionbar = GLProgressTo::create(4, 100, 1);
		//bar->stopAllActions();
		//bar->runAction(actionbar);

		_fightInfoNode->setVisible(false);
		_pFloorTitle->setVisible(false);
	}
}

void GameLayer::setFightName(const std::string& name)
{
	if (_fightInfoNode)
	{
		//static_cast<Text*>(_fightInfoNode->getChildByName("text"))->setString(name);
	}
}

void GameLayer::setCurFightIndex(int cur, int all, float time)
{
	if (_fightInfoNode)
	{
		//static_cast<TextAtlas*>(_fightInfoNode->getChildByName("num"))->setString(String::createWithFormat("%d/%d", cur, all)->getCString());

		auto bar = static_cast<glui::GLSlidBar*>(_fightInfoNode->getChildByName("glbar"));
//		auto txtFloor = static_cast<Text*>(_fightInfoNode->getChildByName("text"));
		auto barBg = _fightInfoNode->getChildByName("barbg");


		if (1 == _fightType)
		{
			_pFloorTitle->setVisible(true);
//			txtFloor->setVisible(true);
			bar->setVisible(false);
			barBg->setVisible(false);

			auto strRes = ResMgr::getInstance()->getString("floor_title");
			auto strNum = __String::createWithFormat("%d", cur);
			std::string strTitle = strRes->getCString();
			ToolsUtil::stringFindReplace(strTitle, "N", strNum->getCString());
			_pFloorTitle->setString(strTitle);
		}
		else
		{
//			txtFloor->setVisible(false);
			_pFloorTitle->setVisible(false);
			bar->setVisible(true);
			barBg->setVisible(true);
			static_cast<Armature*>(bar->getPrecentMarkNode())->getAnimation()->play("jindutiao1");

			if (cur == 0)
			{
				bar->setPercent(0);
			}
			else
			{
				auto action = Sequence::createWithTwoActions(GLProgressTo::create(time, ((float)cur) / all * 100.f, 1), CallFunc::create([=]() {static_cast<Armature*>(bar->getPrecentMarkNode())->getAnimation()->play("jindutiao2"); }));
				bar->runAction(action);
			}

		}

	}
}


void GameLayer::playItemEffect(const std::string &animName, const Vec2 &pos)
{
	auto pArmtr = GameArmtr::createItemEffect();
	pArmtr->setPosition(pos);
	this->addChild(pArmtr, EntityZ_E::E_Z_FIGHT_LAYER);

	pArmtr->play(animName);
	pArmtr->runAction(Sequence::create(DelayTime::create(2.f), RemoveSelf::create(), nullptr));
}

void GameLayer::loadImageCallback(Texture2D *pTexture, LoadResInfo_T resInfo, int param1, int parma2)
{
	if (nullptr != pTexture)
	{
		if (0 == param1)
		{
			bool isEquip = false;
			for (int i = 0; i < 4; i++)
			{
				isEquip = SkillControl::getInstance()->checkIsEquipSkill(i, parma2);
				if (isEquip)
				{
					break;
				}
			}

			if (!isEquip)
			{
				return;
			}
		}
		__String *strPlistName = __String::createWithFormat("%s.plist", resInfo.filePath.c_str());
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(strPlistName->getCString());
	}
}

void GameLayer::showEnterAni()
{
	_bottomUiNode->setPositionX(-200.f);

	auto actionMoveInL = EaseSineOut::create(MoveTo::create(0.5f, Vec2(0.f, 0.f)));
	_bottomUiNode->runAction(actionMoveInL);

	_bottomUiNodeR->setPositionX(200.f);
	auto actionMoveInR = EaseSineOut::create(MoveTo::create(0.5f, Vec2(0.f, 0.f)));
	_bottomUiNodeR->runAction(actionMoveInR);
}

void GameLayer::playSkillBgAnim(int animType)
{
	if (0 == animType)
	{
		_pSkillBg->setVisible(true);
		if (0 == _skillAnimCount)
		{
			_pSkillBg->setOpacity(0);
			_pSkillBg->stopAllActions();
			_pSkillBg->runAction(FadeTo::create(0.3f, 180.f));
		}
		_skillAnimCount++;
	}
	else
	{
		_skillAnimCount--;
		if (0 == _skillAnimCount)
		{
			_pSkillBg->stopAllActions();
			_pSkillBg->runAction(Sequence::create(FadeOut::create(0.3f), Hide::create(), nullptr));
		}
	}
}


void GameLayer::startBarCountdown()
{
	_timeCountdownBar->setVisible(true);
	_timeCountdownBarBG->setVisible(true);

	_timeCountdownBar->stopAllActions();
	_timeCountdownBar->setPercent(100);

	auto action = GLProgressTo::create(6.f, 0);
	_timeCountdownBar->runAction(Sequence::createWithTwoActions(action, CallFunc::create(CC_CALLBACK_0(GameLayer::barCountDownCB, this))));
}


void GameLayer::endBarCountdown()
{
	_timeCountdownBar->setVisible(false);
	_timeCountdownBarBG->setVisible(false);

	_timeCountdownBar->stopAllActions();
}


void GameLayer::barCountDownCB()
{
	CrushLayer::getInstance()->getShieldLayer()->setVisible(true);
	CrushLayer::getInstance()->initRoundData();
	CrushLayer::getInstance()->getStateMac()->changeState(MonsterAttState::getInstance());
}


void GameLayer::popReborn()
{
	string bossRebornKey[] = { StoreAssetMgr::ITEMID_GOOD_REBORN_4, StoreAssetMgr::ITEMID_GOOD_REBORN_2, StoreAssetMgr::ITEMID_GOOD_REBORN_2, StoreAssetMgr::ITEMID_GOOD_REBORN_1 };
	string comRebornKey[] = { StoreAssetMgr::ITEMID_GOOD_REBORN_4, StoreAssetMgr::ITEMID_GOOD_REBORN_1, StoreAssetMgr::ITEMID_GOOD_REBORN_2, StoreAssetMgr::ITEMID_GOOD_REBORN_1 , StoreAssetMgr::ITEMID_GOOD_REBORN_2, };

	int idx = WordMap::getInstance()->getCurPosIdx();
	if (WordMap::getInstance()->checkIsBoss(idx))
	{
		int curRebornIdx = UserData::getInstance()->getBossStagesRebornIdx();
		int next = curRebornIdx + 1 >= 4 ? 0 : curRebornIdx + 1;
		UserData::getInstance()->setBossStagesRebornIdx(next);

		auto layer = PurchaseLayer::create(bossRebornKey[curRebornIdx], "", MainLayer::getCurMainLayer());
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);
	}
	else
	{
		int curRebornIdx = UserData::getInstance()->getBossStagesRebornIdx();
		int next = curRebornIdx + 1 >= 5 ? 0 : curRebornIdx + 1;
		UserData::getInstance()->setComStagesRebornIdx(next);

		auto layer = PurchaseLayer::create(comRebornKey[curRebornIdx], "", MainLayer::getCurMainLayer());
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);
	}
}


void GameLayer::intDungeonInfo()
{
	if (_pDungeonRootNode == nullptr)
	{
		_pDungeonRootNode = Node::create();
		this->addChild(_pDungeonRootNode, EntityZ_E::E_Z_UI);
		_pDungeonRootNode->setPosition(60.f, VisibleRect::top().y - 40.f);

		auto textbs = Sprite::createWithSpriteFrameName("dungeon_text_bs.png");
		_pDungeonRootNode->addChild(textbs);
		textbs->setPositionY(20.f);

		_pStepTextAtlas = TextAtlas::create("40", "fonts/dungeon_font_targetnum.png", 21, 32, ".");
		_pDungeonRootNode->addChild(_pStepTextAtlas);
		_pStepTextAtlas->setPositionY(-20.f);

		_pDungeonRootNode->setVisible(false);

	}
}

void GameLayer::showDungeonInfo(const std::vector<int>& vectorDungeonIdx)
{
	_pDungeonRootNode->setVisible(true);

	for (auto& temp : _vectorDungeonMission)
	{
		temp->removeFromParent();
	}
	_vectorDungeonMission.clear();

	int idx = 0;
	for (auto& temp : vectorDungeonIdx)
	{
		auto mission = DungeonMissionNode::nodeWithIdx(temp);
		_pDungeonRootNode->addChild(mission);
		mission->setPositionX(100.f + 80.f * idx);
		mission->setPositionY(-35.f);
		_vectorDungeonMission.push_back(mission);

		idx++;
	}
}

void GameLayer::setDungeonStep(int idx)
{
	auto action = Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.1f), ScaleTo::create(0.2f, 1.0f));
	_pStepTextAtlas->stopAllActions();
	_pStepTextAtlas->runAction(action);
	_pStepTextAtlas->setString(String::createWithFormat("%d", idx)->getCString());
}

DungeonMissionNode* GameLayer::getDungeonMissionNode(int missionidx)
{
	for (auto& temp : _vectorDungeonMission)
	{
		if (temp->getMissionId() == missionidx)
		{
			return temp;
		}
	}

	return nullptr;
}

void GameLayer::removerDungeonInfo()
{
	_pDungeonRootNode->setVisible(false);
}


void GameLayer::setDungeonMissionNum(int id, int cur, int max)
{
	getDungeonMissionNode(id)->setMissionNum(cur, max);
}