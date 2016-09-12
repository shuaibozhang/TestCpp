#include "AchieveMgr.h"
#include "UserData.h"

const AchGoal_T g_arrAchGoalInfo[ParamData::ACHIEVE_TOTAL_COUNT][3] = {
	{{1000, 5}, {10000, 10}, {100000, 20}},
	{ { 10, 5 },{ 100, 10 },{ 1000, 20 } },
	{ { 10, 5 },{ 20, 10 },{ 40, 20 } },
	{ { 20, 5 },{ 60, 10 },{ 100, 20 } },
	{ { 1, 5 },{ 10, 10 },{ 50, 20 } },
	{ { 10, 5 },{ 100, 10 },{ 300, 20 } },
};

AchieveMgr *AchieveMgr::s_pInstance = nullptr;

AchieveMgr * AchieveMgr::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new AchieveMgr();
	}
	return s_pInstance;
}

AchieveMgr::~AchieveMgr()
{
}

int AchieveMgr::checkFinishAchieve()
{
	int count = 0;
	for (int i = 0; i < ParamData::ACHIEVE_TOTAL_COUNT; i++)
	{
		if (this->checkIsFinishAchieve(i))
		{
			count++;
		}
	}
	return count;
}

bool AchieveMgr::checkIsFinishAchieve(int idx)
{
	int totalCount = this->getCurAchieveTotalCount(idx);
	int count = this->getCurAchieveCount(idx);
	int lv = this->getCurAchieveLv(idx);

	return (count >= totalCount) && lv < 3;
}

int AchieveMgr::getCurAchieveLv(int idx)
{
	return UserData::getInstance()->getAchieveInfo(idx).curLv;
}

int AchieveMgr::getCurAchieveTotalCount(int idx)
{
	int curLv = UserData::getInstance()->getAchieveInfo(idx).curLv;
	curLv = (curLv > 2) ? 2 : curLv;

	return g_arrAchGoalInfo[idx][curLv].count;
}

int AchieveMgr::getCurAchieveCount(int idx)
{
	return UserData::getInstance()->getAchieveInfo(idx).curCount;
}

int AchieveMgr::getCurAchieveReward(int idx)
{
	int curLv = UserData::getInstance()->getAchieveInfo(idx).curLv;
	curLv = (curLv > 2) ? 2 : curLv;

	return g_arrAchGoalInfo[idx][curLv].reward;
}

void AchieveMgr::addAchieveCount(int idx, int addCount, bool isSave)
{
	int curCount = this->getCurAchieveCount(idx);
	UserData::getInstance()->setAchieveCount(idx, curCount+addCount);

	if (isSave)
	{
		UserData::getInstance()->saveUserData(true);
	}
}

void AchieveMgr::finishAchieceLv(int idx)
{
	int lv = this->getCurAchieveLv(idx);
	UserData::getInstance()->setAchieveLv(idx, lv + 1);
	UserData::getInstance()->saveUserData(true);
}

AchieveMgr::AchieveMgr()
{

}