#ifndef __GAMEMAP_H__
#define  __GAMEMAP_H__

#include "cocos2d.h"
#include "GLCommon/UI/GLFollowMap.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GLCommon/UI/TouchesScaleNode.h"
#include "effects/RTTNode.h"
#include "GLCommon/GLGraph.h"
#include "../effects/RTTNode.h"
#include "Defines.h"

USING_NS_CC;
using namespace ui;

enum class FightType
{
	NOR,
	ENDLESS,
	HARD,
	DUNGEON
};

class GameMap : public cocos2d::Layer
{
public:
	GameMap();
	~GameMap();

	static GameMap* getCurGameMap() { return s_curGameMap; }

	static GameMap* createWithIdx(int idx);
	bool initWithIdx(int idx);
	/*
	dur : can not be 0;
	*/
	void moveMap(float dur, float distance);
	void changeToMap(int idx);
	void restartCurMap();
	void changeToFight(int idx);
	void changeToWordMap(int idx);
	void checkChangeMap();
	virtual void update(float delta)override;
	void canMoveMap(bool pause) { _isMove = pause; }
	void setAutoMove(bool automove) { _autoMove = automove; }
	bool getAutoMove() { return _autoMove; }
	/*
	@param
	idx: storyfile idx
	posidx: 0 enter map, 1 meet boss, 2 boss end or leave map
	*/
	void loadStory(int posidx);
	bool checkShowStory(int posidx);
	void showStory(int posidx);
	void storyEnd(int posidx);	//0.stage start 1.boss	2.stage end

	void showResault(bool win, int exp, int gold, float* arr4);
	void hideResault();
	void showResaultTextAni(bool win, int exp, int gold, float* arr4);
	void showResaultNumInfo(bool win, int exp, int gold, float* arr4);

	void enterFight();
	void enterEndlessFight();
	void enterHard();
	void enterDungeon(int idx);

	void prepareMap(int idx);
	void preLoadMapRes();
	inline int getPrepareCount() { return _prepareCount; }
	void freeUnusedMap(int idx);

	void initMoveLayer();

	bool getIsShowScore() { return _isShowScore; }

	void moveUpEffect(float time, bool up);

	bool isCurMapUp() { return _isCurMapUp; }
	bool isInFight() { return _isInFight; }

	void quitFight();

	void pauseGame();
	void resumGame();

	void showLowHpEffect(bool enable);
	CC_SYNTHESIZE(FightType, _fightType, CurFightType);

	Node* getStorySkillNode() { return _storySkipNode; }

	void menuOnScoreFailed(Ref*, Widget::TouchEventType type);

	CC_SYNTHESIZE_READONLY(int, _dungeonIdx, DungeonIdx);
private:
	Node* _storySkipNode;
	bool _pause;
	bool _isCurMapUp;
	void moveMaps(float dt, bool automove);
	float _curMapLayer3Off;
	float _curMapLayer2Off;
	float _curMapLayer1Off;
	float _moveFactor[5];
	cocos2d::Node* _scoreNode;
	cocos2d::ui::Scale9Sprite* _spriteBg;
	int _prepareCount;
	std::queue<std::string> _pngQueue;
	static GameMap* s_curGameMap;
	int _idx;
	float _curDistance;
	cocos2d::Vector<cocos2d::Node*> _needMoveMapLayer;
	cocos2d::Node* _curMapNode_0;
	cocos2d::Node* _nextMapNode_0;
	cocos2d::Vector<cocos2d::Node*> _arrCurBgs;
	RTTNode* _rttNode;
	float _moveTotleTime;
	float _curMoveTime;
	bool _isMove;
	float _moveSpeed;
	bool _isChangingMap;
	bool _autoMove;
	Node* _bgLayer;
	bool _isShowScore;
	float _mapOffpos;

	float _mapStartY;
	bool _isInFight;

	int _scoreShowStep;

	float _resoultExp[4];
};


class WordMap : public cocos2d::Layer
{
public:
	WordMap();
	~WordMap();

	static WordMap *getInstance();
	static WordMap* createWordMap();
	bool initWordMap();

	void addJingYing(int pos);

	void setCanTouchMove(bool canTouch);

	void showDialog(Node* targetNode);
	//void showDialogBuyInStory(int idx, bool story = false);
	void showEnterStoryDiglog(int fileid, int pos);
	void showEnterFightDiglog(int pos, bool inmove = false);

	void loadMapConfig();
	void doSearchRoad(Node* start, Node* end);
	void hightLightRoad(Node* start, Node* end);
	void norCurLightRoad();
	void norLastLightRoad();

	void showShop(int idx);
	//bool checkStory(int idx);
	void menuOnShop(Ref*);
	void menuOnPoint(Ref*);

	bool checkEnemy(int playerpos);

	void enterEndless();
	void enterHardMode();

	void showTargetInfo(int idx);
	void unShowTargetInfo();
	
	cocos2d::Vector<cocos2d::Node*> getTargetInfoVector() { return _targetInfoVector; }
	int getSceneId(int pos);
	void checkAndPreMap();
	bool checkShowShop(Node* btn);
	bool checkShowShop(int posidx);
	virtual void update(float delta)override;

	void startMove();
	void endMove();

	int getCurPosIdx();

	void changeMapLineAndPoints(bool show);

	void setStartPos();

	void pauseGame();
	void resumGame();

	void updateJiantou();
	void hideJiantou();

	void hideFinget();

	bool checkIsBoss(int idx);

	void updataPointsBtns();

	void tryBackToLastPos();

	void updateBoxGetInfo(int idx);

	void updateNewMapBtn();

	static const int s_mapBtnNum = 81;
#if (1 == CC_ENABLE_NEW_WORLD)
	//20161023 J
	static const int s_newMapBtnNum = 80;
	static const int s_newStartIdx = 100;
	static const int s_blankStageNum = 18;

	void toNewMap();
	void toOldMap();
#endif
	void popNearyBoss();
private:
	bool _pauseGame;
	static WordMap *s_pInstance;
	cocos2d::Vector<cocos2d::Node*> _targetInfoVector;
	glui::TouchesScaleNode* _touchScaleNode;
	cocos2d::Size _mapSize;
	float _sizeMax;
	float _sizeMin;
	Node* _mapRoot;

#if (1 == CC_ENABLE_NEW_WORLD)
	//20161020 J
	Node* _newMapRoot;		
	Node * _newBtn[s_newMapBtnNum];
#endif
	Node* _btns[s_mapBtnNum];
	GLGraph _graph;
	Node* _players;
	int _curPosIdx;
	Node* _startNode;
	Node* _nextNode;
	Node* _curNode;
	Node* _endNode;
	std::vector<cocos2d::Node*> _vertorNode;
	Node* _dialogNode;
	cocos2d::Vector<cocos2d::Node*> _eagesVector;
	cocos2d::Vector<cocos2d::Node*> _eagesVectorLast;
	Action* _moveAction;
	bool _needMove;
	int _curNodeIdx;
	float _totalTime;
	float _curTime;
	Vec2 _curOffPos;
	bool _finishMoveRoad;
	Vec2 _startPos;
	bool _canTouch;
	int _arrGuaiQiaMapidx[s_mapBtnNum];
	Vec2 _pos[s_mapBtnNum];
	std::vector<Node*> _mapEageLineVector;
	std::map<std::string, Node*> _mapEageLineMap;

#if (1 == CC_ENABLE_NEW_WORLD)
	//20161023 J
	int _newArrGuaiQiaMapidx[s_newMapBtnNum];
	Vec2 _newPos[s_newMapBtnNum];
	std::vector<Node*> _newMapEageLineVector;
#endif
	bool _isShowPoints;
	cocos2d::Menu* _mapPointsMenu;

#if (1 == CC_ENABLE_NEW_WORLD)
	//20161020 J
	cocos2d::Menu* _newMapPointMenu;
#endif
	cocos2d::Node* _mapLinesNode;
	bool _endmove;

	cocos2d::Sprite* _jiantouStory;
	cocos2d::Sprite* _jiantouPlayer;

	Vec2 _jiantouCenter;
	Node* _fignerAni;
	ui::Button* _newMapBtn;
#if (1 == CC_ENABLE_NEW_WORLD)
	ui::Button* _oldMapBtn;
#endif
};
#endif
