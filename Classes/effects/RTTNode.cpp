#include "RTTNode.h"
#include "GLProgramMgr.h"

RTTNode::~RTTNode()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundlistener);
}

RTTNode * RTTNode::create(int w, int h)
{
	auto pRef = new RTTNode();
	if (pRef && pRef->init(w, h))
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


bool RTTNode::init(int w, int h)
{
	RenderTexture::initWithWidthAndHeight(w, h, Texture2D::PixelFormat::RGBA8888, 0);
	setAutoDraw(true);
	//setKeepMatrix(true);
	_programState = nullptr;

	setClearColor(Color4F(0, 0, 0, 0));
	setClearFlags(GL_COLOR_BUFFER_BIT);
	_sprite->getTexture()->setAntiAliasTexParameters();
	//_sprite->runAction(MoveBy::create(5.f, Vec2(0.f, 400.f)));
	 _backToForegroundlistener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		CCLOG("GLProgramMgr:Dirty Uniform and Attributes of GLProgramState %d", 0);
		auto pos = _sprite->getPosition();
		RenderTexture::initWithWidthAndHeight(_fullviewPort.size.width, _fullviewPort.size.height, Texture2D::PixelFormat::RGBA8888, 0);
		this->setPosition(pos.x, pos.y);
		this->setRTTEffect(_effectType, true);
		setAutoDraw(true);
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundlistener, -1);
	return true;
}


void RTTNode::setPosition(float x, float y)
{
	_sprite->setPosition(x, y);
}

Action* RTTNode::runAction(Action* action)
{
	return _sprite->runAction(action);
}

void RTTNode::stopAction(Action* action)
{
	_sprite->stopAction(action);
}

void RTTNode::setRTTEffect(RTTEffect_E effect, bool fauseupadta)
{
	if (effect != _effectType || fauseupadta == true)
	{
		_effectType = effect;

		if (_effectType == RTTEffect_E::RTTEFFECT_LOWHP)
		{
			auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_BLUR);
			_sprite->setGLProgramState(glprogramstate);
		}
		else if (_effectType == RTTEffect_E::RTTEFFECT_DEFAULT)
		{
			_sprite->setGLProgramState(GLProgramMgr::getInstance()->getDefaultState());
		}
		else if (_effectType == RTTEffect_E::RTTEFFECT_GRADUALALPHA)
		{
			//BlendFunc cbl = { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
			//_sprite->setBlendFunc(cbl);

			auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA);
			_sprite->setGLProgramState(glprogramstate);
			glprogramstate->setUniformFloat("time", 0.f);
			//glprogramstate->setUniformFloat("offy", 0.f);
		}
	}
}
