#include "ParamMgr.h"
#include "ParamData.h"
#include "json/rapidjson.h" 
#include "json/document.h" 
#include "SkillControl.h"
#include "GLCommon/Utils/ResMgr.h"
#include "GLCommon/JRCommon.h"
#include "UserData.h"

USING_NS_CC;
using namespace rapidjson;

ParamMgr *ParamMgr::s_pInstance = nullptr;


ParamMgr::ParamMgr()
	:_showReliveWeight(0)
{

}

ParamMgr::~ParamMgr()
{
	/*you'd better delete these point*/
// 	for (auto it=_arrRoleArmtrInfo.begin(); it!=_arrRoleArmtrInfo.end(); )
// 	{
// 		delete (*it);
// 		it = _arrRoleArmtrInfo.erase(it);
// 	}
}


ParamMgr *ParamMgr::getInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new ParamMgr();
// 		s_pInstance->loadItemsConfig();
// 		s_pInstance->loadSkillsConfig();
// 		s_pInstance->loadWeaponConfig();
// 		s_pInstance->loadPlayerInfo();
// 		s_pInstance->loadWAndSkill();

//		s_pInstance->_skillstartid = s_pInstance->getSkillVector().at(0).id;
//		s_pInstance->_itemstartid = s_pInstance->getItemVector().at(0).id;
//		s_pInstance->_weaponstartid = s_pInstance->getWeaponVector().at(0).id;
	}

	return s_pInstance;
}


void ParamMgr::loadRoleArmtrInfo()
{
	auto info = new DragonBoneInfo_T();
	info->strPathName = "roles/lsj";
	info->strDragonBoneName = "lsj";
	info->strArmtrName = "lsj";
	_arrRoleArmtrInfo.push_back(info);

	info = new DragonBoneInfo_T();
	info->strPathName = "roles/cbd";
	info->strDragonBoneName = "cbd";
	info->strArmtrName = "cbd";
	_arrRoleArmtrInfo.push_back(info);

	info = new DragonBoneInfo_T();
	info->strPathName = "roles/lqc";
	info->strDragonBoneName = "lqc";
	info->strArmtrName = "lqc";
	_arrRoleArmtrInfo.push_back(info);

	info = new DragonBoneInfo_T();
	info->strPathName = "roles/sqy";
	info->strDragonBoneName = "sqy";
	info->strArmtrName = "sqy";
	_arrRoleArmtrInfo.push_back(info);
}


// void ParamMgr::loadMonsterArmtrInfo()
// {
// 	auto info = new DragonBoneInfo_T();
// 	info->strPathName = "monsters/beetle";
// 	info->strDragonBoneName = "beetle";
// 	info->strArmtrName = "beetle";
// 	_arrMonsterArmtrInfo.push_back(info);
// }
// 
// 
// void ParamMgr::loadSkillArmtrInfo()
// {
// 
// }


void ParamMgr::loadWeakInfo()
{
	EleWeakInfo_T arrInfo[4] = {{0, 0, 0, 1, EleWeakType::FIRE, true, true, true, true, 5, "fire"},
	{1, 0, 1, 1, EleWeakType::ICE, true, false, false, false, 1, "ice_state"},
	{2, 0, 2, 2, EleWeakType::ICE, true, false, false, false, 1, "ice_state"},
	{3, 101, 3, 1, EleWeakType::STONE, false, true, false, true, 0, ""}};

	for (int i = 0; i < 4; i++)
	{
		auto info = new EleWeakInfo_T();
		*info = arrInfo[i];
		_arrWeakInfo.push_back(info);
	}
}


void ParamMgr::loadEleSpecInfo()
{
	EleSpecialInfo_T arrInfo[] = {
		{4, 4, 3, 0, EleSpecialType::BOOM, false, false, false, 1, 0, -1, -1, 20.f, 1, "boom"},
		{5, 5, 3, 0, EleSpecialType::VOLCANO, false, false, true, 1, 1, 0, 8, 0, 1, "volcano"},
		{6, -1, -1, 2, EleSpecialType::THUNDER, true, true, false, 0, 0, -1, -1, 20.f, 1, "thunder"},
		{7, -1, -1, 1, EleSpecialType::STONE, true, true, false, 2, 1, 3, -1, 0, 1, "stone"},
		{8, 5, 3, 2, EleSpecialType::THORN_EYE, false, false, true, 0, 3, 10, 1, 0, 1, "thorn_eye"},
		{9, -1, 1, 0, EleSpecialType::THORN, false, false, false, -1, -1, -1, -1, 0, 1, "thorn"},
		{10, 4, 3, 0, EleSpecialType::ICE, false, false, false, 1, 1, 1, 8, 0, 1, "ice"},
		{11, -1, -1, 2, EleSpecialType::RECOVER, true, true, false, 0, 4, -1, -1, 20.f, 1, "recover"},
		{12, -1, -1, 201, EleSpecialType::KEY, true, true, false, 3, 5, 100, -1, 0, 1, "key"},
		{13, -1, 1, 201, EleSpecialType::SLATE, true, false, false, -1, -1, -1, -1, 0, 1, "slate" },
		{ 14, -1, 1, 201, EleSpecialType::SLATE, false, false, false, -1, -1, -1, -1, 0, 1, "slate" },
	};
	int totalCount = sizeof(arrInfo) / sizeof(arrInfo[0]);

	for (int i = 0; i < totalCount; i++)
	{
		auto info = new EleSpecialInfo_T();
		*info = arrInfo[i];
		_arrEleSpecInfo.push_back(info);
	}
}


void ParamMgr::loadMonsterInfo()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/monster.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("Mons_%d", i)->getCString()];

		auto info = new MonsterInfo_T();
		info->id = item["id"].GetDouble();
		info->type = item["typeid"].GetDouble();
		info->lv = item["rank"].GetDouble();
//		info->animId = item["animid"].GetDouble();
		info->attRange[0] = item["minatt"].GetDouble();
		info->attRange[1] = item["maxatt"].GetDouble();
		info->hp = item["hp"].GetDouble();
		info->exp = item["exper"].GetDouble();
		info->runPro = item["runpro"].GetDouble();
		info->goldRange[0] = item["mingold"].GetDouble();
		info->goldRange[1] = item["maxgold"].GetDouble();
		info->animName = item["aniname"].GetString();
		info->name = item["name"].GetString();
		info->fScale = item["zoom"].GetDouble();

		const rapidjson::Value& arry = item["dropnums"];
		for (int i = 0; i < arry.Capacity();  i++)
		{
			const rapidjson::Value& tempinfo = arry[i];
			DropItem_T dropItem = { 0 };
			dropItem.itemId = tempinfo["id"].GetDouble();
			dropItem.pro = tempinfo["pro"].GetDouble();

			info->arrDropInfo.push_back(dropItem);
		}	

		const rapidjson::Value& arry2 = item["def"];
		for (int i = 0; i < arry2.Capacity(); i++)
		{
			DefAttrbt_T defInfo = { 0 };
			const rapidjson::Value& tempinfo = arry2[i];
			defInfo.defId = tempinfo["id"].GetDouble();
			defInfo.hurt = tempinfo["hurt"].GetDouble();
			info->arrDefInfo.push_back(defInfo);
		}

		const rapidjson::Value& arry3 = item["attlogic"];
		for (int i = 0; i < arry3.Capacity(); i++)
		{
			const rapidjson::Value& tempinfo = arry3[i];
			MonsterAttInfo_T attInfo = { 0 };
			if (tempinfo.HasMember("next"))
			{
				attInfo.next = tempinfo["next"].GetDouble();
			}
			else
			{
				attInfo.next = -1;
			}
			
			attInfo.round = tempinfo["round"].GetDouble(); ;
			attInfo.attCount = tempinfo["attNum"].GetInt();//1;
			attInfo.attId = tempinfo["attid"][0].GetDouble();
			info->arrAttInfo.push_back(attInfo);
		}

		_arrMonsterInfo.push_back(info);
	}
}

void ParamMgr::loadMonsterAttArmInfo()
{
#if 1
	rapidjson::Document _jsonDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/monster_kill.json");
	_jsonDoc.Parse<0>(contentStr.c_str());
	if (_jsonDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonDoc.GetParseError());
	}
	auto count = _jsonDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonDoc[String::createWithFormat("%d", i)->getCString()];
		std::map<int, MonsterAttArmInfo_T> attMap;

		const rapidjson::Value& arrAttInfo = item["info"];
		for (int j = 0; j < arrAttInfo.Capacity(); j++)
		{
			const rapidjson::Value& tempinfo = arrAttInfo[j];

			int attId = tempinfo["act_id"].GetInt();
			MonsterAttArmInfo_T infoItem;
			infoItem.roleAct = tempinfo["role_act"].GetString();
			infoItem.buffAnim = tempinfo["buff_act"].GetString();
			infoItem.skillAnim = tempinfo["skill_act"].GetString();
			infoItem.soundName = tempinfo["music_id"].GetString();
			/*
			if (0 != infoItem.soundName.compare(""))
			{
				auto posIdx = infoItem.soundName.find(".ogg");
				infoItem.soundName.replace(posIdx, 4, ".mp3");
			}*/
			
			attMap.insert(make_pair(attId, infoItem));
		}

		_arrMonsterAttArmInfo.push_back(attMap);
	}
#else
	MonsterAttArmInfo_T tmp;
	tmp.skillAnim = "m_skill_0";
	tmp.buffAnim = "";

	std::map<int, MonsterAttArmInfo_T> tmpMap;
	//	tmpMap.insert(make_pair(0, tmp));
	tmpMap.insert(make_pair(1, tmp));
	tmpMap.insert(make_pair(101, tmp));

	_arrMonsterAttArmInfo.push_back(tmpMap);
#endif
}

void ParamMgr::loadStageInfo()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/stage.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.Capacity();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[i];
		rapidjson::Value& arry = item["monsArray"];
		rapidjson::Value& arryName = item["stageName"];

		auto info = new StageInfo_T();
		info->id = item["id"].GetDouble();
		CCLOG("%d", info->id);
		for (int a = 0; a < arry.Capacity(); a++)
		{
			std::vector<std::vector<int>> vectorb;
			rapidjson::Value& arrya = arry[a];
			for (int b = 0; b < arrya.Capacity(); b++)
			{
				std::vector<int> vectorc;
				rapidjson::Value& arryb = arrya[b];
				for (int c = 0; c < arryb.Capacity(); c++)
				{
					vectorc.push_back(arryb[c].GetDouble());
				}
				vectorb.push_back(vectorc);
			}
			info->arrMonsterWaveInfo.push_back(vectorb);
		}
		info->rewardexp = item["award_exp"].GetDouble();
		info->rewardgold = item["award_gold"].GetDouble();
		std::string strDesc = item["stage_des"].GetString();
		ToolsUtil::stringFindReplace(strDesc, "\\n", "\n");
		info->stageDesc = strDesc;// item["stage_des"].GetString();
		info->energyCost = item["need_biaoqi"].GetInt();
		info->stageType = item["style"].GetInt();
		info->boxmonsId = item["boxmons_monsid"].GetInt();
		info->boxmonsBoxid = item["boxmons_id"].GetInt();

		auto& arrboxids = item["boxid"];
		auto& arrboxp = item["boxpro"];
		int boxnnum = arrboxids.Capacity();
		for (int i = 0; i < ParamData::BOX_COUNT; i++)
		{
			info->arrBoxChance[i] = 0.f;
		}
		for (int i = 0; i < boxnnum; i++)
		{
			int boxcurid = arrboxids[i].GetInt();
			float p = arrboxp[i].GetDouble();
			info->arrBoxChance[boxcurid] = p;
		}
		
		for (int b = 0; b < arryName.Capacity(); b++)
		{
			info->arrFloorName.push_back(arryName[b].GetString());			;
		}

		CCLOG("idx:%d", i);
		_arrStageInfo.push_back(info);
	}
	CCLOG("load state OK!");


	_pUnStopStageInfo = nullptr;// new StageInfo_T();
	contentStr = FileUtils::getInstance()->getStringFromFile("json/stage_nostop.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
//	auto count = _jsonBezierDoc.Capacity();
//	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[0];
		rapidjson::Value& arry = item["monsArray"];
		rapidjson::Value& arryName = item["stageName"];

		auto info = new StageInfo_T();
		info->id = item["id"].GetDouble();
		CCLOG("%d", info->id);
		for (int a = 0; a < arry.Capacity(); a++)
		{
			std::vector<std::vector<int>> vectorb;
			rapidjson::Value& arrya = arry[a];
			for (int b = 0; b < arrya.Capacity(); b++)
			{
				std::vector<int> vectorc;
				rapidjson::Value& arryb = arrya[b];
				for (int c = 0; c < arryb.Capacity(); c++)
				{
					vectorc.push_back(arryb[c].GetDouble());
				}
				vectorb.push_back(vectorc);
			}
			info->arrMonsterWaveInfo.push_back(vectorb);
		}
		info->rewardexp = item["award_exp"].GetDouble();
		info->rewardgold = item["award_gold"].GetDouble();

		for (int b = 0; b < arryName.Capacity(); b++)
		{
			info->arrFloorName.push_back(arryName[b].GetString()); ;
		}
		info->boxmonsId = -1;
		info->boxmonsBoxid = -1;

		_pUnStopStageInfo = info;
	}
	CCLOG("load state OK!");

	_pInstanceStageInfo = nullptr;// new StageInfo_T();
	contentStr = FileUtils::getInstance()->getStringFromFile("json/instance_stage.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	//	auto count = _jsonBezierDoc.Capacity();
	//	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[0];
		rapidjson::Value& arry = item["monsArray"];
//		rapidjson::Value& arryName = item["stageName"];

		auto info = new StageInfo_T();
		info->id = item["id"].GetDouble();
		CCLOG("%d", info->id);
		for (int a = 0; a < arry.Capacity(); a++)
		{
			std::vector<std::vector<int>> vectorb;
			rapidjson::Value& arrya = arry[a];
			for (int b = 0; b < arrya.Capacity(); b++)
			{
				std::vector<int> vectorc;
				rapidjson::Value& arryb = arrya[b];
				for (int c = 0; c < arryb.Capacity(); c++)
				{
					vectorc.push_back(arryb[c].GetDouble());
				}
				vectorb.push_back(vectorc);
			}
			info->arrMonsterWaveInfo.push_back(vectorb);
		}
		info->rewardexp = 0;
		info->rewardgold = 0;

// 		for (int b = 0; b < arryName.Capacity(); b++)
// 		{
// 			info->arrFloorName.push_back(arryName[b].GetString()); ;
// 		}
		info->boxmonsId = -1;
		info->boxmonsBoxid = -1;

		_pInstanceStageInfo = info;
	}
}

void ParamMgr::loadBoxDetailInfo()
{
	int arrGiveCount[35][5] = {
		{ 5,1,0,0,32 },
		{ 10,2,0,0,62 },
		{ 15,3,0,0,122 },
		{ 20,4,0,0,212 },
		{ 25,5,0,0,332 },
		{ 10,2,0,0,68 },
		{ 16,4,0,0,108 },
		{ 22,6,0,0,188 },
		{ 28,8,0,0,308 },
		{ 34,10,0,0,468 },
		{ 12,5,1,0,128 },
		{ 20,8,2,0,178 },
		{ 28,11,3,0,278 },
		{ 36,14,4,0,428 },
		{ 44,17,5,0,628 },
		{ 18,8,2,0,248 },
		{ 28,13,3,0,308 },
		{ 38,18,4,0,428 },
		{ 48,23,5,0,608 },
		{ 58,28,6,0,848 },
		{ 30,10,5,3,588 },
		{ 42,16,7,3,688 },
		{ 54,22,9,3,788 },
		{ 66,28,11,4,1088 },
		{ 78,34,13,4,1488 },
		{ 45,25,12,6,1188 },
		{ 53,33,15,7,1288 },
		{ 68,41,18,8,1488 },
		{ 83,49,21,9,1788 },
		{ 98,57,24,10,2188 },
		{ 100,50,25,15,1888 },
		{ 118,60,29,17,1988 },
		{ 136,70,33,19,2188 },
		{ 154,80,37,21,2488 },
		{ 172,90,41,23,2888 }
	};
	int arrTmpData[][7] = {
		{ 5,10,12,18,30,45,100 },
		{ 2,2,3,3,3,3,3 },
		{ 100,100,100,100,100,100,100 },
		{ 75,60,40,30,33,33,33 },
		{ 15,25,30,30,33,33,33 },
		{ 10,15,30,40,34,34,34 },
		{ 1,2,5,8,10,25,50 },
		{ 1,2,2,2,3,3,3 },
		{ 50,100,100,100,100,100,100 },
		{ 80,60,50,40,33,33,33 },
		{ 15,30,30,30,33,33,33 },
		{ 5,10,20,30,34,34,34 },
		{ 0,0,1,2,5,12,25 },
		{ 0,0,1,2,2,3,3 },
		{ 0,0,50,100,100,100,100 },
		{ 0,0,60,70,33,33,33 },
		{ 0,0,30,20,33,33,33 },
		{ 0,0,10,10,34,34,34 },
		{ 0,0,0,0,3,6,15 },
		{ 0,0,0,0,1,1,1 },
		{ 0,0,0,0,100,100,100 },
		{ 0,0,0,0,100,100,100 },
		{ 2,4,5,6,8,10,20 },
		{ 1,1,1,1,1,1,1 },
		{ 100,100,100,100,100,100,100 },
		{ 100,100,100,100,100,100,100 },
		{ 32,68,128,248,588,1188,1888 },
		{ 1,1,1,1,1,1,1 },
		{ 40,50,60,80,100,100,100 },
		{ 100,100,100,100,100,100,100 },
		{ 1,3,5,8,5,12,24 },
		{ 1,1,1,1,1,1,1 },
		{ 6,20,30,50,100,100,100 },
		{ 100,100,100,100,100,100,100 }
	};

	int arrTime[] = { 20, 120, 240, 360, 720, 720, 720 };
	int arrOpenCost[] = { 2, 6, 12, 18, 72, 72, 72 };
	int arrBuyCost[] = { 0, 0, 0, 0, 80, 160, 300 };
	int arrItemCount[] = { 3, 3, 3, 1, 1, 1, 1 };

	for (int i = 0; i < 7; i++)
	{
		auto pInfo = new BoxDetailInfo_T();
		memset(pInfo, 0, sizeof(BoxDetailInfo_T));

		pInfo->id = i;
		pInfo->waitTime = arrTime[i] * 60;//arrTime[i] * 60 * 60;
		pInfo->openCost = arrOpenCost[i];
		pInfo->buyCost = arrBuyCost[i];

		int row = 0;
		for (int j = 0; j < ParamData::BOX_CATEGORY_COUNT; j++)
		{
			BoxCategoryInfo_T *pCat = &(pInfo->arrCatgInfo[j]);
			
			pCat->id = j;

			if (j == (ParamData::BOX_CATEGORY_COUNT-1) || j == (ParamData::BOX_CATEGORY_COUNT - 2))
			{
				if (6 == i && j == (ParamData::BOX_CATEGORY_COUNT - 2))
				{
					for (int m = 0; m < ParamData::BOX_TOTAL_LV; m++)
					{
						pCat->arrGiveCount[m] = 1;
					}
					pCat->giveTypeCount = 1;
					pCat->totalPercent = 8;
					pCat->itemTypeCount = 4;

					for (int k = 0; k < pCat->itemTypeCount; k++)
					{
						pCat->arrItemId[k] = 509 + k * 10;
						pCat->arrItemWeight[k] = 25;
					}
				}
				continue;
			}

			for (int m = 0; m < ParamData::BOX_TOTAL_LV; m++)
			{
				if (4 == j || 6 == j)
				{
					pCat->arrGiveCount[m] = arrTmpData[row][i];
				}
				else
				{
					int tmpCol = (5 == j) ? 4 : j;
					int tmpRow = m + i*ParamData::BOX_TOTAL_LV;
					pCat->arrGiveCount[m] = arrGiveCount[tmpRow][tmpCol];
				}
			}

			row++;
			pCat->giveTypeCount = arrTmpData[row][i];
			row++;
			pCat->totalPercent = arrTmpData[row][i];
//			row++;
			pCat->itemTypeCount = arrItemCount[j];

			for (int k = 0; k < arrItemCount[j]; k++)
			{
				row++;
				pCat->arrItemId[k] = k;
				pCat->arrItemWeight[k] = arrTmpData[row][i];
			}
			row++;
		}

		_arrBoxDetailInfo.push_back(pInfo);
	}

	{
		/*tili box*/
		auto pInfo = new BoxDetailInfo_T();
		memset(pInfo, 0, sizeof(BoxDetailInfo_T));
		pInfo->id = 7;

		for (int j = 0; j < ParamData::BOX_CATEGORY_COUNT; j++)
		{
			BoxCategoryInfo_T *pCat = &(pInfo->arrCatgInfo[j]);

			pCat->id = j;

			if (4 == j)
			{
				for (int m = 0; m < ParamData::BOX_TOTAL_LV; m++)
				{
					pCat->arrGiveCount[m] = 8;
				}
				pCat->giveTypeCount = 1;
				pCat->totalPercent = 100;
				pCat->itemTypeCount = 1;
				pCat->arrItemId[0] = 0;
				pCat->arrItemWeight[0] = 100;
			}
			else if (5 == j)
			{
				for (int m = 0; m < ParamData::BOX_TOTAL_LV; m++)
				{
					pCat->arrGiveCount[m] = 88 + 100 * m;
				}
				pCat->giveTypeCount = 1;
				pCat->totalPercent = 60;
				pCat->itemTypeCount = 1;
				pCat->arrItemId[0] = 0;
				pCat->arrItemWeight[0] = 100;
			}

		}
		_arrBoxDetailInfo.push_back(pInfo);
	}

	int arrExBoxItemTtpeCount[] = { 2, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1 };
	int arrExBoxInfo[][4] = {
		{1000, 1, 1001, 2},
		{-101, 3, -100, 38},
		{16, 1, 0, 0},
		{1008, 1, -100, 88},
		{1000, 1, 1001, 2},
		{1, 1, 0, 0},
		{1004, 1, -100, 188},
		{2, 1, 0, 0},
		{-101, 4, 0, 0},
		{1002, 2, 1003, 1},
		{17, 1, 0, 0},
		{21, 1, -100, 488},
		{-101, 6, 0, 0},
		{11, 1, 0, 0},
		{1010, 3, -100, 688},
		{22, 1, 0, 0},
		{23, 1, 0, 0},
		{-101, 10, 0, 0}
	};

	for (int i = 0; i < 18; i++)
	{
		auto pInfo = new BoxDetailInfo_T();
		memset(pInfo, 0, sizeof(BoxDetailInfo_T));

		pInfo->id = 8 + i;

		for (int j = 0; j < ParamData::BOX_CATEGORY_COUNT; j++)
		{
			BoxCategoryInfo_T *pCat = &(pInfo->arrCatgInfo[j]);

			pCat->id = j;

			if ((ParamData::BOX_CATEGORY_COUNT-1) == j || (ParamData::BOX_CATEGORY_COUNT-2) == j)
			{
				int arrExIndex = j - (ParamData::BOX_CATEGORY_COUNT - 2);
				if (arrExIndex < arrExBoxItemTtpeCount[i])
				{
					pCat->giveTypeCount = 1;
					pCat->itemTypeCount = pCat->giveTypeCount;
					pCat->totalPercent = 100;

					for (int m = 0; m < ParamData::BOX_TOTAL_LV; m++)
					{
						pCat->arrGiveCount[m] = arrExBoxInfo[i][arrExIndex*2+1];
					}
					pCat->arrItemId[0] = arrExBoxInfo[i][arrExIndex * 2];
					pCat->arrItemWeight[0] = 100;
				}
			}
		}
		_arrBoxDetailInfo.push_back(pInfo);
	}
}

void ParamMgr::loadDungeonInfo()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/instance.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.Capacity();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[i];
		rapidjson::Value& arryPos = item["Array"];
		rapidjson::Value& arryDes = item["aim"];

		auto info = new DungeonDeatilInfo_T();
		info->id = item["id"].GetDouble();
		info->infoType = item["type"].GetDouble();
		info->stepCount = item["step"].GetDouble();
		info->initContentId = item["content"].GetDouble();
		info->strSaying = item["saying"].GetString();

		for (int a = 0; a < arryPos.Capacity(); a++)
		{
			rapidjson::Value& posXY = arryPos[a];

			info->arrInitPosX.push_back(posXY[0].GetDouble());
			info->arrInitPosY.push_back(posXY[1].GetDouble());
		}

		for (int b = 0; b < arryDes.Capacity(); b++)
		{
			rapidjson::Value& desItem = arryDes[b];

			DungeonDesInfo_T tmp = { 0 };
			tmp.crushCount = desItem["num"].GetDouble();
			tmp.crushId = desItem["id"].GetDouble();

			info->arrDesInfo.push_back(tmp);
		}

		_arrDungeonInfo.push_back(info);
	}
}

void ParamMgr::loadDungeonRewardInfo()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/ins_reward.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.Capacity();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[i];

		auto info = new DungeonReward_T();
		info->level = item["rank"].GetDouble();
		info->exp = item["exp"].GetDouble();
		info->gold = item["gold"].GetDouble();
		info->equip = item["equip"].GetDouble();
		info->equipLv = item["equip_lv"].GetDouble();

		_arrDungeonReward.push_back(info);
	}
}


const DragonBoneInfo_T *ParamMgr::getRoleArmtrInfo(int id)
{
	return _arrRoleArmtrInfo[id];
}


// const DragonBoneInfo_T *ParamMgr::getMonsterArmtrInfo(int id)
// {
// 	return _arrMonsterArmtrInfo[id];
// }


// const DragonBoneInfo_T *ParamMgr::getSkillArmtrInfo(int id)
// {
// 	return _arrSkillArmtrInfo[id];
// }


const EleWeakInfo_T *ParamMgr::getWeakInfo(int id)
{
	return _arrWeakInfo[id];
}


const EleSpecialInfo_T *ParamMgr::getEleSpecInfo(int id)
{
	return _arrEleSpecInfo[id-EleIconId_E::ELE_ID_SUPER-1];
}


const MonsterInfo_T *ParamMgr::getMonsterInfo(int id)
{
	return _arrMonsterInfo[id];
}

const std::map<int, MonsterAttArmInfo_T>* ParamMgr::getMonsterAttArmInfo(int id)
{
	return &(_arrMonsterAttArmInfo[id]);
}

const StageInfo_T * ParamMgr::getStageInfo(int id)
{
	const StageInfo_T *pRet = nullptr;
	if (-1 == id)
	{
		pRet = _pUnStopStageInfo;
	}
	else if (-2 == id)
	{
		pRet = _pInstanceStageInfo;
	}
	else
	{
		pRet = _arrStageInfo[id];
	}

	return pRet;
}

const BoxDetailInfo_T * ParamMgr::getBoxDetailInfo(int id)
{
	return _arrBoxDetailInfo[id];
}

const Skill_T * ParamMgr::getSkillByIdEx(int idx)
{
	const Skill_T *pRet = nullptr;
	for (int i = 0; i < _vectorSkills.size(); i++)
	{
		if (_vectorSkills[i].id == idx)
		{
			pRet = &(_vectorSkills[i]);
		}
	}

	return pRet;
}

const DungeonDeatilInfo_T * ParamMgr::getDungeonInfo(int idx)
{
	return _arrDungeonInfo[idx];
}

const DungeonReward_T * ParamMgr::getDungeonReward(int idx)
{
	return _arrDungeonReward[idx];
}

void ParamMgr::loadItemsConfig()
{
	rapidjson::Document _jsonBezierDoc;
	auto contentStr = FileUtils::getInstance()->getDataFromFile("jsonb/shopitem.b");
	ToolsUtil::unbtea(contentStr);
	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

	_jsonBezierDoc.Parse<0>(load_str.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		Item_T temp;
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("item_%d", i)->getCString()];
		temp.id = item["id"].GetInt();
		temp.name = item["name"].GetString();
		temp.des = item["description"].GetString();
		temp.type = item["typeid"].GetInt();
		temp.value = item["typeVal"].GetInt();
		temp.price = item["price"].GetInt();
		temp.picname = item["pic_name"].GetString(); 
		temp.paytype = item["price_type"].GetInt();

		_vectorItems.push_back(temp);
	}
	s_pInstance->_itemstartid = s_pInstance->getItemVector().at(0).id;
}

void ParamMgr::loadSkillsConfig()
{
	rapidjson::Document _jsonBezierDoc;

	/*std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/role_kill.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());*/

	auto contentStr = FileUtils::getInstance()->getDataFromFile("json/role_kill.json");
	//ToolsUtil::unbtea(contentStr);
	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());
	_jsonBezierDoc.Parse<0>(load_str.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonSkillParseError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.MemberCount();

	for (int i = 0; i < count; i++)
	{
		Skill_T temp;
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("skill_%d", i)->getCString()];
		temp.id = item["id"].GetInt();
		temp.name = item["name"].GetString();
		temp.des = item["description"].GetString();
		temp.attribute = item["attribute"].GetInt();
		temp.paytype = item["price_type"].GetInt();
		temp.price = item["price"].GetInt();
		temp.icon = item["icon"].GetString();
		temp.mark = item["mark"].GetInt();
		temp.starNum = item["starNum"].GetInt();
		temp.isHurtAll = item["range"].GetBool();
		temp.isNeedMove = item["move"].GetBool();
		temp.roleAnim = item["role_act"].GetString(); //"attack02";// 
		temp.skillAnim = item["skill_act"].GetString(); //"sqy_chf_s";//
		temp.buffAnim = item["buff_act"].GetString(); //"sqy_pt";// 

		const rapidjson::Value& arry = item["normalVal"];
		const rapidjson::Value& arr2 = item["enhanceVal"];
		for (int i = 0; i < 3; i++)
		{
			if (i < arry.Capacity())
			{
				temp.normalVal[i] = arry[i].GetDouble();
			}
			else
			{
				temp.normalVal[i] = 0.f;
			}

			if (i < arr2.Capacity())
			{
				temp.enhanceVal[i] = arr2[i].GetDouble();
			}
			else
			{
				temp.enhanceVal[i] = 0.f;
			}		
		}
		
		_vectorSkills.push_back(temp);
	}

	s_pInstance->_skillstartid = s_pInstance->getSkillVector().at(0).id;
}

void ParamMgr::loadWeaponConfig()
{
	rapidjson::Document _jsonBezierDoc;
	//std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/role_equip.json");
	auto contentStr = FileUtils::getInstance()->getDataFromFile("jsonb/role_equip.b");

	ToolsUtil::unbtea(contentStr);
	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

	_jsonBezierDoc.Parse<0>(load_str.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		Weapon_T temp;
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("equip_%d", i)->getCString()];
		temp.id = item["id"].GetInt();
		temp.equipMinLv = item["limit"].GetInt();
		temp.name = item["name"].GetString();
		temp.des = item["description"].GetString();

		if (temp.equipMinLv != 0)
		{
			temp.des = temp.des + String::createWithFormat(ResMgr::getInstance()->getString("lv_limit")->getCString(), temp.equipMinLv)->getCString();
		}
		temp.paytype = item["price_type"].GetInt();
		temp.type = item["roleid"].GetDouble();

		const rapidjson::Value& arr = item["attrib"];
		const rapidjson::Value& min = item["lvaddmin"];
		const rapidjson::Value& max = item["lvaddmax"];

		temp.attack = arr[0].GetInt();
		temp.def = arr[1].GetInt();
		temp.dpadd = arr[2].GetInt();
		temp.hpadd = arr[3].GetInt();
		
		temp.lvMinAttack = min[0].GetInt();
		temp.lvMinDef = min[1].GetInt();
		temp.lvMinDpadd = min[2].GetInt();
		temp.lvMinHpadd = min[3].GetInt();

		temp.lvMaxAttack = max[0].GetInt();
		temp.lvMaxDef = max[1].GetInt();
		temp.lvMaxDpadd = max[2].GetInt();
		temp.lvMaxHpadd = max[3].GetInt();

		temp.price = item["price"].GetInt();
		temp.picname = item["picname"].GetString();

		_vectorWeapons.push_back(temp);
	}
	s_pInstance->_weaponstartid = s_pInstance->getWeaponVector().at(0).id;

	auto& att = _vectorWeapons;
	for (int i = 0; i < 40; i++)
	{
		if (UserData::getInstance()->getWeaponLv(i) <= 1)
		{
			AttactInfo_T temp;
			temp.attack = att[i].attack;
			temp.def = att[i].def;
			temp.dpAdd = att[i].dpadd;
			temp.hpAdd = att[i].hpadd;
			UserData::getInstance()->setWeaponAttack(i, temp);
		}
	}
	UserData::getInstance()->saveUserData(false);
}

void ParamMgr::loadPlayerInfo()
{
	for (int playidx = 0; playidx < 4; playidx ++)
	{
		rapidjson::Document _jsonBezierDoc;
		/*std::string contentStr = FileUtils::getInstance()->getStringFromFile(String::createWithFormat("json/player_%d.json", playidx)->getCString());
		_jsonBezierDoc.Parse<0>(contentStr.c_str());*/

		auto contentStr = FileUtils::getInstance()->getDataFromFile(String::createWithFormat("jsonb/player_%d.b", playidx)->getCString());
		ToolsUtil::unbtea(contentStr);
		std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

		_jsonBezierDoc.Parse<0>(load_str.c_str());

		if (_jsonBezierDoc.HasParseError())
		{
			CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
		}
		rapidjson::Value& arrhp = _jsonBezierDoc["hp"];
		rapidjson::Value& arrexp = _jsonBezierDoc["exp"];
		rapidjson::Value& arrdp = _jsonBezierDoc["dp"];
		rapidjson::Value* arrattack = nullptr;
		rapidjson::Value* arrdpadd = nullptr;
		rapidjson::Value* arrhpadd = nullptr;
		rapidjson::Value* arrhpdef = nullptr;
		if (_jsonBezierDoc.HasMember("attack"))
		{
			arrattack = &_jsonBezierDoc["attack"];
		}
		if (_jsonBezierDoc.HasMember("defadd"))
		{
			arrdpadd = &_jsonBezierDoc["defadd"];
		}
		if (_jsonBezierDoc.HasMember("hpadd"))
		{
			arrhpadd = &_jsonBezierDoc["hpadd"];
		}
		if (_jsonBezierDoc.HasMember("def"))
		{
			arrhpdef = &_jsonBezierDoc["def"];
		}
		int count = arrhp.Capacity();
		for (int i = 0; i < count; i++)
		{
			PlayerInfo_T temp = {0};
			temp.hp = arrhp[i].GetInt();
			temp.dp = arrdp[i].GetInt();
			if (i != count - 1)
			{
				temp.exp = arrexp[i + 1].GetInt();
			}
			else
			{
				temp.exp = 0;
			}
			
			if (arrattack)
			{
				temp.attack = (*arrattack)[i].GetInt();
			}
			else
			{
				temp.attack = 0;
			}

			if (arrdpadd)
			{
				temp.dpadd = (*arrdpadd)[i].GetInt();
			}
			else
			{
				temp.dpadd = 0;
			}

			if (arrhpadd)
			{
				temp.hpadd = (*arrhpadd)[i].GetInt();
			}
			else
			{
				temp.hpadd = 0;
			}

			if (arrhpdef)
			{
				temp.def = (*arrhpdef)[i].GetInt();
			}
			else
			{
				temp.def = 0;
			}
			_vectorPlayerInfo[playidx].push_back(temp);
		}
	}	
}

void ParamMgr::loadWAndSkill()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/role_attrib.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("skill_%d", i)->getCString()];
		int playeridx = item["roleid"].GetInt();

		PlayerWAndSkill_T& temp = _arrPlayerWAndSkill[playeridx];
		const rapidjson::Value& arryskills = item["skills"];

		SkillIDAndPos sap;
		sap.id = arryskills[0].GetInt();
		sap.pos = arryskills[1].GetInt();
		sap.flag = arryskills[2].GetInt();

		temp._vectorSP.push_back(sap);
	}

	/*for (int i = 0; i < count; i++)
	{
		PlayerWAndSkill_T& temp = _arrPlayerWAndSkill[i];
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("skill_%d", i)->getCString()];
		const rapidjson::Value& arryskills = item["skills"];
		for (int j = 0; j < arryskills.Capacity(); j++)
		{
			const rapidjson::Value& skillinfo = arryskills[j];

			SkillIDAndPos sap;
			sap.id = skillinfo[0].GetInt();
			sap.pos = skillinfo[1].GetInt();
			sap.flag = skillinfo[2].GetInt();

			temp._vectorSP.push_back(sap);
		}
	}*/
}

void ParamMgr::loadShopAttr()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/shop_attr.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("shop_%d", i)->getCString()];
		ShopAttr_T shop;
		shop.pos = item["location"].GetInt();
		rapidjson::Value& arrskill = item["kill"];
		rapidjson::Value& arrequip = item["equip"];
		rapidjson::Value& arritems = item["medical"];

		int count = arrskill.Capacity();
		for (int i = 0; i < count; i++)
		{
			shop._skillsconfig.push_back(arrskill[i].GetInt());
		}

		count = arrequip.Capacity();
		for (int i = 0; i < count; i++)
		{
			shop._weaponsconfig.push_back(arrequip[i].GetInt());
		}

		count = arritems.Capacity();
		for (int i = 0; i < count; i++)
		{
			shop._itemsconfig.push_back(arritems[i].GetInt());
		}

		_vectorShopAttr.push_back(shop);
	}
}
void ParamMgr::loadPosName()
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/stage_name.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	auto count = _jsonBezierDoc.MemberCount();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = _jsonBezierDoc[String::createWithFormat("item_%d", i)->getCString()];

		int pos = item["id"].GetInt();
		std::string name = item["name"].GetString();

		_posnameMap.insert(make_pair(pos, name));

		_vectorStoryPos.push_back(pos);
	}
}

void ParamMgr::loadStorePageOneConfig()
{
	//boxconfig
	{
		rapidjson::Document _jsonBezierDoc;
	/*	std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/storepage_0.json");
		_jsonBezierDoc.Parse<0>(contentStr.c_str());*/

		auto contentStr = FileUtils::getInstance()->getDataFromFile("jsonb/storepage_0.b");
		ToolsUtil::unbtea(contentStr);
		std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());
		_jsonBezierDoc.Parse<0>(load_str.c_str());

		if (_jsonBezierDoc.HasParseError())
		{
			CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
		}

		auto count = _jsonBezierDoc.Capacity();
		for (int i = 0; i < count; i++)
		{
			rapidjson::Value& arr0 = _jsonBezierDoc[i];
			StorePageOneItem_T shop;
			shop.id = arr0["id"].GetInt();
			shop.itemid = arr0["itemid"].GetInt();
			shop.num = arr0["num"].GetInt();
			shop.price = arr0["price"].GetInt();
			shop.type = arr0["type"].GetInt();
			shop.des = arr0["des"].GetString();
			_vectorPageOneItems.push_back(shop);
		}
	}
}

void ParamMgr::loadWenponUpConfig()
{
	rapidjson::Document _jsonBezierDoc;
	/*std::string contentStr = FileUtils::getInstance()->getStringFromFile("json/wenponup.json");
	_jsonBezierDoc.Parse<0>(contentStr.c_str());*/

	auto contentStr = FileUtils::getInstance()->getDataFromFile("jsonb/wenponup.b");
	ToolsUtil::unbtea(contentStr);
	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

	_jsonBezierDoc.Parse<0>(load_str.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	auto count = _jsonBezierDoc.Capacity();
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& arr0 = _jsonBezierDoc[i];
		WenponLvUpInfo_T shop;
		shop.type = arr0[0].GetDouble();
		shop.lv = i;
		shop.costtype = 0;
		shop.costnum = arr0[2].GetDouble();
		shop.costparts = arr0[3].GetDouble();
		shop.upp = arr0[4].GetDouble();

		_vectorArrWenponup[shop.type].push_back(shop);
	}

}

void ParamMgr::reloadStoreFreshItemsConfig()
{
	//item config
	{
		int configidx[] = { 10, 19, 32, 41, 48, 64, 72, 80 };
		int fileconfigidx = 7;
		for (int i = 0; i < 8; i++)
		{
			if (UserData::getInstance()->getMaxPos() < configidx[i])
			{
				fileconfigidx = i;
				break;
			}
		}
		_vvectorBoxPosConfig.clear();

		auto filestring = String::createWithFormat("json/storebox_config_%d.json", fileconfigidx)->getCString();

		rapidjson::Document _jsonBezierDoc;
		std::string contentStr = FileUtils::getInstance()->getStringFromFile(filestring);
		
		/*auto contentStr = FileUtils::getInstance()->getDataFromFile(filestring);
		ToolsUtil::unbtea(contentStr);
		std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

		_jsonBezierDoc.Parse<0>(load_str.c_str());*/
		
		_jsonBezierDoc.Parse<0>(contentStr.c_str());

		if (_jsonBezierDoc.HasParseError())
		{
			CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
		}

		auto count = _jsonBezierDoc.Capacity();
		for (int i = 0; i < count; i++)
		{
			rapidjson::Value& arr0 = _jsonBezierDoc[i];
			std::vector<StorePageOnePosConfig_T> temparr;

			int count = arr0.Capacity();
			for (int i = 0; i < count; i++)
			{
				rapidjson::Value& arr1 = arr0[i];
				StorePageOnePosConfig_T item;
				item.pageoneitemidx = arr1["itemid"].GetInt();
				item.p = 1;
				temparr.push_back(item);
			}

			_vvectorBoxPosConfig.push_back(temparr);
		}
	}
}

const Item_T &  ParamMgr::getItemByID(int idx)
{
	int index = -1;
	for (int i = 0; i < _vectorItems.size(); i++)
	{
		if (_vectorItems[i].id == idx)
		{
			index = i;
			break;
		}
	}
	CCASSERT(index != -1, "no find item");
	return _vectorItems[index];
}

const Skill_T & ParamMgr::getSkillByID(int idx)
{
	int index = -1;
	for (int i = 0; i < _vectorSkills.size(); i++)
	{
		if (_vectorSkills[i].id == idx)
		{
			index = i;
			break;
		}
	}
	CCASSERT(index != -1, "no find skill");
	return _vectorSkills[index];
}

const Weapon_T & ParamMgr::getWeaponByID(int idx)
{
	int index = -1;
	for (int i = 0; i < _vectorWeapons.size(); i++)
	{
		if (_vectorWeapons[i].id == idx)
		{
			index = i;
			break;
		}
	}
	CCASSERT(index != -1, "no find weapon");
	return _vectorWeapons[index];
}

const PlayerInfo_T & ParamMgr::getPlayerInfo(int playeridx, int lv)
{
	if (lv >= getPlayerMaxLv())
	{
		lv = 49;
		CCLOG("lv > 49");
	}
	return (_vectorPlayerInfo[playeridx])[lv];
}

void ParamMgr::getComGameItemInfo(int idx, ComInfo_T& cominfo)
{
	if (idx >= 1500)
	{
		cominfo.id = _vectorWeapons[idx - 1500 + 40].id;
		cominfo.name = _vectorWeapons[idx - 1500 + 40].name;
		cominfo.des = _vectorWeapons[idx - 1500 + 40].des;
		cominfo.picname = _vectorWeapons[idx - 1500 + 40].picname;
		cominfo.price = _vectorWeapons[idx - 1500 + 40].price;
		cominfo.value = _vectorWeapons[idx - 1500 + 40].type;
		cominfo.paytype = _vectorWeapons[idx - 1500 + 40].paytype;
	}
	else if (idx >= 1000)
	{
		cominfo.id = _vectorItems[idx - 1000].id;
		cominfo.name = _vectorItems[idx - 1000].name;
		cominfo.des = _vectorItems[idx - 1000].des;
		cominfo.picname = _vectorItems[idx - 1000].picname;
		cominfo.price = _vectorItems[idx - 1000].price;
		cominfo.value = _vectorItems[idx - 1000].type;
		cominfo.paytype = _vectorItems[idx - 1000].paytype;
	}
	else if (idx >= 500)
	{
		cominfo.id = _vectorWeapons[idx - 500].id;
		cominfo.name = _vectorWeapons[idx - 500].name;
		cominfo.des = _vectorWeapons[idx - 500].des;
		cominfo.picname = _vectorWeapons[idx - 500].picname;
		cominfo.price = _vectorWeapons[idx - 500].price;
		cominfo.value = _vectorWeapons[idx - 500].type;
		cominfo.paytype = _vectorWeapons[idx - 500].paytype;
	}
	else if(idx >= 0)
	{
		cominfo.id = _vectorSkills[idx].id;
		cominfo.name = _vectorSkills[idx].name;
		cominfo.des = _vectorSkills[idx].des;
		cominfo.picname = _vectorSkills[idx].icon;
		cominfo.price = _vectorSkills[idx].price;
		cominfo.value = _vectorSkills[idx].attribute;
		cominfo.paytype = _vectorSkills[idx].paytype;
	}
	else if (idx == ParamData::TILI_ITEM_ID)
	{	
		cominfo.id = ParamData::TILI_ITEM_ID;
		cominfo.name = ResMgr::getInstance()->getString("item_name_-102")->getCString();
		cominfo.picname = "com_icon_jingqi.png";
	}
	else if (idx == ParamData::GOLD_ITEM_ID)
	{	
		cominfo.id = ParamData::GOLD_ITEM_ID;
		cominfo.name = ResMgr::getInstance()->getString("item_name_-100")->getCString();
		cominfo.picname = "com_goldbag.png";
	}
	else if (idx == ParamData::CRYSTAL_ITEM_ID)
	{
		cominfo.id = ParamData::CRYSTAL_ITEM_ID;
		cominfo.name = ResMgr::getInstance()->getString("item_name_-101")->getCString();
		cominfo.picname = "com_crystal.png";
	}
}

std::string  ParamMgr::getPosName(int pos)
{
	std::string temp("");

	if (_posnameMap.find(pos) != _posnameMap.end())
	{
		return _posnameMap.at(pos);
	}
	else
	{		
		int id = 0;
		for (auto&temp : _vectorShopAttr)
		{
			if (temp.pos == pos)
			{
				return ResMgr::getInstance()->getString(String::createWithFormat("shopname_%d", id)->getCString())->getCString();
			}
			++id;
		}
	}

	return temp;
}


void ParamMgr::getBoxItems(int boxidx, std::vector<int>& itemids, std::vector<int>& nums)
{
	auto& arr = _vectorBoxReward[boxidx];
	int idx = 0;
	for (auto typeitems : arr.itemtypeidx)
	{
		auto p = ToolsUtil::getRandomInt(0, 100);
		if (p < arr.probability[idx])
		{
			auto& arr2 = _vectorItemsConfig[typeitems];
			auto& pp = arr2.probabilities;
			std::vector<int> select;
			ToolsUtil::getRandoms(pp, select, arr.gettypenums[idx]);
			for (auto& selectidx : select)
			{
				itemids.push_back(arr2.itemids[selectidx]);
				nums.push_back(arr.nums[idx]);
			}
		}

		idx++;
	}
}

void ParamMgr::getBoxItems(int boxidx, std::vector<PopItemInfo_T>& arrItems)
{
	std::vector<int> arrItemId;
	std::vector<int> arrItemCount;

	getBoxItems(boxidx, arrItemId, arrItemCount);

	for (int i = 0; i < arrItemId.size(); i++)
	{
		PopItemInfo_T tmp;

		tmp.itemId = arrItemId[i];
		tmp.itemCount = arrItemCount[i];

		arrItems.push_back(tmp);
	}
}

void ParamMgr::changeShowReliveWeight(int typeValue)
{
	switch (typeValue)
	{
	case 0:
		_showReliveWeight += 2;
		break;
	case 1:
		_showReliveWeight = MAX(0, _showReliveWeight - 4);
		break;
	case 2:
		_showReliveWeight = MAX(0, _showReliveWeight - 3);
		break;
	}
}
