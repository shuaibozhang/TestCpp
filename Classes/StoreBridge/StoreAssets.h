#ifndef _STOREASSETS_H_
#define _STOREASSETS_H_

#include "cocos2d.h"

enum PayType_E
{
	PAYTYPE_GOLD = 0,
	PAYTYPE_RMB,
	PAYTYPE_CRYSTAL
};

typedef struct _PackItemInfo
{
	std::string itemid;
	int num;
}PackItem_T;

typedef struct _StoreItem
{
	std::string itmid;
	int paytype;
	int payprice;
	std::string name;
	std::string des;
}StoreItem_T;

typedef struct _StorePack
{
	std::string itmid;
	int paytype;
	int payprice;
	std::vector<PackItem_T> _items;
	std::string name;
	std::string des;
}StorePack_T;

class StoreAssetMgr
{
public:
	static StoreAssetMgr* getInstance();
	static StoreAssetMgr* _inStance;
	void initStoreAsset();

	void loadStoreAssets();

	void addSingleItem( StoreItem_T iteminfo);
	void addPackItem( StorePack_T packinfo);

	int getSingleIdx(const std::string& itemid);
	int getPackIdx(const std::string& itemid);

	int getPackPrice(const std::string& itemid);
	int getSinglePrice(const std::string& itemid);

	std::string getPackDes(const std::string& itemid);
	int getPayType(const std::string& itemid);

	std::vector<StorePack_T>& getPackVector() { return _vectorPackInfo; }
	std::vector<StoreItem_T>& getItemVector() { return _vectorItemInfo; }

	static bool checkCanPay(int paytype, int price, bool poptips = false);
private:
	StoreAssetMgr();
	std::vector<StorePack_T> _vectorPackInfo;
	std::vector<StoreItem_T> _vectorItemInfo;

public:
	static const char* ITEMID_GOOD_GOLDPACK_0;
	static const char* ITEMID_GOOD_GOLDPACK_1;
	static const char* ITEMID_GOOD_GOLDPACK_2;
	static const char* ITEMID_GOOD_GOLDPACK_3;

	static const char* ITEMID_GOOD_WENPONPACK_0;
	static const char* ITEMID_GOOD_WENPONPACK_1;
	static const char* ITEMID_GOOD_WENPONPACK_2;
	static const char* ITEMID_GOOD_WENPONPACK_3;
	static const char* ITEMID_GOOD_WENPONPACK_4;

	static const char* ITEMID_GOOD_ITEMPACK_0;
	static const char* ITEMID_GOOD_ITEMPACK_1;
	static const char* ITEMID_GOOD_ITEMPACK_2;

	static const char* ITEMID_GOOD_SKILLPACK_0;
	static const char* ITEMID_GOOD_SKILLPACK_1;
	static const char* ITEMID_GOOD_SKILLPACK_2;
	static const char* ITEMID_GOOD_SKILLPACK_3;
	
	static const char* ITEMID_GOOD_PLAYERSUITPACK_0;
	static const char* ITEMID_GOOD_PLAYERSUITPACK_1;
	static const char* ITEMID_GOOD_PLAYERSUITPACK_2;
	static const char* ITEMID_GOOD_PLAYERSUITPACK_3;

	//new store
	static const char* ITEMID_GOOD_NEWGOLDPACK_0;
	static const char* ITEMID_GOOD_NEWGOLDPACK_1;
	static const char* ITEMID_GOOD_NEWGOLDPACK_2;
	static const char* ITEMID_GOOD_NEWGOLDPACK_3;

	static const char* ITEMID_GOOD_NEWCRYSTALPACK_0;
	static const char* ITEMID_GOOD_NEWCRYSTALPACK_1;
	static const char* ITEMID_GOOD_NEWCRYSTALPACK_2;
	static const char* ITEMID_GOOD_NEWCRYSTALPACK_3;

	static const char* ITEMID_GOOD_STOREBOX_0;
	static const char* ITEMID_GOOD_STOREBOX_1;
	static const char* ITEMID_GOOD_STOREBOX_2;

	static const char* ITEMID_GOOD_REFRESH_STORE;

	static const char* ITEMID_GOOD_FIRSTGIFT;

	static const char* ITEMID_GOOD_REGETTILI;
	static const char* ITEMID_GOOD_BUYTILI;

	static const char* ITEMID_GOOD_TIMEGIFT;
	static const char* ITEMID_GOOD_TIMEGIFT_1;
	static const char* ITEMID_GOOD_TIMEGIFT_2;
	static const char* ITEMID_GOOD_DAYGIFT;

	static const char* ITEMID_GOOD_REBORN_1;
	static const char* ITEMID_GOOD_REBORN_2;
	static const char* ITEMID_GOOD_REBORN_4;

	static const char* ITEMID_GOOD_UNLIMTWENPONGIFT_0;
	static const char* ITEMID_GOOD_UNLIMTWENPONGIFT_1;

	static const char* ITEMID_GOOD_PLAYER_4;
};
#endif