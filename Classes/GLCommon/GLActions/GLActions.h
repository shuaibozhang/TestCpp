#ifndef __GLACTION_CCPROGRESS_TIMER_H__
#define __GLACTION_CCPROGRESS_TIMER_H__

#include "2d/CCActionInterval.h"

USING_NS_CC;

class  GLProgressTo : public ActionInterval
{
public:
	/**
	* @brief Create and initializes with a duration and a destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @param 0 : LoadingBar, 1: glui::GLSlidBar;
	* @return If the creation success, return a pointer of ProgressTo action; otherwise, return nil.
	*/
	static GLProgressTo* create(float duration, float percent, int type = 0);

	//
	// Overrides
	//
	virtual GLProgressTo* clone() const override;
	virtual GLProgressTo* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	GLProgressTo() {}
	virtual ~GLProgressTo() {}

	/**
	* @brief Initializes with a duration and destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @return If the creation success, return true; otherwise, return false.
	*/
	bool initWithDuration(float duration, float percent);
	int _type;
protected:
	float _to;
	float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(GLProgressTo);
};

/**
@brief Progress from a percentage to another percentage.
@since v0.99.1
*/
class  GLProgressFromTo : public ActionInterval
{
public:
	/**
	* @brief Create and initializes the action with a duration, a "from" percentage and a "to" percentage.
	* @param duration Specify the duration of the ProgressFromTo action. It's a value in seconds.
	* @param fromPercentage Specify the source percentage.
	* @param toPercentage Specify the destination percentage.
	* @return If the creation success, return a pointer of ProgressFromTo action; otherwise, return nil.
	*/
	static GLProgressFromTo* create(float duration, float fromPercentage, float toPercentage, int type = 0);

	//
	// Overrides
	//
	virtual GLProgressFromTo* clone() const override;
	virtual GLProgressFromTo* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	GLProgressFromTo() {}
	virtual ~GLProgressFromTo() {}

	/**
	* @brief Initializes the action with a duration, a "from" percentage and a "to" percentage.
	* @param duration Specify the duration of the ProgressFromTo action. It's a value in seconds.
	* @param fromPercentage Specify the source percentage.
	* @param toPercentage Specify the destination percentage.
	* @return If the creation success, return true; otherwise, return false.
	*/
	bool initWithDuration(float duration, float fromPercentage, float toPercentage);
	int _type;
protected:
	float _to;
	float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(GLProgressFromTo);
};


class GLTextNumTo : public ActionInterval
{
public:
	static GLTextNumTo* create(float dur, int from, int to, std::string formet = "%d", int texttype = 0);

	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

	bool initWithDuration(float duration, int fromPercentage, int toPercentage, std::string formet, int type);

CC_CONSTRUCTOR_ACCESS:
	GLTextNumTo() {}
	virtual ~GLTextNumTo() {}

protected:
	int _to;
	int _from;
	int _texttype;
	std::string _formet;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(GLTextNumTo);
};

#endif // __ACTION_CCPROGRESS_TIMER_H__
