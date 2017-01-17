#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "FightLayer.h"
#include "Monster.h"
#include "Player.h"
#include "GameLayer.h"
#include "Scenes/GameMap.h"
#include "Story/StoryMgr.h"
#include "platBridge/cocos2dx_analyze.h"
#include "AchieveMgr.h"
#include "MagPieMgr.h"
#include "ParamMgr.h"
#include "UserData.h"
#include "Scenes/PopRewardLayer.h"
#include "Scenes/MainScene.h"
#include "Defines.h"
#include "FightUtil.h"


SettleState *SettleState::s_pInstance = nullptr;

SettleState * SettleState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new SettleState();
	}
	return s_pInstance;
}

void SettleState::enter(CrushLayer * pOwner)
{
	pOwner->stopAllActionsByTag(CrushMsg::CHANGE_STATE_ACT_TAG);

	int gameState = FightLayer::getInstance()->getGameState();
	bool isWin = (2 == gameState) ? true : false;

	auto strLv = String::createWithFormat("stage_%d", GameLayer::getInstance()->getSceneId());

	if (2 == GameLayer::getInstance()->getFightType())
	{
		strLv = String::createWithFormat("stage_%d_%d", 2, GameLayer::getInstance()->getSceneId());
	}
	else if (3 == GameLayer::getInstance()->getFightType())
	{
		strLv = String::createWithFormat("stage_%d_%d", 3, GameMap::getCurGameMap()->getDungeonIdx());
	}

	if (isWin)
	{
		cocos2dx_analyze::finishLevel(strLv->getCString());
		if (1 != GameLayer::getInstance()->getFightType() && 3 != GameLayer::getInstance()->getFightType())
		{
			AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::STAGE, 1, false);
			MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::STAGE, 1);
		}		
	}
	else
	{
		cocos2dx_analyze::failLevel(strLv->getCString());
	}

	if (!WordMap::getInstance()->checkIsBoss(GameLayer::getInstance()->getSceneId()))
	{
		if (isWin)
		{
			ParamMgr::getInstance()->changeShowReliveWeight(1);
		}
		else
		{
			ParamMgr::getInstance()->changeShowReliveWeight(0);
		}
	}

	auto arrCount = CrushLayer::getInstance()->getCrushEleCount();
	int totalExp = FightLayer::getInstance()->getTotalExp();

	if (isWin && 3 == GameLayer::getInstance()->getFightType())
	{
		int dungeonType = GameMap::getCurGameMap()->getDungeonIdx();
		std::vector<PopItemInfo_T> arrReward;
#if (1 == CC_ENABLE_NEW_PARAM)
		int exp = FightUtil::getDungeonReward(dungeonType, arrReward, FightLayer::getInstance()->getDesignRoleInfo());
#else
		int exp = CrushUtil::getDungeonReward(dungeonType, UserData::getInstance()->getPlayerCurLv(0), arrReward);
#endif
		
		if (0 == dungeonType)
		{
			auto pLayer = PopRewardLayer::create(arrReward, 0);
			MainLayer::getCurMainLayer()->addChild(pLayer, POP_REWARD_Z);
		}
		else if (1 == dungeonType)
		{
			totalExp += exp;
		}
		else if (2 == dungeonType)
		{
			auto pLayer = PopRewardLayer::create(arrReward, 0);
			MainLayer::getCurMainLayer()->addChild(pLayer, POP_REWARD_Z);
		}
	}

	CrushLayer::getInstance()->setVisible(false);
	FightLayer::getInstance()->stopFight();
#if 0

	Player::getInstance()->moveToSettlePos(isWin);
	pOwner->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		auto arrCount = CrushLayer::getInstance()->getCrushEleCount();
		float arrPer[4] = { 0 };

		for (int i = 0; i < ParamData::FIGHT_ROLE_COUNT; i++)
		{
			arrPer[i] = arrCount[i] * 1.f / (arrCount[0]+arrCount[1]+arrCount[2]+arrCount[3]);
		}

		GameMap::getCurGameMap()->showResault(isWin, totalExp, 0, arrPer);
		GameLayer::getInstance()->stopFight(); }), nullptr));
#else	
	pOwner->runAction(Sequence::create(DelayTime::create(1.f),
		CallFunc::create([=] {Player::getInstance()->moveToSettlePos(isWin); }),
		DelayTime::create(0.5f), CallFunc::create([=] {
		auto arrCount = CrushLayer::getInstance()->getCrushEleCount();
		float arrPer[4] = { 0 };

		for (int i = 0; i < ParamData::FIGHT_ROLE_COUNT; i++)
		{
			arrPer[i] = arrCount[i] * 1.f / (arrCount[0] + arrCount[1] + arrCount[2] + arrCount[3]);
		}

		GameMap::getCurGameMap()->showResault(isWin, totalExp, FightLayer::getInstance()->getTotalGold(), arrPer);
		GameLayer::getInstance()->stopFight(); }), nullptr));

#endif
}

void SettleState::execute(CrushLayer * pOwner)
{
}

void SettleState::exit(CrushLayer * pOwner)
{
}

void SettleState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
