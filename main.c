//void AppMain(void);
//void Main(){ AppMain(); }
#include "common.h"
#include "display.h"
#include "sysdef.h"
#include "main.h"
#include "string.h"

GATE_PEAK			GatePeak;
GATE_PARA			AGatePara;			/*8*/
GATE_PARA			BGatePara;			/*8*/
GATE_MAX				GateMax;		//���ڳ��ֵ���߲�

FUNCTION             Function;   //����

SYSTEM_STATUS		SystemStatus;		/*4*/
CHANNEL_STATUS		ChannelStatus;		/*4*/
CHANNEL_PARAMETER	ChannelPara;		/*15*2*/
NOTES	Notes;
CACHE	cache;

FAT					fat;
CURVE_AMEND stCurveAmend;

u_char			bCOM2PC;

WELD_PARA		stWeldPara;
ADJUST_TIME		AdjustTime;
DAC				Dac;
///u_short			DACDB[C_COORWIDTH/2];
u_char			*dactable;
int				HoriOffsetScreen ;		//����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��
int				VertOffsetScreen ;		//����д����Ļ�ϵ����ݣ�ˮƽ����ʼ��
double			dac_coe;	//ϵ������80%�Ĳ��

int SecondTime;

extern char MEMORY_STORAGE[];
char _serial[8];
unsigned short FixGain1 ;
unsigned char ChannelMax ;

extern unsigned int WDownShiftNum;		//���ߵ��ڱ���
extern unsigned int WRF_DOWN;			//��Ƶ�������½����ڱ���
extern unsigned int WFixGain;		//ͻ���Ĳ�������(ͻ�����35.4-35.5֮��)
extern char Serial[8];				//�������
extern int push_top_init;

extern int extend;

extern unsigned int FixGain	;
extern unsigned short FixGainPoint1 ;
extern unsigned short FixGainPoint2 ;
extern const unsigned short FixPoint1 ;
extern const unsigned short FixPoint2 ;
extern const unsigned short FixGain2 ;

extern unsigned short GainTable[];

extern const unsigned char MadeDate[] ;
extern const unsigned char _version[] ;//�汾��
extern const unsigned char PowerTable[];
extern const unsigned short _SYSMENU_B1[];
extern const u_int c_crPara[];
extern const u_int Echo_crPara[];

extern u_char crPara[];

extern int g_nDenoise;
extern int g_nAutoDenoise;
extern int g_nRepeat;
extern int g_nPulseWidth;
extern unsigned char PowerAlarm;

#define C_KEYTABLEADDR 0X408000
char *pSerial = (char *)(C_KEYTABLEADDR + 0X30);
short *pFixgain = (short *)(C_KEYTABLEADDR + 1022);
short *pFWBaseLine = (short *)(C_KEYTABLEADDR + 1002);
short *pRFBaseLine = (short *)(C_KEYTABLEADDR + 1000);
//unsigned char *pPowerAlarm = (char *)(C_KEYTABLEADDR + 998);

unsigned short c_FWBaseLine ;	//ȫ������  070810/08/07/06 =4,070809 = 18
unsigned short c_RFBaseLine ;	//��Ƶ����   875

const u_short _ParaMenu_CE2[] =
{
    C_PULSE_MIN,C_PULSE_MED,C_PULSE_MAX,
};


u_int 	GetBatteryStatus(void)
{
    int rev;
    char a;
  //  BatteryPackRead(0, 8, rev) ;
  BatteryPackRead(0x0B0D,1,&a) ;
 // a-=1879905280;
  rev=((GetPowerStatus())*256)+a;

/*Write_Number(10,100,a,10,0,0);
Write_Number(10,120,rev,10,0,0);
Write_Number(10,140,GetPowerStatus(),6,0,0);
*/    return  rev;
}

u_int g_iTickCount;

int __errno ()
{
    return 1;
}

extern const unsigned char _KeyLock[];

const char _MainInfo[C_LANGUAGE][4][25] =
{
    {"�����Լ������Ժ򡭡�","���ֳ���������  ","����ල�绰��","0513-85284918"},
    {"Self-testing ...  ","Innovation in UT","����ල�绰��","0513-85284918"}
};
extern const unsigned long int ColorsBG[];
extern const unsigned char background[];

/// The MAC address
static unsigned char MacAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

/// The IP address
static unsigned char IpAddress[4] = {192, 168, 111,8 };//{10, 159, 245, 156};
extern int g_iNum;
int nFlag;
int main()
{
    g_iNum=0;
    int i;
    int offset_back;
	int file_total=0;
	TIME_CLOCK TimeClock;
    int temp =0;
   // CharToKeypad(11);
   // ExpendTime(5);

 /*	CharToKeypad(LED_ALARM_OFF);
    ExpendTime(5);
    CharToKeypad(BUZZER_ALARM_OFF);
    ExpendTime(5);
 */
 	SystemInit(500,480);

   //UGetSystemTime(&TimeClock);
   //SetSystemTime(&TimeClock);
   //EthernetStop() ;
#if C_DEVLIB == 1 || C_DEVLIB == 24
    HoriOffsetScreen = 4;
    VertOffsetScreen = 1;
#elif C_DEVLIB == 2
    HoriOffsetScreen = 4;
    VertOffsetScreen = 0;
    SetDisplayColor( COLOR_YELLOW );
#elif C_DEVLIB == 23
    HoriOffsetScreen = 0;
    VertOffsetScreen = 2;
    SetDisplayColor( COLOR_YELLOW );
#elif C_DEVLIB == 3
    HoriOffsetScreen = 4;
    VertOffsetScreen = 0;
    SetDisplayColor( COLOR_YELLOW );
#endif

    MSetAmplifierGain(0);
    MSetAlarmStatus(-1);
//;	SwitchAlarmOn(0);

    SecondTime = GetElapsedTime();
   // MSaveDefualtColor(1);

    int offset = C_OFF_SYSTEM;
    CopyMemoryToSystem(offset,C_LEN_SYSPARA,PM_COPY_READ);
    MCopyProtectedMemory( &SystemStatus, (void*)offset,C_LEN_SYSPARA, PM_COPY_READ);

    if(MGetLanguage()>1 || MGetLanguage()<0)MSetLanguage(0);
    MSetUnitType(0);
    crPara[ C_CR_BACK] = 8;//��
    SetBackgroundColor(COLOR_BLACK);


    DisplayBMP(background,ColorsBG, 0, 100,281,640);
    MSetDisplayColor(COLOR_GREEN);
    TextOut(120, 10,  1, 23,32, (u_char*)_MainInfo[MGetLanguage()][2], 4); //MGetLanguage()
    TextOut(120+24*7, 10,  1, 23,32, (u_char*)_MainInfo[MGetLanguage()][3], 4); //MGetLanguage()
    MSetDisplayColor(COLOR_RED);
    TextOut(120+64+32-46*MGetLanguage(), 40,  1, 23,32, (u_char*)_MainInfo[MGetLanguage()][1], 4); //MGetLanguage()
    MSetDisplayColor(COLOR_YELLOW);
    TextOut(120+32, 340+24*2,  1, 23,32, (u_char*)_MainInfo[MGetLanguage()][0], 4); //MGetLanguage()
   // MAnyKeyReturn();
  //  MKeyRlx();
    offset = C_OFF_INIT ;
    CopyMemoryToSystem(offset,C_OFF_BATTERY-offset,PM_COPY_READ);

    CopyMemoryAllSystem(PM_COPY_READ);
 //#define C_OFF_FILE		(((C_OFF_FAT  + C_LEN_FAT * C_FILE_NUMBER+1)/2)*2)
//#define C_OFF_OTHER		(((C_OFF_FILE + C_LEN_FILE * C_FILE_NUMBER+1)/2)*2)

//Write_Number(0,0,1,4,0,0);

    offset=C_OFF_FILEINFO;
    CopyMemoryToSystem(offset,C_OFF_FAT-C_OFF_FILEINFO,PM_COPY_READ);

    offset = C_OFF_FILEINFO ;
    MCopyProtectedMemory( &file_total, (void*)offset, C_SIZE_INT, PM_COPY_READ);

    offset = C_OFF_CACHE;
    MCopyProtectedMemory( &cache, (void*)offset,sizeof(CACHE), PM_COPY_READ);
    for(i=0;i<32;i+=4)
    {
        MDrawPixel(136+i, 380,DP_SET);
    }
//Write_Number(0,0,file_total,10,0,0);
    for(i=0;i<5;i++)
    {
    temp += cache.cache[i];
    }


    if(file_total>C_FILE_NUMBER || file_total<0 ||temp<=0 || temp>(5*C_FILE_NUMBER))//���ݻָ�
    {
        offset=C_OFF_FILEINFO;
        offset_back = C_OFF_F_BACKUP;
        CopyMemoryToSystem(offset_back,C_OFF_FILEMAX-C_OFF_F_BACKUP,PM_COPY_READ);
        MCopyProtectedMemory( (void*)offset, (void*)offset_back, C_OFF_FILEMAX-C_OFF_F_BACKUP, PM_COPY_MOVE);
        CopyMemoryToSystem( (void*)offset,C_OFF_FAT-C_OFF_FILEINFO,PM_COPY_WRITE);
    }
    else
    {
        offset = C_OFF_FILEINFO;
        offset_back = C_OFF_F_BACKUP;
        MCopyProtectedMemory( (void*)offset_back, (void*)offset, C_OFF_FILEMAX-C_OFF_F_BACKUP, PM_COPY_MOVE);
        CopyMemoryToSystem( (void*)offset_back,C_OFF_FILEMAX-C_OFF_F_BACKUP,PM_COPY_WRITE);

    }

//    CopyMemoryAllData(file_total,PM_COPY_READ);

    offset=C_OFF_FAT;
    for(i=0;i<file_total;i++)
    {
        CopyMemoryToSystem(offset+cache.cache[i]*C_LEN_FAT,C_LEN_FAT,PM_COPY_READ);
    }

    offset=C_OFF_FILE;
    for(i=0;i<file_total;i++)
    {
        CopyMemoryToSystem(offset+cache.cache[i]*C_LEN_FILE,C_LEN_FILE,PM_COPY_READ);
     //       Write_Number(10,100,i,4,0,0);
     //       Write_Number(10,130,file_total,4,0,0);
     //       MAnyKeyReturn();
    }


 /*   for(i=0;i<260;i++)
    {
    Write_Number((i%10)*3*16,(i/10)*24,cache.cache[i],4,0,0);
    }
        MAnyKeyReturn();*/
//Write_Number(0,0,3,4,0,0);
  // TextOut(120, 200,  1, 23,32, (u_char*)_MainInfo[1][0], 4); //MGetLanguage()
    if( true )
    {
        int i;
       /* if( *pFixgain > 0 && *pFixgain < 30)
        {
            FixGain1 = *pFixgain;
            //	PowerAlarm = *pPowerAlarm;
            for( i = 0; i < 8; i++)_serial[i] = *(pSerial+i);
            c_FWBaseLine = *pFWBaseLine;
            c_RFBaseLine = *pRFBaseLine;	//��Ƶ����   875
        }*/
     //   else
        {
            //	PowerAlarm = PowerTable[10];
           /* FixGain1 = 5;
            for( i = 0; i < 8; i++)_serial[i] = ' ';
            c_FWBaseLine = 3;	//ȫ������  070810/08/07/06 =4,070809 = 18
            c_RFBaseLine = 825;	//��Ƶ����   875*/

            INNER_INFO InnerInfo;
            offset = C_OFF_INNER_INFO;
            MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);
            ChannelMax=InnerInfo.Channel;
            if(ChannelMax<20||ChannelMax>50)
            {
                ChannelMax=20;
                InnerInfo.Channel=20;
                MCopyProtectedMemory((void*)offset, &InnerInfo, sizeof(INNER_INFO), PM_COPY_WRITE);
            }
            FixGain1 = InnerInfo.FixGain;
            for( i = 0; i < 8; i++)_serial[i] = Serial[i];
            c_FWBaseLine = WDownShiftNum;	//ȫ������  070810/08/07/06 =4,070809 = 18
            c_RFBaseLine = WRF_DOWN;	//��Ƶ����   875
        }
        PowerAlarm = PowerTable[9];
    }
#if C_DEVLIB == 23 || C_DEVLIB == 24
//u_short CompanyName1[] = {CH(5149),CH(3310),CH(4293),CH(3475),CH(2722),CH(5113),CH(3690)} ;
    /*
    //*/
    push_top_init = 0;
    //int key;
    //char pch[10];
    //SetEchoLayout(0,290,30,0);
    SetEchoLayout(C_COORHPOSI ,C_COORHPOSI+500,C_COORVPOSI+VertOffsetScreen,0);
    EnableEchoDisplay(1) ;

#if C_DEVLIB == 23
    UpdateEchoColor() ;
#endif
    SetDisplayMode(1);
   // EraseWindow(0,0,640,480);
    SetAmplifierGain(999) ;
    SetEchoReject(0) ;
    SetEchoRange(10000) ;
    SetEchoDelay(0) ;

    SetAmplifierBand(AMP_BAND_NARROW_1) ;
    SetEchoMode(FULL_WAVE) ;
    SetEchoBaseLine( 2);

    SetProbeMode(T_T_SINGLE) ;
    SetProbeDamping(LOW_DAMPING) ;
    SetTransmitterVoltage(LOW_PULSE) ;

    int CoorHPosi, CoorVPosi, CoorWidth, CoorHeight;
    CoorHPosi = C_COORHPOSI ;
    CoorVPosi = C_COORVPOSI ;
    CoorWidth =  C_COORWIDTH;
    CoorHeight = C_COORHEIGHT;
#endif

//////////////
    FixGainPoint1 = FixPoint1 + FixGain;
    FixGainPoint2 = FixPoint2 + FixGain;
    MSetKeyLock(0,C_SETMODE_SAVE);//�򿪼�����
    MSetAmp2Gate(0,0);
    MSetAmp2Gate(1,0);//����A,B�������޻ز������Ÿ�

    //MSetChannel(0,0);
    //ChannelParaInit(1);	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��*/
    //CopyMemoryAllSystem(PM_COPY_READ);
    bCOM2PC = 0;
    // SetUsbMode(1);

    EthernetInit(MacAddress, IpAddress);
    ExpendTime(5);
    EthernetStop() ;
    for(i=0;i<32;i+=4)
    {
        MDrawPixel(136+32+i, 380,DP_SET);
    }

//Write_Number(0,0,2,4,0,0);
/*    ExpendTime(10);
    bCOM2PC=EthernetTransmit((u_char*) crPara, 8) ;
Write_Number(0,0,bCOM2PC,4,0,0);
MAnyKeyReturn();
    if(bCOM2PC == 8)
    EthernetStop() ;*/
    SystemParaRestore();
	nFlag = SystemStatus.prepare[2];

    ChannelParaRestore();

	//if(MGetDefPara(MGetChannel()))
		//GetDefPara(MGetChannel());
//	MSetSaveMode(0,C_SETMODE_SETSAVE);
    MSetSaveStatus( 0,C_SETMODE_SETSAVE);
    MSetSavePosition(MGetSaveMode(),C_SETMODE_SETSAVE);

//	MSaveDefualtColor(MGetColorMode());//����Ĭ����ɫ
    MSaveSystemElapsedTime( 0 , 0);	//�洢��ʼʱ�䳤Ϊ0
    MSaveStartTime(	MGetSystemTime(6)*1000 );//�洢����ʱ�䣬����Ϊ��λ

    g_nDenoise = MGetDenoise();
    g_nAutoDenoise = 0;
    g_nRepeat = 200;
    g_nPulseWidth = 1;

    SystemStatus.displaymode = 1;

    if(MGetLanguage()>1 || MGetLanguage()<0)MSetLanguage(0);
//	MSetLanguage(0);
    MSetDenoise(MGetDenoise(), C_SETMODE_SET);
    MSetRepeatRate(MGetRepeatRate(), C_SETMODE_SET);
//    if(extend==0)
    {
        MSetPulseWidthInput(MGetPulseWidthNumber(), C_SETMODE_SET);
    }
//    else
    {
//        MSetPulseWidth(MGetPulseWidth(), C_SETMODE_SET);
    }
/*
Write_Number(0,30,2,4,0,0);
Write_Number(0,30,GetKeyCode(),4,0,0);
Write_Number(30,30,3,4,0,0);*/
    if( true )
    {
        int i;
        int offset = C_OFF_SAMP;
        u_char sampbuff[C_LEN_SAMP+1];

        for( i = 0 ; i <= C_LEN_SAMP; i++)sampbuff[i] = 0;
        MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);

        int days = MGet2kDays(1);//���վ�2000�������
        if( days != MGet2kDays(0) )
        {
            MSet2kDays(days);

            //��������,mode = 0�������ܣ�1���/K/dac/avg,2������
            MSetEquivalentDays(0,MGetEquivalentDays(0)+1);
            MSetEquivalentDays(1,MGetEquivalentDays(1)+1);
            MSetEquivalentDays(2,MGetEquivalentDays(2)+1);
        }
    }

    MKeyRlx();


    if( true )
    {
        int offset ;
        offset = C_OFF_TEMP+60;	//��ɫ�洢λ��
        MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
        MSetColor(C_CR_DEFAULT);
        SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);


        offset = C_OFF_KEYFUNC;
        MCopyProtectedMemory( & Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
		if(Function.funcMenu[0]<0&&Function.funcMenu[0]>8)
		{
			Function.funcMenu[0]=1;
		}

		if(Function.funcMenu[1]<0&&Function.funcMenu[1]>8)
		{
			Function.funcMenu[1]=2;
		}
		
		if(Function.funcMenu[2]<0&&Function.funcMenu[2]>8)
		{
			Function.funcMenu[2]=3;
		}

		if(Function.funcMenu[3]<0&&Function.funcMenu[3]>8)
		{
			Function.funcMenu[3]=0;
		}
    }
  //  MFclearScreen();
    ExpendTime(10);


#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
    //dactable = GetDacBuffer();//dac����������
#endif

// @todo (union#1#): The 1st test point


        char testbuf[100];

    // @todo (union#2#): Test CopyProtectedMemory Function
/*
extern char MEMORY_STORAGE[];

    CopyProtectedMemory(1000,testbuf,100,PM_COPY_WRITE);
    HexDump(10,10,testbuf,20);
    HexDump(10,20,MEMORY_STORAGE+1000,20);
    memset(testbuf,0,100);
    CopyProtectedMemory(testbuf,1000,100,PM_COPY_READ);
    HexDump(10,30,testbuf,20);
    HexDump(10,40,MEMORY_STORAGE+1000,20);

    CopyProtectedMemory(0,1000,100,PM_COPY_MOVE);
    HexDump(10,50,MEMORY_STORAGE,20);
    HexDump(10,60,MEMORY_STORAGE+1000,20);

    while(1);

*/
    g_iTickCount = GetElapsedTime();
   if(0) {
    char *strTestPoint = "This is the 1st test point.";
    ETextOut(10,300, strTestPoint,strlen(strTestPoint));
    RDC16TextOutBase(20, 400,"��ͨ������");
    }


    ExpendTime(10);
    SystemParaStore();

    ///do
    ///{
    ///    Write_Number(0,0,10,4,0,0);

    ///}while(MGetKeyCode(0)==C_KEYCODMAX);

// @todo (union#2#): The 2nd test point
    if(0)
    {

        char *strTestPoint = "This is the 2nd test point.";
        ETextOut(10,310, strTestPoint,strlen(strTestPoint));
        u_int duration = GetElapsedTime() - g_iTickCount;
        char buf[30];
        memset(buf,0,30);
        sprintf(buf, "%d", duration);
        g_iTickCount = GetElapsedTime();
        ETextOut(300,310,buf,strlen(buf));

    }

    ChannelParaStore();
    MainInitialize();

    MSetSystemMode(0,0);	//���ó�һ��״̬
    MSetAcquisition(0);
    MActivateDac(0);

    MainProject();
	
    return 1;
}
#include "char.h"

#include "test.c"
#include "menu.c"
#include "data.c"

void MainInitialize(void)
{
//13052832768
    int offset =  C_OFF_BATTERY + 40;		//��ʼ���ʱ��
    int startchargetime = 0;
// @todo (union#3#): The 3rd test point
    if(0) {

    char *strTestPoint = "This is the 3rd test point.";
    ETextOut(10,320, strTestPoint,strlen(strTestPoint));
    u_int duration = GetElapsedTime() - g_iTickCount;
    char buf[30];
    memset(buf,0,30);
    sprintf(buf, "%d", duration);
    g_iTickCount = GetElapsedTime();
    ETextOut(300,320,buf,strlen(buf));
    }

    ExpendTime(3);
    MCopyProtectedMemory( (void*)offset, &startchargetime, C_SIZE_INT, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, C_SIZE_INT,PM_COPY_WRITE);

    ///elapsedTime1 = elapsedTime2 = 0;
    ExpendTime(2);
            //  InitParaTemp();   //�����Զ�����
            offset = C_OFF_STACK;
            MCopyProtectedMemory( (void*)offset, GainTable,  121*4, PM_COPY_WRITE);
    SystemLog();

    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
    ExpendTime(2);
    SystemParaRestore();
// @todo (union#4#): The 4th test point
    if(0) {

    char *strTestPoint = "This is the 4th test point.";
    ETextOut(10,330, strTestPoint,strlen(strTestPoint));
        u_int duration = GetElapsedTime() - g_iTickCount;
    char buf[30];
    memset(buf,0,30);
    sprintf(buf, "%d", duration);
    g_iTickCount = GetElapsedTime();
    ETextOut(330,330,buf,strlen(buf));

    }

    ExpendTime(2);
    ChannelParaRestore();
    ExpendTime(2);
    SystemClear();
    ExpendTime(2);
    MSetSaveStatus( 0, C_SETMODE_SETSAVE);
    offset = C_OFF_STORE +	34;		//���Զ������ݸ���
    MCopyProtectedMemory( (void*)offset, &startchargetime, C_SIZE_INT, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, C_SIZE_INT,PM_COPY_WRITE);
    offset = C_OFF_STORE +	30;		//ǰ���Զ��洢ʱ�Ŀ���ʱ��sec
    MCopyProtectedMemory( (void*)offset, &startchargetime, C_SIZE_INT, PM_COPY_WRITE);
//            CopyMemoryToSystem(offset, C_SIZE_INT,PM_COPY_WRITE);

}

void SystemClear(void)
{

    MSetCursorEnable(0,0);//���ó���Ļ�޹��

    MSetCursorShow(0, 0);//���óɽ�ֹ�й��
    MSetReject(0,0/*mode = 0\1\2*/);	/*����ϵͳ����Ϊ0	*/

//	MSetSaveMode(0,C_SETMODE_SETSAVE);//�ر�������¼
    MSetSaveStatus( 0,C_SETMODE_SETSAVE);
    MSetSavePosition(MGetSaveMode(),C_SETMODE_SETSAVE);
    //MSetScreenBrightness(2,0/*mode = 0*/);		/**/

    MSetSystemMode(0,0);	//���ó�һ��״̬

    MSetFunctionMode(0,C_GATE_ALARM);

    /*
    	Scrsave_Picture();
    	MFclearScreen();

    	u_short InputChar[20];

    	if(MInputChar(0,0,1,InputChar,40,20) )
    		CEMenuOut(0,30,InputChar+1,InputChar[0],1,24);
    	MAnyKeyReturn();
    */
}

void TestGainTable()
{
    int gain,range,gateposi,gatewide,gatehigh;
    double temp1 = 1.1220184543;//	20log(1.1220184543)=1
    int preamp,amp,amp1,amp2 = 0;
    int keycode;
    int i,j;


    //for(i=0;i<=120;i++)GainTable[i] = 0;//���Ի������
    //GainTable[12] = 2*GainTable[11] - GainTable[10];

    gain = 0;
    range = 10000;
    gateposi = C_COORHORIUNIT * 1;
    gatewide = C_COORHORIUNIT * 8;
    MSetBaseGain(gain,C_SETMODE_SAVE);
    MSetRange(range,C_SETMODE_SETSAVE);		//���õ�ǰͨ��
    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);

    MSetSpeed(5920,C_SETMODE_SETSAVE);		//���õ�ǰͨ��
    MSetAngle(0,C_SETMODE_SETSAVE);			//���õ�ǰͨ��
    MSetDelay(0,C_SETMODE_SETSAVE);			//���õ�ǰͨ��
    MSetOffset(0,C_SETMODE_SETSAVE);	//���õ�ǰͨ��
    MSetProbeMode(3,C_SETMODE_SETSAVE);

    ScreenRenovate();
    DisplayPrompt(15);

    int deci_len ;
    int number;
    deci_len = 0;
    METextOut(0,0,"Begin:",6);
    Input_Number(7*C_ECHAR_HDOT,0,&number,2,&deci_len,0);
    METextOut(0,0,"FixGain:",8);
    if(Input_Number(9*C_ECHAR_HDOT,0,&FixGain,3,&deci_len,0) < 1)return ;

    u_short* pzText;
    const u_short chb1[] = ////�����ⲿ˥��ʹ�ز���80��
    {
        CH(2187),CH(2958),CH(4566),CH(1831),CH(4305),CH(2885),CH(4225),CH(2756),CH(1808),CH(2463),'8','0','%',CH(100),CH(100),CH(100)
    };
    const u_short chb2[] = //����˥��1dBʹ�ز�����
    {
        CH(5286),CH(2851),CH(4305),CH(2885),'1','d','B',CH(4225),CH(2756),CH(1808),CH(2921),CH(2145),CH(16),CH(100),CH(100),CH(100)
    };
    pzText = (u_short*)chb1;
    CEMenuOut(0,0,pzText,13,1,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

    gain = number*10;


    while( number < 30)
    {
        keycode = MGetKeyCode(20);
        if( keycode == C_KEYCOD_CONFIRM)break;
        else if(keycode == C_KEYCOD_PLUS)number++;
        else if(keycode == C_KEYCOD_MINUS)number--;
        else if( keycode == 3)
        {
            range+= 50;
            MSetRange(range,C_SETMODE_SETSAVE);
            WriteScale();	//���
            continue;
        }
        else if( keycode == 4)
        {
            range-= 50;
            MSetRange(range,C_SETMODE_SETSAVE);
            WriteScale();	//���
            continue;
        }
        else continue;
#if C_DEVLIB == 4
        SetAmplifierGain(MGetChannel(),0);
#else
        SetAmplifierGain(0);
#endif
        ExpendTime(10);
    }

    MKeyRlx();
    pzText = (u_short*)chb2;
    CEMenuOut(0,0,pzText,13,1,24);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

//	int k,l = 0;
    MDrawGate(-1,0, -1, 0);

    for(i = number + 1; i <= 74 && GainTable[i] <= 3800; i++)
    {
        ExpendTime(10);
        preamp = 50*MGetAmpMax(0);
        ExpendTime(10);
        preamp += 50*MGetAmpMax(0);
        amp = preamp;

        Write_Number(0,0,i,5,0,0);
        Write_Number(50,0,preamp/20,5,1,0);
        amp1 = (int)( preamp * temp1+0.5);
        Write_Number(100,0,amp1/20,5,1,0);
        Write_Number(150,0,amp1,5,0,0);
        Write_Number(200,0,GainTable[i-1],5,0,0);

        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);
        gatehigh = preamp/100;
        MSetGatePara(gateposi,gatewide,gatehigh,0,C_SETMODE_SETSAVE);
        gatehigh = amp1/100;
        MSetGatePara(gateposi,gatewide,gatehigh,1,C_SETMODE_SETSAVE);
        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);

        GainTable[i] = GainTable[i-1];
        j=0;
        while(true)
        {
            if(j == 0 && amp < amp1 && GainTable[i] <= 3800)
            {
                GainTable[i]++;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
            else j = 1;

            //Write_Number(50,0,gain,5,0,0);

            ExpendTime(2);
            amp = 50*MGetAmpMax(0);
            ExpendTime(5);
            amp += 50*MGetAmpMax(0);

            amp2 = amp1-amp;

            Write_Number(100,10,amp/20,5,1,0);
            Write_Number(150,10,amp,5,0,0);
            Write_Number(200,10,GainTable[i],5,0,0);
            if( j == 1)
            {
                keycode = MGetKeyCode(20);
                if( keycode == C_KEYCOD_CONFIRM)break;
                else if(keycode == C_KEYCOD_PLUS)GainTable[i]++;
                else if(keycode == C_KEYCOD_MINUS)GainTable[i]--;
                else if( keycode == 1)
                {
                    gateposi++;
                    MDrawGate(0,0, -1, 0);
                    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                    MDrawGate(0,0, -1, 0);
                    continue;
                }
                else if( keycode == 2)
                {
                    gateposi--;
                    MDrawGate(0,0, -1, 0);
                    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                    MDrawGate(0,0, -1, 0);
                    continue;
                }
                else if( keycode == 3)
                {
                    range+= 50;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    WriteScale();	//���
                    continue;
                }
                else if( keycode == 4)
                {
                    range-= 50;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    WriteScale();	//���
                    continue;
                }
                else if( keycode == C_KEYCOD_RETURN)break;
                else continue;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
        }
        if( keycode == C_KEYCOD_RETURN)break;
    }

    MFclearScreen();
    for(i=0; i<121; i++)
    {
        Write_Number( (i%6)*44,(i/6)*10,GainTable[i],4,0,0);
    }
    MAnyKeyReturn();
//	FixGain = 215;
}


void TestGainTable1()
{
    int gain,range,gateposi,gatewide,gatehigh;
    double temp1 = 1.1220184543;//	20log(1.1220184543)=1
    int preamp,amp,amp1,amp2 = 0;
    int i,j;
    int keycode;

    unsigned int offset;

    offset = C_OFF_STACK;
    MCopyProtectedMemory( GainTable, (void*)offset,121*4, PM_COPY_READ);

    MFclearScreen();
    for(i=0; i<100; i++)
    {
        Write_Number( (i%6)*40,(i/6)*10+30,GainTable[i],4,0,0);
    }
    MAnyKeyReturn();

    SystemParaRestore();
    ChannelParaRestore();
    MSetSystemMode(0,0);	//���ó�һ��״̬
    MSetSystem();
    gain = 0;
    range = 1000;
    gateposi = C_COORHORIUNIT * 7.5;
    gatewide = 28;
    MSetPara(gain,0,0,0,range,0,0,0,5900);


    DisplayPrompt(15);
    MSetAcquisition(1);

    int deci_len ;
    int number;
    deci_len = 0;
    METextOut(0,0,"Begin:",6);
    number = 0;
    if( Input_Number(7*C_ECHAR_HDOT,0,&number,2,&deci_len,0) < 1)return ;

    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
    //while(MGetAmpDist(0) > range*4/5)MSetOffset(++offset,C_SETMODE_SETSAVE);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*��ָ��λ�úʹ�С�Ĳ�������*/
    WriteScale();	//���

//	MSetAcquisitionMode(1,C_SETMODE_SETSAVE);
//	MSetHardEchoShow(1,C_SETMODE_SET);
    if(number == 0)GainTable[0] = 0;
#if C_DEVLIB == 4
    SetAmplifierGain(MGetChannel(),number*10);
#else
    SetAmplifierGain(number*10);
#endif
    MKeyRlx();

    while( number == 0)
    {
        keycode = MGetKeyCode(20);
        if( keycode == C_KEYCOD_CONFIRM)break;
        else if(keycode == C_KEYCOD_PLUS)GainTable[0]++;
        else if(keycode == C_KEYCOD_MINUS)GainTable[0]--;
        else if( keycode == 3)
        {
            range+= 50;
            MSetRange(range,C_SETMODE_SETSAVE);
            WriteScale();	//���
            continue;
        }
        else if( keycode == 4)
        {
            range-= 50;
            MSetRange(range,C_SETMODE_SETSAVE);
            WriteScale();	//���
            continue;
        }
        else continue;
#if C_DEVLIB == 4
        SetAmplifierGain(MGetChannel(),0);
#else
        SetAmplifierGain(0);
#endif
        ExpendTime(10);
    }
    MDrawGate(-1,0, -1, 0);
    for(i = number + 1; i <= 74 ; i++)
    {
        ExpendTime(10);
        preamp = 50*MGetAmpMax(0);
        ExpendTime(10);
        preamp += 50*MGetAmpMax(0);
        amp = preamp;

        Write_Number(0,0,i,5,0,0);
        Write_Number(50,0,preamp/20,5,1,0);
        amp1 = (int)( preamp * temp1+0.5);
        Write_Number(100,0,amp1/20,5,1,0);
        Write_Number(150,0,amp1,5,0,0);
        Write_Number(200,0,GainTable[i-1],5,0,0);

        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);
        gatehigh = preamp/100;
        MSetGatePara(gateposi,gatewide,gatehigh,0,C_SETMODE_SETSAVE);
        gatehigh = amp1/100;
        MSetGatePara(gateposi,gatewide,gatehigh,1,C_SETMODE_SETSAVE);
        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);

        GainTable[i] = GainTable[i-1];
        j=0;
        while(true)
        {
            if(j == 0 && amp < amp1 && GainTable[i] <= 3800)
            {
                GainTable[i]++;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
            else j = 1;

            //Write_Number(50,0,gain,5,0,0);

            ExpendTime(2);
            amp = 50*MGetAmpMax(0);
            ExpendTime(5);
            amp += 50*MGetAmpMax(0);

            amp2 = amp1-amp;

            Write_Number(100,10,amp/20,5,1,0);
            Write_Number(150,10,amp,5,0,0);
            Write_Number(200,10,GainTable[i],5,0,0);
            if( j == 1)
            {
                keycode = MGetKeyCode(20);
                if( keycode == C_KEYCOD_CONFIRM)break;
                else if(keycode == C_KEYCOD_PLUS)GainTable[i]++;
                else if(keycode == C_KEYCOD_MINUS)GainTable[i]--;
                else if( keycode == 1)
                {
                    gateposi++;
                    MDrawGate(0,0, -1, 0);
                    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                    MDrawGate(0,0, -1, 0);
                    continue;
                }
                else if( keycode == 2)
                {
                    gateposi--;
                    MDrawGate(0,0, -1, 0);
                    MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                    MDrawGate(0,0, -1, 0);
                    continue;
                }
                else if( keycode == 3)
                {
                    range+= 50;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    WriteScale();	//���
                    continue;
                }
                else if( keycode == 4)
                {
                    range-= 50;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    WriteScale();	//���
                    continue;
                }
                else if( keycode == C_KEYCOD_RETURN)break;
                else continue;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
        }
        if( keycode == C_KEYCOD_RETURN)break;
    }

    int step;
    step = (GainTable[i-1] - GainTable[i-5])/4;
    for( ; i <= 120 ; i++)GainTable[i] = GainTable[i-1]+step;

    offset = C_OFF_STACK;
    MCopyProtectedMemory( (void*)offset, GainTable, 121*4, PM_COPY_WRITE);
    MFclearScreen();
    for(i=0; i<120; i++)
    {
        Write_Number((i%6)*44,(i/6)*10+30,GainTable[i],4,0,0);
    }

    MAnyKeyReturn();
}
//*/
void AdjustGainTable()
{
    int gain,range,gateposi,gatewide,gatehigh;
    int preamp,amp,amp1;//,amp2 = 0;
    double temp1 = 1.1220184543;//	20log(1.1220184543)=1
    int i,j;
    int keycode;
    unsigned int offset;

    offset = C_OFF_STACK;
    MCopyProtectedMemory( GainTable, (void*)offset,121*4, PM_COPY_READ);

    int deci_len ;
    int number;
    deci_len = 0;
    METextOut(0,0,"Begin:",6);
    number = 0;
    Input_Number(7*C_ECHAR_HDOT,0,&number,2,&deci_len,0);

    SystemParaRestore();
    ChannelParaRestore();
    MSetSystemMode(0,0);	//���ó�һ��״̬
    MSetSystem();
    gain = 0;
    range = 1000;
    gateposi = C_COORHORIUNIT * 7.5;
    gatewide = 28;
    MSetPara(gain,0,0,0,range,0,0,0,5900);


    MSetAcquisition(1);
#if C_DEVLIB == 4
    SetAmplifierGain(MGetChannel(),number*10);
#else
    SetAmplifierGain(number*10);
#endif
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*��ָ��λ�úʹ�С�Ĳ�������*/
    WriteScale();	//���
    MDrawGate(0,0, -1, 0);
    MDrawGate(1,0, -1, 0);


    for(i = number+1; i <= 120 && GainTable[i] <= 3800; i++)
    {
#if C_DEVLIB == 4
        SetAmplifierGain(MGetChannel(),10*i);
#else
        SetAmplifierGain(10*i);
#endif
        ExpendTime(10);
        preamp = 50*MGetAmpMax(0);
        ExpendTime(10);
        preamp += 50*MGetAmpMax(0);
        amp = preamp;
        //98382051,9159814,
        Write_Number(0,0,i,5,0,0);
        Write_Number(50,0,preamp/20,5,1,0);
        amp1 = (int)( preamp * temp1+0.5);
        Write_Number(100,0,amp1/20,5,1,0);
        Write_Number(150,0,amp1,5,0,0);
        Write_Number(200,0,GainTable[i-1],5,0,0);

        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);
        gatehigh = preamp/100;
        MSetGatePara(gateposi,gatewide,gatehigh,0,C_SETMODE_SETSAVE);
        gatehigh = amp1/100;
        MSetGatePara(gateposi,gatewide,gatehigh,1,C_SETMODE_SETSAVE);
        MDrawGate(0,0, -1, 0);
        MDrawGate(1,0, -1, 0);
        while(true)
        {
            ExpendTime(5);
            amp = 50*MGetAmpMax(0);
            ExpendTime(5);
            amp += 50*MGetAmpMax(0);

            Write_Number(100,10,amp/20,5,1,0);
            Write_Number(150,10,amp,5,0,0);
            Write_Number(200,10,GainTable[i],5,0,0);

            keycode = MGetKeyCode(20);
            if( keycode == C_KEYCOD_CONFIRM)break;
            else if(keycode == C_KEYCOD_PLUS)
            {
                for( j = i; j <= 120; j++)GainTable[j]++;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
            else if(keycode == C_KEYCOD_MINUS)
            {
                for( j = i; j <= 120; j++)GainTable[j]--;
#if C_DEVLIB == 4
                SetAmplifierGain(MGetChannel(),10*i);
#else
                SetAmplifierGain(10*i);
#endif
            }
            else if( keycode == 1)
            {
                gateposi++;
                MDrawGate(0,0, -1, 0);
                MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                MDrawGate(0,0, -1, 0);
                continue;
            }
            else if( keycode == 2)
            {
                gateposi--;
                MDrawGate(0,0, -1, 0);
                MSetGatePara(gateposi,gatewide,140,0,C_SETMODE_SETSAVE);
                MDrawGate(0,0, -1, 0);
                continue;
            }
            else if( keycode == 3)
            {
                range+= 50;
                MSetRange(range,C_SETMODE_SETSAVE);
                WriteScale();	//���
                continue;
            }
            else if( keycode == 4)
            {
                range-= 50;
                MSetRange(range,C_SETMODE_SETSAVE);
                WriteScale();	//���
                continue;
            }
            else if( keycode == C_KEYCOD_RETURN)break;
            else continue;
        }
        if( keycode == C_KEYCOD_RETURN)break;
    }

    offset = C_OFF_STACK;
    MCopyProtectedMemory( (void*)offset, GainTable,  121*4, PM_COPY_WRITE);

    MFclearScreen();
    for(i=0; i<120; i++)
    {
        Write_Number((i%6)*44,(i/6)*10+30,GainTable[i],4,0,0);
    }

    MAnyKeyReturn();

}

