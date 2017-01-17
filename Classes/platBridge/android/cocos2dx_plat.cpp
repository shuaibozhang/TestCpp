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
#include "PlatJniHelpers.h"

void cocos2dx_plat::vibrate(int duration)
{
    PlatJniHelpers::jniCommonVoidCall("Vibrate", "com/gl/jni/JniHelp", duration);
}


string cocos2dx_plat::getVersionName(void)
{
	string ret = PlatJniHelpers::jniCommonStringCall("getVersionName", "com/gl/jni/JniHelp");
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));
	
//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());
	
	return ret;
}


string cocos2dx_plat::getonlineTime(void)
{
	string ret = PlatJniHelpers::jniCommonStringCall("getOnlineTime", "com/gl/jni/JniHelp");
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));

//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());

	return ret;
}



string cocos2dx_plat::getChannelId(void)
{
	string ret = PlatJniHelpers::jniCommonStringCall("getChannelId", "com/gl/jni/JniHelp");
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));
	
//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());
	
	return ret;
}


string cocos2dx_plat::getSignCode(void)
{
	string ret = PlatJniHelpers::jniCommonStringCall("getSignCode", "com/gl/jni/JniHelp");
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));
	
//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());
	
	return ret;
}


void cocos2dx_plat::showToast(string info)
{
    PlatJniHelpers::jniCommonVoidCall("showToast", "com/gl/jni/JniHelp",info.c_str());
}


void cocos2dx_plat::showExitDialog(void)
{
    PlatJniHelpers::jniCommonVoidCall("showExitDialog", "com/gl/jni/JniHelp");
}


void cocos2dx_plat::gameExit(void)
{
    
}

void cocos2dx_plat::openMoreGameWeb(void)
{
    PlatJniHelpers::jniCommonVoidCall("showMore", "com/gl/jni/JniHelp");
}

string cocos2dx_plat::getIsSoundOn(void)
{	
	string ret = PlatJniHelpers::jniCommonStringCall("isSoundOn", "com/gl/jni/JniHelp");
	int strSize = ret.length() + 1;
	char *pStrRet = (char *)malloc(strSize * sizeof(char));

	memset((void *)pStrRet, 0, strSize);
	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());

	return pStrRet;	
}


void cocos2dx_plat::gamePause(void)
{
    PlatJniHelpers::jniCommonVoidCall("pauseGame", "com/gl/jni/JniHelp");
}


bool cocos2dx_plat::hasSensitiveWord(string text)
{
    return PlatJniHelpers::jniCommonBoolCall("hasSensitiveWord", "com/gl/jni/JniHelp", text.c_str());
}


string cocos2dx_plat::getGameValue(string key)
{
	string ret = PlatJniHelpers::jniCommonStringCall("getGameValue", "com/gl/jni/JniHelp", key.c_str());
//	int strSize = ret.length() + 1;
//	char *pStrRet = (char *)malloc(strSize * sizeof(char));
	
//	memset((void *)pStrRet, 0, strSize);
//	memcpy((void *)pStrRet, (void *)ret.c_str(), ret.length());
	
	return ret;
}


void cocos2dx_plat::setGameValue(string key, string strValue)
{
    PlatJniHelpers::jniCommonVoidCall("setGameValue", "com/gl/jni/JniHelp",key.c_str(),strValue.c_str());
}

string cocos2dx_plat::getUserId()
{
	string ret = PlatJniHelpers::jniCommonStringCall("getUserId", "com/gl/jni/JniHelp");
	return ret;
}