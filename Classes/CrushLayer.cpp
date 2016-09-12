#include "CrushLayer.h"
#include "VisibleRect.h"
#include "JRCommon.h"
#include "EleIcon.h"
#include "CrushFSM/CrushFSM.h"
#include "CrushUtil.h"
#include "Monster.h"
#include "Player.h"
#include "GameLayer.h"
#include "FightLayer.h"
#include "GameUtils.h"
#include "FlyIcon.h"
#include "ParamMgr.h"
#include "GameResPool.h"
#include "MagPieMgr.h"
#include "SpecialAttUtils.h"

USING_NS_CC;


CrushLayer *CrushLayer::s_pInstance = nullptr;


CrushLayer *CrushLayer::getInstance()
{
	return s_pInstance;
}


CrushLayer::CrushLayer()
	:_isFallFinish(true)
{
//	memset((void *)_arrCurEleCount, 0, sizeof(_arrCurEleCount));
	memset((void *)_arrIsStone, 0, sizeof(_arrIsStone));
	memset((void *)_arrEleIcon, 0, sizeof(_arrEleIcon));
	memset((void *)_arrCrushEleCount, 0, sizeof(_arrCrushEleCount));

	_touchIndex.column = -1;
	_touchIndex.row = -1;
	_moveIndex.column = -1;
	_moveIndex.row = -1;

	s_pInstance = this;
}


CrushLayer::~CrushLayer()
{
//	_eventDispatcher->removeAllEventListeners();
	_eventDispatcher->removeEventListenersForTarget(this);
	_eventDispatcher->removeCustomEventListeners(CrushMsg::TOUCH_BEGAN);
	_eventDispatcher->removeCustomEventListeners(CrushMsg::TOUCH_MOVED);
	_eventDispatcher->removeCustomEventListeners(CrushMsg::TOUCH_ENDED);
	_eventDispatcher->removeCustomEventListeners(CrushMsg::TOUCH_CANCELLED);
	delete _pStateMac;
	s_pInstance = nullptr;
}


// on "init" you need to initialize your instance
bool CrushLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bg/crush_ele_bg.plist");
	ToolsUtil::initRandomNumber();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float startPosY = (ParamData::CRUSH_BG_HEIGHT - ParamData::CRUSH_ROW*ParamData::ELE_BG_HEIGHT) / 2.f;
	_crushAreaStartPos = Vec2(visibleSize.width/2 - ParamData::CRUSH_COL/2*ParamData::ELE_BG_HEIGHT, startPosY);

	Vec2 areaPos = Vec2(visibleSize.width/2, ParamData::CRUSH_BG_HEIGHT/2);//ParamData::ELE_BG_WIDTH*ParamData::CRUSH_ROW/2);
	auto pCrushArea = Node::create();
	pCrushArea->setPosition(areaPos);
	this->addChild(pCrushArea);
	_pCrushArea = pCrushArea;

	auto pCrushAreaBg = ui::Scale9Sprite::create("bg/bg_fight.png");
	pCrushAreaBg->setPreferredSize(Size(630.f, 560.f));//ParamData::CRUSH_BG_HEIGHT
//	auto pCrushAreaBg = Sprite::create("dikuang.png");
	pCrushArea->addChild(pCrushAreaBg);

// 	auto pBoundary = Sprite::createWithSpriteFrameName("crush_boundary.png");
// 	pBoundary->setPosition(Vec2(0.f, ParamData::CRUSH_BG_HEIGHT / 2 ));
// 	pCrushArea->addChild(pBoundary);

	_pShieldLayer = LayerColor::create(Color4B(0, 0, 0, 122), 630.f, ParamData::CRUSH_BG_HEIGHT);
	_pShieldLayer->setPosition(0.f, 0.f);
	this->addChild(_pShieldLayer, 2);
	_pShieldLayer->setVisible(true);


	int arrNewEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {EleIconId_E::ELE_ID_EMPTY};
	memset((void *)arrNewEleId, EleIconId_E::ELE_ID_EMPTY, sizeof(arrNewEleId));

	int arrNewEleState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
	memset((void *)arrNewEleState, 0, sizeof(arrNewEleState));
	
	this->createNewEleId(arrNewEleId, arrNewEleState, 100, 0, 0);

// 	arrNewEleId[ParamData::CRUSH_ROW-1][3] = arrNewEleId[ParamData::CRUSH_ROW-2][2];
// 	arrNewEleId[ParamData::CRUSH_ROW-3][2] = arrNewEleId[ParamData::CRUSH_ROW-2][2];
// 	arrNewEleId[ParamData::CRUSH_ROW-4][2] = arrNewEleId[ParamData::CRUSH_ROW-2][2];
// 	arrNewEleId[ParamData::CRUSH_ROW-5][2] = arrNewEleId[ParamData::CRUSH_ROW-2][2];
//	arrNewEleId[ParamData::CRUSH_ROW-1][4] = EleIconId_E::ELE_ID_SUPER + 9;
//	arrNewEleId[ParamData::CRUSH_ROW - 3][ParamData::CRUSH_COL-5] = 7;

	if (3 == GameLayer::getInstance()->getFightType())
	{
		auto pDungeonDeatilInfo = FightLayer::getInstance()->getDungeonDeatilInfo();
		if (-1 != pDungeonDeatilInfo->initContentId)
		{
			if (5 == pDungeonDeatilInfo->initContentId || 8 == pDungeonDeatilInfo->initContentId)
			{
				int eleId = 0;

				if (5 == pDungeonDeatilInfo->initContentId)
				{
					eleId = EleIconId_E::ELE_ID_SUPER + 11;
				}
				else if (8 == pDungeonDeatilInfo->initContentId)
				{
					eleId = EleIconId_E::ELE_ID_SUPER + 6;
				}

				for (int i = 0; i < pDungeonDeatilInfo->arrInitPosX.size(); i++)
				{
					int row = pDungeonDeatilInfo->arrInitPosX[i];
					int col = pDungeonDeatilInfo->arrInitPosY[i];
					arrNewEleId[row][col] = eleId;
				}
			}
		}
	}

	int count = 0;
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j=0; j<ParamData::CRUSH_COL; j++)
		{
			Vec2 pos = CrushUtil::getElePos(i, j);
			__String *strFile = __String::createWithFormat("%s%d.png", "crush_ele_bg_", (count+i)%2);
			auto bg = Sprite::createWithSpriteFrameName(strFile->getCString());
			bg->setPosition(pos - areaPos);
			pCrushArea->addChild(bg, 1);
			count++;

			int eleId = arrNewEleId[i][j];
			auto pEle = EleIcon::create(eleId, 0);
			pEle->setPosition(pos);
			this->addChild(pEle);

			pEle->setPosIndex(i, j);
			_arrEleIcon[i][j] = pEle;

//			this->countingEle(eleId, 1);
		}		
	}

	if (3 == GameLayer::getInstance()->getFightType())
	{
		auto pDungeonDeatilInfo = FightLayer::getInstance()->getDungeonDeatilInfo();
		if (-1 != pDungeonDeatilInfo->initContentId)
		{
			if (6 == pDungeonDeatilInfo->initContentId || 7 == pDungeonDeatilInfo->initContentId)
			{
				int weakId = 0;

				if (6 == pDungeonDeatilInfo->initContentId)
				{
					weakId = 1;
				}
				else if (7 == pDungeonDeatilInfo->initContentId)
				{
					weakId = 2;
				}

				for (int i = 0; i < pDungeonDeatilInfo->arrInitPosX.size(); i++)
				{
					int row = pDungeonDeatilInfo->arrInitPosX[i];
					int col = pDungeonDeatilInfo->arrInitPosY[i];
					_arrEleIcon[row][col]->obtainWeak(weakId);
				}
			}
		}

		for (int i = 0; i < pDungeonDeatilInfo->arrDesInfo.size(); i++)
		{
			if (4 == pDungeonDeatilInfo->arrDesInfo[i].crushId)
			{
				SpecialAttUtils::createRandomKey(50);
				break;
			}
		}
	}

	/*debug*/
// 	_arrEleIcon[ParamData::CRUSH_ROW-2][2]->obtainWeak(2);
// 	_arrEleIcon[ParamData::CRUSH_ROW-2][3]->obtainWeak(1);
// 	_arrEleIcon[ParamData::CRUSH_ROW-2][4]->obtainWeak(1);
// 	_arrEleIcon[ParamData::CRUSH_ROW-2][5]->obtainWeak(1);
// 	_arrEleIcon[ParamData::CRUSH_ROW-2][6]->obtainWeak(1);	
// 
//  	_arrEleIcon[ParamData::CRUSH_ROW-4][0]->obtainWeak(1);
//  	_arrEleIcon[ParamData::CRUSH_ROW-4][1]->obtainWeak(1);
//  
//  	_arrEleIcon[ParamData::CRUSH_ROW-5][ParamData::CRUSH_COL-1]->obtainWeak(1);
//  	_arrEleIcon[ParamData::CRUSH_ROW-5][ParamData::CRUSH_COL-2]->obtainWeak(1);

	_pLabel = ui::Text::create();
	this->addChild(_pLabel);
	_pLabel->setPosition(Vec2(320.f, 800.f));
	/*end debug*/

	_pStateMac = new (std::nothrow) StateMachine<CrushLayer>(this);
	_pStateMac->setCurState(WaitTouchState::getInstance());

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(CrushLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CrushLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CrushLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(CrushLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);	
	
	_eventDispatcher->addCustomEventListener(CrushMsg::TOUCH_BEGAN, CC_CALLBACK_1(CrushLayer::handleMessage, this));
	_eventDispatcher->addCustomEventListener(CrushMsg::TOUCH_MOVED, CC_CALLBACK_1(CrushLayer::handleMessage, this));
	_eventDispatcher->addCustomEventListener(CrushMsg::TOUCH_ENDED, CC_CALLBACK_1(CrushLayer::handleMessage, this));
	_eventDispatcher->addCustomEventListener(CrushMsg::TOUCH_CANCELLED, CC_CALLBACK_1(CrushLayer::handleMessage, this));
//	_eventDispatcher->dispatchCustomEvent()
//	EventListenerCustom::create()
	this->scheduleUpdate();


	/*
	bool arrCanFallTest[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {true};
	CrushIndex_T arrSrcTest[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
	CrushIndex_T tmp = {-1};
	tmp.row = -1;
	tmp.column = -1;

	for (int i=ParamData::CRUSH_ROW-1; i>=0; i--)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			arrSrcTest[i][j].row = i;
			arrSrcTest[i][j].column = j;

			arrCanFallTest[i][j] = true;
		}
	}
	

	arrCanFallTest[ParamData::CRUSH_ROW-2][2] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-2][3] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-2][4] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-2][5] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-2][6] = false;

	arrSrcTest[ParamData::CRUSH_ROW-3][3] = tmp;
	arrSrcTest[ParamData::CRUSH_ROW-3][4] = tmp;
	arrSrcTest[ParamData::CRUSH_ROW-3][6] = tmp;


	arrCanFallTest[ParamData::CRUSH_ROW-4][0] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-4][1] = false;

	arrCanFallTest[ParamData::CRUSH_ROW-5][ParamData::CRUSH_COL-1] = false;
	arrCanFallTest[ParamData::CRUSH_ROW-5][ParamData::CRUSH_COL-2] = false;

	CrushUtil::simulateFall(arrCanFallTest, arrSrcTest);*/

    return true;
}


bool CrushLayer::initRoundData()
{
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j=0; j<ParamData::CRUSH_COL; j++)
		{
			if (nullptr != _arrEleIcon[i][j])
			{
				_arrEleIcon[i][j]->setIsActive(true);
			}			
		}		
	}

	auto arrMonster = FightLayer::getInstance()->getCurArrMonster();
	for (int i = 0; i < arrMonster->size(); i++)
	{
		auto pMonster = arrMonster->at(i);
		if (false == pMonster->getIsActive())
		{
			pMonster->setIsActive(true);
		}
	}

// 	_curIndex.column = -1;
// 	_curIndex.row = -1;
// 	_newIndex.column = -1;
// 	_newIndex.row = -1;

	return true;
}

/*
Vec2 CrushLayer::getElePos(int row, int column)
{
	float x = _crushAreaStartPos.x + (column + 0.5f) * ParamData::ELE_BG_WIDTH;
	float y = _crushAreaStartPos.y + (row + 0.5f) * ParamData::ELE_BG_HEIGHT;
	
	return Vec2(x, y);
}


CrushIndex_T CrushLayer::getCrushIndex(const Vec2 &pos)
{
	CrushIndex_T ret = {0};
	Vec2 startPos = _crushAreaStartPos;
	Vec2 endPos = _crushAreaStartPos + Vec2(ParamData::ELE_BG_WIDTH*ParamData::CRUSH_COL, ParamData::ELE_BG_HEIGHT*ParamData::CRUSH_ROW);

	if (pos.x >= startPos.x && pos.y >= startPos.y && pos.x < endPos.x && pos.y < endPos.y)
	{
		float dtX = pos.x - startPos.x;
		float dtY = pos.y - startPos.y;

		ret.column = dtX / ParamData::ELE_BG_WIDTH;
		ret.row = dtY / ParamData::ELE_BG_HEIGHT;
	}
	else
	{
		ret.column = -1;
		ret.row = -1;
	}
	
	return ret;
}*/


void CrushLayer::update(float delta)
{
// 	__String *strText = __String::createWithFormat("%d  %d  %d  %d", _arrCurEleCount[0], _arrCurEleCount[1], _arrCurEleCount[2], _arrCurEleCount[3]);
// 	_pLabel->setString(strText->getCString());
	
	_pStateMac->getCurState()->execute(this);
}


bool CrushLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!this->isVisible() || _pShieldLayer->isVisible())
	{
		return true;
	}
	if (_pStateMac->isInState(WaitTouchState::getInstance()))
	{
		_eventDispatcher->dispatchCustomEvent(CrushMsg::TOUCH_BEGAN, touch);
	}

	return true;
}


void CrushLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!this->isVisible() || _pShieldLayer->isVisible())
	{
		return;
	}
	if (_pStateMac->isInState(WaitMoveState::getInstance()))
	{
		_eventDispatcher->dispatchCustomEvent(CrushMsg::TOUCH_MOVED, touch);
	}	
}


void CrushLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!this->isVisible() || _pShieldLayer->isVisible())
	{
		return;
	}
	if (_pStateMac->isInState(WaitMoveState::getInstance()))
	{
		_eventDispatcher->dispatchCustomEvent(CrushMsg::TOUCH_ENDED, touch);
	}
}


void CrushLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!this->isVisible() || _pShieldLayer->isVisible())
	{
		return;
	}
	if (_pStateMac->isInState(WaitMoveState::getInstance()))
	{
		_eventDispatcher->dispatchCustomEvent(CrushMsg::TOUCH_CANCELLED, touch);
	}
}


void CrushLayer::handleMessage(EventCustom *event)
{
	_pStateMac->handleMessage(event);
}


bool CrushLayer::checkCrushTravelse()
{
	bool bRet = false;
	TravelseInfo_T curInfo = {0};

	curInfo.crushArrCount = 0;
	curInfo.touchIndex = _touchIndex;
	curInfo.moveIndex = _moveIndex;
	memset((void *)curInfo.arrCrushArrIndex, -1, sizeof(curInfo.arrCrushArrIndex));
	memset((void *)curInfo.arrIsTravelsed, 0, sizeof(curInfo.arrIsTravelsed));
	memset((void *)curInfo.arrCrushDetail, 0, sizeof(curInfo.arrCrushDetail));
	memset((void *)curInfo.arrEleWeakState, 0, sizeof(curInfo.arrEleWeakState));

	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr != _arrEleIcon[i][j])
			{
				curInfo.arrEleId[i][j] = _arrEleIcon[i][j]->getEleId();

				if (_arrEleIcon[i][j]->isCrushEffective())
				{
					curInfo.arrEleWeakState[i][j] = 0;
				}
				else
				{
					curInfo.arrEleWeakState[i][j] = 1;
				}				
			}
			else
			{
				curInfo.arrEleId[i][j] = EleIconId_E::ELE_ID_INVALID;
				curInfo.arrEleWeakState[i][j] = 0;
			}			
		}
	}

	if (-1 != _touchIndex.row && -1 != _touchIndex.column)
	{
		int curEleId = _arrEleIcon[_touchIndex.row][_touchIndex.column]->getEleId();//_crushInfo.arrEleId[_curIndex.row][_curIndex.column];//
		int desEleId = _arrEleIcon[_moveIndex.row][_moveIndex.column]->getEleId();//_crushInfo.arrEleId[_newIndex.row][_newIndex.column];//

		if (EleIconId_E::ELE_ID_SUPER == curEleId || EleIconId_E::ELE_ID_SUPER == desEleId)
		{
			curInfo.arrCrushDetail[curInfo.crushArrCount].crushCount = 2;
			curInfo.arrCrushDetail[curInfo.crushArrCount].crushEleId = (curEleId!=EleIconId_E::ELE_ID_SUPER) ?curEleId :desEleId;
			curInfo.arrCrushDetail[curInfo.crushArrCount].isTouchEle = true;
			curInfo.arrCrushDetail[curInfo.crushArrCount].isFinish = true;
			if (_arrEleIcon[_touchIndex.row][_touchIndex.column]->isCrushEffective() && _arrEleIcon[_moveIndex.row][_moveIndex.column]->isCrushEffective())
			{
				curInfo.arrCrushDetail[curInfo.crushArrCount].crushType = 3;
			}
			else
			{
				curInfo.arrCrushDetail[curInfo.crushArrCount].crushType = 4;
			}

			curInfo.arrIsTravelsed[_touchIndex.row][_touchIndex.column] = true;
			curInfo.arrIsTravelsed[_moveIndex.row][_moveIndex.column] = true;
			curInfo.arrCrushArrIndex[_touchIndex.row][_touchIndex.column] = 0;
			curInfo.arrCrushArrIndex[_moveIndex.row][_moveIndex.column] = 0;

// 			if (EleIconId_E::ELE_ID_SUPER == curEleId && EleIconId_E::ELE_ID_SUPER == desEleId)
// 			{
// 				curInfo.arrCrushDetail[curInfo.crushArrCount].attLv = 6;
// 			}
// 			else
// 			{
// 				EleIcon *pEle = (curEleId!=EleIconId_E::ELE_ID_SUPER) ?_arrEleIcon[_curIndex.row][_curIndex.column] :_arrEleIcon[_newIndex.row][_newIndex.column];
// 
// 				curInfo.arrCrushDetail[curInfo.crushArrCount].attLv = 3 + pEle->getEleLv();
// 			}			

			curInfo.crushArrCount++;
			bRet = true;
		}

// 		_curIndex.column = -1;
// 		_curIndex.row = -1;
// 		_newIndex.column = -1;
// 		_newIndex.row = -1;
	}

	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			bool isCrush = CrushUtil::travelseEle(i, j, &curInfo, false, 0);
//			bool isCrush = this->travelseCrushEle(i, j, _curCrushArrCount);
			if (isCrush)
			{
				curInfo.arrCrushDetail[curInfo.crushArrCount].crushEleId = curInfo.arrEleId[i][j];//_arrEleIcon[i][j]->getEleId();

				if (0 == curInfo.arrCrushDetail[curInfo.crushArrCount].crushType)
				{
					if (curInfo.arrCrushDetail[curInfo.crushArrCount].crushCount > 3)
					{
						curInfo.arrCrushDetail[curInfo.crushArrCount].crushType = 1;
					}					
				}				

				curInfo.crushArrCount++;

				bRet = true;
			}			
		}
	}
	
	if (bRet)
	{
		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				if (curInfo.arrCrushArrIndex[i][j] >= 0)
				{
					int attLv = _arrEleIcon[i][j]->getEleLv();
					int index = curInfo.arrCrushArrIndex[i][j];

					if (attLv < 3)
					{
						curInfo.arrCrushDetail[index].arrLvCount[attLv]++;
					}
				}
			}
		}

		_curCrushArrCount = curInfo.crushArrCount;
		memcpy((void *)_arrCrushArrIndex, (void *)curInfo.arrCrushArrIndex, sizeof(_arrCrushArrIndex));
		memcpy((void *)_arrCrushArrInfo, (void *)curInfo.arrCrushDetail, sizeof(_arrCrushArrInfo));
	}
	else
	{
		_curCrushArrCount = 0;
		memset((void *)_arrCrushArrIndex, -1, sizeof(_arrCrushArrIndex));
		memset((void *)_arrCrushArrInfo, 0, sizeof(_arrCrushArrInfo));
	}
	
	for (int j=0; j<ParamData::CRUSH_COL; j++)
	{
		EleIcon *pEle = _arrEleIcon[0][j];
		if (nullptr != pEle)
		{
			if (pEle->isNeedDealBottom())
			{
				_arrCrushArrIndex[0][j] = -2;
				bRet = true;
			}
		}		
	}

	return bRet;
}


bool CrushLayer::doBottomCrush()
{
	bool bRet = false;

	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j=0; j<ParamData::CRUSH_COL; j++)
		{
			if (-2 == _arrCrushArrIndex[i][j])
			{
				if (_arrEleIcon[i][j]->doInBottom())
				{
					_arrEleIcon[i][j] = nullptr;
					bRet = true;
				}				
			}
		}		
	}

	return bRet;
}


bool CrushLayer::doCleanCrush()
{
	bool bRet = false;

	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j=0; j<ParamData::CRUSH_COL; j++)
		{
			if (-1 < _arrCrushArrIndex[i][j])
			{
				int arrIndex = _arrCrushArrIndex[i][j];
				EleIcon *pEle = _arrEleIcon[i][j];
				CrushArr_T *pInfo = &_arrCrushArrInfo[arrIndex];

				if (nullptr != pEle && pEle->getEleId() == EleIconId_E::ELE_ID_HEART && !_arrIsStone[EleIconId_E::ELE_ID_HEART] && 4 != pInfo->crushType)
				{
					if (3 == pInfo->crushType)
					{
						bRet = Player::getInstance()->doHeartSkill(pEle, 1) ? true : bRet;
					}
					else if (pEle->getEleLv() > 0)
					{
						bRet = Player::getInstance()->doHeartSkill(pEle, 0) ? true : bRet;
					}
				}
			}
		}		
	}

	return bRet;
}


bool CrushLayer::doCrush()
{
	bool bRet = false;
	
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j=0; j<ParamData::CRUSH_COL; j++)
		{
			if (-1 < _arrCrushArrIndex[i][j])
			{
				bRet = true;
				int arrIndex = _arrCrushArrIndex[i][j];

				if (4 != _arrCrushArrInfo[arrIndex].crushType && _arrEleIcon[i][j]->getEleId() >= EleIconId_E::ELE_ID_SWORD && _arrEleIcon[i][j]->getEleId() <= EleIconId_E::ELE_ID_HEART)
				{
					auto pFlyIcon = FlyIcon::create(_arrEleIcon[i][j]->getEleId(), _arrEleIcon[i][j]->getEleLv());

					Vec2 worldPos = this->convertToWorldSpaceAR(_arrEleIcon[i][j]->getPosition());
					Vec2 iconPos = FightLayer::getInstance()->convertToNodeSpaceAR(worldPos);
					pFlyIcon->setPosition(iconPos);
					FightLayer::getInstance()->addChild(pFlyIcon, ZInFight_E::Z_F_FLY_ICON);
				}

				if ((2 == _arrCrushArrInfo[arrIndex].crushType || 1 == _arrCrushArrInfo[arrIndex].crushType)
					&& _arrCrushArrInfo[arrIndex].desIndex.row == i && _arrCrushArrInfo[arrIndex].desIndex.column == j)
				{
					if (2 == _arrCrushArrInfo[arrIndex].crushType)
					{
						_arrEleIcon[i][j]->doAddLv(0, 3);
					}
					else
					{
						int eleLv = (4 == _arrCrushArrInfo[arrIndex].crushCount) ? 1 : 2;
						_arrEleIcon[i][j]->doAddLv(0, eleLv);
					}
				}
				else
				{
					int animIndex = 0;

					if (3 == _arrCrushArrInfo[arrIndex].crushType)
					{
						animIndex = 3;
					}
					else if (_arrCrushArrInfo[arrIndex].arrLvCount[2] > 0)
					{
						animIndex = 2;
					}
					else if (_arrCrushArrInfo[arrIndex].arrLvCount[1] > 0)
					{
						animIndex = 1;
					}

					if (_arrEleIcon[i][j]->doCrush())
					{
						_arrEleIcon[i][j]->playCrushEffect(animIndex);
						this->countingEle(_arrEleIcon[i][j]->getEleId(), -1);
						_arrEleIcon[i][j] = nullptr;
					}
					else
					{
						_arrCrushArrIndex[i][j] = -1;
					}
				}

				/*hit*/
				if (i < (ParamData::CRUSH_ROW-1) && -1 == _arrCrushArrIndex[i+1][j] && nullptr != _arrEleIcon[i+1][j])
				{
					if (_arrEleIcon[i+1][j]->doHit())
					{
						_arrEleIcon[i+1][j] = nullptr;
					}
				}
				if (i > 0 && -1 == _arrCrushArrIndex[i-1][j] && nullptr != _arrEleIcon[i-1][j])
				{
					if (_arrEleIcon[i-1][j]->doHit())
					{
						_arrEleIcon[i-1][j] = nullptr;
					}
				}
				if (j > 0 && -1 == _arrCrushArrIndex[i][j-1] && nullptr != _arrEleIcon[i][j-1])
				{
					if (_arrEleIcon[i][j-1]->doHit())
					{
						_arrEleIcon[i][j-1] = nullptr;
					}
				}
				if (j < (ParamData::CRUSH_COL-1) && -1 == _arrCrushArrIndex[i][j+1] && nullptr != _arrEleIcon[i][j+1])
				{
					if (_arrEleIcon[i][j+1]->doHit())
					{
						_arrEleIcon[i][j+1] = nullptr;
					}
				}				
			}
// 			else if (-2 == _arrCrushArrIndex[i][j])
// 			{
// 				if (_arrEleIcon[i][j]->doInBottom())
// 				{
// 					_arrEleIcon[i][j] = nullptr;
// 				}				
// 			}
		}		
	}

	if (bRet)
	{
		this->runAction(Sequence::create(DelayTime::create(ParamData::ELE_FLY_DUR), CallFunc::create([=] {
			for (int i = 0; i < _curCrushArrCount; i++)
			{
				switch (_arrCrushArrInfo[i].crushEleId)
				{
				case 0:
					GameUtils::playEffect("xiaochu_lsj.ogg");
					break;
				case 1:
					GameUtils::playEffect("xiaochu_cbd.ogg");
					break;
				case 2:
					GameUtils::playEffect("xiaochu_lqc.ogg");
					break;
				case 3:
					GameUtils::playEffect("xiaochu_sqy.ogg");
					break;
				}

				Player::getInstance()->addAttInfo(_arrCrushArrInfo + i);
			}
		}), nullptr));
	}

	return bRet;
}


void CrushLayer::doCreate()
{
	/*
	for (int i=0; i<_curCrushArrCount; i++)
	{
		if (2 == _arrCrushArrInfo[i].crushType)
		{
			CrushIndex_T desIndex = _arrCrushArrInfo[i].desIndex;
//			_arrCrushArrIndex[desIndex.row][desIndex.column] = -1;

			Vec2 pos = CrushUtil::getElePos(desIndex.row, desIndex.column);
			auto pEle = EleIcon::create(EleIconId_E::ELE_ID_SUPER, 0);
			pEle->setPosition(pos);
			this->addChild(pEle);

			pEle->setPosIndex(desIndex);
			_arrEleIcon[desIndex.row][desIndex.column] = pEle;

			pEle->doShow();
		}
		else if (1 == _arrCrushArrInfo[i].crushType)
		{
			CrushIndex_T desIndex = _arrCrushArrInfo[i].desIndex;
//			_arrCrushArrIndex[desIndex.row][desIndex.column] = -1;

			int eleLv = (4 == _arrCrushArrInfo[i].crushCount) ?1 :2;

			Vec2 pos = CrushUtil::getElePos(desIndex.row, desIndex.column);
			auto pEle = EleIcon::create(_arrCrushArrInfo[i].crushEleId, eleLv);
			pEle->setPosition(pos);
			this->addChild(pEle);

			this->countingEle(_arrCrushArrInfo[i].crushEleId, 1);
			pEle->setPosIndex(desIndex);
			_arrEleIcon[desIndex.row][desIndex.column] = pEle;

			pEle->doShow();
		}
	}*/

	this->createNewCrushEle();
}


void CrushLayer::doFall()
{
	bool isFinish = true;
	float delayTime = 0.f;
	float actTime = ParamData::ELE_FALL_DUR;
	int arrCellState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};	//0:nor 1:reach from top,top right,top left 2:can't reach 3:can;t move
	memset((void *)arrCellState, 0, sizeof(arrCellState));

	for (int i=ParamData::CRUSH_ROW-1; i>=0; i--)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr != _arrEleIcon[i][j] && !_arrEleIcon[i][j]->isCanFall())
			{
				arrCellState[i][j] = 3;
			}
			else
			{
				if (i < (ParamData::CRUSH_ROW-1))
				{
					if (0 == j)
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j+1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}						
					}
					else if ((ParamData::CRUSH_COL-1) == j)
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j-1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}

					}
					else
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j+1] && 1<arrCellState[i+1][j-1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}						
					}					
				}				
			}
		}
	}

	do 
	{
		isFinish = true;

		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				if (nullptr == _arrEleIcon[i][j])
				{
					CCASSERT(3 != arrCellState[i][j], "line 512,3 != arrCellState[i][j]");
					/*is empty*/
					if (0 == arrCellState[i][j])
					{
						if ((ParamData::CRUSH_ROW-1) == i)
						{
							Vec2 pos = CrushUtil::getElePos(i+1, j);

							int eleId = _arrNewEleId[j][0];
							auto pEle = EleIcon::create(eleId, 0);
							pEle->setPosition(pos);
							this->addChild(pEle);

//							this->countingEle(eleId, 1);
							pEle->playMovePerUnit(0, delayTime, actTime, i, j);

							pEle->setPosIndex(i, j);
							_arrEleIcon[i][j] = pEle;
							_arrNewEleId[j].erase(_arrNewEleId[j].begin());

							isFinish = false;
						}
						else if (nullptr != _arrEleIcon[i+1][j])
						{
							/*top*/
							_arrEleIcon[i][j] = _arrEleIcon[i+1][j];
							_arrEleIcon[i+1][j] = nullptr;

							_arrEleIcon[i][j]->setPosIndex(i, j);
							_arrEleIcon[i][j]->playMovePerUnit(0, delayTime, actTime, i, j);
							isFinish = false;
						}
					}
					else if (1 == arrCellState[i][j] || 2 == arrCellState[i][j])
					{
						if (3 != arrCellState[i+1][j] && nullptr != _arrEleIcon[i+1][j])
						{
							/*top*/
							_arrEleIcon[i][j] = _arrEleIcon[i+1][j];
							_arrEleIcon[i+1][j] = nullptr;

							_arrEleIcon[i][j]->setPosIndex(i, j);
							_arrEleIcon[i][j]->playMovePerUnit(0, delayTime, actTime, i, j);
							isFinish = false;
						}
						else if (j>0 && nullptr != _arrEleIcon[i][j-1] && nullptr != _arrEleIcon[i+1][j-1] && 3 != arrCellState[i+1][j-1])
						{
							/*left top*/
							_arrEleIcon[i][j] = _arrEleIcon[i+1][j-1];
							_arrEleIcon[i+1][j-1] = nullptr;

							_arrEleIcon[i][j]->setPosIndex(i, j);
							_arrEleIcon[i][j]->playMovePerUnit(1, delayTime, actTime, i, j);
							isFinish = false;
						}
						else if (j<(ParamData::CRUSH_COL-1) && nullptr != _arrEleIcon[i][j+1] && nullptr != _arrEleIcon[i+1][j+1] && 3 != arrCellState[i+1][j+1])
						{
							/*right top*/
							_arrEleIcon[i][j] = _arrEleIcon[i+1][j+1];
							_arrEleIcon[i+1][j+1] = nullptr;

							_arrEleIcon[i][j]->setPosIndex(i, j);
							_arrEleIcon[i][j]->playMovePerUnit(2, delayTime, actTime, i, j);
							isFinish = false;
						}
					}
				}				
			}
		}

		delayTime += actTime;
	} while (!isFinish);

	if (delayTime > 0.f)
	{
		_isFallFinish = false;
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(delayTime), CallFunc::create([=](){_isFallFinish = true;})));
	}
	else
	{
		_isFallFinish = true;
	}	
}


bool CrushLayer::isMoveCanCrush()
{
	bool bRet = false;

	if (nullptr == _arrEleIcon[_touchIndex.row][_touchIndex.column]
	|| nullptr == _arrEleIcon[_moveIndex.row][_moveIndex.column])
	{
		return false;
	}
	

	int curEleId = _arrEleIcon[_touchIndex.row][_touchIndex.column]->getEleId();//_crushInfo.arrEleId[_curIndex.row][_curIndex.column];
	int desEleId = _arrEleIcon[_moveIndex.row][_moveIndex.column]->getEleId();//_crushInfo.arrEleId[_newIndex.row][_newIndex.column];


	if (curEleId == EleIconId_E::ELE_ID_SUPER || desEleId == EleIconId_E::ELE_ID_SUPER)
	{
		return true;
	}	

	int arrEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
//	memcpy((void *)arrEleId, (void *)_crushInfo.arrEleId, sizeof(arrEleId));

	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr == _arrEleIcon[i][j])
			{
				arrEleId[i][j] = -2;
			}
			else
			{
				arrEleId[i][j] = _arrEleIcon[i][j]->getEleId();
			}			
		}
	}
	

	{
		int tmp = arrEleId[_touchIndex.row][_touchIndex.column];
		arrEleId[_touchIndex.row][_touchIndex.column] = arrEleId[_moveIndex.row][_moveIndex.column];
		arrEleId[_moveIndex.row][_moveIndex.column] = tmp;
	}

	do 
	{
		{
			int rowStartIndex = _touchIndex.row;
			while ((rowStartIndex-1) >= 0)
			{
				int tmp = rowStartIndex-1;
				if (arrEleId[tmp][_touchIndex.column] == desEleId)
				{
					rowStartIndex = tmp;
				}
				else
				{
					break;
				}
			}

			int rowEndIndex = _touchIndex.row;
			while ((rowEndIndex+1) < ParamData::CRUSH_ROW)
			{
				int tmp = rowEndIndex+1;
				if (arrEleId[tmp][_touchIndex.column] == desEleId)
				{
					rowEndIndex = tmp;
				}
				else
				{
					break;
				}
			}

			if ((rowEndIndex-rowStartIndex) > 1)
			{
				bRet = true;
				break;
//				return true;			
			}


			int colStartIndex = _touchIndex.column;
			while ((colStartIndex-1) >= 0)
			{
				int tmp = colStartIndex-1;
				if (arrEleId[_touchIndex.row][tmp] == desEleId)
				{
					colStartIndex = tmp;
				}
				else
				{
					break;
				}
			}

			int colEndIndex = _touchIndex.column;
			while ((colEndIndex+1) < ParamData::CRUSH_COL)
			{
				int tmp = colEndIndex+1;
				if (arrEleId[_touchIndex.row][tmp] == desEleId)
				{
					colEndIndex = tmp;
				}
				else
				{
					break;
				}
			}

			if ((colEndIndex-colStartIndex) > 1)
			{
				bRet = true;
				break;
//				return true;
			}
		}

		{
			int rowStartIndex = _moveIndex.row;
			while ((rowStartIndex-1) >= 0)
			{
				int tmp = rowStartIndex-1;
				if (arrEleId[tmp][_moveIndex.column] == curEleId)
				{
					rowStartIndex = tmp;
				}
				else
				{
					break;
				}
			}

			int rowEndIndex = _moveIndex.row;
			while ((rowEndIndex+1) < ParamData::CRUSH_ROW)
			{
				int tmp = rowEndIndex+1;
				if (arrEleId[tmp][_moveIndex.column] == curEleId)
				{
					rowEndIndex = tmp;
				}
				else
				{
					break;
				}
			}

			if ((rowEndIndex-rowStartIndex) > 1)
			{
				bRet = true;
				break;
//				return true;			
			}


			int colStartIndex = _moveIndex.column;
			while ((colStartIndex-1) >= 0)
			{
				int tmp = colStartIndex-1;
				if (arrEleId[_moveIndex.row][tmp] == curEleId)
				{
					colStartIndex = tmp;
				}
				else
				{
					break;
				}
			}

			int colEndIndex = _moveIndex.column;
			while ((colEndIndex+1) < ParamData::CRUSH_COL)
			{
				int tmp = colEndIndex+1;
				if (arrEleId[_moveIndex.row][tmp] == curEleId)
				{
					colEndIndex = tmp;
				}
				else
				{
					break;
				}
			}

			if ((colEndIndex-colStartIndex) > 1)
			{
				bRet = true;
				break;
//				return true;
			}
		}
	} while (false);	

	return bRet;
}



bool CrushLayer::createNewCrushEle()
{
	bool bRet = false;
	int arrNewEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {EleIconId_E::ELE_ID_EMPTY};
	int arrEleState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
//	int arrFallCount[ParamData::CRUSH_COL] = {0};

	/*
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			arrNewEleId[i][j] = EleIconId_E::ELE_ID_EMPTY;
			if (-1 == _arrCrushArrIndex[i][j])
			{
				arrNewEleId[i-arrFallCount[j]][j] = _arrEleIcon[i][j]->getEleId();
				arrEleState[i-arrFallCount[j]][j] = 0;//_arrEleIcon[i][j]->getEleId();
			}
			else
			{
				arrFallCount[j]++;
			}			
		}
	}*/

	CrushIndex_T arrEleSrcIndex[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {-1};
	bool arrIsCanFall[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {true};
	memset((void *)arrEleSrcIndex, -1, sizeof(arrEleSrcIndex));

	/*simulateFall*/
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			arrIsCanFall[i][j] = true;
			if (nullptr == _arrEleIcon[i][j])
			{
				arrEleSrcIndex[i][j].row = -1;
				arrEleSrcIndex[i][j].column = -1;
			}
			else
			{
				arrEleSrcIndex[i][j].row = i;
				arrEleSrcIndex[i][j].column = j;

				if (!_arrEleIcon[i][j]->isCanFall())
				{
					arrIsCanFall[i][j] = false;
				}
			}
			/*
			if (-1 == _arrCrushArrIndex[i][j])
			{
				if (nullptr == _arrEleIcon[i][j])
				{
					arrEleSrcIndex[i][j].row = -1;
					arrEleSrcIndex[i][j].column = -1;
				}
				else
				{
					arrEleSrcIndex[i][j].row = i;
					arrEleSrcIndex[i][j].column = j;
				}				

				if (nullptr != _arrEleIcon[i][j] && !_arrEleIcon[i][j]->isCanFall())
				{
					arrIsCanFall[i][j] = false;
				}
			}	*/	
		}
	}
	CrushUtil::simulateFall(arrIsCanFall, arrEleSrcIndex);

	/*create new id*/
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (-1 == arrEleSrcIndex[i][j].row)
			{
				arrNewEleId[i][j] = EleIconId_E::ELE_ID_INVALID;
			}
			else if (arrEleSrcIndex[i][j].row >= ParamData::CRUSH_ROW)
			{
				arrNewEleId[i][j] = EleIconId_E::ELE_ID_EMPTY;
				bRet = true;
			}
			else
			{
				auto pEle = _arrEleIcon[arrEleSrcIndex[i][j].row][arrEleSrcIndex[i][j].column];
				arrNewEleId[i][j] = pEle->getEleId();
				if (pEle->isCrushEffective())
				{
					arrEleState[i][j] = 0;
				}
				else
				{
					arrEleState[i][j] = 1;
				}				
			}		
		}
	}

	if (!bRet)
	{
		return false;
	}	

	this->createNewEleId(arrNewEleId, arrEleState, 100, 0, 0);

	/*copy new id*/
	for (int i=ParamData::CRUSH_ROW-1; i>=0; i--)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if ((ParamData::CRUSH_ROW-1) == i)
			{
				int count = arrEleSrcIndex[i][j].row - (ParamData::CRUSH_ROW-1);

				count++;				
				_arrNewEleId[j].assign(count, -1);
			}
			
			if (arrEleSrcIndex[i][j].row > (ParamData::CRUSH_ROW-1))
			{
				int index = arrEleSrcIndex[i][j].row - ParamData::CRUSH_ROW;
				int col = arrEleSrcIndex[i][j].column;
				_arrNewEleId[col][index] = arrNewEleId[i][j];

				CCASSERT(arrNewEleId[i][j]>-1, "line 770 new ele id error");
			}
		}
	}
	
	return bRet;
}


void CrushLayer::createNewEleId(int (*arrEleId)[ParamData::CRUSH_COL], int (*arrEleState)[ParamData::CRUSH_COL], int sampleCount, int minLv, int maxLv)
{
//	int (*arrSampleEleId)[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = nullptr;
	int *arrWeight = nullptr;
	int *arrSampleIndex = nullptr;
	TravelseInfo_T *arrSampleInfo = nullptr;
	bool arrIsNew[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = { false };

//	arrSampleEleId = (int (*)[ParamData::CRUSH_ROW][ParamData::CRUSH_COL])malloc(sizeof(int)*sampleCount*ParamData::CRUSH_ROW*ParamData::CRUSH_COL);
	
	arrWeight = (int *)malloc(sizeof(int)*sampleCount);
	memset((void *)arrWeight, 0, sizeof(int)*sampleCount);

	arrSampleIndex = (int *)malloc(sizeof(int)*sampleCount);
	memset((void *)arrSampleIndex, 0, sizeof(int)*sampleCount);

	arrSampleInfo = (TravelseInfo_T *)malloc(sizeof(TravelseInfo_T)*sampleCount);
	memset((void *)arrSampleInfo, 0, sizeof(TravelseInfo_T)*sampleCount);


	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			int eleId = arrEleId[i][j];
			if (eleId>=EleIconId_E::ELE_ID_SWORD && eleId<=EleIconId_E::ELE_ID_HEART)
			{
				arrSampleInfo[0].arrEleCount[eleId]++;
			}			
		}
	}

	for (int k=0; k<sampleCount; k++)
	{
		arrSampleInfo[k].touchIndex.row = -1;
		arrSampleInfo[k].touchIndex.column = -1;
		arrSampleInfo[k].moveIndex.row = -1;
		arrSampleInfo[k].moveIndex.column = -1;
		
		memset((void *)arrSampleInfo[k].arrCrushArrIndex, -1, sizeof(arrSampleInfo[k].arrCrushArrIndex));
		
		if (0 != k)
		{
			memcpy((void *)arrSampleInfo[k].arrEleCount, (void *)arrSampleInfo[0].arrEleCount, sizeof(arrSampleInfo[0].arrEleCount));
		}		
	}	

	/*random ele id*/
	for (int i=0; i<ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			for (int k=0; k<sampleCount; k++)
			{
				int eleId = arrEleId[i][j];

				if (EleIconId_E::ELE_ID_EMPTY == eleId)
				{
					if (GameLayer::getInstance()->getIsGuiding())
					{
						eleId = EleIconId_E::ELE_ID_SUPER + 10;
					}
					else
					{
						eleId = CrushUtil::productEleId(arrSampleInfo[k].arrEleCount, i, j, nullptr);
					}
					arrSampleInfo[k].arrEleCount[eleId]++;

					arrIsNew[i][j] = true;
				}

				arrSampleInfo[k].arrEleId[i][j] = eleId;
				arrSampleInfo[k].arrEleWeakState[i][j] = arrEleState[i][j];
// 				if (eleId > EleIconId_E::INVALID && eleId <= EleIconId_E::HEART)
// 				{
// 					arrSampleInfo[k].arrEleCount[eleId]++;
// 				}
			}			
		}
	}


	/*travelse all ele*/
//	bool arrIsTravelsed[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {false};
	for (int k = 0; k < sampleCount; k++)
	{
//		memset((void *)arrIsTravelsed, 0, sizeof(bool)*ParamData::CRUSH_ROW*ParamData::CRUSH_COL);
		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				if (arrIsNew[i][j])
				{
					bool isCrush = CrushUtil::travelseEle(i, j, arrSampleInfo + k, false, 0);
					if (isCrush)
					{
						TravelseInfo_T *pCrushInfo = arrSampleInfo + k;
						int curCrushArrCount = pCrushInfo->crushArrCount;
						CrushArr_T *pCrushArr = pCrushInfo->arrCrushDetail + curCrushArrCount;

						pCrushArr->crushEleId = pCrushInfo->arrEleId[i][j];

						if (0 == pCrushArr->crushType)
						{
							if (pCrushArr->crushCount > 3)
							{
								pCrushArr->crushType = 1;
							}
						}

						pCrushInfo->crushArrCount++;
					}
				}	
			}
		}
	}

	/*calc weight*/
	for (int k=0; k<sampleCount; k++)
	{
		CrushArr_T *arrDetail = arrSampleInfo[k].arrCrushDetail;

		for (int i=0; i<arrSampleInfo[k].crushArrCount; i++)
		{
			CrushArr_T *pDetail = &(arrDetail[i]);
			arrWeight[k] += CrushUtil::calcCrushWeight(pDetail, false);
		}

		std::vector<int> arrSwapWeight;
		int (*arrEleIdTmp)[ParamData::CRUSH_COL] = arrSampleInfo[k].arrEleId;
		for (int i = 0; i < ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				if (arrIsNew[i][j])
				{
					int tmpWeight = 0;
					int maxWeight = 0;
					/*swap top*/
					if (i < (ParamData::CRUSH_ROW-1) && !arrIsNew[i+1][j] && arrEleIdTmp[i][j] != arrEleIdTmp[i+1][j])
					{
//						arrWeight[k] += this->calcSwapEleWeight(i, j, i+1, j, arrSampleInfo+k);
						tmpWeight = this->calcSwapEleWeight(i, j, i + 1, j, arrSampleInfo + k);
						maxWeight = MAX(maxWeight, tmpWeight);
					}
					/*swap bottom*/
					if (i > 0 && arrEleIdTmp[i][j] != arrEleIdTmp[i - 1][j])
					{
//						arrWeight[k] += this->calcSwapEleWeight(i, j, i - 1, j, arrSampleInfo + k);
						tmpWeight = this->calcSwapEleWeight(i, j, i - 1, j, arrSampleInfo + k);
						maxWeight = MAX(maxWeight, tmpWeight);
					}
					/*swap left*/
					if (j > 0 && !arrIsNew[i][j-1] && arrEleIdTmp[i][j] != arrEleIdTmp[i][j-1])
					{
//						arrWeight[k] += this->calcSwapEleWeight(i, j, i, j - 1, arrSampleInfo + k);
						tmpWeight = this->calcSwapEleWeight(i, j, i, j - 1, arrSampleInfo + k);
						maxWeight = MAX(maxWeight, tmpWeight);
					}
					/*swap right*/
					if (j < (ParamData::CRUSH_COL - 1) && arrEleIdTmp[i][j] != arrEleIdTmp[i][j+1])
					{
//						arrWeight[k] += this->calcSwapEleWeight(i, j, i, j + 1, arrSampleInfo + k);
						tmpWeight = this->calcSwapEleWeight(i, j, i, j + 1, arrSampleInfo + k);
						maxWeight = MAX(maxWeight, tmpWeight);
					}

					if (maxWeight > 0)
					{
						arrSwapWeight.push_back(maxWeight);
					}
				}

				int layoutWeight = CrushUtil::calcEleLayoutWeight(i, j, arrEleIdTmp);
				arrWeight[k] += layoutWeight;
			}
		}

		std::sort(arrSwapWeight.begin(), arrSwapWeight.end());
		float arrParam[] = { 1.f, 0.9f, 0.8f };
		for (int i = 0; i < 3 && i < arrSwapWeight.size(); i++)
		{
			arrWeight[k] += (arrSwapWeight[arrSwapWeight.size()-i-1] * arrParam[i]);
		}
	}

	/*sort*/
	for (int i=0; i<sampleCount; i++)
	{
		arrSampleIndex[i] = i;
	}	

	for (int i=0; i<sampleCount-1; i++)
	{
		for (int j=0; j<sampleCount-1-i; j++)
		{
			if (arrWeight[j] > arrWeight[j+1])
			{
				int tmp = arrWeight[j+1];
				arrWeight[j+1] = arrWeight[j];
				arrWeight[j] = tmp;

				tmp = arrSampleIndex[j+1];
				arrSampleIndex[j+1] = arrSampleIndex[j];
				arrSampleIndex[j] = tmp;
			}
		}		
	}

	//debug
	int tmp[100] = {0};
	memcpy(tmp, arrWeight, sizeof(int)*100);

	int sampleArrIndex = minLv;
	if (maxLv > minLv)
	{
		sampleArrIndex = ToolsUtil::getRandomInt(minLv, maxLv);
	}
	sampleArrIndex = arrSampleIndex[sampleArrIndex];

	memcpy((void *)arrEleId, (void *)(arrSampleInfo[sampleArrIndex].arrEleId), sizeof(int)*ParamData::CRUSH_ROW*ParamData::CRUSH_COL);
	
	free(arrWeight);
	free(arrSampleInfo);
	free(arrSampleIndex);
}


void CrushLayer::createWeakEle(int skillId, int count)
{
	int randomCount = 0;
	int curCount = 0;
	if (0 == skillId)
	{
		while (randomCount < 20 && curCount < count)
		{
			int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
			int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);
			EleIcon *pEle = _arrEleIcon[row][col];

			if (nullptr != pEle && pEle->obtainWeak(skillId))
			{
				curCount++;
			}

			randomCount++;
		}
	}
	else if (1 == skillId || 2 == skillId)
	{
		if (1 == count)
		{
			while (randomCount < 20 && curCount < count)
			{
				int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
				int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);
				EleIcon *pEle = _arrEleIcon[row][col];

				if (nullptr != pEle && pEle->obtainWeak(skillId))
				{
					curCount++;
				}

				randomCount++;
			}
		}
		else if (2 == count)
		{
			int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
			int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 2);

			EleIcon *pEle1 = _arrEleIcon[row][col];
			if (nullptr != pEle1)
			{
				pEle1->obtainWeak(skillId);
			}
			EleIcon *pEle2 = _arrEleIcon[row][col + 1];
			if (nullptr != pEle2)
			{
				pEle2->obtainWeak(skillId);
			}
		}
		else if (3 == count)
		{
			int row = ToolsUtil::getRandomInt(1, ParamData::CRUSH_ROW - 2);
			int col = ToolsUtil::getRandomInt(1, ParamData::CRUSH_COL - 2);
			int layoutType = ToolsUtil::getRandomInt(0, 1);
			int arr[3] = { -1, 0, 1 };
			
			for (int i = 0; i < 3; i++)
			{
				int x = row + arr[i];
				int y = col + arr[i];

				if (1 == layoutType)
				{
					y = arr[2 - i];
				}

				EleIcon *pEle = _arrEleIcon[x][y];
				if (nullptr != pEle)
				{
					pEle->obtainWeak(skillId);
				}
			}
		}
		else if (4 == count)
		{
			int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 2);
			int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 2);
			int arrX[4] = { 0, 1, 0, 1};
			int arrY[4] = { 0, 0, 1, 1};

			for (int i = 0; i < 4; i++)
			{
				int x = row + arrX[i];
				int y = col + arrY[i];

				EleIcon *pEle = _arrEleIcon[x][y];
				if (nullptr != pEle)
				{
					pEle->obtainWeak(skillId);
				}
			}
		}
		else if (5 == count)
		{
			int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 5);
			int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);

			for (int i = 0; i < 5; i++)
			{
				int x = row + i;
				int y = col;

				EleIcon *pEle = _arrEleIcon[x][y];
				if (nullptr != pEle)
				{
					pEle->obtainWeak(skillId);
				}
			}
		}
	}
}

void CrushLayer::createSpecialEle(int skillId, int count)
{
	int randomCount = 0;
	int curCount = 0;
	int row = 0;// ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
	int col = 0;// ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);

	do 
	{
		row = ToolsUtil::getRandomInt(1, ParamData::CRUSH_ROW - 2);
		col = ToolsUtil::getRandomInt(1, ParamData::CRUSH_COL - 2);

		auto pSpecInfo = ParamMgr::getInstance()->getEleSpecInfo(EleIconId_E::ELE_ID_SUPER + skillId + 1);
		if (pSpecInfo->isCanFall)
		{
			row = ParamData::CRUSH_ROW - 1;
		}

		EleIcon *pEle = _arrEleIcon[row][col];

		if (nullptr != pEle && nullptr == pEle->getWeakInfo() && nullptr == pEle->getSpecInfo() && pEle->getEleId() != EleIconId_E::ELE_ID_SUPER)
		{
			pEle->removeFromParent();
			_arrEleIcon[row][col] = nullptr;
			pEle = nullptr;

			Vec2 pos = CrushUtil::getElePos(row, col);
			pEle = EleIcon::create(skillId + 1 + EleIconId_E::ELE_ID_SUPER, 0);
			pEle->setPosition(pos);
			this->addChild(pEle);
			pEle->setPosIndex(row, col);
			_arrEleIcon[row][col] = pEle;

			curCount++;
		}

		randomCount++;
	} while (curCount < count && randomCount < 20);
}


void CrushLayer::addWeakEle(EleIcon *pEle)
{
	_arrWeakEle.pushBack(pEle);
}


void CrushLayer::removeWeakEle(EleIcon *pEle)
{
	_arrWeakEle.eraseObject(pEle);
}


void CrushLayer::countingEle(int eleId, int dt)
{
	if (eleId > EleIconId_E::ELE_ID_EMPTY && eleId < ELE_ID_SUPER)
	{
//		_arrCurEleCount[eleId] += dt;
		if (dt < 0)
		{
			_arrCrushEleCount[eleId] -= dt;

			if (0 == eleId)
			{
				MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::LUSANJIN, -dt);
			}
			else if (3 == eleId)
			{
				MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::SHENGQIUYUE, -dt);
			}

			FightLayer::getInstance()->countDungeonDes(eleId, -dt);
		}
	}	
}


bool CrushLayer::isStoneState(int eleId)
{
	return _arrIsStone[eleId];
}


void CrushLayer::setStoneState(int eleId, bool isStone)
{
	_arrIsStone[eleId] = isStone;
}


bool CrushLayer::isCanFall()
{
	bool bRet = false;
	int arrCellState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = { 0 };	//0:nor 1:reach from top,top right,top left 2:can't reach 3:can;t move
	memset((void *)arrCellState, 0, sizeof(arrCellState));

	for (int i = ParamData::CRUSH_ROW - 1; i >= 0; i--)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr != _arrEleIcon[i][j] && !_arrEleIcon[i][j]->isCanFall())
			{
				arrCellState[i][j] = 3;
			}
			else
			{
				if (i < (ParamData::CRUSH_ROW - 1))
				{
					if (0 == j)
					{
						if (1 < arrCellState[i + 1][j] && 1 < arrCellState[i + 1][j + 1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1 < arrCellState[i + 1][j])
						{
							arrCellState[i][j] = 1;
						}
					}
					else if ((ParamData::CRUSH_COL - 1) == j)
					{
						if (1 < arrCellState[i + 1][j] && 1 < arrCellState[i + 1][j - 1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1 < arrCellState[i + 1][j])
						{
							arrCellState[i][j] = 1;
						}

					}
					else
					{
						if (1 < arrCellState[i + 1][j] && 1 < arrCellState[i + 1][j + 1] && 1 < arrCellState[i + 1][j - 1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1 < arrCellState[i + 1][j])
						{
							arrCellState[i][j] = 1;
						}
					}
				}
			}

		}
	}

	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr == _arrEleIcon[i][j])
			{
				CCASSERT(3 != arrCellState[i][j], "line 512,3 != arrCellState[i][j]");
				/*is empty*/
				if (0 == arrCellState[i][j])
				{
					if ((ParamData::CRUSH_ROW - 1) == i)
					{
						bRet = true;
					}
					else if (nullptr != _arrEleIcon[i + 1][j])
					{
						/*top*/
						bRet = true;
					}
				}
				else if (1 == arrCellState[i][j] || 2 == arrCellState[i][j])
				{
					if (3 != arrCellState[i + 1][j] && nullptr != _arrEleIcon[i + 1][j])
					{
						/*top*/
						bRet = true;
					}
					else if (j > 0 && nullptr != _arrEleIcon[i][j - 1] && nullptr != _arrEleIcon[i + 1][j - 1] && 3 != arrCellState[i + 1][j - 1])
					{
						/*left top*/
						bRet = true;
					}
					else if (j < (ParamData::CRUSH_COL - 1) && nullptr != _arrEleIcon[i][j + 1] && nullptr != _arrEleIcon[i + 1][j + 1] && 3 != arrCellState[i + 1][j + 1])
					{
						/*right top*/
						bRet = true;
					}
				}

				if (bRet)
				{
					return bRet;
				}
			}
		}
	}

	return bRet;
}


EleIcon *CrushLayer::getEleIcon(int row, int column)
{
	if (row > -1 && row < ParamData::CRUSH_ROW && column > -1 && column < ParamData::CRUSH_COL)
	{
		return _arrEleIcon[row][column];
	}

	return nullptr;
}

Vector<EleIcon *> & CrushLayer::getWeakEle()
{
	return _arrWeakEle;
}

void CrushLayer::animationEffectEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE)
	{
		GameResPool::getInstance()->recycleRes(armature->getParent());
		armature->getParent()->removeFromParent();
	}
}


int CrushLayer::calcSwapEleWeight(int srcRow, int srcCol, int desRow, int desCol, TravelseInfo_T *pInfo)
{
	int retWeight = 0;
	TravelseInfo_T infoTmp = { 0 };

	CrushUtil::initTravelseInfo(&infoTmp);
	CrushUtil::copyTravelseInfo(&infoTmp, pInfo);
	swap(infoTmp.arrEleId[srcRow][srcCol], infoTmp.arrEleId[desRow][desCol]);
	
	bool isCrush = CrushUtil::travelseEle(srcRow, srcCol, &infoTmp, false, 0);
	if (isCrush)
	{
		int curCrushArrCount = infoTmp.crushArrCount;
		CrushArr_T *pCrushArr = infoTmp.arrCrushDetail + curCrushArrCount;

		pCrushArr->crushEleId = infoTmp.arrEleId[srcRow][srcCol];

		if (0 == pCrushArr->crushType)
		{
			if (pCrushArr->crushCount > 3)
			{
				pCrushArr->crushType = 1;
			}
		}

		infoTmp.crushArrCount++;
	}

	isCrush = CrushUtil::travelseEle(desRow, desCol, &infoTmp, false, 0);
	if (isCrush)
	{
		int curCrushArrCount = infoTmp.crushArrCount;
		CrushArr_T *pCrushArr = infoTmp.arrCrushDetail + curCrushArrCount;

		pCrushArr->crushEleId = infoTmp.arrEleId[desRow][desCol];

		if (0 == pCrushArr->crushType)
		{
			if (pCrushArr->crushCount > 3)
			{
				pCrushArr->crushType = 1;
			}
		}

		infoTmp.crushArrCount++;
	}

	CrushArr_T *arrDetail = infoTmp.arrCrushDetail;
	for (int i = 0; i < infoTmp.crushArrCount; i++)
	{
		CrushArr_T *pDetail = &(arrDetail[i]);
		retWeight += CrushUtil::calcCrushWeight(pDetail, true);
	}

	return retWeight;
}


bool CrushLayer::isCanSwapCrush()
{
	bool bRet = false;
	TravelseInfo_T arrTravelInfo = { 0 };
	CrushUtil::initTravelseInfo(&arrTravelInfo);

	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr == _arrEleIcon[i][j])
			{
				arrTravelInfo.arrEleId[i][j] = EleIconId_E::ELE_ID_INVALID;
			}
			else
			{
				arrTravelInfo.arrEleId[i][j] = _arrEleIcon[i][j]->getEleId();
			}
		}
	}

	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (nullptr != _arrEleIcon[i][j] && CrushUtil::isEleCanTouch(_arrEleIcon[i][j]))
			{
				/*swap top*/
				if (i < (ParamData::CRUSH_ROW - 1) && arrTravelInfo.arrEleId[i][j] != arrTravelInfo.arrEleId[i + 1][j] && nullptr != _arrEleIcon[i+1][j] && CrushUtil::isEleCanTouch(_arrEleIcon[i+1][j]))
				{
					int tmpWeight = this->calcSwapEleWeight(i, j, i + 1, j, &arrTravelInfo);
					if (tmpWeight > 0)
					{
						bRet = true;
						return bRet;
					}
				}
				/*swap right*/
				if (j < (ParamData::CRUSH_COL - 1) && arrTravelInfo.arrEleId[i][j] != arrTravelInfo.arrEleId[i][j+1] && nullptr != _arrEleIcon[i][j+1] && CrushUtil::isEleCanTouch(_arrEleIcon[i][j+1]))
				{
					int tmpWeight = this->calcSwapEleWeight(i, j, i, j + 1, &arrTravelInfo);
					if (tmpWeight > 0)
					{
						bRet = true;
						return bRet;
					}
				}
			}
		}
	}

	return bRet;
}