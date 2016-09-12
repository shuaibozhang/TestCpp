#ifndef __WEAPONCONTRAL_H__
#define __WEAPONCONTRAL_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;
/*o is unequip, 1 is equip 2 is add*/
typedef std::function<void(int playeridx, int weapon, int actiontype)> ccWeaponCallback;

struct WeaponControl_T
{
	int weaponid;
	bool unlock;
	bool isEquip;
};

class WeaponControl
{
public:
	~WeaponControl();
	static WeaponControl* getInstance();
	void loadWeaponConfig();

	void equipWeapon(int playeridx, int skillid);

	bool addWeaponListener(Node* target, ccWeaponCallback callback);
	void removeWeaponListener(Node* target);

	bool checkIsEquipWeapon(int playidx, int skillidx);
	bool checkIsEquipWeapon(int weaponidx);

	void addNewWeapon(int skillid);
	bool isWeaponUnLock(int skillid);

	const Weapon_T & getEquipWenpon(int playerid);
	int getEquipPosIdx(int playerid);

	int getPosIdxByWeaponId(int weaponId) { return (weaponId - s_weaponStartIdx) % 10; }

	const static int s_weaponStartIdx = 500;
	
	int getWenponOwner(int wenponid);

	int getWenponType(int posidx);
private:
	WeaponControl();
	static WeaponControl* _instance;
	std::map<std::string, ccWeaponCallback> _callBackMap;
	WeaponControl_T _arrWeaponControl[40];
};
#endif

