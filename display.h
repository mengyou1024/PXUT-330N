//��ʾ���ֲ�������ֵ�����ڹ��̣��������桢���̡�̽ͷ���͡�ͨ����
#include "globe.h"
#include "sysdef.h"
extern int NumHigh;
#ifndef _DISPLAY_H_


void PageDisp(int xpos,int ypos,u_char* mainpage);		//��ʾ16*16ͼ��

int DrawPower(u_char powerlevel,int mode);	//������ָʾ,mode = 0����1ÿ���Ӹ���һ��
void SysParaDisp(void);		//��ʾͨ���š�̽ͷ���͡�Ƶ����
void WriteProbe(int type, int xpos, int ypos);


//������������йصĺ���

void WriteParaName(u_int ParaIndex); /*���ݲ���������д������*/

int MSelectPrompt(int keycode);//���˳���������ʱ�ж�ʦ��Ҫ��ʾ��ʾ

int MAdjustGain(u_int type,u_int mode,short low,short high);	/*��������,type=0ϵͳ����=1������mode=0����*/
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
void WriteBaseGain(u_int xpos,u_int ypos);	//��������
void WriteCompGain(u_int xpos,u_int ypos);	//��������
void WriteSurfGain(u_int xpos,u_int ypos);	//���油��
void WriteRange(u_int xpos,u_int ypos);	//���̷�Χ
void WriteSpeed(u_int xpos,u_int ypos);	//����
void WriteOffset(u_int xpos,u_int ypos);//���
void WriteReject(u_int xpos,u_int ypos);//���
void WriteDelay(u_int xpos,u_int ypos);	//��ʱ
void WriteAngle(u_int xpos,u_int ypos);	//�Ƕ�
void WriteScale(void);	//���
void WriteLongness(u_short xpos, u_short ypos,int number,u_char total_len/*�ܳ�*/,u_char mode/*�Ƿ�д��λ*/);//д����



void ChannelScreenRenovate(void);	/*ͨ����Ļˢ��*/
void ScreenRenovate(void);	/*��Ļˢ��*/
void MChannelRenovate(void);	//��ͨ����Ļ����ˢ��
int GetDispEchoPara(void);/*�õ���(A+B)�ڻز���λ�ô�С��д����Ļ��*/
void DispEchoPara2(void);

void WritePeakPrompt(int mode1,int mode2);	////mode1=0B��ʧ��1������mode2=0����1dB��
void WritePeakInfo(int mode1,int mode2,int para1,int para2,int para3,int para4);//mode1=0B��ʧ��1������mode2=0����1dB��

void MParaRenovate(u_int mode);	/*����ˢ��,0����*/
int MCoorDraw(u_int CoorHPosi , u_int CoorVPosi , u_int CoorHeight , u_int CoorWidth );	/*��ָ��λ�úʹ�С�Ĳ�������*/
void MGateDraw(void);	/*����*/
void MGateDraw1(int gatetype, int mode);	/*����*/
void MDrawGate( int/*-1���Ŷ���0A��1B��*/ gatetype, int/*-1����1����*/ mode,
                int paratype/*-1ȫ��0��λ1�ſ�2�Ÿ�*/ ,int clear/*0�»�1���*/);
void DrawDacGate(void);

void Scrsave_Picture(void);
void MSetGateAmpColor();

//ɫ��
void MSetColor(int item);
void MSaveColor(int item,u_int cr);
u_int MLoadColor(int item);
void MSaveDefualtColor(int item);//����Ĭ����ɫ
#define _DISPLAY_H_
#endif

int WeldSetup(void);
