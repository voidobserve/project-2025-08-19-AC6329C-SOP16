#include "rf433_app.h"
#include "system/includes.h"
#include "task.h"
#include "event.h"
#include "btstack/btstack_typedef.h"
#include "app_config.h"
#include "WS2812FX.H"
#include "led_strip_sys.h"


#if TCFG_RF433GKEY_ENABLE


#define RF433_PIN	IO_PORTA_07

u8 rf_key_val = NO_KEY;
u16 RF433_T0;  
u16 RF433_T1; 
u16 RF433_CODE = 0xFFFF;

u8 my_ble_state = 1; //默认开启BLE模块

sys_cb_t sys_cb;


void rf433_gpio_init(void)
{

	gpio_set_die(RF433_PIN, 1);
	gpio_set_direction(RF433_PIN, 1);
	gpio_set_pull_up(RF433_PIN,1);
}

u8 rf_key_get(void)
{
    u8 key_val = NO_KEY;
    key_val = rf_key_val;
	return key_val;
}


struct key_driver_para rf_scan_para = {
    .scan_time 	  	  = 50,				//按键扫描频率, 单位: ms
    .last_key 		  = NO_KEY,  		//上一次get_value按键值, 初始化为NO_KEY;
    .filter_time  	  = 0,				//按键消抖延时;
    .long_time 		  = 75,  			//按键判定长按数量
    .hold_time 		  = (75 + 15),  	//按键判定HOLD数量
    .click_delay_time = 2,//20,			//按键被抬起后等待连击延时数量
    .key_type		  = KEY_DRIVER_TYPE_RF433,
    .get_value 		  = rf_key_get,
};

/**
 * @brief 红外解码， 固定125us定时执行
 * 
 */
void timer125us_hook(void)
{
	static u8 Data_Flag,bData_0_1; //数据标志位，数据0,1状态位
	static u8 bit_end,bit_suceess,bit_Syn;
	static u8 Pluse_H_cnt,Pluse_L_cnt;
	static u8 press_cnt;
	static u16 key_up_time = 0;
	static u16 key_tmp;
	static u8 rf_bit_cnt;
	u32 rf_inside_code;
	u8 rf_key_data;
	static u16 last_key_tmp = 0;
	static u8 wrong_cnt = 0;
	static u32 last_code = 0;
	u32 TmpDet = 0;
	u8  TmpCnt = 0;
	static u8 TmpKey = 0;

			/*					开始
							_____
							| 			|
							| 			|
							| 			| 	结束 上一个码也检波成功
			_______|			 |_________

			*/
	// 每个码是由高开始，下一个高之前结束。
	if(gpio_read(RF433_PIN)!=0)
	{
			if(bit_end)
			{
                bit_end = 0;
                bit_suceess = 1;
			}
			else
			{
                Pluse_H_cnt++;
                Pluse_L_cnt =0;
			}
	}
	else
	{
			bit_end = 1;
			Pluse_L_cnt++;
			if(bit_Syn==0){
                Pluse_H_cnt = 0;
			}
	}


	////////////////有个新波形来
	//低电平后在8ms到 15ms		//76
	// if(bit_suceess&&(Pluse_L_cnt>48)&&(Pluse_L_cnt<120))
	if(bit_suceess&&(Pluse_L_cnt>80)&&(Pluse_L_cnt<164))
	{
			//同步码就是开始是8ms到15ms的低电平，后来就是码值
			bit_suceess = 0;
			bit_Syn =1;
			Pluse_H_cnt = 0;
			Pluse_L_cnt = 0;
			rf_bit_cnt = 0;
			Data_Flag = 0;
			sys_cb.RFData = 0;
	}

	///////////////////////////////译码//////////////////////
	if(bit_Syn)//同步码解码成功后执行
	{
		if(bit_suceess)
		{
				bit_suceess = 0;
				/*	    (5~13 )*125us														1
						_____________
						| 			|
						| 			|  (1~4)*125US
						| 			| 						|
					_______|			|_______ |

				*/		 //编码为1，高电平在
				if((Pluse_H_cnt>=6)&&(Pluse_H_cnt<=13)&&(Pluse_L_cnt<=4)&&(Pluse_L_cnt>=2))
				{
						bData_0_1 = 1;
						Data_Flag =1;
						// printf("h\n");
				}

				/*	(1~4)*125US 																			0
						_____
						| 			|
						| 			| 																 |
						| 			| 	(5~13 )*125us 				 |
				_______|			 |____________________|

				*/
				//编码为0，
				else if((Pluse_L_cnt>=6)&&(Pluse_L_cnt<=13)&&(Pluse_H_cnt<=4)&&(Pluse_H_cnt>=2))
				{
						bData_0_1 = 0;
						Data_Flag =1;
						// printf("l\n");
				}
				else
				{
					//两者都不是
					bit_suceess = 0;
					Pluse_H_cnt = 0;
					Pluse_L_cnt = 0;
					rf_bit_cnt = 0;
					Data_Flag = 0;
					sys_cb.RFData = 0;

				}

				Pluse_H_cnt = 1;	/*现在解码是解上一个码的，
									译码标志是以现在这个码第一个高电平计数。，
									在上面没有叠加高位时间，现在补偿H_pluse，所以从1开始*/
				Pluse_L_cnt = 0;
		}
	}


	if(Data_Flag) //数据标志位有效
	{
		Data_Flag =0;
		//先到的数据低位数组BYTE ，
		//先发地址，再发数据共24bit ；3个数组
		sys_cb.RFData >>= 1;
		rf_bit_cnt++;

		if(bData_0_1)
		{
			sys_cb.RFData |= 0x800000;
		}

		if(rf_bit_cnt == 24) //接收完20位内码 + 4位数据  长按时大概50ms入一次
		{
			bit_Syn=0;
			rf_bit_cnt = 0;
			// RF_BuffNum = 0;
			bit_suceess = 0;
			bit_end = 0;
			Pluse_H_cnt = 0;
			Pluse_L_cnt = 0;


            RF433_T0 = 0;
            RF433_T1++;

			/* 提取键值 */
			// 头部0xFF
			// printf("\n sys_cb.RFData=%d",sys_cb.RFData);

			sys_cb.RFAddrCode1 = sys_cb.RFData & 0xFFFF;
			//if( sys_cb.RFAddrCode1 == 0xFFFF)
		//	{
				rf_key_val = sys_cb.RFData >> 16;
				printf("\n rf_key_val=%X",rf_key_val);
		//	}
			sys_cb.RFData = 0;

		}
	}

}


void rf433_handle(void)
{

    u8 key_value = 0;
    if(RF433_T0 < 0xFFFF)
    RF433_T0++;


//短按
    if(RF433_T0 > 15 && RF433_T1 <= 100)
    {
        key_value = rf_key_get();
        if(key_value == NO_KEY) return ;
        rf_key_val = NO_KEY;
        RF433_T0 = 0; 
        RF433_T1 = 0;

        printf(" ------------------------duanna key_value = %x", key_value);

        if(sys_cb.RFAddrCode1 == 0xFFFF && RF433_CODE == 0xFFFF)  //判断客户码
        {

            //开
            if(key_value == RFKEY_ON)
            {
                soft_turn_on_the_light();
            }
            if(key_value == RFKEY_OFF)
            {
                soft_turn_off_lights();
            }
            if(get_on_off_state())
            {

                //速度/亮度 -
                if( key_value == RFKEY_LIGHT_SUB)
                {

                    ls_sub_bright();
                    ls_add_speed(); 
                    ls_sub_sensitive();


                }


                //速度/亮度 +
                if( key_value == RFKEY_LIGHT_PLUS)
                {
                    ls_add_bright();
                    ls_sub_speed();
                    ls_add_sensitive();


                }


                //红色
                if(key_value == RFKEY_R )
                {
                    set_static_mode((RED>>16)&0xff,(RED>>8)&0xff,(RED>>0)&0xff);
                }
                //绿色
                if(key_value == RFKEY_G )
                {
                    set_static_mode((GREEN>>16)&0xff,(GREEN>>8)&0xff,(GREEN>>0)&0xff);
                }
                //蓝色
                if(key_value == RFKEY_B)
                {
                    set_static_mode((BLUE>>16)&0xff,(BLUE>>8)&0xff,(BLUE>>0)&0xff);
                }
                //白色 
                if(key_value == RFKEY_W)
                {
                
                set_static_mode((WHITE>>16)&0xff,(WHITE>>8)&0xff,(WHITE>>0)&0xff);

                }
                //橙色 
                if(key_value == RFKEY_ORANGE )
                {
                    set_static_mode((ORANGE>>16)&0xff,(ORANGE>>8)&0xff,(ORANGE>>0)&0xff);
                    
                }
                //黄色
                if(key_value == RFKEY_YELLOW  )
                {
                    set_static_mode((YELLOW>>16)&0xff,(YELLOW>>8)&0xff,(YELLOW>>0)&0xff);

                }
                //青色
                if(key_value == RFKEY_CYAN)
                {
                    set_static_mode((CYAN>>16)&0xff,(CYAN>>8)&0xff,(CYAN>>0)&0xff);

                }
                //紫色
                if(key_value == RFKEY_PURPLE)
                {
                    set_static_mode((MAGENTA>>16)&0xff,(MAGENTA>>8)&0xff,(MAGENTA>>0)&0xff);

                }
                //3色跳变
                if(key_value == RFKEY_JUMP3)
                {

                    ls_chose_mode_InAPP(1,3,0x07,0x07);
                }
                //7色跳变
                if(key_value == RFKEY_JUMP7)
                {

                    ls_chose_mode_InAPP(1,3,0x08,0x08);
                }
                //7个单色呼吸切换
                if(key_value == RFKEY_FADE3)
                {
                    ls_change_breath_mode();
                
                }
                //彩虹
                if(key_value == RFKEY_FADE7)
                {
                    ls_SM_rainbow();
                }
            
                if(key_value == RFKEY_MUSIC1)
                {
                    app_set_music_mode(0);
                }
                if(key_value == RFKEY_MUSIC2)
                {
                    app_set_music_mode(1);
                }
                if(key_value == RFKEY_MUSIC3)
                {
                    app_set_music_mode(2);
                }
                if(key_value == RFKEY_MUSIC4)
                {
                    app_set_music_mode(3);
                }

                //////////  电机相关  ////////////

                if(key_value == MOTOR_ON)
                {
                    ls_set_motor_on();
                    
                }
                if(key_value == MOTOR_OFF)
                {
                    ls_set_motor_off();
                    
                }
                if(key_value == MOTOR_SPEED_PUL)
                {
                    ls_add_motor_speed();
                    
                }
                if(key_value == MOTOR_SPEED_SUB)
                {
                    ls_sub_motor_speed();
                    
                }

            }
            save_user_data_area3();
     

        }  //判断客户码




    }
    else  if(RF433_T0 > 15)  //松手
    {
        RF433_T1 = 0;
        key_value = rf_key_get();
        rf_key_val = NO_KEY;  //
      
    }
//长按
    if(RF433_T1 > 100)  // 100*50
    {
     
        key_value = rf_key_get();
        if(key_value == 255) return ;
        rf_key_val = NO_KEY;
        RF433_T0 = 0; 

        printf(" ================================longkey_value = %d", key_value);

        if(sys_cb.RFAddrCode1 == 0xFFFF && RF433_CODE == 0xFFFF)  //判断客户码 原本遥控
        {
           
            if(key_value == RFKEY_SPEED_PLUS)
            {
                printf("open ble");
                my_ble_state =1;
                bt_ble_init();
               
            }
            else if(key_value == RFKEY_SPEED_SUB)
            {
               
                printf("close ble");
                bt_ble_exit();
                my_ble_state = 0;
               
            }
            save_user_data_area3();
        }


    
    }



}






#endif



