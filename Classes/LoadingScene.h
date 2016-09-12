#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

class LoadingScene : public Layer
{
public:
	static Scene *createScene();
	CREATE_FUNC(LoadingScene);

	LoadingScene();
	~LoadingScene();

	virtual bool init();

	virtual void update(float delta) override;


	void addImageCallBack(Texture2D *pTexture, LoadResInfo_T *pResInfo);
	void loadGameData(int index);

	void initSkillResData();
	void initEffectResData();
//	void loadSkillImage();
//	void loadSkillImageCallback(Texture2D *pTexture, LoadResInfo_T resInfo);
private:
	Label *_pLblLoad;
	int _curLoadIndex;
	int _totalLoadStep;
//	std::thread* _loadingThread;
	bool _isLoadingRes;
	std::vector<LoadResInfo_T> _arrSkillRes;
//	int _skillResCount;
	int _tipIdx;
};

#endif
