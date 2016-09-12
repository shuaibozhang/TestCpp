#ifndef __SOUNDS_MGR_H__
#define __SOUNDS_MGR_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

using namespace std;
USING_NS_CC;


class EffectInfo
{
public:
	bool bIsPlaying;
	unsigned int nSoundId;
	float fRemainTime;
	string strFileName;

	EffectInfo()
	{
		bIsPlaying = false;
		nSoundId = 0;
		fRemainTime = 0.f;
		strFileName = "";
	};
};

/*
typedef struct _EffectInfo 
{
	bool bIsPlaying;
//	bool bIsLoop;
	unsigned int nSoundId;
	float fRemainTime;
	string strfileName;
}EffectInfo;*/


class SoundsMgr : public Node
{
public:
	static SoundsMgr *getInstance(void);

	virtual bool init();
	virtual void update(float delta);

	void setSoundFilePath(const char* pszFilePath);
	void preloadEffect(const char* pszFilePath);
	unsigned int playEffect(const char* pszFilePath, bool bLoop);	
	unsigned int playEffect(const char* pszFilePath) {
		return this->playEffect(pszFilePath, false);
	}
	void unloadEffect(const char* pszFilePath);
	void stopEffect(unsigned int nSoundId);
	void stopAllEffect();
	void setEffectsVolume(int volume);
	void setBackgroundMusicVolume(int volume);
//	bool isEffectPlaying(unsigned int nSoundId);

	unsigned int playEffectEx(const char* pszFilePath, float duration);	
	unsigned int playEffectEx(const char* pszFilePath) {
		return this->playEffect(pszFilePath, 0.f);
	}
	void stopEffectEx(const char* pszFilePath);

	void playBackgroundMusic(const char* pszFilePath, bool bLoop);
	void playBackgroundMusic(const char* pszFilePath) {
		this->playBackgroundMusic(pszFilePath, false);
	}

	void stopBackgroundMusic(bool bReleaseData = false);
    
private:
	SoundsMgr();
	~SoundsMgr();
    
    static SoundsMgr *s_pInstance;
	__String *m_strSoundFilePath;
	map<string, EffectInfo *> m_effectsMap;
};

#endif  // __RESMGR_H__
