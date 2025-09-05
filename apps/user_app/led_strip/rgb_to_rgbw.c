#include "typedef.h"
#include "stdlib.h"

/**
 * @brief 将RGB值转换为RGBW值
 * 
 * @param r 红色值 (0-255)
 * @param g 绿色值 (0-255)
 * @param b 蓝色值 (0-255)
 * @param rw 输出的红色值指针
 * @param gw 输出的绿色值指针
 * @param bw 输出的蓝色值指针
 * @param ww 输出的白色值指针
 */
void rgb_to_rgbw(u8 r, u8 g, u8 b, u8 *rw, u8 *gw, u8 *bw, u8 *ww)
{
    // 计算RGB中的最小值，作为白色分量
    u8 min_rgb = r;
    if (g < min_rgb) min_rgb = g;
    if (b < min_rgb) min_rgb = b;
    
    // 将白色分量从RGB中提取出来
    *ww = min_rgb;
    
    // 剩余的RGB值减去白色分量
    *rw = r - min_rgb;
    *gw = g - min_rgb;
    *bw = b - min_rgb;
}

/**
 * @brief 将RGB值转换为RGBW值（改进版）
 * 
 * @param r 红色值 (0-255)
 * @param g 绿色值 (0-255)
 * @param b 蓝色值 (0-255)
 * @param rw 输出的红色值指针
 * @param gw 输出的绿色值指针
 * @param bw 输出的蓝色值指针
 * @param ww 输出的白色值指针
 */
void rgb_to_rgbw_improved(u8 r, u8 g, u8 b, u8 *rw, u8 *gw, u8 *bw, u8 *ww)
{
    // 计算RGB中的最小值，作为白色分量的基准
    u8 min_rgb = r;
    if (g < min_rgb) min_rgb = g;
    if (b < min_rgb) min_rgb = b;
    
    // 计算RGB中的最大值
    u8 max_rgb = r;
    if (g > max_rgb) max_rgb = g;
    if (b > max_rgb) max_rgb = b;
    
    // 对于接近灰色的颜色，更多地使用白色LED
    // 计算RGB色彩饱和度 (最大值-最小值)
    u8 saturation = max_rgb - min_rgb;
    
    // 根据饱和度决定白色LED的使用程度
    // 饱和度越低（越接近灰色），使用越多的白色LED
    if (saturation < 50) {
        // 低饱和度，更多使用白色LED
        *ww = min_rgb;
        *rw = r - min_rgb;
        *gw = g - min_rgb;
        *bw = b - min_rgb;
    } else if (saturation < 100) {
        // 中等饱和度，适度使用白色LED
        *ww = min_rgb / 2;
        *rw = r - *ww;
        *gw = g - *ww;
        *bw = b - *ww;
    } else {
        // 高饱和度，较少使用白色LED
        *ww = min_rgb / 4;
        *rw = r - *ww;
        *gw = g - *ww;
        *bw = b - *ww;
    }
    
    // 确保值不会为负数
    if (*rw > r) *rw = 0;
    if (*gw > g) *gw = 0;
    if (*bw > b) *bw = 0;
}

/**
 * @brief 将32位RGB颜色值转换为RGBW值
 * 
 * @param rgb 32位RGB颜色值
 * @param rw 输出的红色值指针
 * @param gw 输出的绿色值指针
 * @param bw 输出的蓝色值指针
 * @param ww 输出的白色值指针
 */
void rgb32_to_rgbw(u32 rgb, u8 *rw, u8 *gw, u8 *bw, u8 *ww)
{
    u8 r = (rgb >> 16) & 0xFF;
    u8 g = (rgb >> 8) & 0xFF;
    u8 b = rgb & 0xFF;
    
    rgb_to_rgbw_improved(r, g, b, rw, gw, bw, ww);
}