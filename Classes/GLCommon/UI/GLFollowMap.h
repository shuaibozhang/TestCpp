#ifndef __GLFOLLOWMAP_H__
#define __GLFOLLOWMAP_H__

#include "cocos2d.h"

USING_NS_CC;
namespace glui {
	class GLFollowLayer : public Layer
	{
	public:
		GLFollowLayer();
		~GLFollowLayer();

		static GLFollowLayer* createLayer();
		bool init();

		inline void setFollowConfig(float right) { _rightDistance = right; }

		inline void setFollowNode(Node* node) { _followNode = node; }
		inline Node* getFollowNode()const { return _followNode; }

		virtual void update(float delta)override;
	private:
		Node* _followNode;
		float _rightDistance;
	};
}
#endif
