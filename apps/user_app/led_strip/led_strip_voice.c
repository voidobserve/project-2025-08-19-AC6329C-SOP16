#include "led_strip_voice.h"
#include "asm/adc_api.h"
#include "led_strip_drive.h"

#define MAX_SOUND 10
struct MUSIC_VOICE_T
{
    u8 sound_trg;
    u8 meteor_trg;
    u32 adc_sum;
    u32 adc_sum_n;
    int sound_buf[MAX_SOUND];
    u8 sound_cnt;
    int c_v;
    int v;
    u8 valid;
};

struct MUSIC_VOICE_T music_voic = {

    .sound_trg = 0,
    .meteor_trg = 0,
    .adc_sum = 0,
    .adc_sum_n = 0,
    .sound_cnt = 0,
    .valid = 0,
    .v = 0,
    .c_v = 0,
};

// 获取声控结果
// 触发条件：（（当前声音大小 - 平均值）* 100 ）/ 平均值 > 灵敏度（0~100）
// 0:没触发
// 1:触发
u8 get_sound_result(void)
{
    u8 p_trg;
    p_trg = music_voic.sound_trg;
    music_voic.sound_trg = 0;
    return p_trg;
}

u8 get_meteor_result(void)
{
    u8 p_metemor_trg;
    p_metemor_trg = music_voic.meteor_trg;
    music_voic.meteor_trg = 0;
    return p_metemor_trg;
}

#if 1 // 移植其他项目的声控程序
// 声控模式使用到的相关定义：
#define SAMPLE_N 20
volatile u8 music_trigger = 0;
u8 adc_v_n, adc_avrg_n;
u32 adc_sum = 0, adc_sum_n = 0;
extern uint8_t met_trg;
extern uint8_t trg_en;
// extern void set_music_oc_trg(u8 p);
u8 i, j;
u32 adc, adc_av, adc_all;
u16 adc_v[SAMPLE_N]; // 记录20个ADC值
u32 adc_avrg[10];    // 记录5个平均值
u32 adc_total[15];   // __attribute__((aligned(4)));
#endif               // #if 1 // 移植其他项目的声控程序

void sound_handle(void)
{
    // 实际测试约13~14ms进入一次该函数：
    // { 
    //     static u16 cnt = 0;
    //     cnt++;

    //     if (cnt >= 100)
    //     {
    //         cnt = 0;
    //         printf("sound_handle\n");
    //     }
    // }

    u16 adc;
    // u8 i;
    // 记录adc值
#if 0 
    if (fc_effect.on_off_flag == DEVICE_ON && (fc_effect.Now_state == IS_light_music))
    {
        music_voic.sound_buf[music_voic.sound_cnt] = check_mic_adc();
        music_voic.c_v = music_voic.sound_buf[music_voic.sound_cnt]; // 记录当前值
        music_voic.sound_cnt++;

        if (music_voic.sound_cnt > (MAX_SOUND - 1))
        {
            music_voic.sound_cnt = 0;
            music_voic.valid = 1;
            music_voic.v = 0;
            for (i = 0; i < MAX_SOUND; i++)
            {
                music_voic.v += music_voic.sound_buf[i];
            }

            music_voic.v = music_voic.v / MAX_SOUND; // 计算平均值
        }

        if (music_voic.valid)
        {

            if (music_voic.c_v > music_voic.v)
            {
                if ((music_voic.c_v - music_voic.v) * 100 / music_voic.v > fc_effect.music.s) // 很灵敏
                {
                    music_voic.sound_trg = 1;  // 七彩声控
                    music_voic.meteor_trg = 1; // 流星声控
                }
            }
        }
    }
    else
    {
        music_voic.valid = 0;
    }
#endif

#if 1 // 移植其他项目的声控程序
    extern u32 adc_get_value(u32 ch);
    extern void WS2812FX_trigger();
    u8 trg;
    u8 trg_v;
    u32 adc_all;
    u32 adc_ttl;

    extern u32 adc_sample(u32 ch);

    // if (fc_effect.on_off_flag == DEVICE_ON && fc_effect.Now_state == IS_light_music)
    // {

    // }

    // 设备未开启，或是状态不是音乐模式，直接退出
    if (fc_effect.on_off_flag != DEVICE_ON || fc_effect.Now_state != IS_light_music)
    {
        return;
    }

    // 记录adc值
    // adc = adc_get_value(AD_CH_PA8);
    adc = check_mic_adc(); // 每次进入，采集一次ad值（即使不在声控模式，也会占用一些时间）

    // printf("adc = %d", adc);

    if (adc >= 1000)
    {
        return;
    }

    // if (adc < 1000) // 当ADC值大于1000，说明硬件电路有问题
    // {
    if (adc_sum_n < 2000)
    {
        // 从0开始，一直加到2000，每10ms加一，总共要20s
        adc_sum_n++;
    }

    if (adc_sum_n == 2000)
    {
        if (adc / (adc_sum / adc_sum_n) > 3)
            return; // adc突变，大于平均值的3倍，丢弃改值

        adc_sum = adc_sum - adc_sum / adc_sum_n;
    }

    adc_sum += adc; // 累加adc值

    adc_v_n %= SAMPLE_N;
    adc_v[adc_v_n] = adc;
    adc_v_n++;
    adc_all = 0;

    // 计算ad值总和
    for (u8 i = 0; i < SAMPLE_N; i++)
    {
        adc_all += adc_v[i];
    }

    // 获取ad值平均值
    adc_avrg_n %= 10;
    adc_avrg[adc_avrg_n] = adc_all / SAMPLE_N;
    adc_avrg_n++;
    adc_ttl = 0;

    // 在平均值的基础上，再求总和
    for (u8 i = 0; i < 10; i++)
    {
        adc_ttl += adc_avrg[i];
    }

    memmove((u8 *)adc_total, (u8 *)adc_total + 4, 14 * 4); // 将 src 指向的内存区域中的前 n 个字节复制到 dest 指向的内存区域（能够安全地处理内存重叠的情况）

    adc_total[14] = adc_ttl / 10; // 总数平均值
    trg = 0;

    if (adc_sum_n != 0)
    {
        if (adc * fc_effect.music.s / 100 > adc_sum / adc_sum_n)
        {

            trg = 200;
            // met_trg = 1;
            // trg_en = 1;
            music_trigger = 1;
            // music_voic.sound_trg = 1;  // 七彩声控
            // music_voic.meteor_trg = 1; // 流星声控

            // if (fc_effect.on_off_flag == DEVICE_ON && fc_effect.Now_state == IS_light_music)
            WS2812FX_trigger(); // 让主循环扫描到立刻更新动画

            // if (adc > adc_sum / adc_sum_n)
            // {
            //     set_music_oc_trg((adc - adc_sum / adc_sum_n) * 100 * fc_effect.music.s / 100 / (adc_sum / adc_sum_n));
            //     extern void set_music_fs_trg(u8 p);
            //     set_music_fs_trg((adc - adc_sum / adc_sum_n) * 100 * fc_effect.music.s / 100 / (adc_sum / adc_sum_n)); // 频谱控制
            // }
        }
    }
    // } // if (adc < 1000) // 当ADC值大于1000，说明硬件电路有问题

#endif // 移植其他项目的声控程序
}
