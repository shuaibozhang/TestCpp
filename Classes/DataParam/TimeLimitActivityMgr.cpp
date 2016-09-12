#include "TimeLimitActivityMgr.h"
#include "../Scenes/MainScene.h"
#include "../GLCommon/Utils/JRTime.h"
#include "../GLCommon/Utils/ToolsUtil.h"
#include "../Scenes/PopRewardLayer.h"
#include "ParamData.h"
#include "UserData.h"
#include "DayActiveMgr.h"
#include "AchieveMgr.h"

TimeLimitActivityMgr* TimeLimitActivityMgr::s_instance = nullptr;

CountdownReward g_arrReward[ParamData::ONLINE_TIME_REWARD_NUM] = {
	{ 0, 5,{ ParamData::GOLD_ITEM_ID },{ 200 } },
	{ 1, 5,{ 1000 },{ 3 } },
	{ 2, 10,{ 1014 },{ 2 } },
	{ 3, 10,{ ParamData::TILI_ITEM_ID },{ 10 } },
	{ 4, 10,{ 1500 },{ 40 } },
	{ 5, 20,{ 1503 },{ 30 } },
	{ 6, 30,{ 1506 },{ 20 } },
	{ 7, 30,{ ParamData::CRYSTAL_ITEM_ID },{ 30 } },

	/*{ 0, 1,{ ParamData::GOLD_ITEM_ID },{ 200 } },
	{ 1, 1,{ 1000 },{ 3 } },
	{ 2, 1,{ 1014 },{ 2 } },
	{ 3, 1,{ ParamData::TILI_ITEM_ID },{ 10 } },
	{ 4, 1,{ 1500 },{ 40 } },
	{ 5, 1,{ 1503 },{ 30 } },
	{ 6, 1,{ 1506 },{ 20 } },
	{ 7, 1,{ ParamData::CRYSTAL_ITEM_ID },{ 30 } },*/
};

CountdownReward g_arrTeacher[] = {
	{ 0, 9,{ ParamData::TILI_ITEM_ID },{ 5 } },
	{ 1, 10,{ ParamData::GOLD_ITEM_ID },{ 1000 } },
	{ 2, 20,{ 1014 },{ 3 } },
	{ 3, 30,{ 1503 },{ 30 } },
	{ 4, 40,{ 1506 },{ 30 } },
	{ 5, 50,{ ParamData::CRYSTAL_ITEM_ID },{ 30 } },

//{ 0, 1,{ ParamData::TILI_ITEM_ID },{ 5 } },
//{ 1, 1,{ ParamData::GOLD_ITEM_ID },{ 1000 } },
//{ 2, 1,{ 1014 },{ 3 } },
//{ 3, 1,{ 1503 },{ 30 } },
//{ 4, 1,{ 1506 },{ 30 } },
//{ 5, 1,{ ParamData::CRYSTAL_ITEM_ID },{ 30 } },
	
};

char* TimeLimitActivityMgr::s_CurActivityName = "teacherReward";

TimeLimitActivityMgr::TimeLimitActivityMgr():_isInited(false)
{
}

TimeLimitActivityMgr * TimeLimitActivityMgr::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new TimeLimitActivityMgr();
		int srcDay = JRTime::getDayInYear(2016, 6, 12);		
		int startday = 0;
		
		if (UserData::getInstance()->getFirstPlayDayInYear() == 0)
		{
			startday = JRTime::compareTheDay(2016, srcDay, JRTime::getCurYear(), JRTime::getCurDayInYear());
			UserData::getInstance()->setFirstPlayDayInYear(startday);
			UserData::getInstance()->saveUserData(false);
		}
		else
		{
			startday = UserData::getInstance()->getFirstPlayDayInYear();
		}		

		//rewardNums must < 8;

		/*TimeLimitActivityInfo temp;
		temp.activityname = "onlineReward";
		temp.activityStartTime = startday;
		temp.activityLongDay = 7;
		temp.pRewadArr = g_arrReward;
		temp.rewardNums = sizeof(g_arrReward) / sizeof(g_arrReward[0]);
		temp._initcall = CC_CALLBACK_1(MainLayer::initActivityInfo, MainLayer::getCurMainLayer());
		s_instance->addActicityInfo(temp);*/

		int srcDayTeacher = JRTime::getDayInYear(2016, 6, 12);
		int teacherDay = JRTime::getDayInYear(2016, 9, 9);
		int teacherStartday = JRTime::compareTheDay(2016, srcDayTeacher, 2016, teacherDay);

		TimeLimitActivityInfo temp;
		temp.activityname = "teacherReward";
		temp.activityStartTime = teacherStartday;
		temp.activityLongDay = 3;
		temp.pRewadArr = g_arrTeacher;
		temp.rewardNums = sizeof(g_arrTeacher) / sizeof(g_arrTeacher[0]);
		temp._initcall = CC_CALLBACK_1(MainLayer::initActivityInfo, MainLayer::getCurMainLayer());
		s_instance->addActicityInfo(temp);
	}

	return s_instance;
}

void TimeLimitActivityMgr::intiAndShowActivitys(int curday)
{
	if (_isInited)
	{
		return;
	}

	_isInited = true;

	for (auto& temp : _arrActivitys)
	{
		int dayidx = curday - temp.activityStartTime;
		//int srcDay = JRTime::getDayInYear(2016, 6, 12) + curday;
		if (temp.activityStartTime <= curday && curday < temp.activityStartTime + temp.activityLongDay)
		
		{
			if (temp._initcall)
			{
				temp._initcall(dayidx);
			}
		}
	}
}

void TimeLimitActivityMgr::addActicityInfo(TimeLimitActivityInfo info)
{
	_arrActivitys.push_back(info);
}

void TimeLimitActivityMgr::getRewardByIdx(std::string activitname,int idx)
{
	std::vector<PopItemInfo_T> arrItems;
	for (auto& temp : _arrActivitys)
	{
		if (temp.activityname.compare(activitname) == 0)
		{
			auto& info = temp.pRewadArr[idx];
			int size = info.itemids.size();
			for (int i = 0; i < size; i++)
			{
				int id = info.itemids[i];
				int num = info.itemNums[i];
				if (id >= 1500)
				{
					int wenponnums[] = { 0,0,0,0 };
					for (int j = 0; j < 4; j++)
					{
						if (j == 3)
						{
							wenponnums[j] = num;
						}
						else
						{
							int curnum = ToolsUtil::getRandomInt(0, num);
							num = num - curnum;
							if (num < 0)
							{
								num = 0;
							}

							wenponnums[j] = curnum;
						}

					}

					int getPartsIdx = 0;
					for (auto& curnum : wenponnums)
					{
						if (wenponnums[getPartsIdx] != 0)
						{
							PopItemInfo_T curiteminfo;
							curiteminfo.itemCount = wenponnums[getPartsIdx];
							curiteminfo.itemId = id + 10 * getPartsIdx;
							arrItems.push_back(curiteminfo);
						}
						getPartsIdx++;
					}
				}
				else
				{
					PopItemInfo_T curiteminfo;
					curiteminfo.itemCount = num;
					curiteminfo.itemId = id;
					arrItems.push_back(curiteminfo);
				}
			}

			UserData::getInstance()->setOnlineReward(idx, 2);

			auto layer = PopRewardLayer::create(arrItems);
			MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);
		}
	}	
}

void TimeLimitActivityMgr::getReward(std::string name)
{
	for (auto& temp : _arrActivitys)
	{
		if (temp.activityname.compare(name) == 0)
		{
			for (int i = 0; i < temp.rewardNums; i++)
			{
				if (UserData::getInstance()->getOnlineTimeReward(i) == 1)
				{
					getRewardByIdx(name,i);
					return;
				}
			}
		}
	}
	
}

const CountdownReward & TimeLimitActivityMgr::getRewardInfo(std::string activitname,int idx)
{
	for (auto& temp : _arrActivitys)
	{
		if (temp.activityname.compare(activitname) == 0 && idx < temp.rewardNums)
		{
			return *(temp.pRewadArr + idx);
		}
	}
	CCASSERT(0, "err name or idx");

	return *(_arrActivitys[0].pRewadArr + idx);
	// TODO: 在此处插入 return 语句
}

int TimeLimitActivityMgr::checkCanGetRewardNum()
{
	int num = 0;

	for (int i = 0; i < ParamData::ONLINE_TIME_REWARD_NUM; i++)
	{
		if (UserData::getInstance()->getOnlineTimeReward(i) == 1)
		{
			num++;
		}
	}

	return num;
}

void TimeLimitActivityMgr::resetOnlineTimeReward()
{
	for (int i = 0; i < ParamData::ONLINE_TIME_REWARD_NUM; i++)
	{
		UserData::getInstance()->setOnlineReward(i, 0);
	}

	UserData::getInstance()->setOnlineCountdownTime(g_arrReward[0].timemin * 60);

	UserData::getInstance()->saveUserData();
}

CommondActivityMagr* CommondActivityMagr::_instance = nullptr;
char* CommondActivityMagr::ACTIVITY_NAME_MIDAUTUMAN = "moonactivity";
CommondActivityMagr* CommondActivityMagr::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new CommondActivityMagr();
		_instance->loadConfigFormFile();
	}

	return _instance;
}

void CommondActivityMagr::intiAndShowActivitys(int curday)
{
	if (_isInited)
	{
		return;
	}

	_isInited = true;

	std::map<std::string, std::function<void(int)>> initCallMap;
	initCallMap.insert(std::pair<std::string, std::function<void(int)> >(ACTIVITY_NAME_MIDAUTUMAN, std::bind(&MainLayer::initCommondActivity, MainLayer::getCurMainLayer(), std::placeholders::_1)));
	for (auto& tempconfig : _mapActivitys)
	{
		std::vector<std::string> tempTime;
		ToolsUtil::split((tempconfig.second)->startday, "-", tempTime);
		int yera = atoi(tempTime[0].c_str());
		int month = atoi(tempTime[1].c_str());
		int day = atoi(tempTime[2].c_str());
		int srcDay = JRTime::getDayInYear(2016, 6, 12);
		int startday = JRTime::compareTheDay(2016, srcDay, yera, JRTime::getDayInYear(yera, month, day));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#else
		if (startday <= curday && curday < startday + tempconfig.second->longtime)
#endif
		
		{
			tempconfig.second->haveInit = true;
			initCallMap.at((tempconfig.second)->storekey)(curday);
		}
	}
	
}

CommondActivityMagr::CommondActivityMagr():_isloadConfig(false),
_isInited(false)
{
}

void CommondActivityMagr::loadConfigFormFile()
{
	if (_isloadConfig)
	{
		return;
	}
	rapidjson::Document _jsonBezierDoc;
	auto contentStr = FileUtils::getInstance()->getDataFromFile("activitys/MidAutumn.b");
	ToolsUtil::unbtea(contentStr);
	std::string load_str((const char*)contentStr.getBytes(), contentStr.getSize());

	_jsonBezierDoc.Parse<0>(load_str.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	CommandActivityStuct_T* commadnActivityStruct = new CommandActivityStuct_T();

	commadnActivityStruct->storekey = _jsonBezierDoc["storekey"].GetString();
	commadnActivityStruct->activityname = _jsonBezierDoc["activityname"].GetString();
	commadnActivityStruct->activitydes = _jsonBezierDoc["activitydes"].GetString();
	commadnActivityStruct->startday = _jsonBezierDoc["startday"].GetString();
	commadnActivityStruct->activitytype = _jsonBezierDoc["activitytype"].GetString();
	commadnActivityStruct->isRepeat = _jsonBezierDoc["isRepeat"].GetBool();
	commadnActivityStruct->longtime = _jsonBezierDoc["longtime"].GetInt();
	commadnActivityStruct->haveInit = false;

	int count = _jsonBezierDoc["activitys"].Capacity();
	rapidjson::Value& activitysArray = _jsonBezierDoc["activitys"];
	for (int i = 0; i < count; i++)
	{
		auto pActivityConfigStruct_T = new ActivityConfigStruct_T();
		rapidjson::Value& singleActivity = activitysArray[i];

		pActivityConfigStruct_T->name = singleActivity["name"].GetString();
		pActivityConfigStruct_T->des = singleActivity["des"].GetString();
		pActivityConfigStruct_T->key = singleActivity["key"].GetString();
		pActivityConfigStruct_T->num = singleActivity["num"].GetInt();

		rapidjson::Value& rewards = singleActivity["reward"];
		rapidjson::Value& rewardcount = singleActivity["rewardcount"];
		for (int n = 0; n < rewards.Capacity(); n++)
		{
			pActivityConfigStruct_T->arrRewardItemid.push_back(rewards[n].GetInt());
			pActivityConfigStruct_T->arrRewardCount.push_back(rewardcount[n].GetInt());
		}
		commadnActivityStruct->arrActivitys.push_back(pActivityConfigStruct_T);
	}

	_mapActivitys.insert(std::pair<std::string, CommandActivityStuct_T*>(commadnActivityStruct->storekey, commadnActivityStruct));

	_isloadConfig = true;
}


int CommondActivityMagr::getCurNum(std::string key)
{
	int num = 0;
	if (key.compare("pass_guanqia") == 0)
	{
		num = UserData::getInstance()->getDayPass();
	}
	else if (key.compare("pass_ta") == 0)
	{
		num = UserData::getInstance()->getDayTa();
	}
	else if (key.compare("payrmb") == 0)
	{
		num = UserData::getInstance()->getPayRmb();
	}
	else if (key.compare("pass_mission") == 0)
	{
		num = DayActivityMgr::getInstance()->getFinishNum();
	}
	return num;
}

int CommondActivityMagr::getCurNumByNameAndIdx(std::string name, int idx)
{

	auto getkey = ((_mapActivitys.at(name))->arrActivitys)[idx]->key;
	return getCurNum(getkey);
}

const ActivityConfigStruct_T & CommondActivityMagr::getActivityConfig(std::string key, int idx)
{
	return *_mapActivitys.at(key)->arrActivitys[idx];
	// TODO: 在此处插入 return 语句
}

int CommondActivityMagr::checkCanGetReward(std::string activitykey, int idx)
{
	if (UserData::getInstance()->getActivityReward(activitykey, idx) == 2)
	{
		return 2;
	}
	auto& config = _mapActivitys.at(activitykey)->arrActivitys[idx];
	int curnum = getCurNum(config->key);
	
	if (curnum >= config->num)
	{
		return 1;
	}

	return 0;
}

void CommondActivityMagr::getReward(std::string activitykey, int idx)
{

	std::vector<PopItemInfo_T> arrItems;
	auto& info = _mapActivitys.at(activitykey);

	int size = (((info->arrActivitys).at(idx))->arrRewardItemid).size();
	for (int i = 0; i < size; i++)
	{
		int id = (((info->arrActivitys).at(idx))->arrRewardItemid)[i];
		int num = (((info->arrActivitys).at(idx))->arrRewardCount)[i];
		if (id >= 1500)
		{
			int wenponnums[] = { 0,0,0,0 };
			for (int j = 0; j < 4; j++)
			{
				if (j == 3)
				{
					wenponnums[j] = num;
				}
				else
				{
					int curnum = ToolsUtil::getRandomInt(0, num);
					num = num - curnum;
					if (num < 0)
					{
						num = 0;
					}

					wenponnums[j] = curnum;
				}

			}

			int getPartsIdx = 0;
			for (auto& curnum : wenponnums)
			{
				if (wenponnums[getPartsIdx] != 0)
				{
					PopItemInfo_T curiteminfo;
					curiteminfo.itemCount = wenponnums[getPartsIdx];
					curiteminfo.itemId = id + 10 * getPartsIdx;
					arrItems.push_back(curiteminfo);
				}
				getPartsIdx++;
			}
		}
		else
		{
			PopItemInfo_T curiteminfo;
			curiteminfo.itemCount = num;
			curiteminfo.itemId = id;
			arrItems.push_back(curiteminfo);
		}
	}

	UserData::getInstance()->setActivityReward(activitykey, idx, 2);

	auto layer = PopRewardLayer::create(arrItems);
	MainLayer::getCurMainLayer()->addChild(layer, MainLayer_Z::POP_REWARD_Z);

}

int CommondActivityMagr::getActivityNums(std::string key)
{
	return _mapActivitys.at(key)->arrActivitys.size();
}

void CommondActivityMagr::resetReward()
{
	for (auto& temp : _mapActivitys)
	{
		if (temp.second->isRepeat && temp.second->haveInit)
		{
			int i = 0;
			for (auto& activity : temp.second->arrActivitys)
			{
				UserData::getInstance()->setActivityReward(temp.second->storekey, i, 0);
				i++;
			}
			
		}
	}
}
