#include "RoleSqy.h"
#include "Player.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "FightLayer.h"
#include "CrushUtil.h"
#include "Monster.h"
#include "Armtr/GameArmtr.h"
#include "GameUtils.h"


RoleSqy * RoleSqy::create(int id)
{
	RoleSqy *pRet = new RoleSqy();

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

bool RoleSqy::init(int id)
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

void RoleSqy::update(float delta)
{
	Role::update(delta);
}

void RoleSqy::changeWeapon(int weaponId)
{
// 	int index = (weaponId - 530) > 4 ? 4 : (weaponId - 530);
// 	_pArmtr->getArmtr()->getBone("Layer26")->changeDisplayByIndex(index, true	
	CrushUtil::changeWeapon(_pArmtr, _roleId, weaponId);
}

void RoleSqy::initAttData()
{
	CrushArr_T info = _attInfoQueue.front();
	int skillFlag = -1;

	/*init data*/
	_curAttInfo.skillId = -1;
	_curAttInfo.pSkillInfo = nullptr;
	_curAttInfo.attCount = 1;
	_curAttInfo.curAttCount = 0;// _curAttInfo.attCount;
	_curAttInfo.skillParam2 = 0;
	_curAttInfo.skillParam1 = 1.f;
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

void RoleSqy::initAttSkillData()
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

	_curAttInfo.desPos = CrushUtil::getRoleCenterPos(FightLayer::getInstance()->getCurPlayerPosId());
	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		_curAttInfo.skillParam1 = _pRoleData->hpadd * (info.crushCount - 2);
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_ATT;
		_norAttInfo.buffAnim = ArmtrName::ROLE_STATE_ADD_HP;
		break;
	case 2:
		if (3 == info.crushType)
		{
			if (0 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->normalVal[0];
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->normalVal[1];
				_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->normalVal[2];
			}
			else if (1 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[0];
				_curAttInfo.skillParam2 = (int)_curAttInfo.pSkillInfo->enhanceVal[1];
				_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->enhanceVal[2];
			}
			else if (2 == itemLv[0])
			{
				_curAttInfo.skillParam1 = _curAttInfo.pSkillInfo->enhanceVal[0];
				_curAttInfo.skillParam2 = (3 * (int)_curAttInfo.pSkillInfo->enhanceVal[2] + 1) / 2;
				_curAttInfo.skillParam3 = _curAttInfo.pSkillInfo->enhanceVal[2];
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
		}

		_curAttInfo.skillParam1 *= _pRoleData->hpadd;
		_curAttInfo.skillParam3 *= _pRoleData->hpadd;
		break;
	case 7:
		for (int i = 0; i < 3; i++)
		{
			if (0 == itemLv[i])
			{
				_curAttInfo.skillParam1 += 1.f;
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[1];
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
		_curAttInfo.skillParam1 += (info.crushCount - 5.f);// -= 2.f;
		_curAttInfo.skillParam2 -= 2;
		_curAttInfo.skillParam3 -= _curAttInfo.pSkillInfo->enhanceVal[2];
		_curAttInfo.skillParam1 *= _pRoleData->hpadd;
		break;
	case 8:
		for (int i = 0; i < 3; i++)
		{
			if (0 == itemLv[i])
			{
				_curAttInfo.skillParam1 += 1.f;
				_curAttInfo.skillParam2 += (int)_curAttInfo.pSkillInfo->normalVal[1];
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
		_curAttInfo.skillParam1 += (info.crushCount - 5.f);// -= 2.f;
		_curAttInfo.skillParam2 -= 2;
		_curAttInfo.skillParam1 *= _pRoleData->hpadd;
		break;
	case 9:
		_curAttInfo.skillParam1 = 3.f * _pRoleData->hpadd;
		Player::getInstance()->getRelive(&_curAttInfo);
		break;
	case 10:
		_curAttInfo.skillParam1 = 3.f * _pRoleData->hpadd;
		break;
	}
}

void RoleSqy::initAttMonsterData()
{
	_curAttInfo.pDesMonster = nullptr;
}

void RoleSqy::doAttPer()
{
	CrushArr_T info = _attInfoQueue.front();
	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		GameUtils::playEffect("sqy_xue.ogg");
		break;
	case 2:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		if (0 != _curAttInfo.skillParam2)
		{
			Player::getInstance()->addRecover(0, _curAttInfo.skillParam2, _curAttInfo.skillParam3);
		}
		if (-1 == _curAttInfo.pSkillInfo->starNum)
		{
			Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim);
		}
		else
		{
			Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		}
		GameUtils::playEffect("sqy_jiaxue.ogg");
		break;
	case 7:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		GameUtils::playEffect("sqy_jinghua.ogg");
		break;
	case 8:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim, _roleId, info.crushCount - 2);
		GameUtils::playEffect("sqy_zengfu.ogg");
		break;
	case 9:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim);
		GameUtils::playEffect("sqy_jiaxue.ogg");
		break;
	case 10:
		Player::getInstance()->addHp(_curAttInfo.skillParam1);
		Player::getInstance()->playStateAnim(_curAttInfo.pSkillInfo->buffAnim);
		GameUtils::playEffect("sqy_jinghua.ogg");
		break;
	}
}