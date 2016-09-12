#ifndef __GLSPRITE_H__
#define __GLSPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

namespace glui {
	class GLSprite : public Sprite
	{
	public:
		static GLSprite* create(const std::string& filename, bool plist);
		static GLSprite* create();
		static GLSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
		bool init(const std::string& filename, bool plist);
		bool checkVisibility(const Mat4 &transform, const Size &size);
		virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)override;
	};
}
#endif
