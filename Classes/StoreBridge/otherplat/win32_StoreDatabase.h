#ifndef __WIN32_STORE_DATABASE_H__
#define __WIN32_STORE_DATABASE_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


typedef struct _Win32StoreItem
{
	__String itemId;
	__String productId;
	int itemType;	//0:virtual item;1:market item
}Win32StoreItem_T;


typedef struct _Win32GoodItem
{
	__String itemId;
	__String costItemId;
	__String giveItemId;
	int itemType;	//0:virtual item;1:market item
	int price;
	int giveCount;
}Win32GoodItem_T;


class Win32StoreDatabase: public cocos2d::CCObject
{
public:
	static Win32StoreDatabase *getInstance(void);

	int getItemBalance(string itemId);
	void giveItem(string itemId, int itemCount);
	void takeItem(string itemId, int itemCount);

	Win32GoodItem_T *getGoodItemInfo(string itemId);

private:
	Win32StoreDatabase();
	~Win32StoreDatabase();
    
    static Win32StoreDatabase *s_pInstance;
};

#endif  // __WIN32_STORE_DATABASE_H__
