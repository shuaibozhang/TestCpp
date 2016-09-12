#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "Player.h"


MonsterWaitState *MonsterWaitState::s_pInstance = nullptr;

MonsterWaitState * MonsterWaitState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new MonsterWaitState();
	}
	return s_pInstance;
}

void MonsterWaitState::enter(CrushLayer * pOwner)
{
//	CrushLayer::getInstance()->getShieldLayer()->setVisible(true);
}

void MonsterWaitState::execute(CrushLayer * pOwner)
{
	if (Player::getInstance()->isAttFinish())
	{
		pOwner->getStateMac()->changeState(MonsterAttState::getInstance());
	}
}

void MonsterWaitState::exit(CrushLayer * pOwner)
{
}

void MonsterWaitState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
