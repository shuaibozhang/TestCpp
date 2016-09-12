#ifndef __STORE_EVENTHANDLER_H__
#define __STORE_EVENTHANDLER_H__

#include "cocos2d.h"

#include <iostream>

#include <string>
#include "../StoreBridge/cocos2dx_EventHandlers.h"

using namespace std;

class StoreEventHandler : public IEventHandler {
public:
    StoreEventHandler();
    ~StoreEventHandler();
    
    static StoreEventHandler* getCurStoreEventHandler(void);
    
    void billingSupported();
    void billingNotSupported();
	void closingStore();
	void currencyBalanceChanged(string &itemId, int balance, int amountAdded);
	void goodBalanceChanged(string &itemId, int balance, int amountAdded);
    void goodEquipped(string& itemId);
    void goodUnequipped(string& itemId);
    void goodUpgrade(string& itemId, string& upgradeItemId);
	void itemPurchased(string& itemId);
	void itemPurchaseStarted();
    void openingStore();
	void marketPurchaseCancelled(string& itemId);
    void marketPurchase(string& itemId);
    void marketPurchaseStarted(string& itemId);
    void marketRefund(string& itemId);
    void restoreTransactions(bool success);
	void restoreTransactionsStarted();
    void unexpectedErrorInStore();
	void itemPurchasedFailed(string& itemId);
    void buyItem(string itemid);
    
    static StoreEventHandler *s_StoreEventHandler;
};

#endif /* defined(__STORE_EVENTHANDLER_H__) */
