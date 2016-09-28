#include "PopRewardLayer.h"
#include "MainScene.h"
#include "VisibleRect.h"
#include "ParamMgr.h"
#include "ParamData.h"
#include "UserData.h"
#include "BoxDataMgr.h"
#include "CrushUtil.h"
#include "MagPieMgr.h"
#include "GameLayer.h"
#include "DayActiveMgr.h"
#include "Armtr/GameArmtr.h"

USING_NS_CC;
USING_NS_CC_EXT;

const float POP_ITEM_SPACE_W = 150.f;
const float POP_ITEM_SPACE_H = 150.f;
const int POP_ITEM_ROW_COUNT = 3;

extern const char* g_boxBtnPicName[7];
extern const char* g_boxNamePicName[7];
extern const char* g_iconbgname[10];

PopRewardLayer *PopRewardLayer::s_pInstance = nullptr;


PopRewardLayer *PopRewardLayer::getInstance(void)
{
	return s_pInstance;
}

PopRewardLayer::PopRewardLayer()
	:_isNeedOpen(false),
	_boxId(0),
	_boxLv(0),
	_pBox(nullptr),
	_pBoxPop(nullptr),
	_curPopIdx(0),
	_titleType(0),
	_isCanTouch(false),
	_topItemPosY(0.f),
	_isFirstTouch(true)
{
	s_pInstance = this;
}


PopRewardLayer::~PopRewardLayer()
{
	s_pInstance = nullptr;
}


PopRewardLayer * PopRewardLayer::create(int itemId, int itemCount, int titleType)
{
	std::vector<PopItemInfo_T> arrItems;
	PopItemInfo_T tmp;
	tmp.itemId = itemId;
	tmp.itemCount = itemCount;
	arrItems.push_back(tmp);

	return PopRewardLayer::create(arrItems, titleType);
}

PopRewardLayer* PopRewardLayer::create(const std::vector<PopItemInfo_T> &arrItems, int titleType)
{
	PopRewardLayer *pRet = new PopRewardLayer();
	if (pRet && pRet->init(arrItems, titleType, false))
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


PopRewardLayer *PopRewardLayer::openBox(int boxId, int boxLv, int param1)
{
	PopRewardLayer *pRet = new PopRewardLayer();
	if (pRet && pRet->initOpenBox(boxId, boxLv, param1))
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

PopRewardLayer * PopRewardLayer::createRoleReward(int roleId)
{
	PopRewardLayer *pRet = new PopRewardLayer();
	if (pRet && pRet->initRoleReward(roleId))
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


bool PopRewardLayer::init(const std::vector<PopItemInfo_T> &arrItems, int titleType, bool isNeedOpen)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!CCLayer::init());

		_arrItems = arrItems;
		_titleType = titleType;
		_curPopIdx = 0;
		_isNeedOpen = isNeedOpen;

		int colCount = POP_ITEM_ROW_COUNT;
		int rowCount = (arrItems.size() - 1) / POP_ITEM_ROW_COUNT + 1;

		if (arrItems.size() < POP_ITEM_ROW_COUNT)
		{
			colCount = arrItems.size();
		}

		float sizeWidth = (colCount - 1) * POP_ITEM_SPACE_W;
		float sizeHeight = (rowCount - 1) * POP_ITEM_SPACE_H;
		float startX = (VisibleRect::right().x - sizeWidth) / 2.f;
		float startY = VisibleRect::top().y - ((VisibleRect::top().y - sizeHeight) / 2.f);

		if (1 == titleType)
		{
			startY -= 100.f;
		}
		_topItemPosY = startY;

		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer);

		float itemPosX = startX;
		float itemPosY = startY;
		for (int i = 0; i < _arrItems.size(); i++)
		{
			auto &itemInfo = _arrItems.at(i);

			auto effectsp = Sprite::createWithSpriteFrameName("box_bg_2.png");
			layer->addChild(effectsp);
			effectsp->setPosition(Vec2(itemPosX, itemPosY));

			auto actionrotate = RepeatForever::create(RotateBy::create(5.f, -360.f));
			effectsp->runAction(actionrotate);

			std::string picName;
			std::string bgName = "box_bg_2.png";
			bool neednum = true;

			if (ParamData::GOLD_ITEM_ID == itemInfo.itemId)
			{
				picName = "com_goldbag.png";
			}
			else if (ParamData::CRYSTAL_ITEM_ID == itemInfo.itemId)
			{
				picName = "com_crystal.png";
			}
			else if (ParamData::TILI_ITEM_ID == itemInfo.itemId)
			{
				picName = "com_icon_jingqi.png";
			}
			else if (PopRewardBoxId::POP_BOX_0 == itemInfo.itemId)
			{
				picName = g_boxBtnPicName[0];
				neednum = false;

				addBoxLvTille(0, itemInfo.itemLv, Vec2(itemPosX, itemPosY - 80.f));
			}
			else if (PopRewardBoxId::POP_BOX_1 == itemInfo.itemId)
			{
				picName = g_boxBtnPicName[1];
				neednum = false;

				addBoxLvTille(1, itemInfo.itemLv, Vec2(itemPosX, itemPosY - 80.f));
			}
			else if (PopRewardBoxId::POP_BOX_2 == itemInfo.itemId)
			{
				picName = g_boxBtnPicName[2];
				neednum = false;

				addBoxLvTille(2, itemInfo.itemLv, Vec2(itemPosX, itemPosY - 80.f));
			}
			else if (PopRewardBoxId::POP_BOX_3 == itemInfo.itemId)
			{
				picName = g_boxBtnPicName[3];
				neednum = false;

				addBoxLvTille(3, itemInfo.itemLv, Vec2(itemPosX, itemPosY - 80.f));
			}
			else
			{
				ComInfo_T item;
				ParamMgr::getInstance()->getComGameItemInfo(itemInfo.itemId, item);
				picName = item.picname;
			}

			auto  icon = Sprite::createWithSpriteFrameName(picName);
			layer->addChild(icon);
			icon->setPosition(Vec2(itemPosX, itemPosY));

			float fScale = MAX(1.f, icon->getContentSize().width / 92.f);
			effectsp->setScale(fScale);

			if (BagItemControl::getInstace()->checkItemType(itemInfo.itemId) == 3)
			{
				auto parttype = BagItemControl::getInstace()->checkWenponPartsType(itemInfo.itemId);
				auto iconparts = Sprite::createWithSpriteFrameName("com_icon_parts_0.png");
				icon->addChild(iconparts);
				iconparts->setPosition(Vec2(10.f, 90.f));
			}

			auto num = ui::TextAtlas::create(String::createWithFormat(".%d", itemInfo.itemCount)->getCString(), "fonts/num_boxreward.png", 18.f, 28.f, ".");
			layer->addChild(num);
			num->setPosition(Vec2(itemPosX + 40.f, itemPosY - 40.f));
			num->setVisible(neednum);

			if (0 == ((i + 1) % POP_ITEM_ROW_COUNT))
			{
				itemPosX = startX;
				itemPosY -= POP_ITEM_SPACE_H;
			}
			else
			{
				itemPosX += POP_ITEM_SPACE_W;
			}
		}

		this->showRewardTitle(_titleType);
		this->giveItemToUser();

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = CC_CALLBACK_2(PopRewardLayer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PopRewardLayer::onTouchEnded, this);//[=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {this->removeFromParent(); };
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

		bRet = true;
	} while (0);

	return bRet;
}

bool PopRewardLayer::initOpenBox(int boxId, int boxLv, int param1)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!CCLayer::init());

		MagPieMgr::getInstance()->addFinishedMagPieGoalNum(MagPieMgr::_MagPieTaskNameIdx::BOX, 1);

		if (boxId == 2)
		{
			DayActivityMgr::getInstance()->addTimes(DayActivityTppe::DAYOPENGOLDBOX, 1, false);
		}

		_boxId = boxId;
		_boxAnimId = this->getBoxAnimId(_boxId);
		_boxLv = boxLv;
		BoxDataMgr::getInstance()->getBoxReward(_boxId, _boxLv, _arrItems);
		_boxParam = param1;

		if (2 == param1 && 0 == _boxId)
		{
			for (int i = 0; i < _arrItems.size(); i++)
			{
				if (_arrItems.at(i).itemId == ParamData::TILI_ITEM_ID)
				{
					_arrItems.at(i).itemCount = 6;
				}
			}
		}
		else if (3 == param1)
		{
			for (int i = 0; i < _arrItems.size(); i++)
			{
				if (_arrItems.at(i).itemId == ParamData::TILI_ITEM_ID)
				{
					_arrItems.erase(_arrItems.begin()+i);
					break;
				}
			}
		}

		_titleType = -1;
		_curPopIdx = 0;
		_isNeedOpen = true;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/baoxiang_kaiqi/baoxiang_kaiqi.ExportJson");

		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer);

		__String *strAnim = __String::createWithFormat("tiaochu_%d", _boxAnimId);
		_pBox = Armature::create("baoxiang_kaiqi");
		_pBox->setPosition(320.f, VisibleRect::top().y * 0.3f);
		this->addChild(_pBox);
		_pBox->getAnimation()->play(strAnim->getCString());
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=] {_isCanTouch = true; })));

		_pBoxPop = Armature::create("baoxiang_kaiqi");
		_pBoxPop->setPosition(320.f, VisibleRect::top().y * 0.3f);
		this->addChild(_pBoxPop);
		_pBoxPop->setVisible(false);

		if (_boxId <= 3)
		{
			addBoxLvTille(_boxId, _boxLv, _pBox->getPosition() + Vec2(0, -80.f));
		}

		this->giveItemToUser();
		
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = CC_CALLBACK_2(PopRewardLayer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PopRewardLayer::onTouchEnded, this);//[=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {this->removeFromParent(); };
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

		bRet = true;
	} while (0);

	return bRet;
}

bool PopRewardLayer::initRoleReward(int roleId)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!CCLayer::init());

		auto layer = LayerColor::create(Color4B(0, 0, 0, 188), 640.f, VisibleRect::top().y);
		this->addChild(layer);

		auto pArmInfo = ParamMgr::getInstance()->getRoleArmtrInfo(RoleId_E::ROLE_ID_LSJ + roleId);
		auto pRole = GameArmtr::createRole(pArmInfo);
		pRole->play(ArmtrName::ROLE_IDLE);

		pRole->setPosition(VisibleRect::center() - Vec2(0, 80.f));
		this->addChild(pRole);

		_topItemPosY = VisibleRect::center().y;

		float posY = (VisibleRect::top().y - _topItemPosY) / 2 + _topItemPosY;
		auto pRoleName = Sprite::createWithSpriteFrameName("mainui_name_4.png");
		pRoleName->setPosition(Vec2(VisibleRect::center().x, posY - 60.f));
		this->addChild(pRoleName);

		this->showRewardTitle(_titleType);		

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = CC_CALLBACK_2(PopRewardLayer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PopRewardLayer::onTouchEnded, this);//[=](cocos2d::Touch *touch, cocos2d::Event *unused_event) {this->removeFromParent(); };
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

		bRet = true;
	} while (0);

	return bRet;
}


void PopRewardLayer::onEnter(void)
{
	CCLayer::onEnter();
}


void PopRewardLayer::update(float delta)
{
	//do nothing
}


void PopRewardLayer::setRankIdx(int randIdx)
{
	auto pRankTitle = Sprite::createWithSpriteFrameName("rank_last_rank.png");
	pRankTitle->setPosition(Vec2(320.f, VisibleRect::top().y - 80.f));
	this->addChild(pRankTitle, 1);

	auto pLblRank = Label::createWithCharMap("fonts/num_boxreward.png", 18, 28, '.');
	auto strRank = __String::createWithFormat("%d", randIdx);
	pLblRank->setString(strRank->getCString());
	this->addChild(pLblRank, 1);
	pLblRank->setPosition(Vec2(320.f, VisibleRect::top().y - 120.f));
}

void PopRewardLayer::showRewardTitle(int titleType)
{
	if (titleType == 0)
	{
		auto textbg = Sprite::createWithSpriteFrameName("com_textbg_boxreward.png");
		this->addChild(textbg, 1);

		auto text = Sprite::createWithSpriteFrameName("com_text_gxhd.png");
		this->addChild(text, 2);

		float posY = (VisibleRect::top().y - _topItemPosY) / 2 + _topItemPosY;
		textbg->setPosition(Vec2(320.f, posY));//Vec2(320.f, VisibleRect::top().y - 90.f)
		text->setPosition(Vec2(320.f, posY+10.f));//Vec2(320.f, VisibleRect::top().y - 80.f)
	}
	else if (titleType == 1)
	{
		auto textbg = Sprite::createWithSpriteFrameName("com_popreward_bg.png");
		this->addChild(textbg, 1);

		auto text = Sprite::createWithSpriteFrameName("rank__reward_title.png");
		this->addChild(text, 2);

		textbg->setPosition(Vec2(320.f, VisibleRect::top().y - 180.f));
		text->setPosition(Vec2(320.f, VisibleRect::top().y - 180.f));
	}
	else if (titleType == 2)
	{
		auto textbg = Sprite::createWithSpriteFrameName("com_popreward_bg.png");
		this->addChild(textbg, 1);

		auto text = Sprite::createWithSpriteFrameName("com_text_gxhd.png");
		this->addChild(text, 2);

		float posY = (VisibleRect::top().y - _topItemPosY) / 2 + _topItemPosY;
		textbg->setPosition(Vec2(320.f, posY));
		text->setPosition(Vec2(320.f, posY + 10.f));
	}
}

void PopRewardLayer::addBoxLvTille(int idx, int lv, Vec2 pos)
{
	lv = lv > 4 ? 4 : lv;
	idx = idx > 3 ? 3 : idx;

	auto lvsp = Sprite::createWithSpriteFrameName(String::createWithFormat("boxlayer_text_lv_%d.png", lv)->getCString());
	auto namesp = Sprite::createWithSpriteFrameName(String::createWithFormat("boxlayer_boxname_%d.png", idx)->getCString());

	lvsp->setAnchorPoint(Vec2(1.f, 0.5f));
	namesp->setAnchorPoint(Vec2(0.f, 0.5f));
	
	this->addChild(lvsp, 5);
	this->addChild(namesp, 5);

	lvsp->setPosition(pos + Vec2(-20.f, 0.f));
	namesp->setPosition(pos + Vec2(-20.f, 0.f));
}

int PopRewardLayer::getBoxAnimId(int boxId)
{
	int ret = boxId+1;

	if (boxId > 6)
	{
		int arrTmp[] = {1, 1, 2, 4, 1, 1, 4, 1, 4, 2, 1, 4, 4, 2, 4, 1, 4, 4, 2};
		ret = arrTmp[boxId-7];
	}

	return ret;
}

bool PopRewardLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	if (_isNeedOpen)
	{
		_pBox->setScale(0.8f);
//		_pBoxPop->setScale(0.8f);
	}
	return true;
}

void PopRewardLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void PopRewardLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	if (!_isNeedOpen)
	{
		this->removeFromParent();
	}
	else
	{
		_pBox->setScale(1.f);
//		_pBoxPop->setScale(1.f);
		if (_isCanTouch)
		{
			float popDur = 1.f;
			_isCanTouch = false;

			std::vector<PopItemInfo_T> arrCurPopItem;

			do 
			{
				for (int i = 0; i < _arrItems.size(); i++)
				{
					auto item = _arrItems.at(i);
					if (_curPopIdx == item.popIdx)
					{
						arrCurPopItem.push_back(item);
					}
				}

				if (arrCurPopItem.empty())
				{
					_curPopIdx++;
				}
			} while (arrCurPopItem.empty() && _curPopIdx < 6);

			if (arrCurPopItem.empty())
			{
				if (4 == _boxParam)
				{
					GameLayer::getInstance()->resumeGame();
				}
				this->removeFromParent();
			}
			else
			{
				int kaIdx = arrCurPopItem.size();
				changeAniPic(_pBoxPop, arrCurPopItem);
//				if (0 == _curPopIdx)
				if (_isFirstTouch)
				{
					_isFirstTouch = false;
					__String *strAnim = __String::createWithFormat("xiang_%d", _boxAnimId);
					_pBox->getAnimation()->play(strAnim->getCString());
// 					this->runAction(Sequence::create(DelayTime::create(1.4f), CallFunc::create([=] {
// 						_pBoxPop->setVisible(true);
// 						__String *strName = __String::createWithFormat("ka_%d", kaIdx);
// 						_pBoxPop->getAnimation()->play(strName->getCString()); }), DelayTime::create(popDur), CallFunc::create([=] {_isCanTouch = true; }), nullptr));

					this->runAction(Sequence::create(DelayTime::create(0.9f), CallFunc::create([=] {
						_pBoxPop->setVisible(true);
						__String *strName = __String::createWithFormat("ka_%d", kaIdx);
						_pBoxPop->getAnimation()->play(strName->getCString());
						_isCanTouch = true;
					}), DelayTime::create(0.4f), CallFunc::create([=] {
						__String *strName = __String::createWithFormat("xiang_dianji%d", _boxAnimId);
						_pBox->getAnimation()->play(strName->getCString());
						 }), nullptr));
				}
				else
				{
					__String *strName = __String::createWithFormat("ka_%d", kaIdx);
					_pBoxPop->getAnimation()->play(strName->getCString());
					_isCanTouch = true;
//					this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=] {_isCanTouch = true; })));
				}
				_curPopIdx++;
			}
		}
	}
}

void PopRewardLayer::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void PopRewardLayer::changeAniPic(cocostudio::Armature * ani, const std::vector<PopItemInfo_T>& arrItems)
{
	if (arrItems.size() == 1)
	{
		auto displayermanger = ani->getBone("Layer8_Copy10")->getDisplayManager();
		auto node = getItemDisplayerNode(arrItems[0].itemId, arrItems[0].itemCount);
		displayermanger->addDisplay(node, 0);
	}
	else if (arrItems.size() == 2)
	{
		auto displayermanger = ani->getBone("Layer8_Copy9")->getDisplayManager();
		auto node = getItemDisplayerNode(arrItems[0].itemId, arrItems[0].itemCount);
		displayermanger->addDisplay(node, 0);

		auto displayermanger2 = ani->getBone("Layer8")->getDisplayManager();
		auto node2 = getItemDisplayerNode(arrItems[1].itemId, arrItems[1].itemCount);
		displayermanger2->addDisplay(node2, 0);
	}
	else if (arrItems.size() == 3)
	{
		auto displayermanger = ani->getBone("Layer8_Copy9")->getDisplayManager();
		auto node = getItemDisplayerNode(arrItems[0].itemId, arrItems[0].itemCount);
		displayermanger->addDisplay(node, 0);

		auto displayermanger3 = ani->getBone("Layer8_Copy10")->getDisplayManager();
		auto node3 = getItemDisplayerNode(arrItems[2].itemId, arrItems[2].itemCount);
		displayermanger3->addDisplay(node3, 0);

		auto displayermanger2 = ani->getBone("Layer8")->getDisplayManager();
		auto node2 = getItemDisplayerNode(arrItems[1].itemId, arrItems[1].itemCount);
		displayermanger2->addDisplay(node2, 0);
	}
}

Node * PopRewardLayer::getItemDisplayerNode(int itemid, int num)
{
	Node* displayerNode = Node::create();

	std::string picName;

	bool neednum = true;

	if (ParamData::GOLD_ITEM_ID == itemid)
	{
		picName = "com_rewardboxgold.png";
	}
	else if (ParamData::CRYSTAL_ITEM_ID == itemid)
	{
		picName = "com_crystal.png";
	}
	else if (ParamData::TILI_ITEM_ID == itemid)
	{
		picName = "com_icon_jingqi.png";
	}
	else if (PopRewardBoxId::POP_BOX_0 == itemid)
	{
		picName = g_boxBtnPicName[0];
		neednum = false;
	}
	else if (PopRewardBoxId::POP_BOX_1 == itemid)
	{
		picName = g_boxBtnPicName[1];
		neednum = false;
	}
	else if (PopRewardBoxId::POP_BOX_2 == itemid)
	{
		picName = g_boxBtnPicName[2];
		neednum = false;
	}
	else if (PopRewardBoxId::POP_BOX_3 == itemid)
	{
		picName = g_boxBtnPicName[3];
		neednum = false;
	}
	else
	{
		ComInfo_T item;
		ParamMgr::getInstance()->getComGameItemInfo(itemid, item);
		picName = item.picname;
	}

	if (BagItemControl::getInstace()->checkItemType(itemid) == 3)
	{
		int picidx = (itemid - 1500) % 10;
		auto bg = Sprite::createWithSpriteFrameName(g_iconbgname[picidx]);
		displayerNode->addChild(bg);
		bg->setScale(0.9f);
	}
	else
	{
		auto bg = Sprite::createWithSpriteFrameName("store_item_bg.png");
		displayerNode->addChild(bg);
	}


	auto  icon = Sprite::createWithSpriteFrameName(picName);
	displayerNode->addChild(icon);

	if (BagItemControl::getInstace()->checkItemType(itemid) == 3)
	{
		auto parttype = BagItemControl::getInstace()->checkWenponPartsType(itemid);
		auto iconparts = Sprite::createWithSpriteFrameName("com_icon_parts_0.png");
		icon->addChild(iconparts);
		iconparts->setPosition(Vec2(10.f, 80.f));
	}

	auto numtext = ui::TextAtlas::create(String::createWithFormat(".%d", num)->getCString(), "fonts/num_boxreward.png", 18.f, 28.f, ".");
	displayerNode->addChild(numtext);
	numtext->setVisible(neednum);
	numtext->setPosition(Vec2(35.f, -35.f));
	return displayerNode;
}

void PopRewardLayer::giveItemToUser()
{
	for (int i = 0; i < _arrItems.size(); i++)
	{
		auto &itemInfo = _arrItems.at(i);
		bool isNeedSave = false;
		if (i == (_arrItems.size()-1))
		{
			isNeedSave = true;
		}
		CrushUtil::giveUserItem(itemInfo.itemId, itemInfo.itemCount, itemInfo.itemLv, isNeedSave);
	}
#if 0
	for (int i = 0; i < _arrItems.size(); i++)
	{
		auto &itemInfo = _arrItems.at(i);

		if (ParamData::GOLD_ITEM_ID == itemInfo.itemId)
		{
			UserData::getInstance()->giveGold(itemInfo.itemCount);
		}
		else if (ParamData::CRYSTAL_ITEM_ID == itemInfo.itemId)
		{
			//unfinish
			UserData::getInstance()->giveCrystal(itemInfo.itemCount);
		}
		else if (ParamData::TILI_ITEM_ID == itemInfo.itemId)
		{
			UserData::getInstance()->giveTili(itemInfo.itemCount);
		}
		else if (itemInfo.itemId >= PopRewardBoxId::POP_BOX_0 && itemInfo.itemId <= PopRewardBoxId::POP_BOX_3)
		{
			int idx = itemInfo.itemId - PopRewardBoxId::POP_BOX_0;
			UserData::getInstance()->addBoxConfig(idx, itemInfo.itemLv);
		}
		else
		{
			if (BagItemControl::getInstace()->checkItemType(itemInfo.itemId) == 3)
			{
				UserData::getInstance()->addWeaponPartsNum(itemInfo.itemId, itemInfo.itemCount);
				MainLayer::getCurMainLayer()->updataEquipLayer((itemInfo.itemId - 1500) / 10);
			}
			else if (BagItemControl::getInstace()->checkItemType(itemInfo.itemId) == 1)
			{
				UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_skill_%d", itemInfo.itemId)->getCString(), 1);
				BagItemControl::getInstace()->addBagItem(itemInfo.itemId);
			}
			else
			{
				for (int j = 0; j < itemInfo.itemCount; j++)
				{
					BagItemControl::getInstace()->addBagItem(itemInfo.itemId);
				}
			}
		}
	}
	BagItemControl::getInstace()->saveItemConfig();
	UserData::getInstance()->saveUserData();

#endif
}