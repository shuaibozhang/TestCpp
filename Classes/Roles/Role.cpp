#include "Role.h"
#include "FightLayer.h"
#include "Armtr/GameArmtr.h"
#include "ParamMgr.h"
#include "Monster.h"
#include "Player.h"
#include "SkillControl.h"
#include "CrushUtil.h"
#include "RoleDialog.h"
#include "GameResPool.h"
#include "WeaponControl.h"
#include "GameLayer.h"


Role::Role()
	:_isCurAttFinish(true),
	_pRoleDialog(nullptr),
	_pBuffAtt(nullptr)
{
	memset((void *)&_curAttInfo, 0, sizeof(_curAttInfo));
}

Role::~Role()
{
}
/*
Role * Role::create(int id)
{
	Role *pRet = new Role();

	if (nullptr != pRet && pRet->init(id))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}*/

bool Role::init(int id)
{
	bool bRet = false;

	do 
	{
		_roleId = id;
		_rolePosIndex = Player::getInstance()->getPosIndexByRoleId(_roleId);
		_pRoleData = Player::getInstance()->getRoleInfo(_roleId);

		auto pArmInfo = ParamMgr::getInstance()->getRoleArmtrInfo(_roleId);
		_pArmtr = GameArmtr::createRole(pArmInfo);
		this->addChild(_pArmtr);

//		_pArmtr->getArmtr()->getCCEventDispatcher()->addCustomEventListener(EventData::COMPLETE, CC_CALLBACK_1(Role::eventHandler, this));
		_pArmtr->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationEvent, this));
		_pArmtr->getArmtr()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Role::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

		_norAttInfo.id = -1;
		_norAttInfo.attribute = AttAttrbt_E::ATT_NORMAL;
		_norAttInfo.mark = -1;
		_norAttInfo.isHurtAll = false;
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_ATT;

//		this->scheduleUpdate();
		bRet = true;
	} while (false);

	return bRet;
}

void Role::update(float delta)
{
	if (FightLayer::getInstance()->getCurFightState() == FightState::Fight && 1 == FightLayer::getInstance()->getGameState())
	{
		if (this->isCanAtt())
		{
//			CrushArr_T info = _attInfoQueue.front();
//			_attInfoQueue.pop();
			bool isSkip = false;
			auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
			if (!arrMonster->empty())
			{
				auto pMonster = arrMonster->at(0);
				if (2 == pMonster->getMonsterInfo()->type && !pMonster->getIsBoxChange())
				{
					isSkip = true;
				}				
			}

			if (3 == GameLayer::getInstance()->getFightType())
			{
				isSkip = true;
			}

			if (isSkip)
			{
				if (!_attInfoQueue.empty())
				{
					_attInfoQueue.pop();
				}
			}
			else
			{
				_isCurAttFinish = false;
				this->initAttData();
				this->startAttAnim();
			}
		}
	}
}

void Role::initFightData()
{
	_pRoleData = Player::getInstance()->getRoleInfo(_roleId);

	while (!_attInfoQueue.empty())
	{
		_attInfoQueue.pop();
	}

	_isCurAttFinish = true;
}

void Role::addAttInfo(CrushArr_T * pCrushInfo)
{
	_attInfoQueue.push(*pCrushInfo);
}

void Role::startAttAnim()
{
	std::string roleAnim = _curAttInfo.pSkillInfo->roleAnim;
// 	if (nullptr == _curAttInfo.pSkillInfo)
// 	{
// 		roleAnim = ArmtrName::ROLE_NOR_ATT;
// 	}
// 	else
// 	{
// 		roleAnim = _curAttInfo.pSkillInfo->roleAnim;
// 	}

	if (_curAttInfo.pSkillInfo->isNeedMove && 0 == _curAttInfo.curAttCount)
	{
		Vec2 desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());

		if (_curAttInfo.pSkillInfo->isHurtAll)
		{
			desPos -= Vec2(90.f, 0.f);
		}
		else
		{
			if (nullptr != _curAttInfo.pDesMonster)
			{
				desPos = _curAttInfo.pDesMonster->getPosition() - Vec2(80.f, 0.f);
			}
		}

		this->playMove();
		this->runAction(Sequence::create(MoveTo::create(0.3f, desPos), CallFunc::create([=] {_pArmtr->play(roleAnim); }), nullptr));
	}
	else
	{
		_pArmtr->play(roleAnim);
	}

	if (CrushUtil::isNeedSkillBg(_curAttInfo.skillId))
	{
		GameLayer::getInstance()->playSkillBgAnim(0);
	}
}

void Role::endAttAnim()
{
	if (_isCurAttFinish)
	{
		return;
	}
	_curAttInfo.curAttCount++;
	if (_curAttInfo.curAttCount == _curAttInfo.attCount)
	{
		//		_curAttInfo.pSkillInfo
		if (CrushUtil::isNeedSkillBg(_curAttInfo.skillId))
		{
			GameLayer::getInstance()->playSkillBgAnim(1);
		}

		if (_curAttInfo.pSkillInfo->isNeedMove)
		{
			auto pos = CrushUtil::getRolePos(_roleId, FightLayer::getInstance()->getCurPlayerPosId());
			this->playMove();
			this->runAction(Sequence::create(MoveTo::create(0.3f, pos), CallFunc::create([=] {
				if (!_attInfoQueue.empty())
				{
					_attInfoQueue.pop();
				}
			_isCurAttFinish = true;
			this->playIdle();
			CC_SAFE_RELEASE(_curAttInfo.pDesMonster); }), nullptr));
		}
		else
		{
			if (!_attInfoQueue.empty())
			{
				_attInfoQueue.pop();
			}
			_isCurAttFinish = true;
			CC_SAFE_RELEASE(_curAttInfo.pDesMonster);
			this->playIdle();
		}
	}
	else
	{
		this->startAttAnim();
	}
}

void Role::playSkillAnim()
{
	if (_isCurAttFinish)
	{
		return;
	}
	if (_curAttInfo.pSkillInfo->skillAnim != "")
	{
		auto pSkill = GameArmtr::createSkill(_curAttInfo.pSkillInfo->skillAnim);
		FightLayer::getInstance()->addChild(pSkill);
		pSkill->setPosition(_curAttInfo.desPos);
//		pSkill->play(ArmtrName::SKILL_NOR_ATT);
		pSkill->play(_curAttInfo.pSkillInfo->skillAnim);
		
		pSkill->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationSkillEvent, this));
		pSkill->getArmtr()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Role::onSkillFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}
}

bool Role::isCanAtt()
{
	return !_attInfoQueue.empty() && _isCurAttFinish && !FightLayer::getInstance()->getCurArrMonster()->empty();
}

void Role::throwDart()
{
	if (_isCurAttFinish)
	{
		return;
	}
	if (RoleId_E::ROLE_ID_LQC == _roleId)
	{
		Vec2 startPos = this->getPosition() + Vec2(20.f, 140.f);//this->convertToWorldSpaceAR(Vec2(20.f, 140.f));

		ccBezierConfig config;
		config.endPosition = _curAttInfo.desPos;

		float configPosX = startPos.x + (config.endPosition.x - startPos.x) * 0.7f;////startPos.x + (config.endPosition.x - startPos.x) * 1 / 4.f;
		float configPosY = startPos.y + 50.f;
		config.controlPoint_1 = Vec2(configPosX, configPosY);
		config.controlPoint_2 = Vec2(configPosX, configPosY);

//		std::string dartName = "feibiao";
//		dartName.append(_curAttInfo.pSkillInfo->roleAnim.substr(7, 1));
		auto strDartName = __String::createWithFormat("feibiao%d", _curAttInfo.pSkillInfo->attribute);
		auto pDart = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_FB));//GameArmtr::createDart();
		FightLayer::getInstance()->addChild(pDart);

		if (AttAttrbt_E::ATT_NORMAL == _curAttInfo.pSkillInfo->attribute)
		{
			auto weaponInfo = WeaponControl::getInstance()->getEquipWenpon(2);
			int weaponIndex = weaponInfo.id - 520;
			auto strName = String::createWithFormat("wuqi/lqc_wuqi_%d.png", weaponIndex + 1);

			Skin *skin = Skin::createWithSpriteFrameName(strName->getCString());
			pDart->getArmtr()->getBone("Layer4")->addDisplay(skin, 0);
			pDart->getArmtr()->getBone("Layer4")->changeDisplayWithIndex(0, true);
		}
		pDart->play(strDartName->getCString());

//		auto act = EaseExponentialIn::create(Sequence::create(CCBezierTo::create(0.8f, config), CallFunc::create([=] {this->endThrowDart(); }), RemoveSelf::create(), nullptr));
		auto act = Sequence::create(EaseSineIn::create(CCBezierTo::create(0.6f, config)), CallFunc::create([=] {this->endThrowDart(); GameResPool::getInstance()->recycleRes(GameResId_E::RES_FB, pDart); }), RemoveSelf::create(), nullptr);
		pDart->setPosition(startPos);
		pDart->runAction(act);
	}
}

void Role::endThrowDart()
{
	if (_isCurAttFinish)
	{
		return;
	}
	if (RoleId_E::ROLE_ID_LQC == _roleId)
	{
		if (_curAttInfo.pSkillInfo->skillAnim == "")
		{
			auto pAnim = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_HIT));//GameArmtr::createHit("hit3");
			pAnim->play("hit3");
			pAnim->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationStateEvent, this));
			FightLayer::getInstance()->addChild(pAnim);
			pAnim->setPosition(_curAttInfo.desPos);

			this->doAttPer();
			this->endAttAnim();
		}
		else
		{
			this->playSkillAnim();
		}
	}
}

Monster * Role::getTargetMonster()
{
	Monster *pRet = nullptr;
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();

	for (int i = 0; i < arrMonster->size(); i++)
	{
		Monster *pMonster = arrMonster->at(i);
		if (!pMonster->getIsDead() || i == (arrMonster->size() - 1))
		{
			pRet = pMonster;
			break;
		}
	}

	return pRet;
}

void Role::playMove()
{
	_pArmtr->play(ArmtrName::ROLE_WALK);
}

void Role::playIdle()
{
	_pArmtr->play(ArmtrName::ROLE_IDLE);
}

void Role::playHurt()
{
	_pArmtr->play("hurt");
}

void Role::playStateAnim(const std::string& animName)
{
	auto pAnim = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_BUFF));//GameArmtr::createBuff(animName);
	this->addChild(pAnim);
	pAnim->play(animName);

	pAnim->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationStateEvent, this));
}

void Role::playHitAnim(const std::string& animName)
{
	auto pAnim = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_HIT));//GameArmtr::createBuff(animName);
	this->addChild(pAnim);
	pAnim->play(animName);

	pAnim->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationStateEvent, this));
}

void Role::playAnim(const std::string & animName)
{
	_pArmtr->play(animName);
}

void Role::playEndDefAnim()
{
	if (RoleId_E::ROLE_ID_CBD == _roleId || RoleId_E::ROLE_ID_QYL == _roleId)
	{
		auto pos = CrushUtil::getRolePos(_roleId, FightLayer::getInstance()->getCurPlayerPosId());
		float moveDur = 0.2f;
		float animDur = 0.5f;
		this->playMove();
		this->runAction(Sequence::create(
			MoveTo::create(moveDur, pos),
			CallFunc::create([=] {this->playIdle(); }),
			nullptr));
	}
}

void Role::playDefAnim()
{
	_pArmtr->play("def");
}

void Role::playCheer()
{
	_pArmtr->play(ArmtrName::ROLE_CHEER);
}

void Role::startTalk(const std::string & talkId, int contentType)
{
	if (nullptr == _pRoleDialog)
	{
		_pRoleDialog = RoleDialog::create(talkId, _rolePosIndex, contentType);
		FightLayer::getInstance()->addChild(_pRoleDialog);
	}
}

void Role::stopTalk()
{
	if (nullptr != _pRoleDialog)
	{
		_pRoleDialog->removeFromParent();
		_pRoleDialog = nullptr;
	}
}

void Role::playBuffAttAnim()
{
	if (nullptr == _pBuffAtt)
	{
		_pBuffAtt = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_BUFF));//GameArmtr::createBuff(animName);
		this->addChild(_pBuffAtt);
		_pBuffAtt->play(ArmtrName::ROLE_STATE_ADD_ATT);
	}
}

void Role::stopBuffAttAnim()
{
	if (nullptr != _pBuffAtt)
	{
		GameResPool::getInstance()->recycleRes(_pBuffAtt);
		_pBuffAtt->removeFromParentAndCleanup(false);
		_pBuffAtt = nullptr;
	}
}


void Role::playStartDefAnim()
{
	if (RoleId_E::ROLE_ID_CBD == _roleId || RoleId_E::ROLE_ID_QYL == _roleId)
	{
		auto desPos = CrushUtil::getRoleDefPos(FightLayer::getInstance()->getCurPlayerPosId());//CrushUtil::getRoleFrontPos(FightLayer::getInstance()->getCurPlayerPosId());
		auto pos = CrushUtil::getRolePos(_roleId, FightLayer::getInstance()->getCurPlayerPosId());
		float moveDur = 0.2f;
		float animDur = 0.5f;
		this->playMove();
		this->runAction(Sequence::create(MoveTo::create(moveDur, desPos), 
			CallFunc::create([=] {this->playIdle(); }), 
			nullptr));
	}
}

bool Role::isAttFinish()
{
	return _attInfoQueue.empty();
}

void Role::animationEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		if (movementID == ArmtrName::ROLE_CHEER)
		{
			this->playIdle();
			return;
		}

		string subMoveId = movementID.substr(0, 6);
		if (movementID == ArmtrName::ROLE_NOR_ATT && RoleId_E::ROLE_ID_LQC != _roleId)
		{
			this->endAttAnim();
// 			_attInfoQueue.pop();
// 			_isCurAttFinish = true;
		}
		else if (RoleId_E::ROLE_ID_LSJ == _roleId)
		{
//			if (movementID == "attack02" || movementID == "attack03" || movementID == "attack05")
			if (movementID == "attack02")
			{
				this->endAttAnim();
			}
			else if (subMoveId == "attack")
			{
				this->playIdle();
			}
		}
		else if (RoleId_E::ROLE_ID_CBD == _roleId)
		{
//			if (movementID == "attack07" || movementID == "attack08" || movementID == "attack02")
			if (movementID == "attack08" || movementID == "attack02" || movementID == "attack04")
			{
				this->endAttAnim();
			}
			else if (subMoveId == "attack")
			{
				this->playIdle();
			}
			else if (movementID == "def")
			{
				this->playIdle();
			}
		}
		else if (RoleId_E::ROLE_ID_LQC == _roleId)
		{
			if (subMoveId == "attack")
			{
				this->playIdle();
			}
		}
		else if (RoleId_E::ROLE_ID_SQY == _roleId)
		{
			if (movementID == "attack01")
			{
				this->endAttAnim();
			}
			else if (movementID == "attack02" || movementID == "attack03" || movementID == "attack04")
			{
				this->endAttAnim();
			}
			else if (subMoveId == "attack")
			{
				this->playIdle();
			}
		}
		else if (RoleId_E::ROLE_ID_QYL == _roleId)
		{
			if (movementID == "attack03")
			{
				this->endAttAnim();
			}
			else if (subMoveId == "attack")
			{
				this->playIdle();
			}
			else if (movementID == "def")
			{
				this->playIdle();
			}
		}
	}
}

void Role::onFrameEvent(cocostudio::Bone * bone, const std::string & evt, int originFrameIndex, int currentFrameIndex)
{
//	CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	if (evt == ArmtrName::ROLE_EVT_HURT)
	{
//		this->playSkillAnim();
		if (RoleId_E::ROLE_ID_LQC == _roleId)
		{
			this->throwDart();
		}
		else
		{
			this->doAttPer();
		}
	}
	else if (evt == ArmtrName::ROLE_EVT_SKILL)
	{
		if (RoleId_E::ROLE_ID_LQC == _roleId)
		{
			this->throwDart();
		}
		else
		{
			this->playSkillAnim();
		}
	}
	else if (evt == ArmtrName::ROLE_EVT_SKILL_HURT)
	{
		this->doAttPer();
	}
}
void Role::animationSkillEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
//		if (movementID == ArmtrName::SKILL_NOR_ATT)
		if (movementID == _curAttInfo.pSkillInfo->skillAnim)
		{
			this->endAttAnim();
//			armature->removeFromParentAndCleanup(false);
			armature->getParent()->removeFromParentAndCleanup(false);
		}
	}
}
void Role::onSkillFrameEvent(cocostudio::Bone * bone, const std::string & evt, int originFrameIndex, int currentFrameIndex)
{
//	CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	if (evt == ArmtrName::ROLE_EVT_SKILL_HURT)
	{
		this->doAttPer();
	}
}

void Role::animationStateEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		GameResPool::getInstance()->recycleRes(armature->getParent());
		
		armature->getParent()->removeFromParentAndCleanup(false);
	}
}

void Role::initAttData()
{
}

void Role::initAttSkillData()
{
}

void Role::initAttMonsterData()
{
}

/*
void Role::eventHandler(cocos2d::EventCustom * event)
{
	EventData *eventData = static_cast<EventData*>(event->getUserData());

	if (nullptr != eventData->animationState)
	{
		if (0 == EventData::COMPLETE.compare(eventData->getStringType()))
		{
			if (0 == eventData->animationState->name.compare("atk1"))
			{
				this->doAtt();

				_isCurAttFinish = true;
			}
		}
	}
}*/

