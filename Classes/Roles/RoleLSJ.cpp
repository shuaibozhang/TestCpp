#include "RoleLSJ.h"
#include "Player.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "FightLayer.h"
#include "CrushUtil.h"
#include "Monster.h"
#include "Armtr/GameArmtr.h"
#include "GameUtils.h"


RoleLsj * RoleLsj::create(int id)
{
	RoleLsj *pRet = new RoleLsj();

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

bool RoleLsj::init(int id)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Role::init(id));

		_norAttInfo.isHurtAll = false;
		_norAttInfo.isNeedMove = true;
		_norAttInfo.roleAnim = ArmtrName::ROLE_NOR_ATT;
		_norAttInfo.buffAnim = "";

		bRet = true;
	} while (false);

	return bRet;
}

void RoleLsj::update(float delta)
{
	Role::update(delta);
}

void RoleLsj::changeWeapon(int weaponId)
{
	CrushUtil::changeWeapon(_pArmtr, _roleId, weaponId);
}

void RoleLsj::initAttData()
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

//	skillFlag = 1;
	if (-1 != skillFlag)
	{
		_curAttInfo.skillId =  Player::getInstance()->getRoleSkillId(_roleId, skillFlag);
		_curAttInfo.pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(_curAttInfo.skillId);
	}
	else
	{
		_curAttInfo.skillId = -1;
		_curAttInfo.pSkillInfo = &_norAttInfo;
	}
	this->initAttSkillData();
	this->initAttMonsterData();
}

void RoleLsj::initAttSkillData()
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
		_curAttInfo.skillParam1 = 1.f * _pRoleData->attack;
		_curAttInfo.skillParam3 = 0.f;
		_curAttInfo.desPos = CrushUtil::getMonsterFrontPos(FightLayer::getInstance()->getCurWaveIndex());
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
	default:
		break;
	}
}

void RoleLsj::initAttMonsterData()
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

void RoleLsj::doAttPer()
{
	switch (_curAttInfo.pSkillInfo->mark)
	{
	case -1:
		if (nullptr != _curAttInfo.pDesMonster)
		{
			_curAttInfo.pDesMonster->doHurtByRole(_curAttInfo.skillParam1, AttAttrbt_E::ATT_NORMAL, 0, 0.f);
		}
		GameUtils::playEffect("lsj_att.ogg");
		break;
	case 1:
		auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
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

		if (6 == _curAttInfo.skillId)
		{
			GameUtils::playEffect("lsj_dyh.ogg");
		}
		else if (7 == _curAttInfo.skillId)
		{
			GameUtils::playEffect("lsj_dghs.ogg");
		}
		else if (AttAttrbt_E::ATT_FIRE == _curAttInfo.pSkillInfo->attribute)
		{
			GameUtils::playEffect("lsj_huo.ogg");
		}
		else if (AttAttrbt_E::ATT_THUNDER == _curAttInfo.pSkillInfo->attribute)
		{
			GameUtils::playEffect("lsj_lei.ogg");
		}

		break;
	}
}

bool RoleLsj::isCanAtt()
{
	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	
	return Role::isCanAtt() && !arrMonster->empty();
}
