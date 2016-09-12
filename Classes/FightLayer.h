#ifndef __FIGHT_LAYER_H__
#define __FIGHT_LAYER_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

class Monster;

class FightLayer : public Layer
{
public:
	static FightLayer *getInstance();

	FightLayer();
	virtual ~FightLayer();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	CREATE_FUNC(FightLayer);

	Vector<Monster *> *getCurArrMonster();
	Vector<Monster *> *getIdleArrMonster();
	
	virtual void update(float delta);

	void startFight();
	bool createFloorMonster(int floorIndex);
	void addIdleToCurMonster();
	void moveRoleToNextPos();
	void moveRoleToOutSidePos(int moveType);	//0.all 1.role 2.bg
	void enterNewFloor();
	void stopFight();

	bool isFirstWave();
	bool isLastWave();
	
	Monster *getFirstMonster();
	void removeMonster(Monster *pMonster);

	void countDungeonDes(int id, int count);	//-1.move 0.lsj 1.cbd 2.lqc 3.sqy 4.yaosshi 5.shitou 6.bingkuai 7.bingfeng 8.jingji
	int getCurDungeonDesCount(int id);
	int getDungeonDesCount(int id);

	void startRound(int roundType);	//0.first

	void showValueChange(int type, float damage, const Vec2 &pos);	//0:role 1:monster //2.role def
//	void notifyMonsterFight();
	void showFightTips(int tipType, int value, const Vec2 &monsterPos);	//0.exp	1.crit 2.def
	
//	CC_SYNTHESIZE(bool, _isFighting, IsFighting);
	CC_SYNTHESIZE(FightState, _curFightState, CurFightState);
	CC_SYNTHESIZE_READONLY(int, _curWaveIndex, CurWaveIndex);
	CC_SYNTHESIZE_READONLY(int, _curPlayerPosId, CurPlayerPosId);
	CC_SYNTHESIZE(int, _totalExp, TotalExp);
	CC_SYNTHESIZE(int, _gameState, GameState);	//0.wait 1.fighting 2.win 3.lose 4.settle 5.wait relive
	CC_SYNTHESIZE(int, _totalGold, TotalGold);
	CC_SYNTHESIZE_READONLY(const StageInfo_T *, _pStageInfo, StageInfo);
	CC_SYNTHESIZE_READONLY(const DungeonDeatilInfo_T *, _pDungeonDeatilInfo, DungeonDeatilInfo);
	CC_SYNTHESIZE_READONLY(int, _endlessFloor, EndlessFloor);
	CC_SYNTHESIZE(int, _openBoxCount, OpenBoxCount);
	CC_SYNTHESIZE(int, _boxMonsterWaveIdx, BoxMonsterWaveIdx)
private:
	int getPlayerPosIndex();	//from 0
	int getTotalPosCount();

	static FightLayer *s_pInstance;
	Vector<Monster *> _arrCurMonster;
	Vector<Monster *> _arrIdleMonster;
	/*wave info*/
	const std::vector<std::vector<std::vector<int>>> *_arrMonsterWaveInfo;
	int _curFloor;
	int _curDungeonStep;
	int _arrDungeonAnim[4];
};



#endif