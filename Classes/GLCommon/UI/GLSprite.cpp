#include "GLSprite.h"

namespace glui {
	GLSprite * GLSprite::create(const std::string & filename, bool plist)
	{
		auto pRef = new GLSprite();
		if (pRef && pRef->init(filename, plist))
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

	GLSprite * GLSprite::create()
	{
		GLSprite *sprite = new (std::nothrow) GLSprite();
		if (sprite && sprite->Sprite::init())
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	GLSprite * GLSprite::createWithSpriteFrame(SpriteFrame * spriteFrame)
	{
		auto pRef = new GLSprite();
		if (pRef && spriteFrame && pRef->initWithSpriteFrame(spriteFrame))
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



	bool GLSprite::init(const std::string & filename, bool plist)
	{
		if (plist)
		{
			Sprite::initWithSpriteFrameName(filename);
		}
		else
		{
			Sprite::initWithFile(filename);
		}
		return true;
	}

	bool GLSprite::checkVisibility(const Mat4 &transform, const Size &size)
	{
		auto scene = Director::getInstance()->getRunningScene();

		//If draw to Rendertexture, return true directly.
		// only cull the default camera. The culling algorithm is valid for default camera.
		if (!scene || (scene && scene->getDefaultCamera() != Camera::getVisitingCamera()))
			return true;

		auto director = Director::getInstance();
		Rect visiableRect(director->getVisibleOrigin(), director->getVisibleSize());

		// transform center point to screen space
		float hSizeX = size.width / 2;
		float hSizeY = size.height / 2;
		Vec3 v3p(hSizeX, hSizeY, 0);
		transform.transformPoint(&v3p);
		Vec2 v2p = Camera::getVisitingCamera()->projectGL(v3p);

		Vec2 point[4];

		Vec3 lb3(0, 0, 0);
		transform.transformPoint(&lb3);
		Vec2 lb2 = Camera::getVisitingCamera()->projectGL(lb3);
		point[0] = lb2;

		Vec3 rb3(hSizeX * 2, 0, 0);
		transform.transformPoint(&rb3);
		Vec2 rb2 = Camera::getVisitingCamera()->projectGL(rb3);
		point[1] = rb2;

		Vec3 lt3(0, hSizeY * 2, 0);
		transform.transformPoint(&lt3);
		Vec2 lt2 = Camera::getVisitingCamera()->projectGL(lt3);
		point[2] = lt2;

		Vec3 rt3(hSizeX * 2, hSizeY * 2, 0);
		transform.transformPoint(&rt3);
		Vec2 rt2 = Camera::getVisitingCamera()->projectGL(rt3);
		point[3] = rt2;

		Vec2 minp = point[0];
		Vec2 maxp = point[0];
		for (auto& temp : point)
		{
			if (temp.x > maxp.x)
			{
				maxp.x = temp.x;
			}

			if (temp.y > maxp.y)
			{
				maxp.y = temp.y;
			}

			if (temp.x < minp.x)
			{
				minp.x = temp.x;
			}

			if (temp.y < minp.y)
			{
				minp.y = temp.y;
			}
		}

		/*Vec2 sizeAfterRotate;
		float tempbx = fabsf(lb2.x - rb2.x);
		float temptx = fabsf(lt2.x - rt2.x);
		float rsx = tempbx > temptx ? tempbx : temptx;

		float tempby = fabsf(lb2.y - lt2.y);
		float tempty = fabsf(rt2.y - rb2.y);
		float rsy = tempby > tempty ? tempby : tempty;

		sizeAfterRotate.x = rsx;
		sizeAfterRotate.y = rsy;

		float rotateScalex = fabsf(sizeAfterRotate.x * 0.5f / hSizeX);
		float rotateScaley = fabsf(sizeAfterRotate.y * 0.5f / hSizeY);
		hSizeX *= rotateScalex;
		hSizeY *= rotateScaley;*/

		/*auto eyesz = Camera::getVisitingCamera()->getPositionZ();
		auto scale = eyesz / (eyesz- v3p.z);
		hSizeX *= scale;
		hSizeY *= scale;*/

		// convert content size to world coordinates
		//float wshw = std::max(fabsf(hSizeX * transform.m[0] + hSizeY * transform.m[4]), fabsf(hSizeX * transform.m[0] - hSizeY * transform.m[4]));
		//float wshh = std::max(fabsf(hSizeX * transform.m[1] + hSizeY * transform.m[5]), fabsf(hSizeX * transform.m[1] - hSizeY * transform.m[5]));

		float wshw = (maxp.x - minp.x) * 0.5;
		float wshh = (maxp.y - minp.y) * 0.5;

		// enlarge visible rect half size in screen coord
		visiableRect.origin.x -= wshw;
		visiableRect.origin.y -= wshh;
		visiableRect.size.width += wshw * 2;
		visiableRect.size.height += wshh * 2;
		bool ret = visiableRect.containsPoint(v2p);
		if (!ret)
		{
			//CCLOG("out");
		}
		return ret;
	}

	void GLSprite::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
	{
#if CC_USE_CULLING
		// Don't do calculate the culling if the transform was not updated
		auto visitingCamera = Camera::getVisitingCamera();
		auto defaultCamera = Camera::getDefaultCamera();
		if (visitingCamera == defaultCamera) {
			_insideBounds = ((flags & FLAGS_TRANSFORM_DIRTY) || visitingCamera->isViewProjectionUpdated()) ? this->checkVisibility(transform, _contentSize) : _insideBounds;
		}
		else
		{
			_insideBounds = renderer->checkVisibility(transform, _contentSize);
		}

		if (_insideBounds)
#endif
		{
			_trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
			renderer->addCommand(&_trianglesCommand);

#if CC_SPRITE_DEBUG_DRAW
			_debugDrawNode->clear();
			auto count = _polyInfo.triangles.indexCount / 3;
			auto indices = _polyInfo.triangles.indices;
			auto verts = _polyInfo.triangles.verts;
			for (ssize_t i = 0; i < count; i++)
			{
				//draw 3 lines
				Vec3 from = verts[indices[i * 3]].vertices;
				Vec3 to = verts[indices[i * 3 + 1]].vertices;
				_debugDrawNode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::WHITE);

				from = verts[indices[i * 3 + 1]].vertices;
				to = verts[indices[i * 3 + 2]].vertices;
				_debugDrawNode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::WHITE);

				from = verts[indices[i * 3 + 2]].vertices;
				to = verts[indices[i * 3]].vertices;
				_debugDrawNode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::WHITE);
			}
#endif //CC_SPRITE_DEBUG_DRAW
		}
		else
		{
			//CCLOG("out of view");
		}
	}
}