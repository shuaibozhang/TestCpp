#include "GameButtonReader.h"
#include "GameButton.h"


static GameButtonReader* instanceButtonReader = nullptr;

IMPLEMENT_CLASS_NODE_READER_INFO(GameButtonReader)

GameButtonReader* GameButtonReader::getInstance()
{
	if (!instanceButtonReader)
	{
		instanceButtonReader = new (std::nothrow) GameButtonReader();
	}
	return instanceButtonReader;
}

void GameButtonReader::purge()
{
	CC_SAFE_DELETE(instanceButtonReader);
}

void GameButtonReader::destroyInstance()
{
	CC_SAFE_DELETE(instanceButtonReader);
}


Node* GameButtonReader::createNodeWithFlatBuffers(const flatbuffers::Table *buttonOptions)
{
	GameButton* button = GameButton::create();

	setPropsWithFlatBuffers(button, (flatbuffers::Table*)buttonOptions);

	return button;
}