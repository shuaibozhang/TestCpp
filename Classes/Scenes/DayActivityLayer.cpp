#include "DayActivityLayer.h"
#include "DayActiveMgr.h"
#include "VisibleRect.h"
#include "ui/CocosGUI.h"
#include "GLCommon/Utils/ResMgr.h"

USING_NS_CC;
using namespace ui;

DayActivityLayer::DayActivityLayer()
{
}

DayActivityLayer::~DayActivityLayer()
{
}

bool DayActivityLayer::init()
{
	LayerColor::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	float offsetY = (VisibleRect::top().y - 960.f);

	_pRoot = CSLoader::createNode("csb/dayactivitylayer.csb");
	this->addChild(_pRoot);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	_pRoot->setPositionY(-VisibleRect::top().y + 100);
	_pRoot->runAction(actionshow);

	static_cast<ImageView*>(_pRoot->getChildByName("main_bg_0"))->setContentSize(Size(640.f, 880.f + offsetY));
	static_cast<ImageView*>(_pRoot->getChildByName("Image_1"))->setContentSize(Size(530.f, 500.f + offsetY));
	static_cast<ImageView*>(_pRoot->getChildByName("ListView_5"))->setContentSize(Size(520.f, 450.f + offsetY));
	_pRoot->getChildByName("nodeup")->setPositionY(offsetY);
	
	static_cast<Button*>(_pRoot->getChildByName("nodeup")->getChildByName("btn_exit"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			this->removeFromParent();
		}
	});

	auto listview = _pRoot->getChildByName("ListView_5");

	for (int i = 0; i < DAYALLNUMS; i++)
	{
		auto node = listview->getChildByName(String::createWithFormat("node_%d", i)->getCString());
		static_cast<Text*>(node->getChildByName("Text_68_0"))->setString(ResMgr::getInstance()->getString(String::createWithFormat("day_activity_des_%d", i)->getCString())->getCString());

		int curtime = DayActivityMgr::getInstance()->getActivityTimes(DayActivityTppe(i));
		int max = DayActivityMgr::getInstance()->getActivityMaxTimes(DayActivityTppe(i));

		auto text1 = static_cast<Text*>(node->getChildByName("Text_68"));
		text1->setString(String::createWithFormat("%d/%d", curtime, max)->getCString());

		text1->setPositionX(text1->getPositionX() + 20.f);

		auto text2 = static_cast<Text*>(node->getChildByName("Text_69"));
		text2->setString(String::createWithFormat("+%d", DayActivityMgr::getInstance()->getActivityScore(DayActivityTppe(i)))->getCString());

		if (curtime == 0)
		{
			text2->setVisible(false);
		}

		if (curtime != max)
		{
			//node->getChildByName("Image_75")->setVisible(false);
			static_cast<ImageView*>(node->getChildByName("Image_75"))->loadTexture("dayactivity_text_wwc.png", Widget::TextureResType::PLIST);
		}
		else
		{
			static_cast<Text*>(node->getChildByName("Text_68"))->setColor(Color3B(0x33, 0x90, 0x05));
		}
	}

	int score = DayActivityMgr::getInstance()->getAllActivityScore();

	static_cast<LoadingBar*>(_pRoot->getChildByName("scorebar"))->setPercent(score / 2.4);
	static_cast<TextAtlas*>(_pRoot->getChildByName("curscore"))->setString(String::createWithFormat("%d", score)->getCString());
	
	int needscore = DayActivityMgr::getInstance()->getNeedScore();
	auto neddscoretext = static_cast<Text*>(_pRoot->getChildByName("needscore"));
	
	if (needscore != 0)
	{
		neddscoretext->setString(String::createWithFormat(neddscoretext->getString().c_str(), needscore)->getCString());
	}
	else
	{
		neddscoretext->setVisible(false);
	}

	updateBtn();

	static_cast<Button*>(_pRoot->getChildByName("btnget"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			DayActivityMgr::getInstance()->getReward();
			updateBtn();
		}
	});

	return true;
}

void DayActivityLayer::updateBtn()
{
	int canget = false;
	for (int i = 0; i < 3; i++)
	{
		auto key = String::createWithFormat("textget_%d", i);
		auto icon = static_cast<Sprite*>(_pRoot->getChildByName(key->getCString()));
		int flag = DayActivityMgr::getInstance()->checkCanGet(i);
		if (flag == 0)
		{
			icon->setVisible(false);
		}
		else if (flag == 1)
		{
			canget = true;
		}
		else if (flag == 2)
		{
			icon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("dayactivity_text_ylq.png"));
		}
	}

	if (canget)
	{
		
	}
	else
	{
		_pRoot->getChildByName("btnget")->setVisible(false);
	}
}
