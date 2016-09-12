/********************************************************************
	created:	2013/08/20
	created:	20:8:2013   19:47
	filename: 	D:\cocos2d-x\projects\tadpole_mini\Classes\ToolsUtil.h
	file path:	D:\cocos2d-x\projects\tadpole_mini\Classes
	file base:	ToolsUtil
	file ext:	h
	author:		Dragon.Zhou
	
	purpose:	
*********************************************************************/
#ifndef __TOOLS_UTIL_H__
#define __TOOLS_UTIL_H__

#include "cocos2d.h"

using namespace cocos2d;

class ToolsUtil
{
public:
	static float getMinScale(float srcWidth, float srcHeight, float dstWidth, float dstHeight);
	
	/*about string*/
	static void stringFindReplace(std::string &strSrc, const std::string &strTarget, const std::string &strReplace);
	
	/*about time*/
	static float getCurTime(void);

	/*about random number*/
	static void initRandomNumber(void);
	static float getRandomFloat(float beginNum, float endNum);
	static int getRandomInt(int beginNum, int endNum);
	
	static void pauseNodeAndChildren(Node *pNode);
	static void resumeNodeAndChildren(Node *pNode);

//	static void drawRect(Rect rBox, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
//	static void drawSolidRect(Rect rBox, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	static Vec2 getNodeOffset(Node *pNode, const Vec2 &desAnchorPoint);

	static void setShader(Node* node, bool useShader);

	static std::string subUTF8(const std::string &str,int from, int to);
	static std::vector<std::string>  parseUTF8(const std::string &str);

	static Vec2 getVelocity(Vec2 vec, float speed);

	static void setAniPosByAp(cocos2d::Node* ani, cocos2d::Vec2 tarpos, cocos2d::Vec2 tarAp);
	
	static bool checkRotateRectColision(Vec2* points, Vec2*  ponits2);

	static void getRandomIntArr(int arrCount, int valueBegin, int valueEnd, int *arrValue);

	static float getCrossValue(float totalVale, int count, int index);

	static bool isFileExist(const char* pFileName);
	static void copyData(const char* pFileName);

	static void deleteSpan(std::string &str);

	static Animation *getOrCreateAnimation(const std::string &animName, int frameCount, float delayPerUnitbool=0.1f, bool isNeedCache=true);

	static void split(std::string str, std::string pattern, std::vector<std::string>& result);

	static void btea(uint32_t *v, int n, uint32_t const key[4]);
	static void unbtea(cocos2d::Data& data);
		//uint32_t const key[4] = {1634167911, 25965, 0, 0 };
		//int n = (contentStr.getSize() / 4);
		//ToolsUtil::btea((uint32_t*)contentStr.getBytes(), -n, key );

	static void getRandoms(std::vector<int> arrWeight, std::vector<int>& arrGot, int nums);
	static void getRandoms(int arrCount, const int *arrWeight, std::vector<int>& arrGot, int nums);
};


#endif