#ifndef __TOUCHESSCALELAYER_H__
#define __TOUCHESSCALELAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace glui {
	class TouchesScaleNode : public cocos2d::Node
	{
	public:
		TouchesScaleNode();
		~TouchesScaleNode();
		static TouchesScaleNode* create();
		virtual bool init()override;

		virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
		virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
        virtual void onTouchesEnd(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

		inline void setCanTouchMove(bool isenable) { _canTouchAndMove = isenable; }
		bool getCanTouchMove()const { return _canTouchAndMove; }
		//virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)override;
		//virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)overridemin
		inline void setMinScale(float min) { _minScale = min; }
		inline void setMaxScale(float max) { _maxScale = max; }
		virtual void setVisible(bool visible)override;
		void checkMove();

		void setScreenRect(const cocos2d::Rect& rect) { _screenRect = rect; }
	protected:
		float _minScale;
		float _maxScale;
		bool _canTouchAndMove;
		//cocos2d::Sprite* _sprite;
		float _touchesDistance;
		float _touchStartScaleX;
		float _touchStartScaleY;
		cocos2d::Vec2 _touchStartLayerPos;
		cocos2d::Vec2 _touchesMidLayerPos;
		//cocos2d::Sprite* _mark;
		//cocos2d::ui::Text* _info;
		cocos2d::Touch* _touches[2];
		cocos2d::Rect _screenRect;
	};
}
#endif
