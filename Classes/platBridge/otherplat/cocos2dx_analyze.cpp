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

#include "../cocos2dx_analyze.h"


void cocos2dx_analyze::setLogEnabled(bool value)
{

}


void cocos2dx_analyze::addCustomEvent(string eventId, string value)
{
}


void cocos2dx_analyze::beginLogPageView(const char *pageName)
{

}


void cocos2dx_analyze::endLogPageView(const char *pageName)
{

}


void cocos2dx_analyze::updateOnlineConfig(void)
{

}


std::string cocos2dx_analyze::getOnlineValue(string key )
{
// 	std::string strRet = "4";
// 	int strSize = strRet.length()+1;
// 	char *pStrRet = (char *)malloc(strSize);
// 
// 	memset((void *)pStrRet, 0, strSize);
// 	memcpy((void *)pStrRet, (void *)strRet.c_str(), strRet.length());

	return "4";
}


void cocos2dx_analyze::setUserLevel(const char *level)
{

}


void cocos2dx_analyze::setUserInfo(const char * userId, int sex, int age, const char * platform)
{

}


void cocos2dx_analyze::startLevel(const char * level)
{

}


void cocos2dx_analyze::finishLevel(const char * level)
{

}


void cocos2dx_analyze::failLevel(const char * level)
{

}


void cocos2dx_analyze::pay(double cash, double coin, int source)
{

}


void cocos2dx_analyze::pay(double cash, const char * item, int amount, double price, int source)
{

}


void cocos2dx_analyze::buy(const char *item, int amount, double price)
{

}


void cocos2dx_analyze::use(const char * item, int amount, double price)
{

}


void cocos2dx_analyze::bonus(double coin, int source)
{

}


void cocos2dx_analyze::bonus(const char * item, int amount, double price, int source)
{

}