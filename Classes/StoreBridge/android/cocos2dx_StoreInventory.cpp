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

#include "../cocos2dx_StoreInventory.h"
#include "../StoreAssets.h"
#include "../../DataParam/UserData.h"
#include "../cocos2dx_EventHandlers.h"
#include "../../platBridge/cocos2dx_plat.h"
#include "GLCommon/Utils/ResMgr.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include "JniHelpers.h"
#endif



int cocos2dx_StoreInventory::buy(string itemId){
	int idx = StoreAssetMgr::getInstance()->getSingleIdx(itemId);
	if (idx != -1)
	{
		auto& vector = StoreAssetMgr::getInstance()->getItemVector();
		if (vector[idx].paytype == PAYTYPE_GOLD)
		{
			if (UserData::getInstance()->getGoldNum() >= vector[idx].payprice)
			{
				UserData::getInstance()->giveGold(-vector[idx].payprice);
				UserData::getInstance()->saveUserData();

				cocos2dx_EventHandlers::getInstance()->itemPurchased(itemId);

				return 0;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_gold")->getCString());
				cocos2dx_EventHandlers::getInstance()->itemPurchasedFailed(itemId);
				return  vector[idx].payprice - UserData::getInstance()->getGoldNum();
			}
		}
		else if (vector[idx].paytype == PAYTYPE_CRYSTAL)
		{
			if (UserData::getInstance()->getCrystalNum() >= vector[idx].payprice)
			{
				UserData::getInstance()->giveCrystal(-vector[idx].payprice);
				UserData::getInstance()->saveUserData();

				cocos2dx_EventHandlers::getInstance()->itemPurchased(itemId);

				return 0;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_caystal")->getCString());
				cocos2dx_EventHandlers::getInstance()->itemPurchasedFailed(itemId);
				return  vector[idx].payprice - UserData::getInstance()->getCrystalNum();
			}
		}
		else
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniHelpers::jniCommonVoidCall(
				"buy",
				"com/soomla/cocos2dx/store/StoreInventoryBridge",
				itemId.c_str(),
				vector[idx].payprice
				);
#else
			CCLOG("%s:%d", itemId.c_str(),
				(int)vector[idx].payprice);
			cocos2dx_EventHandlers::getInstance()->marketPurchase(itemId);
#endif
		}

		return 0;
	}

	idx = StoreAssetMgr::getInstance()->getPackIdx(itemId);
	if (idx != -1)
	{
		auto& vector = StoreAssetMgr::getInstance()->getPackVector();
		if (vector[idx].payprice == PAYTYPE_GOLD)
		{
			if (UserData::getInstance()->getGoldNum() >= vector[idx].payprice)
			{
				UserData::getInstance()->giveGold(-vector[idx].payprice);
				UserData::getInstance()->saveUserData();

				cocos2dx_EventHandlers::getInstance()->itemPurchased(itemId);

				return 0;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_gold")->getCString());
				cocos2dx_EventHandlers::getInstance()->itemPurchasedFailed(itemId);
				return  vector[idx].payprice - UserData::getInstance()->getGoldNum();
			}
		}
		else if (vector[idx].paytype == PAYTYPE_CRYSTAL)
		{
			if (UserData::getInstance()->getCrystalNum() >= vector[idx].payprice)
			{
				UserData::getInstance()->giveCrystal(-vector[idx].payprice);
				UserData::getInstance()->saveUserData();

				cocos2dx_EventHandlers::getInstance()->itemPurchased(itemId);

				return 0;
			}
			else
			{
				cocos2dx_plat::showToast(ResMgr::getInstance()->getString("no_caystal")->getCString());
				cocos2dx_EventHandlers::getInstance()->itemPurchasedFailed(itemId);
				return  vector[idx].payprice - UserData::getInstance()->getCrystalNum();		
			}
		}
		else
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniHelpers::jniCommonVoidCall(
				"buy",
				"com/soomla/cocos2dx/store/StoreInventoryBridge",
				itemId.c_str(),
				vector[idx].payprice
				);
#else
			CCLOG("%s:%d", itemId.c_str(),
				(int)vector[idx].payprice);
			cocos2dx_EventHandlers::getInstance()->marketPurchase(itemId);
#endif
		}
		return 0;
	}

	cocos2dx_plat::showToast("no find itemid");
	return 1;
}