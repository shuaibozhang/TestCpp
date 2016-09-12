#ifndef __CRUSH_LAYER_H__
#define __CRUSH_LAYER_H__

#include "cocos2d.h"
#include "StateMachine.h"
#include "ParamData.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;

#define CC_SYNTHESIZE_ARR_READONLY(varType, varName, funName, arrDim)\
protected: varType varName[arrDim];\
public: virtual varType (*get##funName(void)) { return varName; }

#define CC_SYNTHESIZE_TWO_ARR_READONLY(varType, varName, funName, arrDim1, arrDim2)\
protected: varType varName[arrDim1][arrDim2];\
public: virtual varType (*get##funName(void))[arrDim2] { return varName; }

class EleIcon;
class Monster;

/*
typedef enum _GameState_E
{
	WAIT_CALC = 0,
	WAIT_TOUCH,
	WAIT_MOVE,
	WAIT_CRUSH,
	WAIT_CRUSH_ANIM,
}GameState_E;*/

class CrushLayer : public cocos2d::Layer
{
public:
	static CrushLayer *getInstance();
	CrushLayer();
	virtual ~CrushLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	bool initRoundData();

	// implement the "static create()" method manually
	CREATE_FUNC(CrushLayer);

	virtual void update(float delta);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void handleMessage(EventCustom *event);

//	Vec2 getElePos(int row, int column);
//	CrushIndex_T getCrushIndex(const Vec2 &pos);
	bool checkCrushTravelse();
//	bool travelseCrushEle(int row, int column, int arrIndex);
	bool isMoveCanCrush();

	bool doCrush();
	void doCreate();
	void doFall();
	bool doCleanCrush();
	bool doBottomCrush();

	bool createNewCrushEle();

	void createWeakEle(int skillId, int count);
	void createSpecialEle(int skillId, int count = 1);
	void addWeakEle(EleIcon *pEle);
	void removeWeakEle(EleIcon *pEle);

	void countingEle(int eleId, int dt);
	Vector<EleIcon *> &getWeakEle();

	bool isStoneState(int eleId);
	void setStoneState(int eleId, bool isStone);

	bool isCanFall();
	bool isCanSwapCrush();
	
	void animationEffectEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	void setTouchIndex(int row, int column) { _touchIndex.row = row; _touchIndex.column = column; }
	void setMoveIndex(int row, int column) { _moveIndex.row = row; _moveIndex.column = column; }
	CC_SYNTHESIZE(bool, _isFallFinish, IsFallFinish);
	CC_SYNTHESIZE(CrushIndex_T, _touchIndex, TouchIndex);
	CC_SYNTHESIZE(CrushIndex_T, _moveIndex, MoveIndex);
	CC_SYNTHESIZE(int, _curCrushArrCount, CurCrushArrCount);
	CC_SYNTHESIZE_READONLY(StateMachine<CrushLayer> *, _pStateMac, StateMac);

	EleIcon *getEleIcon(CrushIndex_T posIndex) { return getEleIcon(posIndex.row, posIndex.column); }
	EleIcon *getEleIcon(int row, int column);
	void setEleIcon(int row, int column, EleIcon *pEle) { _arrEleIcon[row][column] = pEle; }
	CC_SYNTHESIZE_TWO_ARR_READONLY(EleIcon *, _arrEleIcon, EleIconArr, ParamData::CRUSH_ROW, ParamData::CRUSH_COL);
	CC_SYNTHESIZE_TWO_ARR_READONLY(int, _arrCrushArrIndex, CrushArrIndexArr, ParamData::CRUSH_ROW, ParamData::CRUSH_COL);
//	CC_SYNTHESIZE_TWO_ARR_READONLY(bool, _arrIsTravelsed, IsTravelsedArr, ParamData::CRUSH_ROW, ParamData::CRUSH_COL);
	CC_SYNTHESIZE_ARR_READONLY(CrushArr_T, _arrCrushArrInfo, CrushArrInfoArr, ParamData::CRUSH_ARR_COUNT);
//	CC_SYNTHESIZE_ARR_READONLY(int, _arrCurEleCount, CurEleCountArr, 4);
	CC_SYNTHESIZE_ARR_READONLY(int, _arrCrushEleCount, CrushEleCount, 4);

	//minLv:0--(sampleCount-1)	arrEleId:-1 is invalid
	void createNewEleId(int (*arrEleId)[ParamData::CRUSH_COL], int (*arrEleState)[ParamData::CRUSH_COL], int sampleCount, int minLv, int maxLv);

	//travelType:1:horizontal 2:vertical 0:both
//	bool travelseEle(int row, int column, TravelseInfo_T *pCrushInfo, bool isCanCrush, int travelType);
	CC_SYNTHESIZE_READONLY(LayerColor *, _pShieldLayer, ShieldLayer);
	CC_SYNTHESIZE_READONLY(Node *, _pCrushArea, CrushArea);
private:
	int calcSwapEleWeight(int srcRow, int srcCol, int desRow, int desCol, TravelseInfo_T *pInfo);

	static CrushLayer *s_pInstance;
	Vec2 _crushAreaStartPos;
	
	ui::Text *_pLabel;
//	int _arrNewEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];
	Vector<EleIcon *> _arrWeakEle;
	std::vector<int> _arrNewEleId[ParamData::CRUSH_COL];

//	TravelseInfo_T _crushInfo;
	bool _arrIsStone[4];

// 	int _arrForbidCol[4];
// 	int _arrCreateCount[4];
// 	bool _isFirstCreate;
};

#endif // __HELLOWORLD_SCENE_H__
