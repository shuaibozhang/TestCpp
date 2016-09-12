#ifndef __GUIDE_LAYER_H__
#define __GUIDE_LAYER_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

typedef enum _GuideZOrder
{
	Z_BLACK_BG = 0,
	Z_STENCIL,
	Z_TIPS_ARR,
	Z_DIALOG_BG,
	Z_TEXT
}GuideZOrder_E;

typedef struct _GuideDetailInfo
{
	int infoType;	//0.dialog 1.role dialog 2.monster dialog 3.create icon 4.swap tip 5.highlight
	int param1;	//dialogId || iconId  || swap type || base on crush_bg_height || base on screen height || base on crush era
	int param2;	//roleId || rowIndex
	int param3;	//colIndex
	float fParam4;	//highlight scale
	float fParam5;	//highlight pos x
	float fParam6; //highlight pos y,base on CRUSH_BG_HEIGHT
}GuideDetailInfo_T;

typedef struct _GuideStepInfo
{
	int triggerTime;	//0.pre finish 1.waitCrush 2.monster att
	int finishTime;	//0.touch 1.crush 2.monster att end(new round)
	std::vector<const GuideDetailInfo_T *> arrDetailInfo;
}GuideStepInf_T;


class GuideLayer : public Layer
{
public:
	static GuideLayer *getInstance();

	GuideLayer();
	virtual ~GuideLayer();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void onTriggerEvent(int eventId);	//0.pre finish 1.waitCrush 2.monster att
	void onFinishEvent(int eventId);	//0.touch 1.crush 2.monster att end(new round)

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	CREATE_FUNC(GuideLayer);
private:
	static GuideLayer *s_pInstance;

	void startGuide(int index);
	void finishGuide(int index);
	void finishAllGuide();
	void removeAllChildrenByTag(int tag);

	std::vector<GuideStepInf_T> _arrStepInfo;

	int _curGuideIndex;
	bool _isGuiding;
	bool _isTouchAvail;
};



#endif