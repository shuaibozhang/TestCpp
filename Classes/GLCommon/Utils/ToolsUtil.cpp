/********************************************************************
	created:	2013/08/20
	created:	20:8:2013   19:47
	filename: 	
	file path:	
	file base:	ToolsUtil
	file ext:	cpp
	author:		Dragon.Zhou
	
	purpose:	
*********************************************************************/
#include "ToolsUtil.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../../Widgets/GameButton.h"
#include <iostream>
#include <fstream>

using namespace std;
USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

static const GLchar* s_szDefaultA8Color_VSH = 
	"													\n\
	attribute vec4 a_position;							\n\
	attribute vec2 a_texCoord;							\n\
	attribute vec4 a_color;								\n\
	\n\
	#ifdef GL_ES										\n\
	varying lowp vec4 v_fragmentColor;					\n\
	varying mediump vec2 v_texCoord;					\n\
	#else												\n\
	varying vec4 v_fragmentColor;						\n\
	varying vec2 v_texCoord;							\n\
	#endif												\n\
	\n\
	void main()											\n\
	{													\n\
	gl_Position = CC_MVPMatrix * a_position;			\n\
	v_fragmentColor = a_color;							\n\
	v_texCoord = a_texCoord;							\n\
	}													\n\
	";


static const GLchar* s_szPassThroghh_FSH = 
	"																\n\
	precision mediump float;\n\
	\n\
	\n\
	varying vec4 v_fragmentColor;\n\
	varying vec2 v_texCoord;\n\
	\n\
	void main(void)\n\
	{\n\
		vec4 c = texture2D(CC_Texture0, v_texCoord);\n\
		gl_FragColor.xyz = vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b);\n\
		gl_FragColor.w = c.w;\n\
	}\n\
	";

float ToolsUtil::getMinScale(float srcWidth, float srcHeight, float dstWidth, float dstHeight)
{
	float fScale = dstWidth / srcWidth;

	fScale = (fScale < (dstHeight / srcHeight)) ? fScale : (dstHeight / srcHeight);

	return fScale;
}


void ToolsUtil::stringFindReplace( std::string &strSrc, const std::string &strTarget, const std::string &strReplace )
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strTarget.size();
	std::string::size_type dstlen = strReplace.size();

	while( (pos=strSrc.find(strTarget, pos)) != std::string::npos )
	{
		strSrc.replace( pos, srclen, strReplace );
		pos += dstlen;
	}
}


float ToolsUtil::getCurTime()
{
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}


void ToolsUtil::initRandomNumber(void)
{
	/*
	cc_timeval psv;
	CCTime::gettimeofdayCocos2d( &psv, NULL ); 
	unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000; */

	struct  timeval  start;
	gettimeofday(&start, NULL);
	unsigned int tsrans = start.tv_sec * 1000 + start.tv_usec / 1000; 
	srand( tsrans );
}


float ToolsUtil::getRandomFloat(float beginNum, float endNum)
{
	float ret = CCRANDOM_0_1();
	ret = ret * (endNum - beginNum) + beginNum;

	return ret;
}


int ToolsUtil::getRandomInt(int beginNum, int endNum)
{
	int ret = ToolsUtil::getRandomFloat(beginNum, endNum + 1);

	if (ret > endNum)
	{
		ret = endNum;
	}

	return ret;
}




void ToolsUtil::pauseNodeAndChildren(Node * node) 
{
	node->pause();

//	Ref * obj;

	for (auto & obj : node -> getChildren()) {  
		Node * n = (Node *)obj;
		pauseNodeAndChildren(n);	
	}  
	
		

}


void ToolsUtil::resumeNodeAndChildren( Node * node )
{
	node->resume();

//	Ref * obj;
	for (auto & obj : node -> getChildren()) {  
		Node * n = (Node *)obj;
		resumeNodeAndChildren(n);	
	}  
}

/*
void ToolsUtil::drawRect(CCRect rBox, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	ccDrawColor4F(r, g, b, a);
	ccDrawRect(rBox.origin, ccpAdd(rBox.origin, ccp(rBox.size.width, rBox.size.height)));
}


void ToolsUtil::drawSolidRect(CCRect rBox, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	ccDrawSolidRect(rBox.origin, ccpAdd(rBox.origin, ccp(rBox.size.width, rBox.size.height)), ccc4f(r, g, b, a));
}*/


Vec2 ToolsUtil::getNodeOffset(Node *pNode, const Vec2 &desAnchorPoint)
{
	Vec2 anchorPos = pNode->getAnchorPoint();
	Vec2 anchorOffset = desAnchorPoint - anchorPos;
	Size nodeSize = pNode->getContentSize();
	Vec2 offsetPos = Vec2(nodeSize.width * anchorOffset.x, nodeSize.height * anchorOffset.y);

	return offsetPos;
}


void ToolsUtil::setShader(cocos2d::Node* node, bool useShader)
{
	if (nullptr == node)
	{
		return;
	}

	if (dynamic_cast<Sprite*>(node)!=NULL)
	{
		if(useShader){
			GLProgram * p = new GLProgram();
			p->initWithByteArrays(s_szDefaultA8Color_VSH, s_szPassThroghh_FSH);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
			p->link();
			p->updateUniforms();
			node->setGLProgram(p);
		}else{//addNormal shader
			node->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
		}
	}
	else if (dynamic_cast<ImageView*>(node)!=NULL)
	{
		if(useShader){
			GLProgram * p = new GLProgram();
			p->initWithByteArrays(s_szDefaultA8Color_VSH, s_szPassThroghh_FSH);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
			p->link();
			p->updateUniforms();
			node->setGLProgram(p);
		}else{//addNormal shader
			node->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
		}
	}

	else if (dynamic_cast<Button*>(node)!=NULL)
	{
		if(useShader){
			GLProgram * p = new GLProgram();
			p->initWithByteArrays(s_szDefaultA8Color_VSH, s_szPassThroghh_FSH);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
			p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
			p->link();
			p->updateUniforms();
			node->setGLProgram(p);
		}else{//addNormal shader
			node->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
		}
	}
	
	if (node->getChildrenCount() > 0)
	{
		for(auto& chide : node->getChildren()) 
		{
			if (dynamic_cast<Sprite*>(chide)!=NULL)
			{
				if(useShader){
					GLProgram * p = new GLProgram();
					p->initWithByteArrays(s_szDefaultA8Color_VSH, s_szPassThroghh_FSH);
					p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
					p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
					p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
					p->link();
					p->updateUniforms();
					chide->setGLProgram(p);
				}else{//addNormal shader
					chide->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
				}
			}
			else
			{
				setShader(chide,useShader);
			}
		}
	}
}


std::vector<std::string>  ToolsUtil::parseUTF8(const std::string &str) {
	int l = str.length();
	std::vector<std::string> ret;
	ret.clear();
	for(int p = 0; p < l; ) {
		int size;
		unsigned char c = str[p];
		if(c < 0x80) {
			size = 1;
		} else if(c < 0xc2) {
		} else if(c < 0xe0) {
			size = 2;
		} else if(c < 0xf0) {
			size = 3;
		} else if(c < 0xf8) {
			size = 4;
		} else if (c < 0xfc) {
			size = 5;
		} else if (c < 0xfe) {
			size = 6;
		}
		std::string temp = "";
		temp = str.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
	return ret;
}


std::string ToolsUtil::subUTF8(const std::string &str,int from, int to)
{
	if(from > to) return "";
	std::vector<std::string> test = parseUTF8(str);
	if (test.size() < to) return str;
	std::vector<std::string>::iterator iter = test.begin();
	std::string res;
	std::string result;
	for(iter=(test.begin() + from); iter != (test.begin() + to); iter++)
	{
		res += *iter;

	}
	return res;
}


Vec2 ToolsUtil::getVelocity(Vec2 vec, float speed)
{
	float angle = vec.getAngle();
	float cosValue = cosf(angle);//cosf(CC_DEGREES_TO_RADIANS(angle));
	float sinValue = sinf(angle);//sinf(CC_DEGREES_TO_RADIANS(angle));
	float speedX = speed * cosValue;
	float speedY = speed * sinValue;

	return Vec2(speedX, speedY);
}


void ToolsUtil::setAniPosByAp(cocos2d::Node* ani, cocos2d::Vec2 tarpos, cocos2d::Vec2 tarAp)
{
	auto orgAp = ani->getAnchorPoint();
	auto offap = orgAp - tarAp;
	auto offsize = Vec2(ani->getContentSize().width * offap.x, ani->getContentSize().height * offap.y);
	auto truepos = tarpos + offsize;
	ani->setPosition(truepos);
}


bool ToolsUtil::checkRotateRectColision(Vec2* points, Vec2*  points2)
{
	Vec2 rect1Max = points[0];
	Vec2 rect1Min = points[0];
	for (int i=0; i<4; i++)
	{
		if (rect1Max.x < points[i].x)
		{
			rect1Max.x = points[i].x;
		}
		else if (rect1Min.x > points[i].x)
		{
			rect1Min.x = points[i].x;
		}

		if (rect1Max.y < points[i].y)
		{
			rect1Max.y = points[i].y;
		}
		else if (rect1Min.y > points[i].y)
		{
			rect1Min.y = points[i].y;
		}
	}

	Vec2 rect2Max = points2[0];
	Vec2 rect2Min = points2[0];
	for (int i=0; i<4; i++)
	{
		if (rect2Max.x < points2[i].x)
		{
			rect2Max.x = points2[i].x;
		}
		else if (rect2Min.x > points2[i].x)
		{
			rect2Min.x = points2[i].x;
		}

		if (rect2Max.y < points2[i].y)
		{
			rect2Max.y = points2[i].y;
		}
		else if (rect2Min.y > points2[i].y)
		{
			rect2Min.y = points2[i].y;
		}
	}

	if (((rect1Max.x < rect2Min.x) || (rect1Min.x > rect2Max.x))
		|| ((rect1Max.y < rect2Min.y) || (rect1Min.y > rect2Max.y)))
	{
		return false;
	}


	Vec2 centerA = (points[0] + points[2])/2;
	Vec2 centerB = (points2[0] + points2[2])/2;
	Vec2 axis1 = (points[0] - points[1]) / 2;
	Vec2 axis2 = (points[1] - points[2]) / 2;
	Vec2 axis3 = (points2[0] - points2[1]) / 2;
	Vec2 axis4 = (points2[1] - points2[2]) / 2;
	Vec2 axisNor1 = (points[0] - points[1]).getNormalized();
	Vec2 axisNor2 = (points[1] - points[2]).getNormalized();
	Vec2 axisNor3 = (points2[0] - points2[1]).getNormalized();
	Vec2 axisNor4 = (points2[1] - points2[2]).getNormalized();

	float rect1W = points[0].distance(points[1]) / 2;
	float rect1H = points[1].distance(points[2]) / 2;
	float rect2W = points2[0].distance(points2[1]) / 2;
	float rect2H = points2[1].distance(points2[2]) / 2;

	Vec2 arraxis[] = {axisNor1, axisNor2, axisNor3, axisNor4};

	/*check rect1-1*/
	Vec2 axis = arraxis[0];
	float distanceCenter = abs((centerB - centerA).dot(axis));
	float distancea1 = abs(axis3.dot(axis));
	float distancea2 = abs(axis4.dot(axis));
	float distancea3 = rect1W;

	if (distanceCenter > (distancea1 + distancea2 + distancea3) )
	{
		return false;
	}

	/*check rect1-2*/
	axis = arraxis[1];
	distanceCenter = abs((centerB - centerA).dot(axis));
	distancea1 = abs(axis3.dot(axis));
	distancea2 = abs(axis4.dot(axis));
	distancea3 = rect1H;

	if (distanceCenter > (distancea1 + distancea2 + distancea3) )
	{
		return false;
	}

	/*check rect2-1*/
	axis = arraxis[2];
	distanceCenter = abs((centerB - centerA).dot(axis));
	distancea1 = abs(axis1.dot(axis));
	distancea2 = abs(axis2.dot(axis));
	distancea3 = rect2W;

	if (distanceCenter > (distancea1 + distancea2 + distancea3) )
	{
		return false;
	}
	/*check rect2-2*/
	axis = arraxis[3];
	distanceCenter = abs((centerB - centerA).dot(axis));
	distancea1 = abs(axis1.dot(axis));
	distancea2 = abs(axis2.dot(axis));
	distancea3 = rect2H;

	if (distanceCenter > (distancea1 + distancea2 + distancea3) )
	{
		return false;
	}

	return true;
	/*
	Vec2 centerA = (points[0] + points[2])/2;
	Vec2 centerB = (points2[0] + points2[2])/2;
	Vec2 axis1 = (points[0] - points[1]).getNormalized();
	Vec2 axis2 = (points[1] - points[2]).getNormalized();
	Vec2 axis3 = (points2[0] - points2[1]).getNormalized();
	Vec2 axis4 = (points2[1] - points2[2]).getNormalized();

	Vec2 arraxis[] = {axis1, axis2, axis3, axis4};

	for (int i = 0; i < 4; i++)
	{
	float distanceCenter = abs((centerB - centerA).dot(arraxis[i]));
	auto debuga1 = points[0] - centerA;
	auto debuga2 = points[1] - centerA;
	float distancea1 =abs((points[0] - centerA).dot(arraxis[i]));
	float distancea2 =abs((points[1] - centerA).dot(arraxis[i]));

	float trueDisa = distancea1 > distancea2 ? distancea1 : distancea2;

	float distanceb1 =abs((points2[0] - centerB).dot(arraxis[i]));
	float distanceb2 =abs((points2[1] - centerB).dot(arraxis[i]));

	float trueDisb = distanceb1 > distanceb2 ? distanceb1 : distanceb2;

	if (trueDisb + trueDisa < distanceCenter)
	{
	return false;
	}
	}
	return true;

	*/
}


void ToolsUtil::getRandomIntArr(int arrCount, int valueBegin, int valueEnd, int *arrValue)
{
//	int *arrTmp = (int *)malloc(arrCount * sizeof(int));

	for (int i=0; i<arrCount; i++)
	{
		bool isHave = false;
		int value;

		do 
		{
			value = ToolsUtil::getRandomInt(valueBegin, valueEnd);

			isHave = false;
			for (int j=0; j<i; j++)
			{
				if (arrValue[j] == value)
				{
					isHave = true;
					break;
				}
			}
		} while (isHave);

		arrValue[i] = value;
	}

//	free((void *)arrTmp);
}


float ToolsUtil::getCrossValue(float totalVale, int count, int index)
{
	float perValue = totalVale / (count-1) / 2;
	int param = (0 == index%2) ? 1 : -1;
	float ret = param * (perValue + (int)(index/2)*2*perValue);

	return ret;
}


bool ToolsUtil::isFileExist(const char* pFileName)
{
#if 0
	if( !pFileName ) return false;
	//strFilePathName is :/data/data/ + package name
	std::string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += pFileName;

	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
#endif
	return FileUtils::getInstance()->isFileExist(pFileName);
}


void ToolsUtil::deleteSpan(std::string &str)
{
	int pos = string::npos;
	while ((pos = str.find_first_of(' ')) != string::npos)
	{
		str.erase(pos, 1);
	}
}


void ToolsUtil::copyData(const char* pFileName)
{
	Data data;
	data = FileUtils::getInstance()->getDataFromFile(pFileName);

	//ssize_t len = 0;
	//unsigned char *data = NULL;
	//data = FileUtils::getInstance()->getFileData(pFileName,"r",&len);
	//
	std::string destPath = FileUtils::getInstance()->getWritablePath();
	destPath += pFileName;

	 const std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	FILE *fp = fopen(destPath.c_str(),"wb+");
	fwrite(data.getBytes(),sizeof(char),data.getSize(),fp);
	//free(data);
	fclose(fp);

	 /*ifstream in;
	 ofstream out;
	 in.open(fullPath,ios::binary);
	 if(in.fail())
	 {
	 log("Error %d: Fail to open the source file\n %s", 1, fullPath.c_str());
	 cout<<"Error 1: Fail to open the source file."<<endl;
	 in.close();
	 out.close();

	 return;
	 }
	 out.open(destPath,ios::binary);
	 if(out.fail())
	 {
	 log("Error %d: Fail to create the new file\n %s", 2, destPath.c_str());
	 cout<<"Error 2: Fail to create the new file."<<endl;
	 out.close();
	 in.close();
	 return;
	 }
	 else
	 {
	 out<<in.rdbuf();
	 out.close();
	 in.close();
	 return;
	 }*/
}


Animation *ToolsUtil::getOrCreateAnimation(const std::string &animName, int frameCount, float delayPerUnitbool, bool isNeedCache)
{
	auto anim = AnimationCache::getInstance()->getAnimation(animName);

	if (nullptr == anim)
	{
		anim = Animation::create();

		for (int i=0; i<frameCount; i++)
		{
			__String *strFile = __String::createWithFormat("%s_%d.png", animName.c_str(), i);

			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strFile->getCString());
			anim->addSpriteFrame(frame);
		}

		anim->setDelayPerUnit(delayPerUnitbool);

		if (isNeedCache)
		{
			AnimationCache::getInstance()->addAnimation(anim, animName);
		}
	}

	return anim;
}

void ToolsUtil::split(std::string str, std::string pattern, std::vector<std::string>& result)
{
	std::string::size_type pos;
	//std::vector<std::string> result;
	str += pattern;
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void ToolsUtil::btea(uint32_t *v, int n, uint32_t const key[4]) {
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	if (n > 1) {          /* Coding Part */
		rounds = 6 + 52 / n;
		sum = 0;
		z = v[n - 1];
		do {
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++) {
				y = v[p + 1];
				z = v[p] += MX;
			}
			y = v[0];
			z = v[n - 1] += MX;
		} while (--rounds);
	}
	else if (n < -1) {  /* Decoding Part */
		n = -n;
		rounds = 6 + 52 / n;
		sum = rounds*DELTA;
		y = v[0];
		do {
			e = (sum >> 2) & 3;
			for (p = n - 1; p > 0; p--) {
				z = v[p - 1];
				y = v[p] -= MX;
			}
			z = v[n - 1];
			y = v[0] -= MX;
			sum -= DELTA;
		} while (--rounds);
	}
}

void ToolsUtil::unbtea(cocos2d::Data & data)
{
	uint32_t const key[4] = { 1701667175, 1701210476, 1701667175, 1701210476 };
	int n = (data.getSize() / 4);
	ToolsUtil::btea((uint32_t*)data.getBytes(), -n, key );
}

void ToolsUtil::getRandoms(std::vector<int> arrWeight, std::vector<int>& arrGot, int nums)
{
	CCASSERT(nums <= arrWeight.size(), "nums must <= pint.size()");
	int curGetNum = 0;
	std::vector<int> arrIndex;

	for (auto i = 0; i != arrWeight.size(); i++)
	{
		arrIndex.push_back(i);
	}

	while (curGetNum != nums)
	{
		int totalWeight = 0;
		for (auto& temp : arrWeight)
		{
			totalWeight += temp;
		}

		int p = getRandomInt(0, totalWeight);
		int idx = 0;
		int pp = 0;
		for (auto& temp2 : arrWeight)
		{
			pp += temp2;
			if (pp >= p)
			{
				arrGot.push_back(arrIndex[idx]);
				arrWeight.erase(arrWeight.begin() + idx);
				arrIndex.erase(arrIndex.begin() + idx);
				curGetNum++;
				break;
			}
			idx++;
		}
	}
}

void ToolsUtil::getRandoms(int arrCount, const int * arrWeight, std::vector<int>& arrGot, int nums)
{
	std::vector<int> arrTmp;
	for (int i = 0; i < arrCount; i++)
	{
		arrTmp.push_back(arrWeight[i]);
	}
	getRandoms(arrTmp, arrGot, nums);
}
