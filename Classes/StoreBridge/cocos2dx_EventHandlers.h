/*
 * Copyright (C) 2012 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __cocos2dx_store__cocos2dx_EventHandler__
#define __cocos2dx_store__cocos2dx_EventHandler__

#include <string>
#include <set>

using namespace std;

class IEventHandler {
public:
    virtual void billingSupported() = 0;
    virtual void billingNotSupported() = 0;
	virtual void closingStore() = 0;
	virtual void currencyBalanceChanged(string &itemId, int balance, int amountAdded) = 0;
	virtual void goodBalanceChanged(string &itemId, int balance, int amountAdded) = 0;
    virtual void goodEquipped(string& itemId) = 0;
    virtual void goodUnequipped(string& itemId) = 0;
    virtual void goodUpgrade(string& itemId, string& upgradeItemId) = 0;
	virtual void itemPurchased(string& itemId) = 0;
	virtual void itemPurchaseStarted() = 0;
    virtual void openingStore() = 0;
	virtual void marketPurchaseCancelled(string& itemId) = 0;
    virtual void marketPurchase(string& itemId) = 0;
    virtual void marketPurchaseStarted(string& itemId) = 0;
    virtual void marketRefund(string& itemId) = 0;
    virtual void restoreTransactions(bool success) = 0;
	virtual void restoreTransactionsStarted() = 0;
    virtual void unexpectedErrorInStore() = 0;
	virtual void itemPurchasedFailed(string& itemId) = 0;
};

class cocos2dx_EventHandlers {
private:
	static cocos2dx_EventHandlers* instance;
	set <IEventHandler*> handlers;
	cocos2dx_EventHandlers() { };
public:
	static cocos2dx_EventHandlers* getInstance();
	void addHandler(IEventHandler* handler);
	void removeHandler(IEventHandler* handler);
	    
    void billingSupported();
    void billingNotSupported();
	void closingStore();
	void currencyBalanceChanged(string &itemId, int balance, int amountAdded);
	void goodBalanceChanged(string &itemId, int balance, int amountAdded);
    void goodEquipped(string& itemId);
    void goodUnequipped(string& itemId);
    void goodUpgrade(string& itemId, string& upgradeItemId);
	void itemPurchased(string& itemId);
	void itemPurchasedFailed(string& itemId);

	void itemPurchaseStarted();
    void openingStore();
	void marketPurchaseCancelled(string& itemId);
    void marketPurchase(string& itemId);
    void marketPurchaseStarted(string& itemId);
    void marketRefund(string& itemId);
    void restoreTransactions(bool success);
	void restoreTransactionsStarted();
    void unexpectedErrorInStore();
};

#endif /* defined(__cocos2dx_store__cocos2dx_EventHandler__) */
