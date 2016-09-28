#include "RoundTag.h"
#include "ParamData.h"

RoundTag::RoundTag()
	:_pBg(nullptr),
	_pLbl(nullptr)
{
}

RoundTag::~RoundTag()
{
}

RoundTag * RoundTag::create(int tagType, int roundCount)
{
	RoundTag *pRet = new RoundTag();

	if (nullptr != pRet && pRet->init(tagType, roundCount, -1))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

RoundTag * RoundTag::createWeakIcon(int tagType, int roundCount, int weakId)
{
	RoundTag *pRet = new RoundTag();

	if (nullptr != pRet && pRet->init(tagType, roundCount, weakId))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

RoundTag * RoundTag::createRoleBuff(int roundCount, int buffId)
{
	RoundTag *pRet = new RoundTag();

	if (nullptr != pRet && pRet->init(4, roundCount, buffId))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoundTag::init(int tagType, int roundCount, int paramId)
{
	bool bRet = false;

	do 
	{
		_tagType = tagType;
		_curRound = roundCount;
		_paramId = paramId;

		if (0 == _tagType)
		{
			//eleicon
			_pBg = Sprite::createWithSpriteFrameName("ele_round_bg.png");
			_pLbl = Label::createWithCharMap("fonts/num_ele_round.png", 20, 20, '.');
		}
		else if (1 == _tagType)
		{
			//monster
			_pBg = Sprite::createWithSpriteFrameName("tag_round_2.png");
			_pLbl = Label::createWithCharMap("fonts/num_monster_round.png", 18, 20, '.');
		}
		else if (2 == _tagType)
		{
			//monster weak
			_pLbl = Label::createWithCharMap("fonts/num_monster_round.png", 18, 20, '.');
//			_pLbl = Label::createWithCharMap("fonts/font_wenpon_num.png", 16, 26, '.');
			_pLbl->setPosition(Vec2(15.f, -15.f));
//			_pLbl->setScale(0.5f);
			if (_paramId == AttAttrbt_E::ATT_FIRE)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_fire_0.png");
			}
			else if (_paramId == AttAttrbt_E::ATT_THUNDER)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_thunder.png");
			}
			else if (_paramId == AttAttrbt_E::ATT_ICE)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_ice.png");
			}
			else if (_paramId == AttAttrbt_E::ATT_NARROW)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_small_0.png");
			}
			else if (_paramId == AttAttrbt_E::ATT_CONFUSE)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_confuse.png");
			}
			else if (_paramId == AttAttrbt_E::ATT_DRUG)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_stay.png");
			}
		}
		else if (3 == _tagType)
		{
			//role weak
		}
		else if (4 == _tagType)
		{
			//role buff
			_pLbl = Label::createWithCharMap("fonts/num_monster_round.png", 18, 20, '.');
//			_pLbl = Label::createWithCharMap("fonts/font_wenpon_num.png", 16, 26, '.');
//			_pLbl->setScale(0.5f);
			_pLbl->setPosition(Vec2(10.f, -10.f));
			if (_paramId == 0)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_hp.png");
			}
			else if (_paramId == 1)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_def_add.png");
			}
			else if (_paramId == 2)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_relive.png");
			}
			else if (_paramId == 3)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_def.png");
			}
			else if (_paramId == 4)
			{
				_pBg = Sprite::createWithSpriteFrameName("tag_att_buff.png");
			}
		}

		this->addChild(_pBg);
		this->addChild(_pLbl);

		this->setCurRound(_curRound);

		bRet = true;
	} while (false);

	return bRet;
}

bool RoundTag::doRound()
{
	return this->setCurRound(_curRound-1);
}

bool RoundTag::setCurRound(int curRound)
{
	_curRound = curRound;
	__String *strNum = __String::createWithFormat("%d", _curRound);
	_pLbl->setString(strNum->getCString());

	if (1 == _tagType)
	{
		if (_curRound <= 1)
		{
			_pBg->setSpriteFrame("tag_round_0.png");
		}
		else if (2 == _curRound)
		{
			_pBg->setSpriteFrame("tag_round_1.png");
		}
		else
		{
			_pBg->setSpriteFrame("tag_round_2.png");
		}
	}
	else if (0 == _tagType || 2 == _tagType || 3 == _tagType)
	{
		_pLbl->runAction(Sequence::create(ScaleTo::create(0.15f, 1.8f), ScaleTo::create(0.18f, 1.f), nullptr));
	}
	else if (4 == _tagType)
	{
		_pBg->runAction(Sequence::create(ScaleTo::create(0.15f, 1.6f), ScaleTo::create(0.18f, 1.f), nullptr));
	}

	return 0 == _curRound;
}
