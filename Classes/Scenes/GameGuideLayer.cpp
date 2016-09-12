#include "GameGuideLayer.h"
#include "GLCommon/JRCommon.h"

GameGuideLayer *GameGuideLayer::s_pInstance = nullptr;

GameGuideLayer::GameGuideLayer()
{
	s_pInstance = this;
}


GameGuideLayer::~GameGuideLayer()
{
	s_pInstance = nullptr;
}


GameGuideLayer * GameGuideLayer::getInstance()
{
	return s_pInstance;
}

GameGuideLayer *GameGuideLayer::create(int guideType)
{
	GameGuideLayer *pRet = new GameGuideLayer();
	if (nullptr != pRet && pRet->init(guideType))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}


bool GameGuideLayer::init(int guideType)
{
	LayerColor::initWithColor(Color4B(0, 0, 0, 0));

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/guideUi.plist");

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameGuideLayer::onTouchBegan, this);//[this](Touch*, Event*) { return true; };
//	listener->onTouchEnded = CC_CALLBACK_2(GameGuideLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_guideType = guideType;
	_guideStep = 0;
	_isStart = false;

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/guangquan_dianji/guangquan_dianji.ExportJson");

	_pArmtr = Armature::create("guangquan_dianji");
	this->addChild(_pArmtr);
	_pArmtr->getAnimation()->play("dianji");
	_pArmtr->setVisible(false);

	return true;
}

bool GameGuideLayer::onTouchBegan(Touch * touch, Event * evt)
{
	if (_isStart)
	{
		if (0 == _touchType)
		{
			this->guideStepEnd();
		}
		else if (1 == _touchType)
		{
			Vec2 pos = touch->getLocation();
			pos = this->convertToNodeSpace(pos);
			Vec2 armPos = _pArmtr->getPosition();

			float width = 70.f;
			float height = 70.f;
			Rect rect(armPos.x-width/2, armPos.y-height/2, width, height);
			if (rect.containsPoint(pos))
			{
				return false;
			}
		}
	}
	return true;
}

void GameGuideLayer::onTouchEnded(Touch * touch, Event * evt)
{
	
}

void GameGuideLayer::guideStepStart(int touchType, const Vec2 &pos)
{
	_isStart = true;
	_touchType = touchType;
	if (0 == _touchType)
	{
		_pArmtr->setVisible(false);
	}
	else if (1 == _touchType)
	{
		_pArmtr->setVisible(true);
		_pArmtr->setPosition(pos);
	}

	if (0 == _guideType)
	{
		if (0 == _guideStep || 3 == _guideStep)
		{
			auto pBg = ui::Scale9Sprite::createWithSpriteFrameName("guide_dialog_bg.png");
			this->addChild(pBg, 1, ParamData::GUIDE_NODE_TAG);
			pBg->setPosition(Vec2(320.f, 120.f));
			pBg->setContentSize(Size(638.f, 236.f));

			auto talkIdx = __String::createWithFormat("guide_0_%d", _guideStep);
			auto strText = ResMgr::getInstance()->getString(talkIdx->getCString());

			auto pLbl = Label::createWithSystemFont(strText->getCString(), "fonts/Marker Felt.ttf", 24, Size(339.f, 172.f));
			pLbl->setAlignment(TextHAlignment::LEFT);
			this->addChild(pLbl, 1, ParamData::GUIDE_NODE_TAG);
			pLbl->setTextColor(Color4B(0xff, 0xfe, 0xa0, 0xff));
			pLbl->setPosition(443.f, 120.f);

			auto pRole = Sprite::createWithSpriteFrameName("guide_role.png");
			pRole->setPosition(120.f, 140.f);
			this->addChild(pRole, 1, ParamData::GUIDE_NODE_TAG);

// 			auto pSkipIcon = Armature::create("jiantou");
// 			pSkipIcon->getAnimation()->play("jiantou");
// 			this->addChild(pSkipIcon, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);
// 			pSkipIcon->setPosition(Vec2(583.f, 63.f));
		}
	}
	else if (1 == _guideType)
	{
		if (0 == _guideStep || 2 == _guideStep || 8 == _guideStep || 10 == _guideStep || 11 == _guideStep)
		{
			float offsetY = 0.f;
			if (0 == _guideStep || 11 == _guideStep)
			{
				offsetY = 300.f;
			}

			auto pBg = ui::Scale9Sprite::createWithSpriteFrameName("guide_dialog_bg.png");
			this->addChild(pBg, 1, ParamData::GUIDE_NODE_TAG);
			pBg->setPosition(Vec2(320.f, 120.f + offsetY));
			pBg->setContentSize(Size(638.f, 236.f));

			auto talkIdx = __String::createWithFormat("guide_1_%d", _guideStep);
			auto strText = ResMgr::getInstance()->getString(talkIdx->getCString());

			auto pLbl = Label::createWithSystemFont(strText->getCString(), "fonts/Marker Felt.ttf", 24, Size(339.f, 172.f));
			pLbl->setAlignment(TextHAlignment::LEFT);
			this->addChild(pLbl, 1, ParamData::GUIDE_NODE_TAG);
			pLbl->setTextColor(Color4B(0xff, 0xfe, 0xa0, 0xff));
			pLbl->setPosition(443.f, 120.f + offsetY);

			auto pRole = Sprite::createWithSpriteFrameName("guide_role.png");
			pRole->setPosition(120.f, 140.f + offsetY);
			this->addChild(pRole, 1, ParamData::GUIDE_NODE_TAG);

			// 			auto pSkipIcon = Armature::create("jiantou");
			// 			pSkipIcon->getAnimation()->play("jiantou");
			// 			this->addChild(pSkipIcon, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);
			// 			pSkipIcon->setPosition(Vec2(583.f, 63.f));
		}
	}

	_guidePos = pos;
}

void GameGuideLayer::guideStepEnd()
{
	_guideStep++;
	_isStart = false;
	_pArmtr->setVisible(false);
	this->removeAllChildrenByTag(ParamData::GUIDE_NODE_TAG);

	if (0 == _guideType)
	{
		if (4 == _guideStep)
		{
			this->removeFromParent();
			return;
		}
	}
	else if (1 == _guideType)
	{
		if (12 == _guideStep)
		{
			this->removeFromParent();
			return;
		}
		else if (2 == _guideStep)
		{
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {guideStepStart(0, Vec2::ZERO); })));
			return;
//			this->guideStepStart(0, Vec2::ZERO);
		}
		else if (8 == _guideStep)
		{
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {guideStepStart(0, Vec2::ZERO); })));
			return;
//			this->guideStepStart(0, Vec2::ZERO);
		}
		else if (10 == _guideStep)
		{
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {guideStepStart(0, Vec2::ZERO); })));
			return;
//			this->guideStepStart(0, Vec2::ZERO);
		}
	}

	_eventDispatcher->dispatchCustomEvent("guide_end");
}

void GameGuideLayer::setTouchInfo(int touchType, const Vec2 & pos)
{
}

void GameGuideLayer::removeAllChildrenByTag(int tag)
{
	while (nullptr != this->getChildByTag(tag))
	{
		this->removeChildByTag(tag);
	}
}