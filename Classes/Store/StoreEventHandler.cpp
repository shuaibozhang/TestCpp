#include "StoreEventHandler.h"
#include "../StoreBridge/Includes.h"
#include "../platBridge/cocos2dx_analyze.h"
#include "../StoreBridge/cocos2dx_StoreInventory.h"
#include "cocos2d.h"
#include "../platBridge/cocos2dx_plat.h"
#include "../StoreBridge/StoreAssets.h"
#include "PurchaseLayer.h"
#include "../DataParam/UserData.h"
#include "../DataParam/DayActiveMgr.h"

USING_NS_CC;



StoreEventHandler *StoreEventHandler::s_StoreEventHandler = NULL;

StoreEventHandler::StoreEventHandler()
{
    s_StoreEventHandler = this;
}

StoreEventHandler* StoreEventHandler::getCurStoreEventHandler(void)
{	
    return s_StoreEventHandler;
}


StoreEventHandler::~StoreEventHandler()
{
    if (s_StoreEventHandler == this)
    {
        s_StoreEventHandler = NULL;
    }
}


void StoreEventHandler::billingSupported()
{
}

void StoreEventHandler::billingNotSupported()
{
}

void StoreEventHandler::closingStore()
{
}

void StoreEventHandler::currencyBalanceChanged(string &itemId, int balance, int amountAdded)
{
}

void StoreEventHandler::goodBalanceChanged(string &itemId, int balance, int amountAdded)
{

}

void StoreEventHandler::goodEquipped(string& itemId)
{
}

void StoreEventHandler::goodUnequipped(string& itemId)
{
}

void StoreEventHandler::goodUpgrade(string& itemId, string& upgradeItemId)
{
}


void StoreEventHandler::itemPurchased(string& itemId)
{
	cocos2dx_analyze::addCustomEvent("purchase_ok_by_gold", itemId);

	int idx = StoreAssetMgr::getInstance()->getSingleIdx(itemId);
	if (idx != -1)
	{
		auto& vector = StoreAssetMgr::getInstance()->getItemVector();
		UserData::getInstance()->giveItem(vector[idx].itmid, 1);

		if (vector[idx].paytype == PAYTYPE_CRYSTAL)
		{
			DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYBUYZUANSHI);
		}
	}
	else
	{
		idx = StoreAssetMgr::getInstance()->getPackIdx(itemId);
		if (idx != -1)
		{
			auto& vector = StoreAssetMgr::getInstance()->getPackVector();
			auto& vector2 = vector[idx]._items;
			for (auto& temp : vector2)
			{
				UserData::getInstance()->giveItem(temp.itemid, temp.num, false);
			}
			UserData::getInstance()->giveItem(itemId, 1, false);
			UserData::getInstance()->freeDB();

			if (vector[idx].paytype == PAYTYPE_CRYSTAL)
			{
				DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYBUYZUANSHI);
			}
		}
	}
	
	string subItemid(itemId, 12);
	//float itemPrice = cocos2dx_StoreInfo::getItemPrice(itemId);
	//cocos2dx_analyze::use(subItemid.c_str(), 1, itemPrice);

	if (nullptr != PurchaseLayer::getCurPurchaseLayer())
	{
		PurchaseLayer::getCurPurchaseLayer()->onPurchaseOk(itemId);
	}
}

void StoreEventHandler::itemPurchaseStarted()
{
}

void StoreEventHandler::openingStore()
{
}


void StoreEventHandler::marketPurchaseCancelled(string& itemId)
{
	cocos2dx_analyze::addCustomEvent("purchase_cancel", itemId);
	if (nullptr != PurchaseLayer::getCurPurchaseLayer())
	{
		PurchaseLayer::getCurPurchaseLayer()->menuCancelCallback(nullptr);
	}
}


void StoreEventHandler::marketPurchase(string& itemId)
{
	cocos2dx_analyze::addCustomEvent("purchase_ok_by_rmb", itemId);
	int curPayRmbNum = 0;
	string subItemId(itemId, 10);
	if ("item_good_firstgift" == itemId)
	{
		cocos2dx_analyze::pay(6, subItemId.c_str(), 60, 100, 1);
		curPayRmbNum = 6;
	}
	else if ("item_good_newcrystalpack_0" == itemId)
	{
		cocos2dx_analyze::pay(6, subItemId.c_str(), 60, 100, 1);
		curPayRmbNum = 6;
	}
	else if ("item_good_newcrystalpack_1" == itemId)
	{
		cocos2dx_analyze::pay(30, subItemId.c_str(), 420, 100, 1);
		curPayRmbNum = 30;
	}
	else if ("item_good_newcrystalpack_2" == itemId)
	{
		cocos2dx_analyze::pay(68, subItemId.c_str(), 1100, 100, 1);
		curPayRmbNum = 68;
	}
	else if ("item_good_newcrystalpack_3" == itemId)
	{
		cocos2dx_analyze::pay(128, subItemId.c_str(), 2400, 100, 1);
		curPayRmbNum = 128;
	}
	else if ("item_good_reborn1" == itemId)
	{
		//		cocos2dx_analyze::pay(30, 420, 1);
		cocos2dx_analyze::pay(1, subItemId.c_str(), 2400, 100, 1);
		curPayRmbNum = 1;
	}
	else if ("item_good_reborn2" == itemId)
	{
		//		cocos2dx_analyze::pay(30, 420, 1);
		cocos2dx_analyze::pay(2, subItemId.c_str(), 2400, 100, 1);
		curPayRmbNum = 2;
	}
	else if ("item_good_reborn4" == itemId)
	{
		//		cocos2dx_analyze::pay(30, 420, 1);
		cocos2dx_analyze::pay(4, subItemId.c_str(), 2400, 100, 1);
		curPayRmbNum = 4;
	}

	UserData::getInstance()->addPayRmb(curPayRmbNum);

	int idx = StoreAssetMgr::getInstance()->getSingleIdx(itemId);
	if (idx != -1)
	{
		auto& vector = StoreAssetMgr::getInstance()->getItemVector();
		UserData::getInstance()->giveItem(vector[idx].itmid, 1);
	}
	else
	{
		idx = StoreAssetMgr::getInstance()->getPackIdx(itemId);
		if (idx != -1)
		{
			auto& vector = StoreAssetMgr::getInstance()->getPackVector();
			auto& vector2 = vector[idx]._items;
			for (auto& temp : vector2)
			{
				UserData::getInstance()->giveItem(temp.itemid, temp.num, false);
			}
			UserData::getInstance()->giveItem(itemId,1, false);
			UserData::getInstance()->freeDB();
		}
	}


	//float payPrice = cocos2dx_StoreInfo::getItemPrice(itemId) / 100.f;

	//string tempItemid(itemId, 12);
	//cocos2dx_analyze::pay(payPrice, tempItemid.c_str(), 1, payPrice, 9);

	if (nullptr != PurchaseLayer::getCurPurchaseLayer())
	{
		PurchaseLayer::getCurPurchaseLayer()->onPurchaseOk(itemId);
	}
}


void StoreEventHandler::marketPurchaseStarted(string& itemId)
{
    cocos2dx_analyze::addCustomEvent("purchase_start", itemId);
}

void StoreEventHandler::marketRefund(string& itemId)
{
}

void StoreEventHandler::restoreTransactions(bool success)
{
}

void StoreEventHandler::restoreTransactionsStarted()
{
}

void StoreEventHandler::unexpectedErrorInStore()
{
}

void StoreEventHandler::itemPurchasedFailed(string & itemId)
{
	PurchaseLayer::getCurPurchaseLayer()->onItemBuyFailed(itemId);
}

void StoreEventHandler::buyItem(string itemid)
{
	cocos2dx_analyze::addCustomEvent("buy_good", itemid);
	cocos2dx_analyze::buy(itemid.c_str(), 1, 0);
	cocos2dx_StoreInventory::buy(itemid);
}