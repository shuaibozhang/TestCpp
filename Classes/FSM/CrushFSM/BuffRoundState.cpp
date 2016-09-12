#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "FightLayer.h"
#include "Monster.h"
#include "Player.h"
#include "GameLayer.h"


BuffRoundState *BuffRoundState::s_pInstance = nullptr;

BuffRoundState * BuffRoundState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new BuffRoundState();
	}
	return s_pInstance;
}

void BuffRoundState::enter(CrushLayer * pOwner)
{
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	for (int i = arrMonster->size()-1; i >= 0; i--)
	{
		auto pMonster = arrMonster->at(i);
		if (pMonster->getIsActive())
		{
			pMonster->doWeakPer();
		}
	}

	Player::getInstance()->doEndDef();

	Player::getInstance()->doRecover();

	auto act = Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] { pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
	if (FightState::Fight == FightLayer::getInstance()->getCurFightState()) 
	{
		CrushLayer::getInstance()->getShieldLayer()->setVisible(false);
		FightLayer::getInstance()->startRound(1);
		//start round count down
		if (1 == GameLayer::getInstance()->getFightType())
		{
			//start round count down
			GameLayer::getInstance()->startBarCountdown();
		}
	}}), nullptr);
	act->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
	pOwner->runAction(act);
}

void BuffRoundState::execute(CrushLayer * pOwner)
{
}

void BuffRoundState::exit(CrushLayer * pOwner)
{
}

void BuffRoundState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
