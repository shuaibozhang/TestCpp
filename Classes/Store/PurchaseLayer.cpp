#include "PurchaseLayer.h"
#include "../store/StoreEventHandler.h"
#include "../StoreBridge/cocos2dx_StoreInventory.h"
#include "../VisibleRect.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../platBridge/cocos2dx_analyze.h"
#include "../Scenes/ShopLayer.h"
#include "StoreBridge/StoreAssets.h"
#include "BagItemControl.h"
#include "Scenes/MainScene.h"
#include "UserData.h"
#include "BoxDataMgr.h"
#include "Widgets/GameCSLoader.h"
#include "ui/CocosGUI.h"
#include "../Scenes/BoxLayer.h"
#include "DataParam/ParamMgr.h"
#include "DataParam/WeaponControl.h"
#include "../Scenes/PopRewardLayer.h"
#include "Player.h"
#include "PlayerMgr.h"
#include "GameLayer.h"
#include "../GLCommon/Utils/ToolsUtil.h"
#include "../DataParam/SkillControl.h"
#include "../DataParam/TimeLimitActivityMgr.h"
#include "../Scenes/VipInfoLayer.h"
#include "../Scenes/BGLLayer.h"
#include"../platBridge/cocos2dx_plat.h"

USING_NS_CC;
using namespace ui;

PurchaseLayer* PurchaseLayer::s_pPurchaseLayer = nullptr;

DEFINE_PURCHASE_BUY_IDX(Skill,"itemid_good_skill_");
DEFINE_PURCHASE_BUY_IDX(Weapon,"itemid_good_weapon_");

PurchaseLayer::PurchaseLayer():
	_btnCancle(nullptr),
	_btnBuy(nullptr)
{
	s_pPurchaseLayer = this;
}

PurchaseLayer::~PurchaseLayer()
{
	if (m_pFather)
	{
		m_pFather->release();
	}
	
	
	if (s_pPurchaseLayer == this)
	{
		s_pPurchaseLayer = NULL;
	}

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("storeassets/storepayui.plist");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("storeassets/uirenwu.ExportJson");
}


PurchaseLayer* PurchaseLayer::getCurPurchaseLayer(void)
{
	return s_pPurchaseLayer;
}


PurchaseLayer* PurchaseLayer::create(string itemId, string descId, Node* father, bool buydirectly)
{
	PurchaseLayer *pRet = new PurchaseLayer();
	if (pRet && pRet->init(itemId, descId, father, buydirectly))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}


bool PurchaseLayer::init(string itemId, string desId, Node* father, bool buydirectly)
{
	bool isbuydir = buydirectly;
	LayerColor::init();
	m_pFather = father;
	m_pFather->retain();
	_node = nullptr;
	gooditemid = itemId;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (!isbuydir)
	{
		showDialog(gooditemid);
	}
	else
	{
		buyDirectly();
	}
	

	return true;
	
}

void PurchaseLayer::showDialog(const std::string & itemid)
{
	auto layercolor = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layercolor);
	Node* paynode = nullptr;
	if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT) == 0)
	{
#if (CC_PAY_SDK == PAY_SDK_MIGU)
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift_mm.csb");
#elif(CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift_zy.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift.csb");
#endif
		
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(Vec2(-88.f, -112.f));

		auto lab = static_cast<TextAtlas*>(paynode->getChildByName("time"));
		lab->setVisible(false);

#if(CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode->getChildByName("Text_1")->setOpacity(0);
		paynode->getChildByName("Text_1")->setPositionY(-VisibleRect::top().y / 2 + 50.f);
		auto actionshow = FadeIn::create(1.0f);
		paynode->getChildByName("Text_1")->runAction(actionshow);
#endif
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT_2) == 0)
	{
#if (CC_PAY_SDK == PAY_SDK_MIGU)
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift_mm.csb");
		static_cast<Sprite*>(paynode->getChildByName("store_des_gift_2"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("store_first2_des.png"));
#elif(CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift_zy1.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/firstbuygiift.csb");
		static_cast<Sprite*>(paynode->getChildByName("store_des_gift_2"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("store_first2_des.png"));
#endif

		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(Vec2(-88.f, -132.f));

		auto lab = static_cast<TextAtlas*>(paynode->getChildByName("time"));
		auto actiontime = Sequence::createWithTwoActions(CallFunc::create([=]() {
		
			int time = MainLayer::getCurMainLayer()->getGiftTimeNode()->getDur();
			//time = 24 * 60 * 60 - time;
			int curhour = time / 3600;
			int curmin = time % 3600 / 60;
			int cursec = time %  60;
			lab->setString(String::createWithFormat("%02d.%02d.%02d", curhour, curmin, cursec)->getCString());
		
		}), DelayTime::create(1.f));

		paynode->runAction(RepeatForever::create(actiontime));

#if(CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode->getChildByName("Text_1")->setOpacity(0);
		paynode->getChildByName("Text_1")->setPositionY(-VisibleRect::top().y / 2 + 50.f);
		auto actionshow = FadeIn::create(1.0f);
		paynode->getChildByName("Text_1")->runAction(actionshow);
#endif
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_DAYGIFT) == 0)
	{

#if (CC_PAY_SDK == PAY_SDK_MIGU)
		paynode = GameCSLoader::createNode("storeassets/daybuygiift_mm.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/daybuygiift.csb");
#endif
		
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		/*ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(paynode->getChildByName("wup_itembg_2_5")->getPosition());*/

		/*auto lab = static_cast<TextAtlas*>(paynode->getChildByName("time"));
		auto actiontime = Sequence::createWithTwoActions(CallFunc::create([=]() {
			if (MainLayer::getCurMainLayer()->getIsGetOnlineTime())
			{
				int time = MainLayer::getCurMainLayer()->getCurTime();
				time = 24 * 60 * 60 - time;
				int curhour = time / 3600;
				int curmin = time % 3600 / 60;
				int cursec = time %  60;
				lab->setString(String::createWithFormat("%02d.%02d.%02d", curhour, curmin, cursec)->getCString());
			}
		}), DelayTime::create(1.f));

		paynode->runAction(RepeatForever::create(actiontime));*/
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/timebuygiift_0.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		/*ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(paynode->getChildByName("wup_itembg_2_5")->getPosition());*/
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/timebuygiift_1.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		/*ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(paynode->getChildByName("wup_itembg_2_5")->getPosition());*/
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/timebuygiift_2.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		/*ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
		auto pArmtr = Armature::create("libao");
		paynode->addChild(pArmtr);
		pArmtr->getAnimation()->play("libao_1");
		pArmtr->setPosition(paynode->getChildByName("wup_itembg_2_5")->getPosition());*/
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_2) == 0)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("storeassets/uirenwu.ExportJson");
#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode = GameCSLoader::createNode("storeassets/reborn_zy.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/reborn.csb");
#endif
		this->addChild(paynode);
		paynode->setPosition(Vec2(220.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		auto logo0 = Armature::create("uirenwu");
		logo0->getAnimation()->play("uirenwu");
		paynode->addChild(logo0);
		logo0->setPosition(Vec2(-100.f, -190.f));

		auto countdown = static_cast<TextAtlas*>(paynode->getChildByName("countdown_num"));
		countdown->setString(String::createWithFormat("%d", 10)->getCString());
#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
#else
		auto pricePic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_1_22"));
		pricePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_2.png"));

		auto zcPic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_2z_21"));
		zcPic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_4z.png"));
#endif
		static int idx = 9;
		idx = 9;
		auto actionchange = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {		
			countdown->setString(String::createWithFormat("%d", idx)->getCString());
			idx--;
		})), 10);

		auto callaction = CallFunc::create([=]() {
			if (_btnBuy->isVisible() == true)
			{
				auto type = Widget::TouchEventType::ENDED;
				this->buttonCancelCallback(nullptr, type);
			}		
		});

		auto finalaction = Sequence::createWithTwoActions(actionchange, callaction);
		finalaction->setTag(999);

		this->runAction(finalaction);

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_1) == 0)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("storeassets/uirenwu.ExportJson");

#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode = GameCSLoader::createNode("storeassets/reborn_zy.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/reborn.csb");
#endif
		this->addChild(paynode);
		paynode->setPosition(Vec2(220.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		auto logo0 = Armature::create("uirenwu");
		logo0->getAnimation()->play("uirenwu");
		paynode->addChild(logo0);
		logo0->setPosition(Vec2(-100.f, -190.f));

		auto countdown = static_cast<TextAtlas*>(paynode->getChildByName("countdown_num"));
		countdown->setString(String::createWithFormat("%d", 10)->getCString());

#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
#else
		auto pricePic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_1_22"));
		pricePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_1.png"));

		auto zcPic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_2z_21"));
		zcPic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_2z.png"));
#endif
		static int idx = 9;
		idx = 9;
		auto actionchange = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
			countdown->setString(String::createWithFormat("%d", idx)->getCString());
			idx--;
		})), 10);

		auto callaction = CallFunc::create([=]() {
			if (_btnBuy->isVisible() == true)
			{
				auto type = Widget::TouchEventType::ENDED;
				this->buttonCancelCallback(nullptr, type);
			}
		});

		auto finalaction = Sequence::createWithTwoActions(actionchange, callaction);
		finalaction->setTag(999);

		this->runAction(finalaction);

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_4) == 0)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("storeassets/uirenwu.ExportJson");

#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
		paynode = GameCSLoader::createNode("storeassets/reborn_zy.csb");
#else
		paynode = GameCSLoader::createNode("storeassets/reborn.csb");
#endif
		this->addChild(paynode);
		paynode->setPosition(Vec2(220.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		auto logo0 = Armature::create("uirenwu");
		logo0->getAnimation()->play("uirenwu");
		paynode->addChild(logo0);
		logo0->setPosition(Vec2(-100.f, -190.f));

		auto countdown = static_cast<TextAtlas*>(paynode->getChildByName("countdown_num"));
		countdown->setString(String::createWithFormat("%d", 10)->getCString());

#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
#else
		auto pricePic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_1_22"));
		pricePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_4.png"));

		auto zcPic = static_cast<Sprite*>(paynode->getChildByName("pay_reborn_2z_21"));
		zcPic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pay_reborn_6z.png"));
#endif
		static int idx = 9;
		idx = 9;
		auto actionchange = Repeat::create(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
			countdown->setString(String::createWithFormat("%d", idx)->getCString());
			idx--;
		})), 10);

		auto callaction = CallFunc::create([=]() {
			if (_btnBuy->isVisible() == true)
			{
				auto type = Widget::TouchEventType::ENDED;
				this->buttonCancelCallback(nullptr, type);
			}
		});

		auto finalaction = Sequence::createWithTwoActions(actionchange, callaction);
		finalaction->setTag(999);

		this->runAction(finalaction);

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_0) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/wenpongift_mm.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_1) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/wenpongift2_mm.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_SUPERSKILL) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/superskillgiift_mm.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));
		
	}
	else if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_SUPERGIFT) == 0)
	{
		paynode = GameCSLoader::createNode("storeassets/supergift.csb");
		this->addChild(paynode);
		paynode->setPosition(Vec2(350.f, VisibleRect::top().y / 2));

		_btnBuy = static_cast<GameButton*>(paynode->getChildByName("btn_buy"));
		_btnBuy->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonBuyCallback, this));

		_btnCancle = static_cast<GameButton*>(paynode->getChildByName("btn_cancle"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(PurchaseLayer::buttonCancelCallback, this));

		auto lab = static_cast<TextAtlas*>(paynode->getChildByName("time"));
		auto actiontime = Sequence::createWithTwoActions(CallFunc::create([=]() {
			
				int time = MainLayer::getCurMainLayer()->getGiftTimeNode()->getDur();
				//time = 24 * 60 * 60 - time;
				int curhour = time / 3600;
				int curmin = time % 3600 / 60;
				int cursec = time % 60;
				lab->setString(String::createWithFormat("%02d.%02d.%02d", curhour, curmin, cursec)->getCString());
			
		}), DelayTime::create(1.f));

		paynode->runAction(RepeatForever::create(actiontime));

	}

	if (paynode)
	{
		paynode->setScale(0.5f);
		auto action = ScaleTo::create(0.2f, 1.f);
		paynode->runAction(action);
	}
}



void PurchaseLayer::showPurchaseDialog(void)
{
	StoreEventHandler::getCurStoreEventHandler()->buyItem(this->gooditemid);
}


void PurchaseLayer::buyDirectly(void)
{
	/*must use action, avoid remove this layer directly*/
	FiniteTimeAction *action = Sequence::create(DelayTime::create(0.01f),
//		CallFunc::create(this, callfunc_selector(PurchaseLayer::showPurchaseDialog)),
		CallFunc::create(CC_CALLBACK_0(PurchaseLayer::showPurchaseDialog, this)),
		NULL);
	this->runAction(action);
}




void PurchaseLayer::menuBuyCallback(Ref* pSender)
{
	StoreEventHandler::getCurStoreEventHandler()->buyItem(gooditemid);
}


void PurchaseLayer::menuCancelCallback(Ref* pSender)
{
	if (0 == gooditemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_4)
		|| 0 == gooditemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_2)
		|| 0 == gooditemid.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_1))
	{
		Player::getInstance()->reliveCB(false);
	}

	this->retain();
	if (dynamic_cast<ItemsShop*>(m_pFather))
	{
		dynamic_cast<ItemsShop*>(m_pFather)->touchBuyCallBack(0, false);
	}
	this->release();
	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(PurchaseLayer::actionCB, this)), nullptr));
//	this->removeFromParent();
	s_pPurchaseLayer = nullptr;
}

void PurchaseLayer::buttonBuyCallback(cocos2d::Ref * pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (_btnBuy)
		{
			_btnBuy->setVisible(false);
		}

		if (_btnCancle)
		{
			_btnCancle->setVisible(false);
		}

		this->stopActionByTag(999);

		buyDirectly();
	}
}

void PurchaseLayer::buttonCancelCallback(cocos2d::Ref * pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		menuCancelCallback(nullptr);
	}
}


void PurchaseLayer::onPurchaseOk(string itemId)
{
	this->retain();
	if (dynamic_cast<ItemsShop*>(m_pFather))
	{
		dynamic_cast<ItemsShop*>(m_pFather)->touchBuyCallBack(0, true);
	}

	if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_0) == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			BagItemControl::getInstace()->addBagItem(500 + i);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_1) == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			BagItemControl::getInstace()->addBagItem(510 + i);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_2) == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			BagItemControl::getInstace()->addBagItem(520 + i);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_3) == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			BagItemControl::getInstace()->addBagItem(530 + i);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SKILLPACK_0) == 0)
	{
		BagItemControl::getInstace()->addBagItem(3);
		BagItemControl::getInstace()->addBagItem(5);
		BagItemControl::getInstace()->addBagItem(7);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SKILLPACK_1) == 0)
	{
		BagItemControl::getInstace()->addBagItem(10);
		BagItemControl::getInstace()->addBagItem(12);
		BagItemControl::getInstace()->addBagItem(15);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SKILLPACK_2) == 0)
	{
		BagItemControl::getInstace()->addBagItem(16);
		BagItemControl::getInstace()->addBagItem(23);
		BagItemControl::getInstace()->addBagItem(27);
		BagItemControl::getInstace()->addBagItem(28);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SKILLPACK_3) == 0)
	{
		BagItemControl::getInstace()->addBagItem(40);
		BagItemControl::getInstace()->addBagItem(42);
		BagItemControl::getInstace()->addBagItem(44);
		BagItemControl::getInstace()->addBagItem(45);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_0) == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			BagItemControl::getInstace()->addBagItem(1000);
			BagItemControl::getInstace()->addBagItem(1001);
			BagItemControl::getInstace()->addBagItem(1008);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_1) == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			BagItemControl::getInstace()->addBagItem(1004);
			BagItemControl::getInstace()->addBagItem(1011);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_2) == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			BagItemControl::getInstace()->addBagItem(1007);
			BagItemControl::getInstace()->addBagItem(1013);
		}
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_GOLDPACK_0) == 0 || itemId.compare(StoreAssetMgr::ITEMID_GOOD_GOLDPACK_1) == 0 || itemId.compare(StoreAssetMgr::ITEMID_GOOD_GOLDPACK_2) == 0 || itemId.compare(StoreAssetMgr::ITEMID_GOOD_GOLDPACK_3) == 0)
	{
		if (MainLayer::getCurMainLayer())
		{
			MainLayer::getCurMainLayer()->updataGold();
		}
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_WENPONPACK_0) == 0)
	{
		BagItemControl::getInstace()->addBagItem(501);
		BagItemControl::getInstace()->addBagItem(511);
		BagItemControl::getInstace()->addBagItem(521);
		BagItemControl::getInstace()->addBagItem(531);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_WENPONPACK_1) == 0)
	{
		BagItemControl::getInstace()->addBagItem(503);
		BagItemControl::getInstace()->addBagItem(513);
		BagItemControl::getInstace()->addBagItem(523);
		BagItemControl::getInstace()->addBagItem(533);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_WENPONPACK_2) == 0)
	{
		BagItemControl::getInstace()->addBagItem(505);
		BagItemControl::getInstace()->addBagItem(515);
		BagItemControl::getInstace()->addBagItem(525);
		BagItemControl::getInstace()->addBagItem(535);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_WENPONPACK_3) == 0)
	{
		BagItemControl::getInstace()->addBagItem(507);
		BagItemControl::getInstace()->addBagItem(517);
		BagItemControl::getInstace()->addBagItem(527);
		BagItemControl::getInstace()->addBagItem(537);
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_WENPONPACK_4) == 0)
	{
		BagItemControl::getInstace()->addBagItem(509);
		BagItemControl::getInstace()->addBagItem(519);
		BagItemControl::getInstace()->addBagItem(529);
		BagItemControl::getInstace()->addBagItem(539);
		BagItemControl::getInstace()->saveItemConfig();
	}

	//new store
	if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_STOREBOX_0) == 0)
	{
		/*UserData::getInstance()->addBoxConfig(4, BoxDataMgr::getInstance()->getCurGetBoxLv());
		UserData::getInstance()->saveUserData();*/
		static_cast<StoreLayer2*>(m_pFather)->getReward(4);
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_STOREBOX_1) == 0)
	{
		/*UserData::getInstance()->addBoxConfig(5, BoxDataMgr::getInstance()->getCurGetBoxLv());
		UserData::getInstance()->saveUserData();*/
		static_cast<StoreLayer2*>(m_pFather)->getReward(5);
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_STOREBOX_2) == 0)
	{
		/*UserData::getInstance()->addBoxConfig(6, BoxDataMgr::getInstance()->getCurGetBoxLv());
		UserData::getInstance()->saveUserData();*/
		static_cast<StoreLayer2*>(m_pFather)->getReward(6);
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_REFRESH_STORE) == 0)
	{
		UserData::getInstance()->setRefashTimes(UserData::getInstance()->getRefashTimes() + 1);
		UserData::getInstance()->saveUserData();

		static_cast<StoreLayer2*>(m_pFather)->freshenItems(true);
	}
// 	else if ( itemId.compare(StoreAssetMgr::ITEMID_GOOD_BUYTILI) == 0)
// 	{
// 		static_cast<TiliRewardLayer*>(m_pFather)->buyCallBack(itemId);
// 	}
// 	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_REGETTILI) == 0)
// 	{
// 		static_cast<TiliRewardLayer*>(m_pFather)->regetCallBack(itemId);
// 	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_DAYGIFT) == 0)
	{
		/*BagItemControl::getInstace()->addBagItem(1014);
		BagItemControl::getInstace()->addBagItem(1014);
		BagItemControl::getInstace()->addBagItem(1014);
		BagItemControl::getInstace()->addBagItem(1014);
		BagItemControl::getInstace()->addBagItem(1007);
		BagItemControl::getInstace()->addBagItem(1007);
		BagItemControl::getInstace()->addBagItem(1007);
		BagItemControl::getInstace()->addBagItem(1007);
		BagItemControl::getInstace()->addBagItem(1013);
		BagItemControl::getInstace()->addBagItem(1013);
		BagItemControl::getInstace()->addBagItem(1013);
		BagItemControl::getInstace()->addBagItem(1013);*/

		std::vector<PopItemInfo_T> arrItems;

#if (CC_PAY_SDK == PAY_SDK_MIGU)
		PopItemInfo_T temp;
		temp.itemId = 1014;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		temp.itemId = 1007;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		temp.itemId = 1013;
		temp.itemCount = 1;
		arrItems.push_back(temp);
#else
		PopItemInfo_T temp;
		temp.itemId = 1014;
		temp.itemCount = 4;
		arrItems.push_back(temp);

		temp.itemId = 1007;
		temp.itemCount = 4;
		arrItems.push_back(temp);

		temp.itemId = 1013;
		temp.itemCount = 4;
		arrItems.push_back(temp);
#endif
		

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

#if (CC_PAY_SDK == PAY_SDK_MIGU)
		BagItemControl::getInstace()->autoEquipInFight();
		GameLayer::getInstance()->updateEquipItems();
#endif

		//BagItemControl::getInstace()->saveItemConfig();
		UserData::getInstance()->setCurDayGiftTimes(1);
		UserData::getInstance()->saveUserData();
		MainLayer::getCurMainLayer()->updataGiftBtns();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int curid = 503 + 10 * i;
			int type = WeaponControl::getInstance()->getWenponType(3);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);

			if (WeaponControl::getInstance()->isWeaponUnLock(curid))
			{
				//UserData::getInstance()->addWeaponPartsNum(curid + 1000, info[0].costparts);
				PopItemInfo_T temp;
				temp.itemId = curid + 1000;
				temp.itemCount = info[0].costparts;
				arrItems.push_back(temp);
			}
			else
			{
				//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", curid)->getCString(), 1);
				//BagItemControl::getInstace()->addBagItem(curid);
				PopItemInfo_T temp;
				temp.itemId = curid;
				temp.itemCount = 1;
				arrItems.push_back(temp);
			}
		}

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			MainLayer::getCurMainLayer()->updataEquipLayer(i);
		}
		MainLayer::getCurMainLayer()->updataGiftBtns();
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;
		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int curid = 506 + 10 * i;
			int type = WeaponControl::getInstance()->getWenponType(6);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);

			if (WeaponControl::getInstance()->isWeaponUnLock(curid))
			{
				PopItemInfo_T temp;
				temp.itemId = curid + 1000;
				temp.itemCount = info[0].costparts;
				arrItems.push_back(temp);
				//UserData::getInstance()->addWeaponPartsNum(curid + 1000, info[0].costparts);
			}
			else
			{
				//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", curid)->getCString(), 1);
				//BagItemControl::getInstace()->addBagItem(curid);
				PopItemInfo_T temp;
				temp.itemId = curid;
				temp.itemCount = 1;
				arrItems.push_back(temp);
			}
		}
		
		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			MainLayer::getCurMainLayer()->updataEquipLayer(i);
		}
		MainLayer::getCurMainLayer()->updataGiftBtns();
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int curid = 508 + 10 * i;
			int type = WeaponControl::getInstance()->getWenponType(8);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);

			if (WeaponControl::getInstance()->isWeaponUnLock(curid))
			{
				PopItemInfo_T temp;
				temp.itemId = curid + 1000;
				temp.itemCount = info[0].costparts;
				arrItems.push_back(temp);

				//UserData::getInstance()->addWeaponPartsNum(curid + 1000, info[0].costparts);
			}
			else
			{
				//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", curid)->getCString(), 1);
				//BagItemControl::getInstace()->addBagItem(curid);

				PopItemInfo_T temp;
				temp.itemId = curid;
				temp.itemCount = 1;
				arrItems.push_back(temp);
			}
		}

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			MainLayer::getCurMainLayer()->updataEquipLayer(i);
		}
		MainLayer::getCurMainLayer()->updataGiftBtns();
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_1) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int curid = 507 + 10 * i;
			int type = WeaponControl::getInstance()->getWenponType(7);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);

			if (WeaponControl::getInstance()->isWeaponUnLock(curid))
			{
				PopItemInfo_T temp;
				temp.itemId = curid + 1000;
				temp.itemCount = info[0].costparts;
				arrItems.push_back(temp);

				//UserData::getInstance()->addWeaponPartsNum(curid + 1000, info[0].costparts);
			}
			else
			{
				//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", curid)->getCString(), 1);
				//BagItemControl::getInstace()->addBagItem(curid);

				PopItemInfo_T temp;
				temp.itemId = curid;
				temp.itemCount = 1;
				arrItems.push_back(temp);
			}
		}

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			MainLayer::getCurMainLayer()->updataEquipLayer(i);
		}
		MainLayer::getCurMainLayer()->updataGiftBtns();
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_0) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			int curid = 504 + 10 * i;
			int type = WeaponControl::getInstance()->getWenponType(4);
			auto& info = ParamMgr::getInstance()->getWenponUpInfoByType(type);

			if (WeaponControl::getInstance()->isWeaponUnLock(curid))
			{
				PopItemInfo_T temp;
				temp.itemId = curid + 1000;
				temp.itemCount = info[0].costparts;
				arrItems.push_back(temp);

				//UserData::getInstance()->addWeaponPartsNum(curid + 1000, info[0].costparts);
			}
			else
			{
				//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", curid)->getCString(), 1);
				//BagItemControl::getInstace()->addBagItem(curid);

				PopItemInfo_T temp;
				temp.itemId = curid;
				temp.itemCount = 1;
				arrItems.push_back(temp);
			}
		}

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

		for (int i = 0; i < ParamData::ROLE_COUNT; i++)
		{
			MainLayer::getCurMainLayer()->updataEquipLayer(i);
		}
		MainLayer::getCurMainLayer()->updataGiftBtns();
		BagItemControl::getInstace()->saveItemConfig();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT) == 0 || itemId.compare(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT_2) == 0)
	{
#if CC_PAY_SDK == PAY_SDK_MIGU
		BagItemControl::getInstace()->addBagItem(1004);
		BagItemControl::getInstace()->addBagItem(1004);

		BagItemControl::getInstace()->addBagItem(14);
		BagItemControl::getInstace()->saveItemConfig();
		UserData::getInstance()->giveTili(5);
#else
		BagItemControl::getInstace()->addBagItem(1004);
		BagItemControl::getInstace()->addBagItem(1004);
		BagItemControl::getInstace()->addBagItem(1004);
		BagItemControl::getInstace()->addBagItem(1004);
		BagItemControl::getInstace()->addBagItem(1004);

		BagItemControl::getInstace()->addBagItem(14);
		BagItemControl::getInstace()->saveItemConfig();
		UserData::getInstance()->giveTili(20);
#endif
		MainLayer::getCurMainLayer()->firstGiftBuyCallBack();
		GameUtils::toastTip("buy_succeed");

		if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT_2) == 0)
		{
			UserData::getInstance()->giveItem(StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT, 1, false);
		}

		UserData::getInstance()->saveUserData();

		MainLayer::getCurMainLayer()->showNewGetItem();
	}
	else if (0 == itemId.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_4)
		|| 0 == itemId.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_2)
		|| 0 == itemId.compare(StoreAssetMgr::ITEMID_GOOD_REBORN_1))
	{
		Player::getInstance()->reliveCB(true);
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_PLAYER_4) == 0)
	{
		PlayerMgr::getInstance()->unLockPlayer(4);
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SUPERSKILL) == 0)
	{
		int superskillid[7] = {7,14,28,31,44,45,53};
		std::vector<int> lockSkillIds;
		for (int i = 0; i < sizeof(superskillid) / sizeof(superskillid[0]); i++)
		{
			int skill = superskillid[i];
			if (SkillControl::getInstance()->isSkillLock(skill) == true)
			{
				lockSkillIds.push_back(skill);
			}
		}
		if (lockSkillIds.size() > 0)
		{
			int buyidx = ToolsUtil::getRandomInt(0, lockSkillIds.size() - 1);
			auto layer = PopRewardLayer::create(lockSkillIds[buyidx], 1);
			MainLayer::getCurMainLayer()->addChild(layer, POP_Z);

			layer->addSkillDes(lockSkillIds[buyidx]);
		}
		
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_VIP) == 0)
	{
		VipMgr::getInstance()->buyVipSucceedCB();
		if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_PLAYER_4, false) == 0)
		{
			UserData::getInstance()->giveItem(StoreAssetMgr::ITEMID_GOOD_PLAYER_4, 1, false);
			PlayerMgr::getInstance()->unLockPlayer(4);
		}

		if (dynamic_cast<VipInfoLayer*>(m_pFather))
		{
			dynamic_cast<VipInfoLayer*>(m_pFather)->updateInfo();
		}
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_BGLJINHUA) == 0)
	{
		UserData::getInstance()->setHaveJinhuaBGL(1);
		static_cast<BGLLayer*>(m_pFather)->updateLvInfo();
		static_cast<BGLLayer*>(m_pFather)->hidePopBuyLayer();
	}
	else if (itemId.compare(StoreAssetMgr::ITEMID_GOOD_SUPERGIFT) == 0)
	{
		MainLayer::getCurMainLayer()->firstGiftBuyCallBack();
		UserData::getInstance()->giveCrystal(320);
		UserData::getInstance()->giveGold(30000);
		//UserData::getInstance()->saveUserData();
		auto layer = PopRewardLayer::openBox(6, BoxDataMgr::getInstance()->getCurGetBoxLv());//create(arrReward);
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
	}

// 	if (dynamic_cast<StoreLayer*>(m_pFather))
// 	{
// 		dynamic_cast<StoreLayer*>(m_pFather)->menuBuyCallback(itemId, true);
// 	}
// 	else 
	if (nullptr != dynamic_cast<GiftLayer*>(m_pFather))
	{
		dynamic_cast<GiftLayer*>(m_pFather)->updateBtns();
	}

	this->release();

	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(PurchaseLayer::actionCB, this)), nullptr));
	//	this->removeFromParent();
	s_pPurchaseLayer = nullptr;
}


void PurchaseLayer::actionCB(void)
{
	this->removeFromParent();
	s_pPurchaseLayer = nullptr;
}

void PurchaseLayer::onItemBuyFailed(string itemid)
{
	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(PurchaseLayer::actionCB, this)), nullptr));
	s_pPurchaseLayer = nullptr;
}
