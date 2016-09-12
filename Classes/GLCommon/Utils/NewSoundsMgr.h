#ifndef __NEW_SOUNDS_MGR_H__
#define __NEW_SOUNDS_MGR_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

using namespace std;
USING_NS_CC;


class NewSoundsMgr : public Node
{
public:
	static NewSoundsMgr *getInstance(void);

	virtual bool init();

	void setSoundFilePath(const char* pszFilePath);
	void preload(const char* pszFilePath, std::function<void(bool isSuccess)> callback = nullptr);
	int play(const char* pszFilePath, bool bLoop=false, float volume=1.f);	
	void unload(const char* pszFilePath);
	void stop(int nSoundId);
	void stopAllEffect();
    
private:
	NewSoundsMgr();
	~NewSoundsMgr();
    
    static NewSoundsMgr *s_pInstance;
	__String *m_strSoundFilePath;
};

#endif  // __RESMGR_H__
