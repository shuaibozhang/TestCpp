#include "Player.h"
#include "Role.h"
#include "FightLayer.h"
#include "CrushUtil.h"
#include "GameLayer.h"
#include "UserData.h"
#include "ParamMgr.h"
#include "WeaponControl.h"
#include "SkillControl.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "RoleLSJ.h"
#include "RoleCBD.h"
#include "RoleLQC.h"
#include "RoleSQY.h"
#include "Scenes/GameMap.h"
#include "RoundTag.h"
#include "Armtr/GameArmtr.h"
#include "GameUtils.h"
#include "JRCommon.h"
#include "Defines.h"


Player *Player::s_pInstance = nullptr;


Player::Player()
{
	memset((void *)_arrRecoverInfo, 0, sizeof(_arrRecoverInfo));
	memset((void *)_arrRoleInfo, 0, sizeof(_arrRoleInfo));
	s_pInstance = this;
	_isDefing = false;
	_pArmtrShield = nullptr;
	_isDead = false;
	_baseLv = 0;
	_isWaitRelive = false;
}

Player::~Player()
{
	s_pInstance = nullptr;
}

Player * Player::getInstance()
{
	return s_pInstance;
}

Player * Player::create()
{
	Player *pRet = new Player();

	if (nullptr != pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool Player::init()
{
	bool bRet = false;
	do 
	{
//		ArmatureDataManager::getInstance()->addArmatureFileInfo("roles/zhuangbei/zhuangbei.ExportJson");

		Role *pRole = RoleLsj::create(0);
		_arrRoles.push_back(pRole);

		pRole = RoleCbd::create(1);
		_arrRoles.push_back(pRole);

		pRole = RoleLqc::create(2);
		_arrRoles.push_back(pRole);

		pRole = RoleSqy::create(3);
		_arrRoles.push_back(pRole);

		FightLayer::getInstance()->addChild(_arrRoles[2]);
		FightLayer::getInstance()->addChild(_arrRoles[0]);
		FightLayer::getInstance()->addChild(_arrRoles[3]);
		FightLayer::getInstance()->addChild(_arrRoles[1]);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			auto weaponInfo = WeaponControl::getInstance()->getEquipWenpon(i);
			_arrRoles[i]->changeWeapon(weaponInfo.id);
		}

		this->initFightData();

		bRet = true;
	} while (false);

	return bRet;
}


void Player::initFightData()
{
	memset(_arrRoleSkillId, -1, sizeof(_arrRoleSkillId));
	_totalHp = 0.f;
	_totalDef = 0.f;
	_baseLv = 1000;
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		int roleLv = UserData::getInstance()->getPlayerCurLv(i);
		_arrRoleInfo[i] = ParamMgr::getInstance()->getPlayerInfo(i, roleLv);

		_baseLv = MIN(_baseLv, roleLv);
		_totalHp += _arrRoleInfo[i].hp;
		_totalDef += _arrRoleInfo[i].dp;

		auto weaponInfo = WeaponControl::getInstance()->getEquipWenpon(i);
		auto attInfo = UserData::getInstance()->getWeaponAttack(weaponInfo.id-500);
		_arrRoleInfo[i].attack += attInfo.attack;//weaponInfo.attack;
		_arrRoleInfo[i].dp += attInfo.def;//weaponInfo.def;
		_arrRoleInfo[i].hpadd += attInfo.hpAdd;//weaponInfo.hpadd;
		_arrRoleInfo[i].dpadd += attInfo.dpAdd;//weaponInfo.dpadd;
		_arrRoleInfo[i].def += attInfo.def;//weaponInfo.def;

		auto &equSkillInfo = SkillControl::getInstance()->getPlayerSkillContralByPlayerId(i);
		for (int j = 0; j < equSkillInfo.size(); j++)
		{
			auto tmp = equSkillInfo.at(j);

			if (SkillControl::getInstance()->checkIsEquipSkill(i, tmp.skillid))
			{
				_arrRoleSkillId[i][tmp.flagmask] = tmp.skillid;	//flag 0.nor 1.super 2.def
			}
		}

		_arrRoles[i]->initFightData();
	}

	if (2 == GameLayer::getInstance()->getFightType())
	{
		int sceneId = GameLayer::getInstance()->getSceneId();
//		_baseLv += CrushUtil::getEliteAddLv(sceneId);//ParamData::ELITE_STAGE_ADD_LV;
		_baseLv = CrushUtil::getEliteAddLv(sceneId);//ParamData::ELITE_STAGE_ADD_LV;
	}

	_curHp = _totalHp;
	_curDef = _totalDef;
	_isDead = false;
	_isWaitRelive = false;
	_reliveTimes = 0;

	GameLayer::getInstance()->setDef(_curDef, _curDef * 100.f / _totalDef);
	GameLayer::getInstance()->setHp(_curHp, _curHp * 100.f / _totalHp);
}

void Player::resetFightData()
{
	for (int i = 0; i < 4; i++)
	{
		if (nullptr != _arrRecoverInfo[i].pTag)
		{
			_arrRecoverInfo[i].pTag->removeFromParent();
			_arrRecoverInfo[i].pTag = nullptr;
		}
	}
	memset((void *)_arrRecoverInfo, 0, sizeof(_arrRecoverInfo));

	if (nullptr != _pArmtrShield)
	{
		_pArmtrShield->removeFromParent();
		_pArmtrShield = nullptr;
	}
}

void Player::resetBarData()
{
	_totalHp = 0.f;
	_totalDef = 0.f;
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		int roleLv = UserData::getInstance()->getPlayerCurLv(i);
		_arrRoleInfo[i] = ParamMgr::getInstance()->getPlayerInfo(i, roleLv);

		_totalHp += _arrRoleInfo[i].hp;
		_totalDef += _arrRoleInfo[i].dp;

		//		_arrRoles[i]->resetFightData();
	}
	_curHp = _totalHp;
	_curDef = _totalDef;

	GameLayer::getInstance()->setDef(_curDef, _curDef * 100.f / _totalDef);
	GameLayer::getInstance()->setHp(_curHp, _curHp * 100.f / _totalHp);
}


void Player::placeRole(const Vec2 & newPos)
{
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		auto pRole = _arrRoles.at(i);
		pRole->setPosition(CrushUtil::getRoleOffsetPos(i) + newPos);
	}
}

void Player::moveToPos(int posIndex, float xDt, float dur, float bufferTime)
{
//	Vec2 layoutPos = CrushUtil::getRoleCenterPos(posIndex);//_startPox + Vec2((posIndex+1) * xDt, 0.f);
	float delayTime = 0.f;// bufferTime / 3.f;

	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		auto pRole = _arrRoles.at(i);
		auto rolePos = CrushUtil::getRolePos(i, posIndex);//CrushUtil::getRoleOffsetPos(i) + layoutPos;

		auto act = Sequence::create(DelayTime::create(delayTime), CallFunc::create([=] {pRole->playMove(); }), MoveTo::create(dur, rolePos), CallFunc::create([=] {pRole->playIdle(); }), nullptr);
		act->setTag(9999);
		pRole->stopActionsByFlags(9999);
		pRole->runAction(Sequence::create(DelayTime::create(delayTime), CallFunc::create([=] {pRole->playMove(); }), MoveTo::create(dur, rolePos), CallFunc::create([=] {pRole->playIdle(); }), nullptr));
		delayTime += (bufferTime / 3.f);
	}

	if (nullptr != _pArmtrShield)
	{
//		_pArmtrShield->runAction(FadeOut::create(0.2f));
		_pArmtrShield->setVisible(false);

		_pArmtrShield->runAction(Sequence::create(DelayTime::create(bufferTime + dur), CallFunc::create([=] {
			Vec2 shieldPos = CrushUtil::getShieldPos(FightLayer::getInstance()->getCurPlayerPosId());
			_pArmtrShield->setPosition(shieldPos); }),
//			FadeIn::create(0.2f),
			Show::create(),
				nullptr));
	}
}

void Player::moveToSettlePos(bool isWin)
{
	float fSpace = 81.f;

	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		float x = fSpace + (640.f - fSpace * 2) / 3 * (ParamData::ROLE_COUNT - 1 - i);
		float y = ParamData::ROLE_START_POS_Y;
		Vec2 rolePos = FightLayer::getInstance()->convertToNodeSpace(Vec2(x, y));
		auto pRole = _arrRoles.at(i);

		pRole->runAction(Sequence::create(CallFunc::create([=] {pRole->playMove(); }), MoveTo::create(0.5f, rolePos), 
			CallFunc::create([=] {
			if (isWin)
			{
				pRole->playCheer();
			}
			else
			{
				pRole->playIdle();
			} }), nullptr));
	}
}

void Player::moveToOutsidePos(int posIndex, float xDt, float dur, float bufferTime)
{
	float delayTime = 0.f;// bufferTime / 3.f;

//	float arrOffsetX[4] = { 92.f, 72.f, 51.f, 0.f };
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		auto pRole = _arrRoles.at(i);
		auto rolePos = CrushUtil::getRolePos(i, posIndex);//CrushUtil::getRoleOffsetPos(i) + layoutPos;
//		rolePos.x += arrOffsetX[i];

		auto act = Sequence::create(DelayTime::create(delayTime), CallFunc::create([=] {pRole->playMove(); }), MoveTo::create(dur, rolePos), CallFunc::create([=] {pRole->playIdle(); }), nullptr);
		act->setTag(9999);
		pRole->stopActionsByFlags(9999);
		pRole->runAction(act);
		delayTime += (bufferTime / 3.f);
	}

	if (nullptr != _pArmtrShield)
	{
		//		_pArmtrShield->runAction(FadeOut::create(0.2f));
		_pArmtrShield->setVisible(false);

		_pArmtrShield->runAction(Sequence::create(DelayTime::create(bufferTime + dur), CallFunc::create([=] {
			Vec2 shieldPos = CrushUtil::getShieldPos(FightLayer::getInstance()->getCurPlayerPosId());
			_pArmtrShield->setPosition(shieldPos); }),
			//			FadeIn::create(0.2f),
				Show::create(),
				nullptr));
	}
}

void Player::addAttInfo(CrushArr_T * pCrushInfo)
{
	if (4 != pCrushInfo->crushType)
	{
		if (pCrushInfo->crushEleId == EleIconId_E::ELE_ID_SUPER)
		{
			for (int i = 0; i < _arrRoles.size(); i++)
			{
				//debug
//				if (1 != i && 0 != i)
// 				if (3 != i)
// 				{
// 					continue;
// 				}
				auto pRole = _arrRoles.at(i);
				pRole->addAttInfo(pCrushInfo);
			}
		}
		else
		{
			//debug
//			if (1 != (pCrushInfo->crushEleId - EleIconId_E::ELE_ID_SWORD) && 0 != (pCrushInfo->crushEleId - EleIconId_E::ELE_ID_SWORD))
// 			if (3 != (pCrushInfo->crushEleId - EleIconId_E::ELE_ID_SWORD))
// 			{
// 				return;
// 			}
			auto pRole = _arrRoles.at(pCrushInfo->crushEleId - EleIconId_E::ELE_ID_SWORD);
			pRole->addAttInfo(pCrushInfo);
		}
	}
}

bool Player::doHeartSkill(EleIcon *pEle, int skillFlag)
{
	int skillId = _arrRoleSkillId[3][skillFlag];
	auto pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(skillId);
	int eleLv = pEle->getEleId();

	if (7 == pSkillInfo->mark)
	{
		int cleanLv = pSkillInfo->normalVal[2];
		int count = pSkillInfo->normalVal[1];
		if (2 == eleLv)
		{
			cleanLv = pSkillInfo->enhanceVal[2];
			count = pSkillInfo->enhanceVal[1];
		}

		return this->doClean(cleanLv, count, 0, true, pEle->getPosition());
	}
	else if (8 == pSkillInfo->mark)
	{
		int count = pSkillInfo->normalVal[1];
		if (2 == eleLv)
		{
			count = pSkillInfo->enhanceVal[1];
		}

		return this->doAddEleLv(1, count, true, pEle->getPosition());
	}
	else if (10 == pSkillInfo->mark)
	{
		return this->doClean(2, -1, 1);
	}

	return false;
}

void Player::doHurt(int attribute, float damage)
{
	if (_isDead || 1 != FightLayer::getInstance()->getGameState())
	{
		return;
	}
	_curHp -= damage;
	_curHp = (_curHp < 0.f) ? 0.f : _curHp;
// 	if (_curHp > 0.f && _curHp < 1.f)
// 	{
// 		_curHp = 1.f;
// 	}

	FightLayer::getInstance()->showValueChange(0, -damage, _arrRoles[0]->getPosition());
	GameLayer::getInstance()->setHp(_curHp, _curHp * 100.f / _totalHp);

	if (_curHp <= 0.f)
	{
		this->doDead();
	}
}

void Player::doHurtByMonster(int attribute, float damage, int monsterLv)
{
	if (_isDead || 1 != FightLayer::getInstance()->getGameState())
	{
		return;
	}

	float actDamage = damage;
	
	if (6 == _arrRecoverInfo[3].param1 && _arrRecoverInfo[3].round > 0)
	{
		if (nullptr != _pArmtrShield)
		{
			_pArmtrShield->play("hit");
			_pArmtrShield->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {_pArmtrShield->play("skill_38"); })));

			this->reduceBuffRound(3);
		}
		return;
	}
	else if (_curDef > 0.f)// || (6 == _arrRecoverInfo[3].param1 && _arrRecoverInfo[3].round>0))
	{
		float armorValue = _arrRoleInfo[1].def;
// 		if (6 == _arrRecoverInfo[3].param1 && _arrRecoverInfo[3].round>0)
// 		{
// 			armorValue *= 3;
// 		}
		float valueN = CrushUtil::getDamageN(MAX(monsterLv, _baseLv));
		actDamage = damage * 1.f / (1.f + valueN * armorValue);

		if (_curDef == _totalDef && damage > 0.f)
		{
			GameLayer::getInstance()->getDefBarArmtr()->setVisible(false);
		}
		_curDef -= damage;
		_curHp -= actDamage;
	}
	else
	{
		_curHp -= actDamage;
	}

	if (_isDefing)
	{
		_arrRoles[1]->playDefAnim();
		_arrRoles[1]->playHitAnim("hit1");
	}
	else
	{
		if (nullptr == _pArmtrShield)
		{
			_arrRoles[0]->playHitAnim("hit2");
		}
		else
		{
			_pArmtrShield->play("hit");
			_pArmtrShield->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {_pArmtrShield->play("skill_38"); })));

			this->reduceBuffRound(3);
		}
	}

	_curHp = (_curHp < 0.f) ? 0.f : _curHp;
	_curDef = (_curDef < 0.f) ? 0.f : _curDef;

	FightLayer::getInstance()->showValueChange(0, -actDamage, _arrRoles[0]->getPosition());
	GameLayer::getInstance()->setDef(_curDef, _curDef * 100.f / _totalDef);
	GameLayer::getInstance()->setHp(_curHp, _curHp * 100.f / _totalHp);
	
	if (_curHp <= 0.f)
	{
		this->doDead();
	}
}

void Player::addDef(const RoleCurAttInfo_T * pAttInfo)
{
//	float addValue = pAttInfo->skillParam1;
	
	this->addDef(pAttInfo->skillParam1);

// 	if (3 == pAttInfo->pSkillInfo->mark)
// 	{
// 		this->addRecover(1, pAttInfo->skillParam2, pAttInfo->skillParam3);
// 	}
// 	else if ()
// 	{
// 	}
}

void Player::addDef(float value)
{
	if (_curDef < _totalDef && (_curDef+value) >= _totalDef)
	{
		GameLayer::getInstance()->getDefBarArmtr()->setVisible(true);
	}
	_curDef += value;
	if (_curDef > _totalDef)
	{
		_curDef = _totalDef;
	}
	GameLayer::getInstance()->playBarIconAnim(1);
	FightLayer::getInstance()->showValueChange(2, value, _arrRoles[1]->getPosition());
	GameLayer::getInstance()->setDef(_curDef, _curDef * 100.f / _totalDef);
}

void Player::addHp(const RoleCurAttInfo_T * pAttInfo)
{
	float addValue = pAttInfo->skillParam1;

	this->addHp(addValue);
}

void Player::addHp(float value)
{
	_curHp += value;
	if (_curHp > _totalHp)
	{
		_curHp = _totalHp;
	}

	GameLayer::getInstance()->playBarIconAnim(0);
	FightLayer::getInstance()->showValueChange(0, value, _arrRoles[3]->getPosition());
	GameLayer::getInstance()->setHp(_curHp, _curHp * 100.f / _totalHp);
}

// void Player::doClean(const RoleCurAttInfo_T * pAttInfo)
// {
// }

bool Player::doClean(int lv, int count, int startType, bool isNeedAnim, Vec2 heartPos)
{
	bool bRet = false;
	auto &arrWeakEle = CrushLayer::getInstance()->getWeakEle();
	int curCount = 0;

	if (-1 == count)
	{
		count = arrWeakEle.size();
	}

	if (0 == startType)
	{
		for (int i = 0; i < arrWeakEle.size() && curCount < count;)
		{
			EleIcon *pEle = arrWeakEle.at(i);
			if (pEle->isCanClean(lv))
			{
				bRet = true;
				float animDelay = 0.f;
				if (isNeedAnim)
				{
					animDelay = 0.3f;
					this->playLinkAnim(heartPos, pEle->getPosition());
				}
				auto posIndex = pEle->getPosIndex();
				auto pSpecl = pEle->getSpecInfo();
				if (pEle->doClean(lv, animDelay))
				{
//					auto posIndex = pEle->getPosIndex();
//					it = arrWeakEle.erase(it);
					if (nullptr != pSpecl)
					{
						CrushLayer::getInstance()->setEleIcon(posIndex.row, posIndex.column, nullptr);
					}
				}
				else
				{
					i++;
				}
				curCount++;
			}
			else
			{
				i++;
			}
		}
		/*
		for (Vector<EleIcon *>::iterator it = arrWeakEle.begin(); it != arrWeakEle.end() && curCount < count;)
		{
			EleIcon *pEle = *it;
			if (pEle->isCanClean(lv))
			{
				if (isNeedAnim)
				{
					this->playLinkAnim(heartPos, pEle->getPosition());
				}
				auto posIndex = pEle->getPosIndex();
				if (pEle->doClean(lv))
				{
//					auto posIndex = pEle->getPosIndex();
					it = arrWeakEle.erase(it);
					CrushLayer::getInstance()->setEleIcon(posIndex.row, posIndex.column, nullptr);
				}
				else
				{
					it++;
				}
				curCount++;
			}
		}*/
	}
	else if (1 == startType)
	{
		for (int i = arrWeakEle.size() - 1; i >= 0 && curCount < count; i--)
		{
			EleIcon *pEle = arrWeakEle.at(i);
			auto pSpecl = pEle->getSpecInfo();
			
			if (pEle->isCanClean(lv))
			{
				bRet = true;
				float animDelay = 0.f;
				if (isNeedAnim)
				{
					animDelay = 0.3f;
					this->playLinkAnim(heartPos, pEle->getPosition());
				}

				auto posIndex = pEle->getPosIndex();
				if (pEle->doClean(lv, animDelay) && nullptr != pSpecl)
				{
//					arrWeakEle.eraseObject(pEle);
					CrushLayer::getInstance()->setEleIcon(posIndex.row, posIndex.column, nullptr);
				}
				curCount++;
			}
		}
	}

	return bRet;
}

bool Player::doAddEleLv(int lv, int count, bool isNeedAnim, Vec2 heartPos)
{
	bool bRet = false;
	int curCount = 0;
	int randomCount = 0;

	while (curCount < count && randomCount < 30)
	{
		int i = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
		int j = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);
		EleIcon *pEle = CrushLayer::getInstance()->getEleIcon(i, j);
		int eleId = pEle->getEleId();
		int eleLv = pEle->getEleLv();

		if (eleId >= EleIconId_E::ELE_ID_SWORD && eleId <= EleIconId_E::ELE_ID_HEART && 0 == eleLv)
		{
			bRet = true;
			if (isNeedAnim)
			{
				this->playLinkAnim(heartPos, pEle->getPosition());
				pEle->doAddLv(1, 1, 0.3f);
			}
			else
			{
				pEle->doAddLv(1, 1);
			}

			curCount++;
		}

		randomCount++;
	}

	return bRet;
}

void Player::getShield(const RoleCurAttInfo_T * pAttInfo)
{
	if (6 == pAttInfo->pSkillInfo->mark)
	{
		_arrRecoverInfo[3].round = pAttInfo->skillParam2;
		_arrRecoverInfo[3].param1 = pAttInfo->pSkillInfo->mark;
		_arrRecoverInfo[3].param2 = 0.f;

		if (nullptr == _arrRecoverInfo[3].pTag)
		{
			_arrRecoverInfo[3].pTag = RoundTag::createRoleBuff(_arrRecoverInfo[3].round, 3);
			GameLayer::getInstance()->addChild(_arrRecoverInfo[3].pTag, EntityZ_E::E_Z_UI);

			this->updateBuffTagPos();

			_pArmtrShield = GameArmtr::createSkill("skill_38");
			_pArmtrShield->setScale(0.8f);
			_pArmtrShield->play("skill_38");
			Vec2 shieldPos = CrushUtil::getShieldPos(FightLayer::getInstance()->getCurPlayerPosId());
			_pArmtrShield->setPosition(shieldPos);
			
			FightLayer::getInstance()->addChild(_pArmtrShield);
		}

		_arrRecoverInfo[3].pTag->setCurRound(_arrRecoverInfo[3].round);

// 		_shieldInfo.id = pAttInfo->pSkillInfo->mark;
// 		_shieldInfo.round = pAttInfo->skillParam2;
	}
}

void Player::getRelive(const RoleCurAttInfo_T * pAttInfo)
{
	if (44 == pAttInfo->skillId)
	{
		if (_arrRecoverInfo[2].round < 2)
		{
			_arrRecoverInfo[2].round += 1;
			_arrRecoverInfo[2].param1 = 44;

			if (nullptr == _arrRecoverInfo[2].pTag)
			{
				_arrRecoverInfo[2].pTag = RoundTag::createRoleBuff(_arrRecoverInfo[2].round, 2);
				GameLayer::getInstance()->addChild(_arrRecoverInfo[2].pTag, EntityZ_E::E_Z_UI);

				this->updateBuffTagPos();
			}

			_arrRecoverInfo[2].pTag->setCurRound(_arrRecoverInfo[2].round);
		}
		else
		{
			GameUtils::toastTip("relive_tip_0");
		}
	}
}

void Player::addRecover(int type, int round, float param)
{
	if (_arrRecoverInfo[type].round < round)
	{
		_arrRecoverInfo[type].round = round;
		_arrRecoverInfo[type].param2 = param;
	}

	if (nullptr == _arrRecoverInfo[type].pTag)
	{
		_arrRecoverInfo[type].pTag = RoundTag::createRoleBuff(round, type);
		GameLayer::getInstance()->addChild(_arrRecoverInfo[type].pTag, EntityZ_E::E_Z_UI);

		this->updateBuffTagPos();
	}

	_arrRecoverInfo[type].pTag->setCurRound(_arrRecoverInfo[type].round);
}

void Player::playStateAnim(const string &animName, int startIndex, int count)
{
	if (animName != "")
	{
		int totalCount = MIN(MAX(1, count), ParamData::ROLE_COUNT);

		int curCount = 0;
		_arrRoles[startIndex]->playStateAnim(animName);
		curCount++;

		for (int i = 0; i < ParamData::ROLE_COUNT && curCount < totalCount; i++)
		{
			if (i != startIndex)
			{
				auto pRole = _arrRoles[i];
				pRole->playStateAnim(animName);
				curCount++;
			}
		}
	}
}

void Player::doDead()
{
#if (1 == CC_ENABLE_DEBUG_MONSTER_ANIM)
	return;
#endif
	if (44 == _arrRecoverInfo[2].param1 && _arrRecoverInfo[2].round > 0)
	{
		this->doRelive(0);
	}
	else
	{
		if (0 == GameLayer::getInstance()->getFightType() || 2 == GameLayer::getInstance()->getFightType())
		{
			if (WordMap::getInstance()->checkIsBoss(GameLayer::getInstance()->getSceneId()))
			{
				if (_reliveTimes < 2)
				{
					_isWaitRelive = true;
					_reliveTimes++;
					GameLayer::getInstance()->popReborn();
				}
			}
			else if (ParamMgr::getInstance()->getShowReliveWeight() >= 4)
			{
				_isWaitRelive = true;
				ParamMgr::getInstance()->changeShowReliveWeight(2);
				GameLayer::getInstance()->popReborn();
			}
		}
		_isDead = true;

//			FightLayer::getInstance()->setGameState(3);
//		GameLayer::getInstance()->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {GameLayer::getInstance()->stopFight(); })));
//		GameLayer::getInstance()->stopFight();
//		GameMap::getCurGameMap()->showResault(false);
	}
}

void Player::doRecover()
{
	bool isBuffEnd = false;
	if (nullptr != _arrRecoverInfo[0].pTag)
	{
		this->addHp(_arrRecoverInfo[0].param2);
		this->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP, 3, 1);

		isBuffEnd = this->reduceBuffRound(0);
	}

	if (nullptr != _arrRecoverInfo[1].pTag)
	{
		this->addDef(_arrRecoverInfo[1].param2);
		this->playStateAnim(ArmtrName::ROLE_STATE_ADD_DEF, 1, 1);

		if (this->reduceBuffRound(1))
		{
			isBuffEnd = true;
		}
	}

	if (isBuffEnd)
	{
		this->updateBuffTagPos();
	}
}

void Player::doRelive(int reliveType)
{
	if (0 == reliveType)
	{
//		this->playStateAnim("buff_10");
		this->reduceBuffRound(2);

		this->addHp(_totalHp * 0.5f);
		this->addDef(_totalDef * 0.5f);
		this->playStateAnim("buff_10");
	}
	else if (1 == reliveType)
	{
		this->addHp(_totalHp);
		this->addDef(_totalDef);
		this->doClean(2, -1, 1);

		this->playStateAnim("buff_10");
	}
}


bool Player::isAttFinish()
{
	bool bRet = true;
	for (int i = 0; i < _arrRoles.size(); i++)
	{
		auto pRole = _arrRoles.at(i);
		if (!pRole->isAttFinish())
		{
			bRet = false;
			break;
		}
	}
	return bRet;
}

bool Player::isCurAttFinish()
{
	bool bRet = true;
	for (int i = 0; i < _arrRoles.size(); i++)
	{
		auto pRole = _arrRoles.at(i);
		if (!pRole->getIsCurAttFinish())
		{
			bRet = false;
			break;
		}
	}
	return bRet;
}

void Player::playMove()
{
	for (int i = 0; i < _arrRoles.size(); i++)
	{
		auto pRole = _arrRoles.at(i);
		pRole->playMove();
	}
}

void Player::playIdle()
{
	for (int i = 0; i < _arrRoles.size(); i++)
	{
		auto pRole = _arrRoles.at(i);
		pRole->playIdle();
	}
}

void Player::doStartDef()
{
	if (_curDef > 0.f && nullptr == _pArmtrShield)
	{
		_isDefing = true;
		_arrRoles[1]->playStartDefAnim();
	}
}

void Player::doEndDef()
{
	if (_isDefing)
	{
		_arrRoles[1]->playEndDefAnim();
		_isDefing = false;
	}
}

void Player::changeWeapon(int roleId, int weaponId)
{
	_arrRoles[roleId]->changeWeapon(weaponId);
}

const PlayerInfo_T * Player::getRoleInfo(int roleId)
{
	return &(_arrRoleInfo[roleId]);
}

int Player::getRoleSkillId(int roleId, int skillFlag)
{
	return _arrRoleSkillId[roleId][skillFlag];
}

Vec2 Player::getCurRolePos(int roleId)
{
	return _arrRoles[roleId]->getPosition();
}

Role * Player::getRole(int roleId)
{
	return _arrRoles[roleId];
}

void Player::reliveCB(bool isSuccess)
{
	_isWaitRelive = false;
	if (isSuccess)
	{
		_isDead = false;
		this->doRelive(1);
		FightLayer::getInstance()->setGameState(1);
	}
	else
	{
		//do nothing
	}
}

void Player::updateBuffTagPos()
{
	float posX1 = 218.f;
	float posX2 = 233.f;
	float posY1 = 50.f + ParamData::CRUSH_BG_HEIGHT;
	float posY2 = 15.f + ParamData::CRUSH_BG_HEIGHT;
	float offsetX = 40.f;

	if (nullptr != _arrRecoverInfo[2].pTag)
	{
		_arrRecoverInfo[2].pTag->setPosition(posX1, posY1);

		posX1 += offsetX;
	}

	if (nullptr != _arrRecoverInfo[0].pTag)
	{
		_arrRecoverInfo[0].pTag->setPosition(posX1, posY1);
	}

	if (nullptr != _arrRecoverInfo[3].pTag)
	{
		_arrRecoverInfo[3].pTag->setPosition(posX2, posY2);

		posX2 += offsetX;
	}

	if (nullptr != _arrRecoverInfo[1].pTag)
	{
		_arrRecoverInfo[1].pTag->setPosition(posX2, posY2);
	}
}

bool Player::reduceBuffRound(int buffIndex)
{
	bool bRet = false;

	if (_arrRecoverInfo[buffIndex].round > 0)
	{
		_arrRecoverInfo[buffIndex].round--;
		_arrRecoverInfo[buffIndex].pTag->setCurRound(_arrRecoverInfo[buffIndex].round);

		if (0 == _arrRecoverInfo[buffIndex].round)
		{
			_arrRecoverInfo[buffIndex].pTag->runAction(Sequence::create(DelayTime::create(0.4f), RemoveSelf::create(), nullptr));
//			_arrRecoverInfo[buffIndex].pTag->removeFromParent();

			_arrRecoverInfo[buffIndex].pTag = nullptr;
			_arrRecoverInfo[buffIndex].param1 = 0;
			_arrRecoverInfo[buffIndex].param2 = 0.f;
			_arrRecoverInfo[buffIndex].round = 0;

			if (3 == buffIndex)
			{
				_pArmtrShield->removeFromParent();
				_pArmtrShield = nullptr;
			}

			bRet = true;
		}
	}

	return bRet;
}

void Player::playLinkAnim(Vec2 srcPos, Vec2 desPos)
{
	float fLength = srcPos.distance(desPos);
	Sprite *pLink = Sprite::create("items/hg_dd1.png");
	float scaleX = fLength / pLink->getContentSize().width;
	Vec2 directV = desPos - srcPos;
	Vec2 lineV = Vec2(-1.f, 0.f);
	
	float angele = directV.getAngle(lineV);//lineV.getAngle(directV);
	
	pLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pLink->setRotation(CC_RADIANS_TO_DEGREES(angele));
	pLink->setPosition(srcPos);
	CrushLayer::getInstance()->addChild(pLink, ZInCrush_E::Z_C_EFFECT);

//	pLink->runAction(Sequence::create(ScaleTo::create(ParamData::ELE_CRUSH_WAIT, scaleX, 1.f), FadeOut::create(0.1f), RemoveSelf::create(), nullptr));
	pLink->runAction(Sequence::create(ScaleTo::create(0.3f, scaleX, 1.f), DelayTime::create(0.1f), FadeOut::create(0.1f), RemoveSelf::create(), nullptr));
}
