#include "BoxLayer.h"
#include "UserData.h"
#include "Widgets/GameButton.h"
#include "TimeCountDownNode.h"
#include "VisibleRect.h"
#include "ParamMgr.h"
#include "MainScene.h"
#include "../Widgets/GameCSLoader.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BoxDataMgr.h"
#include "PopRewardLayer.h"
#include "GameGuideLayer.h"
#include "platBridge/cocos2dx_plat.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "../Store/PurchaseLayer.h"
#include "../StoreBridge/StoreAssets.h"
#include "DayActiveMgr.h"

const char* g_boxBtnPicName[7] = {"boxlayer_boxicon_0.png", "boxlayer_boxicon_1.png", "boxlayer_boxicon_2.png", "boxlayer_boxicon_3.png", "store_boxicon_jxbx.png", "store_boxicon_sqbx.png", "store_boxicon_cqbx.png"};
const char* g_boxNamePicName[7] = { "boxlayer_boxname_0.png", "boxlayer_boxname_1.png", "boxlayer_boxname_2.png", "boxlayer_boxname_3.png", "store_textbox_0.png", "store_textbox_1.png", "store_textbox_2.png" };

BoxLayer::BoxLayer():_unLockingTime(nullptr),
	_unLockingCost(nullptr),
	_getcost(nullptr),
	_selectPopBoxIdx(-1),
	_freeGetTimeIdx(-1)
{
}

BoxLayer::~BoxLayer()
{
	
}

bool BoxLayer::init()
{
	Layer::init();
	_boxPos[0] = Vec2(165.f, 395.f);
	_boxPos[1] = Vec2(460.f, 395.f);
	_boxPos[2] = Vec2(165.f, 185.f);
	_boxPos[3] = Vec2(460.f, 185.f);
	_lastTimeState[0] = -1;
	_lastTimeState[1] = -1;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto root = GameCSLoader::createNode("csb/boxlayer.csb");
	this->addChild(root);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	root->setPositionY(-VisibleRect::top().y + 100.f);
	root->runAction(actionshow);

	auto toptimenode = root->getChildByName("Node_3");
	//tiem
	_hour_0 = static_cast<TextAtlas*>(toptimenode->getChildByName("countdown_0")->getChildByName("hour"));
	_hour_1 = static_cast<TextAtlas*>(toptimenode->getChildByName("countdown_1")->getChildByName("hour"));

	_min_0 = static_cast<TextAtlas*>(toptimenode->getChildByName("countdown_0")->getChildByName("min"));
	_min_1 = static_cast<TextAtlas*>(toptimenode->getChildByName("countdown_1")->getChildByName("min"));

	_timedanwei[0] = static_cast<Sprite*>(toptimenode->getChildByName("countdown_0")->getChildByName("text_xs_31"));
	_timedanwei[1] = static_cast<Sprite*>(toptimenode->getChildByName("countdown_0")->getChildByName("text_fz_32"));
	_timedanwei[2] = static_cast<Sprite*>(toptimenode->getChildByName("countdown_1")->getChildByName("text_xs_31"));
	_timedanwei[3] = static_cast<Sprite*>(toptimenode->getChildByName("countdown_1")->getChildByName("text_fz_32"));

	_texttouch_0 = static_cast<Sprite*>(toptimenode->getChildByName("touch_0"));
	_texttouch_1 = static_cast<Sprite*>(toptimenode->getChildByName("touch_1"));

	_timeNode0 = toptimenode->getChildByName("countdown_0");
	_timeNode1 = toptimenode->getChildByName("countdown_1");

	_winInfoRoot = toptimenode->getChildByName("Node_15");
	_winTimesBar = static_cast<LoadingBar*>(_winInfoRoot->getChildByName("LoadingBar_1"));
	_winTimeText = static_cast<Text*>(_winInfoRoot->getChildByName("Text_1"));

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/texteffect.ExportJson");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/storeui.plist");

	_ani0 = Armature::create("texteffect");
	_ani0->getAnimation()->play("text_0");
	toptimenode->addChild(_ani0);
	_ani0->setPosition(_texttouch_0->getPosition());
	_ani0->getAnimation()->setSpeedScale(0.6f);
	_ani0->setVisible(false);

	_ani1 = Armature::create("texteffect");
	_ani1->getAnimation()->play("text_1");
	toptimenode->addChild(_ani1);
	_ani1->setPosition(_texttouch_1->getPosition());
	_ani1->getAnimation()->setSpeedScale(0.6f);

	static_cast<Button*>(toptimenode->getChildByName("touchget_0"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			/*auto main = MainLayer::getCurMainLayer();
			auto node0 = MainLayer::getCurMainLayer()->getTimeNode(0);
			if (!node0->_getOnlineTime)
			{
				GameUtils::toastTip("box_net_err");
				return;
			}

			if (node0->getDur() > 0)
			{
				return;
			}
			node0->setDur(3 * 60 * 60);
			getReward(0, BoxDataMgr::getInstance()->getCurGetBoxLv(), 2);*/

			getFreeBox();
		}
	});

	static_cast<Button*>(toptimenode->getChildByName("touchget_1"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto main = MainLayer::getCurMainLayer();
			auto node1 = MainLayer::getCurMainLayer()->getTimeNode(1);

			if (UserData::getInstance()->getBoxLayerStage(4) == 2)
			{
				if (!node1->isGetOnlineTime())
				{
					GameUtils::toastTip("box_net_err");
					return;
				}

				UserData::getInstance()->setBoxLayerStage(4, 1);
				UserData::getInstance()->setWinTimes(0);
				UserData::getInstance()->saveUserData();
				node1->setDur(12 * 60 * 60);
				getReward(2, BoxDataMgr::getInstance()->getCurGetBoxLv());

				_texttouch_1->setVisible(false);
				_timeNode1->setVisible(true);
				_winInfoRoot->setVisible(false);
				_ani1->setVisible(false);
			}
			else if(UserData::getInstance()->getBoxLayerStage(4) == 1)
			{
				if (node1->getDur() > 0)
				{
					return;
				}
				else
				{
					UserData::getInstance()->setBoxLayerStage(4, 0);
					UserData::getInstance()->saveUserData();
				}
			}
			else
			{
				GameUtils::toastTip("top_box_info");
				CCLOG("top_box_info");
			}
		}
	});

	auto btncancle = static_cast<Button*>(root->getChildByName("Node_2")->getChildByName("btn_exit"));
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			MainLayer::getCurMainLayer()->enterMainLayer();
			this->removeFromParent();
		}
	});
	//fix bg size
	static_cast<ImageView*>(root->getChildByName("Image_44"))->setContentSize(Size(640.f, VisibleRect::top().y - 100.f));
	static_cast<ImageView*>(root->getChildByName("Image_1"))->setContentSize(Size(531.f, VisibleRect::top().y - 165.f));

	for (int i = 0; i < ParamData::BOX_LAYER_SEAT_COUNT; i++)
	{
		auto& configbox = UserData::getInstance()->getBoxLayerConfig(i);
		if (configbox.boxidx != -1)
		{
			auto boxroot = GameCSLoader::createNode("csb/boxnode.csb");
			root->getChildByName("Node_4")->addChild(boxroot);
			boxroot->setPosition(_boxPos[i]);
			_boxGameBtn[i] = boxroot;

			auto btn = static_cast<Button*>((boxroot)->getChildByName("Button_1"));
			btn->setUserData(((void*)(i)));
			btn->addTouchEventListener(CC_CALLBACK_2(BoxLayer::menuOnBox, this));
			btn->loadTextures(g_boxBtnPicName[configbox.boxidx], "", "", Widget::TextureResType::PLIST);

			auto node1 = (boxroot)->getChildByName("Node_1");
			auto lvsp = static_cast<Sprite*>(node1->getChildByName("boxlayer_text_lv_0_4"));
			lvsp->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("boxlayer_text_lv_%d.png", configbox.boxlv)->getCString()));
			
			auto namesp = static_cast<Sprite*>(node1->getChildByName("boxlayer_boxname_0_5"));
			namesp->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_boxNamePicName[configbox.boxidx]));
		}
		else
		{
			_boxGameBtn[i] = nullptr;
		}
	}
	_curtimenode = MainLayer::getCurMainLayer()->getBoxTimeNode();

	if (_curtimenode && UserData::getInstance()->getIsBoxUnlock() == -1)
	{
		//int pos = UserData::getInstance()->getIsBoxUnlock();
		//int boxidx = UserData::getInstance()->getBoxLayerConfig(pos).boxidx;
		//_curtimenode->setDur(BoxDataMgr::getInstance()->getWaitTime(boxidx));
	}

	auto action = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
		clik();
	}));

	this->runAction(RepeatForever::create(action));

	//fix pos
	toptimenode->setPositionY(VisibleRect::top().y - 960.f);
	root->getChildByName("Node_2")->setPositionY(VisibleRect::top().y - 960.f);
	root->getChildByName("Node_4")->setPositionY(VisibleRect::top().y - 960.f);

	if (nullptr != GameGuideLayer::getInstance())
	{
		GameGuideLayer::getInstance()->guideStepStart(1, _boxGameBtn[0]->convertToWorldSpace(Vec2(0, 35.f)) - Vec2(0.f, -VisibleRect::top().y + 100.f));
	}

	clik();
	updateBoxOpen();
	return  true;
}

void BoxLayer::switchBoxLeftStage()
{
	if (MainLayer::getCurMainLayer()->getIsGetOnlineTime())
	{
		int curhour = MainLayer::getCurMainLayer()->getCurTime() / 3600;
		CCLOG("curhour %d", curhour);

		_freeGetTimeIdx = -1;
		if (curhour >=7 && curhour < 9 )
		{
			_freeGetTimeIdx = 0;
		}
		else if (curhour >= 12 && curhour < 14)
		{
			_freeGetTimeIdx = 1;
		}
		else if (curhour >= 18 && curhour < 20)
		{
			_freeGetTimeIdx = 2;
		}
		else
		{
			_freeGetTimeIdx = -1;
		}

		if (_freeGetTimeIdx != -1 && UserData::getInstance()->getTiliRewardTimes(_freeGetTimeIdx) == 0)
		{
			_timeNode0->setVisible(false);
			_ani0->setVisible(true);
			_texttouch_0->setVisible(true);
		}
		else
		{
			_timeNode0->setVisible(true);
			_ani0->setVisible(false);
			_texttouch_0->setVisible(false);
		}

		if (_timeNode0->isVisible())
		{
			int picidx = 0;
			if (curhour < 7)
			{
				picidx = 0;
			}
			else if (curhour < 12)
			{
				picidx = 1;
			}
			else if (curhour < 18)
			{
				picidx = 2;
			}
			static_cast<Sprite*>(_timeNode0->getChildByName("timepic"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("boxlayer_time_%d.png", picidx)->getCString()));
		}
	}
}

void BoxLayer::getFreeBox()
{
	if (MainLayer::getCurMainLayer()->getIsGetOnlineTime())
	{
		if (_freeGetTimeIdx != -1 && UserData::getInstance()->getTiliRewardTimes(_freeGetTimeIdx) == 0)
		{
			getReward(7, BoxDataMgr::getInstance()->getCurGetBoxLv(), 2);
			UserData::getInstance()->setTiliRewardTimes(_freeGetTimeIdx, 1);

			DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYGETTILI, 1, false);

			UserData::getInstance()->saveUserData();
		}
		else
		{
			GameUtils::toastTip("box_time_err");		
		}
	}
	else
	{
		GameUtils::toastTip("box_net_err");
	}
}


void BoxLayer::updateBoxOpen()
{
	_unLockingTime = nullptr;
	_unLockingCost = nullptr;

	for (int i = 0; i < ParamData::BOX_LAYER_SEAT_COUNT; i++)
	{
		auto stage = UserData::getInstance()->getBoxLayerStage(i);
		if (_boxGameBtn[i])
		{
			if (stage == 0)
			{

				_boxGameBtn[i]->getChildByName("Node_2")->setVisible(false);
				_boxGameBtn[i]->getChildByName("Node_1")->setVisible(true);

			}
			else if (stage == 1)
			{
				_boxGameBtn[i]->getChildByName("Node_2")->setVisible(true);
				_boxGameBtn[i]->getChildByName("Node_1")->setVisible(false);

				_unLockingTime = static_cast<TextAtlas*>(_boxGameBtn[i]->getChildByName("Node_2")->getChildByName("time"));
				_unLockingTime->setString("");
				_unLockingCost = static_cast<TextAtlas*>(_boxGameBtn[i]->getChildByName("Node_2")->getChildByName("cost"));
				_unLockingCost->setString("");
			}
			else if (stage == 2)
			{
				_boxGameBtn[i]->getChildByName("Node_2")->setVisible(false);
				_boxGameBtn[i]->getChildByName("Node_1")->setVisible(false);

				if (_boxGameBtn[i]->getChildByName("ljdksp") == nullptr)
				{
					auto sprite = Sprite::createWithSpriteFrameName("text_djdk.png");
					_boxGameBtn[i]->addChild(sprite, 1);
					sprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f))));
					sprite->setPosition(Vec2(12.f, -30.f));
					sprite->setName("ljdksp");
				}
				
			}
		}	
	}

	if (UserData::getInstance()->getWinTime() >= 10 && UserData::getInstance()->getBoxLayerStage(4) == 0)
	{
		_texttouch_1->setVisible(true);
		_timeNode1->setVisible(false);
		_winInfoRoot->setVisible(false);
		_ani1->setVisible(true);

		UserData::getInstance()->setBoxLayerStage(4, 2);
		UserData::getInstance()->saveUserData();
	}

	auto topboxstate = UserData::getInstance()->getBoxLayerStage(4);
	if (topboxstate == 0)
	{
		_texttouch_1->setVisible(false);
		_timeNode1->setVisible(false);
		_winInfoRoot->setVisible(true);
		_ani1->setVisible(false);

		int curwintime = UserData::getInstance()->getWinTime() > 10 ? 10 : UserData::getInstance()->getWinTime();
		_winTimeText->setString(String::createWithFormat("%d/10", curwintime)->getCString());
		_winTimesBar->setPercent((curwintime / 10.f )* 100);
	}
	else if (topboxstate == 1)
	{
		_texttouch_1->setVisible(false);
		_timeNode1->setVisible(true);
		_winInfoRoot->setVisible(false);
		_ani1->setVisible(false);
	}
	else if (topboxstate == 2)
	{
		_texttouch_1->setVisible(false);
		_timeNode1->setVisible(false);
		_winInfoRoot->setVisible(false);
		_ani1->setVisible(true);
	}
}


void BoxLayer::resetBoxInfo()
{
	_getcost = nullptr;
	_selectPopBoxIdx = -1;
}

void BoxLayer::showBoxInfo(int idx)
{
	_selectPopBoxIdx = idx;

	auto &boxconfig = UserData::getInstance()->getBoxLayerConfig(idx);
	int stage = UserData::getInstance()->getBoxLayerStage(idx);

	auto bglayer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(bglayer);

	auto info = GameCSLoader::createNode("csb/boxinfonode.csb");
	bglayer->addChild(info);
	info->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [=](Touch*, Event*) {
		resetBoxInfo();
		bglayer->removeFromParent();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, info);

	auto gold = static_cast<TextAtlas*>(info->getChildByName("gold"));
	auto parts = static_cast<TextAtlas*>(info->getChildByName("parts"));
	auto spparts = static_cast<TextAtlas*>(info->getChildByName("more")->getChildByName("sp_parts"));

	auto btnstart = static_cast<Button*>(info->getChildByName("btn_start"));
	auto btnopen = static_cast<Button*>(info->getChildByName("btn_open"));
	
	if (nullptr != GameGuideLayer::getInstance())
	{
		GameGuideLayer::getInstance()->guideStepStart(1, btnstart->convertToWorldSpace(Vec2(86.f, 40.f)));
	}

	btnstart->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			UserData::getInstance()->setIsBoxGuided(true);
			bool needguide = false;
			if (nullptr != GameGuideLayer::getInstance())
			{
				GameGuideLayer::getInstance()->guideStepEnd();
				GameGuideLayer::getInstance()->guideStepStart(1, btnopen->convertToWorldSpace(Vec2(86.f, 40.f)));
				needguide = true;
			}

			_curtimenode = MainLayer::getCurMainLayer()->getBoxTimeNode();

			if (needguide == false && _curtimenode->isGetOnlineTime() == false)
			{
				GameUtils::toastTip("box_net_err");
				return;
			}


			btnstart->setVisible(false);
			btnopen->setVisible(true);
			
			auto _isunlockidx = idx;
			UserData::getInstance()->setBoxUnlock(_isunlockidx);
			UserData::getInstance()->setBoxLayerStage(idx, 1);
			UserData::getInstance()->saveUserData(true);

			MainLayer::getCurMainLayer()->addBoxTimeNode(BoxDataMgr::getInstance()->getWaitTime(boxconfig.boxidx));
			
			if (_curtimenode && _getcost)
			{
				if (UserData::getInstance()->getIsBoxUnlock() == _selectPopBoxIdx)
				{
					auto timecur = _curtimenode->getDur();
					_getcost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur))->getCString());
				}
				else
				{
					auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(UserData::getInstance()->getBoxLayerConfig(_selectPopBoxIdx).boxidx);
					_getcost->setString(String::createWithFormat("%d", pBoxInfo->openCost)->getCString());
				}
				//auto timecur = _curtimenode->getDur();
				//_getcost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur))->getCString());
			}

			updateBoxOpen();
		}
	});

	btnopen->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto timecur = _curtimenode->getDur();
			int openCost = 0;

			if (UserData::getInstance()->getIsBoxUnlock() == _selectPopBoxIdx)
			{
				auto timecur = _curtimenode->getDur();
				openCost = BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur);
			}
			else
			{
				auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(UserData::getInstance()->getBoxLayerConfig(_selectPopBoxIdx).boxidx);
				openCost = pBoxInfo->openCost;
			}

			if (nullptr != GameGuideLayer::getInstance())
			{
				UserData::getInstance()->giveCrystal(openCost);
			}

			if (UserData::getInstance()->getBoxLayerStage(_selectPopBoxIdx) == 2 && UserData::getInstance()->getBoxLayerConfig(_selectPopBoxIdx).boxidx != -1)
			{
				openCost = 0;
			}

			if (openCost <= UserData::getInstance()->getCrystalNum())
			{
				UserData::getInstance()->giveCrystal(-openCost);
				if (UserData::getInstance()->getIsBoxUnlock() == idx)
				{
					if (nullptr != GameGuideLayer::getInstance())
					{
						GameGuideLayer::getInstance()->guideStepEnd();
					}
					UserData::getInstance()->setBoxUnlock(-1);
					UserData::getInstance()->setBoxLayerStage(idx, 0);

					_boxGameBtn[idx]->removeFromParent();
					_boxGameBtn[idx] = nullptr;

					getReward(boxconfig.boxidx, boxconfig.boxlv, 1);

					BoxInStoreConfig_T temp = { -1, 0 };
					UserData::getInstance()->setBoxLayerConfig(idx, temp);
					UserData::getInstance()->saveUserData(true);

					updateBoxOpen();

					resetBoxInfo();
					bglayer->removeFromParent();
				}
				else
				{
					UserData::getInstance()->setBoxLayerStage(idx, 0);
					_boxGameBtn[idx]->removeFromParent();
					_boxGameBtn[idx] = nullptr;

					BoxInStoreConfig_T temp = { -1, 0 };
					UserData::getInstance()->setBoxLayerConfig(idx, temp);
					UserData::getInstance()->saveUserData(true);

					getReward(boxconfig.boxidx, boxconfig.boxlv, 1);
					updateBoxOpen();

					resetBoxInfo();
					bglayer->removeFromParent();
				}
			}
			else
			{
				GameUtils::toastTip("toast_tip_0");
			}
			
		}
	});

	auto starttime = static_cast<TextAtlas*>(btnstart->getChildByName("time"));
	_getcost = static_cast<TextAtlas*>(btnopen->getChildByName("cost"));

	auto danwei = static_cast<Sprite*>(btnstart->getChildByName("Sprite_31"));

	auto lvsprite = static_cast<Sprite*>(info->getChildByName("lv"));
	auto iconsprite = static_cast<Sprite*>(info->getChildByName("icon"));
	auto namesprite = static_cast<Sprite*>(info->getChildByName("name"));

	lvsprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("boxlayer_text_lv_%d.png", boxconfig.boxlv)->getCString()));
	namesprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_boxNamePicName[boxconfig.boxidx]));
	iconsprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_boxBtnPicName[boxconfig.boxidx]));

	int mingold = BoxDataMgr::getInstance()->getBoxGoldCount(boxconfig.boxidx, boxconfig.boxlv, 0);
	int maxgold = BoxDataMgr::getInstance()->getBoxGoldCount(boxconfig.boxidx, boxconfig.boxlv, 1);
	gold->setString(String::createWithFormat("%d.%d",  mingold, maxgold)->getCString());

	int minparts = BoxDataMgr::getInstance()->getBoxAboveChipCount(boxconfig.boxidx, boxconfig.boxlv, 0, 0);
	int maxparts = BoxDataMgr::getInstance()->getBoxAboveChipCount(boxconfig.boxidx, boxconfig.boxlv, 0, 1);

	parts->setString(String::createWithFormat("%d.%d", minparts, maxparts)->getCString());
	spparts->setString(String::createWithFormat("/%d", BoxDataMgr::getInstance()->getBoxAboveChipCount(boxconfig.boxidx, boxconfig.boxlv, 2, 1))->getCString());

	if (int(BoxDataMgr::getInstance()->getWaitTime(boxconfig.boxidx) / 3600) == 0)
	{
		starttime->setString(String::createWithFormat("%d", int(BoxDataMgr::getInstance()->getWaitTime(boxconfig.boxidx) / 60))->getCString());
		danwei->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("boxlayer_text_fz.png"));
	}
	else
	{
		starttime->setString(String::createWithFormat("%d", int(BoxDataMgr::getInstance()->getWaitTime(boxconfig.boxidx) / 3600))->getCString());
	}
	

	if (_curtimenode)
	{
		if (UserData::getInstance()->getIsBoxUnlock() == _selectPopBoxIdx)
		{
			auto timecur = _curtimenode->getDur();
			_getcost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur))->getCString());
		}
		else
		{
			auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(UserData::getInstance()->getBoxLayerConfig(_selectPopBoxIdx).boxidx);
			_getcost->setString(String::createWithFormat("%d", pBoxInfo->openCost)->getCString());
		}
	}
	
	//
	if (stage == 0)
	{
		btnopen->setVisible(false);
		btnstart->setVisible(true);
	}
	// when is daojishi
	else if (stage == 1)
	{
		btnopen->setVisible(true);
		btnstart->setVisible(false);
	}

	if (UserData::getInstance()->getIsBoxUnlock() != -1)
	{
		btnopen->setVisible(true);
		btnstart->setVisible(false);
	}
}


void BoxLayer::menuOnBox(Ref* ref, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int idx = int(((Node*)ref)->getUserData());
		int curtimeidx = UserData::getInstance()->getIsBoxUnlock();
		
			if (UserData::getInstance()->getBoxLayerStage(idx) == 2)
			{
				UserData::getInstance()->setBoxLayerStage(idx, 0);

				_boxGameBtn[idx]->removeFromParent();
				_boxGameBtn[idx] = nullptr;
		
				auto boxconfig = UserData::getInstance()->getBoxLayerConfig(idx);
				getReward(boxconfig.boxidx, boxconfig.boxlv, 1);
				
				/*if (boxconfig.boxidx == 2)
				{
					DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYOPENGOLDBOX);
				}*/	

				BoxInStoreConfig_T temp = { -1, 0 };
				UserData::getInstance()->setBoxLayerConfig(idx, temp);
				UserData::getInstance()->saveUserData(true);

				updateBoxOpen();
			}
			else if(UserData::getInstance()->getBoxLayerStage(idx) == 1)
			{
				showBoxInfo(idx);
			}
			else if (UserData::getInstance()->getBoxLayerStage(idx) == 0)
			{
				if (UserData::getInstance()->getIsBoxUnlock() != -1 && _curtimenode->getDur() < 0)
				{
					showBoxInfo(idx);
					/*auto _isunlockidx = idx;
					UserData::getInstance()->setBoxUnlock(_isunlockidx);
					UserData::getInstance()->setBoxLayerStage(idx, 1);
					UserData::getInstance()->saveUserData(true);
					MainLayer::getCurMainLayer()->addBoxTimeNode(20);
					_curtimenode = MainLayer::getCurMainLayer()->getBoxTimeNode();
					updateBoxOpen();*/
				}
				else if (UserData::getInstance()->getIsBoxUnlock() == -1)
				{
					if (nullptr != GameGuideLayer::getInstance())
					{
						GameGuideLayer::getInstance()->guideStepEnd();
					}
					showBoxInfo(idx);
					/*auto _isunlockidx = idx;
					UserData::getInstance()->setBoxUnlock(_isunlockidx);
					UserData::getInstance()->setBoxLayerStage(idx, 1);
					UserData::getInstance()->saveUserData(true);
					MainLayer::getCurMainLayer()->addBoxTimeNode(20);
					_curtimenode = MainLayer::getCurMainLayer()->getBoxTimeNode();
					updateBoxOpen();*/
				}
				else
				{
					showBoxInfo(idx);
				}
				
			}
	}
}


void BoxLayer::getReward(int boxidx, int lv, int pos/*=0*/)
{
	auto layer = PopRewardLayer::openBox(boxidx, lv, pos);//create(arrReward);
	this->addChild(layer);

	if (1 == pos)
	{
		UserData::getInstance()->setIsBoxGuided(true);
	}
}

void BoxLayer::clik()
{
	auto main = MainLayer::getCurMainLayer();
	//auto node0 = MainLayer::getCurMainLayer()->getTimeNode(0);
	auto node1 = MainLayer::getCurMainLayer()->getTimeNode(1);

	//auto time0 = node0->getDur();
	auto time1 = node1->getDur();

	/*if (time0 < 0)
	{
		switchTimeShow(0, false);
	}
	else
	{
		int hour = time0 / 3600;
		int min = time0 % 3600 / 60;
		int sec = time0 % (60);

		updateTimePic(0, hour);
		switchTimeShow(0, true);
		if (_lastTimeState[0] == 1)
		{
			_hour_0->setString(String::createWithFormat("%02d", hour)->getCString());
			_min_0->setString(String::createWithFormat("%02d", min)->getCString());
		}
		else
		{
			_hour_0->setString(String::createWithFormat("%02d", min)->getCString());
			_min_0->setString(String::createWithFormat("%02d", sec)->getCString());
		}
	}*/

	switchBoxLeftStage();


	if (time1 < 0)
	{
		auto stage = UserData::getInstance()->getBoxLayerStage(4);
		if (stage == 1)
		{
			_texttouch_1->setVisible(false);
			_timeNode1->setVisible(false);
			_ani1->setVisible(false);

			_winInfoRoot->setVisible(true);

			UserData::getInstance()->setBoxLayerStage(4, 0);

			updateBoxOpen();
		}
		else if (stage == 2)
		{

		}
		else if (stage == 0)
		{

		}
		//switchTimeShow(1, false);
	}
	else
	{
		int hour = time1 / 3600;
		int min = time1 % 3600 / 60;
		int sec = time1 % (60);

		updateTimePic(1, hour);
		//switchTimeShow(1, true);

		if (_lastTimeState[1] == 1)
		{
			_hour_1->setString(String::createWithFormat("%02d", hour)->getCString());
			_min_1->setString(String::createWithFormat("%02d", min)->getCString());
		}
		else
		{
			_hour_1->setString(String::createWithFormat("%02d", min)->getCString());
			_min_1->setString(String::createWithFormat("%02d", sec)->getCString());
		}
	}

	if (_unLockingTime != nullptr && _curtimenode)
	{
		auto timecur = _curtimenode->getDur();
		auto statie = UserData::getInstance()->getBoxLayerStage(UserData::getInstance()->getIsBoxUnlock());
		if (timecur < 0 && statie == 1)
		{
			UserData::getInstance()->setBoxLayerStage(UserData::getInstance()->getIsBoxUnlock(), 2);
			UserData::getInstance()->setBoxUnlock(-1);
			UserData::getInstance()->saveUserData();
			updateBoxOpen();
		}

		int hour = timecur / 3600;
		int min = timecur % 3600 / 60;
		int sec = timecur % (60);

		auto timestring = String::createWithFormat("%02d.%02d.%02d", hour, min, sec);

		//because updateBoxOpen(); will set _unLockingTime = null;
		if (_unLockingTime)
		{
			_unLockingTime->setString(timestring->getCString());
		}
		
		if (_unLockingCost && UserData::getInstance()->getIsBoxUnlock() != -1 && _curtimenode->isGetOnlineTime())
		{
			auto timecur = _curtimenode->getDur();
			auto& boxconfig = UserData::getInstance()->getBoxLayerConfig(UserData::getInstance()->getIsBoxUnlock());
			_unLockingCost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur))->getCString());
			//_unLockingCost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(UserData::getInstance()->getIsBoxUnlock(), timecur))->getCString());
		}

		if (_getcost && _selectPopBoxIdx != -1 && _selectPopBoxIdx == UserData::getInstance()->getIsBoxUnlock() && _curtimenode->isGetOnlineTime())
		{
			auto timecur = _curtimenode->getDur();
			auto& boxconfig = UserData::getInstance()->getBoxLayerConfig(_selectPopBoxIdx);
			_getcost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(boxconfig.boxidx, timecur))->getCString());
			//cocos2dx_plat::showToast(String::createWithFormat("boxtime3:%d", timecur)->getCString());
			//auto timecur = _curtimenode->getDur();
			//_getcost->setString(String::createWithFormat("%d", BoxDataMgr::getInstance()->getBoxOpenCost(_selectPopBoxIdx, timecur))->getCString());
		}
	}
}

void BoxLayer::updateTimePic(int idx, int hour)
{
	int state = hour > 0 ? 1 : 0;

	if (_lastTimeState[idx] == state)
	{
		return;
	}

	if (idx == 0)
	{
		if (hour > 0)
		{
			_timedanwei[0]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_xs.png"));
			_timedanwei[1]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_fz.png"));
			_lastTimeState[idx] = 1;
		}
		else
		{
			_timedanwei[1]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_m.png"));
			_timedanwei[0]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_fz.png"));
			_lastTimeState[idx] = 0;
		}
	}
	else
	{
		if (hour > 0)
		{
			_timedanwei[2]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_xs.png"));
			_timedanwei[3]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_fz.png"));
			_lastTimeState[idx] = 1;
		}
		else
		{
			_timedanwei[3]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_m.png"));
			_timedanwei[2]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("text_fz.png"));
			_lastTimeState[idx] = 0;
		}
	}
}


void BoxLayer::showBoxget(const std::vector<int>& itemids, const std::vector<int>& nums)
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layer);

	for (int i = 0; i < (int)itemids.size(); i++)
	{
		int posidxX = i % 3;
		int posidxY = i / 3;

		auto effectsp = Sprite::createWithSpriteFrameName("box_bg_2.png");
		layer->addChild(effectsp, 1);
		effectsp->setPosition(Vec2(320.f + (posidxX - 1) * 130.f, VisibleRect::top().y / 2.f - posidxY * 80.f + 100.f));

		auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
		effectsp->runAction(actionrotate);

		ComInfo_T item;
		ParamMgr::getInstance()->getComGameItemInfo(itemids[i], item);

		auto  icon = Sprite::createWithSpriteFrameName(item.picname);
		layer->addChild(icon, 2);
		icon->setPosition(Vec2(320.f + (posidxX - 1) * 130.f, VisibleRect::top().y / 2.f - posidxY * 80.f + 100.f));

		auto num = ui::TextAtlas::create(String::createWithFormat(".%d", nums[i])->getCString(), "fonts/num_boxreward.png", 18.f, 28.f, ".");
		layer->addChild(num, 3);
		num->setPosition(Vec2(320.f + (posidxX - 1) * 130.f + 40.f, VisibleRect::top().y / 2.f - 20.f - posidxY * 80.f + 100.f));
	}

	BagItemControl::getInstace()->saveItemConfig();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch * touch, Event * unusedEvent) {
		return true;
	};

	listener->onTouchEnded = [=](Touch * touch, Event * unusedEvent) {
		layer->removeFromParent();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

}


void BoxLayer::switchTimeShow(int idx, bool showtime)
{
	if (idx == 0)
	{
		_texttouch_0->setVisible(!showtime);
		_timeNode0->setVisible(showtime);
		_ani0->setVisible(!showtime);
	}
	else if (idx == 1)
	{
		_texttouch_1->setVisible(!showtime);
		_timeNode1->setVisible(showtime);
		_ani1->setVisible(!showtime);
	}
}

TiliRewardLayer::TiliRewardLayer():_curTime(0),
	_backToForegroundlistener(nullptr),
	_curRegetBuyIdx(-1)
{
}

TiliRewardLayer::~TiliRewardLayer()
{
	if (_backToForegroundlistener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundlistener);
	}
}

bool TiliRewardLayer::init()
{
	Layer::init();
	Color4B color(0, 0, 0, 200);
	auto layerclolor = LayerColor::create(color);
	layerclolor->setContentSize(Size(640.f, VisibleRect::top().y));
	this->addChild(layerclolor);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *touch, Event *event) {return true; };
	listener->setSwallowTouches(true);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, layerclolor);

	_root = CSLoader::createNode("csb/dayrewardui_layer.csb");
	this->addChild(_root);

	_tiliTimeNode = MainLayer::getCurMainLayer()->getTimeNode(1);
	//this->addChild(_tiliTimeNode);
	//_tiliTimeNode->setTimeCallBack(CC_CALLBACK_1(TiliRewardLayer::timeAction, this));
	//_tiliTimeNode->startCountDown();

	_textOpen = _root->getChildByName("Node_3")->getChildByName("touch_1");
	_timeInfoNode = _root->getChildByName("Node_3")->getChildByName("countdown_1");
	_barNode = _root->getChildByName("Node_3")->getChildByName("Node_15");

	_winTimeText = static_cast<Text*>(_barNode->getChildByName("Text_1"));
	_winTimesBar = static_cast<LoadingBar*>(_barNode->getChildByName("LoadingBar_1"));

	_buyTimeText = static_cast<Text*>(_root->getChildByName("main_bg")->getChildByName("item_4_bg")->getChildByName("cishu")); 

	for (int i = 0; i < 5; i++)
	{
		_btns[i] = static_cast<Button*>(_root->getChildByName("main_bg")->getChildByName(String::createWithFormat("item_%d_bg", i)->getCString())->getChildByName("get_bt"));

		if (i == 0)
		{
			_btns[i]->addTouchEventListener(CC_CALLBACK_2(TiliRewardLayer::menuOnOpenBox, this));
		}
		else if (i == 4)
		{
			_btns[i]->addTouchEventListener(CC_CALLBACK_2(TiliRewardLayer::menuOnBuyTili, this));
		}
		else
		{
			_btns[i]->addTouchEventListener(CC_CALLBACK_2(TiliRewardLayer::menuOnGetTili, this));
			_btns[i]->setUserData((void*)(i));
		}
	}

	auto action = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
		timeAction();
	}));

	this->runAction(RepeatForever::create(action));
	timeAction();
	
	updateBoxStage();

	getCurOnlineTime();
	updateBtnsStage();
	updateStaticBtnInfo();
	return true;
}

void TiliRewardLayer::initTimeInfo()
{
	_backToForegroundlistener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[this](EventCustom*)
	{
		getCurOnlineTime();
		//		cocos2dx_plat::showToast("fix time");
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_backToForegroundlistener, this);
}

void TiliRewardLayer::getCurOnlineTime()
{
	_getOnlineTime = false;

	std::thread t1([&]() {
		std::string timestring = cocos2dx_plat::getonlineTime();
		if (timestring.compare("error") == 0)
		{

		}
		else
		{
			std::vector<std::string > vectorNumString;
			ToolsUtil::split(timestring, "-", vectorNumString);
			int year = atoi(vectorNumString[0].c_str());
			int day = atoi(vectorNumString[1].c_str());
			int hour = atoi(vectorNumString[2].c_str());
			int min = atoi(vectorNumString[3].c_str());
			int sec = atoi(vectorNumString[4].c_str());

			_curTime = hour * 60 * 60 + min * 60 + sec;
			//_curTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;
			_getOnlineTime = true;
		}
	});

	t1.detach();
}

void TiliRewardLayer::timeAction()
{
	if (_getOnlineTime)
	{
		_curTime++;
		updateBtnsStage();
	}
}

void TiliRewardLayer::menuOnOpenBox(Ref *, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	auto main = MainLayer::getCurMainLayer();
	auto node1 = MainLayer::getCurMainLayer()->getTimeNode(1);

	if (UserData::getInstance()->getBoxLayerStage(4) == 2)
	{
		if (!node1->isGetOnlineTime())
		{
			GameUtils::toastTip("box_net_err");
			return;
		}

		UserData::getInstance()->setBoxLayerStage(4, 1);
		UserData::getInstance()->setWinTimes(0);
		UserData::getInstance()->saveUserData();
		node1->setDur(12 * 60 * 60);

		auto layer = PopRewardLayer::openBox(2, BoxDataMgr::getInstance()->getCurGetBoxLv(), 0);//create(arrReward);
		this->addChild(layer);

		_textOpen->setVisible(false);
		_timeInfoNode->setVisible(true);
		_barNode->setVisible(false);
		//_ani1->setVisible(false);
	}
	else if (UserData::getInstance()->getBoxLayerStage(4) == 1)
	{
		if (node1->getDur() > 0)
		{
			return;
		}
		else
		{
			UserData::getInstance()->setBoxLayerStage(4, 0);
			UserData::getInstance()->saveUserData();
		}
	}
	else
	{
		GameUtils::toastTip("top_box_info");
		CCLOG("top_box_info");
	}
}

void TiliRewardLayer::menuOnGetTili(Ref * btn, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (_tiliTimeNode->isGetOnlineTime())
		{
			int idx = int(static_cast<Node*>(btn)->getUserData());
			int curtimeidx = -1;
			if (checkCanGetTili(0,idx - 1))
			{
				UserData::getInstance()->giveTili(10);
				UserData::getInstance()->setTiliRewardTimes(idx, 1);
				UserData::getInstance()->saveUserData();
			}
			else if (checkCanGetTili(1, idx - 1))
			{
				_curRegetBuyIdx = idx;
				auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_REGETTILI, "", this, true);
				this->addChild(layer);
			}
			else
			{

			}
		}
	}
}

//void TiliRewardLayer::menuOnReGetTili(Ref *, Widget::TouchEventType type)
//{
//}

void TiliRewardLayer::menuOnBuyTili(Ref *, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (UserData::getInstance()->getTiliBuyTimes() > 0)
		{
			auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_BUYTILI, "", this, true);
			this->addChild(layer);
		}	
	}
}

void TiliRewardLayer::updateBoxStage()
{
	if (UserData::getInstance()->getWinTime() >= 10 && UserData::getInstance()->getBoxLayerStage(4) == 0)
	{
		_textOpen->setVisible(true);
		_timeInfoNode->setVisible(false);
		_barNode->setVisible(false);
		//_ani1->setVisible(true);

		UserData::getInstance()->setBoxLayerStage(4, 2);
		UserData::getInstance()->saveUserData();
	}

	auto topboxstate = UserData::getInstance()->getBoxLayerStage(4);
	if (topboxstate == 0)
	{
		_textOpen->setVisible(false);
		_timeInfoNode->setVisible(false);
		_barNode->setVisible(true);
		//_ani1->setVisible(false);

		int curwintime = UserData::getInstance()->getWinTime() > 10 ? 10 : UserData::getInstance()->getWinTime();
		_winTimeText->setString(String::createWithFormat("%d/10", curwintime)->getCString());
		_winTimesBar->setPercent((curwintime / 10.f) * 100);
	}
	else if (topboxstate == 1)
	{
		_textOpen->setVisible(false);
		_timeInfoNode->setVisible(true);
		_barNode->setVisible(false);
		//_ani1->setVisible(false);
	}
	else if (topboxstate == 2)
	{
		_textOpen->setVisible(false);
		_timeInfoNode->setVisible(false);
		_barNode->setVisible(false);
		//_ani1->setVisible(true);
	}
}

void TiliRewardLayer::updateBtnsStage()
{
	
	for (int i = 0; i < 3; i++)
	{
		checkBtnShow(i);
	}
}

void TiliRewardLayer::checkBtnShow(int idx)
{
	int flag[] = { 7, 12 , 18 };
	int curhour = _curTime / 3600;
	if (curhour >= flag[idx] + 2 && UserData::getInstance()->getTiliRewardTimes(idx) == 0)
	{
		_btns[idx + 1]->getChildByName("regetnode")->setVisible(true);
		_btns[idx + 1]->getChildByName("get_word")->setVisible(false);
	}
	else
	{
		_btns[idx + 1]->getChildByName("regetnode")->setVisible(false);
		_btns[idx + 1]->getChildByName("get_word")->setVisible(true);
	}
	
}

void TiliRewardLayer::buyCallBack(const std::string & itemid)
{
	UserData::getInstance()->setTiliBuyTimes(UserData::getInstance()->getTiliBuyTimes() - 1);
	UserData::getInstance()->saveUserData();

	if (UserData::getInstance()->getTiliBuyTimes() <= 0)
	{
		_btns[4]->setEnabled(false);
	}

	updateStaticBtnInfo();
}

void TiliRewardLayer::regetCallBack(const std::string & itemid)
{
	if (_curRegetBuyIdx != -1)
	{
		UserData::getInstance()->setTiliRewardTimes(_curRegetBuyIdx, 2);
		UserData::getInstance()->saveUserData();
		_curRegetBuyIdx = -1;

		updateStaticBtnInfo();
	}
}

bool TiliRewardLayer::checkCanGetTili(int type, int idx)
{
	if (_getOnlineTime == false)
	{
		return false;
	}
	
	int curhour = _curTime / 3600;
	int flag[] = { 7, 12 , 18 };

	if (type == 0)
	{
		if (curhour >= flag[idx] && curhour < flag[idx] + 2 && UserData::getInstance()->getTiliRewardTimes(idx) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (type == 1)
	{
		if (UserData::getInstance()->getTiliRewardTimes(idx) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void TiliRewardLayer::updateStaticBtnInfo()
{
	if (UserData::getInstance()->getTiliBuyTimes() > 0)
	{
		_btns[4]->setEnabled(true);
	}
	else
	{
		_btns[4]->setEnabled(false);
	}

	_buyTimeText->setString(String::createWithFormat("%d/%d", UserData::getInstance()->getTiliBuyTimes(), 3)->getCString());
}
