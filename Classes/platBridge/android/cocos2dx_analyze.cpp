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
#include "PlatJniHelpers.h"


void cocos2dx_analyze::setLogEnabled(bool value)
{
    PlatJniHelpers::jniCommonVoidCall("setLogEnabled", "com/gl/jni/JniHelp", value);
}


void cocos2dx_analyze::addCustomEvent(string eventId, string value)
{
    PlatJniHelpers::jniCommonVoidCall("addCustomEvent", "com/gl/jni/JniHelp",eventId.c_str(),value.c_str());
}


void cocos2dx_analyze::beginLogPageView(const char *pageName)
{
    PlatJniHelpers::jniCommonVoidCall("beginLogPageView", "com/gl/jni/JniHelp", pageName);
}


void cocos2dx_analyze::endLogPageView(const char *pageName)
{
    PlatJniHelpers::jniCommonVoidCall("endLogPageView", "com/gl/jni/JniHelp", pageName);
}


void cocos2dx_analyze::updateOnlineConfig(void)
{
    PlatJniHelpers::jniCommonVoidCall("updateOnlineConfig", "com/gl/jni/JniHelp");
}


string cocos2dx_analyze::getOnlineValue(string key )
{
	string ret = PlatJniHelpers::jniCommonStringCall("getOnlineValue", "com/gl/jni/JniHelp", key.c_str());
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));
	
//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());
	
	return ret;
}


void cocos2dx_analyze::setUserLevel(const char *level)
{
    PlatJniHelpers::jniCommonVoidCall("setUserLevel", "com/gl/jni/JniHelp", level);
}


void cocos2dx_analyze::setUserInfo(const char * userId, int sex, int age, const char * platform)
{
    PlatJniHelpers::jniCommonVoidCall("setUserInfo", "com/gl/jni/JniHelp", userId, sex, age, platform);
}


void cocos2dx_analyze::startLevel(const char * level)
{
    PlatJniHelpers::jniCommonVoidCall("startLevel", "com/gl/jni/JniHelp", level);
}


void cocos2dx_analyze::finishLevel(const char * level)
{
    PlatJniHelpers::jniCommonVoidCall("finishLevel", "com/gl/jni/JniHelp", level);
}


void cocos2dx_analyze::failLevel(const char * level)
{
    PlatJniHelpers::jniCommonVoidCall("failLevel", "com/gl/jni/JniHelp", level);
}


void cocos2dx_analyze::pay(double cash, double coin, int source)
{
    PlatJniHelpers::jniCommonVoidCall("pay", "com/gl/jni/JniHelp", cash, coin, source);
}


void cocos2dx_analyze::pay(double cash, const char * item, int amount, double price, int source)
{
    PlatJniHelpers::jniCommonVoidCall("pay", "com/gl/jni/JniHelp", cash, item, amount, price, source);
}


void cocos2dx_analyze::buy(const char *item, int amount, double price)
{
    PlatJniHelpers::jniCommonVoidCall("buy", "com/gl/jni/JniHelp", item, amount, price);
}


void cocos2dx_analyze::use(const char * item, int amount, double price)
{
    PlatJniHelpers::jniCommonVoidCall("use", "com/gl/jni/JniHelp", item, amount, price);
}


void cocos2dx_analyze::bonus(double coin, int source)
{
    PlatJniHelpers::jniCommonVoidCall("bonus", "com/gl/jni/JniHelp", coin, source);
}


void cocos2dx_analyze::bonus(const char * item, int amount, double price, int source)
{
    PlatJniHelpers::jniCommonVoidCall("bonus", "com/gl/jni/JniHelp", item, amount, price, source);
}