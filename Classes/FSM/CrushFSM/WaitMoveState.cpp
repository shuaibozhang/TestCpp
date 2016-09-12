#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"


WaitMoveState *WaitMoveState::s_pInstance = nullptr;

WaitMoveState * WaitMoveState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new WaitMoveState();
	}
	return s_pInstance;
}

void WaitMoveState::enter(CrushLayer * pOwner)
{
}

void WaitMoveState::execute(CrushLayer * pOwner)
{
}

void WaitMoveState::exit(CrushLayer * pOwner)
{
}

void WaitMoveState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
	if (0 == CrushMsg::TOUCH_MOVED.compare(event->getEventName()))
	{
		Touch *touch = static_cast<Touch *>(event->getUserData());

		auto touchPos = pOwner->getTouchIndex();
		auto touchEle = pOwner->getEleIcon(touchPos);
		auto movePosIndex = CrushUtil::getCrushIndex(touch->getLocation());

		if (-1 != movePosIndex.row)
		{
			auto moveEle = pOwner->getEleIcon(movePosIndex);
			if (CrushUtil::isEleCanTouch(moveEle))
			{
				if ((touchPos.row == movePosIndex.row && 1 == abs(movePosIndex.column - touchPos.column))
					|| (touchPos.column == movePosIndex.column && 1 == abs(movePosIndex.row - touchPos.row)))
				{
					touchEle->stopSelAnim();
					pOwner->setMoveIndex(movePosIndex);
					pOwner->getStateMac()->changeState(SwapAnimState::getInstance());
				}
			}
			else
			{
				touchEle->stopSelAnim();
				pOwner->setTouchIndex(-1, -1);
				pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
			}
		}
		else
		{
			touchEle->stopSelAnim();
			pOwner->setTouchIndex(-1, -1);
			pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
		}
	}
	else if (0==CrushMsg::TOUCH_ENDED.compare(event->getEventName()) || 0==CrushMsg::TOUCH_CANCELLED.compare(event->getEventName()))
	{
		pOwner->getStateMac()->changeState(WaitTouchState::getInstance());
	}
}
