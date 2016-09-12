#include "SoundsMgr.h"
#include "ToolsUtil.h"
#include "SimpleAudioEngine.h"
#include <map>

using namespace std;
USING_NS_CC;

SoundsMgr* SoundsMgr::s_pInstance = NULL;

SoundsMgr::SoundsMgr()
	:m_strSoundFilePath(NULL)
{
}


SoundsMgr::~SoundsMgr()
{
	CC_SAFE_RELEASE_NULL(m_strSoundFilePath);

	EffectInfo *pEffectInfo = NULL;
	map<string, EffectInfo *>::iterator iter;

	for (iter=m_effectsMap.begin(); iter!=m_effectsMap.end(); ++iter)
	{
		pEffectInfo = iter->second;

		delete pEffectInfo;
		pEffectInfo = NULL;

		iter->second = NULL;
	}
}


SoundsMgr* SoundsMgr::getInstance(void)
{
    if (NULL == s_pInstance)
    {
        s_pInstance = new SoundsMgr();
        s_pInstance->init();
    }
    
    return s_pInstance;
}


bool SoundsMgr::init()
{
	bool bRet = false;

	do 
	{
		setSoundFilePath("sounds");
//		this->scheduleUpdate();
		bRet = true;
	} while (0);

	return bRet;
}


void SoundsMgr::update(float delta)
{
	EffectInfo *pEffectInfo = NULL;
	map<string, EffectInfo *>::iterator iter;

	for (iter=m_effectsMap.begin(); iter!=m_effectsMap.end(); ++iter)
	{
		pEffectInfo = iter->second;

		if (pEffectInfo->bIsPlaying)
		{
			pEffectInfo->fRemainTime -= delta;
						
			if (pEffectInfo->fRemainTime < 0.f)
			{
				this->stopEffectEx(pEffectInfo->strFileName.c_str());
				//pEffectInfo->bIsPlaying = false;
				//this->stopEffect(pEffectInfo->nSoundId);
			}
			/*
			pEffectInfo->bIsPlaying = this->isEffectPlaying(pEffectInfo->nSoundId);

			if (!pEffectInfo->bIsPlaying && pEffectInfo->fRemainTime > 0.f)
			{
				pEffectInfo->nSoundId = this->playEffect(pEffectInfo->strFileName.c_str());
			}*/
		}		
	}
}


void SoundsMgr::setSoundFilePath(const char* pszFilePath)
{
	m_strSoundFilePath = __String::create(pszFilePath);
	CC_SAFE_RETAIN(m_strSoundFilePath);
}


void SoundsMgr::preloadEffect(const char* pszFilePath)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(strFilePath->getCString());
}


void SoundsMgr::unloadEffect(const char* pszFilePath)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(strFilePath->getCString());
}


unsigned int SoundsMgr::playEffect(const char* pszFilePath, bool bLoop)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strFilePath->getCString(), bLoop);
}


void SoundsMgr::stopEffect(unsigned int nSoundId)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}


void SoundsMgr::stopAllEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}


void SoundsMgr::setEffectsVolume(int volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume / 100.f);
}


void SoundsMgr::setBackgroundMusicVolume(int volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume / 100.f);
}


/*
bool SoundsMgr::isEffectPlaying(unsigned int nSoundId)
{
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->isEffectPlaying(nSoundId);
}*/


unsigned int SoundsMgr::playEffectEx(const char* pszFilePath, float duration)
{
	unsigned int nSoundId = 0;
	map<string, EffectInfo *>::iterator item = m_effectsMap.find(pszFilePath);
	EffectInfo *pEffectInfo = NULL;

	if (m_effectsMap.end() == item)
	{
		//not exist
		pEffectInfo = new EffectInfo();
		pEffectInfo->strFileName = pszFilePath;
		pEffectInfo->bIsPlaying = false;

		m_effectsMap.insert(map<string, EffectInfo *>::value_type(pszFilePath, pEffectInfo));
	}
	else
	{
		//find it
		pEffectInfo = item->second;
	}

	if (!pEffectInfo->bIsPlaying)
	{
		CCLOG("playEffectEx pEffectInfo->bIsPlaying==false");
		nSoundId = this->playEffect(pszFilePath, true);
		pEffectInfo->nSoundId = nSoundId;
		pEffectInfo->bIsPlaying = true;
	}	

	pEffectInfo->fRemainTime = duration;

	return nSoundId;
}


void SoundsMgr::stopEffectEx(const char* pszFilePath)
{
	map<string, EffectInfo *>::iterator item = m_effectsMap.find(pszFilePath);
	EffectInfo *pEffectInfo = NULL;
	unsigned int nSoundId = 0;

	if (m_effectsMap.end() == item)
	{
		//not exist
	}
	else
	{
		//find it
		pEffectInfo = item->second;
		if (pEffectInfo->bIsPlaying)
		{
			nSoundId = pEffectInfo->nSoundId;
			pEffectInfo->fRemainTime = 0.f;
			pEffectInfo->nSoundId = 0;
			pEffectInfo->bIsPlaying = false;
			
			this->stopEffect(nSoundId);
		}
	}
}


void SoundsMgr::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(strFilePath->getCString(), bLoop);
}


void SoundsMgr::stopBackgroundMusic(bool bReleaseData)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
}