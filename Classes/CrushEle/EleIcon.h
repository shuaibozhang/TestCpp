#ifndef __ELEICON_H__
#define __ELEICON_H__

#include "cocos2d.h"
#include "ParamData.h"

USING_NS_CC;

class RoundTag;
class GameArmtr;

class EleIcon : public Node
{
public:
	EleIcon();
	virtual ~EleIcon();

	static EleIcon *create(int eleId, int eleLv);

	virtual bool init(int eleId, int eleLv);

	void doShow();
	bool doCrush();
	bool doClean(int lv, float animDelay = 0.f);
	bool doRound();
	bool doInBottom();
	bool doHit();
	void doAddLv(int type, int lv=-1, float animDelay = 0.f);	//0.crush and add 1.add
	void playMovePerUnit(int fallType, float delayTime, float durTime, int dstRow, int dstColum);	//-1.swap 0.fall
	bool obtainWeak(int weakId);
	void doCleanStone();
	void doRemove(float delayTime = 0.f, bool isCleanup = false);

	void createFlyWeak(int weakId, int roleIndex, float param);	//0.fire 1.boom 2.thunder

	bool isCrushEffective();
	bool isNeedDealBottom();
	bool isCanFall();
	bool isCanClean(int lv);

	float playCrushAnim(bool isStone);
	float playCleanAnim(float animDelay = 0.f, int animIndex = 0);
	void playSelAnim();
	void stopSelAnim();
	void playCrushEffect(int index);

	void changeEle(int newId, int newLv);

	void setPosIndex(int row, int colum);
	void setTotalRound(int totalRound);

	void animationEffectEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	CC_SYNTHESIZE(int, _eleId, EleId);
	CC_SYNTHESIZE(int, _eleLv, EleLv);
	CC_SYNTHESIZE(bool, _isActive, IsActive);
	CC_SYNTHESIZE(CrushIndex_T, _posIndex, PosIndex);
	CC_SYNTHESIZE_READONLY(int, _curRound, CurRound);
	CC_SYNTHESIZE_READONLY(const EleWeakInfo_T *, _pWeakInfo, WeakInfo);
	CC_SYNTHESIZE_READONLY(const EleSpecialInfo_T *, _pSpecInfo, SpecInfo);
	CC_SYNTHESIZE(bool, _isCanTouch, IsCanTouch);
private:
	void doSpecAtt();

	void updateModelSprite();
	void updateWeakSprite();
	void updateSpecSprite();

	void removeWeak();
	void removeSpec();

	void playCreateEffect();
//	void playCrushEffect();
	void playBuffEffect(int index);	//0 clean1 1.addlv 2.clean2
	void playRoundEffect();
	bool isNeedAddWeakArr();
	void updateEleBg();
	void updateEleKeyBgPos(float dt);

private:
	static const int SEL_ANIM_TAG = 101;
	GameArmtr *_pModel;
	RoundTag *_pRound;
	Sprite *_pWeakSprite;
//	int _curWeakLv;
	int _curCrushCount;
	int _stoneEleId;
	int _totalRound;
	Sprite *_pEleBg;
	Sprite *_pArrowTip;
};



#endif
