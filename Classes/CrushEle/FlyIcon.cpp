#include "FlyIcon.h"
#include "JRCommon.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "FightLayer.h"
#include "Player.h"
#include "GameUtils.h"
#include "GameResPool.h"


FlyIcon::FlyIcon()
	:_pModel(nullptr),
	_iconId(0),
	_iconLv(0),
	_totalTime(ParamData::ELE_FLY_DUR)
{
}

FlyIcon::~FlyIcon()
{
	GameResPool::getInstance()->recycleRes(_pModel);
}

FlyIcon * FlyIcon::create(int iconId, int iconLv)
{
	FlyIcon *pRet = new FlyIcon();

	if (nullptr != pRet && pRet->init(iconId, iconLv))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}


bool FlyIcon::init(int iconId, int iconLv)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());

		_iconId = iconId;
		_iconLv = iconLv;

		if (_iconId < EleIconId_E::ELE_ID_SUPER)
		{
			int roleId = Player::getInstance()->getRoleIdByPosIndex(_iconId);
			int resId = GameResId_E::RES_FLY_ICON_START + roleId * 3 + _iconLv;
			_pModel = dynamic_cast<Sprite *> (GameResPool::getInstance()->getResById(resId));
			this->addChild(_pModel);

			_pModel->setOpacity(255);
			_pModel->runAction(FadeOut::create(ParamData::ELE_FLY_DUR));

// 			BlendFunc bf;
// 			bf.src = GL_ONE;
// 			bf.dst = GL_ONE_MINUS_CONSTANT_ALPHA;
// 			_pModel->setBlendFunc(bf);
		}
		
		this->scheduleUpdate();
		bRet = true;
	} while (false);

	return bRet;
}

void FlyIcon::update(float delta)
{
	if (_totalTime <= 0.f)
	{
		this->removeFromParent();
	}
	else
	{
		Vec2 desPos = Player::getInstance()->getCurRolePos(_iconId - EleIconId_E::ELE_ID_0) + Vec2(0.f, 40.f);
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
