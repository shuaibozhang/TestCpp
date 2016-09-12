#ifndef __GLPROGRAMMGR_H__
#define __GLPROGRAMMGR_H__

#include "cocos2d.h"

USING_NS_CC;

class GLProgramMgr : public Ref
{
public:
	~GLProgramMgr();

	static GLProgramMgr* getInstance();

	/*
	@param shadename must be get from one of down
	SHADER_EFFECT_NAME_GRADUAL_ALPHA,
	SHADER_EFFECT_NAME_GRADUAL_SHOW,
	SHADER_EFFECT_NAME_GRADUAL_DISSHOW,
	SHADER_EFFECT_NAME_BLUR,
	SHADER_EFFECT_NAME_GLOW
	*/
	GLProgramState * getUserStateWithName(const std::string& shadername);

	GLProgramState* getDefaultState(void);

	void reloadGLProgram();

	bool init();

	static const std::string SHADER_EFFECT_NAME_GRADUAL_ALPHA;
	static const std::string SHADER_EFFECT_NAME_GRADUAL_SHOW;
	static const std::string SHADER_EFFECT_NAME_GRADUAL_DISSHOW;
	static const std::string SHADER_EFFECT_NAME_BLUR;
	static const std::string SHADER_EFFECT_NAME_GLOW;
protected:
	GLProgramMgr() {};

	static GLProgramMgr* _instance;

	GLProgram* getOrCreateWithName(const std::string& shadername);

	std::map<std::string, std::string> _mapFile;
	Vector<Sprite*> _arrSpriteBak;
	EventListenerCustom* _backToForegroundlistener;
};
#endif
