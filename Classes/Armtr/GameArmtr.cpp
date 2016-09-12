#include "GameArmtr.h"


const std::string ArmtrName::ROLE_WALK = "walk";
const std::string ArmtrName::ROLE_IDLE = "stand";
const std::string ArmtrName::ROLE_CHEER = "win";
const std::string ArmtrName::ROLE_NOR_ATT = "attack01";
const std::string ArmtrName::ROLE_NOR_DEF = "attack02";
const std::string ArmtrName::ROLE_ATT_BASE = "attack";
const std::string ArmtrName::SKILL_NOR_ATT = "attack";
const std::string ArmtrName::ROLE_STATE_ADD_DEF = "buff_6";
const std::string ArmtrName::ROLE_STATE_ADD_HP = "buff_7";
const std::string ArmtrName::ROLE_STATE_CLEAN = "buff_9";
const std::string ArmtrName::ROLE_STATE_STRONG = "buff_8";
const std::string ArmtrName::ROLE_STATE_RELIVE = "buff_10";
const std::string ArmtrName::ROLE_STATE_NARROW = "buff_4";
const std::string ArmtrName::ROLE_STATE_CONFUSE = "buff_5";

const std::string ArmtrName::MONSTER_IDLE = "stand";
const std::string ArmtrName::MONSTER_HURT = "hurt";
const std::string ArmtrName::MONSTER_CLOSE_ATT = "attack0";
const std::string ArmtrName::MONSTER_REMOTE_ATT = "attack100";
const std::string ArmtrName::MONSTER_DEATH = "death";


const std::string ArmtrName::ROLE_EVT_HURT = "act";//"r_e_hurt";
const std::string ArmtrName::ROLE_EVT_SKILL = "skill_act";// "r_e_skill";
const std::string ArmtrName::ROLE_EVT_SKILL_HURT = "skill_hurt";// "skill_hurt";// "r_e_skill_hurt";

const std::string ArmtrName::MONSTER_EVT_HURT = "act";
const std::string ArmtrName::MONSTER_EVT_SKILL = "skill_act";
const std::string ArmtrName::MONSTER_EVT_SKILL_HURT = "skill_hurt";

const float GameArmtr::ARMTR_SCALE_VALE = 1.f;// 0.78f;

GameArmtr::GameArmtr()
{
}


GameArmtr::~GameArmtr()
{
}


GameArmtr *GameArmtr::createRole(const DragonBoneInfo_T *pInfo)
{
	return create(pInfo->strPathName, pInfo->strDragonBoneName, pInfo->strArmtrName, GameArmtrType_E::TYPE_ROLE);
}

GameArmtr * GameArmtr::createSkill(const std::string & skillName)
{
	auto strPatch = __String::createWithFormat("skills/%s", skillName.c_str());
	return create(strPatch->getCString(), skillName, skillName, GameArmtrType_E::TYPE_SKILL);
}

GameArmtr * GameArmtr::createDart()
{
	return create("roles/feibiao", "feibiao", "feibiao", GameArmtrType_E::TYPE_FB);
}

GameArmtr * GameArmtr::createMonster(const std::string & animName)
{
	std::string strPathName = "monsters/";
	strPathName.append(animName);
	return create(strPathName, animName, animName, GameArmtrType_E::TYPE_MONSTER);
}

GameArmtr * GameArmtr::createBuff(const std::string & buffName)
{
	return create("skills/buff", "buff", "buff", GameArmtrType_E::TYPE_BUFF);
}

GameArmtr * GameArmtr::createHit(const std::string & hitName)
{
	return create("skills/hit", "hit", "hit", GameArmtrType_E::TYPE_HIT);
}

GameArmtr * GameArmtr::createEleIcon()
{
	return create("items/crush_icon", "crush_icon", "crush_icon", GameArmtrType_E::TYPE_ELE_ICON);
}


GameArmtr * GameArmtr::createCrushEffect()
{
	return create("items/crush_effect", "crush_effect", "crush_effect", GameArmtrType_E::TYPE_CRUSH_EFFECT);
}

GameArmtr * GameArmtr::createCreateEffect()
{
	return create("items/create", "create", "create", GameArmtrType_E::TYPE_CREATE_EFFECT);
}

GameArmtr * GameArmtr::createIconSkill()
{
	return create("items/icon_skill_effect", "icon_skill_effect", "icon_skill_effect", GameArmtrType_E::TYPE_ICON_SKILL);
}

GameArmtr * GameArmtr::createItemEffect()
{
	return create("items/item_effect", "item_effect", "item_effect", GameArmtrType_E::TYPE_ITEM_EFFECT);
}

GameArmtr * GameArmtr::createSpecial()
{
	return create("items/special_icon", "special_icon", "special_icon", GameArmtrType_E::TYPE_SPECIAL);
}

GameArmtr * GameArmtr::createSpecialEx()
{
	return create("items/special_icon_ex", "special_icon_ex", "special_icon_ex", GameArmtrType_E::TYPE_SPECIAL_EX);
}

GameArmtr * GameArmtr::create(const std::string & pathName, const std::string & dragonBoneName, const std::string & armtrName, int type)
{
	GameArmtr *pGameArmtr = new GameArmtr();

	if (nullptr != pGameArmtr && pGameArmtr->init(pathName, dragonBoneName, armtrName, type))
	{
		pGameArmtr->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pGameArmtr);
		pGameArmtr = nullptr;
	}

	return pGameArmtr;
}

bool GameArmtr::init(const std::string & pathName, const std::string & dragonBoneName, const std::string & armtrName, int type)
{
	bool bRet = false;

	do
	{
		_type = type;

		__String *strConfigPath = __String::createWithFormat("%s/%s.ExportJson", pathName.c_str(), armtrName.c_str());
		ArmatureDataManager::getInstance()->addArmatureFileInfo(strConfigPath->getCString());

		_pArmtr = Armature::create(armtrName);
		this->addChild(_pArmtr);
		
		if (GameArmtrType_E::TYPE_MONSTER == _type)
		{
			_pArmtr->setScaleX(-0.2f);
			_pArmtr->setScaleY(0.2f);
		}
		else if (GameArmtrType_E::TYPE_ROLE == _type)
		{
//			_pArmtr->setScale(0.81f);
			_pArmtr->setScale(0.81f * ARMTR_SCALE_VALE);
		}
		else if (GameArmtrType_E::TYPE_FB == _type)
		{
			_pArmtr->setScale(0.8f);
		}

		bRet = true;
	} while (false);

	return bRet;
}


void GameArmtr::play(const std::string & animationName, int durationTo, int loop)
{
	_pArmtr->getAnimation()->play(animationName, durationTo, loop);
}
