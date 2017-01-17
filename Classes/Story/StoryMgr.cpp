#include "StoryMgr.h"
#include "../WxSql/GLLocalStorage.h"
#include "Commond.h"
#include "../Scenes/GameMap.h"
#include "../Scenes/MainScene.h"
#include "../DataParam/UserData.h"
#include "ParamMgr.h"
#include "../Defines.h"
#include "../GLCommon//Utils/ToolsUtil.h"
#include "../StoreBridge/StoreAssets.h"

const std::string KEY_STORY_IDX = "key_story_idx";

int StoryMgr::s_storyMax = 30;

StoryMgr* StoryMgr::_instance = nullptr;

StoryMgr::StoryMgr()
{

}

StoryMgr::~StoryMgr()
{

}

StoryMgr * StoryMgr::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new StoryMgr();
		_instance->initStory();
	}

	return _instance;
}

void StoryMgr::initStory()
{
	//std::string storyname;
	int id = UserData::getInstance()->getStoryFileIdx();
	if (id > s_storyMax)
	{
		_curStoryConfig.fileid = id;
		_curStoryConfig.posid = -1;
		_curStoryConfig.nextfildid = id;
		return;
	}
	//std::vector<std::string> curconfig;
	//storyname = "0";
	//split(storyname, "-", curconfig);
	
	//for (auto& temp : curconfig)
	//{
		//int id = atoi(storyname.c_str());
		//StoryConfig_T config;

	for (auto& temp : _arrCommandMgr)
	{
		temp = new CommandMgr();
	}

		loadStoryFileByIdx(id, _curStoryConfig);
		//_curStoryConfig.push_back(config);
	//}
}

void StoryMgr::loadStoryFileByIdx(int idx, StoryConfig_T& config)
{
	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(String::createWithFormat("story/story%d.json", idx)->getCString());
	_jsonBezierDoc.Parse<0>(contentStr.c_str());

	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}
	config.fileid = idx;
	config.posid = _jsonBezierDoc["posid"].GetDouble();

	//std::vector<int>selectANext;
	//selectANext.push_back(1);

	//config.vectorNext.push_back(selectANext);

	int i = 0;
	for (auto& temp : _arrCommandMgr)
	{		
		temp->prepare(idx, i);
		i++;
	}
}

void StoryMgr::loadNext()
{
	UserData::getInstance()->setBossPass(_curStoryConfig.posid, 1);

	int nextfile = _curStoryConfig.fileid + 1;
	if (nextfile > s_storyMax)
	{
		MainLayer::getCurMainLayer()->getWordMap()->unShowTargetInfo();
		UserData::getInstance()->setStoryFileIdx(nextfile);
		UserData::getInstance()->saveUserData(true);
		return;
	}
	loadStoryFileByIdx(nextfile, _curStoryConfig);
	
	showTargetInfo(MainLayer::getCurMainLayer()->getWordMap());

	UserData::getInstance()->setStoryFileIdx(nextfile);
	UserData::getInstance()->saveUserData(true);
}

void StoryMgr::showTargetInfo(WordMap* wordmap)
{
	//for (auto& temp : _curStoryConfig)
	//{
	int id = UserData::getInstance()->getStoryFileIdx();
	if (id > s_storyMax)
	{
		return;
	}
	wordmap->unShowTargetInfo();
	wordmap->showTargetInfo(_curStoryConfig.posid);
	//}
	
}


bool StoryMgr::checkShowStory(int btnpos, bool isstart)
{
	//for (auto& temp : _curStoryConfig)
	{
		if (_curStoryConfig.posid == btnpos)
		{
#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
			if ((ToolsUtil::getRandomInt(1, 100) <= ParamData::P_POPBUY)&& UserData::getInstance()->getItemBalance(StoreAssetMgr::ITEMID_GOOD_SUPERGIFT) == 0)
			{
				if (btnpos == 0)
				{
					MainLayer::getCurMainLayer()->getWordMap()->showDialogBuy(_curStoryConfig.posid, true);
				}
				else
				{
					MainLayer::getCurMainLayer()->getWordMap()->showDialogBuy(_curStoryConfig.posid);
				}
				
				MainLayer::getCurMainLayer()->getWordMap()->tryBackToLastPos();
			}
			else
			{
				MainLayer::getCurMainLayer()->getWordMap()->showEnterStoryDiglog(_curStoryConfig.fileid, _curStoryConfig.posid);
			}
#else
			MainLayer::getCurMainLayer()->getWordMap()->showEnterStoryDiglog(_curStoryConfig.fileid, _curStoryConfig.posid);
#endif
			
			//GameMap::getCurGameMap()->enterStory(_curStoryConfig.fileid, 0);
			return true;
		}
	}
	return false;
}

void StoryMgr::reloadStory()
{
	int i = 0;
	for (auto& temp : _arrCommandMgr)
	{
		temp->prepare(_curStoryConfig.fileid, i);
		i++;
	}
}

bool StoryMgr::isStoryPos(int pos)
{
	auto& arr = ParamMgr::getInstance()->getStoryPos();

	for (auto& posstory : arr)
	{
		if (pos == posstory)
		{
			return true;
		}
	}
	return false;
}

bool StoryMgr::isStoryOver()
{
	auto idx = UserData::getInstance()->getStoryFileIdx();
	if (idx > s_storyMax)
	{
		return true;
	}

	return false;
}
