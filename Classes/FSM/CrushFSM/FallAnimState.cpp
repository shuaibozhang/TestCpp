#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"


FallAnimState *FallAnimState::s_pInstance = nullptr;

FallAnimState * FallAnimState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new FallAnimState();
	}
	return s_pInstance;
}

void FallAnimState::enter(CrushLayer * pOwner)
{
	pOwner->doFall();
}

void FallAnimState::execute(CrushLayer * pOwner)
{
	if (pOwner->getIsFallFinish())
	{
		pOwner->getStateMac()->changeState(CheckCrushState::getInstance());
	}
}

void FallAnimState::exit(CrushLayer * pOwner)
{
}

void FallAnimState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
