#include "UserData.h"
#include "SkillControl.h"
#include "WeaponControl.h"
#include "../WxSql/GLLocalStorage.h"
#include "../Scenes/MainScene.h"
#include "ParamMgr.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "AchieveMgr.h"

UserData* UserData::_instance = nullptr;

const char* itemid_gold = "itemid_good_gold";
const char* itemid_crystal = "itemid_good_crystal";

const char* KEY_STORY_IDX = "key_story_idx";
const char* KEY_PLAYERPOS = "key_player_pos";
const char* KEY_MAXPOS = "key_maxpos";
const char* KEY_GUIDE = "key_guide";
const char* KEY_STORE = "key_storeitemid_%d";
const char* KEY_TILI = "key_tili_num";
const char* KEY_WIN_TIMES = "key_win_times";
const char* KEY_STORE_PAGE = "key_store_page_%d";
const char* KEY_STORE_STAGE = "key_store_page_stage_%d";
const char* KEY_PASSBOSS_STAGE = "key_is_passboss_%d";
const char* KEY_PLAYING_QUANQIA = "key_playing_quanqia";

const char* KEY_W_LV = "key_weapon_lv_%d";
const char* KEY_CURATTACK = "key_weapon_attack_%d";
const char* KEY_PARTS_NUM = "key_weapon_partsnum_%d";

const char *KEY_ACCOUNT = "key_account";
const char *KEY_NICK = "key_nick";
const char *KEY_HEADID = "key_headid";
const char *KEY_TOTAL_FLOOR = "key_total_floor";
const char *KEY_WEEKLY_SCORE = "key_weekly_score";
const char *KEY_LAST_SETTLE = "key_last_settle";
const char *KEY_LAST_LOGIN = "key_last_login";
const char *KEY_EXCHANGE = "key_exchange_%d";
const char *KEY_ACHIEVE_COUNT = "key_ach_count_%d";
const char *KEY_ACHIEVE_LV = "key_ach_lv_%d";
const char *KEY_ENDLESS_COUNT = "key_endless_count";
const char *KEY_BOX_GUIDED = "key_box_guided";
const char *KEY_WEAPON_GUIDED = "key_weapon_guided";
const char *KEY_ENDLESS_GUIDED = "key_endless_guided";

/*first sotryidx, second storycheckpos, last commondid*/
const char* KEY_STORY_GOLDGET = "key_story_gold_%d_%d_%d";

const char* KEY_BGL_LV = "key_bglvl";
const char* KEY_DAILY_REWARD = "key_daily_reward_%d";
const char* KEY_TIPS_GUIDE = "key_tips_guide_%d";

const char* KEY_TILI_BUY_TIMES = "key_tili_buy_times";
const char* KEY_TILI_REWARD_TIMES = "key_tili_reward_times_%d";

const char* KEY_REFASH_TIMES = "key_refash_times";

const char* KEY_SHOWSTORYTEXT_IDX = "key_showstorytext_idx";
const char* KEY_CURGIFT_TYPE = "key_curgift_type";
const char* KEY_DAYGIFT_TIMES = "key_daygift_times";

const char* KEY_BOXGET_IDX = "key_boxget_idx_%d";

const char* KEY_POP_BGL = "key_pop_bgl";

//add by jiang 2016/8/4
const char *KEY_MAGPIE_CUR_FINISHED_GOALNUM = "key_magpie_cur_num_%d";
const char *KEY_MAGPIE_ISGOTREWARD = "key_magpie_isgot_%d";

const char* KEY_DAY_PASS = "key_day_pass";
const char* KEY_DAY_WENPONUP = "key_day_wenponup";
const char* KEY_DAY_USEITEMS = "key_day_useitems";
const char* KEY_DAY_SPBOSS = "key_day_spboss";
const char* KEY_DAY_TA = "key_day_ta";
const char* KEY_DAY_BUYITEM = "key_day_buyitem";
const char* KEY_DAY_BGL = "key_day_bgl";
const char* KEY_DAY_OPEN_GOLD_BOX = "key_day_opengoldbox";
const char* KEY_DAY_GET_TILI = "key_day_gettili";
const char* KEY_DAY_BUY_ZUANSHI = "key_day_buyzuanshi";
const char* KEY_DAY_DUNGEON = "key_day_dungeontimes";
const char* KEY_DAY_ACTIVE_REWARD = "key_day_activereward_%d";

const char* KEY_NEWMAPOPEN = "key_new_map_open_%d";

const char* KEY_ONLINETIME_REWARD_STAGE = "key_online_time_rewardstage_%d";
const char* KEY_ONLINE_COUNTDOWN_TIME = "key_online_countdown_time";

const char* KEY_COMSTAGES_REBORNIDX = "key_comstages_rebornidx";
const char* KEY_BOSSSTAGES_REBORNIDX = "key_bossstages_rebornidx";

const char* KEY_DUNGEON_TIMES = "key_dungeon_times_%d";

const char* KEY_FIRST_OPEN_MAP = "key_first_open_newmap";

const char* KEY_WEEK_REWEAT_REWARD_IDX = "key_week_repeat_reward_idx";
const char* KEY_WEEK_WEAPONPART_REWARD_IDX = "key_week_wenponparts_%d";

const char* KEY_FIRSTPLAY_DAYINYEAR = "key_firstplay_dayinyear";

const char* KEY_POP_BOSSGUIDE = "key_pop_bossguide";

const char* KEY_PAY_RMB = "key_pay_rmb";

const char* KEY_ACTIVITY_REWARDGET = "key_activity_%s_rewardget_%d";
const char* g_activityKeys[] = { "moonactivity" };
const int g_activityRewardsNum[] = {4};

UserData::UserData()
{

}


UserData::~UserData()
{

}


UserData* UserData::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new UserData();
		_instance->loadUserData();	
	}

	return _instance;
}

void UserData::freeDB()
{
	if (GLLocalStorage::getIsInitialized() == 1)
	{
		GLLocalStorage::glLocalStorageFree();
	}
}

int UserData::getIsFirstPlay(bool dbclose)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}
	int value = getIntFromDBForKey("firstentergame", 1);

	if (dbclose && GLLocalStorage::getIsInitialized() == 1)
	{
		GLLocalStorage::glLocalStorageFree();
	}

	return value;
}

void UserData::setIsFirstPlay(int idx, bool dbclose)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}
	setIntFromDBForKey("firstentergame", idx);

	if (dbclose && GLLocalStorage::getIsInitialized() == 1)
	{
		GLLocalStorage::glLocalStorageFree();
	}
}


bool UserData::loadUserData()
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}

	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			_userInfo._arrItemConfig[j][i] = getIntFromDBForKey(String::createWithFormat("itemposconfig_%d_page_%d", i, j)->getCString(), -1);
			_userInfoBak._arrItemConfig[j][i] = _userInfo._arrItemConfig[j][i];
		}
	}
	
	for (int i = 0; i < s_equipcount; i++)
	{
		_userInfo._arrEquipConfig[i] = getIntFromDBForKey(String::createWithFormat("equipconfig_%d", i)->getCString(), -1);
		_userInfoBak._arrEquipConfig[i] = _userInfo._arrEquipConfig[i];
	}

	for (int i = 0; i < s_skillNum; i++)
	{		
		_userInfo._arrSkillEquipConfig[i] = getIntFromDBForKey(String::createWithFormat("skillconfig_%d", i)->getCString(), 0);
		_userInfoBak._arrSkillEquipConfig[i] = _userInfo._arrSkillEquipConfig[i];
	}

	for (int i = 0; i < s_weaponNum; i++)
	{
		_userInfo._arrWeaponEquipConfig[i] = getIntFromDBForKey(String::createWithFormat("weaponconfig_%d", i)->getCString(), 0);
		_userInfoBak._arrWeaponEquipConfig[i] = _userInfo._arrWeaponEquipConfig[i];
	}

	for (int i = 0; i < sizeof(_userInfo._playerCurExp)/sizeof(_userInfo._playerCurExp[0]); i++)
	{
		_userInfo._playerCurExp[i] = getIntFromDBForKey(String::createWithFormat("playerexp_%d", i)->getCString(), 0);
		_userInfoBak._playerCurExp[i] = _userInfo._playerCurExp[i];

		_userInfo._playerCurLv[i] = getIntFromDBForKey(String::createWithFormat("playerlv_%d", i)->getCString(), 0);
		_userInfoBak._playerCurLv[i] = _userInfo._playerCurLv[i];
	}

	for (int i = 0; i < sizeof(_userInfo._countDownTime)/sizeof(_userInfo._countDownTime[0]); i++)
	{
		_userInfo._countDownTime[i] = getIntFromDBForKey(String::createWithFormat("boxcountdown_%d", i)->getCString(), -1111);
		_userInfoBak._countDownTime[i] = _userInfo._countDownTime[i];

		_userInfo._countDownStartTime[i] = getIntFromDBForKey(String::createWithFormat("boxcountdownstart_%d", i)->getCString(), -1111);
		_userInfoBak._countDownStartTime[i] = _userInfo._countDownStartTime[i];
	}

	for (int i = 0; i < sizeof(_userInfo._storenew)/sizeof(_userInfo._storenew[0]); i++)
	{
		_userInfo._storenew[i] = getStringFromDBForKey(String::createWithFormat(KEY_STORE, i)->getCString());
		_userInfoBak._storenew[i] = _userInfo._storenew[i];
	}

	for (int i = 0; i < sizeof(_userInfo._boxtype) / sizeof(_userInfo._boxtype[0]); i++)
	{
		_userInfo._boxtype[i].boxidx = getIntFromDBForKey(String::createWithFormat("boxlayertype_%d", i)->getCString(), -1);
		_userInfo._boxtype[i].boxlv = getIntFromDBForKey(String::createWithFormat("boxlayertype_lv_%d", i)->getCString(), 0);
		_userInfoBak._boxtype[i] = _userInfo._boxtype[i];
	}

	for (int i = 0; i < sizeof(_userInfo._boxtypeStage) / sizeof(_userInfo._boxtypeStage[0]); i++)
	{
		_userInfo._boxtypeStage[i] = getIntFromDBForKey(String::createWithFormat("boxlayertypestage_%d", i)->getCString(), 0);
		_userInfoBak._boxtypeStage[i] = _userInfo._boxtypeStage[i];
	}

	for (int i = 0; i < sizeof(_userInfo._isPassBoss) / sizeof(_userInfo._isPassBoss[0]); i++)
	{
		_userInfo._isPassBoss[i] = getIntFromDBForKey(String::createWithFormat(KEY_PASSBOSS_STAGE, i)->getCString(), 0);
		_userInfoBak._isPassBoss[i] = _userInfo._isPassBoss[i];
	}

	for (int i = 0; i < sizeof(_userInfo._storePageOneItemIdx) / sizeof(_userInfo._storePageOneItemIdx[0]); i++)
	{
		_userInfo._storePageOneItemIdx[i] = getIntFromDBForKey(String::createWithFormat(KEY_STORE_PAGE, i)->getCString(), -1);
		_userInfoBak._storePageOneItemIdx[i] = _userInfo._storePageOneItemIdx[i];

		_userInfo._storePageOneItemStage[i] = getIntFromDBForKey(String::createWithFormat(KEY_STORE_STAGE, i)->getCString(), 1);
		_userInfoBak._storePageOneItemStage[i] = _userInfo._storePageOneItemStage[i];
	}

	for (int i = 0; i < sizeof(_userInfo._wenponlv)/sizeof(_userInfo._wenponlv[0]); i++)
	{
		_userInfo._wenponlv[i] = getIntFromDBForKey(String::createWithFormat(KEY_W_LV, i)->getCString(), 0);
		_userInfoBak._wenponlv[i] = _userInfo._wenponlv[i];

		auto value = getStringFromDBForKey(String::createWithFormat(KEY_CURATTACK, i)->getCString());
		if (value.compare("") != 0)
		{
			std::vector<std::string> out;
			ToolsUtil::split(value, "-", out);
			AttactInfo_T temp;
			temp.attack= atoi(out[0].c_str());
			temp.def = atoi(out[1].c_str());
			temp.dpAdd = atoi(out[2].c_str());
			temp.hpAdd = atoi(out[3].c_str());

			_userInfo._wenponCurAttack[i] = temp;
		}
		else
		{
			auto& att = ParamMgr::getInstance()->getWeaponVector();
			AttactInfo_T temp;
			temp.attack = 0;
			temp.def = 0;
			temp.dpAdd = 0;
			temp.hpAdd = 0;

			_userInfo._wenponCurAttack[i] = temp;
		}

		_userInfoBak._wenponCurAttack[i] = _userInfo._wenponCurAttack[i];

		_userInfo._wenponPartNum[i] = getIntFromDBForKey(String::createWithFormat(KEY_PARTS_NUM, i)->getCString(), 0);
		_userInfoBak._wenponPartNum[i] = _userInfo._wenponPartNum[i];
	}

	for (int i = 0; i < sizeof(_userInfo._dungeonTimes) / sizeof(_userInfo._dungeonTimes[0]); i++)
	{
		_userInfo._dungeonTimes[i] = getIntFromDBForKey(String::createWithFormat(KEY_DUNGEON_TIMES, i)->getCString(), 0);
		_userInfoBak._dungeonTimes[i] = _userInfo._dungeonTimes[i];
	}

	_userInfo._isUnlockbox = getIntFromDBForKey("isunlockbox", -1);
	_userInfoBak._isUnlockbox = _userInfo._isUnlockbox;
	
	_userInfo._winTimes = getIntFromDBForKey(KEY_WIN_TIMES, 0);
	_userInfoBak._winTimes = _userInfo._winTimes;

	_userInfo._goldnum = String::createWithFormat("%d", getIntFromDBForKey(itemid_gold, 0))->getCString();
	_userInfoBak._goldnum = _userInfo._goldnum;

	_userInfo._crystalnum = String::createWithFormat("%d", getIntFromDBForKey(itemid_crystal, 0))->getCString();
	_userInfoBak._crystalnum = _userInfo._crystalnum;
	
	_userInfo._storyfileIdx = getIntFromDBForKey(KEY_STORY_IDX, 0);
	_userInfoBak._storyfileIdx = _userInfo._storyfileIdx;

	_userInfo._playerpos = getIntFromDBForKey(KEY_PLAYERPOS, 0);
	_userInfoBak._playerpos = _userInfo._playerpos;

	_userInfo._lastPassMaxIdx = getIntFromDBForKey(KEY_MAXPOS, 0);
	_userInfoBak._lastPassMaxIdx = _userInfo._lastPassMaxIdx;

	_userInfo._guideState = getIntFromDBForKey(KEY_GUIDE, 0);
	_userInfoBak._guideState = _userInfo._guideState;

	_userInfo._guanqiaIsPlayingIdx = getIntFromDBForKey(KEY_PLAYING_QUANQIA, 0);
	_userInfoBak._guanqiaIsPlayingIdx = _userInfo._guanqiaIsPlayingIdx;

	_userInfo._bgllv = getIntFromDBForKey(KEY_BGL_LV, 0);
	_userInfoBak._bgllv = _userInfo._bgllv;

	_userInfo._tiliBuyTimes = getIntFromDBForKey(KEY_TILI_BUY_TIMES, 3);
	_userInfoBak._tiliBuyTimes = _userInfo._tiliBuyTimes;

	USERDATA_LOAD_INT(_userInfo._popBossGuide, _userInfoBak._popBossGuide, KEY_POP_BOSSGUIDE, 1);
	USERDATA_LOAD_INT(_userInfo._firstPlayDayInYear, _userInfoBak._firstPlayDayInYear, KEY_FIRSTPLAY_DAYINYEAR, 0);

	USERDATA_LOAD_INT(_userInfo._weekRepeatRewardIdx, _userInfoBak._weekRepeatRewardIdx, KEY_WEEK_REWEAT_REWARD_IDX, 0);

	USERDATA_LOAD_INT(_userInfo._firstOpenNewMap, _userInfoBak._firstOpenNewMap, KEY_FIRST_OPEN_MAP, 0);
	USERDATA_LOAD_INT(_userInfo._lastShowStoryTextIdx, _userInfoBak._lastShowStoryTextIdx, KEY_SHOWSTORYTEXT_IDX, -1);
	USERDATA_LOAD_INT(_userInfo._curGiftType, _userInfoBak._curGiftType, KEY_CURGIFT_TYPE, 0);
	USERDATA_LOAD_INT(_userInfo._tili, _userInfoBak._tili, KEY_TILI, 0);
	USERDATA_LOAD_INT(_userInfo._dayGiftBuyTimes, _userInfoBak._dayGiftBuyTimes, KEY_DAYGIFT_TIMES, 0);
	USERDATA_LOAD_INT(_userInfo._havePopBgl, _userInfoBak._havePopBgl, KEY_POP_BGL, 0);

	USERDATA_LOAD_INT(_userInfo._refashTimes, _userInfoBak._refashTimes, KEY_REFASH_TIMES, 0);

	USERDATA_LOAD_STRING(_userInfo._account, _userInfoBak._account, KEY_ACCOUNT);
	USERDATA_LOAD_STRING(_userInfo._nickName, _userInfoBak._nickName, KEY_NICK);
	USERDATA_LOAD_INT(_userInfo._headId, _userInfoBak._headId, KEY_HEADID, 0);
	USERDATA_LOAD_INT(_userInfo._totalFloor, _userInfoBak._totalFloor, KEY_TOTAL_FLOOR, 0);
	USERDATA_LOAD_INT(_userInfo._weeklyScore, _userInfoBak._weeklyScore, KEY_WEEKLY_SCORE, 0);
	USERDATA_LOAD_INT(_userInfo._lastSettleDay, _userInfoBak._lastSettleDay, KEY_LAST_SETTLE, 0);
	USERDATA_LOAD_INT(_userInfo._lastLoginDay, _userInfoBak._lastLoginDay, KEY_LAST_LOGIN, 0);
	USERDATA_LOAD_INT(_userInfo._endlessCount, _userInfoBak._endlessCount, KEY_ENDLESS_COUNT, 0);

	USERDATA_LOAD_INT(_userInfo._daypass, _userInfoBak._daypass, KEY_DAY_PASS, 0);
	USERDATA_LOAD_INT(_userInfo._dayWenponUp, _userInfoBak._dayWenponUp, KEY_DAY_WENPONUP, 0);
	USERDATA_LOAD_INT(_userInfo._dayUseItems, _userInfoBak._dayUseItems, KEY_DAY_USEITEMS, 0);
	USERDATA_LOAD_INT(_userInfo._dayPassSpBoss, _userInfoBak._dayPassSpBoss, KEY_DAY_SPBOSS, 0);
	USERDATA_LOAD_INT(_userInfo._dayEnterTa, _userInfoBak._dayEnterTa, KEY_DAY_TA, 0);
	USERDATA_LOAD_INT(_userInfo._dayBuyItems, _userInfoBak._dayBuyItems, KEY_DAY_BUYITEM, 0);
	USERDATA_LOAD_INT(_userInfo._dayUseBGL, _userInfoBak._dayUseBGL, KEY_DAY_BGL, 0);
	USERDATA_LOAD_INT(_userInfo._dayOpenGoldBox, _userInfoBak._dayOpenGoldBox, KEY_DAY_OPEN_GOLD_BOX, 0);
	USERDATA_LOAD_INT(_userInfo._dayGetTili, _userInfoBak._dayGetTili, KEY_DAY_GET_TILI, 0);
	USERDATA_LOAD_INT(_userInfo._dayBuyZuanshi, _userInfoBak._dayBuyZuanshi, KEY_DAY_BUY_ZUANSHI, 0);
	USERDATA_LOAD_INT(_userInfo._dayDungeon, _userInfoBak._dayDungeon, KEY_DAY_DUNGEON, 0);

	USERDATA_LOAD_INT(_userInfo._onlineTimeCountdown, _userInfoBak._onlineTimeCountdown, KEY_ONLINE_COUNTDOWN_TIME, -1);

	USERDATA_LOAD_INT(_userInfo._comStagesRebornPayIdx, _userInfoBak._comStagesRebornPayIdx, KEY_COMSTAGES_REBORNIDX, 0);
	USERDATA_LOAD_INT(_userInfo._bossStagesRebornPayIdx, _userInfoBak._bossStagesRebornPayIdx, KEY_BOSSSTAGES_REBORNIDX, 0);

	USERDATA_LOAD_INT(_userInfo._payRmb, _userInfoBak._payRmb, KEY_PAY_RMB, 0);

	for (int i = 0; i < 3; i++)
	{
		auto strKey = __String::createWithFormat(KEY_WEEK_WEAPONPART_REWARD_IDX, i);
		USERDATA_LOAD_INT(_userInfo._arrWeekWenponPartIdx[i], _userInfoBak._arrWeekWenponPartIdx[i], strKey->getCString(), -1);
	}

	for (int i = 0; i < 3; i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_DAY_ACTIVE_REWARD, i);
		USERDATA_LOAD_INT(_userInfo._dayActiveReward[i], _userInfoBak._dayActiveReward[i], strExchangeKey->getCString(), 0);
	}

	for (int i = 0; i < 10; i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_NEWMAPOPEN, i);
		USERDATA_LOAD_INT(_userInfo._newMapConfig[i], _userInfoBak._newMapConfig[i], strExchangeKey->getCString(), 0);
	}
	
	for (int i = 0; i < sizeof(_userInfo._exchangeState)/ sizeof(_userInfo._exchangeState[0]); i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_EXCHANGE, i);
		USERDATA_LOAD_INT(_userInfo._exchangeState[i], _userInfoBak._exchangeState[i], strExchangeKey->getCString(), 0);
	}

	for (int i = 0; i < 3; i++)
	{
		USERDATA_LOAD_INT(_userInfo._tiliDayReward[i], _userInfoBak._tiliDayReward[i], String::createWithFormat(KEY_TILI_REWARD_TIMES, i)->getCString(), 0);
	}

	for (int i = 0; i < ParamData::ONLINE_TIME_REWARD_NUM; i++)
	{
		auto strKey = __String::createWithFormat(KEY_ONLINETIME_REWARD_STAGE, i);
		USERDATA_LOAD_INT(_userInfo._onlineTimeReward[i], _userInfoBak._onlineTimeReward[i], strKey->getCString(), 0);
	}

	for (int i = 0; i < ParamData::ACHIEVE_TOTAL_COUNT; i++)
	{
		auto strCountKey = __String::createWithFormat(KEY_ACHIEVE_COUNT, i);
		auto strLvKey = __String::createWithFormat(KEY_ACHIEVE_LV, i);

		USERDATA_LOAD_INT(_userInfo._arrAchieveInfo[i].curCount, _userInfoBak._arrAchieveInfo[i].curCount, strCountKey->getCString(), 0);
		USERDATA_LOAD_INT(_userInfo._arrAchieveInfo[i].curLv, _userInfoBak._arrAchieveInfo[i].curLv, strLvKey->getCString(), 0);
	}
	//add by jiang 2016/8/4  
	for (int i = 0; i < ParamData::MagPie_TOTAL_TASKS_NUM; i++)
	{
		auto strCountKey = __String::createWithFormat(KEY_MAGPIE_CUR_FINISHED_GOALNUM, i);
		auto strIsGotKey = __String::createWithFormat(KEY_MAGPIE_ISGOTREWARD, i);

		USERDATA_LOAD_INT(_userInfo._arrUserMagPieInfo[i].curFinishedGoalNum, _userInfoBak._arrUserMagPieInfo[i].curFinishedGoalNum, strCountKey->getCString(), 0);
		USERDATA_LOAD_BOOL(_userInfo._arrUserMagPieInfo[i].isGotReward, _userInfoBak._arrUserMagPieInfo[i].isGotReward, strIsGotKey->getCString(), false);
	}
	//

	USERDATA_LOAD_INT(_userInfo._isBoxGuided, _userInfoBak._isBoxGuided, KEY_BOX_GUIDED, 0);
	USERDATA_LOAD_INT(_userInfo._isWeaponGuided, _userInfoBak._isWeaponGuided, KEY_WEAPON_GUIDED, 0);
	USERDATA_LOAD_INT(_userInfo._isEndlessGuided, _userInfoBak._isEndlessGuided, KEY_ENDLESS_GUIDED, 0);

	for (int i = 0; i < sizeof(_userInfo._arrIsGotDailyReward) / sizeof(_userInfo._arrIsGotDailyReward[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_DAILY_REWARD, i);
		
		USERDATA_LOAD_BOOL(_userInfo._arrIsGotDailyReward[i], _userInfoBak._arrIsGotDailyReward[i], strKey->getCString(), false);
	}

	for (int i = 0; i < sizeof(_userInfo._arrIsTipsGuided) / sizeof(_userInfo._arrIsTipsGuided[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_TIPS_GUIDE, i);

		USERDATA_LOAD_BOOL(_userInfo._arrIsTipsGuided[i], _userInfoBak._arrIsTipsGuided[i], strKey->getCString(), false);
	}

	for (int i = 0; i < sizeof(_userInfo._isBoxGet) / sizeof(_userInfo._isBoxGet[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_BOXGET_IDX, i);

		USERDATA_LOAD_INT(_userInfo._isBoxGet[i], _userInfoBak._isBoxGet[i], strKey->getCString(), 0);
	}

	for (int i = 0; i < sizeof(g_activityKeys) / sizeof(g_activityKeys[0]); i ++)
	{
		for (int j = 0; j < g_activityRewardsNum[i]; j++)
		{
			auto key = String::createWithFormat(KEY_ACTIVITY_REWARDGET, g_activityKeys[i], j);
			_userInfo._activityMap.insert(std::pair<std::string, int>(key->getCString(), getIntFromDBForKey(key->getCString(), 0)));
			_userInfoBak._activityMap.insert(std::pair<std::string, int>(key->getCString(), getIntFromDBForKey(key->getCString(), 0)));
		}
	}

	if (GLLocalStorage::getIsInitialized() == 1)
	{
		GLLocalStorage::glLocalStorageFree();
	}
	return true;
}


bool UserData::saveUserData(bool close)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}

	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (_userInfoBak._arrItemConfig[j][i] != _userInfo._arrItemConfig[j][i])
			{
				setIntFromDBForKey(String::createWithFormat("itemposconfig_%d_page_%d", i, j)->getCString(), _userInfo._arrItemConfig[j][i]);
				_userInfoBak._arrItemConfig[j][i] = _userInfo._arrItemConfig[j][i];
			}
		}
	}

	for(int i = 0; i < s_equipcount; i++)
	{
		if (_userInfo._arrEquipConfig[i] != _userInfoBak._arrEquipConfig[i])
		{
			setIntFromDBForKey(String::createWithFormat("equipconfig_%d", i)->getCString(), _userInfo._arrEquipConfig[i]);
			_userInfoBak._arrEquipConfig[i] = _userInfo._arrEquipConfig[i];
		}	
	}

	for (int i = 0; i < s_skillNum; i++)
	{
		if (_userInfo._arrSkillEquipConfig[i] != _userInfoBak._arrSkillEquipConfig[i])
		{
			setIntFromDBForKey(String::createWithFormat("skillconfig_%d", i)->getCString(), _userInfo._arrSkillEquipConfig[i]);
			_userInfoBak._arrSkillEquipConfig[i] = _userInfo._arrSkillEquipConfig[i];
		}
	}

	for (int i = 0; i < s_weaponNum; i++)
	{
		if (_userInfo._arrWeaponEquipConfig[i] != _userInfoBak._arrWeaponEquipConfig[i])
		{
			setIntFromDBForKey(String::createWithFormat("weaponconfig_%d", i)->getCString(), _userInfo._arrWeaponEquipConfig[i]);
			_userInfoBak._arrWeaponEquipConfig[i] = _userInfo._arrWeaponEquipConfig[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._playerCurLv)/sizeof(_userInfo._playerCurLv[0]); i++)
	{
		if (_userInfoBak._playerCurExp[i] != _userInfo._playerCurExp[i])
		{
			setIntFromDBForKey(String::createWithFormat("playerexp_%d", i)->getCString(), _userInfo._playerCurExp[i]);
			_userInfoBak._playerCurExp[i] = _userInfo._playerCurExp[i];
		}

		if (_userInfoBak._playerCurLv[i] != _userInfo._playerCurLv[i])
		{
			setIntFromDBForKey(String::createWithFormat("playerlv_%d", i)->getCString(), _userInfo._playerCurLv[i]);
			_userInfoBak._playerCurLv[i] = _userInfo._playerCurLv[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._countDownTime)/sizeof(_userInfo._countDownTime[0]); i++)
	{
		if (_userInfo._countDownTime[i] != _userInfoBak._countDownTime[i])
		{
			setIntFromDBForKey(String::createWithFormat("boxcountdown_%d", i)->getCString(), _userInfo._countDownTime[i]);
			_userInfoBak._countDownTime[i] = _userInfo._countDownTime[i];
		}

		if (_userInfo._countDownStartTime[i] != _userInfoBak._countDownStartTime[i])
		{
			setIntFromDBForKey(String::createWithFormat("boxcountdownstart_%d", i)->getCString(), _userInfo._countDownStartTime[i]);
			_userInfoBak._countDownStartTime[i] = _userInfo._countDownStartTime[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._storenew)/sizeof(_userInfo._storenew[0]); i++)
	{
		if (_userInfoBak._storenew[i] != _userInfo._storenew[i])
		{
			setStringFromDBForKey(String::createWithFormat(KEY_STORE, i)->getCString(), _userInfo._storenew[i]);
			_userInfoBak._storenew[i] = _userInfo._storenew[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._boxtype) / sizeof(_userInfo._boxtype[0]); i++)
	{
		if (_userInfoBak._boxtype[i].boxidx != _userInfo._boxtype[i].boxidx)
		{
			setIntFromDBForKey(String::createWithFormat("boxlayertype_%d", i)->getCString(), _userInfo._boxtype[i].boxidx);
			setIntFromDBForKey(String::createWithFormat("boxlayertype_lv_%d", i)->getCString(), _userInfo._boxtype[i].boxlv);
			_userInfoBak._boxtype[i] = _userInfo._boxtype[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._boxtypeStage) / sizeof(_userInfo._boxtypeStage[0]); i++)
	{
		if (_userInfoBak._boxtypeStage[i] != _userInfo._boxtypeStage[i])
		{
			setIntFromDBForKey(String::createWithFormat("boxlayertypestage_%d", i)->getCString(), _userInfo._boxtypeStage[i]);
			_userInfoBak._boxtypeStage[i] = _userInfo._boxtypeStage[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._isPassBoss) / sizeof(_userInfo._isPassBoss[0]); i++)
	{
		if (_userInfoBak._isPassBoss[i] != _userInfo._isPassBoss[i])
		{
			setIntFromDBForKey(String::createWithFormat(KEY_PASSBOSS_STAGE, i)->getCString(), _userInfo._isPassBoss[i]);
			_userInfoBak._isPassBoss[i] = _userInfo._isPassBoss[i];
		}
	}

	for (int i = 0; i < sizeof(_userInfo._wenponlv)/sizeof(_userInfo._wenponlv[0]); i++)
	{
		if (_userInfoBak._wenponlv[i] != _userInfo._wenponlv[i])
		{
			setIntFromDBForKey(String::createWithFormat(KEY_W_LV, i)->getCString(), _userInfo._wenponlv[i]);
			_userInfoBak._wenponlv[i] = _userInfo._wenponlv[i];
		}
	
		if ((_userInfoBak._wenponCurAttack[i] == _userInfo._wenponCurAttack[i]) == false)
		{
			auto value = String::createWithFormat("%d-%d-%d-%d", _userInfo._wenponCurAttack[i].attack, _userInfo._wenponCurAttack[i].def, _userInfo._wenponCurAttack[i].dpAdd, _userInfo._wenponCurAttack[i].hpAdd);
			setStringFromDBForKey(String::createWithFormat(KEY_CURATTACK, i)->getCString(), value->getCString());
			_userInfoBak._wenponCurAttack[i] = _userInfo._wenponCurAttack[i];
		}

		if (_userInfoBak._wenponPartNum[i] != _userInfo._wenponPartNum[i])
		{
			setIntFromDBForKey(String::createWithFormat(KEY_PARTS_NUM, i)->getCString(), _userInfo._wenponPartNum[i]);
			_userInfoBak._wenponPartNum[i] = _userInfo._wenponPartNum[i];
		}
		
	}

	for (int i = 0; i < sizeof(_userInfo._storePageOneItemIdx) / sizeof(_userInfo._storePageOneItemIdx[0]); i++)
	{
		if (_userInfoBak._storePageOneItemIdx[i] != _userInfo._storePageOneItemIdx[i])
		{
			setIntFromDBForKey(String::createWithFormat(KEY_STORE_PAGE, i)->getCString(), _userInfo._storePageOneItemIdx[i]);
			_userInfoBak._storePageOneItemIdx[i] = _userInfo._storePageOneItemIdx[i];
		}

		if (_userInfoBak._storePageOneItemStage[i] != _userInfo._storePageOneItemStage[i])
		{
			setIntFromDBForKey(String::createWithFormat(KEY_STORE_STAGE, i)->getCString(), _userInfo._storePageOneItemStage[i]);
			_userInfoBak._storePageOneItemStage[i] = _userInfo._storePageOneItemStage[i];
		}
	}

	if (_userInfoBak._winTimes != _userInfo._winTimes)
	{
		setIntFromDBForKey(KEY_WIN_TIMES, _userInfo._winTimes);
		_userInfoBak._winTimes = _userInfo._winTimes;
	}

	if (_userInfoBak._isUnlockbox != _userInfo._isUnlockbox)
	{
		setIntFromDBForKey("isunlockbox", _userInfo._isUnlockbox);
		_userInfoBak._isUnlockbox = _userInfo._isUnlockbox;
	}


	if (_userInfoBak._goldnum .compare( _userInfo._goldnum) != 0)
	{
		setIntFromDBForKey(itemid_gold, atoi(_userInfo._goldnum.c_str()));
		_userInfoBak._goldnum = _userInfo._goldnum;
	}

	if (_userInfoBak._crystalnum.compare( _userInfo._crystalnum) != 0)
	{
		setIntFromDBForKey(itemid_crystal, atoi(_userInfo._crystalnum.c_str()));
		_userInfoBak._crystalnum = _userInfo._crystalnum;
	}
	
	if (_userInfo._playerpos != _userInfoBak._playerpos)
	{
		setIntFromDBForKey(KEY_PLAYERPOS, _userInfo._playerpos);
		_userInfoBak._playerpos = _userInfo._playerpos;
	}

	if (_userInfo._storyfileIdx != _userInfoBak._storyfileIdx)
	{
		setIntFromDBForKey(KEY_STORY_IDX, _userInfo._storyfileIdx);
		_userInfoBak._storyfileIdx = _userInfo._storyfileIdx;
	}

	if(_userInfo._lastPassMaxIdx != _userInfoBak._lastPassMaxIdx)
	{
		setIntFromDBForKey(KEY_MAXPOS, _userInfo._lastPassMaxIdx);
		_userInfoBak._lastPassMaxIdx = _userInfo._lastPassMaxIdx;
	}

	if (_userInfoBak._guideState != _userInfo._guideState)
	{
		setIntFromDBForKey(KEY_GUIDE, _userInfo._guideState);
		_userInfoBak._guideState = _userInfo._guideState;
	}

	if (_userInfoBak._guanqiaIsPlayingIdx != _userInfo._guanqiaIsPlayingIdx)
	{
		setIntFromDBForKey(KEY_PLAYING_QUANQIA, _userInfo._guanqiaIsPlayingIdx);
		_userInfoBak._guanqiaIsPlayingIdx = _userInfo._guanqiaIsPlayingIdx;
	}

	if (_userInfoBak._bgllv != _userInfo._bgllv)
	{
		setIntFromDBForKey(KEY_BGL_LV, _userInfo._bgllv);
		_userInfoBak._bgllv = _userInfo._bgllv;
	}

	for (int i = 0; i < sizeof(_userInfo._dungeonTimes) / sizeof(_userInfo._dungeonTimes[0]); i++)
	{
		setIntFromDBForKey(String::createWithFormat(KEY_DUNGEON_TIMES, i)->getCString(), _userInfo._dungeonTimes[i]);
		_userInfoBak._dungeonTimes[i] = _userInfo._dungeonTimes[i];
	}

	USERDATA_SAVE_INT(_userInfo._popBossGuide, _userInfoBak._popBossGuide, KEY_POP_BOSSGUIDE);
	USERDATA_SAVE_INT(_userInfo._firstPlayDayInYear, _userInfoBak._firstPlayDayInYear, KEY_FIRSTPLAY_DAYINYEAR);

	USERDATA_SAVE_INT(_userInfo._weekRepeatRewardIdx, _userInfoBak._weekRepeatRewardIdx, KEY_WEEK_REWEAT_REWARD_IDX);
	USERDATA_SAVE_INT(_userInfo._firstOpenNewMap, _userInfoBak._firstOpenNewMap, KEY_FIRST_OPEN_MAP);
	USERDATA_SAVE_INT(_userInfo._havePopBgl, _userInfoBak._havePopBgl, KEY_POP_BGL);
	USERDATA_SAVE_INT(_userInfo._dayGiftBuyTimes, _userInfoBak._dayGiftBuyTimes, KEY_DAYGIFT_TIMES);
	USERDATA_SAVE_INT(_userInfo._curGiftType, _userInfoBak._curGiftType, KEY_CURGIFT_TYPE);
	USERDATA_SAVE_INT(_userInfo._lastShowStoryTextIdx, _userInfoBak._lastShowStoryTextIdx, KEY_SHOWSTORYTEXT_IDX);
	USERDATA_SAVE_INT(_userInfo._tili, _userInfoBak._tili, KEY_TILI);
	USERDATA_SAVE_INT(_userInfo._refashTimes, _userInfoBak._refashTimes, KEY_REFASH_TIMES);
	USERDATA_SAVE_INT(_userInfo._tiliBuyTimes, _userInfoBak._tiliBuyTimes, KEY_TILI_BUY_TIMES);
	USERDATA_SAVE_STRING(_userInfo._account, _userInfoBak._account, KEY_ACCOUNT);
	USERDATA_SAVE_STRING(_userInfo._nickName, _userInfoBak._nickName, KEY_NICK);
	USERDATA_SAVE_INT(_userInfo._headId, _userInfoBak._headId, KEY_HEADID);
	USERDATA_SAVE_INT(_userInfo._totalFloor, _userInfoBak._totalFloor, KEY_TOTAL_FLOOR);
	USERDATA_SAVE_INT(_userInfo._weeklyScore, _userInfoBak._weeklyScore, KEY_WEEKLY_SCORE);
	USERDATA_SAVE_INT(_userInfo._lastSettleDay, _userInfoBak._lastSettleDay, KEY_LAST_SETTLE);
	USERDATA_SAVE_INT(_userInfo._lastLoginDay, _userInfoBak._lastLoginDay, KEY_LAST_LOGIN);
	USERDATA_SAVE_INT(_userInfo._endlessCount, _userInfoBak._endlessCount, KEY_ENDLESS_COUNT);

	USERDATA_SAVE_INT(_userInfo._daypass, _userInfoBak._daypass, KEY_DAY_PASS);
	USERDATA_SAVE_INT(_userInfo._dayWenponUp, _userInfoBak._dayWenponUp, KEY_DAY_WENPONUP);
	USERDATA_SAVE_INT(_userInfo._dayUseItems, _userInfoBak._dayUseItems, KEY_DAY_USEITEMS);
	USERDATA_SAVE_INT(_userInfo._dayPassSpBoss, _userInfoBak._dayPassSpBoss, KEY_DAY_SPBOSS);
	USERDATA_SAVE_INT(_userInfo._dayEnterTa, _userInfoBak._dayEnterTa, KEY_DAY_TA);
	USERDATA_SAVE_INT(_userInfo._dayBuyItems, _userInfoBak._dayBuyItems, KEY_DAY_BUYITEM);
	USERDATA_SAVE_INT(_userInfo._dayUseBGL, _userInfoBak._dayUseBGL, KEY_DAY_BGL);
	USERDATA_SAVE_INT(_userInfo._dayOpenGoldBox, _userInfoBak._dayOpenGoldBox, KEY_DAY_OPEN_GOLD_BOX);
	USERDATA_SAVE_INT(_userInfo._dayGetTili, _userInfoBak._dayGetTili, KEY_DAY_GET_TILI);
	USERDATA_SAVE_INT(_userInfo._dayBuyZuanshi, _userInfoBak._dayBuyZuanshi, KEY_DAY_BUY_ZUANSHI);
	USERDATA_SAVE_INT(_userInfo._dayDungeon, _userInfoBak._dayDungeon, KEY_DAY_DUNGEON);

	USERDATA_SAVE_INT(_userInfo._onlineTimeCountdown, _userInfoBak._onlineTimeCountdown, KEY_ONLINE_COUNTDOWN_TIME);

	USERDATA_SAVE_INT(_userInfo._comStagesRebornPayIdx, _userInfoBak._comStagesRebornPayIdx, KEY_COMSTAGES_REBORNIDX);
	USERDATA_SAVE_INT(_userInfo._bossStagesRebornPayIdx, _userInfoBak._bossStagesRebornPayIdx, KEY_BOSSSTAGES_REBORNIDX);
	
	USERDATA_SAVE_INT(_userInfo._payRmb, _userInfoBak._payRmb, KEY_PAY_RMB);

	for (int i = 0; i < 3; i++)
	{
		
		auto strKey = __String::createWithFormat(KEY_WEEK_WEAPONPART_REWARD_IDX, i);
		USERDATA_SAVE_INT(_userInfo._arrWeekWenponPartIdx[i], _userInfoBak._arrWeekWenponPartIdx[i], strKey->getCString());
		
	}

	for (int i = 0; i < 3; i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_DAY_ACTIVE_REWARD, i);
		USERDATA_SAVE_INT(_userInfo._dayActiveReward[i], _userInfoBak._dayActiveReward[i], strExchangeKey->getCString());
	}

	for (int i = 0; i < 10; i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_NEWMAPOPEN, i);
		USERDATA_SAVE_INT(_userInfo._newMapConfig[i], _userInfoBak._newMapConfig[i], strExchangeKey->getCString());
	}
	
	for (int i = 0; i < sizeof(_userInfo._exchangeState) / sizeof(_userInfo._exchangeState[0]); i++)
	{
		auto strExchangeKey = __String::createWithFormat(KEY_EXCHANGE, i);
		USERDATA_SAVE_INT(_userInfo._exchangeState[i], _userInfoBak._exchangeState[i], strExchangeKey->getCString());
	}

	for (int i = 0; i < 3; i++)
	{
		USERDATA_SAVE_INT(_userInfo._tiliDayReward[i], _userInfoBak._tiliDayReward[i], String::createWithFormat(KEY_TILI_REWARD_TIMES, i)->getCString());
	}

	for (int i = 0; i < ParamData::ONLINE_TIME_REWARD_NUM; i++)
	{
		auto strKey = __String::createWithFormat(KEY_ONLINETIME_REWARD_STAGE, i);
		USERDATA_SAVE_INT(_userInfo._onlineTimeReward[i], _userInfoBak._onlineTimeReward[i], strKey->getCString());
	}

	for (int i = 0; i < ParamData::ACHIEVE_TOTAL_COUNT; i++)
	{
		auto strCountKey = __String::createWithFormat(KEY_ACHIEVE_COUNT, i);
		auto strLvKey = __String::createWithFormat(KEY_ACHIEVE_LV, i);

		USERDATA_SAVE_INT(_userInfo._arrAchieveInfo[i].curCount, _userInfoBak._arrAchieveInfo[i].curCount, strCountKey->getCString());
		USERDATA_SAVE_INT(_userInfo._arrAchieveInfo[i].curLv, _userInfoBak._arrAchieveInfo[i].curLv, strLvKey->getCString());
	}
	// add by jiang 2016/8/4  
	for (int i = 0; i < ParamData::MagPie_TOTAL_TASKS_NUM; i++)
	{
		auto strCountKey = __String::createWithFormat(KEY_MAGPIE_CUR_FINISHED_GOALNUM, i);
		auto strIsGotKey = __String::createWithFormat(KEY_MAGPIE_ISGOTREWARD, i);

		USERDATA_SAVE_INT(_userInfo._arrUserMagPieInfo[i].curFinishedGoalNum, _userInfoBak._arrUserMagPieInfo[i].curFinishedGoalNum, strCountKey->getCString());
		USERDATA_SAVE_BOOL(_userInfo._arrUserMagPieInfo[i].isGotReward, _userInfoBak._arrUserMagPieInfo[i].isGotReward, strIsGotKey->getCString());
	}
	//

	USERDATA_SAVE_INT(_userInfo._isBoxGuided, _userInfoBak._isBoxGuided, KEY_BOX_GUIDED);
	USERDATA_SAVE_INT(_userInfo._isWeaponGuided, _userInfoBak._isWeaponGuided, KEY_WEAPON_GUIDED);
	USERDATA_SAVE_INT(_userInfo._isEndlessGuided, _userInfoBak._isEndlessGuided, KEY_ENDLESS_GUIDED);

	for (int i = 0; i < sizeof(_userInfo._arrIsGotDailyReward) / sizeof(_userInfo._arrIsGotDailyReward[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_DAILY_REWARD, i);

		USERDATA_SAVE_BOOL(_userInfo._arrIsGotDailyReward[i], _userInfoBak._arrIsGotDailyReward[i], strKey->getCString());
	}

	for (int i = 0; i < sizeof(_userInfo._arrIsTipsGuided) / sizeof(_userInfo._arrIsTipsGuided[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_TIPS_GUIDE, i);

		USERDATA_SAVE_BOOL(_userInfo._arrIsTipsGuided[i], _userInfoBak._arrIsTipsGuided[i], strKey->getCString());
	}

	for (int i = 0; i < sizeof(_userInfo._isBoxGet) / sizeof(_userInfo._isBoxGet[0]); i++)
	{
		auto strKey = __String::createWithFormat(KEY_BOXGET_IDX, i);
		USERDATA_SAVE_INT(_userInfo._isBoxGet[i], _userInfoBak._isBoxGet[i], strKey->getCString());
	}

	
	for (auto& temp : _userInfo._activityMap)
	{
		auto lastvalue = _userInfoBak._activityMap.find(temp.first);
		if (lastvalue != _userInfoBak._activityMap.end() && temp.second != (*lastvalue).second)
		{
			setIntFromDBForKey(temp.first, temp.second);
			(*lastvalue).second = temp.second;
		}
		
	}

	if (close && GLLocalStorage::getIsInitialized() == 1)
	{
		GLLocalStorage::glLocalStorageFree();
	}
	

	return true;
}


void UserData::resetUserData()
{

}

int UserData::getIsSkillEquip(int playeridx, int skillid, bool dbclose)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}
	auto key = String::createWithFormat("isskillequip_%d_%d", playeridx, skillid)->getCString();
	int value = getIntFromDBForKey(key, 0);

	if (GLLocalStorage::getIsInitialized() == 1 && dbclose == true)
	{
		GLLocalStorage::glLocalStorageFree();
	}

	return value;
}

void UserData::setSkillEquip(int playeridx, int skillid, bool isqueip, bool dbclose)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}

	if (isqueip)
	{
		auto key = String::createWithFormat("isskillequip_%d_%d", playeridx, skillid)->getCString();
		setIntFromDBForKey(key, 1);
		//_arrSkillEquipConfig[skillid - SkillControl::s_skillStartIdx] = 1;
	}
	else
	{
		auto key = String::createWithFormat("isskillequip_%d_%d", playeridx,  skillid)->getCString();
		setIntFromDBForKey(key, 0);
		//_arrSkillEquipConfig[skillid - SkillControl::s_skillStartIdx] = 0;
	}

	if (GLLocalStorage::getIsInitialized() == 1 && dbclose == true)
	{
		GLLocalStorage::glLocalStorageFree();
	}
}

int UserData::getIsWeaponEquip(int weaponid)
{
	return _userInfo._arrWeaponEquipConfig[weaponid - WeaponControl::s_weaponStartIdx];
}

void UserData::setWeaponEquip(int weaponid, bool isqueip)
{
	if (isqueip)
	{
		_userInfo._arrWeaponEquipConfig[weaponid - WeaponControl::s_weaponStartIdx] = 1;
	}
	else
	{
		_userInfo._arrWeaponEquipConfig[weaponid - WeaponControl::s_weaponStartIdx] = 0;
	}
}

int UserData::getIsBuySkill(int skillid)
{
	return 1;
}

int UserData::getIsBuyWeapon(int weaponid)
{
	return 1;
}

void UserData::giveItem(std::string key, int num, bool dbflag)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}

	if (key.compare(itemid_gold) == 0)
	{
		giveGold(num);
	}
	else if (key.compare(itemid_crystal) == 0)
	{
		giveCrystal(num);
	}
	else if (key.compare(KEY_TILI) == 0)
	{
		giveTili(num);
	}
	else
	{
		int curnum = getIntFromDBForKey(key, 0);
		setIntFromDBForKey(key, curnum + num);
	}

	saveUserData(false);

	if (GLLocalStorage::getIsInitialized() == 1 && dbflag == true)
	{
		GLLocalStorage::glLocalStorageFree();
	}

}

int UserData::getItemBalance(std::string key, bool dbflag)
{
	if (GLLocalStorage::getIsInitialized() == 0)
	{
		GLLocalStorage::glLocalStorageInit(FileUtils::getInstance()->getWritablePath() + "test.db");
	}
	int value = getIntFromDBForKey(key, 0);

	if (key.compare(itemid_gold) == 0)
	{
		value = getGoldNum();
	}
	else if (key.compare(itemid_crystal) == 0)
	{
		value = getCrystalNum();
	}

	if (GLLocalStorage::getIsInitialized() == 1 && dbflag == true)
	{
		GLLocalStorage::glLocalStorageFree();
	}

	return value;
}

int UserData::getPlayerCurExp(int idx)
{
	return _userInfo._playerCurExp[idx];
}

bool UserData::addPlayerCurExp(int idx, int exp)
{
	bool lvchange = false;
	auto temp = ParamMgr::getInstance()->getPlayerInfo(idx, _userInfo._playerCurLv[idx]);
	auto curexp = _userInfo._playerCurExp[idx] + exp;
	while (curexp >= temp.exp && _userInfo._playerCurLv[idx] < ParamMgr::getInstance()->getPlayerMaxLv())
	{
		curexp = curexp - temp.exp;
		_userInfo._playerCurLv[idx]++;
		temp = ParamMgr::getInstance()->getPlayerInfo(idx, _userInfo._playerCurLv[idx]);
		lvchange = true;

		if (MainLayer::getCurMainLayer())
		{
			MainLayer::getCurMainLayer()->updataTopInfo();
		}
	}
	_userInfo._playerCurExp[idx] = curexp;


	if (MainLayer::getCurMainLayer())
	{
		MainLayer::getCurMainLayer()->updataPlayersInfo(idx);
	}
	return lvchange;
}

int UserData::getPlayerCurLv(int idx)
{
	return _userInfo._playerCurLv[idx];
}

void UserData::setPlayerCurLv(int idx, int lv)
{
	_userInfo._playerCurLv[idx] = lv;
}

int UserData::getGoldNum()
{
	return atoi(_userInfo._goldnum.c_str());
}

void UserData::giveGold(int num)
{
	int curnum = atoi(_userInfo._goldnum.c_str()) + num;
	_userInfo._goldnum = String::createWithFormat("%d", curnum)->getCString();
	//_userInfo._goldnum = _userInfo._goldnum + num;

	if (MainLayer::getCurMainLayer())
	{
		MainLayer::getCurMainLayer()->updataGold();
	}
	if (num > 0)
	{
		AchieveMgr::getInstance()->addAchieveCount(AchieveMgr::_AchieveIdx::GOLD, num, false);
	}
}

int UserData::getCrystalNum()
{
	return atoi(_userInfo._crystalnum.c_str());
}

void UserData::giveCrystal(int num)
{
	int curnum = atoi(_userInfo._crystalnum.c_str()) + num;
	_userInfo._crystalnum = String::createWithFormat("%d", curnum)->getCString();

	//_userInfo._crystalnum = _userInfo._crystalnum + num;

	if (MainLayer::getCurMainLayer())
	{
		MainLayer::getCurMainLayer()->updataGold();
	}
}

void UserData::setMaxPos(int pos)
{
	if (_userInfo._lastPassMaxIdx < pos)
	{
		_userInfo._lastPassMaxIdx = pos;
	}
}

void UserData::setIsNeedGuide(bool isNeedGuide)
{
	if (isNeedGuide)
	{
		_userInfo._guideState = 0;
	}
	else
	{
		_userInfo._guideState = 1;
	}
}

bool UserData::addBoxConfig(int config, int lv)
{
	for (auto& temp : _userInfo._boxtype)
	{
		if (temp.boxidx == -1)
		{
			temp.boxidx = config;
			temp.boxlv = lv;
			return true;
		}
	}

	CCLOG("box is full");
	return false;
}

void UserData::giveTili(int num)
{
	bool needShowMax = false;
	if (num <= 0)
	{
		_userInfo._tili += num;
	}
	else
	{
		if (_userInfo._tili <= 50)
		{
			_userInfo._tili += num;
			if (_userInfo._tili > ParamData::MAX_TILI)
			{
				_userInfo._tili = ParamData::MAX_TILI;
				needShowMax = true;
			}
		}
		else
		{
			_userInfo._tili += num;
		}
	}
		

	if (MainLayer::getCurMainLayer())
	{
		MainLayer::getCurMainLayer()->getTiliMaxIcon()->setVisible(needShowMax);
		MainLayer::getCurMainLayer()->updataGold();
	}	
}

void UserData::addWeekRewardIdx()
{
	
	_userInfo._weekRepeatRewardIdx++;
	if (_userInfo._weekRepeatRewardIdx >= 7)
	{
		_userInfo._weekRepeatRewardIdx = 0;
	}
	
}

void UserData::setActivityReward(std::string key, int idx, int value)
{
	auto truekey = String::createWithFormat(KEY_ACTIVITY_REWARDGET, key.c_str(), idx);
	_userInfo._activityMap.at(truekey->getCString()) = value;
	int a = 0;
}

int UserData::getActivityReward(std::string key, int idx)
{
	auto truekey = String::createWithFormat(KEY_ACTIVITY_REWARDGET, key.c_str(), idx);
	return _userInfo._activityMap.at(truekey->getCString());
}

int UserData::getIntFromDBForKey(std::string key, int defule)
{
	std::string temp("");
	GLLocalStorage::glLocalStorageGetItem(key, &temp);
	if (temp.empty())
	{
		
	}
	else
	{
		defule = atoi(temp.c_str());
	}
	return defule;
}

std::string UserData::getStringFromDBForKey(std::string key)
{
	std::string temp;
	GLLocalStorage::glLocalStorageGetItem(key, &temp);
	return temp;
}

void UserData::setIntFromDBForKey(std::string key, int data)
{
	GLLocalStorage::glLocalStorageSetItem(key, String::createWithFormat("%d", data)->getCString());
}

void UserData::setStringFromDBForKey(std::string key, std::string data)
{
	GLLocalStorage::glLocalStorageSetItem(key, data);
}

void UserData::setBoolFromDBForKey(std::string key, bool data)
{
	if (data)
	{
		GLLocalStorage::glLocalStorageSetItem(key, "true");
	}
	else
	{
		GLLocalStorage::glLocalStorageSetItem(key, "false");
	}
}

bool UserData::getBoolFromDBForKey(std::string key, bool defaultvalue)
{
	bool ret = defaultvalue;
	std::string temp;
	GLLocalStorage::glLocalStorageGetItem(key, &temp);

	if ("true" == temp)
	{
		ret = true;
	}
	else if ("false" == temp)
	{
		ret = false;
	}

	return ret;
}



//USERDATA_DEFINE_IDX(int, Integer, ItemBagPosIdx, "bagitem_", -1);