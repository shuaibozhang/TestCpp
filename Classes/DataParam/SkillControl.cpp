#include "SkillControl.h"
#include "ParamData.h"
#include "ParamMgr.h"
#include "UserData.h"
#include "../WxSql/GLLocalStorage.h"

SkillControl* SkillControl::_instance = nullptr;

SkillControl::SkillControl()
{
	memset(_arrSkillId, -1, sizeof(_arrSkillId));
}

SkillControl::~SkillControl()
{
}

SkillControl * SkillControl::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SkillControl();
		_instance->loadSkillConfig();
	}
	return _instance;
}

void SkillControl::loadSkillConfig()
{
	for (int i = 0; i < 4; i++)
	{
		auto& sas = ParamMgr::getInstance()->getPlayerWAndSkillInfo(i);
		int j = 0;
		for (auto& w : sas._vectorSP)
		{
			SkillControl_T temp;
			temp.skillid = w.id;
			temp.flagmask = w.flag;
			temp.isEquip = UserData::getInstance()->getIsSkillEquip(i, temp.skillid, false) == 1 ? true : false;
			temp.islock = UserData::getInstance()->getItemBalance(String::createWithFormat("itemid_good_skill_%d", temp.skillid)->getCString(), false)  > 0 ? false : true;
			temp.pos = w.pos;
			temp.owner = i;
			_arrPlayerSkillControl[i].push_back(temp);

			if (temp.islock == false)
			{
				auto& skill = temp;
				if (_arrSkillId[skill.pos + i * 12] != -1)
				{
					if (_arrSkillId[skill.pos + i * 12] < skill.skillid)
					{
						_arrSkillId[skill.pos + i * 12] = skill.skillid;
					}
				}
				else
				{
					_arrSkillId[skill.pos + i * 12] = skill.skillid;
				}
			}	
		}
	}

	GLLocalStorage::glLocalStorageFree();
}

void SkillControl::equipSkill(int playeridx, int skillid)
{
	auto& curskillinfo = getPlayerSkillInfo(playeridx, skillid);

	auto& skillarr = _arrPlayerSkillControl[playeridx];
	for (auto& skillitem : skillarr)
	{
		if (skillitem.isEquip == true && curskillinfo.flagmask == skillitem.flagmask)
		{
			skillitem.isEquip = false;
			UserData::getInstance()->setSkillEquip(playeridx, skillitem.skillid, false, false);
			for (auto temp : _callBackMap)
			{
				temp.second(playeridx, skillitem.skillid, 0);
			}
		}
	}

	curskillinfo.isEquip = true;
	UserData::getInstance()->setSkillEquip(playeridx, curskillinfo.skillid, true, false);
	for (auto temp : _callBackMap)
	{
		temp.second(playeridx, skillid, 1);
	}
	UserData::getInstance()->saveUserData();
}

bool SkillControl::addSkillListener(Node * target, ccSkillEquipCallback callback)
{
	auto name = target->getName();
	CCASSERT(name.size() > 0, "you should set name to the node");
	CCASSERT(_callBackMap.find(name) == _callBackMap.end(), "you alread have add this name");
	
	_callBackMap.insert(std::make_pair(name, callback));
	return true;
}

void SkillControl::removeSkillListener(Node * target)
{
	auto name = target->getName();
	CCASSERT(name.size() > 0, "you should set name to the node");
	CCASSERT(_callBackMap.find(name) != _callBackMap.end(), "can not find listener");

	_callBackMap.erase(name);
}

bool SkillControl::checkIsEquipSkill(int playidx, int skillidx)
{
	auto& arr = _arrPlayerSkillControl[playidx];
	for (auto& temp : arr)
	{
		if (temp.skillid == skillidx)
		{
			return temp.isEquip;
		}
	}
	return false;
}

void SkillControl::addNewSkillWithoutBuy(int skillid)
{
	UserData::getInstance()->giveItem(String::createWithFormat("itemid_good_skill_%d", skillid)->getCString(), 1);
	addNewSkill(skillid);
}

void SkillControl::addNewSkill(int skillid)
{
	auto& tempinfo = getPlayerSkillInfo(skillid);
	bool needreequip = false;
	
	if (_arrSkillId[tempinfo.owner * 12 + tempinfo.pos] > skillid || isSkillLock(skillid) == false)
	{
		tempinfo.islock = false;
		return;
	}

	tempinfo.islock = false;

	int lastid = _arrSkillId[tempinfo.owner * 12 + tempinfo.pos];

	if ((_arrSkillId[tempinfo.owner * 12 + tempinfo.pos] != -1) 	&& checkIsEquipSkill(tempinfo.owner, lastid) == true
		)
	{
		needreequip = true;
	}

	_arrSkillId[tempinfo.owner * 12 + tempinfo.pos] = tempinfo.skillid;
	
	for (auto temp : _callBackMap)
	{
		temp.second(tempinfo.owner, skillid, 2);
	}

	if (needreequip)
	{
		equipSkill(tempinfo.owner, skillid);
	}
	//equipSkill(tempinfo.owner, skillid);
}

bool SkillControl::isSkillLock(int skillid)
{
	for (int i = 0; i < 4; i++)
	{
		auto& arr = _arrPlayerSkillControl[i];
		for (auto& item : arr)
		{
			if (skillid == item.skillid)
			{
				return item.islock;
			}
		}
	}

	return false;
}

int SkillControl::getSkillOwnerBySkillid(int skillid)
{
	for (int i = 0; i < 4; i++)
	{
		for (auto& temp : _arrPlayerSkillControl[i])
		{
			if (temp.skillid == skillid)
			{
				return temp.owner;
			}
		}
	}

	return -1;
}


SkillControl_T & SkillControl::getPlayerSkillInfo(int playidx, int skillid)
{
	auto& arr = _arrPlayerSkillControl[playidx];
	for (auto& item : arr)
	{
		if (skillid == item.skillid)
		{
			return item;
		}
	}
	CCASSERT(0, "canot find skill in this player");
}

SkillControl_T & SkillControl::getPlayerSkillInfo(int skillid)
{
	for (int i = 0; i < 4; i++)
	{
		auto& arr = _arrPlayerSkillControl[i];
		for (auto& item : arr)
		{
			if (skillid == item.skillid)
			{
				return item;
			}
		}
	}
	CCASSERT(0, "canot find skill in this player");
}

int SkillControl::getSkillPosByPlayerIdx(int playeridx, int skillid)
{
	auto& infoarr = _arrPlayerSkillControl[playeridx];
	for (int i = 0; i < infoarr.size(); i++)
	{		
		auto& info = infoarr[i];
		if (info.skillid == skillid)
		{
			return info.pos;
		}
	}
	return -1;
}

int SkillControl::getEquipSkillByPlayer(int playerIndex, int flagMask)
{
	auto &equSkillInfo = this->getPlayerSkillContralByPlayerId(playerIndex);
	for (int j = 0; j < equSkillInfo.size(); j++)
	{
		auto tmp = equSkillInfo.at(j);

		if (this->checkIsEquipSkill(playerIndex, tmp.skillid) && flagMask == tmp.flagmask)
		{
			return tmp.skillid;
		}
	}

	return -1;
}
