// test.h
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t dac_line[];

typedef struct
{
    int speed;     // 预置声速
    int range[2];  // 第一声程/第二声程
    int rangemode; // 声程类型0距离，1深度，2不用两次波的曲面

    int             offset; // 零点
    unsigned short *pPmt1;  // 提示1
    unsigned short *pPmt2;  // 提示2

    int len1; // 提示1的每行长度
    int row1; // 提示1的行数
    int len2; // 提示2的每行长度
    int row2; // 提示2的行数

} TEST_OFFSET;

TEST_OFFSET __attribute((weak)) stTestOffset;

typedef struct _TagPOINT {
    long x;
    long y;
} POINT, *LPPOINT;

int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2);

void HotKeySet(void);

void AmplitudedBTable(void);

int MenuChoice(void);

int TestChoice(int Test);

int TestMenu(void);

int TestMenuOffset(void);

int TestOffset(uint32_t testrange1, uint32_t testrange2, uint32_t mode);

int TestMenuAngle(void);

int TestAngle(uint32_t diameter, uint32_t depth);

int TestMenuDac(void);

int TestDac(uint32_t depthmax, uint32_t diameter, uint32_t length, short test_num);

int TestMenuStandard(void);

int TestMenuThick(void);

int TestMenuSystem(void);

int CalcuDac(void);

// 画和或计算DAC曲线,mode=0计算,=1不
int DrawDac2(int mode);

// 画和或计算DAC曲线,mode=0计算,=1不
int DrawDac(int mode);

int GetMiddleValue(int dist1, int db1, int dist2, int db2, int dist);

int GetLine_TwoDot(uint16_t line[], int dist1, int dist2);

void GetCenterPoint(int dist1, int db1, int dist2, int db2);

void GetDacCompensateTable(void);

int TestMenuFft(void);

int TestMenuSensitivity(void);

/**
 * @brief 灵敏度校验
 *
 * @param depth 反射体深度
 * @param stdamp 基准波高度
 * @param mode 0可调增益，1不可调
 * @return int
 */
int TestSensitivity(uint32_t depth, uint32_t stdamp, int mode);

// 人工月性能校验
int ManualMonthTest(void);
// 月性能校验
int MenuMonthTest(void);

int WeldSetup(void);

int ShowWeld(short lxpos, short lypos, short width, short height, short angle, int dist);

int ShowWeldA(short lxpos, short lypos, short width, short height, short angle, int dist);

int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2);

#ifdef __cplusplus
}
#endif
