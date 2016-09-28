#include "PlayerMgr.h"
#include "UserData.h"
#include "../StoreBridge/StoreAssets.h"
#include "../Scenes/PopRewardLayer.h"
#include "../Scenes/MainScene.h"

PlayerMgr* PlayerMgr::_inStance = nullptr;

PlayerMgr * PlayerMgr::getInstance()
{
	if (_inStance == nullptr)
	{
		_inStance = new PlayerMgr();
	}
	return _inStance;
}

PlayerMgr::~PlayerMgr()
{
}

void PlayerMgr::equipPlayer(int idx)
{
	auto laststage = getPlayerStage(idx);
	if (laststage == PLAYERSTAGE_OFF)
	{
		UserData::getInstance()->setPlayerIsOn(idx, 1);
		UserData::getInstance()->saveUserData();

		for (auto& temp : _arrPlayerStageChangeObserver)
		{
			temp(idx, 1);
		}
	}
}

void PlayerMgr::unEquipPlayer(int idx)
{
	auto laststage = getPlayerStage(idx);
	if (laststage == PLAYERSTAGE_ON)
	{
		UserData::getInstance()->setPlayerIsOn(idx, 0);
		UserData::getInstance()->saveUserData();

		for (auto& temp : _arrPlayerStageChangeObserver)
		{
			temp(idx, 0);
		}
	}
}

void PlayerMgr::unLockPlayer(int idx)
{	
	UserData::getInstance()->setPlayerCurLv(idx, UserData::getInstance()->getPlayerCurLv(1));
	UserData::getInstance()->setPlayerIsOn(idx, 0);
	UserData::getInstance()->saveUserData();

	for (auto& temp : _arrPlayerStageChangeObserver)
	{
		temp(idx, 2);
	}
	
	auto layer = PopRewardLayer::createRoleReward(4);
	MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);

}

PlayerStage_E PlayerMgr::switchStage(int idx)
{
	auto stage = getPlayerStage(idx);
	if (stage == PLAYERSTAGE_LOCK)
	{
		
	}
	else if(stage == PLAYERSTAGE_ON)
	{
		unEquipPlayer(idx);
		stage = PLAYERSTAGE_OFF;
	}
	else if (stage == PLAYERSTAGE_OFF)
	{
		equipPlayer(idx);
		stage = PLAYERSTAGE_ON;
	}

	return stage;
}


PlayerStage_E PlayerMgr::getPlayerStage(int playeridx)
{
	PlayerStage_E stage = PLAYERSTAGE_LOCK;
	if (playeridx <=3 )
	{
		if (UserData::getInstance()->getPlayerIsOn(playeridx) == 1)
		{
			stage = PLAYERSTAGE_ON;
		}
		else if (UserData::getInstance()->getPlayerIsOn(playeridx) == 0)
		{
			stage = PLAYERSTAGE_OFF;
		}
	}
	else
	{
		if (UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_PLAYER_4) != 0)
		{
			if (UserData::getInstance()->getPlayerIsOn(playeridx) == 1)
			{
				stage = PLAYERSTAGE_ON;
			}
			else if (UserData::getInstance()->getPlayerIsOn(playeridx) == 0)
			{
				stage = PLAYERSTAGE_OFF;
			}
		}
	}


	return stage;
}

void PlayerMgr::addPlayerStageChangeObserver(ccPlayerCallback changeCallback)
{
	_arrPlayerStageChangeObserver.push_back(changeCallback);
}

bool PlayerMgr::canEquipNewPlayer()
{
	int curnum = 0;
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		if (UserData::getInstance()->getPlayerIsOn(i) != 0)
		{
			curnum++;
		}
	}

	return curnum >= ParamData::FIGHT_ROLE_COUNT ? false : true;
}

int PlayerMgr::getEquipCount()
{
	int count = 0;
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		if (getPlayerStage(i) != PLAYERSTAGE_LOCK)
		{
			if (UserData::getInstance()->getPlayerIsOn(i) != 0)
			{
				count++;
			}
		}
	}
	return count;
}

void PlayerMgr::fixPlayer()
{
	int curEquCount = getEquipCount();
	for (int i = 0; i < ParamData::ROLE_COUNT && curEquCount != ParamData::FIGHT_ROLE_COUNT; i++)
	{
		if (getPlayerStage(i) != PLAYERSTAGE_LOCK)
		{
			if (curEquCount < ParamData::FIGHT_ROLE_COUNT)
			{
				if (UserData::getInstance()->getPlayerIsOn(i) == 0)
				{
					UserData::getInstance()->setPlayerIsOn(i, 1);
					curEquCount++;
				}
			}
			else if (curEquCount > ParamData::FIGHT_ROLE_COUNT)
			{
				if (UserData::getInstance()->getPlayerIsOn(i) != 0)
				{
					UserData::getInstance()->setPlayerIsOn(i, 0);
					curEquCount--;
				}
			}
		}
	}
}
