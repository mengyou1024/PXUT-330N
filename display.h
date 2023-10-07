//��ʾ���ֲ�������ֵ�����ڹ��̣��������桢���̡�̽ͷ���͡�ͨ����
#include "globe.h"
#include "sysdef.h"
extern int NumHigh;
#ifndef _DISPLAY_H_


void PageDisp(int xpos,int ypos,uint8_t* mainpage);		//��ʾ16*16ͼ��

int DrawPower(uint8_t powerlevel,int mode);	//������ָʾ,mode = 0����1ÿ���Ӹ���һ��
void SysParaDisp(void);		//��ʾͨ���š�̽ͷ���͡�Ƶ����
void WriteProbe(int type, int xpos, int ypos);


//������������йصĺ���

void WriteParaName(uint32_t ParaIndex); /*���ݲ���������д������*/

int MSelectPrompt(int keycode);//���˳���������ʱ�ж�ʦ��Ҫ��ʾ��ʾ

int MAdjustGain(uint32_t type,uint32_t mode,short low,short high);	/*��������,type=0ϵͳ����=1������mode=0����*/
int ChannelChange(void);
int GainChange(void);
int BaseGainPlus(int step);		/*������ݲ�������*/
int BaseGainMinus(int step);		/*������ݲ�������*/
int CompGainPlus(int step);		/*������ݲ�������*/
int CompGainMinus(int step);		/*������ݲ�������*/

int OffsetChange(void);
int OffsetPlus(int step);
int OffsetMinus(int step);

int RejectChange(void);
int RejectPlus(int step);
int RejectMinus(int step);

int SpeedChange(void);
int SpeedPlus(int step);
int SpeedMinus(int step);

int RangeChange(void);
int RangePlus(int step);		/*���ݲ�������*/
int RangeMinus(int step);		/*���ݲ���*/

int DelayChange(void);
int DelayPlus(int step);		/*���ݲ�������*/
int DelayMinus(int step);		/*���ݲ���*/

int AngleChange(void);
int AnglePlus(int step);
int AngleMinus(int step);

int GateChange(void);
void MGatePosi(int keycod,int gatetype,int step);	/* ��λ�ı�*/
void MGateWide(int keycod,int gatetype,int step);	/* �ſ�ı�*/
void MGateHigh(int keycod,int gatetype,int step);	/* �Ÿ߸ı�*/
void MDACGateChange(int keycode, int type);	/* �Ÿı�*/

/*��ʾ������ֵ*/
void WriteBaseGain(uint32_t xpos,uint32_t ypos);	//��������
void WriteCompGain(uint32_t xpos,uint32_t ypos);	//��������
void WriteSurfGain(uint32_t xpos,uint32_t ypos);	//���油��
void WriteRange(uint32_t xpos,uint32_t ypos);	//���̷�Χ
void WriteSpeed(uint32_t xpos,uint32_t ypos);	//����
void WriteOffset(uint32_t xpos,uint32_t ypos);//���
void WriteReject(uint32_t xpos,uint32_t ypos);//���
void WriteDelay(uint32_t xpos,uint32_t ypos);	//��ʱ
void WriteAngle(uint32_t xpos,uint32_t ypos);	//�Ƕ�
void WriteScale(void);	//���
void WriteLongness(uint16_t xpos, uint16_t ypos,int number,uint8_t total_len/*�ܳ�*/,uint8_t mode/*�Ƿ�д��λ*/);//д����



void ChannelScreenRenovate(void);	/*ͨ����Ļˢ��*/
void ScreenRenovate(void);	/*��Ļˢ��*/
void MChannelRenovate(void);	//��ͨ����Ļ����ˢ��
int GetDispEchoPara(void);/*�õ���(A+B)�ڻز���λ�ô�С��д����Ļ��*/
void DispEchoPara2(void);

void WritePeakPrompt(int mode1,int mode2);	////mode1=0B��ʧ��1������mode2=0����1dB��
void WritePeakInfo(int mode1,int mode2,int para1,int para2,int para3,int para4);//mode1=0B��ʧ��1������mode2=0����1dB��

void MParaRenovate(uint32_t mode);	/*����ˢ��,0����*/
int MCoorDraw(uint32_t CoorHPosi , uint32_t CoorVPosi , uint32_t CoorHeight , uint32_t CoorWidth );	/*��ָ��λ�úʹ�С�Ĳ�������*/
void MGateDraw(void);	/*����*/
void MGateDraw1(int gatetype, int mode);	/*����*/
void MDrawGate( int/*-1���Ŷ���0A��1B��*/ gatetype, int/*-1����1����*/ mode,
                int paratype/*-1ȫ��0��λ1�ſ�2�Ÿ�*/ ,int clear/*0�»�1���*/);
void DrawDacGate(void);

void Scrsave_Picture(void);
void MSetGateAmpColor();

//ɫ��
void MSetColor(int item);
void MSaveColor(int item,uint32_t cr);
uint32_t MLoadColor(int item);
void MSaveDefualtColor(int item);//����Ĭ����ɫ
#define _DISPLAY_H_
#endif

int WeldSetup(void);
