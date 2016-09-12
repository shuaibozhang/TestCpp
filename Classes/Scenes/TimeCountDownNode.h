#ifndef _TIMECOUNTDOWNNODE_H_
#define _TIMECOUNTDOWNNODE_H_

#include "cocos2d.h"

USING_NS_CC;

enum TimeTypeEnum
{
	TIME_FREEGOLDEBOX = 1,
	TIME_SHOPHOTREFRESH = 2,
	TIME_OPENBOXCOUNTDOWN = 3,
	TIME_ONLINEREWARD = 4,
	TIME_TIMENODENUM = 5
};

class TimeCountDownNode : public Node
{
public:
	static TimeCountDownNode* createWithNode(int time, int idx, int type = 0);
public:
	TimeCountDownNode();
	~TimeCountDownNode();

	bool init(int time, int idx, int type);

	void click();
	//it will calculate dur int another thread, so if you want to use setDur() and startcountdown() , the order must be setDur first;
	void startCountDown();
	void setDur(int time);
	void setTimeCallBack(std::function<void(int)> func) { _click = func; }
	void reActive() { _active = true; }
	inline int getDur() { return _timedur; }
	std::function<void(void)> _timeGetCallBack;
	bool _getOnlineTime;

	int getCurOnlineTime() { return _curOnlineTime; };
	//int getCurTime();
	bool _getOnlineErr;
private:
	int _type;
	void fixTime();
	void resetStartTime();

	int _idx;
	void threadGetTime();
	void threadFixTime();
	void threadResetStartTime();
	int _startTime;
	int _timedurBak;
	int _timedur;
	int _curOnlineTime;
	ActionInterval* _timeDownAction;
	Node* _targetNode;
	std::function<void(int)> _click;
	bool _active;

	EventListenerCustom* _backToForegroundlistener;
};

#endif
