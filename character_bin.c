#include "character_bin.h"
#include "globe.h"
/// �ַ�����


const char _us[C_LANGUAGE][1][4] = {
    {"�� "},
    {"�� "},
};
//---------�������ܲ���---------

const uint8_t _Power_prompt[C_LANGUAGE][8][40] = {
    {
    "ˮƽ����                               ",
    "��ֱ����                               ",
    "�ֱ���                                 ",
    "��̬��Χ                               ",
    "����������                             ",
    "��������ָ��                           ",
    "���ز�  ���˳�                         ",
    "��¼���洢��SD��                      	",
    },
    {
    "Horizontal Linearity                   ",
    "Vertical Linearity                     ",
    "Resolution                             ",
    "Dynamic Range                          ",
    "Surplus Sensitivity                    ",
    "Detector Performance                   ",
    "�� Redo �� Exit                        ",
    "Record is stored in sd                 ",
    }
};

const uint16_t _Power_prompt3[] = {
    '1',
    ' ',
    '#',
    ' ',
    ENTER,
    ' ',
};
const uint16_t _Power_prompt2[] = {
    '1', CH(2082), CH(5101),
    '#', CH(5456), CH(1866),
    ENTER, CH(4543), CH(1986),
};
/*
ˮƽ����
��ֱ����
�ֱ���
��̬��Χ
����������

1.��ӡ
2.�ز�
3.�˳�
*/

const char _Menu_Power[C_LANGUAGE][5][90] = {
    {
        "��ֱ̽ͷ����25mm���Կ��ϣ��ز��ȶ���ȷ��                        ",                     //                     ˮƽ����
        "���ȥ̽ͷ��ȷ��                                                ",                     //                     ����������
        "�뽫̽ͷ�����Կ��ϵ���85��91���ز��ȸ����ȶ�ʱ��ȷ������ס̽ͷ    ", //                                                                          �ֱ���
        "��CS-1-5�Կ����ƶ�̽ͷ��200mm ����2ƽ�׿׻ز����ʱ��ȷ������ס̽ͷ", // ��ֱ����
        "���ȥ̽ͷ��ȷ��                                                ", // ����������
    },
    {
        "Put the normal probe  on the 25mm thick testblock and keep it stable,press Enter        ", //                  0ˮƽ����
        "Remove the probe, press Enter    ", //                                                            1����������
        "The heights of 85 and 91mm depth echoes are all 50%, Press Enter                        ",                                               // 2�ֱ���
        "The highest echo of   200mm deep,��2 flat bottomed hole is 50%, press Enter              ", // 3��ֱ����
        "Remove the probe, press Enter    ",                                                        // 4����������
    }
};
const uint8_t  _Menu_Power1E[] = { "\
Put the normal probe on the \
25mm thick test block and   \
keep it stable,press Enter  "
};
const uint16_t _Menu_Power1[] = {
    CH(2911), CH(5417), CH(4429), CH(4523), CH(2337), CH(5258), '2', '5', 'm', 'm', CH(2681), CH(4252), CH(3173),
    CH(4147), ',', ' ', CH(2756), CH(1808), CH(4640), CH(2208), CH(2683), ENTER, CH(4023), CH(4047), CH(100),
};
/*
��ֱ̽ͷ����25mm���Կ�
�ϣ��ز��ȶ���ENȷ��
*/

const uint16_t _Menu_Power2[] = {CH(1620), ENTER, CH(2892), CH(4023), CH(4047)};
/* ��ȷ�ϼ�ȷ��*/

const uint16_t _Menu_Power3[] = {
    CH(3975), CH(2911), CH(4429), CH(4523), CH(5435), CH(5158), CH(4252), CH(3173), CH(4147),
    CH(2117), CH(4178), '8', '5', CH(2645), '9', '1', CH(3329), CH(2756), CH(1808), CH(2140), CH(2463), CH(3950),
    CH(4640), CH(2208), CH(4217), ENTER, CH(4023), CH(4047), CH(1802), CH(4640), CH(5501), CH(4429), CH(4523), CH(100), CH(100),
};
/*�ֱ���
�뽫̽ͷ�����Կ���
����85��91���ز��ȸ���
�ȶ�ʱ()ȷ�ϲ���ס̽ͷ
*/

const uint16_t _Menu_Power4[] =
    {
        CH(5258), 'C', 'S', '-', '1', '-', '5', CH(4252), CH(3173), CH(4147), CH(5038), CH(2215), CH(4429), CH(4523),
        CH(2117), '2', '0', '0', 'm', 'm', ' ', CH(2006), 136, '2', CH(3829), CH(2155), CH(3155), CH(2756), CH(1808),
        CH(5578), CH(2463), CH(4217), ENTER, CH(4023), CH(4047), CH(1802), CH(4640), CH(5501), CH(4429), CH(4523)};

const uint16_t _Menu_Power5[] =
    {CH(3975), CH(1993), CH(4005), CH(4429), CH(4523), CH(2683), ENTER, CH(4023), CH(4047)};
/*��ֱ����
��CS-1-5�Կ����ƶ�̽ͷ
��200mm ��()2ƽ�׿׻ز�
���ʱ()ȷ�ϲ���ס̽ͷ

����������
���ȥ̽ͷ��ENȷ��
*/

const uint8_t _Menu_Power3E[] = {"\
The heights of 85 and 91mm\
depth echoes are all 50%, \
Press Enter               "};
/*�ֱ���
�뽫̽ͷ�����Կ���
����85��91���ز��ȸ���
�ȶ�ʱ()ȷ�ϲ���ס̽ͷ
*/
const uint8_t _Menu_Power4E[] = {"\
The highest echo of 200mm \
deep, 2 flat-bottomed     \
hole is 50%, press Enter  "};
const uint8_t _Menu_Power5E[] = {"Remove the probe, press Enter    "};

/*��ֱ����
��CS-1-5�Կ����ƶ�̽ͷ
��200mm ��()2ƽ�׿׻ز�
���ʱ()ȷ�ϲ���ס̽ͷ

����������
���ȥ̽ͷ��ENȷ��
*/
const char _ParaName[C_LANGUAGE][21][16] =
    {
        {
         "��������        ",
         "���油��        ",
         "ɨ������        ",
         "��ֱ����        ",
         "ˮƽ����        ",
         "��������        ",
         "��������        ",
         "̽ͷ���        ",
         "��ֱ��ʱ        ",
         "ˮƽ��ʱ        ",
         "������ʱ        ",
         "����            ",
         "����Ƕ�        ",
         "����λ          ",
         "���ſ�          ",
         "���Ÿ�          ",
         "���Ÿ�          ",
         "����λ          ",
         "���ſ�          ",
         "���Ÿ�          ",
         },
        {
         "Basic Gain      ",
         "Surface Gain    ",
         "Scanning Gain   ",
         "Vertical Range  ",
         "Horizontal Range",
         "Distance Range  ",
         "Sound Vel.      ",
         "Probe Zero      ",
         "Vertical Delay  ",
         "Horizontal Delay",
         "Distance Delay  ",
         "Reject          ",
         "Angle           ",
         "Gate A Start    ",
         "Gate A Width    ",
         "Gate A Level    ",
         "Gate A Level    ",
         "Gate B Start    ",
         "Gate B Width    ",
         "Gate B Level    ",
         }
};

// ��¼����
const uint16_t _Report_Info1[] =
    {
        CH(217),
        CH(4017),
        CH(4761),
        CH(1904),
        CH(2240),
        CH(326),
        CH(218),
        CH(4017),
        CH(4761),
        CH(2463),
        CH(2240),
        CH(326),
        CH(219),
        CH(2504),
        CH(2894),
        CH(4837),
        CH(4702),
        CH(326),
        CH(220),
        CH(2876),
        CH(4973),
        CH(2105),
        CH(4627),
        CH(326),
        CH(221),
        CH(4615),
        CH(4548),
        CH(2105),
        CH(4627),
        CH(326),
};
/*	1.ȱ�ݳ���:
    2.ȱ�ݸ߶�:
    3.������Ϣ:
    4.���鵥λ:
    5.ί�е�λ:
*/

const uint16_t _Report_Info2[] =
    {
        CH(217), CH(3153), CH(1655),
        CH(218), CH(4268), CH(4075),
        CH(219), CH(1866), CH(3331)};

/*	1.�հ�
    2.����
    3.����
*/

const uint16_t _Report_Info3[] =
    {
        CH(217), CH(4620), CH(4268), CH(4075),
        CH(218), CH(5049), CH(4268), CH(4075)};
/*
1.δ����
2.������
*/
const uint16_t _Report_Info32[] =
    {
        CH(217), CH(1827), CH(4268), CH(4075),
        CH(218), CH(4268), CH(4075), CH(100)};

const uint16_t _Report_Info31[] =
    {
        CH(217), CH(2504), CH(2894), CH(3591), CH(1938), CH(326),
        CH(218), CH(2504), CH(2894), CH(1764), CH(2637), CH(326),
        CH(219), CH(2504), CH(2894), CH(2570), CH(2481), CH(326),
        CH(220), CH(1836), CH(3347), CH(3738), CH(2637), CH(326),
        CH(221), CH(1777), CH(3570), CH(5520), CH(3186), CH(326)};
/*	1.�������ƣ�
    2.������ţ�
    3.�������
    4.�����ƺţ�
    5.����״����
*/

/*
const uint16_t _Report_Info4[] = {
CH(217),CH(2876),CH(4973),CH(1831),CH(4627),CH( 326),
CH(218),CH(2876),CH(4973),CH(1740),CH(3293),CH( 326),
CH(219),CH(2876),CH(4973),CH(4217),CH(2790),CH( 326),
CH(220),CH(4973),CH(4253),CH(1774),CH(5528),CH( 326),
CH(221),CH(2647),CH(2481),CH(2822),CH(1780),CH( 326),
};
1.���鲿λ:
2.�������:
3.����ʱ��:
4.���ձ�׼:
5.�ϸ񼶱�:
*/

const char _Menu_DB[C_LANGUAGE][4][28] =
    {
        {
         "�������Կ�����",         // 2
            "������̽ͷƵ��",         // 3
            "̽ͷƵ�ײ�����",         // 4
            "Ƶ�ʣ�     ���ؼ��˳� ", // 5
        },
        {
         "Please input piece range:   ",
         "Please input frequency:     ",
         "Frequency testing           ",
         "Frequency��     ��exit      ",
         },
};

const char _TestMenu_A1[C_LANGUAGE][8][13] =
    {
        {
         "����������� ",
         "��������Ƕ� ",
         "�������ģ��� ",
         "���������֣� ",
         "������������ ",
         "����ȱ�ݸ߶� ",
         "����̽ͷƵ�� ",
         "����ͼ� ",
         },
        {
         "1.Zero Test  ",
         "2.Angle Test ",
         "3.DAC Test   ",
         "4.AVG Test   ",
         "5.Performance",
         "6.Flaw High  ",
         "7.Spectrum   ",
         "8.Forging   ",
         },
};

const uint8_t _Menu_Choice[C_LANGUAGE][10][22] =
    {
        {
         "������ͨѶ  ",
         "�����ܲ˵�",
         "�����ò˵�",
         "��ѡ��˵�",
         "��������У",
         "�����ݴ���",
         "����׼����",
         "��ɫ������",
         "������dB��",
         "���������",

         }, //  ����
        {
         "0.Communication       ",
         "1.Function Menu       ",
         "2.Setup Menu          ",
         "3.Option Menu         ",
         "4.Test                ",
         "5.Data Storage        ",
         "6.Standard            ",
         "7.Color Menu          ",
         "8.Amplitude dB        ",
         "9.Hotkey Setup        ",

         }, //  Ӣ��
};
const char _AmplitudedB[C_LANGUAGE][2][30] =
    {
        {
         "���mm  ����dB  ����%",
         "10mm",
         },
        {
         "Deep(mm)Gain(dB)High(%)",
         "10mm  ",
         },
};
const char _TestMenu_Menu[C_LANGUAGE][2][13] =
    {
        {
         "���Զ�����",
         "���ֶ�����",
         },
        {
         "1.Auto    ",
         "2.Manual  ",
         },
};
const uint8_t _TestMenu_Enter[] = {ENTER};

const uint8_t _TestMenu_AB1[C_LANGUAGE][4][17] =
    {
        {
         "��Ԥ�ù������٣� ",
         "��һ�λز����̣� ",
         "�����λز����̣� ",
         "�������������   ",
         }, //  ����
        {
         "1.Sound Velocity:",
         "2.1st BeamPath  :",
         "3.2nd BeamPath  :",
         "��to start Test  ",
         }, //  Ӣ��
};
/*
1.Ԥ�ù�������:
2.һ�λز����̣�
3.���λز����̣�
�� 'z'+ 15 ��ʼ����
*/
const uint8_t _TestMenu_AB3[C_LANGUAGE][4][85] =
    {
        {
         "�ƶ�̽ͷʹ      ��������߻ز�  ������, ��ȷ��  ",
         "̽ͷ��һ�η�����ˮƽ���룺                      ",
         "ʹ�������      ��������߻ز�  ������, ��ȷ��  ",
         "��ס̽ͷʹ      ������ز�      ������, ��ȷ��  ",
         },
        {
         "Move the probe,  make the highest echo with        in gate.         And then press ��",
         "The distance fromprobe to 1st     reflector:                                         ",
         "Make the highest echo with the center depth        in gate.         And then press ��",
         "keep the echo with the center depth                in gate.         And then press ��",
         }
};

const uint8_t _TestForging[C_LANGUAGE][9][95] =
    {
        {
         "Ѱ����ȱ��ʱ�ײ���߲�BG,�Զ�������       ,��ȷ�� ",
         "�ƶ�����,Ѱ��ȱ��",
         "����߲�,�Զ�����",
         "��     ,��ȷ��",
         "�ƶ�����,Ѱ����ȱ��",
         "ʱ�ײ���߲�BF,�Զ�",
         "������     ,��ȷ�� ",
         "����ȱ�ݺ�����Xֵ��        mm",
         "����ȱ�ݺ�����Yֵ��        mm",
         },
        {
         "Find the highest ofbottom wave when   there's no defect, gain it to          ,and then press ��",
         "Move the gate,look forthe highest of the    defect wave,and gain  to        ,and confirm ",
         "Move the wave gate,look for the highest of the defect wave,and gain to      , and confirm ",
         "Move the wave gate,look for the highest of the defect wave,and gain to      , and confirm ",
         "Move the gate,find thehighest of defective  wave ,and gain to      , and confirm  ",
         "Move the gate,find the highest of the defective wave ,and gain to      , and confirm  ",
         "Move the gate,find the highest of the defective wave ,and gain to      , and confirm  ",
         "x of defect abscissa :      ",
         "y of defect abscissa :      ",
         }
};

const uint8_t _TestMenu_AC1[C_LANGUAGE][4][18] =
    {
        {
         "��Ŀ�귴����ֱ����",
         "��������������ȣ�",
         "�����K ֵ����ǣ�",
         "�������������    ",
         }, //  ����
        {
         "1.ObjectDiameter: ",
         "2.Object Depth  : ",
         "3.Marking Angle : ",
         "��to start test   ",
         }, //  Ӣ��
};
const uint8_t _TestMenu_DAC[C_LANGUAGE][5][17] =
    {
        {
         "��  �����ȣ�   ",
         "��������ֱ����   ",
         "�������峤�ȣ�   ",
         "�������������   ",
#if C_DEVTYPE == 14
         "�������ԡ������� ",
#else
            "�������ԡ������� ",
#endif
         }, //  ����
        {
         "1.Maximum Range :",
         "2.ObjectDiameter:",
         "3.Object Length :",
         "��to start draw  ",
#if C_DEVTYPE == 14
         "��Draw,��Correct ",
#else
            "��Draw,��Correct ",
#endif
         }, //  Ӣ��
};

const uint8_t _TestMenu_AVG[C_LANGUAGE][5][17] =
    {
        {
         "��  �����ȣ�   ",
         "��������ֱ����   ",
         "��  ������ʾ��   ",
         "�������������   ",
#if C_DEVTYPE == 14
         "�������ԡ������� ",
#else
            "�������ԡ������� ",
#endif
         }, //  ����
        {
         "1.Maximum Range :",
         "2.ObjectDiameter:",
         "3.Equiv. Display:",
         "��to start draw  ",
#if C_DEVTYPE == 14
         "��Draw,��Correct ",
#else
            "��Draw,��Correct ",
#endif
         }, //  Ӣ��
};

const char _Menu_FJ[C_LANGUAGE][12][40] =
    {
        {
         "�����빤�����",                      //     0
            "��ȱ��ʱ��ƽ�׵ײ�����BG=      dB", // 1
            "�������T=       mm", // 2
            "��ƽ���릵2ƽ�׿׵ĵ�����dB1=      dB", // 3
            "̽��������BG+dB1=      dB", // 5
            "ȱ�ݲ�80%ʱ����dB2=      dB",
         "ȱ�����h=       mm",
         "ȱ�ݿ׾�D2=     mm",
         "ȱ�ݺ�����X=       mm",
         "ȱ��������Y=       mm",
         "�൱�ڦ�4ƽ�׿׵ĵ���ֵ        dB",
         "��ȱ��ʱ�ײ�������BF=       dB",
         },
        {
         "Please input Workpiece thickness:      ",
         "Large flat bottom wave gain BG:      dB",
         "Workpiece thickness T:       mm        ",
         "Differ between bottom and hole dB1:    ",
         "flaw detection sensitivity BG+dB1:      ",
         "Gain at 80% wave defect dB2:      dB     "
            "Defect depth h:       mm               ",
         "Defect depth h:       mm               ",
         "Defect aperture D2:      mm            ",
         "Flaw abscissa x:        mm            ",
         "Flaw abscissa y:        mm             ",
         "Equivalent of ��4 bottom hole:      dB  ",
         "Gain of bottom with defects BF:     dB ",
         },
};

const uint8_t _TestMenu_set[C_LANGUAGE][3][28] =
    {
        {
         "�����油����",
         "��������ȣ�",
         "�����ؤ����",
         }, //  ����
        {
         "1.Compensation: ",
         "2.Thickness   : ",
         "��Return��Finish",
         }, //  Ӣ��
};

const uint8_t _Forging_A1[C_LANGUAGE][2][34] =
    {
        {
         "����ȱ�ݺ�����Xֵ��        mm",
         "����ȱ��������Yֵ��        mm",
         }, //  ����
        {
         "X of  abscissa:        mm",
         "Y of  ordinate:        mm",
         }, //  Ӣ��
};
const uint8_t _Forging_A2[C_LANGUAGE][2][38] =
    {
        {
         "��δ����ȱ��,���ؼ��˳�",
         }, //  ����
        {
         "If no defect is found, the back exits.",
         }, //  Ӣ��
};

const uint8_t _TestMenu_AD2[C_LANGUAGE][4][34] =
    {
        {
#if C_DEVTYPE == 14
         "���Ҽ�ѡ��Ŀ��ز�����������", // 2
            "���Ҽ�ѡ��ز���ȷ��  ������", // 3
            "����ѡ�� ������ ���˳� �����", // 5
            "����ѡ�����µ�������������", // 6
#else
            "������ѡ��Ŀ��ز�����������",
            "������ѡ��ز���ȷ��  ������",
            "����ѡ�� ������ ���˳� �����",
            "��ѡ�� �������� ����� �����",
#endif
        }, //  ����
        {
#if C_DEVTYPE == 14
         "����Select Echo ��Exit	           ",
         "����Select Echo ��Confirm��Return ",
         "����Select��Correct��exit��Finish ",
         "����Select�ġ�Adjust��Del��Finish ",
#else
            "����Select Echo ��Exit			  ",
            "����Select Echo��Confirm��Return ",
            "����Select��Correct��Exit��Finish ",
            "��Select ����Adjust ��Del��Finish ",
#endif
         }, //  Ӣ��
};

const uint16_t _TestMenu_AF1[] =
    {
        CH(217),
        CH(2320),
        CH(4168),
        CH(4469),
        CH(4178),
        CH(2240),
        CH(326),
        CH(218),
        CH(2789),
        CH(5528),
        CH(1808),
        CH(2463),
        CH(2240),
        CH(326),
        CH(1620),
        ENTER,
        CH(2892),
        CH(3110),
        CH(4228),
        CH(1866),
        CH(4252),
}; /*
 //1. У�����ͣ���У/��У
 1.��������ȣ�
 2.��׼���߶ȣ�
 ��'z'+ 15 ��ʼУ��

   */
const uint8_t _TestMenu_AG1[C_LANGUAGE][1][16] =
    {
        {
         "��ȱ�ݸ߶�      ",
         },
        {
         "Flaw height test",
         }
};

const uint8_t _TestMenu_AG2[C_LANGUAGE][3][73] =
    {
        {
         "��̽ͷ������ʹȱ���¶� ��߲�����������",
         "��̽ͷ������ʹȱ���϶� ��߲�����������",
         "���ȱ�ݸ߶�Ϊ��       �Ƿ�洢���Σ��٣���",
         },
        {"Move the probe,make the highest echo of the flawlower tip in gate.       ",
         "Move the probe,make the highest echo of the flawupper tip in gate.       ",
         "The flaw height        save the echo wave? Y/N"}
};

const char EpDots[] =
    {
        0x00, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0E, 0x1E,
        0x1E, 0x1E, 0x3E, 0x3E, 0x3E, 0x7E, 0x7E, 0x7E,
        0x07, 0x3F, 0xFF, 0xCF, 0x0F, 0x0F, 0x0F, 0x0F,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,

        0xC0, 0xF8, 0xFE, 0xE7, 0xE1, 0xE0, 0xE0, 0xE0,
        0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
        0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xF0,
        0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC,

        0x7E, 0x7E, 0x3E, 0x3E, 0x3E, 0x1E, 0x1E, 0x1E,
        0x0E, 0x0E, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x00,
        0x00, 0x00, 0xC0, 0xF8, 0xFF, 0x3F, 0x07, 0x00,

        0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x00,
        0x00, 0x01, 0x07, 0x3F, 0xFE, 0xF8, 0xC0, 0x00,
        0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xF0,
        0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00};

const uint16_t company[] = {CH(5149), CH(3310)}; // ����

const uint16_t company2[] = {CH(3647), CH(4508), CH(5149), CH(3310), CH(4293),
                             CH(3475), CH(2828), CH(4285), CH(3110), CH(2302), CH(5148), CH(4762), CH(2511), CH(4330)};
// ��ͨ�������뼼���������޹�˾

const uint16_t product[] = {CH(348), CH(356), CH(353), CH(352), CH(313), CH(345), CH(317)};

const char Probe[] =
    {
        0x00, 0x00, 0x07, 0x04, 0x04, 0x04, 0x04, 0x3f,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xf0, 0x10, 0x10, 0x10, 0x10, 0xfe,
        0x80, 0x80, 0x80, 0xc0, 0xc0, 0x80, 0x00, 0x00,

        0x00, 0x00, 0x07, 0x04, 0x04, 0x04, 0x04, 0x3f,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xf0, 0x10, 0x10, 0x10, 0x10, 0xfe,
        0x00, 0x80, 0x40, 0x20, 0x18, 0x18, 0x00, 0x00,

        0x00, 0x00, 0x3e, 0x22, 0x22, 0x22, 0x7f, 0x10,
        0x08, 0x04, 0x02, 0x01, 0x07, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x3e, 0x22, 0x22, 0x22, 0x7f, 0x08,
        0x10, 0x20, 0x40, 0x80, 0xe0, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x03, 0x02, 0x02, 0x0f, 0x00, 0x00,
        0x00, 0x0f, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xe0, 0x20, 0x20, 0xf8, 0x80, 0x80,
        0x80, 0xf8, 0x20, 0x20, 0xe0, 0x00, 0x00, 0x00};
const uint8_t _ParaMenu_H1[C_LANGUAGE][1][40] =
    {
        {"          ��  ҳ  ��  ҳ     "},
        {"Current page   Total page    "}};

const uint8_t _ChannelMenu_A1[C_LANGUAGE][9][22] =
    {
        {
         "��ͨ����׼��",
         "��̽ͷ���ͣ�",
         "��̽ͷƵ�ʣ�",
         "����Ƭ�ߴ磺",
         "������Ƕȣ�",
         "���������٣�",
         "��̽ͷǰ�أ�",
         "���˲�Ƶ����",
         "���첨��ʽ��",
         }, //  ����
        {
         "1.Standard in Channel ",
         "2.Probe Type          ",
         "3.Frequency           ",
         "4.Crystal Size        ",
         "5.Angle               ",
         "6.Sound Velocity      ",
         "7.Front Distance      ",
         "8.Filter Band         ",
         "9.Echo Rectify        ",
         }, //  Ӣ��
};
const uint8_t _ChannelMenu_A2[C_LANGUAGE][6][22] =
    {
        {
         "��̽ͷ���᣺",
         "�������ѹ��",
         "���ظ�Ƶ�ʣ�",
         "�����������",
         "�������ȣ�",
         "���û�������",
         }, //  ����
        {
         "1.Damping             ",
         "2.Transmit Voltage    ",
         "3.Repetition Frequency",
         "4.Denoise             ",
         "5.Pulse Breadth       ",
         "6.User Parameter      ",
         }, //  Ӣ��
};

const uint8_t _ChannelMenu_A1S[C_LANGUAGE][9][30] =
    {
        {
         "��Ĭ�ϱ�׼��",
         "�������ѹ��",
         "��Ĭ��̽ͷ���ͣ�",
         "��б̽ͷĬ��Ƶ�ʣ�",
         "��б̽ͷĬ�Ͼ�Ƭ�ߴ磺",
         "��ֱ̽ͷĬ��Ƶ�ʣ�",
         "��ֱ̽ͷĬ�Ͼ�Ƭ�ߴ磺",
         "��˫��ֱ̽ͷĬ��Ƶ�ʣ�",
         "��˫��ֱ̽ͷĬ�Ͼ�Ƭ�ߴ磺",
         }, //  ����
        {
         "1.Standard in Channel ",
         "2.Standard in Channel ",
         "3.Standard in Channel ",
         "4.Standard in Channel ",
         "5.Standard in Channel ",
         "6.Standard in Channel ",
         "7.Standard in Channel ",
         "8.Standard in Channel ",
         "9.Standard in Channel ",
         }, //  Ӣ��
};

const uint8_t _ChannelMenu_A2S[C_LANGUAGE][8][50] =
    {
        {
         "��������㡢�Ƕ�ʱ����������Զ�������",
         "��������㡢�Ƕ�ʱ���ز������Զ�������",
         "��б̽ͷ�����һ�λز�Ĭ�ϣ�",
         "��б̽ͷ�������λز�Ĭ�ϣ�",
         "��ֱ̽ͷ�����һ�λز�Ĭ�ϣ�",
         "��ֱ̽ͷ�������λز�Ĭ�ϣ�",
         "��˫��ֱ̽ͷ�����һ�λز�Ĭ�ϣ�",
         "��˫��ֱ̽ͷ�������λز�Ĭ�ϣ�",
         }, //  ����
        {
         "1.Standard in Channel ",
         "2.Standard in Channel ",
         "3.Standard in Channel ",
         "4.Standard in Channel ",
         "5.Standard in Channel ",
         "6.Standard in Channel ",
         "7.Standard in Channel ",
         "8.Standard in Channel ",
         }, //  Ӣ��
};

const uint8_t _ChannelMenu_A3S[C_LANGUAGE][7][50] =
    {
        {
         "����Ƕ�Ĭ�Ϸ�����ֱ����",
         "����Ƕ�Ĭ�Ϸ�����������ȣ�",
         "�����Kֵ����ǣ�",
         "��б̽ͷ����DAC���߱��油����",
         "��ֱ̽ͷ����DAC���߱��油����",
         "��˫��ֱ̽ͷ����DAC���߱��油����",
         "��DAC����ĩ��ֱ����ʾ��",
         }, //  ����
        {
         "1.Standard in Channel ",
         "2.Standard in Channel ",
         "3.Standard in Channel ",
         "4.Standard in Channel ",
         "5.Standard in Channel ",
         "6.Standard in Channel ",
         "7.Standard in Channel ",
         }, //  Ӣ��
};

const uint8_t _ChannelMenu_A4S[C_LANGUAGE][9][50] =
    {
        {
         "��б̽ͷ����DAC����Ĭ�������ȣ�",
         "��б̽ͷ����DAC����Ĭ�Ϸ�����ֱ����",
         "��б̽ͷ����DAC����Ĭ�Ϸ����峤�ȣ�",
         "��ֱ̽ͷ����DAC����Ĭ�������ȣ�",
         "��ֱ̽ͷ����DAC����Ĭ�Ϸ�����ֱ����",
         "��ֱ̽ͷ����DAC����Ĭ�Ϸ����峤�ȣ�",
         "��˫��ֱ̽ͷ����DAC����Ĭ�������ȣ�",
         "��˫��ֱ̽ͷ����DAC����Ĭ�Ϸ�����ֱ����",
         "��˫��ֱ̽ͷ����DAC����Ĭ�Ϸ����峤�ȣ�",
         }, //  ����
        {
         "1.Standard in Channel ",
         "2.Standard in Channel ",
         "3.Standard in Channel ",
         "4.Standard in Channel ",
         "5.Standard in Channel ",
         "6.Standard in Channel ",
         "7.Standard in Channel ",
         "8.Standard in Channel ",
         "9.Standard in Channel ",
         }, //  Ӣ��
};

const uint8_t _ChannelMenu_B1[] =
    {
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        'M', 'H', 'z',
        ' ', ' ', ' ',
        'm', '/', 's',
        'm', 'm', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' '};
const uint8_t _ChannelMenu_B2[] =
    {
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' '};

const uint8_t _ParaMenu_B1[] =
    {
        ' ', ' ', ' ',
        'M', 'H', 'z',
        ' ', ' ', ' ',
        ' ', ' ', 130,
        ' ', ' ', ' ',
        'm', '/', 's',
        'm', 'm', ' ',
        'd', 'B', ' ',
        ' ', ' ', ' '};

const uint8_t _ParaMenu_AB1[C_LANGUAGE][3][10] =
    {
        {
         "����ֱ    ",
         "��ˮƽ    ",
         "������    ",
         }, //  ����
        {
         "1.V-Range ",
         "2.H-Range ",
         "3.D-Range ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AB2[C_LANGUAGE][5][14] =
    {
        {
         "��ֱ̽ͷ  ",
         "��б̽ͷ  ",
         "��˫���ݲ�ֱ  ",
         "����͸  ",
         "��˫���ݲ�б  ",
         }, //  ����
        {
         "1.Normal  ",
         "2.Angle   ",
         "3.Dual direct  ",
         "4.Through ",
         "5.Dual oblique ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_AB3[] = {"\
               \
1.Epson LQ300K \
2.HP 6L        \
3.Epson C61    \
4.TP UP-NH32P  "};

const uint8_t _ParaMenu_A2[C_LANGUAGE][9][22] =
    {
        {
         "�����̱�ȣ�",
         "�����油����",
         "��������ȣ�",
         "�������⾶��",
         "��������׼��",
         "��������ʾ��",
         "���з���RL��",
         "��������SL��",
         "��������EL��",
         }, //  ����
        {
         "1.Scale               ",
         "2.Surface Gain        ",
         "3.Workpiece Thickness ",
         "4.Outer Radius        ",
         "5.Equiv. Reference    ",
         "6.Equiv. Display      ",
         "7.Rejection           ",
         "8.Sizing              ",
         "9.Evalution           ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_A22[C_LANGUAGE][9][22] =
    {
        {
         "�����̱�ȣ�",
         "�����油����",
         "��������ȣ�",
         "�������⾶��",
         "��������׼��",
         "��������ʾ��",
         "�����֣��ϣ�",
         "�����֣��У�",
         "�����֣��£�",
         }, //  ����
        {
         "1.Scale               ",
         "2.Surface Gain        ",
         "3.Workpiece Thickness ",
         "4.Outer Radius        ",
         "5.Equiv. Reference    ",
         "6.Equiv. Display      ",
         "7.AVG Top             ",
         "8.AVG Middle          ",
         "9.AVG Bottom          ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_B2[] =
    {
        ' ', ' ', ' ',
        'd', 'B', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        'd', 'B', ' ',
        'd', 'B', ' ',
        'd', 'B', ' '};

const uint8_t _ParaMenu_B2A1[] =
    {
        ' ', ' ', ' ',
        'd', 'B', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        'm', 'm', ' ',
        'm', 'm', ' ',
        'm', 'm', ' '};

const uint8_t _ParaMenu_AC1[C_LANGUAGE][2][8] =
    {
        {
         "������  ",
         "������  ",
         }, //  ����
        {
         "1.Single",
         "2.Cont. ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC7[C_LANGUAGE][2][10] =
    {
        {
         "������    ",
         "��SD��    ",
         }, //  ����
        {
         "1.Detector",
         "2.SD      ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC2[C_LANGUAGE][7][10] =
    {
        {
         "��ĸ��    ",
         "���з���  ",
         "��������  ",
         "��������  ",
         "���ĺ���  ",
         "�������  ",
         "��������  ",
         }, //  ����
        {
         "1.Original",
         "2.RL      ",
         "3.SL      ",
         "4.EL      ",
         "5.4th     ",
         "6.5th     ",
         "7.6th     ",
         }, //  Ӣ��
};

const uint8_t _infinity[C_LANGUAGE][1][7] =
    {
        {
         "��     ",
         }, //  ����
        {
         "��     ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC3A[C_LANGUAGE][2][8] =
    {
        {
         "�����֣�",
         "������  ",
         }, //  ����
        {
         "1.AVG   ",
         "2.Equiv.",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC3[C_LANGUAGE][2][8] =
    {
        {
         "�ģ�ֵ��",
         "�׾�  ",
         }, //  ����
        {
         " DB     ",
         "      ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC4[C_LANGUAGE][5][10] =
    {
        {
         "������    ",
         "������    ",
         "����׼    ",
         "������    ",
         "������    ",
         }, //  ����
        {
         "1.Low     ",
         "2.Middle  ",
         "3.Standard",
         "4.High    ",
         "2.High    ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC5[C_LANGUAGE][2][10] =
    {
        {
         "��ʧ����  ",
         "��������  ",
         }, //  ����
        {
         "1.Negative",
         "2.Positive",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AC6[C_LANGUAGE][2][8] =
    {
        {
         "����ֵ  ",
         "��ǰ��  ",
         }, //  ����
        {
         "1.Peak  ",
         "2.Front ",
         }, //  Ӣ��
};
const uint8_t _MainMenu_C1A11[C_LANGUAGE][2][16] =
    {
        {
         "������  ",
         "��English ",
         }, //  ����
        {
         "������  ",
         "��English ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_A3[C_LANGUAGE][9][22] =
    {
        {
         //#if C_UDISK == 1
            "����¼��ʽ��",
         "����¼λ�ã�",
         // #else
            //		"��������λ��",
            //		"������Lang��",
            // #endif
            "��λ����ʾ��",
         "������Lang��",
         "��������;��",
         "������������",
         "����Ļ���ȣ�",
         "����׼���ߣ�",
         "������ͼʾ��",
         }, //  ����
        {
         "1.Save Mode           ",
         "2.Save Location       ",
         "3.Detection Method    ",
         "4.Language ����       ",
         "5.Gate B Usage        ",
         "6.Keypad Beep         ",
         "7.Screen Luminance    ",
         "8.Reference Height    ",
         "9.Weld Seam Sketch    ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_A5[C_LANGUAGE][2][28] =
    {
        {
         "��������ȣ�",
         "�������⾶��",
         }, //  ����
        {
         "1.Thickness   : ",
         "2.Outer radius:  ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_A6[C_LANGUAGE][8][22] =
    {
        {
         "�����̱�ȣ�",
         "�����油����",
         "��������ȣ�",
         "�������⾶��",
         "��������׼��",
         "���з���RL��",
         "��������SL��",
         "��������EL��",
         }, //  ����
        {
         "1.Scale               ",
         "2.Surface Gain        ",
         "3.Workpiece Thickness ",
         "4.Outer Radius        ",
         "5.Equiv. Reference    ",
         "6.Rejection           ",
         "7 Sizing              ",
         "8.Evalution           ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_A6A1[C_LANGUAGE][8][22] =
    {
        {
         "�����̱�ȣ�",
         "�����油����",
         "��������ȣ�",
         "�������⾶��",
         "��������׼��",
         "�����֣��ϣ�",
         "�����֣��У�",
         "�����֣��£�",
         }, //  ����
        {
         "1.Scale               ",
         "2.Surface Gain        ",
         "3.Workpiece Thickness ",
         "4.Outer Radius        ",
         "5.Equiv. Reference    ",
         "6.AVG Top             ",
         "7.AVG Middle          ",
         "8.AVG Bottom          ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_B6[] =
    {
        ' ', ' ', ' ',
        'd', 'B', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        'd', 'B', ' ',
        'd', 'B', ' ',
        'd', 'B', ' '};

const uint8_t _ParaMenu_B6A1[] =
    {
        ' ', ' ', ' ',
        'd', 'B', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        'm', 'm', ' ',
        'm', 'm', ' ',
        'm', 'm', ' '};

const uint8_t _ParaMenu_B3[] =
    {
        ' ',
        ' ',
        ' ', //'m','m',' ',
        ' ',
        ' ',
        ' ', //'m','m',' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        '%',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
};
#if C_DEVLIB != 3
const uint8_t _ParaMenu_AD2[C_LANGUAGE][4][18] =
    {
        {
         "��ȫ�� ",
         "�����벨",
         "�����벨",
         "����Ƶ ",
         }, //  ����
        {
         "1.FULL ",
         "2.HALF +",
         "3.HALF -",
         "4.RF   ",
         }, //  Ӣ��
};
#else
const uint8_t _ParaMenu_AD2[C_LANGUAGE][2][18] =
    {
        {
         "��ȫ�� ",
         "����Ƶ ",
         }, //  ����
        {
         "1.FULL ",
         "2.RF   ",
         }, //  Ӣ��
};
#endif

const uint8_t _ParaMenu_AD3_1[] = {"1. LOW   2. MIDDLE3. HIGH  4. WIDE  "}; /*ÿ��9*/

const uint8_t _ParaMenu_AD3[C_LANGUAGE][4][10] =
    {
        {
         "����Ƶ LOW",
         "����Ƶ MID",
         "����Ƶ HIG",
         "����Ƶ WID",
         }, //  ����
        {
         "1.Low    ",
         "2.Middle ",
         "3.High   ",
         "4.Wide   ",
         }, //  Ӣ��
};
// const uint8_t _ParaMenu_AD3[] = {"1. 2.5MHz2. 5.0MHz3. wide  "};	/*ÿ��9*/

const uint8_t _ParaMenu_AD4[] = {"1. LD/50  2. HD/400  "}; /*ÿ��10*/

#if C_DEVLIB == 23 || C_DEVLIB == 24
const uint8_t _ParaMenu_AD5[C_LANGUAGE][4][18] =
    {
        {
         "����ѹ",
         "����ѹ",
         "����׼",
         "����ѹ",
         }, //  ����
        {
         "1.Low ",
         "2.Middle",
         "3.STD",
         "4.High",
         }, //  Ӣ��
};
// const uint8_t _ParaMenu_AD5[] = {"1. LV/100V2. ML/200V3. MH/400V4. HV/800V"};		/*ÿ��10*/
#else
const uint8_t  _ParaMenu_AD5[] = {"1. LV/200V2. HV/400V"}; /*ÿ��10*/
#endif

const uint8_t _ParaMenu_AD7[C_LANGUAGE][4][18] =
    {
        {
         "����ͨ",
         "������",
         "���Ϻ�",
         "������",
         }, //  ����
        {
         "1.Nantong",
         "2.Beijing",
         "3.Shanghai",
         "4.Guangzhou",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_AD8[C_LANGUAGE][2][6] =
    {
        {
         "����  ",
         "����  ",
         }, //  ����
        {
         "1.On  ",
         "2.Off ",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_AD9[C_LANGUAGE][2][5] =
    {
        {
         "���� ",
         "���� ",
         }, //  ����
        {
         "1.No ",
         "2.Yes",
         }, //  Ӣ��
};
const uint8_t _ParaMenu_C1[C_LANGUAGE][3][22] =
    {
        {
         "���洢Ƶ�ף�",
         "������������",
         "��������λ��",
         }, //  ����
        {
         "1.Printer             ",
         "2.Param Protect       ",
         "3.Unit                ",
         }, //  Ӣ��
};

const uint8_t _ParaMenu_D1[] =
    {
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
};
const uint8_t _ParaMenu_CA1[] = "1.mm  2.inch";

const uint8_t _ParaMenu_CC1[] = "0. 0  1. 1  2. 2  3. 3  4. 4  ";
const uint8_t _ParaMenu_CD1[] = "1. 100  2. 200  3. 300  4. 400  5. 500  6. 600  7. 700  8. 800  9. 900  0. 1000  ";

const uint8_t _MainMenu_C1A1[C_LANGUAGE][4][16] =
    {
        {
         "��ֱ",
         "ˮƽ",
         "����",
         "�̶�",
         }, //  ����
        {
         "V-Range",
         "H-Range",
         "D-Range",
         "Scale  ",
         }, //  Ӣ��
};
const uint8_t _MainMenu_C1A3[C_LANGUAGE][2][16] =
    {
        {
         "խ",
         "��",
         }, //  ����
        {
         "Narrow ",
         "Wide ",
         }, //  Ӣ��
};

const uint8_t _MainMenu_C1A4[C_LANGUAGE][4][16] =
    {
        {
         "ֱ̽ͷ",
         "б̽ͷ",
         "˫�� ",
         "��͸ ",
         }, //  ����
        {
         "Normal",
         "Angle ",
         "Dual  ",
         "Through ",
         }, //  Ӣ��
};
const uint8_t _Weld_A1[C_LANGUAGE][1][40] =
    {
        {"����ͼʾ  ��  ҳ  ��  ҳ     "},
        {"Current page   Total page    "}};
const uint8_t _Weld_A2[C_LANGUAGE][1][40] =
    {
        {"̽ͷǰ�˵�����ߵľ��룺 "},
        {"The dis. from probe to seam:"}};
const uint8_t _Weld_B1[C_LANGUAGE][9][22] =
    {
        {
         "���������ͣ�          ",
         "��̽���� ��           ",
         "������ȣ�          ",
         "�������ȣ�          ",
         "�����溸�ţ�          ",
         "�����溸�ţ�          ",
         "��������ߣ�          ",
         "���¶���ߣ�          ",
         "�����¿ڽǣ�          ",
         },
        {
         "1.Weld Seam Type      ",
         "2.Detecting Surface   ",
         "3.Flange Thickness    ",
         "4.Web Thickness       ",
         "5.A Weld Foot         ",
         "6.B Weld Foot         ",
         "7.Reinforcement A     ",
         "8.Reinforcement B     ",
         "9.Groove Angle A      ",
         }
};
const uint8_t _Weld_B12[C_LANGUAGE][4][22] =
    {
        {
         "�����¿ڽǣ�          ",
         "�����¿��          ",
         "���۱߸߶ȣ�          ",
         "��������϶��          ",
         },
        {
         "1.Bevel Angle B       ",
         "2.Bevel A Depth       ",
         "3.Obtuse Side Height  ",
         "4.Gap Of Root         ",
         }
};

const uint8_t _Weld_B2[C_LANGUAGE][9][22] =
    {
        {
         "���������ͣ�          ",
         "��̽���� ��           ",
         "��������ȣ�          ",
         "����Ч��ȣ�          ",
         "�������ȣ�          ",
         "�������뾶��          ",
         "���϶���ߣ�          ",
         "���¶���ߣ�          ",
         "���¿ڽǶȣ�          ",
         },
        {
         "1.Weld Seam Type      ",
         "2.Detecting Surface   ",
         "3.Whole Thickness     ",
         "4.Effective Ply       ",
         "5.Weld Width          ",
         "6.Radius Of Root      ",
         "7.Top Reinforcement   ",
         "8.Root Reinforcement  ",
         "9.Groove Angle        ",
         }
};
const uint8_t _Weld_B22[C_LANGUAGE][7][22] =
    {
        {
         "���¿���ǣ�          ",
         "���¿���ȣ�          ",
         "���۱߸߶ȣ�          ",
         "��������϶��          ",
         "�����ڽǶȣ�          ",
         "��������ǣ�          ",
         "��������ȣ�          ",
         },
        {
         "1.Groove Side Angle   ",
         "2.Bevel Depth         ",
         "3.Obtuse Side Height  ",
         "4.Gap Of Root         ",
         "5.Inside Angle        ",
         "6.Inside side Angle   ",
         "7.Inside Depth        ",
         }
};

const uint8_t _Weld_C1[C_LANGUAGE][4][10] =
    {
        {
         "����      ",
         "������    ",
         "��˫��    ",
         "������    ",
         },
        {
         "1.Null    ",
         "2.Single  ",
         "3.Double  ",
         "4.T-type  ",
         }
};

const uint8_t _Weld_D1[C_LANGUAGE][3][13] =
    {
        {
         "������       ",
         "������       ",
         "������       ",
         },
        {
         "1.Surface A  ",
         "2.Surface B  ",
         "3.Surface C  ",
         }
};

const uint16_t _OptionMenu_B1[] =
    {
        CH(217),
        CH(4189),
        CH(1944),
        CH(1774),
        CH(2240),
        CH(326),
        CH(218),
        CH(1777),
        CH(3570),
        CH(1825),
        CH(1905),
        CH(326),
        CH(219),
        CH(3833),
        CH(3627),
        CH(3333),
        CH(2240),
        CH(326),
        CH(220),
        CH(4901),
        CH(2082),
        CH(5101),
        CH(2790),
        CH(326),
        CH(221),
        CH(2117),
        CH(3331),
        CH(1774),
        CH(5528),
        CH(326),
        CH(222),
        CH(2876),
        CH(1808),
        CH(2329),
        CH(4229),
        CH(326),
        CH(223),
        CH(3443),
        CH(1808),
        CH(3821),
        CH(2088),
        CH(326),
        CH(224),
        CH(4429),
        CH(4523),
        CH(5572),
        CH(3665),
        CH(326),
        CH(225),
        CH(2302),
        CH(4168),
        CH(2171),
        CH(4925),
        CH(326),
};
/*
1.	���̱�ȣ���ֱ\\ˮƽ\\����2025 5417 4314 3829 3064 3275
2.	���油����dB
3��	��Ļ���ȣ�����\һ��\����\����
4��	ѡ��ӡ����
5��	������׼��ĸ��\�з�\����\����
6��	�첨��ʽ��ȫ��\ ���벨\ ���벨\ ��Ƶ
7�� �˲�Ƶ����2.5\5.0\10\wide
8��	̽ͷ���᣺50Q\400Q
9��	�����ѹ��200V\400V
*/
const uint8_t _OptionMenu_C1[] =
    {
        ' ',
        ' ',
        ' ',
        'd',
        'B',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
        ' ',
};

const uint8_t _ParaProtect1[C_LANGUAGE][6][24] =
    {
        {
         "  ��������              ",
         "��ȫ�����ݣ�            ",
         "����λ������            ",
         "���������ã�            ",
         "��ͨ��������            ",
         "��̽ͷƵ�ʣ�            ",
         },
        {
         "Param Protect           ",
         "0.All Date      :       ",
         "1.Location      :       ",
         "2.Quantification:       ",
         "3.Parameter     :       ",
         "4.Frequency     :       ",
         }
};
const uint8_t _ParaProtect2[C_LANGUAGE][2][8] =
    {
        {
         "�ޡ�    ",
         "����    ",
         },
        {
         "Null    ",
         "Protect ",
         }
};
const uint8_t _ParaMenu_AD6[C_LANGUAGE][5][4] =
    {
        {
         "��  ",
         "����",
         "����",
         "����",
         "ȫ��",
         }, //  ����
        {
         "Null",
         "Part",
         "Part",
         "Part",
         "All ",
         }, //  Ӣ��
};
const uint8_t _DataMenu_A1[C_LANGUAGE][2][20] =
    {
        {
         "������ͨѶ",
         "�����ݴ���",
         }, //  ����
        {
         "1.Print     Screen  ",
         "2.Data      Storage ",
         }, //  Ӣ��
};
const char _DataMenu_A2[C_LANGUAGE][10][10] =
    {
        {
         "����      ",
         "������    ",
         "�����    ",
         "������    ",
         "�����    ",
         "��ͨѶ    ",
         "������    ",
         "��Ƶ��    ",
         "�����    ",
         "��Ƶ��    ",
         },
        {
         "REVIEW    ",
         "1.Date    ",
         "2.ID      ",
         "3.Hide    ",
         "4.Delete  ",
         "5.Comm.   ",
         "6.Recall  ",
         "7.Spec.   ",
         "7.Output  ",
         "8.Spec.   ",
         }
};

#if C_DEVTYPE == 14
const uint16_t _DataMenu_C1[] =
    {
        CH(3312), CH(4888), CH(2839), CH(3428), CH(3110), CH(4228), ',', CH(1620), CH(1703), CH(2070), CH(2892), CH(2965), CH(4288)}; // ������¼��ʼ,�����������  1703 2070
#else
const uint16_t _DataMenu_C1[] =
    {
        CH(3312), CH(4888), CH(2839), CH(3428), CH(3110), CH(4228), ',', CH(1620), CH(2839), CH(3428), CH(2892), CH(2965), CH(4288)}; // ������¼��ʼ,����¼������
#endif
const uint16_t _DataMenu_C2[] =
    {
        CH(1805), CH(2337), CH(3312), CH(4888), CH(2839), CH(3428), CH(1808), CH(4846)}; // ����������¼����

const unsigned short _DataMenu_D1[] =
    {
        CH(217),
        CH(1764),
        CH(2637),
        CH(326),
        CH(218),
        CH(2463),
        CH(2240),
        CH(326),
        CH(219),
        CH(1904),
        CH(2240),
        CH(326),
}; /*
 1.��ţ�
 2.�߶ȣ�
 3.���ȣ�
 / �洢
   //*/
const char _DataMenu_E1[C_LANGUAGE][2][22] =
    {
        {
         "���ǵ�ǰͨ��������?Y/N",
         },
        {
         "Channel overwrite?Y/N ",
         }
};
const char _Message_A1[C_LANGUAGE][31][22] =
    {
        {
         "��ӡ��δ����",             //             0
            "��������    ",               //               1
            "��ȱ�ݼ�¼  ",                //                2
            "�����ѴＫֵ",                    //                    3
            "����ȡֵ����",                        //                        4
            "�洢��������",                            //                            5
            "�ڴ���������",                                //                                6
            "���ݲ��ܱ䶯",                                    //                                    7
            "δ�ñ�׼�Կ�",                                        //                                        8
            "���Ȳ���̽ͷ",                                            //                                            9
            "���ݲ�����  ",                                                 //                                                 10
            "��ʼ�Զ��洢",                                                     //                                                     11
            "ֹͣ�Զ��洢",                                                         //                                                         12
            "��ʼ���ݴ���",                                                             //                                                             13
            "�����Ѵ洢  ",                                                                  //                                                                  14
            "            ",                                                                            //                                                                                               15
            "̽���������", // 16
            "�����������", // 17
            "�Զ�����    ", // 18
            "��������SD��", // 19
            "SD�����ӳɹ�", // 20
            "SD������ʧ��", // 21
            "������¼��  ", // 22
            "SD���Ͽ�����", // 23
            "���ڰ�ȫ�Ͽ�", // 24
            "SD����������", // 25
            "SD����������", // 26
            "SD�����ڴ���", // 27
            "SD�����ڴ���", // 28
            "SD���ڸ�ʽ��", // 29
            "SD��ʽ���ɹ�", // 30
        },
        {
         "No printer            ", //         0
            "Battery low           ", //     1
            "No record             ",   // 2
            "Parameter limit       ",        // 3
            "Value invalid         ",             // 4
            "Record cleared        ",                  // 5
            "Memory is full        ",                       // 6
            "Data change forbidden ",                            // 7
            "                      ",                                // 8
            "Test probe first      ",                                     // 9
            "Record does not exist ",                                          // 10
            "Start auto store      ",                                               // 11
            "Stop auto store       ",                                                    // 12
            "                      ",                                                         // 13
            "Data stored           ",                                                              // 14
            "                      ", // 15
            "Detecting...          ", // 16
            "Deleting record       ", // 17
            "Automatic Gain        ", // 18
            "SD connecting         ", // 19
            "Successful connection ", // 20
            "Connection failed     ", // 21
            "Continuous REC        ", // 22
            "SD disconnected       ", // 23
            "Disconnecting         ", // 24
            "SD capacity full      ", // 25
            "SD will be full       ", // 26
            "SD will be stand by   ", // 27
            "SD will be stand by   ", // 28
            "SD formatting         ", // 29
            "Formatted successfully", // 30
        }
};

const uint8_t _Message_B1[C_LANGUAGE][15][24] =
    {
        {
         "����洢���ݣ��٣���",
         "�Ȳ�������٣��٣���",
         "�Ȳ�����ֵ���٣���",
         "�����ǰ���ݣ��٣���",
         "�˳���ǰ���ԣ��٣���",
         "�洢�������ݣ��٣���",
         "�洢���ڰ��磿�٣���",
         "�洢���ڷ�ֵ���٣���",
         "�����ǰͨ�����٣���",
         "�������ͨ�����٣���",
         "���̽�����ݣ��٣���",
         "����������٣���    ",
         "�������ݣ��٣���    ",
         "����������ݣ��٣���",
         "�Ѳ���SD�����٣���  ",
         },
        {
         "Delete record?Y/N       ",
         "Test probe zero?Y/N     ",
         "Test zero & angle?Y/N   ",
         "Delete record?Y/N       ",
         "Exit?Y/N                ",
         "Save record?Y/N         ",
         "Save envelope?Y/N       ",
         "Save peak?Y/N           ",
         "Clear current CH?Y/N    ",
         "Clear all channel?Y/N   ",
         "Clear flaw record?Y/N   ",
         "Clear all data?Y/N      ",
         "Save record?Y/N         ",
         "Clear ContinuousData?Y/N",
         "SD inserted?Y/N         ",
         },
};

const uint8_t _SaveMenu[C_LANGUAGE][4][12] =
    {
        {
         "���¼ӱ�ǩ ",
         "����ͣ�洢 ",
         "�������洢 ",
         "���洢��Ϣ ",
         },
        {
         "1.New label ",
         "2.Pause save",
         "3.Stop save ",
         "4.SD info   ",
         }
};
const uint8_t _UDiskMenu[C_LANGUAGE][6][30] =
    {
        {
         "��ǰ�ļ�           �ֽ�       ",
         "���ÿռ�           �ֽ�       ",
         "���ÿռ�           �ֽ�       ",
         "����               �ֽ�       ",
         "Ԥ�ƿɴ洢ʱ��    Сʱ  ��  ��",
         "����                          ",
         },
        {
         "Current             Byte      ",
         "Used                Byte      ",
         "Free                Byte      ",
         "Capacity            Byte      ",
         "Estimated time     h     m   s",
         "Used                          ",
         }
};

const uint8_t _PushMenu[C_LANGUAGE][4][12] =
    {
        {
         "�������洢 ",
         "���Ƴ��ӣ� ",
         },
        {
         "1.Continue  ",
         "2.Remove    ",
         }
};

const uint16_t _Message_B2[] =
    {
        CH(331), CH(389), CH(315), CH(378)};
//?y/n
const uint8_t _FuncHotKey_A[C_LANGUAGE][5][22] =
    {
        {
         "��Ĭ������  ",
         "����ݼ�����",
         "����ݼ�����",
         "����ݼ�����",
         "����ݼ�����",
         }, //  ����
        {
         "0.Default  ",
         "1.Hot key 1",
         "2.Hot key 2",
         "3.Hot key 3",
         "4.Hot key 4",
         }, //  Ӣ��
};
const uint8_t _FuncHotKey_B[C_LANGUAGE][11][22] =
    {
        {
         "����ʼ��  ",
         "���ز�����",
         "����ֵ����",
         "���ģ�����",
         "����Ȳ���",
         "��ʵ�Ĳ���",
         "�����ڱ���",
         "�����ݴ���",
         "�����ζ���",
         }, //  ����
        {
         "1.Initialization ",
         "2.Echo Envelope  ",
         "3.Peak Memory    ",
         "4.DAC Gate       ",
         "5.TCG            ",
         "6.Solid Wave     ",
         "7.Alarm in Gate  ",
         "8.Data Storage   ",
         "9.Freeze         ",
         },
};
const uint8_t _FuncMenu_A1[C_LANGUAGE][11][22] =
    {
        {
         "ȷ������Ч",
         "����ʼ��  ",
         "���ز�����",
         "����ֵ����",
         "�����ڱ���",
         "���ģ�����",
         "����Ȳ���",
         "����������",
         "������չ��",
         "�����⿪��",
         "����ɨ��  ",
         }, //  ����
        {
         "Press Enter           ",
         "0.Initialization      ",
         "1.Echo Envelope       ",
         "2.Peak Memory         ",
         "3.Alarm in Gate       ",
         "4.DAC Gate            ",
         "5.TCG                 ",
         "6.C S Amend           ",
         "7.Zoom in Wave        ",
         "8.Light Switch        ",
         "9.B Scan              ",
         }, //  Ӣ��
};
const char _FuncMenu_A2[C_LANGUAGE][20][21] =
    {
        {"��ʼ��    ", "����ǰͨ��", "������ͨ��", "��ȱ������", "������    ", "������ʱ��", "��SD��ʽ��"},
        {
         "Initialization",
         "1.Channel",
         "2.All Channel",
         "3.Flaw Record",
         "4.Detector",
         "5.Time setup",
         "6.Format SD",
         }
};

const uint16_t _FuncMenu_B1[] =
    {
        CH(2504), CH(2894), CH(2681), CH(2240), CH(326), CH(100), CH(100), CH(100), 'm', 'm'};
/*������ȣ�   mm	/9
 */
const uint16_t _FuncMenu_B2[] =
    {
        CH(5038), CH(4429), CH(4523), CH(2187), CH(1846), CH(3331), CH(4225), CH(4017), CH(4761), CH(4147), CH(2243),
        CH(5578), CH(2463), CH(1808), CH(2006), CH(5258), CH(3537), CH(3658), 'E', 'N', CH(4023), CH(4047), CH(100),
        CH(5038), CH(4429), CH(4523), CH(2187), CH(1846), CH(3331), CH(4225), CH(4017), CH(4761), CH(4734), CH(2243),
        CH(5578), CH(2463), CH(1808), CH(2006), CH(5258), CH(3537), CH(3658), 'E', 'N', CH(4023), CH(4047), CH(100)};
/*��̽ͷ������ʹȱ���϶�  11*2
  ��߲���������  ENȷ��
  ��̽ͷ������ʹȱ���¶�
  ��߲���������  ENȷ��
*/

const uint16_t _FuncMenu_B3[] =
    {
        CH(4017), CH(4761), CH(2463), CH(2240), CH(326), CH(100), CH(100), CH(100), 'm', 'm',
        CH(2070), CH(4017), CH(4761), CH(1808), CH(331), CH(389), CH(315), CH(378), CH(100)};
/*ȱ�ݸ߶ȣ�   mm	9*2
  ��ȱ�ݲ�?Y/N
*/

const uint16_t _FuncMenu_C1[] =
    {
        CH(1825),
        CH(1905),
        CH(5421),
        CH(326),
        CH(100),
        CH(100),
        'd',
        'B',
}; // ����ֵ

const uint8_t _MenuPrompt_B1[C_LANGUAGE][1][20] =
    {
        {"��ȷ����Ч�����ط���"},
        {"��  Enter ��  Return"},
};
const uint8_t _MenuPrompt_P1[C_LANGUAGE][1][20] =
    {
        {"����һ�� "},
        {"��  Next "},
};
const uint8_t _MenuPrompt_P2[C_LANGUAGE][1][30] =
    {
        {"�������һȱ��  ���˳���ǰ���� "},
        {"��next defect  ��Exit"},
};
const uint8_t _MenuPrompt_P3[C_LANGUAGE][1][30] =
    {
        {"<-��¼���˸�   "},
        {"<-Record backspace  "},
};
const uint8_t _MenuPrompt_P4[C_LANGUAGE][1][30] =
    {
        {"#���ؼ��������� "},
        {"��Back key to re-enter"},
};

const char _STDMenu_A1[C_LANGUAGE][11][21] =
    {
        {
         "����׼���ͣ�",
         "�����߸�����",
         "���Կ����ͣ�",
         "��������ȣ�",
         "����һ���ߣ�",
         "���ڶ����ߣ�",
         "���������ߣ�",
         "�����ĸ��ߣ�",
         "��������ߣ�",
         "���������ߣ�",
         "�����ȼ���",
         },
        {
         "1.Standard Type",
         "2.Curve Number ",
         "3.Block Type   ",
         "4.Thickness    ",
         "5.1st Curve    ",
         "6.2nd Curve    ",
         "7.3rd Curve    ",
         "8.4th Curve    ",
         "9.5th Curve    ",
         "0.6th Curve    ",
         "3.Test Grade   ",
         }
};
const char _STDMenu_A2[C_LANGUAGE][11][21] =
    {
        {
         "����׼���ͣ�",
         "�����߸�����",
         "��ȱ�ݷ�Χ��",
         "��������ȣ�",
         "���������ߣ�",
         "����¼���ߣ�",
         "���������ߣ�",
         "�����ĸ��ߣ�",
         "��������ߣ�",
         "���������ߣ�",
         "�����ȼ���",
         },
        {
         "1.Standard Type",
         "2.Curve Number ",
         "3.Defect Size  ",
         "4.Thickness    ",
         "5.Acceptance   ",
         "6.Record Levels",
         "7.Evaluation   ",
         "8.4th Curve    ",
         "9.5th Curve    ",
         "0.6th Curve    ",
         "3.Test Grade   ",
         }
};
const char _STDMenu_A3[C_LANGUAGE][11][21] =
    {
        {
         "����׼���ͣ�",
         "�����߸�����",
         "��ȱ�ݷ�Χ��",
         "��������ȣ�",
         "��������һ��",
         "�������߶���",
         "���������ߣ�",
         "�����ĸ��ߣ�",
         "��������ߣ�",
         "���������ߣ�",
         "�����ȼ���",
         },
        {
         "1.Standard Type",
         "2.Curve Number ",
         "3.Defect Size  ",
         "4.Thickness    ",
         "5.Acceptance  1",
         "6.Acceptance  2",
         "7.Evaluation   ",
         "8.4th Curve    ",
         "9.5th Curve    ",
         "0.6th Curve    ",
         "3.Test Grade   ",
         }
};
const char _STDMenu_A4[C_LANGUAGE][11][21] =
    {
        {
         "����׼���ͣ�",
         "�����߸�����",
         "��ȱ�ݷ�Χ��",
         "��������ȣ�",
         "��������һ��",
         "�������߶���",
         "������������",
         "���������ߣ�",
         "��������ߣ�",
         "���������ߣ�",
         "�����ȼ���",
         },
        {
         "1.Standard Type",
         "2.Curve Number ",
         "3.Defect Size  ",
         "4.Thickness    ",
         "5.Acceptance  1",
         "6.Acceptance  2",
         "7.Acceptance  3",
         "8.Evaluation   ",
         "9.5th Curve    ",
         "0.6th Curve    ",
         "3.Test Grade   ",
         }
};
const char _STDMenu_A5[C_LANGUAGE][11][21] =
    {
        {
         "����׼���ͣ�",
         "�����߸�����",
         "��ȱ�ݷ�Χ��",
         "��������ȣ�",
         "��ĸ�ߣ�    ",
         "���������ߣ�",
         "���������ߣ�",
         "�����ĸ��ߣ�",
         "��������ߣ�",
         "���������ߣ�",
         "�����ȼ���",
         },
        {
         "1.Standard Type",
         "2.Curve Number ",
         "3.Defect Size  ",
         "4.Thickness    ",
         "5.Original Line",
         "6.Evaluation   ",
         "7.3rd Curve    ",
         "8.4th Curve    ",
         "9.5th Curve    ",
         "0.6th Curve    ",
         "3.Test Grade   ",
         }
};

const uint16_t _KEYLOCK_A1[] =
    {
        CH(1620), CH(104), CH(2892), CH(4388), CH(2892), CH(3744)}; // �����������
const uint16_t _KEYLOCK_A2[] =
    {
        CH(1620), CH(104), CH(2892), CH(2966), CH(4388), CH(100)}; // ���������
const uint16_t _KEYLOCK_A3[] =
    {
        CH(3975), CH(4740), CH(2966), CH(2892), CH(3744), CH(4388)};
const uint8_t _KEYLOCK_A1_E[] =
    {
        "Now press . !"}; // �����������
const uint8_t _KEYLOCK_A2_E[] =
    {
        "Now press . !"}; // ���������

const uint8_t _KEYLOCK_A3_E[] =
    {
        "Keys are locked!"};
// ���Ƚ������
#define C_STD_MAX         13 // 10
#define C_STD_LEN         22
#define C_BLOCK_MAX       6
#define C_BLOCK_LEN       15
#define C_STD_POS_THNUM   0
#define C_STD_POS_LINENUM 1
#define C_STD_POS_THICK   2

const uint8_t _STDMenu_AB1[] = {"\
1.Custom              \
2.GB/T11345-89        \
3.JB/T4730-2005       \
4.JG/T203-2015        \
5.SY/T4109-2020       \
6.JIS Z3060           \
7.ASME-V              \
8.CB/T3559-2011       \
9.DL/T820-2002        \
1.GB/T50621-2010      \
2.GB/T11345-2013 T1LV2\
3.GB/T11345-2013 T1LV3\
4.NB/T47013-2015      \
5.GB/T50661-2011      \
6.JB/T10559-2006      \
7.AWS D1.1/2020       "};

const uint8_t ZSTDMenu_AB1[] = {"\
1.Custom              \
2.GB/T11345-2013 T1LV2\
3.GB/T11345-2013 T1LV3\
4.NB/T47013-2015      \
5.CB/T3559-2011       \
6.GB/T11345-89        \
7.JG/T203-2015        \
8.SY/T4109-2020       \
9.JIS Z3060           \
1.ASME-V              \
2.DL/T820-2002        \
3.GB/T50621-2010      \
4.JB/T4730-2005       \
5.GB/T50661-2011      \
6.JB/T10559-2006      \
7.AWS D1.1/2020       "};

const uint8_t _STDMenu_B1[][C_BLOCK_LEN * C_BLOCK_MAX + 1] =
    {
        {"1\
1.Custom       "},
        {"3\
1.RB-X   A     \
2.RB-X   B     \
3.RB-X   C     "}, // 2.GB/T11345-89
        {
            "5\
1.CSK-IIIA     \
2.CSK-IIA      \
3.CSK-IVA      \
4.GS-X         \
5.AL           "}, // 3.JB4730.3
        {
            "3\
1.A            \
2.B	           \
3.C            "}, // 4.JG/T203-2007
        {
            "1\
1.SGB          "}, // 5.SY 4065-93
        {
            "1\
1.Custom       "}, // JIS
        {
            "1\
1.Custom       "}, // ASME-3
        {
            "1\
1.CTRB         "}, // CB/T3559-2011
        {
            "3\
1.RB-X A       \
2.RB-X B       \
3.RB-X C       "}, // 9.DL/T820 2002
        {
            "3\
1.A            \
2.B	           \
3.C            "}, // 0.GB/T50621-2010
        {"2\
1.UNKNOW       \
2.IGNORE       \
3.L <= T/2     \
4.L <= T       \
5.L >  T       "}, // GB/T11345-2013
        {"2\
1.UNKNOW       \
2.IGNORE       \
3.L <= T/2     \
4.L <= T       \
5.L >  T       "}, // GB/T11345-2013
        {"5\
1.CSK-IIA      \
2.CSK-IIIA     \
3.CSK-IVA      \
4.GS-X         \
5.AL           "}, // 47013
        {
            "3\
1.A            \
2.B	           \
3.C            "}, // GB/T50661-2011
        {
            "3\
1.I            \
2.II	          \
3.III          "}, // GB/T10559-2006
        {
            "1\
1.Custom       "}, // 0.aws2020
};

const short c_DACLine[][C_BLOCK_MAX][C_STD_POS_THICK + 7 * C_BLOCK_MAX] =
    {
        {
         {
                2,
                3,
                300,
                0,
                0,
                0,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  OTHERS
        {
         {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //	A/C
            {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -40,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //	B/C
            {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -20,
                -80,
                -140,
                -200,
                -260,
                -320,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //	B/C
        }, //  GB/T11345-89
        {
         {
                4,
                3,
                15,
                20,
                -60,
                -120,
                -180,
                -240,
                -300,
                46,
                50,
                -30,
                -90,
                -150,
                -210,
                -270,
                120,
                100,
                0,
                -60,
                -120,
                -180,
                -240,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // CSK-IIIA
            {
                3,
                3,
                46,
                -40,
                -120,
                -180,
                -240,
                -300,
                -360,
                120,
                20,
                -80,
                -140,
                -200,
                -260,
                -320,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // CSK-IIA
            {
                2,
                3,
                400,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // CSK-IVA
            {
                4,
                3,
                8,
                -100,
                -160,
                -160,
                -160,
                -160,
                -160,
                15,
                -70,
                -130,
                -160,
                -160,
                -160,
                -160,
                100,
                -40,
                -100,
                -160,
                -160,
                -160,
                -160,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // GS-X
            {
                2,
                3,
                80,
                -40,
                -120,
                -180,
                -240,
                -300,
                -360,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // ����2
        }, //  JB4730-94
        {

         {
                3,
                3,
                3,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                3,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -40,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                3,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -20,
                -80,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  JG/T203-2007
        {
         {
                3,
                3,
                4,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                -20,
                -80,
                -140,
                -200,
                -260,
                -320,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  5.SY 4109-2020
        {
         {
                2,
                6,
                300,
                180,
                120,
                60,
                0,
                -60,
                -120,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  JIS
        {
         {
                2,
                3,
                300,
                0,
                -60,
                -140,
                -200,
                -260,
                -320,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  ASME-3
        {
         {
                3,
                3,
                50,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                100,
                20,
                -60,
                -120,
                -180,
                -240,
                -300,
                1000,
                20,
                -20,
                -80,
                0,
                0,
                0,
            }, //
        }, //  CB/T3559-2011
        {
         {
                3,
                3,
                13,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                13,
                0,
                0,
                0,
                0,
                0,
                0,
                160,
                -40,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                13,
                0,
                0,
                0,
                0,
                0,
                0,
                160,
                -20,
                -80,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  0.DL/T820 2002
        {

         {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                0,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -40,
                -100,
                -160,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                7,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                -20,
                -80,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  GB 50621-2010
        {
         {
                1,
                2,
                1000,
                0,
                -140,
                0,
                0,
                0,
                0,
            }, // UNKNOW
            {
                4,
                4,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                -40,
                -100,
                -140,
                -140,
                0,
                0,
                100,
                0,
                -60,
                -100,
                -140,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // IGNORE
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                -40,
                -80,
                -140,
                0,
                0,
                0,
                100,
                0,
                -40,
                -140,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L<=T/2
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                -40,
                -80,
                -140,
                0,
                0,
                0,
                100,
                -60,
                -100,
                -140,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L<=T
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                -100,
                -140,
                -140,
                0,
                0,
                0,
                100,
                -100,
                -140,
                -140,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L>T
        }, //  ���յȼ�2
        {
         {
                1,
                2,
                1000,
                0,
                -100,
                0,
                0,
                0,
                0,
            }, // UNKNOWֻ��ʾ�����ȼ���
            {
                4,
                4,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                0,
                -60,
                -100,
                -100,
                0,
                0,
                100,
                40,
                -20,
                -60,
                -100,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // IGNORE��ʾ�������յȼ��ߺ������ȼ���
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                0,
                -40,
                -100,
                0,
                0,
                0,
                100,
                40,
                0,
                -100,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L<=T/2��ʾ��Ӧ�����յȼ��ߡ���¼�ȼ��ߺ������ȼ���
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                0,
                -40,
                -100,
                0,
                0,
                0,
                100,
                -20,
                -60,
                -100,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L<=T
            {
                4,
                3,
                8,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                -60,
                -100,
                -100,
                0,
                0,
                0,
                100,
                -60,
                -100,
                -100,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // L>T
        }, //  ���յȼ�3
        {
         {
                5,
                3,
                6,
                -40,
                -120,
                -180,
                -240,
                -300,
                -360, // ��������Ϊ�Լ���ӣ�������ƫ����ʱҪ�����εݼ�
                40,
                -40,
                -120,
                -180,
                -240,
                -300,
                -360,
                100,
                20,
                -80,
                -140,
                -200,
                -260,
                -320,
                200,
                60,
                -40,
                -100,
                -160,
                -220,
                -300,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,

            }, // CSK-IIA
            {
                5,
                3,
                8,
                20,
                -60,
                -120,
                -180,
                -240,
                -300,
                15,
                20,
                -60,
                -120,
                -180,
                -240,
                -300,
                40,
                50,
                -30,
                -90,
                -150,
                -210,
                -270,
                120,
                100,
                0,
                -60,
                -120,
                -180,
                -240,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // CSK-IIIA
            {
                4,
                3,
                200,
                0,
                0,
                0,
                0,
                0,
                0,
                300,
                30,
                -70,
                -130,
                -190,
                -250,
                -310,
                500,
                50,
                -50,
                -110,
                -170,
                -230,
                -290,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // CSK-IVA
            {
                4,
                3,
                8,
                -120,
                -180,
                -240,
                -240,
                -240,
                -240,
                15,
                -80,
                -140,
                -200,
                -200,
                -200,
                -200,
                300,
                -40,
                -100,
                -160,
                -160,
                -160,
                -160,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // GS-X
            {
                2,
                3,
                300,
                -40,
                -120,
                -180,
                -240,
                -300,
                -360,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, // ����2

        }, //  47013
        {

         {
                3,
                3,
                2,
                0,
                0,
                0,
                0,
                0,
                0,
                50,
                0,
                -60,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                2,
                0,
                0,
                0,
                0,
                0,
                0,
                150,
                0,
                -60,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                3,
                3,
                2,
                0,
                0,
                0,
                0,
                0,
                0,
                150,
                0,
                -60,
                -140,
                -220,
                -280,
                -340,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  GB 50661-2011
        {

         {
                6,
                4,
                5,
                0,
                0,
                0,
                0,
                0,
                0,
                20,
                -100,
                -140,
                -140,
                -180,
                -220,
                -220,
                38,
                -80,
                -100,
                -120,
                -160,
                -220,
                -220,
                65,
                -40,
                -60,
                -80,
                -140,
                -220,
                -220,
                100,
                0,
                -20,
                -40,
                -100,
                -220,
                -220,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                6,
                4,
                5,
                0,
                0,
                0,
                0,
                0,
                0,
                20,
                -80,
                -120,
                -120,
                -160,
                -220,
                -220,
                38,
                -40,
                -60,
                -80,
                -120,
                -220,
                -220,
                65,
                0,
                -20,
                -40,
                -100,
                -220,
                -220,
                100,
                +40,
                +20,
                -20,
                -80,
                -220,
                -220,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            },
         {
                6,
                4,
                5,
                0,
                0,
                0,
                0,
                0,
                0,
                20,
                -60,
                -100,
                -100,
                -140,
                -220,
                -220,
                38,
                0,
                -20,
                -40,
                -80,
                -220,
                -220,
                65,
                +40,
                +20,
                -20,
                -80,
                -220,
                -220,
                100,
                +60,
                +20,
                -20,
                -80,
                -220,
                -220,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  GB/T10559-2006
        {
         {
                2,
                3,
                300,
                0,
                0,
                0,
                0,
                0,
                0,
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
            }, //
        }, //  AWS2020
};

const uint16_t _COMM_B1[] =
    {
        CH(4293), CH(3061), CH(5393), CH(5258), CH(2011), CH(4268), CH(312), CH(3975), CH(2140), CH(2093), '.', '.', '.', ' '}; // �������ڴ��䣬��ȴ�

const uint8_t _Bright_A1[C_LANGUAGE][2][18] =
    {
        {
#if C_DEVTYPE == 14
         "���Ҽ���������",
#else
            "��������������",
#endif
         },
        {
#if C_DEVTYPE == 14
         "����Set Brightness",
#else
            "����Set Brightness",
#endif
         }
};
const uint8_t _Bscan[C_LANGUAGE][4][32] =
    {
        {
         "ɨ�������������˳�            ",
         "�����ײ��ȶ�ʱȷ��              ",
         "���ڹ������ƶ�̽ͷ              ",
         "��������ȣ�                    ",
         },
        {
         "Finish scan,press any key exit  ",
         "Keep back wall stable           ",
         "Move the probe                  ",
         "1.Thickness:                    ",
         }
};
const unsigned short _BSCAN_B2[] =
    {
        CH(217),
        CH(2504),
        CH(2894),
        CH(2681),
        CH(2240),
        CH(326),
}; // ������ȣ��������ȣ�

const short sin_tab[] =
    {
        0, 123, 245, 368, 491, 613, 736, 858, 980, 1102, 1224, 1346, 1467, 1589, 1710, 1830,
        1951, 2071, 2191, 2311, 2430, 2549, 2667, 2785, 2903, 3020, 3137, 3253, 3369, 3484, 3599, 3713,
        3827, 3940, 4052, 4164, 4276, 4386, 4496, 4605, 4714, 4822, 4929, 5035, 5141, 5246, 5350, 5453,
        5556, 5657, 5758, 5858, 5957, 6055, 6152, 6249, 6344, 6438, 6532, 6624, 6716, 6806, 6895, 6984,
        7071, 7157, 7242, 7327, 7410, 7491, 7572, 7652, 7730, 7807, 7883, 7958, 8032, 8105, 8176, 8246,
        8315, 8382, 8449, 8514, 8577, 8640, 8701, 8761, 8819, 8876, 8932, 8987, 9040, 9092, 9142, 9191,
        9239, 9285, 9330, 9373, 9415, 9456, 9495, 9533, 9569, 9604, 9638, 9670, 9700, 9729, 9757, 9783,
        9808, 9831, 9853, 9873, 9892, 9909, 9925, 9939, 9952, 9963, 9973, 9981, 9988, 9993, 9997, 9999,
        10000, 9999, 9997, 9993, 9988, 9981, 9973, 9963, 9952, 9939, 9925, 9909, 9892, 9873, 9853, 9831,
        9808, 9783, 9757, 9729, 9700, 9670, 9638, 9604, 9569, 9533, 9495, 9456, 9415, 9373, 9330, 9285,
        9239, 9191, 9142, 9092, 9040, 8987, 8932, 8876, 8819, 8761, 8701, 8640, 8577, 8514, 8449, 8382,
        8315, 8246, 8176, 8105, 8032, 7958, 7883, 7807, 7730, 7652, 7572, 7491, 7410, 7327, 7242, 7157,
        7071, 6984, 6895, 6806, 6716, 6624, 6532, 6438, 6344, 6249, 6152, 6055, 5957, 5858, 5758, 5657,
        5556, 5453, 5350, 5246, 5141, 5035, 4929, 4822, 4714, 4605, 4496, 4386, 4276, 4164, 4052, 3940,
        3827, 3713, 3599, 3484, 3369, 3253, 3137, 3020, 2903, 2785, 2667, 2549, 2430, 2311, 2191, 2071,
        1951, 1830, 1710, 1589, 1467, 1346, 1224, 1102, 980, 858, 736, 613, 491, 368, 245, 123,
        0, -122, -244, -367, -490, -612, -735, -857, -979, -1101, -1223, -1345, -1466, -1588, -1709, -1829,
        -1950, -2070, -2190, -2310, -2429, -2548, -2666, -2784, -2902, -3019, -3136, -3252, -3368, -3483, -3598, -3712,
        -3826, -3939, -4051, -4163, -4275, -4385, -4495, -4604, -4713, -4821, -4928, -5034, -5140, -5245, -5349, -5452,
        -5555, -5656, -5757, -5857, -5956, -6054, -6151, -6248, -6343, -6437, -6531, -6623, -6715, -6805, -6894, -6983,
        -7070, -7156, -7241, -7326, -7409, -7490, -7571, -7651, -7729, -7806, -7882, -7957, -8031, -8104, -8175, -8245,
        -8314, -8381, -8448, -8513, -8576, -8639, -8700, -8760, -8818, -8875, -8931, -8986, -9039, -9091, -9141, -9190,
        -9238, -9284, -9329, -9372, -9414, -9455, -9494, -9532, -9568, -9603, -9637, -9669, -9699, -9728, -9756, -9782,
        -9807, -9830, -9852, -9872, -9891, -9908, -9924, -9938, -9951, -9962, -9972, -9980, -9987, -9992, -9996, -9998,
        -9999, -9998, -9996, -9992, -9987, -9980, -9972, -9962, -9951, -9938, -9924, -9908, -9891, -9872, -9852, -9830,
        -9807, -9782, -9756, -9728, -9699, -9669, -9637, -9603, -9568, -9532, -9494, -9455, -9414, -9372, -9329, -9284,
        -9238, -9190, -9141, -9091, -9039, -8986, -8931, -8875, -8818, -8760, -8700, -8639, -8576, -8513, -8448, -8381,
        -8314, -8245, -8175, -8104, -8031, -7957, -7882, -7806, -7729, -7651, -7571, -7490, -7409, -7326, -7241, -7156,
        -7070, -6983, -6894, -6805, -6715, -6623, -6531, -6437, -6343, -6248, -6151, -6054, -5956, -5857, -5757, -5656,
        -5555, -5452, -5349, -5245, -5140, -5034, -4928, -4821, -4713, -4604, -4495, -4385, -4275, -4163, -4051, -3939,
        -3826, -3712, -3598, -3483, -3368, -3252, -3136, -3019, -2902, -2784, -2666, -2548, -2429, -2310, -2190, -2070,
        -1950, -1829, -1709, -1588, -1466, -1345, -1223, -1101, -979, -857, -735, -612, -490, -367, -244, -122};
const short cos_tab[] =
    {
        10000, 9999, 9997, 9993, 9988, 9981, 9973, 9963, 9952, 9939, 9925, 9909, 9892, 9873, 9853, 9831,
        9808, 9783, 9757, 9729, 9700, 9670, 9638, 9604, 9569, 9533, 9495, 9456, 9415, 9373, 9330, 9285,
        9239, 9191, 9142, 9092, 9040, 8987, 8932, 8876, 8819, 8761, 8701, 8640, 8577, 8514, 8449, 8382,
        8315, 8246, 8176, 8105, 8032, 7958, 7883, 7807, 7730, 7652, 7572, 7491, 7410, 7327, 7242, 7157,
        7071, 6984, 6895, 6806, 6716, 6624, 6532, 6438, 6344, 6249, 6152, 6055, 5957, 5858, 5758, 5657,
        5556, 5453, 5350, 5246, 5141, 5035, 4929, 4822, 4714, 4605, 4496, 4386, 4276, 4164, 4052, 3940,
        3827, 3713, 3599, 3484, 3369, 3253, 3137, 3020, 2903, 2785, 2667, 2549, 2430, 2311, 2191, 2071,
        1951, 1830, 1710, 1589, 1467, 1346, 1224, 1102, 980, 858, 736, 613, 491, 368, 245, 123,
        0, -122, -244, -367, -490, -612, -735, -857, -979, -1101, -1223, -1345, -1466, -1588, -1709, -1829,
        -1950, -2070, -2190, -2310, -2429, -2548, -2666, -2784, -2902, -3019, -3136, -3252, -3368, -3483, -3598, -3712,
        -3826, -3939, -4051, -4163, -4275, -4385, -4495, -4604, -4713, -4821, -4928, -5034, -5140, -5245, -5349, -5452,
        -5555, -5656, -5757, -5857, -5956, -6054, -6151, -6248, -6343, -6437, -6531, -6623, -6715, -6805, -6894, -6983,
        -7070, -7156, -7241, -7326, -7409, -7490, -7571, -7651, -7729, -7806, -7882, -7957, -8031, -8104, -8175, -8245,
        -8314, -8381, -8448, -8513, -8576, -8639, -8700, -8760, -8818, -8875, -8931, -8986, -9039, -9091, -9141, -9190,
        -9238, -9284, -9329, -9372, -9414, -9455, -9494, -9532, -9568, -9603, -9637, -9669, -9699, -9728, -9756, -9782,
        -9807, -9830, -9852, -9872, -9891, -9908, -9924, -9938, -9951, -9962, -9972, -9980, -9987, -9992, -9996, -9998,
        -9999, -9998, -9996, -9992, -9987, -9980, -9972, -9962, -9951, -9938, -9924, -9908, -9891, -9872, -9852, -9830,
        -9807, -9782, -9756, -9728, -9699, -9669, -9637, -9603, -9568, -9532, -9494, -9455, -9414, -9372, -9329, -9284,
        -9238, -9190, -9141, -9091, -9039, -8986, -8931, -8875, -8818, -8760, -8700, -8639, -8576, -8513, -8448, -8381,
        -8314, -8245, -8175, -8104, -8031, -7957, -7882, -7806, -7729, -7651, -7571, -7490, -7409, -7326, -7241, -7156,
        -7070, -6983, -6894, -6805, -6715, -6623, -6531, -6437, -6343, -6248, -6151, -6054, -5956, -5857, -5757, -5656,
        -5555, -5452, -5349, -5245, -5140, -5034, -4928, -4821, -4713, -4604, -4495, -4385, -4275, -4163, -4051, -3939,
        -3826, -3712, -3598, -3483, -3368, -3252, -3136, -3019, -2902, -2784, -2666, -2548, -2429, -2310, -2190, -2070,
        -1950, -1829, -1709, -1588, -1466, -1345, -1223, -1101, -979, -857, -735, -612, -490, -367, -244, -122,
        0, 123, 245, 368, 491, 613, 736, 858, 980, 1102, 1224, 1346, 1467, 1589, 1710, 1830,
        1951, 2071, 2191, 2311, 2430, 2549, 2667, 2785, 2903, 3020, 3137, 3253, 3369, 3484, 3599, 3713,
        3827, 3940, 4052, 4164, 4276, 4386, 4496, 4605, 4714, 4822, 4929, 5035, 5141, 5246, 5350, 5453,
        5556, 5657, 5758, 5858, 5957, 6055, 6152, 6249, 6344, 6438, 6532, 6624, 6716, 6806, 6895, 6984,
        7071, 7157, 7242, 7327, 7410, 7491, 7572, 7652, 7730, 7807, 7883, 7958, 8032, 8105, 8176, 8246,
        8315, 8382, 8449, 8514, 8577, 8640, 8701, 8761, 8819, 8876, 8932, 8987, 9040, 9092, 9142, 9191,
        9239, 9285, 9330, 9373, 9415, 9456, 9495, 9533, 9569, 9604, 9638, 9670, 9700, 9729, 9757, 9783,
        9808, 9831, 9853, 9873, 9892, 9909, 9925, 9939, 9952, 9963, 9973, 9981, 9988, 9993, 9997, 9999};

const uint16_t PARA_BB1[] =
    {
        // Ϊ���㴦������Ӧ�̶�
        CH(217), CH(2211), CH(2329), CH(2676), '5', ' ', ' ', CH(16),
        CH(218), CH(2211), CH(2329), CH(2676), '5', 'B', ' ', CH(16),
        CH(219), CH(2211), CH(2329), CH(2676), '2', '1', ' ', CH(16),
        CH(220), CH(2211), CH(2371), '3', ' ', ' ', CH(100), CH(16),
        CH(221), CH(2211), CH(2371), '4', 'B', ' ', CH(100), CH(16),
        CH(222), CH(2211), CH(2371), '4', 'C', ' ', CH(100), CH(16),
        CH(223), CH(2211), CH(2371), '4', 'D', ' ', CH(100), CH(16),
        CH(224), CH(2211), CH(2371), '4', 'E', ' ', CH(100), CH(16),
        CH(225), CH(2211), CH(2371), '5', ' ', ' ', CH(100), CH(16),
        CH(226), CH(2211), CH(2371), '5', 'B', ' ', CH(100), CH(16),

        CH(217), CH(2211), CH(2371), '5', 'D', ' ', CH(100), CH(16),
        CH(218), CH(2211), CH(2371), '6', ' ', ' ', CH(100), CH(16),
        CH(219), CH(2211), CH(2371), '7', ' ', ' ', CH(100), CH(16),
        CH(220), CH(2211), CH(2371), '7', 'B', ' ', CH(100), CH(16),
        CH(221), CH(2211), CH(2371), '7', 'C', ' ', CH(100), CH(16),
        CH(222), CH(2211), CH(2371), '7', 'D', ' ', CH(100), CH(16),
        CH(223), CH(2211), CH(2371), '8', ' ', ' ', CH(100), CH(16),
        CH(224), CH(2211), CH(2371), '8', 'B', ' ', CH(100), CH(16),
        CH(225), CH(2211), CH(2371), '1', '0', 'F', CH(100), CH(16),
        CH(226), CH(2211), CH(2371), '1', '1', ' ', CH(100), CH(16),

        CH(217), CH(4156), CH(4129), '3', 'B', ' ', CH(100), CH(16),
        CH(218), CH(4156), CH(4129), '4', ' ', ' ', CH(100), CH(16),
        CH(219), CH(4156), CH(4129), '4', 'B', ' ', CH(100), CH(16),
        CH(220), CH(4156), CH(4129), '4', 'C', ' ', CH(100), CH(16),
        CH(221), CH(4156), CH(4129), '6', 'B', ' ', CH(100), CH(16),
        CH(222), CH(4156), CH(4129), '7', 'B', ' ', CH(100), CH(16),
        CH(223), CH(4156), CH(4129), '8', ' ', ' ', CH(100), CH(16),
        CH(224), CH(4156), CH(4129), '9', ' ', ' ', CH(100), CH(16),
        CH(225), 'N', 'D', '3', ' ', ' ', CH(100), CH(16),
        CH(226), 'N', 'D', '5', ' ', ' ', CH(100), CH(16)};

/*
������5
������5B
������21
����3
����4B
����4C
����4D
����4E
����5
����5B

����5D
����6
����7
����7B
����7C
����7D
����8
����8B
����10F
����11

��ɽ3B
��ɽ 4
��ɽ4B
��ɽ4C
��ɽ6B
��ɽ7B
��ɽ 8
��ɽ 9
ND3
ND5
*/
