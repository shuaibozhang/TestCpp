#include "EffectAction.h"
#include "GLProgramMgr.h"


EffectAction::~EffectAction()
{
	/*if (_target)
	{
		_target->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}*/
}


EffectAction* EffectAction::create(float fDuration, EffectActionType_E type, bool keep)
{
	auto pRef = new EffectAction();
	if (pRef && pRef->initWithDuration(fDuration, type, keep))
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


bool EffectAction::initWithDuration(float fDuration, EffectActionType_E type, bool keep)
{
	ActionInterval::initWithDuration(fDuration);
	_bKeep = keep;
	_effectType = type;
	return true;
}


void EffectAction::startWithTarget(Node *pTarget)
{
	_target = dynamic_cast<Sprite*>(pTarget);
	CCAssert(_target != nullptr, "target must be sprite");
	ActionInterval::startWithTarget(pTarget);
	
	if (_bKeep)
	{
		return;
	}

	if (_effectType == EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT)
	{
		auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_DISSHOW);
		_target->setGLProgramState(glprogramstate);
	}
	else if (_effectType == EffectActionType_E::EFFECTACTION_GRADUAL_SHOW)
	{
		auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_SHOW);
		_target->setGLProgramState(glprogramstate);
	}
	else if (_effectType == EFFECTACTION_ARCHING_SHOW || _effectType == EFFECTACTION_ARCHING_SHOW_OUT)
	{
		auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA);
		_target->setGLProgramState(glprogramstate);
		glprogramstate->setUniformFloat("time", 0.f);
	}
	else if (_effectType == EFFECTACTION_DEFAULT)
	{
		_target->setGLProgramState(GLProgramMgr::getInstance()->getDefaultState());
	}
}


void EffectAction::update(float time)
{
	//ActionInterval::update(time);
	if (_effectType == EffectActionType_E::EFFECTACTION_GRADUAL_DISPPEAR_OUT || _effectType == EffectActionType_E::EFFECTACTION_GRADUAL_SHOW)
	{
		_target->getGLProgramState()->setUniformFloat("time", time);
	}
	else if (_effectType == EFFECTACTION_ARCHING_SHOW)
	{
		_target->getGLProgramState()->setUniformFloat("time", time);
	}
	else if (_effectType == EFFECTACTION_ARCHING_SHOW_OUT)
	{
		_target->getGLProgramState()->setUniformFloat("time", 1.f- time);
	}
}


//EffectMoveBy* EffectMoveBy::create(float dut, const Vec2& pos)
//{
//	auto pRef = new EffectMoveBy();
//	if (pRef && pRef->initWithDuration(dut, pos))
//	{
//		pRef->autorelease();
//		return pRef;
//	}
//	else
//	{
//		delete pRef;
//		return nullptr;
//	}
//}
//
//
//void EffectMoveBy::update(float time)
//{
//	MoveBy::update(time);
//	auto sp = dynamic_cast<Sprite*>(_target);
//	if (sp)
//	{
//		auto off = (_positionDelta * time);
//		if (off.y < 0.f)
//		{
//			off.y = -_positionDelta.y + off.y;
//		}
//		CCLOG("%2.f", -off.y);
//		sp->getGLProgramState()->setUniformFloat("offy", -off.y);
//	}
//}