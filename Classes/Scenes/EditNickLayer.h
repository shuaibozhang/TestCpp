#ifndef __EDIT_NICK_LAYER_H__
#define __EDIT_NICK_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class EditNickLayer : public Layer
{
public:
	EditNickLayer();
	~EditNickLayer();

	static EditNickLayer *getInstance(void);

	CREATE_FUNC(EditNickLayer);
	bool init();

	void updateUI(void);

	void menuOnBackCB(Ref* pSender);
	void menuOnConfirmCB(Ref* pSender);
	void menuOnCancelCB(Ref* pSender);

	virtual void update(float delta);

	CC_SYNTHESIZE_READONLY(ui::Button *, _pBtnClose, BtnClose);
protected:
//	extension::EditBox *m_pEditBox;
	Node *_pRoot;
private:
	static EditNickLayer *s_pInstance;
	int _headId;
	std::string _strNick;
};

#endif