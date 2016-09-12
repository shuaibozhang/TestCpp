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

#include "cocos2dx_EventHandlers.h"
#include "../Store/StoreEventHandler.h"

cocos2dx_EventHandlers* cocos2dx_EventHandlers::instance = NULL;

cocos2dx_EventHandlers* cocos2dx_EventHandlers::getInstance() {
	if ( instance == NULL ) {
		instance = new cocos2dx_EventHandlers();

		auto hander = new StoreEventHandler();
		instance->addHandler(hander);
	}

	return instance;
}
	
void cocos2dx_EventHandlers::addHandler(IEventHandler* handler) {
	handlers.insert(handler);
}
	
void cocos2dx_EventHandlers::removeHandler(IEventHandler* handler) {
	handlers.erase(handler);
}


void cocos2dx_EventHandlers::billingSupported() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->billingSupported();
}
void cocos2dx_EventHandlers::billingNotSupported() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->billingNotSupported();
}
void cocos2dx_EventHandlers::closingStore() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->closingStore();
}
void cocos2dx_EventHandlers::currencyBalanceChanged(string &itemId, int balance, int amountAdded) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->currencyBalanceChanged(itemId, balance, amountAdded);
}
void cocos2dx_EventHandlers::goodBalanceChanged(string &itemId, int balance, int amountAdded) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->goodBalanceChanged(itemId, balance, amountAdded);
}
void cocos2dx_EventHandlers::goodEquipped(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->goodEquipped(itemId);
}
void cocos2dx_EventHandlers::goodUnequipped(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->goodUnequipped(itemId);
}
void cocos2dx_EventHandlers::goodUpgrade(string& itemId, string& upgradeItemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->goodUpgrade(itemId, upgradeItemId);
}
void cocos2dx_EventHandlers::itemPurchased(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->itemPurchased(itemId);
}
void cocos2dx_EventHandlers::itemPurchasedFailed(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for (itr = handlers.begin(); itr != handlers.end(); itr++)
		(*itr)->itemPurchasedFailed(itemId);
}
void cocos2dx_EventHandlers::itemPurchaseStarted() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->itemPurchaseStarted();
}
void cocos2dx_EventHandlers::openingStore() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->openingStore();
}
void cocos2dx_EventHandlers::marketPurchaseCancelled(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->marketPurchaseCancelled(itemId);
}
void cocos2dx_EventHandlers::marketPurchase(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->marketPurchase(itemId);
}
void cocos2dx_EventHandlers::marketPurchaseStarted(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->marketPurchaseStarted(itemId);
}
void cocos2dx_EventHandlers::marketRefund(string& itemId) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->marketRefund(itemId);
}
void cocos2dx_EventHandlers::restoreTransactions(bool success) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->restoreTransactions(success);
}
void cocos2dx_EventHandlers::restoreTransactionsStarted() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->restoreTransactionsStarted();
}
void cocos2dx_EventHandlers::unexpectedErrorInStore() {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->unexpectedErrorInStore();
}

