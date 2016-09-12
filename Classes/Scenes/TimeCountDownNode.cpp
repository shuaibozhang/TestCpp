#include "TimeCountDownNode.h"
#include "platBridge/cocos2dx_plat.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "UserData.h"

TimeCountDownNode * TimeCountDownNode::createWithNode( int time, int idx, int type)
{
	auto pRef = new TimeCountDownNode();
	if (pRef && pRef->init(time, idx, type))
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
}

TimeCountDownNode::TimeCountDownNode() :
	_timedur(0),
	_timeDownAction(nullptr),
	_active(false),
	_startTime(0),
	_targetNode(nullptr),
	_click(nullptr),
	_getOnlineTime(false),
	_backToForegroundlistener(nullptr),
	_idx(-1),
	_timeGetCallBack(nullptr),
	_curOnlineTime(-1),
	_getOnlineErr(false)
{
}

TimeCountDownNode::~TimeCountDownNode()
{
	if (_click)
	{
		_click = nullptr;
	}

	if (_backToForegroundlistener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundlistener);
	}

	UserData::getInstance()->saveUserData();
}

bool TimeCountDownNode::init( int time, int idx, int type)
{
	Node::init();
	_targetNode = this;
	_idx = idx;

	_type = type;

	_timedur = UserData::getInstance()->getBoxCountDownTime(_idx);
	
	if(_timedur == -1111)
	{
		_timedur = time;
	}

	_timedurBak = _timedur;
	
	UserData::getInstance()->setBoxCountDownTime(_idx, _timedurBak);

	_backToForegroundlistener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[this](EventCustom*)
	{
		fixTime();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_backToForegroundlistener, this);

	return true;
}

void TimeCountDownNode::click()
{
	if ( _getOnlineTime)
	{
		_timedur--;
		_curOnlineTime ++;

		if (_timedur >= 0)
		{
			if (_click)
			{
				_click(_timedur);
			}
		}
		else
		{
			_timedur = -1;

			_active = false;
			if (_click)
			{
				_click(_timedur);
			}
		}
	}
}

void TimeCountDownNode::startCountDown()
{
	if (_targetNode != nullptr)
	{
		_timeDownAction = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create(std::bind(&TimeCountDownNode::click, this)));
		_targetNode->runAction(RepeatForever::create(_timeDownAction));

		std::thread t1(&TimeCountDownNode::threadGetTime, this);
		t1.detach();

		_active = true;
	}
}

void TimeCountDownNode::setDur(int time)
{
	_timedur = time;
	_timedurBak = time;
	UserData::getInstance()->setBoxCountDownTime(_idx, _timedurBak);
	resetStartTime();
	_active = true;
	UserData::getInstance()->saveUserData();
}

void TimeCountDownNode::threadGetTime()
{
	_getOnlineErr = false;
	std::string timestring = cocos2dx_plat::getonlineTime();
	if (timestring.compare("error") == 0)
	{
		_getOnlineErr = true;
	}
	else
	{
		std::vector<std::string > vectorNumString;
		ToolsUtil::split(timestring, "-", vectorNumString);
		int year = atoi(vectorNumString[0].c_str());
		int day = atoi(vectorNumString[1].c_str());
		int hour = atoi(vectorNumString[2].c_str());
		int min = atoi(vectorNumString[3].c_str());
		int sec = atoi(vectorNumString[4].c_str());

		if (UserData::getInstance()->getBoxCountDownStartTime(_idx) == -1111)
		{
			_startTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;
			UserData::getInstance()->setBoxCountDownStartTime(_idx,_startTime);
		}
		else
		{
			_startTime = UserData::getInstance()->getBoxCountDownStartTime(_idx);
		}

		_curOnlineTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;

		auto dur = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec - _startTime;

		if (_type == 1)
		{
			dur = 0;
		}

		if (dur < 0)
		{
			_timedur = 0;
		}
		else
		{
			_timedur = _timedurBak - dur;
		}
		_getOnlineTime = true;
	}
}


void TimeCountDownNode::threadFixTime()
{
	_getOnlineErr = false;
	std::string timestring = cocos2dx_plat::getonlineTime();
	if (timestring.compare("error") == 0)
	{
		_getOnlineErr = true;
	}
	else
	{
		std::vector<std::string > vectorNumString;
		ToolsUtil::split(timestring, "-", vectorNumString);
		int year = atoi(vectorNumString[0].c_str());
		int day = atoi(vectorNumString[1].c_str());
		int hour = atoi(vectorNumString[2].c_str());
		int min = atoi(vectorNumString[3].c_str());
		int sec = atoi(vectorNumString[4].c_str());

		auto curtime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;

		if (UserData::getInstance()->getBoxCountDownStartTime(_idx) == -1111)
		{
			_startTime = curtime;
			UserData::getInstance()->setBoxCountDownStartTime(_idx, _startTime);
		}
		else
		{
			_startTime = UserData::getInstance()->getBoxCountDownStartTime(_idx);
		}

		_curOnlineTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;

		auto dur = curtime - _startTime;

		if (dur < 0)
		{
			_timedur = 0;
		}
		else
		{
			_timedur = _timedurBak - dur;
		}

		_getOnlineTime = true;
	}

	if (_timeGetCallBack)
	{
		_timeGetCallBack();
	}

}

void TimeCountDownNode::threadResetStartTime()
{
	_getOnlineErr = false;
	std::string timestring = cocos2dx_plat::getonlineTime();
	if (timestring.compare("error") == 0)
	{
		_getOnlineErr = true;
	}
	else
	{
		std::vector<std::string > vectorNumString;
		ToolsUtil::split(timestring, "-", vectorNumString);
		int year = atoi(vectorNumString[0].c_str());
		int day = atoi(vectorNumString[1].c_str());
		int hour = atoi(vectorNumString[2].c_str());
		int min = atoi(vectorNumString[3].c_str());
		int sec = atoi(vectorNumString[4].c_str());

		auto curtime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;
		UserData::getInstance()->setBoxCountDownStartTime(_idx, curtime);

		_curOnlineTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;

		_getOnlineTime = true;
	}
}

void TimeCountDownNode::fixTime()
{
	_getOnlineTime = false;

	std::thread t1(&TimeCountDownNode::threadFixTime, this);
	t1.detach();
}

void TimeCountDownNode::resetStartTime()
{
	_getOnlineTime = false;
	std::thread t1(&TimeCountDownNode::threadResetStartTime, this);
	t1.detach();
}
