#include "Commond.h"
#include "../Scenes/GameMap.h"
#include "../GLCommon/Utils/ResMgr.h"
#include "../GLCommon/Utils/ToolsUtil.h"
#include "ParamMgr.h"
#include "GameLayer.h"
#include "GLCommon/UI/GLMenuItemSprite.h"
#include "Story/StoryMgr.h"
#include "DataParam/UserData.h"

extern const char* KEY_STORY_GOLDGET;

PlayerSayCommond * PlayerSayCommond::createCommond(cocos2d::Node * target, std::string saycontext, int playeridx)
{
	auto pRef = new PlayerSayCommond();

	if (pRef && pRef->initWithTarget(target, saycontext, playeridx))
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

bool PlayerSayCommond::initWithTarget(cocos2d::Node * target, std::string saycontext, int playeridx)
{
	_targetNode = target;
	_sayContext = saycontext;
	_playeridx = playeridx;

	return true;
}

void PlayerSayCommond::execute()
{
	//CCLOG(_sayContext.c_str());

	auto layer = GameLayer::getInstance()->getStoryListView();
	auto text = ui::Text::create();
	text->setString("");
	text->setFontSize(32);
	text->setColor(Color3B(0xbf, 0x6b, 0x11));
	auto text2 = ui::Text::create();
	text2->setFontSize(26);
	text2->setColor(Color3B(0xc9, 0xa1, 0x53));
	text2->setTextAreaSize(Size(600.f, 100.f));
	
	if (_playeridx >= 0 && _playeridx <= 3)
	{
		text2->setString(string("      ") + _sayContext);

		auto name = ResMgr::getInstance()->getString(String::createWithFormat("player_%d", _playeridx)->getCString())->getCString();
		text->setString(name);
		auto layout2 = ui::LinearLayoutParameter::create();
		layout2->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
		text2->setLayoutParameter(layout2);
		text2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}
	else if (_playeridx == -1)
	{
		text2->setString(_sayContext);
		text2->setTextAreaSize(Size(600.f, 340.f));

		auto layout = ui::LinearLayoutParameter::create();
		layout->setGravity(ui::LinearLayoutParameter::LinearGravity::RIGHT);
		text->setLayoutParameter(layout);

		auto layout2 = ui::LinearLayoutParameter::create();
		layout2->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
		text2->setLayoutParameter(layout2);
		text2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}
	else
	{
		text2->setString(string("      ") + _sayContext);

		auto name = ParamMgr::getInstance()->getMonsterInfo(_playeridx)->name;
		text->setString(name);

		auto layout = ui::LinearLayoutParameter::create();
		layout->setGravity(ui::LinearLayoutParameter::LinearGravity::RIGHT);
		text->setLayoutParameter(layout);
	
		auto layout2 = ui::LinearLayoutParameter::create();
		layout2->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
		text2->setLayoutParameter(layout2);
		text2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}

	layer->pushBackCustomItem(text);
	layer->pushBackCustomItem(text2);

	text->setOpacity(0);
	text2->setOpacity(0);

	auto action = FadeIn::create(1.0f);
	text->runAction(action);

	auto action2 = FadeIn::create(1.0f);
	text2->runAction(action2);
	//layer->scrollToBottom(1.0f, true);

	if (_commondCallback)
	{
		_commondCallback(this);
	}
}

SelectCommonds* SelectCommonds::createCommond(cocos2d::Node* target)
{
	auto pRef = new SelectCommonds();
	if (pRef && pRef->initWithTarget(target))
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
}

bool SelectCommonds::initWithTarget(cocos2d::Node* target)
{
	_target = target;
	return true;
}

void SelectCommonds::execute()
{
	auto layer = GameLayer::getInstance()->getStoryListView();
	MenuItemFont::setFontSize(28);
	int idx = 0;
	auto alignmentMenu = Menu::create();
	
	for (auto& temp : _selectVector)
	{
		auto menuSprite = Sprite::createWithSpriteFrameName("com_story_textbg.png");

		std::string des = String::createWithFormat("%d.", idx + 1)->getCString() + temp.des;
		auto text = Text::create();
		menuSprite->addChild(text);
		text->setString(des);
		text->setColor(Color3B(0xe4, 0xac, 0x03));
		text->setFontSize(24);
		text->setPosition(menuSprite->getContentSize() / 2);

		auto left = glui::GLMenuItemSprite::create(menuSprite, [=](cocos2d::Ref* sender) {
			CCLOG("touch select %d", ((cocos2d::Node*)(sender))->getTag());
			GameLayer::getInstance()->removeSelectMenu();
			GameLayer::getInstance()->contitueStoryAfterSelect(((cocos2d::Node*)(sender))->getTag());
		});

		left->setTag(temp.next);
		idx++;
		
		//left->setAnchorPoint(Vec2(0.f, 0.5f));
		alignmentMenu->addChild(left, 1);
	}

	alignmentMenu->alignItemsVerticallyWithPadding(40.f);
	GameLayer::getInstance()->showSelectMenu(alignmentMenu);

	if (_commondCallback)
	{
		_commondCallback(this);
	}
}

CommandMgr::CommandMgr():_isStoryEnd(false),
	_isExecuting(false),
	_commondidx(0),
	_pause(false)
{
}

CommandMgr::~CommandMgr()
{
}

void CommandMgr::prepare(int idx, int happenpos)
{
	clearCommond();
	_isStoryEnd = false;
	_happonpos = happenpos;

	_commondidx = 0;

	rapidjson::Document _jsonBezierDoc;
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(String::createWithFormat("story/story%d.json", idx)->getCString());
	_jsonBezierDoc.Parse<0>(contentStr.c_str());
	if (_jsonBezierDoc.HasParseError())
	{
		CCLOG("JSonItemsError%s\n", _jsonBezierDoc.GetParseError());
	}

	rapidjson::Value* staritem;
	if (happenpos == 0)
	{
		staritem = &_jsonBezierDoc["story_start"];
	}
	else if(happenpos == 2)
	{
		staritem = &_jsonBezierDoc["story_end"];
	}
	else if (happenpos == 1)
	{
		staritem = &_jsonBezierDoc["story_boss"];
	}

	auto count = (*staritem).MemberCount();
	int next = 0;
#if 1
	for (int i = 0; i < count; i++)
	{
		rapidjson::Value& item = (*staritem)[String::createWithFormat("command_%d", i)->getCString()];
		std::string type = item["action"].GetString();
		if (type.compare("say") == 0)
		{
			auto say = PlayerSayCommond::createCommond(nullptr, item["config"].GetString(), item["target"].GetDouble());
			say->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			say->_autorun = false;
			
			say->_comid = item["id"].GetInt();
			//debug zsb
			say->_nextid = item["next"].GetInt();
			
			/*say->_nextid = i + 1;
			if (i == count - 1)
			{
				say->_nextid = -1;
			}*/
			pushCommond(say);
		}
		else if (type.compare("commands") == 0)
		{
			cocos2d::Vector<BaseCommond*> _vector;
			rapidjson::Value& arrcoms = item["config"];

			for (int i = 0; i < arrcoms.Capacity(); i++)
			{
				rapidjson::Value& singleCommonds = arrcoms[i];
				std::string type2 = singleCommonds["action"].GetString();
				if (type2.compare("say") == 0)
				{
					auto say2 = PlayerSayCommond::createCommond(nullptr, singleCommonds["config"].GetString(), singleCommonds["target"].GetDouble());
					say2->_commondCallback = nullptr;
					_vector.pushBack(say2);
				}
			}

			auto group = GroupCommonds::createCommond(_vector);
			group->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			group->_autorun = false;
			
			group->_comid = item["id"].GetInt();
			group->_nextid = item["next"].GetInt();

			//debug zsb
			/*group->_nextid = i + 1;
			if (i == count - 1)
			{
				group->_nextid = -1;
			}*/

			pushCommond(group);
		}
		else if (type.compare("select") == 0)
		{
			auto say = SelectCommonds::createCommond(nullptr);
			say->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			say->_autorun = false;
			say->_comid = item["id"].GetInt();
			say->_nextid = -1;

			rapidjson::Value& arrdes = item["config"];
			rapidjson::Value& nextarr = item["selectnext"];

			for (int i = 0; i < arrdes.Capacity(); i++)
			{
				Select_T select;
				select.des = arrdes[i].GetString();
				select.next = nextarr[i].GetInt();
				say->_selectVector.push_back(select);
			}

			pushCommond(say);
		}
		else if (type.compare("reward") == 0)
		{
			auto say = RewardCommonds::createCommond(nullptr, item["config"].GetString());
			say->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			say->_autorun = false;
			say->_comid = item["id"].GetInt();
			say->_nextid = item["next"].GetInt();

			rapidjson::Value& rewarditem = item["rewarditem"];
			rapidjson::Value& rewardnum = item["rewardnum"];
			for (int i = 0; i < rewardnum.Capacity(); i++)
			{
				Reward_T select;
				select.itemid = rewarditem[i].GetInt();
				select.num = rewardnum[i].GetInt();
				say->_rewardVector.push_back(select);
			}

			pushCommond(say);
		}
	}
#else
	while (next != -1)
	{
		rapidjson::Value& item = (*staritem)[String::createWithFormat("command_%d", next)->getCString()];
		std::string type = item["action"].GetString();
		if (type.compare("say") == 0)
		{
			auto say = PlayerSayCommond::createCommond(nullptr, item["config"].GetString(), item["target"].GetDouble());
			say->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			say->_autorun = false;
			pushCommond(say);
		}
		else if (type.compare("commands") == 0)
		{
			cocos2d::Vector<BaseCommond*> _vector;
			rapidjson::Value& arrcoms = item["config"];

			for (int i = 0; i < arrcoms.Capacity(); i++)
			{
				rapidjson::Value& singleCommonds = arrcoms[i];
				std::string type2 = singleCommonds["action"].GetString();
				if (type2.compare("say") == 0)
				{
					auto say2 = PlayerSayCommond::createCommond(nullptr, singleCommonds["config"].GetString(), singleCommonds["target"].GetDouble());
					say2->_commondCallback = nullptr;
					_vector.pushBack(say2);
				}
			}

			auto group = GroupCommonds::createCommond(_vector);
			group->_commondCallback = std::bind(&CommandMgr::commondCallback, this, std::placeholders::_1);
			group->_autorun = false;
			pushCommond(group);

		}
		else if ()
		{
		}
	}
#endif
	
	

	
}

void CommandMgr::pushCommond(BaseCommond * com)
{
	com->retain();
	_commondQueue.push_back(com);
}

void CommandMgr::executeCommond()
{
	if (_pause)
	{
		return;
	}

	if (!_commondQueue.empty() && _commondidx != -1)
	{
		_isExecuting = true;
		int tempidx = 0;
		for (auto& temp : _commondQueue)
		{
			if (temp->_comid == _commondidx)
			{
				auto layer = GameLayer::getInstance()->getStoryListView();
				layer->removeAllItems();

				if (dynamic_cast<SelectCommonds*>(temp))
				{
					GameMap::getCurGameMap()->getStorySkillNode()->setVisible(false);
				}
				else
				{
					GameMap::getCurGameMap()->getStorySkillNode()->setVisible(true);
				}

				_commondidx = temp->_nextid;		
				temp->execute();
				return;
			}
			else
			{
			
			}
			tempidx++;
		}
	}
	else
	{
		if (_isStoryEnd == false)
		{
			GameMap::getCurGameMap()->storyEnd(_happonpos);
			_isStoryEnd = true;
		}	
	}
}

void CommandMgr::commondCallback(BaseCommond* type)
{
	if (type->_autorun)
	{
		executeCommond();
	}
	else
	{
		if (dynamic_cast<SelectCommonds*>(type))
		{
			_pause = true;
		}

		_isExecuting = false;
	}
}

void CommandMgr::clearCommond()
{
	while (_commondQueue.size() != 0)
	{
		auto com = _commondQueue.back();
		_commondQueue.pop_back();
		com->release();
	}
}

GroupCommonds * GroupCommonds::createCommond(cocos2d::Vector<BaseCommond*> _vector)
{
	auto pRef = new GroupCommonds();

	if (pRef && pRef->init(_vector))
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

bool GroupCommonds::init(cocos2d::Vector<BaseCommond*> _vector)
{
	_vectorCommonds = _vector;

	return true;
}

void GroupCommonds::execute()
{
	for (auto& temp : _vectorCommonds)
	{
		temp->execute();
	}

	if (_commondCallback)
	{
		_commondCallback(this);
	}
}

RewardCommonds* RewardCommonds::createCommond(cocos2d::Node* target, std::string des)
{
	auto pRef = new RewardCommonds();
	if (pRef && pRef->initWithTarget(target, des))
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
		return nullptr;
	}
}

bool RewardCommonds::initWithTarget(cocos2d::Node* target, std::string des)
{
	_target = target;
	_sayContext = des;

	return true;
}

void RewardCommonds::execute()
{
	auto layer = GameLayer::getInstance()->getStoryListView();
	auto text = ui::Text::create();
	text->setString("");
	text->setFontSize(32);
	text->setColor(Color3B(0xbf, 0x6b, 0x11));
	auto text2 = ui::Text::create();
	text2->setFontSize(26);
	text2->setColor(Color3B(0xc9, 0xa1, 0x53));
	text2->setTextAreaSize(Size(600.f, 90.f));

	text2->setString(_sayContext);
	text2->setTextAreaSize(Size(600.f, 140.f));

	auto layout = ui::LinearLayoutParameter::create();
	layout->setGravity(ui::LinearLayoutParameter::LinearGravity::RIGHT);
	text->setLayoutParameter(layout);

	auto layout2 = ui::LinearLayoutParameter::create();
	layout2->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
	text2->setLayoutParameter(layout2);
	text2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	
	layer->pushBackCustomItem(text);
	layer->pushBackCustomItem(text2);

	text->setOpacity(0);
	text2->setOpacity(0);

	auto action = FadeIn::create(1.0f);
	text->runAction(action);

	auto action2 = FadeIn::create(1.0f);
	text2->runAction(action2);
	
	auto key = String::createWithFormat(KEY_STORY_GOLDGET, StoryMgr::getInstance()->getCurStoryConfig().fileid, GameLayer::getInstance()->getCurStoryCheckType(), _comid);
	
	if (_rewardVector[0].num > 0 && UserData::getInstance()->getItemBalance(key->getCString(), false) == 0)
	{
		GameLayer::getInstance()->showReward(_rewardVector);
		UserData::getInstance()->giveItem(key->getCString(), 1, false);
	}	

	UserData::getInstance()->freeDB();

	if (_commondCallback)
	{
		_commondCallback(this);
	}
}

BaseCommond::BaseCommond():_nextid(-1),
_comid(-1),
_autorun(false),
_commondCallback(nullptr)
{

}
