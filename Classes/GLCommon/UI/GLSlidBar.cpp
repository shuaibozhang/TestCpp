#include "GLSlidBar.h"

namespace glui
{
	GLSlidBar::GLSlidBar():_markNode(nullptr)
	{
	}

	GLSlidBar * GLSlidBar::create(const std::string & textureName, Node * marknode, TextureResType type)
	{
		auto pRef = new GLSlidBar();
		if (pRef && pRef->init(textureName, marknode, type))
		{
			pRef->autorelease();
			return pRef;
		}

		CC_SAFE_DELETE(pRef);
		return nullptr;
	}

	bool GLSlidBar::init(const std::string& textureName, Node* marknode, TextureResType type)
	{	
		LoadingBar::init();
		LoadingBar::loadTexture(textureName, type);
		LoadingBar::setPercent(0);
		_markNode = marknode;
		if (marknode)
		{
			_markNode->setPositionY(LoadingBar::getContentSize().height / 2.f);
			this->addChild(marknode, 1);
		}
		return true;
	}

	void GLSlidBar::setPercent(float percent)
	{
		LoadingBar::setPercent(percent);
		if (_markNode)
		{
			_markNode->setPositionX(LoadingBar::getContentSize().width * percent / 100.f);
		}
	}

}