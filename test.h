// test.h
#pragma once

#include <stdint.h>

typedef struct
{
    int speed;     // Ԥ������
    int range[2];  // ��һ����/�ڶ�����
    int rangemode; // ��������0���룬1��ȣ�2�������β�������

    int             offset; // ���
    unsigned short *pPmt1;  // ��ʾ1
    unsigned short *pPmt2;  // ��ʾ2

    int len1; // ��ʾ1��ÿ�г���
    int row1; // ��ʾ1������
    int len2; // ��ʾ2��ÿ�г���
    int row2; // ��ʾ2������

} TEST_OFFSET;

TEST_OFFSET __attribute((weak)) stTestOffset;
int         TestMenuZero(int iBlock /*�Կ�����*/, bool bBlockModify, int iProbemode /*̽ͷ����*/, bool bProbeModify, int iSoundmode /*��������*/, bool bSoundModify, int iRange1 /*����1*/, bool bRangeModify1, int iRange2 /*����2*/, bool bRangeModify2);
// �����˵�����bool��Ϊ����޸���Ӧ��
int TestZero(int iProbemode /*̽ͷ����*/, int iSpeed /*Ĭ������*/, int iRange1 /*����1*/, int iRange2 /*����2*/, int iForward /*�Ƿ��ǰ��*/);

int TestMenuAngle(int iAngle /*�������*/, bool bAngleModify, int iDiameter /*��ֱ��*/, bool bDiameterModify, int iDepth /*����*/, bool bDepthModify, int iContact /*�Ӵ���*/, bool bContact, int iOutRandius /*��뾶*/);
int TestAngle(int iAngle /*�������*/, int iDiameter /*��ֱ��*/, int iDepth /*����*/, int iContact /*�Ӵ���*/, int iOutRandius, int iInnerRandius);

int TestSensitivity(uint32_t mode, uint32_t iRange, int bParaAdjust); // ��������

int TestMenuOffset(void);
int TestOffset(uint32_t testrang1, uint32_t testrange2, uint32_t mode);
// ��1���̣���2���� mode = 0����⣬=1��� =2 ����
// int TestMenuAngle(void);
// int TestAngle(uint32_t diameter,uint32_t depth);//ֱ�������

int TestMenuDac(void);
int TestDac(uint32_t depthmax, uint32_t diameter, uint32_t length); // �����ȡ�������ֱ��������
int CalcuDac(void);                                                 // ����DAC,
int DrawDac(int mode);                                              // ���ͻ����DAC����,mode=0������=1��
int GetLine_TwoDot(uint16_t *line, int dist1, int dist2);
int GetMiddleValue(int dist1, int db1, int dist2, int db2, int dist); // �õ��м�ֵ

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);
