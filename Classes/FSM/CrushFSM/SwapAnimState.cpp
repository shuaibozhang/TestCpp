#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "GameLayer.h"


SwapAnimState *SwapAnimState::s_pInstance = nullptr;

SwapAnimState * SwapAnimState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new SwapAnimState();
	}
	return s_pInstance;
}

void SwapAnimState::enter(CrushLayer * pOwner)
{
	float dur = ParamData::ELE_SWAP_DUR;
	EleIcon *(*arrEle)[ParamData::CRUSH_COL] = pOwner->getEleIconArr();

	auto touchPosIndex = pOwner->getTouchIndex();
	auto movePosIndex = pOwner->getMoveIndex();

	auto touchEle = arrEle[touchPosIndex.row][touchPosIndex.column];
	auto moveEle = arrEle[movePosIndex.row][movePosIndex.column];

	GameLayer::getInstance()->setEquipBtnEnabled(false);

	if (pOwner->isMoveCanCrush())
	{
		touchEle->playMovePerUnit(-1, 0.f, dur, movePosIndex.row, movePosIndex.column);
		moveEle->playMovePerUnit(-1, 0.f, dur, touchPosIndex.row, touchPosIndex.column);

//		pOwner->setTouchIndex(movePosIndex);
//		pOwner->setMoveIndex(touchPosIndex);

		CrushUtil::swapEle(touchEle, moveEle, arrEle);

		auto act = Sequence::create(DelayTime::create(dur), CallFunc::create([=] {pOwner->getStateMac()->changeState(CheckCrushState::getInstance()); }), nullptr);
		act->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
		pOwner->runAction(act);
	}
	else
	{
		touchEle->playMovePerUnit(-1, 0.f, dur, movePosIndex.row, movePosIndex.column);
		touchEle->playMovePerUnit(-1, dur, dur, touchPosIndex.row, touchPosIndex.column);

		moveEle->playMovePerUnit(-1, 0.f, dur, touchPosIndex.row, touchPosIndex.column);
		moveEle->playMovePerUnit(-1, dur, dur, movePosIndex.row, movePosIndex.column);

//		pOwner->setTouchIndex(-1, -1);
//		pOwner->setMoveIndex(-1, -1);

		auto act = Sequence::createWithTwoActions(DelayTime::create(2 * dur), CallFunc::create([=] {pOwner->getStateMac()->changeState(CheckCrushState::getInstance()); }));
		act->setTag(CrushMsg::CHANGE_STATE_ACT_TAG);
		pOwner->runAction(act);
	}
}

void SwapAnimState::execute(CrushLayer * pOwner)
{
}

void SwapAnimState::exit(CrushLayer * pOwner)
{
}

void SwapAnimState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
}
