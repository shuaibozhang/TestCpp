#include "DungeonLayer.h"
#include "UserData.h"
#include "VisibleRect.h"
#include "GameMap.h"
#include "NetDataMgr.h"
#include "Player.h"
#include "../DataParam/TimeLimitActivityMgr.h"
#include "../Defines.h"

DungeonInfo_T _arrDungeonInfo[3] = {
	{ DUNGEON_GOLD ,{ 1,0,1,0,1,0,1 },{ 5,10,15 } },
	{ DUNGEON_EXP ,{ 0,1,0,1,0,1,1 },{10,20,30 } },
	{ DUNGEON_WENPON ,{ 0,0,0,0,0,0,1 },{20,30,40 } },
};

DungeonLayer::DungeonLayer() :_totalTimes(8),
_freeTimes(5)
{
}

DungeonLayer::~DungeonLayer()
{
}

bool DungeonLayer::init()
{
	int curDayInWeek = NetDataMgr::getInstance()->getOnlineDayInWeek() - 1;

	Layer::init();
	if (VipMgr::getInstance()->haveBuyVip())
	{
		_totalTimes += 2;
		_freeTimes += 2;
	}
	

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	float offsetY = (VisibleRect::top().y - 960.f);

	_mainRoot = CSLoader::createNode("csb/dungeonlayer.csb");
	this->addChild(_mainRoot);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	_mainRoot->setPositionY(-VisibleRect::top().y + 100);
	_mainRoot->runAction(actionshow);

	static_cast<Button*>(_mainRoot->getChildByName("nodeup")->getChildByName("btn_exit"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	static_cast<ImageView*>(_mainRoot->getChildByName("main_bg_0"))->setContentSize(Size(640.f, 880.f + offsetY));
	_mainRoot->getChildByName("nodeup")->setPositionY(offsetY);

	for (int i = 0; i < 3; ++i)
	{
		auto node = _mainRoot->getChildByName(String::createWithFormat("node_%d", i)->getCString());

		node->setPositionY(node->getPositionY() + offsetY);

		auto timeSprite = static_cast<ImageView*>(node->getChildByName("time"));
		int curtime = UserData::getInstance()->getDungeonTime(i);
		int havetime = _freeTimes - curtime >= 0 ? _freeTimes - curtime : 0;
		havetime = havetime * _arrDungeonInfo[i]._weekStage[curDayInWeek];
		timeSprite->loadTexture(String::createWithFormat("dungeon_text_time%d.png", havetime)->getCString(),Widget::TextureResType::PLIST);

		auto costSprite = static_cast<Sprite*>(node->getChildByName("des"));
		if (curtime >= _freeTimes && curtime < _totalTimes)
		{
			auto des = static_cast<Sprite*>(node->getChildByName("des"));
			int priceidx = curtime - _freeTimes;
			priceidx = priceidx >= 3 ? 2 : priceidx;

			auto cost = ui::TextAtlas::create(String::createWithFormat("%d", _arrDungeonInfo[i]._timesCost[priceidx])->getCString(), "fonts/dungeon_font_costnum.png", 17.f, 25.f, ".");
			node->addChild(cost, 3);
			cost->setPosition(des->getPosition() + Vec2(40.f, 0.f));

			des->setPositionX(des->getPositionX() - 20.f);
			des->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("dungeon_text_tz2.png"));
		}

		auto btngo = static_cast<Button*>(node->getChildByName("btn_go"));
		btngo->setUserData((void*)i);
		btngo->addTouchEventListener(CC_CALLBACK_2(DungeonLayer::menuOnGo, this));
	}
	
	updateShowBtn();

	return true;
}

void DungeonLayer::menuOnGo(Ref *ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int curDayInWeek = NetDataMgr::getInstance()->getOnlineDayInWeek() - 1;
	Node* tempBtn = static_cast<Node*>(ref);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int idx = long(tempBtn->getUserData());
#else
	int idx = int(tempBtn->getUserData());
#endif
	
	if (_arrDungeonInfo[idx]._weekStage[curDayInWeek] == 0)
	{
		return;
	}

	int times = UserData::getInstance()->getDungeonTime(idx);
	int cost = 0;

	if (times >= _freeTimes)
	{
		int priceidx = times - _freeTimes;
		priceidx = priceidx >= 3 ? 2 : priceidx;
		cost = _arrDungeonInfo[idx]._timesCost[priceidx];
	}

	if (UserData::getInstance()->getCrystalNum() >= cost)
	{
		UserData::getInstance()->giveCrystal(-cost);
		UserData::getInstance()->setDungeonTime(idx, times + 1);
		UserData::getInstance()->saveUserData();

		GameMap::getCurGameMap()->enterDungeon(idx);

		this->removeFromParent();
		//add enter dungeon;
	}
	else
	{
		return;
	}
}

void DungeonLayer::updateShowBtn()
{
	int curDayInWeek = NetDataMgr::getInstance()->getOnlineDayInWeek() - 1;

	auto state = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_GRAYSCALE);

	for (int i = 0; i < 3; ++i)
	{
		auto node = _mainRoot->getChildByName(String::createWithFormat("node_%d", i)->getCString());
		auto btngo = static_cast<Button*>(node->getChildByName("btn_go"));

		if (UserData::getInstance()->getDungeonTime(i) >=_totalTimes || _arrDungeonInfo[i]._weekStage[curDayInWeek] == 0)
		{
			btngo->setEnabled(false);
		}

	}

}

DungeonMissionNode * DungeonMissionNode::nodeWithIdx(int idx)
{
	auto pRef = new DungeonMissionNode();
	if (pRef && pRef->initWithIdx(idx))
	{
		pRef->autorelease();
		return pRef;
	}
	else
	{
		delete pRef;
	}

	return nullptr;
}

bool DungeonMissionNode::initWithIdx(int idx)
{
	Node::init();
	_pMissionTextAtlas = nullptr;
	_idx = idx;
	_curStageIdx = 0;

	_root = CSLoader::createNode("csb/dungeonMission.csb");
	this->addChild(_root);

	_pMissionTextAtlas = static_cast<TextAtlas*>(_root->getChildByName("num"));

	auto icon = static_cast<Sprite*>(_root->getChildByName("icon_mission"));
	if (idx >= 0 && idx <= 3)
	{
		int roleId = Player::getInstance()->getRoleIdByPosIndex(idx);
		icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("dungeon_icon_player_%d.png", roleId)->getCString()));
	}
	else
	{
		icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("dungeon_icon_effect_%d.png", idx)->getCString()));
	}


	return true;
}

void DungeonMissionNode::setString(const std::string & desstring)
{
	if (_pMissionTextAtlas)
	{
		_pMissionTextAtlas->setString(desstring);
	}
}

void DungeonMissionNode::setMissionNum(int cur, int target)
{
	auto value = String::createWithFormat("%d/%d", cur, target)->getCString();
	if (_pMissionTextAtlas)
	{
		if (cur >= target && _curStageIdx == 0)
		{
			_pMissionTextAtlas->removeFromParent();
			_pMissionTextAtlas = TextAtlas::create(value, "fonts/dungeon_font_num_1.png", 14, 20, ".");
			_root->addChild(_pMissionTextAtlas);
			//_pMissionTextAtlas->setScale(0.75f);
			_pMissionTextAtlas->setPositionY(10.f);

			_curStageIdx = 1;
		}
		else if (cur < target && _curStageIdx == 1)
		{
			_pMissionTextAtlas->removeFromParent();
			_pMissionTextAtlas = TextAtlas::create(value, "fonts/dungeon_font_num_0.png", 14, 20, ".");
			_root->addChild(_pMissionTextAtlas);
			//_pMissionTextAtlas->setScale(0.75f);
			_pMissionTextAtlas->setPositionY(10.f);

			_curStageIdx = 0;
		}

		auto action = Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f));
		_pMissionTextAtlas->stopAllActions();
		_pMissionTextAtlas->runAction(action);
		_pMissionTextAtlas->setString(value);
	}
}
