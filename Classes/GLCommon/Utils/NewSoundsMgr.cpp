#include "NewSoundsMgr.h"
//#include "ToolsUtil.h"

using namespace std;
USING_NS_CC;
using namespace cocos2d::experimental;

NewSoundsMgr* NewSoundsMgr::s_pInstance = NULL;

NewSoundsMgr::NewSoundsMgr()
	:m_strSoundFilePath(NULL)
{
}


NewSoundsMgr::~NewSoundsMgr()
{
	CC_SAFE_RELEASE_NULL(m_strSoundFilePath);
}


NewSoundsMgr* NewSoundsMgr::getInstance(void)
{
    if (NULL == s_pInstance)
    {
        s_pInstance = new NewSoundsMgr();
        s_pInstance->init();
    }
    
    return s_pInstance;
}


bool NewSoundsMgr::init()
{
	bool bRet = false;

	do 
	{
		setSoundFilePath("sounds");
		bRet = true;
	} while (0);

	return bRet;
}


void NewSoundsMgr::setSoundFilePath(const char* pszFilePath)
{
	m_strSoundFilePath = __String::create(pszFilePath);
	CC_SAFE_RETAIN(m_strSoundFilePath);
}


void NewSoundsMgr::preload(const char* pszFilePath, std::function<void(bool isSuccess)> callback)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	if (nullptr == callback)
	{
		AudioEngine::preload(strFilePath->getCString());
	}
	else
	{
		AudioEngine::preload(strFilePath->getCString(), callback);
	}
}


void NewSoundsMgr::unload(const char* pszFilePath)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	AudioEngine::uncache(strFilePath->getCString());
}


int NewSoundsMgr::play(const char* pszFilePath, bool bLoop, float volume)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	return AudioEngine::play2d(strFilePath->getCString(), bLoop, volume);
}


void NewSoundsMgr::stop(int nSoundId)
{
	AudioEngine::stop(nSoundId);
}


void NewSoundsMgr::stopAllEffect()
{
	AudioEngine::stopAll();
}