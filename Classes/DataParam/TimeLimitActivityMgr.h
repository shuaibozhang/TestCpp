#ifndef _TIME_LIMIT_ACTIVITY_MAG_H_
#define  _TIME_LIMIT_ACTIVITY_MAG_H_


#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

typedef std::function<void(int idx)> ccInitActivityCall;

struct CountdownReward
{
	int idx;
	int timemin;
	vector<int> itemids;
	vector<int> itemNums;
};

struct TimeLimitActivityInfo
{
	std::string activityname;
	int activityStartTime;
	int activityLongDay;
	ccInitActivityCall _initcall;
	CountdownReward* pRewadArr;
	int rewardNums;
};


class TimeLimitActivityMgr
{
public:
	TimeLimitActivityMgr();
	static TimeLimitActivityMgr* getInstance();
	void intiAndShowActivitys(int curday);
	void addActicityInfo(TimeLimitActivityInfo info);
	void getRewardByIdx(std::string activitname,int idx);
	void getReward(std::string activitname);
	const CountdownReward& getRewardInfo(std::string activitname,int idx);
	int checkCanGetRewardNum();
	void resetOnlineTimeReward();
private:
	static TimeLimitActivityMgr* s_instance;
	vector<TimeLimitActivityInfo> _arrActivitys;
public:
	static char* s_CurActivityName;
	bool _isInited;
};

struct ActivityConfigStruct_T
{
	std::string name;
	std::string	des;
	std::string key;
	int num;
	std::vector<int> arrRewardItemid;
	std::vector<int> arrRewardCount;
};

struct CommandActivityStuct_T
{
	std::string storekey;
	std::string	activityname;
	std::string	activitydes;
	std::string	activitytype;
	bool isRepeat;
	std::string	startday;
	int	longtime;
	std::vector<ActivityConfigStruct_T*> arrActivitys;
	bool haveInit;
};
class CommondActivityMagr
{
public:
	CommondActivityMagr();

	static CommondActivityMagr* getInstance();
	/*
	*must call after get online time, because i use the instance of mainlayer
	*/
	void intiAndShowActivitys(int curday);
	void loadConfigFormFile();
	
	int getCurNum(std::string key);
	int getCurNumByNameAndIdx(std::string name, int idx);
	int getTargetNumByNameAndIdx(std::string name, int idx);

	const ActivityConfigStruct_T& getActivityConfig(std::string key, int idx);
	int checkCanGetReward(std::string activitykey, int idx);
	void getReward(std::string activitykey, int idx);
	int getActivityNums(std::string key);

	void resetReward();
private:
	std::map<std::string, CommandActivityStuct_T*> _mapActivitys;
	bool _isloadConfig;
	bool _isInited;

public:
	static char* ACTIVITY_NAME_MIDAUTUMAN;
	static char* ACTIVITY_NAME_GUOQING;
	static CommondActivityMagr* _instance;
};
#endif