#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "GameCSLoader.h"
#include "ParamData.h"
#include "../Story/Commond.h"
//#include "cocostudio/CocoStudio.h"

USING_NS_CC;
//using namespace cocostudio;

class CrushLayer;
class FightLayer;
class Monster;
class DungeonMissionNode;

class GameLayer : public cocos2d::Layer
{
public:
	static GameLayer *getInstance();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	GameLayer();
	virtual ~GameLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onEnter();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameLayer);

	virtual void update(float delta);

	void startFight();
	void stopFight();
	void endStory(int index);
	void backToWorldMap();

	void pauseGame();
	void resumeGame();

	void initCombo();
	void addCombo();
	void resetCombo();

	//story
	inline cocos2d::ui::ListView* getStoryListView() { return _storyListView; }
	void initStoryInfo();
	void showStory(int idx);
	void storyend();
	void showSelectMenu(Node* menu);
	void removeSelectMenu();
	void contitueStoryAfterSelect(int next);
	void showReward(std::vector<Reward_T> arr);

	void initFightInfo();
	void setFightName(const std::string& name);
	void setCurFightIndex(int cur, int all, float time = 1.f);
	inline Node* getFightInfoNode() { return _fightInfoNode; }

	void showExpAdd(int exp0, int exp1, int exp2, int exp3);
	void removeUpdateNode();

	void setHp(float hp, float percent);
	void setBossHp(float hp, float percent);

	void setDef(float def, float percent);
	void playBarIconAnim(int animType);	//0.hp 1.def
	CC_SYNTHESIZE_READONLY(int, _sceneId, SceneId);
	CC_SYNTHESIZE(bool, _isGuiding, IsGuiding);
	CC_SYNTHESIZE(int, _fightType, FightType);	//0.nor 1.endless 2.elite 3.instance
	CC_SYNTHESIZE(bool, _isNeedGrow, IsNeedGrow);
	CC_SYNTHESIZE(int, _expTimes, ExpTimes);
	CC_SYNTHESIZE(bool, _isHaveBoxMonster, IsHaveBoxMonster);

	void updateEquipItems();
	void setEquipBtnEnabled(bool enable);

	void itemsCallback(cocos2d::Ref* pSender, Widget::TouchEventType type);
	void switchInfoDisPlay(bool show);

	void playItemEffect(const std::string &animName, const Vec2 &pos);

	void loadImageCallback(Texture2D *pTexture, LoadResInfo_T resInfo, int param1, int parma2);	//param1:0.skill   param2:skillid

	void playSkillBgAnim(int animType);	//0.show 1.hide
	int getCurStoryCheckType() { return _curStorytype; }
	CC_SYNTHESIZE_READONLY(Armature *, _pDefBarArmtr, DefBarArmtr);

	void showEnterAni();

	void startBarCountdown();
	void barCountDownCB();
	void endBarCountdown();

	void popReborn();

	void intDungeonInfo();
	void showDungeonInfo(const std::vector<int>& vectorDungeonIdx);
	void removerDungeonInfo();
	void setDungeonStep(int idx);
	DungeonMissionNode* getDungeonMissionNode(int missionidx);
	Node* getDungeonInfoNode() { return _pDungeonRootNode; }
	void setDungeonMissionNum(int id, int cur, int max);
private:
	void initUpdateNode();
	void showUpdateAni(int idx, float exp);
	void showAttributeAdd(int idx, int curlv, int tarlv);
	void showLvUpLight(int idx);

	static GameLayer *s_pInstance;

	CrushLayer *_pCrushLayer;
	FightLayer *_pFightLayer;
	LoadingBar *_pPlayerHpBar;
	LoadingBar *_pPlayerDefBar;
	TextAtlas *_pHpLbl;
	TextAtlas *_pDefLbl;
	Sprite *_pBarIconHp;
	Sprite *_pBarIconDef;

	Button* _arrEquipItems[4];
	Sprite* _spriteEquipBg[4];
	Node* _bottomUiNode;

	Node* _bottomUiNodeR;
	//Node* _bottomUiNodeL;

	Node* _comboNode;
	int _comboNum;

	Node* _aniNode[4];
	bool _isNeedUpdateExp;

	bool _isExpNodeShow;

	Node* _fightInfoNode;

	int _lastHp;
	int _lastDp;

	LoadingBar* _bossHpBar;
	Sprite* _bossHpBarBg;

	bool _isStartFight;

	//storyshow
	cocos2d::ui::ListView* _storyListView;
	cocos2d::EventListenerTouchOneByOne* _touchStoryListener;
	Node* _storyTouchProxyNode;
	Node* _selectmenu;
	/*0 : start, 2:end, 1:boss*/
	int _curStorytype;
	Label *_pFloorTitle;
	LayerColor *_pSkillBg;
	int _skillAnimCount;
	//bar
	LoadingBar * _timeCountdownBar;
	Sprite* _timeCountdownBarBG;

	//dungeon info
	Node* _pDungeonRootNode;
	TextAtlas* _pStepTextAtlas;
	std::vector<DungeonMissionNode*> _vectorDungeonMission;
};

#endif // __HELLOWORLD_SCENE_H__
