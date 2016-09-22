#include "StoreAssets.h"
#include "../DataParam/ParamData.h"
#include "ParamMgr.h"
#include "cocos2d.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "UserData.h"
#include "GameUtils.h"

USING_NS_CC;

StoreAssetMgr* StoreAssetMgr::_inStance = nullptr;

const char* StoreAssetMgr::ITEMID_GOOD_GOLDPACK_0 = "itemid_good_goldpack_0";
const char* StoreAssetMgr::ITEMID_GOOD_GOLDPACK_1 = "itemid_good_goldpack_1";
const char* StoreAssetMgr::ITEMID_GOOD_GOLDPACK_2 = "itemid_good_goldpack_2";
const char* StoreAssetMgr::ITEMID_GOOD_GOLDPACK_3 = "itemid_good_goldpack_3";

const char* StoreAssetMgr::ITEMID_GOOD_WENPONPACK_0 = "itemid_good_wenponpack_0";
const char* StoreAssetMgr::ITEMID_GOOD_WENPONPACK_1 = "itemid_good_wenponpack_1";
const char* StoreAssetMgr::ITEMID_GOOD_WENPONPACK_2 = "itemid_good_wenponpack_2";
const char* StoreAssetMgr::ITEMID_GOOD_WENPONPACK_3 = "itemid_good_wenponpack_3";
const char* StoreAssetMgr::ITEMID_GOOD_WENPONPACK_4 = "itemid_good_wenponpack_4";

const char* StoreAssetMgr::ITEMID_GOOD_ITEMPACK_0 = "itemid_good_itempack_0";
const char* StoreAssetMgr::ITEMID_GOOD_ITEMPACK_1 = "itemid_good_itempack_1";
const char* StoreAssetMgr::ITEMID_GOOD_ITEMPACK_2 = "itemid_good_itempack_2";

const char* StoreAssetMgr::ITEMID_GOOD_SKILLPACK_0 = "itemid_good_skillpack_0";
const char* StoreAssetMgr::ITEMID_GOOD_SKILLPACK_1 = "itemid_good_skillpack_1";
const char* StoreAssetMgr::ITEMID_GOOD_SKILLPACK_2 = "itemid_good_skillpack_2";
const char* StoreAssetMgr::ITEMID_GOOD_SKILLPACK_3 = "itemid_good_skillpack_3";

const char* StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_0 = "itemid_good_playersuit_0";
const char* StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_1 = "itemid_good_playersuit_1";
const char* StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_2 = "itemid_good_playersuit_2";
const char* StoreAssetMgr::ITEMID_GOOD_PLAYERSUITPACK_3 = "itemid_good_playersuit_3";

//new store
const char* StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_0 = "item_good_newgoldpack_0";
const char* StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_1 = "item_good_newgoldpack_1";
const char* StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_2 = "item_good_newgoldpack_2";
const char* StoreAssetMgr::ITEMID_GOOD_NEWGOLDPACK_3 = "item_good_newgoldpack_3";

const char* StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_0 = "item_good_newcrystalpack_0";
const char* StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_1 = "item_good_newcrystalpack_1";
const char* StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_2 = "item_good_newcrystalpack_2";
const char* StoreAssetMgr::ITEMID_GOOD_NEWCRYSTALPACK_3 = "item_good_newcrystalpack_3";

const char* StoreAssetMgr::ITEMID_GOOD_STOREBOX_0 = "item_good_storebox_0";
const char* StoreAssetMgr::ITEMID_GOOD_STOREBOX_1 = "item_good_storebox_1";
const char* StoreAssetMgr::ITEMID_GOOD_STOREBOX_2 = "item_good_storebox_2";

const char* StoreAssetMgr::ITEMID_GOOD_REFRESH_STORE = "item_good_refresh_store";
const char* StoreAssetMgr::ITEMID_GOOD_FIRSTGIFT = "item_good_firstgift";

const char* StoreAssetMgr::ITEMID_GOOD_REGETTILI = "item_good_regettili";
const char* StoreAssetMgr::ITEMID_GOOD_BUYTILI = "item_good_buytili";

const char* StoreAssetMgr::ITEMID_GOOD_TIMEGIFT = "item_good_timegift";
const char* StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_1 = "item_good_timegift1";
const char* StoreAssetMgr::ITEMID_GOOD_TIMEGIFT_2 = "item_good_timegift2";
const char* StoreAssetMgr::ITEMID_GOOD_DAYGIFT = "item_good_daygift";

const char* StoreAssetMgr::ITEMID_GOOD_REBORN_1 = "item_good_reborn1";
const char* StoreAssetMgr::ITEMID_GOOD_REBORN_2 = "item_good_reborn2";
const char* StoreAssetMgr::ITEMID_GOOD_REBORN_4 = "item_good_reborn4";

const char* StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_0 = "item_good_wpgift0";
const char* StoreAssetMgr::ITEMID_GOOD_UNLIMTWENPONGIFT_1 = "item_good_wpgift1";

const char* StoreAssetMgr::ITEMID_GOOD_PLAYER_4 = "item_good_player_4";

StoreAssetMgr::StoreAssetMgr()
{

}

StoreAssetMgr* StoreAssetMgr::getInstance()
{
	if (_inStance == nullptr)
	{
		_inStance = new StoreAssetMgr();
		_inStance->initStoreAsset();
		_inStance->loadStoreAssets();
	}

	return _inStance;
}

void StoreAssetMgr::initStoreAsset()
{
	auto& vector = ParamMgr::getInstance()->getSkillVector();
	for (auto& tempskill : vector)
	{
		StoreItem_T temp = {String::createWithFormat("itemid_good_skill_%d", tempskill.id)->getCString() , tempskill.paytype, tempskill.price};
		addSingleItem(temp);
	}
	
	auto& vector2 = ParamMgr::getInstance()->getItemVector();
	for (auto& tempskill : vector2)
	{
		StoreItem_T temp = { String::createWithFormat("itemid_good_item_%d", tempskill.id)->getCString() , tempskill.paytype, tempskill.price };
		addSingleItem(temp);
	}

	auto& vector3 = ParamMgr::getInstance()->getWeaponVector();
	for (auto& tempskill : vector3)
	{
		StoreItem_T temp = { String::createWithFormat("itemid_good_weapon_%d", tempskill.id)->getCString() , tempskill.paytype, tempskill.price };
		addSingleItem(temp);
	}
}

void StoreAssetMgr::loadStoreAssets()
{
	rapidjson::Document _jsonBezierDoc;
	
	auto contentStr = FileUtils::getInstance()->getDataFromFile("storeassets/store.b");

	ToolsUtil::unbtea(contentStr);

	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());
	_jsonBezierDoc.Parse<0>((char*)(load_str.c_str()));

	//std::string contentStr = FileUtils::getInstance()->getStringFromFile("storeassets/store.json");
	//_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	int count = _jsonBezierDoc.Capacity();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[i];

		auto itemname = item["itemid"].GetString();
		auto price = item["price"].GetInt();
		auto type = item["paytype"].GetInt();
		auto name = item["name"].GetString();
		auto des = item["des"].GetString();

		if (item.HasMember("get"))
		{
			auto& get = item["get"];

			StorePack_T pack;
			pack.itmid = itemname;
			pack.payprice = price;
			pack.paytype = type;
			pack.des = des;
			pack.name = name;
			std::vector<PackItem_T>& suititems = pack._items;

			int count2 = get.Capacity();
			for (int j = 0; j < count2; j++)
			{
				auto& item2 = get[j];
				auto getitemid = item2["itemid"].GetString();
				auto getnum = item2["getnum"].GetInt();

				PackItem_T pick;
				pick.itemid = getitemid;
				pick.num = getnum;

				suititems.push_back(pick);
			}

			addPackItem(pack);
		}
		else
		{
			StoreItem_T pack;
			pack.itmid = itemname;
			pack.payprice = price;
			pack.paytype = type;
			pack.name = name;
			pack.des = des;

			addSingleItem(pack);
		}
	}


}

void StoreAssetMgr::addPackItem(const StorePack_T packinfo)
{
	_vectorPackInfo.push_back(packinfo);
}

void StoreAssetMgr::addSingleItem(const StoreItem_T iteminfo)
{
	_vectorItemInfo.push_back(iteminfo);
}


int StoreAssetMgr::getSingleIdx(const std::string& itemid)
{
	int idx = -1;
	for (int i = 0; i < _vectorItemInfo.size(); i++)
	{
		if (_vectorItemInfo[i].itmid.compare(itemid) == 0)
		{
			idx = i;
			return idx;
		}
	}
	return -1;
}

int StoreAssetMgr::getPackIdx(const std::string& itemid)
{
	int idx = -1;
	for (int i = 0; i < _vectorPackInfo.size(); i++)
	{
		if (_vectorPackInfo[i].itmid.compare(itemid) == 0)
		{
			idx = i;
			return idx;
		}
	}

	return -1;
}


int StoreAssetMgr::getPackPrice(const std::string& itemid)
{
	for (int i = 0; i < _vectorPackInfo.size(); i++)
	{
		if (_vectorPackInfo[i].itmid.compare(itemid) == 0)
		{
			return _vectorPackInfo[i].payprice;
		}
	}

	return -1;
}


int StoreAssetMgr::getSinglePrice(const std::string& itemid)
{
	for (int i = 0; i < _vectorItemInfo.size(); i++)
	{
		if (_vectorItemInfo[i].itmid.compare(itemid) == 0)
		{
			return _vectorItemInfo[i].payprice;
		}
	}

	return -1;
}

std::string StoreAssetMgr::getPackDes(const std::string& itemid)
{
	for (int i = 0; i < _vectorItemInfo.size(); i++)
	{
		if (_vectorItemInfo[i].itmid.compare(itemid) == 0)
		{
			return _vectorItemInfo[i].des;
		}
	}

	return "";
}

int StoreAssetMgr::getPayType(const std::string& itemid)
{
	int type = -1;
	for (int i = 0; i < _vectorItemInfo.size(); i++)
	{
		if (_vectorItemInfo[i].itmid.compare(itemid) == 0)
		{
			type = _vectorItemInfo[i].paytype;
			return type;
		}
	}

	for (int i = 0; i < _vectorPackInfo.size(); i++)
	{
		if (_vectorPackInfo[i].itmid.compare(itemid) == 0)
		{
			type = _vectorPackInfo[i].paytype;
			return type;
		}
	}

	return type;
}

bool StoreAssetMgr::checkCanPay(int paytype, int price, bool poptip)
{
	bool can = false;
	if (paytype == 0)
	{
		if (UserData::getInstance()->getGoldNum() >= price)
		{
			can = true;
		}
		else if(poptip)
		{
			GameUtils::toastTip("no_gold");
		}
	}
	else if (paytype == 2)
	{
		if (UserData::getInstance()->getCrystalNum() >= price)
		{
			can = true;
		}
		else if(poptip)
		{
			GameUtils::toastTip("no_caystal");
		}
	}

	return can;
}