#include "MagPieLayer.h"
#include "VisibleRect.h"
#include "GameCSLoader.h"
#include "ParamData.h"
#include "MainScene.h"
#include "MagPieMgr.h"
#include "PopRewardLayer.h"
#include "GLCommon/Utils/JRTime.h"
#include "UserData.h"
#include "TimeLimitActivityMgr.h"

MagPieLayer::MagPieLayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/magpieui.plist");
}


MagPieLayer::~MagPieLayer()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/magpieui.plist");
}

bool MagPieLayer::init()
{
	LayerColor::initWithColor(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y*3);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	this->setPositionY(-VisibleRect::top().y + 100.f);
	this->runAction(actionshow);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	float offsetY = (VisibleRect::top().y - 960.f) / 2.f;
	_pRoot = GameCSLoader::createNode("csb/magpie_layer.csb");
	this->addChild(_pRoot);
	_pRoot->setPositionY(offsetY);
	
	this->updateMagPieTask(MagPieMgr::getInstance()->getDayIdx());

	auto btnClose = static_cast<ui::Button *>(_pRoot->getChildByName("bt_close"));
	btnClose->addClickEventListener([=](Ref *) 
	{
		MainLayer::getCurMainLayer()->enterMainLayer();
		this->removeFromParent();
	});

	UserData::getInstance()->saveUserData();

	return true;
}

void MagPieLayer::updateMagPieTask(int dayIdx)
{
	auto textFinishCount = static_cast<ui::Text *>(_pRoot->getChildByName("main_bg")->getChildByName("sub_bg_1")->getChildByName("task_fin_num"));
	auto btnGet = static_cast<ui::Button*>(_pRoot->getChildByName("main_bg")->getChildByName("sub_bg_1")->getChildByName("bt_get"));

	for (int i = 0; i < ParamData::MagPie_TOTAL_TASKS_NUM; i++)
	{
		auto strTaskContent = __String::createWithFormat("task_content_%d", i);
		auto taskContentNode = _pRoot->getChildByName("main_bg")->getChildByName("sub_bg_1")->getChildByName(strTaskContent->getCString());
		taskContentNode->setVisible(false);
	}


	if (dayIdx >= 0 && dayIdx < ParamData::MagPie_TOTAL_TASKS_NUM)
	{
		auto strTaskContent = __String::createWithFormat("task_content_%d", dayIdx);
		auto taskContentNode = _pRoot->getChildByName("main_bg")->getChildByName("sub_bg_1")->getChildByName(strTaskContent->getCString());
		taskContentNode->setVisible(true);

		textFinishCount->setVisible(true);
		auto strFinishedTaskNum = __String::createWithFormat("(%d/%d)", MagPieMgr::getInstance()->getCurFinishedMagPieGoalNum(dayIdx), MagPieMgr::getInstance()->getMagPieTaskGoalNum(dayIdx));
		textFinishCount->setString(strFinishedTaskNum->getCString());

		int taskState = MagPieMgr::getInstance()->getMissionState(dayIdx);

		btnGet->addClickEventListener([=](Ref*)
		{
			std::vector<PopItemInfo_T> arrReward;
			MagPieMgr::getInstance()->getMagPieReward(dayIdx, arrReward);
			auto layer = PopRewardLayer::create(arrReward);
			this->addChild(layer, 1000);

			MagPieMgr::getInstance()->setIsGotMagPieReward(dayIdx);
			btnGet->setVisible(false);
		});

		btnGet->setVisible(true);
		if (0 == taskState)
		{
			btnGet->setEnabled(false);
		}
		else if (1 == taskState)
		{
			btnGet->setEnabled(true);
		}
		else if (2 == taskState)
		{
			btnGet->setVisible(false);
		}
	}
	else
	{
		textFinishCount->setVisible(false);
		btnGet->setVisible(false);
	}
}

MidAutumnLayer::MidAutumnLayer():_pRoot(nullptr)
{
}

MidAutumnLayer::~MidAutumnLayer()
{
}

bool MidAutumnLayer::init()
{
	LayerColor::init();

	auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layer);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_pRoot = CSLoader::createNode("csb/moon_layer.csb");
	this->addChild(_pRoot);
	_pRoot->setPositionY((VisibleRect::top().y - 960.f) * 0.5f + 80.f);
	auto nodebtns = _pRoot->getChildByName("main_bg")->getChildByName("sub_bg_1");

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, (VisibleRect::top().y - 960.f) * 0.5f + 80.f)));
	_pRoot->setPositionY(-VisibleRect::top().y + 100);
	_pRoot->runAction(actionshow);

	auto btncancle = static_cast<ui::Button*>(_pRoot->getChildByName("bt_close"));
	btncancle->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	for (int i = 0; i < 4; i++)
	{
		auto name = String::createWithFormat("bt_get_%d", i)->getCString();
		_arrBtns[i] = static_cast<Button*>(nodebtns->getChildByName(name));
		_arrBtns[i]->addTouchEventListener([=](Ref* ref, Widget::TouchEventType type) {
			if (type != Widget::TouchEventType::ENDED)
			{
				return;
			}

			if (CommondActivityMagr::getInstance()->checkCanGetReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i) == 1)
			{
				CommondActivityMagr::getInstance()->getReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i);
				updateBtns();
			}
		});

		auto text = static_cast<Text*>(nodebtns->getChildByName(String::createWithFormat("task_fin_num_%d", i)->getCString()));
		text->setString(String::createWithFormat(text->getString().c_str(), CommondActivityMagr::getInstance()->getCurNumByNameAndIdx(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i))->getCString());
		int flag = CommondActivityMagr::getInstance()->checkCanGetReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i);

		if (flag != 0)
		{
			text->setColor(Color3B(0x33, 0x90, 0x05));
		}
		
	}
	updateBtns();
	return true;
}

void MidAutumnLayer::updateBtns()
{
	for (int i = 0; i < 4; i++)
	{
		if (CommondActivityMagr::getInstance()->checkCanGetReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i) == 1)
		{
			_arrBtns[i]->setEnabled(true);
			_arrBtns[i]->getChildByName("word_0")->setVisible(true);
			_arrBtns[i]->getChildByName("word_1")->setVisible(false);
		}
		else if (CommondActivityMagr::getInstance()->checkCanGetReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i) == 2)
		{
			_arrBtns[i]->setVisible(false);
		}
		else if (CommondActivityMagr::getInstance()->checkCanGetReward(CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN, i) == 0)
		{
			_arrBtns[i]->setEnabled(false);
			_arrBtns[i]->getChildByName("word_0")->setVisible(false);
			_arrBtns[i]->getChildByName("word_1")->setVisible(true);
		}
	}
}
