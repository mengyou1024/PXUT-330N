//显示各种参量的数值、调节过程，包括增益、声程、探头类型、通道等
#include "globe.h"
#include "sysdef.h"
extern int NumHigh;
#ifndef _DISPLAY_H_


void PageDisp(int xpos,int ypos,uint8_t* mainpage);		//显示16*16图形

int DrawPower(uint8_t powerlevel,int mode);	//画电量指示,mode = 0立即1每分钟更新一次
void SysParaDisp(void);		//显示通道号、探头类型、频带等
void WriteProbe(int type, int xpos, int ypos);


//与各参量调节有关的函数

void WriteParaName(uint32_t ParaIndex); /*根据参量索引号写参量名*/

int MSelectPrompt(int keycode);//在退出参数调节时判断师父要显示提示

int MAdjustGain(uint32_t type,uint32_t mode,short low,short high);	/*调节增益,type=0系统增益=1补偿；mode=0立即*/
int ChannelChange(void);
int GainChange(void);
int BaseGainPlus(int step);		/*增益根据步长增加*/
int BaseGainMinus(int step);		/*增益根据步长增加*/
int CompGainPlus(int step);		/*增益根据步长增加*/
int CompGainMinus(int step);		/*增益根据步长增加*/

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
int RangePlus(int step);		/*根据步长增加*/
int RangeMinus(int step);		/*根据步长*/

int DelayChange(void);
int DelayPlus(int step);		/*根据步长增加*/
int DelayMinus(int step);		/*根据步长*/

int AngleChange(void);
int AnglePlus(int step);
int AngleMinus(int step);

int GateChange(void);
void MGatePosi(int keycod,int gatetype,int step);	/* 门位改变*/
void MGateWide(int keycod,int gatetype,int step);	/* 门宽改变*/
void MGateHigh(int keycod,int gatetype,int step);	/* 门高改变*/
void MDACGateChange(int keycode, int type);	/* 门改变*/

/*显示各参数值*/
void WriteBaseGain(uint32_t xpos,uint32_t ypos);	//基本增益
void WriteCompGain(uint32_t xpos,uint32_t ypos);	//补偿增益
void WriteSurfGain(uint32_t xpos,uint32_t ypos);	//表面补偿
void WriteRange(uint32_t xpos,uint32_t ypos);	//声程范围
void WriteSpeed(uint32_t xpos,uint32_t ypos);	//声速
void WriteOffset(uint32_t xpos,uint32_t ypos);//零点
void WriteReject(uint32_t xpos,uint32_t ypos);//零点
void WriteDelay(uint32_t xpos,uint32_t ypos);	//延时
void WriteAngle(uint32_t xpos,uint32_t ypos);	//角度
void WriteScale(void);	//标度
void WriteLongness(uint16_t xpos, uint16_t ypos,int number,uint8_t total_len/*总长*/,uint8_t mode/*是否写单位*/);//写长度



void ChannelScreenRenovate(void);	/*通道屏幕刷新*/
void ScreenRenovate(void);	/*屏幕刷新*/
void MChannelRenovate(void);	//本通道屏幕参数刷表
int GetDispEchoPara(void);/*得到门(A+B)内回波的位置大小并写在屏幕上*/
void DispEchoPara2(void);

void WritePeakPrompt(int mode1,int mode2);	////mode1=0B门失波1进波，mode2=0波高1dB数
void WritePeakInfo(int mode1,int mode2,int para1,int para2,int para3,int para4);//mode1=0B门失波1进波，mode2=0波高1dB数

void MParaRenovate(uint32_t mode);	/*参数刷新,0立即*/
int MCoorDraw(uint32_t CoorHPosi , uint32_t CoorVPosi , uint32_t CoorHeight , uint32_t CoorWidth );	/*画指定位置和大小的波形坐标*/
void MGateDraw(void);	/*画门*/
void MGateDraw1(int gatetype, int mode);	/*画门*/
void MDrawGate( int/*-1两门都画0A门1B门*/ gatetype, int/*-1向下1向上*/ mode,
                int paratype/*-1全门0门位1门宽2门高*/ ,int clear/*0新画1清除*/);
void DrawDacGate(void);

void Scrsave_Picture(void);
void MSetGateAmpColor();

//色彩
void MSetColor(int item);
void MSaveColor(int item,uint32_t cr);
uint32_t MLoadColor(int item);
void MSaveDefualtColor(int item);//设置默认颜色
#define _DISPLAY_H_
#endif

int WeldSetup(void);
