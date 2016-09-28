#include "DayActiveMgr.h"
#include "UserData.h"
#include "../Scenes/PopRewardLayer.h"
#include "../Scenes/MainScene.h"
#include "BoxDataMgr.h"

DayActivityMgr* DayActivityMgr::s_pInstance = nullptr;

DayActivityRewardInfo _arrRewardInfo[DAYALLNUMS] = {
	{ DayActivityTppe::DAYPASS, 10, 5},
	{ DayActivityTppe::DAYWENPONUP, 5, 5 },
	{ DayActivityTppe::DAYUSEITEMS, 5, 2 },
	{ DayActivityTppe::DAYSPBOSS, 3, 10},
	{ DayActivityTppe::DAYTA, 5, 8 },
	{ DayActivityTppe::DAYBUYITEMS, 5, 3 },
	{ DayActivityTppe::DAYUSEBGL, 3, 10 },
	{ DayActivityTppe::DAYOPENGOLDBOX, 3, 10 },
	{ DayActivityTppe::DAYGETTILI, 3, 2 },
	{ DayActivityTppe::DAYBUYZUANSHI, 3, 20 },
	{ DayActivityTppe::DAYDUNGEON, 3, 10 },
};

int g_scoreReward[3] = { 20, 120, 240 };

DayActivityMgr * DayActivityMgr::getInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new DayActivityMgr();
		s_pInstance->init();
	}

	return s_pInstance;
}

DayActivityMgr::~DayActivityMgr()
{

}

void DayActivityMgr::addTimes(DayActivityTppe type, int num)
{
	this->addTimes(type, num, true);
}

void DayActivityMgr::addTimes(DayActivityTppe type, int num, bool save)
{
	(*_activityInfoMap[type]) += num;

	if (save)
	{
		UserData::getInstance()->saveUserData();
	}
	
	MainLayer::getCurMainLayer()->updataDayActivityBtn();
}

int DayActivityMgr::getAllActivityScore()
{
	int score = 0;
	for (auto& info : _arrRewardInfo)
	{
		int num = *_activityInfoMap[info.activityType] > info.maxNum ? info.maxNum : *_activityInfoMap[info.activityType];
		score += (info.everyNumRewrad * num);
	}
	return score;
}

int DayActivityMgr::getActivityScore(DayActivityTppe type)
{
	auto& info = _arrRewardInfo[type];
	int num = *_activityInfoMap[type] > info.maxNum ? info.maxNum : *_activityInfoMap[info.activityType];
	int score = (info.everyNumRewrad * num);
	return score;
}

int DayActivityMgr::getActivityTimes(DayActivityTppe type)
{
	auto& info = _arrRewardInfo[type];
	int num = *_activityInfoMap[type] > info.maxNum ? info.maxNum : *_activityInfoMap[info.activityType];
	return num;
}

int DayActivityMgr::getActivityMaxTimes(DayActivityTppe type)
{
	auto& info = _arrRewardInfo[type];
	return info.maxNum;
}

int DayActivityMgr::getNeedScore()
{
	int cur = getAllActivityScore();
	for (auto temp : g_scoreReward)
	{
		if (temp - cur > 0)
		{
			return temp - cur;
		}
	}

	return 0;
}

void DayActivityMgr::resetTimes()
{
	for (auto& info : _activityInfoMap)
	{
		(*(info.second)) = 0;
	}

	UserData::getInstance()->setDayActiveReward(0, 0);
	UserData::getInstance()->setDayActiveReward(1, 0);
	UserData::getInstance()->setDayActiveReward(2, 0);

	UserData::getInstance()->saveUserData();
}

int DayActivityMgr::checkCanGet(int idx)
{
	int flag = 0;
	
	int score = getAllActivityScore();
	if (score >= g_scoreReward[idx])
	{
		if (UserData::getInstance()->getDayActiveReward(idx) >= 1)
		{
			flag = 2;
		}
		else
		{
			flag = 1;
		}
	}
	else
	{
		flag = 0;
	}

	return flag;
}

void DayActivityMgr::getReward()
{
	int score = getAllActivityScore();
	
	if (score >= g_scoreReward[0] && UserData::getInstance()->getDayActiveReward(0) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		PopItemInfo_T temp;
		temp.itemId = ParamData::TILI_ITEM_ID;
		temp.itemCount = 2;
		arrItems.push_back(temp);

		temp.itemId = 1004;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		UserData::getInstance()->setDayActiveReward(0, 1);

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
	}
	else if (score >= g_scoreReward[1] && UserData::getInstance()->getDayActiveReward(1) == 0)
	{
		std::vector<PopItemInfo_T> arrItems;

		PopItemInfo_T temp;
		temp.itemId = ParamData::TILI_ITEM_ID;
		temp.itemCount = 5;
		arrItems.push_back(temp);

		temp.itemId = 1011;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		temp.itemId = 1015;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		temp.itemId = 1014;
		temp.itemCount = 1;
		arrItems.push_back(temp);

		UserData::getInstance()->setDayActiveReward(1, 1);

		auto layer = PopRewardLayer::create(arrItems);
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
	}
	else if (score >= g_scoreReward[2] && UserData::getInstance()->getDayActiveReward(2) == 0)
	{
		UserData::getInstance()->setDayActiveReward(2, 1);

		auto layer = PopRewardLayer::openBox(2, BoxDataMgr::getInstance()->getCurGetBoxLv(), 1);
		MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
	}

	UserData::getInstance()->saveUserData();

	MainLayer::getCurMainLayer()->updataDayActivityBtn();
}

int DayActivityMgr::getFinishNum()
{
	int numfinish = 0;
	for (auto& info : _arrRewardInfo)
	{
		if (*_activityInfoMap[info.activityType] >= info.maxNum)
		{
			numfinish++;
		}
	}
	return numfinish;
}

void DayActivityMgr::init()
{
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYPASS, &UserData::getInstance()->getDayPass()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYWENPONUP, &UserData::getInstance()->getDayWenponUp()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYUSEITEMS, &UserData::getInstance()->getDayUseItems()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYSPBOSS, &UserData::getInstance()->getDaySpBoss()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYTA, &UserData::getInstance()->getDayTa()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYBUYITEMS, &UserData::getInstance()->getDayBuyItems()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYUSEBGL, &UserData::getInstance()->getDayUseBGL()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYOPENGOLDBOX, &UserData::getInstance()->getDayOpenGoldBox()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYGETTILI, &UserData::getInstance()->getDayGetTilii()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYBUYZUANSHI, &UserData::getInstance()->getDayBuyZuanshi()));
	_activityInfoMap.insert(std::pair<int, int*>(DayActivityTppe::DAYDUNGEON, &UserData::getInstance()->getDayDungeon()));
}
