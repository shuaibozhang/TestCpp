#include "BagItemControl.h"
#include "SkillControl.h"
#include "WeaponControl.h"
#include "UserData.h"

BagItemControl* BagItemControl::_instacne = nullptr;

BagItemControl * BagItemControl::getInstace()
{
	if (_instacne == nullptr)
	{
		_instacne = new BagItemControl();
		_instacne->init();
	}
	return _instacne;
}

bool BagItemControl::isInBag(int itemid)
{
	for (int i = 0; i < g_pagecount; i++)
	{
		for (int j = 0; j < g_onepagetnum; j++)
		{
			if (_bagItemConfig[i][j] == itemid)
			{
				return true;
			}
		}
	}
	return false;
}

BagItemControl::BagItemControl():_needupdata(false),
	_callback(nullptr),
	_posCallback(nullptr),
	_isbagFull(false)
{
	memset(_bagItemConfig, 0, sizeof(_bagItemConfig));
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			_bagItemConfig[j][i] = -1;
		}
	}
	
	for (auto& config : _bagEquipConfig)
	{
		config = -1;
	}
}

BagItemControl::~BagItemControl()
{

}

void BagItemControl::delBagItem(int posidx, int page)
{
	if (posidx < g_onepagetnum)
	{
		int bak = _bagItemConfig[page][posidx];
		_bagItemConfig[page][posidx] = -1;
		_needupdata = true;
		if (_callback)
		{
			_callback(bak, -1, posidx, page);
		}
		_isbagFull = false;
	}
	else
	{
		int bak = _bagEquipConfig[posidx - g_onepagetnum];
		_bagEquipConfig[posidx - g_onepagetnum] = -1;

		_needupdata = true;
		if (_callback)
		{
			_callback(bak, -1, posidx, page);
		}
	}
	

	saveItemConfig();
}

int BagItemControl::addBagItem(int itemid)
{
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (_bagItemConfig[j][i] == -1)
			{
				_bagItemConfig[j][i] = itemid;
				_needupdata = true;

				if (_callback)
				{
					_callback(itemid, 1, i, j);
				}
				if (i == (g_onepagetnum - 1) && j == (g_pagecount -1))
				{
					_isbagFull = true;
				}

				//saveItemConfig();
				return i;
			}
		}
	}
	
	_isbagFull = true;
	return -1;
}

int BagItemControl::addBagItem(int itemid, int pos, int page)
{
	if (_bagItemConfig[page][pos] == -1)
	{
		_bagItemConfig[page][pos] = itemid;
		_needupdata = true;

		if (_callback)
		{
			_callback(itemid, 1, pos, page);
		}

		//saveItemConfig();
		return pos;
	}
	_isbagFull = true;
	return -1;
}

int BagItemControl::getItemConfig(int posidx, int page)
{
	if (posidx >= g_onepagetnum)
	{
		return _bagEquipConfig[posidx - g_onepagetnum];
	}
	return _bagItemConfig[page][posidx];
}

void BagItemControl::setItemConfig(int posidx, int itemdi, int page)
{
	if (posidx >= g_onepagetnum)
	{
		_bagEquipConfig[posidx] = itemdi;
	}
	else
	{
		_bagItemConfig[page][posidx] = itemdi;
	}
}

void BagItemControl::changePos(int startpos, int endpos, int page)
{
	int type = 0;
	if (startpos < g_onepagetnum && endpos <g_onepagetnum)
	{
		auto temp = _bagItemConfig[page][startpos];
		_bagItemConfig[page][startpos] = _bagItemConfig[page][endpos];
		_bagItemConfig[page][endpos] = temp;
		type = 0;
	}
	else if(startpos >= g_onepagetnum && endpos >= g_onepagetnum)
	{
		auto temp = _bagEquipConfig[startpos - g_onepagetnum];
		_bagEquipConfig[startpos - g_onepagetnum] = _bagEquipConfig[endpos - g_onepagetnum];
		_bagEquipConfig[endpos - g_onepagetnum] = temp;
		type = 3;
	}
	else if (startpos < g_onepagetnum && endpos >= g_onepagetnum)
	{
		auto temp = _bagItemConfig[page][startpos];
		_bagItemConfig[page][startpos] = _bagEquipConfig[endpos - g_onepagetnum];
		_bagEquipConfig[endpos - g_onepagetnum] = temp;
		type = 1;
	}
	else if (startpos >= g_onepagetnum && endpos < g_onepagetnum)
	{
		auto temp = _bagEquipConfig[startpos - g_onepagetnum];
		_bagEquipConfig[startpos - g_onepagetnum] = _bagItemConfig[page][endpos];
		_bagItemConfig[page][endpos] = temp;
		type = 2;
	}


	if (_posCallback)
	{
		_posCallback(startpos, endpos, page, type);
	}

	saveItemConfig();
}

void BagItemControl::init()
{
	_isbagFull = true;
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			_bagItemConfig[j][i] = UserData::getInstance()->getItemPosIdx(i, j);
		}

		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (_bagItemConfig[j][i] != -1)
			{
				_isbagFull = false;
				break;
			}
		}
	}
	
	int idx = 0;
	for (auto&config : _bagEquipConfig)
	{
		config = UserData::getInstance()->getEquipConfig(idx);
		idx++;
	}
}

void  BagItemControl::upDataNeedShowItems()
{
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			auto item = _bagItemConfig[j][i];
			if (item != -1 && std::find(_needShowtemp.begin(), _needShowtemp.end(), item) == _needShowtemp.end())
			{
				if (checkItemType(item) == 1)
				{
					if (SkillControl::getInstance()->isSkillLock(item) == true)
					{
						_curNewItemQueue.push(item);
						_needShowtemp.push_back(item);
					}
				}
				else if (checkItemType(item) == 2)
				{
					if (WeaponControl::getInstance()->isWeaponUnLock(item) == false)
					{
						_curNewItemQueue.push(item);
						_needShowtemp.push_back(item);
					}
				}
			}
		}
	}
}

int BagItemControl::getNeedShowItemOne()
{
	upDataNeedShowItems();

	if (_curNewItemQueue.empty())
	{
		_needShowtemp.clear();
		return -1;
	}
	
	int idx = _curNewItemQueue.front();
	_curNewItemQueue.pop();

	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			if (_bagItemConfig[j][i] == idx && checkItemType(idx) == 1)
			{
				SkillControl::getInstance()->addNewSkill(idx);
				delBagItem(i, j);
				break;
			}
			else if (_bagItemConfig[j][i] == idx && checkItemType(idx) == 2)
			{
				WeaponControl::getInstance()->addNewWeapon(idx);
				delBagItem(i, j);
				break;
			}
		}
	}
	return idx;
}

int BagItemControl::checkItemType(int itemid)
{
	if (itemid >= 1500 && itemid < 2000)
	{
		return 3;
	}
	if (itemid >= 1000)
	{
		return 0;
	}
	else if (itemid >= 500)
	{
		return 2;
	}
	else if (itemid >= 0)
	{
		return 1;
	}

	return itemid;
}


int BagItemControl::checkWenponPartsType(int itemid)
{
	int idx = (itemid - 1500) % 10;
	int type[] = { 9,6,3 };
	int curtype = 0;
	int count = sizeof(type) / sizeof(int);

	for (int i = 0; i < count; i++)
	{
		if (idx >= type[i])
		{
			curtype = count - i;
		}
	}

	return curtype;
}

void BagItemControl::saveItemConfig()
{
	for (int j = 0; j < g_pagecount; j++)
	{
		for (int i = 0; i < g_onepagetnum; i++)
		{
			UserData::getInstance()->setItemPosIdx(i, _bagItemConfig[j][i], j);
		}
	}

	for (int i = 0; i < ParamData::EQUIP_ITEM_MAX_COUNT; i++)
	{
		UserData::getInstance()->setEquipConfig(i, _bagEquipConfig[i]);
	}

	UserData::getInstance()->saveUserData();
}

void BagItemControl::autoEquipItems()
{
	bool isBagHasItem = true;
	for (int i = 0; i < ParamData::EQUIP_ITEM_MAX_COUNT && isBagHasItem; i++)
	{
		const auto& temp = _bagEquipConfig[i];

		if (temp == -1)
		{
			isBagHasItem = false;
			for (int j = 0; j < g_pagecount * g_onepagetnum; j++)
			{
				int page = j / g_onepagetnum;
				int posInPage = j % g_onepagetnum;

				if (_bagItemConfig[page][posInPage] >= 1000 && _bagItemConfig[page][posInPage] < 1500)
				{
					changePos(posInPage, g_onepagetnum + i, page);
					isBagHasItem = true;
					break;
				}
			}
		}
	}
}

void BagItemControl::autoEquipInFight()
{	
	vector<int> itmeids = { 1007, 1013, 1014 };
	vector<int> typecpunt = { 0 , 0, 0, 0};

	for (int i = 0; i < ParamData::EQUIP_ITEM_MAX_COUNT; i++)
	{
		const auto& temp = _bagEquipConfig[i];
		int type = getItemsType(temp);
		typecpunt[type]++;
	}
	bool isBagHasItem = true;
	int totalCount = 0;

	for (int i = 0; i < ParamData::EQUIP_ITEM_MAX_COUNT; i++)
	{
		const auto& temp = _bagEquipConfig[i];
		
		int needEquipType = 0;

		if (temp == -1)
		{
			for (int n = 0; n < itmeids.size(); n++)
			{
				if (typecpunt[n] < typecpunt[needEquipType])
				{
					needEquipType = n;
				}
			}
		}
		else
		{
			continue;
		}
				
		if (temp == -1 && isBagHasItem && totalCount < 3 && itmeids.size() > 0)
		{	
			isBagHasItem = false;
			for (int j = 0; j < g_pagecount * g_onepagetnum; j++)
			{
				int page = j / g_onepagetnum;
				int posInPage = j % g_onepagetnum;

				//if (_bagItemConfig[page][posInPage] >= 1000 && _bagItemConfig[page][posInPage] < 1500)
				if (_bagItemConfig[page][posInPage] == 1014 || _bagItemConfig[page][posInPage] == 1013 || _bagItemConfig[page][posInPage] == 1007)
				{
					isBagHasItem = true;
					if (_bagItemConfig[page][posInPage] == itmeids[needEquipType])
					{
						typecpunt[needEquipType]++;
						itmeids.erase(itmeids.begin() + needEquipType);
						typecpunt.erase(typecpunt.begin() + needEquipType);
						changePos(posInPage, g_onepagetnum + i, page);
						totalCount++;
						break;
					}					
				}
			}
		}	
	}
}

/*
* ITEMS_HP = 0,
* ITEMS_CROSS,
* ITEMS_S
* other
*/
int BagItemControl::getItemsType(int itemid)
{
	int hp[] = {1000,1002,1004,1005,1007};
	int cross[] = { 1008,1009,1010,1011,1012, 1013 };
	int s[] = { 1014 };

	for (int i = 0; i < sizeof(hp) / sizeof(hp[0]); i++)
	{
		int temp = hp[i];
		if (temp == itemid)
		{
			return 0;
		}
	}

	for (int i = 0; i < sizeof(cross) / sizeof(cross[0]); i++)
	{
		int temp = cross[i];
		if (temp == itemid)
		{
			return 1;
		}
	}

	for (int i = 0; i < sizeof(s) / sizeof(s[0]); i++)
	{
		int temp = s[i];
		if (temp == itemid)
		{
			return 2;
		}
	}

	return 3;
}


