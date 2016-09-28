#ifndef __MagPieLayer_H__
#define __MagPieLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
class MagPieLayer : public LayerColor
{
public:
	MagPieLayer();

	~MagPieLayer();

	CREATE_FUNC(MagPieLayer);

	virtual bool init();

	void updateMagPieTask(int dayIdx);
private:
	Node *_pRoot;
	
};


class MidAutumnLayer : public LayerColor
{
public:
	MidAutumnLayer();

	~MidAutumnLayer();

	CREATE_FUNC(MidAutumnLayer);

	virtual bool init();

	void updateBtns();
private:
	Node *_pRoot;
	ui::Button* _arrBtns[4];

};


class GuoqingLayer : public LayerColor
{
public:
	GuoqingLayer();

	~GuoqingLayer();

	CREATE_FUNC(GuoqingLayer);

	virtual bool init();

	void updateBtns();
private:
	Node *_pRoot;
	ui::Button* _arrBtns[4];
	ui::Text* _arrOffNumText[4];
	ui::Text* _arrDesNumText[4];
	Sprite* _arrIcondes[4];
	Sprite* _arrIconnum[4];
};

#endif // !__MagPieLayer_H__



