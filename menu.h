#pragma once

#include "globe.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int NumHigh;
// 按键管理,mode=1renovate
int KeyManage(int keycode, int mode);

// 出现第几页的参数菜单
int ParaMenu1(int page);

// 出现第几页的参数菜单
int ParaMenu(int page);

int ChannelMenu(void);

// 出现第几页的参数菜单
int OptionMenu(int page);

int FuncMenu(void);

extern const uint8_t _COLOR_Menu[C_LANGUAGE][9][15];
extern const uint8_t _COLOR_A1[C_LANGUAGE][10][15];
extern const uint8_t _COLOR_B1[C_LANGUAGE][9][8];

int ColorMenu(void);

int  CustomizeColorMenu(void);
void InitPara(void);

void InitParaTemp(void);

int STDMenu(void);

// 调用已存储的数据并覆盖到当前通道
int DataCall(int file_no, int SaveMode);

// 测高
int  TestFlawHigh(void);

int  TestForging(void);
void BScan(void);

#ifdef __cplusplus
}
#endif
