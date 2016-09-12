#ifndef _DEFINES_H_
#define _DEFINES_H_

#define PAY_SDK_FREE 0
#define PAY_SDK_MILI 1
#define PAY_SDK_JIDI 2
#define PAY_SDK_DIANXIN 3
#define PAY_SDK_UNIPAY 4
#define PAY_SDK_ALLPAY 5
#define PAY_SDK_BAIDU 6

#ifndef CC_PAY_SDK
#define CC_PAY_SDK PAY_SDK_FREE
#endif

#if (CC_PAY_SDK == PAY_SDK_ALLPAY || CC_PAY_SDK == PAY_SDK_JIDI || CC_PAY_SDK == PAY_SDK_UNIPAY || CC_PAY_SDK == PAY_SDK_BAIDU)

#define CC_ENABLE_CHECK_SIGN 0

#else

#define CC_ENABLE_CHECK_SIGN 1

#endif


#ifndef CC_ENABLE_DEBUG_MONSTER_ANIM
#define CC_ENABLE_DEBUG_MONSTER_ANIM 0
#endif

#ifndef CC_ENABLE_NEW_PARAM
#define CC_ENABLE_NEW_PARAM 1
#endif

#ifndef CC_ENABLE_OWN_BUYCONFIRM
#define CC_ENABLE_OWN_BUYCONFIRM 1
#endif

#ifndef CC_ENABLE_CHECK_SIGN
#define CC_ENABLE_CHECK_SIGN 1
#endif

#endif
