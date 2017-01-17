#include "Monster.h"
#include "ParamMgr.h"
#include "Armtr/GameArmtr.h"
#include "RoundTag.h"
#include "FightLayer.h"
#include "SpecialAttUtils.h"
#include "Player.h"
#include "GameCSLoader.h"
#include "CrushUtil.h"
#include "JRCommon.h"
#include "CrushLayer.h"
#include "DropItem.h"
#include "GameUtils.h"
#include "Scenes/GameMap.h"
#include "GameLayer.h"
#include "GLActions/GLActions.h"
#include "RoleDialog.h"
#include "GameResPool.h"
#include "AchieveMgr.h"
#include "BoxDataMgr.h"
#include "Scenes/PopRewardLayer.h"
#include "Scenes/MainScene.h"
#include "UserData.h"
#include "Defines.h"
#include "FightUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static int s_monsterCount = 0;
#endif

Monster::Monster()
	:_isDead(false),
	_isActive(false),
	_isAttFinish(true),
	_curAttCount(0),
	_isWithSkillAnim(false),
	_curShowWeakIndex(-1),
	_isLastMonster(false),
	_totalHp(0.f),
	_pDialog(nullptr),
	_isBoxChange(false)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	s_monsterCount++;
#endif
}

Monster::~Monster()
{
	_pArmtr->getArmtr()->getAnimation()->stop();
	_pArmtr->getArmtr()->getAnimation()->setMovementEventCallFunc(nullptr);
	_pArmtr->getArmtr()->getAnimation()->setFrameEventCallFunc(nullptr);

	if (1 == _pMonsterInfo->type && nullptr != GameLayer::getInstance())
	{
		GameLayer::getInstance()->setBossHp(0.f, 0.f);
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	s_monsterCount--;
	if (0 == s_monsterCount)
	{
		int a=0;
	}
#endif
}

Monster * Monster::create(int id, int posIndex)
{
	Monster *pRet = new Monster();

	if (nullptr != pRet && pRet->init(id, posIndex))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool Monster::init(int id, int posIndex)
{
	bool bRet = false;

	do 
	{
		_monsterId = id;
		_posIndex = posIndex;
		_pMonsterInfo = ParamMgr::getInstance()->getMonsterInfo(_monsterId);
		_curAttIndex = 0;
		_curRound = _pMonsterInfo->arrAttInfo.at(_curAttIndex).round;
#if (1 == CC_ENABLE_NEW_PARAM)
		_totalHp = FightUtil::calcMonsterHp(FightLayer::getInstance()->getDesignRoleInfo(), _pMonsterInfo->rAttMCount);
		_attDamage = FightUtil::calcMonsterAtt(FightLayer::getInstance()->getDesignRoleInfo(), _pMonsterInfo->mAttRCount);
		if (1 == GameLayer::getInstance()->getFightType())
		{
			int times = FightLayer::getInstance()->getEndlessFloor() / 10;

			_totalHp *= (1.f + 0.6f * times);
			_attDamage *= (1.f + 0.4f * times);
		}

		if ((0 == GameLayer::getInstance()->getFightType() || 2 == GameLayer::getInstance()->getFightType())
			&& 0 == _pMonsterInfo->type && 0 != GameLayer::getInstance()->getSceneId())
		{
			_curRound = ToolsUtil::getRandomInt(2, 4);
		}
#else
		_totalHp = CrushUtil::getMonsterHpValue(_pMonsterInfo->lv, Player::getInstance()->getBaseLv(), _pMonsterInfo->hp, _pMonsterInfo->type, GameLayer::getInstance()->getIsNeedGrow());
#endif
		_hp = _totalHp;
		_pMonsterAttArmInfo = ParamMgr::getInstance()->getMonsterAttArmInfo(_monsterId);

//		auto pArmtrInfo = ParamMgr::getInstance()->getMonsterArmtrInfo(_pMonsterInfo->animId);
		_pArmtr = GameArmtr::createMonster(_pMonsterInfo->animName);
		this->addChild(_pArmtr);
		if (2 == _pMonsterInfo->type)
		{
			_pArmtr->play("stand1");
		}
		else
		{
			_pArmtr->play(ArmtrName::MONSTER_IDLE);
		}

		float fScale = _pMonsterInfo->fScale * GameArmtr::ARMTR_SCALE_VALE;//CrushUtil::getMonsterScale(_monsterId);
		_pArmtr->getArmtr()->setScaleX(-fScale);
		_pArmtr->getArmtr()->setScaleY(fScale);

		float hpBarPosY = -10.f;
		_pHpBarNode = GameCSLoader::createNode("csb/monsterHp.csb");
		this->addChild(_pHpBarNode);
		_pHpBarNode->setPosition(Vec2(0.f, hpBarPosY));

		auto hpBar = static_cast<LoadingBar*>(_pHpBarNode->getChildByName("bar_hp"));
		hpBar->setPercent(100.f);

		if (1 == _pMonsterInfo->type)
		{
			GameLayer::getInstance()->setBossHp(_hp, 100.f);
		}
		_pHpBarNode->setVisible(false);

		_pRound = RoundTag::create(1, _curRound);
		_pRound->setPosition(Vec2(0.f, hpBarPosY-10.f));
		this->addChild(_pRound);

//		_pArmtr->getArmtr()->getCCEventDispatcher()->addCustomEventListener(EventData::COMPLETE, CC_CALLBACK_1(Monster::eventHandler, this));
		_pArmtr->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Monster::animationEvent, this));
		_pArmtr->getArmtr()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Monster::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

		auto pShadow = Sprite::createWithSpriteFrameName("lsj/lsj_touying_01.png");
		this->addChild(pShadow, -1);
		if (2 == _pMonsterInfo->type)
		{
			pShadow->setScaleX(fScale * 2.f);
		}
		else
		{
			pShadow->setScaleX(fScale * _pMonsterInfo->designSize);
		}
		pShadow->setOpacity(255 * 0.8f);

		this->schedule(CC_SCHEDULE_SELECTOR(Monster::updateWeakIcon), 1.f);

		bRet = true;
	} while (false);

	return bRet;
}

bool Monster::doRound()
{
	if (!_isDead)
	{
		if (!this->isDrug())
		{
			_curRound--;
			_pRound->setCurRound(_curRound);

			if (0 == _curRound)
			{
				bool bRet = true;
				auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
				int attAnimId = this->getAnimId(pCurAttInfo.attId);

				if (-2 == attAnimId)
				{
					bRet = false;
				}

				this->startAtt();

				return bRet;
			}
		}
	}

	return false;
}

void Monster::startAtt()
{
	this->initAttData();
	this->startAttAnim();
}

#if (CC_PLATFORM_IOS  == CC_TARGET_PLATFORM)
bool Monster::doHurtByRole(float damage, int attribute, long round, float attrParam)
#else
bool Monster::doHurtByRole(float damage, int attribute, int round, float attrParam)
#endif
{
	GameLayer::getInstance()->addCombo();

	if (Player::getInstance()->getAttBuff() > 0.f)
	{
		damage *= Player::getInstance()->getAttBuff();
	}

	return doHurt(damage, attribute, round, attrParam);
}

#if (CC_PLATFORM_IOS  == CC_TARGET_PLATFORM)
bool Monster::doHurt(float damage, int attribute, long round, float attrParam)
#else
bool Monster::doHurt(float damage, int attribute, int round, float attrParam)
#endif
{
	if (!_isDead)
	{
		if (2 == _pMonsterInfo->type && !_isBoxChange)
		{
			return false;
		}
		float factDamage = 0.f;
		float factAttrDamage = 0.f;
		auto pDefAttrbtInfo = this->getDefAttrbtInfo(attribute);

		if (nullptr == pDefAttrbtInfo)
		{
			factDamage = damage;
			factAttrDamage = attrParam;
		}
		else
		{
			if (AttAttrbt_E::ATT_DRUG == attribute)
			{
				round = MAX(0, round - pDefAttrbtInfo->hurt);
			}
			factDamage = damage * pDefAttrbtInfo->hurt;
			factAttrDamage = attrParam * pDefAttrbtInfo->hurt;

			int tipType = (pDefAttrbtInfo->hurt < 1.f) ? 2 : 1;
			FightLayer::getInstance()->showFightTips(tipType, pDefAttrbtInfo->defId, this->getPosition());
		}

		if (AttAttrbt_E::ATT_NORMAL != attribute && 0 != round && AttAttrbt_E::ATT_ABSORB_HP != attribute
			&& !(nullptr != pDefAttrbtInfo && AttAttrbt_E::ATT_DRUG == attribute && 0 == round))// && 0.f != pDefAttrbtInfo->hurt
		{
			bool isFound = false;
			for (int i = 0; i < _arrWeakInfo.size(); i++)
			{
				MonsterWeakInfo_T *pInfo = &(_arrWeakInfo.at(i));

				if (pInfo->attribute == attribute)
				{
					isFound = true;
// 					pInfo->round +=round;
// 					pInfo->curRound += round;
					pInfo->round = round;
					pInfo->curRound = round;
					pInfo->param = factAttrDamage;
					pInfo->pTag->setCurRound(pInfo->curRound);

					break;
				}
			}

			if (!isFound)
			{
				MonsterWeakInfo_T weakInfo = { 0 };

				weakInfo.attribute = attribute;
				weakInfo.curRound = round;
				weakInfo.round = round;
				weakInfo.param = factAttrDamage;
				weakInfo.pTag = RoundTag::createWeakIcon(2, round, attribute);
				weakInfo.pTag->setPosition(-30.f, _pArmtr->getArmtr()->getBoundingBox().getMaxY()+25.f);
				this->addChild(weakInfo.pTag);

				_arrWeakInfo.push_back(weakInfo);
			}
		}

		if (-101.f == attrParam && (AttAttrbt_E::ATT_NORMAL == attribute || AttAttrbt_E::ATT_ABSORB_HP == attribute))
		{
			float *pDamage = (float *)(round);
			*pDamage = MIN(factDamage, _hp);
		}

		if (AttAttrbt_E::ATT_NORMAL != attribute)
		{
			switch (attribute)
			{
			case AttAttrbt_E::ATT_FIRE:
				this->playStateAnim("buff_0");
				break;
			case AttAttrbt_E::ATT_THUNDER:
				this->playStateAnim("buff_1");
				break;
			case AttAttrbt_E::ATT_ICE:
				this->playStateAnim("buff_2");
				break;
			}
		}

		_hp -= factDamage;

		if (_hp < _totalHp && 1 != _pMonsterInfo->type)
		{
			_pHpBarNode->setVisible(true);
		}

		if (_hp <= 0.f)
		{
			_hp = 0.f;
		}

		if (1 != _pMonsterInfo->type)
		{
			auto hpBar = static_cast<LoadingBar*>(_pHpBarNode->getChildByName("bar_hp"));
			//hpBar->setPercent(_hp * 100.f / _totalHp);
			//zhang
			hpBar->stopAllActions();
			auto actionbar = GLProgressTo::create(0.25f, _hp * 100.f / _totalHp);
			hpBar->runAction(actionbar);
		}

		if (AttAttrbt_E::ATT_DRUG != attribute)
		{
			FightLayer::getInstance()->showValueChange(1, -factDamage, this->getPosition());
		}

		
		if (1 == _pMonsterInfo->type)
		{
			GameLayer::getInstance()->setBossHp(_hp, _hp * 100.f / _totalHp);
		}

		_pArmtr->play("hurt");
		if (_hp <= 0.f)
		{
			this->doDead();
			return true;
		}
	}

	return false;
}

void Monster::doDead()
{
	if (!_isDead)
	{
#if (1 == CC_ENABLE_NEW_PARAM)
		int exp = FightUtil::calcMonsterExp(_pMonsterInfo->designType, FightLayer::getInstance()->getDesignRoleInfo().perExp);
#else
		int exp = CrushUtil::getMonsterExpValue(_pMonsterInfo->lv, Player::getInstance()->getBaseLv(), _pMonsterInfo->exp, GameLayer::getInstance()->getIsNeedGrow());
#endif
		exp *= GameLayer::getInstance()->getExpTimes();
		FightLayer::getInstance()->setTotalExp(FightLayer::getInstance()->getTotalExp() + exp);
		_isDead = true;

		if (GameLayer::getInstance()->getExpTimes() > 1)
		{
			FightLayer::getInstance()->showFightTips(0, GameLayer::getInstance()->getExpTimes(), this->getPosition());
		}

		if (_isLastMonster && GameMap::getCurGameMap()->checkShowStory(2))
		{
			_pArmtr->play("stand");
		}
		else
		{
			_pArmtr->play("death");
		}

		if (1 == _pMonsterInfo->type)
		{
			AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::BOSS, 1);
		}

		Vec2 createPos = this->getPosition();
#if (1 == CC_ENABLE_NEW_PARAM)
		int goldCount = FightUtil::calcMonsterGold(_pMonsterInfo->designType, FightLayer::getInstance()->getDesignRoleInfo().perGold);
#else
		int goldCount = ToolsUtil::getRandomInt(_pMonsterInfo->goldRange[0], _pMonsterInfo->goldRange[1]);
		goldCount = CrushUtil::getMonsterGoldValue(_pMonsterInfo->lv, Player::getInstance()->getBaseLv(), goldCount, GameLayer::getInstance()->getIsNeedGrow());
#endif
		//		goldCount = 1;
		FightLayer::getInstance()->setTotalGold(FightLayer::getInstance()->getTotalGold() + goldCount);
		while (goldCount > 0)
		{
			int itemId = 0;
			if (goldCount > 100)
			{
				itemId = 0;
				goldCount -= 100;
			}
			else if (goldCount > 10)
			{
				itemId = 1;
				goldCount -= 10;
			}
			else
			{
				itemId = 2;
				goldCount -= 1;
			}

			float delayTime = ToolsUtil::getRandomFloat(0.f, 0.5f);
			FightLayer::getInstance()->runAction(Sequence::create(DelayTime::create(delayTime), 
				CallFunc::create([=] {
				auto pItem = DropItem::create(itemId);
				Vec2 rolePos = Player::getInstance()->getCurRolePos(pItem->getPosId());
				pItem->setPosition(createPos.x, rolePos.y+15.f);
				FightLayer::getInstance()->addChild(pItem);
			}), nullptr));
		}

		this->dropBoxItem();
	}
}

void Monster::addHP(float addValue)
{
#if (1 == CC_ENABLE_NEW_PARAM)
	float addPerc = 1 / 20.f;
	addValue = _totalHp * addPerc;
#endif
	_hp = MIN(_totalHp, _hp+addValue);

	auto hpBar = static_cast<LoadingBar*>(_pHpBarNode->getChildByName("bar_hp"));
	hpBar->setPercent(_hp * 100.f / _totalHp);

	this->playStateAnim(ArmtrName::ROLE_STATE_ADD_HP);
	FightLayer::getInstance()->showValueChange(1, addValue, this->getPosition());	

	if (1 == _pMonsterInfo->type)
	{
		GameLayer::getInstance()->setBossHp(_hp, _hp * 100.f / _totalHp);
	}
	//unfinished
	//this->playStateAnim(animName);
}

void Monster::updateWeakIcon(float dt)
{
	if (!_arrWeakInfo.empty())
	{
		for (int i = _arrWeakInfo.size() - 1; i >= 0; i--)
		{
			MonsterWeakInfo_T *pInfo = &(_arrWeakInfo[i]);
			
			if (0 == pInfo->curRound)
			{
//				pInfo->pTag->removeFromParent();
				pInfo->pTag->runAction(Sequence::createWithTwoActions(DelayTime::create(0.4f), RemoveSelf::create()));
				pInfo->pTag = nullptr;
				
				_arrWeakInfo.erase(_arrWeakInfo.begin()+i);
			}
		}

		if (!_arrWeakInfo.empty())
		{
			if (_curShowWeakIndex >= 0 && _curShowWeakIndex < _arrWeakInfo.size())
			{
				_arrWeakInfo.at(_curShowWeakIndex).pTag->setVisible(false);
			}
			_curShowWeakIndex = (_curShowWeakIndex + 1) % _arrWeakInfo.size();
			_arrWeakInfo.at(_curShowWeakIndex).pTag->setVisible(true);
		}
	}
}

void Monster::animationEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		string subMoveId = movementID.substr(0, 6);
		if (movementID == ArmtrName::MONSTER_CLOSE_ATT || movementID == ArmtrName::MONSTER_REMOTE_ATT)
		{
			this->endAttAnim();
		}
		else if (subMoveId == "attack")
		{
			if (_isWithSkillAnim)
			{
				_isWithSkillAnim = false;
				this->endAttAnim();
			}
			else
			{
				_pArmtr->play(ArmtrName::MONSTER_IDLE);
			}
		}
		else if (movementID == "death" || movementID == "xiaoshi")
		{
			FightLayer::getInstance()->removeMonster(this);
			this->removeFromParentAndCleanup(false);
		}
		else if (movementID == "hurt")
		{
			if (_isAttFinish && !_isDead)
			{
				_pArmtr->play(ArmtrName::MONSTER_IDLE);
			}
		}
		else if (movementID == "bianshen")
		{
			_isBoxChange = true;
			_curRound = 0;
			this->doEndAtt();
			_pArmtr->play("stand");
		}
	}
}

void Monster::onFrameEvent(cocostudio::Bone * bone, const std::string & evt, int originFrameIndex, int currentFrameIndex)
{
	if (evt == ArmtrName::MONSTER_EVT_HURT)
	{
		//		this->playSkillAnim();
		this->doAttPer();
	}
	else if (evt == ArmtrName::MONSTER_EVT_SKILL)
	{
		this->playSkillAnim();
	}
	else if (evt == ArmtrName::MONSTER_EVT_SKILL_HURT)
	{
		_isWithSkillAnim = true;
		this->doAttPer();
	}
}
void Monster::animationSkillEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
		auto pAttArmInfo = _pMonsterAttArmInfo->at(pCurAttInfo.attId);
		if (movementID == pAttArmInfo.skillAnim)
		{
			this->endAttAnim();
//			armature->removeFromParentAndCleanup(false);
			armature->getParent()->removeFromParentAndCleanup(false);

			CC_SAFE_RELEASE(this);
		}
	}
}
void Monster::onSkillFrameEvent(cocostudio::Bone * bone, const std::string & evt, int originFrameIndex, int currentFrameIndex)
{
	if (evt == ArmtrName::MONSTER_EVT_SKILL_HURT)
	{
		this->doAttPer();
	}
}


void Monster::playStateAnim(const std::string& animName)
{
	auto pAnim = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_BUFF));//GameArmtr::createBuff(animName);
	this->addChild(pAnim);
	pAnim->play(animName);

	pAnim->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Monster::animationStateEvent, this));
}


void Monster::animationStateEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	//unfinished
	if (movementType == COMPLETE)
	{
		GameResPool::getInstance()->recycleRes(armature->getParent());

		armature->getParent()->removeFromParentAndCleanup(false);
	}
}
/*
void Monster::eventHandler(cocos2d::EventCustom *event)
{
	EventData *eventData = static_cast<EventData*>(event->getUserData());

	if (nullptr != eventData->animationState)
	{
		if (0 == EventData::COMPLETE.compare(eventData->getStringType()))
		{
			if (0 == eventData->animationState->name.compare("atk"))
			{
				if (0 == _curRound)
				{
					auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);

					this->doAtt(_curAttIndex);
					if (-1 != pCurAttInfo.next)
					{
						_curAttIndex = pCurAttInfo.next;
					}
					else
					{
						_curAttIndex = ((_curAttIndex + 1) == _pMonsterInfo->arrAttInfo.size()) ? 0 : (_curAttIndex + 1);
					}
					auto pNextAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
					_curRound = pNextAttInfo.round;

					_pRound->setCurRound(_curRound);
				}
			}
		}
	}
}*/

const DefAttrbt_T * Monster::getDefAttrbtInfo(int skillAttrbtId)
{
	const DefAttrbt_T *pRet = nullptr;
	for (int i = 0; i < _pMonsterInfo->arrDefInfo.size(); i++)
	{
		const DefAttrbt_T *pTmp = &(_pMonsterInfo->arrDefInfo.at(i));

		if (pTmp->defId == skillAttrbtId)
		{
			pRet = pTmp;
			break;
		}
	}

	return pRet;
}

void Monster::initAttData()
{
	auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
	
	_isWithSkillAnim = false;
	_isAttFinish = false;
	_curAttCount = 0;
	_oldPos = this->getPosition();
}

void Monster::startAttAnim()
{
	auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
//	auto pAttArmInfo = _pMonsterAttArmInfo->at(pCurAttInfo.attId);
	int attAnimId =	this->getAnimId(pCurAttInfo.attId);	

	if (-1 == attAnimId)
	{
		_pArmtr->play("bianshen");
	}
	else if (-2 == attAnimId)
	{
		_pArmtr->play("death");
	}
	else
	{
		auto pAttArmInfo = _pMonsterAttArmInfo->at(attAnimId);
		auto strRoleAnimName = String::createWithFormat("%s", pAttArmInfo.roleAct.c_str());
		//	auto strRoleAnimName = __String::createWithFormat("attack%d", attAnimId);

		if (!this->isAttNear(pCurAttInfo.attId))
		{
			_pArmtr->play(strRoleAnimName->getCString());
		}
		else
		{
			if (0 == _curAttCount)
			{
				Vec2 desPos = CrushUtil::getMonsterAttPos(FightLayer::getInstance()->getCurPlayerPosId(), _posIndex, _waveCount);//CrushUtil::getRoleFrontPos(FightLayer::getInstance()->getCurPlayerPosId());
				std::string roleAnim = strRoleAnimName->getCString();

				_pArmtr->play("walk");
				this->runAction(Sequence::create(MoveTo::create(0.3f, desPos), CallFunc::create([=] {_pArmtr->play(roleAnim); }), nullptr));
			}
			else
			{
				_pArmtr->play(strRoleAnimName->getCString());
			}
		}
	}
}

void Monster::doAttPer()
{
	float damage = 0.f;
	auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);


#if (1 == CC_ENABLE_NEW_PARAM)
	damage = _attDamage;
	//boss kill
	if (201 == pCurAttInfo.attId)
	{
		damage *= 1.5f;
	}
#else
	damage = ToolsUtil::getRandomFloat(_pMonsterInfo->attRange[0], _pMonsterInfo->attRange[1]);
	//boss kill
	if (201 == pCurAttInfo.attId)
	{
		damage *= 1.5f;
	}
	damage = CrushUtil::getMonsterAttValue(_pMonsterInfo->lv, Player::getInstance()->getBaseLv(), damage, _pMonsterInfo->type, GameLayer::getInstance()->getIsNeedGrow());
#endif

	if (pCurAttInfo.attId > 0 && pCurAttInfo.attId < 200)
	{
		int skillId = (pCurAttInfo.attId % 100) - 1;

		if (skillId >= 0)
		{
			CrushLayer::getInstance()->createSpecialEle(skillId, pCurAttInfo.attCount);
		}
	}
	else if (pCurAttInfo.attId >= 300)
	{
		int skillId = (pCurAttInfo.attId % 100);
		CrushLayer::getInstance()->createWeakEle(skillId, pCurAttInfo.attCount);
	}

	Player::getInstance()->doHurtByMonster(AttAttrbt_E::ATT_NORMAL, damage, _pMonsterInfo->lv);
	
	int soundId = this->getAnimId(pCurAttInfo.attId);
	auto pAttArmInfo = _pMonsterAttArmInfo->at(soundId);
	if (!pAttArmInfo.soundName.empty())
	{
		GameUtils::playEffect(pAttArmInfo.soundName.c_str());
	}
}

void Monster::endAttAnim()
{
	if (_isDead)
	{
		CCASSERT(false, "endAttAnim _isDead");
		return;
	}
	auto curAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
	if (0 != curAttInfo.attId && 100 != curAttInfo.attId)
	{
		curAttInfo.attCount = 1;
	}
	_curAttCount++;

	if (_curAttCount == curAttInfo.attCount)
	{
		if (this->isAttNear(curAttInfo.attId))
		{
			_pArmtr->play("walk");
			this->runAction(Sequence::create(MoveTo::create(0.3f, _oldPos), CallFunc::create([=] {this->doEndAtt(); _pArmtr->play("stand"); }), nullptr));
		}
		else
		{
			this->doEndAtt();
			_pArmtr->play("stand");
		}
	}
	else
	{
		this->startAttAnim();
	}
}

void Monster::playSkillAnim()
{
	auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
	auto pAttArmInfo = _pMonsterAttArmInfo->at(pCurAttInfo.attId);

	if (pAttArmInfo.skillAnim != "")
	{
		Vec2 desPos = CrushUtil::getMonsterSkillPos(FightLayer::getInstance()->getCurPlayerPosId(), _posIndex, _waveCount);//CrushUtil::getRoleFrontPos(FightLayer::getInstance()->getCurPlayerPosId());
		auto pSkill = GameArmtr::createSkill(pAttArmInfo.skillAnim);
		FightLayer::getInstance()->addChild(pSkill);
		pSkill->setPosition(desPos);
		pSkill->play(pAttArmInfo.skillAnim);

		CC_SAFE_RETAIN(this);
		pSkill->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Monster::animationSkillEvent, this));
		pSkill->getArmtr()->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Monster::onSkillFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}
}

void Monster::doEndAtt()
{
	_isWithSkillAnim = false;
	_isAttFinish = true;
	_curAttCount = 0;
	if (0 == _curRound)
	{
		auto pCurAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);

		if (-1 != pCurAttInfo.next)
		{
			_curAttIndex = pCurAttInfo.next;
		}
		else
		{
			_curAttIndex = ((_curAttIndex + 1) == _pMonsterInfo->arrAttInfo.size()) ? 0 : (_curAttIndex + 1);
		}
		auto pNextAttInfo = _pMonsterInfo->arrAttInfo.at(_curAttIndex);
		_curRound = pNextAttInfo.round;

		_pRound->setCurRound(_curRound);
	}
}

bool Monster::isAttNear(int attId)
{
	int tmp = attId / 100;

	return (0 == tmp || 3 == tmp);
}

bool Monster::isDrug()
{
	for (int i = _arrWeakInfo.size() - 1; i >= 0; i--)
	{
		MonsterWeakInfo_T *pInfo = &(_arrWeakInfo.at(i));
		
		if (AttAttrbt_E::ATT_DRUG == pInfo->attribute && pInfo->curRound > 0)
		{
			return true;
		}
	}
	return false;
}

int Monster::getAnimId(int attId)
{
	int animId = attId;

	if (attId >= 300 && attId < 400)
	{
		animId = 0;
	}
	else if (attId >= 400 && attId < 500)
	{
		animId = 100;
	}

	return animId;
}

void Monster::doWeakPer()
{
	if (!_isDead)
	{
		for (int i = _arrWeakInfo.size() - 1; i >= 0; i--)
		{
			MonsterWeakInfo_T *pInfo = &(_arrWeakInfo.at(i));

			pInfo->curRound--;
			pInfo->pTag->setCurRound(pInfo->curRound);

			if (AttAttrbt_E::ATT_FIRE == pInfo->attribute || AttAttrbt_E::ATT_THUNDER == pInfo->attribute || AttAttrbt_E::ATT_ICE == pInfo->attribute)
			{
				this->doHurt(pInfo->param, pInfo->attribute, 0, 0.f);
			}
			else if (AttAttrbt_E::ATT_CONFUSE == pInfo->attribute)
			{
			}
			else if (AttAttrbt_E::ATT_NARROW == pInfo->attribute)
			{
			}
		}
	}
}

void Monster::startTalk(const std::string & talkId)
{
	if (nullptr == _pDialog)
	{
		_pDialog = RoleDialog::create(talkId, -1);
		_pDialog->setPosition(Vec2(-150.f, 100.f));
		this->addChild(_pDialog);
	}
}

void Monster::stopTalk()
{
	if (nullptr != _pDialog)
	{
		_pDialog->removeFromParent();
		_pDialog = nullptr;
	}
}
/*
void Monster::onFightEvent()
{
	if (2 == _pMonsterInfo->type && 0 == _curAttIndex)
	{
		int round = _pMonsterInfo->arrAttInfo.at(0).round;
		SpecialAttUtils::createRandomKey(round);
	}
}*/

void Monster::openBox()
{
	if (2 == _pMonsterInfo->type)
	{
		_pArmtr->play("xiaoshi");
		_isDead = true;
		this->dropBoxItem();
	}
}

void Monster::dropBoxItem()
{
	int param = 0;
	if (2 == _pMonsterInfo->type)
	{
		int boxId = 0;
		int boxLv = 0;
		if (1 == GameLayer::getInstance()->getFightType())
		{
			int openBoxCount = FightLayer::getInstance()->getOpenBoxCount();
			boxId = 0;
			boxLv = BoxDataMgr::getInstance()->getCurGetBoxLv();
			if (0 == openBoxCount)
			{
				boxId = 0;
				//			BoxDataMgr::getInstance()->getBoxReward(0, boxLv, arrItems);
			}
			else if (1 == openBoxCount)
			{
				boxId = 1;
				//			BoxDataMgr::getInstance()->getBoxReward(1, boxLv, arrItems);
			}
			else
			{
				boxId = 2;
				boxLv += (openBoxCount - 2);
				boxLv = MIN(4, boxLv);
				//			BoxDataMgr::getInstance()->getBoxReward(2, boxLv, arrItems);
			}
			FightLayer::getInstance()->setOpenBoxCount(openBoxCount + 1);

			param = 4;
			GameLayer::getInstance()->pauseGame();
		}
		else if (0 == GameLayer::getInstance()->getFightType())
		{
			auto pStageInfo = FightLayer::getInstance()->getStageInfo();
			boxId = pStageInfo->boxmonsBoxid;
			boxLv = 0;

			UserData::getInstance()->setBoxGet(GameLayer::getInstance()->getSceneId(), 1);
			WordMap::getInstance()->updateBoxGetInfo(WordMap::getInstance()->getCurPosIdx());
		}

		auto layer = PopRewardLayer::openBox(boxId, boxLv, param);//PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
	}
}

void Monster::setIsActive(bool isActive)
{
	if (_isActive != isActive)
	{
		if (!_isActive)
		{
			if (2 == _pMonsterInfo->type && 0 == _curAttIndex)
			{
				int round = _pMonsterInfo->arrAttInfo.at(0).round;
				SpecialAttUtils::createRandomKey(round);
			}
		}
		_isActive = isActive;
	}
}

bool Monster::getIsActive()
{
	return _isActive;
}