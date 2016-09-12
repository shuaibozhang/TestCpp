#ifndef __GLSILDBAR_H__
#define __GLSILDBAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

namespace glui
{
	class GLSlidBar : public LoadingBar
	{
	public:
		GLSlidBar();
		static GLSlidBar* create(const std::string& textureName, Node* marknode, TextureResType type);
		bool init(const std::string& textureName, Node* marknode, TextureResType type);

		inline Node* getPrecentMarkNode() { return _markNode; };

		void setPercent(float percent);
	private:
		Node* _markNode;
	};
}
#endif
