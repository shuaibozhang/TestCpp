#ifndef __ANNC_LAYER_H__
#define __ANNC_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;


class AnncLayer : public cocos2d::LayerColor
{
public:
	static const string ANNC_KEY_SHOW;
	static const string ANNC_KEY_CONTENT;

	AnncLayer();
	~AnncLayer();

	CREATE_FUNC(AnncLayer);

	virtual bool init();
private:
	Node *_pRoot;
};


#endif // !__ANNC_LAYER_H__

