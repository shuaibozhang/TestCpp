#include "GuideLayer.h"
#include "VisibleRect.h"
#include "JRCommon.h"
#include "EleIcon.h"
#include "GuideLayer.h"
#include "cocostudio/CocoStudio.h"
#include "Armtr/GameArmtr.h"
#include "ParamMgr.h"
#include "Player.h"
#include "Monster.h"
#include "CrushUtil.h"
#include "ParamData.h"
#include "GameCSLoader.h"
#include "Scenes/GameMap.h"
#include "GameLayer.h"
#include "Story/StoryMgr.h"
#include "Scenes/GameMap.h"
#include "UserData.h"
#include "GameUtils.h"
#include "CrushFSM/CrushFSM.h"
#include "Role.h"
#include "FightLayer.h"
#include "CrushLayer.h"

USING_NS_CC;
using namespace cocostudio;

static const GuideDetailInfo_T s_arrDetailInfo[] = {
	//1
	{ 0, 0, 0, 0, 0.f, 0.f, 0.f },
	/*2.hp*/
	{ 0, 1, 0, 0, 0.f, 0.f, 0.f },
	{ 5, 0, 0, 0, 1.f, 105.f, 50.f },
	/*3.dp*/
	{ 0, 2, 0, 0, 0.f, 0.f, 0.f },
	{ 5, 0, 0, 0, 1.f, 120.f, 20.f },
	/*4.wave*/
	{ 0, 3, 0, 0, 0.f, 0.f, 0.f },
	{ 5, 1, 0, 0, 1.5f, 480.f, -40.f },
	/*5.round*/
	{ 0, 4, 0, 0, 0.f, 0.f, 0.f },
	{ 5, 0, 0, 0, 1.f, 485.f, 82.f },
	/*6.dialog*/
	{ 0, 5, 0, 0, 0.f, 0.f, 0.f },
	/*7.crush ele*/
	{ 0, 6, 0, 0, 0.f, 0.f, 0.f },
	{ 5, 2, 4, 4, 2.f, 0.f, 0.f },
	{ 3, 3, 3, 2, 0.f, 0.f, 0.f },
	{ 3, 2, 3, 3, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 5, 0.f, 0.f, 0.f },
	/*8*/
	{ 0, 7, 0, 0, 0.f, 0.f, 0.f },
	{ 3, 0, 4, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 6, 0.f, 0.f, 0.f },
	/*9.swap sword*/
	{ 1, 8, 0, 0, 0.f, 0.f, 0.f },
	{ 4, 1, 3, 4, 0.f, 0.f, 0.f },	
	/*10.swap drat*/
	{ 1, 9, 2, 0, 0.f, 0.f, 0.f },
	{ 3, 2, 4, 4, 0.f, 0.f, 0.f },
	{ 3, 2, 2, 4, 0.f, 0.f, 0.f },
	{ 4, 0, 3, 4, 0.f, 0.f, 0.f },
	/*11.swap armor*/
	{ 1, 10, 1, 0, 0.f, 0.f, 0.f },
	{ 3, 1, 4, 2, 0.f, 0.f, 0.f },
	{ 3, 0, 4, 3, 0.f, 0.f, 0.f },
	{ 3, 1, 4, 4, 0.f, 0.f, 0.f },
	{ 4, 1, 3, 3, 0.f, 0.f, 0.f },
	/*12.monster att*/
	{ 2, 11, 0, 0, 0.f, 0.f, 0.f },
//	{ 1, 12, 1, 0, 0.f, 0.f, 0.f },
	/*13.swap armor,add dp*/
	{ 1, 13, 1, 0, 0.f, 0.f, 0.f },
	{ 3, 1, 4, 3, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 1, 2, 3, 0.f, 0.f, 0.f },
	{ 4, 0, 3, 4, 0.f, 0.f, 0.f },
	/*14.swap heart, add hp*/
	{ 1, 14, 3, 0, 0.f, 0.f, 0.f },
	{ 3, 3, 4, 4, 0.f, 0.f, 0.f },
	{ 3, 3, 3, 3, 0.f, 0.f, 0.f },
	{ 4, 1, 3, 4, 0.f, 0.f, 0.f },
	/*15.dialog*/
	{ 0, 15, 0, 0, 0.f, 0.f, 0.f },
	/*16.dialog*/
	{ 0, 16, 0, 0, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 2, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 3, 0.f, 0.f, 0.f },
	{ 3, 1, 2, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 5, 0.f, 0.f, 0.f },
	/*17.swap sword,four sword*/
	{ 1, 17, 0, 0, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*18.swap sword, skill*/
	{ 1, 18, 0, 0, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 3, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 5, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*19.swap armor,four armor*/
	{ 1, 19, 1, 0, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 2, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 3, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 5, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*20.swap armor,four armor*/
	{ 1, 20, 1, 0, 0.f, 0.f, 0.f },
	{ 3, 1, 2, 2, 0.f, 0.f, 0.f },
	{ 3, 1, 2, 3, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 4, 0.f, 0.f, 0.f },
	{ 3, 1, 2, 5, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*21.swap armor, armor att*/
	{ 1, 21, 1, 0, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 2, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 3, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 1, 3, 5, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*22.dialog dart, L*/
	{ 0, 22, 2, 0, 0.f, 0.f, 0.f },
	{ 3, 2, 2, 3, 0.f, 0.f, 0.f },
	{ 3, 2, 2, 5, 0.f, 0.f, 0.f },
	{ 3, 2, 2, 6, 0.f, 0.f, 0.f },
	{ 3, 2, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 2, 4, 4, 0.f, 0.f, 0.f },
	/*23.swap dart, L*/
	{ 1, 23, 2, 0, 0.f, 0.f, 0.f },
	{ 4, 0, 2, 4, 0.f, 0.f, 0.f },
	/*24.swap dart, skill*/
	{ 1, 24, 2, 0, 0.f, 0.f, 0.f },
	{ 3, 2, 3, 3, 0.f, 0.f, 0.f },
	{ 3, 2, 1, 3, 0.f, 0.f, 0.f },
	{ 4, 0, 2, 4, 0.f, 0.f, 0.f },
	/*25.swap heart, T*/
	{ 1, 25, 3, 0, 0.f, 0.f, 0.f },
	{ 3, 3, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 3, 2, 3, 0.f, 0.f, 0.f },
	{ 3, 3, 2, 5, 0.f, 0.f, 0.f },
	{ 3, 3, 2, 6, 0.f, 0.f, 0.f },
	{ 3, 3, 1, 4, 0.f, 0.f, 0.f },
	{ 4, 0, 2, 4, 0.f, 0.f, 0.f },
	/*26.swap heart, skill*/
	{ 1, 26, 3, 0, 0.f, 0.f, 0.f },
	{ 3, 3, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 3, 2, 5, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 4, 0.f, 0.f, 0.f },
	{ 4, 0, 2, 5, 0.f, 0.f, 0.f },
	/*27.dialog */
	{ 0, 27, 0, 0, 0.f, 0.f, 0.f },
	{ 3, 0, 3, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 2, 0.f, 0.f, 0.f },
	{ 3, 2, 2, 4, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 6, 0.f, 0.f, 0.f },
	/*28.swap sword, super*/
	{ 1, 28, 0, 0, 0.f, 0.f, 0.f },
	{ 4, 1, 2, 4, 0.f, 0.f, 0.f },
	/*29.dialog*/
	{ 0, 29, 0, 0, 0.f, 0.f, 0.f },
	{ 3, 0, 2, 3, 0.f, 0.f, 0.f },
	/*30.dialog*/
	{ 1, 30, 0, 0, 0.f, 0.f, 0.f },
	{ 4, 0, 2, 4, 0.f, 0.f, 0.f },
	/*31 dialog*/
	{ 0, 31, 0, 0, 0.f, 0.f, 0.f },
};

static const int s_arrTriggerTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 };
static const int s_arrFinishTime[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,  2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0};

static const int s_arrDetailCount[] = {1, 2, 2, 2, 2, 1, 6, 3, 2, 4, 5, 1, 5, 4, 1, 5, 2, 5, 6, 6, 6, 6, 2, 4, 7, 5, 5, 2, 2, 2, 1};
static const int s_guideStep = sizeof(s_arrDetailCount) / sizeof(int);

GuideLayer *GuideLayer::s_pInstance = nullptr;

GuideLayer *GuideLayer::getInstance()
{
	return s_pInstance;
}


GuideLayer::GuideLayer()
	:_isGuiding(false),
	_isTouchAvail(false),
	_curGuideIndex(0)
{
	s_pInstance = this;
}


GuideLayer::~GuideLayer()
{
	s_pInstance = nullptr;

	for (int i = 0; i < ParamData::ROLE_COUNT; i++)
	{
		if (nullptr != Player::getInstance())
		{
			auto pRole = Player::getInstance()->getRole(i);
			pRole->stopTalk();
		}
	}

	_eventDispatcher->removeEventListenersForTarget(this);
}


bool GuideLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/guideUi.plist");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/jiantou.ExportJson");

#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
	int detailtotal = 0;
	for (int i = 0; i < s_guideStep; i++)
	{
		detailtotal += s_arrDetailCount[i];
	}
	int detailtotal2 = sizeof(s_arrDetailInfo) / sizeof(s_arrDetailInfo[0]);
	CCASSERT(detailtotal == detailtotal2, "detailtotal != detailtotal2 GuideLayer");
#endif

	int detailIndex = 0;
	for (int i = 0; i < s_guideStep; i++)
	{
		GuideStepInf_T tmp;

		_arrStepInfo.push_back(tmp);

		auto &info = _arrStepInfo.at(i);
		info.triggerTime = s_arrTriggerTime[i];
		info.finishTime = s_arrFinishTime[i];

		for (int j = 0; j < s_arrDetailCount[i]; j++)
		{
			info.arrDetailInfo.push_back(&s_arrDetailInfo[detailIndex]);
			detailIndex++;
		}
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GuideLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GuideLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GuideLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GuideLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}


void GuideLayer::startGuide(int index)
{
	if (index < _arrStepInfo.size())
	{
		auto &guideStepInfo = _arrStepInfo.at(index);

		for (int i = 0; i < ParamData::CRUSH_ROW; i++)
		{
			for (int j = 0; j < ParamData::CRUSH_COL; j++)
			{
				auto pEle = CrushLayer::getInstance()->getEleIcon(i, j);
				pEle->setIsCanTouch(false);
			}
		}

		for (int i = 0; i < guideStepInfo.arrDetailInfo.size(); i++)
		{
			auto pDetailInfo = guideStepInfo.arrDetailInfo.at(i);

			switch (pDetailInfo->infoType)
			{
			case 0:
				//0.dialog 
			{
				auto pBg = ui::Scale9Sprite::createWithSpriteFrameName("guide_dialog_bg.png");
				this->addChild(pBg, GuideZOrder_E::Z_DIALOG_BG, ParamData::GUIDE_NODE_TAG);
				pBg->setPosition(Vec2(320.f, 120.f));
				pBg->setContentSize(Size(638.f, 236.f));

				auto talkIdx = __String::createWithFormat("guide_dialog_%d", pDetailInfo->param1);
				auto strText = ResMgr::getInstance()->getString(talkIdx->getCString());

				auto pLbl = Label::createWithSystemFont(strText->getCString(), "fonts/Marker Felt.ttf", 24, Size(339.f, 172.f));
				pLbl->setAlignment(TextHAlignment::LEFT);
				this->addChild(pLbl, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);
				pLbl->setTextColor(Color4B(0xff, 0xfe, 0xa0, 0xff));
				pLbl->setPosition(443.f, 120.f);

				auto pRole = Sprite::createWithSpriteFrameName("guide_role.png");
				pRole->setPosition(120.f, 140.f);
				this->addChild(pRole, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);

				auto pSkipIcon = Armature::create("jiantou");
				pSkipIcon->getAnimation()->play("jiantou");
				this->addChild(pSkipIcon, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);
				pSkipIcon->setPosition(Vec2(583.f, 63.f));
				/*
				auto skipNode = Node::create();
				auto bg = Sprite::createWithSpriteFrameName("com_skip_1.png");
				auto skip = Sprite::createWithSpriteFrameName("com_skip_0.png");

				auto action0 = FadeOut::create(1.f);
				auto action1 = FadeIn::create(1.f);
				auto action2 = MoveBy::create(0.5, Vec2(0.f, -20.f));
				auto action3 = MoveBy::create(0.5, Vec2(0.f, 20.f));
				auto fade = RepeatForever::create(Sequence::createWithTwoActions(action0, action1));
				auto move = RepeatForever::create(Sequence::createWithTwoActions(action2, action3));
				skip->runAction(move);
				skip->runAction(fade);

				skipNode->addChild(bg);
				skipNode->addChild(skip);
				this->addChild(skipNode, GuideZOrder_E::Z_TEXT, ParamData::GUIDE_NODE_TAG);
				skipNode->setPosition(Vec2(583.f, 63.f));*/
			}
			break;
			case 1:
				//1.role dialog 
			{
				auto pRole = Player::getInstance()->getRole(pDetailInfo->param2);

				auto talkIdx = __String::createWithFormat("guide_dialog_%d", pDetailInfo->param1);
				pRole->startTalk(talkIdx->getCString());
			}
			break;
			case 2:
				//2.monster dialog 
			{
				auto talkIdx = __String::createWithFormat("guide_dialog_%d", pDetailInfo->param1);
				auto arrCurMonsters = FightLayer::getInstance()->getCurArrMonster();
				auto pMonster = arrCurMonsters->at(0);
				pMonster->startTalk(talkIdx->getCString());
			}
			break;
			case 3:
				//3.create icon 
			{
				auto pEle = CrushLayer::getInstance()->getEleIcon(pDetailInfo->param2+1, pDetailInfo->param3);
				pEle->changeEle(pDetailInfo->param1, 0);
				pEle->setIsCanTouch(false);
			}
			break;
			case 4:
				//4.swap tip 
			{
				auto pTips = Sprite::createWithSpriteFrameName("swap_arrow.png");
				int rowIdx = pDetailInfo->param2 + 1;
				int colIdx = pDetailInfo->param3;
				Vec2 elePos = CrushUtil::getElePos(rowIdx, colIdx);
				Vec2 tipsPos = Vec2::ZERO;

				auto pEle = CrushLayer::getInstance()->getEleIcon(rowIdx, colIdx);
				pEle->setIsCanTouch(true);

				if (0 == pDetailInfo->param1)
				{
					tipsPos = elePos + Vec2(-ParamData::ELE_BG_WIDTH / 2, 0.f);
					pTips->setPosition(tipsPos);

					pEle = CrushLayer::getInstance()->getEleIcon(rowIdx, colIdx-1);
					pEle->setIsCanTouch(true);
				}
				else if (1 == pDetailInfo->param1)
				{
					tipsPos = elePos + Vec2(0.f, ParamData::ELE_BG_HEIGHT / 2);
					pTips->setRotation(90.f);
					pTips->setPosition(tipsPos);

					pEle = CrushLayer::getInstance()->getEleIcon(rowIdx+1, colIdx);
					pEle->setIsCanTouch(true);
				}
				pTips->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3f, 1.2f), ScaleTo::create(0.3f, 1.f), nullptr)));
				this->addChild(pTips, GuideZOrder_E::Z_TIPS_ARR, ParamData::GUIDE_NODE_TAG);
			}

			break;
			case 5:
				//5.highlight
			{
				auto pClippingNode = ClippingNode::create();
				this->addChild(pClippingNode, GuideZOrder_E::Z_BLACK_BG, ParamData::GUIDE_NODE_TAG);


				auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 146));
				pClippingNode->addChild(bgLayer, GuideZOrder_E::Z_BLACK_BG, ParamData::GUIDE_NODE_TAG);

				auto pStencil = Sprite::createWithSpriteFrameName("guide_stencil.png");
				pStencil->setScale(pDetailInfo->fParam4);
				pClippingNode->setStencil(pStencil); 
				pClippingNode->setInverted(true);
				pClippingNode->setAlphaThreshold(1.0f);

				auto pStencil2 = Sprite::createWithSpriteFrameName("guide_stencil.png");
				pStencil2->setOpacity(146.f);
				this->addChild(pStencil2, GuideZOrder_E::Z_STENCIL, ParamData::GUIDE_NODE_TAG);


				if (0 == pDetailInfo->param1)
				{
					Vec2 tmp = Vec2(pDetailInfo->fParam5, pDetailInfo->fParam6 + ParamData::CRUSH_BG_HEIGHT);
					pStencil->setPosition(tmp);
					pStencil2->setPosition(tmp + Vec2(0.f, 0.3f));
				}
				else if (1 == pDetailInfo->param1)
				{
					Vec2 tmp = Vec2(pDetailInfo->fParam5, VisibleRect::top().y + pDetailInfo->fParam6);
					pStencil->setPosition(tmp);
					pStencil2->setPosition(tmp + Vec2(0.f, -0.1f));
				}
				else if (2 == pDetailInfo->param1)
				{
					Vec2 pos = CrushUtil::getElePos(pDetailInfo->param2, pDetailInfo->param3) - Vec2(ParamData::ELE_BG_WIDTH/2.f, 0.f);
					pStencil->setPosition(pos);
					pStencil2->setPosition(pos + Vec2(0.f, 0.2f));
				}
				pStencil2->setScale(pDetailInfo->fParam4);
			}
			break;
			}
		}
	}
}


void GuideLayer::finishGuide(int index)
{
	if (index < _arrStepInfo.size())
	{
		auto &guideStepInfo = _arrStepInfo.at(index);

		for (int i = 0; i < guideStepInfo.arrDetailInfo.size(); i++)
		{
			auto pDetailInfo = guideStepInfo.arrDetailInfo.at(i);

			switch (pDetailInfo->infoType)
			{
			case 0:
				//0.dialog 
			{
				this->removeAllChildrenByTag(ParamData::GUIDE_NODE_TAG);
			}
			break;
			case 1:
				//1.role dialog 
			{
				auto pRole = Player::getInstance()->getRole(pDetailInfo->param2);
				pRole->stopTalk();
			}
			break;
			case 2:
				//2.monster dialog 
			{
				auto arrCurMonsters = FightLayer::getInstance()->getCurArrMonster();
				auto pMonster = arrCurMonsters->at(0);
				pMonster->stopTalk();
			}
			break;
			case 3:
				//3.create icon 
			{
				//do nothing
			}
			break;
			case 4:
				//4.swap tip 
			{
				this->removeAllChildrenByTag(ParamData::GUIDE_NODE_TAG);
			}
			break;
			case 5:
				//5.highlight
			{
				this->removeAllChildrenByTag(ParamData::GUIDE_NODE_TAG);
			}
			break;
			}
		}
	}
}


void GuideLayer::onTriggerEvent(int eventId)
{
	if (!_isGuiding && _curGuideIndex < _arrStepInfo.size())
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (guideStepInfo.triggerTime == eventId)
		{
			this->startGuide(_curGuideIndex);
			_isGuiding = true;
		}
	}
}


void GuideLayer::onFinishEvent(int eventId)
{
	if (_isGuiding && _curGuideIndex < _arrStepInfo.size())
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (guideStepInfo.finishTime == eventId)
		{
			this->finishGuide(_curGuideIndex);
			_curGuideIndex++;
			_isGuiding = false;

			if (_curGuideIndex == _arrStepInfo.size())
			{
				this->finishAllGuide();
			}
			else
			{
				this->onTriggerEvent(0);
			}
		}
	}
}


void GuideLayer::finishAllGuide()
{
	UserData::getInstance()->setIsNeedGuide(false);
	GameLayer::getInstance()->setIsGuiding(false);

	int arrNewEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = { EleIconId_E::ELE_ID_EMPTY };
	memset((void *)arrNewEleId, EleIconId_E::ELE_ID_EMPTY, sizeof(arrNewEleId));

	int arrNewEleState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL] = { 0 };
	memset((void *)arrNewEleState, 0, sizeof(arrNewEleState));

	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			auto pEle = CrushLayer::getInstance()->getEleIcon(i, j);
			pEle->setIsCanTouch(true);
			if (pEle->getEleId() != (EleIconId_E::ELE_ID_SUPER + 10))
			{
				arrNewEleId[i][j] = pEle->getEleId();
			}
		}
	}

	CrushLayer::getInstance()->createNewEleId(arrNewEleId, arrNewEleState, 100, 0, 0);
	for (int i = 0; i < ParamData::CRUSH_ROW; i++)
	{
		for (int j = 0; j < ParamData::CRUSH_COL; j++)
		{
			auto pEle = CrushLayer::getInstance()->getEleIcon(i, j);
			if (pEle->getEleId() == (EleIconId_E::ELE_ID_SUPER + 10))
			{
				pEle->changeEle(arrNewEleId[i][j], 0);
			}
		}
	}

	s_pInstance = nullptr;
	this->removeFromParent();
}


bool GuideLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (_isGuiding)
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (0 == guideStepInfo.finishTime)
		{
			//touch
			Vec2 touchPos = touch->getLocation();
			Rect touchArea(0.f, 0.f, 640.f, 240.f);

			if (touchArea.containsPoint(touchPos))
			{
				_isTouchAvail = true;
			}			
		}
		else if (1 == guideStepInfo.finishTime)
		{
			//crush
			CrushLayer::getInstance()->onTouchBegan(touch, unused_event);
		}
	}

	return true;
}


void GuideLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (_isGuiding)
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (0 == guideStepInfo.finishTime)
		{
			//touch
		}
		else if (1 == guideStepInfo.finishTime)
		{
			//crush
			CrushLayer::getInstance()->onTouchMoved(touch, unused_event);
		}
	}
}


void GuideLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (_isGuiding)
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (0 == guideStepInfo.finishTime)
		{
			//touch
			if (_isTouchAvail)
			{
				Vec2 touchPos = touch->getLocation();
				Rect touchArea(0.f, 0.f, 640.f, 240.f);

				if (touchArea.containsPoint(touchPos))
				{
					this->onFinishEvent(0);
				}
			}
		}
		else if (1 == guideStepInfo.finishTime)
		{
			//crush
			CrushLayer::getInstance()->onTouchEnded(touch, unused_event);
		}
	}
}


void GuideLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (_isGuiding)
	{
		auto &guideStepInfo = _arrStepInfo.at(_curGuideIndex);

		if (0 == guideStepInfo.finishTime)
		{
			//touch
			_isTouchAvail = false;
		}
		else if (1 == guideStepInfo.finishTime)
		{
			//crush
			CrushLayer::getInstance()->onTouchCancelled(touch, unused_event);
		}
	}
}


void GuideLayer::removeAllChildrenByTag(int tag)
{
	while (nullptr != this->getChildByTag(tag))
	{
		this->removeChildByTag(tag);
	}
}