#include "sysdef.h"
#include "common.h"
#include "display.h"
#include "globe.h"
#include "main.h"
#include "sdcard.h"
#include <stdio.h>

#include "Music_alice.c"

extern int         HoriOffsetScreen; // ����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��
extern int         VertOffsetScreen; // ����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��
extern ADJUST_TIME AdjustTime;
extern NOTES       Notes;
// extern uint8_t crPara[];
extern uint8_t               bCOM2PC;
extern const uint32_t          c_crPara[];
extern const uint32_t          Echo_crPara[];
extern unsigned short       c_FWBaseLine; // ����
extern unsigned short       c_RFBaseLine; // ��Ƶ����
int                         extend;
unsigned short              GainTable[121];
extern const unsigned short c_gaintable[2][121];

#if C_DEVTYPE == 14
const unsigned int KEY_DIGITAB[] =
    {C_KEYCODMAX, 20, 15, 1, 8, 2, 1, 19, 11, 14, 5, 0, 12, 13, 12, 4, 6, 14, 32, 10, 21, 7, 9, 9, 8, 16, 18, 19, 7, 17, 8, 16, 17, 18, 2, 3, 3, 10};
/*  350c  00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36 */
#else
const unsigned int KEY_DIGITAB[] =
    //               0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
    {C_KEYCODMAX, 17, 1, 4, 7, 36, 18, 2, 5, 8, 36, 19, 3, 6, 9, 36, 16, 0, 12, 10, 36, 11, 14, 13, 15, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36};
//{ C_KEYCODMAX, 9, 6, 3,19,36,36, 8, 5, 2,18,36,36, 7, 4, 1,17,36,36,10,12, 0,16,36,36,15,13,14,11,36,36,36,36,36,36,36,36,36};

#endif
// const unsigned char MadeDate[21] = "2005-03-15          ";	/*��20*/
extern int curr_cr;

extern const unsigned char _version[]; // �汾��
#if C_DEVTYPE == 1 && C_MAKELIB == 0
const unsigned char _version1[10] = "PXUT-330 "; // �汾��
#elif C_DEVTYPE == 1 && C_MAKELIB == 1
const unsigned char _version1[10] = "JIWU     "; // �汾��
#elif C_DEVTYPE == 1 && C_MAKELIB == 2
const unsigned char _version1[10] = "NDC      "; // �汾��
#elif C_DEVTYPE == 2
const unsigned char _version1[10] = "U1       "; // �汾��
#elif C_DEVTYPE == 3
const unsigned char _version1[10] = "350+     "; // �汾��
#elif C_DEVTYPE == 4
const unsigned char _version1[10] = "320C     "; // �汾��
#elif C_DEVTYPE == 5
const unsigned char _version1[10] = "V1       "; // �汾��
#elif C_DEVTYPE == 6
const unsigned char _version1[10] = "MCh      "; // �汾��
#elif C_DEVTYPE == 7
const unsigned char _version1[10] = "V1       "; // �汾��
#elif C_DEVTYPE == 8
const unsigned char _version1[10] = "350C     "; // �汾��
#elif C_DEVTYPE == 9
const unsigned char _version1[10] = "360B+    "; // �汾��
#elif C_DEVTYPE == 10
const unsigned char _version1[10] = "U2       "; // �汾��
#elif C_DEVTYPE == 11
const unsigned char _version1[10] = "350B+     "; // �汾��
#elif C_DEVTYPE == 12
const unsigned char _version1[10] = "V2       ";
#elif C_DEVTYPE == 13
const unsigned char _version1[10] = "TFT      ";
#elif C_DEVTYPE == 14
const unsigned char _version1[10] = "F1       ";
#elif C_DEVTYPE == 15
const unsigned char _version1[10] = "PXUT-330N";
#elif C_DEVTYPE == 20
const unsigned char _version1[10] = "300C     ";
#endif

const unsigned char AmpMaxCom_Table[] = {
    // 0-8%,8,	16,	24,	32,	40,	48,	56
    5, 4, 3, 3, 2, 2, 2, 1, //;0-12.8dB
    5, 4, 4, 3, 2, 2, 2, 1, //;-25.6dB(20-40
    5, 5, 4, 4, 2, 2, 2, 1, //;-38.4dB(20-40
    5, 5, 4, 3, 2, 2, 2, 1, //;-51.2dB(40-60
    4, 4, 3, 3, 2, 2, 2, 1, //;-64.0dB(40-60
    4, 3, 2, 2, 2, 1, 1, 1, //;-76.8dB(60-80
    3, 3, 2, 1, 1, 1, 0, 0, //;-89.6dB(80-100
    2, 2, 1, 1, 1, 1, 0, 0, //;-102.4dB(80-100
    2, 2, 1, 1, 1, 1, 0, 0, //;-115.2dB(100-120 dB
    2, 2, 1, 1, 1, 1, 0, 0, //;-120.0dB(100-120 dB
};

const unsigned char PowerTable[] =
#if C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
    //	  100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	С��0%����
    {
        100,
        90,
        80,
        70,
        60,
        50,
        40,
        30,
        20,
        10,
        5,
};
#elif C_DEVLIB == 4
    // 100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	С��0%����
    {
        40,
        37,
        34,
        32,
        31,
        29,
        28,
        27,
        26,
        24,
        15,
};
#else
    // 100%, 90%,80%,70%,60%,50%,40%,30%,20%,10%,0%,	С��0%����
    {
        30,
        27,
        25,
        23,
        22,
        21,
        20,
        19,
        17,
        15,
        10,
};
#endif

const unsigned int _RANGEVALUE[] = {50, 100, 200, 300, 500, 1000, 1250, 2000, 2500, 3000, 4000}; /*����300ÿ��100*/

const unsigned short c_SystemPowerStd[] = {20, 60, 260, 260, 460}; // ���ܱ�׼ˮƽ����ֱ���ֱ�������̬������

const unsigned char _LOG_B1[] = {"   Digital Ultrasonic Flaw Detector   "};
const unsigned char _LOG_B2[] = {"           Self-test Done             "};
const unsigned char _LOG_B3[] = {"                                      "};
const unsigned char _LOG_B4[] = {"                                      "};
const unsigned char _LOG_B5[] = {"                                      "};
const unsigned char _LOG_B6[] = {"                                      "};
const unsigned char _LOG_B7[] = {"  Manufactory                         "};
const unsigned char _LOG_B8[] = {"  China Nantong Union TECH&DEV Co.,Ltd"};
const unsigned char _LOG_B9[] = {"Press 9 Initialization, 0 Battery Info"};

const unsigned short _SYSMENU_B1[] = // һ��8�У����ȱ䶯��ÿ�в��ȳ���20�֣���ÿ�б�����CH(16)����
    {
        // CH(16),//'P','X', 'U','T','-' ,'M','1',CH(16),
        CH(2394), CH(4681), CH(2864), CH(2229), CH(2171), CH(2716), CH(16),
        CH(4293), CH(5554), CH(4229), CH(1912), CH(4189), CH(1808), CH(4429), CH(4143), CH(5039), CH(16),
        CH(16),
        CH(5552), CH(2876), CH(4574), CH(1941), CH(16), // CH(4490),CH(3423),CH(2790),CH(4681),CH(5508),CH(5135),CH(16),
        CH(16),
#if C_UDISK == 1
        CH(1730), CH(2790), CH(4242), CH(5135), CH(4566), CH(2951), 'U', CH(3744), CH(16),
        CH(4221), CH(4217), CH(2070), CH(2002), CH(3312), CH(4888), 'A', CH(4108), CH(4293), CH(3061), CH(16),
#else
        CH(16),
        CH(16),
#endif
        CH(1620), '8', CH(2876), CH(2208), ' ', CH(1620), '9', CH(1985), CH(4228), CH(2715), ' ', CH(1620), '0', CH(1873), CH(3120), CH(2171), CH(1956), CH(4837), CH(4702), CH(16),
        CH(16),
        // CH(4077),CH(2894),CH(4721),CH(4519),CH(5141),CH(4346),CH(4616),CH(5487),CH(1954),CH(3110),CH(2302),CH(16),
        //	CH(3647),CH(4508),CH(5149),CH(3310),CH(4293),CH(3475),CH(2828),CH(4285),CH(3110),CH(2302),CH(5148),CH(4762),CH(2511),CH(4330),CH(16),
};

/// ��̽�˽�����ʾ�Ĳ˵���ÿ����������Ϊ6
const unsigned short _MainMenu_B1[] = {
    6 /*��*/,
    CH(217),
    CH(4429),
    CH(4143),
    CH(4293),
    CH(3061),
    CH(2006),
    CH(3277),
    CH(16),
    CH(218),
    CH(4721),
    CH(4519),
    CH(1846),
    CH(4293),
    CH(4172),
    CH(5435),
    CH(16),
    CH(219),
    CH(5039),
    CH(3887),
    CH(4852),
    CH(3660),
    CH(4803),
    CH(4973),
    CH(16),
    CH(220),
    CH(1903),
    CH(2570),
    CH(2329),
    CH(2308),
    CH(4429),
    CH(4143),
    CH(16),
    CH(221),
    CH(1921),
    CH(5465),
    CH(5417),
    CH(4429),
    CH(2009),
    CH(4524),
    CH(16),
    CH(222),
    CH(3454),
    CH(2531),
    CH(4817),
    CH(4429),
    CH(4429),
    CH(4143),
    CH(16),
};
/**
 *  ̽�����ݴ���
 *  ϵͳ��������
 *  ��������У��
 *  ���淽��̽��
 *  ����ֱ̽��͸
 *  �ֹ�б̽̽��
 *
 */

const char _InnerInfo[C_LANGUAGE][20][21] =
    {
        {"������λ", "���������", "���������", "����������", "��ά������", "������汾", "��ͻ���", "��ͨ����"},
        {
         "1.Corporation     :",
         "2.Serial number   :",
         "3.Purchase date   :",
         "4.Personnel       :",
         "5.Governor        :",
         "6.Software edition:",
         "7.Saltation     :",
         "8.Channel max     :",
         }
};

const char _UserInfo[C_LANGUAGE][19][21] = {
    {
     "����λ����:",
     "���������:",
     "����������:",
     "��ʹ����Ա:",
     "��������Ա:",
     "������汾:",
     "���ں˰汾:",
     "�����հ汾:",
     },
    {
     "1.Corporation     :",
     "2.Serial number   :",
     "3.Purchase date   :",
     "4.Personnel       :",
     "5.Governor        :",
     "6.Software edition:",
     "7.Core edition   :",
     "8.Process edition :",
     }
};

extern char _serial[];

extern unsigned int         FixGain;
unsigned short              FixGainPoint1;
unsigned short              FixGainPoint2;
extern const unsigned short FixGain1;
extern const unsigned short FixGain2;
int                         g_nDenoise, g_nAutoDenoise;
int                         g_nRepeat;
int                         g_nPulseWidth;

void MSetAmplifierGain(uint16_t gain) {
    int nDenoise;
    nDenoise = 0;
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24 || C_DEVLIB == 3

    if (gain > 1100)
        gain = 1100 + (gain - 1100) / 4.1;

#endif
    if (gain >= FixGainPoint1)
        gain += FixGain1;
    if (gain >= FixGainPoint2)
        gain += FixGain2;
    if (gain > MAX_GAIN_DB)
        gain = MAX_GAIN_DB;

#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24

    #if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 3
    if (gain > 1100)
        nDenoise = 3;
    else if (gain > 950)
        nDenoise = 2;
    else if (gain > 800)
        nDenoise = 1;
    else
        nDenoise = 0;
    #elif C_DEVLIB == 23 || C_DEVLIB == 24

    if (gain > 1000)
        nDenoise = 0;
    else if (gain > 900)
        nDenoise = 0;
    else
        nDenoise = 0;
    #endif

    if (MGetDenoise() == 0) {
        if (g_nAutoDenoise != nDenoise) {
            g_nAutoDenoise = nDenoise;
            DeNoise(g_nAutoDenoise);
            SetRepeatRate(g_nAutoDenoise * 100 + 100);
            AdjustTime.amp = AdjustTime.amp2 = AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
        }

    } else
        g_nAutoDenoise = 0;

#endif

///	MSetColor(C_CR_WAVE);//����ڵ�������ʱ�䶯��ɫ���ٶȱȽ���
#if C_DEVLIB == 4
    SetAmplifierGain(MGetChannel(), gain);
#else
    SetAmplifierGain(gain);
#endif
}

extern DAC                     Dac;
extern CHANNEL_PARAMETER       ChannelPara;
extern const unsigned long int ColorsBG[];
extern const unsigned char     background[];

int MainProject(void) // ����Ŀ����
{
    int keycode;
    int i;

    MSetColor(C_CR_WAVE);

    MSetAcquisitionEnable(0, C_SETMODE_SETSAVE);
    MSetHardEchoShow(0, C_SETMODE_SETSAVE);

    SystemParaRestore();
    ChannelParaRestore();
    for (i = 0; i < 30; i++) {
        Notes.name[i] = 0;
    }
    MSetSaveStatus(0, C_SETMODE_SETSAVE);
    MSetSavePosition(MGetSaveMode(), C_SETMODE_SETSAVE);

    MSetRange(MGetRange(1), C_SETMODE_SETSAVE /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/
    MSetSystemMode(0, 0);                                        // ���ó�һ��״̬

    if (MGetDelay(3) >= 21000) {
        MSetDelay(130019, C_SETMODE_SETSAVE);
    }

    MFclearScreen();

    MSetSystem();
    MSetSystemGain();
    keycode = C_KEYCODMAX;

    ClearEnvelope();
    MSetAcquisitionEnable(1, C_SETMODE_SETSAVE);
    MSetHardEchoShow(1, C_SETMODE_SETSAVE);
    ScreenRenovate(); /*��Ļˢ��*/
    DrawDac(0);
    MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE));

    ExpendTime(2);

    i                 = 0;
    uint32_t elapsedtime = GetElapsedTime() + 200;

#if (C_WRITE_TO_UDISK == 1)
    memset(&g_UDiskInfo, 0, sizeof(UDISK_INFO));
#endif

    int count = 0;

    TIME_CLOCK TimeClock;

    // for(i=0;i<3;i++)
    // if(MGetDefPara(i)==1)
    //{
    //	GetDefPara(i);
    /// MSetSystem();
    //}

    while (1) {
        if (GetElapsedTime() > elapsedtime) {
            count++;
            count = count % 2;
            SwitchAlarmOn(count);
            elapsedtime = GetElapsedTime() + 200;
        }

        if (keycode < 0 || keycode >= C_KEYCODMAX)
            keycode = MGetKeyCode(0);

        if (keycode != C_KEYCODMAX)
            g_UDiskInfo.DataHeaderMark = 1;

        keycode = KeyManage(keycode, 1);

#if (C_WRITE_TO_UDISK == 1)
        StorageData(keycode);
#endif
        MParaRenovate(1);
    }
}

void SystemLog(void) {
    const char _MainInfo[C_LANGUAGE][3][25] = {
        {"�Լ����  ",         "�밴���������", "���춨ģʽ  ����ʼ��"},
        {"Press any key to start", " ",                     "9.Initialization    "         }
    };
    TIME_CLOCK TimeClock;
    int        offset   = C_OFF_BATTERY;
    int        deci_len = 0;

    uint32_t xpos, ypos, para_xpos, para_ypos;
    uint32_t char_len, row_number, row_height, col_max, align; /*�ַ������ַ�����*/
    int   i, j;

    uint32_t retvalue;

    extend = 1;

    /// ��������
    xpos       = 120;
    ypos       = 20;
    col_max    = 28;
    row_number = 9;
    row_height = 48; // 32;
    align      = 2;  // ����

    MSetColor(C_CR_PARA);
    SetBackgroundColor(COLOR_BLACK);
    MSetDisplayColor(COLOR_YELLOW);
    MEraseWindow(120 + 32, 340 + 24 * 2, 120 + 32 + 246, 340 + 24 * 2 + 30);
    TextOut(120 + 32, 340 + 24 * 2 - 6, 1, 8 + 14 * MGetLanguage(), 32, (uint8_t *)_MainInfo[MGetLanguage()][0], 4);
    if (MGetLanguage() == 0)
        TextOut(120 + 32 + 5 * 24, 340 + 24 * 2 - 6, 1, 14, 32, (uint8_t *)_MainInfo[MGetLanguage()][1], 4);
    TextOut(120 + 32 + 24, 340 + 24 * 3, 1, 20, 32, (uint8_t *)_MainInfo[MGetLanguage()][2], 4);

    UGetSystemTime(&TimeClock);
    ExpendTime(5);
    SetSystemTime(&TimeClock);
    ExpendTime(5);

    ypos = 70;
    WriteDate(xpos + 50, ypos, &TimeClock);
    WriteTime(xpos + 50 + 11 * C_ECHAR_HDOT, ypos, &TimeClock); // ��ʾʱ��
    offset = C_OFF_BATTERY + 10;
    MCopyProtectedMemory((void *)offset, &TimeClock, 8, PM_COPY_WRITE);

    xpos = 60 + 10 * C_ECHAR_HDOT;
    ypos = C_COORVPOSI + C_COORHEIGHT - 8;
    EMenuOut(xpos, ypos, "SN:", 3, 1, 12);

    xpos += C_ECHAR_HDOT * 3;
    INNER_INFO InnerInfo;
    offset = C_OFF_INNER_INFO;
    MCopyProtectedMemory(&InnerInfo, (void *)offset, sizeof(INNER_INFO), PM_COPY_READ);

    for (i = 0; i < 8; i++)
        _serial[i] = InnerInfo.pzSerial[i];
    EMenuOut(xpos + C_ECHAR_HDOT, ypos, _serial, 8, 1, 24);

    MSetAcquisition(0);
    int   keycode;
    uint32_t elapsedtime = GetElapsedTime() + 1000 * 10;
    uint32_t SameKeyTime;
    int   tempi = 0;
    while (GetElapsedTime() < elapsedtime && !bCOM2PC) /*5sec ����а����˳�*/
    {
        if (tempi < 360) {
            for (i = 0; i < 16; i++) {
                tempi += 4;
                MDrawPixel(136 + 72 + tempi, 380, DP_SET);
            }
        }

        for (SameKeyTime = 0; SameKeyTime < 100; SameKeyTime++) {
            ExpendTime(5);
            keycode = MGetKey2();
            if (keycode != C_KEYCODMAX)
                break;
        }
        if (keycode == C_KEYCODMAX)
            continue;

        SameKeyTime = TestSameKeyTime(keycode, GetElapsedTime(), C_SAMEKEYTIME);

        xpos       = 30;
        ypos       = 50;
        char_len   = 6;
        row_number = 6;
        row_height = 48; // 32;

        elapsedtime = GetElapsedTime() + 1000 * 1;
        j           = keycode;

        if (keycode == 1 && SameKeyTime >= C_SAMEKEYTIME) {
            MFclearScreen();

            int number, deci_len, Fixgain;
            number   = 0;
            deci_len = 0;
            EMenuOut(0, 0, "Pwd:", 4, 1, 8);
            if (Input_Number(C_ECHAR_HDOT * 4, 0, &number, 8, &deci_len, 0) < 0) {
                number = C_DEFPWD - 1;
            }
            //	CMenuOut(xpos,ypos,_InnerInfo,char_len,row_number,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            // for (i=0; i<row_number+2; i++)
            // {
            // TextOut(xpos, ypos+row_height*i-C_ECHAR_VDOT*MGetLanguage(),  1, 19,24, (uint8_t*)_InnerInfo[MGetLanguage()][i], 4);
            // }
            TextOut(xpos, ypos + row_height * 0 - C_ECHAR_VDOT * MGetLanguage(), 1, 19, 24, (uint8_t *)_InnerInfo[MGetLanguage()][0], 4);
            TextOut(xpos, ypos + row_height * 1 - C_ECHAR_VDOT * MGetLanguage(), 1, 19, 24, (uint8_t *)_InnerInfo[MGetLanguage()][1], 4);
            TextOut(xpos, ypos + row_height * 2 - C_ECHAR_VDOT * MGetLanguage(), 1, 19, 24, (uint8_t *)_InnerInfo[MGetLanguage()][5], 4);
            TextOut(xpos, ypos + row_height * 3 - C_ECHAR_VDOT * MGetLanguage(), 1, 19, 24, (uint8_t *)_InnerInfo[MGetLanguage()][6], 4);
            TextOut(xpos, ypos + row_height * 4 - C_ECHAR_VDOT * MGetLanguage(), 1, 19, 24, (uint8_t *)_InnerInfo[MGetLanguage()][7], 4);

            para_xpos = xpos + (char_len + 1) * C_CCHAR_HDOT; /*д������λ��ˮƽ��������*/
            para_ypos = ypos;
            MSetColor(C_CR_PARA);

            if (InnerInfo.StorageTime.month > 0x12) {
                InnerInfo.StorageTime.yearh = 0x20;
                InnerInfo.StorageTime.yearl = 0x11;
                InnerInfo.StorageTime.month = 0x11;
                InnerInfo.StorageTime.date  = 1;
            }
            if (InnerInfo.LeaveFactoryTime.month > 0x12) {
                InnerInfo.LeaveFactoryTime.yearh = 0x20;
                InnerInfo.LeaveFactoryTime.yearl = 0x11;
                InnerInfo.LeaveFactoryTime.month = 0x11;
                InnerInfo.LeaveFactoryTime.date  = 1;
            }
            if (InnerInfo.MaintainTime.month > 0x12) {
                InnerInfo.MaintainTime.yearh = 0x10;
                InnerInfo.MaintainTime.yearl = 0x00;
                InnerInfo.MaintainTime.month = 1;
                InnerInfo.MaintainTime.date  = 1;
            }

            if (InnerInfo.pzPurchaser[0] > 22) {
                InnerInfo.pzPurchaser[0] = 0;
            }

            while (number == C_DEFPWD || number == C_SUPERPWD) {
                // ����λ	pzPurchaser[15];
                xpos = para_xpos;
                ypos = para_ypos + row_height * 0;

                //  Write_Number(xpos, 0, InnerInfo.pzPurchaser[0],4,0,1);
                CETextOut2(xpos + 9, ypos, InnerInfo.pzPurchaser + 1, InnerInfo.pzPurchaser[0], 1, 24, 0, 0); /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                // �������
                ypos = para_ypos + row_height * 1;
                //;CETextOut2(xpos,ypos,InnerInfo.pzSerial+1,InnerInfo.pzSerial[0],1,24,0,0);
                // ETextOut(xpos+8,ypos,_serial,8);	//��ʾ���
                for (i = 0; i < 8; i++)
                    _serial[i] = InnerInfo.pzSerial[i];
                EMenuOut(xpos - C_ECHAR_HDOT, ypos, _serial, 8, 1, 24);

                /*    //�������StorageTime;
                   ypos = para_ypos + row_height *2;
                   // WriteDate(xpos, ypos, &InnerInfo.StorageTime);


                   //��������10\8	LeaveFactoryTime;
                   ypos = para_ypos + row_height *3;
                   // WriteDate(xpos, ypos, &InnerInfo.LeaveFactoryTime);

                   //���޳� ������10\8	MaintainTime;
                   ypos = para_ypos + row_height *4;
                   // WriteDate(xpos, ypos, &InnerInfo.MaintainTime);
    */
                ypos = para_ypos + row_height * 2;
                EMenuOut(xpos, ypos, _version1, 9, 1, 12);
                EMenuOut(xpos + 10 * C_ECHAR_HDOT, ypos, _version, 18, 1, 12);

                ypos = para_ypos + row_height * 3;
                Write_Number(xpos, ypos, InnerInfo.FixGain, 4, 0, 1);

                ypos = para_ypos + row_height * 4;
                if (InnerInfo.Channel < 20)
                    InnerInfo.Channel = 20;
                Write_Number(xpos, ypos, InnerInfo.Channel, 4, 0, 0);

                keycode = MenuKeyReturn(8, 3); /*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                if (keycode == C_KEYCOD_RETURN) {
                    MCopyProtectedMemory(&InnerInfo, (void *)offset, sizeof(INNER_INFO), PM_COPY_READ);
                    retvalue = C_FALSE;
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM) {
                    MSetSystem();
                    MCopyProtectedMemory(offset, &InnerInfo, sizeof(INNER_INFO), PM_COPY_WRITE);
                    offset = C_OFF_INIT;
                    CopyMemoryToSystem((void *)offset, C_OFF_USER_INFO - offset, PM_COPY_WRITE);
                    //                    CopyMemoryToSystem((void*)offset,sizeof(INNER_INFO), PM_COPY_WRITE);
                    retvalue = C_TRUE;
                    break;
                }

                ypos = para_ypos + row_height * (keycode - 1);

                MSetInputMode(C_INPUT_PINYIN, 0);
                switch (keycode) {
                    case 1: //
                        Write_Space(xpos, ypos, 14, 1);
                        MInputChar(xpos, ypos, 1, InnerInfo.pzPurchaser, 20, 10);
                        break;
                    case 2: //
                        Write_Space(xpos, ypos + C_ECHAR_VDOT, 19, 0);
                        MSetInputMode(C_INPUT_LOWERCASE, 0); // ���ó�Сд
                        MInputChar(xpos, ypos, 0, InnerInfo.pzSerial, 19, 19);
                        break;
                    // case 3:
                    // InputDate(xpos,ypos, &InnerInfo.StorageTime);
                    // break;
                    // case 4:
                    // InputDate(xpos,ypos, &InnerInfo.LeaveFactoryTime);
                    // break;
                    // case 5:
                    // InputDate(xpos,ypos, &InnerInfo.MaintainTime);
                    // break;
                    case 3:
                        // InputTime(xpos,ypos, &TimeClock);
                        // WriteTime(xpos,ypos+8, &TimeClock);
                        break;
                    case 4: // ͻ���
                        Fixgain = 0;
                        Input_Number(xpos, ypos, &Fixgain, 3, &deci_len, 1);
                        InnerInfo.FixGain = Fixgain;
                        break;
                    case 5: // ͨ����
                        while (1) {
                            Fixgain = 0;
                            Input_Number(xpos, ypos, &Fixgain, 3, &deci_len, 1);
                            if (Fixgain > 19 && Fixgain < 51)
                                break;
                        }
                        InnerInfo.Channel = Fixgain;
                        break;
                }
            }
        } else if (keycode == 2 && SameKeyTime >= C_SAMEKEYTIME) {
            MFclearScreen();
            SysPwd(1);
        } else if (keycode == 3 && SameKeyTime >= C_SAMEKEYTIME) {
            MFclearScreen();
            SysPwd(0);
            int key;
#if C_DEVLIB == 2 || C_DEVLIB == 3 || C_DEVLIB == 23
            int   x, y, len;
            int   color, red = 0, green = 0, blue = 0; // red < 8,green < 8,blue<4
            uint32_t data;
            // set color for screen operation, (b0-b2): red ; (b3-b5): green ; (b6-b7): blue
            x   = 8;
            y   = 32;
            len = 8;
            ETextOut(10, 10, "PRESS #!   ", 8);
            while (true) {
                key = MAnyKeyReturn();
                switch (key) {
                    case 1:
                        red++;
                        break;
                    case 2:
                        red--;
                        break;
                    case 3:
                        green++;
                        break;
                    case 4:
                        green--;
                        break;
                    case 6:
                        blue++;
                        break;
                    case 7:
                        blue--;
                        break;
                    case 5:
                        red = green = blue = 0;
                        break;
                    case 8:
                        len += 2;
                        break;
                    case 9:
                        len -= 2;
                        break;
                }
                if (key == C_KEYCOD_RETURN)
                    break;
                len = len % 32;
                red &= 0x7;
                green &= 0x7;
                blue &= 0x3;
                color = (red + (green << 3) + (blue << 6));
                MEraseWindow(x - 4, y - 4, x + len + 8, y + 4);
                // SetDisplayColor(color);
                Write_Number(100, 100, red, 4, 0, 0);
                Write_Number(100, 110, green, 4, 0, 0);
                Write_Number(100, 120, blue, 4, 0, 0);
                Write_Number(100, 130, len, 4, 0, 0);
                MDrawLine(x, y - 2, x + len, y - 2, C_CR_BACK);
                MDrawLine(x, y - 1, x + len, y - 1, C_CR_BACK);
                MDrawLine(x, y, x + len, y, C_CR_BACK);
                MDrawLine(x, y + 1, x + len, y + 1, C_CR_BACK);

                for (i = 0; i < 8; i++) {
                    data = MGetPixels(x, y - 4 + i, len);
                    Write_Number(x, 60 + i * 10, data, 10, 0, 0);
                }
            }

            MFclearScreen();
            ETextOut(10, 10, "Test LCD Color! Keyin for Test,# next.", 39);

            while (true) {
                key = MAnyKeyReturn();
                if (key == C_KEYCOD_RETURN)
                    break;
                if (key == 0 || key > C_CR_NUMBER)
                    continue;
                MFclearScreen();
                SetDisplayColor(c_crPara[key - 1]);
                for (i = 0; i < C_VERTDOT_SCREEN; i++)
                    DrawLine(0, i, C_HORIDOT_VIDEO - 1, i);
            }
            MFclearScreen();
            ETextOut(10, 10, "Test RGB Color! Please wait!", 28);
            key = 0;

            while (true) {
                int r, g, b;
                key = MAnyKeyReturn();
                if (key == C_KEYCOD_RETURN)
                    break;
                b = 0;
                if (key >= 0 && key < 4)
                    b = key;
                for (r = 0; r < 8; r++) {
                    for (g = 0; g < 8; g++) {
                        SetDisplayColor((b << 6) + (g << 3) + r);
                        for (key = 0; key < 30; key++)
                            DrawLine(r * 40, g * 30 + key, r * 40 + 40, g * 30 + key);
                        SetDisplayColor(COLOR_WHITE);
                        Write_Number(r * 40 + 1, g * 30 + 1, ((b << 6) + (g << 3) + r), 4, 0, 0);
                    }
                }
            }

#endif
#if C_DEVLIB == 23
            int nPulseWidth = 0;

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0, 0); // ���ó�һ��״̬
            MSetBaseGain(400, C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate(); /*��Ļˢ��*/

            nPulseWidth = 33; // 33
            ETextOut(10, 10, "PulseWidth:  ", 11);
            Write_Number(10 + C_ECHAR_HDOT * 11, 10, nPulseWidth, 4, 0, 0);
            ExpendTime(50);
            MKeyRlx();
            while (true) {
                MParaRenovate(1);
                key = MGetKeyCode(0);
                if (key == C_KEYCODMAX)
                    continue;
                if (key == C_KEYCOD_RETURN)
                    break;
                if (key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS) {
                    if (key == C_KEYCOD_PLUS)
                        nPulseWidth++;
                    else if (key == C_KEYCOD_MINUS)
                        nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    if (nPulseWidth >= 127)
                        nPulseWidth = 0;
                    else if (nPulseWidth < 0)
                        nPulseWidth = 127;
                    ETextOut(10, 0, "                  ", 16);
                    ETextOut(10, 10, "PulseWidth:       ", 16);
                    Write_Number(10 + 8 * 11, 10, nPulseWidth, 4, 0, 0);
                    ExpendTime(20);
                    continue;
                }

                if (key == C_KEYCOD_BASEGAIN || key == C_KEYCOD_GATE || key == C_KEYCOD_RANGE) {
                    KeyManage(key, 1);
                    ExpendTime(20);
                }
                MKeyRlx();
            }
#endif
        }

        else if (keycode == 4 && SameKeyTime >= C_SAMEKEYTIME) {
            MFclearScreen();
            int grade = 0;
            EMenuOut(0, 0, "C_OFF_FILE:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 00, C_OFF_FILE, 5, 0, 0);
            EMenuOut(296, 0, "C_OFF_FILEMAX:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 00, C_OFF_FILEMAX, 10, 0, 0);
            EMenuOut(0, 24, "C_LEN_FAT:", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24, C_LEN_FAT, 5, 0, 0);
            EMenuOut(0, 24 * 2, "C_LEN_FILE:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 2, C_LEN_FILE, 5, 0, 0);
            EMenuOut(0, 24 * 3, "FIX_GAIN:", 9, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 3, FixGain, 5, 0, 0);
            EMenuOut(0, 24 * 4, "FIXGAIN1:", 9, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 4, FixGain1 - 13, 5, 0, -1);
            EMenuOut(0, 24 * 5, "FIXPOINT1:", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 5, FixGainPoint1, 5, 0, 0);
            EMenuOut(0, 24 * 6, "BASELINE :", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 6, c_FWBaseLine, 5, 0, 0);
            Write_Number(C_ECHAR_HDOT * 20, 24 * 6, c_RFBaseLine, 5, 0, 0);
            EMenuOut(0, 24 * 7, "DAC:      ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 7, C_LEN_DAC, 5, 0, 0);
            EMenuOut(0, 24 * 8, "gatepeak: ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 8, C_LEN_PEAK, 5, 0, 0);
            EMenuOut(0, 24 * 9, "syspara:  ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 9, C_LEN_SYSPARA, 5, 0, 0);
            EMenuOut(0, 24 * 10, "WELD:     ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 10, C_LEN_WELD, 5, 0, 0);
            EMenuOut(0, 24 * 11, "CHSTAT:   ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 11, C_LEN_CHSTATUS, 5, 0, 0);
            EMenuOut(0, 24 * 12, "CHPARA:   ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 12, C_LEN_CHPARA, 5, 0, 0);
            EMenuOut(0, 24 * 13, "GATEPARA: ", 10, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * 13, C_LEN_GATEPARA, 5, 0, 0);

            grade = 0;
            MAnyKeyReturn();

            // 1#define C_OFF_INIT
            EMenuOut(0, 24 * grade, "C_OFF_INIT:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_INIT, 5, 0, 0);
            grade++;
            // 2#define C_OFF_PWD
            EMenuOut(0, 24 * grade, "C_OFF_PWD: ", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_PWD, 5, 0, 0);
            grade++;
            // 3#define C_OFF_STORE
            EMenuOut(0, 24 * grade, "_OFF_STORE:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_STORE, 5, 0, 0);
            grade++;
            // 4#define C_OFF_TEMP
            EMenuOut(0, 24 * grade, "C_OFF_TEMP:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_TEMP, 5, 0, 0);
            grade++;
            // 5#define C_OFF_STACK
            EMenuOut(0, 24 * grade, "_OFF_STACK:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_STACK, 5, 0, 0);
            grade++;
            // 6#define C_OFF_INNER_INFO/
            EMenuOut(0, 24 * grade, "INNER_INFO:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_INNER_INFO, 5, 0, 0);
            grade++;
            // 7#define C_OFF_REPORT_INFO
            EMenuOut(0, 24 * grade, "REPORTINFO:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_REPORT_INFO, 5, 0, 0);
            grade++;
            // 8#define C_OFF_USER_INFO
            EMenuOut(0, 24 * grade, "_USER_INFO:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_USER_INFO, 5, 0, 0);
            grade++;
            // 9#define C_OFF_BATTERY
            EMenuOut(0, 24 * grade, "OFF_BATTERY:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_BATTERY, 5, 0, 0);
            grade++;
            // 10#define C_OFF_USER
            EMenuOut(0, 24 * grade, "C_OFF_USER: ", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_USER, 5, 0, 0);
            grade++;
            // 11#define C_OFF_USERPARA
            EMenuOut(0, 24 * grade, "OFFUSERPARA:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_USERPARA, 5, 0, 0);
            grade++;
            // 12#define C_OFF_DAC
            EMenuOut(0, 24 * grade, "C_OFF_DAC:  ", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_DAC, 5, 0, 0);
            grade++;
            // 13#define C_OFF_CURVE
            EMenuOut(0, 24 * grade, "C_OFF_CURVE:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_CURVE, 5, 0, 0);
            grade++;
            // 14#define C_OFF_DACDB
            EMenuOut(0, 24 * grade, "C_OFF_DACDB:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_DACDB, 5, 0, 0);
            grade++;
            // 15#define C_OFF_DACGATE
            EMenuOut(0, 24 * grade, "OFF_DACGATE:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_DACGATE, 5, 0, 0);
            grade++;
            // 16#define C_OFF_POWER
            EMenuOut(0, 24 * grade, "C_OFF_POWER:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_POWER, 5, 0, 0);
            grade++;
            // 17#define C_OFF_SYSTEM
            EMenuOut(0, 24 * grade, "_OFF_SYSTEM:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_SYSTEM, 5, 0, 0);
            grade++;
            // 18#define C_OFF_CHANNEL
            EMenuOut(0, 24 * grade, "OFF_CHANNEL:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_CHANNEL, 5, 0, 0);
            grade++;
            // 19#define C_OFF_BACKUP
            EMenuOut(0, 24 * grade, "_OFF_BACKUP:", 11, 1, 12);
            Write_Number(C_ECHAR_HDOT * 12, 24 * grade, C_OFF_BACKUP, 5, 0, 0);
            grade = 0;
            // 20#define C_OFF_FILEINFO
            EMenuOut(296, 24 * grade, "_FILEINFO:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_FILEINFO, 5, 0, 0);
            grade++;
            // 21#define C_OFF_PEAK
            EMenuOut(296, 24 * grade, "COFF_PEAK:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_PEAK, 5, 0, 0);
            grade++;
            // 22#define C_OFF_SAMP
            EMenuOut(296, 24 * grade, "C_OFF_SAMP:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_SAMP, 5, 0, 0);
            grade++;
            // 23#define C_OFF_FREQ
            EMenuOut(296, 24 * grade, "C_OFF_FREQ:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_FREQ, 5, 0, 0);
            grade++;
            // 24#define C_OFF_KEYFUNC
            EMenuOut(296, 24 * grade, "FF_KEYFUNC:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_KEYFUNC, 5, 0, 0);
            grade++;
            // 25#define C_OFF_FAT
            EMenuOut(296, 24 * grade, "C_OFF_FAT: ", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_FAT, 5, 0, 0);
            grade++;
            // 26#define C_OFF_FILE
            EMenuOut(296, 24 * grade, "C_OFF_FILE:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_FILE, 6, 0, 0);
            grade++;
            // 27#define C_OFF_OTHER
            EMenuOut(296, 24 * grade, "COFF_OTHER:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_OTHER, 10, 0, 0);
            grade++;
            // 28#define C_OFF_FILEMAX
            EMenuOut(296, 24 * grade, "   FILEMAX:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_FILEMAX, 10, 0, 0);
            grade++;
            // 28#define C_OFF_CACHE
            EMenuOut(296, 24 * grade, "_OFF_CACHE:", 11, 1, 12);
            Write_Number(296 + C_ECHAR_HDOT * 12, 24 * grade, C_OFF_CACHE, 10, 0, 0);
            grade = 0;

            MAnyKeyReturn();

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0, 0); // ���ó�һ��״̬
            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate(); /*��Ļˢ��*/
            DrawDac(0);

            MParaRenovate(0);
            EMenuOut(10, 30, "LCD", 3, 1, 12);
            grade = 0;
            while (GetElapsedTime() < 1000 * 60 * 5) {
                keycode = MGetKeyCode(0);

                if (keycode == C_KEYCOD_RETURN)
                    break;
#if C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
                if (keycode == 4)
                    grade++;
                else if (keycode == 1)
                    grade--;
                else if (keycode == 0)
                    grade = 0;
                else
                    continue;
                if (grade < 0)
                    grade = 31;
                if (grade > 31)
                    grade = 0;
                ExpendTime(20);
    #if C_DEVLIB == 24
                SetELBrightness(grade);
    #else
                SetLCDBrightness(grade);
    #endif
                Write_Number(10, 40, grade, 4, 0, 0);
#endif
            }
            EMenuOut(10, 30, "VOL", 3, 1, 12);
            MKeyRlx();
            ExpendTime(50);
            grade = 0;
            while (1) {
                keycode = MGetKeyCode(0);

                if (keycode == C_KEYCOD_RETURN)
                    break;
#if C_DEVLIB == 3
                if (keycode == 4)
                    grade++;
                else if (keycode == 1)
                    grade--;
                else if (keycode == 0)
                    grade = 0;
                else
                    continue;
                if (grade < 0)
                    grade = 0;
                if (grade > 31)
                    grade = 31;
                ExpendTime(20);
                SetTransmitterVoltage(grade);
                Write_Number(10, 40, grade, 4, 0, 0);
#endif
            }
        } else if (keycode == 5 && SameKeyTime >= C_SAMEKEYTIME) {
            unsigned short nBaseLine, nFwline, nRfline;
            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0, 0); // ���ó�һ��״̬
            MSetBaseGain(400, C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate(); /*��Ļˢ��*/
            DrawDac(0);

            MParaRenovate(0);
            nFwline = c_FWBaseLine;
            nRfline = c_RFBaseLine;
            while (true) {
                ETextOut(0, 0, "Full Wave BaseLine    ", 20);
                ETextOut(0, 24, "BaseLine    ", 10);
                nBaseLine = nFwline;
                Write_Number(100, 10, nBaseLine, 4, 0, 0);
                SetEchoMode(FULL_WAVE);
                SetEchoBaseLine(c_FWBaseLine);
                while (true) {
                    keycode = MGetKeyCode(0);
                    if (keycode == C_KEYCODMAX)
                        continue;
                    if (keycode == C_KEYCOD_PLUS)
                        nBaseLine++;
                    else if (keycode == C_KEYCOD_MINUS)
                        nBaseLine--;
                    else if (keycode == C_KEYCOD_BASEGAIN)
                        GainChange();
                    else if (keycode == C_KEYCOD_CONFIRM)
                        Input_Number(100, 10, nBaseLine, 4, 0, 0);
                    if (nBaseLine > 4095)
                        nBaseLine = 0;
                    else if (nBaseLine < 1)
                        nBaseLine = 4095;
                    SetEchoBaseLine(nBaseLine);
                    Write_Number(100, 10, nBaseLine, 4, 0, 0);
                    ExpendTime(10);
                    if (keycode == C_KEYCOD_RETURN)
                        break;
                }

                ExpendTime(100);
                MKeyRlx();
                ETextOut(0, 0, "RF Wave BaseLine       ", 20);
                nBaseLine = nRfline;
                Write_Number(100, 10, nBaseLine, 4, 0, 0);
                SetEchoMode(RF_WAVE);
                SetEchoBaseLine(c_RFBaseLine);
                while (true) {
                    keycode = MGetKeyCode(0);
                    if (keycode == C_KEYCODMAX)
                        continue;
                    if (keycode == C_KEYCOD_PLUS)
                        nBaseLine++;
                    else if (keycode == C_KEYCOD_MINUS)
                        nBaseLine--;
                    else if (keycode == C_KEYCOD_BASEGAIN)
                        GainChange();
                    else if (keycode == C_KEYCOD_CONFIRM)
                        Input_Number(100, 10, nBaseLine, 4, 0, 0);
                    if (nBaseLine > 4095)
                        nBaseLine = 4095;
                    else if (nBaseLine < 1)
                        nBaseLine = 0;
                    SetEchoBaseLine(nBaseLine);
                    Write_Number(100, 10, nBaseLine, 4, 0, 0);
                    ExpendTime(10);
                    if (keycode == C_KEYCOD_RETURN)
                        break;
                }
            }
        } else if (keycode == 6 && SameKeyTime >= C_SAMEKEYTIME) {
            MFclearScreen();

            int key;
            int nPulseWidth = 0;

            SystemParaRestore();
            ChannelParaRestore();
            MSetSystemMode(0, 0); // ���ó�һ��״̬
            MSetBaseGain(400, C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);

            ScreenRenovate(); /*��Ļˢ��*/

            nPulseWidth = 33; // 33
            ETextOut(320, 10, "PulseWidth:  ", 11);
            Write_Number(320 + C_ECHAR_HDOT * 11, 10, nPulseWidth, 4, 0, 0);
            ExpendTime(50);
            MKeyRlx();
            while (true) {
                MParaRenovate(1);
                key = MGetKeyCode(0);
                if (key == C_KEYCODMAX)
                    continue;
                if (key == C_KEYCOD_RETURN)
                    break;
                if (key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS) {
                    if (key == C_KEYCOD_PLUS)
                        nPulseWidth++;
                    else if (key == C_KEYCOD_MINUS)
                        nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    if (nPulseWidth >= 127)
                        nPulseWidth = 0;
                    else if (nPulseWidth < 0)
                        nPulseWidth = 127;
                    ETextOut(320, 0, "                  ", 16);
                    ETextOut(320, 10, "PulseWidth:       ", 16);
                    Write_Number(320 + C_ECHAR_HDOT * 11, 10, nPulseWidth, 4, 0, 0);
                    ExpendTime(20);
                    continue;
                }

                if (key == C_KEYCOD_BASEGAIN || key == C_KEYCOD_GATE || key == C_KEYCOD_RANGE) {
                    KeyManage(key, 1);
                    ExpendTime(20);
                }
                MKeyRlx();
            }
        } else if (keycode == 7 && SameKeyTime >= C_SAMEKEYTIME) // ���İ���
        {
            MFclearScreen();
            DisplayBMP(background, ColorsBG, 0, 100 - 24, 281, 640);
            MSetDisplayColor(COLOR_YELLOW);
            EMenuOut(162, 2, "Kernel Testing ...      ", 20, 1, 16);
            EMenuOut(120 + 32, 340 + 24 * 1 - 7, "1.Save Test    :     ", 20, 1, 16);
            EMenuOut(120 + 32, 340 + 24 * 2 - 7, "2.Sound Volume :     ", 20, 1, 16);
            EMenuOut(120 + 32, 340 + 24 * 3 - 7, "3.Internet Test:     ", 20, 1, 16);
            EMenuOut(120 + 32, 340 + 24 * 4 - 7, "4.SD card Test :     ", 20, 1, 16);
            EMenuOut(120 + 32, 340 + 24 * 5 - 7, "5.I2C Test     :     ", 20, 1, 16);

            while (true) {
                int  key   = MGetKeyCode(0);
                char Sound = 50, second = 0, tempSecond = 0;
                int  Internet = 0;

                MSetDisplayColor(COLOR_GREEN);

                GetSystemTime(&TimeClock);
                tempSecond = Bcd2Hex(TimeClock.second & 0x7f) % 60;
                if (tempSecond != second) {
                    ExpendTime(40);
                    // MEraseWindow(120+50,46,120+50+19*C_ECHAR_HDOT,46+24);
                    WriteDate(120 + 50, 46, &TimeClock);
                    WriteTime(120 + 50 + 11 * C_ECHAR_HDOT, 46, &TimeClock); // ��ʾʱ��
                    EMenuOut(120 + 50 + 16 * C_ECHAR_HDOT, 46, ":", 1, 1, 16);
                    if (tempSecond < 10) {
                        Write_Number(120 + 50 + 17 * C_ECHAR_HDOT, 46, 0, 1, 0, 0);
                        Write_Number(120 + 50 + 18 * C_ECHAR_HDOT, 46, tempSecond, 2, 0, 0);
                    } else {
                        Write_Number(120 + 50 + 17 * C_ECHAR_HDOT, 46, tempSecond, 2, 0, 0);
                    }
                    second = tempSecond;
                }

                MSetDisplayColor(COLOR_CYAN);
                if (key == C_KEYCODMAX)
                    continue;
                else if (key == C_KEYCOD_RETURN || key == C_KEYCOD_CONFIRM)
                    break;
                else if (key == 1) // save
                {
                    int temp1, temp2;
                    EMenuOut(120 + 32 * 9, 340 + 24 * 1 - 7, ".......", 7, 1, 16);

                    offset = C_OFF_TEMP;
                    CopyMemoryToSystem(offset, 8, PM_COPY_READ);
                    MCopyProtectedMemory(&temp1, (void *)offset, 8, PM_COPY_READ);
                    //     Write_Number(10, 340+24*1-4,temp1,10,0,0);
                    //     MAnyKeyReturn();
                    if (temp1 == 5)
                        temp2 = 10;
                    else
                        temp2 = 5;
                    MCopyProtectedMemory((void *)offset, &temp2, 8, PM_COPY_WRITE);
                    CopyMemoryToSystem(offset, 8, PM_COPY_WRITE);

                    CopyMemoryToSystem(offset, 8, PM_COPY_READ);
                    MCopyProtectedMemory(&temp2, (void *)offset, 8, PM_COPY_READ);

                    ExpendTime(10);
                    if ((temp1 == 5 && temp2 == 10) || temp2 == 5)
                        EMenuOut(120 + 32 * 9, 340 + 24 * 1 - 7, "Success", 7, 1, 16);
                    else
                        EMenuOut(120 + 32 * 9, 340 + 24 * 1 - 7, "Fail   ", 7, 1, 16);
                    //     MAnyKeyReturn();
                    MCopyProtectedMemory((void *)offset, &temp1, 8, PM_COPY_WRITE);
                    CopyMemoryToSystem(offset, 8, PM_COPY_WRITE);

                    CopyMemoryToSystem(offset, 8, PM_COPY_READ);
                    MCopyProtectedMemory(&temp2, (void *)offset, 8, PM_COPY_READ);
                    //    Write_Number(10, 340+24*1-4,temp2,10,0,0);
                    //     MAnyKeyReturn();

                } else if (key == 2) // Sound
                {
                    WavePlayback((WavHeader *)alice, Sound);
                    WaveVolume(Sound);
                    key         = C_KEYCODMAX;
                    elapsedtime = GetElapsedTime() + 1000 * 5;
                    while (1) {
                        key = MGetKeyCode(0);
                        if (key == C_KEYCOD_RETURN || key == C_KEYCOD_CONFIRM) {
                            MKeyRlx();
                            key = C_KEYCODMAX;
                            break;
                        } else if (key == C_KEYCOD_PLUS)
                            Sound += 10;
                        else if (key == C_KEYCOD_MINUS)
                            Sound -= 10;
                        else if (key != C_KEYCODMAX) {
                            break;
                        }
                        if (Sound >= 100)
                            Sound = 100;
                        else if (Sound <= 0)
                            Sound = 0;
                        Write_Number(120 + 32 * 9, 340 + 24 * 2 - 4, Sound, 3, 0, 0);
                        if (GetElapsedTime() >= elapsedtime) {
                            WavePlayback((WavHeader *)alice, Sound);
                            elapsedtime = GetElapsedTime() + 1000 * 5;
                        }
                        WaveVolume(Sound);
                    }
                    while (1) {
                        if (GetElapsedTime() >= elapsedtime) {
                            break;
                        }
                    }
                } else if (key == 3) // Internet
                {
                    /// The MAC address
                    unsigned char MacAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

                    /// The IP address
                    unsigned char IpAddress[4] = {192, 168, 111, 8}; //{10, 159, 245, 156};

                    EMenuOut(120 + 32 * 9, 340 + 24 * 3 - 7, ".......", 7, 1, 16);
                    Internet = EthernetInit(MacAddress, IpAddress);
                    ExpendTime(10);
                    if (Internet == 0)
                        EMenuOut(120 + 32 * 9, 340 + 24 * 3 - 7, "Success", 7, 1, 16);
                    else
                        EMenuOut(120 + 32 * 9, 340 + 24 * 3 - 7, "Fail   ", 7, 1, 16);
                    // Write_Number(120+32*9, 340+24*4-4,Internet,3,0,0);
                    EthernetStop();
                    MKeyRlx();
                    key = C_KEYCODMAX;
                } else if (key == 4) // SD
                {
                    EMenuOut(120 + 32 * 9, 340 + 24 * 4 - 7, ".......", 7, 1, 16);

                    if (DisplayQuery(14)) // �����Ӻ�U�̣�
                    {
                        // U�̳�ʼ��
                        //                        memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                        DisplayPrompt(19); // ��������U��
                        if (!UDiskInitialize(2)) {
                            // ��ʼ��ʧ��
                            DisplayPrompt(21); // U������ʧ��
                            EMenuOut(120 + 32 * 9, 340 + 24 * 4 - 7, "Fail   ", 7, 1, 16);
                        } else {
                            DisplayPrompt(20); // U�����ӳɹ�
                            if (WriteEchoDateToFile(2))
                                EMenuOut(120 + 32 * 9, 340 + 24 * 4 - 7, "Success", 7, 1, 16);
                            else
                                EMenuOut(120 + 32 * 9, 340 + 24 * 4 - 7, "Fail   ", 7, 1, 16);

                            DisplayPrompt(24); // ���ڰ�ȫ�Ͽ�
                            FileClose(1);
                            DisplayPrompt(23); // U�̶Ͽ�����
                            //                            memset(&g_UDiskInfo,0,sizeof(UDISK_INFO));
                        }
                    }

                    MSetDisplayColor(COLOR_YELLOW);
                    EMenuOut(162, 2, "Kernel Testing ...      ", 20, 1, 16);
                } else if (key == 5) // I2C
                {
                    char a;
                    EMenuOut(120 + 32 * 9, 340 + 24 * 5 - 7, ".......", 7, 1, 16);
                    ExpendTime(50);
                    BatteryPackRead(0x0B0D, 1, &a);
                    if (a > 0 && a <= 100)
                        EMenuOut(120 + 32 * 9, 340 + 24 * 5 - 7, "Success", 7, 1, 16);
                    else
                        EMenuOut(120 + 32 * 9, 340 + 24 * 5 - 7, "Fail   ", 7, 1, 16);
                }
                // else if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
            }

        } else if (keycode == C_KEYCOD_PLUS && SameKeyTime >= C_SAMEKEYTIME) // 2019.3.25 �����Զ�����
        {
            int iXPos   = 4;
            int iYPos   = 4;
            int iHeight = 30, iKeycode = 0;
            int iCh     = 0;
            int offset1 = 0;
            int offset2 = 0;
            int len     = C_LEN_CHSTATUS + C_LEN_CHPARA + C_LEN_GATEPARA * 2;

            while (1) {
                MFclearScreen();
                TextOut(iXPos, 4 + iHeight * 0, 1, 6, 24, "1ͨ��0", 4);
                TextOut(iXPos, 4 + iHeight * 1, 1, 6, 24, "2ͨ��1", 4);
                TextOut(iXPos, 4 + iHeight * 2, 1, 6, 24, "3ͨ��2", 4);
                if (MGetDefPara(0) == 0)
                    TextOut(iXPos + 100, 4 + iHeight * 0, 1, 6, 24, "Ĭ��", 4);
                else
                    TextOut(iXPos + 100, 4 + iHeight * 0, 1, 6, 24, "�Զ���", 4);

                if (MGetDefPara(1) == 0)
                    TextOut(iXPos + 100, 4 + iHeight * 1, 1, 6, 24, "Ĭ��", 4);
                else
                    TextOut(iXPos + 100, 4 + iHeight * 1, 1, 6, 24, "�Զ���", 4);

                if (MGetDefPara(2) == 0)
                    TextOut(iXPos + 100, 4 + iHeight * 2, 1, 6, 24, "Ĭ��", 4);
                else
                    TextOut(iXPos + 100, 4 + iHeight * 2, 1, 6, 24, "�Զ���", 4);

                while (1) {
                    iKeycode = MGetKeyCode(10);
                    if (iKeycode == 1 || iKeycode == 3 || iKeycode == 2 || iKeycode == C_KEYCOD_RETURN || iKeycode == C_KEYCOD_CONFIRM) {
                        MKeyRlx();
                        break;
                    }
                }
                if (iKeycode == C_KEYCOD_RETURN || iKeycode == C_KEYCOD_CONFIRM)
                    break;

                else {
                    MSetDefPara((MGetDefPara(iKeycode - 1) + 1) % 2, iKeycode - 1);
                    SystemParaStore();
                }
            }

        }

        else if (keycode == 8) {
            extend = 0;

        } else if (keycode == C_KEYCOD_RETURN) {
            // �̰����ؼ�����ʾ�û���Ϣ
            USER_INFO UserInfo;
            int       offset = C_OFF_USER_INFO;
            MCopyProtectedMemory(&UserInfo, (void *)offset, sizeof(USER_INFO), PM_COPY_READ);
            MFclearScreen();

            //	CMenuOut(xpos,ypos,_UserInfo,char_len,row_number+2,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            // for (i=0; i<row_number+2; i++)
            // {
            // TextOut(xpos, ypos+row_height*i-(C_ECHAR_VDOT-4)*MGetLanguage(),  1, 19,row_height, (uint8_t*)_UserInfo[MGetLanguage()][i], 4);
            // }
            TextOut(xpos, ypos + row_height * 0 - (C_ECHAR_VDOT - 4) * MGetLanguage(), 1, 19, row_height, (uint8_t *)_UserInfo[MGetLanguage()][0], 4);
            TextOut(xpos, ypos + row_height * 1 - (C_ECHAR_VDOT - 4) * MGetLanguage(), 1, 19, row_height, (uint8_t *)_UserInfo[MGetLanguage()][1], 4);
            TextOut(xpos, ypos + row_height * 2 - (C_ECHAR_VDOT - 4) * MGetLanguage(), 1, 19, row_height, (uint8_t *)_UserInfo[MGetLanguage()][5], 4);
            para_xpos = xpos + char_len * C_CCHAR_HDOT; /*д������λ��ˮƽ��������*/
            para_ypos = ypos;
            while (1) {
                // ��λ����pzPurchaser[15];
                xpos = para_xpos;
                ypos = para_ypos + row_height * 0;
                CETextOut2(xpos + 1, ypos, UserInfo.pzPurchaser + 1, UserInfo.pzPurchaser[0], 1, 24, 0, 0); /*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                // �������
                ypos = para_ypos + row_height * 1;
                ;
                for (i = 0; i < 8; i++)
                    _serial[i] = InnerInfo.pzSerial[i];
                EMenuOut(xpos - C_ECHAR_HDOT, ypos, _serial, 8, 1, 24);

                /* //��������10\8	PurchaseTime;
                ypos = para_ypos + row_height *2;
                //WriteDate(xpos, ypos+8, &UserInfo.PurchaseTime);

                //ʹ����Ա10 pzUser[5];
                ypos = para_ypos + row_height *3;
                CETextOut2(xpos,ypos,UserInfo.pzUser+1,UserInfo.pzUser[0],1,24,0,0);

                //������Ա10		pzGovernor[5];
                ypos = para_ypos + row_height *4;
                CETextOut2(xpos,ypos,UserInfo.pzGovernor+1,UserInfo.pzGovernor[0],1,24,0,0);	*/

                // ����汾
                ypos = para_ypos + row_height * 2;
                EMenuOut(xpos, ypos, _version1, 9, 1, 12);
                EMenuOut(xpos + 10 * C_ECHAR_HDOT, ypos, _version, 18, 1, 12);

                /*   //�ں˰汾
                  ypos = para_ypos + row_height *6;
  #if C_DEVLIB == 24
                  EMenuOut(xpos, ypos+8, "REL V3.0",8,1,12);
  #elif C_DEVTYPE == 3 || C_DEVTYPE == 9 || C_DEVTYPE == 11
                  EMenuOut(xpos, ypos+8, "RDS V4.0",8,1,12);
  #else
                  EMenuOut(xpos, ypos+8, "N0 V3.0 ",8,1,12);
  #endif
                  ypos = para_ypos + row_height *7;
  #if C_PROCESS == 2
                  EMenuOut(xpos, ypos+8, "RP V2.0 ",8,1,12);
  #elif C_PROCESS == 1
                  EMenuOut(xpos, ypos+8, "NG V1.3 ",8,1,12);
  #endif */
                keycode = MenuKeyReturn(5, 3); /*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
                if (keycode == C_KEYCOD_RETURN) {
                    MCopyProtectedMemory(&UserInfo, (void *)offset, sizeof(USER_INFO), PM_COPY_READ);
                    retvalue = C_FALSE;
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM) {
                    MSetSystem();
                    MCopyProtectedMemory((void *)offset, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
                    CopyMemoryToSystem((void *)offset, C_OFF_BATTERY - C_OFF_USER_INFO, PM_COPY_WRITE);
                    retvalue = C_TRUE;
                    break;
                }

                ypos = para_ypos + row_height * (keycode - 1);

                MSetInputMode(C_INPUT_PINYIN, 0);
                switch (keycode) {
                    case 1: //
                        Write_Space(xpos, ypos, 15, 1);
                        MInputChar(xpos, ypos, 1, UserInfo.pzPurchaser, 20, 10);
                        break;
                    case 2: //
                        Write_Space(xpos, ypos + C_ECHAR_VDOT, 20, 0);
                        MSetInputMode(C_INPUT_LOWERCASE, 0); // ���ó�Сд
                        MInputChar(xpos, ypos, 0, UserInfo.pzSerial, 19, 19);
                        break;
                        // case 3:
                        // InputDate(xpos, ypos, &UserInfo.PurchaseTime);
                        // break;
                        // case 4:
                        // Write_Space(xpos ,ypos,4,1);
                        // MInputChar(xpos, ypos,1,UserInfo.pzUser,8,4);
                        // break;
                        // case 5:
                        // Write_Space(xpos ,ypos,4,1);
                        // MInputChar(xpos, ypos,1,UserInfo.pzGovernor,8,4);
                        // break;
                }
            }
        } else if (keycode == 0) {
            // �̰�0������ʾ�����Ϣ
            /*	��������ʼ8���ֽڴ濪��ʱ�䣬��8�ֽڴ浱ǰʱ��...
                0-7			����ʱ��8
                10-17		�ػ�ʱ��8
                20-31		���ָʾ��11
                36-39		����ʱ��,��λ����
                40-159		��ص��������
                160-164		��ʼ���ʱ��,��λ����
            */
            MFclearScreen();
            uint32_t  ontime;
            uint8_t powerlevel;
            offset = C_OFF_BATTERY + 0;
            MCopyProtectedMemory(&TimeClock, (void *)offset, 8, PM_COPY_READ);
            xpos = 0;
            ypos = 20;
            EMenuOut(xpos, ypos, "Open:", 5, 1, 12);
            WriteDate(xpos + 6 * C_ECHAR_HDOT, ypos, &TimeClock);  // ������ʾ����ʱ��
            WriteTime(xpos + 17 * C_ECHAR_HDOT, ypos, &TimeClock); // ������ʾ�ػ�ʱ��

            offset = C_OFF_BATTERY + 36; // ����ʱ���洢��ַ����λ����
            MCopyProtectedMemory(&ontime, (void *)offset, 4, PM_COPY_READ);
            EMenuOut(xpos + 25 * C_ECHAR_HDOT, ypos, "Elapsed:", 8, 1, 12);
            Write_Number(xpos + 33 * C_ECHAR_HDOT, ypos, ontime, 4, 0, 0);

            for (i = 0; i * 5 < ontime && i < 120; i++) {
                offset = C_OFF_BATTERY + 80 + 2 * i;
                MCopyProtectedMemory(&powerlevel, (void *)offset, 1, PM_COPY_READ);
                if (i % 12 == 0) {
                    xpos = 0;
                    ypos += 24;
                    Write_Number(xpos, ypos, i / 12, 2, 0, 0);
                }
                xpos += 3 * C_ECHAR_HDOT;
                Write_Number(xpos, ypos, powerlevel, 2, 0, 0);
            }
            offset = C_OFF_BATTERY + 10;
            MCopyProtectedMemory(&TimeClock, (void *)offset, 8, PM_COPY_READ);
            xpos = 0;
            ypos = 400;
            EMenuOut(xpos, ypos, "Shut:", 5, 1, 12);
            WriteDate(xpos + 6 * C_ECHAR_HDOT, ypos, &TimeClock);  // ������ʾ����ʱ��
            WriteTime(xpos + 18 * C_ECHAR_HDOT, ypos, &TimeClock); // ������ʾ�ػ�ʱ��
            MAnyKeyReturn();
            break;
        } else if (keycode == 9) {
            // �̰�9���������
            InitPara();
            offset = C_OFF_STACK;
            MCopyProtectedMemory((void *)offset, GainTable, 121 * 4, PM_COPY_WRITE);
        }
        break;
    }
#if C_DEVLIB == 1
    /* set echo display points, notext > 0 for 280, otherwise 308 */
    NotExtendEchoDisplay(extend); // ��ʱȥ��
#endif
    offset = C_OFF_BATTERY;
    GetSystemTime(&TimeClock);
    MCopyProtectedMemory((void *)offset, &TimeClock, 8, PM_COPY_WRITE);

    SysPwd(0);
}
extern const uint16_t _COMM_B1[];
extern CACHE         cache;
const uint8_t         pzText2[] = "transmit start      ";

void Usb2PC(void) {
    int           temp = 0;
    int           byteIn;
    int           byteOut;
    int           i, j, k;
    int           xpos, ypos;
    unsigned int  offset;
    unsigned char out[C_LEN_FILE + C_LEN_FAT];
    unsigned char in[C_LEN_FILE + C_LEN_FAT];

    int File_temp;

    const char _COMM_B1[C_LANGUAGE][3][25] =
        {
            {"������",               "����ʧ��"              },
            {"Connecting,please wait!", "Connection failed         "}
    };
    int file_no, file_total;

    POWER power;

    offset = C_OFF_POWER;
    MCopyProtectedMemory(&power, (void *)offset, sizeof(POWER), PM_COPY_READ);

    for (i = 0; i < 60; i++) {
        byteIn = UsbDevDataIn(in, 1);
        if (byteIn > 0)
            break;
        ExpendTime(4);
    }
    j = 0;
    for (k = 0; k < 100; k++)
        out[k] = '\0';

    byteOut = UsbDevDataOut(out, 64);
    temp += 24;

    xpos = 0;
    ypos = 0;
    MEraseWindow(xpos, ypos, xpos + C_ECHAR_HDOT * 23, ypos + C_ECHAR_VDOT);
    TextOut(xpos, ypos, 1, 23, 24, (uint8_t *)_COMM_B1[MGetLanguage()][0], 0); // MGetLanguage()
    // byteIn=1;
    if (byteIn == 0) {
        MEraseWindow(xpos, ypos, xpos + C_ECHAR_HDOT * 23, ypos + C_ECHAR_VDOT);
        TextOut(xpos, ypos, 1, 23, 24, (uint8_t *)_COMM_B1[MGetLanguage()][1], 0); // MGetLanguage()
        ExpendTime(100);
    }
    while (byteIn > 0) {
        file_no = 0;
        j       = 0;
        ExpendTime(1);
        file_total = DataDelete(-1); // ����Ϊ��������ɾ������ʣ�������������������󷵻أ�1

        for (i = 0, offset = C_OFF_FILEINFO; i < 4; i++, offset++) {
            // �洢���ļ�����
            MCopyProtectedMemory(&byteOut, (void *)offset, 1, PM_COPY_READ);
            out[j] = byteOut;
            j++;
            // while( !UsbDevDataOut(byteOut,1) );
        }
        for (i = 0; i < 4; i++) // 36
        {
            // �ļ�ͷ��С
            byteOut = (C_LEN_FAT_C >> (i * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
            // while( !UsbDevDataOut(byteOut,1) );
        }
        for (i = 0; i < 4; i++) // 403	//808
        {
            // �ļ����С
            byteOut = (C_LEN_FILE_C >> (i * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
            // while( !UsbDevDataOut(byteOut,1) );
        }
        for (k = 0; k < 4; k++) {
            // �ͺ�
            byteOut = (C_DEVTYPE >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }

        for (k = 0; k < 4; k++) {
            // Horiline
            byteOut = (power.Horiline >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }
        for (k = 0; k < 4; k++) {
            // vertline
            byteOut = (power.vertline >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }
        for (k = 0; k < 4; k++) {
            // ResolvingPower
            byteOut = (power.ResolvingPower >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }
        for (k = 0; k < 4; k++) {
            // Sensitivity
            byteOut = (power.Sensitivity >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }
        for (k = 0; k < 4; k++) {
            // Dynamic
            byteOut = (power.Dynamic >> (k * 8)) & 0xff;
            out[j]  = byteOut;
            j++;
        }
        byteOut = UsbDevDataOut(out, 36);
        temp += 24;
        // Write_Number(50,100+temp,byteOut,4,0,0);
        ExpendTime(10);

        temp = 0;

        for (file_no = 0; file_no < file_total; file_no++) {
            // �ļ�ͷ
            File_temp = cache.cache[file_no];
            offset    = C_OFF_FAT + C_LEN_FAT * File_temp;

            MCopyProtectedMemory(&out, (void *)offset, C_LEN_FAT, PM_COPY_READ);
            byteOut = UsbDevDataOut(out, C_LEN_FAT_C);
            temp += 24;
            // Write_Number(80,100+temp,File_temp,4,0,0);
            ExpendTime(2);

            // Write_Number(80,100+temp,byteOut,4,0,0);
        }

        temp = 0;
        xpos += 10 * 16 + 4;
        ypos = 10;
        for (file_no = 0; file_no < file_total; file_no++) {
            File_temp = cache.cache[file_no];
            offset    = C_OFF_FILE + C_LEN_FILE * File_temp;

            MCopyProtectedMemory(&out, (void *)offset, C_LEN_FILE, PM_COPY_READ);

            byteOut = UsbDevDataOut(out, C_LEN_FILE_C);
            temp += 24;
            //  Write_Number(110,100+temp,byteOut,4,0,0);
            ExpendTime(2);
            Write_Ascii(xpos, ypos, '.');
            xpos += 8;
            if (xpos >= 240)
                ypos += 10;
            xpos %= 240;
        }
        break;
    }
}
