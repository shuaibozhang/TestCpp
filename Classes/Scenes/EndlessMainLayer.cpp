#include "EndlessMainLayer.h"
#include "NetDataMgr.h"
#include "MainScene.h"
#include "GameCSLoader.h"
#include "VisibleRect.h"
#include "ParamData.h"
#include "AchieveMgr.h"
#include "PopRewardLayer.h"
#include "UserData.h"
#include "EditNickLayer.h"


/***************************RankTableDataSrc*********************************************/

const float MAIN_CELL_SIZE_WIDTH = 530.f;
const float MAIN_CELL_SIZE_HEIGHT = 126.f;

RankTableDataSrc *RankTableDataSrc::create(EndlessMainLayer *pMainLayer)
{
	RankTableDataSrc *pRet = new RankTableDataSrc();
	if (pRet && pRet->init(pMainLayer))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}

	return pRet;
}


bool RankTableDataSrc::init(EndlessMainLayer *pMainLayer)
{
	bool bRet = false;

	do
	{
		m_pMainLayer = pMainLayer;
		bRet = true;
	} while (0);

	return bRet;
}


void RankTableDataSrc::scrollViewDidScroll(extension::ScrollView* view)
{
}
void RankTableDataSrc::scrollViewDidZoom(extension::ScrollView* view)
{
}

/*inherit from CCTableViewDelegate*/
void RankTableDataSrc::tableCellTouched(TableView* table, TableViewCell* cell)
{
	/*
	CCLabelTTF * runk = (CCLabelTTF *)cell->getChildByTag(RANK_TABLEVIEW_TAG_LV);
	__String runkString(runk->getString());
	if (m_pMainLayer->isVisible() == false || runkString.compare("LV.0") == 0 || runkString.compare("") == 0)
	{
		return;
	}
	int idx = cell->getIdx();
	((MainScene*)(m_pMainLayer->getParent()))->hideMainLayer();
	((MainScene*)(m_pMainLayer->getParent()))->showUserInfoLayer(idx);*/
}
void RankTableDataSrc::tableCellHighlight(TableView* table, TableViewCell* cell)
{
}
void RankTableDataSrc::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
}
void RankTableDataSrc::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
}

/*inherit from CCTableViewDataSource*/
Size RankTableDataSrc::cellSizeForTable(TableView *table)
{
	return Size(MAIN_CELL_SIZE_WIDTH, MAIN_CELL_SIZE_HEIGHT);
}
TableViewCell* RankTableDataSrc::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *pCell = table->dequeueCell();
	const RankBaseData_T *arrTopInfo = NetDataMgr::getInstance()->getRealScoreTop();

	if (NULL == pCell)
	{
		float x = 0.f;
		float y = 0.f;

		pCell = new TableViewCell();
		pCell->autorelease();

		auto pItem = GameCSLoader::createNode("csb/rankui_main_sub.csb");
		pCell->addChild(pItem);
		pItem->setPosition(12.f, 6.f);
		pItem->setTag(RANK_TABLEVIEW_TAG_BG);

		CCLOG("NULL == pCell idx=%i", idx);
	}

	if (NULL != arrTopInfo)
	{
		auto pItem = pCell->getChildByTag(RANK_TABLEVIEW_TAG_BG)->getChildByName("item");
		auto pRoleInfo = &arrTopInfo[idx];

		auto pHead = dynamic_cast<ImageView *>(pItem->getChildByName("head"));
		auto strImage = __String::createWithFormat("rank_head_%d.png", pRoleInfo->nRoleId);
		pHead->loadTexture(strImage->getCString(), TextureResType::PLIST);

		auto pFloor = dynamic_cast<ui::TextAtlas *>(pItem->getChildByName("cengshu_num"));
		auto strFloor = __String::createWithFormat("%d", pRoleInfo->nLevel);
		pFloor->setString(strFloor->getCString());

		auto pScore = dynamic_cast<ui::TextAtlas *>(pItem->getChildByName("jifen_num"));
		auto strScore = __String::createWithFormat("%d", pRoleInfo->nValue);
		pScore->setString(strScore->getCString());

		auto pNick = dynamic_cast<ui::Text *>(pItem->getChildByName("txt_name"));
		pNick->setString(pRoleInfo->strNick);

		auto pRank0 = dynamic_cast<ui::ImageView *>(pItem->getChildByName("rank_0"));
		auto pRank1 = dynamic_cast<ui::TextAtlas *>(pItem->getChildByName("rank_1"));
		if (idx < 3)
		{
			pRank0->setVisible(true);
			pRank1->setVisible(false);

			__String *strImgName = nullptr;
			if (0 == idx)
			{
				strImgName = __String::create("rank_first.png");
			}
			else if (1 == idx)
			{
				strImgName = __String::create("rank_second.png");
			}
			else if (2 == idx)
			{
				strImgName = __String::create("rank_third.png");
			}
			pRank0->loadTexture(strImgName->getCString(), TextureResType::PLIST);
		}
		else
		{
			pRank0->setVisible(false);
			pRank1->setVisible(true);

			auto strRank = __String::createWithFormat("%d", idx + 1);
			pRank1->setString(strRank->getCString());
		}
	}

	return pCell;
}
ssize_t RankTableDataSrc::numberOfCellsInTableView(TableView *table)
{
	return RANK_ROLE_COUNT;
}

/*****************************RankTableDataSrc End*******************************************/


EndlessMainLayer *EndlessMainLayer::s_pInstance = nullptr;

EndlessMainLayer * EndlessMainLayer::getInstance()
{
	return s_pInstance;
}

EndlessMainLayer::EndlessMainLayer()
{
	s_pInstance = this;
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/rankui.plist");
}

EndlessMainLayer::~EndlessMainLayer()
{
	s_pInstance = nullptr;
//	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/rankui.plist");
	CC_SAFE_RELEASE_NULL(_pRankTableDataSrc);
}

bool EndlessMainLayer::init()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	this->setPositionY(-VisibleRect::top().y + 100.f);
	this->runAction(actionshow);

	float offsetY = (VisibleRect::top().y - 960.f) / 2.f;
	auto pRoot = GameCSLoader::createNode("csb/rankui_main.csb");
	this->addChild(pRoot);
	pRoot->setPositionY(offsetY);


	auto pBtnClose = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_close"));
	pBtnClose->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			s_pInstance = nullptr;
			MainLayer::getCurMainLayer()->enterMainLayer();
			this->removeFromParent();
		}
	});

	auto pSelfNode = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_self"));
	pSelfNode->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto pLayer = EditNickLayer::create();
			this->addChild(pLayer, 10);
		}
	});

	_pSelfFloor = dynamic_cast<ui::TextAtlas *>(pSelfNode->getChildByName("cengshu_num"));
	_pSelfScore = dynamic_cast<ui::TextAtlas *>(pSelfNode->getChildByName("jifen_num"));
	_pSelfRank = dynamic_cast<ui::TextAtlas *>(pSelfNode->getChildByName("paming_num"));
	_pSelfText = dynamic_cast<ui::Text *>(pSelfNode->getChildByName("txt_name"));

	_pSelfHead = dynamic_cast<ui::ImageView *>(pSelfNode->getChildByName("head"));

//	this->updateSelfInfoUI();

	float tableViewWidth = MAIN_CELL_SIZE_WIDTH;
	float tableViewHeight = MAIN_CELL_SIZE_HEIGHT * 4;
	_pRankTableDataSrc = RankTableDataSrc::create(this);
	CC_SAFE_RETAIN(_pRankTableDataSrc);
	_pRankTV = extension::TableView::create(_pRankTableDataSrc, Size(tableViewWidth, tableViewHeight));
//	_pRankTV->setDirection(kCCScrollViewDirectionVertical);
	_pRankTV->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
	_pRankTV->setDelegate(_pRankTableDataSrc);
	_pRankTV->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_pRankTV->setPosition(Vec2((640.f- tableViewWidth)/2, 118.f));
	pRoot->addChild(_pRankTV);

	_pConnectDesc = Label::createWithSystemFont("connecting...", "", 25);
	pRoot->addChild(_pConnectDesc);
	_pConnectDesc->setPosition(Vec2((640.f - tableViewWidth) / 2, 118.f) + Vec2(tableViewWidth/2, tableViewHeight/2));

	_pCost0 = (pRoot->getChildByName("main_bg")->getChildByName("cost_0"));
	_pCost1 = (pRoot->getChildByName("main_bg")->getChildByName("cost_1"));


	auto pBtnAnnc = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_gonggao"));
	pBtnAnnc->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto pLayer = LayerColor::create(Color4B(0, 0, 0, 180));
			this->addChild(pLayer, 10);

			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);
			listener->onTouchBegan = [this](Touch*, Event*) { return true; };
			pLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pLayer);

			auto pRoot = GameCSLoader::createNode("csb/rankui_rule_layer.csb");
			pLayer->addChild(pRoot);
			pRoot->setPositionY((VisibleRect::top().y - 960.f) / 2.f);

			auto pBtn = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_know"));
			pBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
 				if (type == Widget::TouchEventType::ENDED)
 				{
 					pLayer->removeFromParent();
 				}
 			});
		}
	});
	pBtnAnnc->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.8f, 1.2f), ScaleTo::create(0.8f, 1.f))));

	auto pBtnGo = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_goin"));
	pBtnGo->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			int playTimes = UserData::getInstance()->getEndlessCount();
			int playCost = this->getEndlessCost();
			if (0 == playCost)
			{
				GameMap::getCurGameMap()->enterEndlessFight();
				UserData::getInstance()->setEndlessCount(playTimes+1);
				UserData::getInstance()->saveUserData();
				this->removeFromParent();
			}
			else
			{
				if (UserData::getInstance()->getCrystalNum() >= playCost)
				{
					GameMap::getCurGameMap()->enterEndlessFight();
					UserData::getInstance()->giveCrystal(-playCost);
					UserData::getInstance()->setEndlessCount(playTimes + 1);
					UserData::getInstance()->saveUserData();
					this->removeFromParent();
				}
				else
				{
					GameUtils::toastTip(0);
				}
			}
		}
	});

	if (!UserData::getInstance()->getEndlessGuided())
	{
		auto pLayer = EditNickLayer::create();
		this->addChild(pLayer, 10);
		pLayer->getBtnClose()->setVisible(false);
		/*
		auto pLayer = LayerColor::create(Color4B(0, 0, 0, 180));
		this->addChild(pLayer, 10);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [this](Touch*, Event*) { return true; };
		pLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pLayer);

		auto pRoot = GameCSLoader::createNode("csb/rankui_rule_layer.csb");
		pLayer->addChild(pRoot);
		pRoot->setPositionY((VisibleRect::top().y - 960.f) / 2.f);

		auto pBtn = dynamic_cast<ui::Button *>(pRoot->getChildByName("main_bg")->getChildByName("bt_know"));
		pBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				pLayer->removeFromParent();
			}
		});*/
	}


// 	auto pBg = dynamic_cast<ui::ImageView *>(pRoot->getChildByName("main_bg"));
// 	auto ctnSize = pBg->getContentSize();
// 	pBg->setContentSize(Size(ctnSize.width, ctnSize.height + offsetY*2)
	return true;
}

void EndlessMainLayer::onEnter()
{
	Layer::onEnter();

	NetDataMgr::getInstance()->updateRole(CC_CALLBACK_2(MainLayer::onUpdateSelfInfoCallback, MainLayer::getCurMainLayer()));

	this->updateSelfInfoUI();
	this->updateRankTableView();
}

void EndlessMainLayer::menuCloseCallback(cocos2d::Ref * pSender)
{
}

void EndlessMainLayer::updateRankTableView(void)
{
	if (NetDataMgr::getInstance()->getHasRealRank())
	{
		_pConnectDesc->setVisible(false);
		_pRankTV->setVisible(true);

		for (int i = 0; i < RANK_ROLE_COUNT; i++)
		{
			_pRankTV->updateCellAtIndex(i);
		}
	}
	else
	{
		_pConnectDesc->setVisible(true);
		_pRankTV->setVisible(false);
	}
}

void EndlessMainLayer::updateSelfInfoUI(void)
{
	int totalFloor = UserData::getInstance()->getTotalFloor();
	auto strFloor = __String::createWithFormat("%d", totalFloor);
	_pSelfFloor->setString(strFloor->getCString());

	int score = UserData::getInstance()->getWeeklyScore();
	auto strScore = __String::createWithFormat("%d", score);
	_pSelfScore->setString(strScore->getCString());

	int rankValue = NetDataMgr::getInstance()->getSelfScoreRank();
	auto strRank = __String::createWithFormat("%d", rankValue);
	_pSelfRank->setString(strRank->getCString());

	_pSelfText->setString(UserData::getInstance()->getNickName());

	int roleId = UserData::getInstance()->getHeadId();
	auto strImage = __String::createWithFormat("rank_head_%d.png", roleId);
	_pSelfHead->loadTexture(strImage->getCString(), TextureResType::PLIST);

	if (0 == UserData::getInstance()->getEndlessCount())
	{
		_pCost0->setVisible(true);
		_pCost1->setVisible(false);
	}
	else
	{
		_pCost0->setVisible(false);
		_pCost1->setVisible(true);

		auto pCostText = dynamic_cast<ui::Text *>(_pCost1->getChildByName("cost"));
		auto strCost = __String::createWithFormat("%d", this->getEndlessCost());
		pCostText->setString(strCost->getCString());
	}
}

int EndlessMainLayer::getEndlessCost()
{
	int ret = 0;
	int endlessCount = UserData::getInstance()->getEndlessCount();

	ret = 10 * endlessCount;

	return ret;
}


void EndlessMainLayer::update(float delta)
{
}
