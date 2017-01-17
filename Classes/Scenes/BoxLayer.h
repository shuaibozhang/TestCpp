#ifndef _BOXLAYER_H_
#define _BOXLAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
class GameButton;
class TimeCountDownNode;

class BoxLayer : public Layer
{
public:
	BoxLayer();
	~BoxLayer();

	CREATE_FUNC(BoxLayer);
	bool init()override;
	void menuOnBox(Ref*, Widget::TouchEventType type);
	
	void getReward(int boxidx, int lv, int pos=0);	//0.top box 1.nor box
	void clik();

	void showBoxget(const std::vector<int>& itemids, const std::vector<int>& nums);

	TimeCountDownNode* _curtimenode;

	//time
	void switchTimeShow(int idx, bool showtime);
	void updateTimePic(int idx, int hour);

	void updateBoxOpen();

	void showBoxInfo(int idx);
	void resetBoxInfo();

	void switchBoxLeftStage();

	void getFreeBox();
private:

	Node* _boxGameBtn[4];
	Vec2 _boxPos[4];
	TextAtlas* _getcost;
	int _selectPopBoxIdx;
	//time
	/*0 : hour is0 1: hour  > 0*/
	int _lastTimeState[2];

	TextAtlas* _hour_0;
	TextAtlas* _min_0;
	TextAtlas* _hour_1;
	TextAtlas* _min_1;
	Sprite* _texttouch_0;
	Sprite* _texttouch_1;
	Node* _timeNode0;
	Node* _timeNode1;
	Sprite* _timedanwei[4];
	cocostudio::Armature* _ani0;
	cocostudio::Armature* _ani1;

	Node* _winInfoRoot;
	LoadingBar* _winTimesBar;
	Text* _winTimeText;

	TextAtlas* _unLockingTime;
	TextAtlas* _unLockingCost;
	int _freeGetTimeIdx;
};
#endif
