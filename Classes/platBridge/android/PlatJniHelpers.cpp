/*
 * Copyright (C) 2012 Soomla Inc.
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

#include "PlatJniHelpers.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "()V");
    
    if (!isHave) {
        //do nothing
    } else {
    	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    	minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, int arg0)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(I)V");
    
    if (!isHave) {
        //do nothing
    } else {
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arg0);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, bool arg0)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Z)V");
    
    if (!isHave) {
        //do nothing
    } else {
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arg0);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;)V");
    
    if (!isHave) {
        //do nothing
    } else {
        jstring stringArg0 = minfo.env->NewStringUTF(arg0);
		
    	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg0);
		
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(stringArg0);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, const char* arg1)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;Ljava/lang/String;)V");
    
    if (!isHave) {
        //do nothing
    } else {
        jstring stringArg0 = minfo.env->NewStringUTF(arg0);
        jstring stringArg1 = minfo.env->NewStringUTF(arg1);
		
    	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg0, stringArg1);
		
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(stringArg0);
        minfo.env->DeleteLocalRef(stringArg1);
    }
}


string PlatJniHelpers::jniCommonStringCall(const char* methodName, const char* classPath, const char* arg0) 
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;)Ljava/lang/String;"); 
    
    if (!isHave) {
        //do nothing
    } else {
		jstring stringArg0 = minfo.env->NewStringUTF(arg0);
		
        jstring retString = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, stringArg0);
		
		minfo.env->DeleteLocalRef(stringArg0);
		
		const char *nativeString = minfo.env->GetStringUTFChars(retString, 0);
		string ret(nativeString);
		minfo.env->ReleaseStringUTFChars(retString, nativeString);
        minfo.env->DeleteLocalRef(minfo.classID);
		
		return ret;
    }
	
	return NULL;
}


string PlatJniHelpers::jniCommonStringCall(const char* methodName, const char* classPath) 
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "()Ljava/lang/String;"); 
    
    if (!isHave) {
        //do nothing
    } else {
		
    jstring retString = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
				
		const char *nativeString = minfo.env->GetStringUTFChars(retString, 0);
		string ret(nativeString);
		minfo.env->ReleaseStringUTFChars(retString, nativeString);
    minfo.env->DeleteLocalRef(minfo.classID);
		
		return ret;
    }
	
	return NULL;
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, int arg1)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(DI)V");
    
    if (!isHave) {
        //do nothing
    } else {
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arg0, arg1);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, double arg1, int arg2)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(DDI)V");
    
    if (!isHave) {
        //do nothing
    } else {
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arg0, arg1, arg2);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, double arg2)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;ID)V");
    
    if (!isHave) {
        //do nothing
    } else {
			jstring stringArg0 = minfo.env->NewStringUTF(arg0);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg0, arg1, arg2);
			minfo.env->DeleteLocalRef(stringArg0);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, double arg2, int arg3)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;IDI)V");
    
    if (!isHave) {
        //do nothing
    } else {
			jstring stringArg0 = minfo.env->NewStringUTF(arg0);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg0, arg1, arg2, arg3);
			minfo.env->DeleteLocalRef(stringArg0);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, const char* arg0, int arg1, int arg2, const char* arg3)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;IILjava/lang/String;)V");
    
    if (!isHave) {
        //do nothing
    } else {
			jstring stringArg0 = minfo.env->NewStringUTF(arg0);
			jstring stringArg3 = minfo.env->NewStringUTF(arg3);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg0, arg1, arg2, stringArg3);
			minfo.env->DeleteLocalRef(stringArg0);
			minfo.env->DeleteLocalRef(stringArg3);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void PlatJniHelpers::jniCommonVoidCall(const char* methodName, const char* classPath, double arg0, const char* arg1, int arg2, double arg3, int arg4)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(DLjava/lang/String;IDI)V");
    
    if (!isHave) {
        //do nothing
    } else {
			jstring stringArg1 = minfo.env->NewStringUTF(arg1);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arg0, stringArg1, arg2, arg3, arg4);
			minfo.env->DeleteLocalRef(stringArg1);
			minfo.env->DeleteLocalRef(minfo.classID);
    }
}


bool PlatJniHelpers::jniCommonBoolCall(const char* methodName, const char* classPath, const char* arg0)
{
    cocos2d::JniMethodInfo minfo;
	
    bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo,classPath,methodName, "(Ljava/lang/String;)Z"); 
    
    if (!isHave) {
        //do nothing
    } else {
    	jstring stringArg0 = minfo.env->NewStringUTF(arg0);
    	jboolean retBool = (jboolean) minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, stringArg0);
			bool ret = retBool;
			
      minfo.env->DeleteLocalRef(stringArg0);
    	minfo.env->DeleteLocalRef(minfo.classID);
		
		return ret;
    }
	
	return false;
}