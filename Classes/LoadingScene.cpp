#include "LoadingScene.h"
#include "VisibleRect.h"
#include "UserData.h"
#include "ParamMgr.h"
#include "Scenes/MainScene.h"
#include "SkillControl.h"
#include "GameUtils.h"
#include "GameResPool.h"
#include "Utils/ParticleManager.h"
#include "GLCommon/JRCommon.h"
#include "Defines.h"


static const LoadResInfo_T s_loadGameResInfo[] = {
	{ LoadResType::JPG, "newworldmap/word_0.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_1.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_2.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_3.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_4.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_5.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_6.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_7.jpg", "" },
	{ LoadResType::JPG, "newworldmap/word_8.jpg", "" },
#if (1 == CC_ENABLE_NEW_WORLD)
	{ LoadResType::JPG,"newWorld2/newWorld_0.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_1.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_2.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_3.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_4.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_5.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_6.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_7.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_8.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_9.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_10.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_11.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_12.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_13.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_14.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_15.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_16.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_17.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_18.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_19.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_20.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_21.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_22.jpg","" },
	{ LoadResType::JPG,"newWorld2/newWorld_23.jpg","" },
#endif

	{ LoadResType::PLIST_PVR_CCZ, "ui/mainui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/shopui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/boxlayerui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/storescale9", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/mapui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/rankui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/newstoreui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/comui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/wenponui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/scoreui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/skillicon", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/wupui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/fightUi", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/comitems", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/comscale9", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/dayrewardui", "" },
#if (CC_PAY_SDK == PAY_SDK_MIGU)
	{ LoadResType::PLIST_PVR_CCZ, "uiext/miguui", "" },
#endif
	
	
	{ LoadResType::JPG, "mapsnew/map_3_bg.jpg", "" },
	{ LoadResType::PNG, "mapsnew/commond/map_com_0.png", "" },
	{ LoadResType::PNG, "mapsnew/commond/map_com_1.png", "" },
	{ LoadResType::PNG, "bgs/main_dark_logo.png", "" },
	
	{ LoadResType::PLIST_PVR_CCZ, "roles/cbd/cbd0", "" },
	{ LoadResType::PLIST_PVR_CCZ, "roles/lsj/lsj0", "" },
	{ LoadResType::PLIST_PVR_CCZ, "roles/lqc/lqc0", "" },
	{ LoadResType::PLIST_PVR_CCZ, "roles/sqy/sqy0", "" },
	{ LoadResType::PLIST_PVR_CCZ, "roles/zhuangbei/zhuangbei0", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/newworldui", "" },
	{ LoadResType::PLIST_PVR_CCZ, "ui/dungeonui", "" },
// 	{ LoadResType::ARMTR, "roles/lsj/lsj.ExportJson", "" },
// 	{ LoadResType::ARMTR, "roles/cbd/cbd.ExportJson", "" },
// 	{ LoadResType::ARMTR, "roles/lqc/lqc.ExportJson", "" },
// 	{ LoadResType::ARMTR, "roles/sqy/sqy.ExportJson", "" },

//	{ LoadResType::ARMTR, "roles/zhuangbei/zhuangbei", "" },
};

const std::string g_arrPreLoadSound[] = {
	"jinbi.ogg",
	"dianji.ogg",
	"xiaochu_lsj.ogg",
	"xiaochu_sqy.ogg",
	"xiaochu_cbd.ogg",
	"xiaochu_lqc.ogg",
	"xiaochu_qyl.ogg",
	"lsj_att.ogg",
//	"lsj_huo.ogg",
//	"lsj_lei.ogg",
//	"lsj_dyh.ogg",
//	"lsj_dghs.ogg",
	"cbd_att.ogg",
//	"cbd_bing.ogg",
	"cbd_fangyu.ogg",
//	"cbd_hbfb.ogg",
//	"cbd_jysg.ogg",
	"lqc_att.ogg",
//	"lqc_huo.ogg",
//	"lqc_lei.ogg",
//	"lqc_bing.ogg",
//	"lqc_suoxiao.ogg",
//	"lqc_yss.ogg",
//	"lqc_ldzm.ogg",
//	"lqc_bjbf.ogg",
	"sqy_xue.ogg",
	"sqy_jiaxue.ogg",
//	"sqy_jinghua.ogg",
//	"sqy_zengfu.ogg",
	"qyl_att.ogg",
	"attack1.ogg",
	"attack2.ogg",
	"attack3.ogg",
	"attack4.ogg",
	"attack5.ogg",
	"attack6.ogg",
//	"shengli.ogg",
//	"shibai.ogg",
};


//const int g_loadGameResCount = sizeof(g_loadGameResInfo) / sizeof(g_loadGameResInfo[0]);
const int g_loadFuncCount = 20;
const int g_loadSoundCount = sizeof(g_arrPreLoadSound) / sizeof(g_arrPreLoadSound[0]);
//const int g_loadSkillCount = 9;


Scene * LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();

	scene->addChild(layer);

	return scene;
}

LoadingScene::LoadingScene()
	:_curLoadIndex(0),
	_totalLoadStep(0),
	_isLoadingRes(false)
{

}

LoadingScene::~LoadingScene()
{

}

bool LoadingScene::init()
{
	bool bRet = false;

	do 
	{
		ToolsUtil::initRandomNumber();

		_pLblLoad = Label::createWithTTF("Loading...", "fonts/Berlin Sans FB Demi.ttf", 20);
		this->addChild(_pLblLoad);
		_pLblLoad->setPosition(VisibleRect::center());
		
		int resCount = sizeof(s_loadGameResInfo) / sizeof(s_loadGameResInfo[0]);
		for (int i = 0; i < resCount; i++)
		{
			_arrSkillRes.push_back(s_loadGameResInfo[i]);
		}

		_totalLoadStep = g_loadFuncCount + _arrSkillRes.size() + 3 * 4 + g_loadSoundCount;

		std::string arrMonsterName[5] = {"fire", "snake", "black_luopo", "tree_01", "dragon01"};
		std::string arrAnimName[5] = {"stand", "walk", "walk" , "stand" , "walk" };
		int randomIdx = ToolsUtil::getRandomInt(0, 4);
		auto pArmtr = GameArmtr::createMonster(arrMonsterName[randomIdx]);
		this->addChild(pArmtr);
		pArmtr->setPosition(VisibleRect::center() + Vec2(0.f, 50.f));
		pArmtr->play(arrAnimName[randomIdx]);

		_tipIdx = ToolsUtil::getRandomInt(0, ParamData::LOAD_TIPS_COUNT - 1);
		auto strKey = __String::createWithFormat("load_tips_%d", _tipIdx);
		Label *pTips = Label::createWithSystemFont(ResMgr::getInstance()->getString(strKey->getCString())->getCString(), "fonts/Marker Felt.ttf", 20);
		this->addChild(pTips);
		pTips->setPosition(Vec2(320.f, 100.f));

		this->runAction(RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create([=] {
			_tipIdx = (_tipIdx + 1) % ParamData::LOAD_TIPS_COUNT;
			auto strKey = __String::createWithFormat("load_tips_%d", _tipIdx);
			pTips->setString(ResMgr::getInstance()->getString(strKey->getCString())->getCString());
		}))));


		this->scheduleUpdate();

		bRet = true;
	} while (false);

	return bRet;
}

void LoadingScene::update(float delta)
{
	auto strText = __String::createWithFormat("Loading...  %d%%", _curLoadIndex * 100 / _totalLoadStep);
	_pLblLoad->setString(strText->getCString());

	if (!_isLoadingRes)
	{
		if (_curLoadIndex < g_loadFuncCount)
		{
			_isLoadingRes = true;
			this->loadGameData(_curLoadIndex);
			_isLoadingRes = false;
			_curLoadIndex++;
		}
		else if (_curLoadIndex < (g_loadFuncCount + _arrSkillRes.size()))
		{
			if (_curLoadIndex == g_loadFuncCount)
			{
				this->initSkillResData();
				this->initEffectResData();
				_totalLoadStep = g_loadFuncCount + _arrSkillRes.size() + g_loadSoundCount;
			}
			_isLoadingRes = true;

			__String *strImageFile = nullptr;
			int index = _curLoadIndex - g_loadFuncCount;
			LoadResInfo_T *pInfo = &_arrSkillRes[index];

			if (LoadResType::PNG == pInfo->resType || LoadResType::JPG == pInfo->resType)
			{
				Director::getInstance()->getTextureCache()->addImageAsync(pInfo->filePath, CC_CALLBACK_1(LoadingScene::addImageCallBack, this, pInfo));
			}
			else if (LoadResType::PLIST_PNG == pInfo->resType)
			{
				strImageFile = __String::createWithFormat("%s.png", pInfo->filePath.c_str());
				Director::getInstance()->getTextureCache()->addImageAsync(strImageFile->getCString(), CC_CALLBACK_1(LoadingScene::addImageCallBack, this, pInfo));
			}
			else if (LoadResType::PLIST_PVR_CCZ == pInfo->resType)
			{
				strImageFile = __String::createWithFormat("%s.pvr.ccz", pInfo->filePath.c_str());
				Director::getInstance()->getTextureCache()->addImageAsync(strImageFile->getCString(), CC_CALLBACK_1(LoadingScene::addImageCallBack, this, pInfo));
			}
		}
		else if (_curLoadIndex < (g_loadFuncCount + _arrSkillRes.size() + g_loadSoundCount))
		{
			if (_curLoadIndex == (g_loadFuncCount + _arrSkillRes.size()))
			{
				if (UserData::getInstance()->isNeedGuide())
				{
					GameResPool::getInstance()->loadRes(GameResId_E::RES_SPECIAL, ParamData::CRUSH_COL*ParamData::CRUSH_ROW);
				}
				else
				{
					GameResPool::getInstance()->loadRes(GameResId_E::RES_SPECIAL, 4);
				}
				GameResPool::getInstance()->loadRes(GameResId_E::RES_ELE_ICON, ParamData::CRUSH_COL*ParamData::CRUSH_ROW+6);
				ParticleManager::getInstance()->addPlistData("items/crush_effect/xingxing_02/xingxing_02.plist", "crush_1");
			}
			_isLoadingRes = true;
			int index = _curLoadIndex - (g_loadFuncCount + _arrSkillRes.size());
			GameUtils::preloadEffect(g_arrPreLoadSound[index].c_str());
			_isLoadingRes = false;
			_curLoadIndex++;
		}
		else if (_curLoadIndex == _totalLoadStep)
		{
			ArmatureDataManager::getInstance()->addArmatureFileInfo("roles/zhuangbei/zhuangbei.ExportJson");
			_isLoadingRes = true;
			auto scene = MainLayer::scene();
			Director::getInstance()->replaceScene(scene);
		}
	}
}


void LoadingScene::loadGameData(int index)
{
	std::function<void()> arrFunc[] = { \
		UserData::getInstance,
		CC_CALLBACK_0(ParamMgr::loadRoleArmtrInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadWeakInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadEleSpecInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadMonsterInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadMonsterAttArmInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadStageInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadItemsConfig, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadSkillsConfig, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadWeaponConfig, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadPlayerInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadWAndSkill, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadShopAttr, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadPosName, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadStorePageOneConfig, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadWenponUpConfig, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadBoxDetailInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadDungeonInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadDungeonRewardInfo, ParamMgr::getInstance()),
		CC_CALLBACK_0(ParamMgr::loadHotStoreWenponCofig, ParamMgr::getInstance()),
	};

//	CCASSERT((_curLoadIndex - g_loadGameResCount) >= 0, "_curLoadIndex error!!!!");
	CCASSERT(g_loadFuncCount == sizeof(arrFunc) / sizeof(arrFunc[0]), "g_loadFuncCount error!!!!");

	arrFunc[index]();
}


void LoadingScene::addImageCallBack(Texture2D *pTexture, LoadResInfo_T *pResInfo)
{
	const LoadResInfo_T *pInfo = pResInfo;

	if (LoadResType::JPG == pInfo->resType || LoadResType::PNG == pInfo->resType)
	{
	}
	else if (LoadResType::PLIST_PNG == pInfo->resType || LoadResType::PLIST_PVR_CCZ == pInfo->resType)
	{
//		__String *strImageFile = __String::createWithFormat("%s%s", pInfo->filePath.c_str(), ".png");
		__String *strPlistFile = __String::createWithFormat("%s%s", pInfo->filePath.c_str(), ".plist");

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(strPlistFile->getCString());
	}

	_curLoadIndex++;
	_isLoadingRes = false;
}


void LoadingScene::initSkillResData()
{
	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 2 && i != RoleId_E::ROLE_ID_CBD)
			{
				break;
			}
			int skillId = SkillControl::getInstance()->getEquipSkillByPlayer(i, j);
			auto pSkillInfo = ParamMgr::getInstance()->getSkillByIdEx(skillId);
			
			if (pSkillInfo->skillAnim != "")
			{
				int index = 0;
				do
				{
					__String *strPlistName = __String::createWithFormat("skills/%s/%s%d.plist", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);

					if (FileUtils::getInstance()->isFileExist(strPlistName->getCString()))
					{
						__String *strBaseName = __String::createWithFormat("skills/%s/%s%d", pSkillInfo->skillAnim.c_str(), pSkillInfo->skillAnim.c_str(), index);

						LoadResInfo_T resInfo;
						resInfo.resType = LoadResType::PLIST_PVR_CCZ;
						resInfo.filePath = strBaseName->getCString();

						_arrSkillRes.push_back(resInfo);
						index++;
					}
					else
					{
						break;
					}

				} while (true);
			}
		}

	}
}


void LoadingScene::initEffectResData()
{
	std::string arrBasePath[] = { "items/crush_effect/crush_effect", "items/create/create", "items/crush_icon/crush_icon" };//, "items/special_icon/special_icon" };
	int arrarySize = sizeof(arrBasePath) / sizeof(arrBasePath[0]);

	for (int i = 0; i < arrarySize; i++)
	{
		int fileIndex = 0;

		do 
		{
			__String *strPlistName = __String::createWithFormat("%s%d.plist", arrBasePath[i].c_str(), fileIndex);

			if (FileUtils::getInstance()->isFileExist(strPlistName->getCString()))
			{
				__String *strBaseName = __String::createWithFormat("%s%d", arrBasePath[i].c_str(), fileIndex);
				LoadResInfo_T resInfo;
				resInfo.resType = LoadResType::PLIST_PVR_CCZ;
				resInfo.filePath = strBaseName->getCString();

				_arrSkillRes.push_back(resInfo);
				fileIndex++;
			}
			else
			{
				break;
			}

		} while (true);
	}
}