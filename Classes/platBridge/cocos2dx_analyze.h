

#ifndef __COCOS2DX_ANALYZE_H__
#define __COCOS2DX_ANALYZE_H__

#include <iostream>
#include <string>

using namespace std;

class cocos2dx_analyze {

public:
	
	/** �����Ƿ��ӡsdk��log��Ϣ,Ĭ�ϲ�����
	@param value ����Ϊtrue,umeng SDK �����log��Ϣ,�ǵ�release��ƷʱҪ���û�false.
	@return .
	@exception .
	*/

	static void setLogEnabled(bool value);

	///---------------------------------------------------------------------------------------
	/// @name  �¼�ͳ��
	///---------------------------------------------------------------------------------------


	/** �Զ����¼�,����ͳ��.
	ʹ��ǰ�����ȵ�����App�����̨������->�༭�Զ����¼� �������Ӧ���¼�ID��Ȼ���ڹ����д�����Ӧ���¼�ID

	@param  eventId ��վ��ע����¼�Id.
	@param  label �����ǩ����ͬ�ı�ǩ��ֱ����ͳ�ƣ�����ͬһ�¼��Ĳ�ͬ��ǩ�ĶԱ�,ΪNULL����ַ���ʱ��̨�����ɺ�eventIdͬ���ı�ǩ.
	@return void.
	*/
	static void addCustomEvent(string eventId, string value);
//	static void event(const char * eventId, const char * label = NULL);
	/** �Զ����¼�,����ͳ��.
	ʹ��ǰ�����ȵ�����App�����̨������->�༭�Զ����¼� �������Ӧ���¼�ID��Ȼ���ڹ����д�����Ӧ���¼�ID
	*/
//	static void event(const char * eventId, eventDict * attributes, int counter = 0);


	///---------------------------------------------------------------------------------------
	/// @name  ҳ���ʱ
	///---------------------------------------------------------------------------------------


	/** ҳ��ʱ��ͳ��,��¼ĳ��view���򿪶೤ʱ��,�����Լ���ʱҲ���Ե���beginLogPageView,endLogPageView�Զ���ʱ

	@param pageName ��Ҫ��¼ʱ����view����.
	@return void.
	*/

	static void beginLogPageView(const char *pageName);
	static void endLogPageView(const char *pageName);

	///---------------------------------------------------------------------------------------
	/// @name  ���߲���
	///---------------------------------------------------------------------------------------


	/** ʹ�����߲������ܣ��������㶯̬�޸�Ӧ���еĲ���ֵ,
	���ô˷��������Զ�ӵ�����߸���SDK�˷��Ͳ��ԵĹ���,����Ҫ���ڷ����������ú����߲���.
	����startWithAppkey����֮�����;
	@param ��.
	@return void.
	*/

	static void updateOnlineConfig(void);

	/** ��ȡ��������߲�������ֵ
	�������ķ�����ȡĳ��key��ֵ�����������Ļ�ȡ���е����߲���.
	��Ҫ�ȵ���updateOnlineConfig����ʹ��

	@param key
	@return const char *  .
	*/

	static string getOnlineValue(string key);
//	static string getConfigParam(const char * key);

	//��Ϸͳ�ƿ�ʼ

	///---------------------------------------------------------------------------------------
	/// @name  �����������
	///---------------------------------------------------------------------------------------

	/** ������ҵĵȼ�����Ϸ�е�ΨһId���Ա��������Դ.
	*/

	/** ������ҵȼ�����.
	@param level ��ҵȼ�
	@return void
	*/
	static void setUserLevel(const char *level);

	/** ������ҵȼ�����.
	@param userId ���Id
	@param sex �Ա�
	@param age ����
	@param platform ��Դ
	@return void
	*/

	enum Sex{
		Unkonwn = 0,
		Male = 1,
		Female = 2,
	};

	static void setUserInfo(const char * userId, int sex, int age, const char * platform);

	///---------------------------------------------------------------------------------------
	/// @name  �ؿ�ͳ��
	///---------------------------------------------------------------------------------------

	/** ��¼��ҽ���ؿ���ͨ���ؿ���ʧ�ܵ����.
	*/


	/** ����ؿ�.
	@param level �ؿ�
	@return void
	*/
	static void startLevel(const char * level);

	/** ͨ���ؿ�.
	@param level �ؿ�,���level == NULL ��Ϊ��ǰ�ؿ�
	@return void
	*/
	static void finishLevel(const char * level);

	/** δͨ���ؿ�.
	@param level �ؿ�,���level == NULL ��Ϊ��ǰ�ؿ�
	@return void
	*/

	static void failLevel(const char * level);

	///---------------------------------------------------------------------------------------
	/// @name  ֧��ͳ��
	///---------------------------------------------------------------------------------------

	/** ��¼���ʹ����ʵ���ҵ��������
	*/


	/** ���֧�����Ҷһ������.
	@param cash ��ʵ��������
	@param source ֧������
	@param coin ���������
	@return void
	*/

	static void pay(double cash, double coin, int source);

	/** ���֧�����ҹ������.
	@param cash ��ʵ��������
	@param source ֧������
	@param item ��������
	@param amount ��������
	@param price ���ߵ���
	@return void
	*/
	static void pay(double cash, const char * item, int amount, double price, int source);

	///---------------------------------------------------------------------------------------
	/// @name  ����ҹ���ͳ��
	///---------------------------------------------------------------------------------------

	/** ��¼���ʹ������ҵ��������
	*/


	/** ���ʹ������ҹ������
	@param item ��������
	@param amount ��������
	@param price ���ߵ���
	@return void
	*/
	static void buy(const char *item, int amount, double price);


	///---------------------------------------------------------------------------------------
	/// @name  ��������ͳ��
	///---------------------------------------------------------------------------------------

	/** ��¼��ҵ����������
	*/


	/** ���ʹ������ҹ������
	@param item ��������
	@param amount ��������
	@param price ���ߵ���
	@return void
	*/

	static void use(const char * item, int amount, double price);


	///---------------------------------------------------------------------------------------
	/// @name  ����Ҽ����߽���ͳ��
	///---------------------------------------------------------------------------------------

	/** ��¼��һ�������Ҽ����ߵ����
	*/


	/** ��һ�����ҽ���
	@param coin ���������
	@param source ������ʽ
	@return void
	*/

	static void bonus(double coin, int source);

	/** ��һ���߽���
	@param item ��������
	@param amount ��������
	@param price ���ߵ���
	@param source ������ʽ
	@return void
	*/

	static void bonus(const char * item, int amount, double price, int source);
};

#endif
