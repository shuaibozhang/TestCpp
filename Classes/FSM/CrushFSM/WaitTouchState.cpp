#include "CrushFSM.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "GameLayer.h"
#include "GuideLayer.h"
#include "Player.h"
#include "Role.h"


WaitTouchState *WaitTouchState::s_pInstance = nullptr;

WaitTouchState * WaitTouchState::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new WaitTouchState();
		s_pInstance->_isCanSwapCrush = true;
	}
	return s_pInstance;
}

void WaitTouchState::enter(CrushLayer * pOwner)
{
	/*init crush data*/
// 	CrushIndex_T tmp = { 0 };
// 	tmp.row = -1;
// 	tmp.column = -1;
// 	pOwner->setTouchIndex(tmp);

	if (GameLayer::getInstance()->getIsGuiding() && nullptr != GuideLayer::getInstance())
	{
		GuideLayer::getInstance()->onTriggerEvent(1);
	}
	GameLayer::getInstance()->setEquipBtnEnabled(true);

	if (!pOwner->isCanSwapCrush())
	{
		if (_isCanSwapCrush && !GameLayer::getInstance()->getIsGuiding())
		{
			auto pRole = Player::getInstance()->getRole(0);
			pRole->startTalk("guide_tip_6");
			Player::getInstance()->runAction(Sequence::create(DelayTime::create(4.f), CallFunc::create([=] {pRole->stopTalk(); }), nullptr));
		}
		_isCanSwapCrush = false;
	}
	else
	{
		_isCanSwapCrush = true;
	}
}

void WaitTouchState::execute(CrushLayer * pOwner)
{
}

void WaitTouchState::exit(CrushLayer * pOwner)
{
}

void WaitTouchState::handleMessage(CrushLayer *pOwner, EventCustom * event)
{
	if (0 == CrushMsg::TOUCH_BEGAN.compare(event->getEventName()))
	{
		Touch *touch = static_cast<Touch *>(event->getUserData());

		auto touchPosIndex = pOwner->getTouchIndex();
		auto touchEle = pOwner->getEleIcon(touchPosIndex);
		auto newTouchIndex = CrushUtil::getCrushIndex(touch->getLocation());
		
		if (-1 != newTouchIndex.row)
		{
			auto newTouchEle = pOwner->getEleIcon(newTouchIndex);
			if (CrushUtil::isEleCanTouch(newTouchEle))
			{
				if (-1 != touchPosIndex.row)
				{
					touchEle->stopSelAnim();
					if ((newTouchIndex.row == touchPosIndex.row && 1 == abs(newTouchIndex.column - touchPosIndex.column))
						|| (newTouchIndex.column == touchPosIndex.column && 1 == abs(newTouchIndex.row - touchPosIndex.row)))
					{
						pOwner->setMoveIndex(newTouchIndex);
						pOwner->getStateMac()->changeState(SwapAnimState::getInstance());
					}
					else
					{
//						pOwner->setTouchIndex(-1, -1);
						pOwner->setTouchIndex(newTouchIndex);
						newTouchEle->playSelAnim();
						pOwner->getStateMac()->changeState(WaitMoveState::getInstance());
					}
				}
				else
				{
					pOwner->setTouchIndex(newTouchIndex);
					newTouchEle->playSelAnim();
					pOwner->getStateMac()->changeState(WaitMoveState::getInstance());
				}
			}
			else
			{
				if (-1 != touchPosIndex.row)
				{
					touchEle->stopSelAnim();
				}
				pOwner->setTouchIndex(-1, -1);
			}
		}
		else
		{
			if (-1 != touchPosIndex.row)
			{
				touchEle->stopSelAnim();
			}
			pOwner->setTouchIndex(-1, -1);
		}
	}
}
