#include "GameResPool.h"
#include "ParamData.h"
#include "Armtr/GameArmtr.h"


GameResPool *GameResPool::s_pInstance = nullptr;

GameResPool * GameResPool::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new GameResPool();
		s_pInstance->init();
	}

	return s_pInstance;
}


GameResPool::~GameResPool()
{
}


GameResPool::GameResPool()
{

}

void GameResPool::loadRes(int resId, int resCount)
{
	for (int i = 0; i < resCount; i++)
	{
		auto pRes = this->createRes(resId);
		_arrResPool[resId].pushBack(pRes);
	}
}

void GameResPool::recycleRes(int resId, Node *pRes)
{
	if (GameResId_E::RES_BUFF == resId || GameResId_E::RES_FB == resId || GameResId_E::RES_HIT == resId || GameResId_E::RES_ELE_ICON == resId || GameResId_E::RES_CRUSH_EFFECT == resId 
		|| (GameResId_E::RES_GOLD_0 <= resId && resId <= GameResId_E::RES_FLY_ICON_END))
	{
//		pRes->stopAllActions();
		_arrResPool[resId].pushBack(pRes);
	}
}

void GameResPool::recycleRes(Node * pRes)
{
	int resId = (int)(pRes->getUserData());
	recycleRes(resId, pRes);
}

Node * GameResPool::getResById(int resId)
{
	Node *pRet = nullptr;

	if (_arrResPool[resId].empty())
	{
		pRet = this->createRes(resId);
	}
	else
	{
		pRet = _arrResPool[resId].back();
		pRet->retain();
		pRet->autorelease();
		_arrResPool[resId].popBack();
	}
	pRet->setPosition(Vec2::ZERO);
	pRet->setVisible(true);

	return pRet;
}

bool GameResPool::init()
{
//	this->loadEleIcon(EleIconId_E::ELE_ID_SWORD, ParamData::CRUSH_ROW * ParamData::CRUSH_COL+4);

	return true;
}

Node * GameResPool::createRes(int resId)
{
	Node *pRet = nullptr;
	switch (resId)
	{
	case RES_ROLE:
		break;
	case RES_MONSTER:
		break;
	case RES_SKILL:
		break;
	case RES_BUFF:
		pRet = GameArmtr::createBuff("");
		break;
	case RES_FB:
		pRet = GameArmtr::createDart();
		break;
	case RES_HIT:
		pRet = GameArmtr::createHit("");
		break;
	case RES_ELE_ICON:
		pRet = GameArmtr::createEleIcon();
		break;
	case RES_CRUSH_EFFECT:
		pRet = GameArmtr::createCrushEffect();
		break;
	case RES_CREATE_EFFECT:
		pRet = GameArmtr::createCreateEffect();
		break;
	case RES_ICON_SKILL:
		break;
	case RES_ITEM_EFFECT:
		break;
	case RES_SPECIAL:
		pRet = GameArmtr::createSpecial();
		break;
	case RES_SPECIAL_EX:
		pRet = GameArmtr::createSpecialEx();
		break;
	case RES_GOLD_0:
		pRet = Sprite::createWithSpriteFrameName("drop_yb.png");
		break;
	case RES_GOLD_1:
		pRet = Sprite::createWithSpriteFrameName("drop_gold.png");
		break;
	case RES_GOLD_2:
		pRet = Sprite::createWithSpriteFrameName("drop_silver.png");
		break;
	case RES_FLY_ICON_START:
	case RES_FLY_ICON_START + 1:
	case RES_FLY_ICON_START + 2:
	case RES_FLY_ICON_START + 3:
	case RES_FLY_ICON_START + 4:
	case RES_FLY_ICON_START + 5:
	case RES_FLY_ICON_START + 6:
	case RES_FLY_ICON_START + 7:
	case RES_FLY_ICON_START + 8:
	case RES_FLY_ICON_START + 9:
	case RES_FLY_ICON_START + 10:
	case RES_FLY_ICON_START + 11:
	case RES_FLY_ICON_START + 13:
	case RES_FLY_ICON_START + 14:
	case RES_FLY_ICON_START + 15:
	{
		std::string arrName[] = {"dao.png", "dao_2.png", "dao_3.png" ,
		"dun.png", "dun_2.png", "dun_3.png",
		"feibiao.png", "feibiao_2.png", "feibiao_3.png",
		"aixin.png", "aixin_2.png", "aixin_3.png",
			"aixin.png", "aixin_2.png", "aixin_3.png",};

		__String *strFileName = String::createWithFormat("anniutubiao/%s", arrName[resId - RES_FLY_ICON_START].c_str());;

		pRet = Sprite::createWithSpriteFrameName(strFileName->getCString());
	}
		break;
	}

	pRet->setUserData((void *)resId);

	return pRet;
}
