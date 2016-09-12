#include "EditNickLayer.h"
#include "UserData.h"
#include "GameCSLoader.h"
#include "VisibleRect.h"
#include "platBridge/cocos2dx_plat.h"
#include "EndlessMainLayer.h"
#include "MainScene.h"
#include "NetDataMgr.h"
#include "JRCommon.h"

USING_NS_CC;
using namespace std;


EditNickLayer *EditNickLayer::s_pInstance = nullptr;


EditNickLayer *EditNickLayer::getInstance(void)
{
	return s_pInstance;
}


EditNickLayer::EditNickLayer()
{
	s_pInstance = this;
}


EditNickLayer::~EditNickLayer()
{
	if (this == s_pInstance)
	{
		s_pInstance = nullptr;
	}
}

bool EditNickLayer::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Layer::init());

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [this](Touch*, Event*) { return true; };
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		LayerColor *pLayer = LayerColor::create(Color4B(0, 0, 0, 180));
		this->addChild(pLayer, 0);

		auto pRoot = GameCSLoader::createNode("csb/rankui_personInfo_layer.csb");
		this->addChild(pRoot);
		pRoot->setPositionY((VisibleRect::top().y - 960.f) / 2.f);
		_pRoot = pRoot;

		auto pTitle0 = pRoot->getChildByName("title_0");
		auto pTitle1 = pRoot->getChildByName("title_1");

		auto strNick = UserData::getInstance()->getNickName();
		if (strNick.empty() || "" == strNick)
		{
			pTitle0->setVisible(false);
			pTitle1->setVisible(true);
		}
		else
		{
			pTitle0->setVisible(true);
			pTitle1->setVisible(false);
		}
		_strNick = strNick;

		auto pMain = pRoot->getChildByName("main_bg");
		int headId = UserData::getInstance()->getHeadId();
		_headId = headId;
		for (int i = 0; i < ParamData::HEAD_TOTAL_COUNT; i++)
		{
			auto strBtnName = __String::createWithFormat("bt_head_%d", i);
			auto pHeadBtn = dynamic_cast<ui::Button *>(pMain->getChildByName(strBtnName->getCString()));

			pHeadBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					_headId = i;
					this->updateUI();
				}
			});
		}

		auto pBtnDice = pMain->getChildByName("bt_dice");
		pBtnDice->setVisible(false);

		auto pNick = dynamic_cast<ui::TextField *>(pMain->getChildByName("your_name"));
		auto pBtnSure = dynamic_cast<ui::Button *>(pMain->getChildByName("bt_sure"));
		pBtnSure->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				std::string strNick = pNick->getString();
				if (cocos2dx_plat::hasSensitiveWord(strNick))
				{
					GameUtils::toastTip(2);
				}
				else
				{
					UserData::getInstance()->setNickName(strNick.c_str());
				}

				UserData::getInstance()->setHeadId(_headId);
				UserData::getInstance()->saveUserData(true);
				EndlessMainLayer::getInstance()->updateSelfInfoUI();
				NetDataMgr::getInstance()->updateRole(CC_CALLBACK_2(MainLayer::onUpdateSelfInfoCallback, MainLayer::getCurMainLayer()));
				this->removeFromParent();
			}
		});

		auto pBtnClose = dynamic_cast<ui::Button *>(pRoot->getChildByName("bt_close"));
		pBtnClose->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				this->removeFromParent();
			}
		});
		_pBtnClose = pBtnClose;

		auto pTextNick = dynamic_cast<ui::TextField *>(pMain->getChildByName("your_name"));
//		pTextNick->setTouchAreaEnabled(true);
		pTextNick->addEventListener([=](Ref *, ui::TextField::EventType evt) {
			switch (evt)
			{
			case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
			{
//				if (_inputNode != nullptr)
				{
					auto action = EaseSineOut::create(MoveBy::create(0.2f, Vec2(0.f, 160.f)));
					pRoot->runAction(action);

//					_isAttacking = true;
				}
			}
			break;
			case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
			{
//				if (_inputNode != nullptr)
				{
					auto action = Sequence::createWithTwoActions(EaseSineOut::create(MoveBy::create(0.2f, Vec2(0.f, -160.f))), CallFunc::create([=]() {
//						_isAttacking = false;
					}));
					pRoot->runAction(action);
				}
			}
			break;
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
				break;
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
				break;
			default:
				break;
			}

		});
 		pTextNick->setPlaceHolderColor(Color4B(255, 255, 255, 158));
 		pTextNick->setMaxLengthEnabled(true);
 		pTextNick->setMaxLength(10);
 		pTextNick->setTextHorizontalAlignment(TextHAlignment::CENTER);
 		pTextNick->setTextVerticalAlignment(TextVAlignment::CENTER);
		pTextNick->setString(ResMgr::getInstance()->getString("nick_default")->getCString());

		this->updateUI();
		bRet = true;
	} while (0);

	return bRet;
}


void EditNickLayer::updateUI(void)
{
	int headId = _headId;// UserData::getInstance()->getHeadId();
	auto pMain = _pRoot->getChildByName("main_bg");
	for (int i = 0; i < ParamData::HEAD_TOTAL_COUNT; i++)
	{
		auto strBtnName = __String::createWithFormat("bt_head_%d", i);
		auto pHeadBtn = dynamic_cast<ui::Button *>(pMain->getChildByName(strBtnName->getCString()));
		auto pSelect = pHeadBtn->getChildByName("selecting");

		if (i != headId)
		{
			pSelect->setVisible(false);
		}
		else
		{
			pSelect->setVisible(true);
		}
	}

	auto pNick = dynamic_cast<ui::TextField *>(pMain->getChildByName("your_name"));
	auto strNick = _strNick;// UserData::getInstance()->getNickName();
	if (!strNick.empty() && "" != strNick)
	{
		pNick->setString(strNick);
	}
}


void EditNickLayer::menuOnBackCB(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
	s_pInstance = nullptr;
}


void EditNickLayer::menuOnConfirmCB(Ref* pSender)
{
//	UserData::getInstance()->setNickName(m_pEditBox->getText());
//	UserData::getInstance()->saveUserData();
//	NetDataMgr::getInstance()->updateRole(m_pMainScene->getMainLayer(), httpresponse_selector(MainLayer::onUpdateSelfInfoCallback));
//	m_pMainScene->getCurLayer()->update(0.f);
//	this->menuOnCancelCB(nullptr);
}


void EditNickLayer::menuOnCancelCB(Ref* pSender)
{
//	s_pInstance = nullptr;
//	this->removeFromParentAndCleanup(true);
}


void EditNickLayer::update(float delta)
{
	Layer::update(delta);
//	this->updateUI();
}