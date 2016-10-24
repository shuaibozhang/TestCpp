#include "CrushUtil.h"
#include "JRCommon.h"
#include "EleIcon.h"
#include "Armtr/GameArmtr.h"
#include "ParamMgr.h"
#include "GameLayer.h"
#include "UserData.h"
#include "BoxDataMgr.h"
#include "Scenes/PopRewardLayer.h"
#include "Scenes/MainScene.h"
#include "WeaponControl.h"
#include "SkillControl.h"
#include "Player.h"

bool CrushUtil::travelseEle(int row, int column, TravelseInfo_T *pCrushInfo, bool isCanCrush, int travelType)
{
	bool bRet = isCanCrush;	

	do 
	{
		if (pCrushInfo->arrIsTravelsed[row][column])
		{
			break;
		}

		int eleId = pCrushInfo->arrEleId[row][column];
		pCrushInfo->arrIsTravelsed[row][column] = true;

		if (eleId >= EleIconId_E::ELE_ID_ROLE_COUNT || eleId <= EleIconId_E::ELE_ID_EMPTY)
		{
			break;
		}

		int arrIndex = pCrushInfo->crushArrCount;
		int (* arrEleId)[ParamData::CRUSH_COL] = pCrushInfo->arrEleId;
		int (* arrEleWeakState)[ParamData::CRUSH_COL] = pCrushInfo->arrEleWeakState;
		int (* arrCrushArrIndex)[ParamData::CRUSH_COL] = pCrushInfo->arrCrushArrIndex;
		CrushArr_T (* arrCrushDetail) = pCrushInfo->arrCrushDetail;

		if (0 != arrEleWeakState[row][column])
		{
//			CCASSERT(0 == travelType, "travelseEleEx 0 == travelType Line183");
			/*travelse vertical*/
			if (0 == travelType || 2 == travelType)
			{
				int rowStartIndex = row;
				while ((rowStartIndex-1) >= 0)
				{
					int tmp = rowStartIndex-1;
					if (arrEleId[tmp][column] == eleId 
						&& (-1 == arrCrushArrIndex[tmp][column] 
					|| (-1 != arrCrushArrIndex[tmp][column] && !arrCrushDetail[arrCrushArrIndex[tmp][column]].isFinish)))
					{
						rowStartIndex = tmp;
					}
					else
					{
						break;
					}
				}

				int rowEndIndex = row;
				while (((rowEndIndex+1) < ParamData::CRUSH_ROW))
				{
					int tmp = rowEndIndex+1;
					if (arrEleId[tmp][column] == eleId 
						&& (-1 == arrCrushArrIndex[tmp][column] 
					|| (-1 != arrCrushArrIndex[tmp][column] && !arrCrushDetail[arrCrushArrIndex[tmp][column]].isFinish)))
					{
						rowEndIndex = tmp;
					}
					else
					{
						break;
					}
				}


				if ((rowEndIndex-rowStartIndex) > 1)
				{
					bRet = true;
					arrCrushDetail[arrIndex].crushType = 4;

					for (int i=rowStartIndex; i<=rowEndIndex; i++)
					{
						if (-1 == arrCrushArrIndex[i][column])
						{
							arrCrushArrIndex[i][column] = arrIndex;
							arrCrushDetail[arrIndex].crushCount++;

							if (0 != arrEleWeakState[i][column])
							{
								CrushUtil::travelseEle(i, column, pCrushInfo, true, 1);
							}
						}
					}			
				}
			}

			/*travelse horizontal*/
			if (0 == travelType || 1 == travelType)
			{
				int colStartIndex = column;
				while ((colStartIndex-1) >= 0)
				{
					int tmp = colStartIndex-1;
					if (arrEleId[row][tmp] == eleId 
						&& (-1 == arrCrushArrIndex[row][tmp] 
					|| (-1 != arrCrushArrIndex[row][tmp] && !arrCrushDetail[arrCrushArrIndex[row][tmp]].isFinish)))
					{
						colStartIndex = tmp;
					}
					else
					{
						break;
					}
				}

				int colEndIndex = column;
				while ((colEndIndex+1) < ParamData::CRUSH_COL)
				{
					int tmp = colEndIndex+1;
					if (arrEleId[row][tmp] == eleId 
						&& (-1 == arrCrushArrIndex[row][tmp] 
					|| (-1 != arrCrushArrIndex[row][tmp] && !arrCrushDetail[arrCrushArrIndex[row][tmp]].isFinish)))
					{
						colEndIndex = tmp;
					}
					else
					{
						break;
					}
				}


				if ((colEndIndex-colStartIndex) > 1)
				{
					bRet = true;
					arrCrushDetail[arrIndex].crushType = 4;
					
					for (int i=colStartIndex; i<=colEndIndex; i++)
					{
						if (-1 == arrCrushArrIndex[row][i])
						{
							arrCrushArrIndex[row][i] = arrIndex;
							arrCrushDetail[arrIndex].crushCount++;

							if (0 != arrEleWeakState[row][i])
							{
								CrushUtil::travelseEle(row, i, pCrushInfo, true, 2);
							}
						}
					}			
				}
			}
		}
		else
		{
			bool isHasWeak = false;
			/*travelse vertical*/
			if (0 == travelType || 2 == travelType)
			{
				int rowStartIndex = row;
				while ((rowStartIndex-1) >= 0 && !isHasWeak)
				{
					int tmp = rowStartIndex-1;
					if (arrEleId[tmp][column] == eleId 
						&& (-1 == arrCrushArrIndex[tmp][column] || arrIndex == arrCrushArrIndex[tmp][column]
					|| (arrIndex != arrCrushArrIndex[tmp][column] && !arrCrushDetail[arrCrushArrIndex[tmp][column]].isFinish)))
					{
						rowStartIndex = tmp;

						if (0 != arrEleWeakState[tmp][column])
						{
							isHasWeak = true;
							break;
						}
					}
					else
					{
						break;
					}
				}

				int rowEndIndex = row;
				while (((rowEndIndex+1) < ParamData::CRUSH_ROW) && !isHasWeak)
				{
					int tmp = rowEndIndex+1;
					if (arrEleId[tmp][column] == eleId && (-1 == arrCrushArrIndex[tmp][column] || arrIndex == arrCrushArrIndex[tmp][column]
					|| (arrIndex != arrCrushArrIndex[tmp][column] && !arrCrushDetail[arrCrushArrIndex[tmp][column]].isFinish)))
					{
						rowEndIndex = tmp;

						if (0 != arrEleWeakState[tmp][column])
						{
							isHasWeak = true;
							break;
						}
					}
					else
					{
						break;
					}
				}


				if (!isHasWeak && (rowEndIndex-rowStartIndex) > 1)
				{
					bRet = true;

					int count = rowEndIndex-rowStartIndex+1;
					if (count >= 5)
					{
						arrCrushDetail[arrIndex].crushType = 2;
					}			

					for (int i=rowStartIndex; i<=rowEndIndex; i++)
					{
						CrushUtil::travelseEle(i, column, pCrushInfo, true, 1);
					}			
				}
			}

			isHasWeak = false;
			/*travelse horizontal*/
			if (0 == travelType || 1 == travelType)
			{
				int colStartIndex = column;
				while ((colStartIndex-1) >= 0 && !isHasWeak)
				{
					int tmp = colStartIndex-1;
					if (arrEleId[row][tmp] == eleId && (-1 == arrCrushArrIndex[row][tmp] || arrIndex == arrCrushArrIndex[row][tmp]
					|| (arrIndex != arrCrushArrIndex[row][tmp] && !arrCrushDetail[arrCrushArrIndex[row][tmp]].isFinish)))
					{
						colStartIndex = tmp;
						if (0 != arrEleWeakState[row][tmp])
						{
							isHasWeak = true;
							break;
						}
					}
					else
					{
						break;
					}
				}

				int colEndIndex = column;
				while ((colEndIndex+1) < ParamData::CRUSH_COL && !isHasWeak)
				{
					int tmp = colEndIndex+1;
					if (arrEleId[row][tmp] == eleId && (-1 == arrCrushArrIndex[row][tmp] || arrIndex == arrCrushArrIndex[row][tmp]
					|| (arrIndex != arrCrushArrIndex[row][tmp] && !arrCrushDetail[arrCrushArrIndex[row][tmp]].isFinish)))
					{
						colEndIndex = tmp;
						if (0 != arrEleWeakState[row][tmp])
						{
							isHasWeak = true;
							break;
						}
					}
					else
					{
						break;
					}
				}


				if ((colEndIndex-colStartIndex) > 1 && !isHasWeak)
				{
					bRet = true;

					int count = colEndIndex-colStartIndex+1;
					if (count >= 5)
					{
						arrCrushDetail[arrIndex].crushType = 2;
					}	

					for (int i=colStartIndex; i<=colEndIndex; i++)
					{
						CrushUtil::travelseEle(row, i, pCrushInfo, true, 2);
					}			
				}
			}

			if (bRet)
			{
				if (-1 != arrCrushArrIndex[row][column] && !arrCrushDetail[arrCrushArrIndex[row][column]].isFinish)
				{
					int weakArrIndex = arrCrushArrIndex[row][column];

					arrCrushDetail[weakArrIndex].crushCount--;
//					arrIsLinkWeak[row][column] = false;
				}
				arrCrushDetail[arrIndex].crushCount++;
				arrCrushArrIndex[row][column] = arrIndex;

				if ((pCrushInfo->touchIndex.column == column && pCrushInfo->touchIndex.row == row)
					|| (pCrushInfo->moveIndex.column == column && pCrushInfo->moveIndex.row == row))
				{
					arrCrushDetail[arrIndex].isTouchEle = true;
					arrCrushDetail[arrIndex].desIndex.row = row;
					arrCrushDetail[arrIndex].desIndex.column = column;
				}

				if (!arrCrushDetail[arrIndex].isTouchEle
					&& ((column > arrCrushDetail[arrIndex].desIndex.column) || (column == arrCrushDetail[arrIndex].desIndex.column && row > arrCrushDetail[arrIndex].desIndex.row)))
				{
					arrCrushDetail[arrIndex].desIndex.row = row;
					arrCrushDetail[arrIndex].desIndex.column = column;
				}
			}
		}
		
	} while (false);

	return bRet;
}


int CrushUtil::productEleId(int arrEleCount[4], int row, int col, int *arrForbidInf)
{
#if 1
	int randomValue = ToolsUtil::getRandomInt(0, 1023);
	int eleId = randomValue / 256;

	return eleId;
#else
	int eleId = 0;
	do 
	{
		int randomValue = ToolsUtil::getRandomInt(0, 999);
		eleId = randomValue / 250;

		if (col != arrForbidInf[eleId-EleIconId_E::ELE_ID_SWORD])
		{
			return eleId;
		}
	} while (true);

	return eleId;
#endif
}


void CrushUtil::simulateFall(bool (*arrIsCanFall)[ParamData::CRUSH_COL], CrushIndex_T (*arrEleSrcIndex)[ParamData::CRUSH_COL])
{
	bool isFinish = true;
	int arrNewCount[ParamData::CRUSH_COL] = {0};
	int arrCellState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = {0};	//0:nor 1:reach from top,top right,top left 2:can't reach 3:can;t move
	memset((void *)arrCellState, 0, sizeof(arrCellState));

	for (int i=ParamData::CRUSH_ROW-1; i>=0; i--)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			if (!arrIsCanFall[i][j])
			{
				arrCellState[i][j] = 3;
			}
			else
			{
				if (i < (ParamData::CRUSH_ROW-1))
				{
					if (0 == j)
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j+1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}						
					}
					else if ((ParamData::CRUSH_COL-1) == j)
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j-1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}
						
					}
					else
					{
						if (1<arrCellState[i+1][j] && 1<arrCellState[i+1][j+1] && 1<arrCellState[i+1][j-1])
						{
							arrCellState[i][j] = 2;
						}
						else if (1<arrCellState[i+1][j])
						{
							arrCellState[i][j] = 1;
						}						
					}					
				}				
			}

		}
	}

	do 
	{
		isFinish = true;

		for (int i=0; i<ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				if (-1 == arrEleSrcIndex[i][j].row)
				{	
					/*is empty*/
					if (0 == arrCellState[i][j])
					{
						if ((ParamData::CRUSH_ROW-1) == i)
						{
							arrEleSrcIndex[i][j].row = ParamData::CRUSH_ROW + arrNewCount[j];
							arrEleSrcIndex[i][j].column = j;
							arrNewCount[j]++;

							isFinish = false;
						}
						else if (-1 != arrEleSrcIndex[i+1][j].column)
						{
							/*top*/
							arrEleSrcIndex[i][j] = arrEleSrcIndex[i+1][j];
							arrEleSrcIndex[i+1][j].column = -1;
							arrEleSrcIndex[i+1][j].row = -1;
							isFinish = false;
						}
					}
					else if (1 == arrCellState[i][j] || 2 == arrCellState[i][j])
					{
						if (3 != arrCellState[i+1][j] && -1 != arrEleSrcIndex[i+1][j].column)
						{
							/*top*/
							arrEleSrcIndex[i][j] = arrEleSrcIndex[i+1][j];
							arrEleSrcIndex[i+1][j].column = -1;
							arrEleSrcIndex[i+1][j].row = -1;
							isFinish = false;
						}
						else if (j>0 && -1 != arrEleSrcIndex[i][j-1].row && -1 != arrEleSrcIndex[i+1][j-1].row && 3 != arrCellState[i+1][j-1])
						{
							/*left top*/
							arrEleSrcIndex[i][j] = arrEleSrcIndex[i+1][j-1];
							arrEleSrcIndex[i+1][j-1].column = -1;
							arrEleSrcIndex[i+1][j-1].row = -1;
							isFinish = false;
						}
						else if (j<(ParamData::CRUSH_COL-1) && -1 != arrEleSrcIndex[i][j+1].row && -1 != arrEleSrcIndex[i+1][j+1].row && 3 != arrCellState[i+1][j+1])
						{
							/*right top*/
							arrEleSrcIndex[i][j] = arrEleSrcIndex[i+1][j+1];
							arrEleSrcIndex[i+1][j+1].column = -1;
							arrEleSrcIndex[i+1][j+1].row = -1;
							isFinish = false;
						}
					}
				}				
			}
		}

	} while (!isFinish);
}



void CrushUtil::swapEle(EleIcon *pEle1, EleIcon *pEle2, EleIcon *(*arrEle)[ParamData::CRUSH_COL])
{
	CrushIndex_T pos1 = pEle1->getPosIndex();
	CrushIndex_T pos2 = pEle2->getPosIndex();
	
	arrEle[pos1.row][pos1.column] = pEle2;
	pEle2->setPosIndex(pos1);

	arrEle[pos2.row][pos2.column] = pEle1;
	pEle1->setPosIndex(pos2);
}


void CrushUtil::swapEle(CrushIndex_T pos1, CrushIndex_T pos2, EleIcon *(*arrEle)[ParamData::CRUSH_COL])
{
	CrushUtil::swapEle(arrEle[pos1.row][pos1.column], arrEle[pos2.row][pos2.column], arrEle);
}


Vec2 CrushUtil::getCrushAreaStartPos()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	float startPosY = (ParamData::CRUSH_BG_HEIGHT - ParamData::CRUSH_ROW*ParamData::ELE_BG_HEIGHT) / 2.f;
	Vec2 startPos = Vec2(visibleSize.width/2 - ParamData::CRUSH_COL/2*ParamData::ELE_BG_HEIGHT, startPosY);

	return startPos;
}


Vec2 CrushUtil::getElePos(int row, int column)
{
	Vec2 startPos = CrushUtil::getCrushAreaStartPos();
	float x = startPos.x + (column + 0.5f) * ParamData::ELE_BG_WIDTH;
	float y = startPos.y + (row + 0.5f) * ParamData::ELE_BG_HEIGHT;

	return Vec2(x, y);
}


CrushIndex_T CrushUtil::getCrushIndex(const Vec2 &pos)
{
	CrushIndex_T ret = {0};
	Vec2 startPos = CrushUtil::getCrushAreaStartPos();
	Vec2 endPos = startPos + Vec2(ParamData::ELE_BG_WIDTH*ParamData::CRUSH_COL, ParamData::ELE_BG_HEIGHT*ParamData::CRUSH_ROW);

	if (pos.x >= startPos.x && pos.y >= startPos.y && pos.x < endPos.x && pos.y < endPos.y)
	{
		float dtX = pos.x - startPos.x;
		float dtY = pos.y - startPos.y;

		ret.column = dtX / ParamData::ELE_BG_WIDTH;
		ret.row = dtY / ParamData::ELE_BG_HEIGHT;
	}
	else
	{
		ret.column = -1;
		ret.row = -1;
	}

	return ret;
}

void CrushUtil::setPosIndex(CrushIndex_T & posIndex, int row, int column)
{
	posIndex.row = row;
	posIndex.column = column;
}

bool CrushUtil::isEleCanTouch(EleIcon * pEle)
{
	bool bRet = false;

	if (nullptr != pEle)
	{
		int eleId = pEle->getEleId();

		bRet = (eleId >= EleIconId_E::ELE_ID_0 && eleId <= EleIconId_E::ELE_ID_SUPER);

		if (bRet && nullptr != pEle->getWeakInfo() && !pEle->getWeakInfo()->isCanFall)
		{
			bRet = false;			
		}
		else if (!pEle->getIsCanTouch())
		{
			bRet = false;
		}
	}

	return bRet;
}


Vec2 CrushUtil::getMonsterPos(int index, int count, int waveIndex)
{
	float offsetX = ParamData::POS_X_INTERVAL;
	Vec2 centerPos = CrushUtil::getMonsterCenterPos(waveIndex);// Vec2(490.f + ParamData::POS_X_INTERVAL, 743.f) + Vec2(offsetX*waveIndex, 0.f);
	Vec2 startOffset = Vec2(-114.f, 15.f);
	Vec2 endOffset = Vec2(114.f, -15.f);
	Vec2 offset = (endOffset - startOffset) / (count + 1);

	return centerPos + startOffset + offset*(index + 1);
}

Vec2 CrushUtil::getRoleOffsetPos(int roleId)
{
// 	float layoutWidth = 200.f;
// 	float layoutHeight = 100.f;
	float posX = 0.f;
	float posY = 0.f;

	switch (roleId)
	{
	case 0:
		posX = 70.f;
		posY = -8.f;
		break;
	case 1:
		posX = 9.f;
		posY = -22.f;
		break;
	case 2:
		posX = -29.f;
		posY = 24.f;
		break;
	case 3:
		posX = -74.f;//-70.f;
		posY = -2.f;
		break;
	default:
		break;
	}

	return Vec2(posX, posY);
}


Vec2 CrushUtil::getMonsterCenterPos(int waveIndex)
{
	float offsetX = ParamData::POS_X_INTERVAL;
	Vec2 centerPos = Vec2(ParamData::MONSTER_START_POS_X + ParamData::POS_X_INTERVAL, ParamData::MONSTER_START_POS_Y) + Vec2(offsetX*waveIndex, 0.f);

	return centerPos;
}


Vec2 CrushUtil::getMonsterFrontPos(int waveIndex)
{
	Vec2 centerPos = CrushUtil::getMonsterCenterPos(waveIndex);
	return centerPos - Vec2(80.f, 0.f);
}


Vec2 CrushUtil::getRoleCenterPos(int posId)
{
	Vec2 startPox = Vec2(ParamData::ROLE_START_POS_X, ParamData::ROLE_START_POS_Y);
	Vec2 layoutPos = startPox + Vec2((posId + 1) * ParamData::POS_X_INTERVAL, 0.f);

	return layoutPos;
}


cocos2d::Vec2 CrushUtil::getRolePos(int roleIndex, int posId)
{
	Vec2 centerPos = CrushUtil::getRoleCenterPos(posId);
	Vec2 offsetPos = CrushUtil::getRoleOffsetPos(roleIndex);
	return centerPos+offsetPos;
}

Vec2 CrushUtil::getMonsterAttPos(int posId, int monsterIndex, int waveCount)
{
//	Vec2 rolePos = CrushUtil::getRolePos(0, posId);
	Vec2 defPos = CrushUtil::getRoleDefPos(posId);
	float height = 40.f;
	float offsetY = 0.f;
	
	if (1 != waveCount)
	{
		offsetY = -height / 2 + height / (waveCount - 1) * monsterIndex;
	}

	return defPos + Vec2(60.f, offsetY);
}

Vec2 CrushUtil::getMonsterSkillPos(int posId, int monsterIndex, int waveCount)
{
	Vec2 rolePos = CrushUtil::getRolePos(0, posId);
	return rolePos;// +Vec2(120.f, 0.f);
}

Vec2 CrushUtil::getRoleDefPos(int posId)
{
	Vec2 rolePos = CrushUtil::getRolePos(0, posId);
	return rolePos + Vec2(70.f, 0.f);
}

Vec2 CrushUtil::getShieldPos(int posId)
{
	return getRoleCenterPos(posId);
}


float CrushUtil::getDamageN(int maxLv)
{
	float ret = 0.f;

	if (maxLv < 3)
	{
		ret = 0.1f;
	}
	else if (maxLv < 7)
	{
		ret = 0.05f;
	}
	else if (maxLv < 11)
	{
		ret = 0.02f;
	}
	else if (maxLv < 29)
	{
		ret = 0.009f;
	}
	else if (maxLv < 37)
	{
		ret = 0.006f;
	}
	else if (maxLv < 56)
	{
		ret = 0.003f;
	}
	else
	{
		ret = 0.003f;
	}

	return ret;
}

void CrushUtil::changeWeapon(GameArmtr * pArm, int roleId, int weaponId)
{
	string arrBoneName[ParamData::ROLE_COUNT] = { "Layer28", "Layer32" , "Layer54" , "Layer26", "Layer37" };
	__String *arrWeaponName[ParamData::ROLE_COUNT][10] = { 0 };
	__String *arrDefName[10] = { 0 };
	int arrWeaponStartIndex[ParamData::ROLE_COUNT] = { 500, 510, 520, 530, 540};
	int weaponLv = weaponId - arrWeaponStartIndex[roleId];

	for (int i = 0; i < 10; i++)
	{
		int tmp = i;
		auto strName = __String::createWithFormat("wuqi/lsj_wuqi_%d.png", tmp + 1);
		arrWeaponName[RoleId_E::ROLE_ID_LSJ][i] = strName;

		strName = __String::createWithFormat("wuqi/cbd_wuqi_%d.png", tmp + 1);
		arrWeaponName[RoleId_E::ROLE_ID_CBD][i] = strName;

		strName = __String::createWithFormat("wuqi/cbd_dun_%d.png", tmp + 1);
		arrDefName[i] = strName;
		
		strName = __String::createWithFormat("wuqi/lqc_wuqi_%d.png", tmp + 1);
		arrWeaponName[RoleId_E::ROLE_ID_LQC][i] = strName;
		
		strName = __String::createWithFormat("wuqi/sqy_wuqi_%d.png", tmp + 1);
		arrWeaponName[RoleId_E::ROLE_ID_SQY][i] = strName;

		strName = __String::createWithFormat("wuqi/qyl_wuqi_%d.png", tmp + 1);
		arrWeaponName[RoleId_E::ROLE_ID_QYL][i] = strName;
	}
	
	
	Skin *skin = Skin::createWithSpriteFrameName(arrWeaponName[roleId][weaponLv]->getCString());
	pArm->getArmtr()->getBone(arrBoneName[roleId])->addDisplay(skin, 0);
	pArm->getArmtr()->getBone(arrBoneName[roleId])->changeDisplayWithIndex(0, true);

	if (RoleId_E::ROLE_ID_CBD == roleId)
	{
		skin = Skin::createWithSpriteFrameName(arrDefName[weaponLv]->getCString());
		pArm->getArmtr()->getBone("Layer30")->addDisplay(skin, 0);
		pArm->getArmtr()->getBone("Layer30")->changeDisplayWithIndex(0, true);
	}
}

void CrushUtil::loadSkillImage(int skillId)
{
	auto pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(skillId);

	if (pSkillInfo->skillAnim != "")
	{
//		__String *strCfgName = __String::createWithFormat("skills/%s/%s.ExportJson", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str());
//		ArmatureDataManager::getInstance()->addArmatureFileInfo(strCfgName->getCString());
		
		int index = 0;
		do 
		{
//			__String *strTxtName = __String::createWithFormat("skills/%s/%s%d.png", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);
			__String *strTxtName = __String::createWithFormat("skills/%s/%s%d.pvr.ccz", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);

			if (FileUtils::getInstance()->isFileExist(strTxtName->getCString()))
			{
				__String *strBaseName = __String::createWithFormat("skills/%s/%s%d", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);
				LoadResInfo_T resInfo;
				resInfo.resType = LoadResType::ARMTR;
				resInfo.filePath = strBaseName->getCString();

				Director::getInstance()->getTextureCache()->addImageAsync(strTxtName->getCString(), CC_CALLBACK_1(GameLayer::loadImageCallback, GameLayer::getInstance(), resInfo, 0, skillId));

				index++;
			}
			else
			{
				break;
			}

		} while (true);
	}
}

void CrushUtil::releaseSkillImage(int skillId)
{
	auto pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(skillId);

	if (pSkillInfo->skillAnim != "")
	{
		__String *strCfgName = __String::createWithFormat("skills/%s/%s.ExportJson", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str());
		ArmatureDataManager::getInstance()->removeArmatureFileInfo(strCfgName->getCString());

		int index = 0;
		do
		{
//			__String *strTxtName = __String::createWithFormat("skills/%s/%s%d.pvr.ccz", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);
//			__String *strTxtName = __String::createWithFormat("skills/%s/%s%d.png", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);
			__String *strPlistName = __String::createWithFormat("skills/%s/%s%d.plist", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);

			if (FileUtils::getInstance()->isFileExist(strPlistName->getCString()))
			{
				SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(strPlistName->getCString());

				index++;
			}
			else
			{
				break;
			}

		} while (true);
	}
}

float CrushUtil::getMonsterScale(int monsterId)
{
	float ret = 0.2f;

	switch (monsterId)
	{
	case 12:
	case 33:
	case 34:
	case 50:
	case 51:
	case 52:
		ret = 0.4f;
		break;
	case 23:
	case 41:
	case 63:
		ret = 0.45f;
		break;
	case 60:
	case 61:
		ret = 0.5f;
		break;
	default:
		ret = 0.2f;
		break;
	}

	return ret;
}

float CrushUtil::getMonsterAttValue(int monsterLv, int roleLv, float attValue, int monsterType, bool isGrow)
{
	float ret = attValue;

	if (roleLv > monsterLv && isGrow)
	{
		if (1 == monsterType)
		{
			ret = attValue + (roleLv - monsterLv) * 30;
		}
		else
		{
			ret = attValue + (roleLv - monsterLv) * 10;
		}
	}

	return ret;
}

float CrushUtil::getMonsterHpValue(int monsterLv, int roleLv, float hpValue, int monsterType, bool isGrow)
{
	float ret = hpValue;

	if (roleLv > monsterLv && isGrow)
	{
		if (1 == monsterType)
		{
			ret = hpValue + (roleLv - monsterLv) * 400;
		}
		else
		{
			ret = hpValue + (roleLv - monsterLv) * 30;
		}
	}

	return ret;
}

float CrushUtil::getWeakDamageValue(int weakId, int roleLv, float damage, bool isGrow)
{
	float ret = 0.f;

	switch (weakId)
	{
	case 0:
		//fire
		ret = roleLv * 4.f / 5 + 10.f;
		break;
	case 1:
		//boom
		ret = roleLv * 3 + 50.f;
		break;
	case 2:
		//thunder
		ret = roleLv * 6.f / 5 + 10.f;
		break;
	case 3:
		//add hp
		ret = roleLv * 10 + 30.f;
		break;
	}

	return ret;
}

int CrushUtil::getMonsterExpValue(int monsterLv, int roleLv, int exp, bool isGrow)
{
	if (roleLv > monsterLv && isGrow)
	{
		exp += ((roleLv - monsterLv) * 20);
	}
	return exp;
}

int CrushUtil::getMonsterGoldValue(int monsterLv, int roleLv, int gold, bool isGrow)
{
	if (roleLv > monsterLv && isGrow)
	{
		gold += ((roleLv - monsterLv) * 5);
	}

	return gold;
}

int CrushUtil::calcCrushWeight(const CrushArr_T * pCrushInfo, bool isSwap)
{
	int comboWeight[5] = { 2, 5, 8, 14, 2 };
	int swapWeight[5] = {2, 5, 8, 14, 2};
	float comboTimes = 5.f;//7.5f;
	float swapTimes = 1.f;
	int *arrValue = nullptr;
	int ret = 0;

	for (int i = 0; i < 5; i++)
	{
		comboWeight[i] *= comboTimes;
		swapWeight[i] *= swapTimes;
	}

	if (isSwap)
	{
		arrValue = swapWeight;
	}
	else
	{
		arrValue = comboWeight;
	}

	if (0 == pCrushInfo->crushType)
	{
		ret = arrValue[0];
	}
	else if (1 == pCrushInfo->crushType)
	{
		if (pCrushInfo->crushCount == 4)
		{
			ret = arrValue[1];
		}
		else
		{
			ret = arrValue[2];
		}
	}
	else if (2 == pCrushInfo->crushType)
	{
		ret = arrValue[3];
	}
	else if (4 == pCrushInfo->crushType)
	{
		ret = arrValue[4];
	}

	return ret;
}

void CrushUtil::initTravelseInfo(TravelseInfo_T * pInfo)
{
	memset(pInfo, 0, sizeof(TravelseInfo_T));

	pInfo->touchIndex.row = -1;
	pInfo->touchIndex.column = -1;
	pInfo->moveIndex.row = -1;
	pInfo->moveIndex.column = -1;

	memset((void *)pInfo->arrCrushArrIndex, -1, sizeof(pInfo->arrCrushArrIndex));

}

void CrushUtil::copyTravelseInfo(TravelseInfo_T * pDesInfo, TravelseInfo_T * pSrcInfo)
{
	memcpy(pDesInfo->arrEleId, pSrcInfo->arrEleId, sizeof(pDesInfo->arrEleId));
	memcpy(pDesInfo->arrEleWeakState, pSrcInfo->arrEleWeakState, sizeof(pDesInfo->arrEleWeakState));
}

int CrushUtil::calcEleLayoutWeight(int row, int col, int(*arrEleId)[ParamData::CRUSH_COL])
{
	int ret = 0;
	if (row > 3 && arrEleId[row][col] == arrEleId[row-1][col] && arrEleId[row - 1][col] == arrEleId[row - 3][col] && arrEleId[row - 3][col] == arrEleId[row - 4][col])
	{
		//|
		ret += 5;
	}

	if (col < 4 && arrEleId[row][col] == arrEleId[row][col+1] && arrEleId[row][col+1] == arrEleId[row][col+3] && arrEleId[row][col+3] == arrEleId[row][col+4])
	{
		//-
		ret += 5;
	}

	if (row > 1)
	{
		//7
		if (col > 1 && arrEleId[row][col-1] == arrEleId[row][col - 2] && arrEleId[row][col-2] == arrEleId[row-1][col] && arrEleId[row-1][col] == arrEleId[row-2][col])
		{
			ret += 4;
		}

		//F
		if (col < 6 && arrEleId[row][col + 1] == arrEleId[row][col + 2] && arrEleId[row][col + 2] == arrEleId[row - 1][col] && arrEleId[row - 1][col] == arrEleId[row - 2][col])
		{
			ret += 4;
		}
	}

	if (row < 5)
	{
		//_|
		if (col > 1 && arrEleId[row][col - 1] == arrEleId[row][col - 2] && arrEleId[row][col - 2] == arrEleId[row + 1][col] && arrEleId[row + 1][col] == arrEleId[row + 2][col])
		{
			ret += 4;
		}

		//L
		if (col < 6 && arrEleId[row][col + 1] == arrEleId[row][col + 2] && arrEleId[row][col + 2] == arrEleId[row + 1][col] && arrEleId[row + 1][col] == arrEleId[row + 2][col])
		{
			ret += 4;
		}
	}

	return ret;
}

void CrushUtil::addPopItem(std::vector<PopItemInfo_T>& arrReward, int itemId, int itemCount)
{
	PopItemInfo_T tmp = {0};
	tmp.itemId = itemId;
	tmp.itemCount = itemCount;

	arrReward.push_back(tmp);
}

void CrushUtil::getSettleReward(int rankIdx, std::vector<PopItemInfo_T>& arrReward)
{
	int boxLv = 0;// BoxDataMgr::getInstance()->getCurGetBoxLv();
	int times = 1;

	if (1 == rankIdx)
	{
		times = 4;
		BoxDataMgr::getInstance()->getBoxReward(6, boxLv, arrReward);
	}
	else if (2 == rankIdx)
	{
		times = 3;
		BoxDataMgr::getInstance()->getBoxReward(6, boxLv, arrReward);
	}
	else if (3 == rankIdx)
	{
		times = 2;
		BoxDataMgr::getInstance()->getBoxReward(6, boxLv, arrReward);
	}
	else if (rankIdx <= 20)
	{
		times = 3;
		BoxDataMgr::getInstance()->getBoxReward(5, boxLv, arrReward);
	}
	else if (rankIdx <= 50)
	{
		times = 2;
		BoxDataMgr::getInstance()->getBoxReward(5, boxLv, arrReward);
	}
	else if (rankIdx <= 100)
	{
		times = 1;
		BoxDataMgr::getInstance()->getBoxReward(5, boxLv, arrReward);
	}
	else if (rankIdx <= 200)
	{
		times = 1;
		BoxDataMgr::getInstance()->getBoxReward(4, boxLv, arrReward);
	}
	else if (rankIdx <= 500)
	{
		times = 1;
		BoxDataMgr::getInstance()->getBoxReward(3, boxLv, arrReward);
	}
	else if (rankIdx <= 1000)
	{
		times = 1;
		BoxDataMgr::getInstance()->getBoxReward(2, boxLv, arrReward);
	}

	for (int i = 0; i < arrReward.size(); i++)
	{
		arrReward.at(i).itemCount *= times;
	}
}

void CrushUtil::getExchangeReward(int codeId, std::vector<PopItemInfo_T>& arrReward)
{
	int *arrItemCount = nullptr;
	int *arrItemId = nullptr;
	int arrSize = 0;

	int arrCount[][5] = { { 5, 1, 1, 1, 1 } ,
	{ 20, 3, 3, 3, 10 },
	{ 20, 3, 3, 3, 5 },
	{ 100 },
	{10, 3, 2},
	{ 10, 3, 2 },
	{ 5, 3, 3 },
	{ 5, 3, 3 }
	};
	int arrId[][5] = { { -102, 1014, 1000, 1001, 1015 },
	{ -102, 1014, 1004, 1015, 1513 },
	{ -102, 1014, 1011, 1015, 1536 },
	{ -101 },
	{-101, 1004, 1011},
	{ -101, 1004, 1011 },
	{ -102, 1014, 1015 },
	{ -102, 1004, 1011 }
	};

	arrItemCount = &arrCount[codeId - 1][0];
	arrItemId = &arrId[codeId - 1][0];

	if (ExchangeType_E::EXCHANGE_LV_1 == codeId)
	{
		arrSize = 5;
	}
	else if (ExchangeType_E::EXCHANGE_LV_2 == codeId)
	{
		arrSize = 5;
	}
	else if (ExchangeType_E::EXCHANGE_LV_3 == codeId)
	{
		arrSize = 5;
	}
	else if (ExchangeType_E::EXCHANGE_DIAMOND == codeId)
	{
		arrSize = 1;
	}
	else if (ExchangeType_E::EXCHANGE_MAGPIE == codeId)
	{
		arrSize = 3;
	}
	else if (ExchangeType_E::EXCHANGE_NEW_VER == codeId)
	{
		arrSize = 3;
	}
	else if (ExchangeType_E::EXCHANGE_FOLLOW == codeId)
	{
		arrSize = 3;
	}
	else if (ExchangeType_E::EXCHANGE_GUOQING == codeId)
	{
		arrSize = 3;
	}

	for (int i = 0; i < arrSize; i++)
	{
		PopItemInfo_T itemInfo = { 0 };
		itemInfo.itemCount = arrItemCount[i];
		itemInfo.itemId = arrItemId[i];
		itemInfo.itemLv = 0;

		arrReward.push_back(itemInfo);
	}
}

int CrushUtil::getEndlessBaseLv(int roleLv)
{
	int ret = roleLv;

	if (ret <= 5)
	{
	}
	else if (ret <= 11)
	{
		ret += (ret - 7);
	}
	else if (ret <= 17)
	{
		ret += ((ret - 7) * 3 / 2);
	}
	else if (ret <= 25)
	{
		ret += ((ret - 7) * 2);
	}
	else
	{
		ret += ((ret - 7) * 5 / 2);
	}

	return ret;
}

int CrushUtil::getEndlessAddLv(int floorIdx)
{
	return 0;
}

int CrushUtil::getEliteAddLv(int sceneId)
{
	int ret = 0;

	switch (sceneId)
	{
	case 10:
		ret = 8;
		break;
	case 19:
		ret = 12;
		break;
	case 32:
		ret = 16;
		break;
	case 41:
		ret = 20;
		break;
	case 48:
		ret = 25;
		break;
	case 58:
		ret = 30;
		break;
	case 61:
		ret = 35;
		break;
	case 64:
		ret = 38;
		break;
	case 68:
		ret = 45;
		break;
	case 77:
		ret = 50;
		break;
	case 80:
		ret = 60;
		break;
	default:
		CCASSERT(false, "getEliteAddLv");
		break;
	}

	return ret;
}

bool CrushUtil::isNeedSkillBg(int skillId)
{
	if (6 == skillId || 7 == skillId || 15 == skillId || 28 == skillId || 31 == skillId)
	{
		return true;
	}
	return false;
}

void CrushUtil::giveUserItem(int itemId, int itemCount, int itemLv, bool isNeedSave)
{
	if (ParamData::GOLD_ITEM_ID == itemId)
	{
		UserData::getInstance()->giveGold(itemCount);
	}
	else if (ParamData::CRYSTAL_ITEM_ID == itemId)
	{
		UserData::getInstance()->giveCrystal(itemCount);
	}
	else if (ParamData::TILI_ITEM_ID == itemId)
	{
		UserData::getInstance()->giveTili(itemCount);
	}
	else if (itemId >= PopRewardBoxId::POP_BOX_0 && itemId <= PopRewardBoxId::POP_BOX_3)
	{
		int idx = itemId - PopRewardBoxId::POP_BOX_0;
		UserData::getInstance()->addBoxConfig(idx, itemLv);
	}
	else
	{
		int itemType = BagItemControl::getInstace()->checkItemType(itemId);
		if (itemType == 3)
		{
			UserData::getInstance()->addWeaponPartsNum(itemId, itemCount);
			MainLayer::getCurMainLayer()->updataEquipLayer((itemId - 1500) / 10);
		}
		else if (itemType == 1)
		{
			UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_skill_%d", itemId)->getCString(), 1);
			//BagItemControl::getInstace()->addBagItem(itemId);
			SkillControl::getInstance()->addNewSkill(itemId);
		}
		else if (itemType == 2)
		{
			UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_weapon_%d", itemId)->getCString(), 1);
			WeaponControl::getInstance()->addNewWeapon(itemId);
		}
		else if (itemType == 0)
		{
			for (int j = 0; j < itemCount; j++)
			{
				BagItemControl::getInstace()->addBagItem(itemId);
			}
		}
	}

	if (isNeedSave)
	{
		BagItemControl::getInstace()->saveItemConfig();
		//UserData::getInstance()->saveUserData();
	}
}

string CrushUtil::getItemPicName(int itemId)
{
	string picName = "";
	const char* arrBoxBtnPicName[7] = { "boxlayer_boxicon_0.png", "boxlayer_boxicon_1.png", "boxlayer_boxicon_2.png", "boxlayer_boxicon_3.png", "store_boxicon_jxbx.png", "store_boxicon_sqbx.png", "store_boxicon_cqbx.png" };

	if (ParamData::GOLD_ITEM_ID == itemId)
	{
		picName = "com_goldbag.png";
	}
	else if (ParamData::CRYSTAL_ITEM_ID == itemId)
	{
		picName = "com_crystal.png";
	}
	else if (ParamData::TILI_ITEM_ID == itemId)
	{
		picName = "com_icon_jingqi.png";
	}
	else if (PopRewardBoxId::POP_BOX_0 == itemId)
	{
		picName = arrBoxBtnPicName[0];
	}
	else if (PopRewardBoxId::POP_BOX_1 == itemId)
	{
		picName = arrBoxBtnPicName[1];
	}
	else if (PopRewardBoxId::POP_BOX_2 == itemId)
	{
		picName = arrBoxBtnPicName[2];
	}
	else if (PopRewardBoxId::POP_BOX_3 == itemId)
	{
		picName = arrBoxBtnPicName[3];
	}
	else
	{
		ComInfo_T item;
		ParamMgr::getInstance()->getComGameItemInfo(itemId, item);
		picName = item.picname;
	}

	return picName;
}

int CrushUtil::getDungeonReward(int dungeonType, int lv, std::vector<PopItemInfo_T>& arrReward)
{
	int ret = -1;
	auto pReward = ParamMgr::getInstance()->getDungeonReward(lv);

	if (0 == dungeonType)
	{
		CrushUtil::addPopItem(arrReward, -100, pReward->gold);
	}
	else if (1 == dungeonType)
	{
		ret = pReward->exp;
	}
	else if (2 == dungeonType)
	{
		int itemId = 1500 + ToolsUtil::getRandomInt(0, ParamData::ROLE_COUNT-1) * 10 + pReward->equipLv - 1;
		CrushUtil::addPopItem(arrReward, itemId, pReward->equip);
	}

	return ret;
}

__String * CrushUtil::getEleAnimName(int eleId, int eleLv, bool isStone, int animId)
{
	int animEleId = EleIconId_E::ELE_ID_SUPER;
	__String *strName = nullptr;

	if (eleId < EleIconId_E::ELE_ID_ROLE_COUNT)
	{
		animEleId = Player::getInstance()->getRoleIdByPosIndex(eleId);
		if (animEleId >= EleIconId_E::ELE_ID_SUPER)
		{
			animEleId++;
		}
	}

	if (0 == animId)
	{
		/*idle*/
		if (isStone)
		{
			strName = String::createWithFormat("shihua_%d", animEleId);
		}
		else
		{
			strName = String::createWithFormat("idle_%d_%d", animEleId, eleLv + 1);
		}
	}
	else if (1 == animId)
	{
		/*miss*/
		if (isStone)
		{
			strName = String::createWithFormat("shihua_miss_%d", animEleId);
		}
		else
		{
			strName = String::createWithFormat("miss_%d_%d", animEleId, eleLv + 1);
		}
	}
	else if (2 == animId)
	{
		/*touch*/
		if (isStone)
		{
			strName = String::createWithFormat("shihua_touch_%d", animEleId);
		}
		else
		{
			strName = String::createWithFormat("touch_%d_%d", animEleId, eleLv + 1);
		}
	}

	return strName;
}
