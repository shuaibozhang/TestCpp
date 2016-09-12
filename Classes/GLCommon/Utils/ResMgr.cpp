
#include "ResMgr.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "ToolsUtil.h"

using namespace std;

ResMgr* ResMgr::s_gSharedResMgr = NULL;

ResMgr::ResMgr()
:mStrMgr(NULL),
m_strSoundFilePath(NULL)
{
}


ResMgr::~ResMgr()
{
    CC_SAFE_RELEASE_NULL(mStrMgr);
	CC_SAFE_RELEASE_NULL(m_strSoundFilePath);
}


ResMgr* ResMgr::getInstance(void)
{
    if (! s_gSharedResMgr)
    {
        s_gSharedResMgr = new ResMgr();
        s_gSharedResMgr->init();
    }
    
    return s_gSharedResMgr;
}


void ResMgr::purgeResMgr(void)
{
    CC_SAFE_RELEASE_NULL(s_gSharedResMgr);
}


void ResMgr::init()
{
    mStrMgr = Dictionary::createWithContentsOfFile("fonts/strings.xml");
    CC_SAFE_RETAIN(mStrMgr);
}


__Dictionary* ResMgr::getStrMgr(void)
{
    return mStrMgr;
}


__String *ResMgr::getString(const char* stringid)
{
    __String *textvalue = (__String*)mStrMgr->objectForKey(stringid);
    std::string text = textvalue->getCString();
    ToolsUtil::stringFindReplace(text, "\\n", "\n");
    
    __String *ret = __String::create(text);
    return ret;
}


RichText* ResMgr::getRichText(RichText* textRich, const char* stringid, float r, float g, float b, int size)
{
	__Array *textvalue = dynamic_cast<__Array*>(mStrMgr->objectForKey(stringid));
	for (int i = 0; i < textvalue->count(); i++)
	{
		auto temparray = (__Array*)textvalue->getObjectAtIndex(i);
		auto context = (__String*)temparray->getObjectAtIndex(0);
		if (temparray->count() == 1)
		{

			auto text = RichElementText::create(i,Color3B(r,g,b), 255, context->getCString(), "", size);
			textRich->pushBackElement(text);
		}
		else
		{
			auto num = (__String*)temparray->getObjectAtIndex(1);
			int colornum = atoi(num->getCString());
			float r = colornum/1000000;
			float g = colornum%1000000/1000;
			float b = colornum%1000;
			auto text = RichElementText::create(i,Color3B(r,g,b), 255, context->getCString(), "", size);
			textRich->pushBackElement(text);
		}
	}
	
	return textRich;
}


void ResMgr::setSoundFilePath(const char* pszFilePath)
{
	m_strSoundFilePath = __String::create(pszFilePath);
	CC_SAFE_RETAIN(m_strSoundFilePath);
}


void ResMgr::preloadEffect(const char* pszFilePath)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(strFilePath->getCString());
}


unsigned int ResMgr::playEffect(const char* pszFilePath, bool bLoop)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strFilePath->getCString(), bLoop);
}


void ResMgr::stopEffect(unsigned int nSoundId)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}


void ResMgr::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	__String *strFilePath = __String::createWithFormat("%s/%s", m_strSoundFilePath->getCString(), pszFilePath);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(strFilePath->getCString(), bLoop);
}