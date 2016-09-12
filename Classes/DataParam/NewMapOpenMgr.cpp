#include "NewMapOpenMgr.h"
#include "UserData.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "../Scenes/PopRewardLayer.h"
#include "../Scenes/MainScene.h"
#include "WeaponControl.h"
#include "ParamMgr.h"
#include "platBridge/cocos2dx_analyze.h"

NewMapOpenMgr* NewMapOpenMgr::s_pInstance = nullptr;

NewMapOpenMgr * NewMapOpenMgr::getInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new NewMapOpenMgr();
		s_pInstance->init();
	}
	return s_pInstance;
}

NewMapOpenMgr::~NewMapOpenMgr()
{
}

bool NewMapOpenMgr::isGet(MapOpenType type)
{
	return UserData::getInstance()->getNewMapConfig(type) == 0 ? false : true;
}

bool NewMapOpenMgr::tryGet(MapOpenType type, bool needpop)
{
	if (UserData::getInstance()->getNewMapConfig(type) > 0)
	{
		return false;
	}
	

	if (UserData::getInstance()->getIsBossPass(80) == 0)
	{
		return false;
	}

	bool isget = false;
	int p = 0;

	switch (type)
	{
	case PASS68:
		p = ToolsUtil::getRandomInt(1, 100);
		if (p < 30)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	case PASS77:
		p = ToolsUtil::getRandomInt(1, 100);
		if (p < 20)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	case PASS80:
		p = ToolsUtil::getRandomInt(1, 100);
		if (p < 15)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	case TA200:
		p = ToolsUtil::getRandomInt(1, 100);
		if (p < 50)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	case TA500:
		p = ToolsUtil::getRandomInt(1, 100);
		if (p < 20)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	case WENPON20_0:
		{
			int lv = UserData::getInstance()->getPlayerCurLv(0);
			const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(0, lv);
			int posidx = WeaponControl::getInstance()->getEquipPosIdx(0);
			auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(0 * 10 + posidx);

			if (info.attack + weaponinfo.attack >= 500)
			{
				UserData::getInstance()->setNewMapConfig(type, 1);
				isget = true;
			}
		}
		break;
	case WENPON20_1:
	{
		int lv = UserData::getInstance()->getPlayerCurLv(1);
		const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(1, lv);
		int posidx = WeaponControl::getInstance()->getEquipPosIdx(1);
		auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(1 * 10 + posidx);

		if (info.attack + weaponinfo.attack >= 460)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
	}
		
		break;
	case WENPON20_2:
	{
		int lv = UserData::getInstance()->getPlayerCurLv(2);
		const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(2, lv);
		int posidx = WeaponControl::getInstance()->getEquipPosIdx(2);
		auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(2 * 10 + posidx);

		if (info.attack + weaponinfo.attack >= 480)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
	}		
		break;
	case WENPON20_3:
	{
		int lv = UserData::getInstance()->getPlayerCurLv(3);
		const PlayerInfo_T& info = ParamMgr::getInstance()->getPlayerInfo(3, lv);
		int posidx = WeaponControl::getInstance()->getEquipPosIdx(3);
		auto&  weaponinfo = UserData::getInstance()->getWeaponAttack(3 * 10 + posidx);

		if (info.hpadd + weaponinfo.hpAdd >= 450)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
	}
		break;
	case BGL50000:
		p = ToolsUtil::getRandomInt(1, 100);
		if(p < 10)
		{
			UserData::getInstance()->setNewMapConfig(type, 1);
			isget = true;
		}
		break;
	default:
		break;
	}

	if (isget && needpop == false)
	{
		_getnum++;
		UserData::getInstance()->saveUserData();
	}
	else if (isget && needpop == true)
	{
		_getnum++;
		popGet();
	}

	if (isget)
	{
		cocos2dx_analyze::buy(String::createWithFormat("newmap_config_%d", type)->getCString(), 1, 0);
	}

	return isget;
}

void NewMapOpenMgr::popGet()
{
	if (_getnum == 0)
	{
		return;
	}

	std::vector<PopItemInfo_T> arrItems;

	for (int i = 0; i < _getnum; i++)
	{
		
		PopItemInfo_T temp;
		temp.itemId = 1016;
		temp.itemCount = 1;
		arrItems.push_back(temp);
	}

	auto layer = PopRewardLayer::create(arrItems);
	MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);

	_getnum = 0;
}

void NewMapOpenMgr::init()
{
	_getnum = 0;
}
