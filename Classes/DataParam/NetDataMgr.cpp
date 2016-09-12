#include "NetDataMgr.h"
#include "spine/Json.h"
#include "../platBridge/cocos2dx_plat.h"
#include "UserData.h"
#include "GLCommon/JRCommon.h"

using namespace std;
USING_NS_CC;


NetDataMgr* NetDataMgr::s_pInstance = nullptr;

NetDataMgr::NetDataMgr()
	:m_pRealRank(nullptr),
	m_pSettleRank(nullptr),
	m_pSelfInfo(nullptr),
	m_bHasSelfInfo(false),
	m_bHasRealRank(false),
	m_bHasSettleRank(false),
	_onlineDay(-1),
	_onlineDayInWeek(1)
{
	m_pSelfInfo = new RankRoleInfo_T();
	m_pSettleRank = new RankSettleInfo_T();
	m_pRealRank = new RankRealInfo_T();
}


NetDataMgr::~NetDataMgr()
{
	CC_SAFE_DELETE(m_pSelfInfo);
	CC_SAFE_DELETE(m_pRealRank);
	CC_SAFE_DELETE(m_pSettleRank);
}


NetDataMgr* NetDataMgr::getInstance(void)
{
    if (nullptr == s_pInstance)
    {
        s_pInstance = new NetDataMgr();
        s_pInstance->init();
    }
    
    return s_pInstance;
}


void NetDataMgr::init()
{
}


const RankRoleInfo_T *NetDataMgr::getSelfInfo(void)
{
	return m_pSelfInfo;
}


const RankRealInfo_T *NetDataMgr::getRealRank(void)
{
	return m_pRealRank;
}


const RankSettleInfo_T *NetDataMgr::getSettleRank(void)
{
	return m_pSettleRank;
}


int NetDataMgr::getSelfScore(void)
{
	int nValue = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_SCORE == m_pSelfInfo->arrCurRank[i].nRankId)
		{
			nValue = m_pSelfInfo->arrCurRank[i].nValue;
			break;
		}
	}
	
	return nValue;
}


int NetDataMgr::getSelfFortune(void)
{
	int nValue = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_FORTUNE == m_pSelfInfo->arrCurRank[i].nRankId)
		{
			nValue = m_pSelfInfo->arrCurRank[i].nValue;
			break;
		}
	}

	return nValue;
}

const string &NetDataMgr::getAccount(void)
{
	return m_pSelfInfo->strAccount;
}

const string &NetDataMgr::getNick(void)
{
	return m_pSelfInfo->strNick;
}
int NetDataMgr::getSelfScoreRank(void)
{
	int nRet = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_SCORE == m_pSelfInfo->arrCurRank[i].nRankId)
		{
			nRet = m_pSelfInfo->arrCurRank[i].nIndex;
			break;
		}
	}

	return nRet;

}
int NetDataMgr::getSelfFortuneRank(void)
{
	int nRet = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_FORTUNE == m_pSelfInfo->arrCurRank[i].nRankId)
		{
			nRet = m_pSelfInfo->arrCurRank[i].nIndex;
			break;
		}
	}

	return nRet;
}

int NetDataMgr::getSelfSettleScoreRank(void)
{
	int nRet = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_SCORE == m_pSettleRank->arrRoleRank[i].nRankId)
		{
			nRet = m_pSettleRank->arrRoleRank[i].nIndex;
			break;
		}
	}

	return nRet;
}

int NetDataMgr::getSelfSettleFortuneRank(void)
{
	int nRet = 0;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_FORTUNE == m_pSettleRank->arrRoleRank[i].nRankId)
		{
			nRet = m_pSettleRank->arrRoleRank[i].nIndex;
			break;
		}
	}

	return nRet;
}


const RankBaseData_T *NetDataMgr::getRealScoreTop(void)
{
	const RankBaseData_T *pRet = nullptr;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_SCORE == m_pRealRank->arrTopRank[i][0].nRankId)
		{
			pRet = m_pRealRank->arrTopRank[i];
			break;
		}		
	}
	return pRet;
}
const RankBaseData_T *NetDataMgr::getRealFortuneTop(void)
{
	const RankBaseData_T *pRet = nullptr;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_FORTUNE == m_pRealRank->arrTopRank[i][0].nRankId)
		{
			pRet = m_pRealRank->arrTopRank[i];
			break;
		}		
	}
	return pRet;

}
const RankBaseData_T *NetDataMgr::getSettleScoreTop(void)
{
	const RankBaseData_T *pRet = nullptr;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_SCORE == m_pSettleRank->arrTopRank[i][0].nRankId)
		{
			pRet = m_pRealRank->arrTopRank[i];
			break;
		}		
	}
	return pRet;
}
const RankBaseData_T *NetDataMgr::getSettleFortuneTop(void)
{
	const RankBaseData_T *pRet = nullptr;

	for (int i=0; i<RANK_COUNT; ++i)
	{
		if (RANK_ID_FORTUNE == m_pSettleRank->arrTopRank[i][0].nRankId)
		{
			pRet = m_pRealRank->arrTopRank[i];
			break;
		}		
	}
	return pRet;
}

string NetDataMgr::getJsonReqItem(const char *strKey, const char *strValue, bool isString, bool isNeedCut)
{
	string strItem;

	strItem.append("\"");
	strItem.append(strKey);
	strItem.append("\":");
	if (isString)
	{
		strItem.append("\"");
	}
	strItem.append(strValue);
	if (isString)
	{
		strItem.append("\"");
	}
	if (isNeedCut)
	{
		strItem.append(",");
	}

	return strItem;
}


string NetDataMgr::getReqItem(const char *strKey, const char *strValue, bool isNeedCut)
{
	string strItem;

	strItem.append(strKey);
	strItem.append("=");
	strItem.append(strValue);

	if (isNeedCut)
	{
		strItem.append("&");
	}

	return strItem;
}


void NetDataMgr::parseBaseDataJson(RankBaseData_T *pRankBaseData, Json *pJson)
{
	if (nullptr != pJson)
	{
		pRankBaseData->nRankId = Json_getInt(pJson, STR_RANKID_KEY, 0);
		pRankBaseData->nIndex = Json_getInt(pJson, STR_INDEX_KEY, 0);
		pRankBaseData->strAccount = Json_getString(pJson, STR_ACC_KEY, "");
		pRankBaseData->strNick = Json_getString(pJson, STR_NICK_KEY, "");
		pRankBaseData->nRoleId = Json_getInt(pJson, STR_ROLEID_KEY, 0);
		pRankBaseData->nLevel = Json_getInt(pJson, STR_LV_KEY, 0);
		pRankBaseData->nValue = Json_getInt(pJson, STR_VALUE_KEY, 0);
		pRankBaseData->nAward = Json_getInt(pJson, STR_AWARD_KEY, 0);
		pRankBaseData->strDate = Json_getString(pJson, STR_DATE_KEY, "");
	}	
}


void NetDataMgr::parseTopArray(RankBaseData_T arrBaseData[][RANK_ROLE_COUNT], Json *pJson)
{
	/*get top rank*/
	if (nullptr != pJson)
	{
		//	modified by zhou @2016.02.22
		//		Json *pArrInfo = Json_create(pJson->valuestring);
		Json *pArrInfo = pJson;

		for (int i=0; i<RANK_COUNT; i++)
		{
			Json *pRankInfo = Json_getItemAt(pArrInfo, i);

			for (int j=0; ; ++j)
			{
				RankBaseData_T *pRankBaseData = nullptr;
				RankBaseData_T *pEmptyBaseData = nullptr;
				Json *pRoleInfo = Json_getItemAt(pRankInfo, j);

				if (nullptr == pRoleInfo)
				{
					break;
				}

				int nRankId = Json_getInt(pRoleInfo, STR_RANKID_KEY, 0);

				for (int k=0; k<RANK_COUNT; ++k)
				{
					if (nRankId == arrBaseData[k][0].nRankId)
					{
						pRankBaseData = arrBaseData[k];
						break;
					}

					if (nullptr == pEmptyBaseData && 0 == arrBaseData[k][0].nRankId)
					{
						pEmptyBaseData = arrBaseData[k];
					}			
				}

				if (nullptr == pRankBaseData)
				{
					pRankBaseData = pEmptyBaseData;
					pRankBaseData[0].nRankId = nRankId;
				}

				int nIndex = Json_getInt(pRoleInfo, STR_INDEX_KEY, 0);
				if (nIndex > 0 && nIndex < (RANK_ROLE_COUNT+1))
				{
					RankBaseData_T *pRoleData = &pRankBaseData[nIndex-1];

					parseBaseDataJson(pRoleData, pRoleInfo);
				}
			}		
		}
	}	
}


void NetDataMgr::parseSelfArray(RankBaseData_T *arrBaseData, Json *pJson)
{
	/*get cur rank*/
	if (nullptr != pJson)
	{
//		Json *pArrInfo = Json_create(pJson->valuestring);
		Json *pArrInfo = pJson;
		for (int i=0; (nullptr != pArrInfo) && (i<RANK_COUNT); ++i)
		{
			Json *pSubItem = Json_getItemAt(pArrInfo, i);
			RankBaseData_T *pRankBaseData = nullptr;
			RankBaseData_T *pEmptyBaseData = nullptr;

			if (nullptr != pSubItem)
			{
				int nRankId = Json_getInt(pSubItem, STR_RANKID_KEY, 0);

				for (int j=0; j<RANK_COUNT; ++j)
				{
					if (nRankId == arrBaseData[j].nRankId)
					{
						pRankBaseData = &arrBaseData[j];
						break;
					}

					if (nullptr == pEmptyBaseData && 0 == arrBaseData[j].nRankId)
					{
						pEmptyBaseData = &arrBaseData[j];
					}
				}

				if (nullptr == pRankBaseData)
				{
					pRankBaseData = pEmptyBaseData;
					pRankBaseData->nRankId = nRankId;
				}

				parseBaseDataJson(pRankBaseData, pSubItem);	
			}
		}

//		Json_dispose(pArrInfo);
	}
}


#if 0
void NetDataMgr::queryRole(const ccHttpRequestCallback& callback)
{
	string strReqData;
	
	strReqData.append(getReqItem(STR_GAMEID_KEY, STR_GAME_ID, true).c_str());
	strReqData.append(getReqItem(STR_VER_KEY, STR_RANK_VER, true).c_str());
	strReqData.append(getReqItem(STR_ACC_KEY, UserData::getInstance()->getAccount().c_str(), false));

	GameUtils::getDataFromServicer(STR_SERVICER_URL, STR_QUERY_SELF, strReqData.c_str(), "", callback);
}
#endif


void NetDataMgr::updateRole(const ccHttpRequestCallback& callback)
{
	UserData *pUserDataManager = UserData::getInstance();
	string strReqData;

	strReqData.append("{");
	//gameid
	strReqData.append(this->getJsonReqItem(STR_GAMEID_KEY, STR_GAME_ID, false, true).c_str());
	//chnlid
	strReqData.append(this->getJsonReqItem(STR_CHNLID_KEY, cocos2dx_plat::getChannelId().c_str(), true, true).c_str());
	//version
	strReqData.append(this->getJsonReqItem(STR_VER_KEY, STR_RANK_VER, false, true).c_str());
	//account
	strReqData.append(this->getJsonReqItem(STR_ACC_KEY, pUserDataManager->getAccount().c_str(), true, true));
	//nick
	strReqData.append(this->getJsonReqItem(STR_NICK_KEY, pUserDataManager->getNickName().c_str(), true, true));
	//role id
	int roleId = pUserDataManager->getHeadId();
	strReqData.append(getJsonReqItem(STR_ROLEID_KEY, CCString::createWithFormat("%d", roleId)->getCString(), false, true));
	//role level
	int roleLv = pUserDataManager->getTotalFloor();
	strReqData.append(getJsonReqItem(STR_LV_KEY, CCString::createWithFormat("%d", roleLv)->getCString(), false, true));
	//weekly dist
	int dist = pUserDataManager->getWeeklyScore();
	strReqData.append(getJsonReqItem(STR_RUN_KEY, CCString::createWithFormat("%d", dist)->getCString(), false, true));
	//weekly consume
	int consume = pUserDataManager->getTotalFloor();
	strReqData.append(getJsonReqItem(STR_DIAMOND_KEY, CCString::createWithFormat("%d", consume)->getCString(), false, false));
	strReqData.append("}");

	GameUtils::postDataToServicer(STR_SERVICER_URL, STR_UPDATE_ROLE, strReqData.c_str(), "", callback);
}


void NetDataMgr::postTmpSort(int nDist, const ccHttpRequestCallback& callback)
{
	UserData *pUserDataManager = UserData::getInstance();
	string strReqData;

	strReqData.append("{");
	//gameid
	strReqData.append(this->getJsonReqItem(STR_GAMEID_KEY, STR_GAME_ID, false, true).c_str());
	//chnlid
	strReqData.append(this->getJsonReqItem(STR_CHNLID_KEY, cocos2dx_plat::getChannelId().c_str(), true, true).c_str());
	//version
	strReqData.append(this->getJsonReqItem(STR_VER_KEY, STR_RANK_VER, false, true).c_str());
	//account
	strReqData.append(this->getJsonReqItem(STR_ACC_KEY, pUserDataManager->getAccount().c_str(), true, true));
	//role id
	int roleId = pUserDataManager->getHeadId();
	strReqData.append(getJsonReqItem(STR_ROLEID_KEY, CCString::createWithFormat("%d", roleId)->getCString(), false, true));
	//role level
	int roleLv = pUserDataManager->getTotalFloor();
	strReqData.append(getJsonReqItem(STR_LV_KEY, CCString::createWithFormat("%d", roleLv)->getCString(), false, true));
	//weekly dist
	strReqData.append(getJsonReqItem(STR_RUN_KEY, CCString::createWithFormat("%d", nDist)->getCString(), false, false));
	strReqData.append("}");

	GameUtils::postDataToServicer(STR_SERVICER_URL, STR_TMP_SORT, strReqData.c_str(), "", callback);
}

#if 0
void NetDataMgr::queryRealSort(const ccHttpRequestCallback& callback)
{
	UserData *pUserDataManager = UserData::getInstance();
	string strReqData;

	//gameid
	strReqData.append(getReqItem(STR_GAMEID_KEY, STR_GAME_ID, true));
	//version
	strReqData.append(getReqItem(STR_VER_KEY, STR_RANK_VER, true));
	//account
	strReqData.append(getReqItem(STR_ACC_KEY, pUserDataManager->getAccount().c_str(), false));	

	GameUtils::getDataFromServicer(STR_SERVICER_URL, STR_REAL_SORT, strReqData.c_str(), "", callback);
}
#endif


void NetDataMgr::querySettleRank(const ccHttpRequestCallback& callback)
{
	UserData *pUserDataManager = UserData::getInstance();
	string strReqData;

	//gameid
	strReqData.append(getReqItem(STR_GAMEID_KEY, STR_GAME_ID, true));
	//version
	strReqData.append(getReqItem(STR_VER_KEY, STR_RANK_VER, true));
	//account
	strReqData.append(getReqItem(STR_ACC_KEY, pUserDataManager->getAccount().c_str(), false));

	GameUtils::getDataFromServicer(STR_SERVICER_URL, STR_SETTLE_RANK, strReqData.c_str(), "", callback);
}

void NetDataMgr::checkExchangeCode(const std::string & exchangeCode, const ccHttpRequestCallback & callback)
{
	std::string exchangeCodeTmp = exchangeCode;
	ToolsUtil::stringFindReplace(exchangeCodeTmp, " ", "");
	UserData *pUserDataManager = UserData::getInstance();
	string strReqData;

	//gameid
	strReqData.append(getReqItem("appId", STR_GAME_ID, true));
	//version
	strReqData.append(getReqItem("channel", cocos2dx_plat::getChannelId().c_str(), true));
//	strReqData.append(getReqItem("code", exchangeCode.c_str(), false));
	strReqData.append(getReqItem("code", exchangeCodeTmp.c_str(), true));
	//account
	strReqData.append(getReqItem(STR_ACC_KEY, pUserDataManager->getAccount().c_str(), false));

	GameUtils::getDataFromServicer(STR_SERVICER_URL_EX, "checkCode", strReqData.c_str(), "", callback);
}


void NetDataMgr::saveSelfInfo(HttpClient* client, HttpResponse* response)
{
	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	CCLOG("strRepData=%s", strRepData.c_str());
	Json *pJson = Json_create(strRepData.c_str());

	if (nullptr == m_pSelfInfo)
	{
		m_pSelfInfo = new RankRoleInfo_T();
	}
	m_bHasSelfInfo = true;

//	m_pSelfInfo->strAccount = Json_getString(pJson, STR_ACC_KEY, "");
//	m_pSelfInfo->strNick = Json_getString(pJson, STR_NICK_KEY, "");

	/*get cur rank*/
	Json *pCurRankItem = Json_getItem(pJson, STR_RANK_KEY);
	parseSelfArray(m_pSelfInfo->arrCurRank, pCurRankItem);	

	/*get history rank*/
	Json *pHistoryRankItem = Json_getItem(pJson, STR_RANK_2_KEY);
	parseSelfArray(m_pSelfInfo->arrHistoryRank, pHistoryRankItem);

	std::string strDate = Json_getString(pJson, STR_DATE_KEY, "");
//	_onlineDay = 2;

	Json_dispose(pJson);

	return;
}


void NetDataMgr::saveUpdateRole(HttpClient* client, HttpResponse* response)
{
	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	CCLOG("strRepData=%s", strRepData.c_str());
	Json *pJson = Json_create(strRepData.c_str());

	if (nullptr == m_pSelfInfo)
	{
		m_pSelfInfo = new RankRoleInfo_T();
	}

	if (nullptr == m_pRealRank)
	{
		m_pRealRank = new RankRealInfo_T();
	}

	m_bHasSelfInfo = true;
	m_bHasRealRank = true;

	m_pSelfInfo->strAccount = Json_getString(pJson, STR_ACC_KEY, "");
	m_pSelfInfo->strNick = Json_getString(pJson, STR_NICK_KEY, "");

	/*get cur rank*/
	Json *pCurRankItem = Json_getItem(pJson, STR_RANK_KEY);
	parseSelfArray(m_pSelfInfo->arrCurRank, pCurRankItem);

	/*get top rank*/
	Json *pAllRankInfo = Json_getItem(pJson, STR_RANK_2_KEY);
	parseTopArray(m_pRealRank->arrTopRank, pAllRankInfo);

	int curYear = 0;
	int curMonth = 0;
	int curDay = 0;
	std::string strDate = Json_getString(pJson, "srvDate", "");
	JRTime::parseTime(strDate.c_str(), curYear, curMonth, curDay);
	int srcDay = JRTime::getDayInYear(2016, 6, 12);
	int desDay = JRTime::getDayInYear(curYear, curMonth, curDay);

	_onlineDay = JRTime::compareTheDay(2016, srcDay, curYear, desDay);
	_onlineDayInWeek = JRTime::getDayInWeek(curYear, desDay);
	Json_dispose(pJson);

	return;
}


void NetDataMgr::saveRealRankInfo(HttpClient* client, HttpResponse* response)
{
	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	CCLOG("strRepData=%s", strRepData.c_str());
	Json *pJson = Json_create(strRepData.c_str());

	if (nullptr == m_pRealRank)
	{
		m_pRealRank = new RankRealInfo_T();
	}

	if (nullptr == m_pSelfInfo)
	{
		m_pSelfInfo = new RankRoleInfo_T();
	}

	m_bHasRealRank = true;

	/*get self rank info*/
	Json *pSelfInfo = Json_getItem(pJson, STR_RANK_KEY);
	parseSelfArray(m_pSelfInfo->arrCurRank, pSelfInfo);

	Json *pAllRankInfo = Json_getItem(pJson, STR_RANK_2_KEY);
	parseTopArray(m_pRealRank->arrTopRank, pAllRankInfo);

	Json_dispose(pJson);
}


void NetDataMgr::saveSettleRankInfo(HttpClient* client, HttpResponse* response)
{
	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	Json *pJson = Json_create(strRepData.c_str());

	if (nullptr == m_pSettleRank)
	{
		m_pSettleRank = new RankSettleInfo_T();
	}

	m_bHasSettleRank = true;

	m_pSettleRank->strDate = Json_getString(pJson, STR_DATE_KEY, "");

	/*get self rank info*/
	Json *pSelfInfo = Json_getItem(pJson, STR_RANK_KEY);
	parseSelfArray(m_pSettleRank->arrRoleRank, pSelfInfo);

	Json *pAllRankInfo = Json_getItem(pJson, STR_RANK_2_KEY);
	parseTopArray(m_pSettleRank->arrTopRank, pAllRankInfo);

	Json_dispose(pJson);

	return;
}


RankBaseData_T *NetDataMgr::analyzeTmpSort(HttpClient* client, HttpResponse* response)
{
	std::vector<char> *buffer = response->getResponseData();
	string strRepData(buffer->begin(), buffer->end());
	Json *pJson = Json_create(strRepData.c_str());
	RankBaseData_T *pRet = new RankBaseData_T[2]();
	//debug
	//CCLOG("%s",strRepData.c_str);
	/*get self rank info*/
	Json *pSelfInfo = Json_getItem(pJson, STR_RANK_KEY);

	//int state = Json_getInt(pJson,"statue", 404);

//	modified by zhou @2016.02.22
//	pSelfInfo = Json_create(pSelfInfo->valuestring);
	for (int i=0; (nullptr != pSelfInfo) && (i<RANK_COUNT); ++i)
	{
		Json *pSubItem = Json_getItemAt(pSelfInfo, i);
		RankBaseData_T *pRankBaseData = &pRet[i];

		parseBaseDataJson(pRankBaseData, pSubItem);		
	}

//	modified by zhou @2016.02.22
//	Json_dispose(pSelfInfo);
	Json_dispose(pJson);

	return pRet;
}



/*
http://192.168.1.242:8000/queryrole?req={%22account%22:%224415a786ce009be4e3bf467832606d5d%22,%22gameid%22:1,%22version%22:1}
{"DATE2":"2014-04-02 18:19","nick":"??12","DATE":"2014-04-02 18:18","account":"4415a786ce009be4e3bf467832606d5d","DATA":[{"index":0,"level":100,"roleid":1,"value":1000,"code":1003},{"index":0,"level":100,"roleid":1,"value":100,"code":1004},{"index":3,"level":100,"roleid":1,"value":30,"code":1001},{"index":3,"level":100,"roleid":1,"value":100,"code":1002},{"index":0,"level":100,"roleid":1,"value":11000,"code":1005}]}

http://192.168.1.242:8000/updatenick?req={%22account%22:%224415a786ce009be4e3bf467832606d5d%22,%22nick%22:%22HELLO%22}
OK

http://192.168.1.242:8000/updaterole?req={%22chnlid%22:%22joyriver%22,%22gameid%22:1,%22version%22:1,%22roleid%22:1,%20%22level%22:100,%22STR%22:100,%22AGI%22:10,%22VIT%22:4,%22ATK%22:19,%22MGK%22:19,%22score%22:100,%22kill%22:1000,%22mission%22:100,%22diamond%22:100,%22goldcoin%22:1000}%20{%22chnlid%22:%22joyriver%22,%22gameid%22:1,%22version%22:1,%22roleid%22:1,%20%22level%22:1000,%22STR%22:1000,%22AGI%22:100,%22VIT%22:40,%22ATK%22:190,%22MGK%22:190,%22score%22:1000,%22kill%22:1000,%22mission%22:100,%22diamond%22:100,%22goldcoin%22:1000}%20{%22chnlid%22:%22joyriver%22,%22gameid%22:1,%22version%22:1,%22roleid%22:1,%20%22level%22:1000,%22STR%22:1000,%22AGI%22:100,%22VIT%22:40,%22ATK%22:190,%22MGK%22:190,%22score%22:1000,%22kill%22:1000,%22mission%22:100,%22diamond%22:100,%22goldcoin%22:1000}%20{%22chnlid%22:%22joyriver%22,%22gameid%22:1,%22version%22:1,%22roleid%22:1,%20%22level%22:1000,%22STR%22:0,%22AGI%22:100,%22VIT%22:40,%22ATK%22:190,%22MGK%22:190,%22score%22:1000,%22kill%22:1000,%22mission%22:0,%22diamond%22:0,%22goldcoin%22:0}
{"nick":"??1","account":"28f071aec22196bc597543d3aed7a9a7"}

//youwenti
http://192.168.1.242:8000/updaterole?req={%22chnlid%22:%22joyriver%22,%22account%22:%224415a786ce009be4e3bf467832606d5d%22,%22gameid%22:1,%22version%22:1,%22roleid%22:1,%20%22level%22:100,%22STR%22:100,%22AGI%22:10,%22VIT%22:4,%22ATK%22:19,%22MGK%22:19,%22score%22:100,%22kill%22:1000,%22mission%22:100,%22diamond%22:100,%22goldcoin%22:1000}
SC_OK

http://192.168.1.242:8000/realsort?req={%22nick%22:%22??12%22,%22account%22:%224415a786ce009be4e3bf467832606d5d%22,%22DATA%22:{%221003%22:0,%221004%22:2,%221001%22:5,%221002%22:0,%22COUNT%22:5,%221005%22:2}}
youwenti

*/