#include "GameSpriteReader.h"
#include "../GLCommon/UI/GLSprite.h"


static GameSpriteReader* instanceButtonReader = nullptr;

IMPLEMENT_CLASS_NODE_READER_INFO(GameSpriteReader)

GameSpriteReader* GameSpriteReader::getInstance()
{
	if (!instanceButtonReader)
	{
		instanceButtonReader = new (std::nothrow) GameSpriteReader();
	}
	return instanceButtonReader;
}

void GameSpriteReader::purge()
{
	CC_SAFE_DELETE(instanceButtonReader);
}

void GameSpriteReader::destroyInstance()
{
	CC_SAFE_DELETE(instanceButtonReader);
}


Node* GameSpriteReader::createNodeWithFlatBuffers(const flatbuffers::Table *buttonOptions)
{
	glui::GLSprite* button = glui::GLSprite::create();

	setPropsWithFlatBuffers(button, (flatbuffers::Table*)buttonOptions);

	return button;
}