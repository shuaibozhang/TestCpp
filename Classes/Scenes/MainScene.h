#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "GLCommon/UI/GLMenuItemSprite.h"
#include "GLCommon/UI/GLPageView.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameMap.h"
#include "DataParam/ParamData.h"
#include "../Widgets/GameButton.h"
#include "BagItemControl.h"
#include "NetDataMgr.h"

class GameArmtr;
enum MenuItem_E
{
	MainItem = 0,
	CharacterItem,
	WeaponsItem,
	SkillItem,
	BagItem,
	MapItem,
	ItemNums
};

enum MainLayer_Z
{
	WORLDMAP_Z = 0,
	GUANQIAMAP_Z,
	MAPUI_Z,
	MAPUI_POP,
	BG_Z,
	PLAYERINFOLAYER_Z,
	BAGLAYER_Z,
	TOUCH_Z,
	MENU_Z,
	POP_Z,
	POP_REWARD_Z,
};

class PlayersLayer;
class GameMap;
class BagLayer;
class EquipLayer;
class SkillLayer;
class TimeCountDownNode;
class MainLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* scene();
	static MainLayer* create();

	MainLayer();
	~MainLayer();

	virtual bool init()override;
	virtual void onEnter() override;
	
	void initMenus();
	void initPlayersLayer();
	void initSkillAndEpuipLayer();
	void initTouchLsitener();
	void initMapLayer();
	void menuCallback(cocos2d::Ref* pSender);
	
	void initTimeCounrdown();

	void initActivityInfo(int idx);

	void resetMarkPoint();

	std::function<void()> _showNewGetCallBack;
	void showNewGetItem();

	void meunEffectMainOut(float dur);
	void menuEffectSecondOut();

	void setStartState();

	bool onTouchBegan(Touch *touch, Event *unusedEvent);
	void onTouchMoved(Touch *touch, Event *unusedEvent);
	void onTouchEnded(Touch *touch, Event *unusedEvent);
	void onTouchCancelled(Touch *touch, Event *unusedEvent);

	void startMenuProtect(float time);

	void enterStory();
	void leaveStory(bool win = true);

	void enterFight();
	void leaveFight();

	void enterMainLayer();

	void moveDownMenu(float time);
	void moveUpMenu(float time);

	void updataTopInfo();
	void updataGold();
	void updataPlayersInfo(int idx);

	SkillLayer* getCurDisPlaySkillLayer();
	EquipLayer* getCurDisplayEquipLayer();
	PlayersLayer* getCurPlayersLayer() { return _playersLayer; }

	inline WordMap* getWordMap() { return _worldLayer; }
	
	static MainLayer* getCurMainLayer();

	void pauseGame();
	void resumGame();

	cocos2d::ui::Button* getPauseBtn() { return _btnPause; }

	TimeCountDownNode* getTimeNode(int idx) {return _countdownNode[idx];}

	TimeCountDownNode* getBoxTimeNode() { return _boxtimenode; };
	void removeBoxTimeNode();
	void addBoxTimeNode(int time);

	void clickAction(int click);
	void offlineClickAction(float dt);

//	void onGetTopDataCallback(HttpClient* client, HttpResponse* response);
//	void onGetSelfInfoCallback(HttpClient* client, HttpResponse* response);
	void onUpdateSelfInfoCallback(HttpClient* client, HttpResponse* response);
//	void onGetRealTopCallback(HttpClient* client, HttpResponse* response);
	void onGetSettleTopCallback(HttpClient* client, HttpResponse* response);
//	void onPostTmpSortCallback(HttpClient* client, HttpResponse* response);
	void onCheckCodeCallback(HttpClient* client, HttpResponse* response);

	void updataEquipLayer(int playeridx);

	bool checkGuide();
	void startGuideStep();
	void endGuideStep();
	void guideEvent(EventCustom *evt);

	void getCurOnlineTime();
	bool checkCanFreeGetBox();

	CC_SYNTHESIZE(int, _settleRankState, SettleRankState);//0:no data Or aleady get the reward;1:getting settle rank data;2:need show settle rank; 3.need get settle
	CC_SYNTHESIZE(int, _curTime, CurTime);
	CC_SYNTHESIZE(bool, _getOnlineTime, IsGetOnlineTime);
	Node** getLightArr() { return _lightArr; }
	Node* getLeftMenus() { return _leftBtnsNode; }

	CC_SYNTHESIZE(GameButton*, _daylimitGift, DayLimitGift);
	CC_SYNTHESIZE(GameButton*, _7xiActivityBtn, 7xiActivityBtn);

	void firstGiftBuyCallBack();
	/*-1 : no gift, 0 day gift, 1 time gift0, 2 time gift1, 3 timegift2*/
	int checkBuyGiftType();

	int checkTimeLimitGift();

	void updataGiftBtns();
	void updataDayActivityBtn();
	void touchAddBtn(Ref* btn, Widget::TouchEventType type);

	void updataDayAndTimeGiftBtn();

	/*
	idx = 0 : pop timelimitgift;
	idx = 1: pop store hot;
	idx = 2 : pop daylimitgift;
	idx = 3 : pop store box;
	*/
	void popBuyItemOrLayer(int idx);
	Node* getTopGoldNode() { return _topGoldNode; }

	Node* getTiliMaxIcon() { return _pTiliMaxIcon; }

	void popBossGuide();

	void initCommondActivity(int curday);
private:
	EventListenerCustom* _backToForegroundlistener;

	static MainLayer* s_curMainLayer;
	int _curMenuIdx;
	cocos2d::ui::ImageView* _playerbg;
	cocos2d::Sprite* _logo;
	cocos2d::Menu* _mainMenu;
	cocos2d::Menu* _secondMenu;
	bool _isShowMainMenu;
	glui::GLMenuItemSprite* _arrMenuItems[ItemNums];
	cocos2d::ui::Button* _btnPause;
	//cocos2d::Sprite* _arrSpriteMenuText[ItemNums];
	PlayersLayer* _playersLayer;
	cocos2d::Layer* _itemsLayer;
	glui::GLPageView* _equipPageView;
	glui::GLPageView* _skillPageView;
	glui::GLPageView* _curShowPageView;
	cocos2d::Vec2 _pageViewFinalPos;
	cocos2d::Vec2 _pageViewFinalPos2;
	cocos2d::Vec2 _pageViewFinalPos3;
	cocos2d::Size _pageSize;
	cocos2d::Node* _touchListenerMarkNode;
	cocos2d::EventListenerTouchOneByOne* _touchOneByOneListener;
	GameMap* _mapLayer;
	WordMap* _worldLayer;
	BagLayer* _bagLayer;
	float _menuProtectTime;
	bool _isMenuInProtect;

	cocos2d::Node* _topGoldNode;
	cocos2d::ui::TextAtlas* _goldNumAtlas;
	cocos2d::ui::TextAtlas* _crystalNumAtlas;
	cocos2d::ui::TextAtlas* _tiliNumAtlas;
	cocos2d::Node* _topHpInfoNode;
	cocos2d::Node* _markNode;
	cocos2d::ui::TextAtlas* _topHpAtlas;
	cocos2d::ui::TextAtlas* _topDpAtlas;

	cocos2d::Node* _numInfoNode;

	SkillLayer* _arrSkillLayer[ParamData::ROLE_COUNT];
	EquipLayer* _arrEquipLayer[ParamData::ROLE_COUNT];

	cocos2d::Sprite* _menubg;
	cocos2d::Sprite* _selectLight;
	bool _menuUp;

	Node* _leftBtnsNode;
	Node* _rightBtnsNode;
	GameButton* _boxStoreBtn;
	GameButton* _wujingBtn;
	GameButton* _chengjiuBtn;
	GameButton* _rewardBtn;
	GameButton* _bglBtn;
	GameButton* _giftBtn;
	GameButton* _dayAcitvityBtn;
	ui::TextAtlas* _countdownNum[1];
	TimeCountDownNode* _countdownNode[2];
	cocos2d::Node* _boxCountBg;
	cocos2d::ui::Text* _boxCountText;
	TimeCountDownNode* _boxtimenode;
	cocos2d::Sprite* _achCountBg;
	cocos2d::ui::Text* _achCountText;
	bool _isNeedDaily;
	bool _isNeedWeekReward;
	Armature* _rewardBtnani;
	Node* _lightArr[4];

	GameButton* _timeLimitActivityBtn;
	GameButton* _commondActivityBtn;
	ui::TextAtlas* _timeLimitActivityText;

	std::vector<Node*> _arrrRightBnts;
	
	Node* _pTiliMaxIcon;
};


class EquipLayer : public cocos2d::Layer
{
public:
	~EquipLayer();
	static EquipLayer* createLayer(int idx);
	bool initWithIdx(int idx);

	virtual void onEnter()override;

	bool checkHaveWenpon(int skillid);
	bool checkHaveEquipWenpon(int skillid);

	bool checkCanHeCheng(int posidx);

	int checkTouchBtn(cocos2d::Vec2 pos);
	void touchBegan(Vec2 pos);

	void updataWenponDes(int posidx);

	void equipCallBack(int playeridx, int skillid, int action);	//action:0.delete 1.euqip 2.unlock

	void updateWenponInfo(int posidx);
	void updateWUPBtn();

	void menuOnWUP(Ref* ref, Widget::TouchEventType type);
	void showUpdataWeapon();

	void freshenUpadtaLayer();
	void guideEvent(EventCustom *evt);
private:
	cocos2d::Node* _attackNode[3];
	int _idx;
	ui::Text* _textDes;
	ui::Text* _textTitle;
	ui::Text* _textWenponRare;
	ui::Text* _textWenponLv;
	Node* _layer;
	Vec2 _startPos0;
	Vec2 _startPos1;
	Vec2 _startLayerPos0;
	Vec2 _startLayerPos1;
	int _selectIdx;
	Node* _btnNode;
	ui::Button* _btnok;
	Sprite* _selectMark;
	Vec2 _arrPos[10];
	int _arrWeaponid[10];
	Sprite* _arrWeaponIcon[10];
	ui::LoadingBar* _arrparsBar[10];
	ui::Text* _textparts[10];
	Sprite* _equipIcon;
	ui::Button* _btnWUP;
	Sprite* _jiantou;
	Sprite* _tipText;
	Node* _wupGoldNode;
	LayerColor* _layercolor;
	Node* _arrLvNode[10];
	Node* _infoNode[10];
	Node* _arrPartsIcon[10];
};


class SkillLayer : public cocos2d::Layer
{
public:
	~SkillLayer();
	static SkillLayer* createLayer(int idx);
	bool initWithIdx(int idx);
	virtual void onEnter()override;
	int checkTouchBtn(cocos2d::Vec2 pos);
	void touchBegan(Vec2 pos);

	bool checkHaveSkill(int skillid);
	bool checkHaveEquipSkill(int skillid);

	void skillCallBack(int playeridx, int skillid, int action);	//action:0.unequip 1.equip 2.got
private:
	ui::Text* _textDes;
	ui::Text* _textTitle;
	Node* _layer;
	Vec2 _startPos0;
	Vec2 _startPos1;
	int _selectIdx;
	int _idx;
	Node* _btnNode;
	ui::Button* _btnok;
	Sprite* _selectMark;
	Vec2 _arrPos[12];
	//int _arrSkillid[12];
	Sprite* _arrSkillIcon[12];
};


class BagLayer : public cocos2d::Layer
{
public:
	struct ItemWithSprite_T
	{
		int itemid;
		int id;
		cocos2d::Sprite* sprite;
		int posidx;
	};

	struct mycomp
	{
		bool operator() (const ItemWithSprite_T &a, const ItemWithSprite_T &b);
	};

	BagLayer();
	~BagLayer();

	static BagLayer* createLayer();
	bool init();

	virtual void onEnter()override;

	bool onTouchBegan(Touch *touch, Event *unusedEvent);
	void onTouchMoved(Touch *touch, Event *unusedEvent);
	void onTouchEnded(Touch *touch, Event *unusedEvent);
	void onTouchCancelled(Touch *touch, Event *unusedEvent);

	void setCanTouch(bool cantouch);

	int checkIsTouchItem(cocos2d::Vec2 touchPos);

	void ItemChanges(int itemid, int numchange, int bagpos, int page);
	void PosChanges(int start, int end, int page, int type);
	void reSortItems();

	void showDelDialog(int posidx);
private:
	EventListenerTouchOneByOne* _touchOneByOneListener;
	cocos2d::Vec2 _itemsStartPos;
	cocos2d::Node* _rootLayer;
	cocos2d::ui::Text* _textTitle;
	cocos2d::ui::Text* _textDes;
	//cocos2d::Node* _itemsSprite[18];
	int _flagBag[g_pagecount][14];
	int _flagEquip[4];
	cocos2d::Vec2 _itemsPos[18];
	cocos2d::Rect _equipRectInWorld[4];
	ItemWithSprite_T _vectorPlayerItems[g_pagecount][14];
	ItemWithSprite_T _vectorEquipPlayerItems[4];
	int _touchItemsIdx;
	bool _isMove;
	cocos2d::Sprite* _selectMark;
	cocos2d::Sprite* _arrArrows[4];
	cocostudio::timeline::ActionTimeline* _actionArrow;
	cocos2d::ui::Button* _delBtn;
	cocos2d::Node* _pageNode[g_pagecount];
	int _curpageidx;
	cocos2d::ui::Button* _btnChangePageR;
	cocos2d::ui::Button* _btnChangePageL;
	
	int _itemStartId;
};


class PlayersLayer : public cocos2d::ui::Layout
{
public:
	PlayersLayer();
	~PlayersLayer();

	static PlayersLayer* createPlayerLayer();
	bool init();
	void updataInfo();
	void updataPlayerInfo(int idx);
	void showWenponInfo(int playerid, int wenponid);

	void equipCallBack(int playeridx, int skillid, int action);

	void resetPos();
	void showChangeToOtherLayer(float dur);
	void showChangeBackTo(float dur, int idx);
	void setScoreShow(bool show);
	 glui::GLPageView* getPlayersPages () const { return _scrollPlayersPageView; }
	 GameArmtr* getPlayerArm(int idx) { return _playerArm[idx]; }
	 GameArmtr* getPlayerArm2(int idx) { return _playerArm2[idx]; }
private:
	glui::GLPageView* _scrollPlayersPageView;
	cocos2d::Vector<Node*> _arrPlayers;
	cocos2d::Vec2 _arrStartPos[ParamData::ROLE_COUNT];
	cocos2d::Vec2 _arrStartPosInScreen[ParamData::ROLE_COUNT];
	cocos2d::Vec2 _wenPonNodePos;
	cocos2d::Vec2 _wenPonNodeStartPos;
	std::vector<cocos2d::Node*> _vectorPlayerNode;
	GameArmtr* _playerArm[ParamData::ROLE_COUNT];
	GameArmtr* _playerArm2[ParamData::ROLE_COUNT];
	ui::ScrollView* _playerScoreView;
};

class PageSelecter : public cocos2d::Node
{
public:
	typedef std::function <void(int, int)> ccPageChangeCallBack;

	static PageSelecter* create(int maxidx, std::string pic, std::string pic2, std::string pic3, ui::TextAtlas* font,ccPageChangeCallBack pagecall);
	bool init(int maxidx, std::string pic, std::string pic2, std::string pic3, ui::TextAtlas* font, ccPageChangeCallBack pagecall);
	void setProtectTime(float time) { _protectTime = time; }
private:
	ccPageChangeCallBack _callback;
	GameButton* _btnL;
	GameButton* _btnR;
	int _curidx;
	int _maxidx;
	ui::TextAtlas* _textPage;
	float _protectTime;
	bool _inProtect;
};
#endif
