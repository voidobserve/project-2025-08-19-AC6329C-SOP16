#ifndef rf433_app_h
#define rf433_app_h


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



//17键流星遥控 62FC

#define _17_ON_OFF         0x80  //开关
#define _17_METEOR_LEN_PUL 0xA0  //拖尾+
#define _17_SPEED_SUB      0xE0  //速度-
#define _17_RESET          0x10  //复位 流星7
#define _17_SPEED_PUL      0x90//速度+
#define _17_PERIOD_PUL     0x50  //时间间隔+
#define _17_METEOR_LEN_SUB 0xD0  //拖尾-
#define _17_BRIGHT_PUL     0x30  //亮度+
#define _17_PERIOD_SUB     0xB0  //时间间隔-
#define _17_METEOR_DIR     0x70  //正反流
#define _17_BRIGHT_SUB     0xF0  //亮度-
#define _17_MIN_PERIOD     0x08  //周期最短
#define _17_PERIOD_10S     0x88 //定周期 10s
#define _17_MAX_PERIOD     0x48 //周期最长
#define _17_METEOR_MUSIC1  0xC8 //律动1
#define _17_METEOR_MUSIC2  0x28  //律动2
#define _17_METEOR_MUSIC3  0xA8 //律动3












#endif

