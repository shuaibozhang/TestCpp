#ifndef __SHOPLAYER_H__
#define  __SHOPLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Widgets/GameButton.h"
#include "BagItemControl.h"
#include "ParamData.h"

USING_NS_CC;
using namespace ui;
class GameArmtr;
class ShopLayer : public cocos2d::LayerColor
{
public:
	ShopLayer();
	~ShopLayer();

	static ShopLayer* createLayer(int shopidx);
	bool initLayer(int shopidx);
};


class ItemsShop : public cocos2d::LayerColor
{
public:
	ItemsShop();
	~ItemsShop();
	/* item 0, skill 1, wenpon 2*/
	static ItemsShop* createShop(int shoptype, int shopidx);
	bool initShop(int shoptype, int shopidx);
	int checkIsTouchItem(cocos2d::Vec2 touchPos);

	bool onTouchBegan(Touch *touch, Event *unusedEvent);
	void onTouchMoved(Touch *touch, Event *unusedEvent);
	void onTouchEnded(Touch *touch, Event *unusedEvent);
	void onTouchCancelled(Touch *touch, Event *unusedEvent);

	void showItem(int idx);
	void hideItemInfo();
	virtual void onEnter()override;

	cocos2d::Sprite* addItem(int id, Vec2 pos);

	void showBuyDialog(bool istouchbuy = true);
	void showDelDialog(bool istouchbuy = true);
	/*
	@param type: 0 is for buy, 1 is for sell
	*/
	void touchBuyCallBack(int type ,bool succeed);

	void updataSkillSp();

	void say(int idx);

	void updataShopBagIcons();

	void removeUnequipPlayerSkill();
private:
	//const static int BAGPAGES = 3;
	/*0~13 bagitems 14~17 shop buy items*/
	std::vector<cocos2d::Vec2> _itemsPos;
	std::vector<cocos2d::Vec2> _sellitemsPos;
	std::vector<cocos2d::Rect> _equipRectInWorld;
	cocos2d::Vec2 _itemsStartPos;
	cocos2d::Node* _rootLayer;
	cocos2d::Node* _itemsRoot;
	int _touchItemsIdx;
	int _touchEndIdx;
	cocos2d::ui::Text* _textTitle;
	cocos2d::ui::Text* _textDes;
	cocos2d::ui::Text* _textWenponRare;
	cocos2d::Sprite* _selectMark;
	cocos2d::Sprite* _selectItemsSprite;
	cocos2d::Sprite* _itemsSprite[g_pagecount][g_onepagetnum];
	cocos2d::Node* _itembgNode;
	cocos2d::Sprite* _itemsBgSprite[g_pagecount * g_onepagetnum];
	std::vector<int> _vectorSellItems;
	cocos2d::Sprite* _arrowSprite;
	Sprite* _iconSellOrBuy;
	TextAtlas* _textPriceSellOrBuy;
	TextAtlas* _textGold;
	TextAtlas* _textCrystall;
	Button* _btnOk;
	Sprite* _priceBg;
	//std::vector<ui::TextAtlas*> _vectorItemPrices;
	bool _isMove;
	cocos2d::Node* _buyDialog;

	int _shoptype;
	int _shopidx;
	bool _istouchbuy;
	bool isNeedRunMoveAndEnd;
	int _curpage;

	GameButton* _btnChangePageR;
	GameButton* _btnChangePageL;

	GameArmtr* _playerAni[ParamData::FIGHT_ROLE_COUNT];
	std::vector<int> _vectorSellISkillsSp;
	Node* _sellSpriteNode[4];
};


class TimeCountDownNode;
class StoreLayer2 : public Layer
{
public:
	CREATE_FUNC(StoreLayer2);
	bool init();

	void refreshTagBtnsShow();

	void menuOnPageChangeBtn(Ref* ref, Widget::TouchEventType type);

	void menuOnRefresh(Ref* ref, Widget::TouchEventType type);

	void menuOnPageOneBtns(Ref* ref, Widget::TouchEventType type);
	void menuOnPageTwoBtns(Ref* ref, Widget::TouchEventType type);
	void menuOnPageThreeBtns(Ref* ref, Widget::TouchEventType type);
	void menuOnPageFourBtns(Ref* ref, Widget::TouchEventType type);

	void menuOnPageOneShowInfoBtns(Ref* ref, Widget::TouchEventType type);

	void showBoxget(const std::vector<int>& itemids, const std::vector<int>& nums);

	void freshenItems(bool changids);

	void getReward(int boxid);

	void jumpToPage(int pageidx);
private:
	void showWenInfo(int wenponid, int btnidx);
	void showSkillInfo(int wenponid, int btnidx);
	void showItemInfo(int itemid, int btnidx);
	const int _refreshTime = 4 * 60 * 60;
	TimeCountDownNode* _timenode;
	PageView* _mainPageView;
	Node* _topNode;
	Node* _exchangeNode;
	Node* _pageRootNode[4];
	Button* _tagChangeBtns[4];
	Sprite* _tagTextSprites[4];
	int _curTagIdx;

	TextField* _exchangeTextField;
	Button* _exchangeBtn;

	//page 0
	struct  PageOneItem_T
	{
		Text* name;
		Sprite* icon;
		TextAtlas* getnum;
		Sprite* payicon;
		Text* pricetext;
		Sprite* particon;
		Node* soldout;
		Sprite* bgicon;
		GameButton* infoBtn;
	};

	PageOneItem_T _arrPageOneItems[9];
	int _storeItemCurIdx[9];
};

class GiftLayer : public Layer
{
public:
	CREATE_FUNC(GiftLayer);
	bool init();
	void menuOnBuy(Ref*, Widget::TouchEventType type);
	void updateBtns();
	int getShoulddShowGiftIdx();
private:
	Node* _root;
	Vector<Node*> _giftNodes;
};
#endif