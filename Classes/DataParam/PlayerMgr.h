#ifndef _PLAYER_MGR_H_
#define _PLAYER_MGR_H_

#include "cocos2d.h"

enum PlayerStage_E
{
	PLAYERSTAGE_ON = 2,
	PLAYERSTAGE_OFF = 1,
	PLAYERSTAGE_LOCK = 0
};

/*o is unequip, 1 is equip 2 is addNew*/
typedef std::function<void(int playeridx, int actiontype)> ccPlayerCallback;


class PlayerMgr
{
public:
	static PlayerMgr* getInstance();
	~PlayerMgr();

	void equipPlayer(int idx);
	void unEquipPlayer(int idx);
	void unLockPlayer(int idx);

	PlayerStage_E getPlayerStage(int playeridx);

	void addPlayerStageChangeObserver(ccPlayerCallback changeCallback);

	bool canEquipNewPlayer();
private:
	std::vector<ccPlayerCallback> _arrPlayerStageChangeObserver;
	static PlayerMgr* _inStance;
	PlayerMgr() {};

};
#endif
