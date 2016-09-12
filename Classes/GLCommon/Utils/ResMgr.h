#ifndef __RESMGR_H__
#define __RESMGR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class ResMgr: public Ref
{
public:
	static ResMgr *getInstance(void);
    void init();
    
    __Dictionary* getStrMgr(void);
    void purgeResMgr(void);
	__String *getString(const char* stringid);
	RichText* getRichText(RichText* text, const char* stringid ,float r, float g, float b, int size =26);

	void setSoundFilePath(const char* pszFilePath);
	void preloadEffect(const char* pszFilePath);
	unsigned int playEffect(const char* pszFilePath, bool bLoop);	
	unsigned int playEffect(const char* pszFilePath) {
		return this->playEffect(pszFilePath, false);
	}
	void stopEffect(unsigned int nSoundId);

	void playBackgroundMusic(const char* pszFilePath, bool bLoop);
	void playBackgroundMusic(const char* pszFilePath) {
		this->playBackgroundMusic(pszFilePath, false);
	}
    
private:
	ResMgr();
	~ResMgr();
    
    static ResMgr *s_gSharedResMgr;
    __Dictionary *mStrMgr;
	__String *m_strSoundFilePath;
};

#endif  // __RESMGR_H__
