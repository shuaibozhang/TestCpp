#ifndef __STORYMGR_H__
#define __STORYMGR_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h" 
#include "Commond.h"

USING_NS_CC;
using namespace rapidjson;

class WordMap;

typedef struct _StoryConfig_T
{
	int fileid;
	int posid;
	int nextfildid;
}StoryConfig_T;


class StoryMgr
{
public:
	~StoryMgr();
	static StoryMgr* getInstance();

	void initStory();
	void loadStoryFileByIdx(int idx, StoryConfig_T& config);
	void loadNext();
	void showTargetInfo(WordMap* wordmap);
	bool checkShowStory(int btnpos, bool isstart);
	void reloadStory();
	bool isStoryPos(int pos);
	bool isStoryOver();
	CC_SYNTHESIZE_READONLY(StoryConfig_T, _curStoryConfig, CurStoryConfig);

	inline CommandMgr* getCommandMgrByIdx(int idx) { return _arrCommandMgr[idx]; }
	static int s_storyMax;
private:
	static StoryMgr* _instance;
	StoryMgr();
	CommandMgr* _arrCommandMgr[3];
//	StoryConfig_T _curStoryConfig;
};
#endif
