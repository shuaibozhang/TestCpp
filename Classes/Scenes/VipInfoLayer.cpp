#include "VipInfoLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "VisibleRect.h"
#include "DataParam/TimeLimitActivityMgr.h"
#include "MainScene.h"
#include "../StoreBridge/StoreAssets.h"
#include "ui/UIButton.h"
#include "Store/PurchaseLayer.h"

USING_NS_CC;
using namespace ui;

VipInfoLayer::VipInfoLayer():_buyBtn(nullptr),
_getRewardBtn(nullptr),
_infoNode(nullptr)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("uiext/vip.plist");
}

VipInfoLayer::~VipInfoLayer()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("uiext/vip.plist");
}

bool VipInfoLayer::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto root = CSLoader::createNode("uiext/vipnode.csb");
	this->addChild(root);
	root->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	_buyBtn = static_cast<ui::Button*>(root->getChildByName("btnbuy"));
	_infoNode = root->getChildByName("nodeinfo");

	auto cancle = static_cast<ui::Button*>(root->getChildByName("cancle"));
	cancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});
	_getRewardBtn = static_cast<ui::Button*>(_infoNode->getChildByName("btn_get"));
	_getRewardBtn->addTouchEventListener(CC_CALLBACK_2(VipInfoLayer::touchGetRewardBtn, this));
	_buyBtn->addTouchEventListener(CC_CALLBACK_2(VipInfoLayer::touchBuyBtn, this));
	updateInfo();
	return true;
}

void VipInfoLayer::updateInfo()
{
	if (VipMgr::getInstance()->haveBuyVip())
	{
		if (VipMgr::getInstance()->haveGetCryReward())
		{
			_getRewardBtn->setVisible(false);
		}
		else
		{
			_getRewardBtn->setVisible(true);
		}
		static_cast<ui::TextAtlas*>(_infoNode->getChildByName("num_days"))->setString(String::createWithFormat("%d", VipMgr::getInstance()->getLeftDays())->getCString());
		_infoNode->setVisible(true);
		_buyBtn->setVisible(false);
	}
	else
	{
		_infoNode->setVisible(false);
		_buyBtn->setVisible(true);
	}
}

void VipInfoLayer::touchBuyBtn(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_VIP, "", this, true);
		this->addChild(layer, MainLayer_Z::POP_Z);
		//MainLayer::getCurMainLayer()->popPurchaseLayer(StoreAssetMgr::ITEMID_GOOD_VIP, true);
	}
}

void VipInfoLayer::touchGetRewardBtn(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	if (VipMgr::getInstance()->haveBuyVip())
	{
		VipMgr::getInstance()->getReward();
		updateInfo();
	}
}
