#include "DataParam\MagPieMgr.h"
#include "UserData.h"
#include "CrushUtil.h"

const MagPieTask_T g_arrMagPieTaskInfo[ParamData::MagPie_TOTAL_TASKS_NUM] =
{
	{ 77, 1, { {ParamData::TILI_ITEM_ID, 10}, {0} } },
	{ 77, 2,{ { 1531, 50 },{ 1501, 50 } } },
	{ 7, 1,{ { ParamData::TILI_ITEM_ID, 20 },{ 0 } } },
	{ 7, 1,{ { 1513, 30 },{ 0 } } },
	{ 7, 1,{ { 1533, 60 },{ 0 } } },
	{ 7, 1,{ { 1526, 30 },{ 0 } } },
	{ 7, 1,{ { 1506, 60 },{ 0 } } },
};

MagPieMgr *MagPieMgr::s_pInstance = nullptr;

MagPieMgr * MagPieMgr::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new MagPieMgr();
	}
	return s_pInstance;
}

int MagPieMgr::getFinishMagPieTasksNum()
{
	int count = 0;
	for (int i = 0; i < ParamData::MagPie_TOTAL_TASKS_NUM; i++)
	{
		if (this->checkIsFinishMagPieTask(i))
		{
			count++;
		}
	}
	return count;
}

bool MagPieMgr::checkIsFinishMagPieTask(int idx)
{
	int totalCount = this->getMagPieTaskGoalNum(idx);
	int count = this->getCurFinishedMagPieGoalNum(idx);

	return (count >= totalCount);
}

bool MagPieMgr::checkIsGotReward(int idx)
{
	auto &info = UserData::getInstance()->getUserMagPieInfo(idx);
	return info.isGotReward;
}

int MagPieMgr::getMagPieTaskGoalNum(int idx)
{
	return g_arrMagPieTaskInfo[idx].goalNum;
}

void MagPieMgr::getMagPieReward(int idx, std::vector<PopItemInfo_T> &arrReward)
{
	for (int i = 0; i < g_arrMagPieTaskInfo[idx].rewardCount; i++)
	{
		CrushUtil::addPopItem(arrReward, g_arrMagPieTaskInfo[idx].arrReward[i].itemId, g_arrMagPieTaskInfo[idx].arrReward[i].itemCount);
	}

	int finishCount = this->getFinishMagPieTasksNum();

	if (2 == finishCount)
	{
		CrushUtil::addPopItem(arrReward, 1014, 5);
	}
	else if (4 == finishCount)
	{
		CrushUtil::addPopItem(arrReward, 7, 1);
	}
	else if (6 == finishCount)
	{
		CrushUtil::addPopItem(arrReward, 31, 1);
	}
}

int MagPieMgr::getCurFinishedMagPieGoalNum(int idx)
{
	return UserData::getInstance()->getUserMagPieInfo(idx).curFinishedGoalNum;
}

void MagPieMgr::addFinishedMagPieGoalNum(int idx, int addNum)
{
	if (idx != _dayIdx)
	{
		return;
	}
	int curNum = this->getCurFinishedMagPieGoalNum(idx);
	UserData::getInstance()->setMagPieGoalNum(idx, curNum + addNum);
//	UserData::getInstance()->saveUserData(true);
}

void MagPieMgr::setIsGotMagPieReward(int idx)
{
	UserData::getInstance()->setIsGotMagPieReward(idx);
	UserData::getInstance()->saveUserData(true);
}

int MagPieMgr::getMissionState(int idx)
{
	if (this->checkIsGotReward(idx))
	{
		return 2;
	}
	else if (this->checkIsFinishMagPieTask(idx))
	{
		return 1;
	}

	return 0;
}

MagPieMgr::MagPieMgr()
	:_dayIdx(ParamData::MagPie_TOTAL_TASKS_NUM)
{

}


MagPieMgr::~MagPieMgr()
{

}
