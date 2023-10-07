// test.h
#pragma once

#include <stdint.h>

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
int         TestMenuZero(int iBlock /*试块类型*/, bool bBlockModify, int iProbemode /*探头类型*/, bool bProbeModify, int iSoundmode /*声波类型*/, bool bSoundModify, int iRange1 /*声程1*/, bool bRangeModify1, int iRange2 /*声程2*/, bool bRangeModify2);
// 测零点菜单，各bool量为真可修改相应量
int TestZero(int iProbemode /*探头类型*/, int iSpeed /*默认声速*/, int iRange1 /*声程1*/, int iRange2 /*声程2*/, int iForward /*是否测前沿*/);

int TestMenuAngle(int iAngle /*标称折射*/, bool bAngleModify, int iDiameter /*孔直径*/, bool bDiameterModify, int iDepth /*孔深*/, bool bDepthModify, int iContact /*接触面*/, bool bContact, int iOutRandius /*外半径*/);
int TestAngle(int iAngle /*标称折射*/, int iDiameter /*孔直径*/, int iDepth /*孔深*/, int iContact /*接触面*/, int iOutRandius, int iInnerRandius);

int TestSensitivity(uint32_t mode, uint32_t iRange, int bParaAdjust); // 调灵敏度

int TestMenuOffset(void);
int TestOffset(uint32_t testrang1, uint32_t testrange2, uint32_t mode);
// 第1声程，第2声程 mode = 0距离测，=1深度 =2 曲面
// int TestMenuAngle(void);
// int TestAngle(uint32_t diameter,uint32_t depth);//直径、深度

int TestMenuDac(void);
int TestDac(uint32_t depthmax, uint32_t diameter, uint32_t length); // 最大深度、反射体直径、长度
int CalcuDac(void);                                                 // 计算DAC,
int DrawDac(int mode);                                              // 画和或计算DAC曲线,mode=0，计算=1不
int GetLine_TwoDot(uint16_t *line, int dist1, int dist2);
int GetMiddleValue(int dist1, int db1, int dist2, int db2, int dist); // 得到中间值

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);
