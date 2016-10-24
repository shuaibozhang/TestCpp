#ifndef __BAGITEMCONTROL_H__
#define  __BAGITEMCONTROL_H__

#include "cocos2d.h"

typedef std::function<void(int itemid, int numchange, int bagpos, int pageidx)> ccBagItemChangeCallback;
typedef std::function<void(int startidx, int endidx, int pageidx, int type)> ccBagItemPosCallback;

const static int g_pagecount = 5;
const static int g_onepagetnum = 14;

class BagItemControl
{
public:
	static BagItemControl* getInstace();
	~BagItemControl();

	void delBagItem(int posidx, int page);
	int addBagItem(int itemid);
	int addBagItem(int itemid, int pos, int page);
	bool isNeedUpdata() { return _needupdata; }

	int getItemConfig(int posidx, int page);
	void setItemConfig(int posidx, int itemdi, int page);
	void changePos(int startpos, int endpos, int page);
	inline bool IsBagFull() { return _isbagFull; }
	void init();
	void upDataNeedShowItems();

	int getNeedShowItemOne();

	/*0 is hp... ,1 is skill, 2 iswenpon ,3 is wenparts, -102 is tili*/
	int checkItemType(int itemid);
	/*0 is n, 1 is xiyou*/
	int checkWenponPartsType(int itemid);

	void saveItemConfig();

	void autoEquipItems();
	void autoEquipInFight();
	int getItemsType(int itemid);

	int getEquipItemsByIdx(int idx) { return _bagEquipConfig[idx]; }

	ccBagItemChangeCallback _callback;
	ccBagItemPosCallback _posCallback;

	bool isInBag(int itemid);
private:
	static BagItemControl* _instacne;
	BagItemControl();
	bool _needupdata;
	
	int _bagItemConfig[g_pagecount][g_onepagetnum];

	int _bagEquipConfig[4];
	bool _isbagFull;
	std::queue<int> _curNewItemQueue;
	std::vector <int> _needShowtemp;
	//int _pageidx;
};
#endif
