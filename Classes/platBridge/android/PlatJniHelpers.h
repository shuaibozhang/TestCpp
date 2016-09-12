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

#ifndef __COCOS2DX_PLATJNIHELPERS__
#define __COCOS2DX_PLATJNIHELPERS__

#include <iostream>
#include <string>

using namespace std;

class PlatJniHelpers {
public:
	static void jniCommonVoidCall(const char* methodName, const char* classPath);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, int arg0);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, bool arg0);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, const char* arg1);	
	static void jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, int arg1);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, double arg1, int arg2);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, double arg2);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, double arg2, int arg3);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, int arg2, const char* arg3);
	static void jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, const char* arg1, int arg2, double arg3, int arg4);	
	static string jniCommonStringCall(const char* methodName, const char* classPath, const char* arg0);
	static string jniCommonStringCall(const char* methodName, const char* classPath);
	static bool jniCommonBoolCall(const char* methodName, const char* classPath, const char* arg0);
    
};

#endif /* defined(__COCOS2DX_PLATJNIHELPERS__) */
