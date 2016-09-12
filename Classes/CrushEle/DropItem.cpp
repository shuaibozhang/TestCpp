#include "DropItem.h"
#include "JRCommon.h"
#include "ParamData.h"
#include "CrushUtil.h"
#include "FightLayer.h"
#include "Player.h"
#include "GameUtils.h"
#include "UserData.h"
#include "GameResPool.h"


DropItem::DropItem()
	:_pModel(nullptr),
	_pShadow(nullptr)
{
}

DropItem::~DropItem()
{
	GameResPool::getInstance()->recycleRes(_pModel);
}

DropItem * DropItem::create(int itemId)
{
	DropItem *pRet = new DropItem();

	if (nullptr != pRet && pRet->init(itemId))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DropItem::init(int itemId)
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());

		_itemId = itemId;

// 		__String *strName = nullptr;
// 
// 		if (0 == itemId)
// 		{
// 			strName = __String::create("drop_yb.png");
// 		}
// 		else if (1 == itemId)
// 		{
// 			strName = __String::create("drop_gold.png");
// 		}
// 		else if (2 == itemId)
// 		{
// 			strName = __String::create("drop_silver.png");
// 		}

		_pModel = dynamic_cast<Sprite *>(GameResPool::getInstance()->getResById(itemId+GameResId_E::RES_GOLD_0));//Sprite::createWithSpriteFrameName(strName->getCString());
		_pModel->setPosition(Vec2::ZERO);
		this->addChild(_pModel, 1);

		_pShadow = Sprite::createWithSpriteFrameName("fight_item_shadow.png");
		this->addChild(_pShadow, 1);
		_pShadow->setScale(0.5f);

		_posId = ToolsUtil::getRandomInt(0, ParamData::ROLE_COUNT - 1);

		float offsetY = ToolsUtil::getRandomFloat(80.f, 200.f);
		float offsetX = ToolsUtil::getRandomFloat(-150.f, 150.f);

		float dur = 1.f;
		auto moveAct = Sequence::createWithTwoActions(EaseSineOut::create(MoveBy::create(dur/2, Vec2(offsetX / 2, offsetY))), EaseSineIn::create(MoveBy::create(dur/2, Vec2(offsetX / 2, -offsetY))));
		auto rotateAct = RotateBy::create(dur, ToolsUtil::getRandomFloat(300.f, 420.f));
//		auto spawnAct = Spawn::createWithTwoActions(rotateAct, moveAct);
		_pModel->runAction(rotateAct);
		this->runAction(Sequence::create(moveAct, CallFunc::create([=] {this->scheduleUpdate(); }), nullptr));
		
		bRet = true;
	} while (false);

	return bRet;
}

void DropItem::setPosition(float x, float y)
{
	Node::setPosition(x, y);

	Vec2 rolePos = CrushUtil::getRolePos(_posId, FightLayer::getInstance()->getCurPlayerPosId());
	Vec2 nodePos = this->convertToNodeSpace(Vec2(0.f, rolePos.y));

	_pShadow->setPosition(0.f, nodePos.y);
}

void DropItem::setPosition(const Vec2 & position)
{
	this->setPosition(position.x, position.y);
}

void DropItem::update(float delta)
{
	Vec2 desPos = Player::getInstance()->getCurRolePos(_posId) + Vec2(0.f, 40.f);
	Vec2 curPos = this->getPosition();
	float fSpeed = 500.f * delta;

	if (desPos.getDistanceSq(curPos) < fSpeed*fSpeed)
	{
		if (0 == _itemId)
		{
			UserData::getInstance()->giveGold(100);
		}
		else if (1 == _itemId)
		{
			UserData::getInstance()->giveGold(10);
		}
		else if (2 == _itemId)
		{
			UserData::getInstance()->giveGold(1);
		}

		GameUtils::playEffect("jinbi.ogg");
		this->removeFromParent();
	}
	else
	{
		Vec2 moveVec = (desPos - curPos);
		moveVec.normalize();
		Vec2 newPos = moveVec * fSpeed + curPos;

		this->setPosition(newPos);
	}
}
