#ifndef _BGLLAYER_H_
#define _BGLLAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

enum 
{
	ACTION_UNKNOW = 0,
	ACTION_LONGPRESS,
	ACTION_MOVE,
	ACTION_TIP,
};

class BGLLayer : public Layer
{
public:
	~BGLLayer();
	CREATE_FUNC(BGLLayer);

	virtual bool init()override;

	virtual void update(float delta)override;

	int checkTouchBtn(Vec2 touchpos);

	void handleActions();

	void menuOndel(Ref* btn, Widget::TouchEventType type);

	void changePartsNum(int idx, int numchange);
	/*
	* type = 0; put all in
	* type = 1; put all out
	*/
	void changeAllParts(int idx, int type);

	void armCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID);

	void getReward(float exp);

	void updateLvInfo();
	void updateNumsInfo();
	void showDes();

	void updateAllInBtn();
private:
	float _pressdur;
	bool _ispressed;
	int _pressIdx;
	bool _isDelPress;
	bool _isZhuruPress;
	bool _isPlayAni;

	Vec2 _startPressPos;
	int _curActionType;
	int _tempNums[40];
	ui::TextAtlas* _textTempNum[40];
	int _curpageidx;
	Button* _delBtns[40];
	Sprite* _pagetTips[4];
	Rect _touchRect;
	Armature* _mainArm;
	ProgressTimer *_lhProgress;
	ui::TextAtlas* _bgllv;
	ui::TextAtlas* _costGoldNum;
	float _totalExp;
	Vec2 _startpos;
	Node* _upNode;
	Node* _partsInfoNodes[40];
	Sprite* _selecIcon;
	Vec2 _iconPosBak[10];
	Node* _pageLayout[4];
	Button* _btnall;
};
#endif
