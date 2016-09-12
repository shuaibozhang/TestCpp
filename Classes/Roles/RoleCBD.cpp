#include "RoleCbd.h"
#include "Player.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "FightLayer.h"
#include "CrushUtil.h"
#include "Monster.h"
#include "Armtr/GameArmtr.h"
#include "GameUtils.h"


RoleCbd * RoleCbd::create(int id)
{
	RoleCbd *pRet = new RoleCbd();

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

bool RoleCbd::init(int id)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Role::init(id));

		_norAttInfo.isHurtAll = false;
		_norAttInfo.isNeedMove = true;

		bRet = true;
	} while (false);

	return bRet;
}

void RoleCbd::update(float delta)
{
	Role::update(delta);
}

void RoleCbd::changeWeapon(int weaponId)
{
	CrushUtil::changeWeapon(_pArmtr, _roleId, weaponId);
}

void RoleCbd::initAttData()
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
			if (1 == _roleId && Player::getInstance()->getCurDef() < Player::getInstance()->getTotalDef())
			{
				skillFlag = 2;
			}
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

		if (Player::getInstance()->getCurDef() < Player::getInstance()->getTotalDef())
		{
			_norAttInfo.mark = 0;
		}
		else
		{
			_norAttInfo.mark = -1;
		}
	}
	this->initAttSkillData();
	this->initAttMonsterData();
}

void RoleCbd::initAttSkillData()
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
		_norAttInfo.isNeedMove = true;
		_curAttInfo.attCount = info.crushCount - 2;
		_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		_curAttInfo.skillParam1 = _pRoleData->attack;
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_ATT;
		_norAttInfo.buffAnim = "";
		break;
	case 0:
		_norAttInfo.isNeedMove = false;
		_curAttInfo.attCount = 1;
		_curAttInfo.skillParam1 = (info.crushCount - 2) * _pRoleData->dpadd;
		_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_DEF;
		_norAttInfo.buffAnim = ArmtrName::ROLE_STATE_ADD_DEF;
		break;
	case 1:
		if (3 == info.crushType)
		{
			if (0 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->normalVal[1];
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->normalVal[2];
			}
			else if (1 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[1];
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[2];
			}
			else if (2 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[1];
				_curAttInfo.skillParam2 = (3 * (int)_curAttInfo.pSkillInfo->enhanceVal[2] + 1) / 2;
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (0 == itemLv[i])
				{
					_curAttInfo.skillParam1 += 1.f;
				}
				else if (1 == itemLv[i])
				{
					_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->normalVal[1];
					_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[2];
				}
				else if (2 == itemLv[i])
				{
					_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->enhanceVal[1];
					_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->enhanceVal[2];
				}
			}
			_curAttInfo.skillParam1 -= 2.f;
		}

		_curAttInfo.skillParam3 = ParamData::WEAK_ROUND_DAMAGE_PARAM;

		_curAttInfo.skillParam1 *= _pRoleData->attack;
		_curAttInfo.skillParam3 *= _curAttInfo.skillParam1; //_pRoleData->attack;

		if (_curAttInfo.pSkillInfo->isHurtAll)
		{
			_curAttInfo.desPos = CrushUtil::getMonsterCenterPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		else
		{
			_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
		}
		break;
	case 3:
		_curAttInfo.attCount = 1;
		for (int i = 0; i < 3; i++)
		{
			if (0 == itemLv[i])
			{
				_curAttInfo.skillParam1 += 1.f;
			}
			else if (1 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->normalVal[0];
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[1];
			}
			else if (2 == itemLv[i])
			{
				_curAttInfo.skillParam1 += _curAttInfo.pSkillInfo->enhanceVal[0];
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->enhanceVal[1];
			}
		}

		if (1 == itemLv[0])
		{
			_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->normalVal[2];
		}
		else if (2 == itemLv[0])
		{
			_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->enhanceVal[2];
		}
		_curAttInfo.skillParam1 += (info.crushCount - 5.f);
		_curAttInfo.skillParam1 *= _pRoleData->dpadd;
		_curAttInfo.skillParam3 *= _pRoleData->dpadd;
		_curAttInfo.desPos = CrushUtil::getRoleCenterPos(FightLayer::getInstance()->getCurPlayerPosId());
		break;
	case 6:
		_curAttInfo.attCount = 1;

		if (0 == itemLv[0])
		{
			_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->normalVal[0];
			_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->normalVal[2];
			_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->normalVal[1];
		}
		else if (1 == itemLv[0])
		{
			_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->normalVal[0];
			_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[2];
			_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->enhanceVal[1];
		}
		else if (2 == itemLv[0])
		{
			_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[0];
			_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[2] + 1;
			_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->enhanceVal[1];
		}
//		_curAttInfo.skillParam1 -= 2.f;
//		_curAttInfo.skillParam2 -= 2;

		_curAttInfo.skillParam1 *= _pRoleData->dpadd;
		_curAttInfo.desPos = CrushUtil::getRoleCenterPos(FightLayer::getInstance()->getCurPlayerPosId());
		break;
	default:
		break;
	}
}

void RoleCbd::initAttMonsterData()
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
	case 1:
		if (!pSkillInfo->isHurtAll)
		{
			_curAttInfo.pDesMonster = this->getTargetMonster();
			_curAttInfo.desPos = _curAttInfo.pDesMonster->getPosition();
		}
		break;
	}

	CC_SAFE_RETAIN(_curAttInfo.pDesMonster);
}

void RoleCbd::doAttPer()
{
	if (_isCurAttFinish)
	{
		return;
	}
	CrushArr_T info = _attInfoQueue.front();
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		if (nullptr != _curAttInfo.pDesMonster)
		{
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, AttAttrbt_E::ATT_NORMAL, 0, 0.f);
		}
		GameUtils::playEffect("cbd_att.ogg");
		break;
	case 0:
		Player::getInstance()->addDef(_curAttInfo.skillParam1);
		//		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		GameUtils::playEffect("cbd_fangyu.ogg");
		break;
	case 1:
		if (!arrMonster->empty())
		{
			if (_curAttInfo.pSkillInfo->isHurtAll)
			{
				for (int i = arrMonster->size() - 1; i >= 0; i--)
				{
					auto pMonster = arrMonster->at(i);
					pMonster->doHurtByRole(_curAttInfo.skillParam1, _curAttInfo.pSkillInfo->attribute, _curAttInfo.skillParam2, _curAttInfo.skillParam3);
				}
			}
			else
			{
				_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, _curAttInfo.pSkillInfo->attribute, _curAttInfo.skillParam2, _curAttInfo.skillParam3);
			}
		}

		if (15 == _curAttInfo.skillId)
		{
			GameUtils::playEffect("cbd_hbfb.ogg");
		}
		else
		{
			GameUtils::playEffect("cbd_bing.ogg");
		}
		break;
	case 3:
		Player::getInstance()->addDef(&_curAttInfo);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		if (_curAttInfo.skillParam2 > 0)
		{
			Player::getInstance()->addRecover(1, _curAttInfo.skillParam2, _curAttInfo.skillParam3);
		}
		GameUtils::playEffect("cbd_fangyu.ogg");
		break;
	case 6:
		Player::getInstance()->addDef(&_curAttInfo);
		Player::getInstance()->getShield(&_curAttInfo);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim);
		GameUtils::playEffect("cbd_jysg.ogg");
		break;
	}
}

bool RoleCbd::isCanAtt()
{
	bool bRet = false;
	if (Role::isCanAtt())
	{
		auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
		if ((Player::getInstance()->getCurDef() < Player::getInstance()->getTotalDef() && !arrMonster->empty())
			|| (Player::getInstance()->getCurDef() >= Player::getInstance()->getTotalDef()))
		{
			bRet = true;
		}
	}

	return bRet;
}
