#ifndef __SHADERSPRITE_H__
#define  __SHADERSPRITE_H__

#include "cocos2d.h"

USING_NS_CC;

enum ShaderSprite_E
{
	SHADERSPRITE_DEFAULT = 0,
	SHADERSPRITE_GLOW,
	SHADERSPRITE_GRADUALALPHA,
};

class ShaderSprite : public Sprite
{
public:
	static void addShaderEffect(cocos2d::Sprite* sprite, ShaderSprite_E shadertype);

	static ShaderSprite* create(const std::string& name, bool isPlist = false);
	bool init(const std::string& name, bool isPlist);

	void setShaderEffect(ShaderSprite_E shadertype);
private:
	ShaderSprite_E _shaderType;
};
#endif
