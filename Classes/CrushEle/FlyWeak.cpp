#include "FlyWeak.h"
#include "JRCommon.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "FightLayer.h"
#include "Player.h"
#include "GameUtils.h"
#include "Monster.h"


FlyWeak::FlyWeak()
	:_pModel(nullptr),
	_totalTime(ParamData::ELE_FLY_DUR)
{
}

FlyWeak::~FlyWeak()
{
}

FlyWeak * FlyWeak::create(int weakId, int roleId, float param)
{
	FlyWeak *pRet = new FlyWeak();

	if (nullptr != pRet && pRet->init(weakId, roleId, param))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}


bool FlyWeak::init(int weakId, int roleId, float param)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());

		_weakId = weakId;
		_roleId = roleId;
		_param = param;
		
		switch (_weakId)
		{
		case 0:
			_pModel = Sprite::createWithSpriteFrameName("fire_0.png");
			break;
		case 1:
			_pModel = Sprite::createWithSpriteFrameName("zd_d01.png");//("boom_0.png");
			break;
		case 2:
			_pModel = Sprite::createWithSpriteFrameName("tubiao_d3/ldbj_1.png");// ("thunder_0.png");
			break;
		case 3:
			_pModel = Sprite::createWithSpriteFrameName("tubiao_d3/hfzx_1.png");// ("recover_0.png");
			break;
		}

		this->addChild(_pModel);
		_pModel->runAction(FadeOut::create(ParamData::ELE_FLY_DUR));
		
		this->scheduleUpdate();
		bRet = true;
	} while (false);

	return bRet;
}

void FlyWeak::update(float delta)
{
	if (_totalTime <= 0.f)
	{
		if (3 == _weakId)
		{
			auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
			if (!arrMonster->empty())
			{
				arrMonster->at(0)->addHP(_param);
			}
		}
		else if (0 == _weakId)
		{
			Player::getInstance()->doHurt(AttAttrbt_E::ATT_FIRE, _param);
			Player::getInstance()->playStateAnim("buff_0", _roleId, 1);
		}
		else if (1 == _weakId)
		{
			if (0 == _roleId)
			{
				Player::getInstance()->doHurt(AttAttrbt_E::ATT_FIRE, _param);
			}
			Player::getInstance()->playStateAnim("buff_0", _roleId, 1);
		}
		else if (2 == _weakId)
		{
			if (0 == _roleId)
			{
				Player::getInstance()->doHurt(AttAttrbt_E::ATT_THUNDER, _param);
			}
			Player::getInstance()->playStateAnim("buff_2", _roleId, 1);
		}

		this->removeFromParent();
	}
	else
	{
		Vec2 desPos = Vec2::ZERO;

		if (3 == _weakId)
		{
			auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
			if (arrMonster->empty())
			{
				this->removeFromParent();
				return;
			}
			else
			{
				desPos = arrMonster->at(0)->getPosition();
			}
		}
		else
		{
			desPos = Player::getInstance()->getCurRolePos(_roleId) + Vec2(0.f, 50.f);
		}
		Vec2 curPos = this->getPosition();
		float fDist = curPos.getDistance(desPos);
		Vec2 norVec = desPos - curPos;
		norVec.normalize();
		float fSpeed = fDist * delta / _totalTime;
		Vec2 newPos = norVec * fSpeed + curPos;

		this->setPosition(newPos);

		_totalTime -= delta;
	}
}
