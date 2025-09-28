#ifndef _RF433_APP_H_
#define _RF433_APP_H_
#include "board_ac632n_demo_cfg.h"

//*********************************************************************************//
//                                 433遥控 配置                                      //
//*********************************************************************************//

#define TCFG_RF433GKEY_ENABLE			    1 //是否使能433遥控




#if TCFG_RF433GKEY_ENABLE

typedef struct {
	unsigned char RFStudyFlag;
	unsigned int  RFData;
	unsigned int  RFAddrCode1;
	unsigned int  RFAddrCode2;
	unsigned int  RFAddrCodeLast;
	unsigned char RFCodeInit;
	unsigned char RFStudyCount;
	unsigned char RFKeyTableNum;
	unsigned char RFKeyTableNum2;
	unsigned char RFKeyValue;
} sys_cb_t;
//黑色遥控，FFFF
//开启或关闭ble功能
#define RFKEY_SPEED_PLUS 0xC0
#define RFKEY_SPEED_SUB 0x20
#define RFKEY_MODE_ADD 0x40
#define RFKEY_ON_OFF 0x80
//七彩
#define RFKEY_ON 0xE0
#define RFKEY_OFF 0x10
#define RFKEY_LIGHT_PLUS 0xA0
#define RFKEY_LIGHT_SUB 0x60
#define RFKEY_R 0x90
#define RFKEY_G 0x50
#define RFKEY_B 0xD0
#define RFKEY_YELLOW 0x70
#define RFKEY_CYAN 0xF0
#define RFKEY_PURPLE 0x8
#define RFKEY_ORANGE 0xB0
#define RFKEY_JUMP3 0x88
#define RFKEY_JUMP7 0x48
#define RFKEY_FADE3 0xC8
#define RFKEY_FADE7 0x28

#define RFKEY_W 0x30
#define RFKEY_MUSIC1 0xA8
#define RFKEY_MUSIC2 0x68
#define RFKEY_MUSIC3 0xE8
#define RFKEY_MUSIC4 0x18

//电机
#define MOTOR_ON         0x98
#define MOTOR_OFF        0X58
#define MOTOR_SPEED_PUL  0XD8
#define MOTOR_SPEED_SUB  0X38




#endif

#endif

