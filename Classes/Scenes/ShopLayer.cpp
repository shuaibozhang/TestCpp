#include "ShopLayer.h"
#include "../VisibleRect.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "DataParam/BagItemControl.h"
#include "DataParam/ParamData.h"
#include "MainScene.h"
#include "../Store/PurchaseLayer.h"
#include "ParamMgr.h"
#include "../Widgets/GameCSLoader.h"
#include "UserData.h"
#include "ParamMgr.h"
#include "GameUtils.h"
#include "WeaponControl.h"
#include "SkillControl.h"
#include "../Armtr/GameArmtr.h"
#include "CrushUtil.h"
#include "TimeCountDownNode.h"
#include "StoreBridge/StoreAssets.h"
#include "GLCommon/Utils/ResMgr.h"
#include "platBridge/cocos2dx_plat.h"
#include "TimeCountDownNode.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "PopRewardLayer.h"
#include "GLCommon/Utils/ResMgr.h"
#include "BoxDataMgr.h"
#include "platBridge/cocos2dx_analyze.h"
#include "MagPieMgr.h"
#include "DayActiveMgr.h"

using namespace cocos2d;

std::string g_btntextname[6] = { "store_text_suit_0.png", "store_text_suit_1.png", "store_text_gift_0.png", "store_text_gift_1.png", "store_text_gold_0.png", "store_text_gold_1.png" };
std::string g_list_item_0[5] = { StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_0, StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_1, StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_2, StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_3 };
std::string g_list_item_1[7] = {StoreAssetMgr::ITEMID_GOOD_SKILLPACK_0, StoreAssetMgr::ITEMID_GOOD_SKILLPACK_1, StoreAssetMgr::ITEMID_GOOD_SKILLPACK_2, StoreAssetMgr::ITEMID_GOOD_SKILLPACK_3, StoreAssetMgr::ITEMID_GOOD_ITEMPACK_0, StoreAssetMgr::ITEMID_GOOD_ITEMPACK_1, StoreAssetMgr::ITEMID_GOOD_ITEMPACK_2};
std::string g_list_item_2[4] = {StoreAssetMgr::ITEMID_GOOD_GOLDPACK_0, StoreAssetMgr::ITEMID_GOOD_GOLDPACK_1, StoreAssetMgr::ITEMID_GOOD_GOLDPACK_2,StoreAssetMgr::ITEMID_GOOD_GOLDPACK_3};
int g_wenponidx[5] = { 1, 3, 5, 7, 9 };

extern const char* g_iconbgname[10];
struct Resfresh_T
{
	int paytype;
	int num;
};

Resfresh_T g_arrResfresh[10] = {
	{0, 200},
	{0, 500},
	{0, 1000},
	{2, 20},
	{2, 40},
	{2, 60},
	{2, 80},
	{2, 100},
	{2, 120},
	{2, 140}
};

ShopLayer::ShopLayer()
{
}

ShopLayer::~ShopLayer()
{
	CCLOG("Shop del");
}

ShopLayer * ShopLayer::createLayer(int shopidx)
{
	auto pRef = new ShopLayer();
	if (pRef && pRef->initLayer(shopidx))
	{
		pRef->autorelease();
	}
	else 
	{
		delete pRef;
		pRef = nullptr;
	}
	return pRef;
}

bool ShopLayer::initLayer(int shopidx)
{
	LayerColor::initWithColor(Color4B(0,0,0,100), 640.f, VisibleRect::top().y);

	auto actionfade = FadeTo::create(0.5f, 200);
	this->runAction(actionfade);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto root = GameCSLoader::createNode("csb/shopnode.csb");
	this->addChild(root);
	root->setPosition(VisibleRect::center());

	auto btnskill = static_cast<ui::Button*>(root->getChildByName("Button_4"));
	btnskill->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto layer = ItemsShop::createShop(1, shopidx);
			this->addChild(layer, 1000);
		}
	});

	auto btnWeapon = static_cast<ui::Button*>(root->getChildByName("Button_1"));
	btnWeapon->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto layer = ItemsShop::createShop(2, shopidx);
			this->addChild(layer);
		}
	});

	auto btnItem = static_cast<ui::Button*>(root->getChildByName("Button_3"));
	btnItem->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto layer = ItemsShop::createShop(0, shopidx);
			this->addChild(layer);
		}
	});

	auto btnexit = static_cast<ui::Button*>(root->getChildByName("Button_2"));
	btnexit->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			root->removeFromParent();
			auto action = Sequence::createWithTwoActions(FadeOut::create(0.5f), RemoveSelf::create());
			this->runAction(action);
		}
	});

	return true;
}

ItemsShop::ItemsShop() :_isMove(false),
	_arrowSprite(nullptr),
	_priceBg(nullptr),
	_btnOk(nullptr),
	_buyDialog(nullptr),
	_istouchbuy(true),
	_curpage(0)
{
}

ItemsShop::~ItemsShop()
{
	CCLOG("SkillLayer deleat");
	if (MainLayer::getCurMainLayer())
	{
		MainLayer::getCurMainLayer()->_showNewGetCallBack = nullptr;
	}	
}

ItemsShop * ItemsShop::createShop(int shoptype, int shopidx)
{
	auto pRef = new ItemsShop();
	if (pRef&&pRef->initShop(shoptype, shopidx))
	{
		pRef->autorelease();
	}
	else
	{
		delete pRef;
		pRef = nullptr;
	}
	return pRef;
}

bool ItemsShop::initShop(int shoptype, int shopidx)
{
	_shoptype = shoptype;
	_shopidx = shopidx;
	_textWenponRare = nullptr;

	MainLayer::getCurMainLayer()->_showNewGetCallBack = std::bind(&ItemsShop::updataShopBagIcons, this);

	auto& shopinfo = ParamMgr::getInstance()->getShopAttr();

	//item shop
	if (_shoptype == 0)
	{
		LayerColor::initWithColor(Color4B(0, 0, 0, 255), 640.f, VisibleRect::top().y);
		_rootLayer = GameCSLoader::createNode("csb/itemshoplayer.csb");
		this->addChild(_rootLayer, 1);
		for (auto&temp : shopinfo[shopidx]._itemsconfig)
		{
			_vectorSellItems.push_back(temp);
		}
	}
	else
	{
		LayerColor::initWithColor(Color4B(0, 0, 0, 255), 640.f, VisibleRect::top().y);
		_rootLayer = GameCSLoader::createNode("csb/itemshoplayer.csb");
		this->addChild(_rootLayer, 1);

		if (shoptype == 1)
		{
			for (auto&temp : shopinfo[shopidx]._skillsconfig)
			{
				_vectorSellISkillsSp.push_back(temp);
			}

			for (int i = 0; i < 4; i++)
			{
				_vectorSellItems.push_back(shopinfo[shopidx]._skillsconfig[i]);
			}
		}
		else
		{
			for (auto&temp : shopinfo[shopidx]._weaponsconfig)
			{
				_vectorSellItems.push_back(temp);
			}
		}
	}
	
	auto bg = ImageView::create("bgs/main_darkness_bg.png");
	this->addChild(bg, 0);
	bg->setScale9Enabled(true);
	bg->setContentSize(Size(640.f, VisibleRect::top().y));
	bg->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	auto logo = Sprite::create("bgs/main_dark_logo.png");
	this->addChild(logo, 0);
	logo->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	auto offy = VisibleRect::top().y - 960.f;

	auto topnode = _rootLayer->getChildByName("node_0");
	//topnode->setPositionY(VisibleRect::top().y - 5.f);

	auto midnode = _rootLayer->getChildByName("node_1");
	//midnode->setPositionY(midnode->getPositionY() + offy * 0.5f);

	_textWenponRare = Text::create();
	midnode->addChild(_textWenponRare, 1);
	_textWenponRare->setFontSize(24);
	_textWenponRare->setTextColor(Color4B(0x7C, 0x4E, 0x2E, 0xff));
	_textWenponRare->setPosition(Vec2(170.f, 105.f));

	_itemsRoot = Node::create();
	this->addChild(_itemsRoot, 1);
	_itemsRoot->setContentSize(Size(150.f * _vectorSellItems.size(), 150.f));
	_itemsRoot->setPositionX(320.f -150.f * _vectorSellItems.size()/2.f);

	_itembgNode = Node::create();
	_rootLayer->addChild(_itembgNode, 1);

	if (shoptype == 1)
	{
		//auto temp : _vectorSellItems
		for (int i = 0; i < 4; i++)
		{
			auto sprite = addItem(_vectorSellItems[i], Vec2(75.f + 150.f * i, 670.f));
			_sellitemsPos.push_back(sprite->convertToWorldSpaceAR(Vec2(0.f, 0.f)));
			_sellSpriteNode[i] = sprite->getParent();
		}
	}
	else
	{
		int i = 0;
		for (auto temp : _vectorSellItems)
		{
			auto sprite = addItem(temp, Vec2(75.f + 150.f * i, 670.f));
			_sellitemsPos.push_back(sprite->convertToWorldSpaceAR(Vec2(0.f, 0.f)));
			i++;
		}
	}

	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto itemid = BagItemControl::getInstace()->getItemConfig(i, j);
			if (itemid != -1)
			{
				ComInfo_T temp;
				ParamMgr::getInstance()->getComGameItemInfo(itemid, temp);
				auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
				_rootLayer->addChild(sprite,2);
				_itemsSprite[j][i] = sprite;
			}
			else
			{
				_itemsSprite[j][i] = nullptr;
			}
			auto bg = Sprite::createWithSpriteFrameName("mainui_bag_combg_.png");
			_itemsBgSprite[j * g_onepagetnum + i] = bg;
			_itembgNode->addChild(bg, 1);

		}
	}
	

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ItemsShop::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ItemsShop::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ItemsShop::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ItemsShop::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_selectMark = Sprite::createWithSpriteFrameName("mainui_bag_select_select.png");
	_rootLayer->addChild(_selectMark, 3);
	_selectMark->setVisible(false);

	auto node_1 = _rootLayer->getChildByName("node_1");
	auto node_0 = _rootLayer->getChildByName("node_0");
	_textTitle = static_cast<ui::Text*>(node_1->getChildByName("text_title"));
	_textDes = static_cast<ui::Text*>(node_1->getChildByName("text_des"));

	_textPriceSellOrBuy = static_cast<TextAtlas*>(node_1->getChildByName("price_cost_sell"));
	_textPriceSellOrBuy->setString("");

	_iconSellOrBuy = Sprite::createWithSpriteFrameName("com_goldbag.png");
	_textPriceSellOrBuy->getParent()->addChild(_iconSellOrBuy);
	_iconSellOrBuy->setPosition(_textPriceSellOrBuy->getPosition() + Vec2(-70.f, 0.f));
	_iconSellOrBuy->setVisible(false);
	_iconSellOrBuy->setScale(0.8f);

	auto btnback = static_cast<Button*>(_rootLayer->getChildByName("back"));
	btnback->addTouchEventListener([this](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	_arrowSprite = Sprite::createWithSpriteFrameName("mainui_arrow.png");
	_arrowSprite->setRotation(180.f);
	this->addChild(_arrowSprite, 1);
	_arrowSprite->setVisible(false);

	_priceBg = static_cast<Sprite*>(node_1->getChildByName("pricebg"));
	_btnOk = static_cast<Button*>(node_1->getChildByName("btn_ok"));
	_btnOk->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_touchItemsIdx != -1)
			{
				if (_touchItemsIdx >= g_onepagetnum)
				{
					_istouchbuy = false;
					showBuyDialog(false);
				}
				else
				{
					_istouchbuy = false;
					showDelDialog(false);
				}
			}
		}
	});
	_priceBg->setVisible(false);
	_btnOk->setVisible(false);

	_textGold = static_cast<TextAtlas*>(node_1->getChildByName("gold"));
	_textGold->setString(String::createWithFormat("%d", UserData::getInstance()->getGoldNum())->getCString());

	_textCrystall = static_cast<TextAtlas*>(node_1->getChildByName("crystall"));
	_textCrystall->setString(String::createWithFormat("%d", UserData::getInstance()->getCrystalNum())->getCString());

	float off = 50.f;
	Vec2 posarr[4] = { Vec2(140.f + off, 0), Vec2(250.f + off,0), Vec2(400.f + off,0.f), Vec2(510.f + off, 0.f) };

	for (int i = 0; i < 4; i++)
	{
		auto pArmInfo = ParamMgr::getInstance()->getRoleArmtrInfo(i);
		auto _pArmtr = GameArmtr::createRole(pArmInfo);
		_rootLayer->addChild(_pArmtr);
		_pArmtr->setPosition(posarr[i]);
		_pArmtr->play(ArmtrName::ROLE_IDLE);

		_playerAni[i] = _pArmtr;

		auto& wenpon = WeaponControl::getInstance()->getEquipWenpon(i);
		CrushUtil::changeWeapon(_pArmtr, i, wenpon.id);
	}

	/*_btnChangePageR = GameButton::create("jiantou_0.png", "", "", Widget::TextureResType::PLIST);
	_rootLayer->addChild(_btnChangePageR);
	_btnChangePageR->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_curpage != 2)
			{
				_curpage++;
				for (int j = 0; j < BAGPAGES; j++)
				{
					for (int i = 0; i < 14; i++)
					{
						auto actionmove = MoveBy::create(0.5f, Vec2(-640.f, 0.f));
						if (_itemsSprite[j][i])
						{
							_itemsSprite[j][i]->runAction(actionmove);
						}
					}
				}
				auto actionmove = MoveBy::create(0.5f, Vec2(-640.f, 0.f));
				_itembgNode->runAction(actionmove);
			}
		}
	});

	_btnChangePageL = GameButton::create("jiantou_0.png", "", "", Widget::TextureResType::PLIST);
	_rootLayer->addChild(_btnChangePageL);
	_btnChangePageL->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (_curpage != 0)
			{
				_curpage--;
				for (int j = 0; j < BAGPAGES; j++)
				{
					for (int i = 0; i < 14; i++)
					{
						auto actionmove = MoveBy::create(0.5f, Vec2(640.f, 0.f));
						if (_itemsSprite[j][i])
						{
							_itemsSprite[j][i]->runAction(actionmove);
						}
					}
				}

				auto actionmove = MoveBy::create(0.5f, Vec2(640.f, 0.f));
				_itembgNode->runAction(actionmove);
			}
		}
	});*/

	auto textpagechange = ui::TextAtlas::create("110", "fonts/num_pageselect.png", 16.f, 26.f, ".");
	auto pageselect = PageSelecter::create(g_pagecount, "com_pagejt_0.png", "com_pagejt_1.png", "com_pagejt_2.png", textpagechange, [=](int start, int end) {
		for (int j = 0; j < g_pagecount; j++)
		{
			for (int i = 0; i < g_onepagetnum; i++)
			{
				auto actionmove = MoveBy::create(0.5f, Vec2(640.f * (start - end), 0.f));
				if (_itemsSprite[j][i])
				{
					_itemsSprite[j][i]->runAction(actionmove);
				}
			}
		}

		auto actionmove = MoveBy::create(0.5f, Vec2(640.f  * (start - end), 0.f));
		_itembgNode->runAction(actionmove);

		_curpage = end - 1;
	});
	_rootLayer->addChild(pageselect);
	pageselect->setPosition(Vec2(320.f, 138.f));

	auto actionsay = Sequence::createWithTwoActions(DelayTime::create(3.f), CallFunc::create([=]() {
		auto idxrad = ToolsUtil::getRandomInt(0, 3);
		say(idxrad);
	}));

	this->runAction(RepeatForever::create(actionsay));

	auto say = static_cast<Text*>(node_0->getChildByName("Text_3"));
	auto saybg = node_0->getChildByName("Sprite_8");
	
	auto actionmove = RepeatForever::create(Sequence::createWithTwoActions(
		MoveBy::create(1.f, Vec2(0.f, 10.f)), MoveBy::create(1.f, Vec2(0.f, -10.f))
		));
	say->runAction(actionmove);
	saybg->runAction(actionmove->clone());

	auto role = node_0->getChildByName("Sprite_1");
	auto actionscale = RepeatForever::create(Sequence::createWithTwoActions(
		ScaleTo::create(1.f, 0.98f), ScaleTo::create(1.f, 1.0f)
		));
	role->runAction(actionscale);

	updataSkillSp();

	return true;
}

void ItemsShop::onEnter()
{
	LayerColor::onEnter();
	
	_itemsStartPos = _rootLayer->getChildByName("btn_7")->convertToWorldSpaceAR(Vec2(0.f, 0.f)) + Vec2(0.f, -45.f);
	auto posmark = _rootLayer->getChildByName("btn_7")->getPosition();

	for (int i = 0; i < g_onepagetnum; i++)
	{
			auto truepos = Vec2(posmark.x + (i % 7) * 90.f, posmark.y + 90.f - 90.f * (i / 7));
			_itemsPos.push_back(truepos);
			for (int j = 0; j < g_pagecount; j++)
			{
				if (_itemsSprite[j][i])
				{
					_itemsSprite[j][i]->setPosition(truepos + Vec2(640.f*j, 0.f));
				}

				_itemsBgSprite[j * g_onepagetnum + i]->setPosition(truepos + Vec2(640.f*j, 0.f));
			}			
	}
	
	//_btnChangePageL->setPosition(_itemsPos[0] + Vec2(0.f, 50.f));
	//_btnChangePageR->setPosition(_itemsPos[7] + Vec2(0.f, 50.f));

	int itemcount = _shoptype == 0 ? 5 : 4;

	for (int i = 0; i < _sellitemsPos.size(); i++)
	{
		Rect temp;
		temp.origin = _sellitemsPos[i] + Vec2(-45.f, -45.f);
		temp.size = Size(90.f, 90.f);
		_equipRectInWorld.push_back(temp);
		_itemsPos.push_back(_sellitemsPos[i]);
	}

}

Sprite* ItemsShop::addItem(int id, Vec2 pos)
{
	ComInfo_T item = { 0 };
	ParamMgr::getInstance()->getComGameItemInfo(id, item);

	auto node = GameCSLoader::createNode("csb/shopitemnode.csb");
	_itemsRoot->addChild(node, 2);
	node->setPosition(pos);

	auto itemtype = BagItemControl::getInstace()->checkItemType(id);

	Sprite* sprite = nullptr;

	if (itemtype == -102)
	{
		sprite = Sprite::createWithSpriteFrameName("com_icon_jingqi.png");
		node->addChild(sprite);
		sprite->setPosition(Vec2(0.f, 90.f));
		sprite->setName("skillicon");
	}
	else
	{
		sprite = Sprite::createWithSpriteFrameName(item.picname);
		node->addChild(sprite);
		sprite->setPosition(Vec2(0.f, 90.f));
		sprite->setName("skillicon");
	}

	auto text = static_cast<TextAtlas*>(node->getChildByName("price"));
	text->setString(String::createWithFormat("%d", item.price)->getCString());
	text->setPositionX(text->getPosition().x + 20.f);
	text->setLocalZOrder(5);

	if (item.paytype == 0)
	{
		auto spriteicon = Sprite::createWithSpriteFrameName("com_goldbag.png");
		spriteicon->setScale(0.7f);
		spriteicon->setPosition(text->getPosition() + Vec2(-55.f, 0.f));
		node->addChild(spriteicon);
		spriteicon->setName("paytype");
	}
	else if (item.paytype == 2)
	{
		auto spriteicon = Sprite::createWithSpriteFrameName("com_crystal.png");
		spriteicon->setScale(0.7f);
		spriteicon->setPosition(text->getPosition() + Vec2(-55.f, 0.f));
		spriteicon->setName("paytype");
		node->addChild(spriteicon);
	}

	if (id < 1000)
	{
		node->getChildByName("shadow")->setVisible(false);
	}

	return sprite;
}

void ItemsShop::showBuyDialog(bool istouchbuy)
{
	bool bFull = true;
	
	for (int i = 0; i < g_onepagetnum; i++)
	{
		if (_itemsSprite[_curpage][i] == nullptr)
		{
			bFull = false;
			break;
		}
	}

	if (bFull)
	{
		cocos2dx_plat::showToast(ResMgr::getInstance()->getString("pop_toast_1")->getCString());
		return;
	}

	_buyDialog = LayerColor::create(Color4B(0,0,0,188), 640.f, VisibleRect::top().y);
	auto node = GameCSLoader::createNode("csb/itembuynode.csb");
	node->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f));
	_buyDialog->addChild(node);
	this->addChild(_buyDialog, 1000);

	auto itemid = _vectorSellItems[_touchItemsIdx - g_onepagetnum];
	ComInfo_T iteminfo;
	ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);
	auto text = static_cast<Text*>(node->getChildByName("text_title"));
	text->setString(iteminfo.name);

	auto textdel = Sprite::createWithSpriteFrameName("mainui_text_mjwp.png");
	node->addChild(textdel);
	textdel->setPosition(text->getPosition());
	text->setVisible(false);

	auto pricebg = Sprite::createWithSpriteFrameName("mainui_text_bg.png");
	node->addChild(pricebg);
	pricebg->setPositionY(50.f);

	auto textprice = Text::create();
	textprice->setFontSize(26);
	node->addChild(textprice);
	textprice->setString(String::createWithFormat("%d", iteminfo.price)->getCString());
	textprice->setPositionY(50.f);

	auto textname = static_cast<Text*>(node->getChildByName("text_name"));
	textname->setString(iteminfo.des);
	
	auto icon = static_cast<Sprite*>(node->getChildByName("item_icon"));
	icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iteminfo.picname));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _buyDialog);

	auto btnok = static_cast<Button*>(node->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				//UserData::getInstance()->getGoldNum() < iteminfo.price
				if (StoreAssetMgr::checkCanPay(iteminfo.paytype, iteminfo.price, true) == false)
				{
					touchBuyCallBack(0, false);
					_buyDialog->removeFromParent();
					return;
				}

				if (BagItemControl::getInstace()->checkItemType(itemid) == 0)
				{
					auto layer = PurchaseLayer::create(String::createWithFormat("itemid_good_item_%d", itemid)->getCString(), "", this, true);
					this->addChild(layer, 1000);
					_buyDialog->removeFromParent();
				}
				else if (BagItemControl::getInstace()->checkItemType(itemid) == 2)
				{
					auto layer = PurchaseLayer::create(String::createWithFormat("itemid_good_weapon_%d", itemid)->getCString(), "", this, true);
					this->addChild(layer, 1000);
					_buyDialog->removeFromParent();
				}
				else
				{
					auto layer = PurchaseLayer::create(String::createWithFormat("itemid_good_skill_%d", itemid)->getCString(), "", this, true);
					this->addChild(layer, 1000);
					_buyDialog->removeFromParent();
				}
			}
		});

	auto btncancle = static_cast<Button*>(node->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			touchBuyCallBack(0, false);
			_buyDialog->removeFromParent();
		}
	});
}

void ItemsShop::showDelDialog(bool istouchbuy)
{
	if (BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage) == -1)
	{
		return;
	}
	_buyDialog = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	auto node = GameCSLoader::createNode("csb/itemdelnode.csb");
	node->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f));
	_buyDialog->addChild(node);
	this->addChild(_buyDialog, 1000);

	auto itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage);
	ComInfo_T iteminfo;
	ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);
	auto text = static_cast<Text*>(node->getChildByName("text_title"));
	text->setString(iteminfo.name);

	auto textdel = Sprite::createWithSpriteFrameName("mainui_text_mcwp.png");
	node->addChild(textdel);
	textdel->setPosition(text->getPosition());
	text->setVisible(false);

	auto pricebg = Sprite::createWithSpriteFrameName("mainui_text_bg.png");
	node->addChild(pricebg);
	pricebg->setPositionY(25.f);

	auto textprice = Text::create();
	textprice->setFontSize(26);
	node->addChild(textprice);
	int pricegoldsold = (int)(iteminfo.price / 10);
	
	textprice->setString(String::createWithFormat("%d", pricegoldsold)->getCString());
	textprice->setPositionY(25.f);

	auto textname = static_cast<Text*>(node->getChildByName("text_name"));
	textname->setString(iteminfo.name);

	auto icon = static_cast<Sprite*>(node->getChildByName("item_icon"));
	icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iteminfo.picname));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _buyDialog);

	auto btnok = static_cast<Button*>(node->getChildByName("btn_ok"));
	btnok->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			touchBuyCallBack(1, true);
			_buyDialog->removeFromParent();
		}
	});

	auto btncancle = static_cast<Button*>(node->getChildByName("btn_cancle"));
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			touchBuyCallBack(1, false);
			_buyDialog->removeFromParent();
		}
	});
}

void ItemsShop::touchBuyCallBack(int type, bool succeed)
{
	bool istouchbuy = _istouchbuy;
	if (istouchbuy)
	{
		if (type == 0)
		{
			if (succeed)
			{
				_itemsSprite[_curpage][_touchEndIdx] = _selectItemsSprite;
				_selectItemsSprite->setPosition(_itemsPos[_touchEndIdx]);
				BagItemControl::getInstace()->addBagItem(_vectorSellItems[_touchItemsIdx - g_onepagetnum], _touchEndIdx, _curpage);
				GameUtils::playEffect("jinbi.ogg", false);

				MainLayer::getCurMainLayer()->showNewGetItem();
			}
			else
			{
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::create(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }), RemoveSelf::create(), nullptr);
				_selectItemsSprite->runAction(action);
			}
		}
		else if (type == 1)
		{
			if (succeed)
			{
				GameUtils::playEffect("jinbi.ogg", false);
				auto itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage);
				ComInfo_T iteminfo;
				ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);

				UserData::getInstance()->giveGold(int(iteminfo.price / 10));
				//UserData::getInstance()->saveUserData();

				_itemsSprite[_curpage][_touchItemsIdx] = nullptr;
				_selectItemsSprite->removeFromParent();
				BagItemControl::getInstace()->delBagItem(_touchItemsIdx, _curpage);

				
			}
			else
			{
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }));
				_selectItemsSprite->runAction(action);
			}
		}
	}
	else
	{
		if (type == 0)
		{
			if (succeed)
			{
				for (int i = 0; i < g_onepagetnum; i++)
				{
					if (BagItemControl::getInstace()->getItemConfig(i, _curpage) == -1)
					{
						_touchEndIdx = i;
						break;
					}
				}
				ComInfo_T iteminfo;
				ParamMgr::getInstance()->getComGameItemInfo(_vectorSellItems[_touchItemsIdx - 14], iteminfo);
				_selectItemsSprite = Sprite::createWithSpriteFrameName(iteminfo.picname);
				_rootLayer->addChild(_selectItemsSprite, 1);
				_selectItemsSprite->setPosition(_itemsPos[_touchItemsIdx]);
				_itemsSprite[_curpage][_touchEndIdx] = _selectItemsSprite;

				_isMove = true;
				auto truepos = _itemsPos[_touchEndIdx];

				auto time = 0.3f;
				auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }));
				_selectItemsSprite->runAction(action);
				BagItemControl::getInstace()->addBagItem(_vectorSellItems[_touchItemsIdx - g_onepagetnum], _touchEndIdx, _curpage);

				GameUtils::playEffect("jinbi.ogg", false);

				MainLayer::getCurMainLayer()->showNewGetItem();
			}
			else
			{
			}
		}
		else if (type == 1)
		{
			if (succeed)
			{

				auto itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage);
				ComInfo_T iteminfo;
				ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);

				UserData::getInstance()->giveGold(int(iteminfo.price / 10));
			//	UserData::getInstance()->saveUserData();

				_itemsSprite[_curpage][_touchItemsIdx] = nullptr;
				_selectItemsSprite->removeFromParent();
				BagItemControl::getInstace()->delBagItem(_touchItemsIdx, _curpage);

				GameUtils::playEffect("jinbi.ogg", false);
			}
			else
			{

			}
		}
	}

	_textGold->setString(String::createWithFormat("%d", UserData::getInstance()->getGoldNum())->getCString());
	_textCrystall->setString(String::createWithFormat("%d", UserData::getInstance()->getCrystalNum())->getCString());

	BagItemControl::getInstace()->saveItemConfig();

	if (_touchItemsIdx >= g_onepagetnum)
	{
		int itemid = _vectorSellItems[_touchItemsIdx - g_onepagetnum];
		if (_shoptype == 2 && (WeaponControl::getInstance()->isWeaponUnLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
		{
			_btnOk->setVisible(false);
		}
		else if (_shoptype == 1 && (!SkillControl::getInstance()->isSkillLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
		{
			_btnOk->setVisible(false);
		}
		else
		{
			_btnOk->setVisible(true);
		}
	}

	if (succeed)
	{
		updataSkillSp();
	}
}

void ItemsShop::updataSkillSp()
{
	if (_shoptype != 1 || _vectorSellISkillsSp.size() <= 4)
	{
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		int skillid = _vectorSellItems[i];
		if ((!SkillControl::getInstance()->isSkillLock(skillid) || BagItemControl::getInstace()->isInBag(skillid)))
		{
			int trueskil = 0;
			for (auto& newskill : _vectorSellISkillsSp)
			{	
				trueskil = newskill;
				bool haveskill = false;
				if ((!SkillControl::getInstance()->isSkillLock(newskill) || BagItemControl::getInstace()->isInBag(newskill)))
				{		
					haveskill = true;
				}
				else if(std::find(_vectorSellItems.begin(), _vectorSellItems.end(), newskill) != _vectorSellItems.end())
				{
					haveskill = true;
				}

				if (haveskill == false)
				{				
					_vectorSellItems[i] = trueskil;			
					break;
				}

			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		ComInfo_T item = { 0 };

		int id = _vectorSellItems[i];

		ParamMgr::getInstance()->getComGameItemInfo(id, item);

		auto node = _sellSpriteNode[i];

		auto itemtype = BagItemControl::getInstace()->checkItemType(id);
	
		static_cast<Sprite*>(node->getChildByName("skillicon"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(item.picname));
		
	

		auto text = static_cast<TextAtlas*>(node->getChildByName("price"));
		text->setString(String::createWithFormat("%d", item.price)->getCString());
	
		if (item.paytype == 0)
		{
			static_cast<Sprite*>(node->getChildByName("paytype"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_goldbag.png"));
		}
		else if (item.paytype == 2)
		{
			static_cast<Sprite*>(node->getChildByName("paytype"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_crystal.png"));
		}

		if (id < 1000)
		{
			node->getChildByName("shadow")->setVisible(false);
		}
	}
}

/*idx : 0-3*/
void ItemsShop::say(int idx)
{
	auto text = ResMgr::getInstance()->getString(String::createWithFormat("shop_say_%d", idx)->getCString());
	auto say = static_cast<Text*>(_rootLayer->getChildByName("node_0")->getChildByName("Text_3"));
	say->setString(text->getCString());
}

void ItemsShop::updataShopBagIcons()
{
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto itemid = BagItemControl::getInstace()->getItemConfig(i, j);
			if (itemid == -1 && _itemsSprite[j][i] != nullptr)
			{	
				_itemsSprite[j][i]->removeFromParent();
				_itemsSprite[j][i] = nullptr;
				_isMove = false;
			}
		}
	}
}

int ItemsShop::checkIsTouchItem(cocos2d::Vec2 touchPos)
{
	if (touchPos.y >= _itemsStartPos.y && touchPos.y <= _itemsStartPos.y + 180.f && touchPos.x >= 5.f && touchPos.x <= 635.f)
	{
		int idx = (touchPos.x - 5.f) / 90.f;
		int idy = (touchPos.y - _itemsStartPos.y) / 90.f;
		CCASSERT((idx + 7 - idy * 7) < 14, "error");
		return (idx + 7 - idy * 7);
	}
	else
	{
		int idx = 0;
		for (auto& rect : _equipRectInWorld)
		{
			if (rect.containsPoint(touchPos))
			{
				return g_onepagetnum + idx;
			}
			idx++;
		}
	}

	return -1;
}

bool ItemsShop::onTouchBegan(Touch * touch, Event * unusedEvent)
{
	auto pos = touch->getLocation();
	_touchItemsIdx = checkIsTouchItem(pos);

	if (_touchItemsIdx >= g_onepagetnum)
	{
		_arrowSprite->setPosition(_itemsPos[_touchItemsIdx] + Vec2(0.f, 90.f));
		_arrowSprite->setVisible(true);

		_selectMark->setPosition(_itemsPos[_touchItemsIdx]);
		_selectMark->setVisible(false);
		int itemid = _vectorSellItems[_touchItemsIdx - g_onepagetnum];
		{
			ComInfo_T info;
			ParamMgr::getInstance()->getComGameItemInfo(itemid, info);
			_textTitle->setString(info.name);
			_textDes->setString(info.des);

			showItem(itemid);
		}
		ComInfo_T iteminfo;
		ParamMgr::getInstance()->getComGameItemInfo(_vectorSellItems[_touchItemsIdx - 14], iteminfo);
		_selectItemsSprite = Sprite::createWithSpriteFrameName(iteminfo.picname);
		this->addChild(_selectItemsSprite, 2);
		_selectItemsSprite->setPosition(_itemsPos[_touchItemsIdx]);
		_textPriceSellOrBuy->setString(String::createWithFormat("%d", iteminfo.price)->getCString());

		if (iteminfo.paytype == 2)
		{
			_iconSellOrBuy->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_crystal.png"));
		}
		else if (iteminfo.paytype == 0)
		{
			_iconSellOrBuy->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_goldbag.png"));
		}
		
		_iconSellOrBuy->setVisible(true);
		_priceBg->setVisible(true);

		if (_shoptype == 2 && (WeaponControl::getInstance()->isWeaponUnLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
		{
			_btnOk->setVisible(false);
		}
		else if(_shoptype == 1 && (!SkillControl::getInstance()->isSkillLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
		{
			_btnOk->setVisible(false);
		}
		else
		{
			_btnOk->setVisible(true);
		}

		
		_btnOk->loadTextures("shopui_buy.png", "", "", Widget::TextureResType::PLIST);

		if (BagItemControl::getInstace()->checkItemType(itemid) == 1)
		{
			auto info = SkillControl::getInstance()->getPlayerSkillInfo(itemid);
			for (int i = 0; i < 4; i++)
			{
				if (i == info.owner)
				{
					_playerAni[i]->play("win", -1, 1);
				}
				else
				{
					_playerAni[i]->play("stand");
				}
			}	
		}
		else if (BagItemControl::getInstace()->checkItemType(itemid) == 2)
		{
			int owner = WeaponControl::getInstance()->getWenponOwner(itemid);

			for (int i = 0; i < 4; i++)
			{
				if (i == owner)
				{
					_playerAni[i]->play("win", -1, 1);
				}
				else
				{
					_playerAni[i]->play("stand");
				}
			}

			_textWenponRare->setString(ResMgr::getInstance()->getString(String::createWithFormat("wenpon_lv_pos_%d", (itemid - 500) % 10)->getCString())->getCString());
		}

	}
	else if (_touchItemsIdx != -1)
	{
		_selectMark->setPosition(_itemsPos[_touchItemsIdx]);
		_selectMark->setVisible(true);
		_arrowSprite->setVisible(false);

		

		int itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage);
		if (itemid == -1)
		{
			_textTitle->setString("");
			_textDes->setString("");
			_textPriceSellOrBuy->setString("");
			_textWenponRare->setString("");

			_priceBg->setVisible(false);
			_btnOk->setVisible(false);
			_iconSellOrBuy->setVisible(false);
			hideItemInfo();
		}
		else
		{
			if (BagItemControl::getInstace()->checkItemType(itemid) == 2)
			{
				showItem(itemid);
				_textWenponRare->setString(ResMgr::getInstance()->getString(String::createWithFormat("wenpon_lv_pos_%d", (itemid - 500) % 10)->getCString())->getCString());
			}
			else
			{
				_textWenponRare->setString("");
				hideItemInfo();
			}

			_priceBg->setVisible(true);
			_btnOk->setVisible(true);
			_btnOk->loadTextures("shopui_soldout.png", "", "", Widget::TextureResType::PLIST);
			ComInfo_T iteminfo;
			ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);
			_textTitle->setString(iteminfo.name);
			_textDes->setString(iteminfo.des);

			_textPriceSellOrBuy->setString(String::createWithFormat("%d",(int) (iteminfo.price / 10))->getCString());

			if (iteminfo.paytype == 2)
			{
				_priceBg->setVisible(false);
				_btnOk->setVisible(false);
				_textPriceSellOrBuy->setString("");
			}
			else
			{
				_priceBg->setVisible(true);
				_btnOk->setVisible(true);
				_iconSellOrBuy->setVisible(true);
				_iconSellOrBuy->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_goldbag.png"));
			}
		}

		_selectItemsSprite = _itemsSprite[_curpage][_touchItemsIdx];
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{	
			_playerAni[i]->play("stand");		
		}
	}

	isNeedRunMoveAndEnd = true;

	if (_isMove || _touchItemsIdx == -1)
	{
		isNeedRunMoveAndEnd = false;
	}

	if (_touchItemsIdx < g_onepagetnum && BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage) == -1)
	{
		isNeedRunMoveAndEnd = false;
	}

	CCLOG("baglayer: %d item touched", _touchItemsIdx);
	return true;
}

void ItemsShop::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	if (isNeedRunMoveAndEnd == false)
	{
		return;
	}

	if (_touchItemsIdx != -1 )
	{
		auto off = touch->getDelta();
		_selectItemsSprite->setPosition(_selectItemsSprite->getPosition() + off);

		auto Idx = checkIsTouchItem(touch->getLocation());
		if (Idx != -1 && Idx < g_onepagetnum)
		{
			_selectMark->setVisible(true);
			_selectMark->setPosition(_itemsPos[Idx]);
		}
		else
		{
			_selectMark->setVisible(false);
		}
	}
	else
	{
		_selectMark->setVisible(false);
	}
}

void ItemsShop::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	if (isNeedRunMoveAndEnd == false)
	{
		return;
	}
	auto pos = touch->getLocation();
	int endidx = checkIsTouchItem(pos);
	_touchEndIdx = endidx;
	if (_touchItemsIdx < g_onepagetnum)
	{
		if (endidx != -1)
		{
			int flagend = BagItemControl::getInstace()->getItemConfig(endidx, _curpage);
			if (endidx < g_onepagetnum)
			{
				if (flagend == -1)
				{
					auto truepos = _itemsPos[endidx];
					_selectItemsSprite->setPosition(truepos);
					
					_itemsSprite[_curpage][endidx] = _selectItemsSprite;
					_itemsSprite[_curpage][_touchItemsIdx] = nullptr;

					BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpage);
				}
				else if (endidx == _touchItemsIdx)
				{
					auto truepos = _itemsPos[_touchItemsIdx];
					_selectItemsSprite->setPosition(truepos);
				}
				else if (flagend != -1)
				{
					auto truepos = _itemsPos[endidx];
					_selectItemsSprite->setPosition(truepos);

					_isMove = true;
					auto startpos = _itemsPos[_touchItemsIdx];
					auto time = 0.3f;
					auto action = Sequence::createWithTwoActions(MoveTo::create(time, startpos), CallFunc::create([=]() {_isMove = false; }));
					_itemsSprite[_curpage][endidx]->runAction(action);

					auto back = _itemsSprite[_curpage][endidx];
					_itemsSprite[_curpage][endidx] = _selectItemsSprite;
					_itemsSprite[_curpage][_touchItemsIdx] = back;

					BagItemControl::getInstace()->changePos(_touchItemsIdx, endidx, _curpage);				
				}

				_touchItemsIdx = endidx;
			}
			else
			{
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }));
				_selectItemsSprite->runAction(action);
			}

		}
		else if ( endidx >= g_onepagetnum)
		{
			_isMove = true;
			auto truepos = _itemsPos[_touchItemsIdx];

			auto time = 0.3f;
			auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }));
			_selectItemsSprite->runAction(action);
		}
		else if (endidx == -1)
		{
			int itemid = BagItemControl::getInstace()->getItemConfig(_touchItemsIdx, _curpage);
			int paytype = 2;
			if (itemid != -1)
			{
				ComInfo_T iteminfo;
				ParamMgr::getInstance()->getComGameItemInfo(itemid, iteminfo);
				paytype = iteminfo.paytype;
			}
			
			if (paytype == 2)
			{
				_btnOk->setVisible(false);
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::createWithTwoActions(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }));
				_selectItemsSprite->runAction(action);
			}
			else
			{
				_istouchbuy = true;
				showDelDialog();
			}		
		}
	}
	else 
	{
		if (endidx != -1 && endidx < g_onepagetnum && BagItemControl::getInstace()->getItemConfig(endidx, _curpage) == -1)
		{
			auto itemid = _vectorSellItems[_touchItemsIdx - g_onepagetnum];
			if (_shoptype == 2 && (WeaponControl::getInstance()->isWeaponUnLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
			{
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::create(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }), RemoveSelf::create(), nullptr);
				_selectItemsSprite->runAction(action);
			}
			else if (_shoptype == 1 && (!SkillControl::getInstance()->isSkillLock(itemid) || BagItemControl::getInstace()->isInBag(itemid)))
			{
				_isMove = true;
				auto truepos = _itemsPos[_touchItemsIdx];

				auto time = 0.3f;
				auto action = Sequence::create(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }), RemoveSelf::create(), nullptr);
				_selectItemsSprite->runAction(action);
			}
			else
			{
				_istouchbuy = true;
				showBuyDialog();
			}	
		}
		else 
		{
			_isMove = true;
			auto truepos = _itemsPos[_touchItemsIdx];

			auto time = 0.3f;
			auto action = Sequence::create(MoveTo::create(time, truepos), CallFunc::create([=]() {_isMove = false; }), RemoveSelf::create(), nullptr);
			_selectItemsSprite->runAction(action);
		}
	}
	
}

void ItemsShop::onTouchCancelled(Touch * touch, Event * unusedEvent)
{
}

void ItemsShop::showItem(int id)
{
	if (_shoptype == 2)
	{
		auto nodeinfo = _rootLayer->getChildByName("node_1");
		auto curnode = nodeinfo->getChildByName("wenponinfo");
		if (curnode)
		{
			curnode->removeFromParent();
		}

		auto wenponinfo = ParamMgr::getInstance()->getWeaponByID(id);

		auto node = Node::create();
		node->setName("wenponinfo");
		nodeinfo->addChild(node);
		node->setPosition(Vec2(-220.f, -40.f));

		if (wenponinfo.type == 0)
		{
			Vec2 pos(0, 0);
			auto bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			auto icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_3.png");
			bg->addChild(icon);
			auto text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.attack)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos);
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));
		}
		else if (wenponinfo.type == 1)
		{
			Vec2 pos(0, 0);
			auto bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			auto icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_0.png");
			bg->addChild(icon);
			auto text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.attack)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos);
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));

			bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_4.png");
			bg->addChild(icon);
			text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.def)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos + Vec2(120.f, 0.f));
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));

			bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_5.png");
			bg->addChild(icon);
			text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.dpadd)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos + Vec2(240.f, 0.f));
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));
		}
		else if (wenponinfo.type == 2)
		{
			Vec2 pos(0, 0);
			auto bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			auto icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_2.png");
			bg->addChild(icon);
			auto text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.attack)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos);
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));
		}
		else if (wenponinfo.type == 3)
		{
			Vec2 pos(0, 0);
			auto bg = Sprite::createWithSpriteFrameName("mainui_wenpon_shuxinbg.png");
			node->addChild(bg);
			auto icon = Sprite::createWithSpriteFrameName("mainui_shuxinicon_10.png");
			bg->addChild(icon);
			auto text = ui::TextAtlas::create(String::createWithFormat("%d", wenponinfo.hpadd)->getCString(), "fonts/font_wenpon_num.png", 16.f, 26.f, ".");
			bg->addChild(text);

			bg->setPosition(pos);
			icon->setPosition(Vec2(40.f, 25.f));
			text->setPosition(Vec2(85.f, 25.f));
		}

	}
}

void ItemsShop::hideItemInfo()
{
	auto nodeinfo = _rootLayer->getChildByName("node_1");
	auto curnode = nodeinfo->getChildByName("wenponinfo");
	if (curnode)
	{
		curnode->removeFromParent();
	}
}

bool StoreLayer::init()
{

	_lastTimeState[0] = -1;
	_lastTimeState[1] = -1;

	_lastListIdx = -1;

	Layer::init();

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/texteffect.ExportJson");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/storeui.plist");

	auto _touchOneByOneListener = EventListenerTouchOneByOne::create();
	_touchOneByOneListener->setSwallowTouches(true);
	_touchOneByOneListener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchOneByOneListener, this);

	auto off = VisibleRect::top().y - 960.f;

	_root = GameCSLoader::createNode("csb/storelayer.csb");
	this->addChild(_root);


	static_cast<ui::ImageView*>(_root->getChildByName("mainbg"))->setContentSize(Size(640.f, VisibleRect::top().y));
	_root->getChildByName("titlebg")->setPositionY(VisibleRect::top().y - 70.f);

	for (int i = 0; i < 3; i++)
	{
		auto list = _root->getChildByName(String::createWithFormat("list_%d", i)->getCString());
		list->setPositionY(VisibleRect::top().y - 180.f);
		list->setContentSize(Size(640.f, 600.f + off));
		_listview[i] = (ListView*)list;
	}
	

	_hour_0 = static_cast<TextAtlas*>(_root->getChildByName("countdown_0")->getChildByName("hour"));
	_hour_1 = static_cast<TextAtlas*>(_root->getChildByName("countdown_1")->getChildByName("hour"));

	_min_0 = static_cast<TextAtlas*>(_root->getChildByName("countdown_0")->getChildByName("min"));
	_min_1 = static_cast<TextAtlas*>(_root->getChildByName("countdown_1")->getChildByName("min"));

	_timedanwei[0] = static_cast<Sprite*>(_root->getChildByName("countdown_0")->getChildByName("text_xs_31"));
	_timedanwei[1] = static_cast<Sprite*>(_root->getChildByName("countdown_0")->getChildByName("text_fz_32"));
	_timedanwei[2] = static_cast<Sprite*>(_root->getChildByName("countdown_1")->getChildByName("text_xs_31"));
	_timedanwei[3] = static_cast<Sprite*>(_root->getChildByName("countdown_1")->getChildByName("text_fz_32"));

	_texttouch_0 = static_cast<Sprite*>(_root->getChildByName("touch_0"));
	_texttouch_1 = static_cast<Sprite*>(_root->getChildByName("touch_1"));

	_timeNode0 = _root->getChildByName("countdown_0");
	_timeNode1 = _root->getChildByName("countdown_1");

	_ani0 = Armature::create("texteffect");
	_ani0->getAnimation()->play("text_0");
	this->addChild(_ani0);
	_ani0->setPosition(_texttouch_0->getPosition());
	_ani0->getAnimation()->setSpeedScale(0.6f);
	
	_ani1 = Armature::create("texteffect");
	_ani1->getAnimation()->play("text_1");
	this->addChild(_ani1);
	_ani1->setPosition(_texttouch_1->getPosition());
	_ani1->getAnimation()->setSpeedScale(0.6f);

	auto actionlight = RepeatForever::create(RotateBy::create(10.f, 360.f));

	auto light0 = _root->getChildByName("box_bg_2_22");
	auto light1 = _root->getChildByName("box_bg_2_23");
	light0-> runAction(actionlight);
	light1->runAction(actionlight->clone());

	light0->setVisible(false);
	light1->setVisible(false);

	auto action = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {		
		click();
	}));

	this->runAction(RepeatForever::create(action));

	static_cast<Button*>(_root->getChildByName("touchget_0"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto main = MainLayer::getCurMainLayer();
			auto node0 = MainLayer::getCurMainLayer()->getTimeNode(0);
			if (node0->getDur() > 0)
			{
				return;
			}

			int lv[] = {0,6,11,21,31,41};
			int goldstart[] = { 50, 80, 120, 200, 400, 1000 };
			int goldadd[] = {6, 8, 6, 10, 40, 25};

			int playerlv = 0;
			for (int i = 0; i < 4; i++)
			{
				int curlv = UserData::getInstance()->getPlayerCurLv(i);
				if (curlv > playerlv)
				{
					playerlv = curlv;
				}
			}

			int idx = 0;
			for (int i = 3; i >= 0; i--)
			{
				if (playerlv>= lv[i])
				{
					idx = i;
				}
			}

			int rewardgold = goldstart[idx] + (playerlv - lv[idx])*goldadd[idx];
			showLBoxReward(rewardgold);
			node0->setDur(3 * 60 * 60);

		}
	});

	static_cast<Button*>(_root->getChildByName("touchget_1"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			auto main = MainLayer::getCurMainLayer();
			auto node1 = MainLayer::getCurMainLayer()->getTimeNode(1);
			if (node1->getDur() > 0)
			{
				return;
			}

			int lv[] = { 0,6,11,21,31,41 };

			int playerlv = 0;
			for (int i = 0; i < 4; i++)
			{
				int curlv = UserData::getInstance()->getPlayerCurLv(i);
				if (curlv > playerlv)
				{
					playerlv = curlv;
				}
			}

			int idx = 0;
			for (int i = 3; i >= 0; i--)
			{
				if (playerlv >= lv[i])
				{
					idx = i;
				}
			}

			
			showRBoxReward(idx);	
			node1->setDur(12 * 60 * 60);

		}
	});

	auto btncancle = static_cast<Button*>(_root->getChildByName("btn_cancle"));
	btncancle->setPositionY(VisibleRect::top().y - 110.f);
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			MainLayer::getCurMainLayer()->enterMainLayer();
			this->removeFromParent();
		}
	});

	auto titlebg = _root->getChildByName("titlebg");
	for (int i = 0; i < 3; i++)
	{
		_btnList[i] = static_cast<Button*>(titlebg->getChildByName(String::createWithFormat("btn_%d", i)->getCString()));
		//_btnList[i]->setUserData((void*)(i));
		_spriteBtnName[i] = static_cast<Sprite*>(titlebg->getChildByName(String::createWithFormat("text_%d", i)->getCString()));
		_btnList[i]->addTouchEventListener([=](Ref* btn, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				//auto idx = (int)(((Node*)(btn))->getUserData());
				switchList(i);
			}
		});
	}

	click();

	switchList(0);
	updateListSuit(0);
	for (int i = 0; i < 4; i++)
	{
		auto btn = static_cast<Button*>(_listview[0]->getChildByName(String::createWithFormat("root_%d", i)->getCString())->getChildByName("btn_buy"));
		btn->setUserData((void*)(&g_list_item_0[i]));
		btn->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonTouchCallback, this));
	}

	if (_listview[0]->getChildByName("root_4"))
	{
		auto btn = static_cast<Button*>(_listview[0]->getChildByName(String::createWithFormat("root_%d", 4)->getCString())->getChildByName("btn_buy"));
		btn->setUserData((void*)(&g_list_item_0[4]));
		btn->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonTouchCallback, this));
	}


	for (int i = 0; i < 7; i++)
	{
		auto btn = static_cast<Button*>(_listview[1]->getChildByName(String::createWithFormat("root_%d", i)->getCString())->getChildByName("btn_buy"));
		btn->setUserData((void*)(&g_list_item_1[i]));
		btn->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonTouchCallback, this));
	}
	for (int i = 0; i < 4; i++)
	{
		auto btn = static_cast<Button*>(_listview[2]->getChildByName(String::createWithFormat("root_%d", i)->getCString())->getChildByName("btn_buy"));
		btn->setUserData((void*)(&g_list_item_2[i]));
		btn->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonTouchCallback, this));
	}

	//upDataWenponSuit(3);
	return true;
}

void StoreLayer::switchTimeShow(int idx, bool showtime)
{
	if (idx == 0)
	{
		_texttouch_0->setVisible(!showtime);
		_timeNode0->setVisible(showtime);
		_ani0->setVisible(!showtime);

		auto light0 = _root->getChildByName("box_bg_2_22");
		light0->setVisible(!showtime);
	}
	else if (idx == 1)
	{
		_texttouch_1->setVisible(!showtime);
		_timeNode1->setVisible(showtime);
		_ani1->setVisible(!showtime);

		auto light1 = _root->getChildByName("box_bg_2_23");		
		light1->setVisible(!showtime);
	}
}

void StoreLayer::updateTimePic(int idx, int hour)
{
	int state = hour > 0 ? 1 : 0;

	if (_lastTimeState[idx] == state)
	{
		return;
	}

	if (idx == 0)
	{
		if (hour >0)
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

void StoreLayer::click()
{
	auto main = MainLayer::getCurMainLayer();
	auto node0 = MainLayer::getCurMainLayer()->getTimeNode(0);
	auto node1 = MainLayer::getCurMainLayer()->getTimeNode(1);

	auto time0 = node0->getDur();
	auto time1 = node1->getDur();

	if (time0 < 0)
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
	}


	if (time1 < 0)
	{
		switchTimeShow(1, false);
	}
	else
	{
		int hour = time1 / 3600;
		int min = time1 % 3600 / 60;
		int sec = time1 % (60);

		updateTimePic(1, hour);
		switchTimeShow(1, true);

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
}

void StoreLayer::switchList(int idx)
{
	if (_lastListIdx == idx)
	{
		return;
	}

	_lastListIdx = idx;

	for (int i = 0; i < 3; i++)
	{
		if (i == idx)
		{
			_listview[i]->setVisible(true);
			_btnList[i]->loadTextures("store_titlebg_1.png", "", "", Widget::TextureResType::PLIST);
			_spriteBtnName[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_btntextname[i * 2 + 1]));
		}
		else
		{
			_listview[i]->setVisible(false);
			_btnList[i]->loadTextures("store_titlebg_0.png", "", "", Widget::TextureResType::PLIST);
			_spriteBtnName[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_btntextname[i * 2]));
		}
	}
}

void StoreLayer::updateListSuit(int idx)
{
	auto root = _listview[0]->getChildByName("root_4");
	
	if (!root)
	{
		return;
	}

	const char* name[4] = { "lsj_jian_%02d.png","cbd_dun_%02d.png","lqc_biao_%02d.png","sqy_hua_%02d.png" };
	ImageView* item[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		item[i] = static_cast<ImageView*>(root->getChildByName(String::createWithFormat("item_%d", i)->getCString()));
	}

	auto title = static_cast<Sprite*>(root->getChildByName("store_text_title_0.png"));

	if (checkAllWenponLv(9))
	{
		root->removeFromParent();
	}

	if (checkAllWenponLv(7))
	{
		for (int i = 0; i < 4; i++)
		{
			
			item[i]->loadTexture(String::createWithFormat(name[i], 10)->getCString(),Widget::TextureResType::PLIST);
		}
		g_list_item_0[4] = StoreAssetMgr::ITEMID_GOOD_WENPONPACK_4;
		_wenponlistidx = 4;
	}
	else if (checkAllWenponLv(5))
	{
		for (int i = 0; i < 4; i++)
		{
			item[i]->loadTexture(String::createWithFormat(name[i], 8)->getCString(), Widget::TextureResType::PLIST);
		}
		g_list_item_0[4] = StoreAssetMgr::ITEMID_GOOD_WENPONPACK_3;
		_wenponlistidx = 3;
	}
	else if (checkAllWenponLv(3))
	{
		for (int i = 0; i < 4; i++)
		{
			item[i]->loadTexture(String::createWithFormat(name[i], 6)->getCString(), Widget::TextureResType::PLIST);
		}
		g_list_item_0[4] = StoreAssetMgr::ITEMID_GOOD_WENPONPACK_2;
		_wenponlistidx = 2;
	}
	else if(checkAllWenponLv(1))
	{
		for (int i = 0; i < 4; i++)
		{
			item[i]->loadTexture(String::createWithFormat(name[i], 4)->getCString(), Widget::TextureResType::PLIST);
		}
		g_list_item_0[4] = StoreAssetMgr::ITEMID_GOOD_WENPONPACK_1;
		_wenponlistidx = 1;
	}
	else
	{
		g_list_item_0[4] = StoreAssetMgr::ITEMID_GOOD_WENPONPACK_0;
		_wenponlistidx = 0;
	}
}

bool StoreLayer::checkAllWenponLv(int wenponlv)
{
	int startid = 500;
	for (int i = 0; i < 4; i++)
	{
		auto isunlock = WeaponControl::getInstance()->isWeaponUnLock(startid + 10 * i + wenponlv);
		auto isinbag = BagItemControl::getInstace()->isInBag(startid + 10 * i + wenponlv);
		if (isunlock == false && isinbag == false)
		{
			return false;
		}
	}
	return true;
}

void StoreLayer::buttonTouchCallback(Ref * btn, ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}
	auto name = *((std::string*)(((Node*)(btn))->getUserData()));

	auto layer = PurchaseLayer::create(name, "", this, true);
	this->addChild(layer);
}

void StoreLayer::showLBoxReward(int gold)
{
	std::vector<PopItemInfo_T> arrItems;
	PopItemInfo_T tmp;
	tmp.itemId = ParamData::GOLD_ITEM_ID;
	tmp.itemCount = gold;

	arrItems.push_back(tmp);

	auto pPop = PopRewardLayer::create(arrItems);
	this->addChild(pPop, 100);
}

void StoreLayer::showRBoxReward(int idx)
{
	int itemid[][3][2] = {
		{
			{ 1000, 3 },{ 1001, 3 },{ 1008, 3 }
		},
		{
			{ 1000, 5 },{ 1001, 6 },{ 1008, 5 }
		},
		{
			{ 1002, 3 },{ 1003, 4 },{ 1009, 3 }
		},
		{
			{ 1002, 5 },{ 1003, 6 },{ 1010, 3 }
		},
		{
			{ 1005, 3 },{ 1006, 4 },{ 1011, 3 }
		},
		{
			{ 1005, 3 },{ 1006, 4 },{ 1012, 3 }
		},
	};
	std::vector<PopItemInfo_T> arrItems;

	for (int i = 0; i < 3; i++)
	{
		PopItemInfo_T tmp;
		tmp.itemId = itemid[idx][i][0];
		tmp.itemCount = itemid[idx][i][1];

		arrItems.push_back(tmp);
	}

	auto pPop = PopRewardLayer::create(arrItems);
	this->addChild(pPop, 100);
}

void StoreLayer::menuBuyCallback(std::string itemid, bool succeed)
{
	if (succeed)
	{
		updateListSuit(0);

		if (itemid.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_0) == 0 || itemid.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_1) == 0 || itemid.compare(StoreAssetMgr::ITEMID_GOOD_ITEMPACK_2) == 0)
		{
			cocos2dx_plat::showToast(ResMgr::getInstance()->getString("pop_toast_0")->getCString());
		}
		else
		{
			showNewGet();
		}
		
	}
}

void StoreLayer::upDataWenponSuit(int idx)
{
	bool needchange = true;
	std::string wenponname[4] = { "lsj_jian_%02d.png", "cbd_dun_%02d.png", "lqc_biao_%02d.png", "sqy_hua_%02d.png" };

	for (int i = 0; i < 4; i++)
	{
		if (WeaponControl::getInstance()->isWeaponUnLock(500 + i * 10 + g_wenponidx[idx]))
		{

		}
		else
		{
			needchange = false;
			break;
		}
	}

	if (needchange)
	{
		static_cast<ImageView*>(_listview[0]->getChildByName("root_4")->getChildByName("item_0"))->loadTexture(String::createWithFormat(wenponname[0].c_str(), g_wenponidx[idx])->getCString(), Widget::TextureResType::PLIST);
		static_cast<ImageView*>(_listview[0]->getChildByName("root_4")->getChildByName("item_0"))->loadTexture(String::createWithFormat(wenponname[1].c_str(), g_wenponidx[idx])->getCString(), Widget::TextureResType::PLIST);
		static_cast<ImageView*>(_listview[0]->getChildByName("root_4")->getChildByName("item_0"))->loadTexture(String::createWithFormat(wenponname[2].c_str(), g_wenponidx[idx])->getCString(), Widget::TextureResType::PLIST);
		static_cast<ImageView*>(_listview[0]->getChildByName("root_4")->getChildByName("item_0"))->loadTexture(String::createWithFormat(wenponname[3].c_str(), g_wenponidx[idx])->getCString(), Widget::TextureResType::PLIST);
	}
	else
	{
		idx = idx--;
		
		if (idx < 0)
		{
			return;
		}

		upDataWenponSuit(idx);
	}

}

void StoreLayer::showNewGet()
{
	int cur = BagItemControl::getInstace()->getNeedShowItemOne();
	if (cur != -1)
	{
		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer, 5);

		auto textbg = Sprite::createWithSpriteFrameName("mainui_wenpontext_bg.png");
		layer->addChild(textbg);
		textbg->setPosition(Vec2(320.f, VisibleRect::top().y / 2));
		textbg->setScale(0.7f);

		auto effectsp = Sprite::createWithSpriteFrameName("effect_com_light.png");
		layer->addChild(effectsp);
		effectsp->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f + 180.f));
		auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
		effectsp->runAction(actionrotate);

		int playeridx = 0;
		std::string name;
		std::string get;
		std::string itemname;

		std::string title;
		if (BagItemControl::getInstace()->checkItemType(cur) == 1)
		{
			playeridx = SkillControl::getInstance()->getSkillOwnerBySkillid(cur);
			get = ResMgr::getInstance()->getString("getnewskill")->getCString();
			title = ResMgr::getInstance()->getString("getnewskilltitle")->getCString();
		}
		else
		{
			//warming 500 is the start id of wenponid
			playeridx = (cur - 500) / 10;
			get = ResMgr::getInstance()->getString("getneww")->getCString();
			title = ResMgr::getInstance()->getString("getnewwtitle")->getCString();
		}

		name = ResMgr::getInstance()->getString(String::createWithFormat("player_%d", playeridx)->getCString())->getCString();

		ComInfo_T temp;
		ParamMgr::getInstance()->getComGameItemInfo(cur, temp);

		itemname = temp.name;

		auto richtextCur = RichText::create();
		auto temp0 = RichElementText::create(0, Color3B(145, 80, 39), 255, name, "", 24);
		auto temp1 = RichElementText::create(0, Color3B(), 255, get, "", 24);
		auto temp2 = RichElementText::create(0, Color3B(145, 80, 39), 255, itemname, "", 24);

		richtextCur->pushBackElement(temp0);
		richtextCur->pushBackElement(temp1);
		richtextCur->pushBackElement(temp2);

		richtextCur->setPosition(Vec2(320.f, VisibleRect::top().y / 2 - 15.f));
		layer->addChild(richtextCur);

		auto texttitle = Text::create();
		layer->addChild(texttitle);
		texttitle->setString(title);
		texttitle->setFontSize(30);
		texttitle->setPosition(Vec2(320.f, VisibleRect::top().y / 2 + 45.f));
		texttitle->setTextColor(Color4B(129, 77, 0, 255));

		auto sprite = Sprite::createWithSpriteFrameName(temp.picname);
		layer->addChild(sprite);
		sprite->setPosition(Vec2(320.f, VisibleRect::top().y / 2.f + 180.f));

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch * touch, Event * unusedEvent) {
			int nextidx = BagItemControl::getInstace()->getNeedShowItemOne();
			if (nextidx != -1)
			{
				ComInfo_T tempnext;
				ParamMgr::getInstance()->getComGameItemInfo(nextidx, tempnext);
				sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(tempnext.picname));

				int playeridx = 0;
				std::string name;
				std::string get;
				std::string itemname;
				std::string title;
				if (BagItemControl::getInstace()->checkItemType(nextidx) == 1)
				{
					playeridx = SkillControl::getInstance()->getSkillOwnerBySkillid(nextidx);
					get = ResMgr::getInstance()->getString("getnewskill")->getCString();
					title = ResMgr::getInstance()->getString("getnewskilltitle")->getCString();
				}
				else
				{
					//warming 500 is the start id of wenponid
					playeridx = (nextidx - 500) / 10;
					get = ResMgr::getInstance()->getString("getneww")->getCString();
					title = ResMgr::getInstance()->getString("getnewwtitle")->getCString();
				}

				name = ResMgr::getInstance()->getString(String::createWithFormat("player_%d", playeridx)->getCString())->getCString();
				texttitle->setString(title);

				itemname = tempnext.name;

				auto temp0 = RichElementText::create(0, Color3B(145, 80, 39), 255, name, "", 24);
				auto temp1 = RichElementText::create(0, Color3B(), 255, get, "", 24);
				auto temp2 = RichElementText::create(0, Color3B(145, 80, 39), 255, itemname, "", 24);

				richtextCur->removeElement(2);
				richtextCur->removeElement(1);

				richtextCur->pushBackElement(temp0);
				richtextCur->pushBackElement(temp1);
				richtextCur->pushBackElement(temp2);

				richtextCur->removeElement(0);
			}
			else
			{
				layer->removeFromParent();
			}

			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
	}
}

bool StoreLayer2::init()
{
	Layer::init();
	
	ParamMgr::getInstance()->reloadStoreFreshItemsConfig();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {  };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_curTagIdx = 0;

	auto root = GameCSLoader::createNode("csb/newstoreui.csb");
	this->addChild(root);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	root->setPositionY(-VisibleRect::top().y + 120.f);
	root->runAction(actionshow);

	//exchange
	_exchangeNode = root->getChildByName("Node_1");
	_exchangeNode->setPositionY(VisibleRect::top().y - 960.f);
	_exchangeTextField = static_cast<TextField*>(_exchangeNode->getChildByName("textfield"));
	_exchangeBtn = static_cast<Button*>(_exchangeNode->getChildByName("exchange"));
	_exchangeNode->setVisible(false);

	static_cast<ImageView*>(_exchangeNode->getChildByName("Image_45_0"))->loadTexture("com_input.png", Widget::TextureResType::PLIST);

	_exchangeBtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			std::string strInput = _exchangeTextField->getString();

			if (!strInput.empty() && "" != strInput)
			{
				NetDataMgr::getInstance()->checkExchangeCode(strInput, CC_CALLBACK_2(MainLayer::onCheckCodeCallback, MainLayer::getCurMainLayer()));
			}
			else
			{
				GameUtils::toastTip(6);
			}
		}
	});

	_exchangeTextField->addEventListener([=](Ref*, TextField::EventType type) {
		if (type == TextField::EventType::ATTACH_WITH_IME)
		{
			_exchangeNode->setPositionY(VisibleRect::top().y / 2);
		}
		else if (type == TextField::EventType::DETACH_WITH_IME)
		{
			_exchangeNode->setPositionY(0.f);
		}
	});

	//fix bg size
	static_cast<ImageView*>(root->getChildByName("Image_44"))->setContentSize(Size(640.f, VisibleRect::top().y - 60.f));
	static_cast<ImageView*>(root->getChildByName("Image_1"))->setContentSize(Size(531.f, VisibleRect::top().y - 180.f));

	//init nodes
	_topNode = static_cast<Node*>(root->getChildByName("Node_2"));
	_mainPageView = static_cast<PageView*>(root->getChildByName("PageView_1"));
	_mainPageView->setTouchEnabled(false);

	for (int i = 0; i < 4; i++)
	{
		_pageRootNode[i] = _mainPageView->getPage(i)->getChildByName("pageroot");
		_tagChangeBtns[i] = static_cast<Button*>(_topNode->getChildByName(String::createWithFormat("selectbtn_%d", i)->getCString()));
		_tagChangeBtns[i]->setUserData((void*)(i));
		_tagChangeBtns[i]->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageChangeBtn, this));
		_tagTextSprites[i] = static_cast<Sprite*>(_topNode->getChildByName(String::createWithFormat("tag_%d", i)->getCString()));
	}

	//inti btns call
	for (int i = 0; i < 9; i++)
	{
		auto btn = _pageRootNode[1]->getChildByName(String::createWithFormat("item_%d", i)->getCString());
		btn->setUserData((void*)(i));
		((Button*)btn)->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageOneBtns, this));

		auto infobtn = GameButton::create("store_iteminfo_btn.png", "", "", Widget::TextureResType::PLIST);
		infobtn->setUserData((void*)(i));
		btn->addChild(infobtn, 3);
		infobtn->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageOneShowInfoBtns, this));
		infobtn->setPosition(Vec2(115.f, 155.f) + Vec2(20.f, 30.f) + Vec2(0.f, -30.f));
		infobtn->setScale(1.2f);

		_arrPageOneItems[i].icon = static_cast<Sprite*>(btn->getChildByName("Sprite_121"));
		_arrPageOneItems[i].payicon = static_cast<Sprite*>(btn->getChildByName("Sprite_2"));
		_arrPageOneItems[i].getnum = static_cast<TextAtlas*>(btn->getChildByName("AtlasLabel_22_0"));
		_arrPageOneItems[i].pricetext = static_cast<Text*>(btn->getChildByName("Text_2"));
		_arrPageOneItems[i].name = static_cast<Text*>(btn->getChildByName("Text_1_0"));
		_arrPageOneItems[i].particon = static_cast<Sprite*>(btn->getChildByName("part"));
		_arrPageOneItems[i].particon->setVisible(false);
		_arrPageOneItems[i].bgicon = static_cast<Sprite*>(btn->getChildByName("Sprite_120"));
		_arrPageOneItems[i].infoBtn = infobtn;

		auto nodeout = Node::create();
		_pageRootNode[1]->addChild(nodeout, 1);
		nodeout->setPosition(btn->getPosition());

		auto bgnode = Sprite::createWithSpriteFrameName("com_soldout_bg.png");
		nodeout->addChild(bgnode);

		auto out = Sprite::createWithSpriteFrameName("com_soldout.png");
		nodeout->addChild(out);

		_arrPageOneItems[i].soldout = nodeout;
	}

	auto btn = _pageRootNode[1]->getChildByName("Button_27");
	((Button*)btn)->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnRefresh, this));

	auto pricebtn = static_cast<TextAtlas*>(btn->getChildByName("AtlasLabel_22"));
	pricebtn->setString(String::createWithFormat("%d", StoreAssetMgr::getInstance()->getSinglePrice(StoreAssetMgr::ITEMID_GOOD_REFRESH_STORE))->getCString());

	for (int i = 0; i < 3; i++)
	{
		auto btn = _pageRootNode[0]->getChildByName(String::createWithFormat("btn_%d", i)->getCString());
		btn->setUserData((void*)(i));
		((Button*)btn)->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageTwoBtns, this));

		//int gold = BoxDataMgr::getInstance()->getBoxDescGoldCount(i + 4, BoxDataMgr::getInstance()->getCurGetBoxLv());
		int wenponparts = BoxDataMgr::getInstance()->getBoxAboveChipCount(i + 4, BoxDataMgr::getInstance()->getCurGetBoxLv(),0, 1);

		int wenponparts2 = 0;
		if (i == 0)
		{
			wenponparts2 = BoxDataMgr::getInstance()->getBoxAboveChipCount(i + 4, BoxDataMgr::getInstance()->getCurGetBoxLv(), 1, 1);
		}
		else
		{
			wenponparts2 = BoxDataMgr::getInstance()->getBoxAboveChipCount(i + 4, BoxDataMgr::getInstance()->getCurGetBoxLv(), 2, 1);
		}
		
		auto textdes = static_cast<Text*>(btn->getChildByName("partsnumtext"));
		textdes->setString(String::createWithFormat(textdes->getString().c_str(), wenponparts)->getCString());
		auto textdes2 = static_cast<Text*>(btn->getChildByName("partsnumtext2"));
		textdes2->setString(String::createWithFormat(textdes2->getString().c_str(),wenponparts2)->getCString());

		auto costnum = static_cast<Text*>(btn->getChildByName("num"));
		int price = BoxDataMgr::getInstance()->getBoxBuyCost(4 + i);
		costnum->setString(String::createWithFormat("%d", price)->getCString());	

		auto light = (btn->getChildByName("light"));
		auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
		light->runAction(actionrotate);
	}

	for (int i = 0; i < 4; i++)
	{
		auto btn = _pageRootNode[2]->getChildByName(String::createWithFormat("btn_%d", i)->getCString());
		btn->setUserData((void*)(i));
		((Button*)btn)->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageFourBtns, this));
	}

	for (int i = 0; i < 4; i++)
	{
		auto btn = _pageRootNode[3]->getChildByName(String::createWithFormat("btn_%d", i)->getCString());
		btn->setUserData((void*)(i));
		((Button*)btn)->addTouchEventListener(CC_CALLBACK_2(StoreLayer2::menuOnPageThreeBtns, this));
	}

	//fix pos;
	_topNode->setPositionY(VisibleRect::top().y - 960.f);
	_mainPageView->setPositionY(VisibleRect::top().y - 960.f);
	
	auto btn_exit = static_cast<Button*>(_topNode->getChildByName("btn_exit"));
	btn_exit->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	_timenode = TimeCountDownNode::createWithNode(_refreshTime, TimeTypeEnum::TIME_SHOPHOTREFRESH);
	this->addChild(_timenode);

	static_cast<Text*>(_pageRootNode[1]->getChildByName("Text_1"))->setString(ResMgr::getInstance()->getString("toast_tip_1")->getCString());
	/*_timenode->_errorcallback = [=]() {
		static_cast<Text*>(_pageRootNode[0]->getChildByName("Text_1"))->setString(ResMgr::getInstance()->getString("toast_tip_1")->getCString());
	};*/

	_timenode->setTimeCallBack([=](int click) {

		if (click >= 0)
		{
			int hour = click / 3600;
			int min = click % 3600 / 60;
			int sec = click % (60);
			static_cast<Text*>(_pageRootNode[1]->getChildByName("Text_1"))->setString(String::createWithFormat(ResMgr::getInstance()->getString("text_fresh")->getCString(), hour, min, sec)->getCString());
		}
		else
		{
			freshenItems(true);
			_timenode->setDur(_refreshTime);
		}
	
	});

	_timenode->startCountDown();

	if (UserData::getInstance()->getStorePageItemid(0) == -1)
	{
		freshenItems(true);
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			_storeItemCurIdx[i] = UserData::getInstance()->getStorePageItemid(i);
		}
		freshenItems(false);
	}
	refreshTagBtnsShow();
	return true;
}

void StoreLayer2::refreshTagBtnsShow()
{
	std::string temppicnames = { "store_text_tag_%d_%d.png" };
	std::string truetemppicnames[] = { "store_text_tag_1_%d.png", "store_text_tag_0_%d.png","store_text_tag_3_%d.png","store_text_tag_2_%d.png" };

	for (int i = 0; i < 4; i++)
	{
		_tagTextSprites[i]->setLocalZOrder(2);

		if (i == _curTagIdx)
		{
			_tagTextSprites[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(truetemppicnames[i].c_str(), 1)->getCString()));
			_tagChangeBtns[i]->loadTextures("store_tag_1.png", "", "", Widget::TextureResType::PLIST);
			_tagChangeBtns[i]->setLocalZOrder(1);
		}
		else
		{
			_tagTextSprites[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(truetemppicnames[i].c_str(), 0)->getCString()));
			_tagChangeBtns[i]->loadTextures("store_tag_0.png", "", "", Widget::TextureResType::PLIST);

			_tagChangeBtns[i]->setLocalZOrder(0);
		}
		if (_curTagIdx == 1)
		{
			_exchangeNode->setVisible(false);
		}
		else
		{
			_exchangeNode->setVisible(true);
		}
	}
}

void StoreLayer2::menuOnPageChangeBtn(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int idx = int(((Node*)(ref))->getUserData());
	_mainPageView->setCurPageIndex(idx);
	_curTagIdx = idx;

	refreshTagBtnsShow();
}

void StoreLayer2::menuOnRefresh(Ref * ref, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int curtimes = UserData::getInstance()->getRefashTimes();
		
		curtimes = curtimes > 9 ? 9 : curtimes;

		auto& priceinfo = g_arrResfresh[curtimes];
		int canafford = -1;
		if (priceinfo.paytype == 0)
		{
			if (UserData::getInstance()->getGoldNum() >= priceinfo.num)
			{
				canafford = 0;
				UserData::getInstance()->giveGold(-priceinfo.num);
			}
			else
			{
				canafford = -2;
			}
		}
		else if(priceinfo.paytype == 2)
		{
			if (UserData::getInstance()->getCrystalNum() >= priceinfo.num)
			{
				canafford = 2;
				UserData::getInstance()->giveCrystal(-priceinfo.num);
			}
			else
			{
				canafford = -3;
			}
		
		}

		if (canafford == 0 || canafford ==2)
		{
			auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_REFRESH_STORE, "", this, true);
			this->addChild(layer);
		}
		else if (canafford == -2)
		{
			GameUtils::toastTip("no_gold");
		}
		else if (canafford == -3)
		{
			GameUtils::toastTip("no_caystal");
		}
	}
}

void StoreLayer2::menuOnPageOneBtns(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int btnidx = int(((Node*)(ref))->getUserData());
	CCLOG("paget %d: btn-%d touched", _curTagIdx, btnidx);

	int num = UserData::getInstance()->getStorePageStage(btnidx);
	if (num <= 0)
	{
		return;
	}

	auto& items = ParamMgr::getInstance()->getStorePageOneItems();
	auto idx =UserData::getInstance()->getStorePageItemid(btnidx);
	StorePageOneItem_T info;

	if (btnidx >= 1 && btnidx <= 6)
	{
		auto tempcofig = ParamMgr::getInstance()->getHotStoreWenponsByIdx(_storeItemCurIdx[btnidx]);
		info.des = tempcofig->name;
		info.itemid = tempcofig->wenponpartid;
		info.num = tempcofig->num;
		info.price = tempcofig->price;
		info.type = tempcofig->pricetype;
	}
	else
	{
		info = items[idx];
	}

	//check is skill allready buy
	int itemtype = BagItemControl::getInstace()->checkItemType(info.itemid);
	if (itemtype == 1)
	{
		if ((!SkillControl::getInstance()->isSkillLock(info.itemid) || BagItemControl::getInstace()->isInBag(info.itemid)))
		{
			cocos2dx_plat::showToast(ResMgr::getInstance()->getString("allready_have_skill")->getCString());
			return;
		}
	}
	

	bool enough = false;
	if (info.type == 0)
	{
		if (UserData::getInstance()->getGoldNum() >= info.price)
		{
			enough = true;
			UserData::getInstance()->giveGold(-info.price);
		}
		else
		{
			cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_gold")->getCString());
		}
	}
	else if (info.type == 2)
	{
		if (UserData::getInstance()->getCrystalNum() >= info.price)
		{
			enough = true;
			UserData::getInstance()->giveCrystal(-info.price);

			DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYBUYZUANSHI);
		}
		else
		{
			cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_caystal")->getCString());
		}
	}

	if (enough)
	{
		UserData::getInstance()->setStorePageStage(btnidx, 0);
		DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYBUYITEMS);
		/*	auto type = BagItemControl::getInstace()->checkItemType(info.itemid);
			if (type == 3)
			{
				UserData::getInstance()->addWeaponPartsNum(info.itemid, info.num);
			}
			else if (type == 1)
			{
				UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_skill_%d", info.itemid)->getCString(), 1);
				BagItemControl::getInstace()->addBagItem(info.itemid);
			}
			else if (type == 0)
			{
				for (int i = 0; i < info.num; i++)
				{
					BagItemControl::getInstace()->addBagItem(info.itemid);
				}
			}*/

		auto layer = PopRewardLayer::create(info.itemid, info.num);
		this->addChild(layer, 10);
		//layer->showRewardTitle(0);
		auto analyItemid = string("hotstore_") + String::createWithFormat("%d", info.itemid)->getCString();
		cocos2dx_analyze::buy(analyItemid.c_str(), 1, 0);
		MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::SHOPING, 1);

		UserData::getInstance()->saveUserData();

		if (UserData::getInstance()->getStorePageStage(btnidx) > 0)
		{
			_arrPageOneItems[btnidx].soldout->setVisible(false);
		}
		else
		{
			_arrPageOneItems[btnidx].soldout->setVisible(true);
		}
	}
}

void StoreLayer2::menuOnPageTwoBtns(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int idx = int(((Node*)(ref))->getUserData());
	
	if (idx == 0)
	{
		auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_STOREBOX_0, "", this, true);
		this->addChild(layer);
	}
	else if (idx == 1)
	{
		auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_STOREBOX_1, "", this, true);
		this->addChild(layer);
	}
	else if (idx == 2)
	{
		auto layer = PurchaseLayer::create(StoreAssetMgr::ITEMID_GOOD_STOREBOX_2, "", this, true);
		this->addChild(layer);
	}

	CCLOG("paget %d: btn-%d touched", _curTagIdx, idx);
}

void StoreLayer2::menuOnPageThreeBtns(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int idx = int(((Node*)(ref))->getUserData());

	const char* items[] = { StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_0 , StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_1 , StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_2 , StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_3 };
	auto layer = PurchaseLayer::create(items[idx], "", this, true);
	this->addChild(layer);

	CCLOG("paget %d: btn-%d touched", _curTagIdx, idx);
}

void StoreLayer2::menuOnPageFourBtns(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}
	int idx = int(((Node*)(ref))->getUserData());

	const char* items[] = { StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_0 , StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_1 , StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_2 , StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_3 };
	auto layer = PurchaseLayer::create(items[idx], "", this, true);
	this->addChild(layer);

	
	CCLOG("paget %d: btn-%d touched", _curTagIdx, idx);
}

void StoreLayer2::menuOnPageOneShowInfoBtns(Ref * ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int idx = int(((Node*)(ref))->getUserData());
	
	auto& items = ParamMgr::getInstance()->getStorePageOneItems();
	auto& iteminfo = items[_storeItemCurIdx[idx]];
	int typeidx =BagItemControl::getInstace()->checkItemType(iteminfo.itemid);
	if (typeidx == 3)
	{
		showWenInfo(iteminfo.itemid - 1000, idx);
	}
	else if (typeidx == 1)
	{
		showSkillInfo(iteminfo.itemid, idx);
	}
	else if (typeidx == 0 || typeidx == -102)
	{
		showItemInfo(iteminfo.itemid, idx);
	}
	
}


void StoreLayer2::showBoxget(const std::vector<int>& itemids, const std::vector<int>& nums)
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

void StoreLayer2::freshenItems(bool changids)
{
	int curtimes = UserData::getInstance()->getRefashTimes();
	curtimes = curtimes > 9 ? 9 : curtimes;
	auto& priceinfo = g_arrResfresh[curtimes];

	const char* pricepicname[3] = { "com_goldbag.png","com_crystal.png","com_crystal.png" };

	auto nodebtn = _pageRootNode[1]->getChildByName("Button_27");
	static_cast<Sprite*>(nodebtn->getChildByName("Sprite_1"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pricepicname[priceinfo.paytype]));
	static_cast<TextAtlas*>(nodebtn->getChildByName("AtlasLabel_22"))->setString(String::createWithFormat("%d", priceinfo.num)->getCString());

	auto& items = ParamMgr::getInstance()->getStorePageOneItems();
	auto& config = ParamMgr::getInstance()->getStoreOneConfig();
	
	std::vector<int> arrPlayerWenponId[4];
	
	for (int i = 0; i < 4; i++)
	{
		WeaponControl::getInstance()->getPlayerUnlockWenponIds(i, arrPlayerWenponId[i]);
	}
	

	for (int i = 0; i < 9; i++)
	{
		auto& curposconfig = config[i];
		int total = int(curposconfig.size());
		int cur = ToolsUtil::getRandomInt(0, total -1);

		if (changids)
		{
			_storeItemCurIdx[i] = curposconfig[cur].pageoneitemidx - 1;
			UserData::getInstance()->setStorePageItemid(i, _storeItemCurIdx[i]);
			UserData::getInstance()->setStorePageStage(i, 1);
		}
		
		StorePageOneItem_T iteminfo;

		if (changids || _storeItemCurIdx[i] < WeaponControl::s_weaponStartIdx)
		{
			if (i >= 1 && i <= 4)
			{
				int selectidx = ToolsUtil::getRandomInt(0, ((int)arrPlayerWenponId[i - 1].size() - 1));
				int wenponid = arrPlayerWenponId[i - 1].at(selectidx);
				UserData::getInstance()->setStorePageItemid(i, wenponid);
				_storeItemCurIdx[i] = wenponid;
			}
			else if (i == 5 || i == 6)
			{
				int playerid = ToolsUtil::getRandomInt(0, 3);
				int wenponid = arrPlayerWenponId[playerid].back();
				UserData::getInstance()->setStorePageItemid(i, wenponid);
				_storeItemCurIdx[i] = wenponid;
			}
		}


		if (i >= 1 && i <= 6)
		{
			auto tempcofig = ParamMgr::getInstance()->getHotStoreWenponsByIdx(_storeItemCurIdx[i]);
			iteminfo.des = tempcofig->name;
			iteminfo.itemid = tempcofig->wenponpartid;
			iteminfo.num = tempcofig->num;
			iteminfo.price = tempcofig->price;
			iteminfo.type = tempcofig->pricetype;
		}
		else
		{
			iteminfo = items[_storeItemCurIdx[i]];
		}

		if (iteminfo.itemid == -102)
		{
			_arrPageOneItems[i].name->setString(ResMgr::getInstance()->getString("item_name_-102")->getCString());
			_arrPageOneItems[i].getnum->setString(String::createWithFormat("/%d", iteminfo.num)->getCString());
			_arrPageOneItems[i].pricetext->setString(String::createWithFormat("%d", iteminfo.price)->getCString());
			_arrPageOneItems[i].icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_icon_jingqi.png"));
			_arrPageOneItems[i].getnum->setScale(0.9f);
			_arrPageOneItems[i].particon->setVisible(false);

			if (iteminfo.num > 0)
			{
				_arrPageOneItems[i].getnum->setVisible(true);
			}
			else
			{
				_arrPageOneItems[i].getnum->setVisible(false);
			}

			if (iteminfo.type == 2)
			{
				_arrPageOneItems[i].payicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_crystal.png"));
			}
			else if (iteminfo.type == 0)
			{
				_arrPageOneItems[i].payicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_goldbag.png"));
			}
		}
		else
		{
			ComInfo_T cominfo;
			ParamMgr::getInstance()->getComGameItemInfo(iteminfo.itemid, cominfo);

			_arrPageOneItems[i].name->setString(cominfo.name);
			_arrPageOneItems[i].getnum->setString(String::createWithFormat("/%d", iteminfo.num)->getCString());
			_arrPageOneItems[i].pricetext->setString(String::createWithFormat("%d", iteminfo.price)->getCString());
			_arrPageOneItems[i].icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(cominfo.picname));
			_arrPageOneItems[i].getnum->setScale(0.9f);

			if (BagItemControl::getInstace()->checkItemType(iteminfo.itemid) == 3)
			{
				int picidx = (iteminfo.itemid - 1500) % 10;
				_arrPageOneItems[i].particon->setVisible(true);
				_arrPageOneItems[i].bgicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(g_iconbgname[picidx]));
				_arrPageOneItems[i].bgicon->setScale(0.9f);
			}
			else
			{
				_arrPageOneItems[i].bgicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("store_item_bg.png"));
				_arrPageOneItems[i].particon->setVisible(false);
				_arrPageOneItems[i].bgicon->setScale(1.0f);
			}

			if (iteminfo.num > 0)
			{
				_arrPageOneItems[i].getnum->setVisible(true);
			}
			else
			{
				_arrPageOneItems[i].getnum->setVisible(false);
			}

			if (iteminfo.type == 2)
			{
				_arrPageOneItems[i].payicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_crystal.png"));
			}
			else if (iteminfo.type == 0)
			{
				_arrPageOneItems[i].payicon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("com_goldbag.png"));
			}
		}
		
		if (UserData::getInstance()->getStorePageStage(i) > 0)
		{
			_arrPageOneItems[i].soldout->setVisible(false);
		}
		else
		{
			_arrPageOneItems[i].soldout->setVisible(true);
		}
	}

	if (changids)
	{
		UserData::getInstance()->saveUserData(true);
	}
}

void StoreLayer2::getReward(int boxid)
{
//	std::vector<PopItemInfo_T> arrReward;
//	BoxDataMgr::getInstance()->getBoxReward(boxid, BoxDataMgr::getInstance()->getCurGetBoxLv(), arrReward);

	auto layer = PopRewardLayer::openBox(boxid, BoxDataMgr::getInstance()->getCurGetBoxLv());//create(arrReward);
	this->addChild(layer);
}

void StoreLayer2::jumpToPage(int pageidx)
{
	_mainPageView->setCurPageIndex(pageidx);
	_curTagIdx = pageidx;
	refreshTagBtnsShow();
}

void StoreLayer2::showWenInfo(int wenponid, int btnidx)
{
	float posy[] = { 590.f, 590.f, 590.f, 380.f, 380.f, 380.f, 170.f, 170.f, 170.f };
	float posx[] = { 270.f, 320.f,370.f, 270.f, 320.f,370.f, 270.f, 320.f,370.f,};

	Vec2 tippos[] = { Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f), Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) ,Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) };

	Vec2 infopos(-155.f, -20.f);

	auto node = Node::create();

	auto tip = Sprite::createWithSpriteFrameName("store_iteminfo_tip.png");
	node->addChild(tip);
	tip->setPosition(*(tippos + btnidx));

	auto imageview = ImageView::create("store_iteminfo_tipbg.png", Widget::TextureResType::PLIST);
	imageview->setScale9Enabled(true);
	imageview->setContentSize(Size(370.f, 145.f));
	node->addChild(imageview, 0);
	this->addChild(node, 5);
	node->setPosition(Vec2(posx[btnidx], VisibleRect::top().y - 960.f + posy[btnidx] - 80.f));

	auto& wenponinfo = ParamMgr::getInstance()->getWeaponByID(wenponid);

	auto textname = Text::create();
	textname->setAnchorPoint(Vec2(0.f, 0.5f));
	node->addChild(textname, 1);
	textname->setFontSize(24);
	textname->setTextColor(Color4B(0xff, 0xaf, 0x04, 0xff));
	textname->setString(wenponinfo.name);
	textname->setPosition(infopos + Vec2(0.f, 40.f));

	if (wenponinfo.attack != 0)
	{
		auto attstring = ResMgr::getInstance()->getString("store_att");
		auto attvalue = String::createWithFormat("%d", wenponinfo.attack);

		auto textatt = Text::create();
		textatt->setFontSize(18);
		textatt->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		textatt->setString(attstring->getCString());
		node->addChild(textatt, 1);
		textatt->setPosition(infopos);
		
		infopos.x += 80.f;

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0x51, 0xff, 0x05, 0xff));
		textvalue->setString(attvalue->getCString());	
		node->addChild(textvalue, 1);
		textvalue->setPosition(infopos);

		infopos.x += 30.f;

		textatt->setAnchorPoint(Vec2(0.f, 0.5f));
		textvalue->setAnchorPoint(Vec2(0.f, 0.5f));
	}

	if (wenponinfo.def != 0)
	{
		auto attstring = ResMgr::getInstance()->getString("store_def");
		auto attvalue = String::createWithFormat("%d", wenponinfo.def);

		auto textatt = Text::create();
		textatt->setFontSize(18);
		textatt->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		textatt->setString(attstring->getCString());
		node->addChild(textatt, 1);
		textatt->setPosition(infopos);

		infopos.x += 80.f;

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0x51, 0xff, 0x05, 0xff));
		textvalue->setString(attvalue->getCString());
		node->addChild(textvalue, 1);
		textvalue->setPosition(infopos);

		infopos.x += 30.f;

		textatt->setAnchorPoint(Vec2(0.f, 0.5f));
		textvalue->setAnchorPoint(Vec2(0.f, 0.5f));
	}

	if (wenponinfo.dpadd != 0)
	{
		auto attstring = ResMgr::getInstance()->getString("store_dpadd");
		auto attvalue = String::createWithFormat("%d", wenponinfo.attack);

		auto textatt = Text::create();
		textatt->setFontSize(18);
		textatt->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		textatt->setString(attstring->getCString());
		node->addChild(textatt, 1);
		textatt->setPosition(infopos);

		infopos.x += 80.f;

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0x51, 0xff, 0x05, 0xff));
		textvalue->setString(attvalue->getCString());
		node->addChild(textvalue, 1);
		textvalue->setPosition(infopos);

		infopos.x += 30.f;

		textatt->setAnchorPoint(Vec2(0.f, 0.5f));
		textvalue->setAnchorPoint(Vec2(0.f, 0.5f));
	}

	if (wenponinfo.hpadd != 0)
	{
		auto attstring = ResMgr::getInstance()->getString("store_hpadd");
		auto attvalue = String::createWithFormat("%d", wenponinfo.hpadd);

		auto textatt = Text::create();
		textatt->setFontSize(18);
		textatt->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		textatt->setString(attstring->getCString());
		node->addChild(textatt, 1);
		textatt->setPosition(infopos);

		infopos.x += 80.f;

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0x51, 0xff, 0x05, 0xff));
		textvalue->setString(attvalue->getCString());
		node->addChild(textvalue, 1);
		textvalue->setPosition(infopos);

		infopos.x += 30.f;

		textatt->setAnchorPoint(Vec2(0.f, 0.5f));
		textvalue->setAnchorPoint(Vec2(0.f, 0.5f));
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [=](Touch*, Event*) {
		node->removeFromParent();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

void StoreLayer2::showSkillInfo(int skillid, int btnidx)
{
	float posy[] = { 590.f, 590.f, 590.f, 380.f, 380.f, 380.f, 170.f, 170.f, 170.f };
	float posx[] = { 270.f, 320.f,370.f, 270.f, 320.f,370.f, 270.f, 320.f,370.f, };

	Vec2 tippos[] = { Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f), Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) ,Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) };

	Vec2 infopos(-175.f, -20.f);

	auto node = Node::create();

	auto tip = Sprite::createWithSpriteFrameName("store_iteminfo_tip.png");
	node->addChild(tip);
	tip->setPosition(*(tippos + btnidx));

	auto imageview = ImageView::create("store_iteminfo_tipbg.png", Widget::TextureResType::PLIST);
	imageview->setScale9Enabled(true);
	imageview->setContentSize(Size(400.f, 145.f));
	node->addChild(imageview, 0);
	this->addChild(node, 5);
	node->setPosition(Vec2(posx[btnidx], VisibleRect::top().y - 960.f + posy[btnidx] - 80.f));

	auto& skillinfo = ParamMgr::getInstance()->getSkillByID(skillid);

	auto textname = Text::create();
	textname->setAnchorPoint(Vec2(0.f, 0.5f));
	node->addChild(textname, 1);
	textname->setFontSize(24);
	textname->setTextColor(Color4B(0xff, 0xaf, 0x04, 0xff));
	textname->setString(skillinfo.name);
	textname->setPosition(infopos + Vec2(0.f, 60.f));

	auto textvalue = Text::create();
	textvalue->setFontSize(18);
	textvalue->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
	node->addChild(textvalue, 1);
	textvalue->setString(skillinfo.des);

	auto length = skillinfo.des.length() / 2 / 18;
	textvalue->setTextAreaSize(Size(360.f, 20 * length));
	textvalue->setPosition(infopos + Vec2(180.f, 0.f));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [=](Touch*, Event*) {
		node->removeFromParent();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

void StoreLayer2::showItemInfo(int itemid, int btnidx)
{
	float posy[] = { 590.f, 590.f, 590.f, 380.f, 380.f, 380.f, 170.f, 170.f, 170.f };
	float posx[] = { 270.f, 320.f,370.f, 270.f, 320.f,370.f, 270.f, 320.f,370.f, };

	Vec2 tippos[] = { Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f), Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) ,Vec2(-110.f, 75.f), Vec2(0.f, 75.f) ,Vec2(150.f, 75.f) };

	Vec2 infopos(-175.f, -20.f);

	auto node = Node::create();

	auto tip = Sprite::createWithSpriteFrameName("store_iteminfo_tip.png");
	node->addChild(tip);
	tip->setPosition(*(tippos + btnidx));

	auto imageview = ImageView::create("store_iteminfo_tipbg.png", Widget::TextureResType::PLIST);
	imageview->setScale9Enabled(true);
	imageview->setContentSize(Size(400.f, 145.f));
	node->addChild(imageview, 0);
	this->addChild(node, 5);
	node->setPosition(Vec2(posx[btnidx], VisibleRect::top().y - 960.f + posy[btnidx] - 80.f));

	//_arrPageOneItems[i].name->setString(ResMgr::getInstance()->getString("item_name_-102")->getCString());
	if (itemid == -102)
	{
		auto textname = Text::create();
		textname->setAnchorPoint(Vec2(0.f, 0.5f));
		node->addChild(textname, 1);
		textname->setFontSize(24);
		textname->setTextColor(Color4B(0xff, 0xaf, 0x04, 0xff));
		textname->setString(ResMgr::getInstance()->getString("item_name_-102")->getCString());
		textname->setPosition(infopos + Vec2(0.f, 60.f));

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		node->addChild(textvalue, 1);
		std::string desvalue = ResMgr::getInstance()->getString("item_des_-102")->getCString();
		textvalue->setString(desvalue);

		auto length = desvalue.length() / 2 / 18;
		textvalue->setTextAreaSize(Size(360.f, 20 * length));
		textvalue->setPosition(infopos + Vec2(180.f, 0.f));
	}
	else
	{
		const Item_T& skillinfo = ParamMgr::getInstance()->getItemByID(itemid);

		auto textname = Text::create();
		textname->setAnchorPoint(Vec2(0.f, 0.5f));
		node->addChild(textname, 1);
		textname->setFontSize(24);
		textname->setTextColor(Color4B(0xff, 0xaf, 0x04, 0xff));
		textname->setString(skillinfo.name);
		textname->setPosition(infopos + Vec2(0.f, 60.f));

		auto textvalue = Text::create();
		textvalue->setFontSize(18);
		textvalue->setTextColor(Color4B(0xff, 0xf8, 0xbd, 0xff));
		node->addChild(textvalue, 1);
		textvalue->setString(skillinfo.des);

		auto length = skillinfo.des.length() / 2 / 18;
		textvalue->setTextAreaSize(Size(360.f, 20 * length));
		textvalue->setPosition(infopos + Vec2(180.f, 0.f));
	}
	

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [=](Touch*, Event*) {
		node->removeFromParent();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

//std::string g_giftItemids[] = { StoreAssetMgr::ITEMID_GOOD_TIMEGIFT, StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1, StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2, StoreAssetMgr::ITEMID_GOOD_DAYGIFT };
//Button* g_bntGift[4] = { 0 };

std::string g_giftItemids[] = { StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_0, StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_1, StoreAssetMgr::ITEMID_GOOD_DAYGIFT };
Button* g_bntGift[3] = { 0 };

bool GiftLayer::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/storeui.plist");

	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_root = CSLoader::createNode("csb/storelayernew.csb");
	this->addChild(_root);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	_root->setPositionY(-VisibleRect::top().y + 100);
	_root->runAction(actionshow);

	auto offy = VisibleRect::top().y - 960.f;

	static_cast<ui::ImageView*>(_root->getChildByName("mainbg"))->setContentSize(Size(640.f, 880.f + offy));
	
	auto titlenode = _root->getChildByName("titlebg");
	titlenode->setPositionY(titlenode->getPositionY() + offy);

	auto btncancle = static_cast<Button*>(_root->getChildByName("btn_cancle"));
	btncancle->setPositionY(btncancle->getPositionY() + offy);
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	auto list = static_cast<ListView*>(_root->getChildByName("ListView_1"));
	list->setPositionY(list->getPositionY() + offy);

	for (int i = 0; i < 3; i++)
	{
		auto keyStr = String::createWithFormat("btn_%d", i)->getCString();

		auto btnbuy = static_cast<Button*>(list->getChildByName(keyStr));
		btnbuy->setUserData((void*)(i));

		btnbuy->addTouchEventListener(CC_CALLBACK_2(GiftLayer::menuOnBuy, this));
		g_bntGift[i] = btnbuy;

		_giftNodes.pushBack(btnbuy);
	}
	/*for (int i = 0; i < 3; i++)
	{
		if (UserData::getInstance()->getItemBalance(g_giftItemids[i]) > 0)
		{
			g_bntGift[i]->removeFromParent();
			g_bntGift[i] = nullptr;
		}
	}*/
	updateBtns();

	return true;
}

void GiftLayer::menuOnBuy(Ref* ref, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int idx =(int)((Node*)(ref))->getUserData();
		auto layer = PurchaseLayer::create(g_giftItemids[idx], "", this, true);
		this->addChild(layer);
	}
}

void GiftLayer::updateBtns()
{
	/*for (int i = 0; i < 3; i++)
	{
		if (UserData::getInstance()->getItemBalance(g_giftItemids[i]) > 0)
		{
			if (g_bntGift[i])
			{
				g_bntGift[i]->removeFromParent();
				g_bntGift[i] = nullptr;
			}
		}
	}*/
	for (int i = 0; i < 3; i++)
	{
		if (_giftNodes.at(i)->getParent() != nullptr)
		{
			_giftNodes.at(i)->removeFromParent();
		}	
	}
#if 0
	int idx = getShoulddShowGiftIdx();
	if (idx != -1)
	{
		_root->getChildByName("ListView_1")->addChild(_giftNodes.at(idx));
	}
#else
	if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_0) == 0)
	{
		_root->getChildByName("ListView_1")->addChild(_giftNodes.at(0));
	}

	if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_1) == 0 && UserData::getInstance()->getIsBossPass(32) == 1)
	{
		_root->getChildByName("ListView_1")->addChild(_giftNodes.at(1));
	}
#endif
	

	_root->getChildByName("ListView_1")->addChild(_giftNodes.at(2));
}


int GiftLayer::getShoulddShowGiftIdx()
{
#if 0
	int idx = 0;
	int tempidx = -1;
	std::string itemids[3] = { StoreAssetMgr::ITEMID_GOOD_TIMEGIFT , StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1 , StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2 };

	if (UserData::getInstance()->getPlayerCurLv(0) < 13)
	{
		tempidx = 0;
	}
	else if (UserData::getInstance()->getPlayerCurLv(0) >= 13 && UserData::getInstance()->getPlayerCurLv(0) <= 20)
	{
		tempidx = 1;
	}
	else if (UserData::getInstance()->getPlayerCurLv(0) > 20)
	{
		tempidx = 2;
	}

	idx = tempidx;

	for (int i = tempidx; i < 3; i++)
	{
		if (UserData::getInstance()->getItemBalance(itemids[i]) == 0)
		{
			break;
		}
		idx++;
	}

	int trueidx = idx > 2 ? -1 : idx;

	return trueidx;
#else
	return -1;
#endif
}
