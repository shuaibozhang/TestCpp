#include "TimeCountDownNode.h"
#include "platBridge/cocos2dx_plat.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "UserData.h"
#include "MainScene.h"
#include "Defines.h"

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
#ifdef USING_TIME_MGR
	if (TimeMgr::getInstane())
	{
		TimeMgr::getInstane()->removeTimeNode(this);
	}
	
#endif
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
#ifdef USING_TIME_MGR
	TimeMgr::getInstane()->addTimeNode(this);
#else
	_backToForegroundlistener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[this](EventCustom*)
	{
		fixTime();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_backToForegroundlistener, this);
#endif
	

	return true;
}

void TimeCountDownNode::click()
{
	if ( isGetOnlineTime())
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

#ifdef USING_TIME_MGR
		threadGetTime();
#else
		std::thread t1(&TimeCountDownNode::threadGetTime, this);
		t1.detach();
#endif
		

		_active = true;
	}
}

void TimeCountDownNode::setDur(int time)
{
	//auto curtime = MainLayer::getCurMainLayer()->getCurOnlineBoxTime();
	//UserData::getInstance()->setBoxCountDownStartTime(_idx, curtime);

	_timedur = time;
	_timedurBak = time;
	
	UserData::getInstance()->setBoxCountDownTime(_idx, _timedurBak);
	resetStartTime();
	_active = true;
//	UserData::getInstance()->saveUserData();
}

bool TimeCountDownNode::isGetOnlineTime()
{
	return TimeMgr::getInstane()->isGetOnlineTime();
}

void TimeCountDownNode::threadGetTime()
{
	_getOnlineErr = false;

#ifdef USING_TIME_MGR
	if (TimeMgr::getInstane()->isGetOnlineTime() == false)
	{
		_getOnlineErr = true;
	}
	else
	{
		if (UserData::getInstance()->getBoxCountDownStartTime(_idx) == -1111)
		{
			_startTime = TimeMgr::getInstane()->getTime();
			UserData::getInstance()->setBoxCountDownStartTime(_idx, _startTime);
		}
		else
		{
			_startTime = UserData::getInstance()->getBoxCountDownStartTime(_idx);
		}

		_curOnlineTime = TimeMgr::getInstane()->getTime();

		auto dur = TimeMgr::getInstane()->getTime() - _startTime;

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
#else
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
#endif
	
}


void TimeCountDownNode::threadFixTime()
{
	_getOnlineErr = false;
	
#ifdef USING_TIME_MGR
	if (TimeMgr::getInstane()->isGetOnlineTime() == false)
	{
		_getOnlineErr = true;
	}
	else
	{
		if (UserData::getInstance()->getBoxCountDownStartTime(_idx) == -1111)
		{
			_startTime = TimeMgr::getInstane()->getTime();
			UserData::getInstance()->setBoxCountDownStartTime(_idx, _startTime);
		}
		else
		{
			_startTime = UserData::getInstance()->getBoxCountDownStartTime(_idx);
		}

		_curOnlineTime = TimeMgr::getInstane()->getTime();

		auto dur = TimeMgr::getInstane()->getTime() - _startTime;

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
#else
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
#endif
	if (_timeGetCallBack)
	{
		_timeGetCallBack();
	}

}

void TimeCountDownNode::threadResetStartTime()
{
	_getOnlineErr = false;
	
#ifdef USING_TIME_MGR
	if (TimeMgr::getInstane()->isGetOnlineTime() == false)
	{
		_getOnlineErr = true;
	}
	else
	{
		auto curtime = TimeMgr::getInstane()->getTime();
		UserData::getInstance()->setBoxCountDownStartTime(_idx, curtime);

		_curOnlineTime = TimeMgr::getInstane()->getTime();

		_getOnlineTime = true;
	}
#else
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
#endif
}

void TimeCountDownNode::fixTime()
{
	_getOnlineTime = false;

#ifdef USING_TIME_MGR
	threadFixTime();
#else
	std::thread t1(&TimeCountDownNode::threadFixTime, this);
	t1.detach();
#endif
}

void TimeCountDownNode::resetStartTime()
{
	_getOnlineTime = false;
#ifdef USING_TIME_MGR
	threadResetStartTime();
#else
	std::thread t1(&TimeCountDownNode::threadResetStartTime, this);
	t1.detach();
#endif	
}

TimeMgr* TimeMgr::_instance = nullptr;

TimeMgr::TimeMgr():_backToForegroundlistener(nullptr),
_getOnlineTime(false),
_getOnlineErr(false),
_isFixing(false)
{

}

TimeMgr::~TimeMgr()
{
	
}

TimeMgr * TimeMgr::getInstane()
{
	if (_instance == nullptr)
	{
		_instance = new TimeMgr();
		_instance->init();

	}
	return _instance;
}

void TimeMgr::init()
{
	fixTime();
}

//void TimeMgr::addGetCB(ccGetTimeCallBack cb)
//{
//	
//}

void TimeMgr::addTimeNode(TimeCountDownNode * cb)
{
	_arrTimeNodes.push_back(cb);
}

void TimeMgr::removeTimeNode(TimeCountDownNode * cb)
{
	auto find =std::find(_arrTimeNodes.begin(), _arrTimeNodes.end(), cb);
	if (find != _arrTimeNodes.end())
	{
		_arrTimeNodes.erase(find);
	}
}

void TimeMgr::updataTime(float dt)
{
	if (_getOnlineTime)
	{
		_curOnlineTime++;
	}	
}

void TimeMgr::fixTime()
{
	if (!_isFixing)
	{
		_getOnlineTime = false;

		_isFixing = true;
		std::thread t1(&TimeMgr::threadFixTime, this);
		t1.detach();
	}
}

void TimeMgr::threadFixTime()
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

		_curOnlineTime = day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;
		_getOnlineTime = true;
	}

	for (auto& node: _arrTimeNodes)
	{
		node->fixTime();
	}

	_isFixing = false;
}
