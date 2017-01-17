#ifndef _DEFINES_H_
#define _DEFINES_H_

#define PAY_SDK_FREE 0
#define PAY_SDK_MIGU 1
#define PAY_SDK_ZHUOYI 2

#ifndef CC_PAY_SDK
#define CC_PAY_SDK PAY_SDK_FREE
#endif

#if (CC_PAY_SDK == PAY_SDK_MIGU)

#define CC_ENABLE_CHECK_SIGN 0

#else

#define CC_ENABLE_CHECK_SIGN 1

#endif

#ifndef CC_ENABLE_NEW_WORLD
#define CC_ENABLE_NEW_WORLD 0
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

#define USING_TIME_MGR

#endif
