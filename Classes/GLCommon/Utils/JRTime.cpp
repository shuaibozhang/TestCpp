#include "JRTime.h"

USING_NS_CC;

const char* JRTIME_LEAVE_DAY  = "jrtime_leave_day";
const char* JRTIME_LEAVE_SEC = "jrtime_leave_sec";

int JRTime::getCurYear(void)
{
	struct tm *tm;
	time_t timep;
	int year = 1900;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	year += tm->tm_year;

	return year;
}


int JRTime::getCurMonth(void)
{
	struct tm *tm;
	time_t timep;
	int month = 1;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	month += tm->tm_mon;

	return month;
}


int JRTime::getCurHour(void)
{
	struct tm *tm;
	time_t timep;
	int hour = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	hour = tm->tm_hour;

	return hour;
}


int JRTime::getMillisecondNow()
{
	struct timeval now; 
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
}


int JRTime::getCurDayInMonth(void)
{
	struct tm *tm;
	time_t timep;
	int day = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	day += tm->tm_mday;

	return day;
}


int JRTime::getCurDayInYear(void)
{
	struct tm *tm;
	time_t timep;
	int day = 1;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	day += tm->tm_yday;

	return day;
}


int JRTime::getCurMinInDay(void)
{
	struct tm *tm;
	time_t timep;
	int min = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	min = tm->tm_hour*60 + tm->tm_min;

	return min;
}

int JRTime::getCurSecInMin(void)
{
	struct tm *tm;
	time_t timep;
	int sec = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	sec = tm->tm_sec;

	return sec;
}


int JRTime::getCurSecInDay()
{
	struct tm *tm;
	time_t timep;
	int sec = 0;
	int min = 0;
	int hour = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	sec = tm->tm_sec;
	min = tm->tm_min;
	hour = tm->tm_hour;
	return (hour*60+ min)*60 + sec;
}


int JRTime::getCurMinInHour()
{
	struct tm *tm;
	time_t timep;
	int sec = 0;
	int min = 0;
	int hour = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now; 
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep);
	min = tm->tm_min;
	return min;
}


bool JRTime::isCurLeapYear(void)
{
	int curYear = JRTime::getCurYear();

	return JRTime::isLeapYear(curYear);
}


bool JRTime::isLeapYear(int nYear)
{
	bool bRet = false;

	if ((0 == nYear%4 && 0 != nYear%100) || (0 == nYear%400))
	{
		bRet = true;
	}

	return bRet;
}


int JRTime::getDayInYear(int nYear, int nMonth, int nDay)
{
	int nRet = 0;
	
	switch (nMonth-1)
	{
	case 12:
		nRet += 31;
		//need't break
	case 11:
		nRet += 30;
	case 10:
		nRet += 31;
	case 9:
		nRet += 30;
	case 8:
		nRet += 31;
	case 7:
		nRet += 31;
	case 6:
		nRet += 30;
	case 5:
		nRet += 31;
	case 4:
		nRet += 30;
	case 3:
		nRet += 31;
	case 2:
		{
			if (JRTime::isLeapYear(nYear))
			{
				nRet += 29;
			}
			else
			{
				nRet += 28;
			}
		}
	case 1:
		nRet += 31;
	default:
		break;
	}

	nRet += nDay;

	return nRet;
}


int JRTime::compareTheDay(int srcYear, int srcDay, int desYear, int desDay)
{
	int ret = 0;

	if (srcYear == desYear)
	{
		ret = desDay - srcDay;
	}
	else
	{
		bool isSmall = (srcYear < desYear);
		int minYear = isSmall ? srcYear : desYear;
		int maxYear = isSmall ? desYear : srcYear;
		int totalDay = 0;
		
		for (int i=minYear; i<maxYear; ++i)
		{
			int days = JRTime::isLeapYear(i) ? 366 : 365;

			totalDay += days;
		}

		if (isSmall)
		{
			totalDay = totalDay - srcDay + desDay;
		}
		else
		{
			totalDay = totalDay - desDay + srcDay;
			totalDay = -totalDay;
		}

		ret = totalDay;
	}

	return ret;
}


bool JRTime::isTheNextDay(int srcYear, int srcDay, int desYear, int desDay)
{
	bool bRet = false;

	if (srcYear == desYear)
	{
		if ((srcDay + 1) == desDay)
		{
			bRet = true;
		}
	}
	else if ((srcYear + 1) == desYear)
	{
		int totalDays = 365;

		if (JRTime::isLeapYear(srcYear))
		{
			totalDays = 366;
		}

		if (totalDays == srcDay && 1 == desDay)
		{
			bRet = true;
		}
	}

	return bRet;
}


bool JRTime::isTheSameDay(int srcYear, int srcDay, int desYear, int desDay)
{
	return (srcYear == desYear) && (srcDay == desDay);
}


bool JRTime::isTheSameWeek(int srcYear, int srcDay, int desYear, int desDay)
{
	bool bRet = false;

	//week start Saturday,2014-5-3 is Saturday
	int nMarkDay = JRTime::getDayInYear(2016, 6, 12);
	int nSrcDelt = JRTime::compareTheDay(2016, nMarkDay, srcYear, srcDay);
	int nDesDelt = JRTime::compareTheDay(2016, nMarkDay, desYear, desDay);

	if (nSrcDelt * nDesDelt < 0)
	{
		bRet = false;
	}
	else
	{
		if (nSrcDelt < 0)
		{
			nSrcDelt = (nSrcDelt + 1) * (-1);
			nDesDelt = (nDesDelt + 1) * (-1);
		}

		bRet = (nSrcDelt / 7) == (nDesDelt / 7);
	}

	return bRet;
}

bool JRTime::isTheSameWeek(int srcDay, int desDay)
{
	bool bRet = false;
	int nMarkDay = 0;// JRTime::getDayInYear(2016, 6, 12);
	int nSrcDelt = srcDay - nMarkDay;
	int nDesDelt = desDay - nMarkDay;

	if (nSrcDelt * nDesDelt < 0)
	{
		bRet = false;
	}
	else
	{
		if (nSrcDelt < 0)
		{
			nSrcDelt = (nSrcDelt + 1) * (-1);
			nDesDelt = (nDesDelt + 1) * (-1);
		}

		bRet = (nSrcDelt / 7) == (nDesDelt / 7);
	}

	return bRet;
}


void JRTime::parseTime(const char *strTime, int &desYear, int &desMonth, int &desDay)
{
	std::string strDate(strTime);
	std::string::size_type prePos = 0;
	std::string::size_type curPos = 0;
//	std::string::size_type srclen = strDate.length();

	/*year*/
	curPos = strDate.find("-", prePos);
	desYear = CCString::create(strDate.substr(prePos, curPos-prePos).c_str())->intValue();
	prePos = curPos + 1;
	/*month*/
	curPos = strDate.find("-", prePos);
	desMonth = CCString::create(strDate.substr(prePos, curPos-prePos).c_str())->intValue();
	prePos = curPos + 1;
	/*day*/
	curPos = strDate.find(" ", prePos);
	desDay = CCString::create(strDate.substr(prePos, curPos-prePos).c_str())->intValue();
}


int JRTime::getDayInWeek(int nYear, int nDay)
{
	int nRet = 0;
	//week start Monday,2014-5-5 is Monday
	int nMarkDay = JRTime::getDayInYear(2014, 5, 5);
	int nSrcDelt = JRTime::compareTheDay(2014, nMarkDay, nYear, nDay);

	nRet = (nSrcDelt % 7) + 1;

	return nRet;
}


int JRTime::compareTheMin(int srcYear, int srcDay, int srcMin, int desYear, int desDay, int desMin)
{
	int ret = 0;
	int dtDay = JRTime::compareTheDay(srcYear, srcDay, desYear, desDay);

	if (0 == dtDay)
	{
		ret = desMin - srcMin;
	}
	else if (dtDay > 0)
	{
		ret = (dtDay-1) * 24 * 60;

		ret += (desMin + 24 * 60 - srcMin);
	}
	else if (dtDay < 0)
	{
		ret = (-dtDay-1) * 24 * 60;
		ret += (srcMin + 24 * 60 - desMin);

		ret = -ret;
	}

	return ret;
}


void JRTime::saveLeaveTimeInfo(int idx)
{
	int year = JRTime::getCurYear();
	int day = JRTime::getCurDayInYear();
	int secInDay = JRTime::getCurSecInDay();

	auto tempstring = __String::createWithFormat("%d", idx)->getCString();

	if ((year * 1000 + day) > UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), 0))
	{
		UserDefault::getInstance()->setIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), year * 1000 + day);
	}

	UserDefault::getInstance()->setIntegerForKey((std::string(JRTIME_LEAVE_SEC) + tempstring).c_str(), secInDay);
}


int JRTime::getDurSec(int idx)
{
	int year = JRTime::getCurYear();
	int day = JRTime::getCurDayInYear();
	
	int durtime = 0;	

	auto tempstring = __String::createWithFormat("%d", idx)->getCString();
	if (UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), 0) == 0)
	{

	}
	else if ((year * 1000 + day) > UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), 0))
	{
		int lastleaveSec = UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_SEC) + tempstring).c_str());
		int secInDay = JRTime::getCurSecInDay();
		int passday = year * 1000 + day - UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), 0);
		durtime = secInDay + 24*60*60* passday - lastleaveSec;
	}
	else if((year * 1000 + day) == UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_DAY) + tempstring).c_str(), 0))
	{
		int lastleaveSec = UserDefault::getInstance()->getIntegerForKey((std::string(JRTIME_LEAVE_SEC) + tempstring).c_str());
		int secInDay = JRTime::getCurSecInDay();
		durtime = (secInDay - lastleaveSec) >= 0 ? (secInDay - lastleaveSec) : 0;		
	}
	else
	{
		
	}
	return durtime;
}
