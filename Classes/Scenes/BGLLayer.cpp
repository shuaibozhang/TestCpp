#include "BGLLayer.h"
#include "GLCommon/UI/GLPageView.h"
#include "DataParam/UserData.h"
#include "DataParam/ParamMgr.h"
#include "GLCommon/Utils/ToolsUtil.h"
#include "PopRewardLayer.h"
#include "VisibleRect.h"
#include "GameUtils.h"
#include "MainScene.h"
#include "DayActiveMgr.h"
#include "NewMapOpenMgr.h"

extern const char* g_iconbgname[];

const float g_totletime = 0.5f;
const float g_moveoff = 100.f;

const float g_offdown = 10.f;

const float g_bglExpPoint[] = {0, 500, 4000, 50000 , 100000};

struct BGLWenponInfo_T
{
	float getexp;
	float needexpfactor;
};

BGLWenponInfo_T g_BglWenponInfo[10] = {
	{4.f, 2.3f },
	{7.f, 2.4f },
	{ 8.f, 2.5f },
	{ 25.f, 2.5f },
	{ 40.f, 2.5f },
	{ 55.f, 3.f },
	{ 200.f, 3.f },
	{ 300.f, 3.f },
	{ 400.f, 3.5f },
	{ 600.f, 4.f }	
};

struct BGLUpInfo_T
{
	float factorchange;
	float costgold;
};

BGLUpInfo_T g_BglUpInfo[10] = {
	{ -0.1f, 500.f },
	{ -0.1f, 1000.f },
	{ -0.1f, 2000.f },
	{ -0.1f, 3000.f },
	{ -0.1f, 5000.f },
	{ -0.1f, 6000.f },
	{ -0.1f, 7000.f },
	{ -0.1f, 8000.f },
	{ -0.1f, 9000.f },
	{ -0.1f, 10000.f },
};

const int g_BGLoutPartsNeedExp[] = {
	10,
	100,
	200,
	500,
	1000,
	2000,
	4000,
	10000,
	20000,
	50000
};

struct BGLExReward_T
{
	int itemid;
	int minnum;
	int maxnum;
};

const BGLExReward_T g_arrBGLExReward[] = {
	{1000, 1,1},
	{1001,1,1},
	{1008,1,1},
	{ 1009,1,3 },
	{ 1002,1,3 },
	{ 1003,1,3 },
	{ 1010,1,3 },
	{ 1004,1,3 },
	{ 1011,1,3 },
	{ 1012,1,3 },
};

BGLLayer::~BGLLayer()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/bglui.plist"); 
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("effect/bagualu.ExportJson");
}

bool BGLLayer::init()
{
	_selecIcon = nullptr;

	auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layer);

	auto roonode = Node::create();
	this->addChild(roonode);

	auto actionshow = EaseSineOut::create(MoveTo::create(0.3f, Vec2(0.f, 0.f)));
	roonode->setPositionY(-VisibleRect::top().y + 100);
	roonode->runAction(actionshow);

	auto listenerlayer = EventListenerTouchOneByOne::create();
	listenerlayer->setSwallowTouches(true);
	listenerlayer->onTouchBegan = [this](Touch* touch, Event*) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerlayer, layer);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/bglui.plist");

	_pressdur = 0;
	_ispressed = false;
	_pressIdx = -1;
	_isDelPress = false;
	_curActionType = ACTION_UNKNOW;
	_curpageidx = 0;
	_isZhuruPress = false;
	_totalExp = 0.f;
	_isPlayAni = false;

	auto btncancle = Button::create("wup_close_0.png", "wup_close_1.png", "", Widget::TextureResType::PLIST);
	roonode->addChild(btncancle);
	btncancle->setPosition(Vec2(600.f, VisibleRect::top().y - 50.f));
	btncancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_isPlayAni)
			{
				return;
			}
			this->removeFromParent();
			for (int i = 0; i < 4; i++)
			{
				MainLayer::getCurMainLayer()->updataEquipLayer(i);
			}
		}
	});

	auto lvbg = Sprite::createWithSpriteFrameName("baguan_lvbg.png");
	roonode->addChild(lvbg);
	lvbg->setPosition(Vec2(320.f, VisibleRect::top().y - 50.f));

	auto iconlv = Sprite::createWithSpriteFrameName("baguan_text_lv.png");
	roonode->addChild(iconlv);
	iconlv->setPosition(lvbg->getPosition() + Vec2(-40.f, 0.f));

	_bgllv = ui::TextAtlas::create("0", "fonts/num_bgllv.png", 19.f, 28.f, ".");
	roonode->addChild(_bgllv);
	_bgllv->setPosition(lvbg->getPosition() + Vec2(40.f, 0.f));

	auto mainicon = Sprite::createWithSpriteFrameName("baguan_mainicon.png");
	roonode->addChild(mainicon);
	mainicon->setPosition(Vec2(335.f, 430.f));

	auto pagebg = Sprite::createWithSpriteFrameName("baguan_mainbg.png");
	roonode->addChild(pagebg);
	pagebg->setPosition(Vec2(320.f, 177.f));

	_upNode = Node::create();
	roonode->addChild(_upNode);

	auto goldbg = Sprite::createWithSpriteFrameName("baguan_goldbg.png");
	_upNode->addChild(goldbg);
	goldbg->setPosition(Vec2(320.f, 400.f - 15.f));

	auto goldicon = Sprite::createWithSpriteFrameName("com_goldbag.png");
	_upNode->addChild(goldicon);
	goldicon->setPosition(goldbg->getPosition() + Vec2(65.f, 0.f));
	goldicon->setScale(0.8f);

	_costGoldNum = ui::TextAtlas::create("0", "fonts/font_playerlv.png", 20.f, 28.f, ".");
	_upNode->addChild(_costGoldNum);
	_costGoldNum->setPosition(goldbg->getPosition() + Vec2(0.f, 0.f));
	_costGoldNum->setScale(0.8f);

	auto upbtn = Button::create("baguan_btn_up_0.png", "baguan_btn_up_1.png", "baguan_btn_up_2.png", Widget::TextureResType::PLIST);
	_upNode->addChild(upbtn);
	upbtn->setPosition(Vec2(320.f, 450.f - 15.f));
	upbtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			int curlv = UserData::getInstance()->getBGLLv();
			int cost = g_BglUpInfo[curlv].costgold;
			if (UserData::getInstance()->getGoldNum() >= cost)
			{
				_mainArm->getAnimation()->play("bagua_shengji");
				UserData::getInstance()->setBGLLv(curlv + 1);
				UserData::getInstance()->giveGold(-cost);
				UserData::getInstance()->saveUserData();

				updateLvInfo();
			}
			else
			{
				GameUtils::toastTip("no_gold");
			}
			
		}
	});

	auto textpng = Sprite::createWithSpriteFrameName("bgguan_text_sjrl.png");
	_upNode->addChild(textpng);
	textpng->setPosition(upbtn->getPosition() + Vec2(0.f, 3.f));

	auto desbtn = Button::create("baguan_btn_shuoming.png", "", "", Widget::TextureResType::PLIST);
	roonode->addChild(desbtn);
	desbtn->setPosition(Vec2(320.f, 450.f - 15.f) + Vec2(150.f, 100.f));
	desbtn->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			showDes();
		}
	});

	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/bagualu.ExportJson");
	_mainArm = Armature::create("bagualu");
	_mainArm->getAnimation()->play("bagua_daiji");
	roonode->addChild(_mainArm);
	_mainArm->setPosition(Vec2(330.f, 720.f));
	_mainArm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(BGLLayer::armCallBack, this));

	auto updown = Sequence::createWithTwoActions(EaseSineOut::create(MoveBy::create(1.f, Vec2(0.f, 20.f))), EaseSineIn::create(MoveBy::create(1.f, Vec2(0.f, -20.f))));
	_mainArm->runAction(RepeatForever::create(updown));

	auto tempsprite = Sprite::createWithSpriteFrameName("bagua/jindu.png");
	_lhProgress = ProgressTimer::create(tempsprite);
	_lhProgress->setPercentage(0);
	_lhProgress->setType(ProgressTimer::Type::RADIAL);
	_lhProgress->setRotation(180.f);
	
	_mainArm->getBone("Layer1")->getDisplayManager()->addDisplay(_lhProgress, 0);

	_touchRect.size = Size(160.f, 160.f);
	_touchRect.origin = Vec2(330.f, 720.f) - Vec2(80.f, 80.f);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto drawnode = DrawNode::create();
	this->addChild(drawnode);
	drawnode->drawRect(_touchRect.origin, Vec2(_touchRect.getMaxX(), _touchRect.getMaxY()), Color4F(1.f, 1.f, 1.f, 1.f));
#endif

	auto pageview = glui::GLPageView::create();
	roonode->addChild(pageview);
	pageview->setContentSize(Size(640.f, 350.f));
	//pageview->setCanScroll(false);
	pageview->setClippingEnabled(true);
	pageview->setCustomScrollThreshold(150.f);

	pageview->addEventListener([=](Ref*, ui::PageView::EventType type) {
		if (type == ui::PageView::EventType::TURNING)
		{
			_curpageidx = pageview->getCurPageIndex();
			for (int idxpage = 0; idxpage < 4; idxpage++)
			{
				if (idxpage == _curpageidx)
				{
					_pagetTips[idxpage]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_1.png"));
				}
				else
				{
					_pagetTips[idxpage]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_0.png"));
				}
				
			}
		}
	});

	for (int i = 0; i < 4; i++)
	{
		auto layout = ui::Layout::create();	
		pageview->addPage(layout);
		_startpos = Vec2(80.f, 220.f + g_offdown);
		_pageLayout[i] = layout;
		/************/		
		/*0 1 2 3 4*/
		/*5 6 7 8 9 */
		/***********/
		for (int j = 0; j < 10; j++)
		{
			auto bgicon = Sprite::createWithSpriteFrameName(g_iconbgname[j]);
			layout->addChild(bgicon, 0);
			bgicon->setPosition(_startpos + Vec2(j % 5 * 120.f, -j / 5 * 120));
			int pratsnum = UserData::getInstance()->getWeaponParts(i * 10 + j);
			
			_iconPosBak[j] = bgicon->getPosition();

			_tempNums[i * 10 + j] = pratsnum;
			_textTempNum[i * 10 + j] = nullptr;
			_delBtns[i * 10 + j] = nullptr;

			if (_selecIcon == nullptr)
			{
				_selecIcon = Sprite::createWithSpriteFrameName("wup_icon_select.png");
				layout->addChild(_selecIcon, 10);
				_selecIcon->setVisible(false);
			}

			_partsInfoNodes[i * 10 + j] = Node::create();
			layout->addChild(_partsInfoNodes[i * 10 + j]);
			auto tempInfonode = _partsInfoNodes[i * 10 + j];
			//if (pratsnum > 0)
			//{
				auto particon = Sprite::createWithSpriteFrameName("com_icon_parts_0.png");
				tempInfonode->addChild(particon, 1);
				particon->setPosition(bgicon->getPosition() + Vec2(-35.f, 35.f));

				auto& wenponinfo = ParamMgr::getInstance()->getWeaponVector();

				auto wenponicon = Sprite::createWithSpriteFrameName(wenponinfo[i * 10 + j].picname);
				tempInfonode->addChild(wenponicon, 2);
				wenponicon->setPosition(bgicon->getPosition());

				auto textnum = ui::TextAtlas::create("0", "fonts/num_next_exp.png", 14.f, 16.f, ".");
				textnum->setString(String::createWithFormat("%d", pratsnum)->getCString());
				//textnum->setFontSize(18);
				textnum->setPosition(bgicon->getPosition() + Vec2(35.f, -35.f));
				tempInfonode->addChild(textnum, 3);
				textnum->setScale(1.2f);

				_textTempNum[i * 10 + j] = textnum;

				auto btndel = Button::create("baguan_icon_del.png", "", "", Widget::TextureResType::PLIST);
				tempInfonode->addChild(btndel, 4);
				btndel->setPosition(bgicon->getPosition() + Vec2(40.f, 40.f));
				btndel->setUserData((void*)(i * 10 + j));
				btndel->addTouchEventListener(CC_CALLBACK_2(BGLLayer::menuOndel, this));
				btndel->setVisible(false);

				_delBtns[i * 10 + j] = btndel;

				if (pratsnum > 0)
				{
					_partsInfoNodes[i * 10 + j]->setVisible(true);
				}
				else
				{
					_partsInfoNodes[i * 10 + j]->setVisible(false);
				}
			//}
		}
		_pagetTips[i] = Sprite::createWithSpriteFrameName("mainui_pagemark_0.png");
		roonode->addChild(_pagetTips[i]);

		_pagetTips[i]->setPosition(Vec2(45.f * (i - 1.5f) + 320.f, 20.f));

		
	}
	_pagetTips[0]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainui_pagemark_1.png"));

	auto touchnode = Node::create();
	roonode->addChild(touchnode, 1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = [this](Touch* touch, Event*) 
	{
		if (_isPlayAni)
		{
			return true;
		}

		_startPressPos = touch->getLocation();
		_pressIdx = checkTouchBtn(touch->getLocation());
		if (_touchRect.containsPoint(_startPressPos))
		{
			_isZhuruPress = true;
			_mainArm->getAnimation()->play("bagua_lianhua2");
			_lhProgress->setPercentage(0);
			_mainArm->getBone("Layer12")->setRotation(0.f);

			int idx = 0;
			for (auto& temp : _tempNums)
			{
				UserData::getInstance()->setWeaponPartsNum(idx, temp);
				if (_delBtns[idx])
				{
					_delBtns[idx]->setVisible(false);
				}
				idx++;
			}

			UserData::getInstance()->saveUserData();
		}
		else if (_pressIdx == -1)
		{
		
		}
		else
		{
			_ispressed = true;
		}
		
		return true; 
	};

	listener->onTouchMoved = [=](Touch* touch, Event*) {
		float dis = _startPressPos.getDistanceSq(touch->getLocation());
		if (dis > g_moveoff && _curActionType == ACTION_UNKNOW)
		{
			_curActionType = ACTION_MOVE;
		}
	};
	
	listener->onTouchEnded = [=](Touch* touch, Event*) {
		if (_curActionType == ACTION_UNKNOW)
		{
			if (_ispressed && _isDelPress == false)
			{
				_curActionType = ACTION_TIP;
				int curpartidx = _curpageidx * 10 + _pressIdx;

				changePartsNum(curpartidx,-1);

				if (_selecIcon)
				{
					_selecIcon->setVisible(true);
					_selecIcon->setPosition(_iconPosBak[_pressIdx]);
					_selecIcon->retain();

					_selecIcon->removeFromParent();
					_pageLayout[_curpageidx]->addChild(_selecIcon, 10);
					_selecIcon->release();
				}
			}
		}

		if (_isZhuruPress)
		{
			_isZhuruPress = false;
			if (_touchRect.containsPoint(touch->getLocation()))
			{
				_mainArm->getAnimation()->play("bagua_lianhua1");
				_isPlayAni = true;
			}
			else
			{
				_mainArm->getAnimation()->play("bagua_daiji");
			}
			
		}

		_pressdur = 0.f;
		_ispressed = false;
		_curActionType = ACTION_UNKNOW;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, touchnode);

	updateLvInfo();

	this->scheduleUpdate();

	if (UserData::getInstance()->getHavePopBgl() == 0)
	{
		UserData::getInstance()->setHavePopBgl(1);
		showDes();
		UserData::getInstance()->saveUserData();
	}

	_btnall = Button::create("baguan_btn_allin_0.png", "baguan_btn_allin_1.png", "baguan_btn_allin_2.png", Widget::TextureResType::PLIST);
	this->addChild(_btnall, 5);
	_btnall->setPosition(Vec2(550, 320));
	_btnall->addTouchEventListener([=](Ref*, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			if (_isPlayAni)
			{
				return;
			}
			int curpartidx = _curpageidx * 10 + _pressIdx;

			if (_tempNums[curpartidx] != 0)
			{
				changeAllParts(_curpageidx * 10 + _pressIdx, 0);
			}
			else
			{
				changeAllParts(_curpageidx * 10 + _pressIdx, 1);
			}			
		}
	});

	_btnall->setEnabled(false);

	return true;
}

void BGLLayer::update(float delta)
{
	static float startlongpresstime = 0.f;
	if (_ispressed && _isDelPress == false)
	{
		_pressdur += delta;
		if (_pressdur > g_totletime && _curActionType == ACTION_UNKNOW)
		{
			_curActionType = ACTION_LONGPRESS;
			startlongpresstime = 0.f;
		}

		if (_curActionType == ACTION_LONGPRESS)
		{
			startlongpresstime += delta;
			int curpartidx = _curpageidx * 10 + _pressIdx;
			int sendnum = 0;
			if (startlongpresstime >= 0.1f)
			{
				changePartsNum(curpartidx, -1);
				startlongpresstime = 0.f;
			}	
		}
	}
	else
	{
		_pressdur = 0.f;
	}
}

int BGLLayer::checkTouchBtn(Vec2 touchpos)
{
	int idx = -1;
	if (touchpos.x < 20 || touchpos.x > 620.f || touchpos.y < 40.f + g_offdown || touchpos.y > (280.f + g_offdown))
	{

	}
	else
	{
		int idxx = (touchpos.x - 20) / 120.f;
		int idxy = (touchpos.y - (40 + g_offdown)) / 120.f;

		idx = idxx + 5 * (1 - idxy);
	}
	return idx;
}

void BGLLayer::handleActions()
{
	
}

void BGLLayer::menuOndel(Ref * btn, Widget::TouchEventType type)
{
	_isDelPress = true;
	if (type == Widget::TouchEventType::ENDED && _isPlayAni == false)
	{
		int idx = int(((Node*)btn)->getUserData());
		changePartsNum(idx, 1);
		_isDelPress = false;

	}
}

void BGLLayer::changePartsNum(int idx, int numchange)
{
	int truechange = 0;
	if (_pressIdx != -1 && _textTempNum[idx])
	{
		int curbak = _tempNums[idx];
		_tempNums[idx] = _tempNums[idx] + numchange > 0 ? _tempNums[idx] + numchange : 0;

		truechange = _tempNums[idx] - curbak;

		_textTempNum[idx]->setString(String::createWithFormat("%d", _tempNums[idx])->getCString());
		
		if (UserData::getInstance()->getWeaponParts(idx) != _tempNums[idx])
		{
			_delBtns[idx]->setVisible(true);
		}
		else
		{
			_delBtns[idx]->setVisible(false);
		}
	}

	if (truechange < 0)
	{
		auto& wenponinfo = ParamMgr::getInstance()->getWeaponVector();
		auto wenponicon = Sprite::createWithSpriteFrameName(wenponinfo[idx].picname);
		this->addChild(wenponicon);
		int j = idx % 10;
		wenponicon->setPosition(_startpos + Vec2(j % 5 * 120.f, -j / 5 * 120));
		
		auto action = Sequence::createWithTwoActions(Spawn::createWithTwoActions(
			EaseSineOut::create(MoveTo::create(0.8f, Vec2(330.f, 720.f))),
			EaseBackIn::create(ScaleTo::create(0.4f, 0.2f))
			),
			RemoveSelf::create());

		wenponicon->runAction(action);
	}

	_totalExp -= (truechange * g_BglWenponInfo[idx % 10].getexp);
	int expidx = 0;
	for (int i = 0; i < sizeof(g_bglExpPoint) / sizeof(g_bglExpPoint[0]); i++)
	{
		expidx = i;
		if (_totalExp < g_bglExpPoint[i])
		{
			break;
		}
		else
		{

		}
	}
	float fenmu = g_bglExpPoint[expidx] - g_bglExpPoint[expidx - 1];

	float present = expidx * 25 - 25 + 25 * ((_totalExp - g_bglExpPoint[expidx - 1]) / fenmu);
	_lhProgress->setPercentage(present);
	_mainArm->getBone("Layer12")->setRotation(present * 3.6f);

	updateAllInBtn();
	CCLOG(String::createWithFormat("curaction: %d", int(_lhProgress->getPercentage() * 100))->getCString());
}

void BGLLayer::changeAllParts(int idx, int type)
{
	int truechange = 0;
	if (_pressIdx != -1 && _textTempNum[idx])
	{
		int curbak = _tempNums[idx];

		if (type == 0)
		{
			_tempNums[idx] = 0;
		}
		else
		{
			_tempNums[idx] = UserData::getInstance()->getWeaponParts(idx);
		}
		

		truechange = _tempNums[idx] - curbak;

		if (truechange < 0)
		{
			auto& wenponinfo = ParamMgr::getInstance()->getWeaponVector();
			int moninum = abs(truechange) > 5 ? 5 : abs(truechange);
			for (int i = 0; i < moninum; i++)
			{
				auto wenponicon = Sprite::createWithSpriteFrameName(wenponinfo[idx].picname);
				this->addChild(wenponicon);
				wenponicon->setVisible(false);

				int j = idx % 10;
				wenponicon->setPosition(_startpos + Vec2(j % 5 * 120.f, -j / 5 * 120));

				auto action = Sequence::create(DelayTime::create(i * 0.1f), Show::create(), Spawn::createWithTwoActions(
					EaseSineOut::create(MoveTo::create(0.8f, Vec2(330.f, 720.f))),
					EaseBackIn::create(ScaleTo::create(0.4f, 0.2f))
					),
					RemoveSelf::create(), nullptr);

				wenponicon->runAction(action);
			}
			
		}

		_textTempNum[idx]->setString(String::createWithFormat("%d", _tempNums[idx])->getCString());

		if (UserData::getInstance()->getWeaponParts(idx) != _tempNums[idx])
		{
			_delBtns[idx]->setVisible(true);
		}
		else
		{
			_delBtns[idx]->setVisible(false);
		}
	}

	_totalExp -= (truechange * g_BglWenponInfo[idx % 10].getexp);
	int expidx = 0;
	for (int i = 0; i < sizeof(g_bglExpPoint) / sizeof(g_bglExpPoint[0]); i++)
	{
		expidx = i;
		if (_totalExp < g_bglExpPoint[i])
		{
			break;
		}
		else
		{

		}
	}
	float fenmu = g_bglExpPoint[expidx] - g_bglExpPoint[expidx - 1];

	float present = expidx * 25 - 25 + 25 * ((_totalExp - g_bglExpPoint[expidx - 1]) / fenmu);
	_lhProgress->setPercentage(present);
	_mainArm->getBone("Layer12")->setRotation(present * 3.6f);

	updateAllInBtn();
}

void BGLLayer::armCallBack(Armature * armature, MovementEventType movementType, const std::string & movementID)
{
	if (movementType == COMPLETE)
	{
		if (movementID.compare("bagua_lianhua1") == 0)
		{
			_mainArm->getAnimation()->play("bagua_daiji");

			if (_totalExp >= 50000)
			{
				NewMapOpenMgr::getInstance()->tryGet(MapOpenType::BGL50000);
			}

			getReward(_totalExp);
			_totalExp = 0.f;
			_isPlayAni = false;
			updateAllInBtn();
		}
		else if (movementID.compare("bagua_shengji") == 0)
		{
			_mainArm->getAnimation()->play("bagua_daiji");
		}
	}
}

void BGLLayer::getReward(float exp)
{
	std::vector<PopItemInfo_T> arrItems;

	int idx = 0;
	int getidx = 0;
	int getnum = 0;
	for (auto& temp : g_BGLoutPartsNeedExp)
	{
		if (exp <= temp)
		{
			break;
		}

		idx++;
	}

	if (idx == 0)
	{

	}
	else
	{
		getidx = idx - 1;
		getnum = exp / (g_BglWenponInfo[getidx].getexp * (g_BglWenponInfo[getidx].needexpfactor - UserData::getInstance()->getBGLLv() * 0.1f));

		PopItemInfo_T temp;
		temp.itemId = g_arrBGLExReward[getidx].itemid;
		temp.itemCount = ToolsUtil::getRandomInt(g_arrBGLExReward[getidx].minnum, g_arrBGLExReward[getidx].maxnum);
		arrItems.push_back(temp);

		CCLOG("get %d: %d", getidx, getnum);
	}

	updateNumsInfo();

	if (getnum <= 0)
	{
		GameUtils::toastTip("bgl_failed");
		return;
	}

	DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYUSEBGL);

	int getitemsnum[4] = { 0 };

	for (int i = 0; i < getnum; i++)
	{
		int idx = ToolsUtil::getRandomInt(0, 3);
		getitemsnum[idx]++;
	}

	for (int i = 0; i < 4; i++)
	{
		if (getitemsnum[i] > 0)
		{
			PopItemInfo_T temp;
			temp.itemId = 1500 + i * 10 + getidx;
			temp.itemCount = getitemsnum[i];

			arrItems.push_back(temp);
		}
	}

	auto poplayer = PopRewardLayer::create(arrItems);
	this->addChild(poplayer, 100);

	updateNumsInfo();
}

void BGLLayer::updateLvInfo()
{
	int lv = UserData::getInstance()->getBGLLv();
	if (lv >= 10)
	{
		_upNode->setVisible(false);
	}
	else
	{
		_costGoldNum->setString(String::createWithFormat("%d", int(g_BglUpInfo[UserData::getInstance()->getBGLLv()].costgold))->getCString());
	}

	_bgllv->setString(String::createWithFormat("%d", lv)->getCString());
	
}

void BGLLayer::updateNumsInfo()
{
	for (int i = 0; i < ParamData::WENPON_COUNT; i++)
	{
		int pratsnum = UserData::getInstance()->getWeaponParts(i);
		_tempNums[i] = pratsnum;
		_textTempNum[i]->setString(String::createWithFormat("%d", _tempNums[i])->getCString());

		if (pratsnum > 0)
		{
			_partsInfoNodes[i]->setVisible(true);
		}
		else
		{
			_partsInfoNodes[i]->setVisible(false);
		}
	}
}

void BGLLayer::showDes()
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
	this->addChild(layer, 1000);

	auto listenerlayer = EventListenerTouchOneByOne::create();
	listenerlayer->setSwallowTouches(true);
	listenerlayer->onTouchBegan = [this](Touch* touch, Event*) {return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerlayer, layer);

	auto root = CSLoader::createNode("csb/bgldes.csb");
	layer->addChild(root);
	root->setPosition(Vec2(320.f, VisibleRect::top().y / 2));

	auto btn = static_cast<Button*>(root->getChildByName("btn_get"));
	btn->addTouchEventListener([=](Ref * btn, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			layer->removeFromParent();
		}
	});
}

void BGLLayer::updateAllInBtn()
{
	if (_pressIdx == -1)
	{
		_btnall->setEnabled(false);
		return;
	}
	int curpartidx = _curpageidx * 10 + _pressIdx;
	if (UserData::getInstance()->getWeaponParts(curpartidx) != 0)
	{
		_btnall->setEnabled(true);
		if (_tempNums[curpartidx] != 0)
		{
			_btnall->loadTextures("baguan_btn_allin_0.png", "baguan_btn_allin_1.png", "baguan_btn_allin_2.png", Widget::TextureResType::PLIST);
		}
		else
		{
			_btnall->loadTextures("baguan_btn_all_0.png", "baguan_btn_all_1.png", "baguan_btn_all_2.png", Widget::TextureResType::PLIST);
		}
	}
	else
	{
		_btnall->setEnabled(false);
	}
}
