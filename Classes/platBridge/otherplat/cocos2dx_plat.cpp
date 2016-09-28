/*
 * Copyright (C) 2012 joyriver.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../cocos2dx_plat.h"
#include "cocos2d.h"
#include "GLCommon/Utils/JRTime.h"

void cocos2dx_plat::vibrate(int duration)
{
}

std::string cocos2dx_plat::getVersionName( void )
{
	return "1.000";
}

std::string cocos2dx_plat::getChannelId( void )
{
	return "1";
}


std::string cocos2dx_plat::getSignCode( void )
{
//	char* p = (char*)malloc(2*sizeof(char));
//	strcpy(p, "1");
	return "1";
}


void cocos2dx_plat::showToast(string info)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	CCLOG(("android toast :" + info).c_str());
#endif
}


void cocos2dx_plat::showExitDialog(void)
{
}

std::string cocos2dx_plat::getonlineTime( void )
{
	auto yera = JRTime::getCurYear();
	auto day = JRTime::getCurDayInYear();
	auto hour = JRTime::getCurHour();
	auto min = JRTime::getCurMinInHour();
	auto sec = JRTime::getCurSecInMin();
//	static char a[50];
	char a[50] = {0};
	sprintf(a, "%d-%d-%d-%d-%d", yera, day + 11, hour, min, sec);

	string ret(a);

	return ret;
	//return "error";
}


void cocos2dx_plat::openMoreGameWeb(void)
{

}


void cocos2dx_plat::gameExit()
{

}



string cocos2dx_plat::getIsSoundOn()
{
	return "true";
}


void cocos2dx_plat::gamePause(void)
{

}


bool cocos2dx_plat::hasSensitiveWord(string text)
{
	return false;
}

string cocos2dx_plat::getGameValue(string key)
{
	return "";
}

void cocos2dx_plat::setGameValue(string key, string strValue)
{
	return;
}