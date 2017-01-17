#ifndef __PARAMDATA_H__
#define __PARAMDATA_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace std;
using namespace cocostudio;


typedef enum _EntityZ_E
{
	E_Z_BG = 0,
	E_Z_CRUSH_LAYER,
	E_Z_FIGHT_LAYER,
	E_Z_UI,
	E_Z_GUIDE,
//	E_Z_MONSTER,
//	E_Z_ROLE,
//	E_Z_EFFECT,
}EntityZ_E;


typedef enum _ZInCrush
{
	Z_C_BG = 0,
	Z_C_ICON,
	Z_C_EFFECT
}ZInCrush_E;


typedef enum _ZInFight
{
	Z_F_BG = 0,
	Z_F_FLY_ICON,
	Z_F_EFFECT,
	Z_F_UI
}ZInFight_E;


typedef enum _EleIconId
{
	ELE_ID_INVALID = -2,
	ELE_ID_EMPTY = -1,
	ELE_ID_0,
//	ELE_ID_SWORD = ELE_ID_0,
	ELE_ID_1,
	ELE_ID_2, 
	ELE_ID_3,
//	ELE_ID_HEART = ELE_ID_3,
	ELE_ID_ROLE_COUNT,
	ELE_ID_SUPER = ELE_ID_ROLE_COUNT,
	/*special*/
 	ELE_ID_BOOM,
 	ELE_ID_VOLCANO,
 	ELE_ID_THUNDER,
 	ELE_ID_STONE,
 	ELE_ID_THORN_EYE,
 	ELE_ID_THORN,
 	ELE_ID_ICE,
	ELE_ID_RECOVER,
	ELE_ID_KEY,
	ELE_ID_SPEC_LINE,
	ELE_ID_SLATE = ELE_ID_SPEC_LINE,
	ELE_ID_SLATE_EX,	//can't fall
	//keep it
//	ELE_ID_COUNT
}EleIconId_E;


typedef enum _RoleId
{
	ROLE_ID_LSJ = 0,
	ROLE_ID_CBD,
	ROLE_ID_LQC,
	ROLE_ID_SQY,
	ROLE_ID_QYL,
	ROLE_ID_COUNT
}RoleId_E;


typedef enum _RoleAnimIndex
{
	RA_IDLE = 0,
	RA_MOVE,
	RA_ATK,
	RA_SKILL,
	RA_CHEER,
	RA_ADD,
	RA_DEF,
}RoleAnimIndex_E;

typedef enum _AttAttrbt
{
	ATT_FIRE = 0,
	ATT_THUNDER,
	ATT_ICE,
	ATT_NORMAL = 3,
	ATT_NARROW,
	ATT_CONFUSE,
	ATT_DRUG,
//	ATT_ABSORB_DP_HP,
	ATT_ABSORB_HP,
	//keep it
	ATT_COUNT
}AttAttrbt_E;

enum class EleWeakType : int
{
	FIRE = 0,
	ICE,
	STONE
};

enum class EleSpecialType : int
{
	BOOM = 0,
	VOLCANO,
	THUNDER,
	STONE,
	THORN_EYE,
	THORN,
	ICE,
	RECOVER,
	KEY,
	SLATE
};
/*
enum class GameState : int
{
	WaitTouch = 0,
	WaitMove,
	SwapAnim,
	CrushAnim,
	CreateAnim,
	FallAnim,
};*/


enum class PlayerState : int
{
	IDLE = 0,
	MOVE,
	ATTACK,
};


enum class FightState : int
{
	InMain = 0,
	Idle,
	Move,
	Fight,
	WaitSelect,
};


class ParamData
{
public:
	static const int CRUSH_ROW = 7;
	static const int CRUSH_COL = 8;
	static const int CRUSH_ARR_COUNT = (CRUSH_ROW * CRUSH_COL / 3) + 1;
	static const float ELE_BG_WIDTH;
	static const float ELE_BG_HEIGHT;
	static const float ELE_SWAP_DUR;
	static float CRUSH_BG_HEIGHT;
	static const float POS_X_INTERVAL;
	static const float ROLE_START_POS_X;
	static float ROLE_START_POS_Y;
	static const float MONSTER_START_POS_X;
	static float MONSTER_START_POS_Y;
	static float FIGHT_BG_HEIGHT;
	static const float WEAK_ROUND_DAMAGE_PARAM;
	static float FIGHT_LAYER_POS_Y;

	static const int ROLE_COUNT = 5;
	static const int FIGHT_ROLE_COUNT = 4;
	static const int EQUIP_ITEM_MAX_COUNT = 4;
	static const int BOX_LAYER_SEAT_COUNT = 4;

	static const int SKILL_COUNT = 12 * ROLE_COUNT;
	static const int WENPON_COUNT = 10 * ROLE_COUNT;

	static const float ELE_CRUSH_DUR;
	static const float ELE_CRUSH_WAIT;
//	static const float ELE_CREATE_DUR;
	static const float ELE_CREATE_WAIT;
	static const float ELE_FALL_DUR;
	static const float ELE_FLY_DUR;

	static const int FORBID_COUNT = 3;
	static const int GUIDE_NODE_TAG = 800;

	static const int BOX_COUNT = 26;// 7;

//	static const int ELITE_STAGE_ADD_LV = 2;
	static const int ENDLESS_STAGE_ADD_LV = 4;

	static const int GOLD_ITEM_ID = -100;
	static const int CRYSTAL_ITEM_ID = -101;	
	static const int TILI_ITEM_ID = -102;

	static const int ACHIEVE_TOTAL_COUNT = 6;
	static const int MagPie_TOTAL_TASKS_NUM = 7;

	static const int BOX_CATEGORY_COUNT = 9;// 7;
	static const int BOX_TOTAL_LV = 5;

	static const int HEAD_TOTAL_COUNT = 6;
	static const int LOAD_TIPS_COUNT = 14;

	static const int ONLINE_TIME_REWARD_NUM = 8;
	static const int ONLINE_TEACHER_REWARD_NUM = 6;

	static const int MAX_TILI = 50;

	static const int P_POPBUY = 40;
};


typedef struct _CrushIndex_T
{
	int row;
	int column;
}CrushIndex_T;

typedef struct _CrushArr_T
{
	bool isTouchEle;
	bool isFinish;
	int crushType; //0:3;1:T or L;2: line 5;3:super swap;4:weak crush
	int crushCount;
	int crushEleId;
//	int attLv;	//0:nor 1:skill 2:enhance skill 3:super 4:super skill 5:super enhance 6:super super
	int arrLvCount[3];	//per lv count arrLvCount[0].lv0 1.lv1 2.lv2
	CrushIndex_T desIndex;
}CrushArr_T;

typedef struct _TravelseInfo_T
{
	int crushArrCount;
	CrushIndex_T touchIndex;
	CrushIndex_T moveIndex;
	int arrEleCount[4];
	bool arrIsTravelsed[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];
	//	bool arrIsLinkWeak[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];
	int arrEleId[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];
	int arrEleWeakState[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];	//0:normal
	int arrCrushArrIndex[ParamData::CRUSH_ROW][ParamData::CRUSH_COL];
	CrushArr_T arrCrushDetail[ParamData::CRUSH_ARR_COUNT];
}TravelseInfo_T;


typedef struct _DragonBoneInfo_T
{
	string strPathName;
	string strDragonBoneName;
	string strArmtrName;
}DragonBoneInfo_T;


typedef struct _EleWeakInfo_T
{
	int id;
	int lv;
	int weight;
	int totalCrush;
	EleWeakType type;
	bool isCanCrushClean;	//clean by crush
//	bool isCanClean;	//clean by skill or item
	bool isCanCrush;	//ice can't crush
	bool isHostAvail;	//after crush, whether avail
	bool isCanFall;
	int frameCount;
	std::string animName;
}EleWeakInfo_T;


typedef struct _EleSpecialInfo_T
{
	int id;
	int totalRound;
	int totalCrush;
	int lv;
	EleSpecialType type;
	bool isCanFall;
	bool isNeedDealBottom;
	bool isNeedChangeAnim;
	int attTime;
	int attType;
	int attParam1;
	int attParam2;
	float attParam3;
	int frameCount;
	std::string animName;
}EleSpecialInfo_T;


typedef struct _AttInfo_T
{
	int eleId;
	int totalCount;
	int eleLvCount[3];
}AttInfo_T;


typedef struct _RoleEquInfo_T
{
	int weaponId;
	int skillId[3];		//0:skill 2:super 3:other skill
}RoleEquInfo_T;


typedef struct _MonsterAttInfo_T
{
	int next;
	int round;
	int attId;
	int attCount;
}MonsterAttInfo_T;


typedef struct _DropItem_T
{
	int itemId;
	float pro;
}DropItem_T;


typedef struct _DefAttrbt_T
{
	int defId;
	float hurt;
}DefAttrbt_T;


typedef struct _MonsterInfo_T
{
	int id;
	int type;	//0.nor 1.boss 2.box
	int lv;
	int animId;
	float attRange[2];
	int goldRange[2];
	float hp;
	int exp;
	int designType;	//0.nor 1.elite 2.boss
	int rAttMCount;
	int mAttRCount;
	float runPro;
	float fScale;
	float designSize;
	std::string animName;
	std::vector<MonsterAttInfo_T> arrAttInfo;
	std::vector<DefAttrbt_T> arrDefInfo;
	std::vector<DropItem_T> arrDropInfo;
	std::string name;
}MonsterInfo_T;


typedef struct _Items
{
	int id;
	std::string name;
	std::string des;
	int type;
	int value;
	int price;
	int paytype;
	std::string picname;
}Item_T;


typedef struct _Weapon
{
	int id;
	std::string name;
	std::string des;
	int type;
	int attack;
	int def;
	int dpadd;
	int hpadd;

	int lvMinAttack;
	int lvMinDef;
	int lvMinDpadd;
	int lvMinHpadd;

	int lvMaxAttack;
	int lvMaxDef;
	int lvMaxDpadd;
	int lvMaxHpadd;

	int price;
	int paytype;
	std::string picname;
	int equipMinLv;
}Weapon_T;


typedef struct _Skill
{
	int id;
	std::string name;
	std::string des;
	int starNum;
	int attribute;	//ice fire thunder 
//	int classtype;	//value type
	int price;
	int paytype;
	int mark;	//1.damage 2.add hp 3. add dp 4.narrow 5.confuse 6.shield 7.clean 8. add lv 9.relive 10.clean all 11.absorb 12.drug 13.add att 14.dp att
	float normalVal[3];
	float enhanceVal[3];
	/*add by zhou*/
	bool isNeedMove;
	bool isHurtAll;
	std::string icon;
	std::string roleAnim;
	std::string skillAnim;
	std::string buffAnim;
}Skill_T;


typedef struct _ComInfo
{
	int id;
	int paytype;
	int price;
	int value;
	std::string name;
	std::string des;
	std::string picname;	
}ComInfo_T;

typedef struct _PlayerInfo
{
	int hp;
	int dp;
	int exp;
	int attack;
	/*def*/
	int def;
	int dpadd;
	/*cure*/
	int hpadd;
}PlayerInfo_T;

/***********************************************************************************/
struct WenponIDAndPos
{
	int id;
	int pos;
};

struct SkillIDAndPos
{
	int id;
	int pos;
	int flag;
};

typedef struct _PlayerWAndSkill
{
	std::vector<WenponIDAndPos> _vectorWP;
	std::vector<SkillIDAndPos> _vectorSP;
}PlayerWAndSkill_T;

typedef struct _ShopAttr
{
	int pos;
	std::vector<int> _skillsconfig;
	std::vector<int> _itemsconfig;
	std::vector<int> _weaponsconfig;
}ShopAttr_T;
/***********************************************************************************/

class Monster;
typedef struct _RoleCurAttInfo
{
	//	bool isNeedBack;
	//	bool isAllHurt;
	int skillId;
	int attCount;
	int curAttCount;
	const Skill_T *pSkillInfo;
	Monster *pDesMonster;
	int skillParam2;	//round count
	float skillParam1;	//damage or add
	float skillParam3;	//round damage or add
	Vec2 desPos;
}RoleCurAttInfo_T;


typedef struct _MonsterAttArmInfo
{
	std::string roleAct;
	std::string skillAnim;
	std::string buffAnim;
	std::string soundName;
}MonsterAttArmInfo_T;


typedef struct _StageInfo
{
	int id;
	int rewardgold;
	int rewardexp;	
	std::vector<string> arrFloorName;	//every floor name
	std::vector<std::vector<std::vector<int>>> arrMonsterWaveInfo;	//every wave monster info
	std::string stageName;
	std::string stageDesc;
	int energyCost;
	int stageType;
	float arrBoxChance[ParamData::BOX_COUNT];
	int boxmonsId;
	int boxmonsBoxid;
	int roleLv;
	int arrWeaponId[ParamData::FIGHT_ROLE_COUNT];
	int arrWeaponLv[ParamData::FIGHT_ROLE_COUNT];
	int goldPer;
	int expPer;
}StageInfo_T;


enum class LoadResType : int
{
	PNG,
	JPG,
	PLIST_PNG,
	PLIST_PVR_CCZ,
	ARMTR,
	PARTICLE
};


typedef struct _LoadResInfo
{
	LoadResType resType;
	std::string filePath;
	std::string strKey;
}LoadResInfo_T;

typedef struct _ForbidInfo
{
	int cloIdx;
	int eleId;
}ForbidInfo_T;

typedef struct _BoxRewardConfig
{
	std::vector<int> itemtypeidx;
	std::vector<int> probability;
	std::vector<int> nums;
	std::vector<int> gettypenums;
}BoxRewardConfig_T;

typedef struct _BoxItemConfig
{
	int typeidx;
	std::vector<int> probabilities;
	std::vector<int> itemids;
}BoxItemConfig_T;

typedef struct _WenponLvUpInfo
{
	int type;
	int lv;
	int costnum;
	int costtype;
	int costparts;
	float upp;
}WenponLvUpInfo_T;

typedef struct _AttactInfo
{
	int attack;
	int def;
	int dpAdd;
	int hpAdd;

	bool operator==(const _AttactInfo & other);
}AttactInfo_T;

typedef struct _PopItemInfo
{
	int itemId;
	int itemCount;
	int itemLv;
	int popIdx;
}PopItemInfo_T;

typedef struct _UserAchieveInfo
{
	int curCount;
	int curLv;
}UserAchieveInfo_T;

typedef struct _UserMagPieInfo
{
	bool isGotReward;
	int curFinishedGoalNum;
}UserMagPieInfo_T;

typedef struct _BoxCategoryInfo
{
	int id;
	int arrGiveCount[ParamData::BOX_TOTAL_LV];
	int giveTypeCount;
	int totalPercent;
	int itemTypeCount;
	int arrItemId[4];
	int arrItemWeight[4];
}BoxCategoryInfo_T;

typedef struct _BoxDetailInfo
{
	int id;
	BoxCategoryInfo_T arrCatgInfo[ParamData::BOX_CATEGORY_COUNT];
	int waitTime;
	int openCost;
	int buyCost;
}BoxDetailInfo_T;

typedef struct _DungeonDesInfo
{
	int crushId;
	int crushCount;
}DungeonDesInfo_T;

typedef struct _DungeonDeatilInfo
{
	int id;
	int infoType;
	int stepCount;
	int initContentId;
	std::vector<int> arrInitPosX;
	std::vector<int> arrInitPosY;
	std::vector<DungeonDesInfo_T> arrDesInfo;
	std::string strSaying;
}DungeonDeatilInfo_T;

typedef struct _DungeonReward
{
	int level;
	int exp;
	int gold;
	int equip;
	int equipLv;
}DungeonReward_T;


/***************************Rank Struct Begin*********************************************/
const int RANK_COUNT = 2;
const int RANK_ROLE_COUNT = 30;

typedef struct _RankBaseData
{
	int nRankId;
	int nIndex;	//start form 1
	int nRoleId;
	int nLevel;
	int nValue;
	int nAward;	//-1:invalid
				//	CCString strAccount;
				//	CCString strNick;
				//	CCString strDate;
	string strAccount;
	string strNick;
	string strDate;
public:
	_RankBaseData()
	{
		nRankId = 0;
		nIndex = 0;
		nRoleId = 0;
		nLevel = 0;
		nValue = 0;
		nAward = 0;
		strAccount = "";
		strNick = "";
		strDate = "";
	}
}RankBaseData_T;


typedef struct _RankRoleInfo
{
	string strAccount;
	string strNick;
	RankBaseData_T arrCurRank[RANK_COUNT];
	RankBaseData_T arrHistoryRank[RANK_COUNT];
public:
	_RankRoleInfo()
	{
		strAccount = "";
		strNick = "";
	}
}RankRoleInfo_T;


typedef struct _RankRealInfo
{
	//	RankBaseData_T arrRoleRank[RANK_COUNT];
	RankBaseData_T arrTopRank[RANK_COUNT][RANK_ROLE_COUNT];
}RankRealInfo_T;


typedef struct _RankSettleInfo
{
	string strDate;
	RankBaseData_T arrRoleRank[RANK_COUNT];
	RankBaseData_T arrTopRank[RANK_COUNT][RANK_ROLE_COUNT];
}RankSettleInfo_T;

typedef struct _StorePageOneItem
{
	int id;
	int itemid;
	std::string des;
	int num;
	int price;
	int type;
}StorePageOneItem_T;

typedef struct _StorePageOnePosConfig
{
	int pageoneitemidx;
	int p;
}StorePageOnePosConfig_T;

typedef struct _BoxInStoreConfig
{
	int boxidx;
	int boxlv;
}BoxInStoreConfig_T;

/***************************Rank Struct End*********************************************/


typedef struct _DesignRoleInfo
{
	int perGold;
	int perExp;
	float hp;
	float dp;
	float att;
	float def;
	float dpAdd;	//defRecover
	float hpAdd;
}DesignRoleInfo_T;

typedef struct _HotStoreWenpon_T
{
	int wenponpartid;
	std::string name;
	int num;
	int price;
	int pricetype;
}HotStoreWenpon_T;


typedef enum _ExchangeType_E
{
	EXCHANGE_LV_1 = 1,
	EXCHANGE_LV_2,
	EXCHANGE_LV_3,
	EXCHANGE_DIAMOND,
	EXCHANGE_MAGPIE,
	EXCHANGE_NEW_VER,
	EXCHANGE_FOLLOW,
	EXCHANGE_GUOQING,
	//keep it
	EXCHANGE_TMP,
	EXCHANGE_COUNT = EXCHANGE_TMP - 1
}ExchangeType_E;


#endif
