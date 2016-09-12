#ifndef __SCROLLBG_H__
#define __SCROLLBG_H__


#include "cocos2d.h"
USING_NS_CC;

namespace glui {

	class ScrollBg : public Layer
	{
	public:
		ScrollBg();

		static ScrollBg* createWithSprite(Node* bgsprite, Node* bgsprite2, float v, bool horizontal = true);
		bool initWithSprite(Node* bgsprite, Node* bgsprite2,float v, bool horizontal);
		virtual void update(float delta);
		inline void setSpeed(const float speed) { _speed = speed; }
	protected:
	private:
		bool _isHorizontal;
		float _speed;
		Node* _bgNode;
		Node* _bgNode2;
		Size _bgSize;
	};
}
#endif
