#include "GLMenuItemSprite.h"
#include "GameUtils.h"

namespace glui {
	GLMenuItemSprite * GLMenuItemSprite::create(Node* normalSprite, const ccMenuCallback& callback)
	{
		auto pRef = new GLMenuItemSprite();
		if (pRef && pRef->initWithNormalSprite(normalSprite, callback))
		{
			pRef->autorelease();
			return pRef;
		}
		else
		{
			delete pRef;
			pRef = nullptr;
			return nullptr;
		}
	}


	bool GLMenuItemSprite::initWithNormalSprite(Node * normalSprite,const ccMenuCallback & callback)
	{
		MenuItemSprite::initWithNormalSprite(normalSprite, nullptr, nullptr, callback);
		_activeProtectDur = 0.05f;
		_bgSprite = nullptr;
		//_activeAction = nullptr;
		_isProtect = false;
		_normalImage->setAnchorPoint(Vec2(0.5f, 0.5f));
		_normalImage->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		_pressZoomAction = nullptr;
		return true;
	}


	void GLMenuItemSprite::selected()
	{
		MenuItemSprite::selected();
		if (_normalImage)
		{
			_normalImage->stopAction(_pressZoomAction);
			Action *zoomTitleAction = ScaleTo::create(0.05f,
				1.0f + 0.05f, 1.0f + 0.05f);
			_normalImage->runAction(zoomTitleAction);

			_pressZoomAction = zoomTitleAction;
		}
	}

	void GLMenuItemSprite::unselected()
	{
		MenuItemSprite::unselected();
		if (_normalImage)
		{
			Action *zoomTitleAction = ScaleTo::create(0.05f, 1.0f, 1.0f);
			_normalImage->runAction(zoomTitleAction);
		}
	}


	void GLMenuItemSprite::activate()
	{
		if (_isProtect)
		{
			return;
		}
		GameUtils::playEffect("anniu.ogg", false);

		_isProtect = true;

		MenuItemSprite::activate();

		//_normalImage->stopAction(_activeAction);
		 auto activeAction = Sequence::createWithTwoActions(DelayTime::create(_activeProtectDur), CallFunc::create([=]() {
			 _isProtect = false;
		}));
		 _normalImage->runAction(activeAction);
		//_activeAction = activeAction;
	}

	void GLMenuItemSprite::setBgSprite(cocos2d::Node * sprite)
	{
		if (_bgSprite != sprite && sprite != nullptr)
		{
			if (_bgSprite !=nullptr)
			{
				this->removeChild(_bgSprite);
			}	

			_normalImage->setLocalZOrder(1);
			this->addChild(sprite);
			_bgSprite = sprite;
		}	
	}
}

