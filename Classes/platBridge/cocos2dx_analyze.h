

#ifndef __COCOS2DX_ANALYZE_H__
#define __COCOS2DX_ANALYZE_H__

#include <iostream>
#include <string>

using namespace std;

class cocos2dx_analyze {

public:
	
	/** 设置是否打印sdk的log信息,默认不开启
	@param value 设置为true,umeng SDK 会输出log信息,记得release产品时要设置回false.
	@return .
	@exception .
	*/

	static void setLogEnabled(bool value);

	///---------------------------------------------------------------------------------------
	/// @name  事件统计
	///---------------------------------------------------------------------------------------


	/** 自定义事件,数量统计.
	使用前，请先到友盟App管理后台的设置->编辑自定义事件 中添加相应的事件ID，然后在工程中传入相应的事件ID

	@param  eventId 网站上注册的事件Id.
	@param  label 分类标签。不同的标签会分别进行统计，方便同一事件的不同标签的对比,为NULL或空字符串时后台会生成和eventId同名的标签.
	@return void.
	*/
	static void addCustomEvent(string eventId, string value);
//	static void event(const char * eventId, const char * label = NULL);
	/** 自定义事件,数量统计.
	使用前，请先到友盟App管理后台的设置->编辑自定义事件 中添加相应的事件ID，然后在工程中传入相应的事件ID
	*/
//	static void event(const char * eventId, eventDict * attributes, int counter = 0);


	///---------------------------------------------------------------------------------------
	/// @name  页面计时
	///---------------------------------------------------------------------------------------


	/** 页面时长统计,记录某个view被打开多长时间,可以自己计时也可以调用beginLogPageView,endLogPageView自动计时

	@param pageName 需要记录时长的view名称.
	@return void.
	*/

	static void beginLogPageView(const char *pageName);
	static void endLogPageView(const char *pageName);

	///---------------------------------------------------------------------------------------
	/// @name  在线参数
	///---------------------------------------------------------------------------------------


	/** 使用在线参数功能，可以让你动态修改应用中的参数值,
	调用此方法您将自动拥有在线更改SDK端发送策略的功能,您需要先在服务器端设置好在线参数.
	请在startWithAppkey方法之后调用;
	@param 无.
	@return void.
	*/

	static void updateOnlineConfig(void);

	/** 获取缓存的在线参数的数值
	带参数的方法获取某个key的值，不带参数的获取所有的在线参数.
	需要先调用updateOnlineConfig才能使用

	@param key
	@return const char *  .
	*/

	static string getOnlineValue(string key);
//	static string getConfigParam(const char * key);

	//游戏统计开始

	///---------------------------------------------------------------------------------------
	/// @name  玩家属性设置
	///---------------------------------------------------------------------------------------

	/** 设置玩家的等级、游戏中的唯一Id、性别、年龄和来源.
	*/

	/** 设置玩家等级属性.
	@param level 玩家等级
	@return void
	*/
	static void setUserLevel(const char *level);

	/** 设置玩家等级属性.
	@param userId 玩家Id
	@param sex 性别
	@param age 年龄
	@param platform 来源
	@return void
	*/

	enum Sex{
		Unkonwn = 0,
		Male = 1,
		Female = 2,
	};

	static void setUserInfo(const char * userId, int sex, int age, const char * platform);

	///---------------------------------------------------------------------------------------
	/// @name  关卡统计
	///---------------------------------------------------------------------------------------

	/** 记录玩家进入关卡，通过关卡及失败的情况.
	*/


	/** 进入关卡.
	@param level 关卡
	@return void
	*/
	static void startLevel(const char * level);

	/** 通过关卡.
	@param level 关卡,如果level == NULL 则为当前关卡
	@return void
	*/
	static void finishLevel(const char * level);

	/** 未通过关卡.
	@param level 关卡,如果level == NULL 则为当前关卡
	@return void
	*/

	static void failLevel(const char * level);

	///---------------------------------------------------------------------------------------
	/// @name  支付统计
	///---------------------------------------------------------------------------------------

	/** 记录玩家使用真实货币的消费情况
	*/


	/** 玩家支付货币兑换虚拟币.
	@param cash 真实货币数量
	@param source 支付渠道
	@param coin 虚拟币数量
	@return void
	*/

	static void pay(double cash, double coin, int source);

	/** 玩家支付货币购买道具.
	@param cash 真实货币数量
	@param source 支付渠道
	@param item 道具名称
	@param amount 道具数量
	@param price 道具单价
	@return void
	*/
	static void pay(double cash, const char * item, int amount, double price, int source);

	///---------------------------------------------------------------------------------------
	/// @name  虚拟币购买统计
	///---------------------------------------------------------------------------------------

	/** 记录玩家使用虚拟币的消费情况
	*/


	/** 玩家使用虚拟币购买道具
	@param item 道具名称
	@param amount 道具数量
	@param price 道具单价
	@return void
	*/
	static void buy(const char *item, int amount, double price);


	///---------------------------------------------------------------------------------------
	/// @name  道具消耗统计
	///---------------------------------------------------------------------------------------

	/** 记录玩家道具消费情况
	*/


	/** 玩家使用虚拟币购买道具
	@param item 道具名称
	@param amount 道具数量
	@param price 道具单价
	@return void
	*/

	static void use(const char * item, int amount, double price);


	///---------------------------------------------------------------------------------------
	/// @name  虚拟币及道具奖励统计
	///---------------------------------------------------------------------------------------

	/** 记录玩家获赠虚拟币及道具的情况
	*/


	/** 玩家获虚拟币奖励
	@param coin 虚拟币数量
	@param source 奖励方式
	@return void
	*/

	static void bonus(double coin, int source);

	/** 玩家获道具奖励
	@param item 道具名称
	@param amount 道具数量
	@param price 道具单价
	@param source 奖励方式
	@return void
	*/

	static void bonus(const char * item, int amount, double price, int source);
};

#endif
