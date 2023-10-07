#include <stdarg.h>
#include <math.h>
#include "M10Era.h"
#include "tools.h"
#include "sysdef.h"


int g_ypos =10;
int g_xpos =10;


void delay(uint32_t interval)
{
    uint32_t start_time;

    start_time = GetElapsedTime();
    while(GetElapsedTime() < start_time + interval);

    return;
}

uint32_t drawpixel(uint32_t x,uint32_t y,uint32_t color)
{
    uint16_t *pSrcBuf,oldvalue;
    pSrcBuf = GetDisplayBufferBase();
    oldvalue = *(pSrcBuf + SCREEN_WIDTH*y + x);
    *(pSrcBuf + SCREEN_WIDTH*y + x) = (((0xff&color)>>3)<<11)
                                      + (((0xff00&color)>>(8+2))<<5)
                                      + (((0xff0000&color)>>(16+3)));

    return oldvalue;
}


/**********************************************************
*
* 名称: RDC16TextOutBase
* 功能: 在指定的位置输出一个汉字字符串[16*16]
* 参数:
*		x, y    开始位置
*		cCHStr  汉字字符串
*
* 返回:
*
* 附注: 该函数可以用RDC16TextOut来代替，还没有找到问题所在
*
***********************************************************/
void RDC16TextOutBase(int x , int y ,char *cCHStr)
{
	int k,iLen;
    uint16_t iTemp,iCode[60];
	char *cStr;
	uint8_t ss,*cc;

	cStr = cCHStr;
	iLen = strlen(cStr);

	cc = (uint8_t *) iCode;
	memcpy(iCode, cStr, iLen);
	for (k=0; k < iLen; k++)
			cc[k] -= 0xa0;
	for (k=0; k < iLen/2 ; k++)
	{
		ss=cc[2*k];
		cc[2*k]=cc[2*k+1];
		cc[2*k+1]=ss;
	}
	CTextOut(x,y,iCode,iLen/2);
}

void HexDump(int x,int y,char *buf, int length)
{
    char table[200];
    int i;
    memset(table,0,200);
    for(i=0;i<length;i++)
        sprintf(table + i*3,"%02x ",buf[i]);
    EraseWindow(x,y,strlen(table)*8,8);
    ETextOut(x,y,table,strlen(table));

}

void   _trace(int x, int y,char   *prompt,   ...)
{
#pragma warn -8004
     char   buf[2048];
     va_list   argptr;
     int cnt;
     va_start(argptr,   prompt);
     cnt   =   vsprintf(buf,   prompt,   argptr);
     //OutputDebugString(buf);
     if (g_ypos>470)
		g_ypos = 10;

     EraseWindow(x,y,strlen(buf)*8, 8);
     ETextOut(x,y,buf,strlen(buf));
     va_end(argptr);
}


uint32_t WaitForKeyStroke()
{
	uint32_t keycode ;
	TRACE(g_xpos,g_ypos+=10,"-I- Press 'Return' key to continue...  ");

	while(MAnyKeyReturn()!=C_KEYCOD_RETURN);
	return ;
}

