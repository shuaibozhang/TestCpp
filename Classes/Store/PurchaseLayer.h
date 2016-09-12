#ifndef __PURCHASE_LAYER_H__
#define __PURCHASE_LAYER_H__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace ui;

#define DECLARE_PURCHASE_BUY(funName)\
public: static PurchaseLayer* create##funName##Purchase(string desid, Node* parent, bool buydirectly = false);


#define DECLARE_PURCHASE_BUY_IDX(funName)\
public: static PurchaseLayer* create##funName##Purchase(string desid, Node* parent, string idx, bool buydirectly = false);


#define  DEFINE_PURCHASE_BUY(funName, itemName)\
PurchaseLayer* PurchaseLayer::create##funName##Purchase(string desid, Node* parent, bool buydirectly)\
{\
	return PurchaseLayer::create(itemName, desid, parent, buydirectly);\
}\

#define  DEFINE_PURCHASE_BUY_IDX(funName, itemName)\
	PurchaseLayer* PurchaseLayer::create##funName##Purchase(string desid, Node* parent, string idx, bool buydirectly)\
{\
	return PurchaseLayer::create(itemName + idx, desid, parent, buydirectly);\
}\


class GameButton;
class PurchaseLayer : public LayerColor
{
public:
	PurchaseLayer();
	~PurchaseLayer();

	static PurchaseLayer* getCurPurchaseLayer(void);
	static PurchaseLayer* create(string itemId, string desId, Node* father, bool buydirectly = false);

	DECLARE_PURCHASE_BUY_IDX(Skill);
	DECLARE_PURCHASE_BUY_IDX(Weapon);


	void showPurchaseDialog();
	void buyDirectly();

	bool init(string itemId, string desId, Node* father, bool isneeddialog);

	void showDialog(const std::string& itemid);

	void menuBuyCallback(cocos2d::Ref* pSender);
	void menuCancelCallback(cocos2d::Ref* pSender);

	void buttonBuyCallback(cocos2d::Ref* pSender, Widget::TouchEventType type);
	void buttonCancelCallback(cocos2d::Ref* pSender, Widget::TouchEventType type);

	void onPurchaseOk(string itemid);
	void actionCB(void);

	void onItemBuyFailed(string itemid);

	CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, gooditemid, GoodItemId);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, gooditemDesId, GoodDesId);
private:
	Node* m_pFather;
	static PurchaseLayer *s_pPurchaseLayer;
	Sprite *mpWaitting;
	LayerColor* bgLayer;
	Node* _node;
	bool b_isBuyDirectly;
	GameButton* _btnCancle;
	GameButton* _btnBuy;
};


#endif
