#include "win32_StoreDatabase.h"

USING_NS_CC;
using namespace std;

const string WIN32_DATA_KEY_TAG = "_win32";
//const int WIN32_GOOD_ITEM_COUNT = 47;

Win32GoodItem_T g_win32GoodItemInfo[] =
{
	/*itemId	costItemId	giveItemId	itemType	price	 giveCount*/
	{"itemid_good_bulletlv_0", "itemid_currency_goldcoin", "itemid_good_bulletlv_0", 1, 0, 1},
	{"itemid_good_bulletlv_1", "itemid_currency_goldcoin", "itemid_good_bulletlv_1", 1, 0, 1},
	{"itemid_good_bulletlv_2", "itemid_currency_goldcoin", "itemid_good_bulletlv_2", 1, 0, 1},
	{"itemid_good_bulletlv_3", "itemid_currency_goldcoin", "itemid_good_bulletlv_3", 1, 0, 1},
	{"itemid_good_bulletmax_0", "itemid_currency_goldcoin", "itemid_good_bulletmax_0", 1, 0, 1},
	{"itemid_good_bulletmax_1", "itemid_currency_goldcoin", "itemid_good_bulletmax_1", 1, 0, 1},
	{"itemid_good_bulletmax_2", "itemid_currency_goldcoin", "itemid_good_bulletmax_2", 1, 0, 1},
	{"itemid_good_bulletmax_3", "itemid_currency_goldcoin", "itemid_good_bulletmax_3", 1, 0, 1},

	{"itemid_good_pet_1", "itemid_currency_goldcoin", "itemid_good_pet_1", 1, 0, 1},
	{"itemid_good_pet_2", "itemid_currency_goldcoin", "itemid_good_pet_2", 1, 0, 1},
	{"itemid_good_pet_3", "itemid_currency_goldcoin", "itemid_good_pet_3", 1, 0, 1},
	{"itemid_good_pet_4", "itemid_currency_goldcoin", "itemid_good_pet_4", 1, 0, 1},
	{"itemid_good_pet_5", "itemid_currency_goldcoin", "itemid_good_pet_5", 1, 0, 1},
	{"itemid_good_pet_0", "itemid_currency_goldcoin", "itemid_good_pet_0", 1, 0, 1},

	{"itemid_good_plane_1", "itemid_currency_goldcoin", "itemid_good_plane_1", 1, 0, 1},
	{"itemid_good_reborn", "itemid_currency_goldcoin", "itemid_good_revive", 1, 0, 1},
	{"itemid_good_tilirmb", "itemid_currency_goldcoin", "itemid_good_tilirmb", 1, 0, 1},
	{"itemid_good_goldpack", "itemid_currency_goldcoin", "itemid_good_goldpack", 1, 0, 1},

	{"itemid_good_boom", "itemid_currency_goldcoin", "itemid_good_boom", 1, 0, 1},
	{"itemid_good_protect", "itemid_currency_goldcoin", "itemid_good_protect", 1, 0, 1},
	{"itemid_good_petmax", "itemid_currency_goldcoin", "itemid_good_petmax", 1, 0, 1},
	{"itemid_good_drawten", "itemid_currency_goldcoin", "itemid_good_drawten", 1, 0, 1},
	{"itemid_good_firstplaygift", "itemid_currency_goldcoin", "itemid_good_firstplaygift", 1, 0, 1},

	{"itemid_good_state_ss_0", "itemid_currency_goldcoin", "itemid_good_state_ss_0", 1, 0, 1},
	{"itemid_good_state_ss_1", "itemid_currency_goldcoin", "itemid_good_state_ss_1", 1, 0, 1},
	{"itemid_good_state_ss_2", "itemid_currency_goldcoin", "itemid_good_state_ss_2", 1, 0, 1},
	{"itemid_good_state_ss_3", "itemid_currency_goldcoin", "itemid_good_state_ss_3", 1, 0, 1},
	{"itemid_good_state_ss_4", "itemid_currency_goldcoin", "itemid_good_state_ss_4", 1, 0, 1},
	{"itemid_good_state_ss_5", "itemid_currency_goldcoin", "itemid_good_state_ss_5", 1, 0, 1},
	
	{"itemid_good_finalevolve", "itemid_currency_goldcoin", "itemid_good_finalevolve", 1, 0, 1},
	{"itemid_good_pvpskill", "itemid_currency_goldcoin", "itemid_good_pvpskill", 1, 0, 1},
	{"itemid_good_shuijing_lv", "itemid_currency_goldcoin", "itemid_good_shuijing_lv", 1, 0, 1},
	{"itemid_good_niaocao_lv", "itemid_currency_goldcoin", "itemid_good_niaocao_lv", 1, 0, 1},
	{"itemid_good_drawone", "itemid_currency_goldcoin", "itemid_good_drawone", 1, 0, 1},
	{"itemid_good_goldevolve", "itemid_currency_goldcoin", "itemid_good_goldevolve", 1, 0, 1},
	{"itemid_good_petsupport", "itemid_currency_goldcoin", "itemid_good_petsupport", 1, 0, 1},
	{"itemid_good_gamevip", "itemid_currency_goldcoin", "itemid_good_gamevip", 1, 0, 1},
};


const int WIN32_GOOD_ITEM_COUNT = sizeof(g_win32GoodItemInfo) / sizeof(g_win32GoodItemInfo[0]);

Win32StoreDatabase* Win32StoreDatabase::s_pInstance = NULL;


Win32StoreDatabase::Win32StoreDatabase()
{
}


Win32StoreDatabase::~Win32StoreDatabase()
{
}


Win32StoreDatabase *Win32StoreDatabase::getInstance(void)
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new Win32StoreDatabase();
	}

	return s_pInstance;
}


int Win32StoreDatabase::getItemBalance(string itemId)
{
	UserDefault *pCCUserDefault = UserDefault::getInstance();

	itemId.append(WIN32_DATA_KEY_TAG);
	int itemCount = pCCUserDefault->getIntegerForKey(itemId.c_str(), 0);

	return itemCount;
}


void Win32StoreDatabase::giveItem(string itemId, int itemCount)
{
	UserDefault *pCCUserDefault = UserDefault::getInstance();
	int curCount = Win32StoreDatabase::getItemBalance(itemId);

	itemId.append(WIN32_DATA_KEY_TAG);
	pCCUserDefault->setIntegerForKey(itemId.c_str(), curCount + itemCount);
	pCCUserDefault->flush();
}


void Win32StoreDatabase::takeItem(string itemId, int itemCount)
{
	UserDefault *pCCUserDefault = UserDefault::getInstance();
	int curCount = Win32StoreDatabase::getItemBalance(itemId);

	itemId.append(WIN32_DATA_KEY_TAG);
	pCCUserDefault->setIntegerForKey(itemId.c_str(), curCount - itemCount);
	pCCUserDefault->flush();
}


Win32GoodItem_T *Win32StoreDatabase::getGoodItemInfo(string itemId)
{
	Win32GoodItem_T *pRet = NULL;

	for (int i=0; i<WIN32_GOOD_ITEM_COUNT; i++)
	{
		if (0 == g_win32GoodItemInfo[i].itemId.compare(itemId.c_str()))
		{
			pRet = g_win32GoodItemInfo + i;
			break;
		}		
	}

	return pRet;
}