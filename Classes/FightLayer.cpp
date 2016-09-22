#include "FightLayer.h"
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
#include "ParamData.h"
#include "GameCSLoader.h"
#include "Scenes/GameMap.h"
#include "GameLayer.h"
#include "Story/StoryMgr.h"
#include "Scenes/GameMap.h"
#include "UserData.h"
#include "GameUtils.h"
#include "CrushFSM/CrushFSM.h"
#include "AchieveMgr.h"
#include "DayActiveMgr.h"
#include "NewMapOpenMgr.h"
#include "Defines.h"
#include "Role.h"
#include "WeaponControl.h"
#include "FightUtil.h"

USING_NS_CC;
using namespace cocostudio;
//USING_NS_DB;

FightLayer *FightLayer::s_pInstance = nullptr;

FightLayer *FightLayer::getInstance()
{
	return s_pInstance;
}


FightLayer::FightLayer()
	:_curWaveIndex(0),
	_totalExp(0),
	_gameState(0),
	_totalGold(0),
	_pStageInfo(nullptr),
	_endlessFloor(0),
	_curPlayerPosId(-1)
{
	s_pInstance = this;
}


FightLayer::~FightLayer()
{
	s_pInstance = nullptr;
}


// on "init" you need to initialize your instance
bool FightLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto pPlayer = Player::create();
	this->addChild(pPlayer);

	float moveTime = 0.5f;
	float bufferTime = 0.2f;
	Player::getInstance()->placeRole(Vec2(ParamData::ROLE_START_POS_X/2, ParamData::ROLE_START_POS_Y));
	Player::getInstance()->moveToOutsidePos(_curPlayerPosId, ParamData::POS_X_INTERVAL, moveTime, bufferTime);

//	pPlayer->playIdle();
//	Player::getInstance()->placeRole(Vec2(ParamData::ROLE_START_POS_X, ParamData::ROLE_START_POS_Y));

    return true;
}


void FightLayer::update(float delta)
{
	if (2 == _gameState || 3 == _gameState)
	{
		if (Player::getInstance()->isCurAttFinish())
		{
			bool isAllFinish = true;
			for (int i = 0; i < _arrCurMonster.size(); i++)
			{
				auto pMonster = _arrCurMonster.at(i);
				if (!pMonster->getIsAttFinish())
				{
					isAllFinish = false;
					break;
				}
			}

			if (isAllFinish)
			{
				CrushLayer::getInstance()->getStateMac()->changeState(SettleState::getInstance());
				_gameState = 4;
			}
		}
	}
	else if (1 == _gameState)
	{
		if (FightState::Idle == _curFightState)
		{
			if (_curPlayerPosId < _curWaveIndex)
			{
				if (1 == GameLayer::getInstance()->getFightType())
				{
					GameLayer::getInstance()->setCurFightIndex(_endlessFloor+1, 0);
				}
				else
				{
					GameLayer::getInstance()->setCurFightIndex(this->getPlayerPosIndex() + 1, this->getTotalPosCount(), 2.f);
				}
				this->moveRoleToNextPos();
				CrushLayer::getInstance()->getShieldLayer()->setVisible(true);
				GameLayer::getInstance()->setEquipBtnEnabled(false);
				if (CrushLayer::getInstance()->isVisible() && 1 == GameLayer::getInstance()->getFightType())
				{
					//start round count down
					GameLayer::getInstance()->startBarCountdown();
				}
			}
			else if (_curPlayerPosId == _curWaveIndex)
			{
				/*move end*/
				if (!_arrIdleMonster.empty())
				{
					this->addIdleToCurMonster();
					CrushLayer::getInstance()->getShieldLayer()->setVisible(false);
					GameLayer::getInstance()->setEquipBtnEnabled(true);

					if (this->isFirstWave())
					{
						if (GameMap::getCurGameMap()->checkShowStory(0))
						{
							_curFightState = FightState::WaitSelect;
							GameMap::getCurGameMap()->showStory(0);
						}
						else
						{
							GameLayer::getInstance()->endStory(0);
							_curFightState = FightState::Fight;
//							this->notifyMonsterFight();
						}
					}
					else if (this->isLastWave())
					{
						if (GameMap::getCurGameMap()->checkShowStory(1))
						{
							CrushLayer::getInstance()->setVisible(false);
							_curFightState = FightState::WaitSelect;
							GameMap::getCurGameMap()->showStory(1);
						}
						else
						{
							_curFightState = FightState::Fight;
//							this->notifyMonsterFight();
						}

					}
					else
					{
						_curFightState = FightState::Fight;
//						this->notifyMonsterFight();
					}
				}
				else
				{
					if (1 == GameLayer::getInstance()->getFightType())
					{
						/*enter new floor*/
						this->enterNewFloor();
						_curFightState = FightState::Idle;
						AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::FLOOR, 1);
					}
					else
					{
						if (_curFloor < (_arrMonsterWaveInfo->size() - 1))
						{
							/*enter new floor*/
							this->enterNewFloor();
							_curFightState = FightState::Idle;
						}
						else
						{
							/*win*/
							_gameState = 2;
						}
					}
				}
			}
		}
		else if (FightState::Move == _curFightState)
		{
			
		}
		else if (FightState::Fight == _curFightState)
		{
			if (this->isLastWave() && GameMap::getCurGameMap()->checkShowStory(2))
			{
				if (1 == _arrCurMonster.size() && _arrCurMonster.at(0)->getIsDead() && Player::getInstance()->isCurAttFinish())
				{
//					auto pFloorWave = _arrMonsterWaveInfo->at(_curFloor);
//					GameLayer::getInstance()->setCurFightIndex(pFloorWave.size(), pFloorWave.size());

					GameMap::getCurGameMap()->showStory(2);
					Player::getInstance()->resetFightData();
					CrushLayer::getInstance()->setVisible(false);
					_curFightState = FightState::WaitSelect;
				}
			}
			else
			{
				if (_arrCurMonster.empty() && Player::getInstance()->isCurAttFinish())
				{
					auto pFloorWave = _arrMonsterWaveInfo->at(_curFloor);
					if (this->isLastWave())
					{
						_gameState = 2;
					}
					else
					{
						_curWaveIndex++;
						_curFightState = FightState::Idle;

// 						if ((_curWaveIndex+1) <= pFloorWave.size())
// 						{
// 							GameLayer::getInstance()->setCurFightIndex(_curWaveIndex + 1, pFloorWave.size());
// 						}
						if (_curWaveIndex == pFloorWave.size())
						{
							_curWaveIndex--;
						}
					}
				}
			}
		}
		else if (FightState::WaitSelect == _curFightState)
		{

		}

		if (Player::getInstance()->getIsDead() && 2 != _gameState && FightState::WaitSelect != _curFightState)
		{
			if (Player::getInstance()->getIsWaitRelive())
			{
				bool isAllFinish = true;
				for (int i = 0; i < _arrCurMonster.size(); i++)
				{
					auto pMonster = _arrCurMonster.at(i);
					if (!pMonster->getIsAttFinish())
					{
						isAllFinish = false;
						break;
					}
				}

				if (isAllFinish)
				{
					GameLayer::getInstance()->popReborn();
					_gameState = 5;
				}
//				_gameState = 5;
			}
			else
			{
				_gameState = 3;
			}
			return;
		}
	}
	else if (5 == _gameState)
	{
		if (Player::getInstance()->getIsDead() && 2 != _gameState && FightState::WaitSelect != _curFightState)
		{
			if (Player::getInstance()->getIsWaitRelive())
			{
				_gameState = 5;
			}
			else
			{
				_gameState = 3;
			}
			return;
		}
	}
}


Vector<Monster *> *FightLayer::getCurArrMonster()
{
	return &_arrCurMonster;
}


Vector<Monster *> *FightLayer::getIdleArrMonster()
{
	return &_arrIdleMonster;
}


bool FightLayer::createFloorMonster(int floorIndex)
{
#if (1 == CC_ENABLE_DEBUG_MONSTER_ANIM)
	//debug monster anim
	
	for (int i = 0; i < 1; i++)
	{
		auto newMonster = Monster::create(52, 0);

		newMonster->setWaveCount(1);
		this->addChild(newMonster, -1);

		Vec2 pos = CrushUtil::getMonsterPos(0, 1, 0);
		newMonster->setPosition(pos);

		_arrIdleMonster.pushBack(newMonster);
	}
	return true;

#endif

	int totalFloor = _arrMonsterWaveInfo->size();
	
	if (floorIndex < totalFloor)
	{
		std::vector<std::vector<int>> floorWave = _arrMonsterWaveInfo->at(floorIndex);

		int boxMonsterWave = -1;
		if (0 == floorIndex && GameLayer::getInstance()->getIsHaveBoxMonster())
		{
			if (floorWave.size() <= 2)
			{
				boxMonsterWave = 0;
			}
			else
			{
				boxMonsterWave = ToolsUtil::getRandomInt(0, floorWave.size() - 2);
			}
			_boxMonsterWaveIdx = boxMonsterWave;
		}

		for (int i=0; i<floorWave.size(); i++)
		{
			auto &waveInfo = floorWave.at(i);
			int waveSize = (boxMonsterWave == i) ? waveInfo.size() + 1 : waveInfo.size();

			for (int j = 0; j < waveSize; j++)
			{
				int monsterId;
				if (boxMonsterWave == i && j == (waveSize-1))
				{
					monsterId = _pStageInfo->boxmonsId;
				}
				else
				{
					monsterId = waveInfo.at(j);
				}

				auto newMonster = Monster::create(monsterId, j);
				newMonster->setWaveCount(waveSize);
				this->addChild(newMonster, -1);

				Vec2 pos = CrushUtil::getMonsterPos(j, waveSize, i);
				newMonster->setPosition(pos);

				_arrIdleMonster.pushBack(newMonster);

				if (floorIndex == (totalFloor - 1) && i == (floorWave.size() - 1) && j == (waveSize - 1))
				{
					newMonster->setIsLastMonster(true);
				}
			}
			/*
			for (int j = 0; j < waveInfo.size(); j++)
			{
				int monsterId = waveInfo.at(j);

				auto newMonster = Monster::create(monsterId, j);
				newMonster->setWaveCount(waveInfo.size());
				this->addChild(newMonster, -1);

				Vec2 pos = CrushUtil::getMonsterPos(j, waveInfo.size(), i);
				newMonster->setPosition(pos);

				_arrIdleMonster.pushBack(newMonster);

				if (floorIndex == (totalFloor-1) && i == (floorWave.size()-1) && j == (waveInfo.size()-1))
				{
					newMonster->setIsLastMonster(true);
				}
			}*/			
		}
	}
	else
	{
		return false;
	}
	

	return true;
}


void FightLayer::addIdleToCurMonster()
{
	if (_arrCurMonster.empty())
	{
#if (1 == CC_ENABLE_DEBUG_MONSTER_ANIM)
		//debug monster anim
		int countTmp = _arrIdleMonster.size();
		for (int i = 0; i < countTmp; i++)
		{
		auto pMonster = _arrIdleMonster.front();

		_arrCurMonster.pushBack(pMonster);

		_arrIdleMonster.erase(0);
		}

		return;
#endif
		
		int count = _arrMonsterWaveInfo->at(_curFloor).at(_curWaveIndex).size();

		if (0 == _curFloor && _boxMonsterWaveIdx == _curWaveIndex)
		{
			count++;
		}

		for (int i=0; i<count; i++)
		{
			auto pMonster = _arrIdleMonster.front();

			_arrCurMonster.pushBack(pMonster);

			_arrIdleMonster.erase(0);
		}
	}	
}


void FightLayer::startFight()
{
	_totalExp = 0;
	_curWaveIndex = 0;
	_curPlayerPosId = -1;
	_curFloor = -1;
	_endlessFloor = -1;
	_gameState = 1;
	_openBoxCount = 0;
	_boxMonsterWaveIdx = -1;

	Player::getInstance()->initFightData();

	if (1 == GameLayer::getInstance()->getFightType())
	{
		_pStageInfo = ParamMgr::getInstance()->getStageInfo(-1);
#if (0 == CC_ENABLE_NEW_PARAM)
		//set base lv
		int baseLv = CrushUtil::getEndlessBaseLv(Player::getInstance()->getBaseLv());
		Player::getInstance()->setBaseLv(baseLv);
#endif
	}
	else if (3 == GameLayer::getInstance()->getFightType())
	{
		_pStageInfo = ParamMgr::getInstance()->getStageInfo(-2);
		int dungeonType = GameMap::getCurGameMap()->getDungeonIdx();
		int dungeonIndex = ToolsUtil::getRandomInt(5 * dungeonType, 5 * (dungeonType+1) - 1);

		_pDungeonDeatilInfo = ParamMgr::getInstance()->getDungeonInfo(dungeonIndex);
		_curDungeonStep = _pDungeonDeatilInfo->stepCount;
		std::vector<int> vectorDungeonIdx;
		for (int i = 0; i < sizeof(_arrDungeonAnim) / sizeof(_arrDungeonAnim[0]); i++)
		{
			if (i<_pDungeonDeatilInfo->arrDesInfo.size())
			{
				int desId = _pDungeonDeatilInfo->arrDesInfo[i].crushId;
				_arrDungeonAnim[i] = 0;
				vectorDungeonIdx.push_back(desId);
			}
			else
			{
				_arrDungeonAnim[i] = -1;
			}
		}

		GameLayer::getInstance()->showDungeonInfo(vectorDungeonIdx);
		GameLayer::getInstance()->setDungeonStep(_curDungeonStep);

		for (int i = 0; i < sizeof(_arrDungeonAnim) / sizeof(_arrDungeonAnim[0]); i++)
		{
			if (i < _pDungeonDeatilInfo->arrDesInfo.size())
			{
				int desId = _pDungeonDeatilInfo->arrDesInfo[i].crushId;

				GameLayer::getInstance()->setDungeonMissionNum(desId, 0, _pDungeonDeatilInfo->arrDesInfo[i].crushCount);
			}
		}
	}
	else if (0 == GameLayer::getInstance()->getFightType() || 2 == GameLayer::getInstance()->getFightType())
	{
		_pStageInfo = ParamMgr::getInstance()->getStageInfo(GameLayer::getInstance()->getSceneId());
	}
	_arrMonsterWaveInfo = &_pStageInfo->arrMonsterWaveInfo;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	float totalTime = 0.f;
	for (int i = 0; i < _arrMonsterWaveInfo->size(); i++)
	{
		auto arrFloorInfo = _arrMonsterWaveInfo->at(i);
		for (int j = 0; j < arrFloorInfo.size(); j++)
		{
			auto arrWaveInfo = arrFloorInfo.at(j);

			for (int k = 0; k < arrWaveInfo.size(); k++)
			{
				int monsterId = arrWaveInfo.at(k);
				auto pMonsterInfo = ParamMgr::getInstance()->getMonsterInfo(monsterId);

				if (0 == pMonsterInfo->designType)
				{
					totalTime += FightUtil::DESIGN_NOR_MONSTER_TIME;
				}
				else if (1 == pMonsterInfo->designType)
				{
					totalTime += FightUtil::DESIGN_ELITE_MONSTER_TIME;
				}
				else if (2 == pMonsterInfo->designType)
				{
					totalTime += FightUtil::DESIGN_BOSS_TIME;
				}
			}
		}
	}
	CCLOG("*****************sceneId = %d, designTime = %f*********************", _pStageInfo->id, totalTime);
#endif

#if (1 == CC_ENABLE_NEW_PARAM)
	this->initDesignInfo();
#endif

	if (0 == GameLayer::getInstance()->getFightType() && -1 != _pStageInfo->boxmonsId && UserData::getInstance()->getBoxGet(GameLayer::getInstance()->getSceneId()) == 0)
	{
		GameLayer::getInstance()->setIsHaveBoxMonster(true);
	}
	else
	{
		GameLayer::getInstance()->setIsHaveBoxMonster(false);
	}

	GameLayer::getInstance()->setCurFightIndex(0, this->getTotalPosCount());

	this->enterNewFloor();

	this->scheduleUpdate();
}


void FightLayer::stopFight()
{
	this->unscheduleUpdate();

	if (1 == GameLayer::getInstance()->getFightType())
	{
		GameLayer::getInstance()->endBarCountdown();
	}

	for (int i = _arrCurMonster.size() - 1; i >= 0; i--)
	{
		auto pMonster = _arrCurMonster.at(i);
		_arrCurMonster.erase(i);
		pMonster->removeFromParent();
	}

	for (int i = _arrIdleMonster.size() - 1; i >= 0; i--)
	{
		auto pMonster = _arrIdleMonster.at(i);
		_arrIdleMonster.erase(i);
		pMonster->removeFromParent();
	}

// 	auto arrCount = CrushLayer::getInstance()->getCrushEleCount();
// 	if ((arrCount[0] + arrCount[1] + arrCount[2] + arrCount[3]) > 0)
// 	{
// 		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
// 		{
// 			int exp = _totalExp * arrCount[i] / (arrCount[0] + arrCount[1] + arrCount[2] + arrCount[3]);
// 			UserData::getInstance()->addPlayerCurExp(i, exp);
// 		}
// 	}

	_gameState = 0;
	Player::getInstance()->resetFightData();
}


void FightLayer::moveRoleToNextPos()
{
	float moveTime = 2.f;
	float bufferTime = 0.3f;
	_curPlayerPosId++;
	Player::getInstance()->moveToPos(_curPlayerPosId, ParamData::POS_X_INTERVAL, moveTime, bufferTime);
//	GameMap::getCurGameMap()->moveMap(moveTime, -ParamData::POS_X_INTERVAL);
//CallFunc::create([=] {GameMap::getCurGameMap()->moveMap(moveTime, -ParamData::POS_X_INTERVAL); }), 


//	if (_curWaveIndex != _arrMonsterWaveInfo.at(_curFloor).size())
	{
		Vec2 newPos = this->getPosition() - Vec2(ParamData::POS_X_INTERVAL, 0.f);
		this->runAction(Sequence::create(DelayTime::create(bufferTime / 2.f), CallFunc::create([=] {GameMap::getCurGameMap()->moveMap(moveTime, -ParamData::POS_X_INTERVAL); }),MoveTo::create(moveTime, newPos), DelayTime::create(bufferTime / 2.f), CallFunc::create([=] {_curFightState = FightState::Idle; }), nullptr));
//		this->runAction(Sequence::create(MoveTo::create(moveTime, newPos), DelayTime::create(bufferTime), CallFunc::create([=] {_curFightState = FightState::Idle; }), nullptr));
	}
	_curFightState = FightState::Move;
}


void FightLayer::moveRoleToOutSidePos(int moveType)
{
	float moveTime = 2.f;
	float bufferTime = 0.3f;
	if (0 == moveType || 1 == moveType)
	{
		_curPlayerPosId++;
		Player::getInstance()->moveToOutsidePos(_curPlayerPosId, ParamData::POS_X_INTERVAL, moveTime, bufferTime);
	}

	if (0 == moveType || 2 == moveType)
	{
		Vec2 newPos = this->getPosition() - Vec2(ParamData::POS_X_INTERVAL, 0.f);
		this->runAction(Sequence::create(DelayTime::create(bufferTime / 2.f), CallFunc::create([=] {GameMap::getCurGameMap()->moveMap(moveTime, -ParamData::POS_X_INTERVAL); }), MoveTo::create(moveTime, newPos), DelayTime::create(bufferTime / 2.f), CallFunc::create([=] {_curFightState = FightState::Idle; }), nullptr));
		//		this->runAction(Sequence::create(MoveTo::create(moveTime, newPos), DelayTime::create(bufferTime), CallFunc::create([=] {_curFightState = FightState::Idle; }), nullptr));
	}
	_curFightState = FightState::Move;
}


void FightLayer::enterNewFloor()
{
	_curPlayerPosId = -1;
	_curWaveIndex = 0;
	_curFloor++;

	if (1 == GameLayer::getInstance()->getFightType())
	{
		_endlessFloor++;
		if (_curFloor == _pStageInfo->arrMonsterWaveInfo.size())
		{
#if (0 == CC_ENABLE_NEW_PARAM)
			Player::getInstance()->setBaseLv(Player::getInstance()->getBaseLv() + ParamData::ENDLESS_STAGE_ADD_LV);
#endif
			_curFloor = 0;
		}
		if (_endlessFloor > 0)
		{
			int totalFloor = UserData::getInstance()->getTotalFloor();
			UserData::getInstance()->setTotalFloor(totalFloor+1);

			DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYTA);
			if (totalFloor >= 100 - 1)
			{
				NewMapOpenMgr::getInstance()->tryGet(MapOpenType::TA200, false);
			}
			if (totalFloor >= 300 - 1)
			{
				NewMapOpenMgr::getInstance()->tryGet(MapOpenType::TA500, false);
			}
			NewMapOpenMgr::getInstance()->popGet();

			int playerLv = UserData::getInstance()->getPlayerCurLv(0);
			int totalScore = UserData::getInstance()->getWeeklyScore();
			int perScore = 10 + playerLv * 2;
			UserData::getInstance()->setWeeklyScore(totalScore + perScore);
		}
	}

	this->createFloorMonster(_curFloor);

//	GameLayer::getInstance()->setFightName(_pStageInfo->arrFloorName[_curFloor]);

	_curFightState = FightState::Idle;
	Player::getInstance()->placeRole(Vec2(ParamData::ROLE_START_POS_X, ParamData::ROLE_START_POS_Y));
	Player::getInstance()->playIdle();

	this->setPosition(Vec2::ZERO);
}


Monster *FightLayer::getFirstMonster()
{
	if (!_arrCurMonster.empty())
	{
		return _arrCurMonster.front();
	}

	return nullptr;
}


void FightLayer::removeMonster(Monster *pMonster)
{
	_arrCurMonster.eraseObject(pMonster);
	_arrIdleMonster.eraseObject(pMonster);
}


void FightLayer::showValueChange(int type, float damage, const Vec2 &pos)
{
	Label *pLbl = nullptr;
	__String *strContent = nullptr;

	if (2 == type)
	{
		pLbl = Label::createWithCharMap("fonts/num_add_def.png", 19, 19, '.');
		if (damage > 0.f)
		{
			strContent = __String::createWithFormat("/%d", (int)damage);
		}
		else
		{
			strContent = __String::createWithFormat(".%d", (int)(-damage));
		}
	}
	else
	{
		pLbl = Label::createWithCharMap("fonts/num_damage.png", 19, 19, '.');
		if (damage > 0.f)
		{
			strContent = __String::createWithFormat(".%d", (int)damage);
		}
		else
		{
			strContent = __String::createWithFormat("/%d", (int)(-damage));
		}
	}

	pLbl->setString(strContent->getCString());

	pLbl->setPosition(pos);
	this->addChild(pLbl, ZInFight_E::Z_F_UI);

	if (damage > 0)
	{
		float dur = 1.5f;
		pLbl->runAction(Sequence::create(MoveBy::create(dur, Vec2(0.f, 200.f)), RemoveSelf::create(false), nullptr));
	}
	else
	{
		float dur = 1.8f;
		float jumpHeight = ToolsUtil::getRandomFloat(230.f, 285.f);
		if (0 == type)
		{
			auto actionjump = JumpBy::create(dur, Vec2(-150.f, -70.f), jumpHeight, 1);
			auto actionfinal = Sequence::createWithTwoActions(actionjump, RemoveSelf::create(false));
			pLbl->runAction(actionfinal);
		}
		else if (type == 1)
		{
			auto actionjump = JumpBy::create(dur, Vec2(150.f, -70.f), jumpHeight, 1);
			auto actionfinal = Sequence::createWithTwoActions(actionjump, RemoveSelf::create(false));
			pLbl->runAction(actionfinal);
		}
	}	
}

/*
void FightLayer::notifyMonsterFight()
{
	if (!_arrCurMonster.empty())
	{
		auto pMonster = _arrCurMonster.at(0);
		pMonster->onFightEvent();
	}
}*/


bool FightLayer::isFirstWave()
{
	return ((0 == _curFloor) && (0 == _curWaveIndex) && (1 != GameLayer::getInstance()->getFightType())) || (1 == GameLayer::getInstance()->getFightType() && 0 == _endlessFloor);
}


bool FightLayer::isLastWave()
{
	return (1 != GameLayer::getInstance()->getFightType()) && ((_arrMonsterWaveInfo->size()-1) == _curFloor) && ((_arrMonsterWaveInfo->at(_curFloor).size()-1) == _curWaveIndex);
}


int FightLayer::getPlayerPosIndex()
{
	int ret = 0;

	for (int i = 0; i < _curFloor && i < _arrMonsterWaveInfo->size(); i++)
	{
		auto pFloorWave = _arrMonsterWaveInfo->at(i);
		ret += pFloorWave.size();
	}

	ret += (_curPlayerPosId+1);
	return ret;
}


int FightLayer::getTotalPosCount()
{
	int ret = 0;

	for (int i = 0; i < _arrMonsterWaveInfo->size(); i++)
	{
		auto pFloorWave = _arrMonsterWaveInfo->at(i);
		ret += pFloorWave.size();
	}

	return ret;
}


void FightLayer::showFightTips(int tipType, int value, const Vec2 &monsterPos)	//0.exp	1.crit 2.def
{
	float dur = 1.f;
	if (0 == tipType)
	{
		auto pNode = Node::create();
		auto pExp = Sprite::createWithSpriteFrameName("tip_exp.png");
		auto pLbl = Label::createWithCharMap("fonts/font_boxlayer_partnum_1.png", 24, 30, '.');
		auto strCtn = __String::createWithFormat("/%d", value);
		pLbl->setString(strCtn->getCString());

		pExp->setPosition(Vec2(-25.f, 0.f));
		pLbl->setPosition(Vec2(30.f, 0.f));
		pNode->addChild(pExp);
		pNode->addChild(pLbl);
		pNode->setPosition(monsterPos+Vec2(0, 40.f));

		this->addChild(pNode, ZInFight_E::Z_F_UI);
		pNode->runAction(Sequence::create(MoveBy::create(dur, Vec2(0.f, 100.f)), RemoveSelf::create(false), nullptr));
	}
	else if (1 == tipType)
	{
		if (value < 3)
		{
			auto strFile = __String::createWithFormat("crit_%d.png", value);
			auto pTips = Sprite::createWithSpriteFrameName(strFile->getCString());

			pTips->setScale(1.5f);
			pTips->setPosition(monsterPos + Vec2(0, 40.f));
			this->addChild(pTips, ZInFight_E::Z_F_UI);

			//		pTips->runAction(Sequence::create(FadeOut::create(dur), RemoveSelf::create(false), nullptr));
			pTips->runAction(Sequence::create(MoveBy::create(dur, Vec2(0.f, 100.f)), RemoveSelf::create(false), nullptr));
		}
	}
	else if (2 == tipType)
	{
		auto strFile = __String::createWithFormat("def_%d.png", value);
		auto pTips = Sprite::createWithSpriteFrameName(strFile->getCString());

		pTips->setScale(1.5f);
		pTips->setPosition(monsterPos + Vec2(0, 40.f));
		this->addChild(pTips, ZInFight_E::Z_F_UI);

//		pTips->runAction(Sequence::create(FadeOut::create(dur), RemoveSelf::create(false), nullptr));
		pTips->runAction(Sequence::create(MoveBy::create(dur, Vec2(0.f, 100.f)), RemoveSelf::create(false), nullptr));
	}
}

void FightLayer::countDungeonDes(int id, int count)
{
	if (3 == GameLayer::getInstance()->getFightType())
	{
		switch (id)
		{
		case -1:
			_curDungeonStep -= count;
			GameLayer::getInstance()->setDungeonStep(_curDungeonStep);
			break;
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			for (int i = 0; i < _pDungeonDeatilInfo->arrDesInfo.size(); i++)
			{
				if (_pDungeonDeatilInfo->arrDesInfo[i].crushId == id)
				{
					_arrDungeonAnim[i] = MIN(_arrDungeonAnim[i] + count, _pDungeonDeatilInfo->arrDesInfo[i].crushCount);

					GameLayer::getInstance()->setDungeonMissionNum(id, _arrDungeonAnim[i], _pDungeonDeatilInfo->arrDesInfo[i].crushCount);

					break;
				}
			}
			break;
		}
	}
}	


int FightLayer::getCurDungeonDesCount(int id)
{
	int ret = -1;

	for (int i = 0; i < _pDungeonDeatilInfo->arrDesInfo.size(); i++)
	{
		if (_pDungeonDeatilInfo->arrDesInfo[i].crushId == id)
		{
			ret = _arrDungeonAnim[i];
			break;
		}
	}

	return ret;
}


int FightLayer::getDungeonDesCount(int id)
{
	int ret = -1;

	for (int i = 0; i < _pDungeonDeatilInfo->arrDesInfo.size(); i++)
	{
		if (_pDungeonDeatilInfo->arrDesInfo[i].crushId == id)
		{
			ret = _pDungeonDeatilInfo->arrDesInfo[i].crushCount;
			break;
		}
	}
	return ret;
}


void FightLayer::startRound(int roundType)
{
	if (3 == GameLayer::getInstance()->getFightType())
	{
		if (0 == roundType)
		{
			auto pRole = Player::getInstance()->getRole(0);
			pRole->startTalk(_pDungeonDeatilInfo->strSaying, 1);
			Player::getInstance()->runAction(Sequence::create(DelayTime::create(10.f), CallFunc::create([=] {pRole->stopTalk(); }), nullptr));
		}

		bool isSuccess = true;
		for (int i = 0; i < _pDungeonDeatilInfo->arrDesInfo.size(); i++)
		{
			if (_arrDungeonAnim[i] < _pDungeonDeatilInfo->arrDesInfo[i].crushCount)
			{
				isSuccess = false;
				break;
			}
		}

		if (isSuccess)
		{
			_gameState = 2;
			return;
		}

		if (_curDungeonStep <= 0)
		{
			Player::getInstance()->doHurtByMonster(0, 999999999.f, 0);
			return;
		}
	}
}


void FightLayer::initDesignInfo()
{
	auto pStageInfo = _pStageInfo;

	switch (GameLayer::getInstance()->getFightType())
	{
	case 0:
		break;
	case 1:
	{
		int maxPos = UserData::getInstance()->getMaxPos();
		pStageInfo = ParamMgr::getInstance()->getStageInfo(maxPos);
	}
		break;
	case 2:
	{
		int sceneId = GameLayer::getInstance()->getSceneId();
		int designSceneId = FightUtil::getDesignEliteSceneId(sceneId);
		pStageInfo = ParamMgr::getInstance()->getStageInfo(designSceneId);
	}
		break;
	case 3:
	{
		int maxPos = UserData::getInstance()->getMaxPos();
		pStageInfo = ParamMgr::getInstance()->getStageInfo(maxPos);
	}
		break;
	}

	int roleLv = pStageInfo->roleLv;

	memset((void *)&_designRoleInfo, 0, sizeof(_designRoleInfo));
	for (int i = 0; i < ParamData::FIGHT_ROLE_COUNT; i++)
	{
		auto roleInfo = ParamMgr::getInstance()->getPlayerInfo(i, roleLv);

		_designRoleInfo.hp += roleInfo.hp;
		_designRoleInfo.dp += roleInfo.dp;

		int weaponIndex = pStageInfo->arrWeaponId[i] + i * 10;
		int weaponId = weaponIndex + 500;
		auto weaponInfo = ParamMgr::getInstance()->getWeaponByID(weaponId);

// 		if (1 == i)
// 		{
// 			_designRoleInfo.att += ((roleInfo.attack + weaponInfo.attack) / 2.f);
// 		}
// 		else
// 		{
// 			_designRoleInfo.att += (roleInfo.attack + weaponInfo.attack);
// 		}
		_designRoleInfo.att += (roleInfo.attack + weaponInfo.attack);
		_designRoleInfo.def += (roleInfo.def + weaponInfo.def);
		_designRoleInfo.dpAdd += (roleInfo.dpadd + weaponInfo.dpadd);
		_designRoleInfo.hpAdd += (roleInfo.hpadd + weaponInfo.hpadd);

		if (pStageInfo->arrWeaponLv[i] > 0)
		{
			auto& weaponUpInfo = (ParamMgr::getInstance()->getWeaponVector())[weaponIndex];

			_designRoleInfo.att += (weaponUpInfo.lvMaxAttack + weaponUpInfo.lvMinAttack) * pStageInfo->arrWeaponLv[i] / 2.f;
			_designRoleInfo.def += (weaponUpInfo.lvMaxDef + weaponUpInfo.lvMinDef) * pStageInfo->arrWeaponLv[i] / 2.f;
			_designRoleInfo.dpAdd += (weaponUpInfo.lvMaxDpadd + weaponUpInfo.lvMinDpadd) * pStageInfo->arrWeaponLv[i] / 2.f;
			_designRoleInfo.hpAdd += (weaponUpInfo.lvMaxHpadd + weaponUpInfo.lvMinHpadd) * pStageInfo->arrWeaponLv[i] / 2.f;
		}
	}

	_designRoleInfo.att /= 3.f;

	_designRoleInfo.perGold = pStageInfo->goldPer;
	_designRoleInfo.perExp = pStageInfo->expPer;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	if (true)
	{
		CCLOG("*********TIME************");
		for (int s = 0; s < 80; s++)
		{
			auto pStageInfoT = ParamMgr::getInstance()->getStageInfo(s);
			auto arrMonsterWaveInfo = pStageInfoT->arrMonsterWaveInfo;

			float totalTime = 0.f;
			for (int i = 0; i < arrMonsterWaveInfo.size(); i++)
			{
				auto arrFloorInfo = arrMonsterWaveInfo.at(i);
				for (int j = 0; j < arrFloorInfo.size(); j++)
				{
					auto arrWaveInfo = arrFloorInfo.at(j);

					for (int k = 0; k < arrWaveInfo.size(); k++)
					{
						int monsterId = arrWaveInfo.at(k);
						auto pMonsterInfo = ParamMgr::getInstance()->getMonsterInfo(monsterId);

						if (0 == pMonsterInfo->designType)
						{
							totalTime += FightUtil::DESIGN_NOR_MONSTER_TIME;
						}
						else if (1 == pMonsterInfo->designType)
						{
							totalTime += FightUtil::DESIGN_ELITE_MONSTER_TIME;
						}
						else if (2 == pMonsterInfo->designType)
						{
							totalTime += FightUtil::DESIGN_BOSS_TIME;
						}
					}
				}
			}

			CCLOG("%f", totalTime);
		}

		CCLOG("********GOLD*************");
		for (int s = 0; s < 80; s++)
		{
			auto pStageInfoT = ParamMgr::getInstance()->getStageInfo(s);


			CCLOG("%d", pStageInfoT->goldPer);
		}

		CCLOG("********EXP*************");
		for (int s = 0; s < 80; s++)
		{
			auto pStageInfoT = ParamMgr::getInstance()->getStageInfo(s);


			CCLOG("%d", pStageInfoT->expPer);
		}
	}

	return;
#endif

}