#ifndef __SKILLCONTROL_H__
#define __SKILLCONTROL_H__

#include "cocos2d.h"

USING_NS_CC;
/*o is unequip, 1 is equip, 2 is new unlock*/
typedef std::function<void(int playeridx, int skillid, int actiontype)> ccSkillEquipCallback;

struct SkillControl_T
{
	int skillid;
	bool islock;
	bool isEquip;
	int flagmask;	//0.normal skill 1.super skill 2.def skill
	int pos;
	int owner;
};

class SkillControl
{
public:
	~SkillControl();
	static SkillControl* getInstance();
	void loadSkillConfig();

	void equipSkill(int playeridx, int skillid);

	bool addSkillListener(Node* target, ccSkillEquipCallback callback);
	void removeSkillListener(Node* target);

	bool checkIsEquipSkill(int playidx, int skillidx);

	void addNewSkill(int skillid);
	void addNewSkillWithoutBuy(int skillid);
	bool isSkillLock(int skillid);

	int getSkillOwnerBySkillid(int skillid);

	const std::vector<SkillControl_T>& getPlayerSkillContralByPlayerId(int playeridx) { return _arrPlayerSkillControl[playeridx]; }
	SkillControl_T& getPlayerSkillInfo(int playidx, int skillid);
	SkillControl_T& getPlayerSkillInfo(int skillid);
	int getSkillPosByPlayerIdx(int playeridx, int skillid);

	int getEquipSkillByPlayer(int playerIndex, int flagMask); //flag 0.nor 1.super 2.def

	int _arrSkillId[48];
private:
	SkillControl();
	static SkillControl* _instance;
	std::map<std::string, ccSkillEquipCallback> _callBackMap;
	//SkillControl_T _arrSkillControl[48];	
	std::vector<SkillControl_T> _arrPlayerSkillControl[4];
};
#endif
