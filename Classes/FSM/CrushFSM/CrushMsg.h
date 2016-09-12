#ifndef __CRUSH_MSG_H__
#define __CRUSH_MSG_H__

#include <iostream>

using namespace std;

class CrushMsg
{
public:
	static const int CHANGE_STATE_ACT_TAG = 9;
	static const string TOUCH_BEGAN;
	static const string TOUCH_MOVED;
	static const string TOUCH_ENDED;
	static const string TOUCH_CANCELLED;
};


#endif
