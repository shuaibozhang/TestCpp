#ifndef __ENDLESS_MAIN_LAYER_H__
#define __ENDLESS_MAIN_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameCSLoader.h"
#include "ParamData.h"

USING_NS_CC;
USING_NS_CC_EXT;
//using namespace cocostudio;

class EndlessMainLayer;

enum
{
	RANK_TABLEVIEW_TAG_BG = 101,
	RANK_TABLEVIEW_TAG_CUP,
	RANK_TABLEVIEW_TAG_RANK,
	RANK_TABLEVIEW_TAG_NICK,
	RANK_TABLEVIEW_TAG_VALUE,
	RANK_TABLEVIEW_TAG_HEAD,
	RANK_TABLEVIEW_TAG_LV,
	RANK_TABLEVIEW_TAG_ICON,
};

class RankTableDataSrc : public Node, public TableViewDataSource, public TableViewDelegate
{
public:
	static RankTableDataSrc *create(EndlessMainLayer *pMainLayer);
	bool init(EndlessMainLayer *pMainLayer);

	/*inherit from CCScrollViewDelegate*/
	virtual void scrollViewDidScroll(extension::ScrollView* view);
	virtual void scrollViewDidZoom(extension::ScrollView* view);

	/*inherit from CCTableViewDelegate*/
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
	virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);

	/*inherit from CCTableViewDataSource*/
	virtual Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
private:
	EndlessMainLayer *m_pMainLayer;
};



class EndlessMainLayer : public cocos2d::Layer
{
public:
	static EndlessMainLayer *getInstance();
	EndlessMainLayer();
	virtual ~EndlessMainLayer();

	// implement the "static create()" method manually
	CREATE_FUNC(EndlessMainLayer);

    virtual bool init() override;
	virtual void onEnter() override;
	virtual void update(float delta) override;
    
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void updateRankTableView(void);
	void updateSelfInfoUI(void);
//	void updateSelfData(void); 
	int getEndlessCost();

private:
	static EndlessMainLayer *s_pInstance;
	ui::TextAtlas *_pSelfFloor;
	ui::TextAtlas *_pSelfScore;
	ui::TextAtlas *_pSelfRank;
	ui::Text *_pSelfText;
	ui::ImageView *_pSelfHead;
	RankTableDataSrc *_pRankTableDataSrc;
	extension::TableView *_pRankTV;
	Label *_pConnectDesc;
	Node *_pCost0;
	Node *_pCost1;
};

#endif // 