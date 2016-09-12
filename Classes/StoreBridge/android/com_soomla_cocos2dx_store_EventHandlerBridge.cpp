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

#include "com_soomla_cocos2dx_store_EventHandlerBridge.h"

#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

#include "../cocos2dx_EventHandlers.h"

#include <string>

using namespace std;


JNIEXPORT void JNICALL Java_com_soomla_cocos2dx_store_EventHandlerBridge_playPurchaseCancelled
  (JNIEnv * env, jobject obj, jstring itemId){
   	  string itemIdStr = cocos2d::JniHelper::jstring2string(itemId);
   	  string msg("Market purchase just cancelled for: ");
   	  msg += itemIdStr;
   	  __android_log_write(ANDROID_LOG_DEBUG, "SOOMLA JNI", msg.c_str());

   	  cocos2dx_EventHandlers::getInstance()->marketPurchaseCancelled(itemIdStr);
  }

JNIEXPORT void JNICALL Java_com_soomla_cocos2dx_store_EventHandlerBridge_playPurchase
  (JNIEnv * env, jobject obj, jstring itemId){
   	  string itemIdStr = cocos2d::JniHelper::jstring2string(itemId);
   	  string msg("Market purchase for: ");
  	  msg += itemIdStr;
   	  __android_log_write(ANDROID_LOG_DEBUG, "SOOMLA JNI", msg.c_str());

   	  cocos2dx_EventHandlers::getInstance()->marketPurchase(itemIdStr);
  }