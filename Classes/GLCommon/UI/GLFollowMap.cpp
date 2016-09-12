#include "GLFollowMap.h"
namespace glui {
	GLFollowLayer::GLFollowLayer() :_followNode(nullptr),
		_rightDistance(0.f)
	{
	}

	GLFollowLayer::~GLFollowLayer()
	{
	}

	GLFollowLayer * GLFollowLayer::createLayer()
	{
		auto pRef = new GLFollowLayer();
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

	bool GLFollowLayer::init()
	{
		Layer::init();
		this->scheduleUpdate();
		return true;
	}

	void GLFollowLayer::update(float delta)
	{
		if (_followNode == nullptr)
		{
			return;
		}
		auto follownodepos = _followNode->getPosition();
		auto posInScreen = _followNode->convertToWorldSpace(Vec2(0.f, 0.f));
		CCLOG("GLFollowMap: posInScrenn: %f, %f", posInScreen.x, posInScreen.y);
		auto width = Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width;
		auto flagPoxX = width - _rightDistance;
		if (posInScreen.x > flagPoxX)
		{
			setPositionX(getPositionX() - (posInScreen.x - flagPoxX));
		}
	}
}
