#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "FightLayer.h"
#include "Monster.h"
#include "Player.h"
#include "GameLayer.h"
#include "GameLayer.h"
#include "GuideLayer.h"
#include "Defines.h"


MonsterAttState *MonsterAttState::s_pInstance = nullptr;

MonsterAttState * MonsterAttState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new MonsterAttState();
	}
	return s_pInstance;
}

void MonsterAttState::enter(CrushLayer * pOwner)
{
	if (GameLayer::getInstance()->getIsGuiding() && nullptr != GuideLayer::getInstance())
	{
		GuideLayer::getInstance()->onTriggerEvent(2);
	}

	bool isNeedFall = false;
	EleIcon *(*arrEle)[ParamData::CRUSH_COL] = pOwner->getEleIconArr();
	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			auto pEle = arrEle[i][j];

			if (nullptr != pEle && pEle->getIsActive())
			{
				if (pEle->doRound())
				{
					arrEle[i][j] = nullptr;
					isNeedFall = true;
				}
			}
		}
	}

	if (isNeedFall)
	{
		pOwner->setCurCrushArrCount(0);
		pOwner->doCreate();
		pOwner->doFall();
	}

	bool isNeedDef = false;
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	for (int i = 0; i < arrMonster->size(); i++)
	{
		auto pMonster = arrMonster->at(i);
		if (pMonster->getIsActive())
		{
			if (pMonster->doRound())
			{
				isNeedDef = true;
			}
		}
	}

	if (isNeedDef)
	{
		CrushLayer::getInstance()->getShieldLayer()->setVisible(true);
		Player::getInstance()->doStartDef();
	}

	GameLayer::getInstance()->resetCombo();
}

void MonsterAttState::execute(CrushLayer * pOwner)
{
	if (pOwner->getIsFallFinish())
	{
		bool isAllFinish = true;
		auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
		for (int i = 0; i < arrMonster->size(); i++)
		{
			auto pMonster = arrMonster->at(i);
			if (!pMonster->getIsAttFinish())
			{
				isAllFinish = false;
				break;
			}
		}

		if (isAllFinish)
		{
			if (GameLayer::getInstance()->getIsGuiding() && nullptr != GuideLayer::getInstance())
			{
				GuideLayer::getInstance()->onFinishEvent(2);
			}
#if (1 == CC_ENABLE_DEBUG_MONSTER_ANIM)
			pOwner->getStateMac()->changeState(MonsterAttState::getInstance());
			return;
#endif
			pOwner->getStateMac()->changeState(BuffRoundState::getInstance());
		}
	}
}

void MonsterAttState::exit(CrushLayer * pOwner)
{
}

void MonsterAttState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
