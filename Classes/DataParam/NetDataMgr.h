#ifndef __NETDATA_MGR_H__
#define __NETDATA_MGR_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "spine/Json.h"
#include "ParamData.h"
#include "GameUtils.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;


/*************************rank info begin*************************/
//simba http://192.168.1.254:8082
const char *const STR_SERVICER_URL = "http://mk.shininghunter.com:9810/gamerank";//"http://192.168.2.211:9800/gamerank";// "http://mk.shininghunter.com:9800/gamerank";//
const char *const STR_SERVICER_URL_EX = "http://mk.shininghunter.com:9820/gamecode";// "http://192.168.2.211:9801/gamecode";

const char *const STR_QUERY_SELF = "queryrole";
const char *const STR_UPDATE_ROLE = "updaterole";
const char *const STR_TMP_SORT = "tmpsort";
const char *const STR_REAL_SORT = "real";
const char *const STR_SETTLE_RANK = "settle";


const char *const STR_QUERY_TOP = "querytoprole";
const char *const STR_UPDATE_NICK = "updatenick";

const char *const STR_ACC_KEY = "account";
const char *const STR_NICK_KEY = "nick";
const char *const STR_GAMEID_KEY = "gameid";
const char *const STR_VER_KEY = "version";
const char *const STR_CHNLID_KEY = "chnlid";
const char *const STR_ROLEID_KEY = "roleid";
const char *const STR_LV_KEY = "level";
const char *const STR_SCORE_KEY = "score";
const char *const STR_FORTUNE_KEY = "fortune";
const char *const STR_DATE_KEY = "date";
const char *const STR_INDEX_KEY = "index";
const char *const STR_VALUE_KEY = "value";
const char *const STR_RANKID_KEY = "node";
const char *const STR_RUN_KEY = "run";
const char *const STR_DIAMOND_KEY = "diamond";
const char *const STR_RANK_KEY = "rank";
const char *const STR_RANK_2_KEY = "rank_2";
const char *const STR_AWARD_KEY = "award";

const char *const STR_GAME_ID = "2";

const int RANK_ID_SCORE = 1001;
const int RANK_ID_FORTUNE = 1002;

const char *const STR_RANK_VER = "3";
/*************************rank info end*************************/


class NetDataMgr: public Ref
{
public:
	static const int RANK_ID_SCORE = 1001;
	static const int RANK_ID_FORTUNE = 1002;


	~NetDataMgr();

	static NetDataMgr *getInstance(void);
    void init();

	const RankRoleInfo_T *getSelfInfo(void);
	const RankRealInfo_T *getRealRank(void);
	const RankSettleInfo_T *getSettleRank(void);
	int getSelfScore(void);
	int getSelfFortune(void);
	const string &getAccount(void);
	const string &getNick(void);
	int getSelfScoreRank(void);
	int getSelfFortuneRank(void);
	int getSelfSettleScoreRank(void);
	int getSelfSettleFortuneRank(void);
	const RankBaseData_T *getRealScoreTop(void);
	const RankBaseData_T *getRealFortuneTop(void);
	const RankBaseData_T *getSettleScoreTop(void);
	const RankBaseData_T *getSettleFortuneTop(void);

//	void queryRole(const ccHttpRequestCallback& callback);
	void updateRole(const ccHttpRequestCallback& callback);
	void postTmpSort(int nDist, const ccHttpRequestCallback& callback);
//	void queryRealSort(const ccHttpRequestCallback& callback);
	void querySettleRank(const ccHttpRequestCallback& callback);
	void checkExchangeCode(const std::string &exchangeCode, const ccHttpRequestCallback& callback);

	void saveSelfInfo(HttpClient* client, HttpResponse* response);
	void saveUpdateRole(HttpClient* client, HttpResponse* response);
	void saveRealRankInfo(HttpClient* client, HttpResponse* response);
	void saveSettleRankInfo(HttpClient* client, HttpResponse* response);

	RankBaseData_T *analyzeTmpSort(HttpClient* client, HttpResponse* response);

	CC_SYNTHESIZE_READONLY(bool, m_bHasSelfInfo, HasSelfInfo);
	CC_SYNTHESIZE_READONLY(bool, m_bHasRealRank, HasRealRank);
	CC_SYNTHESIZE_READONLY(bool, m_bHasSettleRank, HasSettleRank);
	CC_SYNTHESIZE_READONLY(int, _onlineDay, OnlineDay);
	CC_SYNTHESIZE_READONLY(int, _onlineDayInWeek, OnlineDayInWeek);
private:
	NetDataMgr();

	string getJsonReqItem(const char *strKey, const char *strValue, bool isString, bool isNeedCut);
	string getReqItem(const char *strKey, const char *strValue, bool isNeedCut);
	void parseBaseDataJson(RankBaseData_T *pRankBaseData, Json *pJson);
	void parseTopArray(RankBaseData_T arrBaseData[][RANK_ROLE_COUNT], Json *pJson);
	void parseSelfArray(RankBaseData_T *arrBaseData, Json *pJson);

	RankRoleInfo_T *m_pSelfInfo;
	RankRealInfo_T *m_pRealRank;
	RankSettleInfo_T *m_pSettleRank;
    
    static NetDataMgr *s_pInstance;
};

#endif  // __NETDATA_MGR_H__
