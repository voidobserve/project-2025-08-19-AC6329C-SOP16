#ifndef __RGB_TO_RGBW_H
#define __RGB_TO_RGBW_H

#include "typedef.h"

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
void rgb_to_rgbw(u8 r, u8 g, u8 b, u8 *rw, u8 *gw, u8 *bw, u8 *ww);

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
void rgb_to_rgbw_improved(u8 r, u8 g, u8 b, u8 *rw, u8 *gw, u8 *bw, u8 *ww);

/**
 * @brief 将32位RGB颜色值转换为RGBW值
 * 
 * @param rgb 32位RGB颜色值
 * @param rw 输出的红色值指针
 * @param gw 输出的绿色值指针
 * @param bw 输出的蓝色值指针
 * @param ww 输出的白色值指针
 */
void rgb32_to_rgbw(u32 rgb, u8 *rw, u8 *gw, u8 *bw, u8 *ww);

#endif // __RGB_TO_RGBW_H