#pragma once

#include "globe.h"
#include <stdint.h>

// 请先解键盘锁
#define C_STD_MAX         13 // 10
#define C_STD_LEN         22
#define C_BLOCK_MAX       6
#define C_BLOCK_LEN       15
#define C_STD_POS_THNUM   0
#define C_STD_POS_LINENUM 1
#define C_STD_POS_THICK   2

#ifdef __cplusplus
extern "C" {
#endif

extern const char _us[C_LANGUAGE][1][4];

extern const uint8_t _Power_prompt[C_LANGUAGE][8][40];

extern const uint16_t _Power_prompt3[];

extern const uint16_t _Power_prompt2[];

extern const char _Menu_Power[C_LANGUAGE][5][90];

extern const uint8_t _Menu_Power1E[];

extern const uint16_t _Menu_Power1[];

extern const uint16_t _Menu_Power2[];

extern const uint16_t _Menu_Power3[];

extern const uint16_t _Menu_Power4[];

extern const uint16_t _Menu_Power5[];

extern const uint8_t _Menu_Power3E[];

extern const uint8_t _Menu_Power4E[];

extern const uint8_t _Menu_Power5E[];

extern const char _ParaName[C_LANGUAGE][21][16];

extern const uint16_t _Report_Info1[];

extern const uint16_t _Report_Info2[];

extern const uint16_t _Report_Info3[];

extern const uint16_t _Report_Info32[];

extern const uint16_t _Report_Info31[];

extern const uint16_t _Report_Info4[];

extern const char _Menu_DB[C_LANGUAGE][4][28];

extern const char _TestMenu_A1[C_LANGUAGE][8][13];

extern const uint8_t _Menu_Choice[C_LANGUAGE][10][22];

extern const char _AmplitudedB[C_LANGUAGE][2][30];

extern const char _TestMenu_Menu[C_LANGUAGE][2][13];

extern const uint8_t _TestMenu_Enter[];

extern const uint8_t _TestMenu_AB1[C_LANGUAGE][4][17];

extern const uint8_t _TestMenu_AB3[C_LANGUAGE][4][85];

extern const uint8_t _TestForging[C_LANGUAGE][9][95];

extern const uint8_t _TestMenu_AC1[C_LANGUAGE][4][18];

extern const uint8_t _TestMenu_DAC[C_LANGUAGE][5][17];

extern const uint8_t _TestMenu_AVG[C_LANGUAGE][5][17];

extern const char _Menu_FJ[C_LANGUAGE][12][40];

extern const uint8_t _TestMenu_set[C_LANGUAGE][3][28];

extern const uint8_t _Forging_A1[C_LANGUAGE][2][34];

extern const uint8_t _Forging_A2[C_LANGUAGE][2][38];

extern const uint8_t _TestMenu_AD2[C_LANGUAGE][4][34];

extern const uint16_t _TestMenu_AF1[];

extern const uint8_t _TestMenu_AG1[C_LANGUAGE][1][16];

extern const uint8_t _TestMenu_AG2[C_LANGUAGE][3][73];

extern const char EpDots[];

extern const uint16_t company[];

extern const uint16_t company2[];

extern const uint16_t product[];

extern const char Probe[];

extern const uint8_t _ParaMenu_H1[C_LANGUAGE][1][40];

extern const uint8_t _ChannelMenu_A1[C_LANGUAGE][9][22];

extern const uint8_t _ChannelMenu_A2[C_LANGUAGE][6][22];

extern const uint8_t _ChannelMenu_A1S[C_LANGUAGE][9][30];

extern const uint8_t _ChannelMenu_A2S[C_LANGUAGE][8][50];

extern const uint8_t _ChannelMenu_A3S[C_LANGUAGE][7][50];

extern const uint8_t _ChannelMenu_A4S[C_LANGUAGE][9][50];

extern const uint8_t _ChannelMenu_B1[];

extern const uint8_t _ChannelMenu_B2[];

extern const uint8_t _ParaMenu_B1[];

extern const uint8_t _ParaMenu_AB1[C_LANGUAGE][3][10];

extern const uint8_t _ParaMenu_AB2[C_LANGUAGE][5][14];

extern const uint8_t _ParaMenu_AB3[];

extern const uint8_t _ParaMenu_A2[C_LANGUAGE][9][22];

extern const uint8_t _ParaMenu_A22[C_LANGUAGE][9][22];

extern const uint8_t _ParaMenu_B2[];

extern const uint8_t _ParaMenu_B2A1[];

extern const uint8_t _ParaMenu_AC1[C_LANGUAGE][2][8];

extern const uint8_t _ParaMenu_AC7[C_LANGUAGE][2][10];

extern const uint8_t _ParaMenu_AC2[C_LANGUAGE][7][10];

extern const uint8_t _infinity[C_LANGUAGE][1][7];

extern const uint8_t _ParaMenu_AC3A[C_LANGUAGE][2][8];

extern const uint8_t _ParaMenu_AC3[C_LANGUAGE][2][8];

extern const uint8_t _ParaMenu_AC4[C_LANGUAGE][5][10];

extern const uint8_t _ParaMenu_AC5[C_LANGUAGE][2][10];

extern const uint8_t _ParaMenu_AC6[C_LANGUAGE][2][8];

extern const uint8_t _MainMenu_C1A11[C_LANGUAGE][2][16];

extern const uint8_t _ParaMenu_A3[C_LANGUAGE][9][22];

extern const uint8_t _ParaMenu_A5[C_LANGUAGE][2][28];

extern const uint8_t _ParaMenu_A6[C_LANGUAGE][8][22];

extern const uint8_t _ParaMenu_A6A1[C_LANGUAGE][8][22];

extern const uint8_t _ParaMenu_B6[];

extern const uint8_t _ParaMenu_B6A1[];

extern const uint8_t _ParaMenu_B3[];

extern const uint8_t _ParaMenu_AD2[C_LANGUAGE][4][18];

extern const uint8_t _ParaMenu_AD3_1[];

extern const uint8_t _ParaMenu_AD3[C_LANGUAGE][4][10];

extern const uint8_t _ParaMenu_AD4[];

extern const uint8_t _ParaMenu_AD5[C_LANGUAGE][4][18];

extern const uint8_t _ParaMenu_AD7[C_LANGUAGE][4][18];

extern const uint8_t _ParaMenu_AD8[C_LANGUAGE][2][6];

extern const uint8_t _ParaMenu_AD9[C_LANGUAGE][2][5];

extern const uint8_t _ParaMenu_C1[C_LANGUAGE][3][22];

extern const uint8_t _ParaMenu_D1[];

extern const uint8_t _ParaMenu_CA1[];

extern const uint8_t _ParaMenu_CC1[];

extern const uint8_t _ParaMenu_CD1[];

extern const uint8_t _MainMenu_C1A1[C_LANGUAGE][4][16];

extern const uint8_t _MainMenu_C1A3[C_LANGUAGE][2][16];

extern const uint8_t _MainMenu_C1A4[C_LANGUAGE][4][16];

extern const uint8_t _Weld_A1[C_LANGUAGE][1][40];

extern const uint8_t _Weld_A2[C_LANGUAGE][1][40];

extern const uint8_t _Weld_B1[C_LANGUAGE][9][22];

extern const uint8_t _Weld_B12[C_LANGUAGE][4][22];

extern const uint8_t _Weld_B2[C_LANGUAGE][9][22];

extern const uint8_t _Weld_B22[C_LANGUAGE][7][22];

extern const uint8_t _Weld_C1[C_LANGUAGE][4][10];

extern const uint8_t _Weld_D1[C_LANGUAGE][3][13];

extern const uint16_t _OptionMenu_B1[];

extern const uint8_t _OptionMenu_C1[];

extern const uint8_t _ParaProtect1[C_LANGUAGE][6][24];

extern const uint8_t _ParaProtect2[C_LANGUAGE][2][8];

extern const uint8_t _ParaMenu_AD6[C_LANGUAGE][5][4];

extern const uint8_t _DataMenu_A1[C_LANGUAGE][2][20];

extern const char _DataMenu_A2[C_LANGUAGE][10][10];

extern const uint16_t _DataMenu_C1[];

extern const uint16_t _DataMenu_C2[];

extern const unsigned short _DataMenu_D1[];

extern const char _DataMenu_E1[C_LANGUAGE][2][22];

extern const char _Message_A1[C_LANGUAGE][31][22];

extern const uint8_t _Message_B1[C_LANGUAGE][15][24];

extern const uint8_t _SaveMenu[C_LANGUAGE][4][12];

extern const uint8_t _UDiskMenu[C_LANGUAGE][6][30];

extern const uint8_t _PushMenu[C_LANGUAGE][4][12];

extern const uint16_t _Message_B2[];

extern const uint8_t _FuncHotKey_A[C_LANGUAGE][5][22];

extern const uint8_t _FuncHotKey_B[C_LANGUAGE][11][22];

extern const uint8_t _FuncMenu_A1[C_LANGUAGE][11][22];

extern const char _FuncMenu_A2[C_LANGUAGE][20][21];

extern const uint16_t _FuncMenu_B1[];

extern const uint16_t _FuncMenu_B2[];

extern const uint16_t _FuncMenu_B3[];

extern const uint16_t _FuncMenu_C1[];

extern const uint8_t _MenuPrompt_B1[C_LANGUAGE][1][20];

extern const uint8_t _MenuPrompt_P1[C_LANGUAGE][1][20];

extern const uint8_t _MenuPrompt_P2[C_LANGUAGE][1][30];

extern const uint8_t _MenuPrompt_P3[C_LANGUAGE][1][30];

extern const uint8_t _MenuPrompt_P4[C_LANGUAGE][1][30];

extern const char _STDMenu_A1[C_LANGUAGE][11][21];

extern const char _STDMenu_A2[C_LANGUAGE][11][21];

extern const char _STDMenu_A3[C_LANGUAGE][11][21];

extern const char _STDMenu_A4[C_LANGUAGE][11][21];

extern const char _STDMenu_A5[C_LANGUAGE][11][21];

extern const uint16_t _KEYLOCK_A1[];

extern const uint16_t _KEYLOCK_A2[];

extern const uint16_t _KEYLOCK_A3[];

extern const uint8_t _KEYLOCK_A1_E[];

extern const uint8_t _KEYLOCK_A2_E[];

extern const uint8_t _KEYLOCK_A3_E[];

extern const uint8_t _STDMenu_AB1[];

extern const uint8_t ZSTDMenu_AB1[];

extern const uint8_t _STDMenu_B1[][C_BLOCK_LEN * C_BLOCK_MAX + 1];

extern const short c_DACLine[][C_BLOCK_MAX][C_STD_POS_THICK + 7 * C_BLOCK_MAX];

extern const uint16_t _COMM_B1[];

extern const uint8_t        _Bright_A1[C_LANGUAGE][2][18];
extern const uint8_t        _Bscan[C_LANGUAGE][4][32];
extern const unsigned short _BSCAN_B2[];

extern const short sin_tab[];
extern const short cos_tab[];

extern const uint16_t PARA_BB1[];

#ifdef __cplusplus
}
#endif
