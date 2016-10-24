#ifndef _USERDATA_H_
#define _USERDATA_H_

#include "cocos2d.h"
#include "ParamData.h"
#include "BagItemControl.h"
#include "../Scenes/TimeCountDownNode.h"

USING_NS_CC;
using namespace std;

#define USERDATA_DECLAR(type, funName)\
	void set##funName(type);\
	type get##funName();\


#define USERDATA_DECLAR_IDX(type, funName)\
	void set##funName(type, std::string );\
	type get##funName( std::string);\

#define  USERDATA_DEFINE(type, userdefaulttype,funName, key,  defaultvalue)\
type UserData::get##funName()\
{\
	return UserDefault::getInstance()->get##userdefaulttype##ForKey(key, defaultvalue);\
}\
\
\
void UserData::set##funName(type a)\
{\
	UserDefault::getInstance()->set##userdefaulttype##ForKey(key, a);\
}\

#define  USERDATA_DEFINE_IDX(type, userdefaulttype,funName, key,  defaultvalue)\
type UserData::get##funName(std::string idx)\
{\
	return UserDefault::getInstance()->get##userdefaulttype##ForKey((key + idx).c_str(), defaultvalue);\
}\
\
\
void UserData::set##funName(type a, std::string idx)\
{\
	UserDefault::getInstance()->set##userdefaulttype##ForKey((key + idx).c_str(), a);\
}\

#define USERDATA_LOAD_INT(param, paramBak, key, defaultvalue) \
param = getIntFromDBForKey(key, defaultvalue); \
paramBak = param;

#define USERDATA_LOAD_STRING(param, paramBak, key) \
param = getStringFromDBForKey(key); \
paramBak = param;

#define USERDATA_SAVE_INT(param, paramBak, key) \
if (param != paramBak) \
{\
	setIntFromDBForKey(key, param);\
	paramBak = param;\
}

#define USERDATA_SAVE_STRING(param, paramBak, key) \
if (param != paramBak) \
{\
	setStringFromDBForKey(key, param);\
	paramBak = param;\
}

#define USERDATA_LOAD_BOOL(param, paramBak, key, defaultvalue) \
param = getBoolFromDBForKey(key, defaultvalue); \
paramBak = param;

#define USERDATA_SAVE_BOOL(param, paramBak, key) \
if (param != paramBak) \
{\
	setBoolFromDBForKey(key, param);\
	paramBak = param;\
}


class UserData
{
public:
	~UserData();
	static UserData* getInstance();

	void freeDB();

	int getIsFirstPlay(bool dbclose);
	void setIsFirstPlay(int first, bool dbclose);

	bool loadUserData(void);
	bool saveUserData(bool clsoe = true);
	void resetUserData(void);
	
	int getItemPosIdx(int posidx, int pageidx) { return _userInfo._arrItemConfig[pageidx][posidx]; }
	void setItemPosIdx(int posidx, int itemid, int pageidx) { _userInfo._arrItemConfig[pageidx][posidx] = itemid; }

	int getIsSkillEquip(int playeridx, int skillid, bool dbcloseflag = true);
	void setSkillEquip(int playeridx, int skillid, bool isqueip, bool dbcloseflag = true);

	int getIsWeaponEquip(int weaponid);
	void setWeaponEquip(int weaponid, bool isqueip);

	int getIsBuySkill(int skillid);
	int getIsBuyWeapon(int weaponid);
	
	void giveItem(std::string key, int num, bool dbflag  = true);
	int getItemBalance(std::string key, bool dbflag = true);

	int getPlayerCurExp(int idx);
	bool addPlayerCurExp(int idx, int exp);

	int getPlayerCurLv(int idx);
	void setPlayerCurLv(int idx, int lv);

	int getGoldNum();
	void giveGold(int num);

	int getCrystalNum();
	void giveCrystal(int num);
	
	int getStoryFileIdx() { return _userInfo._storyfileIdx; }
	void setStoryFileIdx(int idx) { _userInfo._storyfileIdx = idx; }

	int getPlayerPos() { return _userInfo._playerpos; }
	void setPlayerPos(int pos) { _userInfo._playerpos = pos; }

	int getMaxPos() { return _userInfo._lastPassMaxIdx; }
	void setMaxPos(int pos);

	int getBoxCountDownTime(int i) { return _userInfo._countDownTime[i]; }
	void setBoxCountDownTime(int i, int time) { _userInfo._countDownTime[i] = time; }

	int getBoxCountDownStartTime(int i) { return _userInfo._countDownStartTime[i]; }
	void setBoxCountDownStartTime(int i, int time) { _userInfo._countDownStartTime[i] = time; }
	
	std::string getStoreItemid(int i) { return _userInfo._storenew[i]; }
	void setStoreItemid(int i, const string& itmeid) { _userInfo._storenew[i] = itmeid; }

	bool isNeedGuide() { return 0 == _userInfo._guideState; }
	void setIsNeedGuide(bool isNeedGuide);

	int getEquipConfig(int idx) { return _userInfo._arrEquipConfig[idx]; }
	void setEquipConfig(int idx, int itemid) { _userInfo._arrEquipConfig[idx] = itemid; }	

	const BoxInStoreConfig_T& getBoxLayerConfig(int i) { return _userInfo._boxtype[i]; };
	void setBoxLayerConfig(int i, const BoxInStoreConfig_T config) { _userInfo._boxtype[i] = config; };
	bool addBoxConfig(int config, int lv);

	int getBoxLayerStage(int i) { return _userInfo._boxtypeStage[i]; };
	void setBoxLayerStage(int i, int config) { _userInfo._boxtypeStage[i] = config; };

	int getIsBoxUnlock() { return _userInfo._isUnlockbox; }
	void setBoxUnlock(int isunlock) { _userInfo._isUnlockbox = isunlock; }

	int getWeaponLv(int idx) { return _userInfo._wenponlv[idx]; }
	void setWeaponLv(int idx, int lv) { _userInfo._wenponlv[idx] = lv; }

	const AttactInfo_T& getWeaponAttack(int idx) { return _userInfo._wenponCurAttack[idx]; }
	void setWeaponAttack(int idx, const AttactInfo_T& lv) { _userInfo._wenponCurAttack[idx] = lv; }

	const AttactInfo_T& getWeaponExtAtt(int idx) { return _userInfo._wenponExternAtt[idx]; }
	void setWeaponExtAtt(int idx, const AttactInfo_T& att) { _userInfo._wenponExternAtt[idx] = att; }

	AttactInfo_T getTrueWeaponAttack(int idx);

	int getWeaponParts(int idx) { return _userInfo._wenponPartNum[idx]; }
	void setWeaponPartsNum(int idx, int num) { _userInfo._wenponPartNum[idx] = num; }
	void addWeaponPartsNum(int wenpartsid, int numchange) { _userInfo._wenponPartNum[wenpartsid - 1500] += numchange; }

	int getStorePageItemid(int idx) { return _userInfo._storePageOneItemIdx[idx]; }
	void setStorePageItemid(int idx, int itemidx) { _userInfo._storePageOneItemIdx[idx] = itemidx; }

	int getStorePageStage(int idx) { return _userInfo._storePageOneItemStage[idx]; }
	void setStorePageStage(int idx, int stage) { _userInfo._storePageOneItemStage[idx] = stage; }

	const string &getAccount() { return _userInfo._account; }
	void setAccount(const char *strAcc) { _userInfo._account = strAcc; }

	const string &getNickName() { return _userInfo._nickName; }
	void setNickName(const char *strNick) { _userInfo._nickName = strNick; }

	int getHeadId() { return _userInfo._headId; }
	void setHeadId(int headId) { _userInfo._headId = headId; }

	int getTotalFloor() { return _userInfo._totalFloor; }
	void setTotalFloor(int totalFloor) { _userInfo._totalFloor = totalFloor; }

	int getWeeklyScore() { return _userInfo._weeklyScore; }
	void setWeeklyScore(int score) { _userInfo._weeklyScore = score; }

	int getLastSettleDay() { return _userInfo._lastSettleDay; }
	void setLastSettleDay(int day) { _userInfo._lastSettleDay = day; }

	int getLastLoginDay() { return _userInfo._lastLoginDay; }
	void setLastLoginDay(int day) { _userInfo._lastLoginDay = day; }

	int getTili() { return _userInfo._tili; }
	void setTili(int tili) { _userInfo._tili = tili; }
	void giveTili(int num);

	int getWinTime() { return _userInfo._winTimes; }
	void setWinTimes(int time) { _userInfo._winTimes = time; }
	void addWinTime(int times) 
	{
		if (_userInfo._boxtypeStage[4] != 1)
		{
			_userInfo._winTimes += times;
		}
	}

	bool getIsExchange(int codeIdx) { return _userInfo._exchangeState[codeIdx] == 1; }
	void setIsExchange(int codeIdx, bool isExchange) {
		if (isExchange)
		{
			_userInfo._exchangeState[codeIdx] = 1;
		}
		else
		{
			_userInfo._exchangeState[codeIdx] = 0;
		}
	}

	const UserAchieveInfo_T &getAchieveInfo(int idx) {
		return _userInfo._arrAchieveInfo[idx];
	}
	void setAchieveCount(int idx, int count) {
		_userInfo._arrAchieveInfo[idx].curCount = count;
	}
	void setAchieveLv(int idx, int lv) {
		_userInfo._arrAchieveInfo[idx].curLv = lv;
	}
	//ad by jiang 206/8/4
	const _UserMagPieInfo &getUserMagPieInfo(int idx)
	{
		return _userInfo._arrUserMagPieInfo[idx];
	}

	void setIsGotMagPieReward(int idx)
	{
		_userInfo._arrUserMagPieInfo[idx].isGotReward = true;
	}

	void setMagPieGoalNum(int idx, int count) {
		_userInfo._arrUserMagPieInfo[idx].curFinishedGoalNum = count;
	}
	//

	int getIsBossPass(int posidx) {
		return _userInfo._isPassBoss[posidx];
	}

	int getPlayingGuanqiaIdx() { return _userInfo._guanqiaIsPlayingIdx; }
	void setPlayingGuanqiaIdx(int posidx) { _userInfo._guanqiaIsPlayingIdx = posidx; }

	void setBossPass(int posidx, int ispossvaleue) { _userInfo._isPassBoss[posidx] = ispossvaleue; }

	int getEndlessCount() { return _userInfo._endlessCount; }
	void setEndlessCount(int endlessCount) { _userInfo._endlessCount = endlessCount; }

	bool getIsBoxGuided() { return _userInfo._isBoxGuided == 1; }
	void setIsBoxGuided(bool isGuided) {
		if (isGuided)
		{
			_userInfo._isBoxGuided = 1;
		}
		else
		{
			_userInfo._isBoxGuided = 0;
		}
	}

	bool getWeaponGuided() { return _userInfo._isWeaponGuided == 1; }
	void setWeaponGuided(bool isGuided) {
		if (isGuided)
		{
			_userInfo._isWeaponGuided = 1;
		}
		else
		{
			_userInfo._isWeaponGuided = 0;
		}
	}

	bool getEndlessGuided() { return _userInfo._isEndlessGuided == 1; }
	void setEndlessGuided(bool isGuided) {
		if (isGuided)
		{
			_userInfo._isEndlessGuided = 1;
		}
		else
		{
			_userInfo._isEndlessGuided = 0;
		}
	}

	void setBGLLv(int lv) { _userInfo._bgllv = lv; }
	int getBGLLv() { return _userInfo._bgllv; }

	bool getIsGotDailyReward(int dayIdx) { return _userInfo._arrIsGotDailyReward[dayIdx]; }
	void setIsGotDailyReward(int dayIdx, bool isGot) { _userInfo._arrIsGotDailyReward[dayIdx] = isGot; }

	bool getIsTipsGuided(int idx) { return _userInfo._arrIsTipsGuided[idx]; }
	void setIsTipsGuided(int idx, bool isGuided) { _userInfo._arrIsTipsGuided[idx] = isGuided; }

	void setTiliBuyTimes(int times) {_userInfo._tiliBuyTimes = times;}
	int getTiliBuyTimes() {return _userInfo._tiliBuyTimes;}

	int getTiliRewardTimes(int idx) { return _userInfo._tiliDayReward[idx]; }
	void setTiliRewardTimes(int idx, int times) { _userInfo._tiliDayReward[idx] = times; }

	int getRefashTimes() {return  _userInfo._refashTimes;}
	void setRefashTimes(int times) { _userInfo._refashTimes = times; }

	int getShowTextIdx() { return _userInfo._lastShowStoryTextIdx; }
	void setShowTextIdx(int idx) { _userInfo._lastShowStoryTextIdx = idx; }

	int getCurGiftType() { return _userInfo._curGiftType; }
	void setCurGiftType(int gifttype) { _userInfo._curGiftType = gifttype; }

	int getCurDayGiftTimes() { return _userInfo._dayGiftBuyTimes; }
	void setCurDayGiftTimes(int gifttype) { _userInfo._dayGiftBuyTimes = gifttype; }

	int getBoxGet(int idx) { return _userInfo._isBoxGet[idx]; }
	void setBoxGet(int idx, int get) { _userInfo._isBoxGet[idx] = get; }

	int getHavePopBgl() { return _userInfo._havePopBgl; }
	void setHavePopBgl(int havepop) { _userInfo._havePopBgl = havepop; }

	int& getDayPass() { return _userInfo._daypass; }
	int& getDayWenponUp() { return _userInfo._dayWenponUp; }
	int& getDayUseItems() { return _userInfo._dayUseItems; }
	int& getDaySpBoss() { return _userInfo._dayPassSpBoss; }
	int& getDayTa() { return _userInfo._dayEnterTa; }
	int& getDayBuyItems() { return _userInfo._dayBuyItems; }
	int& getDayUseBGL() { return _userInfo._dayUseBGL; }
	int& getDayOpenGoldBox() { return _userInfo._dayOpenGoldBox; }
	int& getDayGetTilii() { return _userInfo._dayGetTili; }
	int& getDayBuyZuanshi() { return _userInfo._dayBuyZuanshi; }
	int& getDayDungeon() { return _userInfo._dayDungeon; }

	int getDayActiveReward(int idx) { return _userInfo._dayActiveReward[idx]; }
	void setDayActiveReward(int idx, int get) { _userInfo._dayActiveReward[idx] = get; }

	int getNewMapConfig(int idx) { return _userInfo._newMapConfig[idx]; }
	void setNewMapConfig(int idx, int config) { _userInfo._newMapConfig[idx] = config; }

	int getOnlineTimeReward(int idx) { return _userInfo._onlineTimeReward[idx]; }
	void setOnlineReward(int idx, int stage) { _userInfo._onlineTimeReward[idx] = stage; }

	int getOnlineCountdownTime() { return _userInfo._onlineTimeCountdown; }
	void setOnlineCountdownTime(int time) { _userInfo._onlineTimeCountdown = time; }

	int getComStagesRebornIdx() { return _userInfo._comStagesRebornPayIdx; }
	void setComStagesRebornIdx(int idx) { _userInfo._comStagesRebornPayIdx = idx; }

	int getBossStagesRebornIdx() { return _userInfo._bossStagesRebornPayIdx; }
	void setBossStagesRebornIdx(int idx) { _userInfo._bossStagesRebornPayIdx = idx; }

	int getDungeonTime(int idx) { return _userInfo._dungeonTimes[idx]; }
	void setDungeonTime(int idx, int times) { _userInfo._dungeonTimes[idx] = times; }

	int getFirstOpenNewMap() { return _userInfo._firstOpenNewMap; }
	void setFirstOpenNewMap( int times) { _userInfo._firstOpenNewMap = times; }

	int getWeekRewardIdx() { return _userInfo._weekRepeatRewardIdx; }
	void setWeekRewardIdx(int idx) { _userInfo._weekRepeatRewardIdx = idx; }
	void addWeekRewardIdx();

	int getWeekWenponPartsIdx(int idx) { return _userInfo._arrWeekWenponPartIdx[idx]; }
	void setWeekWenponPartsIdx(int idx, int wenponpart) { _userInfo._arrWeekWenponPartIdx[idx] = wenponpart; }
	
	int getFirstPlayDayInYear() { return _userInfo._firstPlayDayInYear; }
	void setFirstPlayDayInYear(int day) { _userInfo._firstPlayDayInYear = day; }

	int getPopGuideBoss() { return _userInfo._popBossGuide;}
	void setPopGuideBoss(int flag) { _userInfo._popBossGuide = flag;}

	void setActivityReward(std::string key, int idx, int value);
	int getActivityReward(std::string key, int idx);

	void addPayRmb(int num) { _userInfo._payRmb += num; }
	int getPayRmb() { return _userInfo._payRmb; }
	void setPayRmb(int num) { _userInfo._payRmb = num; }

	void addPayRmbGuoqing(int num) { _userInfo._payrmbGuoqing += num; }
	int getPayRmbGuoqing() { return _userInfo._payrmbGuoqing; }
	void setPayRmbGuoqing(int num) { _userInfo._payrmbGuoqing = num; }

	int getPlayerIsOn(int playeridx) { return _userInfo._arrPlayersIsOn[playeridx]; }
	void setPlayerIsOn(int playeridx, int ison) {_userInfo._arrPlayersIsOn[playeridx] = ison;}

	int getVipTargetDay() { return _userInfo._vipTargetDay; }
	void setVipTargetDay(int day) { _userInfo._vipTargetDay = day; }

	int getVipDayRewardCrystal() { return _userInfo._vipRewardCrystal; }
	void setVipDayRewardCrystal(int stage) {_userInfo._vipRewardCrystal = stage; }

	int getMainSceneGiftIdx() { return _userInfo._curMainSceneShowGiftIdx; }
	void setMainSceneGiftIdx(int curIdx) { _userInfo._curMainSceneShowGiftIdx = curIdx; }

	int getHaveJinhuaBGL() { return _userInfo._haveJinhuaBGL; }
	void setHaveJinhuaBGL(int haveJinhua) { _userInfo._haveJinhuaBGL = haveJinhua; }
private:
	int getIntFromDBForKey(std::string key, int defult);
	std::string getStringFromDBForKey(std::string key);

	void setIntFromDBForKey(std::string key, int data);
	void setStringFromDBForKey(std::string key, std::string data);

	void setBoolFromDBForKey(std::string key, bool data);
	bool getBoolFromDBForKey(std::string key, bool defaultvalue);

	UserData();
	static UserData* _instance;

	const static int s_equipcount = 4;

	typedef struct _UserDataInfo {

		int _arrEquipConfig[s_equipcount];
		int _arrItemConfig[g_pagecount][g_onepagetnum];

		int _arrSkillEquipConfig[ParamData::SKILL_COUNT];

		int _arrWeaponEquipConfig[ParamData::WENPON_COUNT];

		int _playerCurExp[ParamData::ROLE_COUNT];

		int _playerCurLv[ParamData::ROLE_COUNT];

		//int _goldnum;
		//int _crystalnum;
		std::string _goldnum;
		std::string _crystalnum;

		int _storyfileIdx;

		int _playerpos;

		int _lastPassMaxIdx;

		//0 1 2:shuaxing  3:store shauxing 4:unlockbox
		int _countDownTime[TimeTypeEnum::TIME_TIMENODENUM];
		int _countDownStartTime[TimeTypeEnum::TIME_TIMENODENUM];

		int _guideState;	//0.need guide

		std::string _storenew[6];

		int _isUnlockbox;

		int _winTimes;
		BoxInStoreConfig_T _boxtype[4];
		//0~3 is nor box; 4 is topwin get box
		int _boxtypeStage[5];

		int _wenponlv[ParamData::WENPON_COUNT];

		int _wenponPartNum[ParamData::WENPON_COUNT];

		int _storePageOneItemIdx[9];
		int _storePageOneItemStage[9];

		AttactInfo_T _wenponCurAttack[ParamData::WENPON_COUNT];
		AttactInfo_T _wenponExternAtt[ParamData::WENPON_COUNT];

		int _tili;
		std::string _account;
		std::string _nickName;
		int _headId;
		int _totalFloor;
		int _weeklyScore;
		int _lastSettleDay;
		int _lastLoginDay;
		int _exchangeState[ExchangeType_E::EXCHANGE_COUNT];	//0.not exchange
		UserAchieveInfo_T _arrAchieveInfo[ParamData::ACHIEVE_TOTAL_COUNT];
		UserMagPieInfo_T _arrUserMagPieInfo[ParamData::MagPie_TOTAL_TASKS_NUM];
		int _endlessCount;
		int _isBoxGuided;
		int _isWeaponGuided;
		int _isEndlessGuided;

		int _isPassBoss[81];

		int _guanqiaIsPlayingIdx;
		int _bgllv;
		bool _arrIsGotDailyReward[7];
		bool _arrIsTipsGuided[7];
		int _tiliBuyTimes;
		int _tiliDayReward[3];
		int _refashTimes;
		int _lastShowStoryTextIdx;

		int _curGiftType;
		int _dayGiftBuyTimes;

		int _isBoxGet[81];
		int _havePopBgl;

		int _daypass;
		int _dayWenponUp;
		int _dayUseItems;
		int _dayPassSpBoss;
		int _dayEnterTa;
		int _dayBuyItems;
		int _dayUseBGL;
		int _dayOpenGoldBox;
		int _dayGetTili;
		int _dayBuyZuanshi;
		int _dayDungeon;

		int _dayActiveReward[3];

		int _newMapConfig[10];
		int _onlineTimeReward[ParamData::ONLINE_TIME_REWARD_NUM];
		int _onlineTimeCountdown;

		int _comStagesRebornPayIdx;
		int _bossStagesRebornPayIdx;

		int _dungeonTimes[3];
		int _firstOpenNewMap;
		int _weekRepeatRewardIdx;
		int _arrWeekWenponPartIdx[3];
		int _firstPlayDayInYear;
		int _popBossGuide;
		int _payRmb;
		int _arrPlayersIsOn[ParamData::ROLE_COUNT];
		std::map<std::string, int> _activityMap;

		int _payrmbGuoqing;

		int _vipTargetDay;
		int _vipRewardCrystal;

		/*0 is stand gift, 1 is sp time gift*/
		int _curMainSceneShowGiftIdx;

		int _haveJinhuaBGL;
	}UserDataInfo_T;

	UserDataInfo_T _userInfo;
	UserDataInfo_T _userInfoBak;
};
#endif