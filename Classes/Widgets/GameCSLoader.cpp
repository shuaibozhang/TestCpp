#include "GameCSLoader.h"
#include "cocostudio/WidgetReader/NodeReaderDefine.h"
#include "GameButtonReader.h"
#include "../GLCommon/UI/GLSprite.h"

static GameCSLoader* _sharedCSLoader = nullptr;

GameCSLoader* GameCSLoader::getInstance()
{
	if (! _sharedCSLoader)
	{
		_sharedCSLoader = new GameCSLoader();
		_sharedCSLoader->init();
	}

	return _sharedCSLoader;
}


void GameCSLoader::destroyInstance()
{
	CC_SAFE_DELETE(_sharedCSLoader);
	timeline::ActionTimelineCache::destroyInstance();
}


GameCSLoader::GameCSLoader()
{
	CREATE_CLASS_NODE_READER_INFO(GameButtonReader);
}


Node* GameCSLoader::createNode(const std::string& filename)
{
	std::string path = filename;
	size_t pos = path.find_last_of('.');
	std::string suffix = path.substr(pos + 1, path.length());
	CCLOG("suffix = %s", suffix.c_str());

	GameCSLoader* load = GameCSLoader::getInstance();

	if (suffix == "csb")
	{
		return load->createNodeWithFlatBuffersFile(filename);
	}
	else if (suffix == "json" || suffix == "ExportJson")
	{
		return load->createNodeFromJson(filename);
	}

	return nullptr;
}

Node* GameCSLoader::createNode(const std::string &filename, const ccNodeLoadCallback &callback)
{
	std::string path = filename;
	size_t pos = path.find_last_of('.');
	std::string suffix = path.substr(pos + 1, path.length());
	CCLOG("suffix = %s", suffix.c_str());

	GameCSLoader* load = GameCSLoader::getInstance();

	if (suffix == "csb")
	{
		return load->createNodeWithFlatBuffersFile(filename, callback);
	}

	return nullptr;
}

ActionTimeline* GameCSLoader::createTimeline(const std::string &filename)
{
	std::string path = filename;
	size_t pos = path.find_last_of('.');
	std::string suffix = path.substr(pos + 1, path.length());
	CCLOG("suffix = %s", suffix.c_str());

	ActionTimelineCache* cache = ActionTimelineCache::getInstance();

	if (suffix == "csb")
	{
		return cache->createActionWithFlatBuffersFile(filename);
	}
	else if (suffix == "json" || suffix == "ExportJson")
	{
		return cache->createActionFromJson(filename);
	}

	return nullptr;
}


std::string GameCSLoader::getGUIClassName(const std::string &name)
{
	std::string convertedClassName = CSLoader::getGUIClassName(name);

	if (name == "Button")
	{
		convertedClassName = "GameButton";
	}
// 	else if (name == "Sprite")
// 	{
// 		convertedClassName = "GameSprite";
// 	}

	return convertedClassName;
}
