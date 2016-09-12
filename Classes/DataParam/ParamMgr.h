#ifndef __PARAM_MGR_H__
#define __PARAM_MGR_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

#define PARAM_MGR_STATEMENT(structName, funName, arrName)\
public: const static structName *get##funName(int id); 

#define PARAM_MGR_DEFINITION(structName, funName, arrName)\
extern structName arrName[];\
const structName *ParamMgr::get##funName(int id)\
{\
	return &arrName[id];\
}

#if 0
const structName *ParamMgr::get##funName(int id)\
{\
	CCASSERT(id < countName, "get##funName");\
	return &arrName[id];\
}
#endif


class ParamMgr
{
public:
	~ParamMgr();

	static ParamMgr *getInstance();

	void loadRoleArmtrInfo();
//	void loadMonsterArmtrInfo();
//	void loadSkillArmtrInfo();
	void loadWeakInfo();
	void loadEleSpecInfo();
	void loadMonsterInfo();
	void loadMonsterAttArmInfo();
	void loadStageInfo();
	void loadBoxDetailInfo();
	void loadDungeonInfo();
	void loadDungeonRewardInfo();
	/*simba*/
	void loadItemsConfig();
	void loadSkillsConfig();
	void loadWeaponConfig();
	void loadPlayerInfo();
	void loadWAndSkill();
	void loadShopAttr();
	void loadPosName();
	void loadStorePageOneConfig();
	void loadWenponUpConfig();

	void reloadStoreFreshItemsConfig();

	const DragonBoneInfo_T *getRoleArmtrInfo(int id);
//	const DragonBoneInfo_T *getMonsterArmtrInfo(int id);
//	const DragonBoneInfo_T *getSkillArmtrInfo(int id);
	const EleWeakInfo_T *getWeakInfo(int id);
	const EleSpecialInfo_T *getEleSpecInfo(int id);
	const MonsterInfo_T *getMonsterInfo(int id);
	const std::map<int, MonsterAttArmInfo_T> *getMonsterAttArmInfo(int id);
	const StageInfo_T *getStageInfo(int id);	//0.nor -1.endless -2.instance
	const BoxDetailInfo_T *getBoxDetailInfo(int id);

	const Skill_T  *getSkillByIdEx(int idx);

	const DungeonDeatilInfo_T *getDungeonInfo(int idx);
	const DungeonReward_T *getDungeonReward(int idx);

	/*simba*/
	const Item_T  & getItemByID(int idx);
	const Skill_T  & getSkillByID(int idx);
	const Weapon_T & getWeaponByID(int idx);
	const PlayerInfo_T& getPlayerInfo(int playeridx, int lv);

	inline const std::vector<Skill_T>& getSkillVector() { return _vectorSkills; }
	inline const std::vector<Weapon_T>& getWeaponVector() { return _vectorWeapons; }
	inline const std::vector<Item_T>& getItemVector() { return _vectorItems; }

	void getComGameItemInfo(int idx, ComInfo_T& cominfo);

	inline const PlayerWAndSkill_T& getPlayerWAndSkillInfo(int playeridx) { return _arrPlayerWAndSkill[playeridx]; }

	inline const std::vector<ShopAttr_T>& getShopAttr() {return _vectorShopAttr;}

	int getPlayerMaxLv() { return 49; }

	std::string getPosName(int pos);

	const std::vector<int>& getStoryPos() { return _vectorStoryPos; };

	const std::vector<BoxRewardConfig_T>& getBoxRewardConfig() { return _vectorBoxReward; };
	const std::vector<BoxItemConfig_T>& getBoxItemsConfig() { return _vectorItemsConfig; };

	void getBoxItems(int boxidx, std::vector<int>& itemids, std::vector<int>& nums);
	void getBoxItems(int boxidx, std::vector<PopItemInfo_T>& arrItems);

	const std::vector<StorePageOneItem_T>& getStorePageOneItems() { return _vectorPageOneItems; }
	const std::vector<std::vector<StorePageOnePosConfig_T> >& getStoreOneConfig() { return _vvectorBoxPosConfig; }

	std::vector<WenponLvUpInfo_T>& getWenponUpInfoByType(int type) { return _vectorArrWenponup[type]; }

	int _itemstartid;
	int _skillstartid;
	int _weaponstartid;

	void changeShowReliveWeight(int typeValue);	//0.lose 1.win 2.show
	CC_SYNTHESIZE_READONLY(int, _showReliveWeight, ShowReliveWeight);
private:
	static ParamMgr *s_pInstance;
	vector<DragonBoneInfo_T *> _arrRoleArmtrInfo;
//	vector<DragonBoneInfo_T *> _arrMonsterArmtrInfo;
	vector<DragonBoneInfo_T *> _arrSkillArmtrInfo;
	vector<EleWeakInfo_T *> _arrWeakInfo;
	vector<EleSpecialInfo_T *> _arrEleSpecInfo;
	vector<MonsterInfo_T *> _arrMonsterInfo;
	std::vector<std::map<int, MonsterAttArmInfo_T>> _arrMonsterAttArmInfo;
	vector<StageInfo_T *> _arrStageInfo;
	StageInfo_T *_pUnStopStageInfo;
	StageInfo_T *_pInstanceStageInfo;
	/*simba*/
	std::vector<Item_T> _vectorItems;
	std::vector<Skill_T> _vectorSkills;
	std::vector<Weapon_T> _vectorWeapons;
	std::vector<PlayerInfo_T>_vectorPlayerInfo[4];
	PlayerWAndSkill_T _arrPlayerWAndSkill[4];

	std::vector<ShopAttr_T> _vectorShopAttr;
	std::map<int, std::string> _posnameMap;
	std::vector<int>_vectorStoryPos;
	ParamMgr();

	std::vector<BoxRewardConfig_T> _vectorBoxReward;
	std::vector<BoxItemConfig_T> _vectorItemsConfig;
	
	std::vector<WenponLvUpInfo_T> _vectorArrWenponup[4];
	
	std::vector<StorePageOneItem_T> _vectorPageOneItems;

	std::vector<std::vector<StorePageOnePosConfig_T> > _vvectorBoxPosConfig;
	
	std::vector<BoxDetailInfo_T *> _arrBoxDetailInfo;

	std::vector<DungeonDeatilInfo_T *> _arrDungeonInfo;
	std::vector<DungeonReward_T *> _arrDungeonReward;
};


#endif