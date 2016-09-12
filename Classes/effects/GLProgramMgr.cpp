#include "GLProgramMgr.h"

const std::string GLProgramMgr::SHADER_EFFECT_NAME_BLUR = "shader_blur";
const std::string GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_ALPHA = "shader_gradual_alpha";
const std::string GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_SHOW = "shader_gradual_show";
const std::string GLProgramMgr::SHADER_EFFECT_NAME_GRADUAL_DISSHOW = "shader_gradual_disshow";
const std::string GLProgramMgr::SHADER_EFFECT_NAME_GLOW = "shader_glow";

GLProgramMgr* GLProgramMgr::_instance = nullptr;


GLProgramMgr::~GLProgramMgr()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundlistener);
	_arrSpriteBak.clear();
}


bool GLProgramMgr::init()
{
	_mapFile.insert(std::pair<std::string, std::string>(SHADER_EFFECT_NAME_BLUR, "shader/lowhp"));
	_mapFile.insert(std::pair<std::string, std::string>(SHADER_EFFECT_NAME_GRADUAL_ALPHA, "shader/groundmix2"));
	_mapFile.insert(std::pair<std::string, std::string>(SHADER_EFFECT_NAME_GRADUAL_SHOW, "shader/gradual_show"));
	_mapFile.insert(std::pair<std::string, std::string>(SHADER_EFFECT_NAME_GRADUAL_DISSHOW, "shader/gradual_disappear"));
	_mapFile.insert(std::pair<std::string, std::string>(SHADER_EFFECT_NAME_GLOW, "shader/glowfade"));

	auto mask0 = Sprite::create("shader/groudnmix2.jpg");
	auto mask1 = Sprite::create("shader/mask0.png");
	auto mask2 = Sprite::create("shader/cooldown3.png");

	_arrSpriteBak.pushBack(mask0);
	_arrSpriteBak.pushBack(mask1);
	_arrSpriteBak.pushBack(mask2);

	CCLOG("GLProgramMgr: create rendererRecreatedListener for GLProgramState %d",0);
	_backToForegroundlistener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		CCLOG("GLProgramMgr:Dirty Uniform and Attributes of GLProgramState %d", 0);
		reloadGLProgram();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundlistener, -1);

	return true;
}


GLProgram* GLProgramMgr::getOrCreateWithName(const std::string& shadername)
{
	auto glprogram = GLProgramCache::getInstance()->getGLProgram(shadername);

	if (!glprogram) {
		glprogram = GLProgram::createWithFilenames(_mapFile[shadername] + ".vsh", _mapFile[shadername] + ".fsh");
		GLProgramCache::getInstance()->addGLProgram(glprogram, shadername);
	}

	return glprogram;
}


GLProgramState* GLProgramMgr::getUserStateWithName(const std::string& shadername)
{
	auto glprogram = this->getOrCreateWithName(shadername);
	auto state = GLProgramState::create(glprogram);

	if (SHADER_EFFECT_NAME_GRADUAL_ALPHA.compare(shadername) == 0)
	{
		auto mask = Sprite::create("shader/groudnmix2.jpg");
		state->setUniformTexture("u_texture1", mask->getTexture());
	}
	else if (SHADER_EFFECT_NAME_GLOW.compare(shadername) == 0)
	{
		state->setUniformVec3("colorglow", Vec3(1, 1, 0));
	}
	else if (SHADER_EFFECT_NAME_BLUR.compare(shadername) == 0)
	{
		auto mask = Sprite::create("shader/mask0.png");
		state->setUniformTexture("u_texture1", mask->getTexture());
	}
	else if (SHADER_EFFECT_NAME_GRADUAL_SHOW.compare(shadername) == 0)
	{
		auto mask = Sprite::create("shader/cooldown3.png");
		state->setUniformTexture("u_texture1", mask->getTexture());
	}
	else if (SHADER_EFFECT_NAME_GRADUAL_DISSHOW.compare(shadername) == 0)
	{
		auto mask = Sprite::create("shader/cooldown3.png");
		state->setUniformTexture("u_texture1", mask->getTexture());
	}

	 return state;
}


GLProgramState* GLProgramMgr::getDefaultState()
{	
	return GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
}


void GLProgramMgr::reloadGLProgram()
{
	for (auto it = _mapFile.begin(); it != _mapFile.end(); ++it)
	{
		auto shadername = it->first;
		auto p = this->getOrCreateWithName(shadername);
		p->reset();
		p->initWithFilenames(_mapFile[shadername] + ".vsh", _mapFile[shadername] + ".fsh");
		p->link();
		p->updateUniforms();
		CCLOG("reload shader %d", 0);
	}
}


GLProgramMgr* GLProgramMgr::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GLProgramMgr();
		_instance->init();
	}

	return _instance;
}