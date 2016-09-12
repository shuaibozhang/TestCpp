#include "DailyRewardLayer.h"
#include "GameCSLoader.h"
#include "VisibleRect.h"
#include "ParamMgr.h"
#include "ParamData.h"
#include "UserData.h"
#include "BoxDataMgr.h"
#include "CrushUtil.h"
#include "PopRewardLayer.h"
#include "MainScene.h"
#include "Armtr/GameArmtr.h"
#include "WeaponControl.h"
#include "GLCommon/Utils/ToolsUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;


DailyRewardLayer *DailyRewardLayer::s_pInstance = nullptr;

const int g_arrDailyRewardId[7] = {1014, 1502, 21, 1015, 1515, 1004, 536};
const int g_arrDailyRewardCount[7] = {2, 30, 1, 3, 20, 5, 1};

const int g_arrWeekRepeatRewardId[7] = { 1012, 1503, -102, 1506, 1004, 1014, 1508 };
const int  g_arrWeekRepeatRewardCount[7] = { 2, 10, 20, 10, 5, 5, 10 };

DailyRewardLayer *DailyRewardLayer::getInstance(void)
{
	return s_pInstance;
}

DailyRewardLayer::DailyRewardLayer()
{
	s_pInstance = this;
}


DailyRewardLayer::~DailyRewardLayer()
{
	s_pInstance = nullptr;
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ui/loginui.plist");
}


DailyRewardLayer* DailyRewardLayer::create(bool isWeekReward)
{
	DailyRewardLayer *pRet = new DailyRewardLayer();
	if (pRet && pRet->init(isWeekReward))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}

	return pRet;
}


bool DailyRewardLayer::init(bool isWeekReward)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!CCLayer::init());
		
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/loginui.plist");

		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = CC_CALLBACK_2(DailyRewardLayer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(DailyRewardLayer::onTouchEnded, this);//[=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {this->removeFromParent(); };
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

		
		for (int i = 0; i < 3; i++)
		{
			if (UserData::getInstance()->getWeekWenponPartsIdx(i) == -1)
			{
				int wenponOwner = ToolsUtil::getRandomInt(0, 3);
				UserData::getInstance()->setWeekWenponPartsIdx(i, wenponOwner);
			}
		}
		
		static int arrwWenponOwner[7] = { 0 };
		arrwWenponOwner[1] = UserData::getInstance()->getWeekWenponPartsIdx(0);
		arrwWenponOwner[3] = UserData::getInstance()->getWeekWenponPartsIdx(1);
		arrwWenponOwner[6] = UserData::getInstance()->getWeekWenponPartsIdx(2);
		
		for (int i = 0; i < 7; i++)
		{
			if (!UserData::getInstance()->getIsGotDailyReward(i))
			{
				_curDayIdx = i;
				break;
			}
		}

		if (isWeekReward)
		{
			_curDayIdx = UserData::getInstance()->getWeekRewardIdx();
		}

		float offsetY = (VisibleRect::top().y - 960.f) / 2.f;
		_pRoot = GameCSLoader::createNode("csb/loginui_main_layer.csb");
		this->addChild(_pRoot);
		_pRoot->setPositionY(offsetY);
		
		auto pMain = _pRoot->getChildByName("main_bg");

		if (isWeekReward)
		{
			auto des = static_cast<ImageView*>(pMain->getChildByName("intr_bg")->getChildByName("intr_content"));

			des->loadTexture("loginui_week_title.png", Widget::TextureResType::PLIST);
		}
		for (int i = 0; i < 7; i++)
		{
			auto strItemName = __String::createWithFormat("bt_item_%d", i);
			auto strConverName = __String::createWithFormat("cover_item_%d", i);
			auto pItem = dynamic_cast<Button *>(pMain->getChildByName(strItemName->getCString()));
			auto pConver = pMain->getChildByName(strConverName->getCString());
			auto pTitle = pItem->getChildByName("day_title_bg");

			auto pTextName = static_cast<Text*>(pItem->getChildByName("item_name"));
			auto pCount = static_cast<TextAtlas*>(pItem->getChildByName("item_bg")->getChildByName("count"));

			auto strPicName = CrushUtil::getItemPicName(g_arrDailyRewardId[i]);
			if (isWeekReward)
			{
				std::string name("");

				if (g_arrWeekRepeatRewardId[i] >= 1500)
				{
					strPicName = CrushUtil::getItemPicName(g_arrWeekRepeatRewardId[i] + 10 * arrwWenponOwner[i]);

					ComInfo_T cominfo;
					ParamMgr::getInstance()->getComGameItemInfo(g_arrWeekRepeatRewardId[i] + 10 * arrwWenponOwner[i], cominfo);
					name = cominfo.name;

					auto spriteParts = Sprite::createWithSpriteFrameName("com_icon_parts_0.png");
					pItem->addChild(spriteParts);
					spriteParts->setPosition(Vec2(45.f, 120.f));
					if (i == 6)
					{
						spriteParts->setPosition(Vec2(175.f, 110.f));
					}
				}
				else
				{
					strPicName = CrushUtil::getItemPicName(g_arrWeekRepeatRewardId[i]);

					ComInfo_T cominfo;
					ParamMgr::getInstance()->getComGameItemInfo(g_arrWeekRepeatRewardId[i], cominfo);

					name = cominfo.name;
				}
				
				if (i != 6)
				{
					pTextName->setString(name);
				}

				
			}
			else
			{
				if (g_arrDailyRewardId[i] >= 1500)
				{
					auto spriteParts = Sprite::createWithSpriteFrameName("com_icon_parts_0.png");
					pItem->addChild(spriteParts,2);
					spriteParts->setPosition(Vec2(45.f, 120.f));
				}
			}
			auto pItemPic = dynamic_cast<ImageView *>(pItem->getChildByName("item_bg")->getChildByName("item"));
			auto pItemCount = (pItem->getChildByName("item_bg")->getChildByName("count"));

			if (nullptr != pItemCount)
			{
				int count = g_arrDailyRewardCount[i];
				if (isWeekReward)
				{
					count = g_arrWeekRepeatRewardCount[i];
					pCount->setString(String::createWithFormat("/%d", count)->getCString());
				}
				else
				{
					count = g_arrDailyRewardCount[i];
					pCount->setString(String::createWithFormat("/%d", count)->getCString());
				}

				if (1 == count)
				{
					pItemCount->setVisible(false);
				}
				else
				{
					pItemCount->setVisible(true);
				}
			}

			if (UserData::getInstance()->getIsGotDailyReward(i))
			{
				pConver->setVisible(true);
			}
			else
			{
				pConver->setVisible(false);
			}

			if (isWeekReward)
			{
				if (i < UserData::getInstance()->getWeekRewardIdx())
				{
					pConver->setVisible(true);
				}
				else
				{
					pConver->setVisible(false);
				}
			}

			if (i == _curDayIdx)
			{
				pItem->setEnabled(true);
				pItem->addClickEventListener([=](Ref *) {
					if (isWeekReward)
					{
						int itemId = g_arrWeekRepeatRewardId[_curDayIdx];
						int itemCount = g_arrWeekRepeatRewardCount[_curDayIdx];

						if (1 == _curDayIdx || 3 == _curDayIdx || 6 == _curDayIdx)
						{						
							itemId = g_arrWeekRepeatRewardId[_curDayIdx] + arrwWenponOwner[i] * 10;
						}

						
						auto pLayer = PopRewardLayer::create(itemId, itemCount);
						MainLayer::getCurMainLayer()->addChild(pLayer, POP_REWARD_Z);
						this->removeFromParent();
					}
					else
					{
						int itemId = g_arrDailyRewardId[_curDayIdx];
						int itemCount = g_arrDailyRewardCount[_curDayIdx];

						if (6 == _curDayIdx)
						{
							if (WeaponControl::getInstance()->isWeaponUnLock(g_arrDailyRewardId[_curDayIdx]))
							{
								itemId = g_arrDailyRewardId[_curDayIdx] - 500 + 1500;
								itemCount = 30;
								if (6 == _curDayIdx)
								{
									itemCount = 20;
								}
							}
						}

						UserData::getInstance()->setIsGotDailyReward(_curDayIdx, true);
						auto pLayer = PopRewardLayer::create(itemId, itemCount);
						MainLayer::getCurMainLayer()->addChild(pLayer, POP_REWARD_Z);
						this->removeFromParent();
					}				
				});

				pTitle->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.4f, 1.3f), ScaleTo::create(0.4f, 1.f))));
			}
			else
			{
				pItem->setEnabled(false);
			}

			if (6 == i && isWeekReward == false)
			{
				Vec2 itemPos = pItemPic->getPosition();
				auto pArmInfo = ParamMgr::getInstance()->getRoleArmtrInfo(3);
				auto pArmtr = GameArmtr::createRole(pArmInfo);
				pItem->addChild(pArmtr, 2);
				pArmtr->setPosition(itemPos + Vec2(37.f, -27.f));
				pArmtr->play("stand");

				ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/libao.ExportJson");
				auto pArmtrEffect = Armature::create("libao");
				pItem->addChild(pArmtrEffect);
				pArmtrEffect->getAnimation()->play("libao_1");
				pArmtrEffect->setPosition(itemPos+Vec2(144.f, 16.f));
			}
			else if (6 == i && isWeekReward == true)
			{
				auto picname = pItem->getChildByName("item_name");
				picname->setVisible(false);

				auto spritename = Sprite::createWithSpriteFrameName("loginui_text_xysj.png");
				pItem->addChild(spritename);
				spritename->setPosition(picname->getPosition() + Vec2(0.f, -30.f));

				ComInfo_T cominfo;
				ParamMgr::getInstance()->getComGameItemInfo(g_arrWeekRepeatRewardId[i] + arrwWenponOwner[6] * 10, cominfo);

				auto texttitle = Text::create();
				texttitle->setFontSize(30);
				texttitle->setColor(Color3B(0x6d,0x3c, 0x0c));
				texttitle->setString(cominfo.name);
				texttitle->setPosition(picname->getPosition() + Vec2(0.f, 20.f));
				pItem->addChild(texttitle);

			}

			pItemPic->loadTexture(strPicName.c_str(), ui::Widget::TextureResType::PLIST);

			
		}

		if (isWeekReward)
		{
			//_curDayIdx = UserData::getInstance()->getWeekRewardIdx();
			if (UserData::getInstance()->getWeekRewardIdx() >= 6)
			{				
				for (int i = 0; i < 3; i++)
				{
					int wenponOwner = ToolsUtil::getRandomInt(0, 3);
					UserData::getInstance()->setWeekWenponPartsIdx(i, wenponOwner);
				}
			}

			UserData::getInstance()->addWeekRewardIdx();
		}

		bRet = true;
	} while (0);

	return bRet;
}

bool DailyRewardLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return true;
}

void DailyRewardLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void DailyRewardLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void DailyRewardLayer::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}
