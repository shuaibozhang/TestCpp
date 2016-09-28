#include "SpecialAttUtils.h"
#include "GameLayer.h"
#include "CrushLayer.h"
#include "EleIcon.h"
#include "JRCommon.h"
#include "CrushUtil.h"
#include "Player.h"
#include "FightLayer.h"
#include "Monster.h"


void SpecialAttUtils::attPlayer(float damage)
{
	CrushLayer::getInstance();
	Player::getInstance()->doHurt(AttAttrbt_E::ATT_FIRE, damage);
}


void SpecialAttUtils::createWeak(int weakId, int count, int posType, CrushIndex_T posIndex)
{
	if (0 == posType)
	{
		CrushIndex_T arrOffsetPos[8] = {{1, 0},{1, 1},{0, 1},{-1, 1},{-1, 0},{-1, -1},{0, -1},{1, -1},};

		EleIcon *(*arrEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();
		for (int i=0; i<8; i++)
		{
			CrushIndex_T desPos;
			desPos.row = posIndex.row + arrOffsetPos[i].row;
			desPos.column = posIndex.column + arrOffsetPos[i].column;

			if (desPos.row > -1 && desPos.row < ParamData::CRUSH_ROW
				&& desPos.column > -1 && desPos.column < ParamData::CRUSH_COL)
			{
				if (nullptr != arrEle[desPos.row][desPos.column])
				{
					arrEle[desPos.row][desPos.column]->obtainWeak(weakId);
				}
			}
		}
	}
	else if (1 == posType)
	{
		EleIcon *(*arrEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();

		for (int i=0; i<count; i++)
		{
			int row = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW-1);
			int col = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL-1);

			if (nullptr != arrEle[row][col])
			{
				arrEle[row][col]->obtainWeak(weakId);
			}
		}
	}
}


typedef struct _PosWeight_T
{
	int weight;
	CrushIndex_T posIndex;
}PosWeight_T;


void SpecialAttUtils::createSpecEle(EleSpecialType specType, int eleId, int count, CrushIndex_T posIndex)
{
	if (EleSpecialType::THORN_EYE == specType)
	{
		EleIcon *(*arrEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();
		if (count > 1)
		{
			CrushIndex_T arrOffset[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
			int curCount = 0;
			for (int i=0; i<4 && curCount<count; i++)
			{
				CrushIndex_T tmpPos;

				tmpPos.row = posIndex.row + arrOffset[i].row;
				tmpPos.column = posIndex.column + arrOffset[i].column;
				if (tmpPos.row >= 0 && tmpPos.row < ParamData::CRUSH_ROW
					&& tmpPos.column >= 0 && tmpPos.column < ParamData::CRUSH_COL)
				{
					EleIcon *pEle = arrEle[tmpPos.row][tmpPos.column];

					if (nullptr != pEle && pEle->getEleId() >= EleIconId_E::ELE_ID_0 && pEle->getEleId() < EleIconId_E::ELE_ID_ROLE_COUNT
						&& nullptr == pEle->getWeakInfo())
					{
						pEle->doRemove(false);

						auto pNewEle = EleIcon::create(eleId, 0);
						pNewEle->setPosIndex(tmpPos);
						pNewEle->setPosition(CrushUtil::getElePos(tmpPos.row, tmpPos.column));
						arrEle[tmpPos.row][tmpPos.column] = pNewEle;

						CrushLayer::getInstance()->addChild(pNewEle);

						curCount++;
					}

				}
			}
		}
		else if (1 == count)
		{
			int arrTag[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
			SpecialAttUtils::travelseThorn(posIndex.row, posIndex.column, arrTag, arrEle);

			Vector<EleIcon *> arrVaild;
			for (int i=0; i<ParamData::CRUSH_ROW; i++)
			{
				for (int j=0; j<ParamData::CRUSH_COL; j++)
				{
					if (-1 == arrTag[i][j])
					{
						arrVaild.pushBack(arrEle[i][j]);
					}
				}
			}

			if (0 != arrVaild.size())
			{
				auto pEle = arrVaild.getRandomObject();
				auto curPos = pEle->getPosIndex();

				pEle->doRemove(false);

				auto pNewEle = EleIcon::create(eleId, 0);
				pNewEle->setPosIndex(curPos);
				pNewEle->setPosition(CrushUtil::getElePos(curPos.row, curPos.column));
				arrEle[curPos.row][curPos.column] = pNewEle;

				CrushLayer::getInstance()->addChild(pNewEle);
			}
		}

#if 0
		PosWeight_T arrWeight[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};
		std::vector<PosWeight_T *> arrSortWeight;//(ParamData::CRUSH_ROW*ParamData::CRUSH_COL);

		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j=0; j<ParamData::CRUSH_COL; j++)
			{
				arrWeight[i][j].posIndex.row = i;
				arrWeight[i][j].posIndex.column = j;
				arrWeight[i][j].weight = (i-posIndex.row)*(i-posIndex.row) + (j-posIndex.column)*(j-posIndex.column);

				bool isInsert = false;
				for (auto it=arrSortWeight.begin(); it!=arrSortWeight.end(); it++)
				{
					if (arrWeight[i][j].weight < (*it)->weight)
					{
						arrSortWeight.insert(it, &arrWeight[i][j]);
						isInsert = true;
						break;
					}
				}

				if (!isInsert)
				{
					arrSortWeight.push_back(&arrWeight[i][j]);
				}
			}
		}

		int curCount = 0;
		for (auto it=arrSortWeight.begin(); it!=arrSortWeight.end() && curCount<count; it++)
		{
			EleIcon *(*arrEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();
			CrushIndex_T curPos = (*it)->posIndex;
			EleIcon *pEle = arrEle[curPos.row][curPos.column];

			if (nullptr != pEle && pEle->getEleId() >= EleIconId_E::ELE_ID_SWORD && pEle->getEleId() <= EleIconId_E::ELE_ID_HEART
				&& nullptr == pEle->getWeakInfo())
			{
				pEle->doRemove(false);

				auto pNewEle = EleIcon::create(eleId, 0);
				pNewEle->setPosIndex(curPos);
				pNewEle->setPosition(CrushUtil::getElePos(curPos.row, curPos.column));
				arrEle[curPos.row][curPos.column] = pNewEle;

				CrushLayer::getInstance()->addChild(pNewEle);

				curCount++;
			}
		}
#endif
	}
}

/*
void SpecialAttUtils::addMonsterHp(float hp)
{
	auto arrMonsters = FightLayer::getInstance()->getCurArrMonster();

	for (int i = 0; i < arrMonsters->size(); i++)
	{
		auto pMonster = arrMonsters->at(i);
		pMonster->addHP(hp);
	}
}*/


void SpecialAttUtils::openBox(int count)
{
	auto arrMonsters = FightLayer::getInstance()->getCurArrMonster();

	for (int i = 0; i < arrMonsters->size(); i++)
	{
		auto pMonster = arrMonsters->at(i);
		if (2 == pMonster->getMonsterInfo()->type)
		{
			pMonster->openBox();
			break;
		}
	}
}


int SpecialAttUtils::createStoneState()
{
	int ret = -1;
	int randomValue = ToolsUtil::getRandomInt(0, ParamData::FIGHT_ROLE_COUNT-1);

	int startId = randomValue;

	do 
	{
		if (!CrushLayer::getInstance()->isStoneState(startId))
		{
			ret = startId;
			break;
		}

		startId = (startId+1) % 4;
	} while (startId != randomValue);

	if (-1 != ret)
	{
		CrushLayer::getInstance()->setStoneState(startId, true);
		EleIcon *(*arrCrushEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();

		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j=0; j<ParamData::CRUSH_COL; j++)
			{
				EleIcon *pEle = arrCrushEle[i][j];
				if (nullptr != pEle && ret == pEle->getEleId())
				{
					pEle->obtainWeak(3);
				}
			}
		}
	}

	return ret;
}


void SpecialAttUtils::cleanStoneState(int eleId)
{
	if (-1 != eleId)
	{		
		CrushLayer::getInstance()->setStoneState(eleId, false);
		EleIcon *(*arrCrushEle)[ParamData::CRUSH_COL] = CrushLayer::getInstance()->getEleIconArr();

		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j=0; j<ParamData::CRUSH_COL; j++)
			{
				EleIcon *pEle = arrCrushEle[i][j];
				if (nullptr != pEle && eleId == pEle->getEleId())
				{
					pEle->doCleanStone();
				}
			}
		}
	}
}

void SpecialAttUtils::createRandomS()
{
	int randomCount = 0;
	do 
	{
		int tmpRow = ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW-1);
		int tmpCol = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL-1);
		EleIcon *pEle = CrushLayer::getInstance()->getEleIcon(tmpRow, tmpCol);

		if (nullptr != pEle && EleIconId_E::ELE_ID_0 <= pEle->getEleId() && pEle->getEleId() < EleIconId_E::ELE_ID_SUPER)
		{
			pEle->changeEle(EleIconId_E::ELE_ID_SUPER, 0);
			break;
		}
		randomCount++;

	} while (randomCount < 20);
}

void SpecialAttUtils::createRandomKey( int round )
{
	int randomCount = 0;
	int tmpRow = ParamData::CRUSH_ROW - 1;// ToolsUtil::getRandomInt(0, ParamData::CRUSH_ROW - 1);
	int tmpCol = ToolsUtil::getRandomInt(0, ParamData::CRUSH_COL - 1);
	do
	{
		EleIcon *pEle = CrushLayer::getInstance()->getEleIcon(tmpRow, tmpCol);

		if (nullptr != pEle && EleIconId_E::ELE_ID_0 <= pEle->getEleId() && pEle->getEleId() < EleIconId_E::ELE_ID_SUPER && nullptr == pEle->getWeakInfo())
		{
			pEle->changeEle(EleIconId_E::ELE_ID_KEY, 0);
			pEle->setTotalRound(round);
			break;
		}
		randomCount++;
		tmpCol = (tmpCol + 1) % ParamData::CRUSH_COL;

	} while (randomCount < ParamData::CRUSH_COL);
}




void SpecialAttUtils::travelseThorn(int row, int col, int arrTag[ParamData::CRUSH_ROW][ParamData::CRUSH_COL], EleIcon *arrEle[ParamData::CRUSH_ROW][ParamData::CRUSH_COL])
{
	if (row < 0 || row >= ParamData::CRUSH_ROW || col < 0 || col >= ParamData::CRUSH_COL)
	{
		return;
	}

	if (0 != arrTag[row][col])
	{
		return;
	}

	EleIcon *pEle = arrEle[row][col];
	if (nullptr != pEle && nullptr != pEle->getSpecInfo()
		&& (EleSpecialType::THORN_EYE == pEle->getSpecInfo()->type || EleSpecialType::THORN == pEle->getSpecInfo()->type))
	{
		arrTag[row][col] = 1;

		SpecialAttUtils::travelseThorn(row+1, col, arrTag, arrEle);
		SpecialAttUtils::travelseThorn(row-1, col, arrTag, arrEle);
		SpecialAttUtils::travelseThorn(row, col+1, arrTag, arrEle);
		SpecialAttUtils::travelseThorn(row, col-1, arrTag, arrEle);
	}
	else if (nullptr != pEle && pEle->getEleId() >= EleIconId_E::ELE_ID_0 && pEle->getEleId() < EleIconId_E::ELE_ID_ROLE_COUNT
		&& nullptr == pEle->getWeakInfo())
	{
		arrTag[row][col] = -1;
	}
	else
	{
		arrTag[row][col] = -2;
	}

	return;
}