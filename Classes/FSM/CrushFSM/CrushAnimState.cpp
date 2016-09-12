#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"


CrushAnimState *CrushAnimState::s_pInstance = nullptr;

CrushAnimState * CrushAnimState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new CrushAnimState();
	}
	return s_pInstance;
}

void CrushAnimState::enter(CrushLayer * pOwner)
{
//	float arrDur[3] = {0.5f, 0.5f, 0.5f};
	float dur = 0.f;

	if (pOwner->doBottomCrush())
	{
//		dur += 0.5f;// arrDur[0];
		dur = 0.1f;// ParamData::ELE_CRUSH_WAIT;
	}

	if (pOwner->doCleanCrush())
	{
//		dur += 0.5f;// arrDur[1];
		dur = 0.1f;//ParamData::ELE_CRUSH_WAIT;
	}

	auto act = Sequence::create(DelayTime::create(dur),
		CallFunc::create([=] {
		if (pOwner->doCrush())
		{
			auto actEx = Sequence::createWithTwoActions(DelayTime::create(dur + ParamData::ELE_CRUSH_WAIT), CallFunc::create([=] { pOwner->getStateMac()->changeState(CreateAnimState::getInstance()); }));
			actEx->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
			pOwner->runAction(actEx);
		}
		else
		{
			pOwner->getStateMac()->changeState(CreateAnimState::getInstance());
		}
	}), nullptr);
	act->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
	pOwner->runAction(act);
}

void CrushAnimState::execute(CrushLayer * pOwner)
{
}

void CrushAnimState::exit(CrushLayer * pOwner)
{
}

void CrushAnimState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
