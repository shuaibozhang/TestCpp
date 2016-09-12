#include "AchieveLayer.h"
#include "GameCSLoader.h"
#include "VisibleRect.h"
#include "ParamData.h"
#include "AchieveMgr.h"
#include "PopRewardLayer.h"
#include "MainScene.h"


AchieveLayer::AchieveLayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/achievementui.plist");
}

AchieveLayer::~AchieveLayer()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/achievementui.plist");
}

bool AchieveLayer::init()
{
	LayerColor::initWithColor(Color4B(0, 0, 0, 0), 640.f, VisibleRect::top().y);

//	auto actionfade = FadeTo::create(0.5f, 200);
//	this->runAction(actionfade);
	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	this->setPositionY(-VisibleRect::top().y + 100.f);
	this->runAction(actionshow);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	float offsetY = (VisibleRect::top().y - 960.f) / 2.f;
	_pRoot = GameCSLoader::createNode("csb/achievement_layer.csb");
	this->addChild(_pRoot);
	_pRoot->setPositionY(offsetY);

	_isInit = true;
	for (int i = 0; i < ParamData::ACHIEVE_TOTAL_COUNT; i++)
	{
		this->updateAchieveItem(i);
	}
	_isInit = false;

	auto btnClose = dynamic_cast<ui::Button *>(_pRoot->getChildByName("main_bg")->getChildByName("bt_close"));
	btnClose->addClickEventListener([=](Ref *) {
		MainLayer::getCurMainLayer()->enterMainLayer();
		this->removeFromParent();
	});

	return true;
}

void AchieveLayer::updateAchieveItem(int idx)
{
	auto strItemName = __String::createWithFormat("item_%d", idx);
	auto itemNode = _pRoot->getChildByName("main_bg")->getChildByName(strItemName->getCString());
	int lv = AchieveMgr::getInstance()->getCurAchieveLv(idx);

	for (int i = 0; i < 3; i++)
	{
		auto strStar = __String::createWithFormat("star_%d", i);
		auto starNode = itemNode->getChildByName(strStar->getCString());
		if (i < lv)
		{
			starNode->setVisible(true);
		}
		else
		{
			starNode->setVisible(false);
		}
	}

	auto rewardNode = dynamic_cast<ui::Text *>(itemNode->getChildByName("reward_count"));
	int rewardCount = AchieveMgr::getInstance()->getCurAchieveReward(idx);
	auto strRewardCount = __String::createWithFormat("%d", rewardCount);
	rewardNode->setString(strRewardCount->getCString());

	int totalCount = AchieveMgr::getInstance()->getCurAchieveTotalCount(idx);
	auto contentNode = dynamic_cast<ui::Text *>(itemNode->getChildByName("item_content"));
	if (_isInit)
	{
		_arrStrContent[idx] = contentNode->getString().c_str();
	}
	auto strContent = __String::createWithFormat(_arrStrContent[idx].c_str(), totalCount);
	contentNode->setString(strContent->getCString());

	int curCount = AchieveMgr::getInstance()->getCurAchieveCount(idx);
	auto barText = dynamic_cast<ui::Text *>(itemNode->getChildByName("persent"));
	auto strBar = __String::createWithFormat("%d/%d", curCount, totalCount);
	barText->setString(strBar->getCString());

	auto barNode = dynamic_cast<ui::LoadingBar *>(itemNode->getChildByName("bar"));
	float percentValue = curCount * 100.f / totalCount;
	percentValue = MIN(percentValue, 100.f);
	barNode->setPercent(percentValue);

	auto btn = dynamic_cast<ui::Button *>(itemNode->getChildByName("bt_get"));
	if (lv < 3)
	{
		btn->setVisible(true);
		if (AchieveMgr::getInstance()->checkIsFinishAchieve(idx))
		{
			btn->setEnabled(true);
			btn->addClickEventListener([=](Ref *) {
				int reward = AchieveMgr::getInstance()->getCurAchieveReward(idx);
				auto layer = PopRewardLayer::create(ParamData::CRYSTAL_ITEM_ID, reward);
				this->addChild(layer, 1000);
				AchieveMgr::getInstance()->finishAchieceLv(idx);
				this->updateAchieveItem(idx);
			});
		}
		else
		{
			btn->setEnabled(false);
		}
	}
	else
	{
		btn->setVisible(false);
	}
}
