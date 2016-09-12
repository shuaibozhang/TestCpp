#include "TouchesScaleNode.h"
#include "VisibleRect.h"

USING_NS_CC;

namespace glui {
	TouchesScaleNode::TouchesScaleNode() :_maxScale(10.f),
		_minScale(0.1f)
	{
		memset(_touches, 0, sizeof(_touches));
	}
	TouchesScaleNode::~TouchesScaleNode()
	{
		memset(_touches, 0, sizeof(_touches));
	}
	TouchesScaleNode * TouchesScaleNode::create()
	{
		auto pRef = new TouchesScaleNode();
		if (pRef && pRef->init())
		{
			pRef->autorelease();
		}
		else
		{
			delete pRef;
			pRef = nullptr;
		}
		return pRef;
	}

	bool TouchesScaleNode::init()
	{
		Node::init();

		_canTouchAndMove = true;

		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(TouchesScaleNode::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(TouchesScaleNode::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(TouchesScaleNode::onTouchesEnd, this);
        listener->onTouchesCancelled = CC_CALLBACK_2(TouchesScaleNode::onTouchesEnd, this);
        
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_touchesDistance = 0.0f;
		
		_screenRect = Rect(0.f, 0.f, VisibleRect::right().x, VisibleRect::top().y);

		return true;
	}

	void TouchesScaleNode::onTouchesBegan(const std::vector<Touch*>& touches, Event * unused_event)
	{
		if (!_canTouchAndMove)
		{
			return;
		}

		_touchStartLayerPos = this->getPosition();
        for(auto& curtouche : touches)
        {
            if (curtouche->getID() < 2) {
                _touches[curtouche->getID()] = curtouche;
            }
        }
        
		auto p1 = _touches[0];
        auto p2 = _touches[1];
        if (p1 == nullptr || p2 == nullptr)
        {
            return;
        }

        _touchesDistance = (p1->getLocation()).getDistance(p2->getLocation());
        _touchStartScaleX = this->getScaleX();
        _touchStartScaleY = this->getScaleY();
			
        auto posO = (p1->getLocation() + p2->getLocation()) / 2.0f;
        _touchesMidLayerPos = posO - this->convertToWorldSpaceAR(Vec2(0.f, 0.f));
		
	}

	void TouchesScaleNode::onTouchesMoved(const std::vector<Touch*>& touches, Event * unused_event)
	{
		if (!_canTouchAndMove || !isVisible())
		{
			return;
		}

		bool canContitueScale = true;

		if (_touches[0] != nullptr && _touches[1] != nullptr)
		{
			//_touches[touches.at(0)->getID()] = touches.at(0);
			//_touches[touches.at(1)->getID()] = touches.at(1);

			auto p1 = _touches[0];
			auto p2 = _touches[1];
			

			float curDistance = (p1->getLocation()).getDistance(p2->getLocation());
	
			float scale = curDistance / _touchesDistance;
			float trueScaleX = _touchStartScaleX * scale;
			float trueScaleY = _touchStartScaleY*scale;

			if (trueScaleX > _maxScale)
			{
				trueScaleX = _maxScale;
				canContitueScale = false;
			}
			else if (trueScaleX < _minScale)
			{
				trueScaleX = _minScale;
				canContitueScale = false;
			}

			if (trueScaleY > _maxScale)
			{
				trueScaleY = _maxScale;
				canContitueScale = false;
			}
			else if (trueScaleY < _minScale)
			{
				trueScaleY = _minScale;
				canContitueScale = false;
			}

			this->setScale(trueScaleX, trueScaleY);

			if (canContitueScale)
			{
				auto off = _touchesMidLayerPos * (scale - 1.0f);
				this->setPosition(_touchStartLayerPos - off);
			}
		}
		else if(_touches[0] != nullptr && _touches[1] == nullptr)
		{
            auto p1 = _touches[0];
            auto last = p1->getPreviousLocation();
			auto off = p1->getLocation() - last;
			this->setPosition(this->getPosition() + off);
		}

		checkMove();
	}
    
    void TouchesScaleNode::onTouchesEnd(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
    {
        
        for(auto& curtouche : touches)
        {
            if (curtouche->getID() < 2) {
                _touches[curtouche->getID()] = nullptr;
            }
        }
    }

	void TouchesScaleNode::setVisible(bool visible)
	{
		Node::setVisible(visible);
		setCanTouchMove(visible);
	}

	void TouchesScaleNode::checkMove()
	{
		auto scalex = getScaleX();
        auto scaley = getScaleY();
		auto top = this->getContentSize().height / 2.f;
		auto right = this->getContentSize().width / 2.f;

		
		auto p0 = this->convertToWorldSpaceAR(Vec2(-right, -top));
		Rect rec2(p0, Size(right * 2.f * scalex, top * 2.f * scaley));
		float offy = 0.f;
		float offx = 0.f;
		if (rec2.getMaxY() < _screenRect.getMaxY())
		{
			offy = _screenRect.getMaxY() - rec2.getMaxY();
		}
		else if (rec2.getMinY() > _screenRect.getMinY())
		{
			offy = _screenRect.getMinY() - rec2.getMinY();
		}

		if (rec2.getMaxX() < _screenRect.getMaxX())
		{
			offx = _screenRect.getMaxX() - rec2.getMaxX();
		}
		else if (rec2.getMinX() > _screenRect.getMinX())
		{
			offx = _screenRect.getMinX() - rec2.getMinX();
		}

		this->setPosition(getPosition() + Vec2(offx, offy));
	}
}



