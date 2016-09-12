#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "State.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

template <class T>
class StateMachine
{
private:
	T *_pOwner;
// 	State<T> *_pCurState;
// 	State<T> *_pPreState;
// 	State<T> *_pGlobalState;

public:
	StateMachine(T *pOwner):_pOwner(pOwner),
		_pCurState(nullptr),
		_pPreState(nullptr),
		_pGlobalState(nullptr)
	{}

// 	void setCurrentState(State<T> *pState) { _pCurState = pState; }
// 	void setGlobalState(State<T> *pState) { _pGlobalState = pState; }
// 	void setPreviousState(State<T> *pState) { _pPreState = pState; }

	void update() const
	{
		if (nullptr != _pGlobalState)
		{
			_pGlobalState->execute(_pOwner);
		}

		if (nullptr != _pCurState)
		{
			_pCurState->execute(_pOwner);
		}
	}

	void changeState(State<T> *pNewState)
	{
		CCASSERT(nullptr != pNewState, "<StateMachine::changeState>:try to change to a null state");
		
		_pPreState = _pCurState;
		_pPreState->exit(_pOwner);

		_pCurState = pNewState;
		_pCurState->enter(_pOwner);
	}

	void revertToPreviousState()
	{
		changeState(_pPreState);
	}

	bool isInState(const State<T> *pState) const
	{
		return pState == _pCurState;
	}


	void handleMessage(EventCustom *event)
	{
		if (nullptr != _pCurState)
		{
			_pCurState->handleMessage(_pOwner, event);
		}
	}

	CC_SYNTHESIZE(State<T> *, _pCurState, CurState);
	CC_SYNTHESIZE(State<T> *, _pPreState, PreState);
	CC_SYNTHESIZE(State<T> *, _pGlobalState, GlobalState);
};


#endif
