#include "BoxDataMgr.h"
#include "ParamMgr.h"
#include "GLCommon/JRCommon.h"
#include "CrushUtil.h"
#include "UserData.h"

BoxDataMgr * BoxDataMgr::getInstance()
{
	return nullptr;
}

BoxDataMgr::BoxDataMgr()
{
}

int BoxDataMgr::getItemId(int categoryIdx, int itemIdx)
{
	int ret = 0;
	int randomIdx = ToolsUtil::getRandomInt(0, 3);

	switch (categoryIdx)
	{
	case 0:
		ret = 1500 + itemIdx + randomIdx * 10;
		break;
	case 1:
		ret = 1503 + itemIdx + randomIdx * 10;
		break;
	case 2:
		ret = 1506 + itemIdx + randomIdx * 10;
		break;
	case 3:
		ret = 1509 + itemIdx + randomIdx * 10;
		break;
	case 4:
		ret = ParamData::TILI_ITEM_ID;
		break;
	case 5:
		ret = ParamData::GOLD_ITEM_ID;
		break;
	case 6:
		ret = ParamData::CRYSTAL_ITEM_ID;
		break;
	case 7:
		ret = itemIdx;
		break;
	}

	return ret;
}

BoxDataMgr::~BoxDataMgr()
{
}

void BoxDataMgr::getBoxReward(int boxIdx, int boxLv, std::vector<PopItemInfo_T>& arrReward)
{
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);

	for (int i = 0; i < ParamData::BOX_CATEGORY_COUNT; i++)
	{
		float randomValue = ToolsUtil::getRandomInt(0, 100);
		auto pCathInfo = &(pBoxInfo->arrCatgInfo[i]);

		if (randomValue <= pCathInfo->totalPercent)
		{
			std::vector<int> arrGot;
			
			ToolsUtil::getRandoms(pCathInfo->itemTypeCount, pCathInfo->arrItemWeight, arrGot, pCathInfo->giveTypeCount);

			for (int j = 0; j < arrGot.size(); j++)
			{
				int itemId = arrGot.at(j);

				if (i > 6)
				{
					itemId = pCathInfo->arrItemId[itemId];
				}
				else
				{
					itemId = this->getItemId(i, itemId);
				}

				CrushUtil::addPopItem(arrReward, itemId, pCathInfo->arrGiveCount[boxLv]);

				int popIdx = 0;
				if (i >= 4 && i <= 6)
				{
					popIdx = 0;
				}
				else if (i >= 0 && i < 4)
				{
					popIdx = i + 1;
				}
				else
				{
					if (boxIdx < 8)
					{
						popIdx = 5;
					}
					else
					{
						popIdx = 0;
					}
				}
				arrReward.back().popIdx = popIdx;
			}
		}
	}
}

int BoxDataMgr::getBoxOpenCost(int boxId, int surplusTime)
{
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxId);

	surplusTime += (pBoxInfo->waitTime / pBoxInfo->openCost - 1);

	return pBoxInfo->openCost * surplusTime / pBoxInfo->waitTime;
}

int BoxDataMgr::getBoxBuyCost(int boxId)
{
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxId);
	return pBoxInfo->buyCost;
}

int BoxDataMgr::getBoxGoldCount(int boxIdx, int boxLv, int countType)
{
	int ret = 0;
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);
	auto pCathInfo = &(pBoxInfo->arrCatgInfo[5]);

	if (0 == countType && pCathInfo->totalPercent < 100)
	{
		ret = 0;
	}
	else
	{
		ret = (pCathInfo->arrGiveCount[boxLv] * pCathInfo->giveTypeCount);
	}

	return ret;
}
/*
int BoxDataMgr::getBoxDescChipCount(int boxIdx, int boxLv)
{
	int ret = 0;
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);
	
	for (int i = 0; i < 4; i++)
	{
		auto pCathInfo = &(pBoxInfo->arrCatgInfo[i]);
		ret += (pCathInfo->arrGiveCount[boxLv] * pCathInfo->giveTypeCount);
	}
	return ret;
}

int BoxDataMgr::getBoxDescRareChipCount(int boxIdx, int boxLv)
{
	int ret = 0;
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);

	for (int i = 2; i < 4; i++)
	{
		auto pCathInfo = &(pBoxInfo->arrCatgInfo[i]);
		ret += (pCathInfo->arrGiveCount[boxLv] * pCathInfo->giveTypeCount);
	}
	return ret;
}*/

int BoxDataMgr::getBoxChipCount(int boxIdx, int boxLv, int chipType, int countType)
{
	int ret = 0;
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);
	auto pCathInfo = &(pBoxInfo->arrCatgInfo[chipType]);

	if (0 == countType && pCathInfo->totalPercent < 100)
	{
		ret = 0;
	}
	else
	{
		ret = (pCathInfo->arrGiveCount[boxLv] * pCathInfo->giveTypeCount);
	}


	return ret;
}

int BoxDataMgr::getBoxAboveChipCount(int boxIdx, int boxLv, int chipType, int countType)
{
	int ret = 0;

	for (int i = chipType; i < 4; i++)
	{
		ret += getBoxChipCount(boxIdx, boxLv, i, countType);
	}

	return ret;
}

int BoxDataMgr::getWaitTime(int boxIdx)
{
	auto pBoxInfo = ParamMgr::getInstance()->getBoxDetailInfo(boxIdx);
	return pBoxInfo->waitTime;
}

int BoxDataMgr::getCurGetBoxLv(int sceneId)
{
	if (-1 == sceneId)
	{
		sceneId = UserData::getInstance()->getMaxPos();
	}
	int lv = 0;
//	int playerlv = UserData::getInstance()->getPlayerCurLv(0);
//	lv = playerlv / 10;
//	lv = lv > 4 ? 4 : lv;

	if (sceneId < 32)
	{
		lv = 0;
	}
	else if (sceneId < 48)
	{
		lv = 1;
	}
	else if (sceneId < 64)
	{
		lv = 2;
	}
	else if (sceneId < 77)
	{
		lv = 3;
	}
	else
	{
		lv = 4;
	}

	return lv;
}

bool BoxDataMgr::isBoxInBagFull()
{
	for (int i = 0; i < 4; i++)
	{
		auto& info = UserData::getInstance()->getBoxLayerConfig(i);
		
		if (info.boxidx == -1)
		{
			return false;
		}
	}
	return true;
}
