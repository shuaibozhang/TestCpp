#include "ShaderSprite.h"
#include "GLProgramMgr.h"

ShaderSprite * ShaderSprite::create(const std::string& name, bool isPlist)
{
	auto pRef = new ShaderSprite();
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

bool ShaderSprite::init(const std::string& name, bool isPlist)
{
	if (isPlist)
	{
		Sprite::initWithSpriteFrameName(name);
	}
	else
	{
		Sprite::initWithFile(name);
	}

	_shaderType = ShaderSprite_E::SHADERSPRITE_DEFAULT;

	return true;
}


void ShaderSprite::addShaderEffect(cocos2d::Sprite* sprite, ShaderSprite_E shadertype)
{
	if (shadertype == ShaderSprite_E::SHADERSPRITE_GLOW)
	{
		BlendFunc cbl = { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
		sprite->setBlendFunc(cbl);

		auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GLOW);
		sprite->setGLProgramState(glprogramstate);
	}
	else if (shadertype == ShaderSprite_E::SHADERSPRITE_GRADUALALPHA)
	{
		auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA);
		sprite->setGLProgramState(glprogramstate);
		glprogramstate->setUniformFloat("time", 0.f); 
		//glprogramstate->setUniformFloat("offy", 0.f);
	}
	else if (shadertype == ShaderSprite_E::SHADERSPRITE_DEFAULT)
	{
		BlendFunc cbl = BlendFunc::ALPHA_PREMULTIPLIED;
		sprite->setBlendFunc(cbl);
		sprite->setGLProgramState(GLProgramMgr::getInstance()->getDefaultState());
	}
}


void ShaderSprite::setShaderEffect(ShaderSprite_E shadertype)
{
	if (shadertype != _shaderType)
	{
		_shaderType = shadertype;

		if (_shaderType == ShaderSprite_E::SHADERSPRITE_GLOW)
		{
			BlendFunc cbl = { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
			setBlendFunc(cbl);

			auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GLOW);
			setGLProgramState(glprogramstate);
		}
		else if (_shaderType == ShaderSprite_E::SHADERSPRITE_GRADUALALPHA)
		{
			//BlendFunc cbl = { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
			//setBlendFunc(cbl);
		
			auto glprogramstate = GLProgramMgr::getInstance()->getUserStateWithName(GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA);
			setGLProgramState(glprogramstate);
			glprogramstate->setUniformFloat("time", 0.f);
			//glprogramstate->setUniformFloat("offy", 0.f);
			
		}
		else if (_shaderType == ShaderSprite_E::SHADERSPRITE_DEFAULT)
		{
			BlendFunc cbl = BlendFunc::ALPHA_PREMULTIPLIED;
			setBlendFunc(cbl);
			setGLProgramState(GLProgramMgr::getInstance()->getDefaultState());
		}
	}
}
