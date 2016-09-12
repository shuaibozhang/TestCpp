#include "EleIcon.h"
#include "CrushLayer.h"
#include "ParamMgr.h"
#include "RoundTag.h"
#include "JRCommon.h"
#include "SpecialAttUtils.h"
#include "CrushUtil.h"
#include "GameUtils.h"
#include "Armtr/GameArmtr.h"
#include "FlyWeak.h"
#include "FightLayer.h"
#include "Player.h"
#include "GameResPool.h"
#include "Role.h"
#include "UserData.h"
#include "GameLayer.h"
#include "Defines.h"
#include "FightUtil.h"


EleIcon::EleIcon()
	:_pRound(nullptr),
	_curRound(-1),
	_pSpecInfo(nullptr),
	_pWeakInfo(nullptr),
	_pWeakSprite(nullptr),
	_curCrushCount(-1),
	_stoneEleId(-1),
	_isCanTouch(true),
	_pModel(nullptr),
	_totalRound(0),
	_pEleBg(nullptr),
	_pArrowTip(nullptr)
{

}

EleIcon::~EleIcon()
{
	if (_eleId >= EleIconId_E::ELE_ID_SWORD && _eleId <= EleIconId_E::ELE_ID_SUPER)
	{
		GameResPool::getInstance()->recycleRes(GameResId_E::RES_ELE_ICON, _pModel);
	}
	else
	{
		GameResPool::getInstance()->recycleRes(GameResId_E::RES_SPECIAL, _pModel);
	}
	_pModel = nullptr;

	if (nullptr != _pEleBg)
	{
		_pEleBg->removeFromParent();
		_pEleBg = nullptr;
	}

	if (nullptr != _pArrowTip)
	{
		_pArrowTip->removeFromParent();
		_pArrowTip = nullptr;
	}
}

EleIcon * EleIcon::create(int eleId, int eleLv)
{
	EleIcon *pRet = new EleIcon();

	if (nullptr != pRet && pRet->init(eleId, eleLv))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EleIcon::init(int eleId, int eleLv)
{
	bool bRet = false;

	do 
	{
		_eleId = eleId;
		_eleLv = eleLv;
		
		__String *strFileName = nullptr;
		if (_eleId <= EleIconId_E::ELE_ID_SUPER)
		{
			_pModel = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_ELE_ICON));//GameArmtr::createEleIcon();
			strFileName = String::createWithFormat("idle_%d_%d", _eleId, _eleLv + 1);
			_pModel->play(strFileName->getCString()); 
			_pModel->setPosition(Vec2::ZERO);
			this->addChild(_pModel);

			if (CrushLayer::getInstance()->isStoneState(_eleId))
			{
				this->obtainWeak(3);
			}
		}
		else
		{
			_pSpecInfo = ParamMgr::getInstance()->getEleSpecInfo(_eleId);
			strFileName = String::createWithFormat("daiji_%d_0", _eleId-EleIconId_E::ELE_ID_SUPER-1);
			_pModel = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_SPECIAL)); //GameArmtr::createSpecial();
			_pModel->play(strFileName->getCString());
			_pModel->setPosition(Vec2::ZERO);
			this->addChild(_pModel);

			this->setTotalRound(_pSpecInfo->totalRound);
			_curCrushCount = 0;
			if (-1 != _curRound)
			{
				_pRound = RoundTag::create(0, _curRound);
				_pRound->setPosition(12.f, -14.f);
				this->addChild(_pRound);
			}

			if (2 == _pSpecInfo->attTime)
			{
				if (_pSpecInfo->type == EleSpecialType::STONE)
				{
					GameUtils::playEffect("shihua.ogg");
					_stoneEleId = SpecialAttUtils::createStoneState();
				}
			}

			if (this->isNeedAddWeakArr())
			{
				CrushLayer::getInstance()->addWeakEle(this);
			}

			if (_pSpecInfo->type == EleSpecialType::STONE || _pSpecInfo->type == EleSpecialType::THUNDER || _pSpecInfo->type == EleSpecialType::RECOVER)
			{
				if (!UserData::getInstance()->getIsTipsGuided(5))
				{
					auto pRole = Player::getInstance()->getRole(0);
					pRole->startTalk("guide_tip_5");
					Player::getInstance()->runAction(Sequence::create(DelayTime::create(3.f), CallFunc::create([=] {pRole->stopTalk(); }), nullptr)); 
					UserData::getInstance()->setIsTipsGuided(5, true);
				}
			}
		}
		this->updateEleBg();

		this->playCreateEffect();

		bRet = true;
	} while (false);

	return bRet;
}

void EleIcon::doShow()
{

}

bool EleIcon::doCrush()
{
	bool bRet = false;

	if (nullptr != _pSpecInfo)
	{
		return false;
	}
	
	bool isStone = false;
	if (nullptr != _pWeakInfo && _pWeakInfo->type == EleWeakType::STONE)
	{
		isStone = true;
	}

	if (nullptr != _pWeakInfo)
	{
		if (_pWeakInfo->isCanCrush)
		{
			this->removeWeak();
		}
		else if (_pWeakInfo->isCanCrushClean)
		{
			//do clean
			this->doClean(_pWeakInfo->lv);
			return false;
		}
	}

	float dur = this->playCrushAnim(isStone);
	this->doRemove(dur);

	return true;
}

bool EleIcon::doClean(int lv, float animDelay)
{
	if (nullptr != _pWeakInfo)
	{
		if (lv >= _pWeakInfo->lv)
		{
			this->playCleanAnim(animDelay);

			if (lv == _pWeakInfo->lv)
			{
				_curCrushCount++;
			}
			else
			{
				_curCrushCount += (lv - _pWeakInfo->lv + 1);
				_curCrushCount = (_curCrushCount > _pWeakInfo->totalCrush) ? _pWeakInfo->totalCrush : _curCrushCount;
			}

			if (_curCrushCount >= _pWeakInfo->totalCrush)
			{
				this->removeWeak();
				return true;
			}
			else
			{
				this->updateWeakSprite();
			}
		}
	}
	else if (nullptr != _pSpecInfo)
	{
		if (lv >= _pSpecInfo->lv)
		{
			float dur = animDelay;// 0.f;
			if (_pSpecInfo->type == EleSpecialType::KEY)
			{
				this->playCleanAnim(animDelay, 3);
			}
			else
			{
				this->playCleanAnim(animDelay);
			}

			if (lv == _pSpecInfo->lv)
			{
				_curCrushCount++;
			}
			else
			{
				_curCrushCount += (lv - _pSpecInfo->lv + 1);
				_curCrushCount = (_curCrushCount > _pSpecInfo->totalCrush) ? _pSpecInfo->totalCrush : _curCrushCount;
			}

			if (_pSpecInfo->isNeedChangeAnim)
			{
				this->updateSpecSprite();
			}

			if (_curCrushCount >= _pSpecInfo->totalCrush)
			{
				if (2 == _pSpecInfo->attTime)
				{
					if (_pSpecInfo->type == EleSpecialType::STONE)
					{
						SpecialAttUtils::cleanStoneState(_stoneEleId);
					}
				}
				else if (3 == _pSpecInfo->attTime)
				{
					this->doSpecAtt();
				}

				this->removeSpec();
				this->doRemove(dur);
				return true;
			}
		}
	}

	return false;
}

bool EleIcon::doRound()
{
	if (nullptr != _pWeakInfo)
	{
		if (_pWeakInfo->type == EleWeakType::FIRE)
		{
			//unfinished
//			this->playRoundEffect();
			int roleId = _eleId - EleIconId_E::ELE_ID_SWORD;
			roleId = (roleId >= 0 && roleId <= 3) ? roleId : 0;
#if (1 == CC_ENABLE_NEW_PARAM)
			float param = FightUtil::calcWeakDamageValue(0, FightLayer::getInstance()->getDesignRoleInfo());
#else
			float param = CrushUtil::getWeakDamageValue(0, Player::getInstance()->getBaseLv(), 10.f);
#endif
			this->createFlyWeak(0, roleId, param);
		}
	}
	else if (nullptr != _pSpecInfo)
	{
		if (-1 != _totalRound)//_pSpecInfo->totalRound)
		{
			_curRound--;
			if (nullptr != _pRound)
			{
				_pRound->setCurRound(_curRound);
			}
			//writing
		}

		if (0 == _pSpecInfo->attTime || (1==_pSpecInfo->attTime && 0==_curRound))
		{
			this->playRoundEffect();
			this->doSpecAtt();
		}

		if (0 == _curRound)
		{
			this->removeSpec();
			this->doRemove(0.f);
			return true;
		}
	}

	return false;
}

bool EleIcon::doInBottom()
{
	if (this->isNeedDealBottom())
	{
		return this->doClean(999);
	}

	return false;
}

bool EleIcon::doHit()
{
	if (nullptr != _pSpecInfo && !_pSpecInfo->isCanFall && _pSpecInfo->type != EleSpecialType::SLATE)
	{
		return this->doClean(_pSpecInfo->lv);
	}
	return false;
}

void EleIcon::doAddLv(int type, int lv, float animDelay)
{
	if (nullptr != _pWeakInfo && 0 == type)
	{
		if (_pWeakInfo->isCanCrush)
		{
			this->removeWeak();
		}
	}

	if (-1 == lv)
	{
		_eleLv++;
	}
	else
	{
		_eleLv = lv;
		if (3 == _eleLv)
		{
			_eleLv = 0;
			_eleId = EleIconId_E::ELE_ID_SUPER;
		}
	}

	if (_eleLv < 3)
	{
		if (animDelay > 0.f)
		{
			this->runAction(Sequence::create(DelayTime::create(animDelay), CallFunc::create([=] {
				this->updateModelSprite();
				this->playCreateEffect(); }), nullptr));
		}
		else
		{
			this->updateModelSprite();
			this->playCreateEffect();
		}
	}
	this->updateEleBg();
}

void EleIcon::playMovePerUnit(int fallType, float delayTime, float durTime, int dstRow, int dstColum)
{
	Vec2 dstPos = CrushUtil::getElePos(dstRow, dstColum);
	
	if (-1 == fallType)
	{
		if (delayTime > 0.f)
		{
			this->runAction(Sequence::create(DelayTime::create(delayTime), MoveTo::create(durTime, dstPos), nullptr));
		}
		else
		{
			this->runAction(MoveTo::create(durTime, dstPos));
		}
	}
	else if (0 == fallType || 1 == fallType || 2 == fallType)
	{
		if (dstRow == (ParamData::CRUSH_ROW - 1))
		{
			this->setPosition(dstPos);
			_pModel->setVisible(false);
//			if (delayTime > 0.f)
			{
				_pModel->runAction(Sequence::create(DelayTime::create(delayTime + durTime), Show::create(), nullptr));
			}
//			else
//			{
//				_pModelEx->runAction(FadeIn::create(durTime));
//			}
		}
		else if (dstRow < (ParamData::CRUSH_ROW - 1))
		{
			if (delayTime > 0.f)
			{
				this->runAction(Sequence::create(DelayTime::create(delayTime), MoveTo::create(durTime, dstPos), nullptr));
			}
			else
			{
				this->runAction(MoveTo::create(durTime, dstPos));
			}
		}
		else
		{
			_pModel->setVisible(false);
		}
	}
}

bool EleIcon::obtainWeak(int weakId)
{
	auto pNewWeakInfo = ParamMgr::getInstance()->getWeakInfo(weakId);
	
	if (_eleId < EleIconId_E::ELE_ID_SWORD || _eleId > EleIconId_E::ELE_ID_HEART)
	{
		return false;
	}

	if (nullptr != _pWeakInfo && _pWeakInfo->weight >= pNewWeakInfo->weight)
	{
		return false;
	}

	if (this->isNeedAddWeakArr())
	{
		CrushLayer::getInstance()->removeWeakEle(this);
	}
	_pWeakInfo = pNewWeakInfo;
//	_curWeakLv = _pWeakInfo->lv;
	_curCrushCount = 0;// _pWeakInfo->totalCrush;
	if (_pWeakInfo->frameCount > 0)
	{
		__String *strFileName = __String::createWithFormat("%s_%d.png", _pWeakInfo->animName.c_str(), _pWeakInfo->totalCrush-1);
		if (nullptr == _pWeakSprite)
		{
			_pWeakSprite = Sprite::createWithSpriteFrameName(strFileName->getCString());
			this->addChild(_pWeakSprite);
		}
		else
		{
			_pWeakSprite->setSpriteFrame(strFileName->getCString());
		}

		if (_pWeakInfo->frameCount > 1)
		{
			auto anim = ToolsUtil::getOrCreateAnimation(_pWeakInfo->animName, _pWeakInfo->frameCount);
			_pWeakSprite->runAction(RepeatForever::create(Animate::create(anim)));
		}
		else
		{
			_pWeakSprite->stopAllActions();
		}
	}
	else
	{
		/*stone*/
		auto strAnimName = String::createWithFormat("shihua_%d", _eleId);
		_pModel->play(strAnimName->getCString());

		if (nullptr != _pWeakSprite)
		{
			_pWeakSprite->removeFromParent();
			_pWeakSprite = nullptr;
		}
	}

	if (1 == weakId || 2 == weakId)
	{
		auto pEffect = GameArmtr::createSpecialEx();
		pEffect->play("fadong_2_6");
		this->addChild(pEffect, 2);

		pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EleIcon::animationEffectEvent, this));
	}

	if (this->isNeedAddWeakArr())
	{
		CrushLayer::getInstance()->addWeakEle(this);
	}

	return true;
}

void EleIcon::doCleanStone()
{
	if (nullptr != _pWeakInfo && _pWeakInfo->type == EleWeakType::STONE)
	{
		this->doClean(_pWeakInfo->lv);
	}
}

void EleIcon::doRemove(float delayTime, bool isCleanup)
{
//	this->removeWeak();
//	this->removeSpec();
	if (delayTime > 0.f)
	{
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(delayTime), CallFunc::create([=] {this->removeFromParentAndCleanup(isCleanup); })));		
	}
	else
	{
		this->removeFromParentAndCleanup(isCleanup);
	}
}

void EleIcon::createFlyWeak(int weakId, int roleId, float param)
{
	auto pFlyIcon = FlyWeak::create(weakId, roleId, param);

	Vec2 worldPos = this->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 iconPos = FightLayer::getInstance()->convertToNodeSpace(worldPos);
	pFlyIcon->setPosition(iconPos);

	FightLayer::getInstance()->addChild(pFlyIcon, ZInFight_E::Z_F_FLY_ICON);
}

bool EleIcon::isCrushEffective()
{
	if (nullptr != _pWeakInfo)
	{
		return _pWeakInfo->isHostAvail;
	}

	return true;
}

bool EleIcon::isNeedDealBottom()
{
	bool bRet = false;

	if (nullptr != _pSpecInfo)
	{
		bRet = _pSpecInfo->isNeedDealBottom;//(-1 == _pSpecInfo->totalCrush);
	}

	return bRet;
}

bool EleIcon::isCanFall()
{
	bool bRet = true;

	if (nullptr != _pSpecInfo)
	{
		bRet = _pSpecInfo->isCanFall;
	}
	else if (nullptr != _pWeakInfo)
	{
		bRet = _pWeakInfo->isCanFall;
	}

	return bRet;
}

bool EleIcon::isCanClean(int lv)
{
	if (nullptr != _pWeakInfo)
	{
		return lv >= _pWeakInfo->lv;
	}
	else if (nullptr != _pSpecInfo)
	{
		return lv >= _pSpecInfo->lv;
	}

	return false;
}

float EleIcon::playCrushAnim(bool isStone)
{
	float dur = ParamData::ELE_CRUSH_DUR;

	if (_eleId <= EleIconId_E::ELE_ID_SUPER)
	{
		if (isStone)
		{
			auto strAnimName = String::createWithFormat("shihua_miss_%d", _eleId);
			_pModel->play(strAnimName->getCString());
		}
		else
		{
			auto strAnimName = String::createWithFormat("miss_%d_%d", _eleId, _eleLv + 1);
			_pModel->play(strAnimName->getCString());
		}
	}
	else
	{
		CCASSERT(false, "playCrushAnim");
	}

	return dur;
}

float EleIcon::playCleanAnim(float animDelay, int animIndex)
{
	if (0.f == animDelay)
	{
		this->playBuffEffect(animIndex);
	}
	else
	{
		this->runAction(Sequence::create(DelayTime::create(animDelay), CallFunc::create([=] {this->playBuffEffect(animIndex); }), nullptr));
	}
	
	return 0.f;
}

void EleIcon::playSelAnim()
{
	if (nullptr != _pWeakInfo && _pWeakInfo->type == EleWeakType::STONE)
	{
		auto strAnimName = String::createWithFormat("shihua_touch_%d", _eleId);
		_pModel->play(strAnimName->getCString());
	}
	else
	{
		auto strAnimName = String::createWithFormat("touch_%d_%d", _eleId, _eleLv + 1);
		_pModel->play(strAnimName->getCString());
	}

	GameUtils::playEffect("dianji.ogg");
}

void EleIcon::stopSelAnim()
{
	if (nullptr != _pWeakInfo && _pWeakInfo->type == EleWeakType::STONE)
	{
		auto strAnimName = String::createWithFormat("shihua_%d", _eleId);
		_pModel->play(strAnimName->getCString());
	}
	else
	{
		auto strAnimName = String::createWithFormat("idle_%d_%d", _eleId, _eleLv + 1);
		_pModel->play(strAnimName->getCString());
	}
}

void EleIcon::playCrushEffect(int index)
{
	if (_eleId >= EleIconId_E::ELE_ID_SWORD && _eleId <= EleIconId_E::ELE_ID_SUPER && nullptr == _pWeakInfo)
	{
		auto pEffect = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_CRUSH_EFFECT));//GameArmtr::createCrushEffect();
		auto strName = __String::createWithFormat("crush_%d", index + 1);//

// 		auto aa = pEffect->getArmtr()->getBone("Layer12");
// 		auto bb = aa->getDisplayManager()->getDecorativeDisplayList().at(0);
// 		auto cc = bb->getDisplay();
// 		auto dd = dynamic_cast<ParticleSystem *>(cc);
		if (1 == (index+1) || 3 == (index+1) || 4 == (index+1))
		{
			auto plistMap = ParticleManager::getInstance()->getPlistData("crush_1");
			auto pSystem = ParticleSystemQuad::create(plistMap);
			pSystem->setAutoRemoveOnFinish(true);
			CrushLayer::getInstance()->addChild(pSystem, ZInCrush_E::Z_C_EFFECT);
			pSystem->setPosition(this->getPosition());
		}

		CrushLayer::getInstance()->addChild(pEffect, ZInCrush_E::Z_C_EFFECT);
		pEffect->setPosition(this->getPosition());
		pEffect->play(strName->getCString());
		pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(CrushLayer::animationEffectEvent, CrushLayer::getInstance()));
	}
}

void EleIcon::changeEle(int newId, int newLv)
{
	this->removeSpec();
	this->removeWeak();

	if (_eleId <= EleIconId_E::ELE_ID_SUPER)
	{
		GameResPool::getInstance()->recycleRes(GameResId_E::RES_ELE_ICON, _pModel);
	}
	else
	{
		GameResPool::getInstance()->recycleRes(GameResId_E::RES_SPECIAL, _pModel);
	}
	_pModel->removeFromParent();
	_pModel = nullptr;
	
	_eleId = newId;
	_eleLv = newLv;


	__String *strFileName = nullptr;
	if (_eleId <= EleIconId_E::ELE_ID_SUPER)
	{
		_pModel = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_ELE_ICON));//GameArmtr::createEleIcon();
		_pModel->setPosition(Vec2::ZERO);
		this->addChild(_pModel);
		strFileName = __String::createWithFormat("idle_%d_%d", _eleId, _eleLv+1);
		_pModel->play(strFileName->getCString());

		if (CrushLayer::getInstance()->isStoneState(_eleId))
		{
			this->obtainWeak(3);
		}
	}
	else
	{
		_pModel = dynamic_cast<GameArmtr *>(GameResPool::getInstance()->getResById(GameResId_E::RES_SPECIAL));// GameArmtr::createSpecial();
		_pModel->setPosition(Vec2::ZERO);
		this->addChild(_pModel);
		_pSpecInfo = ParamMgr::getInstance()->getEleSpecInfo(_eleId);

		strFileName = String::createWithFormat("daiji_%d_0", _eleId - EleIconId_E::ELE_ID_SUPER - 1);
		_pModel->play(strFileName->getCString());

		this->setTotalRound(_pSpecInfo->totalRound);
		_curCrushCount = 0;
		if (-1 != _curRound)
		{
			_pRound = RoundTag::create(0, _curRound);
			_pRound->setPosition(12.f, -14.f);
			this->addChild(_pRound);
		}

		if (2 == _pSpecInfo->attTime)
		{
			if (_pSpecInfo->type == EleSpecialType::STONE)
			{
				GameUtils::playEffect("shihua.ogg");
				_stoneEleId = SpecialAttUtils::createStoneState();
			}
		}

		if (this->isNeedAddWeakArr())
		{
			CrushLayer::getInstance()->addWeakEle(this);
		}
	}
	this->updateEleBg();

	auto pEffect = GameArmtr::createCreateEffect();
	pEffect->play("create1");
	this->addChild(pEffect, 2);

	pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EleIcon::animationEffectEvent, this));
}

void EleIcon::setPosIndex(int row, int colum)
{
	_posIndex.row = row;
	_posIndex.column = colum;
}

void EleIcon::setTotalRound(int totalRound)
{
	_totalRound = totalRound;
	_curRound = _totalRound;
}

void EleIcon::animationEffectEvent(cocostudio::Armature * armature, cocostudio::MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		armature->getParent()->removeFromParent();
//		armature->removeFromParent();
	}
}

void EleIcon::doSpecAtt()
{
	if (nullptr != _pSpecInfo)
	{

		switch (_eleId-EleIconId_E::ELE_ID_SUPER-1)
		{
		case 0:
			//boom
			GameUtils::playEffect("zhadan.ogg");
			break;
		case 1:
			//volcano
			GameUtils::playEffect("huoshan.ogg");
			break;
		case 2:
			//thunder
			GameUtils::playEffect("leidian.ogg");
			break;
		case 3:
			//stone
			GameUtils::playEffect("shihua.ogg");
			break;
		case 4:
			//thorn_eye
			GameUtils::playEffect("jingji.ogg");
			break;
		case 6:
			//ice
			GameUtils::playEffect("bingfeng.ogg");
			break;
		}

		switch (_pSpecInfo->attType)
		{
		case 0:
			for (int i = 0; i < ParamData::ROLE_COUNT; i++)
			{
				if (5 == _eleId)
				{
#if (1 == CC_ENABLE_NEW_PARAM)
					float param = FightUtil::calcWeakDamageValue(1, FightLayer::getInstance()->getDesignRoleInfo());
#else
					float param = CrushUtil::getWeakDamageValue(1, Player::getInstance()->getBaseLv(), 0.f);
#endif
					this->createFlyWeak(1, i, param);
				}
				else if (7 == _eleId)
				{
#if (1 == CC_ENABLE_NEW_PARAM)
					float param = FightUtil::calcWeakDamageValue(2, FightLayer::getInstance()->getDesignRoleInfo());
#else
					float param = CrushUtil::getWeakDamageValue(2, Player::getInstance()->getBaseLv(), 0.f);
#endif
					this->createFlyWeak(2, i, param);
				}
			}
//			SpecialAttUtils::attPlayer(_pSpecInfo->attParam3);
			break;
		case 1:
			SpecialAttUtils::createWeak(_pSpecInfo->attParam1, _pSpecInfo->attParam2, 0, _posIndex);
			break;
		case 2:
			SpecialAttUtils::createWeak(_pSpecInfo->attParam1, _pSpecInfo->attParam2, 1, _posIndex);
			break;
		case 3:
			{
				int count = 1;
				if (_pSpecInfo->type == EleSpecialType::THORN_EYE && _curRound == (_totalRound - 1)) //&& _curRound == (_pSpecInfo->totalRound - 1))
				{
					count = 8;
				}
				SpecialAttUtils::createSpecEle(_pSpecInfo->type, _pSpecInfo->attParam1, count, _posIndex);
			}
			break;
		case 4:
		{
#if (1 == CC_ENABLE_NEW_PARAM)
			float param = FightUtil::calcWeakDamageValue(3, FightLayer::getInstance()->getDesignRoleInfo());
#else
			float param = CrushUtil::getWeakDamageValue(3, Player::getInstance()->getBaseLv(), 0.f);
#endif
			this->createFlyWeak(3, -1, param);
		}
//			SpecialAttUtils::addMonsterHp(_pSpecInfo->attParam3);
			break;
		case 5:
			if (3 == GameLayer::getInstance()->getFightType())
			{
				FightLayer::getInstance()->countDungeonDes(4, 1);

				if (FightLayer::getInstance()->getCurDungeonDesCount(4) < FightLayer::getInstance()->getDungeonDesCount(4))
				{
					SpecialAttUtils::createRandomKey(50);
				}
			}
			else
			{
				SpecialAttUtils::openBox(_pSpecInfo->attParam1);
			}
			break;
		default:
			break;
		}
	}
}

void EleIcon::updateModelSprite()
{
	__String *strFileName = nullptr;
	if (_eleId <= EleIconId_E::ELE_ID_SUPER)
	{
		strFileName = __String::createWithFormat("idle_%d_%d", _eleId, _eleLv+1);
		_pModel->play(strFileName->getCString());
	}
}

void EleIcon::updateWeakSprite()
{
	if (nullptr != _pWeakInfo)
	{
		if (-1 != _pWeakInfo->frameCount && _pWeakInfo->totalCrush > 1)
		{
			__String *strFileName = __String::createWithFormat("%s_%d.png", _pWeakInfo->animName.c_str(), _pWeakInfo->totalCrush - _curCrushCount - 1);
			_pWeakSprite->setSpriteFrame(strFileName->getCString());
		}
	}
}

void EleIcon::updateSpecSprite()
{
	if (nullptr != _pSpecInfo)
	{
		if (_pSpecInfo->isNeedChangeAnim && _curCrushCount!=_pSpecInfo->totalCrush)
		{
			__String *strName = __String::createWithFormat("daiji_%d_%d", _eleId-EleIconId_E::ELE_ID_SUPER-1, _curCrushCount);
			_pModel->play(strName->getCString());
		}
	}
}

void EleIcon::removeWeak()
{
	if (nullptr != _pWeakInfo)
	{
		auto strFileName = __String::createWithFormat("idle_%d_%d", _eleId, _eleLv + 1);
		_pModel->play(strFileName->getCString());

		if (1 == _pWeakInfo->id)
		{
			FightLayer::getInstance()->countDungeonDes(6, 1);
		}
		else if (2 == _pWeakInfo->id)
		{
			FightLayer::getInstance()->countDungeonDes(7, 1);
		}

		_pWeakInfo = nullptr;

		if (nullptr != _pWeakSprite)
		{
			_pWeakSprite->setVisible(false);
			_pWeakSprite->removeFromParent();
			_pWeakSprite = nullptr;
		}

		CrushLayer::getInstance()->removeWeakEle(this);
	}
}

void EleIcon::removeSpec()
{
	if (nullptr != _pSpecInfo)
	{
		if (_eleId == (EleIconId_E::ELE_ID_SUPER + 6))
		{
			FightLayer::getInstance()->countDungeonDes(8, 1);
		}

		_pSpecInfo = nullptr;

//		_pRound->setVisible(false);
		CrushLayer::getInstance()->removeWeakEle(this);
	}
}

void EleIcon::playCreateEffect()
{
	if (_eleId >= EleIconId_E::ELE_ID_SWORD && _eleId <= EleIconId_E::ELE_ID_HEART)
	{
		if (_eleLv > 0)
		{
			auto pEffect = GameArmtr::createCreateEffect();
// 			auto strName = __String::createWithFormat("create%d", _eleLv);
// 			pEffect->play(strName->getCString());
			pEffect->play("create1");
			this->addChild(pEffect, 2);

			pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EleIcon::animationEffectEvent, this));
		}
	}
	else if (_eleId == EleIconId_E::ELE_ID_SUPER)
	{
		auto pEffect = GameArmtr::createCreateEffect();
		pEffect->play("create1");
		this->addChild(pEffect, 2);

		pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EleIcon::animationEffectEvent, this));
	}
	else
	{
		if ((_eleId - EleIconId_E::ELE_ID_SUPER - 1) < 9)
		{
			auto pEffect = GameArmtr::createSpecialEx();
			auto strName = __String::createWithFormat("shengcheng_%d", _eleId - EleIconId_E::ELE_ID_SUPER - 1);
			pEffect->play(strName->getCString());
			this->addChild(pEffect, 2);

			pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EleIcon::animationEffectEvent, this));
		}
	}
}

// void EleIcon::playCrushEffect()
// {
// 	if (_eleId >= EleIconId_E::ELE_ID_SWORD && _eleId <= EleIconId_E::ELE_ID_HEART)
// 	{
// 		auto pEffect = GameArmtr::createCrushEffect();
// 		auto strName = __String::createWithFormat("crush_%d", _eleLv + 1);
// 		pEffect->play(strName->getCString());
// 		CrushLayer::getInstance()->addChild(pEffect, ZInCrush_E::Z_C_EFFECT);
// 		pEffect->setPosition(this->getPosition());
// 
// 		pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(CrushLayer::animationEffectEvent, CrushLayer::getInstance()));
// 	}
// }

void EleIcon::playBuffEffect(int index)
{
	std::string arrName[4] = { "clean1", "addlv", "clean2", "clean3"};

	auto pEffect = GameArmtr::createIconSkill();
	pEffect->play(arrName[index]);
	pEffect->setPosition(this->getPosition());
	CrushLayer::getInstance()->addChild(pEffect, ZInCrush_E::Z_C_EFFECT);

	pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(CrushLayer::animationEffectEvent, CrushLayer::getInstance()));
}

void EleIcon::playRoundEffect()
{
	if (nullptr != _pSpecInfo)
	{
		auto pEffect = GameArmtr::createSpecialEx();
		auto strName = __String::createWithFormat("fadong_1_%d", _eleId - EleIconId_E::ELE_ID_SUPER - 1);
		pEffect->play(strName->getCString());
		pEffect->setPosition(this->getPosition());
		CrushLayer::getInstance()->addChild(pEffect, ZInCrush_E::Z_C_EFFECT);

		pEffect->getArmtr()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(CrushLayer::animationEffectEvent, CrushLayer::getInstance()));
	}
}

bool EleIcon::isNeedAddWeakArr()
{
	if (nullptr != _pSpecInfo && _pSpecInfo->type != EleSpecialType::KEY && _pSpecInfo->type != EleSpecialType::SLATE)
	{
		return true;
	}

	if (nullptr != _pWeakInfo && 3 != _pWeakInfo->id)
	{
		return true;
	}

	return false;
}

void EleIcon::updateEleBg()
{
	if (_eleId == EleIconId_E::ELE_ID_SUPER)
	{
		if (nullptr == _pEleBg)
		{
			_pEleBg = Sprite::createWithSpriteFrameName("crush_ele_bg_4.png");
			this->addChild(_pEleBg, -1);
		}
		_pEleBg->setSpriteFrame("crush_ele_bg_4.png");
	}
	else if (_eleId >= EleIconId_E::ELE_ID_SWORD && _eleId <= EleIconId_E::ELE_ID_HEART && _eleLv > 0)
	{
		auto strName = __String::createWithFormat("crush_ele_bg_%d.png", _eleLv + 1);
		if (nullptr == _pEleBg)
		{
			_pEleBg = Sprite::createWithSpriteFrameName(strName->getCString());
			this->addChild(_pEleBg, -1);
		}
		_pEleBg->setSpriteFrame(strName->getCString());
	}
	else if (nullptr != _pSpecInfo && _pSpecInfo->type == EleSpecialType::KEY)
	{
		if (nullptr == _pEleBg)
		{
			_pEleBg = Sprite::createWithSpriteFrameName("crush_ele_key_bg.png");
//			CrushLayer::getInstance()->getCrushArea()->addChild(_pEleBg, 2);
			CrushLayer::getInstance()->addChild(_pEleBg, 1);
			this->schedule(CC_SCHEDULE_SELECTOR(EleIcon::updateEleKeyBgPos), 1 / 60.f);

			float dur = 1.f;
			_pEleBg->setScaleY(1.8f);
			_pEleBg->runAction(RepeatForever::create(Sequence::create(FadeTo::create(dur, 100), FadeTo::create(dur, 255), nullptr)));

			Vec2 areaPos = CrushLayer::getInstance()->getCrushArea()->getPosition();
			_pArrowTip = Sprite::createWithSpriteFrameName("tag_col.png");
//			CrushLayer::getInstance()->getCrushArea()->addChild(_pArrowTip, 2);
			CrushLayer::getInstance()->addChild(_pArrowTip, 1);
			_pArrowTip->setPosition(0.f, areaPos.y - 260.f);
			_pArrowTip->runAction(RepeatForever::create(Sequence::create(MoveBy::create(dur, Vec2(0, -10.f)), MoveBy::create(dur, Vec2(0.f, 10.f)), nullptr)));
//			_pArrowTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(dur, 1.4f), ScaleTo::create(dur, 1.f), nullptr)));
		}
		_pEleBg->setSpriteFrame("crush_ele_key_bg.png");
	}
	else
	{
		if (nullptr != _pEleBg)
		{
			_pEleBg->removeFromParent();
			_pEleBg = nullptr;
		}
	}
}

void EleIcon::updateEleKeyBgPos(float dt)
{
	if (nullptr != _pEleBg)
	{
		Vec2 areaPos = CrushLayer::getInstance()->getCrushArea()->getPosition();
		Vec2 elePos = this->convertToWorldSpaceAR(Vec2::ZERO);

		Vec2 bgPos = _pEleBg->getParent()->convertToNodeSpace(elePos);
		_pEleBg->setPosition(bgPos.x, areaPos.y);

//		_pArrowTip->setPosition(bgPos.x, areaPos.y - 260.f);
		_pArrowTip->setPositionX(bgPos.x);
	}
}
