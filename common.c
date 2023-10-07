/*common.c*/
//common.c
#include "common.h"
#include "sysdef.h"
#include "display.h"
#include "main.h"
//#include "hzk.h"
//#include "main.h"

extern GATE_PEAK			GatePeak;
extern GATE_PARA			AGatePara;			/*8*/
extern GATE_PARA			BGatePara;			/*8*/
extern GATE_MAX				GateMax;		//���ڳ��ֵ���߲�
extern SYSTEM_STATUS		SystemStatus;		/*4*/
extern CHANNEL_STATUS		ChannelStatus;		/*4*/
extern CHANNEL_PARAMETER	ChannelPara;		/*15*2*/
extern FAT					fat;
extern CURVE_AMEND stCurveAmend;
extern WELD_PARA stWeldPara;

extern unsigned char ChannelMax;
extern unsigned short c_FWBaseLine ;	//ȫ������
extern unsigned short c_RFBaseLine ;	//��Ƶ����

//USER stUser[C_USERMAX];

extern u_short dac_line[];

extern ADJUST_TIME			AdjustTime;
extern u_char  *dactable;

///extern u_int FixGain ;
extern int extend;
extern int HoriOffsetScreen ;		//����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��
extern int VertOffsetScreen ;		//����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��

extern DAC			Dac;

extern const u_int KEY_DIGITAB[];
extern const u_char MadeDate[21];//������ǰ
extern const u_char _version[30];//�汾��

extern const u_char AmpMaxCom_Table[] ;
extern const u_char PowerTable[] ;
extern const u_int _RANGEVALUE[];

extern u_short GainTable[] ;
extern const u_short c_gaintable[][121] ;

extern const short sin_tab[] ;
extern const short cos_tab[] ;

extern int g_nDenoise;
extern const u_int c_crPara[];
extern const u_int Echo_crPara[];

extern u_char crPara[];

extern const u_short _ParaMenu_CE2[];


//���ú���
int curr_cr;



void InvertBytes(u_int XPos, u_int YPos, u_int bytes) /*������ʾ�����ֽڵ�����*/
{
    if( bytes > 0 )
        MInvertWindow( XPos, YPos, (XPos+ C_ECHAR_HDOT * bytes)-1, YPos + C_ECHAR_VDOT ) ;
}

void InvertWords(u_int XPos, u_int YPos, u_int words) /*������ʾ�����ֵ�����*/
{
    if( words > 0 )
        MInvertWindow( XPos, YPos, (XPos+ C_CCHAR_HDOT * words) - 1, YPos + C_CCHAR_VDOT ) ;
}

int CharStr2Number( char str[],int number)//number������
{
    int i;
    int retvalue = 0;
    int sign;
    int temp;
    if( str[0] == '-')
    {
        sign = -1;
        i = 1;
    }
    else
    {
        sign = 1;
        i = 0;
    }
    for( ; i < number ; i++)
    {
        temp = str[i] - 0x30;
        if( temp > 9 || temp < 0)continue;
        retvalue *= 10;
        retvalue += temp;
    }
    return retvalue;
}
int ShortStr2Number( short str[],int number)
{
    int i;
    int retvalue = 0;
    int sign;
    int temp;
    if( str[0] == '-')
    {
        sign = -1;
        i = 1;
    }
    else
    {
        sign = 1;
        i = 0;
    }
    for( ; i < number ; i++)
    {
        temp = str[i] - 0x30;
        if( temp > 9 || temp < 0)continue;
        retvalue *= 10;
        retvalue += temp;
    }
    return retvalue;
}

u_int Bcd2Hex(u_int val)
{
    u_int temp1 = val % 16 ;
    u_int temp2 = 1;

    while(val > 0)
    {
        val = val / 16 ;
        temp2 *= 10;
        temp1 += (val % 16) * temp2;	//ȡ��ǰ���λ
    }
    return temp1;
}

u_int Hex2Bcd(u_int val)
{
    u_int temp1 = (val % 10) ;
    u_int temp2 = 1;

    while(val > 0)
    {
        val = val/10 ;
        temp2 *= 16;
        temp1 += (val % 10) * temp2;	//ȡ��ǰ���λ
    }
    return temp1;
}

u_int GetNumberLen(u_int number)	/*�õ�����λ��*/
{
    u_int len=0;	/*λ�� */

    do
    {
        number /= 10;
        len++;
    }
    while (number);
    return len;
}

void Swap(void *x,void *y)		/*��������*/
{
    void *temp;
    temp = x;
    x = y;
    y = temp;
}

void ExpendTime(int time)	/*����ʱ�䣬��10msΪ��λ*/
{
    //Added By WST
//	return ;
    // int keycode=MGetKey();
    u_int PreElapsedTime;

    if(time > 0)time = (time-1)*10;	//*����ʱ�䵥λΪ10ms
    else time = 0;

    PreElapsedTime = GetElapsedTime() + time;
    while(1)
    {
        //	StorageData(C_KEYCODMAX);
        if( GetElapsedTime() > PreElapsedTime )break;
        PreElapsedTime /= 2;
        PreElapsedTime *= 2;
        //MGetKey();
        // while(keycode<C_KEYCODMAX)
        // {
        //     keycode=MGetKey();
        // }

    }

}

void BuzzerOn(u_int time) 		/*����0.1 * time �룬�������time=0��ת����ǰ����״̬*/
{
//	MSetAlarmStatus(1);
//	return;
    u_int PreElapsedTime = GetElapsedTime() + 10 * time*4;
    if( time >= 0 )
    {
        PreElapsedTime = GetElapsedTime() + 1 ;
        while(1)
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        //SwitchBuzzerOn(1) ;
        CharToKeypad(BUZZER_ALARM_ON) ;
        PreElapsedTime = GetElapsedTime() + 10 * time*4;
        while(1)		/*����10msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        // SwitchBuzzerOn(0) ;	//�������time=0��ת����ǰ����״̬
        CharToKeypad(BUZZER_ALARM_OFF) ;
        PreElapsedTime = GetElapsedTime() + 10 * time*4;;
        while(1)		/*����10msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
    }
}
void AlarmOn(void) 		/*����0.1 * time �룬�������time=0��ת����ǰ����״̬*/
{
    u_int PreElapsedTime = GetElapsedTime() + 3 ;
//	return;

    if( MGetAcquisitionEnable() != 1 )return ;

    /*   if(MGetAlarmSatus()>0 && MGetFunctionMode(C_GATE_ALARM)==1)
       {
           ExpendTime(5);
           CharToKeypad(LED_ALARM_ON);
           ExpendTime(10);
           CharToKeypad(BUZZER_ALARM_ON);
       }
       else
       {
           ExpendTime(5);
           CharToKeypad(LED_ALARM_OFF);
           ExpendTime(10);
           CharToKeypad(BUZZER_ALARM_OFF);
       }
    return;*/
    {
        PreElapsedTime = GetElapsedTime() + 5 ;
        while(1)		/*����1msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        ;
        //SwitchAlarmOn(1);
        CharToKeypad(BUZZER_ALARM_ON) ;
        PreElapsedTime = GetElapsedTime() + 5 ;
        while(1)		/*����1msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        // SwitchBuzzerOn(1) ;
        CharToKeypad(LED_ALARM_ON) ;
        PreElapsedTime = GetElapsedTime() + 6 ;
        while(1)		/*����3msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        ;
        //SwitchAlarmOn(0);
        CharToKeypad(BUZZER_ALARM_OFF) ;
        PreElapsedTime = GetElapsedTime() + 5 ;
        while(1)		/*����1msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
        //SwitchBuzzerOn(0) ;
        CharToKeypad(LED_ALARM_OFF) ;
        PreElapsedTime = GetElapsedTime() + 5 ;
        while(1)		/*����1msʱ��*/
        {
            if(  GetElapsedTime() > PreElapsedTime )break;
            PreElapsedTime /= 2;
            PreElapsedTime *= 2;
        }
    }
}
BOOL bAlarmStatus;
int nAlarmStartTime;
int nAlarmElapsedTime = 2;
void MSetAlarmStatus(int on)
{
    int ElapTime = GetElapsedTime();

    if( on < 0)
    {
        nAlarmStartTime = 0;
        bAlarmStatus = 0;
        // SwitchBuzzerOn(0) ;
        // SwitchAlarmOn(0);

        CharToKeypad(LED_ALARM_OFF) ;
        CharToKeypad(BUZZER_ALARM_OFF) ;
    }
    else if(on==0)
    {
        bAlarmStatus = 0;
    }
    else
    {
        bAlarmStatus = 1;
    }
    /*
        else if(nAlarmStartTime <= ElapTime)
        {
            on = on%2;
            if( on != MGetAlarmSatus() )
            {
                //SwitchBuzzerOn(on) ;
                //SwitchAlarmOn(on);

                CharToKeypad(LED_ALARM_OFF-on) ;
                CharToKeypad(BUZZER_ALARM_OFF-on) ;
                nAlarmStartTime = ElapTime + nAlarmElapsedTime;
            }
            else if( on )
            {
                on = 0;
               // SwitchBuzzerOn(0) ;
               // SwitchAlarmOn(0);
                CharToKeypad(LED_ALARM_OFF) ;
                CharToKeypad(BUZZER_ALARM_OFF) ;
                nAlarmStartTime = ElapTime + nAlarmElapsedTime;
            }
            bAlarmStatus = on;
        }*/
}
int MGetAlarmSatus()
{
    return bAlarmStatus%2;
}
u_int TestSameKeyTime(u_int Prekeycode,u_int PreElapsedTime,u_int MaxTime)
/*���԰���Prekeycode�Ѱ�ʱ����PreElapsedTimeΪ����ʱ��,MaxTimeΪ�ʱ�䣬
��ʱ�䵽���в�ͬ�����˳�,����ֵΪͬһ��������ʱ��*/
{
    u_int samekeytime;
    int offset = C_OFF_STORE;

    if(Prekeycode != C_KEYCODMAX)
    {
        MaxTime += PreElapsedTime;		//
        do
        {
            samekeytime = GetElapsedTime();
            if(Prekeycode != MGetKeyCode(0) )break;
            MaxTime *=2;
            MaxTime /=2;	//��������ʱ��
        }
        while ( samekeytime <= MaxTime );

        samekeytime -= PreElapsedTime;	//ͬһ����ʱ��
        MCopyProtectedMemory( (void*)offset, &samekeytime,4, PM_COPY_WRITE);
    }
    else
    {
        //�õ�ǰ�ε��õķ���ֵ
        MCopyProtectedMemory(&samekeytime, (void*)offset, 4, PM_COPY_READ);
    }

//Write_Number(0,30,samekeytime,8,3,0);

    return samekeytime;

}

u_int GetSameKeyTime(void)
/*�õ���ֵ��������������*/
{
    int offset ;
    int samekeytime = 0;

    offset = C_OFF_STORE + 21;		//ͬһ��ֵ������������
    MCopyProtectedMemory( &samekeytime,(void*)offset,  1, PM_COPY_READ);

    return samekeytime;
}

u_int GetNoKeyTime(void)	//�õ�����ǰ�ΰ�����Ŀǰֹ��ʱ������λs
{
    int offset ;
    int keyintime=0;

    offset = C_OFF_STORE + 22;		//22-25		���°���ʱ���࿪��ʱ��ʱ����λms
    MCopyProtectedMemory( &keyintime,(void*)offset,  4, PM_COPY_READ);
    keyintime = (GetElapsedTime() - keyintime)/1000;

    return keyintime;
}

u_int CheckDate(u_int year,u_int month,u_int date)	//����ꡢ�¡����Ƿ����
{
    //����ꡢ�¡����Ƿ���Ϲ��򣬷���ֵ0���ϣ���0�����ϣ�1��year,2:month,4:date
    //1+2:year+moth,1+4:year+date,2+4:month+date,1+2+4:year+month+date
    u_int ret_year = 0 , ret_month = 0 ,ret_date = 0 ;

    if( year > 9999)ret_year = 1;	//�겻������λ��
    if( month < 1 || month > 12 )ret_month = 2;
    if( date < 1 || date > 31 ) ret_date = 4;

    if( month == 4 || month == 6 || month == 9 || month == 11 )
    {
        if( date > 30 )ret_date = 4;	//С��
    }
    else if(month == 2)
    {
        if( year%4 > 0)
        {
            if( date > 28 )ret_date = 4;	//������
        }
        else
        {
            if( year % 100 == 0 && year % 400 != 0)
            {
                if( date > 28 )ret_date = 4;	//������
            }
            if( date > 29 )ret_date = 4;	//����
        }
    }
    return ret_year + ret_month + ret_date;
}


void MPush(int mint)		//ѹջ
{
    int offset;
    int sPoint;		//ջָ��
    offset = C_OFF_STACK ;
    MCopyProtectedMemory(&sPoint, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    sPoint++;
    MCopyProtectedMemory((void*)offset, &sPoint, C_SIZE_INT, PM_COPY_WRITE);

    offset = C_OFF_STACK + 10 + (sPoint - 1)* C_SIZE_INT;
    MCopyProtectedMemory((void*)offset, &mint, C_SIZE_INT, PM_COPY_WRITE);
}
int  MPop(int mode)			//��ջ
{
    int offset;
    int sPoint;		//ջָ��
    int mint;

    offset = C_OFF_STACK ;
    MCopyProtectedMemory(&sPoint, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    sPoint--;

    if(mode == 0 || sPoint < 0)sPoint = 0;
    MCopyProtectedMemory((void*)offset, &sPoint, C_SIZE_INT, PM_COPY_WRITE);
    //дջָ��

    offset = C_OFF_STACK + 10 + sPoint * C_SIZE_INT;
    MCopyProtectedMemory(&mint, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    return mint;
}

int test_fft(void)
{
    //short dataI[C_MAX_DOT_NUMBER];
    short *dataI;
    int temp0;
    float temp2;
    int i,j;
    //short w[280+2];
    short *w;
    //int keycode;
    u_char* sampbuff ;
    float fAcquisitionTime = 0;
    float f= MGetFrequence()/10.0;
    int mode = 3;
    dataI = dac_line;
    w = dac_line;
    if (f < 8) f = 10.0;
    else if(f < 16)f = 20.0;
    else if(f < 40)f = 50.0;
    else if(f < 80)f = 100.0;
    else f = 200.0;
    if(f<11)
    {
        fAcquisitionTime = 125.0*12.5/(float)(1*5*f);
        mode = 4;
    }
    else if(f<21)
    {
        fAcquisitionTime = 250.0*12.5/(float)(1*5*f);
        mode = 3;
    }
    else if(f<101)
    {
        fAcquisitionTime = 500.0*12.5/(float)(1*5*f);
        mode = 2;
    }
    else if(f<201)
    {
        fAcquisitionTime = 1000.0*12.5/(float)(1*5*f);
        mode = 1;
    }
    //����ʱ��us,f=2.5M t= 500/25=20us

    for( i = 0; i < 282 ; i++)
    {
        //	w[i] = 0;
    }

    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*��ָ��λ�úʹ�С�Ĳ�������*/

    int range  = 0 ;
    u_int xpos = 0,ypos = 458;
    for(i=0; i<5 ; i++)
    {
        Write_Number(xpos,ypos,range,5,1,0);
        range += (int)(2 * f /10);	//
        xpos += 6 * C_ECHAR_HDOT;
    }

    range = (int)( (float)(C_COORWIDTH*MGetSpeed() * fAcquisitionTime)/(200.0*C_MAX_DOT_NUMBER) + 0.5)/2;
    MSetScaleMode(2,0);
    MSetRange(range,C_SETMODE_SETSAVE);
    WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
    MSetGatePara(C_COORHORIUNIT*0.5,C_COORHORIUNIT*9,160,0,C_SETMODE_SETSAVE);
    MDrawGate(0,0,-1,0);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MAdjustGain(0,0,400,400) ;

    MParaRenovate(0);
    ExpendTime(50);

    //temp2 = (float)( 280.0 / C_MAX_DOT_NUMBER ) ;
    temp2 = 1;
    sampbuff = GetSampleBuffer() ;
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    for( i = 0; i < C_MAX_DOT_NUMBER ; i++)
    {
        temp0 = (int)( (float)i * temp2 );
        if(temp0<C_COORWIDTH)dataI[i] = ((sampbuff[temp0]-128) )*20;
        else dataI[i] = 0;
    }
    FFT(dataI,w,mode);

    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);

    for(i = C_COORHORIUNIT , j = 0, temp0 = 0; i < C_COORWIDTH && i < C_MAX_DOT_NUMBER ; i++)
    {
        if( w[i] > temp0)
        {
            temp0 = w[i];
            j = i;
        }
    }
    int offset = C_OFF_FREQ;
    u_char temp_w;
    for( i = 0; i < C_LEN_SAMP; i++,offset++)
    {
        temp_w = (u_char)w[i];
        MCopyProtectedMemory( (void*)offset, &temp_w, 1, PM_COPY_WRITE);//��Ƶ��
    }

//Write_Number(10,30,5,2,0,0);
    return (int)(0.99 * f * j /C_COORHORIUNIT);

}

void FFT(short dataI[],short w[],int mode)		//ͨ��ת����dataI�е���������ת��dataR��
{
    short xx;
    short L,i,j,k,b,p;
    short TR,TI;
    short TBR_COS,TBR_SIN,TBI_COS,TBI_SIN;
    short dataR[C_MAX_DOT_NUMBER];

    for( i = 0; i < C_MAX_DOT_NUMBER; i++)
    {
        for(xx = 0,j = 0; j < C_MAX_BIT; j++)
        {
            xx = ( xx << 1) + ( (i >> j) & 0x01 ) ;
        }
        dataR[xx] = dataI[i];		//ͨ��ת����dataI�е���������ת��dataR��
        dataI[i] = 0;

        //	w[i] = 0;
    }

    for(L = 0; L < C_MAX_BIT; L++)
    {
        /* for(1) */
        b = 1;
        b = ( b << L);		 /* b= 2^(L) */
        for(j = 0; j < b; j++) /* for (2) */
        {
            p = 1;
            i = C_MAX_BIT - L - 1;
            p = ( p << i) * j *1; /* p=pow(2,7-L)*j; */

            for( k = j ; k < C_MAX_DOT_NUMBER; k += 2*b) /* for (3) */
            {
                TR = dataR[k];
                TI = dataI[k];
                TBR_COS = dataR[k+b] * cos_tab[p]/10000;
                TBR_SIN = dataR[k+b] * sin_tab[p]/10000;
                TBI_COS = dataI[k+b] * cos_tab[p]/10000;
                TBI_SIN = dataI[k+b] * sin_tab[p]/10000;

                dataR[k] = TR + TBR_COS + TBI_SIN;
                dataI[k] = TI - TBR_SIN + TBI_COS;
                dataR[k+b] = TR - TBR_COS - TBI_SIN;
                dataI[k+b] = TI + TBR_SIN - TBI_COS;
            } /* END for (3) */
        } /* END for (2) */
    } /* END for (1) */


///	DrawCurver( w,0,280);
    b = 0;
    p = 1;
    p = ( p << (C_MAX_BIT - 1) );
    // if(C_MAX_DOT_NUMBER/4 > C_COORWIDTH)k = C_COORWIDTH;
    // else k = C_MAX_DOT_NUMBER/4;
    k = C_COORWIDTH;
    for(i = 0; i <= k; i++)
    {
        w[i] = (short)( 0.5 + 100*sqrt( dataR[i] * dataR[i] + dataI[i] * dataI[i] )/p );
        if(b < w[i])b = w[i];
    }
    for( i = 0 ; i < 25; i++)w[i] /= 4;
    for(i = 0,b=0; i <= k; i++)
    {
        if(b < w[i])b = w[i];
    }
    ;;
///	w[0]=w[0]/2;
    for(i = 0; i <= k; i++)
    {
        w[i] = w[i] * 190/b;
    }

    for(i = mode; i > 0; i--)
    {
        b = C_MAX_DOT_NUMBER/2;
        if(b > 250)b = 250;
        w[2*b] = w[b];
        for(j = b -1 ; j >= 0; j--)
        {
            w[2*j] = w[j];
            w[2*j+1] = (w[2*j]+w[2*j+2])/2;
        }
    }

    for(j = C_MAX_DOT_NUMBER/2-1 ; j >= 0; j--)
    {
        w[2*j] = w[j];
        w[2*j+1] = (w[2*j]+w[2*j+2])/2;
    }

    DrawCurver( w,0,C_COORWIDTH);
} /* END FFT */

u_int Random()
{
    u_int randvalue;

    u_int x,y,len;

    x = GetElapsedTime();
    y = GetElapsedTime() * GetElapsedTime() + 1;
    x = ( ( x + 1) * GetElapsedTime() ) % 308 + 1;
    y = ( x * ( x + y ) * GetElapsedTime() ) % 240 + 1;
    MCopyProtectedMemory( &len, (void*)( (x * y )%1000), sizeof(int), PM_COPY_READ);

    len = ( x * y + len ) % 32 +1;

    randvalue = ( ( MGetPixels( x, y, len) + x * y )* x + x * y * len ) /117 * 19 ;//+ rand();

    return randvalue;
}

float Mexpf(float ex, u_int len)//ָ���������õ�e^ex,λ��������len��8
{
    float retvalue ;
    int nMax = 1;
    float fMax = 0.0;
    if( len <= 1 )
    {
        len = 1;
        //fMax = 5.541263545;	//ln(255);
        fMax = 4.844187086;//ln(127)
    }
    else if( len == 2)
    {
        len = 2;
        //fMax = 11.09033963; //ln(65535)
        fMax = 10.39717719;	//ln(32767
    }
    else if( len > 2 && len <= 4)
    {
        len = 4;
        //fMax = 22.18067926;
        fMax = 10.39717719*2;
    }
    else if( len > 4)
    {
        len = 8;
        //fMax = 22.18067926*2;	//
        fMax = 10.39717719*4;
    }
    if( ex > fMax)ex = fMax;
    nMax = ( nMax << ( len * 8 ) ) - 1;

    retvalue = expf(ex);
    //if( retvalue > fMax )retvalue = fMax;

    return retvalue;
}
float MexpfA(float ex)//ָ���������õ�e^ex,λ��������len��8
{
    float retvalue ;
    float fMax ;

    fMax = 10.39717719;	//ln(32767
    if( ex > fMax)ex = fMax;

    retvalue = expf(ex);

    return retvalue;
}

int Mabsi( int x)
{
    x = ( x >= 0)? x : 0 - x;
    return x;
}
float Mabsf( float x)
{
    x = ( x >= 0)? x : 0 - x;
    return x;
}
double Mabsd( double x)
{
    x = ( x >= 0)? x : 0 - x;
    return x;
}
short Mabss( short x)
{
    x = ( x >= 0)? x : 0 - x;
    return x;
}
char Mabsc( char x)
{
    x = ( x <= 0)? 0 - x : x;
    return x;
}

//�Ӱ����˴������ݵı����з�����ָ��λ�õı���
u_int BitLoad(u_int des/*�������ݵı���*/, u_int bits/*�ֽ���*/, u_int cx/*��ʼ��λ��*/)
{
    u_int bx;
    bx = 1;
    bx = ( bx << bits ) - 1;
    return ( (des >> cx) & bx );
}
//������ָ��λ�ð����˴������ݵı���
u_int BitSave(u_int des/*��Ҫ�����������ݵı���*/, u_int bits/*�ֽ���*/, u_int cx/*��ʼ��λ��*/,u_int var/*����ı���*/)
{
    u_int bx , dx;
    bx = 1;
    bx = ( bx << bits ) - 1;
    dx = var & bx;
    bx = ~( bx << cx );
    return ( ( des & bx ) + ( dx << cx ) );
}
//������ϵͳ̽ͷ�йصĺ���
u_int MLoadStartTime(void)//����
{
    return SystemStatus.nStartElapsedTime/65536;
}
void MSaveStartTime(int time)
{
    SystemStatus.nStartElapsedTime = ( SystemStatus.nStartElapsedTime & 0x0000ffff ) + (time << 16);
}

u_int MLoadCurrentTime(void)//����
{
    return SystemStatus.nStartElapsedTime/65536 + SystemStatus.nStartElapsedTime%65536;
}

//ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
u_int MLoadSystemElapsedTime( int mode)
{
    u_int nElapsedTime;
    switch( mode )
    {
    case C_TIME_MILLION:
        nElapsedTime = SystemStatus.nElapsedTime;
        break;
    case C_TIME_SECOND:
        nElapsedTime = SystemStatus.nElapsedTime/1000;
        break;
    case C_TIME_MINUTE:
//		nElapsedTime = SystemStatus.nElapsedTime/(1000*60);
//		break;
    default:
        nElapsedTime = SystemStatus.nStartElapsedTime % 65536;
        break;
    }
    return nElapsedTime;
}
//	u_int nElapsedTime;			//����ʱ�����Ժ���Ϊ��λ
//	u_int nStartElapsedTime;	//��16λ����ʱ�䣬��16λ����ʱ�����Է�Ϊ��λ

void MSaveSystemElapsedTime( u_int nElapsedTime ,int mode)
{
    switch( mode )
    {
    case C_TIME_MILLION:
        SystemStatus.nElapsedTime = nElapsedTime;
        break;
    case C_TIME_SECOND:
        SystemStatus.nElapsedTime = nElapsedTime*1000;
        break;
    case C_TIME_MINUTE:
//		SystemStatus.nElapsedTime = nElapsedTime*(1000*60);
//		SystemStatus.nStartElapsedTime = ( SystemStatus.nStartElapsedTime & 0xffff0000 ) + (nElapsedTime % 65536);
//		break;
    default:
        SystemStatus.nStartElapsedTime = ( SystemStatus.nStartElapsedTime & 0xffff0000 ) + (nElapsedTime % 65536);
        break;
    }
}
void 	UGetSystemTime(TIME_CLOCK* tm)
{
    //  TIME_CLOCK TimeClock;
    int i;
    int keycode;
    u_char KeyState,KeyNumber;


    keycode= GetKeyCode();
    for(i=0; i<7; i++)
    {
        CharToKeypad(RTC_R_YEAR+i) ;
        ExpendTime(2);
        keycode= GetKeyCode();
        KeyState=(keycode>>8)& 0xff;   //״̬λ
        KeyNumber=keycode & 0xff;
        switch(KeyState)
        {
        case RTC_R_YEAR_ACK:
            tm->yearl= KeyNumber;
            break;
        case RTC_R_MONTH_ACK:
            tm->month= KeyNumber;
            break;
        case RTC_R_DATE_ACK:
            tm->date= KeyNumber;
            break;
        case RTC_R_HOUR_ACK:
            tm->hour= KeyNumber;
            break;
        case RTC_R_MINUTE_ACK:
            tm->minute= KeyNumber;
            break;
        case RTC_R_SECOND_ACK:
            tm->second= KeyNumber;
            break;
        case RTC_R_DAY_ACK:
            tm->day= KeyNumber;
            break;

        default:
            break;
        }
        ExpendTime(2);
    }
    tm->yearh=32;
    // TimeClock.second=8;

    //  *tm=TimeClock;
}

u_int MSetSystemTime(void)
{
    TIME_CLOCK TimeClock;
    u_int xpos,ypos;
    int keycode,i;
    int revlue;
    /*   year, month, day, hour, minute, second;

      GetSystemTime(&TimeClock);

      TimeClock.date =  Hex2Bcd(28);
      TimeClock.day =  Hex2Bcd(4);
      TimeClock.month =  Hex2Bcd(4);
      TimeClock.yearh =  Hex2Bcd(20);
      TimeClock.yearl =  Hex2Bcd(12);
      TimeClock.hour =  Hex2Bcd(20);
      TimeClock.minute =  Hex2Bcd(30);
      TimeClock.second =  Hex2Bcd(15);
    */
    while(1)
    {
        UGetSystemTime(&TimeClock);
		// GetSystemTime(&TimeClock) ;
        xpos = C_CCHAR_HDOT;
        ypos = 18*C_CCHAR_VDOT;
        WriteDate(xpos+50, ypos, &TimeClock);
        WriteTime(xpos+50+11*C_ECHAR_HDOT,ypos, &TimeClock);//��ʾʱ��
        // WriteWeek(xpos+50+17*C_ECHAR_HDOT,ypos,&TimeClock);

        while(InputDate(xpos+50, ypos-32, &TimeClock))
        {
            if(TimeClock.yearl>=1)break;
        }
        WriteDate(xpos+50, ypos, &TimeClock);

        {
            MKeyRlx();
            InputTime(xpos+50+11*C_ECHAR_HDOT,ypos-32, &TimeClock);//��ʾʱ��
            WriteTime(xpos+50+11*C_ECHAR_HDOT,ypos, &TimeClock);//��ʾʱ��
            break;
        }

    }


    CharToKeypad(RTC_W_REQ);
    ExpendTime(5);
    CharToKeypad(RTC_W_READY);
    ExpendTime(5);
    CharToKeypad(RTC_W_ACK);
    ExpendTime(5);
    CharToKeypad(TimeClock.yearl) ;
    ExpendTime(5);
    CharToKeypad(TimeClock.month) ;
    ExpendTime(5);
    CharToKeypad(TimeClock.date) ;
    ExpendTime(5);
    CharToKeypad(TimeClock.hour) ;
    ExpendTime(5);
    CharToKeypad(TimeClock.minute) ;
    ExpendTime(5);

    SetSystemTime(&TimeClock);
}
//mode = 0�꣬1�£�2���գ�3�ܼ���4Сʱ��5���ӣ�6�룬
//-1������,	-2��2000���ڼ��գ�-3�ڼ��ܣ�-4����ڼ��գ�-5�ڼ���,��6���쿪ʼ����ٷ���,��7���쿪ʼ�������
u_int MGetSystemTime(int mode)
{
    TIME_CLOCK TimeClock,tcm;
    u_int date, year, month, day, hour, minute, second;
    u_int time1,time2;
    u_int retvalue = 0;
    time1 = GetElapsedTime()/(1000*60);
    time2 = C_OFF_BATTERY + 10;
    MCopyProtectedMemory( &tcm, (void*)time2, 8, PM_COPY_READ);

    /*  if( time1 > 1 && ( time1 != MLoadSystemElapsedTime(C_TIME_MINUTE) || (mode == 6 || mode == 7) ) )
      {
          GetSystemTime(&TimeClock);
          TimeClock.date = tcm.date;
          TimeClock.day = tcm.day;
          TimeClock.month = tcm.month;
          TimeClock.yearh = tcm.yearh;
          TimeClock.yearl = tcm.yearl;
          MCopyProtectedMemory( (void*)time2, &TimeClock, 8, PM_COPY_WRITE);
      }
      else
      {
          //�����µ�һ����
          MCopyProtectedMemory( &TimeClock, (void*)time2, 8, PM_COPY_READ);
      }*/
    GetSystemTime(&TimeClock);
    hour = Bcd2Hex(TimeClock.hour & 0x3f)%24;
    minute = Bcd2Hex(TimeClock.minute  & 0x7f)%60;
    second = Bcd2Hex(TimeClock.second  & 0x7f)%60;

    year = Bcd2Hex( TimeClock.yearh &0x3f) * 100 + Bcd2Hex(TimeClock.yearl) ;
    month = Bcd2Hex(TimeClock.month & 0x1f )%13;
    date = Bcd2Hex(TimeClock.date & 0x3f )%32;	/* ����  BCD��ת����ʮ������ */
    day = Bcd2Hex(TimeClock.day & 0x3f)%8;
    if( month == 0) month = 1;
    if( day == 0 ) day = 1;

    //time1 = ( year * 256 + month) *256 + date;
    time1 = date2int(&TimeClock);
    time2 = ( hour * 256 + minute ) * 256 + second;
    switch( mode )
    {
    case 0://��
        retvalue = year;
        break;
    case 1://��
        retvalue = month;
        break;
    case 2://��
        retvalue = date;
        break;
    case 3://�ܼ�
        retvalue = day;
        break;
    case 4://Сʱ��
        retvalue = hour;
        break;
    case 5://���ӣ�
        retvalue = minute;
        break;
    case 6://��
        retvalue = second;
        break;
    case 7://ʱ����
        retvalue = time2;
        break;
    case 8://ʱ��
        retvalue = time2;
        break;
    case -1://������
        retvalue = time1;
        break;
    case -2://2000���ڼ��գ�
        retvalue = MGetYearDateCount(2000,year) + MGetCurrentYearTimeCount(time1,time2,2);
        break;
    case -3://2000���ڼ��ܣ�
        retvalue = ( MGetYearDateCount(2000,year) + MGetCurrentYearTimeCount(time1,time2,2) )/7;
        break;
    case -4://����ڼ��գ�
        MGetCurrentYearTimeCount(time1,time2,2);
        break;
    case -5://����ڼ���
        retvalue = MGetCurrentYearTimeCount(time1,time2,2)/7;
        break;
    case -6://��5���쿪ʼ����ٷ���
        retvalue = hour * 60 + minute;
        break;
    case -7://���쿪ʼ�������
        retvalue = (hour * 60 + minute) * 60;
        break;
    }

    return retvalue;

}
//�õ���ǰʱ��൱��1��1��0��Ĳ�࣬mode = 2���գ�3�ܣ�4Сʱ��5���ӣ�6��
u_int MGetCurrentYearTimeCount(int date, int time,int mode)
{
    u_int retvalue = 0;
    u_int year ;//��
    u_int month;//��
    u_int day;//��
    u_int hour = 0, minute = 0, second = 0;
    day = ( (date%65536)%256 )%32;
    month = ( (date%65536)/256 )%13 ;
    year = date/65536;
    if( day == 0 )day = 1;
    if( month == 0 ) month = 1;

    hour = ( ( time/65536 )%256 ) % 24;
    minute = ( ( time%65536 )/256 ) % 60;
    minute = ( ( time%65536 )%256 ) % 60;


    switch( month )
    {
    case 12:
        day += 30;
    case 11:
        day += 31;
    case 10:
        day += 30;
    case 9:
        day += 31;
    case 8:
        day += 31;
    case 7:
        day += 30;
    case 6:
        day += 31;
    case 5:
        day += 30;
    case 4:
        day += 31;
    case 3:
        if( ( year%4 == 0 && year%100 != 0) || ( year % 400 == 0) )day += 29;
        else day += 28;
    case 2:
        day += 31;
    case 1:
        break;
    }//���ϵõ���1��1�յ�����
    day--;

    switch( mode )
    {
    case 2://��
        retvalue = day;
        break;
    case 3://�ܼ�
        retvalue = day/7;
        break;
    case 4://Сʱ��
        retvalue = day*24+hour;
        break;
    case 5://���ӣ�
        retvalue = (day*24+hour)*60+minute;
        break;
    case 6://��
        retvalue = ( (day*24+hour)*60+minute )* 60 + second;
        break;
    }
    return retvalue;
}

u_int MGetYearDateCount( int year1, int year2)//�õ���year1����year2֮�������������2003��2004��365��
{
    u_int retvalue = 0;
    int i;
    for( i = year1 ; i < year2; i++)
    {
        if( ( i%4 == 0 && i%100 != 0) || ( i % 400 == 0) )
        {
            retvalue += 366;
        }
        else
        {
            retvalue += 365;
        }
    }
    return retvalue;
}

u_int MGet2kDays(int mode)
{
    u_int days;
    if( mode == 0 )days = SystemStatus.days;
    else
    {
        TIME_CLOCK TimeClock;
        u_int date, year, month;

        GetSystemTime(&TimeClock);
        date = TimeClock.date;
        year = TimeClock.yearh * 100 + TimeClock.yearl;
        month = TimeClock.month%13;
        days = MGetYearDateCount(2000,year);
        days += MGetCurrentYearTimeCount( ( year * 256 + month) *256 + date, 0, 2);//�õ����յ�����1��1�յ�����
    }
    return days;
}
void MSet2kDays(u_int days)//���õõ���2000���������
{
    SystemStatus.days = days;
}

u_int MGetDenoise()
{
    return SystemStatus.denoise;
}
void MSetDenoise(u_int denoise,int mode/*mode = 0\1\2*/)
{
    denoise %= 5;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        SystemStatus.denoise = denoise;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        DeNoise(denoise);
    }

}
u_int MGetRepeatRate()
{
    return SystemStatus.repeat;
}
void MSetRepeatRate(u_int repeat,int mode/*mode = 0\1\2*/)
{
//	repeat = (repeat/100)*100;
    if( repeat < 10)repeat = 10;
    if( repeat > 1000)repeat = 1000;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        SystemStatus.repeat = repeat;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        SetRepeatRate(repeat);
    }

}
u_int MGetPulseWidth()
{
    return SystemStatus.pulsewidth;
}
void MSetPulseWidth(u_int pulsewidth,int mode/*mode = 0\1\2*/)
{
    pulsewidth %= 2;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        SystemStatus.pulsewidth = pulsewidth;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        SetPulseWidth(_ParaMenu_CE2[pulsewidth]);
        SystemStatus.prepare[0] = _ParaMenu_CE2[pulsewidth];
    }

}
u_int MGetPulseWidthNumber()
{
    return SystemStatus.prepare[0];
}
void MSetPulseWidthInput(u_int pulsewidth,int mode/*mode = 0\1\2*/)
{
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        SystemStatus.prepare[0] = pulsewidth;
        if(pulsewidth<35)
            SystemStatus.pulsewidth = 0;
        else
            SystemStatus.pulsewidth = 1;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        SetPulseWidth(pulsewidth);
    }

}


///���ò����Զ���
u_int MGetDefPara(u_int iCh)
{
    return (SystemStatus.prepare[1]>>iCh)&0x01;
}
void MSetDefPara(u_int value,u_int iCh)
{

    u_int iBit=0x01;
    u_int iTemp=~(iBit<<iCh)&SystemStatus.prepare[1];
    SystemStatus.prepare[1]=iTemp+(value<<iCh);

}


void  MSet11345CurveNum(u_int iOn)
{
	ChannelStatus.prepare[0]=iOn;
}

u_int  MGet11345CurveNum()
{
	return ChannelStatus.prepare[0];
}


u_int MGetBandMode(void)	/*��������Ƶ����ȣ�2.5\5.0\10\wide	*/
{
    /*/
    u_int Ax = ChannelStatus.sys;

    Ax = Ax >> 30;
    Ax = Ax & 3;
    return Ax;
    //*/
    return BitLoad( ChannelStatus.sys, 2, 30)%4;
}
void MSetBandMode(u_int iBand,int mode/*mode = 0\1\2*/)
{
    /*
    u_int ax, bx, cx, dx;
    cx = 30;	//��λ
    dx = iBand & 0x03;
    bx = 0x3;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
    	bx = ~( bx << cx );	//=0011,1111,1111,1111,1111,1111,1111,1111
    	bx = ChannelStatus.sys & bx;
    	ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    	SetAmplifierBand(dx + AMP_BAND_NARROW_1);
    }
    //	*/
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelStatus.sys = BitSave( ChannelStatus.sys, 2, 30, iBand%4);
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB != 3
        SetAmplifierBand( iBand%4 + AMP_BAND_NARROW_1);
#endif
    }

}

u_int MGetEchoMode(void)	/*�������ü첨��ʽ��ȫ��\���벨\���벨\��Ƶ */
{
    return BitLoad( ChannelStatus.sys, 2, 28);
    //u_int Ax = ChannelStatus.sys;

    //Ax = Ax >> 28;
    //Ax = Ax & 3;
    //return Ax;
}
void MSetEchoMode(u_int Wave,int mode/*mode = 0\1\2*/)
{
    /*
    u_int ax, bx, cx, dx;
    cx = 28;	//��λ
    dx = Wave & 0x03;
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
    	bx = ~( bx << cx );
    	bx = ChannelStatus.sys & bx;
    	ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
     	SetEchoMode(dx + FULL_WAVE) ;
    }
    //*/
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelStatus.sys = BitSave( ChannelStatus.sys, 2, 28, Wave%4);
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB == 4
        SetEchoMode( MGetChannel(), Wave%4  + FULL_WAVE ) ;
#elif C_DEVLIB == 3
        if( Wave == 3 )SetEchoMode( RF_WAVE );
        else SetEchoMode( FULL_WAVE ) ;
#else
        SetEchoMode( Wave%4  + FULL_WAVE ) ;
#if C_DEVLIB == 23 || C_DEVLIB == 24
        if( Wave%4 == 3)SetEchoBaseLine( c_RFBaseLine);
        else SetEchoBaseLine( c_FWBaseLine);
#endif
#endif
        //  MSetFunctionMode(0,C_DEPTH_COMPENSATE);
    }

}//5767736,gao

u_int MGetDampMode(void)	/*��������̽ͷ���᣺50Q\400Q 	*/
{
    return BitLoad( ChannelStatus.sys, 1, 26);
    /*
    	u_int Ax = ChannelStatus.sys;

    	Ax = Ax >> 26;
    	Ax = Ax & 1;//Ŀǰֻ������
    	return Ax;
    	*/
}

void MSetDampMode(u_int iDamp,int mode/*mode = 0\1\2*/)
{
    /*
    u_int ax, bx, cx, dx;
    cx = 26;	//��λ
    dx = iDamp & 0x01;//Ŀǰֻ������
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
    	bx = ~( bx << cx );
    	bx = ChannelStatus.sys & bx;
    	ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    	SetProbeDamping(dx + LOW_DAMPING) ;
    }
    //*/
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelStatus.sys = BitSave( ChannelStatus.sys, 1, 26, iDamp%2);
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB == 4
        SetProbeDamping( MGetChannel(), (iDamp+1)%2  + LOW_DAMPING) ;
#else
        SetProbeDamping( (iDamp+1)%2  + LOW_DAMPING) ;
#endif
    }

}

u_int MGetPulseMode(void)	/*�������÷����ѹ��200V\400V  	*/
{
    u_int Ax = ChannelStatus.sys;

    Ax = Ax >> 24;
    Ax = Ax & 0x03;//Ŀǰֻ������
    return Ax;
}
void MSetPulseMode(u_int Pulse,int mode/*mode = 0\1\2*/)
{
    u_int ax, bx, cx, dx;
    cx = 24;	//��λ
#if C_DEVLIB == 23 || C_DEVLIB == 24
    dx = Pulse & 0x03;//Ŀǰֻ��4��
#else
    dx = Pulse & 0x01;//Ŀǰֻ������
#endif
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.sys & bx;
        ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB == 23 || C_DEVLIB == 24
        if( dx == 3)dx = 4;
        SetTransmitterVoltage(dx + LOW_PULSE) ;

#elif C_DEVLIB != 3
        SetTransmitterVoltage(dx + LOW_PULSE) ;
#else
        if( dx > 0)SetTransmitterVoltage(HIGH_PULSE) ;
        else SetTransmitterVoltage(LOW_PULSE) ;
#endif
    }
}
u_int MGetProbeMode(void)	/*����ֵΪ��ǰ̽ͷģʽ1\2\3\4\5=ֱ\б\˫��\��͸\˫��б*/
{
    u_int Ax = ChannelStatus.sys;

    Ax = Ax >> 12;
    Ax = Ax & 0x07;//Ŀǰֻ����λ,4
    return Ax;
}
void MSetProbeMode(u_int iProbe,int mode/*mode = 0\1\2*/)	/*����̽ͷģʽ*/
{
    u_int ax, bx, cx, dx;
    cx = 12;	//��λ
    dx = iProbe & 0x07;//
    ax = dx << cx;
    bx = 0x07;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.sys & bx;
        ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemProbe();
    }
}

u_int MGetStdMode(void)	/*�������ñ�׼	*/
{
    u_int Ax = ChannelStatus.sys;

    Ax = Ax >> 8;
    Ax = Ax & 0xf;
    return Ax;

}
void MSetStdMode(u_int iStd,int mode/*mode = 0*/)
{
    u_int ax, bx, cx, dx;
    cx = 8;	//��λ
    dx = iStd & 0x0f;//
    ax = dx << cx;
    bx = 0xf;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.sys & bx;
        ChannelStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}


u_int MGetFunctionMode(u_int type)
//�õ�type��ָ���ܵĵ�ǰ״̬,���й���ֻ������1��ر�0����״̬,
{
    u_int ax = ChannelStatus.sys;

    ax = ( ax >> type );

    ax = ax & 1;
    return ax;

}

void MSetFunctionMode(u_int func,u_int type)	//����type��ָ���ܵ�״̬
{
    u_int ax = func%2;		//ż��Ϊ�أ�����Ϊ��
    u_int bx = 0xfffffffe;
    int i;

    if( type == C_FUNCTION_ALL && func == 0)
    {
        ChannelStatus.sys &= 0xffffff00;//���8λΪ0
    }
    else
    {
        ax = ( ax << type );
        for(i = 0; i < type; i++)
        {
            bx = ( bx << 1);
            bx++;
        }
        ChannelStatus.sys &= bx;
        ChannelStatus.sys += ax;
    }
    if( type == C_ECHOMAX_MEMORY || type == C_ECHO_ENVELOPE )
        ClearEnvelope();
    switch (type)
    {
    case C_ECHO_ENVELOPE:
        //	if( MGetFunctionMode(type) == 1)
        //		MSetFunctionMode(0,C_ECHOMAX_MEMORY);
//		ActivateEchoEnvelope( MGetFunctionMode(C_ECHO_ENVELOPE)*2-1 ) ;
        break;
    case C_ECHOMAX_MEMORY:		//��ֵ����
        //	if( MGetFunctionMode(type) == 1)
        //		MSetFunctionMode(0,C_ECHO_ENVELOPE);
        break;
    case C_GATE_ALARM:
        break;
    case C_DAC_GATE:
        if( func == 1 )
        {
            if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0  )	//���ز���״̬<0=δ��>=0�Ѳ⣬
                MSetFunctionMode(0,C_DAC_GATE);
            //	else MSetFunctionMode(0,C_DEPTH_COMPENSATE);
        }
        break;
    case C_DEPTH_COMPENSATE:
#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)

        /* if( func != 1 )
         {
             for(i = 0; i < C_COORWIDTH; i++)
             {
                 dactable[i] = 160;
             }
         }*/
        MActivateDac(func);
#endif
        if( func == 1 )
        {
            if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0  )	//���ز���״̬<0=δ��>=0�Ѳ⣬
                MSetFunctionMode(0,C_DEPTH_COMPENSATE);
        }
        break;
    case C_AUTOADJUST_GAIN:
        break;
    case C_CURVER_AMEND:
        break;
    }
}

u_int MGetDacMode(void)	/*�������õ�����׼��ĸ��\�з�\����\����	*/
{
    int Ax = ChannelStatus.option;

    Ax = (Ax >> 28);
    Ax = ( Ax & 0x0f ) % 7;
    return Ax;

}
void MSetDacMode(u_int iDac,int mode/*mode = 0*/)
{
    u_int ax, bx, cx, dx;
    cx = 28;	//��λ
    dx = (iDac%7) & 0x0f;
    ax = dx << cx;
    bx = 0xf;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.option & bx;
        ChannelStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }

}

u_int MGetAvgMode(void)	/*�������õ�����ʾ��dBֵ\�׾�	*/
{
    int Ax = ChannelStatus.option;

    Ax = (Ax >> 26);
    Ax = ( Ax & 0x03 ) ;
    return Ax;

}
void MSetAvgMode(u_int iAvg,int mode/*mode = 0*/)
{
    u_int ax, bx, cx, dx;
    cx = 26;	//��λ
    dx = iAvg & 0x03;
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.option & bx;
        ChannelStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}

int MGetEquivalentMode(void)		//������ʾ��ʽ��<0���ߣ�DAC:1ĸ��2RL3SL4EL5-7,AVG:10dBֵ11�׾�
{
    int mode;
    if(MGetTestStatus(C_TEST_DAC) == 1) mode = MGetDacMode()+1;
    else if( MGetTestStatus(C_TEST_AVG) == 1)
    {
        //mode = MGetAvgMode()+10;//MGetAvgMode()=0dB,1�׾�
        mode = 1+10;//Ĭ�Ͽ׾�
    }
    else mode = -1;
    return mode;
}

int MGetScaleMode(void)	/*����ֵΪ��ǰ���0\1\2=��ֱ\ˮƽ\����*/
{
    int Ax = ChannelStatus.option;		/*��ȣ���ֱ\ˮƽ\����2λ	d3d2*/

    Ax = (Ax >> 24);
    Ax = Ax & 3;
    return Ax;
}

void MSetScaleMode(int scale,int mode/*mode = 0*/)
{
    u_int ax, bx, cx, dx;
    cx = 24;	//��λ
    dx = (scale%3) & 0x03;
    if(MGetAngle(0) == 0 && dx == 1)dx++;//ֱ̽ͷ��ˮƽ���
    if(MGetAngle(0) == 90 && dx == 2)dx--;//ֱ̽ͷ��ˮƽ���
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.option & bx;
        ChannelStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}

int MGetParaLock(int mode,int type)	//���ز�������״��0δ��1����,mode=1����ʾ
{
    int Ax = ChannelStatus.option;

    type %= ( C_LOCK_ALL + 1);
    Ax = (Ax >> (16+type) );//d23-d16������
    Ax = Ax & 0x1;
    if(mode == 1 && Ax == 1)DisplayPrompt(7);
    return Ax;
}
void MSetParaLock(int status, int type)	//���ز�������״��0δ��1������
{
    u_int ax, bx, cx;
    int i;

    status = status%2;		//ż��Ϊ�أ�����Ϊ��
    type %= ( C_LOCK_ALL + 1);
    bx = 0x1;

    if( type < C_LOCK_ALL)
    {
        cx = type+16;	//��λ//d23-d16������
        ax = status << cx;

        bx = ~( bx << cx );
        bx = ChannelStatus.option & bx;
        ChannelStatus.option = ax + bx;
        //MSetParaLock(!status,C_LOCK_ALL);
    }
    else
    {
        for( i = 0; i < C_LOCK_ALL; i++)MSetParaLock(status,i);
    }
}

int MGetBGateMode()
{
    return ( (ChannelStatus.option >> 4 ) & 0x1 );
}
void MSetBGateMode(int bgate,int mode)	//����B�ţ�bgate=0ʧ��1����
{
    u_int ax, bx, cx, dx;
    cx = 4;	//��λ
    dx = bgate & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = ChannelStatus.option & bx;
        ChannelStatus.option = ax + bx;
    }
}

int MGetParaStep(u_int paraindex)	//�õ������ڲ�������
{
    int step = 0;
//	u_int bx;
    switch (paraindex)
    {
    case C_BASEGAIN:
        step =  ( ChannelStatus.option >> 14 )&0x3 ;
        if(step == 1)step = 10;
        else if(step == 2) step = 20;
        else if(step == 3)step = 60;
        else step = 1;
        break;
    case C_COMPGAIN:		//d7d6
        step =  ( ChannelStatus.option >> 12 )&0x3 ;
        if(step == 1)step = 1;
        else if(step == 2) step = 20;
        else if(step == 3)step = 60;
        else step = 0;
        break;
    case C_RANGE:	//d5
        step = (  ChannelStatus.option >> 11 ) & 0x1;
        step++;
        break;
    case C_DELAY:
        step = (  ChannelStatus.option >> 10 ) & 0x1;
        step++;
        break;
    case C_SPEED:
        step = (  ChannelStatus.option >> 9 ) & 0x1;
        step++;
        break;
    }

    return step;
}

int ParaStepChange(u_int paraindex)	//���ô����ڲ�������
{
    int step = 0;
    int bx,cx;

    switch (paraindex)
    {
    case C_BASEGAIN:
        cx = 14;
        bx = 0x3;
        step =  ( ( ( ChannelStatus.option >> cx ) & bx ) + 1 ) &bx ;
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ( step << cx );
        if(step == 1)step = 10;
        else if(step == 2) step = 20;
        else if(step == 3)step = 60;
        else step = 1;
        break;
    case C_COMPGAIN:		//d7d6
        cx = 12;
        bx = 0x3;
        step =  ( ( ( ChannelStatus.option >> cx ) & bx ) + 1 )&bx ;
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ( step << cx );
        if(step == 1)step = 1;
        else if(step == 2) step = 20;
        else if(step == 3)step = 60;
        else step = 0;
        break;
    case C_RANGE:	//d5
        cx = 11;
        bx = 0x1;
        step =  ( ( ( ChannelStatus.option >> cx ) & bx ) + 1 ) & bx;
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ( step << cx );
        step++;
        break;
    case C_DELAY:
        cx = 10;
        bx = 0x1;
        step =  ( ( ( ChannelStatus.option >> cx ) & bx ) + 1 ) & bx;
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ( step << cx );
        step++;
        break;
    case C_SPEED:
        cx = 9;
        bx = 0x1;
        step =  ( ( ( ChannelStatus.option >> cx ) & bx ) + 1 ) & bx;
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ( step << cx );
        step++;
        break;
    }
    //BuzzerOn(1) ;	 		/*����0.1 * 1 ��*/
    return step;
}

int MGetParaSort(int index)		//�����ڲ�����(��A/��B),ϵͳ����/��������
{
    //option,d8-d5�������d8:��׼����\�������棬d7:��A\��B��D6D5����λ\��\��
    int retvalue = 0;

    switch (index)
    {
    case C_BASEGAIN:
    case C_SURFGAIN:
    case C_COMPGAIN:		//d7
        retvalue = ( ChannelStatus.option >> 8 ) & 0x1;	//0ϵͳ1����
        break;
    case C_AGATEPOSI:
    case C_AGATEWIDE:
    case C_AGATEHIGH:
    case C_BGATEPOSI:
    case C_BGATEWIDE:
    case C_BGATEHIGH:
        retvalue = (  ChannelStatus.option >> 5 ) & 0x07;
        if(retvalue ==3 )retvalue = 0;
        else if(retvalue ==7 )retvalue = 4;
        //0A��λ1A�ſ�2A�Ÿ�4B��5B�ſ�6B�Ÿ�
        break;
    }

    return retvalue;
}
int MSetParaSort(int status,int index)		//�����ڲ�����(��A/��B),ϵͳ����/��������
{
    //option,d8-d5�������d8:��׼����\�������棬d7:��A\��B��D6D5����λ\��\��

    u_int ax,bx,cx;

    switch (index)
    {
    case C_BASEGAIN:
    case C_SURFGAIN:
    case C_COMPGAIN:		//d7d6
        cx = 8;
        bx = 0x1;
        ax = ( (status & bx) << cx);
        ChannelStatus.option = ( ChannelStatus.option & ( ~( bx << cx ) ) ) + ax;
        break;
    case C_AGATEPOSI:
    case C_AGATEWIDE:
    case C_AGATEHIGH:
    case C_BGATEPOSI:
    case C_BGATEWIDE:
    case C_BGATEHIGH:
        cx = 5;
        bx = 0x7;
        ax = ( (status & bx) << cx);
        ChannelStatus.option = ( ( ~( bx << cx ) ) & ChannelStatus.option ) + ax;
        //0A��λ1A�ſ�2A�Ÿ�4B��5B�ſ�6B�Ÿ�
        break;
    }

    return 1;
}

int MGetCurveNumber(void)	//�õ����ߵ���Ŀ
{
    u_int number;
    number = ( ( ChannelStatus.option & 0x07) % 7 );//������6��
    return number;
}
void MSetCurveNumber(int number)//�������ߵ���Ŀ
{
    ChannelStatus.option &= 0xfffffff8;	//d2-d0=0
    ChannelStatus.option += (number % 8);
}

int MGetBlock(void)//�õ��Կ�����
{
    return ( ChannelStatus.block & 0x1f );
}

void MSetBlock(int block)//�����Կ�����
{
    ChannelStatus.block &= 0xe0;//d4-d0=0
    ChannelStatus.block += ( block & 0x1f);
}

int MGetEquivalentDays(int mode)//��������,mode = 0�������ܣ�1���/K/dac/avg,2������
{
    int ax = 0, bx = 0 ,cx = 0;
    switch( mode)
    {
    case 0:
        bx = 0xff;
        cx = 24;
        break;
    case 1:
        bx = 0x3f;
        cx = 18;
        break;
    case 2:
        bx = 0xf;
        cx = 14;
        break;
    }
    ax = ( (ChannelStatus.status >> cx) & bx );
    //ax = ( ax > 0)? ax : -1;
    return ax;
}
//	u_int status;		//d31-d24�������ܲ���������d23d22������Ŀ���������/Kֵ/DAC\AVG,D21-d16̽ͷ��������(��㡢Kֵ��DAC)��
//d15��d12�����Ȳ�������
void MSetEquivalentDays(int mode,int days)//��������,mode = 0�������ܣ�1���/K/dac/avg,2������
{
    int ax = 0, bx = 0 ,cx = 0,dx = 0;

    if(days == 0)days = 0x7fff;	//�����0�죬����Ϊ���
    switch( mode)
    {
    case 0://��������
        bx = 0xff;
        cx = 24;
        days = ( days > C_TESTDAYSMAX_0)? C_TESTDAYSMAX_0:days;
        break;
    case 1:
        bx = 0x3f;
        cx = 18;
        days = ( days > C_TESTDAYSMAX_1)? C_TESTDAYSMAX_1:days;
        break;
    case 2:
        bx = 0xf;
        cx = 14;
        days = ( days > C_TESTDAYSMAX_2)? C_TESTDAYSMAX_2:days;
        break;
    }
    dx = days & bx;//Ŀǰֻ������
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = ChannelStatus.status & bx;
    ChannelStatus.status = ax + bx;
}
int MGetAmpStdMax(void)
{
    return 2*( ( (ChannelStatus.status >> 7 ) & 0x7f )%100 );
}
void MSetAmpStdMax(int amp)//�����Զ�����ʱ��׼����
{
    u_int ax, bx, cx, dx;
    cx = 7;	//��λ
    bx = 0x7f;
    dx = ( amp & bx )%100;
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = ChannelStatus.status & bx;
    ChannelStatus.status = ax + bx;
}

int MGetTestStatus(u_int index)	//���ز���״̬0=δ��1=�Ѳ⣬
{
    int ax;
    index = index % C_TEST_MAX;
    ax = ( ChannelStatus.status >>index )&0x1;
    return ax;
}
void MSetTestStatus(u_int index,int status)	//���ò���״̬
{
    //C_TEST_OFFSET\ANGLE\DAC\AVG
    int ax, bx, cx;
    status &= 1;

    if( index == C_TEST_ALL && status == 0)
    {
        bx = 0x7f;
        bx = ~bx;
        bx = ChannelStatus.status & bx;
        ChannelStatus.status = bx;
    }
    else
    {
        if( index == C_MAKE_DACAVG)
        {
            int Zero = 0;
            int offset = C_OFF_DAC + 21*C_SIZE_INT;
            MCopyProtectedMemory((void*)offset,&Zero, 1*C_SIZE_INT, PM_COPY_WRITE);
        }
        cx = index%C_TEST_MAX;
        bx = 0x1;
        bx = ~( bx << cx );
        bx = ChannelStatus.status & bx;
        ax = status << cx;
        ChannelStatus.status = ax + bx;
    }
}


/*
��ز������йصĺ���
*/

void MGetGatePeakBuffer()
{
    int offset =  C_OFF_STORE + 40;		//ǰ�ζ��ز�����ʱ��ʱ��
    int pretime,curtime;

    if( MGetAcquisitionEnable() != 1 )return ;

    //pretimeΪǰ�ζ����β����ݿ���ʱ�䳤��
    MCopyProtectedMemory( &pretime, (void*)offset, C_SIZE_INT, PM_COPY_READ);

    curtime = GetElapsedTime();//��ǰ�����β����ݿ���ʱ�䳤��

    if(Mabsi(curtime - pretime) > 10 )//���10ms�Ŷ�һ��
    {
        MCopyProtectedMemory( (void*)offset, &curtime, C_SIZE_INT, PM_COPY_WRITE);

        GATE_PEAK* pGatePeak;
        //	MSetAcquisitionEnable(0,C_SETMODE_SET);
        MDacGatePeak();
        //	MSetAcquisitionEnable(1,C_SETMODE_SET);
        pGatePeak = GetGatePeakBuffer();

        if( MGetProbeMode() == C_P_TRANSMISSION)
        {
            GatePeak.Gate1PeakSampPoints = pGatePeak->Gate1PeakSampPoints<<1;
            GatePeak.Gate2PeakSampPoints = pGatePeak->Gate2PeakSampPoints<<1;
        }
        else
        {
            GatePeak.Gate1PeakSampPoints = pGatePeak->Gate1PeakSampPoints;
            GatePeak.Gate2PeakSampPoints = pGatePeak->Gate2PeakSampPoints;
        }

        {
            GatePeak.Gate1PeakPos = pGatePeak->Gate1PeakPos;
            if( g_nDenoise > 0)
            {
                GatePeak.Gate1PeakAmp = AdjustTime.amp = ( AdjustTime.amp + pGatePeak->Gate1PeakAmp )>>1 ;
            }
            else
            {
                GatePeak.Gate1PeakAmp = pGatePeak->Gate1PeakAmp ;
            }
            AdjustTime.amp_total = GatePeak.Gate1PeakAmp;
            AdjustTime.time = 1;
        }

        {
            GatePeak.Gate2PeakPos = pGatePeak->Gate2PeakPos ;
            if( g_nDenoise > 0)
            {
                GatePeak.Gate2PeakAmp = AdjustTime.amp2 = ( AdjustTime.amp2 + pGatePeak->Gate1PeakAmp )>>1 ;
            }
            else
            {
                GatePeak.Gate2PeakAmp = pGatePeak->Gate2PeakAmp ;
            }
            AdjustTime.amp_total2 = GatePeak.Gate2PeakAmp;
            AdjustTime.time2 = 1;
        }
        GatePeak.Gate1PeakAmp = AdjustTime.amp = AdjustTime.amp_total /AdjustTime.time;
        GatePeak.Gate2PeakAmp = AdjustTime.amp2 = AdjustTime.amp_total2 /AdjustTime.time2;
        if( AdjustTime.time > 100)AdjustTime.amp_total = AdjustTime.time = 0;
        if( AdjustTime.time2 > 100)AdjustTime.amp_total2 = AdjustTime.time2 = 0;
        //MSetAcquisitionEnable(1,C_SETMODE_SET);
    }

}


int MGetGateParaMax(int gatetype)
{

    int elapsedtime = GetElapsedTime(),elapsedtime1;
    //�õ�������ʷ��߻ز��Ĵ���ʱ�䡢ˮƽ�㡢���롢����,gatetype = 0��A�ţ�=1��B�ţ�
    short gateamp = MGetAmpMax(gatetype);
    elapsedtime1=GetElapsedTime();
    ExpendTime(2);
    // Write_Number(30,70,1,10,0,0);
    // Write_Number(30,100,elapsedtime1-elapsedtime,10,0,0);
    //    MAnyKeyReturn();
    int retvalue = C_FALSE;

    //        Write_Number(100,100,gateamp,4,0,0);
    //  MGetGatePeakBuffer() ;  	//����������ܵõ����ڲ���
    // gateamp = GatePeak.Gate1PeakAmp;
    // Write_Number(10,100,gateamp,4,0,0);
    // Write_Number(10,130,GateMax.Amp,4,0,0);
    if(gateamp > GateMax.Amp )
    {
        short pos,start,end;
        elapsedtime = GetElapsedTime();
        char *sampbuffer = GetSampleBuffer();
        elapsedtime1=GetElapsedTime();

        ExpendTime(2);
        // Write_Number(30,130,2,10,0,0);
        // Write_Number(30,160,elapsedtime1-elapsedtime,10,0,0);
        //   MAnyKeyReturn();

        GateMax.Amp = gateamp;
        GateMax.Time = GatePeak.Gate1PeakSampPoints ;//MGetAmpTime(gatetype);
        pos = start = MGetGatePara(gatetype,0);
        end = start + MGetGatePara(gatetype,1);
        for( ; start < end; start++)
        {
            if( *( sampbuffer + start ) > *( sampbuffer + pos ) )pos = start;
        }//����������߲�
        GateMax.Pos = pos;//MGetAmpPos(gatetype);
        GateMax.Dist = GateMax.Time * MGetSpeed() /(C_SAMPLE_FREQ) ;//MGetAmpDist(gatetype);
        retvalue = C_TRUE;
    }
    return retvalue;
}

void MSetGateParaInit(void)	//���ֵ
{
    GateMax.Amp = 0;
    GateMax.Time = 0;
    GateMax.Pos = 0;
    GateMax.Dist = 0;
}

int MGetAmpMax(int gatetype)	/*�õ�ָ���ţ�0=A or 1=B)�ڻز��߶ȣ�*/
{
    int gateamp = 0;
    int elapsedtime = GetElapsedTime(),elapsedtime1;
    //�õ�������ʷ��߻ز��Ĵ���ʱ�䡢ˮƽ�㡢���롢����,gatetype = 0��A�ţ�=1��B�ţ�
    //if(gatetype > 1)gatetype = 0;

    //if(MGetAcquisitionEnable() == 1 )
    {
        {
            MGetGatePeakBuffer() ;  	//����������ܵõ����ڲ���

            elapsedtime1=GetElapsedTime();
            //  Write_Number(10,190,3,10,0,0);
            //  Write_Number(10,210,elapsedtime1-elapsedtime,10,0,0);
            //  MAnyKeyReturn();
        }        /* get the pointer to kernel structure GATE_PEAK, don't modify! */
        elapsedtime = GetElapsedTime();
        if(gatetype == 0 )
        {
            //if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 )
            {
                //��DAC��
                gateamp = MGetAmpPos1(gatetype,11);//����
            }
            //else gateamp = GatePeak.Gate1PeakAmp;
        }
        else if(gatetype == 1 )
        {
            gateamp = MGetAmpPos1(gatetype,11);//����
        }
        else if(gatetype > 1)
        {
            gateamp = MGetAmpPos1(0,11);//����
            ///gateamp = GatePeak.Gate1PeakAmp;
#if C_DEVLIB == 23 || C_DEVLIB == 24
            if( MGetEchoMode() == C_RF_WAVE)
            {
                gateamp = (gateamp)*2;
            }
#else
            if( MGetEchoMode() == C_RF_WAVE)
            {
                gateamp -= 128;
                gateamp *= 2;
            }
#endif
#if C_AMPCOMP == 1
            if( Mabsi(gateamp) < 64*2 && Mabsi(gateamp) > 1)
            {
                u_short gain = MGetBaseGain() + MGetSurfGain() + MGetCompGain();
                gain /=128;
                if(gateamp > 0)gateamp += AmpMaxCom_Table[ gain * 8 + gateamp/16];
                else gateamp -= AmpMaxCom_Table[ gain * 8 + Mabsi(gateamp)/16];
            }
#endif
            int amp1 = gateamp;
            gateamp = MGetAmpPos1(1,11);//����
#if C_DEVLIB == 23 || C_DEVLIB == 24
            if( MGetEchoMode() == C_RF_WAVE)
            {
                gateamp = (gateamp)*2;
            }
#else
            if( MGetEchoMode() == C_RF_WAVE)
            {
                gateamp -= 128;
                gateamp *= 2;
            }
#endif
#if C_AMPCOMP == 1
            if( Mabsi(gateamp) < 64*2 && Mabsi(gateamp) > 1)
            {
                u_short gain = MGetBaseGain() + MGetSurfGain() + MGetCompGain();
                gain /=128;
                if(gateamp > 0)gateamp += AmpMaxCom_Table[ gain * 8 + gateamp/16];
                else gateamp -= AmpMaxCom_Table[ gain * 8 + Mabsi(gateamp)/16];
            }
#endif
            return (amp1 * 256 + gateamp);
        }

        elapsedtime1=GetElapsedTime();
        //  Write_Number(10,240,4,10,0,0);
        //  Write_Number(10,270,elapsedtime1-elapsedtime,10,0,0);
        //   MAnyKeyReturn();
#if C_DEVLIB == 23 || C_DEVLIB == 24
        if( MGetEchoMode() == C_RF_WAVE)
        {
            gateamp = (gateamp)*2;
        }
#else
        if( MGetEchoMode() == C_RF_WAVE)
        {
            gateamp -= 128;
            gateamp *= 2;
        }
#endif
        //Write_Number( 100,30,Mabsi(gateamp),4,0,0);
#if C_AMPCOMP == 1
        if( Mabsi(gateamp) < 64*2 && Mabsi(gateamp) > 1)
        {
            u_short gain = MGetBaseGain() + MGetSurfGain() + MGetCompGain();
            gain /=128;
            //	Write_Number(10,30,gateamp,4,0,0);
            //	Write_Number( 10, 40, gain * 8 + gateamp/16,4,0,0);
            //	Write_Number( 10, 50, AmpMaxCom_Table[ gain * 8 + gateamp/16],4,0,0);
            if(gateamp > 0)gateamp += AmpMaxCom_Table[ gain * 8 + gateamp/16];
            else gateamp -= AmpMaxCom_Table[ gain * 8 + Mabsi(gateamp)/16];
            //	Write_Number(10,60,gateamp,4,0,0);

        }
#endif
    }
    //else gateamp = GateMax.Amp;

    return gateamp;
}

int MGetAmpPos(int gatetype)	/*�õ�ָ���ţ�A or B)�ڻز�ˮƽ������*/
{
    if(gatetype > 1)return -1;
    int pos = 0;

///	if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 && gatetype == 0)
    {
        //��DAC��
        pos = MGetAmpPos1(gatetype,1);//
    }
//	else//*/
    return pos;
}
int MGetAmpPos1(int gatetype,int mode)	/*�õ�ָ�����ڻز�mode=0��ʼ/1���/2����3/ǰ�����ཻ/4������ཻ�ĵ�����*/
{
    if(gatetype > 1)return -1;
    short pos = 0;
    short start,end;
    u_char amp1,amp2;
    u_char * sampbuffer;

    MGetGatePeakBuffer() ;  	//����������ܵõ����ڲ���

//;	MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    sampbuffer = GetSampleBuffer();
    amp1 = amp2 = 0;
    pos = start = MGetGatePara(gatetype,0);
    end = start + MGetGatePara(gatetype,1);
    for( ; start < end; start++)
    {
        if( *( sampbuffer + start ) > *( sampbuffer + pos ) )pos = start;
    }//����������߲�
//*
    if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 && gatetype == 0)
    {
        //��DAC��
        short db1,db2;
        int offset;

        GatePeak.Gate1PeakPos = pos ;
        start = pos+1;
        end = MGetGatePara(gatetype,0) + MGetGatePara(gatetype,1);
        amp2 = 0;
        db2 = 0;
        for(  ; start < end; start++)
        {
            for( ; ( amp1 >= amp2 || amp1 < 10) && start < end; start++)
            {
                //һֱ�ȵ��ò��ν���
                amp1 = amp2;
                amp2 = sampbuffer[start];
            }
            for(  ; start < end; start++)
            {
                amp1 = amp2;
                amp2 = sampbuffer[start];
                if( amp1 > amp2 )break;
            }//�ҵ�һ����߲�
            start--;
            offset = 2*start + C_OFF_DACGATE;
            MCopyProtectedMemory( &db1, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
            if( amp1 > db1 )//����dac��
            {
                if( (amp1 - db1)*pos > db2*start )
                {
                    pos = start;
                    db2 = amp1 - db1;
                }
            }
            start++;
        }
        if( GatePeak.Gate1PeakPos != pos )
        {
            GatePeak.Gate1PeakSampPoints = (int)( (double)( ( (double)pos+0.5) * MGetRange(3) * (C_SAMPLE_FREQ) )/ (double)( MGetSpeed() * C_COORWIDTH )  + 0.5);
            GatePeak.Gate1PeakPos = pos ;
            amp1 = *( sampbuffer + pos );
            GatePeak.Gate1PeakAmp = amp1;
        }
        else MGetGatePeakBuffer() ;  	//����������ܵõ����ڲ���
    }
    switch( mode )
    {
    case 0://�������ε���ǰ��
        end = pos-1;
        amp2 = *(sampbuffer + end );
        start = MGetGatePara(gatetype,0);
        for( ; end > start; end--)
        {
            amp1 = amp2;
            amp2 = *(sampbuffer + end );
            if( amp1 < amp2 || amp2 == 0)break;
        }
        pos = end+1;
        break;
    case 1://�������ε���ߵ�
        break;
    case 2://�������ε�����
        start = pos+1;
        amp2 = *(sampbuffer + start );
        end = MGetGatePara(gatetype,0) + MGetGatePara(gatetype,1);
        for( ; start < end ; start++)
        {
            amp1 = amp2;
            amp2 = *(sampbuffer + start );
            if( amp1 < amp2 || amp2 == 0)break;
        }
        pos = start - 1;
        break;
    case 3://��������ǰ�������ཻ�ĵ�
        end = pos-1;
        start = MGetGatePara(gatetype,0);
        amp1 = MGetGatePara(gatetype,2);//�Ÿ�
        for( ; end > start; end--)
        {
            amp2 = *(sampbuffer + end );
            if( amp1 > amp2 )break;
        }
        pos = end+1;
        break;
    case 4://�������κ�������ཻ�ĵ�
        start = pos+1;
        end = MGetGatePara(gatetype,0) + MGetGatePara(gatetype,1);
        amp1 = MGetGatePara(gatetype,2);//�Ÿ�
        for( ; start < end ; start++)
        {
            amp2 = *(sampbuffer + start );
            if( amp1 > amp2)break;
        }
        pos = start - 1;
        break;
    case 5://��������һ��ĵ�
        if( *(sampbuffer + pos + 1)  > *(sampbuffer + pos - 1) )pos++;
        break;

    case 10://ʱ��
        if( MGetEchoMode() != C_RF_WAVE)
        {
            if( gatetype == 0)
            {
                pos = GatePeak.Gate1PeakSampPoints;
            }
            else pos = GatePeak.Gate2PeakSampPoints;
        }
        else
        {
            int offset =  C_OFF_TEMP+42;
            int Range;
            MCopyProtectedMemory( &Range, (void*)offset, C_SIZE_INT, PM_COPY_WRITE);
            pos = 2*pos * Range/C_COORWIDTH;
        }
        break;
    case 11://����
        if( gatetype == 0)
        {
            amp1 = GatePeak.Gate1PeakAmp;
            //   Write_Number(10,50,amp1,10,0,0);
        }
        else amp1 = GatePeak.Gate2PeakAmp;
        //  if( MGetSystemMode() == 0 && MGetFunctionMode(C_DEPTH_COMPENSATE))
        {
            //��DAC����
            //amp1 = amp1 * dactable[0] / dactable[pos] ;
        }
        pos = amp1;
        break;
    }
///Write_Number(10+40*mode,30+10*gatetype,pos,4,0,0);
//;	MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);

    return pos;
}
int MGetAmpTime(int gatetype)	/*�õ�ָ���ţ�A or B)�ڻز�����ʱ�䣬*/
{
    if(gatetype > 1)return -1;
    int AmpTime = 0;

    {
        MGetGatePeakBuffer() ;  	//����������ܵõ����ڲ���
        if( MGetEchoMode() != C_RF_WAVE)
        {
// TODO (lzg#1#): ����ƫ����Ϊ�ļ�ȥ��2000������
            if(gatetype == 0 )AmpTime = GatePeak.Gate1PeakSampPoints;//-2000 ;
            else if(gatetype == 1 )AmpTime = GatePeak.Gate2PeakSampPoints ;
        }
        else
        {
            AmpTime = MGetAmpPos1( gatetype, 1);
            AmpTime = (int)( (double)(AmpTime * MGetRange(3)/C_COORWIDTH ) * (C_SAMPLE_FREQ) /  (double)MGetSpeed() ) ;;
        }
    }
//	if ((MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) && MGetFunctionMode(C_DAC_GATE) == 1 && gatetype == 0)
    {
        //��DAC��
//		AmpTime = MGetAmpPos1(gatetype,10);//ʱ��
    }
    //else//*/
    if( MGetReadForeland() > 0 && MGetAmpMax(gatetype) >= MGetGatePara(gatetype,2))
    {
        AmpTime = MGetAmpPos1( gatetype, 3);//��������ǰ�������ཻ�ĵ�
        AmpTime = ( AmpTime * MGetRange(3)/C_COORWIDTH ) * (C_SAMPLE_FREQ) /  MGetSpeed()  ;
    }


    if( MGetProbeMode() == C_P_TRANSMISSION )//��͸
    {
        //	AmpTime <<= 2;
    }
    return AmpTime;

}
int Amp2db(int pos,int amp)		//���ݻز�λ�úͲ��ߵõ�dBֵ
{
    u_short dac_db[C_COORWIDTH];
    int offset;

    if(amp<1)amp=1;
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_db, (void*)offset, C_COORWIDTH*C_SIZE_SHORT, PM_COPY_READ);

    int db;

    db = MGetBaseGain() + MGetCompGain();
    db += (int)( 200 * ( log10(255) - log10(amp) ) );	//�軯Ϊ128%=255ʱ������ֵ
    //return (db - dac_db[pos]);
    if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 1 )
        return ( dac_db[0] - db );
    else
        return (dac_db[pos]- db);
}

int MGetAmpDist(int gatetype)
{
    if( gatetype == -1) return ( GatePeak.Gate1PeakSampPoints * MGetSpeed() /(C_SAMPLE_FREQ)  );
    else if( gatetype == -2) return ( GatePeak.Gate2PeakSampPoints * MGetSpeed() /(C_SAMPLE_FREQ)  );
    else if( gatetype >=0 && gatetype < 2)	return MGetAmpTime(gatetype) * MGetSpeed() /(C_SAMPLE_FREQ) ;
    else return -1;
}
int MGetAmpDist1(int gatetype)
{
    double dist;
    if( gatetype == -1) dist = ( MGetAmpTime(0) * MGetSpeed() /(double)(C_SAMPLE_FREQ)  );
    else if( gatetype == -2) dist =  ( MGetAmpTime(1) * MGetSpeed() /(double)(C_SAMPLE_FREQ)  );
    else if( gatetype >=0 && gatetype < 2)	dist = MGetAmpTime(gatetype) * MGetSpeed() /(double)(C_SAMPLE_FREQ) ;
    else return -1;
    dist += MGetDelay(3);
    gatetype = (int)( dist + 0.5);
    if( (gatetype)%5 < 1);
    else if( (gatetype+2)%5 < 2)gatetype++;
    else if( gatetype > 1)gatetype--;
    return gatetype;
}

//����ϵͳ״̬����
u_int MLoadSystemStatus( int mode)//ȡϵͳ״̬�е�ĳ������,mode=0-n,����ĸ˳����ϵ���
{
    switch(mode)
    {
    case 0:
        return SystemStatus.days;
    case 1:
        return SystemStatus.nElapsedTime;
    case 2:
        return SystemStatus.nStartElapsedTime;
    case 3:
        return SystemStatus.option;
    case 4:
        return SystemStatus.sys;
    }
    return 0;
}
void MSaveSystemStatus( int mode, u_int para)
{
    switch(mode)
    {
    case 0:
        SystemStatus.days = para;
        break;
    case 1:
        SystemStatus.nElapsedTime = para;
        break;
    case 2:
        SystemStatus.nStartElapsedTime = para;
        break;
    case 3:
        SystemStatus.option = para;
        break;
    case 4:
        SystemStatus.sys = para;
        break;
    }
}


u_int MGetPrinterMode(void)	/*����ֵΪ��ǰ��ӡ������1\2\3\4=LQ\HP\EPSON C/P*/
{
    u_int ax = SystemStatus.sys;	/**/
    ax = ax >> 29;
    ax = ax & 3;//Ŀǰֻ��3��
    return ax;
}
void MSetPrinterMode(u_int iPrinter,int mode/*mode = 0*/)	/*���õ�ǰ��ӡ������*/
{
    u_int ax, bx, cx, dx;
    cx = 29;	//��λ
    dx = iPrinter & 0x03;
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}

u_int MGetInputMode(void)	/*����ֵΪ��ǰ���뷨����1\2\3\4\5=����\��д\Сд\ƴ��\��λ*/
{
    u_int ax = SystemStatus.sys;	/**/
    ax = ax >> 26;
    ax = ax & 7;
    return ax;
}
void MSetInputMode(u_int iInput,int mode/*mode = 0*/)	/*���õ�ǰ���뷨����*/
{
    u_int ax, bx, cx, dx;
    cx = 26;	//��λ
    dx = iInput & 0x07;
    ax = dx << cx;
    bx = 0x7;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
u_int MGetSystemMode(void)	/*����ֵΪ��ǰϵͳ״̬0\1=����\����*/
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 25;
    ax = ax & 1;
    return ax;
}
void MSetSystemMode(u_int iSystem ,int mode/*mode = 0*/)	/*���õ�ǰϵͳ״̬*/
{
    u_int ax, bx, cx, dx;
    cx = 25;	//��λ
    dx = iSystem & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        EnableDataAcquisition(dx);
        if (dx==1)
        {
            ExpendTime(20);
            EnableDataAcquisition(dx);

            while (IfDataAcquisitionActivated() == 0)
            {
                ExpendTime(1);
            }
        }
    }
}

u_int MGetAcquisitionEnable(void)	/*�Ƿ��������d5*/
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 24;
    ax = ax & 1;
    return ax;
}
void MSetAcquisitionEnable(u_int acquisition,int mode/*mode = 0\1\2*/)
{
    u_int ax, bx, cx, dx;
    cx = 24;	//��λ
    dx = acquisition & 0x01;
    ax = dx << cx;
    bx = 0x1;

    //;if( MGetAcquisitionEnable() == dx)return;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        EnableDataAcquisition(dx);

        if (dx==1)
        {
            ExpendTime(20);
            EnableDataAcquisition(dx) ;
            while (IfDataAcquisitionActivated() == 0)
            {
                ExpendTime(1);
            }

        }
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
        //    if( dx && MGetFunctionMode(C_DEPTH_COMPENSATE) )MActivateDac( MGetFunctionMode(C_DEPTH_COMPENSATE) );
        //   else if( !dx && MGetFunctionMode(C_DEPTH_COMPENSATE) )MActivateDac( 0 ) ;
#endif
    }
}
void MSetAcquisition(u_int mode)	/*���ò������0������1����*/
{
    mode %= 2;
    //if( MGetAcquisitionEnable() != mode)
    {
        MSetAcquisitionEnable(mode,C_SETMODE_SETSAVE/*mode = 0\1\2*/);
    }
    //if( MGetHardEchoShow() != mode)
    {
        MSetHardEchoShow(mode,C_SETMODE_SETSAVE/*mode = 0\1\2*/);
    }

}
u_int MGetFill(void)//��ʾӲ������ʵ�Ļ��ǿ���
{
    return SystemStatus.fill;
}
void MSetFill(u_int fill,int mode/*mode = 0\1\2*/)
{
    fill %= 2;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        SystemStatus.fill = fill;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        //EnableEchoDisplay(MGetHardEchoShow(),fill) ;
        //EnableEchoDisplay(1) ;
        SetEchoDisplayMode(fill,MGetEchoMode()/3) ;
    }

}
u_int MGetHardEchoShow(void)//��ʾӲ������
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 23;
    ax = ax & 1;
    return ax;
}
void MSetHardEchoShow(u_int show, int mode)
{
    u_int ax, bx, cx, dx;
    cx = 23;	//��λ
    dx = show & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        //	if( MGetHardEchoShow() == dx)return ;
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        EnableEchoDisplay(dx) ;
        SetEchoDisplayMode(MGetFill(),MGetEchoMode()/3) ;
        //EnableEchoDisplay(1) ;
    }
}
u_int MGetSoftEchoShow(void)//��ʾ�������
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 22;
    ax = ax & 1;
    return ax;
}
void MSetSoftEchoShow(u_int show , int mode)
{
    u_int ax, bx, cx, dx;
    cx = 22;	//��λ
    dx = show & 0x01;
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
u_int MGetCursorEnable(void)//�Ƿ������й��
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 21;
    ax = ax & 1;
    return ax;
}
void MSetCursorEnable(u_int cursor,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 21;	//��λ
    dx = cursor & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}

u_int MGetCursorShow(void)//�Ƿ���ʾ���
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 20;
    ax = ax & 1;
    return ax;
}
void MSetCursorShow(u_int cursor,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 20;	//��λ
    dx = cursor & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}

u_int MGetChannel(void)	/*���ط�����ǰͨ����*/
{
    u_int ax = SystemStatus.sys;	/**/

    ax = ax >> 12;
    ax = ax & 0xff;
    if (ax >= ChannelMax )ax = 0;
    return ax;
}
int MSetChannel(int channel,int mode/*mode = 0*/)
{
    u_int ax, bx, cx, dx;
    cx = 12;	//��λ
    dx =  channel% ChannelMax;

    ax = dx << cx;
    bx = 0xff;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
    return C_TRUE;
}


u_int MGetSoundAlarm(int mode)//mode = 0������1����
{
    u_int ax = SystemStatus.sys;	/**/

    if( mode == 0)ax = ax >> 11;
    else if( mode == 1)ax = ax >> 10;
    ax = ax & 1;
    return ax;
}

void MSetSoundAlarm( int mode, int alarm )//mode = 0������1����
{
    u_int ax = 0, bx = 0 ,cx = 0,dx = 0;
    if( mode == 0)cx = 11;
    else if( mode == 1)cx = 10;
    dx = alarm & 0x01;
    ax = dx << cx;
    bx = 0x1;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.sys;
        SystemStatus.sys = ax + bx;
    }
}

u_int MGetScreenBrightness(void)	/*����ֵΪ��ǰ��Ļ����*/
{
    //u_int ax = SystemStatus.sys;	/**/

    //ax = ax >> 8;
    //ax = ax & 0x3;
    //return ax;
    return BitLoad(SystemStatus.sys, 2 , 8);
}
void MSetScreenBrightness(u_int iBrightness,int mode/*mode = 0\1\2*/)		/**/
{
    //u_int ax, bx, cx, dx;
    //cx = 8;
    //dx = iBrightness & 0x03;
    //ax = dx << cx;
    //bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        //bx = ~( bx << cx );
        //bx = bx & SystemStatus.sys;
        //SystemStatus.sys = ax + bx;
        SystemStatus.sys = BitSave(SystemStatus.sys, 2, 8, iBrightness);
    }
    if( mode == C_SETMODE_SET || mode == C_SETMODE_SETSAVE)
    {
        MSetSystemBright();
    }
}

int MGetInputEnable(int type)
{
    u_int ax = SystemStatus.sys;	/**/
    type = ( type - 1)%C_MAX_INPUTMODE;
    ax = ax >> (0+ type-1);
    ax = ax & 0x1;
    return ax;
}
void MSetInputEnable(int bEnable,int type)//����ĳ�����뷨�Ƿ��ѡ
{
    u_int ax, bx, cx, dx;
    type %= ( C_INPUT_ALL + 1);
    cx = 0 + type ;
    dx = bEnable & 0x01;
    ax = dx << cx;
    bx = 0x1;

    bx = ~( bx << cx );
    bx = bx & SystemStatus.sys;
    SystemStatus.sys = ax + bx;
}

int MGetSaveMode(void)		//�洢ģʽ0��1����
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>31;
    ax = ax & 0x1;
    return ax;
}
int MSetSaveMode(int style,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 31;
    dx = style & 0x01;
    ax = dx << cx;

    if( ax == 1)
    {
        int offset = C_OFF_STORE +	34;		//���Զ��洢����
        int autosavenum = 0;
        MCopyProtectedMemory( (void*)offset, &autosavenum, C_SIZE_INT, PM_COPY_WRITE);
    }
    MSetSaveStatus( 0,C_SETMODE_SETSAVE);	//ֻҪ���ô洢ģʽ���������

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = 0x1;
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
    return 1;
}
int MGetSaveStatus(void)	//�洢״̬0���ڴ棬1���ڴ�
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>29;
    ax = ax & 0x3;
    return ax;

}
int MSetSaveStatus(int status,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 29;
    dx = status & 0x03;
    ax = dx << cx;
    bx = 0x3;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
    return 1;
}
int MGetSaveTime(void)		//�õ���������ʱ��
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>22;
    ax = ax & 0xff;
    ax %= 251;
    return ax;

}
int MSetSaveTime(int time,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 22;
    dx = (time%251) & 0xff;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = 0xff;

        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
    return 1;
}
int MGetSavePosition(void)	//�õ��洢λ�ã�������U�̡�
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>22;
    ax = ax & 0x01;
//	ax %= 251;
    return ax;

}
int MSetSavePosition(int time,int mode)	//���ô洢λ�ã�������U��
{
    u_int ax, bx, cx, dx;
    cx = 22;
    bx = 0x1;
    dx = (time%2) & 0x01;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
    return 1;
}

int MGetSaveFrequency()
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>21;
    ax = ax & 0x1;
    return ax;
}
void MSetSaveFrequency(int bSave, int mode)//�洢Ƶ��
{
    u_int ax, bx, cx, dx;
    cx = 21;
    bx = 0x1;
    dx = (bSave%2) & bx;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
int MGetCraft()	//�õ����պ�
{
    u_int ax = SystemStatus.option;	/*5λ*/

    ax = ax >>16;
    ax = ax & 0x1f;
    return ax;
}
void MSetCraft(int craft,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 16;
    bx = 0x1f;
    dx = (craft%16) & bx;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
u_int MGetColorMode(void)//�Ƿ�
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >> 14;
    ax = ax & 0x1f;
    return ax;
}
void MSetColorMode(u_int item,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 14;	//��λ
    dx = item & 0x1f;
    ax = dx << cx;
    bx = 0x1f;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
int MGetDacShowMode(void)
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>13;
    ax = ax & 0x1;
    return ax;
}
void MSetDacShowMode(int show,int mode)
{
    u_int ax, bx, cx, dx;
    cx = 13;
    bx = 0x1;
    dx = (show%2) & bx;
    ax = dx << cx;

    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        bx = ~( bx << cx );
        bx = bx & SystemStatus.option;
        SystemStatus.option = ax + bx;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
    }
}
int MGetKeyLock()
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>12;
    ax = ax & 0x1;
    ax = 0; //ȥ�����̱�������
    return ax;
}
void MSetKeyLock(int lock,int mode)//������,1��,0��
{
    u_int ax, bx, cx, dx;
    cx = 12;
    bx = 0x1;
    dx = (lock%2) & bx;
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = bx & SystemStatus.option;
    SystemStatus.option = ax + bx;
}

int MGetAmp2Gate( int gatetype)
{
    u_int ax = SystemStatus.option;	/**/
    gatetype = gatetype%2;

    ax = ax >>(10+gatetype);
    ax = ax & 0x1;
    return ax;
}
void MSetAmp2Gate( int gatetype,int gate)//���ڻز��Ƿ�����Ÿ�
{
    u_int ax, bx, cx, dx;

    gatetype = gatetype%2;
    gate = gate%2;
    cx = 10+gatetype;
    bx = 0x1;
    dx = (gate%2) & bx;
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = bx & SystemStatus.option;
    SystemStatus.option = ax + bx;
}

int MGetGateEchoColor( int gatetype)
{
    u_int ax = SystemStatus.option;	/**/
    gatetype = gatetype%2;

    ax = ax >>(8+gatetype);
    ax = ax & 0x1;
    return ax;
}
void MSetGateEchoColor( int gatetype,int gate)//���ڻز���ɫ�Ƿ�Ҫ��
{
    u_int ax, bx, cx, dx;

    gatetype = gatetype%2;
    cx = 8+gatetype;
    bx = 0x1;
    dx = (gate%2) & bx;
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = bx & SystemStatus.option;
    SystemStatus.option = ax + bx;
}

int MGetReadForeland( )
{
    u_int ax = SystemStatus.option;	/**/

    ax = ax >>7;
    ax = ax & 0x1;
    return ax;
}
void MSetReadForeland( int read)//�Ƿ�ǰ�ض���
{
    u_int ax, bx, cx, dx;

    cx = 7;
    bx = 0x1;
    dx = (read%2) & bx;
    ax = dx << cx;

    bx = ~( bx << cx );
    bx = bx & SystemStatus.option;
    SystemStatus.option = ax + bx;
}


int MGetLanguage(void)// �õ���������
{
    return SystemStatus.language%4;	/**/
}
void MSetLanguage(int language)// �õ���������
{
    SystemStatus.language = language%4;
}

int MGetUnitType(void)// �õ�������λ
{
    return 0;
    // return SystemStatus.unit%4;	/**/
}
void MSetUnitType(int unit)// �õ�������λ
{
    SystemStatus.unit = unit%4;
}

/*
�����ƶ�������
*/
/* transfer data to/from/within the system protected memory,
   the address of the protected memory start from 0,
   copymode must be in {PM_COPY_READ, PM_COPY_WRITE, PM_COPY_MOVE} ,
   it's better to check the return value to see if succeeded(>0) or failed(0)
int 	MCopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode) ;
*/
int ChannelParaStore(void)		/*ͨ�������浽������*/
{
    int retvalue;
    int Channel = MGetChannel();
    int offset;
    int nCurrentUser = GetCurrentUser();

    offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * Channel;

    retvalue = MCopyProtectedMemory((void*)offset, &ChannelStatus,C_LEN_CHSTATUS, PM_COPY_WRITE);

    offset += C_LEN_CHSTATUS;
    retvalue = MCopyProtectedMemory((void*)offset, &ChannelPara, C_LEN_CHPARA, PM_COPY_WRITE);

    offset += C_LEN_CHPARA;
    retvalue = MCopyProtectedMemory((void*)offset, &AGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

    offset += C_LEN_GATEPARA;
    retvalue = MCopyProtectedMemory((void*)offset, &BGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

    offset += C_LEN_GATEPARA;
    retvalue = MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

    offset += C_LEN_DAC;
    retvalue &= MCopyProtectedMemory( (void*)offset, &stWeldPara, C_LEN_WELD, PM_COPY_WRITE);

    offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * Channel;
//    CopyMemoryToSystem(offset, C_LEN_CHSTATUS+C_LEN_CHPARA+C_LEN_GATEPARA*2+C_LEN_DAC+C_LEN_WELD,PM_COPY_WRITE);


    /*if(Channel==0||Channel==1||Channel==2)///ͨ��0 1 2 �����ֱ𱣴浽50 51 52
    {
        int iCh=C_CHMAX-10+Channel;

        offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * iCh;

        retvalue = MCopyProtectedMemory((void*)offset, &ChannelStatus,C_LEN_CHSTATUS, PM_COPY_WRITE);

        offset += C_LEN_CHSTATUS;
        retvalue = MCopyProtectedMemory((void*)offset, &ChannelPara, C_LEN_CHPARA, PM_COPY_WRITE);

        offset += C_LEN_CHPARA;
        retvalue = MCopyProtectedMemory((void*)offset, &AGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

        offset += C_LEN_GATEPARA;
        retvalue = MCopyProtectedMemory((void*)offset, &BGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

        //offset += C_LEN_GATEPARA;
        //retvalue = MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

        //offset += C_LEN_DAC;
        //retvalue &= MCopyProtectedMemory( (void*)offset, &stWeldPara, C_LEN_WELD, PM_COPY_WRITE);
    }*/

	//if(MGetDefPara(Channel))
	//	SaveDefPara(Channel);
    return retvalue;
}

int ChannelParaRestore(void)		/*ͨ�������ӱ�����ȡ*/
{
    int retvalue;
    int Channel = MGetChannel();
    int offset ;
    int nCurrentUser = GetCurrentUser();

    offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * Channel;
    retvalue = MCopyProtectedMemory(&ChannelStatus, (void*)offset, C_LEN_CHSTATUS, PM_COPY_READ);

    offset += C_LEN_CHSTATUS;
    retvalue = MCopyProtectedMemory(&ChannelPara, (void*)offset, C_LEN_CHPARA, PM_COPY_READ);

    offset += C_LEN_CHPARA;
    retvalue = MCopyProtectedMemory(&AGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

    offset += C_LEN_GATEPARA;
    retvalue = MCopyProtectedMemory(&BGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

    offset += C_LEN_GATEPARA;
    retvalue = MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);

    offset += C_LEN_DAC;
    retvalue &= MCopyProtectedMemory( &stWeldPara, (void*)offset,C_LEN_WELD, PM_COPY_READ);


    offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * nCurrentUser + C_LEN_CHNPARA * Channel;
//   CopyMemoryToSystem(offset, C_LEN_CHSTATUS+C_LEN_CHPARA+C_LEN_GATEPARA*2+C_LEN_DAC+C_LEN_WELD,PM_COPY_READ);

    stCurveAmend.Angle = MGetAngle(0);
    stCurveAmend.Diameter = MGetDiameter();
    stCurveAmend.Thick = MGetThick();
    MSetCurveAmend(&stCurveAmend);
    if( stCurveAmend.Thick < 20)MSetFunctionMode(0,C_CURVER_AMEND);

    ///if(MGetDefPara(Channel))
	///	GetDefPara(Channel);
    return retvalue;
}

int SystemParaStore(void)		/*ϵͳ�����浽������*/
{
    int retvalue = 0;
    int offset;
    int nCurrentUser = GetCurrentUser();

    offset = C_OFF_SYSTEM + C_LEN_SYSPARA * nCurrentUser;

    retvalue = MCopyProtectedMemory( (void*)offset, &SystemStatus, C_LEN_SYSPARA, PM_COPY_WRITE);
    u_int addr = (u_int)&SystemStatus;


//    CopyMemoryToSystem(offset, C_LEN_SYSPARA,PM_COPY_WRITE);
#ifdef DEBUG
    /*	DebugIntData(10, 130,"Read Repeat=",SystemStatus.repeat);
    	DebugIntData(10, 140,"Read Addr=",addr);
    	DebugIntData(10, 150,"Read offset=",offset);

    	HexTextOut(10, 160, (char *)addr, 12, COLOR_RED,COLOR_WHITE);*/
#endif

    return retvalue;
}
int SystemParaRestore(void)		/*ϵͳ�����ӱ�����ȡ*/
{
    int retvalue = 0;
    int offset;

    int nCurrentUser = GetCurrentUser();
    offset = C_OFF_SYSTEM + C_LEN_SYSPARA * nCurrentUser;
    retvalue = MCopyProtectedMemory( &SystemStatus, (void*)offset,C_LEN_SYSPARA, PM_COPY_READ);
//	MSetChannel(Channel,0);

    //  CopyMemoryToSystem(offset, C_LEN_SYSPARA,PM_COPY_READ);
    return retvalue;
}
int AllParaStore(void)
{
    int retvalue;
    int offset ;

    offset = C_OFF_BACKUP;

    retvalue = MCopyProtectedMemory((void*)offset, &ChannelStatus,C_LEN_CHSTATUS, PM_COPY_WRITE);

    offset += C_LEN_CHSTATUS;
    retvalue &= MCopyProtectedMemory((void*)offset, &ChannelPara, C_LEN_CHPARA, PM_COPY_WRITE);

    offset += C_LEN_CHPARA;
    retvalue &= MCopyProtectedMemory((void*)offset, &AGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

    offset += C_LEN_GATEPARA;
    retvalue &= MCopyProtectedMemory((void*)offset, &BGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

    offset += C_LEN_GATEPARA;
    retvalue &= MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

    offset += C_LEN_DAC;
    retvalue &= MCopyProtectedMemory( (void*)offset, &SystemStatus, C_LEN_SYSPARA, PM_COPY_WRITE);

    offset = C_OFF_BACKUP;
//    CopyMemoryToSystem(offset, C_LEN_CHSTATUS+C_LEN_CHPARA+C_LEN_GATEPARA*2+C_LEN_DAC+C_LEN_SYSPARA,PM_COPY_WRITE);
    return retvalue;

}
int AllParaRestore(void)//ȫ���������ݺͻָ�
{
    int retvalue;
    int offset ;

    offset = C_OFF_BACKUP;
    retvalue = MCopyProtectedMemory(&ChannelStatus, (void*)offset, C_LEN_CHSTATUS, PM_COPY_READ);

    offset += C_LEN_CHSTATUS;
    retvalue &= MCopyProtectedMemory(&ChannelPara, (void*)offset, C_LEN_CHPARA, PM_COPY_READ);

    offset += C_LEN_CHPARA;
    retvalue &= MCopyProtectedMemory(&AGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

    offset += C_LEN_GATEPARA;
    retvalue &= MCopyProtectedMemory(&BGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

    offset += C_LEN_GATEPARA;
    retvalue &= MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);

    offset += C_LEN_DAC;
    retvalue &= MCopyProtectedMemory( &SystemStatus, (void*)offset,C_LEN_SYSPARA, PM_COPY_READ);


    offset = C_OFF_BACKUP;
//   CopyMemoryToSystem(offset, C_LEN_CHSTATUS+C_LEN_CHPARA+C_LEN_GATEPARA*2+C_LEN_DAC+C_LEN_SYSPARA,PM_COPY_READ);
    ChannelParaStore();
    SystemParaStore();
    return retvalue;
}


void SaveDefPara(int Channel)
{

    if(Channel==0||Channel==1||Channel==2)///ͨ��0 1 2 �����ֱ𱣴浽50 51 52
    {
        int iCh=C_CHMAX+Channel;

        int offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * GetCurrentUser()+ C_LEN_CHNPARA * iCh;


        int retvalue = MCopyProtectedMemory((void*)offset, &ChannelStatus,C_LEN_CHSTATUS, PM_COPY_WRITE);

        offset += C_LEN_CHSTATUS;
        retvalue = MCopyProtectedMemory((void*)offset, &ChannelPara, C_LEN_CHPARA, PM_COPY_WRITE);

        offset += C_LEN_CHPARA;
        retvalue = MCopyProtectedMemory((void*)offset, &AGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

        offset += C_LEN_GATEPARA;
        retvalue = MCopyProtectedMemory((void*)offset, &BGatePara, C_LEN_GATEPARA, PM_COPY_WRITE);

        ///offset += C_LEN_GATEPARA;
        //retvalue = MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

        ///offset += C_LEN_DAC;
        ///retvalue &= MCopyProtectedMemory( (void*)offset, &stWeldPara, C_LEN_WELD, PM_COPY_WRITE);
    }
}

void GetDefPara(int Channel)
{
    if(Channel==0||Channel==1||Channel==2)///ͨ��0 1 2 �����ֱ𱣴浽50 51 52
    {
        int iCh=C_CHMAX+Channel;

        int offset = C_OFF_CHANNEL + C_LEN_CHNPARA * C_CHMAX * GetCurrentUser()+ C_LEN_CHNPARA * (iCh);

        memset(&ChannelStatus,0,C_LEN_CHSTATUS);
        int retvalue = MCopyProtectedMemory(&ChannelStatus,(void*)offset, C_LEN_CHSTATUS, PM_COPY_READ);

        offset += C_LEN_CHSTATUS;
        memset(&ChannelPara,0,C_LEN_CHPARA);
        retvalue = MCopyProtectedMemory(&ChannelPara,(void*)offset,  C_LEN_CHPARA, PM_COPY_READ);

        offset += C_LEN_CHPARA;
        memset(&AGatePara,0,C_LEN_GATEPARA);
        retvalue = MCopyProtectedMemory(&AGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

        offset += C_LEN_GATEPARA;
        memset(&BGatePara,0,C_LEN_GATEPARA);
        retvalue = MCopyProtectedMemory(&BGatePara, (void*)offset, C_LEN_GATEPARA, PM_COPY_READ);

		///offset += C_LEN_GATEPARA;
		///memset(&Dac,0,C_LEN_DAC);
        ///retvalue = MCopyProtectedMemory( &Dac, (void*)offset, C_LEN_DAC, PM_COPY_READ);

        MSetAmpStdMax(MGetAmpStdMax());//�ز�����ʱ�ı�׼�߶ȣ�Ĭ��80��



        /*����ֵΪ��ǰ���1\2\3=��ֱ\ˮƽ\����*/
        MSetScaleMode(0,2/*mode = 0\1\2*/);	/*���õ�ǰ���*/

        /*�������õ�����׼��ĸ��\�з�\����\����	*/
        MSetDacMode(2,0/*mode = 0*/);

        /*�������õ�����ʾ��dBֵ\�׾�	*/
        MSetAvgMode(0,0/*mode = 0*/);

        /*��������Ƶ����ȣ�2.5\5.0\10\wide	*/

        MSetBandMode(MGetBandMode(),0/*mode = 0\1\2*/);
        /*��������̽ͷ���᣺50Q\400Q 	*/
        MSetDampMode(MGetDampMode(),0/*mode = 0\1\2*/);

        /*�������÷����ѹ��200V\400V  	*/
        MSetPulseMode(MGetPulseMode(),0/*mode = 0\1\2*/);

        /*�������ü첨��ʽ��ȫ��\���벨\���벨\��Ƶ  	*/
        MSetEchoMode(MGetEchoMode(),0/*mode = 0\1\2*/);


        /*����ֵΪ��ǰ̽ͷģʽ0\1\2\3=ֱ\б\˫��*/
        MSetProbeMode(MGetProbeMode(),0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/

        /*����ֵΪ��ǰ����״̬0\1\2\=δ��\AVG\DAC\AVG*/
        MSetTestStatus(C_TEST_ALL,0);

        /*
        ����Ϊ���ֲ����Ļ��������
        */
        ///MSetGatePara(MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SETSAVE);
        ///MSetGatePara(MGetGatePara(1,0),MGetGatePara(1,1),MGetGatePara(1,2),1,C_SETMODE_SETSAVE);
		MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
		MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
        MSetGatedB(21, 0);


        MSetSpeed(MGetSpeed(),C_SETMODE_SAVE);

        MSetProbeMode(MGetProbeMode(),0);

        MSetDepth(MGetDepth(),0);
        /*����ֵΪ��ǰͨ�������	һλС��*/
        MSetAngle(MGetAngle(0),0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


        /*����ֵΪ��ǰͨ��̽ͷǰ�س��� һλС��	*/
        MSetForward(MGetForward(),0/*mode = 0*/);	/*���õ�ǰͨ��	*/

        /*����ֵΪ��ǰͨ��̽ͷƵ�� һλС��	*/
        MSetFrequence(MGetFrequence(),0/*mode = 0*/);	/*���õ�ǰͨ��	*/

        /*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
        MSetCrystal_l(MGetCrystal_l(),0/*mode = 0*/);	/*���õ�ǰͨ��	*/
        MSetCrystal_w(MGetCrystal_w(),0/*mode = 0*/);	/*���õ�ǰͨ��	*/

        /*����ֵΪ��ǰͨ���������� һλС��	*/
        MSetBaseGain(450,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

        /*����ֵΪ��ǰͨ�����油������ һλС��	*/
        MSetSurfGain(MGetSurfGain(),0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

        /*����ֵΪ��ǰͨ���������� һλС��	*/
        MSetCompGain(MGetCompGain(),0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


        /*����ֵΪ��ǰͨ��	*/
        int i=0;
        for(i = 0; i < 6 ; i++)MSetLineGain(i,0);

        /*����ֵΪ��ǰͨ��	*/

        MSetRange(MGetRange(1),C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


        /*����ֵΪ��ǰͨ��	*/
        MSetDelay(MGetDelay(1),0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

        MSetDepthGain(MGetDepthGain());


        MSetReject(MGetReject(),0);

        /*����ֵΪ��ǰͨ��	*/
        MSetOffset(MGetOffset(),0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

        MSetFunctionMode(0,C_FUNCTION_ALL);
        /*�������ñ�׼	*/

        MSetStdMode(MGetStdMode(),0/*mode = 0*/);


        //MSetStdMode(1,C_SETMODE_SAVE);
        MSetCurveNumber(MGetCurveNumber());
        ///MSetAcquisition(1);
        MSetBlock(MGetBlock());
        MSetThick(MGetThick());
        MSetDiameter(MGetDiameter());

    }
}

#include "print.c"

#include "input.c"

//����̺���ʾ�йصĺ���
#include "keyio.c"

#include "function.c"

//��ͨ�������йصĺ���
#include "para.c"

//���û������йصĺ���
#include "user.c"


void DebugInfo(int num)//C_DEBUG == 1ʱ���ڣ�10��30����ʾ����
{
#if C_DEBUG == 1
    Write_Number(10,30,num,10,0,0);
#endif
}


#if C_DEVLIB == 4
void	SwitchBuzzerOn(u_int bEnable) {}
u_char* GetDacBuffer()
{
    u_int off = 0 ;
    return (void *)off;
}
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	ActivateDac(int bOn) {}
/* peak amplitude dac adjusting */
void    DacGatePeak() {}
void 	UpdateEchoColor() {}
void 	EnableEchoDisplay(u_int ena, u_int mode_fill) {}
void 	NotExtendEchoDisplay(u_int notext) {}
u_int 	SerialPortByteOut(u_int dout)
{
    return 0;
}
u_int 	SerialPortByteIn()
{
    return 0;
}
/* noise reduction , level: 0->no, 1->2 average, 2->4 average, 3->8 average */
//void 	DeNoise(u_int level) {}

#elif C_DEVLIB == 23 || C_DEVLIB == 24

//u_char* GetDacBuffer() {u_int off = 0 ;return (void *)off;}
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
//void	ActivateDac(int bOn) {}
/* peak amplitude dac adjusting */
//void    DacGatePeak() {}
u_int 	SerialPortByteOut(u_int dout)
{
    return 0;
}
u_int 	SerialPortByteIn()
{
    return 0;
}

#if C_DEVLIB == 24
void 	SetDisplayMode(u_int mode_xor) {}
#endif
#endif


//*/

