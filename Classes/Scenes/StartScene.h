#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class StartScene : public Layer
{public:
	StartScene();
	~StartScene();
	static Scene* scene();
	CREATE_FUNC(StartScene);
	virtual bool init()override;
	void showHelp();
};

class StoryInfoLayer : public Layer
{
public:
	StoryInfoLayer();
	~StoryInfoLayer();
	CREATE_FUNC(StoryInfoLayer);
	virtual bool init()override;

	static int getCurShowTextIdx();
private:
	Node* _root;
	Node* _textNode[7];
	ui::Text* _curNewText;
};

#endif // _STARTSCENE_H_
