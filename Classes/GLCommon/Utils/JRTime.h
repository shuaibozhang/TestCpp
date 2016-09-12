#ifndef __JR_TIME_H__
#define __JR_TIME_H__

#include "cocos2d.h"

USING_NS_CC;


class JRTime
{
public:
	static int getCurYear(void);
	static int getCurMonth(void);
	static int getCurHour(void);
	static int getCurDayInMonth(void);
	static int getCurDayInYear(void);
	static int getCurMinInDay(void);
	static int getCurSecInMin(void);
	static int getCurSecInDay(void);
	static int getCurMinInHour(void);

	static bool isCurLeapYear(void);
	static bool isLeapYear(int nYear);
	static int getDayInYear(int nYear, int nMonth, int nDay);
	static int compareTheDay(int srcYear, int srcDay, int desYear, int desDay);
	static bool isTheNextDay(int srcYear, int srcDay, int desYear, int desDay);
	static bool isTheSameDay(int srcYear, int srcDay, int desYear, int desDay);
	static bool isTheSameWeek(int srcYear, int srcDay, int desYear, int desDay);
	static bool isTheSameWeek(int srcDay, int desDay);	//day start 2016.1.1
	static void parseTime(const char *strTime, int &desYear, int &desMonth, int &desDay);
	static int getDayInWeek(int nYear, int nDay);
	static int compareTheMin(int srcYear, int srcDay, int srcMin, int desYear, int desDay, int desMin);
	static int getMillisecondNow(void);
	static void saveLeaveTimeInfo(int idx);
	static int getDurSec(int idx);
};

#endif