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

#endif // !__MagPieLayer_H__



