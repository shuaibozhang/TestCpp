#include "GradualAlpha.h"
#include "GLProgramMgr.h"

GradualAlphaSprite * GradualAlphaSprite::create(const std::string& name, bool isPlist)
{
	auto pRef = new GradualAlphaSprite();
	if (pRef && pRef->init(name, isPlist))
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

bool GradualAlphaSprite::init(const std::string& name, bool isPlist)
{
	if (isPlist)
	{
		Sprite::initWithSpriteFrameName(name);
	}
	else
	{
		Sprite::initWithFile(name);
	}

	BlendFunc cbl = { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
	setBlendFunc(cbl);
	auto mask = Sprite::create("shader/groudnmix.jpg");

	auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA);
	setGLProgramState(glprogramstate);

	glprogramstate->setUniformTexture("u_texture1", mask->getTexture());

	return true;
}
