#include "StartScene.h"
#include "Widgets/GameCSLoader.h"
#include "LoadingScene.h"
#include "VisibleRect.h"
#include "MainScene.h"
#include "GameButton.h"
#include "GLCommon/UI/GLSlidBar.h"
#include "platBridge/cocos2dx_plat.h"
#include "../Story/StoryMgr.h"
#include "UserData.h"
#include "platBridge/cocos2dx_analyze.h"
#include "Defines.h"
#include "PlayerMgr.h"

StartScene::StartScene()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/denglu.ExportJson");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/startui.plist");
}

StartScene::~StartScene()
{
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("effect/denglu.ExportJson");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/startui.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("maps/start_logo_bg.png");
}

Scene * StartScene::scene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	Layer::init();

#ifdef  USING_TIME_MGR
	TimeMgr::getInstane();
#endif
	
	PlayerMgr::getInstance()->fixPlayer();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [=](Touch*, Event*) {

		string signCode = cocos2dx_plat::getSignCode();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		if (true)
#else
		if ("-1920181079" == signCode)
#endif
		{
			if (MainLayer::getCurMainLayer())
			{
				Director::getInstance()->popScene();
			}
			else
			{
				auto scene = LoadingScene::createScene();
				Director::getInstance()->replaceScene(scene);
			}
		}
		else
		{
			cocos2dx_analyze::addCustomEvent("other_things", "support_legal");
			GameUtils::toastTip("support_legal");
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	/*auto bglayer = LayerGradient::create(Color4B(0, 128, 255, 255), Color4B(255, 255, 255, 255));
	this->addChild(bglayer);
	bglayer->setContentSize(Size(640.f, VisibleRect::top().y));*/

	auto logobg = ImageView::create("maps/start_logo_bg.png", Widget::TextureResType::LOCAL);
	this->addChild(logobg);
	logobg->setScale9Enabled(true);
	logobg->setCapInsets(Rect(0.f, 950.f, 640.f, 10.f));
	logobg->setContentSize(Size(640.f, VisibleRect::top().y));
	logobg->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	auto logo0 = Armature::create("denglu");
	logo0->getAnimation()->play("denglu");
	this->addChild(logo0);
	logo0->setPosition(Vec2(320.f, 480.f));
	logo0->getBone("Layer10")->setPositionY(VisibleRect::top().y - 960.f);
	logo0->getBone("Layer1")->setPositionY(VisibleRect::top().y - 960.f - 120.f);
	
	//logo0->getAnimation()->setSpeedScale(0.7f);

	/*auto logo1 = Armature::create("yemian");
	logo1->getAnimation()->play("logo_1");
	this->addChild(logo1);
	logo1->setPosition(Vec2(320.f, VisibleRect::top().y - 400.f));
	logo1->getAnimation()->setSpeedScale(0.2f);*/

	/*auto start = GameButton::create("start_mainbtn.png", "", "", Widget::TextureResType::PLIST);
	start->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (MainLayer::getCurMainLayer())
			{
				Director::getInstance()->popScene();
			}
			else
			{
				auto scene = LoadingScene::createScene();
				Director::getInstance()->replaceScene(scene);
			}
			
		}
	});

	this->addChild(start);
	start->setPosition(Vec2(320.f, 70.f));*/

	/*auto textlogo = Sprite::createWithSpriteFrameName("start_text_jryx.png");
	this->addChild(textlogo);
	textlogo->setPosition(Vec2(320.f, 75.f));*/

	/*auto logo = Sprite::createWithSpriteFrameName("start_logo.png");
	this->addChild(logo);
	logo->setPosition(Vec2(320.f, VisibleRect::top().y - 300.f));*/

	auto help = GameButton::create("start_help.png", "start_help_1.png", "", Widget::TextureResType::PLIST);
	help->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			showHelp();
		}
	});

	this->addChild(help);
	help->setPosition(Vec2(70.f, 60.f));

	auto textqq = Text::create();
	this->addChild(textqq);
	textqq->setFontSize(24);
	textqq->setString("QQ: 3286043664");
	textqq->setPosition(Vec2(530.f, 20.f));

#if (CC_PAY_SDK == PAY_SDK_ZHUOYI)
	auto textUserId = Text::create();
	this->addChild(textUserId);
	textUserId->setFontSize(24);
	textUserId->setPosition(Vec2(530.f, 50.f));

	auto action = Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]() {
		textUserId->setString(string("UserID: ") + cocos2dx_plat::getUserId());
	}));
	this->runAction(action);
#endif
	

	setKeyboardEnabled(true);

	_keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		if (code == EventKeyboard::KeyCode::KEY_BACK)
		{
			cocos2dx_plat::showExitDialog();
		}
	};
	
	return true;
}

void StartScene::showHelp()
{
	auto layer = GameCSLoader::createNode("csb/helplayer.csb");
	this->addChild(layer, 5);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

	static_cast<ImageView*>(layer->getChildByName("main_bg_0"))->setContentSize(Size(640.f, VisibleRect::top().y));
	static_cast<ImageView*>(layer->getChildByName("main_bg_1"))->setContentSize(Size(520.f, VisibleRect::top().y - 140.f));
	auto score = static_cast<ui::ScrollView*>(layer->getChildByName("scroll"));
	score->setPositionY(VisibleRect::top().y - 960.f);

	//score->getChildByName("panel_all_content")->setPositionY(VisibleRect::top().y - 960.f + 300.f);

	static_cast<Button*>(layer->getChildByName("bt_close"))->setPositionY(VisibleRect::top().y - 100.f);
	
	layer->getChildByName("title")->setPositionY(VisibleRect::top().y - 85.f);
	layer->getChildByName("titlename")->setPositionY(VisibleRect::top().y - 65.f);

	static_cast<Button*>(layer->getChildByName("bt_close"))->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			layer->removeFromParent();
		}
	});
}

StoryInfoLayer::StoryInfoLayer()
{
}

StoryInfoLayer::~StoryInfoLayer()
{
}

bool StoryInfoLayer::init()
{
	_curNewText = nullptr;
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	
	_root = CSLoader::createNode("csb/story_layer.csb");
	this->addChild(_root);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	_root->setPositionY(-VisibleRect::top().y + 100.f);
	_root->runAction(actionshow);

	float off = VisibleRect::top().y - 960.f;

	auto bg = static_cast<ImageView*>(_root->getChildByName("main_bg"));
	bg->setContentSize(Size(640.f, VisibleRect::top().y));

	auto title = bg->getChildByName("title_bg");
	title->setPositionY(title->getPositionY() + off);

	auto quxiao =static_cast<Button*>( bg->getChildByName("bt_close"));
	quxiao->setPositionY(quxiao->getPositionY() + off);
	quxiao->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			MainLayer::getCurMainLayer()->enterMainLayer();
			this->removeFromParent();
		}
	});
	auto textbg = static_cast<ImageView*>(bg->getChildByName("main_sub_bg_2"));
	textbg->setContentSize(Size(580.f, 800.f + off));
	textbg->setPositionY(textbg->getPositionY() + off / 2);

	auto scroll = static_cast<ui::ScrollView*>(textbg->getChildByName("scroll"));
	scroll->setContentSize(Size(512.f, 693.f + off));
	//scroll->setInnerContainerSize(Size());
	static_cast<Text*>(scroll->getChildByName("Text_1"))->setFontSize(21);
	
	for (int i = 0; i < 7; i++)
	{
		_textNode[i] = scroll->getChildByName(String::createWithFormat("item_%d", i)->getCString());
		_textNode[i]->setVisible(false);

		static_cast<Text*>(_textNode[i]->getChildByName("content_0"))->setFontSize(21);
		static_cast<Text*>(_textNode[i]->getChildByName("content_1"))->setFontSize(21);
	}

	int nodetextnum[] = { 2,2, 2, 1,1,2,1 };
	int storyidxnum[] = { 1,4,8,10,13,16,18,21,24,26,30};

	int fileid = StoryMgr::getInstance()->getCurStoryConfig().fileid;

	int churtextidx = 10;
	for (int i = 0; i < 11; i++)
	{
		if (fileid <= storyidxnum[i])
		{
			churtextidx = i - 1;
			break;
		}
	}

	if (churtextidx != -1)
	{
		int totlanum = 0;
		int truestoryidx = 0;
		int offidx = 0;
		for (auto temp : nodetextnum)
		{
			totlanum = totlanum + temp;
			if (totlanum > churtextidx)
			{
				offidx = churtextidx - totlanum + temp;
				break;
			}
			truestoryidx++;
		}

		for (int i = 0; i < 7; i++)
		{
			if (i == truestoryidx)
			{
				_textNode[i]->setVisible(true);
				if (offidx == 1)
				{
					_textNode[i]->getChildByName("content_1")->setVisible(true);
					_curNewText = static_cast<Text*>(_textNode[i]->getChildByName("content_1"));
				}
				else
				{
					_curNewText = static_cast<Text*>(_textNode[i]->getChildByName("content_0"));
					_textNode[i]->getChildByName("content_1")->setVisible(false);
				}
			}
			else if (i < truestoryidx)
			{
				_textNode[i]->setVisible(true);
			}
			else
			{
				_textNode[i]->setVisible(false);
			}
		}

		float percent = 1.f - _textNode[truestoryidx]->getPositionY() / (scroll->getInnerContainerSize().height - scroll->getContentSize().height/2.f);

		scroll->jumpToPercentVertical(percent * 100);
		if (_curNewText && churtextidx != UserData::getInstance()->getShowTextIdx())
		{
			_curNewText->setOpacity(0);
			_curNewText->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f) ,FadeIn::create(1.5f)));
			UserData::getInstance()->setShowTextIdx(churtextidx);
			MainLayer::getCurMainLayer()->getLeftMenus()->getChildByName("hongdianleft")->setVisible(false);
		}
	}
	return true;
}

int StoryInfoLayer::getCurShowTextIdx()
{
	int storyidxnum[] = { 1,4,8,10,13,16,18,21,24,26,30 };

	int fileid = StoryMgr::getInstance()->getCurStoryConfig().fileid;

	int churtextidx = 0;
	for (int i = 0; i < 11; i++)
	{
		if (fileid <= storyidxnum[i])
		{
			churtextidx = i - 1;
			return churtextidx;
		}
	}

	return -1;
}
