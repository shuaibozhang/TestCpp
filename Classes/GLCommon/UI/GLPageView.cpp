#include "GLPageView.h"

namespace glui {
	GLPageView::~GLPageView()
	{
		_arrFollowPosNode.clear();
	}
	GLPageView * glui::GLPageView::create()
	{
		auto pRef = new GLPageView();
		if (pRef && pRef->init())
		{
			pRef->autorelease();
			return pRef;
		}
		else
		{
			delete pRef;
			return nullptr;
		}
	}

	bool glui::GLPageView::init()
	{
		ui::PageView::init();
		_bCanScroll = true;
		return true;
	}

	bool GLPageView::onTouchBegan(Touch * touch, Event * unusedEvent)
	{
		if (!_bCanScroll)
		{
			return false;
		}
		auto pass = PageView::onTouchBegan(touch, unusedEvent);
		return pass;
	}

	//void GLPageView::onTouchMoved(Touch * touch, Event * unusedEvent)
	//{

	//}

	//void GLPageView::onTouchEnded(Touch * touch, Event * unusedEvent)
	//{
	//}

	//void GLPageView::onTouchCancelled(Touch * touch, Event * unusedEvent)
	//{
	//}

	/*void GLPageView::addFollowPosNode(cocos2d::Node * node)
	{
		_arrFollowPosNode.pushBack(node);
	}*/
}
