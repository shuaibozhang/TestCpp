#include "GLActions.h"
#include "2d/CCProgressTimer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GLCommon/UI/GLSlidBar.h"

#define kProgressTimerCast ui::LoadingBar*

// implementation of ProgressTo

GLProgressTo* GLProgressTo::create(float duration, float percent, int type)
{
	GLProgressTo *progressTo = new (std::nothrow) GLProgressTo();
	progressTo->initWithDuration(duration, percent);
	progressTo->autorelease();
	progressTo->_type = type;

	return progressTo;
}

bool GLProgressTo::initWithDuration(float duration, float percent)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_to = percent;

		return true;
	}

	return false;
}

GLProgressTo* GLProgressTo::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) GLProgressTo();
	a->_type = _type;
	a->initWithDuration(_duration, _to);
	a->autorelease();
	return a;
}

GLProgressTo* GLProgressTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in ProgressTo");
	return nullptr;
}

void GLProgressTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_from = ((kProgressTimerCast)(target))->getPercent();
}

void GLProgressTo::update(float time)
{
	if (_type == 0)
	{
		((kProgressTimerCast)(_target))->setPercent(_from + (_to - _from) * time);
	}
	else if (_type == 1)
	{
		((glui::GLSlidBar*)(_target))->setPercent(_from + (_to - _from) * time);
	}
}

// implementation of ProgressFromTo

GLProgressFromTo* GLProgressFromTo::create(float duration, float fromPercentage, float toPercentage, int type)
{
	GLProgressFromTo *progressFromTo = new (std::nothrow) GLProgressFromTo();
	progressFromTo->initWithDuration(duration, fromPercentage, toPercentage);
	progressFromTo->autorelease();
	progressFromTo->_type = type;

	return progressFromTo;
}

bool GLProgressFromTo::initWithDuration(float duration, float fromPercentage, float toPercentage)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_to = toPercentage;
		_from = fromPercentage;

		return true;
	}

	return false;
}

GLProgressFromTo* GLProgressFromTo::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) GLProgressFromTo();
	a->initWithDuration(_duration, _from, _to);
	a->autorelease();
	return a;
}


GLProgressFromTo* GLProgressFromTo::reverse() const
{
	return GLProgressFromTo::create(_duration, _to, _from);
}

void GLProgressFromTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

void GLProgressFromTo::update(float time)
{
	if (_type == 0)
	{
		((kProgressTimerCast)(_target))->setPercent(_from + (_to - _from) * time);
	}
	else if (_type == 1)
	{
		((glui::GLSlidBar*)(_target))->setPercent(_from + (_to - _from) * time);
	}
	
}

GLTextNumTo * GLTextNumTo::create(float dur, int from, int to, std::string formet,int texttype)
{

	GLTextNumTo *progressTo = new (std::nothrow) GLTextNumTo();
	progressTo->initWithDuration(dur, from, to, formet, texttype);
	progressTo->autorelease();

	return progressTo;
}

void GLTextNumTo::startWithTarget(Node * target)
{
	ActionInterval::startWithTarget(target);

	if (_texttype == 0)
	{
		static_cast<ui::TextAtlas*>(target)->setString(String::createWithFormat(_formet.c_str(), _from)->getCString());
	}
	else
	{
		static_cast<ui::Text*>(target)->setString(String::createWithFormat(_formet.c_str(), _from)->getCString());
	}
}

void GLTextNumTo::update(float time)
{
	if (_texttype == 0)
	{
		((ui::TextAtlas*)(_target))->setString(String::createWithFormat(_formet.c_str(), (int)(_from + (_to - _from) * time))->getCString());
	}
	else
	{
		((ui::Text*)(_target))->setString(String::createWithFormat(_formet.c_str(), (int)(_from + (_to - _from) * time))->getCString());
	}
}

bool GLTextNumTo::initWithDuration(float duration, int fromPercentage, int toPercentage, std::string formet, int type)
{
	ActionInterval::initWithDuration(duration);
	_from = fromPercentage;
	_to = toPercentage;
	_texttype = type;
	_formet = formet;
	return true;
}
