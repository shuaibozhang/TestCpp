#include "WeaponControl.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "UserData.h"
#include "../WxSql/GLLocalStorage.h"
#include "AchieveMgr.h"

WeaponControl* WeaponControl::_instance = nullptr;

int WeaponControl::getWenponOwner(int wenponid)
{
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		if (_arrWeaponControl[i].weaponid == wenponid)
		{
			return i / 10;
		}
	}
	return 0;
}

int WeaponControl::getWenponType(int posidx)
{
	int postype[] = { 0,0,0,1,1,1,2,2,2,3 };
	return *(postype + posidx);
}

void WeaponControl::getPlayerUnlockWenponIds(int playerid, std::vector<int>& temp)
{
	for (int i = 0; i < 10; i++)
	{
		if (_arrWeaponControl[i + playerid * 10].unlock == true)
		{
			temp.push_back(_arrWeaponControl[i + playerid * 10].weaponid);
		}
	}
}

WeaponControl::WeaponControl()
{

}

WeaponControl::~WeaponControl()
{
}

WeaponControl * WeaponControl::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new WeaponControl();
		_instance->loadWeaponConfig();
	}
	return _instance;
}

void WeaponControl::loadWeaponConfig()
{
	auto& vectorWenpon = ParamMgr::getInstance()->getWeaponVector();
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		WeaponControl_T temp;
		temp.weaponid = vectorWenpon[i].id;
		temp.isEquip = UserData::getInstance()->getIsWeaponEquip(temp.weaponid) == 1 ? true : false;
		temp.unlock = UserData::getInstance()->getItemBalance(String::createWithFormat("itemid_good_weapon_%d", temp.weaponid)->getCString(), false) > 0 ? true : false;

		if (temp.unlock == true && UserData::getInstance()->getWeaponLv(i) == 0)
		{
			UserData::getInstance()->setWeaponLv(i, 1);
		}

		_arrWeaponControl[i] = temp;
	}

	GLLocalStorage::glLocalStorageFree();
}

void WeaponControl::equipWeapon(int playeridx, int weaponid)
{
	int staridx = playeridx * 10;

	int lastskillPosidx = -1;
	int needchangePosid = -1;

	auto& skillinfo = ParamMgr::getInstance()->getWeaponByID(weaponid);

	for (int i = 0; i < 10; i++)
	{
		auto& temp = _arrWeaponControl[staridx + i];
		if (temp.isEquip == true)
		{
			lastskillPosidx = i;
		}

		if (temp.weaponid == weaponid)
		{
			needchangePosid = i;
		}
	}

	if (lastskillPosidx != -1)
	{
		_arrWeaponControl[staridx + lastskillPosidx].isEquip = false;

		UserData::getInstance()->setWeaponEquip(_arrWeaponControl[staridx + lastskillPosidx].weaponid, 0);

		for (auto temp : _callBackMap)
		{
			temp.second(playeridx, _arrWeaponControl[staridx + lastskillPosidx].weaponid, 0);
		}
	}

	if (needchangePosid != -1)
	{
		_arrWeaponControl[staridx + needchangePosid].isEquip = true;

		UserData::getInstance()->setWeaponEquip(_arrWeaponControl[staridx + needchangePosid].weaponid, 1);

		for (auto temp : _callBackMap)
		{
			temp.second(playeridx, weaponid, 1);
		}
	}

	UserData::getInstance()->saveUserData();
}

bool WeaponControl::addWeaponListener(Node * target, ccWeaponCallback callback)
{
	auto name = target->getName();
	CCASSERT(name.size() > 0, "you should set name to the node");
	CCASSERT(_callBackMap.find(name) == _callBackMap.end(), "you alread have add this name");

	_callBackMap.insert(std::make_pair(name, callback));
	return true;
}

void WeaponControl::removeWeaponListener(Node * target)
{
	auto name = target->getName();
	CCASSERT(name.size() > 0, "you should set name to the node");
	CCASSERT(_callBackMap.find(name) != _callBackMap.end(), "can not find listener");

	_callBackMap.erase(name);
}

bool WeaponControl::checkIsEquipWeapon(int playidx, int skillidx)
{
	for (int i = 0; i < 10; i++)
	{
		if (_arrWeaponControl[playidx * 10 + i].weaponid == skillidx)
		{
			return _arrWeaponControl[playidx * 10 + i].isEquip;
		}
	}
	CCASSERT(0, "no find weapon");
	return false;
}

bool WeaponControl::checkIsEquipWeapon(int weaponidx)
{
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		if (_arrWeaponControl[i].weaponid == weaponidx)
		{
			return _arrWeaponControl[i].isEquip;
		}
	}
	CCASSERT(0, "no find weapon");
	return false;
}

void WeaponControl::addNewWeapon(int skillid)
{
	//UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", skillid)->getCString(), 1);
	int idx = -1;
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		if (_arrWeaponControl[i].weaponid == skillid)
		{
			if (_arrWeaponControl[i].unlock == false)
			{
				UserData::getInstance()->setWeaponLv(i, 1);
			}

			_arrWeaponControl[i].unlock = true;
			idx = i;
			break;
		}
	}

	for (auto temp : _callBackMap)
	{
		temp.second(idx / 10, skillid, 2);
	}

	AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::WEAPON, 1, false);
	
	UserData::getInstance()->saveUserData();
}

bool WeaponControl::isWeaponUnLock(int skillid)
{
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		if (_arrWeaponControl[i].weaponid == skillid)
		{
			return _arrWeaponControl[i].unlock;
		}
	}
	CCASSERT(0, "no find weapon");
	return false;
}

const Weapon_T & WeaponControl::getEquipWenpon(int playerid)
{
	for (int i = 0; i < 10; i++)
	{
		if (_arrWeaponControl[10 * playerid + i].isEquip == true)
		{
			return ParamMgr::getInstance()->getWeaponByID(_arrWeaponControl[10 * playerid + i].weaponid);
		}
	}

	return ParamMgr::getInstance()->getWeaponByID(_arrWeaponControl[10 * playerid].weaponid);
}

int WeaponControl::getEquipPosIdx(int playerid)
{
	for (int i = 0; i < 10; i++)
	{
		if (_arrWeaponControl[10 * playerid + i].isEquip == true)
		{
			return i;
		}
	}

	return 0;
}
