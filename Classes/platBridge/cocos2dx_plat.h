//
//  cocos2dx_analyze.h
//  tadpole
//
//  Created by gary on 13-7-6.
//
//

#ifndef tadpole_cocos2dx_plat_h
#define tadpole_cocos2dx_plat_h

#include <iostream>
#include <string>

using namespace std;

class cocos2dx_plat {
    
public:
    //vibrate a duration in millisecends
    static void vibrate(int duration);
	static string getVersionName(void);
	static string getChannelId(void);
	static string getSignCode(void);
	static void showToast(string info);
	static void showExitDialog(void);
	static string getonlineTime(void);
	static void openMoreGameWeb(void);
	static string getIsSoundOn(void);
	static void gameExit(void);
	static void gamePause(void);
	static bool hasSensitiveWord(string text);
	static string getGameValue(string key);
	static void setGameValue(string key, string strValue);
	static string getUserId();
};

#endif
