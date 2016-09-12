#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"


CreateAnimState *CreateAnimState::s_pInstance = nullptr;

CreateAnimState * CreateAnimState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new CreateAnimState();
	}
	return s_pInstance;
}

void CreateAnimState::enter(CrushLayer * pOwner)
{
	pOwner->doCreate();
	auto act = Sequence::createWithTwoActions(DelayTime::create(ParamData::ELE_CREATE_WAIT), CallFunc::create([=] {pOwner->getStateMac()->changeState(FallAnimState::getInstance()); }));
	act->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
	pOwner->runAction(act);
}

void CreateAnimState::execute(CrushLayer * pOwner)
{
}

void CreateAnimState::exit(CrushLayer * pOwner)
{
}

void CreateAnimState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
