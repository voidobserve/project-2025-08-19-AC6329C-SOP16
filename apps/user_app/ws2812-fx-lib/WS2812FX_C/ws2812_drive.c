
#include "ws2812_bsp.h"
#include "debug.h"
#include "my_effect.h"

#include "led_strip_drive.h"

static unsigned long tick_ms;
void ws281x_init()
{
}
void ws281x_show(unsigned char *pixels_pattern, unsigned short pattern_size)
{
    // extern void fc_driver(u8 r,u8 g ,u8 b);
    // fc_driver(  *pixels_pattern,\      
    //             *(pixels_pattern + 1),\  
    //             *(pixels_pattern + 2));
    // ledc_send_rgbbuf(0, pixels_pattern, pattern_size, 0);

    fc_rgbw_driver(*pixels_pattern,
                   *(pixels_pattern + 1),
                   *(pixels_pattern + 2),
                   *(pixels_pattern + 3));
}

// 周期10ms
unsigned long HAL_GetTick(void)
{
    return tick_ms;
}

// 每10ms调用一次
void run_tick_per_10ms(void)
{
    tick_ms += 10;
}
