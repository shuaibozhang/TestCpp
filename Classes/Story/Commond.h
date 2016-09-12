#ifndef __STORYCOMMOND_H__
#define __STORYCOMMOND_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h" 

USING_NS_CC;
using namespace rapidjson;

class BaseCommond;

typedef std::function<void(BaseCommond* type)> ccCommondCallback;

class BaseCommond : public cocos2d::Ref
{
public:
	BaseCommond();
	~BaseCommond() { CCLOG("~Commond"); }
	virtual void execute() = 0;

	ccCommondCallback _commondCallback;
	bool _autorun;
	int _comid;
	int _nextid;
};

class  GroupCommonds : public BaseCommond
{
public:
	static GroupCommonds* createCommond(cocos2d::Vector<BaseCommond*> _vector);
	bool init(cocos2d::Vector<BaseCommond*> _vector);

	void execute()override;

private:
	cocos2d::Vector<BaseCommond*> _vectorCommonds;
};

typedef struct _Select
{
	std::string des;
	int next;
}Select_T;

class SelectCommonds : public BaseCommond
{
public:
	static SelectCommonds* createCommond(cocos2d::Node* target);
	bool initWithTarget(cocos2d::Node* target);

	std::vector<Select_T> _selectVector;
	void execute()override;
private:
	cocos2d::Node* _target;
};

typedef struct _Reward
{
	int itemid;
	int num;
}Reward_T;

class RewardCommonds : public BaseCommond
{
public:
	static RewardCommonds* createCommond(cocos2d::Node* target, std::string des);
	bool initWithTarget(cocos2d::Node* target, std::string des);

	std::vector<Reward_T> _rewardVector;
	void execute()override;
private:
	cocos2d::Node* _target;
	std::string _sayContext;
};

class PlayerSayCommond : public BaseCommond
{
public:
	static PlayerSayCommond* createCommond(cocos2d::Node* target, std::string saycontext, int playeridx);
	bool initWithTarget(cocos2d::Node* target, std::string saycontext, int playeridx);

	void execute()override;

private:
	std::string _sayContext;
	cocos2d::Node* _targetNode;
	int _playeridx;
};


class CommandMgr
{
public:
	~CommandMgr();
	CommandMgr();

	//static CommandMgr* getInstance();
	/*
	@param
	idx: storyfile idx
	happenpos: 0 enter map, 1 meet boss, 2 boss end or leave map
	*/
	void prepare(int idx, int happenpos = 0);
	

	void executeCommond();
	
	void commondCallback(BaseCommond*);
	bool _isExecuting;
	bool isHaveDialog() { return !_commondQueue.empty(); }

	bool _pause;
	int _commondidx;

private:
	CommandMgr& operator=(const CommandMgr&);
	CommandMgr(const CommandMgr&);

	bool _isStoryEnd;
	//static CommandMgr* _instance;
	int _happonpos;
	void pushCommond(BaseCommond* com);
	void clearCommond();

	std::vector<BaseCommond*> _commondQueue;

};
#endif
