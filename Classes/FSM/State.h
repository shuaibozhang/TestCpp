#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

template <class T>
class State
{
public:
	virtual ~State() {}
	virtual void enter(T *) = 0;
	virtual void execute(T *) = 0;
	virtual void exit(T *) = 0;
	virtual void handleMessage(T *, EventCustom *) = 0;
};

#endif