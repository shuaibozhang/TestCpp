#include "RoleQyl.h"
#include "Player.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "FightLayer.h"
#include "CrushUtil.h"
#include "Monster.h"
#include "Armtr/GameArmtr.h"
#include "GameUtils.h"
#include "GameResPool.h"


RoleQyl * RoleQyl::create(int id)
{
	RoleQyl *pRet = new RoleQyl();

	if (nullptr != pRet && pRet->init(id))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoleQyl::init(int id)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Role::init(id));

		_norAttInfo.isHurtAll = false;
		_norAttInfo.isNeedMove = false;

		bRet = true;
	} while (false);

	return bRet;
}

void RoleQyl::update(float delta)
{
	Role::update(delta);
}

void RoleQyl::changeWeapon(int weaponId)
{
	CrushUtil::changeWeapon(_pArmtr, _roleId, weaponId);
}

void RoleQyl::initAttData()
{
	CrushArr_T info = _attInfoQueue.front();
	int skillFlag = -1;

	/*init data*/
	_curAttInfo.skillId = -1;
	_curAttInfo.pSkillInfo = nullptr;
	_curAttInfo.attCount = (0 >= (info.crushCount - 2)) ? 1 : (info.crushCount - 2);
	_curAttInfo.curAttCount = 0;// _curAttInfo.attCount;
	_curAttInfo.skillParam2 = 0;
	_curAttInfo.skillParam1 = 0.f;
	_curAttInfo.skillParam3 = 0.f;
	_curAttInfo.pDesMonster = nullptr;

	if (3 == info.crushType)
	{
		skillFlag = 1;
	}
	else
	{
		if (info.arrLvCount[1] > 0 || info.arrLvCount[2] > 0)
		{
			skillFlag = 0;
		}
	}

	if (-1 != skillFlag)
	{
		_curAttInfo.skillId = Player::getInstance()->getRoleSkillId(_roleId, skillFlag);
		_curAttInfo.pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(_curAttInfo.skillId);
	}
	else
	{
		_curAttInfo.skillId = -1;
		_curAttInfo.pSkillInfo = &_norAttInfo;
		_norAttInfo.mark = -1;
	}
	this->initAttSkillData();
	this->initAttMonsterData();
}

void RoleQyl::initAttSkillData()
{
	CrushArr_T info = _attInfoQueue.front();
	int itemLv[3] = { 0 };
	int damageCount = 0;

	for (int lv = 2; lv >= 0; lv--)
	{
		for (int i = 0; i < info.arrLvCount[lv] && damageCount < 3; i++)
		{
			itemLv[damageCount] = lv;
			damageCount++;

			if (3 == damageCount)
			{
				break;
			}
		}
	}

	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		_norAttInfo.isNeedMove = false;
		_curAttInfo.attCount = info.crushCount - 2;
		_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		_curAttInfo.skillParam1 = _pRoleData->attack;
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_ATT;
		_norAttInfo.buffAnim = "";
		break;
	case 11:
		for (int i = 0; i < 3; i++)
		{
			if (0 == itemLv[i])
			{
				_curAttInfo.skillParam1 += 1.f;
			}
			else if (1 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->normalVal[1];
			}
			else if (2 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->enhanceVal[1];
			}
		}
		_curAttInfo.skillParam1 -= 2.f;
		_curAttInfo.skillParam1 *= _pRoleData->attack;
		_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		break;
	case 12:
		if (3 == info.crushType)
		{
			if (0 == itemLv[0])
			{
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->normalVal[1];
			}
			else if (1 == itemLv[0])
			{
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[1];
			}
			else if (2 == itemLv[0])
			{
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[1];
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (0 == itemLv[i])
				{
					_curAttInfo.skillParam2 += 1;
				}
				else if (1 == itemLv[i])
				{
					_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[1];
				}
				else if (2 == itemLv[i])
				{
					_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->enhanceVal[1];
				}
			}
			_curAttInfo.skillParam2 -= 2;
		}

		if (_curAttInfo.pSkillInfo->isHurtAll)
		{
			_curAttInfo.desPos = CrushUtil::getMonsterCenterPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		else
		{
			_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		break;
	case 13:
		for (int i = 0; i < 3; i++)
		{
			if (0 == itemLv[i])
			{
				_curAttInfo.skillParam1 += 1.f;
				_curAttInfo.skillParam3 += 1.f;
				_curAttInfo.skillParam2 += 1;
			}
			else if (1 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->normalVal[0];
				_curAttInfo.skillParam3 += _curAttInfo.pSkillInfo->normalVal[1];
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[2];
			}
			else if (2 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->enhanceVal[0];
				_curAttInfo.skillParam3 += _curAttInfo.pSkillInfo->enhanceVal[1];
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->enhanceVal[2];
			}
		}
		_curAttInfo.skillParam1 -= 2.f;
		_curAttInfo.skillParam3 -= 2.f;
		_curAttInfo.skillParam2 -= 2;
		_curAttInfo.skillParam1 *= _pRoleData->attack;
		break;
	case 14:
		if (3 == info.crushType)
		{
			if (0 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->normalVal[1];
			}
			else if (1 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[1];
			}
			else if (2 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[1];
			}

			_curAttInfo.skillParam3 = Player::getInstance()->getCurDp();
		}
		break;
	default:
		break;
	}
}

void RoleQyl::initAttMonsterData()
{
	auto pSkillInfo = _curAttInfo.pSkillInfo;
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();

	_curAttInfo.pDesMonster = nullptr;
	switch (pSkillInfo->mark)
	{
	case -1:
		_curAttInfo.pDesMonster = this->getTargetMonster();
		_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		break;
	case 11:
		if (!pSkillInfo->isHurtAll)
		{
			_curAttInfo.pDesMonster = this->getTargetMonster();
			_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		}
		break;
	case 12:
		if (!pSkillInfo->isHurtAll)
		{
			_curAttInfo.pDesMonster = this->getTargetMonster();
			_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		}
		else
		{
			_curAttInfo.desPos = CrushUtil::getMonsterCenterPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		break;
	case 13:
		if (!pSkillInfo->isHurtAll)
		{
			_curAttInfo.pDesMonster = this->getTargetMonster();
			_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		}
		break;
	case 14:
		if (!pSkillInfo->isHurtAll)
		{
			_curAttInfo.pDesMonster = this->getTargetMonster();
			_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		}
		else
		{
			_curAttInfo.desPos = CrushUtil::getMonsterCenterPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		break;
	}

	CC_SAFE_RETAIN(_curAttInfo.pDesMonster);
}

void RoleQyl::doAttPer()
{
	CrushArr_T info = _attInfoQueue.front();
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		if (nullptr != _curAttInfo.pDesMonster)
		{
			float damage = 0.f;
			this->retain();
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, AttAttrbt_E::ATT_NORMAL, (int)(&damage), -101.f);
			this->release();
			Player::getInstance()->addHp(damage * 0.4f);
			Player::getInstance()->addDp(damage * 0.3f);

			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP, 0, 1);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_DEF, 1, 1);

			/*hit5*/
			auto pAnim = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_HIT));//GameArmtr::createHit("hit3");
			pAnim->play("hit5");
			pAnim->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Role::animationStateEvent, this));
			FightLayer::getInstance()->addChild(pAnim);
			pAnim->setPosition(_curAttInfo.desPos);
		}
		GameUtils::playEffect("qyl_att.ogg");
		break;
	case 11:
		if (!arrMonster->empty())
		{
			float damage = 0.f;
			this->retain();
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, _curAttInfo.pSkillInfo->attribute, (int)(&damage), -101.f);
			this->release();

			Player::getInstance()->addHp(damage, _rolePosIndex);
			Player::getInstance()->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP, _rolePosIndex, info.crushCount - 2);
		}
		GameUtils::playEffect("qyl_tunshi.ogg");
		break;
	case 12:
		if (!arrMonster->empty())
		{
			if (_curAttInfo.pSkillInfo->isHurtAll)
			{
				for (int i = arrMonster->size() - 1; i >= 0; i--)
				{
					auto pMonster = arrMonster->at(i);
					pMonster->doHurtByRole(0.f, _curAttInfo.pSkillInfo->attribute, _curAttInfo.skillParam2, 0.f);
				}
				GameUtils::playEffect("qyl_byxh.ogg");
			}
			else
			{
				_curAttInfo.pDesMonster->doHurtByRole(0.f, _curAttInfo.pSkillInfo->attribute, _curAttInfo.skillParam2, 0.f);
				GameUtils::playEffect("qyl_mazui.ogg");
			}
		}
		break;
	case 13:
		if (!arrMonster->empty())
		{
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, _curAttInfo.pSkillInfo->attribute, 0, 0.f);

			Player::getInstance()->playBuffAttAnim();

			Player::getInstance()->addAttBuff(_curAttInfo.skillParam2, _curAttInfo.skillParam3);
		}
		GameUtils::playEffect("qyl_kurong.ogg");
		break;
	case 14:
		if (!arrMonster->empty())
		{
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1 * _curAttInfo.skillParam3, AttAttrbt_E::ATT_NORMAL, 0, 0.f);
			Player::getInstance()->addDp(-_curAttInfo.skillParam3);
		}
		GameUtils::playEffect("qyl_zmyj.ogg");
		break;
	}
}

bool RoleQyl::isCanAtt()
{
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();

	return Role::isCanAtt() && !arrMonster->empty();
}
