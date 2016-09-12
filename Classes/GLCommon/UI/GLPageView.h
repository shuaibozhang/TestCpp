#ifndef __GLPAGEVIEW_H__
#define  __GLPAGEVIEW_H__

#include "ui/CocosGUI.h"

USING_NS_CC;

namespace glui {

	class GLPageView : public ui::PageView
	{
	public:
		~GLPageView();
		static GLPageView* create();
		bool init()override;
		
		virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
		//virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
		//virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
		//virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;

		inline void setCanScroll(bool can) { _bCanScroll = can; }

		//void addFollowPosNode(cocos2d::Node* node);
	private:
		bool _bCanScroll;
		cocos2d::Vector<cocos2d::Node*> _arrFollowPosNode;
	};
}
#endif
