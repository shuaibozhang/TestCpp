#include "GameGUIReader.h"
#include "GameButton.h"


static GameGUIReader* sharedReader = nullptr;


GameGUIReader::GameGUIReader()
{
	ObjectFactory* factoryCreate = ObjectFactory::getInstance();
	factoryCreate->registerType(CREATE_CLASS_GUI_INFO(GameButton));
}


GameGUIReader* GameGUIReader::getInstance()
{
	if (!sharedReader)
	{
		sharedReader = new (std::nothrow) GameGUIReader();
	}
	return sharedReader;
}


void GameGUIReader::destroyInstance()
{
	CC_SAFE_DELETE(sharedReader);
}

#if 0
Widget* GameGUIReader::widgetFromJsonFile(const char *fileName)
{
	std::string jsonpath;
	rapidjson::Document jsonDict;
	jsonpath = fileName;
	//    jsonpath = CCFileUtils::getInstance()->fullPathForFilename(fileName);
	size_t pos = jsonpath.find_last_of('/');
	m_strFilePath = jsonpath.substr(0,pos+1);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	jsonDict.Parse<0>(contentStr.c_str());
	if (jsonDict.HasParseError())
	{
		CCLOG("GetParseError %s\n",jsonDict.GetParseError());
	}
	Widget* widget = nullptr;
	const char* fileVersion = DICTOOL->getStringValue_json(jsonDict, "version");
	WidgetPropertiesReader * pReader = nullptr;
	if (fileVersion)
	{
		int versionInteger = getVersionInteger(fileVersion);
		if (versionInteger < 250)
		{
			pReader = new (std::nothrow) WidgetPropertiesReader0250();
			widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
		}
		else
		{
			pReader = new (std::nothrow) GameWidgetPropertiesReader0300();
			widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
		}
	}
	else
	{
		pReader = new (std::nothrow) WidgetPropertiesReader0250();
		widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
	}

	CC_SAFE_DELETE(pReader);
	return widget;
}
#endif

Widget* GameGUIReader::widgetFromJsonFile(const char *fileName)
{
	std::string jsonpath;
	rapidjson::Document jsonDict;
	jsonpath = fileName;
	//    jsonpath = CCFileUtils::getInstance()->fullPathForFilename(fileName);
	size_t pos = jsonpath.find_last_of('/');
	m_strFilePath = jsonpath.substr(0,pos+1);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	jsonDict.Parse<0>(contentStr.c_str());
	if (jsonDict.HasParseError())
	{
		CCLOG("GetParseError %s\n",jsonDict.GetParseError());
	}
	Widget* widget = nullptr;
	const char* fileVersion = DICTOOL->getStringValue_json(jsonDict, "version");
	WidgetPropertiesReader * pReader = nullptr;
	if (fileVersion)
	{
		int versionInteger = getVersionInteger(fileVersion);
		if (versionInteger < 250)
		{
			pReader = new (std::nothrow) WidgetPropertiesReader0250();
			widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
		}
		else
		{
			pReader = new (std::nothrow) GameWidgetPropertiesReader0300();
			widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
		}
	}
	else
	{
		pReader = new (std::nothrow) WidgetPropertiesReader0250();
		widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
	}

	CC_SAFE_DELETE(pReader);
	return widget;
}


std::string GameWidgetPropertiesReader0300::getGUIClassName(const std::string &name)
{
	std::string convertedClassName = WidgetPropertiesReader0300::getGUIClassName(name);

	if (name == "Button")
	{
		convertedClassName = "GameButton";
	}
	/*else if (name == "Sprite")
	{
		convertedClassName = "GameSprite";
	}*/

	return convertedClassName;
}