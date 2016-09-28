#include "GameMap.h"
#include "effects/ShaderSprite.h"
#include "GLCommon/UI/TouchesScaleNode.h"
#include "effects/EffectAction.h"
#include "VisibleRect.h"
#include "ShopLayer.h"
#include "MainScene.h"
#include "GLCommon/GLGraph.h"
#include "GLCommon/UI/GLSprite.h"
#include "../effects/RTTNode.h"
#include "../Widgets/GameCSLoader.h"
#include "json/rapidjson.h" 
#include "json/document.h" 
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "../Story/Commond.h"
#include "../Story/StoryMgr.h"
#include "../DataParam/UserData.h"
#include "ParamMgr.h"
#include "GameLayer.h"
#include "../GLCommon/Utils/ResMgr.h"
#include "../Story/StoryMgr.h"
#include "Player.h"
#include "GameUtils.h"
#include "Story/StoryMgr.h"
#include "CrushLayer.h"
#include "FightLayer.h"
#include "MainScene.h"
#include "GLActions/GLActions.h"
#include "Story/StoryMgr.h"
#include "platBridge/cocos2dx_plat.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "BoxDataMgr.h"
#include "PopRewardLayer.h"
#include "../DataParam/DayActiveMgr.h"
#include "../DataParam/NewMapOpenMgr.h"
#include "NewWorldInfoLayer.h"

extern const char* g_boxBtnPicName[7];

#define  EFFECTACTION_TAG 1000

Vec2 cloudPos[10] =
{
	Vec2(-110.f, 440.f),
	Vec2(217.f, 266.f),
	Vec2(-315.f, 115.f),
	Vec2(360.f, -250.f),
	Vec2(-372.f, -362.f),
	Vec2(446.f, 80.f),
	Vec2(-600.f, 5.f),
	Vec2(515.f, 480.f),
	Vec2(-125.f, -75.f),
	Vec2(-450.f, 450.f),
};



enum MapIdx
{
	MapIdx_City = 0,
	MapIdx_Stone
};

GameMap* GameMap::s_curGameMap = nullptr;

GameMap::GameMap() :_autoMove(false),
_curMapNode_0(nullptr),
_nextMapNode_0(nullptr),
_prepareCount(0),
_bgLayer(nullptr),
_scoreNode(nullptr),
_isShowScore(false),
_mapOffpos(0.f),
_isCurMapUp(true),
_isInFight(false),
_pause(false),
_storySkipNode(nullptr),
_fightType(FightType::NOR)
{
	s_curGameMap = this;
}

GameMap::~GameMap()
{
	s_curGameMap = nullptr;
}

GameMap * GameMap::createWithIdx(int idx)
{
	auto pRef = new GameMap();
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

bool GameMap::initWithIdx(int idx)
{
	Layer::init();

	_curDistance = 0.f;
	_moveTotleTime = 0.f;
	_curMoveTime = 0.f;
	_isMove = false;
	_moveSpeed = 0.f;
	_idx = idx;
	_isChangingMap = false;
	_mapStartY = 40.f;

	_resoultExp[0] = 0.f;
	_resoultExp[1] = 0.f;
	_resoultExp[2] = 0.f;
	_resoultExp[3] = 0.f;

	if ((VisibleRect::top().y / VisibleRect::right().x) > (1160.f / 640.f))
	{
		_mapOffpos = VisibleRect::top().y - 580.f - _mapStartY;
	}
	else
	{
		_mapOffpos = 580.f - _mapStartY;
	}

	_spriteBg =ui::Scale9Sprite::create("bgs/main_darkness_bg.png");
	this->addChild(_spriteBg);
	_spriteBg->setPosition(Vec2(320.f, VisibleRect::top().y/2.f));
	_spriteBg->setContentSize(Size(640.f, VisibleRect::top().y));
	_spriteBg->setVisible(false);

	/*_storyListView = ui::ListView::create();
	_storyListView->setItemsMargin(20.f);

	this->addChild(_storyListView);
	_storyListView->setContentSize(Size(600.f, 550.f));
	_storyListView->setClippingEnabled(false);
	_storyListView->setPosition(Vec2(20.f, _mapOffpos + _mapStartY - 570.f));
	_storyListView->setVisible(false);
	_storyListView->setScrollBarEnabled(false);*/

	_storySkipNode = Node::create();
	auto bg = Sprite::createWithSpriteFrameName("com_skip_1.png");
	//auto skip = Sprite::createWithSpriteFrameName("com_skip_0.png");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/jiantou.ExportJson");
	auto logo0 = Armature::create("jiantou");
	logo0->getAnimation()->play("jiantou");
	_storySkipNode->addChild(logo0, 1);

	_storySkipNode->addChild(bg);
	this->addChild(_storySkipNode);
	_storySkipNode->setPosition(Vec2(540.f, 70.f));
	_storySkipNode->setVisible(false);
	
	_rttNode = RTTNode::create(640, 580);
	this->addChild(_rttNode);
	_rttNode->setPosition(320.f, 290.f + _mapStartY);
	_rttNode->setRTTEffect(RTTEffect_E::RTTEFFECT_GRADUALALPHA);
	//_rttNode->setRTTEffect(RTTEffect_E::RTTEFFECT_LOWHP);

	_curMapNode_0 = GameCSLoader::createNode(String::createWithFormat("mapsnew/map_%d_new.csb", _idx)->getCString());
	_rttNode->addChild(_curMapNode_0, 1);

	auto pBoundary = Sprite::createWithSpriteFrameName("crush_boundary.png");
	pBoundary->setPosition(Vec2(320.f,2.f));
	_rttNode->addChild(pBoundary, 2);


	initMoveLayer();

	_curDistance = 1920.f;

	auto layer = GameLayer::create();
	this->addChild(layer);
	layer->setPositionY(ParamData::FIGHT_LAYER_POS_Y);

	this->scheduleUpdate();
	return true;
}

void GameMap::moveMap(float dur, float distance)
{
	_moveTotleTime = dur;
	_isMove = true;
	_curMoveTime = 0.f;
	_moveSpeed = distance / dur;
}

void GameMap::changeToMap(int idx)
{
	if (_idx == idx)
	{
		return;
	}
	int lasetidx = _idx;

	_idx = idx;

	_isChangingMap = true;

	_nextMapNode_0 = GameCSLoader::createNode(String::createWithFormat("mapsnew/map_%d_new.csb", _idx)->getCString());
	_rttNode->addChild(_nextMapNode_0, 0);

	auto sprite = _rttNode->getSprite();
	auto action = Sequence::createWithTwoActions(FadeTo::create(0.25f, 100), FadeTo::create(0.25f, 255));
	sprite->runAction(action);

	auto actionRemoveMap = CallFunc::create([=]() {
		freeUnusedMap(lasetidx);
	});

	auto actionRemove = Sequence::create(DelayTime::create(0.25f), CallFunc::create([=]() {
		_curMapNode_0->removeFromParent();
		_curMapNode_0 = _nextMapNode_0;
		_nextMapNode_0 = nullptr;

		_curMapNode_0->setLocalZOrder(1);

		initMoveLayer();


		_isChangingMap = false; 
	}), actionRemoveMap, nullptr);

	this->runAction(actionRemove);

	
}

void GameMap::restartCurMap()
{
	_isChangingMap = true;
	auto sprite = _rttNode->getSprite();
	auto action = Sequence::createWithTwoActions(FadeOut::create(0.2f), FadeIn::create(0.2f));
	sprite->runAction(action);
	auto actionRemove = Sequence::create(DelayTime::create(0.25f), CallFunc::create([this]() {
		_curMapNode_0->setPositionX(0.f); 
		_isChangingMap = false; 
		for (int i = _needMoveMapLayer.size() - 1; i != -1; i--)
		{
			auto movelayer = _needMoveMapLayer.at(i);
			movelayer->setPositionX(0.f);
		}
	}), nullptr);
	this->runAction(actionRemove);
}

void GameMap::changeToFight(int idx)
{
	auto actionmove = Sequence::create(MoveBy::create(0.5f, Vec2(0.f, _mapOffpos)), nullptr);
	_rttNode->getSprite()->runAction(actionmove);

//	GameLayer::getInstance()->runAction(MoveTo::create(0.5f, Vec2::ZERO));
	GameLayer::getInstance()->runAction(MoveBy::create(0.5f, Vec2(0.f, -ParamData::FIGHT_LAYER_POS_Y)));

	auto action = Sequence::createWithTwoActions(EffectAction::create(2.f, EffectActionType_E::EFFECTACTION_ARCHING_SHOW, true), EffectAction::create(0.f, EFFECTACTION_DEFAULT));
	_rttNode->getSprite()->runAction(action);
	action->setTag(EFFECTACTION_TAG);

	for (int i = _needMoveMapLayer.size() - 1; i != -1; i--)
	{
		auto movelayer = _needMoveMapLayer.at(i);
		movelayer->setPositionY(0.f);
	}
	_isInFight = true;
	GameUtils::playEffect("jinru.ogg");

	if (_fightType != FightType::DUNGEON)
	{
		auto actionshow = Sequence::createWithTwoActions(DelayTime::create(1.f), Show::create());
		GameLayer::getInstance()->getFightInfoNode()->runAction(actionshow);
	}

	WordMap::getInstance()->setCanTouchMove(false);

	MainLayer::getCurMainLayer()->getPauseBtn()->setEnabled(false);
	auto deytime = Sequence::createWithTwoActions(DelayTime::create(2.f), CallFunc::create([=]() {
		MainLayer::getCurMainLayer()->getPauseBtn()->setEnabled(true);
	}));

	MainLayer::getCurMainLayer()->enterFight();

	this->runAction(deytime);
}

void GameMap::changeToWordMap(int idx)
{
	_fightType = FightType::NOR;

	MainLayer::getCurMainLayer()->getWordMap()->setVisible(true);
	_spriteBg->setPositionY(VisibleRect::top().y / 2 - ParamData::FIGHT_BG_HEIGHT);

	MainLayer::getCurMainLayer()->getPauseBtn()->setEnabled(false);
	MainLayer::getCurMainLayer()->leaveFight();

	auto actionbg = Sequence::create(DelayTime::create(1.6f), MoveBy::create(0.5f, Vec2(0.f, -_mapOffpos)), CallFunc::create([=]() {
		WordMap::getInstance()->setCanTouchMove(true);
		MainLayer::getCurMainLayer()->getPauseBtn()->setEnabled(true);
	}), nullptr);
	_spriteBg->runAction(actionbg);

	_rttNode->getSprite()->stopAllActions();

	auto actionmove = Sequence::create(DelayTime::create(1.6f), MoveBy::create(0.5f, Vec2(0.f, -_mapOffpos)), nullptr);
	_rttNode->getSprite()->runAction(actionmove);

//	auto gameLayerPos = Vec2(0.f, ParamData::FIGHT_LAYER_POS_Y);
//	GameLayer::getInstance()->runAction(Sequence::createWithTwoActions(DelayTime::create(1.6f), MoveTo::create(0.5f, gameLayerPos)));
	GameLayer::getInstance()->runAction(Sequence::createWithTwoActions(DelayTime::create(1.6f), MoveBy::create(0.5f, Vec2(0.f, ParamData::FIGHT_LAYER_POS_Y))));

	ShaderSprite::addShaderEffect(_rttNode->getSprite(), ShaderSprite_E::SHADERSPRITE_GRADUALALPHA);
	auto action = Sequence::create(EffectAction::create(1.f, EffectActionType_E::EFFECTACTION_ARCHING_SHOW_OUT),nullptr);
	_rttNode->getSprite()->runAction(action);

	_isInFight = false;

	GameLayer::getInstance()->backToWorldMap();
	GameUtils::stopBackgroundMusic(false);
	GameUtils::playBackgroundMusic("daiji.mp3", true);

	GameLayer::getInstance()->getFightInfoNode()->stopAllActions();
	GameLayer::getInstance()->getFightInfoNode()->setVisible(false);
	GameLayer::getInstance()->removerDungeonInfo();

	WordMap::getInstance()->hideFinget();
	MainLayer::getCurMainLayer()->getTopGoldNode()->setVisible(true);
}

void GameMap::checkChangeMap()
{
	
	auto movelayer = _needMoveMapLayer.back();
	
	if (!_isChangingMap && movelayer && movelayer->getPositionX() -50.f - 640.f < -_curDistance)
	{
		//CCLOG("%f", movelayer->getPositionX());
		restartCurMap();
	}
}

void GameMap::update(float delta)
{
	if (_pause || !isVisible())
	{
		return;
	}

	if (_isMove)
	{
		_curMoveTime += delta;
		
		if (_curMoveTime < _moveTotleTime)
		{
			moveMaps(delta, false);
		}
		else
		{
			_isMove = false;
			_moveSpeed = 0.f;
			_curMoveTime = 0.f;
		}
	}
	else if (_autoMove)
	{
		_moveSpeed = -ParamData::POS_X_INTERVAL / 2.f;
		moveMaps(delta, true);
	}
	
	//checkChangeMap();
}


bool GameMap::checkShowStory(int posidx)
{	
	auto storypos = StoryMgr::getInstance()->getCurStoryConfig().posid;
	if (storypos == -1)
	{
		return false;
	}
	auto curpos = WordMap::getInstance()->getCurPosIdx();
	auto havestory = StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->isHaveDialog();
	if (storypos == curpos && havestory && _fightType == FightType::NOR)
	{
		return true;
	}
	return false;
}


void GameMap::showStory(int posidx)
{
	/*auto action = Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]() {
		_storyListView->removeAllItems();
		StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->executeCommond();
		_storyListView->setVisible(true);
		_touchListener = EventListenerTouchOneByOne::create();
		_touchListener->setSwallowTouches(true);
		_touchListener->onTouchBegan = [=](Touch*, Event*) {
			if (!StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->_isExecuting)
			{
				StoryMgr::getInstance()->getCommandMgrByIdx(posidx)->executeCommond();
			}
			return true;
		};

		_eventDispatcher->addEventListenerWithFixedPriority(_touchListener, -1);
	}));

	this->runAction(action);

	GameLayer::getInstance()->switchInfoDisPlay(false);
	MainLayer::getCurMainLayer()->getPauseBtn()->setVisible(false);*/
	GameLayer::getInstance()->showStory(posidx);
	_storySkipNode->setVisible(true);
}


void GameMap::loadStory( int posidx)
{
	if (posidx == 0)
	{	
		//reloadStory() must earlier than changeToFight,for the rrtnode effectaction;
		StoryMgr::getInstance()->reloadStory();	
		MainLayer::getCurMainLayer()->enterStory();
		//changeToFight() must earlier than startFight
		changeToFight(0);
		GameLayer::getInstance()->startFight();
		_spriteBg->setPositionY(VisibleRect::top().y / 2);
		_spriteBg->runAction(FadeIn::create(0.5f));
		_spriteBg->setVisible(true);
	}
	else if (posidx == 2)
	{
		
	}
	
}

void GameMap::storyEnd(int posidx)
{
	GameLayer::getInstance()->storyend();

	if (posidx == 2)
	{
		GameLayer::getInstance()->endStory(2);
	}
	else if (0 == posidx)
	{
		GameLayer::getInstance()->endStory(0);
	}
	else if (1 == posidx)
	{
		GameLayer::getInstance()->endStory(1);
	}

	GameLayer::getInstance()->switchInfoDisPlay(true);
	MainLayer::getCurMainLayer()->getPauseBtn()->setVisible(true);

	_storySkipNode->setVisible(false);
}

void GameMap::showResault(bool win, int exp, int gold, float* arr4)
{
	if (_scoreNode == nullptr)
	{
		_resoultExp[0] = arr4[0];
		_resoultExp[1] = arr4[1];
		_resoultExp[2] = arr4[2];
		_resoultExp[3] = arr4[3];

		GameLayer::getInstance()->switchInfoDisPlay(false);
		MainLayer::getCurMainLayer()->getPauseBtn()->setVisible(false);

		_scoreNode = Node::create();
		_scoreNode->setPosition(Vec2(320.f, ParamData::CRUSH_BG_HEIGHT - 50.f));
		this->addChild(_scoreNode, 11);

		GameUtils::stopBackgroundMusic(false);

		if (_fightType != FightType::DUNGEON)
		{
			if (win)
			{
				DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYPASS, 1, false);

				if (WordMap::getInstance()->checkIsBoss(WordMap::getInstance()->getCurPosIdx()) && UserData::getInstance()->getIsBossPass(WordMap::getInstance()->getCurPosIdx()) == 1)
				{
					DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYSPBOSS, 1, false);
					if (WordMap::getInstance()->getCurPosIdx() == 68)
					{
						NewMapOpenMgr::getInstance()->tryGet(MapOpenType::PASS68);
					}
					else if (WordMap::getInstance()->getCurPosIdx() == 77)
					{
						NewMapOpenMgr::getInstance()->tryGet(MapOpenType::PASS77);
					}
					else if (WordMap::getInstance()->getCurPosIdx() == 80)
					{
						NewMapOpenMgr::getInstance()->tryGet(MapOpenType::PASS80);
					}
				}

				//if play endlessMode will made pass?
				UserData::getInstance()->addWinTime(1);
				UserData::getInstance()->setBossPass(WordMap::getInstance()->getCurPosIdx(), 1);
				UserData::getInstance()->setPlayingGuanqiaIdx(WordMap::getInstance()->getCurPosIdx());
				UserData::getInstance()->setPlayerPos(WordMap::getInstance()->getCurPosIdx());
				WordMap::getInstance()->updateNewMapBtn();
				WordMap::getInstance()->updataPointsBtns();
				GameUtils::playEffect("shengli.ogg", false);
			}
			else
			{
				GameUtils::playEffect("shibai.ogg", false);
				WordMap::getInstance()->tryBackToLastPos();
			}
		}
		else
		{
			if (win)
			{
				GameUtils::playEffect("shengli.ogg", false);
				DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYDUNGEON, 1, false);
			}
			else
			{
				GameUtils::playEffect("shibai.ogg", false);
			}
		}

		showResaultTextAni(win, exp, gold, arr4);
	}
}

void GameMap::hideResault()
{
	if (_scoreNode)
	{
		auto action = FadeOut::create(1.5f);
		_scoreNode->getChildByName("csbname")->runAction(action);

		auto remove = Sequence::createWithTwoActions(DelayTime::create(1.5f), RemoveSelf::create());
		_scoreNode->runAction(remove);

		_scoreNode = nullptr;
		_isShowScore = false;

		GameLayer::getInstance()->switchInfoDisPlay(true);
		MainLayer::getCurMainLayer()->getPauseBtn()->setVisible(true);
		GameLayer::getInstance()->removeUpdateNode();

		_resoultExp[0] = 0.f;
		_resoultExp[1] = 0.f;
		_resoultExp[2] = 0.f;
		_resoultExp[3] = 0.f;
	}
}

void GameMap::showResaultTextAni(bool win, int exp, int gold, float* arr4)
{
	if (win)
	{
		Vec2 pos(0.f, 100.f);

		auto spritebg = Sprite::createWithSpriteFrameName("score_failed_bg.png");
		_scoreNode->addChild(spritebg);
		spritebg->setPosition(pos);
		spritebg->setName("textbg");

		auto failed = Sprite::createWithSpriteFrameName("score_text_win.png");
		_scoreNode->addChild(failed);
		failed->setName("scoretext");
		failed->setPosition(pos);

		auto actiondis0 = Sequence::createWithTwoActions(DelayTime::create(1.f), FadeOut::create(1.f));
		spritebg->runAction(actiondis0);
		failed->runAction(actiondis0->clone());

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/scoretexteffect.ExportJson");
		auto logo1 = Armature::create("scoretexteffect");
		logo1->getAnimation()->play("win");
		_scoreNode->addChild(logo1, 10);
		logo1->getAnimation()->setSpeedScale(1.f);
		logo1->setPositionY(260.f);
		logo1->setScale(1.5f);

		std::function<void(Armature*, MovementEventType, const std::string&)> armatureFun = [=](Armature* armature, MovementEventType type, const std::string& id)
		{
			if (type == MovementEventType::COMPLETE)
			{
				armature->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), RemoveSelf::create()));
				showResaultNumInfo(win, exp, gold, arr4);
			}
		};

		logo1->getAnimation()->setMovementEventCallFunc(armatureFun);
	}
	else
	{
		Vec2 pos(0.f, 100.f);

		auto spritebg = Sprite::createWithSpriteFrameName("score_failed_bg.png");
		_scoreNode->addChild(spritebg);
		spritebg->setPosition(pos);

		auto failed = Sprite::createWithSpriteFrameName("score_text_failed.png");
		_scoreNode->addChild(failed);
		failed->setPosition(pos);

		auto actiondis0 = Sequence::createWithTwoActions(DelayTime::create(1.f), FadeOut::create(1.f));
		spritebg->runAction(actiondis0);
		failed->runAction(actiondis0->clone());

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/scoretexteffect.ExportJson");
		auto logo1 = Armature::create("scoretexteffect");
		logo1->getAnimation()->play("failed");
		_scoreNode->addChild(logo1, 10);
		logo1->getAnimation()->setSpeedScale(1.0f);
		logo1->setPositionY(260.f);
		logo1->setScale(1.5f);

		std::function<void(Armature*, MovementEventType, const std::string&)> armatureFun = [=](Armature* armature, MovementEventType type, const std::string& id)
		{
			if (type == MovementEventType::COMPLETE)
			{
				armature->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), RemoveSelf::create()));
				showResaultNumInfo(win, exp, gold, arr4);
			}
		};

		logo1->getAnimation()->setMovementEventCallFunc(armatureFun);
	}
}

void GameMap::showResaultNumInfo(bool win, int exp, int gold, float* arr4)
{
	float exppoints0 = _resoultExp[0];
	float exppoints1 = _resoultExp[1];
	float exppoints2 = _resoultExp[2];
	float exppoints3 = _resoultExp[3];

	_scoreShowStep = 0;

	int targetStep = 1;
	std::vector<PopItemInfo_T> arrItemsBox;

	auto bossexp = ParamMgr::getInstance()->getStageInfo(WordMap::getInstance()->getCurPosIdx())->rewardexp;
	auto bossgold = ParamMgr::getInstance()->getStageInfo(WordMap::getInstance()->getCurPosIdx())->rewardgold;
	_isShowScore = true;

	bool getbox = false;

	Node* scoreNodeCSB = nullptr;

	if (win)
	{
		if (_fightType != FightType::DUNGEON)
		{
			WordMap::getInstance()->addJingYing(WordMap::getInstance()->getCurPosIdx());
		}

		scoreNodeCSB = GameCSLoader::createNode("csb/scorenode.csb");
		scoreNodeCSB->setName("csbname");
		//scoreNodeCSB->setPosition(Vec2(320.f, ParamData::CRUSH_BG_HEIGHT - 50.f));
		_scoreNode->addChild(scoreNodeCSB, 11);

		auto num_0 = static_cast<ui::TextAtlas*>(scoreNodeCSB->getChildByName("Node_2")->getChildByName("num_0"));
		auto num_1 = static_cast<ui::TextAtlas*>(scoreNodeCSB->getChildByName("Node_3")->getChildByName("num_1"));
		auto num_2 = static_cast<ui::TextAtlas*>(scoreNodeCSB->getChildByName("Node_4")->getChildByName("num_2"));

		num_0->setString(String::createWithFormat("%d", exp)->getCString());
		num_1->setString(String::createWithFormat("%d", bossexp)->getCString());
		num_2->setString(String::createWithFormat("%d", bossgold)->getCString());

		auto nodebox = scoreNodeCSB->getChildByName(String::createWithFormat("Node_%d", 6)->getCString());

		auto info = ParamMgr::getInstance()->getStageInfo(WordMap::getInstance()->getCurPosIdx());
		auto& boxp = info->arrBoxChance;

		Vec2 nodepos(-136.f, -436.f);
		float offnodebox = 0.f;
		int idx = 0;
		if (BoxDataMgr::getInstance()->isBoxInBagFull() == true)
		{
			GameUtils::toastTip("boxfull");
			getbox = false;
		}
		else if (_fightType == FightType::DUNGEON)
		{
			getbox = false;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				auto p = ToolsUtil::getRandomFloat(0.f, 1.f);
				if (getbox == false && p <= boxp[i])
				{
					getbox = true;
					auto bg = Sprite::createWithSpriteFrameName("score_boxbg.png");
					nodebox->addChild(bg);
					bg->setPosition(nodepos + Vec2(90.f * idx, 0.f));
					auto icon = Sprite::createWithSpriteFrameName(g_boxBtnPicName[i]);
					nodebox->addChild(icon);
					icon->setPosition(nodepos + Vec2(90.f * idx, 0.f));
					icon->setScale(0.5f);

					PopItemInfo_T boxt;
					boxt.itemId = PopRewardBoxId::POP_BOX_0 + i;
					boxt.itemCount = 1;
					int boxLv = BoxDataMgr::getInstance()->getCurGetBoxLv(WordMap::getInstance()->getCurPosIdx());

					boxt.itemLv = boxLv;
					if (FightType::HARD == GameMap::getCurGameMap()->getCurFightType())
					{
						boxt.itemLv = MIN(4, boxt.itemLv + 1);
					}
					arrItemsBox.push_back(boxt);

					idx++;
				}
			}
		}

		//UserData::getInstance()->saveUserData();

		float time = 0.f;
		for (int i = 1; i < 7; i++)
		{
			auto node = scoreNodeCSB->getChildByName(String::createWithFormat("Node_%d", i)->getCString());
			node->setPositionX(node->getPositionX() + 620.f);

			auto action = Sequence::createWithTwoActions(DelayTime::create(time), MoveBy::create(0.3f, Vec2(-620.f, 0.f)));
			time += 0.2f;

			node->runAction(action);

			if (bossexp == 0 && i > 2)
			{
				node->setVisible(false);
				offnodebox = 150.f;
				continue;
			}
			else if (StoryMgr::getInstance()->getCurStoryConfig().posid != WordMap::getInstance()->getCurPosIdx() && i > 2)
			{
				node->setVisible(false);
				offnodebox = 150.f;
				continue;
			}

		}

		if (getbox)
		{
			scoreNodeCSB->getChildByName(String::createWithFormat("Node_%d", 5)->getCString())->setVisible(true);
			nodebox->setVisible(true);

			auto node5 = scoreNodeCSB->getChildByName("Node_5");
			auto node6 = scoreNodeCSB->getChildByName("Node_6");
			node5->setPositionY(offnodebox);
			node6->setPositionY(offnodebox);

			targetStep = 2;
		}
	}
	else
	{
		scoreNodeCSB = GameCSLoader::createNode("csb/scorenode_0.csb");
		//scoreNodeCSB->setPosition(Vec2(320.f, ParamData::CRUSH_BG_HEIGHT - 50.f));
		scoreNodeCSB->setName("csbname");
		_scoreNode->addChild(scoreNodeCSB, 11);

		auto num_0 = static_cast<ui::TextAtlas*>(scoreNodeCSB->getChildByName("Node_2")->getChildByName("num_0"));
		num_0->setString(String::createWithFormat("%d", exp)->getCString());

		float time = 0.f;

		for (int i = 1; i < 7; i++)
		{
			auto node = scoreNodeCSB->getChildByName(String::createWithFormat("Node_%d", i)->getCString());
			node->setPositionX(node->getPositionX() + 620.f);

			auto action = Sequence::createWithTwoActions(DelayTime::create(time), MoveBy::create(0.3f, Vec2(-620.f, 0.f)));
			time += 0.2f;

			node->runAction(action);

			if (i >= 4)
			{
				auto btn = static_cast<Button*>(node->getChildByName("textbtn"));
				btn->setTag(i);
				btn->addTouchEventListener(CC_CALLBACK_2(GameMap::menuOnScoreFailed, this));
			}
		}
	}

	if (StoryMgr::getInstance()->getCurStoryConfig().posid == WordMap::getInstance()->getCurPosIdx())
	{
		if (win)
		{
			exp = exp + bossexp;
			gold = bossgold + gold;

			StoryMgr::getInstance()->loadNext();
		}
	}

	auto actionshowadd = Sequence::createWithTwoActions(DelayTime::create(2.f), CallFunc::create([=]() {

		GameLayer::getInstance()->showExpAdd(exp*exppoints0, exp*exppoints1, exp*exppoints2, exp*exppoints3);

		_scoreShowStep = 1;
	}));
	scoreNodeCSB->runAction(actionshowadd);

	UserData::getInstance()->giveGold(gold);
	//UserData::getInstance()->saveUserData(true);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch*, Event*) {
		if (_isShowScore && _scoreShowStep == targetStep)
		{
			hideResault();
			FightLayer::getInstance()->moveRoleToOutSidePos(0);

			changeToWordMap(0);
			MainLayer::getCurMainLayer()->leaveStory(win);
		}
		else if (getbox && _scoreShowStep == 1)
		{
			int curPosIdx = WordMap::getInstance()->getCurPosIdx();
			if (WordMap::getInstance()->checkIsBoss(curPosIdx) && !GameLayer::getInstance()->getIsNeedGrow())
			{
				int param = 0;
// 				if (FightType::HARD == GameMap::getCurGameMap()->getCurFightType())
// 				{
// 					param = 3;
// 				}
				auto boxInfo = arrItemsBox.at(0);
				auto layer = PopRewardLayer::openBox(boxInfo.itemId - PopRewardBoxId::POP_BOX_0, boxInfo.itemLv, param);
				this->addChild(layer, 12);
			}
			else
			{
				auto layer = PopRewardLayer::create(arrItemsBox);
				this->addChild(layer, 12);
			}
			_scoreShowStep = 2;
		}
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, scoreNodeCSB);
}

void GameMap::enterFight()
{
	changeToFight(0);

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=] {GameLayer::getInstance()->startFight(); })));
	MainLayer::getCurMainLayer()->enterStory();
	_spriteBg->setPositionY(VisibleRect::top().y / 2);
	_spriteBg->runAction(FadeIn::create(0.5f));
	_spriteBg->setVisible(true);
}

void GameMap::enterEndlessFight()
{
	_fightType = FightType::ENDLESS;
	
	changeToFight(0);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=] {GameLayer::getInstance()->startFight(); })));
	MainLayer::getCurMainLayer()->enterStory();
	_spriteBg->setPositionY(VisibleRect::top().y / 2);
	_spriteBg->runAction(FadeIn::create(0.5f));
	_spriteBg->setVisible(true);
}

void GameMap::enterHard()
{
	_fightType = FightType::HARD;

	changeToFight(0);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=] {GameLayer::getInstance()->startFight(); })));
	MainLayer::getCurMainLayer()->enterStory();
	_spriteBg->setPositionY(VisibleRect::top().y / 2);
	_spriteBg->runAction(FadeIn::create(0.5f));
	_spriteBg->setVisible(true);
}

void GameMap::enterDungeon(int idx)
{
	_dungeonIdx = idx;
	_fightType = FightType::DUNGEON;
	changeToFight(0);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=] {GameLayer::getInstance()->startFight(); })));
	MainLayer::getCurMainLayer()->enterStory();
	_spriteBg->setPositionY(VisibleRect::top().y / 2);
	_spriteBg->runAction(FadeIn::create(0.5f));
	_spriteBg->setVisible(true);

	auto actionshow = Sequence::createWithTwoActions(DelayTime::create(1.f), Show::create());
	GameLayer::getInstance()->getDungeonInfoNode()->runAction(actionshow);

	MainLayer::getCurMainLayer()->getTopGoldNode()->setVisible(false);
}

void GameMap::prepareMap(int idx)
{
	if (_idx == idx)
	{
		return;
	}

	if (idx == 0)
	{
		_prepareCount = 1;
		_pngQueue.push("mapsnew/map_0.pvr.ccz");
	}
	else if (idx == 1)
	{
		_prepareCount = 1;
		_pngQueue.push("mapsnew/map_1.pvr.ccz");
	}
	else if (idx == 2)
	{
		_prepareCount = 1;
		_pngQueue.push("mapsnew/map_2.pvr.ccz");
	}
	else if (idx == 3)
	{
		_prepareCount = 1;
		_pngQueue.push("mapsnew/map_3.pvr.ccz");
	}
	else if (idx == 4)
	{
		_prepareCount = 1;
		_pngQueue.push("mapsnew/map_4.pvr.ccz");
	}
	
	preLoadMapRes();
}

void GameMap::preLoadMapRes()
{
	if ((int)_pngQueue.size() == 0)
	{
		return;
	}

	Director::getInstance()->getTextureCache()->addImageAsync(_pngQueue.front(), [&](Texture2D*) {
		_prepareCount--;
		_pngQueue.pop();
		preLoadMapRes();
	});
}

void GameMap::freeUnusedMap(int idx)
{
	if (idx == 0)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mapsnew/map_0.plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_0.pvr.ccz");
		//Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_0_bg_0.png");
	}
	else if (idx == 1)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mapsnew/map_1.plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_1.pvr.ccz");
	}
	else if (idx == 2)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mapsnew/map_2.plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_2.pvr.ccz");
	}
	else if (idx == 3)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mapsnew/map_3.plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_3.pvr.ccz");
		//Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_3_bg.jpg");
	}
	else if (idx == 4)
	{		
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mapsnew/map_4.plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey("mapsnew/map_4.pvr.ccz");
	}
}

void GameMap::initMoveLayer()
{
	//_curMapOffDistance = 1020.f;
	_moveFactor[0] = 0.2f;
	_moveFactor[1] = 0.6f;
	_moveFactor[2] = 0.8f;
	_moveFactor[3] = 1.f;
	_moveFactor[4] = 1.2f;

	if (_idx == 0)
	{
		_curMapLayer3Off = 626.f * 3.f-5.f;
		_curMapLayer2Off = 1503.f;
		_curMapLayer1Off = 840.f;

		_moveFactor[0] = 0.2f;
		_moveFactor[1] = 0.6f;
		_moveFactor[2] = 1.f;
		_moveFactor[3] = 1.f;
		_moveFactor[4] = 1.2f;
	}
	else if (_idx == 3)
	{
		_curMapLayer3Off = 205.f * 5.f-5.f;
		_curMapLayer2Off = 1050.f;
		_curMapLayer1Off = 1050.f;
	}
	else if (_idx == 1 || _idx == 4)
	{
		_curMapLayer2Off = 1135.f;
		_curMapLayer3Off = 1436.f;
		_curMapLayer1Off = 1195.f;

		_moveFactor[0] = 0.2f;
		_moveFactor[1] = 0.6f;
		_moveFactor[2] = 1.f;
		_moveFactor[3] = 1.f;
		_moveFactor[4] = 1.2f;
	}
	else if (_idx == 2)
	{
		_curMapLayer2Off = 1600.f;
		_curMapLayer3Off = 1360.f;
		_curMapLayer1Off = 1000.f;
	}
	

	_needMoveMapLayer.clear();

	auto layer = _curMapNode_0->getChildByName("root")->getChildren();
	auto root = _curMapNode_0->getChildByName("root");
	auto count = layer.size();
	for (int i = 0; i < count; i++)
	{
		auto temp = root->getChildByName(String::createWithFormat("layer_%d", i)->getCString());	
		//auto name = temp->getName();
		//auto idx = name.find("layer_", 0);
		//if (idx != std::string::npos)
		if(temp !=nullptr)
		{
			temp->setLocalZOrder(i);
			_needMoveMapLayer.pushBack(temp);
		}
	}
}

void GameMap::moveUpEffect(float time, bool up)
{
	if (up)
	{
		if (_isCurMapUp)
		{
			return;
		}

		auto action = EaseSineOut::create(MoveBy::create(time, Vec2(0.f, _mapStartY)));
		_rttNode->runAction(action);

		GameLayer::getInstance()->runAction(action->clone());

		_isCurMapUp = true;
	}
	else
	{
		if (!_isCurMapUp)
		{
			return;
		}
		auto action = EaseSineOut::create(MoveBy::create(time, Vec2(0.f, -_mapStartY)));
		_rttNode->runAction(action);

		GameLayer::getInstance()->runAction(action->clone());

		_isCurMapUp = false;
	}
		
		

}

void GameMap::quitFight()
{
	GameLayer::getInstance()->stopFight();
	changeToWordMap(0);
	MainLayer::getCurMainLayer()->leaveStory();
	WordMap::getInstance()->tryBackToLastPos();
}

void GameMap::pauseGame()
{
	//debug zsb
	_rttNode->getSprite()->pause();
	_pause = true;
}

void GameMap::resumGame()
{
	//debug zsb
	_rttNode->getSprite()->resume();
	_pause = false;
}

void GameMap::showLowHpEffect(bool enable)
{
	if (!_isInFight)
	{
		return;
	}

	if (_rttNode->getSprite()->getActionByTag(EFFECTACTION_TAG))
	{
		return;
	}

	if (enable)
	{
		_rttNode->setRTTEffect(RTTEFFECT_LOWHP);
	}
	else
	{
		_rttNode->setRTTEffect(RTTEFFECT_DEFAULT);
	}
}

void GameMap::menuOnScoreFailed(Ref * btn, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int tag = static_cast<Node*>(btn)->getTag();

		/*auto layer = GiftLayer::create();
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_Z);*/

		/*if (tag == 4)
		{
			if (MainLayer::getCurMainLayer()->checkTimeLimitGift() != -1)
			{
				MainLayer::getCurMainLayer()->popBuyItemOrLayer(0);
			}
			else
			{
				MainLayer::getCurMainLayer()->popBuyItemOrLayer(3);
			}
		}
		else if (tag == 5)
		{
			MainLayer::getCurMainLayer()->popBuyItemOrLayer(2);
		}
		else if (tag == 6)
		{
			MainLayer::getCurMainLayer()->popBuyItemOrLayer(1);
		}*/
	}
}

void GameMap::moveMaps(float dt, bool automove)
{
	if (!automove)
	{
		for (int i = _needMoveMapLayer.size() - 1; i != -1; i--)
		{
			auto movelayer = _needMoveMapLayer.at(i);
//			auto off = -180.f * dt * _moveFactor[i];
			auto off = _moveSpeed * dt * _moveFactor[i];
			for (auto temp : movelayer->getChildren())
			{
				temp->setPositionX(temp->getPositionX() + off);
				if (temp->getPositionX() < -temp->getContentSize().width)
				{
					if (i == 3)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer3Off);
					}
					else if (i == 2)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer2Off);
					}
					else if (i == 1)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer1Off);
					}
					else
					{
						temp->setPositionX(640.f);
					}
				}
			}

			off = off * 0.6f;
		}
	}
	else
	{
		for (int i = _needMoveMapLayer.size() - 1; i != -1; i--)
		{
//			auto off = -180.f * dt * _moveFactor[i];
			auto off = _moveSpeed * dt * _moveFactor[i];

			auto movelayer = _needMoveMapLayer.at(i);		

			for (auto temp : movelayer->getChildren())
			{
				temp->setPositionX(temp->getPositionX() + off);
			
				if (temp->getPositionX() < -temp->getContentSize().width)
				{
					if (i == 3)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer3Off);
					}
					else if (i == 2)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer2Off);
					}
					else if (i == 1)
					{
						temp->setPositionX(temp->getPositionX() + _curMapLayer1Off);
					}
					else
					{
						temp->setPositionX(640.f);
					}
				}
			}
		}
	}
}


WordMap *WordMap::s_pInstance = nullptr;

WordMap::WordMap():_moveAction(nullptr),
	_nextNode(nullptr),
	_needMove(false),
	_startNode(nullptr),
	_endNode(nullptr),
	_curPosIdx(0),
	_curTime(0.f),
	_totalTime(0.f),
	_finishMoveRoad(true),
	_curNode(nullptr),
	_canTouch(true),
	_isShowPoints(true),
	_endmove(true),
	_pauseGame(false),
	_fignerAni(nullptr),
	_newMapBtn(nullptr)
{
	memset(_arrGuaiQiaMapidx, -1, sizeof(_arrGuaiQiaMapidx));
	s_pInstance = this;	
}

WordMap::~WordMap()
{
	s_pInstance = nullptr;
}

WordMap * WordMap::getInstance()
{
	return s_pInstance;
}

WordMap * WordMap::createWordMap()
{
	auto pRef = new WordMap();
	if (pRef && pRef->initWordMap())
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

bool WordMap::initWordMap()
{
	//srand((unsigned)time(nullptr));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/mapui.plist");

	Layer::init();
	_mapSize = Size(3062.f, 2396.f);
	_sizeMin = MAX(640.f / _mapSize.width, VisibleRect::top().y / _mapSize.height);
	_sizeMax = 1.6f;

	_curPosIdx = 0;
	_nextNode = nullptr;

	_touchScaleNode = glui::TouchesScaleNode::create();
	_touchScaleNode->setContentSize(_mapSize);
	_touchScaleNode->setMinScale(_sizeMin);
	_touchScaleNode->setMaxScale(_sizeMax);
	_touchScaleNode->setPosition(VisibleRect::center());
	_touchScaleNode->setScale(0.8f);
	Rect temp(0.f, 120.f, 640.f, VisibleRect::top().y - 120.f);
	_touchScaleNode->setScreenRect(temp);
	this->addChild(_touchScaleNode);

	_mapRoot = GameCSLoader::createNode("newworldmap/wordmapnode.csb");
	_mapRoot->setPosition(Vec2(-9.f,7.f));
	_touchScaleNode->addChild(_mapRoot);
	
	auto _mapStartY = 50.f;
	_jiantouCenter = Vec2(320.f, (VisibleRect::top().y) * 0.5f + 360.f * 0.5f);

	_jiantouStory = Sprite::createWithSpriteFrameName("jiantou_1.png");
	this->addChild(_jiantouStory, 1000);

	_jiantouPlayer = Sprite::createWithSpriteFrameName("jiantou_0.png");
	this->addChild(_jiantouPlayer, 1000);

	auto actionblink = Sequence::createWithTwoActions(FadeTo::create(0.5f, 100), FadeTo::create(0.5f, 255));
	_jiantouPlayer->runAction(RepeatForever::create(actionblink));

	auto actionblink2 = Sequence::createWithTwoActions(FadeTo::create(0.5f, 100), FadeTo::create(0.5f, 255));
	_jiantouStory->runAction(RepeatForever::create(actionblink2));

	for (int i = 0; i < 10; i++)
	{
		auto cloudblack = Sprite::create("maps/maps_cloud_0_bg.png");
		cloudblack->setLocalZOrder(1);
		_touchScaleNode->addChild(cloudblack);
		cloudblack->setPosition(cloudPos[i] / 1.5f);
		cloudblack->setColor(Color3B(122, 122, 122));
		cloudblack->setOpacity(60);
		cloudblack->setScale((CCRANDOM_0_1() * 0.15f + 0.22f) * 1.1f);

		auto cloud = glui::GLSprite::create("maps/maps_cloud_0.png", false);
		cloudblack->setLocalZOrder(2);
		_touchScaleNode->addChild(cloud);
		cloud->setPosition(cloudPos[i]/1.5f);
		cloud->setPositionZ(400.f);
		cloud->setScale(cloudblack->getScale());


		float time = CCRANDOM_0_1() * 5 + 10.f;

		auto action = Sequence::createWithTwoActions(MoveBy::create(time, Vec2(-200.f, 0.f)),CallFunc::create([=]() {		
			cloud->setPosition(cloudPos[i]/1.5f);
		}));
		auto action2 = Sequence::createWithTwoActions(MoveBy::create(time, Vec2(-200.f, 0.f)), CallFunc::create([=]() {
			cloudblack->setPosition(cloudPos[i] / 1.5f);
		}));

		auto actiontwo = Sequence::create(FadeIn::create(time * 0.2f), DelayTime::create(time * 0.6f), FadeOut::create(time * 0.2f), nullptr);
		auto actiontwo2 = Sequence::create(FadeTo::create(time * 0.2f, 60), DelayTime::create(time * 0.6f), FadeTo::create(time * 0.2f, 0), nullptr);

		cloudblack->runAction(RepeatForever::create(actiontwo2));
		cloudblack->runAction(RepeatForever::create(action2));
		cloud->runAction(RepeatForever::create(actiontwo));
		cloud->runAction(RepeatForever::create(action));


	}
	
	/*tools start*/
	/*rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	rapidjson::Value arrayroot(rapidjson::kArrayType);*/
	/*tools end*/

	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/mappos.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	rapidjson::Value& posarr = _jsonBezierDoc["pos"];

	_mapPointsMenu = Menu::create();
	_mapRoot->addChild(_mapPointsMenu, 3);
	_mapPointsMenu->setPosition(Vec2(0.f, 0.f));

	Vec2 newWorldMapPos = Vec2(1338.f, 405);
	_newMapBtn = Button::create("mainui_newmappoint_1.png", "", "", Widget::TextureResType::PLIST);
	_mapRoot->addChild(_newMapBtn, 4);
	_newMapBtn->setPosition(newWorldMapPos);
	auto actionRotate = RepeatForever::create(RotateBy::create(1.f, 180.f));
	_newMapBtn->runAction(actionRotate);

	_newMapBtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto layer = NewWorldInfo::create();
			MainLayer::getCurMainLayer()->addChild(layer, POP_Z);
		}
	});

	auto newmapbg = Sprite::createWithSpriteFrameName("mainui_newmappoint_0.png");
	_newMapBtn->addChild(newmapbg);
	newmapbg->setPosition(_newMapBtn->getContentSize() / 2.f);
	auto actionRotate2 = RepeatForever::create(RotateBy::create(0.5f, -180.f));
	newmapbg->runAction(actionRotate2);

	for (int i = 0; i < s_mapBtnNum; i++)
	{	
		rapidjson::Value& xy = posarr[i];
		int x = xy[0].GetInt();
		int y = xy[1].GetInt();
		_pos[i] = Vec2(x, y);

		/*tools start*/
		/*auto node = _mapRoot->getChildByName(String::createWithFormat("pos_%d", i)->getCString());
		{
			rapidjson::Value array(rapidjson::kArrayType);

			rapidjson::Value x(rapidjson::kNumberType);
			rapidjson::Value y(rapidjson::kNumberType);

			x = (int)node->getPositionX();
			y = (int)node->getPositionY();

			array.PushBack(x, allocator);
			array.PushBack(y, allocator);

			arrayroot.PushBack(array, allocator);
		}*/
		/*tools end*/
		bool isshop = false;

		auto& shopinfo = ParamMgr::getInstance()->getShopAttr();
		for (auto&temp : shopinfo)
		{
			if (temp.pos == i)
			{
				isshop = true;
				break;
			}
		}

		if (isshop)
		{
			auto btnsprite = Sprite::createWithSpriteFrameName("word_shop.png");
			glui::GLMenuItemSprite* btn = glui::GLMenuItemSprite::create(btnsprite, CC_CALLBACK_1(WordMap::menuOnShop, this));
			btn->setScale(1.1f);

			auto jump = Sequence::createWithTwoActions(DelayTime::create(2.f), JumpTo::create(0.5f, _pos[i], 15.f, 2));
			btn->runAction(RepeatForever::create(jump));

			_mapPointsMenu->addChild(btn);
			btn->setPosition(_pos[i]);
			btn->setAnchorPoint(Vec2(0.5f, 0.22f));

			btn->setName(String::createWithFormat("%d", i)->getCString());
			_btns[i] = btn;
			_graph.addNode(btn);
		}
		else
		{
			auto btnsprite = Sprite::createWithSpriteFrameName("mapui_point.png");
			glui::GLMenuItemSprite* btn = glui::GLMenuItemSprite::create(btnsprite, CC_CALLBACK_1(WordMap::menuOnPoint, this));
			btn->setScale(1.3f);
			_mapPointsMenu->addChild(btn);
			btn->setPosition(_pos[i]);

			if (StoryMgr::getInstance()->isStoryPos(i))
			{
				btn->setScale(1.8f);
			}

			if (UserData::getInstance()->getIsBossPass(i) == 1 && checkIsBoss(i))
			{
				auto jingyingicon = Sprite::createWithSpriteFrameName("guanqia_boss.png");
				_mapRoot->addChild(jingyingicon, 5);

				jingyingicon->setName(String::createWithFormat("boss_jingying_%d", i)->getCString());

				jingyingicon->setPosition(_pos[i] + Vec2(0.f, 40.f));
				auto actionmove = Sequence::createWithTwoActions(MoveBy::create(1.f, Vec2(0.f, 30.f)), MoveBy::create(1.f, Vec2(0.f, -30.f)));
				jingyingicon->runAction(RepeatForever::create(actionmove));
			}

			btn->setName(String::createWithFormat("%d", i)->getCString());
			_btns[i] = btn;
			_graph.addNode(btn);

			updateBoxGetInfo(i);
		}
	}

	/*tools start*/
	//writedoc.AddMember("pos", arrayroot, allocator);
	//rapidjson::StringBuffer buffer;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	//writedoc.Accept(writer);

	//auto path = FileUtils::getInstance()->getWritablePath();
	//path.append("mappos.json");
	//FILE* file = fopen(path.c_str(), "wb");
	//if (file)
	//{
	//	fputs(buffer.GetString(), file);
	//	fclose(file);
	//}
	/*tools end*/

	loadMapConfig();

	_players = Sprite::createWithSpriteFrameName("mapui_playerpos_0.png");
	_players->setAnchorPoint(Vec2(0.5f, 0.2f));
	_mapRoot->addChild(_players,3);
	_players->setPosition(_btns[UserData::getInstance()->getPlayerPos()]->getPosition());

	_players->stopAllActions();
	Vector<SpriteFrame*> animations;
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "mapui_playerpos_%d.png", i);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animations.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(animations, 0.1f);
	Animate* animate = Animate::create(animation);
	_players->runAction(RepeatForever::create(animate));

	if (UserData::getInstance()->isNeedGuide())
	{
		_fignerAni = Sprite::createWithSpriteFrameName("com_finger_0.png");
		_mapRoot->addChild(_fignerAni, 4);
		_fignerAni->setPosition(_btns[0]->getPosition() + Vec2(30.f, -40.f));

		auto action = Sequence::createWithTwoActions(MoveBy::create(0.5f, Vec2(10.f, -20.f)), MoveBy::create(0.5f, Vec2(-10.f, 20.f)));
		_fignerAni->runAction(RepeatForever::create(action));
	}

	_curNode = _btns[UserData::getInstance()->getPlayerPos()];
	_startPos = _curNode->getPosition();

	StoryMgr::getInstance()->showTargetInfo(this);

	/*auto& shopinfo = ParamMgr::getInstance()->getShopAttr();
	for (auto&temp : shopinfo)
	{
		auto shopicon = Sprite::createWithSpriteFrameName("mapui_target.png");
		auto pos = _btns[temp.pos]->getPosition();
		shopicon->setPosition(pos);

		_mapRoot->addChild(shopicon, 4);
	}*/

	setStartPos();

	updataPointsBtns();
	updateNewMapBtn();
	this->scheduleUpdate();
	return true;
}

void WordMap::addJingYing(int i)
{
	if (checkIsBoss(i) && _mapRoot->getChildByName(String::createWithFormat("boss_jingying_%d", i)->getCString()) == nullptr)
	{
		auto jingyingicon = Sprite::createWithSpriteFrameName("guanqia_boss.png");
		_mapRoot->addChild(jingyingicon, 5);

		jingyingicon->setName(String::createWithFormat("boss_jingying_%d", i)->getCString());

		jingyingicon->setPosition(_pos[i] + Vec2(0.f, 40.f));
		auto actionmove = Sequence::createWithTwoActions(MoveBy::create(1.f, Vec2(0.f, 30.f)), MoveBy::create(1.f, Vec2(0.f, -30.f)));
		jingyingicon->runAction(RepeatForever::create(actionmove));
	}
}

void WordMap::setCanTouchMove(bool canTouch)
{
	_touchScaleNode->setCanTouchMove(canTouch);
	_canTouch = canTouch;
}

void WordMap::showDialog(Node* targetNode)
{
	_needMove = false;
	GameMap::getCurGameMap()->setAutoMove(false);

	_dialogNode = GameCSLoader::createNode("maps/mapdialognode_0.csb");
	MainLayer::getCurMainLayer()->addChild(_dialogNode, MainLayer_Z::POP_Z);
	_dialogNode->setPosition(310.f, VisibleRect::top().y / 2.f - 140.f);
	
	auto text2 = static_cast<Text*>(_dialogNode->getChildByName("posname"));
	auto name = ParamMgr::getInstance()->getPosName(atoi(targetNode->getName().c_str()));

	auto guanqia = String::createWithFormat(ResMgr::getInstance()->getString("guanqia_idx")->getCString(), atoi(targetNode->getName().c_str()) + 1)->getCString() + name;

	if (!checkShowShop(atoi(targetNode->getName().c_str())))
	{
		text2->setString(guanqia);
	}
	else
	{
		text2->setString(name);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _dialogNode);

	auto btnok = static_cast<ui::Button*>(_dialogNode->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			doSearchRoad(_curNode, targetNode);
			_dialogNode->removeFromParent();
			_eagesVectorLast.clear();
			_eagesVectorLast = _eagesVector;
			_needMove = true;
			startMove();
		}
	});

	auto btncancle = static_cast<ui::Button*>(_dialogNode->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_finishMoveRoad)
			{
				norCurLightRoad();
				norLastLightRoad();
			}
			else
			{
				norCurLightRoad();
				for (auto &temp : _eagesVectorLast)
				{
					temp->setColor(Color3B(255, 0, 0));
				}
				_needMove = true;
				GameMap::getCurGameMap()->setAutoMove(true);
			}
			_dialogNode->removeFromParent();			
		}
	});


}

void WordMap::showEnterStoryDiglog(int fileid, int posidx)
{
	auto info = ParamMgr::getInstance()->getStageInfo(posidx);

	auto dialogNode = GameCSLoader::createNode("maps/mapdialognode.csb");
	MainLayer::getCurMainLayer()->addChild(dialogNode,MainLayer_Z::POP_Z);
	dialogNode->setPosition(310.f, VisibleRect::top().y /2.f - 140.f);

	auto text = static_cast<Text*>(dialogNode->getChildByName("tiptext"));
	text->setString(info->stageDesc);

	auto text2 = static_cast<Text*>(dialogNode->getChildByName("posname"));
	auto name = ParamMgr::getInstance()->getPosName(StoryMgr::getInstance()->getCurStoryConfig().posid);
	auto guanqia = String::createWithFormat(ResMgr::getInstance()->getString("guanqia_idx")->getCString(), posidx + 1)->getCString() + name;
	if (!checkShowShop(posidx))
	{
		text2->setString(guanqia);
	}
	else
	{
		text2->setString(name);
	}

	auto tili = static_cast<TextAtlas*>(dialogNode->getChildByName("numcost"));
	tili->setString(String::createWithFormat("/%d", info->energyCost)->getCString());

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, dialogNode);

	auto btnok = static_cast<ui::Button*>(dialogNode->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (UserData::getInstance()->getTili() < info->energyCost)
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_enough_tili")->getCString());
				return;
			}
			else
			{
				UserData::getInstance()->giveTili(-info->energyCost);
				MainLayer::getCurMainLayer()->updataGold();
			}

			GameMap::getCurGameMap()->loadStory(0);
			dialogNode->removeFromParent();
		}
	});

	auto btncancle = static_cast<ui::Button*>(dialogNode->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			dialogNode->removeFromParent();
			tryBackToLastPos();
		}
	});

	btnok->setPositionY(btnok->getPositionY());

	auto boxpos = Vec2(-105.f, 95.f);
	auto& boxget = info->arrBoxChance;
	int idxbox = 0;
	for (int i = 0; i < sizeof(boxget) / sizeof(int); i++)
	{
		if (boxget[i] == 0)
		{
			continue;
		}

		auto bg = Sprite::createWithSpriteFrameName("guanqia_boxbg.png");
		dialogNode->addChild(bg);
		bg->setPosition(boxpos + Vec2(90.f * idxbox, 0.f));

		auto icon = Sprite::createWithSpriteFrameName(g_boxBtnPicName[i]);
		icon->setScale(0.4f);
		//auto icon = Sprite::createWithSpriteFrameName(String::createWithFormat("quanqia_box_%d.png", i)->getCString());
		dialogNode->addChild(icon);
		icon->setPosition(boxpos + Vec2(90.f * idxbox, 0.f));
	
		idxbox++;
	}
}

void WordMap::showEnterFightDiglog(int idx, bool inmove)
{
	bool bossAndHardMode = false;

	auto info = ParamMgr::getInstance()->getStageInfo(idx);

	auto dialogNode = GameCSLoader::createNode("maps/mapdialognode.csb");
	MainLayer::getCurMainLayer()->addChild(dialogNode, MainLayer_Z::POP_Z);
	dialogNode->setPosition(310.f, VisibleRect::top().y / 2.f - 140.f);

	auto text = static_cast<Text*>(dialogNode->getChildByName("tiptext"));
	text->setString(info->stageDesc);

	auto text2 = static_cast<Text*>(dialogNode->getChildByName("posname"));
	auto name = ParamMgr::getInstance()->getPosName(idx);
	
	auto guanqia = String::createWithFormat(ResMgr::getInstance()->getString("guanqia_idx")->getCString(), idx + 1)->getCString() + name;
	if (!checkShowShop(idx))
	{
		text2->setString(guanqia);
	}
	else
	{
		text2->setString(name);
	}
	
	
	if (checkIsBoss(idx) && UserData::getInstance()->getIsBossPass(idx) == 1)
	{
		auto textok = static_cast<Sprite*>(dialogNode->getChildByName("test_guanqia_tz_8"));
		textok->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("guanqia_text_boss.png"));
		bossAndHardMode = true;
	}

	auto tili = static_cast<TextAtlas*>(dialogNode->getChildByName("numcost"));
	tili->setString(String::createWithFormat("/%d", info->energyCost)->getCString());

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, dialogNode);

	auto btnok = static_cast<ui::Button*>(dialogNode->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (UserData::getInstance()->getTili() < info->energyCost)
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_enough_tili")->getCString());
				return;
			}
			else
			{
				UserData::getInstance()->giveTili(-info->energyCost);
				MainLayer::getCurMainLayer()->updataGold();
			}

			if (inmove)
			{
				norCurLightRoad();
				norLastLightRoad();
				_nextNode = nullptr;
				_finishMoveRoad = true;
				_needMove = false;
				GameMap::getCurGameMap()->setAutoMove(false);
			}

			if (bossAndHardMode)
			{
				GameMap::getCurGameMap()->enterHard();
			}
			else
			{
				GameMap::getCurGameMap()->enterFight();
			}
			
			if (!_endmove)
			{
				MainLayer::getCurMainLayer()->moveUpMenu(1.0f);
				GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
			}

			//UserData::getInstance()->setPlayerPos(idx);
			//UserData::getInstance()->saveUserData(true);

			dialogNode->removeFromParent();	
		}
	});
	btnok->setPositionY(btnok->getPositionY());

	auto btncancle = static_cast<ui::Button*>(dialogNode->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (inmove)
			{
				if (_finishMoveRoad)
				{
					norCurLightRoad();
					norLastLightRoad();
				}
				else
				{
					_needMove = true;
					GameMap::getCurGameMap()->setAutoMove(true);
				}			
			}

			tryBackToLastPos();
			dialogNode->removeFromParent();
		}
	});

	auto boxpos = Vec2(-105.f, 95.f);
	auto& boxget = info->arrBoxChance;
	int idxbox = 0;
	for (int i = 0; i < sizeof(boxget) / sizeof(int); i++)
	{
		if (boxget[i] == 0)
		{
			continue;
		}

		auto bg = Sprite::createWithSpriteFrameName("guanqia_boxbg.png");
		dialogNode->addChild(bg);
		bg->setPosition(boxpos + Vec2(90.f * idxbox, 0.f));
		auto icon = Sprite::createWithSpriteFrameName(g_boxBtnPicName[i]);
		icon->setScale(0.4f);
		//auto icon = Sprite::createWithSpriteFrameName(String::createWithFormat("quanqia_box_%d.png", i)->getCString());
		dialogNode->addChild(icon);
		icon->setPosition(boxpos + Vec2(90.f * idxbox, 0.f));

		idxbox++;
	}
}

void WordMap::loadMapConfig()
{
	auto node = _mapRoot->getChildByName("eages");
	//node->setVisible(false);
	_mapLinesNode = Node::create();
	_mapRoot->addChild(_mapLinesNode, 2);

	for (auto& eage : node->getChildren())
	{
		eage->setLocalZOrder(2);
		//_mapEageLineVector.push_back(eage);
		eage->setVisible(false);

		auto name = eage->getName();
		auto itr = name.find_first_of('-');
		auto num1 = std::string(name, 0, itr);
		auto num2 = std::string(name, itr + 1);
		int eageleft = atoi(num1.c_str());
		int egaeright = atoi(num2.c_str());

		auto spriteline = Sprite::createWithSpriteFrameName("mapui_line.png");
		spriteline->setPosition((_pos[eageleft] + _pos[egaeright]) * 0.5f);
		spriteline->setLocalZOrder(2);
		spriteline->setName(name);

		_mapLinesNode->addChild(spriteline);

		auto angle = Vec2(1.f, 0.f).getAngle(_pos[eageleft] - _pos[egaeright]);
		spriteline->setRotation(-angle / 3.14 * 180);

		float trueleagth = _pos[eageleft].getDistance(_pos[egaeright]);
		float nowleagth = spriteline->getContentSize().width;
		float scalex = trueleagth / nowleagth;
		spriteline->setScaleX(scalex);
		spriteline->setScaleY(1.3f);

		_mapEageLineVector.push_back(spriteline);

		_graph.addEage(_btns[eageleft], _btns[egaeright]);
	
	}

	node->removeFromParent();

	{
		rapidjson::Document _jsonBezierDoc;
		std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/stage.json");
		_jsonBezierDoc.Parse<0>(contentStr.c_str());
		if (_jsonBezierDoc.HasParseError())
		{
			CCLOG("JSonSkillParseError%s\n", _jsonBezierDoc.GetParseError());
		}

		for (int i = 0; i < s_mapBtnNum; i++)
		{
			const rapidjson::Value& item = _jsonBezierDoc[i];
			const rapidjson::Value& arry = item["sceneid"];

			_arrGuaiQiaMapidx[i] = arry[0].GetDouble();
		}
	}
}

void WordMap::doSearchRoad(Node * start, Node* end)
{
	_startNode = start;
	_endNode = end;
	if (_startNode->getName().compare(_endNode->getName()) == 0)
	{
		_curPosIdx = 1;
		_vertorNode[0] = start;
		_startPos = _players->getPosition();
		_curOffPos = start->getPosition() - _players->getPosition();
		_totalTime = _curOffPos.distance(Vec2(0.f, 0.f)) / 100.f;
		_curTime = 0;
		_needMove = true;
		_finishMoveRoad = false;
	}
	else
	{
		if (_finishMoveRoad)
		{
			_graph.getSeatchPath(start, end, _vertorNode);
			_startPos = _players->getPosition();
			_curPosIdx = _vertorNode.size();
			_nextNode = _vertorNode[_curPosIdx - 1];
			_curOffPos = _nextNode->getPosition() - _players->getPosition();
			_totalTime = _curOffPos.distance(Vec2(0.f, 0.f)) / 100.f;
			_curTime = 0;
			_needMove = true;
			_finishMoveRoad = false;
		}
		else
		{
			
			if (_nextNode->getName().compare(end->getName()) == 0)
			{
				_curPosIdx = 1;
				_vertorNode[0] = end;
				_startPos = _players->getPosition();
				_curOffPos = end->getPosition() - _players->getPosition();
				_totalTime = _curOffPos.distance(Vec2(0.f, 0.f)) / 100.f;
				_curTime = 0;
				_needMove = true;
				_finishMoveRoad = false;
			}
			else
			{
				_startPos = _players->getPosition();
				std::vector<cocos2d::Node*> temp1;
				_graph.getSeatchPath(start, end, temp1);
				std::vector<cocos2d::Node*> temp2;
				_graph.getSeatchPath(_nextNode, end, temp2);

				//_vertorNode = temp2.size() <= temp1.size() ? temp2 : temp1;
				if (temp2.size() <= temp1.size())
				{
					temp2.push_back(start);
					_vertorNode = temp2;
					_curPosIdx = _vertorNode.size() - 1;
					_nextNode = _vertorNode[_curPosIdx - 1];
				}
				else
				{
					_vertorNode = temp1;
					_curPosIdx = _vertorNode.size();
					_nextNode = _vertorNode[_curPosIdx - 1];
				}

				//_curPosIdx = _vertorNode.size();
				//_nextNode = _vertorNode[_curPosIdx - 1];
				_curOffPos = _nextNode->getPosition() - _players->getPosition();
				_totalTime = _curOffPos.distance(Vec2(0.f, 0.f)) / 100.f;
				_curTime = 0;
				_needMove = true;
				_finishMoveRoad = false;
			}
			
		}
		
	}
	
}

void WordMap::hightLightRoad(Node * start, Node * end)
{
	if (start->getName().compare(end->getName()) == 0)
	{
		if (_nextNode != nullptr)
		{
			norCurLightRoad();
			norLastLightRoad();
			_eagesVector.clear();
			auto node = _mapLinesNode;
				//->getChildByName("eages");
			{
				auto eageleft = _curNode->getName();
				auto egaeright = _nextNode->getName();
				auto truname = eageleft + "-" + egaeright;
				auto truname2 = egaeright + "-" + eageleft;
				auto btn = node->getChildByName(truname);
				if (btn == nullptr)
				{
					btn = node->getChildByName(truname2);
				}
				if (btn)
				{
					_eagesVector.pushBack(btn);
					btn->setColor(Color3B(255, 0, 0));
				}
			}
		}	
	}
	else
	{
		if (_finishMoveRoad)
		{
			norCurLightRoad();
			_eagesVector.clear();
			std::vector<Node*> road;
			_graph.getSeatchPath(start, end, road);
			auto node = _mapLinesNode;
				//->getChildByName("eages");
			for (size_t i = 0; i < road.size() - 1; i++)
			{
				auto eageleft = road[i]->getName();
				auto egaeright = road[i + 1]->getName();
				auto truname = eageleft + "-" + egaeright;
				auto truname2 = egaeright + "-" + eageleft;
				auto btn = node->getChildByName(truname);
				if (btn == nullptr)
				{
					btn = node->getChildByName(truname2);
				}

				_eagesVector.pushBack(btn);
				btn->setColor(Color3B(255, 0, 0));
			}
			_eagesVectorLast = _eagesVector;
		}
		else
		{
			if (_nextNode->getName().compare(end->getName()) == 0)
			{
				norCurLightRoad();
				norLastLightRoad();
				_eagesVector.clear();
				auto node = _mapLinesNode;
					//->getChildByName("eages");
				{
					auto eageleft = _curNode->getName();
					auto egaeright = _nextNode->getName();
					auto truname = eageleft + "-" + egaeright;
					auto truname2 = egaeright + "-" + eageleft;
					auto btn = node->getChildByName(truname);
					if (btn == nullptr)
					{
						btn = node->getChildByName(truname2);
					}
					if (btn)
					{
						_eagesVector.pushBack(btn);
						btn->setColor(Color3B(255, 0, 0));
					}
				}
			}
			else
			{
				norLastLightRoad();
				norCurLightRoad();
				_eagesVector.clear();
				std::vector<Node*> temp1; 
				_graph.getSeatchPath(start, end, temp1);
				std::vector<Node*> temp2;
				_graph.getSeatchPath(_nextNode, end, temp2);

				auto road = temp2.size() <= temp1.size() ? temp2 : temp1;
				auto node = _mapLinesNode;
					//->getChildByName("eages");
				for (size_t i = 0; i < road.size() - 1; i++)
				{
					auto eageleft = road[i]->getName();
					auto egaeright = road[i + 1]->getName();
					auto truname = eageleft + "-" + egaeright;
					auto truname2 = egaeright + "-" + eageleft;
					auto btn = node->getChildByName(truname);
					if (btn == nullptr)
					{
						btn = node->getChildByName(truname2);
					}

					_eagesVector.pushBack(btn);
					btn->setColor(Color3B(255, 0, 0));
				}
				//if _cur and _next is same , it will crash
				{
					auto eageleft = _curNode->getName();
					auto egaeright = _nextNode->getName();
					auto truname = eageleft + "-" + egaeright;
					auto truname2 = egaeright + "-" + eageleft;
					auto btn = node->getChildByName(truname);
					if (btn == nullptr)
					{
						btn = node->getChildByName(truname2);
					}
					if (btn)
					{
						_eagesVector.pushBack(btn);
						btn->setColor(Color3B(255, 0, 0));
					}			
				}

				if (_eagesVectorLast.empty())
				{
					_eagesVectorLast = _eagesVector;
				}
			}
		}
	}
	
}

void WordMap::norCurLightRoad()
{
	for (auto &temp : _eagesVector)
	{
		temp->setColor(Color3B(255, 255, 255));
	}
}

void WordMap::norLastLightRoad()
{
	for (auto &temp : _eagesVectorLast)
	{
		temp->setColor(Color3B(255, 255, 255));
	}
}

void WordMap::showShop(int idx)
{
	auto layer = ShopLayer::createLayer(idx);
	MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_Z);
}

void WordMap::menuOnShop(Ref *btnref)
{
	if (!_canTouch)
	{
		return;
	}

	if (GameMap::getCurGameMap()->isInFight())
	{
		return;
	}

	auto btn2 = (Node*)(btnref);

	if (_curNode->getName().compare(btn2->getName()) == 0 && _finishMoveRoad)
	{
		checkShowShop(btn2);
		//int idx = atoi(btn2->getName().c_str());
		//StoryMgr::getInstance()->checkShowStory(idx, true);
	}
	else
	{
		hightLightRoad(_curNode, btn2);
		showDialog(btn2);
	}
}

void WordMap::menuOnPoint(Ref *btnref)
{
	if (!_canTouch)
	{
		return;
	}

	if (GameMap::getCurGameMap()->isInFight())
	{
		return;
	}
	//float arr[4] = { 0.2,0.2,0.3,0.3 };
	//GameMap::getCurGameMap()->showResault(true, 10, 10, arr);

	auto btn2 = (Node*)(btnref);

	if (_curNode->getName().compare(btn2->getName()) == 0 && _finishMoveRoad)
	{
		//checkShowShop(btn2);
		int idx = atoi(btn2->getName().c_str());
		if (!StoryMgr::getInstance()->checkShowStory(idx, true))
		{
			auto name = ParamMgr::getInstance()->getPosName(idx);
			if (name.compare("") != 0 
				//|| idx == UserData::getInstance()->getPlayingGuanqiaIdx()
				)
			{
				WordMap::getInstance()->showEnterFightDiglog(idx);
			}
		}
	}
	else
	{
		hightLightRoad(_curNode, btn2);
		showDialog(btn2);
	}
}

//bool WordMap::checkStory(int idx)
//{
//	GameMap::getCurGameMap()->enterStory(0);
//	return true;
//}

bool WordMap::checkEnemy(int playerpos)
{
	if (StoryMgr::getInstance()->getCurStoryConfig().posid == playerpos)
	{
		return false;
	}

	if (checkShowShop(playerpos))
	{
		return false;
	}

	if (UserData::getInstance()->getIsBossPass(playerpos) == 0)
	{
		showEnterFightDiglog(playerpos, true);
		return true;
	}

	if (CCRANDOM_0_1() < 0.3f )
	{
		showEnterFightDiglog(playerpos, true);

		/*GameMap::getCurGameMap()->enterFight();
		if (!_endmove)
		{
			MainLayer::getCurMainLayer()->moveUpMenu(1.0f);
			GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
		}

		UserData::getInstance()->setPlayerPos(playerpos);
		UserData::getInstance()->saveUserData(true);*/
		return true;
	}

	return false;
}

void WordMap::enterEndless()
{
	int idx = atoi(_curNode->getName().c_str());

	GameMap::getCurGameMap()->enterEndlessFight();
	if (!_endmove)
	{
		MainLayer::getCurMainLayer()->moveUpMenu(1.0f);
		GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
	}
}

void WordMap::enterHardMode()
{
	int idx = atoi(_curNode->getName().c_str());

	GameMap::getCurGameMap()->enterHard();
	if (!_endmove)
	{
		MainLayer::getCurMainLayer()->moveUpMenu(1.0f);
		GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
	}
}

void WordMap::showTargetInfo(int idx)
{
	UserData::getInstance()->setMaxPos(idx);
	int max = UserData::getInstance()->getMaxPos();
	
	//addJingYing(getCurPosIdx());

	auto sprite = Sprite::createWithSpriteFrameName("mapui_target.png");
	sprite->setPosition(_pos[idx] + Vec2(0.f, 40.f));
	_mapRoot->addChild(sprite,5);
	_targetInfoVector.pushBack(sprite);
	sprite->setName(String::createWithFormat("targetsprite_%d", idx)->getCString());

	auto actionmove = Sequence::createWithTwoActions(MoveBy::create(1.f, Vec2(0.f, 30.f)), MoveBy::create(1.f, Vec2(0.f, -30.f)));
	sprite->runAction(RepeatForever::create(actionmove));

	for (int i = 0; i < s_mapBtnNum; i++)
	{
		auto eage = _mapEageLineVector[i];
		auto name = eage->getName();
		auto itr = name.find_first_of('-');
		auto num1 = std::string(name, 0, itr);
		auto num2 = std::string(name, itr + 1);
		int eageleft = atoi(num1.c_str());
		int egaeright = atoi(num2.c_str());

		if (eageleft <= max && egaeright <= max)
		{
			eage->setVisible(true);
		}
		else
		{
			eage->setVisible(false);
		}


		if (i <= max)
		{
			_btns[i]->setVisible(true);
			_graph._vertexInfoVector[i].islocked = false;
		}
		else
		{
			_btns[i]->setVisible(false);
			_graph._vertexInfoVector[i].islocked = true;
		}
	}
}

void WordMap::unShowTargetInfo()
{
	if (_targetInfoVector.size() > 0)
	{
		_targetInfoVector.at(0)->removeFromParent();
	}
	_targetInfoVector.clear();
}

int WordMap::getSceneId(int pos)
{
	return _arrGuaiQiaMapidx[pos]; 
}

void WordMap::checkAndPreMap()
{
	for (int i = 0; i < sizeof(_arrGuaiQiaMapidx); i++)
	{
		if (_curNode->getName().compare(String::createWithFormat("%d", i)->getCString()) == 0)
		{
			while (1)
			{
				if (GameMap::getCurGameMap()->getPrepareCount() == 0)
				{
					GameMap::getCurGameMap()->changeToMap(_arrGuaiQiaMapidx[i]);
					break;
				}
			}

			break;
		}
	}

	for (int i = 0; i < sizeof(_arrGuaiQiaMapidx); i++)
	{
		if(_curPosIdx > 0 && _vertorNode[_curPosIdx - 1]->getName().compare(String::createWithFormat("%d", i)->getCString()) == 0)
		{
			GameMap::getCurGameMap()->prepareMap(_arrGuaiQiaMapidx[i]);
			return;
		}
	}
	
}

bool WordMap::checkShowShop(Node * btn)
{
	int idx = atoi(btn->getName().c_str());
	auto& shopinfo = ParamMgr::getInstance()->getShopAttr();
	int id = 0;
	for (auto&temp : shopinfo)
	{
		if (temp.pos == idx)
		{
			showShop(id);
			return true;
		}
		id++;
	}

	return false;
}

bool WordMap::checkShowShop(int posidx)
{
	auto& shopinfo = ParamMgr::getInstance()->getShopAttr();
	int id = 0;
	for (auto&temp : shopinfo)
	{
		if (temp.pos == posidx)
		{
			return true;
		}
		id++;
	}

	return false;
}

void WordMap::update(float delta)
{
	if (_pauseGame || !isVisible())
	{
		return;
	}

	if (_touchScaleNode->getScaleX() < 0.7f && _isShowPoints == true)
	{
		changeMapLineAndPoints(false);
	}
	else if (_touchScaleNode->getScaleX() >= 0.7f && _isShowPoints == false)
	{
		changeMapLineAndPoints(true);
	}

	updateJiantou();

	if (_needMove && !_finishMoveRoad)
	{
		if (_curPosIdx > 0)
		{
			_curTime += delta;
			_players->setPosition(_startPos + _curOffPos * _curTime / _totalTime);
			if (_curTime >= _totalTime)
			{
				bool needshop = false;
				bool story = false;
				bool bossguanqia = false;

				float totaltimebak = _totalTime;
				_curPosIdx -= 1;
				_curNode = _vertorNode[_curPosIdx];
				_players->setPosition(_vertorNode[_curPosIdx]->getPosition());
				_startPos = _players->getPosition();

				if (_curPosIdx > 0)
				{
					_nextNode = _vertorNode[_curPosIdx - 1];
					_curOffPos = _nextNode->getPosition() - _players->getPosition();

					checkAndPreMap();

					_totalTime = _curOffPos.getDistance(Vec2(0.f, 0.f)) / 100.f;
					_curTime = 0;
					_finishMoveRoad = false;
				}
				else
				{
					_nextNode = nullptr;
					_curOffPos = Vec2(0.f, 0.f);

					norCurLightRoad();
					norLastLightRoad();
					_nextNode = nullptr;
					_finishMoveRoad = true;
					_needMove = false;
					GameMap::getCurGameMap()->setAutoMove(false);

					int idx = atoi(_endNode->getName().c_str());
					story = StoryMgr::getInstance()->checkShowStory(idx, true);

					if (UserData::getInstance()->getIsBossPass(idx))
					{
						UserData::getInstance()->setPlayerPos(getCurPosIdx());
						UserData::getInstance()->saveUserData();
					}


					endMove();

					needshop = checkShowShop(_vertorNode[0]);

					if (story == false && StoryMgr::getInstance()->isStoryPos(idx))
					{
#if 0
						GameMap::getCurGameMap()->enterFight();
						if (!_endmove)
						{
							MainLayer::getCurMainLayer()->moveUpMenu(1.0f);
							GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
						}
#else
						WordMap::getInstance()->showEnterFightDiglog(idx);
#endif
						
						bossguanqia = true;
					}

					GameMap::getCurGameMap()->changeToMap(_arrGuaiQiaMapidx[idx]);
				}

				int idx = atoi(_curNode->getName().c_str());

				if (totaltimebak != 0.0 && !needshop && !story && bossguanqia == false&& checkEnemy(idx))
				{
#if 0
					norCurLightRoad();
					norLastLightRoad();
					_nextNode = nullptr;
					_finishMoveRoad = true;
					_needMove = false;
					GameMap::getCurGameMap()->setAutoMove(false);
#endif
					GameMap::getCurGameMap()->setAutoMove(false);
					_needMove = false;
					return;
				}
			}
		}
		else
		{
			
		}
	}
	else
	{
		GameMap::getCurGameMap()->setAutoMove(false);
	}
}

void WordMap::startMove()
{
	CCLOG("start move");
	Player::getInstance()->playMove();
	MainLayer::getCurMainLayer()->moveDownMenu(1.5f);

	if (!GameMap::getCurGameMap()->isCurMapUp())
	{
		auto action = Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]() {
			GameMap::getCurGameMap()->setAutoMove(true);
		}));
		this->runAction(action);
	}
	else
	{
		auto action = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
			GameMap::getCurGameMap()->setAutoMove(true);
		}));
		this->runAction(action);
	}

	GameMap::getCurGameMap()->moveUpEffect(0.5f, false);
	_endmove = false;


	
}

void WordMap::endMove()
{
	CCLOG("end move");
	Player::getInstance()->playIdle();
	MainLayer::getCurMainLayer()->moveUpMenu(1.f);
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([]() {
		GameMap::getCurGameMap()->moveUpEffect(0.5f, true);
	}));

	this->runAction(action);

	_endmove = true;

	//_players->stopAllActions();
	//((Sprite*)(_players))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mapui_playerpos_0.png"));
}

int WordMap::getCurPosIdx()
{
	if (_curNode)
	{
		return atoi(_curNode->getName().c_str());
	}
	else
	{
		return 0;
	}
}

void WordMap::changeMapLineAndPoints(bool show)
{
	if (show)
	{
		_mapPointsMenu->setVisible(true);
		_mapLinesNode->setVisible(true);
		_isShowPoints = true;
	}
	else
	{
		_mapPointsMenu->setVisible(false);
		_mapLinesNode->setVisible(false);
		_isShowPoints = false;
	}
}

void WordMap::setStartPos()
{
	auto node = _btns[UserData::getInstance()->getPlayerPos()];
	_touchScaleNode->setPosition(_touchScaleNode->getPosition() - node->getPosition());
	_touchScaleNode->checkMove();
}

void WordMap::pauseGame()
{
	_pauseGame = true;
}

void WordMap::resumGame()
{
	_pauseGame = false;
}

void WordMap::updateJiantou()
{
	auto screenPlayerpos = _players->convertToWorldSpaceAR(Vec2(0.f, 0.f));
	if (!Rect(0, 360, 640, VisibleRect::top().y - 360).containsPoint(screenPlayerpos))
	{
		_jiantouPlayer->setVisible(true);

		Vec2 jiantouSize = Vec2(600, VisibleRect::top().y - 300.f);


		auto rayStoryPos = screenPlayerpos - _jiantouCenter;
		auto angle = (rayStoryPos.getAngle(Vec2(0.f, 1.f))) * 180.f / 3.14f;
		_jiantouPlayer->setRotation(angle);

		auto storyRayK = rayStoryPos.y / rayStoryPos.x;
		auto stand = Vec2(640.f, VisibleRect::top().y) - _jiantouCenter;
		auto k = stand.y / stand.x;

		auto crosspos = Vec2(0.f, 0.f);

		if (storyRayK > k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(jiantouSize.y / 2.f / storyRayK, jiantouSize.y / 2.f);
			}
			else
			{
				crosspos = Vec2(-jiantouSize.y / 2.f / storyRayK, -jiantouSize.y / 2.f);
			}
		}
		else if (storyRayK < k && storyRayK > -k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(jiantouSize.x / 2.f, jiantouSize.x / 2.f * storyRayK);
			}
			else
			{
				crosspos = Vec2(-jiantouSize.x / 2.f, -jiantouSize.x / 2.f * storyRayK);
			}
		}
		else if (storyRayK < -k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(-jiantouSize.y / 2.f / storyRayK, -jiantouSize.y / 2.f);
			}
			else
			{
				crosspos = Vec2(jiantouSize.y / 2.f / storyRayK, jiantouSize.y / 2.f);
			}
		}

		_jiantouPlayer->setPosition(crosspos * 0.8f + _jiantouCenter);
	}
	else
	{
		_jiantouPlayer->setVisible(false);
	}

	if (StoryMgr::getInstance()->isStoryOver())
	{
		hideJiantou();
		return;
	}

	auto storyid = StoryMgr::getInstance()->getCurStoryConfig().posid;
	auto screenStorypos = _targetInfoVector.at(0)->convertToWorldSpaceAR(Vec2(0.f,0.f));
	if (!Rect(0, 360, 640, VisibleRect::top().y - 360).containsPoint(screenStorypos))
	{
		_jiantouStory->setVisible(true);

		Vec2 jiantouSize = Vec2(600, VisibleRect::top().y - 360.f);
	

		auto rayStoryPos = screenStorypos - _jiantouCenter;
		auto angle = (rayStoryPos.getAngle(Vec2(0.f, 1.f))) * 180.f / 3.14f;
		_jiantouStory->setRotation(angle);

		auto storyRayK = rayStoryPos.y / rayStoryPos.x;
		auto stand = Vec2(640.f, VisibleRect::top().y) - _jiantouCenter;
		auto k = stand.y / stand.x;
		
		auto crosspos = Vec2(0.f, 0.f);
		
		if (storyRayK > k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(jiantouSize.y/2.f / storyRayK , jiantouSize.y/2.f);
			}
			else
			{
				crosspos = Vec2(-jiantouSize.y/2.f / storyRayK, -jiantouSize.y/2.f);
			}
		}
		else if (storyRayK < k && storyRayK > -k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(jiantouSize.x/2.f, jiantouSize.x/2.f * storyRayK);
			}
			else
			{
				crosspos = Vec2(-jiantouSize.x / 2.f, -jiantouSize.x / 2.f * storyRayK);
			}
		}
		else if (storyRayK < -k)
		{
			if (rayStoryPos.x > 0)
			{
				crosspos = Vec2(-jiantouSize.y/2.f / storyRayK, -jiantouSize.y/2.f);
			}
			else
			{
				crosspos = Vec2(jiantouSize.y/2.f / storyRayK, jiantouSize.y/2.f);
			}
		}

		_jiantouStory->setPosition(crosspos * 0.8f + _jiantouCenter);
	}
	else
	{
		_jiantouStory->setVisible(false);
	}
}

void WordMap::hideJiantou()
{
	_jiantouStory->setVisible(false);
}

void WordMap::hideFinget()
{
	if (_fignerAni && UserData::getInstance()->isNeedGuide() == false)
	{
		_fignerAni->removeFromParent();
		_fignerAni = nullptr;
	}
}

bool WordMap::checkIsBoss(int idx)
{
	auto info = ParamMgr::getInstance()->getStageInfo(idx);
	if (info->stageType == 2)
	{
		return true;
	}
	return false;
}

void WordMap::updataPointsBtns()
{
	/*int max = UserData::getInstance()->getPlayingGuanqiaIdx();
	while (checkShowShop(max))
	{
		UserData::getInstance()->setPlayingGuanqiaIdx(max + 1);
		max++;
	}*/

	for (int i = 0; i < s_mapBtnNum; i++)
	{
		auto eage = _mapEageLineVector[i];
		auto name = eage->getName();
		auto itr = name.find_first_of('-');
		auto num1 = std::string(name, 0, itr);
		auto num2 = std::string(name, itr + 1);
		int eageleft = atoi(num1.c_str());
		int egaeright = atoi(num2.c_str());

		//if (eageleft <= max && egaeright <= max)
		//{
			//eage->setColor(Color3B(255, 255, 255));
		//}
		//else
		//{
			//eage->setColor(Color3B(128, 128, 128));
		//}
		bool pass = UserData::getInstance()->getIsBossPass(i) == 1 ? true : false;
		if (checkShowShop(i))
		{
			pass = true;
		}
		
		if (!pass)
		{
			//static_cast<glui::GLMenuItemSprite*>(_btns[i])->setEnabled(false);
			static_cast<glui::GLMenuItemSprite*>(_btns[i])->setColor(Color3B(128, 128, 128));
			//_graph._vertexInfoVector[i].islocked = true;
		}
		else
		{
			//static_cast<glui::GLMenuItemSprite*>(_btns[i])->setEnabled(true);
			static_cast<glui::GLMenuItemSprite*>(_btns[i])->setColor(Color3B(255, 255, 255));
			//_graph._vertexInfoVector[i].islocked = false;
		}
	}
}

void WordMap::tryBackToLastPos()
{
	int idx = getCurPosIdx();
	if (UserData::getInstance()->getIsBossPass(idx) == 0)
	{
		if (_vertorNode.size() >= 2)
		{
			norCurLightRoad();
			norLastLightRoad();
			_nextNode = nullptr;
			_finishMoveRoad = true;
			_needMove = false;
			GameMap::getCurGameMap()->setAutoMove(false);

			endMove();
			int idxlast = atoi(_vertorNode[0]->getName().c_str());
			for (int i = _vertorNode.size() - 1; i >=0 ;  i--)
			{	
				auto tempidx = atoi(_vertorNode[i]->getName().c_str());	
				if (UserData::getInstance()->getIsBossPass(tempidx) == 0 && checkShowShop(tempidx) == false)
				{
					if (i == _vertorNode.size() - 1)
					{						
						idxlast = atoi(_vertorNode[i]->getName().c_str());									
					}
					else
					{
						idxlast = atoi(_vertorNode[i + 1]->getName().c_str());
					}				
					break;
				}		
			}
				
			
			//_curNode = _vertorNode[idxlast];
			//int idx = atoi(_curNode->getName().c_str());
			int idx = idxlast >=0 ? idxlast : 0;
			//_curNode = _vertorNode[nodeidx];
			_curNode = _btns[idx];
			_players->setPosition(_pos[idx]);

			UserData::getInstance()->setPlayerPos(idx);
			UserData::getInstance()->saveUserData(true);
		}
	}
}

void WordMap::updateBoxGetInfo(int idx)
{
	auto info = ParamMgr::getInstance()->getStageInfo(idx);
	int flagIsBox = info->boxmonsId;

	if (UserData::getInstance()->getMaxPos() < idx)
	{
		return;
	}

	if (flagIsBox != -1 && UserData::getInstance()->getIsBossPass(idx) == 1 && UserData::getInstance()->getBoxGet(idx) == 0)
	{
		auto lastsprite = _mapRoot->getChildByName(String::createWithFormat("boxget_%d", idx)->getCString());
		if (lastsprite == nullptr)
		{
			auto sprite = Sprite::createWithSpriteFrameName("mapui_box.png");
			_mapRoot->addChild(sprite, 5);
			sprite->setName(String::createWithFormat("boxget_%d", idx)->getCString());
			sprite->setPosition(_pos[idx] + Vec2(0.f, 40.f));

			auto actionmove = Sequence::createWithTwoActions(MoveBy::create(1.f, Vec2(0.f, 30.f)), MoveBy::create(1.f, Vec2(0.f, -30.f)));
			sprite->runAction(RepeatForever::create(actionmove));
		}
		
	}
	else if(flagIsBox != -1)
	{
		auto sprite = _mapRoot->getChildByName(String::createWithFormat("boxget_%d", idx)->getCString());
		if (sprite)
		{
			sprite->stopAllActions();
			sprite->removeFromParent();
		}
	}
}

void WordMap::updateNewMapBtn()
{
	if (_newMapBtn)
	{
		if (UserData::getInstance()->getIsBossPass(80))
		{
			_newMapBtn->setVisible(true);
		}
		else
		{
			_newMapBtn->setVisible(false);
		}
	}
}

void WordMap::popNearyBoss()
{
	int max = UserData::getInstance()->getMaxPos();
	for (int i = max - 1; i > 0; i--)
	{
		if (checkIsBoss(i) == true)
		{
			menuOnPoint(_btns[i]);
			return;
		}
	}
}
