#include "RoleDialog.h"
#include "Role.h"
#include "FightLayer.h"
#include "Player.h"
#include "ui/CocosGUI.h"
#include "JRCommon.h"

USING_NS_CC;
using namespace ui;

RoleDialog::RoleDialog()
	:_rolePosIndex(-1)
{
}

RoleDialog::~RoleDialog()
{
}

RoleDialog * RoleDialog::create(const std::string & talkId, int rolePosIndex, int contentType)
{
	auto *pRet = new RoleDialog();

	if (nullptr != pRet && pRet->init(talkId, rolePosIndex, contentType))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoleDialog::init(const std::string & talkId, int rolePosIndex, int contentType)
{
	bool bRet = false;

	do 
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/guideUi.plist");

		_rolePosIndex = rolePosIndex;
		
		float lblWidth = 300.f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		float lblHeight = 60.f;
#else
		float lblHeight = 50.f;
#endif
		float bgWidth = lblWidth + 30.f;
		float bgHeight = lblHeight + 60.f;

		auto pBg = Scale9Sprite::createWithSpriteFrameName("guide_role_dialog_bg.png");
		pBg->setPreferredSize(Size(bgWidth, bgHeight));
		this->addChild(pBg);
		if (-1 == _rolePosIndex)
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
	if (-1 != _rolePosIndex)
	{
		Vec2 rolePos = Player::getInstance()->getCurRolePos(_rolePosIndex);//Player::getInstance()->getRole(_rolePosIndex);

		Vec2 newPos = rolePos + Vec2(190.f, 150.f);

		if (2 == _rolePosIndex)
		{
			newPos += Vec2(0.f, 30.f);
		}

		this->setPosition(newPos);
	}
}
