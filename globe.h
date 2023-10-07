/**
 * @file globe.h
 * @author mengyou (1523038073@qq.com)
 * @brief ȫ���Եı���
 * @version 0.1
 * @date 2023-10-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "sysdef.h"

#define C_MAKELIB 0

#define C_DEVLIB  23 // 23tft .24 EL

#define C_FILL    1 // ʵ�Ĳ�	//360b+ & F1 &350

#ifndef _CUDEFM1_H_

    #include "M10Era.h"
    #define PM_COPY_MOVE 1
    #define ARROW1       238
    #define ARROW2       239
    #define ARROW3       240
    #define ARROW4       241
    #define ENTER        242
    #define PHI          243
    #define ANGLE        160

    /*
    (phi)= 'z' + 14     ==216
    (angle) = 'z'+ 8    ==167
    (\>) = 'z'+9        ==211
    (->) = 'z'+10       ==212
    (|>) = 'z'+11       ==213
    (^) = 'z' + 12      ==214
    (enter) = 'z'+ 15   ==215
    */
    /* CH( 100) = " " */
    #if C_DEVLIB == 4
        #ifndef DP_NOT
            #define DP_NOT 0
        #endif
    #endif

    #ifndef DP_SET
        #define DP_SET 0
    #endif
    #ifndef DP_NOT
        #define DP_NOT 0
    #endif

    #ifndef USB_DEV_DATA_TRANSFER_OK
        #define USB_DEV_DATA_TRANSFER_OK USB_DATA_TRANSFER_OK
    #endif
    #ifndef USB_DEV_DATA_TRANSFER_OK
        #define USB_DEV_DATA_TRANSFER_OK USB_DATA_TRANSFER_OK
    #endif
    #ifndef USB_DEV_DATA_TRANSFER_BUSY
        #define USB_DEV_DATA_TRANSFER_BUSY USB_DATA_TRANSFER_BUSY
    #endif
    #ifndef USB_DEV_DATA_TRANSFER_ERROR
        #define USB_DEV_DATA_TRANSFER_ERROR USB_DATA_TRANSFER_ERROR
    #endif
    #ifndef USB_DEV_DATA_TRANSFER_DECLINED
        #define USB_DEV_DATA_TRANSFER_DECLINED USB_DATA_TRANSFER_DECLINED
    #endif

    #define _CUDEFM1_H_
#endif

// ��Ƭ������ָ��
#define LED_ALARM_ON         100
#define LED_ALARM_OFF        101
#define BUZZER_ALARM_ON      102
#define BUZZER_ALARM_OFF     103
#define KEYCODE_T_REQ        104
#define LOW_BATTERY_OFF      105
#define DETECTOR_ON_CODE     106
#define DETECTOR_ON_ACK      107
#define DETECTOR_OFF_CODE    108
#define DETECTOR_OFF_ACK     109

#define RTC_R_YEAR           110
#define RTC_R_MONTH          111
#define RTC_R_DATE           112
#define RTC_R_HOUR           113
#define RTC_R_MINUTE         114
#define RTC_R_SECOND         115
#define RTC_R_DAY            116

#define RTC_R_YEAR_ACK       117
#define RTC_R_MONTH_ACK      118
#define RTC_R_DATE_ACK       119
#define RTC_R_HOUR_ACK       120
#define RTC_R_MINUTE_ACK     121
#define RTC_R_SECOND_ACK     122
#define RTC_R_DAY_ACK        123

#define RTC_W_YEAR           124
#define RTC_W_MONTH          125
#define RTC_W_DATE           126
#define RTC_W_HOUR           127
#define RTC_W_MINUTE         128

#define RTC_W_YEAR_ACK       129
#define RTC_W_MONTH_ACK      130
#define RTC_W_DATE_ACK       131
#define RTC_W_HOUR_ACK       132
#define RTC_W_MINUTE_ACK     133

#define RTC_W_READY          134
#define BACKLIGHT_ON         135
#define BACKLIGHT_OFF        136

#define RTC_W_ACK            155
#define RTC_W_REQ            196

// 0�ڰ�1��ɫ
/*
const uint32_t c_crPara[] = {
    0x007,//��
    0x0c7,//�ۺ�
    0x03f,//��
    0x038,//��
    0x0f8,//��
    0x0c0,//��
    0x084,//��
    0x0ff,//��
};*/
/*
1. һ�λز���
2�����λز���
3����ֵ���磺
4����ʾ�˵���
5������A��
6������B��
7���������ߣ�
8. �������
*/
#define C_CR_BACK            0
#define C_CR_WAVEBACK        1
#define C_CR_WAVE            2
#define C_CR_SEWAVE          3
#define C_CR_MENU            4
#define C_CR_CURVE           5
#define C_CR_PARA            6
#define C_CR_GATEA           7
#define C_CR_GATEB           8
#define C_CR_ENVELOPE        9

#define C_CR_MAX             10

#define C_CR_UNDO            11
#define C_CR_CURRENT         12
#define C_CR_DEFAULT         13

#define C_CR_RED             0
#define C_CR_GREEN           3
#define C_CR_BLUE            5
#define C_CR_YELLOW          2
#define C_CR_PINK            1
#define C_CR_CYAN            4
#define C_CR_WHITE           7
#define C_CR_BLACK           8
#define C_CR_PURPLE          6
#define C_CR_NUMBER          9

#define C_EXPF               2.302585

#define C_SAMPLE_FREQ        (200 * SAMPLE_FREQ)

#define C_DEBUG              1

// #if C_MAKELIB == 2
// #define C_CHMAX				5					/*ͨ����*/
// #else
// #define C_CHMAX				10				/*ͨ����*/
// #endif

#define C_PI                 3.1415926535897932
#define UINT                 unsigned int
#define PI                   3.1415926535897932

#define C_LANGUAGE           2
// ����16*16�ֿ�
#define HZCODE               160
// �Խ�16*16�ֿ�
#define HZCDEF               144
// �Խ�16*8���ֿ�
#define HZCEDEF              140
// ����8*8�ֿ�
#define HZECODE              32
// �Խ�8*8�ֿ�,< 32
#define HZEDEF               16

#define TXAL                 6
#define TXAR                 7
#define TXALR                8
#define TXAC                 9
#define TXCR                 10
#define TXEOF                0
#define TXSP                 32
/*
#define CH(a)		( ( (a)/100 )*256 + ( (a)%100 ) )
#define ch(a)		( ( (a)/100 )*256 + ( (a)%100 ) )
*/
// #define C16(a)		( (a)/100 ) + 144 ),( (a)%100 )  + 144)
// #define c16(a)		( (a)/100 ) + 144 ),( (a)%100 )  + 144)
// ����16*16�ֿ�
#define HF(a)                ((a) / 100 + HZCODE), ((a) % 100 + HZCODE)
// �Խ�16*16�ֿ�
#define FF(a)                ((a) / 100 + HZCDEF), ((a) % 100 + HZCDEF)
// �Խ�16*8�ֿ�
#define FO(a)                ((a) / 100 + HZCEDEF), ((a) % 100 + HZCEDEF)
// ����8*8�ֿ�
#define HO(a)                ((a) / 100)
// �Խ�8*8�ֿ�
#define OO(a)                ((a) / 100 + HZEDEF)

#define C_INPUT_DIGITAL      0
#define C_INPUT_CAPITAL      (C_INPUT_DIGITAL + 1)
#define C_INPUT_LOWERCASE    (C_INPUT_CAPITAL + 1)
#define C_INPUT_PINYIN       (C_INPUT_LOWERCASE + 1)
#define C_INPUT_QUWEI        (C_INPUT_PINYIN + 1)
#define C_INPUT_ALL          (C_INPUT_QUWEI + 1)

/*���뷨����*/
#define C_MAX_INPUTMODE      C_INPUT_ALL

#define C_ZPROBE             0              /*ֱ̽ͷΪ0*/
#define C_APROBE             (C_ZPROBE + 1) /*б̽ͷΪ1*/
#define C_DPROBEZ            (C_APROBE + 1) /*˫��̽ͷΪ2*/
#define C_MANUAL             (C_DPROBEZ + 1)
#define C_DPROBEA            (C_MANUAL + 1)

// const uint8_t MadeDate[21] = "2003-04-10 A10      ";	/*��20*/
#define C_P_NORMAL           0
#define C_P_ANGLE            1
#define C_P_DOUBLE           2
#define C_P_TRANSMISSION     3

#define C_SEGMENT_VIDEO      0x2000
#define C_OFFSET_VIDEO       1
#define C_SCREEN_WIDTH       500  // 308
#define C_VERTDOT_VIDEO      480  // 240
#define C_HORIDOT_VIDEO      1024 // 512
#define C_HORIDOT_SCREEN     640  // 320
#define C_VERTDOT_SCREEN     480  // 240

#define C_HORIDOT            500
#define C_MenuHigh           30
#define C_PeakHPOSI          20
#define C_PeakVPOSI          10
#define C_COORHPOSI          1
/*����������´�ֱλ�õ���*/
#define C_COORVPOSI          55 // 28
/*����߶�*/
#define C_COORHEIGHT         400 // 200
/*����ˮƽÿ�����*/
#define C_COORHORIUNIT       (48) //(28)
#define C_HORIUNIT           48   // 28
/*����ˮƽ�ܵ���*/
#define C_COORWIDTH          (10 * C_COORHORIUNIT)
/*����ˮƽ�ܵ���*/
#define C_WAVEWIDTH          (C_COORWIDTH - 2)
#define C_COORVPOSI2         460 // 229
#define C_GATEPOSIMAX        (C_COORWIDTH + 10)

#define C_STD_AWS_NUM        10
#define C_STD_11345_2013_NUM 11

#define C_ECHAR_VDOT         24 /*һ��Ӣ���ַ���ֱ�������*/
#define C_ECHAR_HDOT         16 /*һ��Ӣ���ַ�ˮƽ�������*/
#define C_CCHAR_VDOT         24 /*һ�������ַ���ֱ�������*/
#define C_CCHAR_HDOT         24 /*һ�������ַ�ˮƽ�������*/

// ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
#define C_TIME_MILLION       0
#define C_TIME_SECOND        1
#define C_TIME_MINUTE        2

#define C_PRB_NUM            5
// ÿ��ͨ����̽ͷ��0����̽ͷ,1��̽2�˲����3б4��ֱ5˫ֱ
#define C_PRB_1ST            4
#define C_PRB2_COMB          0
#define C_PRB2_ANGLE         1
#define C_PRB2_NORMAL        2
#define C_PRB2_DOUBLE        3
// ��̽/���/б/ֱ/˫ֱ

#define PASS                 0x0
#define FOR_CHECK            0x40
#define CHECK_PASS           0x80
#define PERIMETER_DOT        0xc0 /*1100 �����״	192*/
#define PERIMETER_PLANE      0xd0 /*1101				208*/
#define RADII_DOT            0xe0 /*1110	�����״	224*/
#define RADII_PLANE          0xf0 /*1111				240*/

#define NORMAL               0
#define DOUBLE_OUT           1
#define DOUBLE_IN            2
#define TRANSMISSION         3
#define AXES                 0
#define RADIAL               1

/* ��������*/
#define C_LOCK_OFFSET        0
#define C_LOCK_DACAVG        1
#define C_LOCK_PROBE         2
#define C_LOCK_DAC           3
// #define	C_LOCK_SYSTEM		3
#define C_LOCK_ALL           4

/*
1.��λ����
2.��������
3.̽ͷ����
4.ȫ������
*/

#define C_KEYDELAY           25

// #define C_SIZE_INT		( sizeof(int) )
#define C_SIZE_INT           (4)
#define C_SIZE_SHORT         (2)
#define C_SIZE_CHAR          (1)
#define C_TRUE               1
#define C_FALSE              0

#define C_SETMODE_SAVE       0                    /*���ú���ʱֻ��*/
#define C_SETMODE_SET        (C_SETMODE_SAVE + 1) /*���ú���ʱֻ����*/
#define C_SETMODE_SETSAVE    (C_SETMODE_SET + 1)  /*���ú���ʱ�ȴ�Ҳ����*/

#define C_GAINPLUSTIME       4000 /*�����ʱ��,��λΪ1ms*/
#define C_GAINMINUSTIME      1500 /*�����ʱ��,��λΪ1ms*/
#define C_PARADISPLAYTIME    50   /*���β�����ʾʱ��,��λΪ1ms*/

#if C_DEVLIB == 1 || C_DEVLIB == 2
    #define C_BRIGHTNESS_MAX 31 /*����*/
    #define C_BRIGHTNESS_MID 13 /*����*/
    #define C_BRIGHTNESS_COM 9  /*һ��*/
    #define C_BRIGHTNESS_MIN 5  /*�*/
#elif C_DEVLIB == 24
    #define C_BRIGHTNESS_MAX 0  /*����*/
    #define C_BRIGHTNESS_MID 19 /*����*/
    #define C_BRIGHTNESS_COM 23 /*һ��*/
    #define C_BRIGHTNESS_MIN 27 /*�*/
#elif C_DEVLIB == 23
    #define C_BRIGHTNESS_MAX 200 /*����*/
    #define C_BRIGHTNESS_MID 120 /*����*/
    #define C_BRIGHTNESS_COM 80  /*��׼*/
    #define C_BRIGHTNESS_MIN 40  /*һ��*/
#elif C_DEVLIB == 3
    #define C_BRIGHTNESS_MAX 31 /*����*/
    #define C_BRIGHTNESS_MID 10 /*����*/
    #define C_BRIGHTNESS_COM 3  /*һ��*/
    #define C_BRIGHTNESS_MIN 0  /*�*/
#endif

#define C_PULSE_MIN        18 // 33
#define C_PULSE_MED        33 // 36
#define C_PULSE_MAX        36 // 40

// ����
#define C_CRAFT_NORMAL     0 /*����̽��*/
#define C_CRAFT_TRANS      1 /*ֱ̽ͷ��͸*/
#define C_CRAFT_MAX        2 /*��������*/
/*
������
*/
#define C_BASEGAIN         0
#define C_SURFGAIN         (C_BASEGAIN + 1)
#define C_COMPGAIN         (C_SURFGAIN + 1)
#define C_RANGE            (C_COMPGAIN + 1) /*�ִ�ֱˮƽ��������*/
#define C_SPEED            (C_RANGE + 3)
#define C_OFFSET           (C_SPEED + 1)
#define C_DELAY            (C_OFFSET + 1) /*�ִ�ֱˮƽ��������*/
#define C_REJECT           (C_DELAY + 3)
#define C_ANGLE            (C_REJECT + 1)
#define C_AGATEPOSI        (C_ANGLE + 1)
#define C_AGATEWIDE        (C_AGATEPOSI + 1)
#define C_AGATEHIGH        (C_AGATEWIDE + 1)
#define C_BGATEPOSI        (C_AGATEHIGH + 2)
#define C_BGATEWIDE        (C_BGATEPOSI + 1)
#define C_BGATEHIGH        (C_BGATEWIDE + 1)
#define C_CHANNEL          (C_BGATEHIGH + 2)

#define C_TESTDAYSMAX_0    184
#define C_TESTDAYSMAX_1    62
#define C_TESTDAYSMAX_2    15
/*
������
*/
#define C_ECHO_ENVELOPE    0 /*����*/
#define C_ECHOMAX_MEMORY   1 /*��ֵ����*/
#define C_GATE_ALARM       2 /*���ڱ���*/
#define C_DAC_GATE         3 /*DAC��*/
#define C_DEPTH_COMPENSATE 4 /*��Ȳ���*/
#define C_CURVER_AMEND     5 /*��������*/
#if C_FILL == 1
    #define C_WAVE_FILL       7 /*ʵ�Ĳ���*/
    #define C_WAVE_EXPEND     6 /*����չ��*/
    #define C_BSCAN           8
    #define C_AUTOADJUST_GAIN 9 /*�Զ�����*/
    #define C_FLAW_HEIGHT     10
    // #define C_INITIALIZE		9
    #define C_FUNCTION_ALL    11
    #define C_FUNCTION_MAX    8
#else
    #define C_WAVE_FILL       11 /*ʵ�Ĳ���*/
    #define C_WAVE_EXPEND     6  /*����չ��*/
    #define C_BSCAN           7
    #define C_AUTOADJUST_GAIN 8 /*�Զ�����*/
    #define C_FLAW_HEIGHT     9
    // #define C_INITIALIZE		9
    #define C_FUNCTION_ALL    10
    #define C_FUNCTION_MAX    7
#endif

/*Function
d0:����
d1:��ֵ
d2:����
d3:DAC��
d4:��Ȳ���
d5:�Զ�����
d6:��������
d7:
�ڲ���ʱd0-d2��ʹ��
*/

#define C_RF_WAVE (RF_WAVE - FULL_WAVE)

/*------------
thick;		//�������
angle[4];	//�����1

zero[4];	//���
g_base[4];	//��������
g_comp[4];	//��������
diameter;	//�ֹ�ֱ��
Frequence;	//̽ͷƵ�ʣ�2.5Mhz

SurfGain;	//���油������
BaseGain;	//��������
CompGain;	//��������

Range;
Delay;
Offset;
*/

/*
�ṹ���������
*/
#if C_MAKELIB > 0
    #define C_USERMAX 5 /*Ա�������*/
#else
    #define C_USERMAX 1 /*Ա�������*/
#endif
#define C_USERNAMELEN 4        /*��������*/
#define C_USERNUMLEN  10       /*���ų���*/
#define C_SUPERPWD    13578642 /*Ĭ�ϳ�������*/
#define C_DEFPWD      1470     /*��ʼ����*/

typedef struct
{
    uint8_t funcType[9]; // ��������
    uint8_t funcMenu[4]; // 4�����ܼ�F1-F4
} FUNCTION;

typedef struct
{
    uint32_t pwd;                                 // ����
    uint16_t name[((C_USERNAMELEN + 2) / 2) * 2]; // ����
    uint8_t  num[((C_USERNUMLEN + 4) / 4) * 4];   // ����
} USER;

typedef struct
{
    int left;
    int top;
    int bottom;
    int right;
} RECT;

typedef struct
{
    unsigned int type   :3;  // ����0�ޣ�1���棬2˫�棬3T��
    unsigned int face   :3;  // ̽����0A�棬1B�棬2C��
    unsigned int thick1 :14; // ���1��������ȡ������<200.0mm;
    unsigned int thick2 :14; // ���2����Ч��ȡ�������<200.0mm;
    unsigned int wide   :10; // �����ȡ�A�溸�ţ�< 100mm;
    unsigned int radius :10; // �����뾶��B�溸�ţ�< 100mm;
    unsigned int high1  :7;  // �϶���ߡ�A����ߣ�<100mm;
    unsigned int high2  :7;  // �¶���ߡ�B����ߣ�<100mm;
    unsigned int angle1 :11; // �¿ڽǶȡ�A�¿ڽǣ�< 180;
    unsigned int angle2 :10; // �¿���ǡ�B�¿ڽǣ�< 90;
    unsigned int deep   :10; // �¿���ȣ�< 100mm;
    unsigned int blunt  :10; // �۱߸߶ȣ�< 100mm;
    unsigned int space  :10; // ������϶��< 100mm
    unsigned int probe  :14;

    unsigned int angle3 :10; // ���ڽǶȣ�< 180;
    unsigned int angle4 :10; // ������ǣ�< 180;
    unsigned int deep3  :10; // ������ȣ�< 100mm;
} WELD_PARA;

typedef struct
{
    unsigned short lpos; // ��ʾ�������
    unsigned short rpos; // ��ʾ�����Ҷ�
    unsigned short xpos; // ���������ˮƽ
    unsigned short ypos; // ��������Ĵ�ֱ
    unsigned short size; // ��ʾ�����Ĵ�С
    unsigned short wxpos;
    unsigned short wypos; //
    unsigned short dots;
} WELD_POSI;

typedef struct
{
    uint32_t nElapsedTime;      // ����ʱ�����Ժ���Ϊ��λ
    uint32_t nStartElapsedTime; // ��16λ����ʱ�䣬��16λ����ʱ�����Է�Ϊ��λ
    uint32_t days       :16;    // ��2000��1��1�յ�����
    uint32_t denoise    :3;     // �������
    uint32_t repeat     :10;    // �ظ�Ƶ��
    uint32_t pulsewidth :1;     // ������
    uint32_t fill       :1;     // ʵ�Ĳ�
    uint32_t sys;               // d31-d29��ӡ����d28-26���뷨,d25����״̬,d24����״̬��d23Ӳ�����Σ�d22�������
    // d21�����꣬d20���޹�꣬d19-d16�û���(���16)��d15-d12ͨ����(���16)
    // d11��������d10��������,d9d8��Ļ���ȣ�d7-d0���뷨�Ƿ��ѡ

    uint32_t option; // d31���μ�¼��ʽ(0����1����),d30����������,d29-d22��¼���(��λΪ0.1��,���25.0��)
    // d21�洢ʱ�Ƿ��¼ȱ�ݲ�Ƶ�ף�d20-d16��ǰѡ���̽�˹���
    // d15d14��ɫģʽ
    // D13������ʾģʽ��0������1dBֵ����ʱ�Զ���������
    // D12������
    // d11,d10��A��B�ڻز��Ƿ�����Ÿ�
    // d9=1A�ű�ɫ,d8=1B�ű�ɫ
    // d7=0��ֵ����1ǰ�ض���

    // U2: start
    uint32_t displaymode  :1;  // ��ʾ��ʽ,1:ȫ��,0��
    uint32_t mainmenuitem :5;  // ��ǰ���˵�����,ÿҳ6��,���24��
    uint32_t samp_len     :10; // ��ʾ�ز�����
    uint32_t coor_width   :9;  // ����������
    uint32_t coor_extern  :6;  // �������ҷ���չ����
    uint32_t hunit        :6;  // ������ˮƽ����ÿ�����
    uint32_t language     :2;  // ����
    uint32_t unit         :2;  // ��λ
    uint8_t  menu2page[24];    // �μ��˵�ҳ��,���4ҳ,ÿҳ4��
    // U2: end
    uint32_t prepare[4]; // Ԥ��ֵ

} SYSTEM_STATUS; // ����ϵͳ��״̬ 68 BYTES

typedef struct
{
    uint32_t sys; // ����̽ͷ״̬��31d30:Ƶ����ȣ�2.5\5.0\10\wide��
    // d29d28�첨��ʽ��ȫ��\���벨\���벨\��Ƶ d27d26̽ͷ���᣺50Q\400Q
    // d25d24�����ѹ��200V\400V ,d23-d20�ظ�Ƶ�ʣ�
    // d19d18���������1-4�Σ�d17d16����:�ݲ�/�Შ
    // d15-d12̽ͷ��ʽ����ֱ/��б/˫��/��͸
    // d11-d8��׼��
    // d7-d0����

    uint32_t block; // �Կ飬d19-d0һλ��Ӧһ���Կ飬1��ʾ���Կ�ѡ��˵��л����
    // d31-d24��8λΪ��ǰ��ѡ�Կ�
    // uint16_t probe;		//̽ͷ,����D3D2�Შ/�ڲ���ֱ/б/˫��/��͸d1d0,���̽ͷd6-d4
    // d9d8�˲���̽ͷ00���̽ͷ01С��K1б̽ͷ10����ֱ̽ͷ11˫��ֱ̽ͷ
    // d11������d12�˲������̽ͷd13С��K1б̽ͷd14����ֱ̽ͷd15˫��ֱ̽ͷ=0δ��1�Ѳ�,

    uint32_t option; // d31��d28������׼:0000ĸ��/���ϵ��£�
    // d27d26������ʾ��%/dBֵ/�׾�,d25d24��ȣ���ֱ\ˮƽ\����
    // d23-d16������
    // d15-d9��������,d15d14��׼���棬d13d12�������棬d11���̣�d10��ʱ��d9����
    // d8-d5�������d8:��׼����\�������棬d7:��A\��B��D6D5����λ\��\��
    // d4B������
    // d2-d0������Ŀ

    uint32_t status; // d31-d24�������ܲ���������D23-d18̽ͷ��������(��㡢Kֵ��DAC)��
    // d17��d14�����Ȳ���������d13-d7��׼����(0-128)
    // d6-d0:d0:������٣�D1��Kֵ\����ǣ�D2��DAC,D3:AVG,
    // 3300+: start
    uint32_t externstatus; // d0 ̽���棺�ڲ�/���
    // d1 �Զ������С
    // d2 �����ʽ d3 ģ�������̽��(0���֣�1ģ��),d4-d10��׼�߶�(0-128),d11,12ͨ�á���׼,������
    // d13,d14 ��ƽ�ף�Բ���棬������
    // 3300+: start
    uint32_t prepare[4]; // Ԥ��ֵ

} CHANNEL_STATUS; // ÿ��ͨ����״̬ 36 BYTES

typedef struct
{
    uint32_t Range;
    uint32_t OldRange; // չ��ǰ������
    uint32_t OldDelay; // չ��ǰ����ʱ
    uint32_t Delay;

    uint32_t wavepara[4]; // ���ε��ĸ�����(����,ˮƽ,��ֱ,�߶�/λ��,�߶�,λ��,�߶�)

    uint16_t Thick;
    uint16_t Diameter; // ֱ��

    uint16_t Offset;
    uint16_t Speed;     /*�Լ����٣�	m/s	*/
    uint16_t Angle;     /*����ǣ�60o	*/
    uint16_t Forward;   /*̽ͷǰ�س���	*/
    uint16_t Frequence; /*̽ͷƵ�ʣ�2.5Mhz	*/

    uint16_t Reject;

    uint16_t SurfGain; /*���油������	*/
    uint16_t BaseGain; /*��������	*/

    uint16_t DepthGain; // ��Ȳ���ֵ
    short    CompGain;  /*��������	*/

    short lineGain[6]; // ������ƫ��

    short gatedB;
    short depth; // ȱ�ݸ߶�

    uint16_t Crystal_l; /*��Ƭ�ߴ�	*/
    uint16_t Crystal_w; /*��Ƭ�ߴ�	*/

    // 3300+: start
    uint16_t PoreLong;     // ��׳���
    uint16_t PoreDiameter; // ���ֱ��
    uint16_t PoreDepth[2]; // 2��׿���
    // 3300+: end
    uint32_t prepare[4]; // Ԥ��ֵ

} CHANNEL_PARAMETER; // 100 BYTES

typedef struct
{
    uint32_t sys;    // d31d30��Ļ����,d29-d27������ʽ,d26�Ƿ�Ҫ���룬d25-d21������ʱ(������30����λΪ��)
    uint32_t option; // d31-d24��ǰѡ���̽�⹤�գ�

} USER_STATUS;

#define C_CS_BLOCK      0
#define C_CS_COMPCONT   1
#define C_CS_LOCOMOTIVE 2
#define C_CS_NUMBER     3
#define C_CS_OBJECT     4
#define C_CS_OPTION     5
#define C_CS_STATUS     6
#define C_CS_SYS        7

typedef struct
{
    short    cgain[C_CRAFT_MAX];
    uint16_t bgain[C_CRAFT_MAX];
    uint16_t sgain[C_CRAFT_MAX];
    uint16_t offset[C_CRAFT_MAX];
    uint16_t speed[C_CRAFT_MAX];
    uint16_t angle[C_CRAFT_MAX];
    uint16_t range[C_CRAFT_MAX];
    uint16_t ampstd[C_CRAFT_MAX];
} USER_PARA;

typedef struct
{
    uint32_t wavepara[4];           // ���ε��ĸ�����(����,ˮƽ,��ֱ,�߶�/λ��,�߶�,λ��,�߶�)
    uint8_t  sampbuff[C_COORWIDTH]; // ������
} WAVEPARA;
typedef struct
{
    int      posi;     // �ļ�λ��
    uint32_t date;     // �洢���� (256*(256*(100*yearh + yearl) + month) + date)
    uint16_t name[12]; // �ļ���
} FAT;                 //

typedef struct
{
    uint16_t name[30]; // ��ǩ��
} NOTES;               // ��ǩ

typedef struct
{
    uint16_t Thick;
    uint16_t Diameter; // ֱ��
    uint16_t Angle;
    uint16_t Half_Len_Bow; // ���ҳ�
    uint16_t Half_len_Arc; // �뻡��
    uint16_t Beta;         // ���ҳ���Ӧ�ĽǶ�
} CURVE_AMEND;
typedef struct
{
    uint16_t GainPlusTime;    /*�����ʱ��*/
    uint16_t GainMinusTime;   /*�����ʱ��*/
    uint16_t ParaDisplayTime; /*������ʾʱ��*/
    uint32_t PreElapsedTime;  /*ǰ�ζ����Ŀ���ʱ��*/

    int amp_total;  // �ۼƲ��θ߶�
    int amp;        // ǰ��ƽ������
    int amp_total2; // B�����ۼƲ��θ߶�
    int amp2;       // B����ǰ��ƽ������
    int time;       // �����Ĵ���
    int time2;      // B���ڶ����Ĵ���

} ADJUST_TIME; /*���ֵ���ʱ��,��λΪ1msec*/

typedef struct
{
    uint32_t Time; // ����ʱ��
    uint32_t Dist; // ��������
    uint16_t Pos;  // ����Ļ�ϵ�ˮƽ����
    uint16_t dB;   // ��߲���dB��
    short    Amp;  // ������߲�

} GATE_MAX; // ���ڳ��ֵ���߲�
typedef struct
{
    uint8_t mode[2];
    int     para[4];
} GATE_AMPMAX;

typedef struct
{
    uint8_t date;
    uint8_t month;
    uint8_t yearl;
    uint8_t yearh;
} SAVE_DATE;

typedef struct
{
    uint8_t    Channel;          // ͨ����
    short      FixGain;          // ͻ���Ĳ�������(ͻ�����32.9-33.0֮��)9
    uint16_t   pzPurchaser[11];  // ����λ	22��11����
    uint16_t   pzSerial[20];     // �ڲ��������20
    TIME_CLOCK StorageTime;      // �������10\8
    TIME_CLOCK LeaveFactoryTime; // ��������10\8
    TIME_CLOCK MaintainTime;     // ���޳�������10\8
} INNER_INFO;

typedef struct
{
    uint16_t   pzPurchaser[11]; // ��λ����	22��11����
    uint16_t   pzSerial[20];    // �������20
    TIME_CLOCK PurchaseTime;    // ��������10\8
    uint16_t   pzUser[5];       // ʹ����Ա10
    uint16_t   pzGovernor[5];   // ������Ա10
} USER_INFO;

typedef struct
{
    int      input;
    uint16_t BugLength;          // ȱ�ݳ���
    uint16_t BugHeight;          // ȱ�ݸ߶�
    uint16_t TestDep[20];        // ���鵥λ
    uint16_t EntrustDep[20];     // ί�е�λ
    uint16_t WorkpieceName[20];  // ��������
    uint16_t WorkpieceNum[20];   // �������
    uint16_t WorkpieceSpec[20];  // �������
    uint16_t MaterialNumber[20]; // �����ƺ�
    uint16_t SurfaceStatus[20];  // ����״��
    // uint16_t		HeatTreatment[11];	//�ȴ���
    // uint16_t		coupling[11];		//��ϼ�
    // ��¼���
    // uint16_t		position[11];		//��ⲿλ
    // uint16_t		scale[11];			//������
    // uint16_t		time[11];			//���ʱ��
    // uint16_t		standard[11];		//���ձ�׼
    // uint16_t		level[11];			//�ϸ񼶱�
} REPORT_INFO;

typedef struct
{
    uint16_t dac_db[C_COORWIDTH];   // ĸ��
    uint16_t dac_high[C_COORWIDTH]; // ���߸߶�
} DAC_LINE;

typedef struct
{
    uint16_t db[10];
    uint16_t dist[10];
    short    num;
    uint16_t diameter; // ������ֱ��
    uint16_t length;   // �����峤��
} DAC;

// probe
typedef struct
{
    uint16_t pzProbeManufactory[13]; // ̽ͷ����
    uint16_t pzProbeSerial[11];      // ̽ͷ���
    uint32_t date;                   // �洢���� (256*(256*(256*yearh + yearl) + month) + date)

    short ProbeMode; // ̽ͷ����
    // short Horiline;		//ˮƽ����
    // short Vertline;		//��ֱ����
    short Lvalue;          // ǰ�س���
    short Angle;           // �����
    short Frequence;       // Ƶ��
    short Sensitivity;     // ����������
    short StartWaveWidth;  // ʼ�����
    short ResolvingPower1; // �ֱ���
    short ResolvingPower2; // �ֱ���
    short Bevel;           // ƫб��
} PROBE_CAPACITY;

uint16_t __attribute__((weak)) gaintable[121];

//------��������-----
typedef struct
{
    uint32_t test;
    uint32_t Horiline;       // ˮƽ����
    uint32_t vertline;       // ��ֱ����
    uint32_t ResolvingPower; // �ֱ���
    uint32_t Sensitivity;    // ����������
    uint32_t Dynamic;        // ��̬��Χ
    int      gain[5];        // ��¼5��״̬�µ�����0ˮƽ1��ֱ2�ֱ���3����������4��̬��Χ
} POWER;

//

#define C_CHANNEL_ZPROBE  0                      /*ֱ̽ͷͨ����Ϊ0*/
#define C_CHANNEL_APROBE  (C_CHANNEL_ZPROBE + 1) /*б̽ͷͨ����Ϊ1*/
#define C_CHANNEL_LPROBE  (C_CHANNEL_APROBE + 1) /*С�Ƕ�̽ͷͨ����Ϊ2*/
#define C_CHANNEL_MANUAL  (C_CHANNEL_LPROBE + 1)

/*��������*/
#define C_MAX_SAMEKEYTIME 30 /*100*10ms = 1 sec*/

/*��ʾ���洢����*/
#define C_BASEGAIN_HPOSI  510                          //(9*16)
#define C_BASEGAIN_VPOSI  C_COORVPOSI + C_MenuHigh * 2 //(0)
#define C_COMPGAIN_HPOSI  510                          // C_BASEGAIN_HPOSI+4*16-3//(13*16)
#define C_COMPGAIN_VPOSI  C_COORVPOSI + C_MenuHigh * 3 //(0)
#define C_SURFGAIN_HPOSI  510                          //(17*16)
#define C_SURFGAIN_VPOSI  C_COORVPOSI + C_MenuHigh * 4 //(0)
#define C_RANGE_HPOSI     510                          //(25*16)
#define C_RANGE_VPOSI     C_COORVPOSI + C_MenuHigh * 6 //(0)
#define C_POWER_HPOSI     ((C_HORIDOT_SCREEN - 2 * C_ECHAR_HDOT) / 16 * 16 + 4)
#define C_POWER_VPOSI     (C_COORVPOSI + C_COORHEIGHT - 6)
#define C_SYSTIME_HPOSI   510                               // 8//( ( C_HORIDOT_SCREEN - 3 * C_ECHAR_HDOT) /16 * 16 - 4)
#define C_SYSTIME_VPOSI   C_COORVPOSI + C_MenuHigh * 11 + 4 // + C_COORHEIGHT+1//( C_COORVPOSI + C_COORHEIGHT + C_ECHAR_VDOT+1)
#define C_CHANNEL_HPOSI   564                               //( C_COORWIDTH + 10 )
#define C_CHANNEL_VPOSI   C_COORVPOSI                       // C_COORVPOSI+5//( C_COORVPOSI + 80+4 )

#define C_PBASEGAIN_HPOSI (33 * 8)
#define C_PBASEGAIN_VPOSI (0)
#define C_PCOMPGAIN_HPOSI (33 * 8)
#define C_PCOMPGAIN_VPOSI (0)
#define C_PRANGE_HPOSI    (22 * 8)
#define C_PRANGE_VPOSI    (0)
#define C_PPOWER_HPOSI    (37 * 8 + 3)
#define C_PPOWER_VPOSI    (0)

// #define C_SIZE_INT		( sizeof(int) )

#define C_SETMODE_SAVE    0                    /*���ú���ʱֻ��*/
#define C_SETMODE_SET     (C_SETMODE_SAVE + 1) /*���ú���ʱֻ����*/
#define C_SETMODE_SETSAVE (C_SETMODE_SET + 1)  /*���ú���ʱ�ȴ�Ҳ����*/

#define C_GAINPLUSTIME    4000 /*�����ʱ��,��λΪ1ms*/
#define C_GAINMINUSTIME   1500 /*�����ʱ��,��λΪ1ms*/
#define C_PARADISPLAYTIME 50   /*���β�����ʾʱ��,��λΪ1ms*/

// ͨ�����ݳ���

#define C_LEN_DAC         ((sizeof(DAC) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_PEAK        ((sizeof(GATE_PEAK) / C_SIZE_INT + 1) * C_SIZE_INT)     /*�ز�����*/
#define C_LEN_SYSPARA     ((sizeof(SYSTEM_STATUS) / C_SIZE_INT + 1) * C_SIZE_INT) /*ϵͳ���ݳ���*/
#define C_LEN_SAMP        ((C_COORWIDTH / C_SIZE_INT + 1) * C_SIZE_INT)           /*���β���*/
#define C_LEN_WELD        ((sizeof(WELD_PARA) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_CHSTATUS    ((sizeof(CHANNEL_STATUS) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_POWER       ((sizeof(POWER) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_CHPARA      ((sizeof(CHANNEL_PARAMETER) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_GATEPARA    ((sizeof(GATE_PARA) / C_SIZE_INT + 1) * C_SIZE_INT)
#define C_LEN_GATEPEAK    ((sizeof(GATE_PEAK) / C_SIZE_INT + 1) * C_SIZE_INT)

#define C_LEN_CHNPARA     (C_LEN_CHSTATUS + C_LEN_CHPARA + 2 * C_LEN_GATEPARA + C_LEN_DAC + C_LEN_WELD)
#define C_LEN_USER        ((sizeof(USER) / C_SIZE_INT + 1) * C_SIZE_INT * C_USERMAX)
#define C_LEN_USERPARA    ((sizeof(USER_PARA) / C_SIZE_INT + 1) * C_SIZE_INT * C_USERMAX)
/// #define C_LEN_CHNPARA	( C_LEN_CHSTATUS + C_LEN_CHPARA + 2*C_LEN_GATEPARA )
#define C_LEN_CURVE       ((sizeof(CURVE_AMEND) / C_SIZE_INT + 1) * C_SIZE_INT)

#define C_LEN_FILE_C      (C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 2 * C_LEN_SAMP)
#define C_LEN_FILE        (((C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 2 * C_LEN_SAMP + 255) / 256) * 256)
/// #define C_LEN_FILE		(C_LEN_SYSPARA + C_LEN_CHNPARA + C_LEN_PEAK + 1*C_LEN_SAMP + C_LEN_DAC)
#define C_LEN_FAT_C       ((sizeof(FAT) / C_SIZE_INT + 1) * C_SIZE_INT) /*ÿ���ļ�ͷ����*/
#define C_LEN_FAT         (((sizeof(FAT) + 255) / 256) * 256)           /*ÿ���ļ�ͷ����*/
#define C_LEN_DACDB       (((500 * C_SIZE_SHORT) / C_SIZE_INT + 1) * C_SIZE_INT)

#define C_LEN_FUNCTION    ((sizeof(FUNCTION) / C_SIZE_INT + 1) * C_SIZE_INT) /*�����ȼ�����*/

#if C_MAKELIB > 0
    #define C_FILE_NUMBER 200 /*�ܴ洢���ļ���*/
#else
    #define C_FILE_NUMBER 1000 /*�ܴ洢���ļ���*/
#endif

typedef struct
{
    int cache[C_FILE_NUMBER]; //
} CACHE;

#define C_OFF_INIT        0  //(0+64*1024)
#define C_OFF_PWD         20 //(20+64*1024)					/*�������׵�ַ*/
#define C_OFF_STORE       40 //(40+64*1024)					/*�������׵�ַ*/
/*
    0-3			�ں�������ʱ����ͬһ��������ʱ��
    4-11		�����ʱ�Ĺ��λ��
    20			ǰ�ΰ����ļ�ֵ
    21			ͬһ��ֵ������������
    22-25		���°���ʱ���࿪��ʱ��ʱ����λms
    26-29		ͬһ��������ʱ��
    30-33		ǰ���Զ��洢ʱ�Ŀ���ʱ��sec
    34-37		���Զ������ݸ���
    40			//ǰ�ζ��ز�����ʱ��ʱ��
    50			ǰ����ťֵ
    54			ǰ��ȡ��ťֵʱʱ��
    58			����ťֵ
    60			����ťֵ
*/

#define C_OFF_TEMP        (C_OFF_STORE + 100) /*̽�˹�����ʱ�洢���׵�ַ*/
/*
    0-3			��ǰ���õ�����ֵtestrange[0]
    4-11		�����������ʱһ�κͶ�������ֵtestrange[1]/[2]��2���֣�8
    20-27		��Kֵ������������Ⱥͷ�����ֱ��
    28-29		��Kֵ��������
    30-33		DAC���̽�����
    34-41		������ֱ���ͳ���
    42-49		ʵ������ʱ�����̣���ͨ�����㣩
    50��57		��Kֵ������������Ⱥͻ�׼���߶�

    60			��ɫ
*/
#define C_OFF_STACK       (C_OFF_TEMP + 100) /*ջ��ʼ*/
/*
    0-4			ջָ��,ָ���һ����λ��
    10-99		����
*/
#define C_OFF_INNER_INFO  (C_OFF_STACK + 256 * C_SIZE_INT) // 121
/*
    0-29		����λ	30��15����
    30-69		�ڲ��������20
    70-77		�������8
    80-87		��������8
    90-97		���޳�������8
*/
#define C_OFF_REPORT_INFO (C_OFF_INNER_INFO + ((sizeof(INNER_INFO) + 3) / 4) * 4)
/* */
#define C_OFF_USER_INFO   (((C_OFF_REPORT_INFO + sizeof(REPORT_INFO) + 255) / 256) * 256) /*�û���Ϣ*/
/*
    0-29		��λ����	30��15����
    30-69		�������20
    70-77		��������8
    80-89		ʹ����Ա10
    90-99		������Ա10
*/
#define C_OFF_BATTERY     (((C_OFF_USER_INFO + sizeof(USER_INFO) + 8 + 255) / 256) * 256) /*�����Ϣ*/
/*	��������ʼ8���ֽڴ濪��ʱ�䣬��8�ֽڴ浱ǰʱ��...
    0-7			����ʱ��8
    10-17		�ػ�ʱ��8
    20-31		���ָʾ��11
    36-39		����ʱ��,��λ����
    40-		��ʼ���ʱ��,��λ����
    80-320		��ص��������
*/
#define C_OFF_USER        (C_OFF_BATTERY + 320) /*��Ա��Ϣ*/
/*
0-3	nUserNumber
4-7 nCurrentUser
8-	stUser
*/
#define C_OFF_USERPARA    (((C_OFF_USER + C_LEN_USER + C_SIZE_INT * 2 + 1) / 2) * 2) /*��Ա�����ղ�����Ϣ*/
/*
0-3	nUserNumber
4-7 nCurrentUser
8-	stUser
*/
#define C_OFF_DAC         (((C_OFF_USERPARA + C_LEN_USERPARA + 1) / 2) * 2)
/*
    0-9*C_SIZE_SHORT	���Ծ���
    10-19				����128��ʱ��������
    20(0-3)				���Ե���
    21(4-8)
    22					����
    23					����
    24					��ʱ
*/
#define C_OFF_CURVE       (((C_OFF_DAC + C_LEN_DAC + 1) / 2) * 2)
// �������
#define C_OFF_DACDB       (((C_OFF_CURVE + C_LEN_CURVE + 1) / 2) * 2)
// DAC��dBֵ���õ��Ļ�׼��
#define C_OFF_DACGATE     (((C_OFF_DACDB + C_LEN_DACDB + 1) / 2) * 2)

#define C_OFF_POWER       (((C_OFF_DACGATE + C_LEN_DACDB + 1) / 2) * 2)

#define C_OFF_SYSTEM      (((C_OFF_POWER + C_LEN_POWER + 2) / 2) * 2)

#define C_OFF_CHANNEL     (((C_OFF_SYSTEM + C_LEN_SYSPARA * C_USERMAX + 1) / 2) * 2) /*������ͨ�����ݵ��׵�ַ*/

#define C_OFF_BACKUP      (((C_OFF_CHANNEL + C_LEN_CHNPARA * (C_CHMAX + 10) * C_USERMAX + 1) / 2) * 2)
// ����ϵͳ������״̬����
#define C_OFF_KEYFUNC     (((C_OFF_BACKUP + C_LEN_CHNPARA + C_LEN_SYSPARA + 1) / 2) * 2)

#define C_OFF_FILEINFO    (((C_OFF_KEYFUNC + C_LEN_FUNCTION + 511) / 512) * 512)
/*
    0-3		����
    4-8		λ��	struct FAT
    8��12	����	y2-m1-d1
    12��35	�ļ���	SHORT 12 �� 2
*/
#define C_OFF_PEAK        (((C_OFF_FILEINFO + C_SIZE_INT + C_LEN_FAT + 1) / 2) * 2)
// GatePeak

#define C_OFF_SAMP        (((C_OFF_PEAK + C_LEN_PEAK + 1) / 4) * 4)
// sampbuff

#define C_OFF_FREQ        (((C_OFF_SAMP + C_LEN_SAMP + 1) / 4) * 4)
// Ƶ��
#define C_OFF_CACHE       (((C_OFF_FREQ + C_LEN_SAMP + 255) / 256) * 256)

#define C_OFF_FAT         (((C_OFF_CACHE + sizeof(CACHE) + 255) / 256) * 256)
/*
    0-3		λ��
    4��7	����	y2-m1-d1
    8��19	�ļ���
*/
#define C_OFF_FILE        (((C_OFF_FAT + C_LEN_FAT * C_FILE_NUMBER + 255) / 256) * 256)
#define C_OFF_OTHER       (((C_OFF_FILE + C_LEN_FILE * C_FILE_NUMBER + 255) / 256) * 256)

#define C_OFF_F_BACKUP    (((C_OFF_OTHER + 255) / 256) * 256)

#define C_OFF_FILEMAX     (((C_OFF_F_BACKUP + (C_OFF_FAT - C_OFF_FILEINFO) + 255) / 256) * 256)

// ����״̬��
#define C_TEST_OFFSET     0
#define C_TEST_ANGLE      (C_TEST_OFFSET + 1)
#define C_TEST_DAC        (C_TEST_ANGLE + 1)
#define C_TEST_AVG        (C_TEST_DAC + 1)
#define C_MAKE_DACAVG     (C_TEST_AVG + 1)
#define C_TEST_MAX        (C_MAKE_DACAVG + 1)
#define C_TEST_ALL        8

#define C_OFF_START
/*
0-4		���˵�λ��
1-26*4	�����ղ˵�λ��
*/
