/**
 * @file sysdef.h
 * @author mengyou (1523038073@qq.com)
 * @brief ������ϵͳ�����Ҫ����
 * @version 0.1
 * @date 2023-10-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

// 1=330��2��U1,3=350+��4=320c,5=v1,6=8channel,7=jiwu,8=350c,9=360B,10=u2,11=350b,12=v2
// 13=TFT,20=300C,14=F1,15=390(class)            //360b+ & F1 ��Ҫ��C_FILL 1	//ʵ�Ĳ�
#define C_DEVTYPE 15
#define C_UDISK   1 // 0,����U�̼�¼��1��U�̼�¼
/*
��Ҫ������������
C_DEVTYPE�������ͺ�
C_UDISK��U�̼�¼
C_FILL��ʵ�Ĳ�	��globe.h���޸�
C_DEVLIB	23	//23tft .24 EL
*/

#define C_PROCESS 1 // Ӳ�����ӹ���
#define C_ACQU    0 // ��ʱ������

#if C_DEVTYPE == 20
    #define C_LIGHT 0 // ��Ļ���ȿɵ�
#else
    #define C_LIGHT 1 // ��Ļ���ȿɵ�
#endif

#if C_DEVTYPE == 1
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 2
    #define C_COLOR  1
    #define C_ROTARY 1
#elif C_DEVTYPE == 3
    #define C_COLOR  0
    #define C_ROTARY 0
#elif C_DEVTYPE == 4
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 20
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 5 || C_DEVTYPE == 12
    #define C_COLOR  1
    #define C_ROTARY 1
#elif C_DEVTYPE == 6
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 7
    #define C_COLOR  0
    #define C_ROTARY 0
#elif C_DEVTYPE == 8
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 9
    #define C_COLOR  0
    #define C_ROTARY 0
#elif C_DEVTYPE == 10
    #define C_COLOR  1
    #define C_ROTARY 1
#elif C_DEVTYPE == 11
    #define C_COLOR  0
    #define C_ROTARY 0
#elif C_DEVTYPE == 13
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 14
    #define C_COLOR  1
    #define C_ROTARY 0
#elif C_DEVTYPE == 15
    #define C_COLOR  1
    #define C_ROTARY 0
#endif
// ��ֱ���Բ���
#if C_DEVLIB == 24
    #ifdef C_COLOR
        #undef C_COLOR
    #endif
    #define C_COLOR 0
#endif
#define C_AMPCOMP        0

#define MB_OK            1
#define MB_CANCEL        2
#define MB_OKCANCEL      (MB_OK + MB_CANCEL)
#define MB_YES           4
#define MB_NO            8
#define MB_YESNO         (MB_YES + MB_NO)

#define C_MAX_BIT        9
//=pow(2,C_MAX_BIT)
#define C_MAX_DOT_NUMBER 512

#define CH(a)            (a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define ch(a)            (a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define cH(a)            (a) //( ( (a)/100 )*256 + ( (a)%100 ) )
#define Ch(a)            (a) //( ( (a)/100 )*256 + ( (a)%100 ) )

#define msin(a)          sin((a)*C_PI / 180.0)
#define mcos(a)          cos((a)*C_PI / 180.0)
#define mtan(a)          tan((a)*C_PI / 180.0)
#define macos(a)         (acos(a) * 1800 / C_PI)
#define masin(a)         (asin(a) * 1800 / C_PI)
#define matan(a)         (atan(a) * 1800 / C_PI)

#define C_SIZE_INT       (4)
#define C_SIZE_SHORT     (2)
#define C_SIZE_CHAR      (1)
#define C_TRUE           1
#define C_FALSE          0

// ���β���ˢ��ʱ��
#define C_RENOVATE_TIME  30

#define C_DEBUG          1

#if C_DEVTYPE == 14
    #define C_CHMAX 60 /*ͨ����*/
#else
    #define C_CHMAX 50 /*ͨ����*/
#endif

#if C_DEVTYPE == 3 || C_DEVTYPE == 5
    #define C_MAX_SYSGAIN    1100
    #define C_MAX_BASEGAIN   1100  /*��ʾ���������ֵ*/
    #define C_MAX_ADJUSTGAIN 800   /*�����Զ����ڵ��������*/
    #define C_MAX_RANGE      60000 /*�������Χ5000mm*/
    #define C_MIN_RANGE      100
    #define C_MAX_DELAY      (200 * 200 * SAMPLE_FREQ) /*��ʱ���Χ500ms*/
#elif C_DEVTYPE == 8
    #define C_MAX_SYSGAIN    1200
    #define C_MAX_BASEGAIN   1200  /*��ʾ���������ֵ*/
    #define C_MAX_ADJUSTGAIN 800   /*�����Զ����ڵ��������*/
    #define C_MAX_RANGE      60000 /*�������Χ5000mm*/
    #define C_MIN_RANGE      100
    #define C_MAX_DELAY      (200 * 200 * SAMPLE_FREQ) /*��ʱ���Χ500ms*/

#elif C_DEVTYPE == 1
    #define C_MAX_SYSGAIN    1100
    #define C_MAX_BASEGAIN   1100   /*��ʾ���������ֵ*/
    #define C_MAX_ADJUSTGAIN 800    /*�����Զ����ڵ��������*/
    #define C_MAX_RANGE      100000 /*�������Χ5000mm*/
    #define C_MIN_RANGE      100
    #define C_MAX_DELAY      (200 * 200 * SAMPLE_FREQ) /*��ʱ���Χ500ms*/

#elif C_DEVTYPE == 2 || C_DEVTYPE == 10 || C_DEVTYPE == 11 || C_DEVTYPE == 9 || C_DEVTYPE == 13 || C_DEVTYPE == 14 || C_DEVTYPE == 15
    #define C_MAX_SYSGAIN    1200
    #define C_MAX_BASEGAIN   1200   /*��ʾ���������ֵ*/
    #define C_MAX_ADJUSTGAIN 800    /*�����Զ����ڵ��������*/
    #define C_MAX_RANGE      100000 /*�������Χ5000mm*/
    #define C_MIN_RANGE      100
    #define C_MAX_DELAY      (200 * 200 * SAMPLE_FREQ) /*��ʱ���Χ500ms*/
#else
    #define C_MAX_SYSGAIN    1100
    #define C_MAX_BASEGAIN   1100  /*��ʾ���������ֵ*/
    #define C_MAX_ADJUSTGAIN 800   /*�����Զ����ڵ��������*/
    #define C_MAX_RANGE      50000 /*�������Χ5000mm*/
    #define C_MIN_RANGE      100
    #define C_MAX_DELAY      (200 * 200 * SAMPLE_FREQ) /*��ʱ���Χ500ms*/
#endif

#define C_MAX_ADJUSTGAIN   800 /*�����Զ����ڵ��������*/
#define C_MAX_OFFSET       16 * 300
#define C_MAX_ZERO         16 * 300
#define C_MAX_REJECT       160   /*��ʱ���Χ80��*/
#define C_MAX_SURFGAIN     500   /*���油���������ֵ*/
#define C_MAX_RF_RANGE     14000 /*��Ƶ���������Χ2600mm*/
#define C_MAX_FREQUENCE    1500  /*Ƶ�����ֵ15.0MHz*/

#define C_MIN_TIMERANGE    320

#define C_MAX_ANGLE        900 // 810
#define C_MAX_SPEED        9999
#define C_MIN_SPEED        200
#define CD_SPEED_1         5920
#define CD_SPEED_2         3230
#define CD_SPEED_3         5700
#define CD_RANGE           1250
#define CD_BASEGAIN        450
#define CD_COMPGAIN        0
#define CD_SURFGAIN        0

// ��ֵ
#define C_KEYCODMAX        36

#define C_KEYCOD_AUTOGAIN  11
#define C_KEYCOD_BASEGAIN  0
#define C_KEYCOD_GATE      1
#define C_KEYCOD_OFFSET    7
#define C_KEYCOD_RANGE     4
#define C_KEYCOD_DELAY     5
#define C_KEYCOD_REJECT    5
#define C_KEYCOD_ANGLE     8

#define C_KEYCOD_DAC       9
#define C_KEYCOD_SETUP     6
#define C_KEYCOD_RETURN    15
#define C_KEYCOD_INPUT     15
#define C_KEYCOD_CONFIRM   10
#define C_KEYCOD_PAGEDOWN  10

#define C_KEYCOD_MINUS1    13
#define C_KEYCOD_MINUS     13
#define C_KEYCOD_LEFT      13
#define C_KEYCOD_RIGHT     14
#define C_KEYCOD_PLUS      14

#define C_KEYCOD_SAVE      12
#define C_KEYCOD_DEL       12
#define C_KEYCOD_POINT     14
#define C_KEYCOD_ASCII     14
#define C_KEYCOD_CHANNEL   3
#define C_KEYCOD_YES       9
#define C_KEYCOD_NO        6

#define C_KEYCOD_SPEED     2
#define C_KEYCOD_COLOR     20

#define C_FUNC_1           16 // ��ݹ���1
#define C_FUNC_2           17 // �ز�����
#define C_FUNC_3           18 // �ز�����
#define C_FUNC_4           19

#define C_FUNC_INIT        0 // ��ʼ��
#define C_FUNC_ENVELOPE    1 // �ز�����
#define C_FUNC_ECHOMAX     2 // ��ֵ����
#define C_FUNC_DACGATE     3 // DAC��
#define C_FUNC_DACADJUST   4 // ��Ȳ���
#define C_FUNC_FILL        5 // ʵ�Ĳ�
#define C_FUNC_GATEALARM   6 // ���ڱ���
#define C_FUNC_DATA        7 // ���ݴ���
#define C_FUNC_FREEZ       8 // ���ζ���
#define C_FUNC_GATEEXPEND  9 // ����չ��

#define C_SENSITIVITY_GAIN 30 /*����������У��������ֵ����λ0.1dB*/

#define C_DAYS_PROBE       7  /*�Կ�У����Ч����*/
#define C_DAYS_SYSTEM      30 /*ϵͳ����У����Ч����*/
#define C_DAYS_SENSITIVITY 1  /*������У����Ч����*/

#define C_SAMEKEYTIME      400 /*�Գ�ʱ���㳤������λ1ms*/

#ifdef __cplusplus
extern "C" {
#endif

void MSetAmplifierGain(uint16_t gain);
/**
 * @brief ����Ŀ����
 *
 * @return int
 */
int MainProject(void);

/**
 * @brief ��������
 *
 */

void SystemLog(void);
void Usb2PC(void);
#ifdef __cplusplus
}
#endif
