#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "GameLayer.h"
#include "GuideLayer.h"
#include "FightLayer.h"
#include "Player.h"
#include "Role.h"


CheckCrushState *CheckCrushState::s_pInstance = nullptr;

CheckCrushState * CheckCrushState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new CheckCrushState();
	}
	return s_pInstance;
}

void CheckCrushState::enter(CrushLayer * pOwner)
{
	float dur = ParamData::ELE_SWAP_DUR;
	auto touchPosIndex = pOwner->getTouchIndex();

	if (-1 == touchPosIndex.row)
	{
		if (pOwner->checkCrushTravelse())
		{
//			pOwner->setTouchIndex(-1, -1);
//			pOwner->setMoveIndex(-1, -1);
			pOwner->getStateMac()->changeState(CrushAnimState::getInstance());
		}
		else
		{
			pOwner->getStateMac()->changeState(MonsterWaitState::getInstance());
//			pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
		}
	}
	else
	{
		bool isCanCrush = pOwner->checkCrushTravelse();
		pOwner->setTouchIndex(-1, -1);
		pOwner->setMoveIndex(-1, -1);
		if (isCanCrush)
		{
			if (3 == GameLayer::getInstance()->getFightType())
			{
				auto pRole = Player::getInstance()->getRole(0);
				pRole->stopTalk();
			}
			FightLayer::getInstance()->countDungeonDes(-1, 1);
			pOwner->initRoundData();
			pOwner->getStateMac()->changeState(CrushAnimState::getInstance());
			if (GameLayer::getInstance()->getIsGuiding() && nullptr != GuideLayer::getInstance())
			{
				GuideLayer::getInstance()->onFinishEvent(1);
			}

			if (1 == GameLayer::getInstance()->getFightType())
			{
				GameLayer::getInstance()->endBarCountdown();
			}
		}
		else
		{
			pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
		}
	}
}

void CheckCrushState::execute(CrushLayer * pOwner)
{
}

void CheckCrushState::exit(CrushLayer * pOwner)
{
}

void CheckCrushState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
