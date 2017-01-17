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
	TIME_GIFTCHANGE = 5,
	TIME_TIMENODENUM = 6
};

enum TimeCountTimes
{
	TIME_GIFT_CHANGE = 24*60*60,
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
	bool isGetOnlineTime();

	int getCurOnlineTime() { return _curOnlineTime; };
	//int getCurTime();
	bool _getOnlineErr;
	void fixTime();
private:
	int _type;
	void resetStartTime();
	bool _getOnlineTime;
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

typedef std::function<void()> ccGetTimeCallBack;
class TimeMgr
{
public:
	~TimeMgr();
	static TimeMgr* getInstane();
	void init();
	//void addGetCB(ccGetTimeCallBack cb);
	void addTimeNode(TimeCountDownNode* cb);
	void removeTimeNode(TimeCountDownNode* cb);
	void updataTime(float dt);

	bool isGetOnlineTime() { return _getOnlineTime; }
	int getTime() { return _curOnlineTime; }
	void fixTime();

private:
	
	void threadFixTime();

	int _curOnlineTime;
	static TimeMgr* _instance;
	TimeMgr();
	EventListenerCustom* _backToForegroundlistener;
	bool _getOnlineTime;
	bool _getOnlineErr;
	std::vector<TimeCountDownNode*> _arrTimeNodes;
	bool _isFixing;
};

#endif
