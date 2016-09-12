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


ItemRoundState *ItemRoundState::s_pInstance = nullptr;

ItemRoundState * ItemRoundState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new ItemRoundState();
	}
	return s_pInstance;
}

void ItemRoundState::enter(CrushLayer * pOwner)
{
	if (pOwner->isCanFall())
	{
		pOwner->setCurCrushArrCount(0);
		pOwner->doCreate();
		pOwner->doFall();
	}
}

void ItemRoundState::execute(CrushLayer * pOwner)
{
	if (pOwner->getIsFallFinish())
	{
		pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
	}
}

void ItemRoundState::exit(CrushLayer * pOwner)
{
}

void ItemRoundState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
