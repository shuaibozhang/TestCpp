#include "ScrollBg.h"

namespace glui {
	ScrollBg::ScrollBg() :_isHorizontal(true),
		_speed(0.f),
		_bgNode(nullptr),
		_bgNode2(nullptr),
		_bgSize(Size(0.f,0.f))
	{
	}

	ScrollBg * ScrollBg::createWithSprite(Node * bgsprite, Node* bgsprite2,float v, bool horizontal)
	{
		auto pRef = new ScrollBg();
		pRef->initWithSprite(bgsprite, bgsprite2,v, horizontal);
		return pRef;
	}

	bool ScrollBg::initWithSprite(Node * bgsprite, Node * bgsprite2,float v, bool horizontal)
	{
		Layer::init();

		_bgNode = bgsprite;
		_bgNode2 = bgsprite2;

		_bgSize = bgsprite->getContentSize() + bgsprite2->getContentSize();
		_speed = v;
		_isHorizontal = horizontal;

		_bgNode->setAnchorPoint(Vec2(0.f, 0.f));
		_bgNode2->setAnchorPoint(Vec2(0.f, 0.f));

		addChild(_bgNode);
		addChild(_bgNode2);

		if (_isHorizontal)
		{
			_bgNode2->setPositionX(_bgNode->getContentSize().width);
		}
		
		scheduleUpdate();
		return true;
	}

	void ScrollBg::update(float delta)
	{
		if (_isHorizontal)
		{
			float pass = _speed * delta;

			{
				float curpos = _bgNode->getPositionX();				
				float nextpos = curpos - pass;
				if (nextpos <= -_bgNode->getContentSize().width)
				{
					int idx = pass / _bgSize.width;
					nextpos += idx * _bgSize.width + _bgSize.width;
				}
				_bgNode->setPositionX(nextpos);
			}

			{
				float curpos = _bgNode2->getPositionX();		
				float nextpos = curpos - pass;
				if (nextpos <= -_bgNode2->getContentSize().width)
				{
					int idx = pass / _bgSize.width;
					nextpos += idx * _bgSize.width + _bgSize.width;
				}
				_bgNode2->setPositionX(nextpos);
			}
		}
	}

}
