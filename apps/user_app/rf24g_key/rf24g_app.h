#ifndef _RF24G_APP_H_
#define _RF24G_APP_H_

#include "board_ac632n_demo_cfg.h"

#if TCFG_RF24GKEY_ENABLE

#pragma pack (1)
//遥控配�??
typedef struct
{
    u8 pair[3];
    u8 flag;    //0:表示该数组没使用�?0xAA：表示改数组已配对使�?
}rf24g_pair_t;

//2.4G遥控 不同的遥控，该结构体不一�?
typedef struct
{
    u8 header1;
    u8 header2;
    u8 key_v;
    u8 pair[3];         //客户码
    u8 dynamic_code;    //  动态码
}rf24g_ins_t;   //指令数据


struct RF24G_PARA{


    u8 rf24g_rx_flag ;
    u8 last_dynamic_code;   
    u8 last_key_v;
    u8 rf24g_key_state;   
    u8 clink_delay_up;
    u8 long_press_cnt;
    u16 hold_pess_cnt;
    const u16 is_long_time;
    const u8 is_click;
    const u8 is_long;
    const u8 _sacn_t;
};

#pragma pack ()



#define RF24_K01 0x11	
#define RF24_K02 0x12	
#define RF24_K03 0x13	
#define RF24_K04 0x14	

#define RF24_K05 0x21  
#define RF24_K06 0x22	
#define RF24_K07 0x23	
#define RF24_K08 0x24	

#define RF24_K09 0x31	
#define RF24_K10 0x32	
#define RF24_K11 0x33	
#define RF24_K12 0x34

#define RF24_K13 0x41	
#define RF24_K14 0x42	
#define RF24_K15 0x43	
#define RF24_K16 0x44	

#define RF24_K17 0x51	
#define RF24_K18 0x52	
#define RF24_K19 0x53	
#define RF24_K20 0x54

#define RF24_K21 0x61	
#define RF24_K22 0x62	
#define RF24_K23 0x63	
#define RF24_K24 0x64

#define RF24_K25 0x01	
#define RF24_K26 0x02	
#define RF24_K27 0x03	
#define RF24_K28 0x04	

extern rf24g_pair_t rf24g_pair[];        //需要写flash
extern rf24g_ins_t rf24g_ins;


void RF24G_Key_Handle(void);


#endif
#endif

