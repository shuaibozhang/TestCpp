#ifndef __DUNGEONLAYER_H__
#define __DUNGEONLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

enum DungeonType
{
	DUNGEON_GOLD = 0,
	DUNGEON_EXP ,
	DUNGEON_WENPON
};

struct DungeonInfo_T
{
	DungeonType type;
	int _weekStage[7];
	int _timesCost[8];
};

class DungeonLayer : public Layer
{
public:
	DungeonLayer();
	~DungeonLayer();

	CREATE_FUNC(DungeonLayer);

	bool init();
	void menuOnGo(Ref*, Widget::TouchEventType type);
	void updateShowBtn();
private:
	Node* _mainRoot;
};


class DungeonMissionNode : public Node
{
public:
	DungeonMissionNode() {};
	~DungeonMissionNode() {};

	static DungeonMissionNode* nodeWithIdx(int idx);
	bool initWithIdx(int idx);
	void setString(const std::string& desstring);
	void setMissionNum(int cur, int target);
	int getMissionId() { return _idx; }
private:
	TextAtlas* _pMissionTextAtlas;
	int _idx;
	int _curStageIdx;
	Node* _root;
};
#endif
