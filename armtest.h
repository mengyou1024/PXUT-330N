#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void RDDelay(uint32_t time);

/**
 * @brief 输出一个字符串
 *
 * @param x start display coor
 * @param y start display coor
 * @param pzText characters' buf
 * @param iSectionLen characters' iSectionLen
 * @param color Display color
 */
void FTextOut(int x, int y, char *pzText, int iSectionLen, int color);

/**
 * @brief 按十六进制输出字符串中的内容
 *
 * @param x start display coor
 * @param y start display coor
 * @param pzText characters' buf
 * @param iSectionLen characters' iSectionLen
 * @param color1 Display color 1
 * @param color2 Display color 2
 */
void HexTextOut(int x, int y, char *pzText, int iSectionLen, int color1, int color2);

/**
 * @brief 用格式化方式打印一个数字
 *
 * @param x start display coor
 * @param y start display coor
 * @param value
 * @param iSectionLen
 * @param mode 0  十进制 1 十六进制
 */
void FormatPrint(int x, int y, int value, int iSectionLen, int mode);

int GetCh(void);

/**
 * @brief 延时一个固定的时间，在该时间内有任意键按下，按"返回"键退出
 *
 * @param delay_second  延时时间，以秒为单位
 * @return int
 */
int RDDelayFixedTime(uint32_t delay_second);

/**
 * @brief 只有按键释放后才返回键值，按下时是直接返回的
 *
 * @return int 返回按键的绝对值
 */
int RDKbhit(void);

/**
 * @brief 让蜂鸣器鸣叫叫一段时间，以100ms为单位
 *
 * @param time 鸣叫时间
 */
void RDBuzzerOn(uint32_t time);
void DebugIntData(int x, int y, char *cTips, int ii);

#ifdef __cplusplus
}
#endif
