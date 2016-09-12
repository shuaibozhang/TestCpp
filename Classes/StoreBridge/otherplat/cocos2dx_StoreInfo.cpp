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

#include "../cocos2dx_StoreInfo.h"
#include "win32_StoreDatabase.h"


string cocos2dx_StoreInfo::getGoodFirstUpgrade(string goodItemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	return "";
}

string cocos2dx_StoreInfo::getGoodLastUpgrade(string goodItemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	return "";
}

string cocos2dx_StoreInfo::getItemProductId(string itemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	return "";
}

string cocos2dx_StoreInfo::getItemName(string itemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	return "";
}

string cocos2dx_StoreInfo::getItemDescription(string itemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	return "";
}

double cocos2dx_StoreInfo::getItemPrice(string itemId) throw (cocos2dx_VirtualItemNotFoundException&) {
	Win32GoodItem_T *pItemInfo = Win32StoreDatabase::getInstance()->getGoodItemInfo(itemId);
	return pItemInfo->price;
}
