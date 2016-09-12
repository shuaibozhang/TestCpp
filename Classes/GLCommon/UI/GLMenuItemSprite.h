#ifndef __GLMENUITEMSPRITE_H__
#define __GLMENUITEMSPRITE_H__

#include "cocos2d.h"
#include "2d/CCNode.h"
#include "base/CCProtocols.h"

USING_NS_CC;

namespace glui {

	class GLMenuItemSprite : public MenuItemSprite
	{
	public:
		static GLMenuItemSprite* create(Node* normalSprite, const ccMenuCallback& callback);
		bool initWithNormalSprite(Node* normalSprite, const ccMenuCallback& callback);

		/**
		* The item was selected (not activated), similar to "mouse-over".
		@since v0.99.5
		*/
		virtual void selected()override;

		/** The item was unselected. */
		virtual void unselected()override;

		virtual void activate()override;

		void setBgSprite(cocos2d::Node* sprite);
		inline cocos2d::Node* getBgSprite() const { return _bgSprite; }

		void setActiveProtectDur(float dur) { _activeProtectDur = dur; }
		float getActiveProtectDur()const { return _activeProtectDur; }

		Node* getRenderNorSprite() {return _normalImage;}
	private:
		bool _isProtect;
		Action* _pressZoomAction;
		//Action* _activeAction;
		cocos2d::Node* _bgSprite;
		float _activeProtectDur;
	};

}

#endif
