#include "RoleDialog.h"
#include "Role.h"
#include "FightLayer.h"
#include "Player.h"
#include "ui/CocosGUI.h"
#include "JRCommon.h"

USING_NS_CC;
using namespace ui;

RoleDialog::RoleDialog()
	:_roleId(-1)
{
}

RoleDialog::~RoleDialog()
{
}

RoleDialog * RoleDialog::create(const std::string & talkId, int roleId, int contentType)
{
	auto *pRet = new RoleDialog();

	if (nullptr != pRet && pRet->init(talkId, roleId, contentType))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoleDialog::init(const std::string & talkId, int roleId, int contentType)
{
	bool bRet = false;

	do 
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/guideUi.plist");

		_roleId = roleId;
		
		float lblWidth = 300.f;
		float lblHeight = 50.f;
		float bgWidth = lblWidth + 30.f;
		float bgHeight = lblHeight + 60.f;

		auto pBg = Scale9Sprite::createWithSpriteFrameName("guide_role_dialog_bg.png");
		pBg->setPreferredSize(Size(bgWidth, bgHeight));
		this->addChild(pBg);
		if (-1 == _roleId)
		{
			pBg->setFlippedX(true);
		}

		__String *strText = nullptr;
		if (0 == contentType)
		{
			strText = ResMgr::getInstance()->getString(talkId.c_str());
		}
		else if (1 == contentType)
		{
			strText = __String::create(talkId);
		}

		auto pLbl = Label::createWithSystemFont(strText->getCString(), "fonts/Marker Felt.ttf", 24, Size(lblWidth, lblHeight));
		pLbl->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
		pLbl->setPosition(Vec2(0.f, 12.f));
		this->addChild(pLbl);

		pLbl->setTextColor(Color4B::BLACK);

		this->scheduleUpdate();
		bRet = true;
	} while (false);

	return bRet;
}

void RoleDialog::update(float dt)
{
	if (-1 != _roleId)
	{
		auto pRole = Player::getInstance()->getRole(_roleId);

		Vec2 newPos = pRole->getPosition() + Vec2(190.f, 150.f);

		if (2 == _roleId)
		{
			newPos += Vec2(0.f, 30.f);
		}

		this->setPosition(newPos);
	}
}
