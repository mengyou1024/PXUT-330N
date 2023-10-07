/*test.c,���ֲ��Ժ���*/
#include "sdcard.h"

extern UDISK_INFO g_UDiskInfo;
extern const unsigned short c_SystemPowerStd[];
extern GATE_PEAK			GatePeak;

#if C_MAX_DOT_NUMBER > C_SCREEN_WIDTH
uint16_t dac_line[C_MAX_DOT_NUMBER];
#else
uint16_t dac_line[C_SCREEN_WIDTH];
#endif
uint16_t daccurve[C_SCREEN_WIDTH];

typedef struct _TagPOINT
{
    long x;
    long y;
} POINT,*LPPOINT;

int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2);

void HotKeySet(void)
{
    uint32_t char_len = 6, crow_height = 48;
    int xpos,ypos;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 0) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    int row;
    uint16_t* cpText = 0;
    int i;
    int len;
    int item;
    uint32_t elapsedtime1 = GetElapsedTime();
    int offset;
    int crnum = 0;

    //for( i = 0 ; i < 4 ; i++)Function.funcMenu[i] = 0; //����ԭ����

    //MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);

    offset = C_OFF_KEYFUNC;
    MCopyProtectedMemory( & Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
    crnum = 5;
    //SystemParaStore();
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    while(true)
    {
        para_ypos =  4;
        MFclearScreen();
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT+16, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(uint8_t*)_FuncHotKey_A[MGetLanguage()][i],4);
        }
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }
        for( i = 0,row = 1; i < crnum-1 ; i++,row++)
        {
            xpos = para_xpos + C_ECHAR_HDOT+24*MGetLanguage();
            ypos = para_ypos + crow_height *row ;
            //MSetColor( i ) ;
            TextOut(xpos,ypos,1,17,crow_height,(uint8_t*)_FuncHotKey_B[MGetLanguage()][Function.funcMenu[i]]+2,4);
            // MSetColor(C_CR_UNDO);
        }
        MKeyRlx();		/*ֻ�а������ͷŲ��˳�*/

        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*������ʾ�����ֵ�����*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode <= crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );

        if (keycode == 0)
        {
            Function.funcMenu[0]=1;
            Function.funcMenu[1]=2;
            Function.funcMenu[2]=7;
            Function.funcMenu[3]=0;
            continue;
        }
        else if (keycode == C_KEYCOD_RETURN)
        {
            //  SystemParaRestore();
            //  ChannelParaRestore();

            offset = C_OFF_KEYFUNC;
            MCopyProtectedMemory( &Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
            offset = C_OFF_KEYFUNC;
            MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_LEN_FUNCTION,PM_COPY_WRITE);
            // SystemParaStore();
            //  ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        if(MGetLanguage())
        {
            para_ypos-=C_ECHAR_VDOT;
        }
        row = item = keycode ;
        //  row--;
        item--;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*������ʾ�����ֵ�����*/
        ypos = para_ypos;

        len = 6;
        row = 9;
        MPushWindow(xpos, ypos, xpos+ (len+5+10*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        EraseDrawRectangle(xpos, ypos, xpos+ (len+5+10*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        for( i = 0; i < row ; i++)
        {
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+i*crow_height,1,17,crow_height,(uint8_t*)_FuncHotKey_B[MGetLanguage()][i],4);
        }
        while(1)
        {
            keycode = MenuKeyReturn(row,2);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
            if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                break;
            else if(keycode > 0 && keycode <= row)
            {
                Function.funcMenu[item]=keycode-1;
                break;
            }
        }
        MPopWindow();
    }
    MSetAcquisition(1);
    ClearEnvelope();	//�������ֵ
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);		//�»�DAC

    return ;
}
void AmplitudedBTable(void)
{
    int i;
    int xpos=105;	//��ʼ����
    int ypos=48;
    int Gain[16],High[16];
    int key=C_KEYCODMAX;
    for (i=0; i<16; i++)
    {
        Gain[i]=0;
        High[i]=0;
    }
    MSetColor(C_CR_UNDO);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    //���Ը���
    MSetAcquisition(0);	//�ز���,ֻ������
    //��������100,ֻ��ĸ��,
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,0);
    }
    MSetSurfGain(0,C_SETMODE_SETSAVE);
    MSetRange(1000,C_SETMODE_SETSAVE);
    MSetSystem();

    DrawDac(2);
    MSetBaseGain( MGetBaseGain()-200*log10( dac_line[48]/320.0),C_SETMODE_SETSAVE);
    DrawDac(2);
    MSetBaseGain( MGetBaseGain()-200*log10( dac_line[48]/320.0),C_SETMODE_SETSAVE);
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);

    uint32_t elapsedtime = GetElapsedTime() + 100;

    while( GetElapsedTime() < elapsedtime || key==C_KEYCODMAX)
    {
        key=MGetKeyCode(0);
        // MAnyKeyReturn();
    }
    for (i=0; i<16; i++)
    {
        High[i]=(dac_line[24+24*i]*10)/4+0.5;
    }
    for (i=0; i<16; i++)		//����dBֵ
    {

        DrawDac(2);
        MSetBaseGain( MGetBaseGain()-200*log10( dac_line[24+24*i]/320.0),C_SETMODE_SETSAVE);
        Gain[i]=MGetBaseGain();
    }
    //����
    MSetAcquisition(0);
    MFclearScreen();
    MSetColor(C_CR_MENU);
    TextOut(10,36,1,30,32,(uint8_t*)_AmplitudedB[MGetLanguage()][0],4*(1-MGetLanguage()));
	TextOut(325,36,1,30,32,(uint8_t*)_AmplitudedB[MGetLanguage()][0],4*(1-MGetLanguage()));
	MSetColor(C_CR_PARA);
    EMenuOut(45,ypos+36," 510152025303540",2,8,48);
	EMenuOut(360,ypos+36,"4550556065707580",2,8,48);
	MSetColor(C_CR_MENU);
	
    // DrawRectangle(xpos,ypos,SCREEN_WIDTH-xpos,SCREEN_HEIGHT-ypos);
    for (i=0; i<10; i++)
    {
        CheckDrawLine(5,ypos*(i)+24,SCREEN_WIDTH-5,ypos*(i)+24);

        // Write_Number(xpos*3+32,ypos*(2+i)+16,Gain[i],5,1,0);
        // Write_Number(xpos*5+32*2+16,ypos*(2+i)+16,High[i],5,1,0);
        if (i<7)
        {
            CheckDrawLine(xpos*i+5,24,xpos*i+5,SCREEN_HEIGHT-24);
        }
    }
	for (i=0; i<16; i++)
    {
		if(i<8)
		{
			Write_Number(xpos*1+30,ypos*(i+1)+36,Gain[i],5,1,0);
			Write_Number(xpos*2+30,ypos*(i+1)+36,High[i],5,1,0);
		}
		else
		{
			Write_Number(xpos*4+30,ypos*(i-8+1)+36,Gain[i],5,1,0);
			Write_Number(xpos*5+30,ypos*(i-8+1)+36,High[i],5,1,0);
		}

    }

    MAnyKeyReturn();
    SystemParaRestore();
    ChannelParaRestore();
}

int MenuChoice(void)	//ѡ��˵�
{
    uint32_t char_len = 5,row_number = 9,crow_height = 38;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    uint16_t *cpText;
    uint8_t *epText;
    int i;
    int prestatus=0;
    uint32_t elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;
    int revalue=1;

    row_number = 10;
    crow_height =40;// C_ECHAR_VDOT*2;

    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();

    DisplayPrompt(16);

    xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1+6*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * (row_number)+6*C_FILL ) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;

    for (i=0; i<row_number; i++)
    {
        epText = (uint8_t*) _Menu_Choice[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,11+5*MGetLanguage(),crow_height,epText,4*(1-MGetLanguage()));
        //  if (MGetLanguage())
        {
            //      TextOut(xpos+4,ypos+3+C_ECHAR_VDOT+crow_height*i,1,11,8,epText+11,0);
        }
    }


    MKeyRlx();
    while (revalue)
    {
        keycode=MGetKeyCode(0);
        revalue=0;
        switch (keycode)
        {
        case 1:	//���ܲ˵�
            FuncMenu();
            return C_KEYCOD_RETURN;
        case 2:	//���ò˵�
            ChannelMenu();
            return C_KEYCOD_RETURN;
            break;
        case 3:	//ѡ��˵�
            ParaMenu(1);
            return C_KEYCOD_RETURN;
            break;
        case 4:	//������У

            if(MGetSystemMode() == 1)break;
            if (MGetSaveStatus() == 1)break;
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);		//�»�DAC
            TestMenu();
            return C_KEYCOD_RETURN;
            break;
        case 5:	//���ݴ���
            if(MGetSystemMode() == 1)break;
            if (MGetSaveStatus() == 1)break;
				DataProcedure();
            break;
        case 6:	//��׼
            if( MGetTestStatus(C_TEST_AVG) == 1)break;//����AVG����ʾ��׼�˵�
            if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;	//��������
            STDMenu();
            return C_KEYCOD_RETURN;
        case 7:	//ɫ��
            ColorMenu();
            return C_KEYCOD_RETURN;
            break;
        case 8:	//����dB��
            if(MGetUnitType()==0 && (MGetTestStatus(C_TEST_DAC) == 1 || (MGetTestStatus(C_TEST_AVG) == 1 && MGetAvgMode()==0)))
            {
                AmplitudedBTable();
            }
            break;
        case 9:	//�������
            HotKeySet();
            return C_KEYCOD_RETURN;
            break;
		case 0:	//��ʼ��
            Usb2PC();
            //InitPara();
            break; 
        case C_KEYCOD_RETURN:
        case C_KEYCOD_CONFIRM:
            break;
        default:
            revalue=1;
            break;
        }
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//�������ֵ
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();
    DrawDac(0);
    return C_KEYCOD_RETURN;
}
int TestChoice(int Test)
{
    uint32_t char_len = 5,row_number = 9,crow_height = 38;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    uint16_t *cpText;
    uint8_t *epText;
    int i;
    int prestatus=0;
    uint32_t elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;
    //int revalue=-1;

    row_number = 2;
    crow_height = 48;

    if(MGetSystemMode() == 1)return -1;

    if (MGetSaveStatus() == 1)return -1;

    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();

    DisplayPrompt(16);
    if (Test == C_TEST_OFFSET)
    {
        WriteParaName(C_OFFSET);
        //EMenuOut(C_CCHAR_HDOT * 8, 0,"us",2,1,12);
        TextOut(C_CCHAR_HDOT * (8+MGetLanguage()*4), 5,1,4,16,(uint8_t*)_us[0][0],0);
        EMenuOut(C_CCHAR_HDOT * (8+MGetLanguage()*4)+C_ECHAR_HDOT+4, 0,"s",1,1,12);
        WriteOffset((4+MGetLanguage()*4)*C_CCHAR_HDOT,0);
    }
    else if (Test == C_TEST_ANGLE)
    {

        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut(7*C_CCHAR_HDOT,0,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(7*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
        }
        WriteParaName(C_ANGLE);
        WriteAngle(4*C_CCHAR_HDOT,0);
        Write_Ascii(4*C_CCHAR_HDOT + 4 * C_ECHAR_HDOT,0 ,ANGLE);
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {
            TextOut(7*C_CCHAR_HDOT,0,1,4,7,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(7*C_CCHAR_HDOT,0,( MGetAngle(1) + 50)/100,4,2,0);
        }
        // MChannelRenovate();
        //	Write_Number(4*C_CCHAR_HDOT,8,( MGetAngle(1) + 50)/100,4,2,0);
        //	Write_Number(C_RANGE_HPOSI + 12 * C_ECHAR_HDOT, C_ECHAR_HDOT + 1, (MGetAngle(1) + 50)/100, 4,2,0);
    }
    else if (Test == C_TEST_DAC)
    {
        MSetTestStatus(C_MAKE_DACAVG,0);	//���ó�����DAC
        keycode = TestMenuDac();
        SystemParaRestore();
        ChannelParaRestore();
		if(MGetTestStatus(C_TEST_DAC) == 1 )
		{
			MSetGatePara(C_COORHORIUNIT * 0.8,C_COORHORIUNIT*15.1/2,140,0,C_SETMODE_SETSAVE);
		}
        MSetSystemMode(0,C_SETMODE_SETSAVE);	//���óɲ���״̬
        MSetSystem();
        ScreenRenovate();
        DrawDac(0);
        MSetAcquisition(1);
        return C_KEYCOD_RETURN;
    }
    xpos = C_COORWIDTH - ( char_len + 1 ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1)*C_CCHAR_HDOT, ypos + crow_height * (row_number)+6*C_FILL ) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;

    for (i=0; i<row_number; i++)
    {
        epText = (uint8_t*) _TestMenu_Menu[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,10-2*MGetLanguage(),crow_height,epText,0);
        //    if (MGetLanguage())
        {
            //       TextOut(xpos+4,ypos+3+10+crow_height*i,1,8,8,epText+11,0);
        }
    }


    MKeyRlx();
    while (1)
    {
        keycode=MGetKeyCode(0);
        if (keycode==1 ||keycode==7  ||keycode==8 )
        {
            if (Test == C_TEST_OFFSET &&(keycode==7 || keycode==1) )
            {
                keycode = TestMenuOffset();//�����
            }
            else if (Test == C_TEST_ANGLE && (keycode==8 || keycode==1) )
            {
                keycode = TestMenuAngle();//��Ƕ�
            }
            else if (Test == C_TEST_DAC)
            {
            }
            break;
        }
        else if (keycode==2)
        {
            MKeyRlx();
            keycode=-1;

            MSetColor(C_CR_UNDO);
            ClearEnvelope();	//�������ֵ
            SystemParaRestore();
            ChannelParaRestore();

            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            MSetSystem();
            ScreenRenovate();	/*��Ļˢ��*/
            DrawDac(0);
            if (Test == C_TEST_OFFSET)
            {
                keycode = OffsetChange();
            }
            else if (Test == C_TEST_ANGLE)
            {
                keycode = AngleChange();
            }
            else if (Test == C_TEST_DAC)
            {

            }
            break;
        }
        else if(keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
        {
            //revalue=C_KEYCOD_RETURN;
            break;
        }
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//�������ֵ
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	/*��Ļˢ��*/
    DrawDac(0);
    return C_KEYCOD_RETURN;
}
int TestMenu(void)
{
    if(MGetSystemMode() == 1)return C_FALSE;

    if (MGetSaveStatus() == 1)return C_FALSE;

    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 12 ) / 8 * 8;
    int ypos = C_COORVPOSI;

    int keycode;
    int char_len = 6;
    int row_number = 6;
    uint32_t row_height = 48;
    int retvalue = 0;
#if C_DEVTYPE == 1
    row_number = 6;
#elif C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    row_number = 5;
#elif C_DEVTYPE == 3 || C_DEVTYPE == 8 || C_DEVTYPE == 11 || C_DEVTYPE == 12
    row_number = 6;
#else
    if(MGetProbeMode() == 0)
	{
		row_number = 8;
	}
	else
	{
		row_number = 7;
	}
#endif

    ClearEnvelope();	//�������ֵ
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, xpos+ (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+16, ypos + row_number * row_height) ;
//	CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_A1,char_len,row_number,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    TextOut(xpos + C_ECHAR_HDOT-1,ypos+4,row_number,13,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][0],4);
    MSetColor(C_CR_UNDO);

    MSetSystemMode(1,0);		//���ó��Զ�����״̬
    keycode = MenuKeyReturn(row_number,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
    //;MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*������,�������������*/
    MChannelRenovate();

    DisplayPrompt(15);
    switch (keycode)
    {
    case 1:
        retvalue = TestMenuOffset();
        break;
    case 2:

        retvalue = C_TRUE;
        if (MGetOffset() == 0)
        {
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE)
                {
                    retvalue = C_FALSE;
                }
            }
        }
        if(retvalue == C_TRUE)
            retvalue = TestMenuAngle();
        break;
    case 3:
        MSetTestStatus(C_MAKE_DACAVG,0);	//���ó�����DAC
        retvalue = TestMenuDac();
        break;
    case 4:
        MSetTestStatus(C_MAKE_DACAVG,1);	//���ó�����AVG
        retvalue = TestMenuDac();
        break;
    case 5:
        retvalue = TestMenuSystem();
        break;
    case 6:
#if C_MAKELIB == 1
        retvalue = TestMenuSensitivity();
#else
        TestFlawHigh();		//���
        retvalue = C_FALSE;
#endif
        break;
    case 7:
#if C_DEVTYPE != 3 && C_DEVTYPE != 8 && C_DEVTYPE != 12 && C_DEVTYPE != 11
        //   break;
        TestMenuFft();  //��Ƶ��
#else
        TestMenuSystem();
#endif
        retvalue = C_FALSE;
        break;
    case 8:
		if(MGetProbeMode() == 0)
		{
			retvalue = TestForging();
		}
        break;
    case 9:
        retvalue = TestMenuThick();
        break;
    case C_KEYCOD_RETURN:
        MKeyRlx();
        break;
    }
    if( retvalue == C_FALSE)
    {
        SystemParaRestore();
        ChannelParaRestore();
    }
    else
    {
        SystemParaStore();
        ChannelParaStore();
    }
    MKeyRlx();
    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//�������ֵ
    SystemParaRestore();
    ChannelParaRestore();
    MSetSystemMode(0,C_SETMODE_SETSAVE);	//���óɲ���״̬
    MSetSystem();
    ScreenRenovate();
    DrawDac(0);
    MSetAcquisition(1);
    return C_TRUE;
}

int TestMenuOffset(void)
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 8 - C_ECHAR_HDOT * (13+6*MGetLanguage())) / 8 * 8;
    int ypos = C_COORVPOSI;
    uint32_t para_xpos,para_ypos;
    uint32_t testrange[3];		//��������ֵ
    uint32_t offset;
    int deci_len, number = 1;
    //uint16_t* cpText;
    //uint32_t crow_height = 24;

    int retvalue = -1;
    uint32_t row_height = 48;
    int keycode;

    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+1,5,1,row_height);
    TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][0]+2,4);
    EraseDrawRectangle(xpos, ypos, xpos+ (14+3*MGetLanguage())*C_CCHAR_HDOT+4, ypos + 4 * row_height+24) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AB1,8,4,row_height);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,4,17,row_height,(uint8_t*)_TestMenu_AB1[MGetLanguage()][0],4);
//    EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

    MSetColor(C_CR_UNDO);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT-8;	/*д������λ��ˮƽ��������*/
    para_ypos = ypos + 4;

    offset = C_OFF_TEMP + 0;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);
    while(retvalue == -1)
    {
        /*����*/
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        WriteSpeed(xpos,ypos+C_ECHAR_VDOT+1 );


        /*һ������*/
        ypos = para_ypos + row_height *1 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*
        if( MGetUnitType() > 0)
        {//��λinch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"inch",4,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm  ",4,1,8);
        }
        //*/

        /*��������*/
        ypos = para_ypos + row_height *2 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[2],5,1);
        /*
        if( MGetUnitType() > 0)
        {//��λinch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"inch",4,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm  ",4,1,8);
        }
        //*/

        keycode = MenuKeyReturn(3,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            if( MGetUnitType() > 0)deci_len = 2;//��λinch
            else deci_len = 0;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_SPEED)
                    {
                        MSetSpeed(number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 30000)
                    {
                        testrange[1] = number;
                        if( MGetProbeMode() == C_P_TRANSMISSION)testrange[2] = number * 3;
                        else testrange[2] = number * 2;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 30000*2 )
                    {
                        testrange[2] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            if( testrange[1] > 10)
            {
                retvalue = 1;
                offset = C_OFF_TEMP + 0;
                MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
            }
            break;
        }
    }
    if(retvalue == 1)
    {
        USER_INFO UserInfo;
		int offsets = C_OFF_USER_INFO;
		MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
        MSetTestStatus(C_TEST_DAC,0);	//���ò���״̬,DAC��
        MSetTestStatus(C_TEST_AVG,0);	//���ò���״̬,AVG��
        MSetFunctionMode(0,C_FUNCTION_ALL);
		if(UserInfo.pzPurchaser[0]==0)
		{
			MSetFunctionMode(UserInfo.pzSerial[0],C_ECHOMAX_MEMORY);
			MSetFunctionMode(UserInfo.pzSerial[1],C_ECHO_ENVELOPE);
		}
			
        MSetSystem();
        // MChannelRenovate();	//��ͨ����Ļ����ˢ��

        while(1)
        {
            int Zerooffset=MGetOffset();
            if (TestOffset(testrange[1],testrange[2],0) == C_FALSE)
            {
                MSetOffset(Zerooffset,C_SETMODE_SETSAVE) ;
                return C_FALSE;
            }
            return C_TRUE;
            //;if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    return C_FALSE;
}

int TestOffset(uint32_t testrange1,uint32_t testrange2,uint32_t mode)
{
    /*
    t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
    */
    GateMax.Amp = 0;	//��߲��ó�ֵΪ0

    int gain = 400,range = 1000,offset = 0, offset0 = 0, delay = 0,speed = MGetSpeed();
    uint32_t echotime = 0;
    short gateamp;
    int i;
    uint32_t char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos,menu_xpos,menu_ypos;
    int confirm_num = 0;
    uint32_t preElapsedtime;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;
    //int i;
//	int elapsedtime = GetElapsedTime() + 1000;
    uint32_t testrange0 = testrange1;


    MSetEchoMode(0,C_SETMODE_SAVE);
    if( MGetProbeMode() == C_DPROBEZ)
    {
        offset0 = 160*800/( testrange1 + testrange2 );
        if( offset0 > 160 * 8)offset0 = (offset0 - 160 * 8)/2 + 160 * 8;
        if( offset0 > 160 * 4)offset0 = (offset0 - 160 * 4)/2 + 160 * 4;
        if( offset0 > 160 * 2)offset0 = (offset0 - 160 * 2)/2 + 160 * 2;;
        MSetGatePara(C_COORHORIUNIT * 6-2,C_COORHORIUNIT * 3.5,120,0,C_SETMODE_SETSAVE);
    }
	else if(MGetProbeMode() == C_DPROBEA)
	{
		offset0 = 160*800/( testrange1 + testrange2 );
        if( offset0 > 160 * 8)offset0 = (offset0 - 160 * 8)/2 + 160 * 8;
        if( offset0 > 160 * 4)offset0 = (offset0 - 160 * 4)/2 + 160 * 4;
        if( offset0 > 160 * 2)offset0 = (offset0 - 160 * 2)/2 + 160 * 2;;
        MSetGatePara(C_COORHORIUNIT * 3-2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
	}
    else MSetGatePara(C_COORHORIUNIT * 4-2,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);

    MKeyRlx();

    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
	if(testrange0>=160)range = testrange0 * 10/4;		//���ڵ�4��
	else range= 400;
//	if( MGetProbeMode() == C_P_TRANSMISSION)range /= 2;
    MSetPara(gain,0,0,2,range,offset0,delay,MGetAngle(0),speed);
//	MSetPulseMode(0,C_SETMODE_SAVE);
    MSetSystem();
    MChannelRenovate();	//��ͨ����Ļ����ˢ��
	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;
	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
    if(UserInfo.pzPurchaser[0]==1)
    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetAcquisition(1);

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-30*MGetLanguage();
    menu_ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())) ;
//	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB3 ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestMenu_AB3[MGetLanguage()][0], 4);

    WriteLongness(menu_xpos + 9 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage() ,testrange0,4/*�ܳ�*/,0/*�Ƿ�д��λ*/);
    if( MGetUnitType() > 0)EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+72*MGetLanguage(), menu_ypos  + 20+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);
    ///Write_Number(menu_xpos + 11 * C_ECHAR_HDOT,menu_ypos  + 4 ,testrange0,4,1,0);
    MSetColor(C_CR_UNDO);


    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    MSetGateParaInit();	//������߲����ֵ

    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

    // MSetSystemMode(1,0);		//���ó��Զ�����״̬
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+1,5,1,24);
    TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][0]+2,4);
    while (1)
    {
        ///if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
        {
            if(MAdjustGain(0,1,0,MGetAmpStdMax()))
            {
                MSetGateParaInit();	//������߲����ֵ
            }

        }
        keycode = MGetKeyCode(0);
        MParaRenovate(1);
        gateamp = MGetAmpMax(gatetype);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if(MGetGateParaMax(gatetype) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
        }

#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
        if(keycode == C_KEYCOD_CONFIRM && confirm_num == 0)
        {
            //t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)

            confirm_num++;
            ClearCursor(2);
            //echotime = MGetAmpTime(gatetype);
            //testrange0 = MGetAmpDist(gatetype);
            echotime = GateMax.Time;
            testrange0 = GateMax.Dist;
            //
            offset = echotime - testrange1*C_SAMPLE_FREQ/speed;//�Ȱ�Ԥ�����ٺ�һ�β�ʱ��������

            MKeyRlx();

            if( testrange2 <= testrange1 )
            {
                //�����������С��һ�����̻�Ϊ0����ֻ��һ��
                confirm_num++;
                break;
            }
            else
            {
                //range = MGetRange(3) + (testrange2 + testrange0 - 2*testrange1) * 100/40;		//���ڵ�4��
                range = (testrange2 + testrange0 - testrange1) * 100/40;		//���ڵ�4��
                testrange0 = testrange2;
//				if( MGetProbeMode() == C_P_TRANSMISSION)range /= 2;
                gain = MGetBaseGain()+120;
                MSetRange(range,C_SETMODE_SETSAVE);
                MSetBaseGain(gain,C_SETMODE_SAVE);
                MSetSystem();

				if( MGetProbeMode() == C_DPROBEZ||MGetProbeMode() == C_DPROBEA )MSetGatePara(C_COORHORIUNIT * 2-7,C_COORHORIUNIT * 4,120,0,C_SETMODE_SETSAVE);
                else MSetGatePara(C_COORHORIUNIT * 4-7,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);


                if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
                else ClearEnvelope();

                MChannelRenovate();	//��ͨ����Ļ����ˢ��

                //WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
                //WriteScale();	//���

                GateMax.Amp = 0;

                //MGateDraw();
                //MGateDraw();

                char_len = 7;
                row_number = 3;
                crow_height = 48-14*MGetLanguage();
                xpos = C_COORHORIUNIT * 10-(char_len+1)*C_CCHAR_HDOT;
                xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-16*MGetLanguage();
                ypos = C_COORVPOSI + 1;

                MSetColor(C_CR_MENU);
                EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height * (row_number+MGetLanguage())) ;
                //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB3 ,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestMenu_AB3[MGetLanguage()][0], 4);

                WriteLongness(menu_xpos + 9 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage() ,testrange0,4/*�ܳ�*/,0/*�Ƿ�д��λ*/);
                if( MGetUnitType() > 0)EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage(),"in",2,1,8);
                else EMenuOut(menu_xpos + 9 * C_ECHAR_HDOT+72*MGetLanguage(), menu_ypos  + 20+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);
            }
        }
        else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 1)
        {
            confirm_num++;
            //offset = MGetAmpTime(gatetype) - echotime ;	//T2 - T1
            offset = GateMax.Time - echotime ;	//T2 - T1
            offset = echotime - testrange1 * offset / (testrange2 - testrange1);

            speed = testrange1 * C_SAMPLE_FREQ/(echotime - offset);
            MSetSpeed(speed,C_SETMODE_SETSAVE);

            break;

        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
                //SystemParaRestore();
                //ChannelParaRestore();
                break;
            }

        }
        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {
            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;

            /*Write_Number(50,100,keycode,2,0,0);
            Write_Number(50,120,Function.funcMenu[0],2,0,0);
            Write_Number(50,140,Function.funcMenu[1],2,0,0);
            Write_Number(50,160,Function.funcMenu[2],2,0,0);
            Write_Number(50,180,Function.funcMenu[3],2,0,0);
            */
            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//������߲����ֵ
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }
            SysParaDisp();
        }
#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            MSetGateParaInit();	//������߲����ֵ
            MKeyRlx();
        }
        else if (keycode == C_KEYCOD_GATE)
        {
            GateChange();
            MKeyRlx();
        }
        else if (keycode == C_KEYCOD_RANGE)
        {
            RangeChange();
            MKeyRlx();
        }
        //else if (keycode == C_KEYCOD_OFFSET)
        //{
        //	OffsetChange();
        //}
        else continue;
        DisplayPrompt(15);
        MSetColor(C_CR_MENU);
        //	CMenuOut(0,0,_TestMenu_A1+1,5,1,24);
        TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][0]+2,4);
        //ExpendTime(10);
    }
    if( confirm_num > 1)
    {
//		if( MGetProbeMode() == C_P_TRANSMISSION)offset /= 2;
        MSetOffset(offset + MGetOffset(),C_SETMODE_SETSAVE);
        if( MGetProbeMode() == C_APROBE||MGetProbeMode() == C_DPROBEA )
        {
            //б̽ͷ
            char_len = 8;
            row_number = 2;
            crow_height = 48-14*MGetLanguage();

            menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-16*MGetLanguage();
            menu_ypos = C_COORVPOSI + 1;

            //;MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*������,�������������*/

            MChannelRenovate();
            MSetColor(C_CR_MENU);
//			CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB7,char_len,row_number,crow_height);
            EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + (crow_height+MGetLanguage()) * row_number+8*MGetLanguage()) ;
            TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+6, row_number+1*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestMenu_AB3[MGetLanguage()][1], 4);

            int number,deci_len;

            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = testrange1;
            menu_xpos = menu_xpos+8*C_ECHAR_HDOT+C_ECHAR_HDOT*3*MGetLanguage();
            menu_ypos = C_COORVPOSI + 50+MGetLanguage();
            EMenuOut(menu_xpos + 4 * C_ECHAR_HDOT, menu_ypos,"mm",2,1,24);
            while(1)
            {
                if( Input_Number(menu_xpos, menu_ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number <= testrange1)
                    {
                        ///Write_Number(menu_xpos, menu_ypos, number, 2, 1, 0);
                        WriteLongness(menu_xpos,menu_ypos,number,4,1);
                        MSetForward(testrange1-number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
        }
        MSetTestStatus(C_TEST_OFFSET,1);	//���ò���״̬,����Ѳ�

        MSetGatePara(C_COORHORIUNIT * 3, C_COORHORIUNIT * 2 ,120,0,C_SETMODE_SETSAVE);

        //	MChannelRenovate();	//��ͨ����Ļ����ˢ��
        //SystemParaStore();
        //ChannelParaStore();

        retvalue = C_TRUE;
    }

    MKeyRlx();
    range = testrange1 * 100/40;		//���ڵ�6.5��
    MSetRange(range,C_SETMODE_SETSAVE);
    WriteScale();	//���
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
	MSetFunctionMode(0,C_ECHO_ENVELOPE);

    return retvalue;
}

int TestMenuAngle(void)
{
	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;
	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 11 - C_ECHAR_HDOT * (10+MGetLanguage()*11)) / 8 * 8;
    int ypos = C_COORVPOSI;
    uint32_t para_xpos,para_ypos;
    uint32_t testrange[2];		//[0]��������ȣ�[1]������ֱ����
    uint32_t offset;
    int deci_len, number = 1;

    int retvalue = -1;
    uint32_t row_height = 48;
    int keycode;

    ScreenRenovate();
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
//	CMenuOut(0,0,_TestMenu_A1+7,5,1,row_height);
    TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][1]+2,4);

    if(UserInfo.pzPurchaser[0]==0)
		MSetKvalue(UserInfo.pzSerial[15],C_SETMODE_SAVE);
	else
	{
    	if( MGetAngle(0) < 10)
			MSetKvalue(200,C_SETMODE_SAVE);
	}

//	EraseDrawRectangle(xpos, ypos, xpos+ 14*C_CCHAR_HDOT, ypos + 4 * row_height) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AC1,9,4,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    EraseDrawRectangle(xpos, ypos, xpos+ (16+3*MGetLanguage())*C_CCHAR_HDOT+16, ypos + 4 * row_height+24) ;
    //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AB1,8,4,row_height);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,4,18,row_height,(uint8_t*)_TestMenu_AC1[MGetLanguage()][0],4);
//    EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

    MSetColor(C_CR_UNDO);

    offset = C_OFF_TEMP + 20;
    MCopyProtectedMemory( testrange, (void*)offset, 8, PM_COPY_READ);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT+8;	/*д������λ��ˮƽ��������*/
    para_ypos = ypos + 4;

    while(retvalue == -1)
    {
        /*ֱ�� */
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[0],5,1);
        /*
        if( MGetUnitType() > 0)
        {//��λinch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"in",2,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        }
        //*/
        /*���*/
        ypos = para_ypos + row_height *1 ;
        WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*
        if( MGetUnitType() > 0)
        {//��λinch
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],4,3,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"in",2,1,8);
        }
        else
        {
        	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],4,1,0);
        	EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        }
        //*/
        /*���Kֵ\����*/
        xpos = para_xpos  ;
        ypos = para_ypos + row_height *2;
        if (MGetAngle(0)>=C_MAX_ANGLE)
        {

            Write_Ascii(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+6  ,' ');
            Write_Ascii(xpos + 1 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+6  ,' ');
            TextOut(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT-6+4 ,1,4,10,(char *)_infinity[MGetLanguage()][0],0);
        }
        else
        {
            Write_Number(xpos + 0 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1+4 ,( MGetAngle(1) + 50)/100,4,2,0);
        }
        Write_Ascii(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,'/');
        Write_Number(xpos + 6 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,MGetAngle(0),4,1,0);
        Write_Ascii(xpos + 10 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT+1 ,ANGLE);


        keycode = MenuKeyReturn(3,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[0] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            if( MGetUnitType() > 0)deci_len = 3;//��λinch
            else deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE-1)
                    {
                        testrange[1] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            deci_len = 2;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 9000)
                    {
                        if(number<1000)		/*б̽ͷ����Kֵ*/
                            MSetKvalue(number,C_SETMODE_SAVE);
                        else MSetAngle(number/10,C_SETMODE_SAVE);	/*���õ�ǰͨ��	*/

                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            retvalue = 1;
            offset = C_OFF_TEMP + 20;
            MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue == 1 && MGetAngle(0) > 0)
    {
		MSetFunctionMode(0,C_FUNCTION_ALL);
		if(UserInfo.pzPurchaser[0]==0)
		{
			MSetFunctionMode(UserInfo.pzSerial[0],C_ECHOMAX_MEMORY);
			MSetFunctionMode(UserInfo.pzSerial[1],C_ECHO_ENVELOPE);
		}
		MSetSystem();
        while(1)
        {
            if (TestAngle(testrange[0],testrange[1]) == C_FALSE)return C_FALSE;
            return C_TRUE;

            //;if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    else if(retvalue == 1 && MGetAngle(0) == 0)return C_TRUE;
    return C_FALSE;
}

int TestAngle(uint32_t diameter,uint32_t depth)	//ֱ�������
{
    GateMax.Amp = 0;	//��߲��ó�ֵΪ0

    int gain = 600,range = 1000,angle;
    //uint32_t echotime;
    uint32_t gateamp;

    uint32_t char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos;
    uint32_t preElapsedtime;
    int retvalue = C_TRUE;
    int keycode,i;
    int gatetype = 0;
    double dAngle;
//	int elapsedtime = GetElapsedTime() + 1000;
    uint32_t testrange0 = depth * 10000 / MGetAngle(3) - diameter/2;
    //MGetAngle(3) = 34.2
    //depth = 0.03
    MSetAcquisition(1);

    MKeyRlx();
    MSetScaleMode(2,0);
	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;
	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
    if(UserInfo.pzPurchaser[0]==1)
		MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetDelay(0,C_SETMODE_SAVE);
    MSetBaseGain(gain,C_SETMODE_SAVE);
    MSetCompGain(0,C_SETMODE_SAVE);
    MSetSurfGain(0,C_SETMODE_SAVE);
    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);

    range = testrange0 * 100/30;		//���ڵ�7��
    MSetRange(range,C_SETMODE_SAVE);
    MSetSystem();

    MChannelRenovate();	//��ͨ����Ļ����ˢ��

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORHORIUNIT *5-17-MGetLanguage()*C_CCHAR_HDOT;// 6-(char_len+1)*C_CCHAR_HDOT-17;
    ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, xpos+(char_len+3+MGetLanguage()*2)*C_CCHAR_HDOT+6*MGetLanguage(), ypos + crow_height * (row_number+MGetLanguage())) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestMenu_AB3[MGetLanguage()][2], 4);

    WriteLongness(xpos + 9 * C_ECHAR_HDOT+32*MGetLanguage(), ypos  + 4+(crow_height+13)*1*MGetLanguage() ,depth,4/*�ܳ�*/,0/*�Ƿ�д��λ*/);
    if( MGetUnitType() > 0)EMenuOut(xpos + 9 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 20+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(xpos + 9 * C_ECHAR_HDOT+94*MGetLanguage(), ypos  + 22+(crow_height-4)*1*MGetLanguage(),"mm",2,1,8);

    GateMax.Amp = 0;
    xpos = 0;
    ypos = 0;
    ClearCursor(2);

    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

    ///MSetSystemMode(1,0);		//���ó��Զ�����״̬

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+7,5,1,24);
    TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][1]+2,4);
    while (1)
    {
//		AdjustTime.ParaDisplayTime++;
        ///if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
        {
            if (MAdjustGain(0,1,0,MGetAmpStdMax()) )
            {
                GateMax.Amp = 0;	//�����Զ����ڹ�
            }
        }
        MParaRenovate(1);
        keycode = MGetKeyCode(0);
        gateamp = MGetAmpMax(gatetype);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if(MGetGateParaMax(gatetype) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
        }
        dAngle = acos( (float)depth / (float)( GateMax.Time * MGetSpeed() /(C_SAMPLE_FREQ) + diameter / 2)  ) ;
//		if( AdjustTime.ParaDisplayTime > C_RENOVATE_TIME )
        if(GetElapsedTime() > preElapsedtime + 30)
        {
            Write_Number(C_ECHAR_HDOT*10,0,(int)( 1800 * dAngle/C_PI),4,1,0);
            Write_Ascii(C_ECHAR_HDOT*14,0,ANGLE);
            Write_Ascii(C_ECHAR_HDOT*15,0,'/');
            Write_Number(C_ECHAR_HDOT*16,0,(int)( 100*tanf(dAngle) + 0.5),4,2,0);

//			AdjustTime.ParaDisplayTime=0;
        }
        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            MKeyRlx();

            angle = (int)( 1800.0 * dAngle/C_PI +0.5 );
            MSetAngle(angle,0);

            MSetTestStatus(C_TEST_ANGLE,1);	//���ò���״̬,Kֵ�Ѳ�
            //SystemParaStore();
            //ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            int iret=DisplayQuery(4);

            if( iret == 1)
            {
                retvalue = C_FALSE;
//				SystemParaRestore();
//				ChannelParaRestore();
                break;
            }
            else if(iret == 2)
            {

				DisplayPrompt(15);
				MSetColor(C_CR_MENU);
				TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][1]+2,4);
            }
        }

        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {
            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;


            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//������߲����ֵ
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }

            SysParaDisp();
        }

#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            MSetGateParaInit();	//������߲����ֵ
            MKeyRlx();
        }
//;		else if(keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            KeyManage(keycode,1);
            MKeyRlx();
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            //CMenuOut(0,0,_TestMenu_A1+7,5,1,24);
            TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][1]+2,4);
        }
#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
    }
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
	MSetFunctionMode(0,C_ECHO_ENVELOPE);
    MKeyRlx();
    return retvalue;
}

int TestMenuDac(void)
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 9 - C_ECHAR_HDOT * (9+8*MGetLanguage())) / 8 * 8-32;
    int ypos = C_COORVPOSI;
	int k, i, j;
    uint32_t para_xpos,para_ypos;
    uint32_t testrange[3];		//�����ȡ�ֱ��������
    uint32_t offset;
    uint16_t* cpText = 0;
    int deci_len, number = 1;

    int retvalue = -1;
    uint16_t row_height = 48, row_number = 3;//�˵�����

    int keycode;

    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        if (MGetTestStatus(C_TEST_OFFSET) == 0 || MGetTestStatus(C_TEST_ANGLE) == 0 || MGetOffset() == 0 || MGetAngle(0) == 0 )
        {
            if( DisplayQuery(2) == 1)
            {
                if (MGetTestStatus(C_TEST_OFFSET) == 0|| MGetOffset() == 0)
                {
                    if( TestMenuOffset() == C_FALSE)
                    {
                        retvalue = C_FALSE;
                        MChannelRenovate();
                        MSetSystemMode(0,0);		//���ó��Զ�����״̬
                        return 0;
                    }
                    SystemParaStore();
                    ChannelParaStore();
                }
                if (MGetTestStatus(C_TEST_ANGLE) == 0|| MGetAngle(0) == 0)
                {
                    if( TestMenuAngle() == C_FALSE )
                    {
                        retvalue = C_FALSE;
                        MSetSystemMode(0,0);		//���ó��Զ�����״̬
                        ScreenRenovate();
                        return 0;
                    }
                    SystemParaStore();
                    ChannelParaStore();
                }
            }
        }
    }
    else
    {
        if (MGetTestStatus(C_TEST_OFFSET) == 0 || MGetOffset() == 0 )
        {
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE)
                {
                    retvalue = C_FALSE;
                    MChannelRenovate();
                    MSetSystemMode(0,0);		//���ó��Զ�����״̬
                    return 0;
                }
            }
        }
    }

    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	//��ͨ����Ļ����ˢ��
    MSetColor(C_CR_MENU);
    DisplayPrompt(15);
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
    else
        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);



    EraseDrawRectangle(xpos, ypos, xpos+ (13+3*MGetLanguage())*C_CCHAR_HDOT + C_CCHAR_HDOT, ypos + (row_number+1) * row_height+24) ;

    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,(row_number+1),17,row_height,(uint8_t*)_TestMenu_DAC[MGetLanguage()][0],4);
//        EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if( MGetTestStatus(C_TEST_DAC) != 0 )
        {
            MEraseWindow(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24, xpos+C_ECHAR_HDOT*18,ypos+4+row_height*3+24*2) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24,(1),17,row_height,(uint8_t*)_TestMenu_DAC[MGetLanguage()][4],4);
//            EMenuOut(xpos + C_ECHAR_HDOT+8,ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AD1A,7,(row_number+1),row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1+21,7,1,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    }
    else
    {
        TextOut(xpos + C_ECHAR_HDOT,ypos+4+24,(row_number+1),17,row_height,(uint8_t*)_TestMenu_AVG[MGetLanguage()][0],4);
//        EMenuOut(xpos + 2*C_ECHAR_HDOT+8-2*C_ECHAR_HDOT*MGetLanguage(),ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if( MGetTestStatus(C_TEST_AVG) != 0 )
        {
            MEraseWindow(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24, xpos+C_ECHAR_HDOT*18,ypos+4+row_height*3+24*2) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3+24,(1),17,row_height,(uint8_t*)_TestMenu_AVG[MGetLanguage()][4],4);
//            EMenuOut(xpos + C_ECHAR_HDOT+8,ypos+3 * row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AD1,7,(row_number+1),row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
        //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1A+21,7,1,row_height);
        //	if( MGetTestStatus(C_TEST_AVG) == 0  )
        //		CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4+row_height*3,_TestMenu_AD1+21,7,1,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    }
    MSetColor(C_CR_UNDO);
    MSetAvgMode(0,C_SETMODE_SETSAVE);

    para_xpos = xpos + (9+3*MGetLanguage()) * C_CCHAR_HDOT;	/*д������λ��ˮƽ��������*/
    para_ypos = ypos + 4;
	
	MSetProbeMode(MGetProbeMode(),C_SETMODE_SAVE);	/*����̽ͷģʽ*/
	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;
	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
	if(MGetProbeMode() != 1&&MGetProbeMode() != 4)	//��б̽ͷ
	{
		/*if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//��λinch
		else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);

		if( MGetUnitType() > 0)
			MSetCrystal_l(20000/25.4,C_SETMODE_SETSAVE);//��λinch
		else
			MSetCrystal_l(20000,C_SETMODE_SETSAVE);
		MSetCrystal_w(0,C_SETMODE_SETSAVE);
		MSetAngle(0,C_SETMODE_SAVE);
		MSetForward(0,C_SETMODE_SETSAVE);*/
		//MSetFrequence(250,2);
		if(UserInfo.pzPurchaser[0]==0)
		{
			if(MGetProbeMode()==0)
			{
				testrange[1] = UserInfo.pzSerial[16];
				testrange[2] = UserInfo.pzSerial[17];
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = UserInfo.pzSerial[9];
				testrange[1] = UserInfo.pzSerial[10];
				testrange[2] = UserInfo.pzSerial[11];
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if(MGetProbeMode()==2||MGetProbeMode() == 4)
			{
				testrange[1] = UserInfo.pzSerial[18];
				testrange[2] = UserInfo.pzSerial[19];
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = UserInfo.pzSerial[12];
				testrange[1] = UserInfo.pzSerial[13];
				testrange[2] = UserInfo.pzSerial[14];
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);				
			}
		}
		else
		{
			if (MGetStdMode()==2)	//JB/T4730-2005
			{
				testrange[1] = 1000;
				testrange[2] = 0;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 2000;
				testrange[1] = 20;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if (MGetStdMode()==12) //JB/T47013-2005
			{
				testrange[1] = 1000;
				testrange[2] = 0;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 2000;
				testrange[1] = 20;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if (MGetStdMode()==7)	//CB/T3559-94
			{
				testrange[1] = 1000;
				testrange[2] = 2000;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 800;
				testrange[1] = 30;
				testrange[2] = 400;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
		}
	}
	else	//б̽ͷ
	{
		/*if( MGetUnitType() > 0)
		{
			MSetCrystal_l(13000/25.4,C_SETMODE_SETSAVE);
			MSetCrystal_w(13000/25.4,C_SETMODE_SETSAVE);
		}
		else
		{
			MSetCrystal_l(13000,C_SETMODE_SETSAVE);
			MSetCrystal_w(13000,C_SETMODE_SETSAVE);
		}
		if( MGetUnitType() > 0)
			MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//��λinch
		else
			MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);
		MSetScaleMode(2,0);
		if(MGetRange(1)>C_MAX_RANGE/2- MGetDelay(3))
		{
			MSetRange(C_MAX_RANGE/2 - MGetDelay(3), C_SETMODE_SETSAVE);
		}
		MSetScaleMode(MGetScaleMode(),0);*/

		/*if(MGetStdMode()==12)
			MSetFrequence(500,2);
		else
			MSetFrequence(250,2);*/
		if(UserInfo.pzPurchaser[0]==0)
		{
		 	if(MGetStdMode()==10||MGetStdMode()==11)///11345
			{
				MSetBlock(0);
				MSetDacMode(0,0);
				MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );
				k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//�Կ�����
				number = MGetThick()/10;
				if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
				for(i = 0 ; i < k; i++)
				{
					if(MGetStdMode()==0)break;

					else if(MGetStdMode()==10||MGetStdMode()==11)
					{
						if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

					}
					else
					{
						if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
					}

					for(j = 0; j < MGetCurveNumber(); j++)
					{
						MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
					}
					break;
				}
			}
			
			testrange[1] = UserInfo.pzSerial[2];
			testrange[2] = UserInfo.pzSerial[3];
			offset = C_OFF_TEMP + 0;
			MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			testrange[0] = UserInfo.pzSerial[4];
			testrange[1] = UserInfo.pzSerial[5];
			offset = C_OFF_TEMP + 20;
			MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
			testrange[0] = UserInfo.pzSerial[6];
			testrange[1] = UserInfo.pzSerial[7];
			testrange[2] = UserInfo.pzSerial[8];
			offset = C_OFF_TEMP + 30;
			MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);			
		}
		else
		{
			if (MGetStdMode()==2)	//JB/T4730-2005
			{
				testrange[1] = 1000;
				testrange[2] = 0;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 500;
				testrange[1] = 300;
				offset = C_OFF_TEMP + 20;
				MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
				testrange[0] = 500;
				testrange[1] = 10;
				testrange[2] = 60;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if (MGetStdMode()==12) //JB/T47013-2005
			{
				testrange[1] = 1000;
				testrange[2] = 0;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 500;
				testrange[1] = 300;
				offset = C_OFF_TEMP + 20;
				MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
				testrange[0] = 800;
				testrange[1] = 20;
				testrange[2] = 400;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if (MGetStdMode()==7)	//CB/T3559-94
			{
				testrange[1] = 500;
				testrange[2] = 1000;
				offset = C_OFF_TEMP + 0;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
				testrange[0] = 500;
				testrange[1] = 300;
				offset = C_OFF_TEMP + 20;
				MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
				testrange[0] = 800;
				testrange[1] = 30;
				testrange[2] = 400;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
			}
			else if(MGetStdMode()==10||MGetStdMode()==11)///11345
			{

				int testrange[3];
				int offset;
				testrange[0] = 800;
				testrange[1] = 30;
				testrange[2] = 400;
				offset = C_OFF_TEMP + 30;
				MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
		   }
		}
	}
	MSetSystem();
    SystemParaStore();
    ChannelParaStore();
	
    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( testrange, (void*)offset, 12, PM_COPY_READ);
    while(retvalue == -1)
    {
        /*������*/
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        /*		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[0],5,1);
        */
        if( MGetUnitType() > 0)
        {
            //��λinch
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],6,3,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
        }
        else
        {
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],6,1,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
        }
//
        /*������ֱ��*/
        ypos = para_ypos + row_height *1 ;
///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
        /*		Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
        		EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
        */

        if( MGetUnitType() > 0)
        {
            //��λinch
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,3,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
        }
        else
        {
            Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],6,1,0);
            EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
        }
//
        /*�����峤��*/
        ypos = para_ypos + row_height *2 ;
        if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
        {
///			WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[2],5,1);
            /*			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
            			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);
            */
            if( MGetUnitType() > 0)
            {
                //��λinch
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,3,0);
                EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"in",2,1,8);
            }
            else
            {
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[2],6,1,0);
                EMenuOut(xpos + 5 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"mm",2,1,8);
            }
//
            /*���油��*/
            ypos = para_ypos + row_height *3 ;
            ///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
//;			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,MGetSurfGain(),4,1,0);
//;			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"dB",2,1,8);
            while(true)
            {
                keycode = MAnyKeyReturn();
#if C_DEVTYPE == 14
                if( ( keycode >= 1 && keycode <= row_number)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_RETURN )
#else
                if( ( keycode >= 1 && keycode <= row_number)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_RETURN )
#endif
                {
                    MKeyRlx();
                    break;
                }
            }
        }
        else
        {
#if C_DEVTYPE==20
            MSetAvgMode(0,C_SETMODE_SAVE);
#endif
            ypos = para_ypos + row_height *2+24 ;
            //	cpText = (uint16_t*)_ParaMenu_AC3A + (MGetAvgMode()-0) * 5 + 1;		//����DB��Ϊ������
            //	CEMenuOut(xpos,ypos , cpText ,2,1,16);
            MEraseWindow(xpos, ypos, xpos + C_CCHAR_HDOT*2, ypos + C_CCHAR_VDOT) ;
            TextOut(xpos,ypos ,1,6,16,(uint8_t*)_ParaMenu_AC3A[MGetLanguage()][MGetAvgMode()]+2,4);
            /*���油��*/
            ypos = para_ypos + row_height *3 ;
            ///		WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,testrange[1],5,1);
//;			Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,MGetSurfGain(),4,1,0);
//;			EMenuOut(xpos + 6 * C_ECHAR_VDOT,ypos+8,"dB",2,1,8);

            testrange[2] = 0;
//			keycode = MenuKeyReturn(2,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
            while(true)
            {
                keycode = MAnyKeyReturn();
                //	if( MGetTestStatus(C_MAKE_DACAVG) != 0 && keycode == C_KEYCOD_DEL)continue;
#if C_DEVTYPE == 14
                if( ( keycode >= 1 || keycode == 2 || keycode == 3)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_RETURN )
#else
                if( ( keycode == 1 || keycode == 2 || keycode == 3)||
                        keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_RETURN )
#endif
                {
                    MKeyRlx();
                    break;
                }
            }
        }


        ypos = para_ypos + row_height * (keycode - 1);

        if( MGetUnitType() > 0)deci_len = 3;//��λinch
        else deci_len = 1;
        switch (keycode)
        {
        case 1:
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,5, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[0] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:
            //	deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= C_MAX_RANGE)
                    {
                        testrange[1] = number;
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 3:
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
            {
                //		deci_len = 1;
                number = 0;

                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_RANGE)
                        {
                            testrange[2] = number;
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
            }
            else
            {
#if C_DEVTYPE!=20
                //	cpText = (uint16_t*)_ParaMenu_AC3A ;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + C_CCHAR_VDOT * 2+4+24) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + C_CCHAR_VDOT * 2+24) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,22);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
                TextOut(xpos+4,ypos+4 ,2,8,C_CCHAR_VDOT+8,(uint8_t*)_ParaMenu_AC3A[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetAvgMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
#endif
            }
            break;
        case 4:
            //	deci_len = 1;
            number = 0;

            while(1)
            {
                if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                else
                {
                    if( number <= 80)
                    {
                        MSetSurfGain(number,C_SETMODE_SAVE);
                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case C_KEYCOD_RETURN:
            MKeyRlx();
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
#if C_DEVTYPE == 14
        case C_KEYCOD_POINT:
#else
        case C_KEYCOD_DEL:
#endif
            ChannelParaStore();
            if( MGetUnitType() > 0)
            {
                if( testrange[0] < 10 || testrange[0] > 50000 || testrange[1] > (int)(1000/0.254) || testrange[2] > (int)(1000/0.254))
                {
                    DisplayPrompt(4);
                    DisplayPrompt(15);
                    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    else
                        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);

                    break;
                }
            }
            else
            {
                if( testrange[0] < 10 || testrange[0] > 50000 || testrange[1] > 1000 || testrange[2] > 1000)
                {
                    DisplayPrompt(4);
                    DisplayPrompt(15);
                    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    else
                        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,row_height);
                        TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);

                    break;
                }
            }
            retvalue = 1;
#if C_DEVTYPE == 14
            if( C_KEYCOD_POINT == keycode )retvalue = 2;
#else
            if( C_KEYCOD_DEL == keycode )retvalue = 2;
#endif
            offset = C_OFF_TEMP + 30;
            MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue >= 1)
    {
        while(1)
        {
            if( retvalue > 1)retvalue = Dac.num;
            else retvalue = 0;

            if (TestDac(testrange[0],testrange[1],testrange[2],retvalue) == C_FALSE) return C_FALSE;
            //;if( DisplayQuery(12) == 1)
            {
                SystemParaStore();
                ChannelParaStore();
                return C_TRUE;
            }
        }
    }
    MSetSystem();
    return C_FALSE;
}

int TestDac(uint32_t depthmax,uint32_t diameter,uint32_t length,short test_num )//�����ȡ�������ֱ��������
{
	USER_INFO UserInfo;
    int offsets = C_OFF_USER_INFO;
    MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
	if(MGetStdMode()==10||MGetStdMode()==11)
	{
		g_iNum=0;
	}

	int ndiameter = diameter;
	int nlength = length;
    int gain = 500,range = 1000;
    uint32_t gateamp;

    uint8_t* sampbuffer;

    uint16_t char_len,row_number,crow_height;
    //int number,deci_len;
    short xpos,ypos,menu_xpos,menu_ypos,para_xpos,para_ypos;
    int deci_len, number = 1;

    uint32_t preElapsedtime;
    int retvalue = C_FALSE;
    short keycode,rotary;
    uint16_t gatetype = 0;

    int space;
    uint16_t dac_db[10],dac_dist[10];
    short SameKeyTime;
    int temp,i,j;
//	int offset;
    short oldbasegain;
//	short newbasegain;
    short surfgain = MGetSurfGain();
//	int elapsedtime = GetElapsedTime() + 1000;

    if( depthmax > 1500 && MGetPulseMode() == 0) MSetPulseMode(1,C_SETMODE_SETSAVE);

    MSetAcquisition(1);
	MSetFunctionMode(0,C_FUNCTION_ALL);

    ClearCursor(2);
    MKeyRlx();
    Dac.num = test_num ;	//ѡ�в�����
    for(i=0; i< 10; i++)
    {
//		Dac.db[i] = 0;
//		Dac.dist[i] = 0;
        dac_db[i] = Dac.db[i] ;
        dac_dist[i] = Dac.dist[i] ;
    }
    if( test_num == 0)
    {
        MSetThick(0);
        MSetScaleMode(0,0);
        MSetDelay(0,C_SETMODE_SAVE);
        MSetBaseGain(gain,C_SETMODE_SETSAVE);
        MSetCompGain(0,C_SETMODE_SAVE);
        MSetSurfGain(0,C_SETMODE_SAVE);

        MSetGatePara(C_COORHORIUNIT * 1,C_COORHORIUNIT * 8,0,1,C_SETMODE_SETSAVE);
    }
    else
    {
        diameter = Dac.diameter ;
        length = Dac.length;//�����ȡ�������ֱ��������
    }
    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
//	range = depthmax * 100/80;		//���ڵ�8��
//	range = ((range+249)/250)*250;
    MSetCompGain(0,C_SETMODE_SETSAVE);
    MSetSurfGain(0,C_SETMODE_SAVE);
    range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//���ڵ�8��
    MSetRange(range,C_SETMODE_SAVE);
    MSetSystem();
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        MSetGatePara(C_COORHORIUNIT * 0.8,C_COORHORIUNIT*15.1/2,140,0,C_SETMODE_SETSAVE);
    }
    else
    {
        MSetGatePara(C_COORHORIUNIT * 7.1,C_COORHORIUNIT * 1.5,140,0,C_SETMODE_SETSAVE);
    }
    if( MGetStdMode() == C_STD_AWS_NUM +5)//AWS
    {
        MSetStdMode(0,C_SETMODE_SAVE);
    }
    short linegain[6];
    for(i = 0 ; i < 6; i++)
    {
        linegain[i] = MGetLineGain(i);
    }
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,0);
    }
    ScreenRenovate();	//��ͨ����Ļ����ˢ��
    DrawDac(0);
    DisplayPrompt(15);
    WritePeakPrompt( MGetBGateMode(),-1);

    char_len = 14+3*MGetLanguage();
    row_number = 1;
    crow_height = 48-14*MGetLanguage();
    MSetColor(C_CR_MENU);
    if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
    {
        //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
        TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
        menu_xpos = C_COORHORIUNIT * 11-(char_len+3+MGetLanguage())*C_CCHAR_HDOT-20*MGetLanguage();
        menu_ypos = C_COORVPOSI ;
    }
    else
    {
        //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
        TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);
        menu_xpos = 0;
        menu_ypos = C_COORVPOSI ;
    }


    EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
    MSetColor(C_CR_MENU);
    if(test_num > 0)
    {
        TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][2],4);
//        EMenuOut(menu_xpos+8 + (16+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][0],4);
//	if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
//	else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);

    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

    MSetSystemMode(1,0);			//���ó��Զ�����״̬
    MSetTestStatus(C_TEST_DAC,0);	//���ò���״̬,DAC��
    MSetTestStatus(C_TEST_AVG,0);	//���ò���״̬,AVG��

    GateMax.Amp = 0;	//��߲��ó�ֵΪ0
    xpos = 0;
    ypos = 0;

    if( MGetUnitType() > 0)
    {
        diameter = diameter*254/1000;
        length = length*254/1000;//�����ȡ�������ֱ��������
    }



    while(1)
    {

        keycode = MGetKeyCode(0);
        while (test_num == 0)
        {
            MParaRenovate(1);
            ///if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
            {
                if (MAdjustGain(0,0,0,MGetAmpStdMax()))
                {
                    GateMax.Amp = 0;	//�����Զ����ڹ�
                }
            }
            if( GateMax.Amp == 0)
            {
                DrawDac(0);
                GateMax.Amp = 1;
            }

            gateamp = MGetAmpMax(gatetype);
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE) )
            {
                if ( MGetGateParaMax( gatetype) == C_TRUE  )
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
            }


            oldbasegain = MGetBaseGain();
            keycode = MGetKeyCode(0);
            if(keycode != C_KEYCODMAX) break;
            else if( MGetRotary(-1) != 0)break;
        }

        MParaRenovate(1);
        if(GateMax.Amp == 0)
        {

//;			newbasegain = MGetBaseGain();
//;			MSetBaseGain(oldbasegain,C_SETMODE_SAVE);
//;			DrawDac(1);
//;			MSetBaseGain(newbasegain,C_SETMODE_SAVE);
//;			DrawDac(1);
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            if(test_num > 0)
            {
                TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][2],4);
//                EMenuOut(menu_xpos + (16+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_CCHAR_VDOT);
            }
            else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][0],4);

            /*			EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
            			MSetColor(C_CR_MENU);
            			if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
            			else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
            */
            GateMax.Amp = 1;
        }

        gateamp = MGetAmpMax(gatetype);
        if ( !MGetFunctionMode(C_ECHOMAX_MEMORY) && !MGetFunctionMode(C_ECHO_ENVELOPE))
        {
            if( MGetGateParaMax( gatetype) == C_TRUE )
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
            }
        }
        else
        {
            ///xpos = GateMax.Pos;
			ClearCursor(2);
            int iAmpMax=0,iPosMax=0;
		    uint8_t SampBuf[C_LEN_SAMP+1] ;
            uint32_t  offset1 = C_OFF_SAMP ;
            MCopyProtectedMemory( SampBuf, (void*)offset1, C_LEN_SAMP, PM_COPY_READ);

            for(i=MGetGatePara(0,0);i<MGetGatePara(0,0)+MGetGatePara(0,1);i++)
            {
            	if(iAmpMax<=SampBuf[i])
            	{
            		iAmpMax=SampBuf[i];
            		iPosMax=i;
            	}
            }
            xpos = iPosMax;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - iAmpMax*2 ;
            if((iAmpMax*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
        }

        if(keycode == C_KEYCODMAX)
        {
            /*
            			rotary = MGetRotary(-2);
            			if(rotary > 0)rotary = 1;
            			else if( rotary < 0)rotary = -1;
            			if( rotary != 0)
            			{
            				BaseGainPlus(20*rotary);
            				WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
            			}
            			rotary = MGetRotary(1);
            			if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
            			else if( rotary < 0)keycode = C_KEYCOD_LEFT;
            			else continue;
            /*/
            rotary = MGetRotary(-1);
            if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
            else if( rotary < 0 )keycode = C_KEYCOD_LEFT;
            else continue;
//*/
        }
        else rotary  = 0;
        SameKeyTime = TestSameKeyTime(keycode,GetElapsedTime(),C_SAMEKEYTIME);
        oldbasegain = MGetBaseGain();
#if C_ACQU == 1
        if (GetElapsedTime() >= elapsedtime)
        {
            elapsedtime = GetElapsedTime() + 1000;
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
        }
#endif
        if( keycode == C_KEYCOD_CONFIRM && test_num != 0)
        {
            ClearEnvelope();	//�������ֵ
            ClearCursor(2);
            if(MGetTestStatus(C_MAKE_DACAVG) == 0)
                MSetTestStatus(C_TEST_DAC,1);	//���ò���״̬,DAC����
            else
            {
                MSetTestStatus(C_TEST_AVG,1);	//���ò���״̬,AVG����
                if( MGetCurveNumber() > 3)MSetCurveNumber(3);//AVG������3����
            }
            MSetEquivalentDays(1,1);
            MSetCompGain(0,C_SETMODE_SETSAVE);
            range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//���ڵ�8��
            MSetRange(range,C_SETMODE_SAVE);
            Dac.diameter = ndiameter;//diameter;
            Dac.length = nlength;//length;//�����ȡ�������ֱ��������
			MSetParaLock(1, C_LOCK_DAC);	//ͨ��������������
///			MSetParaLock(1, C_LOCK_OFFSET);	//���kֵ���ٶ�������
///			MSetParaLock(1, C_LOCK_PROBE);	//ͨ��������������
/////////////////////////////////////////////////////////////////////

            MKeyRlx();

            ExpendTime(50);
            MChannelRenovate();
            xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 7 - C_ECHAR_HDOT * (9+5*MGetLanguage())) / 8 * 8;
            ypos = C_COORVPOSI+C_CCHAR_VDOT;

            if (MGetTestStatus(C_MAKE_DACAVG) == 1)
            {
                if (MGetLanguage())
                {
                    EraseDrawRectangle(xpos-112, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + (1+3*MGetLanguage())*C_ECHAR_HDOT, ypos + 2 * crow_height) ;
                    //  TextOut(xpos + C_ECHAR_HDOT-10,ypos+10-24, 1, 14,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0]+14, 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+1, 1, 15,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height, 1, 16,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][2], 4);
                }
                else
                {
                    EraseDrawRectangle(xpos, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 2 * crow_height) ;
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 14,C_CCHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 14,C_CCHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][2], 4);
                }
//                EMenuOut(xpos + 2*C_ECHAR_HDOT+3*C_CCHAR_HDOT+8,ypos+4+ crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
            }
            else
            {
                if (MGetLanguage())
                {
                    EraseDrawRectangle(xpos-112, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                    //  TextOut(xpos + C_ECHAR_HDOT-16,ypos+10-24, 1, 14,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0]+14, 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+1, 1, 15,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height, 1, 15,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][1], 4);
                    TextOut(xpos + C_ECHAR_HDOT-112,ypos+4+crow_height*2, 1, 16,C_ECHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][2], 4);
                }
                else
                {
                    EraseDrawRectangle(xpos, ypos-C_CCHAR_VDOT, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 14,C_CCHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][0], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 14,C_CCHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][1], 4);
                    TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height*2, 1, 14,C_CCHAR_VDOT, (uint8_t*)_TestMenu_set[MGetLanguage()][2], 4);
                }
//                EMenuOut(xpos + 2*C_ECHAR_HDOT+2*C_CCHAR_HDOT+8,ypos+4+ 2* crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
            }
            MKeyRlx();

            para_xpos = xpos + 6 * C_CCHAR_HDOT+16;	/*д������λ��ˮƽ��������*/
            para_ypos = ypos + 4-C_CCHAR_VDOT;

            length = MGetThick();
			 if(UserInfo.pzPurchaser[0]==0)
            {
                if(MGetProbeMode()==0)
                    surfgain = UserInfo.pzGovernor[2];
                else if(MGetProbeMode()==1)
                    surfgain = UserInfo.pzGovernor[1];
                else if(MGetProbeMode()==2||MGetProbeMode() == 4)
                    surfgain = UserInfo.pzGovernor[3];
            }
            else
                surfgain = MGetSurfGain();
            retvalue = -1;
            while(retvalue == -1)
            {
                /*���油��*/
                xpos = para_xpos;
                ypos = para_ypos + crow_height *0 ;
                Write_Number(xpos,ypos+C_ECHAR_VDOT+1,surfgain,4,1,0);
                EMenuOut(xpos + 4 * C_ECHAR_HDOT,ypos+C_ECHAR_VDOT,"dB",2,1,8);
				MSetSurfGain(surfgain,C_SETMODE_SAVE);

                if (MGetTestStatus(C_MAKE_DACAVG) == 0)
                {
                    /*�������*/
                    ypos = para_ypos + crow_height *1 ;
                    WriteLongness(xpos,ypos+C_ECHAR_VDOT+1,MGetThick(),4,1);
                }
                while(true)
                {
                    keycode = MAnyKeyReturn();
                    if( ( keycode >= 1 && keycode <= 2)||
                            keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
                    {
                        MKeyRlx();
                        break;
                    }
                }

                ypos = para_ypos + crow_height * (keycode - 1);

                if( MGetUnitType() > 0)deci_len = 3;//��λinch
                else deci_len = 1;
                switch (keycode)
                {
                case 1:
                    deci_len = 1;
                    number = 0;
                    if ( Input_Number(xpos,ypos,&number,3, &deci_len,0) == 1)
                    {
                        if( number <= 500)
                            MSetSurfGain(number,C_SETMODE_SAVE);
						surfgain = MGetSurfGain();
                    }
                    break;
                case 2:
                    if (MGetTestStatus(C_MAKE_DACAVG) == 0)
                    {
                        number = 0;
                        if ( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                        {
                            if( number <= 60000)
                                MSetThick(number);
                        }
                    }
                    break;
                case C_KEYCOD_CONFIRM:
                    length = MGetThick();
                    surfgain = MGetSurfGain();
                    MSetDiameter(0);
                    retvalue = 1;
                    break;

                case C_KEYCOD_RETURN:
                    retvalue = 2;
                    break;
                }
            }
////////////////////////////////////////////////////////////
            if (retvalue == 2)	//���ز��˳�
            {

                EraseDrawRectangle(xpos, ypos, xpos+ 10*C_CCHAR_HDOT + C_ECHAR_HDOT, ypos + 3 * crow_height) ;
                ScreenRenovate();	//��ͨ����Ļ����ˢ��
                MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
                MSetColor(C_CR_MENU);
                if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                {
                    //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                    TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
                    menu_xpos = C_COORHORIUNIT * 11-(char_len+3+MGetLanguage())*C_CCHAR_HDOT-20*MGetLanguage();
                    menu_ypos = C_COORVPOSI ;
                }
                else
                {
                    //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                    TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);
                    menu_xpos = 0;
                    menu_ypos = C_COORVPOSI ;
                }

                EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
                MSetColor(C_CR_MENU);
                if(test_num > 0)
                {
                    TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][2],4);
//                    EMenuOut(menu_xpos+8 + (25+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
                }
                else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][0],4);

                /*				EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
                				MSetColor(C_CR_MENU);
                				if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
                				else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
                */
                DrawDac(0);
                continue;
            }

            MSetThick(length);
            MSetSurfGain(surfgain,C_SETMODE_SAVE);

            retvalue = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
            number = MGetThick()/10;

            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//��λinch
            if (length == 0 || MGetTestStatus(C_MAKE_DACAVG) == 1)
            {
                //	MSetStdMode(0,C_SETMODE_SAVE);
                for( i = 0 ; i < retvalue; i++)
                {
                    for( j = 0; j < 6; j++)
                    {
                        linegain[j] = 0;
                    }
                    break;

                }
            }
            else
            {
                for( i = 0 ; i < retvalue; i++)
                {
                    if(MGetStdMode()==10||MGetStdMode()==11)
                    {
                        MSetBlock(0);//2019.3.19
                        if( number>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;
                        //MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );
                        //MSetCurveNumber( 2);
						for( j = 0; j < MGetCurveNumber(); j++)
						{
							MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
						}
						break;

                    }
                    else
                    {
                        if( number>c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;

                    }


                    for( j = 0; j < 6; j++)
                    {
                        linegain[j] = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1];

                    }
                    break;
                }
            }

            retvalue = C_TRUE;

            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            int iret=DisplayQuery(4);
            if( iret == 1)
            {
                ClearCursor(2);
                retvalue = C_FALSE;
                SystemParaRestore();
                ChannelParaRestore();
                break;
            }
            else if(iret==2)
            {
				if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
				{
					 DisplayPrompt(15);
					//	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
					TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
				}
				else
				{
					 DisplayPrompt(15);
					//	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
					TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);
				}
            }

        }
        else if( (gateamp > 5 || MGetFunctionMode(C_ECHOMAX_MEMORY) ) && (keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT || ( keycode == C_KEYCOD_CONFIRM && MGetTestStatus(C_MAKE_DACAVG)  && test_num == 0 ) ) )
        {
            int search = 1;
            uint8_t echobuff[C_LEN_SAMP+1] ;
            uint32_t  offset = C_OFF_SAMP ;

            if( MGetFunctionMode(C_ECHOMAX_MEMORY) ) search = 0;
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            MCopyProtectedMemory( echobuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
            sampbuffer = GetSampleBuffer();
//;			ClearCursor(2);
            if(keycode == C_KEYCOD_CONFIRM)keycode = C_KEYCOD_RIGHT;

            //if( search )
            {
                if(keycode == C_KEYCOD_RIGHT)xpos-= 2;
                else if(keycode == C_KEYCOD_LEFT)xpos+= 2;	//ʹ�õ�ǰ��߲��ܵ�һ�α�ѡ��
            }
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
            MSetColor(C_CR_MENU);
            //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD3 ,char_len,row_number,crow_height);
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+4*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][1],4);
//            EMenuOut(menu_xpos +8+ (11+2*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

            ExpendTime(10);
            MKeyRlx();
            while( xpos > 0 )	//��SearchPeak����-1ʱ��ʾδ���ҵ���߲�
            {
                if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                {

                    if( !search )
                    {
                        //search = 1;
                        ClearCursor(2);
                        sampbuffer = echobuff;
                        xpos = SearchPeak(sampbuffer,xpos,keycode);
                    }
                    else
                    {
                        ClearCursor(2);
                        sampbuffer = GetSampleBuffer();
                        xpos = SearchPeak(sampbuffer,xpos,keycode);
                    }
                    if( xpos <= 0 || xpos >= C_COORWIDTH)break;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (*(sampbuffer + xpos))*2;


                    DrawCursor(xpos,ypos,2);
                    ExpendTime(20);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
				   if( MGetFunctionMode(C_ECHOMAX_MEMORY))ClearEnvelope();///4.19��
                    ClearCursor(2);
                    if( test_num > 0)DrawDac(1);		//����Ϊ1��ʾ��DAC
                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 )test_num = 0;
                    dac_dist[test_num] = MGetRange(3) * xpos / C_COORWIDTH;	//�õ����������
                    if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//����MM
                    ///gateamp = *(sampbuffer + xpos) * 100 / 98;
                    gateamp = *(sampbuffer + xpos) ;

                    if ( MGetFunctionMode(C_ECHOMAX_MEMORY))
                    {
                        gateamp -=1;
                    }
                    dac_db[test_num] = MGetBaseGain() + MGetCompGain() + (int)( 200 * ( log10(255) - log10(gateamp) ) );
                    //�軯Ϊ128%=255ʱ������ֵ
                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 )
                    {
                        //test_num = 0;
                        if( test_num < 1 )
                        {
                            if( diameter < 5)
                            {
                                //��ƽ��
                                if( MGetUnitType() > 0)
                                {
                                    i =  (int)( 200.0*log10(2.0*(MGetSpeed()*254/1000)* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ) + 0.5 );
                                }
                                else
                                {
                                    i =  (int)( 200.0*log10(2.0*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ) + 0.5 );
                                }
//Write_Number(10,30,MGetSpeed(),8,0,0);
//Write_Number(10,40,dac_dist[0],8,0,0);
//Write_Number(10,50,(int)( MGetFrequence() * 100.0*C_PI*4),8,0,0);
//Write_Number(10,60,200*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4),8,0,0);
//Write_Number(10,70,200*log10(2.0*MGetSpeed()* dac_dist[0]/( MGetFrequence() * 100.0*C_PI*4) ),8,0,0);
                                //		i -= 20;
                                dac_db[0] += i;
                            }
                            else
                            {
                                //ƽ�׿�
                                i =  (int)( 400.0 * log10(diameter/20.0) + 0.5);
                                dac_db[0] += i;
                            }
                            test_num++;
                        }

                        /*		ETextOut(10,50,"i:              ",9);
                        		Write_Number(10,60,i,4,0,0);
                        		ETextOut(10,70,"dac_db:         ",9);
                        		Write_Number(10,80,dac_db[0],4,0,0);
                        		ETextOut(10,90,"dac_dist:       ",9);
                        		Write_Number(10,100,dac_dist[0],4,0,0);
                        */
                    }
                    else test_num++;

                    for(i = 0; i < test_num-1; i++)
                    {
                        if(depthmax>= 400)
                        {
                            space=25;
                        }
                        else if (depthmax>= 200)
                        {
                            space=15;
                        }
                        else
                        {
                            space=10;
                        }

                        if( Mabsi( dac_dist[i] - dac_dist[test_num-1] ) < space )	//������С��2.5mm
                        {
                            test_num--;
                            dac_db[i] = dac_db[test_num];
                            break;
                        }
                    }

                    for(i = 0; i < test_num; i++)
                    {
                        //�����е㰴ǳ��˳������
                        for(j = i+1; j < test_num; j++)
                        {
                            if(dac_dist[i] > dac_dist[j] )
                            {
                                temp = dac_dist[i];
                                dac_dist[i] = dac_dist[j];
                                dac_dist[j] = temp;
                                temp = dac_db[i];
                                dac_db[i] = dac_db[j];
                                dac_db[j] = temp;
                            }
                        }
                    }
                    for(i = 1; i < test_num; i++)	//���������ڿ����ĵ�ɾ��һ��
                    {
                        if(depthmax>= 400)
                        {
                            space=25;
                        }
                        else if (depthmax>= 200)
                        {
                            space=15;
                        }
                        else
                        {
                            space=10;
                        }

                        if( Mabsi( dac_dist[i-1] - dac_dist[i] ) < space )	//������С��2.5mm
                        {
                            test_num--;
                            for(j = i; j< test_num ; j++)
                            {
                                dac_dist[j] = dac_dist[j+1];
                                dac_db[j] = dac_db[j+1];
                            }
                        }
                    }
                    if(test_num == 10)	//�������࣬�Բ��ֿ����ĵ���кϲ�
                    {
                        temp = (dac_dist[9] - dac_dist[0])/10;

                        for(i = 1; i < 9; i++)	//��1��͵�10�㲻���д���
                        {
                            if(dac_dist[i] - dac_dist[i-1] <= temp )
                            {
                                //��������С��10��ƽ�����룬��ɾ����һ��
                                test_num--;
                                for(j = i; j< test_num ; j++)
                                {
                                    dac_dist[j] = dac_dist[j+1];
                                    dac_db[j] = dac_db[j+1];
                                }
                                break;
                            }
                        }
                    }
                    Dac.num = test_num;	//ѡ�в�����
                    for(i=0; i< 10; i++)
                    {
                        Dac.db[i] = dac_db[i];
                        Dac.dist[i] = dac_dist[i];
                    }

                    //offset = C_OFF_DAC + 0*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,dac_db, 10*C_SIZE_SHORT, PM_COPY_WRITE);
                    //offset = C_OFF_DAC + 10*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,dac_dist, 10*C_SIZE_SHORT, PM_COPY_WRITE);
                    //offset = C_OFF_DAC + 20*C_SIZE_SHORT;
                    //MCopyProtectedMemory((void*)offset,&test_num, 1*C_SIZE_SHORT, PM_COPY_WRITE);

//			offset = C_OFF_DAC ;
//			MCopyProtectedMemory((void*)offset, &Dac, C_LEN_DAC, PM_COPY_WRITE);

                    if(MGetTestStatus(C_MAKE_DACAVG) == 1 && MGetAvgMode() == 1)
                    {
                        {
                            MSetTestStatus(C_TEST_AVG,1);	//���ò���״̬,AVG����
                            if( MGetCurveNumber() > 3)MSetCurveNumber(3);//AVG������3����
                        }
                        MSetEquivalentDays(1,1);
                        MSetCompGain(0,C_SETMODE_SETSAVE);
                        MSetSurfGain(surfgain,C_SETMODE_SAVE);
                        range = (int)( ( depthmax * 100/80 )/50.0+0.9)*50;		//���ڵ�8��
                        MSetRange(range,C_SETMODE_SAVE);
                        Dac.diameter = diameter;
                        Dac.length = length;//�����ȡ�������ֱ��������

                        retvalue = C_TRUE;
                        break;
                    }
                    DrawDac(0);	//����Ϊ0��ʾ��DAC
                    GateMax.Amp = 1;

                    MSetCompGain(0,C_SETMODE_SETSAVE);
                    WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    break;
                }
                while(true)
                {
                    keycode = MGetKeyCode(20);
                    if(keycode == C_KEYCODMAX)
                    {
                        rotary = MGetRotary(-1);
                        if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
                        else if( rotary < 0)keycode = C_KEYCOD_LEFT;
                        else continue;
                        break;
                    }
                    if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode ==  C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM )
                    {
                        MKeyRlx();
                        rotary  = 0;
                        break;
                    }
                    else continue;
                }
                ///if( MGetFunctionMode(C_ECHOMAX_MEMORY))ClearEnvelope();///4.19
            }
            if( retvalue == C_TRUE)break;
            ExpendTime(20);//��ʱ0.2s
            MSetGateParaInit();	//������߲����ֵ
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
        }
        else if( keycode == C_FUNC_1 ||keycode == C_FUNC_2||keycode == C_FUNC_3||keycode == C_FUNC_4)
        {

            if(keycode == C_FUNC_1)i=0;
            else if(keycode == C_FUNC_2)i=1;
            else if(keycode == C_FUNC_3)i=2;
            else if(keycode == C_FUNC_4)i=3;


            if(Function.funcMenu[i]==C_ECHO_ENVELOPE+1 )
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
                {
                    MSetFunctionMode(1,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                }
                else if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                }
                MKeyRlx();
            }
            else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
            {
                ClearEnvelope();
                if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                {
                    MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetGateParaInit();	//������߲����ֵ
                }
                else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                {
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MKeyRlx();
            }

            SysParaDisp();
        }
        else if(keycode == C_KEYCOD_BASEGAIN && SameKeyTime > C_SAMEKEYTIME)
        {
            GainChange();
            MSetColor(C_CR_MENU);
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
            else
                //	CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);
            GateMax.Amp = 0;
            MKeyRlx();
        }
#if C_DEVTYPE == 1 || C_DEVTYPE == 15
        else if(keycode == C_KEYCOD_AUTOGAIN )
#else
        else if(keycode == C_KEYCOD_BASEGAIN )//&& GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
#endif
        {
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
            else ClearEnvelope();
            DrawDac(1);

            if ((ChannelPara.wavepara[2]<150 && ChannelPara.wavepara[2]>70)|| test_num == 0)
            {
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            else
            {
                MAdjustGain(1,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            MSetGateParaInit();	//������߲����ֵ
            GateMax.Amp = 0;
            DrawDac(0);
            MKeyRlx();
        }
#if C_DEVTYPE == 14
        else if (keycode == C_KEYCOD_POINT && test_num > 0)
#else
        else if (keycode == C_KEYCOD_DEL && test_num > 0)
#endif
        {
            int num = 0;
            if( !MGetFunctionMode(C_ECHOMAX_MEMORY))ClearCursor(2);
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
            MSetColor(C_CR_MENU);
            //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD6 ,char_len,row_number,crow_height);
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][3],4);
//            EMenuOut(menu_xpos +8+ (17+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

            ExpendTime(10);
            MKeyRlx();
            MSetCompGain(0,C_SETMODE_SETSAVE);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
            xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//�õ����������
            gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
            while(true)
            {
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - gateamp*2;
                DrawCursor(xpos,ypos,2);

                while(true)
                {
                    keycode = MGetKey();
                    if(keycode == C_KEYCODMAX)
                    {
                        rotary = MGetRotary(-1);
                        if( rotary > 0 )keycode = C_KEYCOD_RIGHT;
                        else if( rotary < 0)keycode = C_KEYCOD_LEFT;
                        else continue;
                        break;
                    }
#if C_DEVTYPE == 14
                    else if(keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_POINT || keycode == C_KEYCOD_CONFIRM  || keycode == C_KEYCOD_RETURN )
#else
                    else if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_DEL || keycode == C_KEYCOD_CONFIRM  || keycode == C_KEYCOD_RETURN )
#endif
                        break;
                }
#if C_DEVTYPE == 14
                if( keycode == C_KEYCOD_PLUS )
                {
                    gateamp+=3;
                }
                else if(keycode == C_KEYCOD_MINUS)
                {
                    gateamp-=3;
                }
                else if(keycode == C_KEYCOD_LEFT)
                {
                    num--;
                    if( num <0)num = test_num-1;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//�õ����������
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }
                else if(keycode == C_KEYCOD_RIGHT)
                {
                    num++;
                    if( num >= test_num)num = 0;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//�õ����������
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }

#else
                if( keycode == C_KEYCOD_RIGHT )
                {
                    gateamp+=3;
                }
                else if(keycode == C_KEYCOD_LEFT)
                {
                    gateamp-=3;
                }
#if C_DEVTYPE == 14
                else if (keycode == C_KEYCOD_POINT)
#else
                else if(keycode == C_KEYCOD_DEL)
#endif
                {
                    num++;
                    if( num >= test_num)num = 0;
                    ClearCursor(2);
                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//�õ����������
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    continue;
                }
#endif
                else if(keycode == C_KEYCOD_RETURN)
                {
                    DrawDac(1);
                    for(i = num; i < test_num; i++)
                    {
                        dac_dist[i] = dac_dist[i+1];
                        dac_db[i] = dac_db[i+1];
                    }
                    test_num--;
                    Dac.num = test_num;	//ѡ�в�����
                    for(i=0; i< 10; i++)
                    {
                        Dac.db[i] = dac_db[i];
                        Dac.dist[i] = dac_dist[i];
                    }
                    if( test_num < 1)break;
                    if( num >= test_num)num = test_num-1;
                    DrawDac(0);	//����Ϊ0��ʾ��DAC
                    ClearCursor(2);
                    EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number);
                    MSetColor(C_CR_MENU);
                    //	CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD6 ,char_len,row_number,crow_height);
                    TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][3],4);
//                    EMenuOut(menu_xpos +8+ (17+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

                    xpos = dac_dist[num] * C_COORWIDTH/MGetRange(3);	//�õ����������
                    gateamp = pow(10, ( (int)( 200 * log10(255) ) - dac_db[num] + MGetBaseGain() + MGetCompGain() ) /200.0 );
                    MKeyRlx();
                    ExpendTime(20);
                    continue;
                }
                else if( keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    break;
                }
                ClearCursor(2);
                dac_db[num] = MGetBaseGain() + MGetCompGain() + (int)( 200 * ( log10(255) - log10(gateamp) ) );
                Dac.num = test_num;	//ѡ�в�����
                for(i=0; i< 10; i++)
                {
                    Dac.db[i] = dac_db[i];
                    Dac.dist[i] = dac_dist[i];
                }
                DrawDac(-1);
            }
            MSetGateParaInit();	//������߲����ֵ
            MKeyRlx();
        }
//		else if(keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            if( MGetFunctionMode(C_ECHOMAX_MEMORY))ClearEnvelope();
            KeyManage(keycode,1);
            MKeyRlx();
            oldbasegain = MGetBaseGain();

            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            if( MGetTestStatus(C_MAKE_DACAVG) == 0 )
                //	CMenuOut(0,0,_TestMenu_A1+13,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][2]+2,4);
            else
                //CMenuOut(0,0,_TestMenu_A1+19,5,1,24);
                TextOut(0,0,1,11,C_CCHAR_VDOT,(uint8_t*)_TestMenu_A1[MGetLanguage()][3]+2,4);

            GateMax.Amp = 0;
        }
        else continue;

        /*	EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1)*C_CCHAR_HDOT, menu_ypos + crow_height * row_number) ;
        	MSetColor(C_CR_MENU);
        	if(test_num > 0)CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD5 ,char_len,row_number,crow_height);
        	else CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AD2 ,char_len,row_number,crow_height);
        */
        EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (char_len+1+2*MGetLanguage())*C_CCHAR_HDOT+18, menu_ypos + crow_height * row_number) ;
        MSetColor(C_CR_MENU);

        if(test_num > 0)
        {
            TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][2],4);
//            EMenuOut(menu_xpos+8 + (20+3*MGetLanguage())*C_ECHAR_HDOT,menu_ypos+1,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        }
        else TextOut(menu_xpos + C_ECHAR_HDOT-10*MGetLanguage(),menu_ypos + 4,1,28+5*MGetLanguage(),C_CCHAR_VDOT,(uint8_t*)_TestMenu_AD2[MGetLanguage()][0],4);

    }
    for(i = 0 ; i < 6; i++)
    {
        MSetLineGain(i,linegain[i]);
    }
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
    MKeyRlx();

    if( retvalue == C_TRUE && MGetTestStatus(C_MAKE_DACAVG) == 0)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        MSetSystemMode(0,0);
        DrawDac(2);
        if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )// if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
        {
            if (MGetLineGain(0)==0 && MGetLineGain(1)==0 && MGetLineGain(2)==0 )
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/320.0),C_SETMODE_SETSAVE);
            }
            else if (MGetThick()==0)
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==2)	//JB4730
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/40.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==7||MGetStdMode()==4)	//CBT3559-94
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
             else if (MGetStdMode()==12||MGetStdMode()==9)	//JB47013
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);
            }
            else if(MGetLineGain(0)-MGetLineGain(2) < 130)
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/60.0),C_SETMODE_SETSAVE);
            }
            else
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/60.0),C_SETMODE_SETSAVE);
            }
        }
        else
        {
            MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/40.0),C_SETMODE_SETSAVE);
        }
        if (MGetThick()>=30)
        {
            int Scale=MGetScaleMode();
            MSetScaleMode(0,C_SETMODE_SETSAVE);
            if (MGetThick()<160)
            {
                MSetRange(MGetThick() * 100/30+0.5,C_SETMODE_SETSAVE);
            }
            else if(MGetThick()>490)
            {
                MSetRange(MGetThick() * 100/80+0.5,C_SETMODE_SETSAVE);
            }
            else
            {
                MSetRange(MGetThick() * 100/40+0.5,C_SETMODE_SETSAVE);
            }
            MSetScaleMode(Scale,C_SETMODE_SETSAVE);
        }
    }
    return retvalue;
}


int TestMenuStandard(void)
{
    return 0;
}

int TestMenuThick(void)
{
    return 0;
}

int TestMenuSystem(void)
{
    return MenuMonthTest();//������У��
}

int CalcuDac(void)
{
    int offset;
    short test_num = 0;	//ѡ�в�����
    short near_time = 1;
    int dac_db[10+2],dac_dist[10+2];	//�ڻ�DAC����ʱ���ӹ�β���㣬��ֵ��ԭͷβ��ͬ
//	uint16_t dac_line[308*2];
    //int basegain;
    int i,j;
    int dist1,dist2;
    int delay,range;

    if( (MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 )&& MGetAvgMode() == 1)
    {
        near_time = 0;
    }
    else near_time = 3;

    test_num = Dac.num ;	//ѡ�в�����

    if(test_num < 1 || test_num > 10)return -1;

    range = MGetRange(3);
    if( MGetUnitType() > 0)range = range*254/1000;
    delay = MGetDelay(3);
    if( MGetUnitType() > 0)delay = delay*254/1000;


    for(i=0; i< 10; i++)
    {
        dac_db[i] = Dac.db[i];
        dac_dist[i] = Dac.dist[i];
    }

    if( MGetTestStatus(C_MAKE_DACAVG) == 1 || MGetTestStatus(C_TEST_AVG) == 1)
    {
        //AVG
        if( dac_dist[0] <= MGetNearField(near_time) )
        {
            // ��һ����������������
            for(i = test_num; i > 0; i--)	//���������һ��,����ͷ
            {
                dac_dist[i] = dac_dist[i-1];
                dac_db[i] = dac_db[i-1];
            }
        }
        else
        {
            // ��һ����������������
            for(i = test_num + 1; i > 1; i--)	//���е����������,����ͷ
            {
                dac_dist[i] = dac_dist[i-2];
                dac_db[i] = dac_db[i-2];
            }
            test_num++;
            if( MGetNearField(near_time) < dac_dist[2]/5) dac_dist[1] = dac_dist[2]/5;
            else
                dac_dist[1] = MGetNearField(near_time);//��2��Ϊ3��������

            dac_db[1] = 400*( log10(dac_dist[2]) - log10(dac_dist[1]) ) - 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[2] - dac_dist[1] );

            if( dac_db[1] >= dac_db[2] )dac_db[1] = 0;
            else dac_db[1] = dac_db[2] - dac_db[1] ;
            dac_dist[0] = 0;
            dac_db[0] = dac_db[1];
        }

        dac_dist[0] = 0;	//���ӵ�ͷ����Ϊ0��dB����ԭ��һ����ͬ
        test_num++;
        dac_dist[test_num] = range + delay;//MGetRange(3) + MGetDelay(3);		//���һ��Ϊ��ǰ�������
        ///if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//����MM
        if(dac_dist[test_num] >= 3 * dac_dist[test_num -1] )
        {
            //���һ�����Ե����Ļ�Ҷ˷ǳ�������һ��
            test_num++;
            dac_dist[test_num] = dac_dist[test_num-1];
            dac_dist[test_num - 1] = 2 * dac_dist[test_num - 2];
//			dac_db[test_num - 1] = dac_db[test_num - 2] + 120 + 2 * MGetAttenuate(dac_dist[test_num-1]/1000) * dac_dist[test_num - 2] ;
            dac_db[test_num - 1] = dac_db[test_num - 2] + 120 + 2 * MGetAttenuate(dac_dist[2]) *( dac_dist[2] - dac_dist[1] );
        }
        //dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]) - log10(dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[test_num-1]/10) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]) - log10(dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        //dac_db[test_num] = dac_db[test_num - 1] + 400*( log10(dac_dist[test_num]*1.0/dac_dist[test_num - 1]) ) + 2 * MGetAttenuate(dac_dist[2]) * ( dac_dist[test_num] - dac_dist[test_num - 1] );
        test_num++;			//����һͷһβ������
    }
    else
    {
        //DAC
        for(i = test_num; i > 0; i--)	//���������һ��,����ͷ
        {
            dac_dist[i] = dac_dist[i-1];
            dac_db[i] = dac_db[i-1];
        }
        dac_dist[0] = 0;	//���ӵ�ͷ����Ϊ0��dB����ԭ��һ����ͬ
        test_num++;
        dac_dist[test_num] = range + delay;//MGetRange(3) + MGetDelay(3);		//���һ��Ϊ��ǰ�������
        ///if( MGetUnitType() > 0)dac_dist[test_num] = dac_dist[test_num] *254/1000;//����MM
        dac_db[test_num] = dac_db[test_num-1];
        test_num++;			//����һͷһβ������
    }

    for(i = 1; i< test_num ; i++)
    {
        //����ʱ���д���
        //if(MGetDelay(3) < dac_dist[i])
        if( delay < dac_dist[i])
        {
            //����ʱ���ӵ�i-1����
            dac_db[i-1] = GetMiddleValue(dac_dist[i-1],dac_db[i-1],dac_dist[i],dac_db[i],delay );//MGetDelay(3) );
            dac_dist[i-1] = delay;//MGetDelay(3);
            test_num = test_num - (i - 1);		//���Ե����i-1����
            for(j = 0; j<test_num ; j++)
            {
                //����ʱ��ǰȥ��
                dac_dist[j] = dac_dist[j+i-1];
                dac_db[j] = dac_db[j+i-1];
            }
            break;
        }
    }

    for(i = 1 ; i < test_num; i++)//���� ��ʱ+���� һ��
    {
        if( dac_dist[i] >delay + range)// MGetDelay(3)+MGetRange(3) )
        {
            dac_db[i] = GetMiddleValue(dac_dist[i-1],dac_db[i-1],dac_dist[i],dac_db[i], delay+range);//MGetDelay(3)+MGetRange(3) );
            dac_dist[i] = delay+range;//MGetDelay(3)+MGetRange(3);
            test_num = i+1;
            break;
        }
    }
    for(i = 0; i < test_num; i++)
    {
        if(dac_db[i] > 65535 )dac_db[i] = 65535;
        else if(dac_db[i] < 0 )dac_db[i] = 0;
    }
    dist2 = 0;
    dac_line[dist2] = dac_db[0];
    //Write_Number(10,30,22,4,0,0);


    for(i = 1, j=0, offset = C_OFF_DACDB; i< test_num && dist2 < C_SCREEN_WIDTH-1; i++)
    {
        //������ֵ��Ϊ��Ļ�ϵĵ�
        dist1 = dist2 ;
//		dist2 = ( dac_dist[i] - MGetDelay(3) ) * C_COORWIDTH / MGetRange(3);
        dist2 = ( dac_dist[i] - delay ) * C_COORWIDTH / range;

        if( dist2 >= C_SCREEN_WIDTH)
        {
            dac_line[C_SCREEN_WIDTH-1] = GetMiddleValue(dist1,dac_line[0],dist2,dac_db[i], C_SCREEN_WIDTH-1 );
            dist2 = C_SCREEN_WIDTH-1;
        }
        else dac_line[dist2] = dac_db[i];
        j = dist2 - dist1;
        dac_line[j] = dac_line[dist2];
        GetLine_TwoDot(dac_line,0,j);
        //	dist2--;
        //	j--;
        MCopyProtectedMemory((void*)offset, dac_line, (j+1)*C_SIZE_SHORT, PM_COPY_WRITE);
        dac_line[0] = dac_line[j];
        offset += (j)*C_SIZE_SHORT;
    }

    return 2;
}


int DrawDac2(int mode)		//���ͻ����DAC����,mode=0����,=1��
{
    int offset;
    int basegain;
    int i;
    int dist1,dist2;
    int step = 4;
//	uint16_t dac_line[308*2];
//	uint32_t dac_temp;
    int retvalue = -1;
#define C_EXPF 2.302585
    //#define C_EXPF 1.848489938
    //#define C_EXPF 1.319249827
    int test_num;
    double dTemp;
//	int xpos, ypos;
    uint8_t pzText[2] = " ";

    pzText[0] = PHI;

    retvalue = 0;
    if( (MGetSystemMode() == 0 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )|| MGetEchoMode() == C_RF_WAVE)
    {
//Write_Number(10,30,1,4,0,0);
        retvalue = -1;		//�����Զ�����״̬��Ҳδ����DAC\AVG,����Ƶ
    }

    //offset = C_OFF_DAC + 20*C_SIZE_SHORT;
    //MCopyProtectedMemory(&test_num, (void*)offset,1*C_SIZE_SHORT, PM_COPY_READ);


///	if(test_num < 1 || test_num > 10)return -1;

    test_num = Dac.num;
    if( test_num < 1)retvalue = -1;
    if(mode == 0 && retvalue != -1)
    {
//Write_Number(10,30,2,4,0,0);
        if( CalcuDac() < 0)retvalue = -1;
    }
//Write_Number(10,30,3,4,0,0);

    if( retvalue == -1 )
    {
        if( MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DEPTH_COMPENSATE) )	//���ز���״̬<0=δ��>=0�Ѳ⣬
        {
            /*           dist1 = 0;
                       dist2 = C_COORWIDTH-1;
                       dac_line[dist2] = (uint32_t)( 160 / Mexpf(C_EXPF*( MGetDepthGain() )/200.0, C_SIZE_SHORT) + 0.5 );
                       //Write_Number( 100,30,dac_line[dist1],10,0,0);
                       dac_line[dist1] = 160;
                       GetLine_TwoDot(dac_line,dist1,dist2);
                       //MDacGatePeak();
            #if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
                      dac_coe = 160.0/(double)dac_line[0];
                       //Write_Number( 100,40,dac_coe*1000,10,3,0);
                       //	DrawCurver(dac_line,0,C_COORWIDTH);
                       for(i = 0; i < C_COORWIDTH; i++)
                       {
                           if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
                           else dactable[i] = 160;
                       }
                       MActivateDac(1);
            #endif*/

            for(i=0; i<500; i++)
            {

                //       SetAmplifierDynamicGain(dac_line[i]-dac_line[0]) ;
            }
            MActivateDac(1);
        }
        return retvalue;
    }
//Write_Number(10,30,4,4,0,0);

    //if( (MGetSystemMode() == 1 || MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) )
#if C_DEVLIB == 1 || C_DEVLIB == 2 || C_DEVLIB == 23 || C_DEVLIB == 24
    if( MGetSystemMode() == 0 && MGetFunctionMode(C_DEPTH_COMPENSATE))
    {
        uint16_t DynamicGain[C_MAX_DOT_NUMBER];
        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

        for(i=0; i<500; i++)
        {
            DynamicGain[i]=10;//dac_line[i]-dac_line[0] ;
        }
        Write_Number(20,100,1,1,0,0);

        SystemInit(480,500);
        Write_Number(20,100,11,2,0,0);
//        SetAmplifierDynamicGain(DynamicGain) ;
        /* basegain = MGetBaseGain() + 481.31;	//��׼����
         dist2 = 0;
         //dac_line[dist2] = (uint32_t)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
        dac_line[dist2] = (uint32_t)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
         dac_line[dist2] /= 5;
         while(dist2 <= C_COORWIDTH)
         {
             dist1 = dist2;
             dist2 += step;
             //dac_line[dist2] = (uint32_t)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
        	dac_line[dist2] = (uint32_t)( dac_line[dist2] + MGetLineGain(0) - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
             dac_line[dist2] /= 5;
             GetLine_TwoDot(dac_line,dist1,dist2);
         }
         for(i = 0 ; i < C_COORWIDTH; i ++ )
         {
             if(dac_line[i] >= C_COORHEIGHT)dac_line[i] = C_COORHEIGHT - 1;
         }
         //MDacGatePeak();

         dac_coe = 160.0/(double)dac_line[0];
        #if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
         for(i = 0; i < C_COORWIDTH; i++)
         {
             if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
             else dactable[i] = 160;
         }
        #endif
        */
        Write_Number(20,100,2,1,0,0);
        MActivateDac(1);
        Write_Number(20,100,3,1,0,0);
    }
    else
    {
#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
        /*    for(i = 0; i < C_COORWIDTH; i++)
            {
                dactable[i] = 160;
            }*/
        MActivateDac(0);
#endif
    }
//-------dac-------
#endif

//Write_Number(10,30,5,4,0,0);
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

    if( (MGetSystemMode() == 1 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0) )
    {
        basegain = MGetBaseGain() + 481.31;	//��׼����
    }
    else
    {
        basegain = MGetBaseGain() + MGetLineGain(0) + 481.31;	//��׼���棬Ӧ�����з���
    }
    dist2 = 0;
    //dac_line[dist2] = (uint32_t)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
    dac_line[dist2] = (uint32_t)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
    dac_line[dist2] /= 5;
    if(MGetFunctionMode(C_DEPTH_COMPENSATE) == 1)
    {
        dist1 = dist2;
        dist2 = C_COORWIDTH;
        dac_line[dist2] = dac_line[dist1];
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    else
    {
        while(dist2 < C_SCREEN_WIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            //dac_line[dist2] = (uint32_t)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = (uint32_t)( dac_line[dist2] + MGetLineGain(0)  - ( 200 * ( log10(255) - log10(MGetAmpStdMax()) ) ) + 0.5) ;
            dac_line[dist2] /= 5;
            GetLine_TwoDot(dac_line,dist1,dist2);
        }
    }

    MSetColor(C_CR_CURVE);
    DrawCurver(dac_line,0,C_COORWIDTH);
    MSetColor(C_CR_UNDO);
    if( MGetSystemMode() == 1 )return 1;

//Write_Number(10,30,6,4,0,0);

    int line;
    MSetColor(C_CR_CURVE);
    for(line = 1; line < MGetCurveNumber(); line++)
    {
        if( MGetLineGain(line) != MGetLineGain(line-1) )
        {
            dTemp = ( MGetLineGain(line-1) - MGetLineGain(line) )/5  ;
            dac_line[0] = (int)( dac_line[0]-dTemp + 0.5);
            for(i = 4 ; i < C_COORWIDTH+4; i+=4)
            {
                dac_line[i] = (int)( dac_line[i]-dTemp + 0.5);
                dac_line[i-2] = (int)( ( dac_line[i-4] + dac_line[i] )/2.0 + 0.5);
                dac_line[i-3] = (int)( ( dac_line[i-4] + dac_line[i-2] )/2.0 + 0.5);
                dac_line[i-1] = (int)( ( dac_line[i-2] + dac_line[i] )/2.0 + 0.5);
            }
            DrawCurver( dac_line,0,C_COORWIDTH);

        }
    }
    MSetColor(C_CR_UNDO);
    return 1;
}

int DrawDac(int mode)		//���ͻ����DAC����,mode=0����,=1��
{

    int offset;
    int basegain;
    int i;
    int dist1,dist2;
    int step = 1;
//	uint16_t dac_line[308*2];
//	uint32_t dac_temp;
    int retvalue = -1;
    //#define C_EXPF 1.848489938
    //#define C_EXPF 1.319249827
    int test_num;
    double dTemp;
//	int xpos, ypos;
    uint8_t pzText[2] = " ";

    POINT PTin[16],PTout[C_SCREEN_WIDTH];
//;	if( MGetDacShowMode() > 0)return DrawDac2(mode);//������ʾ�ӵ͵��ߵ�DAC����

    pzText[0] = PHI;

    retvalue = 0;
    if( (MGetSystemMode() == 0 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )|| MGetEchoMode() == C_RF_WAVE)
    {
//Write_Number(10,30,1,4,0,0);
        retvalue = -1;		//�����Զ�����״̬��Ҳδ����DAC\AVG,����Ƶ
    }


    test_num = Dac.num;
    if( test_num < 1)retvalue = -1;
    if(mode <= 0 && retvalue != -1)
    {
        if( CalcuDac() < 0)retvalue = -1;
    }



    uint32_t DynamicGain[C_MAX_DOT_NUMBER];
    //if( (MGetSystemMode() == 1 || MGetTestStatus(C_TEST_DAC) == 1 || MGetTestStatus(C_TEST_AVG) == 1) )
#if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
    if( MGetSystemMode() == 0 && MGetFunctionMode(C_DEPTH_COMPENSATE))
    {

        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

        for(i=0; i<500; i++)
        {
            DynamicGain[i]=(dac_line[i]-dac_line[0]+MGetBaseGain()) ;//0;//170;//
            if(DynamicGain[i]>900 ||DynamicGain[i]<0)
            {
                //   Write_Number(10,60,DynamicGain[i],10,0,0);
                //   MAnyKeyReturn();
                DynamicGain[i]=0;

            }
        }
        /*  for(i=100;i<500;i++)
          {
              DynamicGain[i]=40;//dac_line[0]-dac_line[i] ;
          }*/

        // for(i=0;i<240;i++)
        {
            //    Write_Number((i%10)*64,(i/10)*20,DynamicGain[i+200],4,0,0);
        }
//MAnyKeyReturn();
        // SystemInit(480,500);
        //MSetBaseGain( MGetBaseGain(), C_SETMODE_SET );
        SetAmplifierDynamicGain(DynamicGain) ;

        AmplifierDynamicGainOnOff(1) ;
//   AmplifierDynamicGainOnOff(0) ;
        MSetAmplifierGain(MGetBaseGain());
    }
    else
    {
        /*      MActivateDac(0);
              for(i=0;i<500;i++)
              {
                 DynamicGain[i]=MGetBaseGain() ;
              }
              SetAmplifierDynamicGain(DynamicGain) ;
        */
        AmplifierDynamicGainOnOff(0) ;
    }
    if( retvalue == -1 )
    {
        if( MGetEchoMode() != C_RF_WAVE && MGetFunctionMode(C_DEPTH_COMPENSATE) )	//���ز���״̬<0=δ��>=0�Ѳ⣬
        {

            //  for(i=0;i<500;i++)
            {

                //  SetAmplifierDynamicGain(dac_line[i]-dac_line[0], i) ;
            }
            //   MActivateDac(1);

            //Write_Number(10,70,3,4,0,0);

            /*            dist1 = 0;
            dist2 = C_COORWIDTH-1;
            dac_line[dist2] = (uint32_t)( 160 / Mexpf(C_EXPF*( MGetDepthGain() )/200.0, C_SIZE_SHORT) + 0.5 );
            //Write_Number( 100,30,dac_line[dist1],10,0,0);
            dac_line[dist1] = 160;
            GetLine_TwoDot(dac_line,dist1,dist2);
            //MDacGatePeak();
            #if ( C_DEVLIB == 1 || C_DEVLIB	== 2 || C_DEVLIB == 23 || C_DEVLIB == 24)
            dac_coe = 160.0/(double)dac_line[0];
            for(i = 0; i < C_COORWIDTH; i++)
            {

                if( dac_line[i] < 256)dactable[i] = (int)( dac_coe*dac_line[i] + 0.5);
                else dactable[i] = 255;
            }
            MActivateDac(1);
            #endif*/
        }
        return retvalue;
    }
//Write_Number(10,30,4,4,0,0);
//-------dac-------
#endif

//Write_Number(10,30,5,4,0,0);
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);

    /*
    for(i = 0; i < C_COORWIDTH; i++)
    {	//������ֵ��Ϊ��Ļ�ϵĵ�
    dac_line[i] /= 4;
    }
    DrawCurver(dac_line,0,C_COORWIDTH);
    for(i = 0; i < C_COORWIDTH; i++)
    {	//������ֵ��Ϊ��Ļ�ϵĵ�
    dac_line[i] *= 4;
    }
    */
    /*for(i=0;i<C_COORWIDTH;i++)
    {
        Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
    }
        Write_Number(0,0,1,3,0,0);
    MAnyKeyReturn();*/
    if( (MGetSystemMode() == 1 && MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0) )
    {
        basegain = MGetBaseGain() + 481.31;	//��׼����
    }
    else
    {
        basegain = MGetBaseGain() + MGetLineGain(0) + 481.31;	//��׼���棬Ӧ�����з���
    }
    dist2 = 0;
    //dac_line[dist2] = (uint32_t)expf( C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
    dac_line[dist2] = (uint32_t)(Mexpf(C_EXPF*( basegain - dac_line[dist2] )/200.0, C_SIZE_SHORT) *2);
    if(dac_line[dist2] > 32767 )    dac_line[dist2] = 32767;
    if(MGetFunctionMode(C_DEPTH_COMPENSATE) == 1)
    {
        dist1 = dist2;
        dist2 = C_COORWIDTH;
        dac_line[dist2] = dac_line[dist1];
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    else
//		0.011512925465
    {
        int temp[10];
        for(i=0; i<10; i++)
        {
            temp[i]=Dac.db[i];
        }
        while(dist2 < C_SCREEN_WIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            //dac_line[dist2] = (uint32_t)expf(C_EXPF*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = (uint32_t)(Mexpf(C_EXPF*( basegain - dac_line[dist2] )/200.0,C_SIZE_SHORT)*2);// C_SIZE_SHORT) ;
            //dac_line[dist2] = (uint32_t)exp( (0.011512925465*( basegain - dac_line[dist2] ) ) );
            //dac_line[dist2] = (uint32_t)pow(10.0,( basegain - dac_line[dist2] )/200.0) ;

            if(dac_line[dist2] > 32767 )    dac_line[dist2] = 32767;
            GetLine_TwoDot(dac_line,dist1,dist2);

            /*   PTin[0].x = dist1;
               PTin[0].y = dac_line[dist1];
               PTin[1].x = dist2;
               PTin[1].y = dac_line[dist2];

               int nnum = tspLine(PTin,PTout,2,0,0,0,0,0);

                  for(i=dist1;i<nnum;i++)
                   {
                       dac_line[i] = PTout[i].y;
                   }*/
        }
        /*
        for(i=0;i<C_COORWIDTH;i++)
        {
            Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
        }
        */
        for(i=0; i<10; i++)
        {
            Dac.db[i]=temp[i];
        }
//    offset = C_OFF_DACDB;
//    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
        /* PTin[0].x=0;
         PTin[0].y=(uint32_t)Mexpf(C_EXPF*( basegain - dac_line[0] )/200.0,C_SIZE_SHORT);
         PTin[7].x=C_SCREEN_WIDTH-1;
         PTin[7].y=dac_line[C_SCREEN_WIDTH-1];*/
        /*    DrawCurver(dac_line,0,C_COORWIDTH);
           Write_Number(50,100,100,3,0,0);
           MAnyKeyReturn();
           int nstep = C_SCREEN_WIDTH/7;
           for(i=0;i<8;i++)
           {
               PTin[i].x = nstep*i;
               PTin[i].y = dac_line[nstep*i];//dac_line[nstep*i];
           }
           int nnum = tspLine(PTin,PTout,8,0,0,0,0,0);
          for(i=0;i<nnum;i++)
           {
               dac_line[i] = PTout[i].y;
           }
           DrawCurver(dac_line,0,C_COORWIDTH);
           Write_Number(50,100,nnum,3,0,0);
           MAnyKeyReturn();
         for(i=0;i<nnum;i++)
           {
               dac_line[i] = PTout[i].y;
           }
           MCopyProtectedMemory((void*)offset, dac_line, C_LEN_DACDB, PM_COPY_WRITE);*/

    }
    /*for(i=0;i<C_COORWIDTH;i++)
    {
        Write_Number((i%10)*64,(i/10)*24,dac_line[i],3,0,0);
    }
        Write_Number(0,0,2,3,0,0);
    MAnyKeyReturn();*/
    if( !((MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 ) && MGetAvgMode() == 1) )
    {
        if( mode < 0) DrawCurver(daccurve,0,C_COORWIDTH);
        for( i = 0; i < C_SCREEN_WIDTH; i++)    daccurve[i] = dac_line[i];
        if( mode < 2) DrawCurver(daccurve,0,C_COORWIDTH);
        if( MGetSystemMode() == 1 ) return 1;
    }
//Write_Number(10,30,6,4,0,0);

    int line;
    for(line = 1; line < MGetCurveNumber(); line++)
    {
        if( MGetLineGain(line) != MGetLineGain(line-1) )
        {
            dTemp = expf( C_EXPF*( MGetLineGain(line-1) - MGetLineGain(line) )/200.0) ;
            //dac_line[0] = (int)( dac_line[0]/dTemp + 0.5);

            for(i = 0 ; i < C_COORWIDTH; i++)
            {
                dac_line[i] = (int)( dac_line[i]/dTemp + 0.5);
            }
            /*	for(i = 4 ; i < C_COORWIDTH+4; i+=4)
            	{
            		dac_line[i] = (int)( dac_line[i]/dTemp + 0.5);
            		dac_line[i-2] = ( (int)( dac_line[i-4] + dac_line[i]  ) ) >> 1;
            		dac_line[i-3] = ( (int)( dac_line[i-4] + dac_line[i-2] ) ) >>1;
            		dac_line[i-1] = ( (int)( dac_line[i-2] + dac_line[i] ) ) >>1;
            	}*/
            if( !((MGetTestStatus(C_TEST_AVG) == 1 || MGetTestStatus(C_MAKE_DACAVG) == 1 ) && MGetAvgMode() == 1) )
            {
                if( mode < 2)   DrawCurver( dac_line,0,C_COORWIDTH);
            }
        }
    }
    return 1;
}

int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist)
{
    //��������м��ĵ���
    int retvalue ;

    if(dist >= dist1 && dist <= dist2 && dist1 != dist2)
    {
        retvalue = (double)( dist - dist1 ) * ( db2 - db1 )/(double)( dist2 - dist1 ) + db1;
    }
    else retvalue = -1;	//����-1��ʾ�����������

    return retvalue ;
}


int GetLine_TwoDot(uint16_t line[],int dist1,int dist2)
{

//*
    int dist;
    double k;
    if( dist1 > dist2 + 1 )
    {
        dist = dist1;
        dist1 = dist2;
        dist2 = dist;
    }
    else if( dist1 >= dist2 - 1)return -1;
    if( dist2 >= C_COORWIDTH)dist2 = C_COORWIDTH;

    k = (double)( line[dist2] - line[dist1])/(double)(dist2 - dist1);
    for( dist = 1 ; dist1 + dist < dist2; dist++)
    {
        line[dist1 + dist] = (int)( dist*k + line[dist1] );
    }

    /*/
    	int dist = ( (dist1 + dist2) / 2 );

    	if( dist > dist1 )
    	{
    		line[dist] = ( ( line[dist1] + line[dist2] ) / 2 );
    		GetLine_TwoDot(line,dist1,dist);
    		GetLine_TwoDot(line,dist,dist2);
    	}
    //*/
    return 1;
}

void GetCenterPoint(int dist1,int db1,int dist2,int db2)
{
    int dist = ( (dist1 + dist2) >> 1);
    int db = ( (db1 + db2) >> 1);

    if( dist > dist1)
    {
        GetCenterPoint(dist1,db1,dist,db);
    }
}


void GetDacCompensateTable(void)
{
    int offset;
    int basegain;
    //int i,j;
    int dist1,dist2;
    int step = 14;
    uint16_t dac_line[C_SCREEN_WIDTH];
    //int test_num;
    uint16_t first_high;
    basegain = MGetBaseGain() + 481.31;	//��׼����

    if( MGetSystemMode() != 0 && MGetTestStatus(C_TEST_DAC) == 1)
    {
        offset = C_OFF_DACDB;
        MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
        basegain = MGetBaseGain() + 481.31;	//��׼����
        dist2 = 0;
        dac_line[dist2] = (int)expf(2.302585*( basegain - dac_line[dist2] )/200.0) ;
        first_high = dac_line[ dist2 ];
        dac_line[dist2] = 256;	//����256��
        while(dist2 <= C_COORWIDTH)
        {
            dist1 = dist2;
            dist2 += step;
            dac_line[dist2] = (int)expf(2.302585*( basegain - dac_line[dist2] )/200.0) ;
            dac_line[dist2] = ( dac_line[ dist2 ] << 8 )/first_high;	//����256��
            GetLine_TwoDot(dac_line,dist1,dist2);
        }
    }
}


int TestMenuFft(void)
{
    int xpos,ypos;
    int deci_len = 1;
    int number;
    int frequency = 0;
    int keycode;
    int offset;
//	int retvalue = C_FALSE;
    /*	short mode;

    	short gain1 = MGetBaseGain();
    	short gain2 = MGetCompGain();
    	short gain3 = MGetSurfGain();
    	short scale = MGetScaleMode();
    	short fre0 = MGetFrequence(),fre1;
    	short delay0 = MGetDelay(scale);
    	short echomode = MGetEchoMode();
    	short bandmode = MGetBandMode();
    	short range0 = MGetRange(1);
    	short posi,wide,high;
    	short daccomp = MGetFunctionMode(C_DEPTH_COMPENSATE);
    	short probemode = MGetProbeMode();

    	mode = MGetSaveFrequency();

    	posi = MGetGatePara(0,0);
    	wide = MGetGatePara(0,1);
    	high = MGetGatePara(0,2);
    //*/
    //  int savefre;
    int bandmode;   // = MGetBandMode();
    //  savefre = MGetSaveFrequency( ) ;
    AllParaStore();
	MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetTestStatus(C_TEST_ALL,0);	//���ò���״̬,DAC��

    xpos = 0;
    ypos = 0;
    //EraseDrawRectangle(xpos, ypos, 28*8, 229) ;
    MSetColor(C_CR_MENU);
    //CEMenuOut(xpos, ypos, _Menu_DB3 ,8,1,24);	//������̽ͷƵ��
    TextOut(xpos, ypos,1,14+MGetLanguage()*12,8,(uint8_t*)_Menu_DB[MGetLanguage()][1],0);
    number = 0;
    deci_len = 2;
    if ( Input_Number(xpos + (17+MGetLanguage()*12)*C_ECHAR_HDOT,ypos,&number,2,&deci_len,0) )
    {
        MSetFrequence(number,C_SETMODE_SETSAVE);

        //	CEMenuOut(xpos, ypos, _Menu_DB2 ,8,1,24);	//�Կ�����
        MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
        TextOut(xpos, ypos,1,14+MGetLanguage()*12,8,(uint8_t*)_Menu_DB[MGetLanguage()][0],0);

        number = 0;
        if( MGetUnitType() > 0)deci_len = 3;//��λinch
        else deci_len = 1;
        if( Input_Number(xpos + (17+MGetLanguage()*12)*C_ECHAR_HDOT,ypos,&number,2,&deci_len,0) )
        {
            uint8_t *sampbuff;
            uint8_t echobuffer[C_LEN_SAMP];	//�����洢������ֵ
            int i;

            MSetSaveFrequency(1, C_SETMODE_SETSAVE ) ;
            SystemParaStore();
            ChannelParaStore();

            MEraseWindow(0, 0, 26*16,24) ;
            DisplayPrompt(15);
            //	MSetProbeMode(C_P_NORMAL,C_SETMODE_SETSAVE);
            if( DisplayQuery(1) == 1)
            {
                if( TestMenuOffset() == C_FALSE )return C_FALSE;
            }
            DisplayPrompt(15);

            MSetAcquisition(1);
            MSetEchoMode(C_RF_WAVE,C_SETMODE_SETSAVE);
            if( MGetFrequence() < 400 )MSetBandMode(0,C_SETMODE_SETSAVE);
            else if( MGetFrequence() < 800 )MSetBandMode(1,C_SETMODE_SETSAVE);
            else if( MGetFrequence() < 1200 )MSetBandMode(2,C_SETMODE_SETSAVE);
            else MSetBandMode(3,C_SETMODE_SETSAVE);

            //MSetBandMode(bandmode,C_SETMODE_SAVE);
            MSetBGateMode(0,C_SETMODE_SETSAVE);
            MSetGatePara(5,28*9,120,0,C_SETMODE_SETSAVE);
            MChannelRenovate();

            MSetScaleMode(2,C_SETMODE_SETSAVE);
            if( number > 1000)MSetScaleDelay(number - 40,C_SETMODE_SETSAVE);
            else MSetScaleDelay(number * 96/100,C_SETMODE_SETSAVE);
            MSetBaseGain(100,C_SETMODE_SETSAVE);
            MSetCompGain(0,C_SETMODE_SETSAVE);
            MSetSurfGain(0,C_SETMODE_SETSAVE);
            MSetAcquisition(1);

            SystemParaStore();
            ChannelParaStore();

            ExpendTime(50);
            MKeyRlx();

            while(true)
            {
                MEraseWindow(0, 0, 20*16,24) ;
                MSetColor(C_CR_MENU);
                //	CEMenuOut(0,0, _Menu_DB4 ,8,1,24);	//̽ͷƵ�ղ�����
                MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
                TextOut(0, 0,1,21+MGetLanguage()*2,16,(uint8_t*)_Menu_DB[MGetLanguage()][2],4);
                //MAdjustGain(0,0,120,160) ;
                frequency = test_fft();
                MEraseWindow(0, 0, 10*32, 24) ;
                MSetColor(C_CR_MENU);
                //	CEMenuOut(0, 0, _Menu_DB5 ,10,1,24);
                MEraseWindow(xpos, ypos,xpos+C_CCHAR_HDOT*7, ypos+24);
                TextOut(0, 0,1,22+MGetLanguage()*3,32,(uint8_t*)_Menu_DB[MGetLanguage()][3],4);
                Write_Number(5*16+2+124*MGetLanguage(),4,frequency,4,2,0);
                keycode = MAnyKeyReturn();
                if( keycode == C_KEYCOD_RETURN)
                {
                    sampbuff = GetSampleBuffer();
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    for( i = 0; i< C_LEN_SAMP; i++)
                    {
                        if( sampbuff[i] >= 28)echobuffer[i] = sampbuff[i]-28;//-28;
                        else echobuffer[i] =0;
                    }
                    if( DisplayQuery(5) == 1)
                    {
                        MSetFrequence(frequency,C_SETMODE_SETSAVE);
                        MSetSystemMode(0,0);	//���ó�һ��״̬
                        //	MSetBandMode(bandmode,C_SETMODE_SETSAVE);
                        SystemParaStore();
                        ChannelParaStore();

                        MSetSystem();
                        MEraseWindow(0, 0, 20*8, 24) ;
                        offset = C_OFF_PEAK;
                        MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//�Ų���
                        offset = C_OFF_SAMP;
                        MCopyProtectedMemory( (void*)offset, echobuffer, C_LEN_SAMP, PM_COPY_WRITE); //����Ϊ����
                        MSetSaveFrequency(1, C_SETMODE_SETSAVE ) ;
                        DataSave();
                        number = 1;
                        break;
                    }
                    else
                    {
                        number = 0;
                        break;
                    }
                }
            }
        }
    }

    AllParaRestore();
    if( number == 1)
    {
        MSetFrequence(frequency,C_SETMODE_SETSAVE);
        SystemParaStore();
        ChannelParaStore();
    }
    /*
    	SystemParaRestore();
    	ChannelParaRestore();

    	MSetBaseGain(gain1,C_SETMODE_SETSAVE);
    	MSetCompGain(gain2,C_SETMODE_SETSAVE);
    	MSetSurfGain(gain3,C_SETMODE_SETSAVE);
    	MSetSaveFrequency(mode, C_SETMODE_SETSAVE ) ;
    	MSetFrequence(fre0,C_SETMODE_SETSAVE);
    	MSetScaleMode(scale,C_SETMODE_SETSAVE);
    	MSetDelay(delay0,scale);
    	MSetEchoMode(echomode,C_SETMODE_SETSAVE);
    	MSetBandMode(bandmode,C_SETMODE_SETSAVE);
    	MSetRange(range0,C_SETMODE_SETSAVE);
    	MSetGatePara(posi,wide,high,0,C_SETMODE_SAVE);
    	MSetFunctionMode(daccomp,C_DEPTH_COMPENSATE);
    	MSetProbeMode(probemode,C_SETMODE_SETSAVE);
    //*/
    SystemParaRestore();
    ChannelParaRestore();
    MSetSystem();

    return C_FALSE;
}

int TestMenuSensitivity(void)//������У��
{
    int xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 9 - C_ECHAR_HDOT * 10) / 8 * 8;
    int ypos = C_COORVPOSI;
    uint32_t para_xpos,para_ypos;
    uint32_t testrange[2];		//[0]��������ȣ�[1]��׼���߶ȣ�
    uint32_t offset;
    int deci_len, number = 1;

    int retvalue = -1;
    uint32_t row_height = 48;
    int keycode;

    ScreenRenovate();
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
//	CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,row_height);//��ʾ��������У��

    TextOut(0,0,1,11,row_height,(uint8_t*)_TestMenu_A1[MGetLanguage()][5]+2,4);

    EraseDrawRectangle(xpos, ypos, xpos+ 14*C_CCHAR_HDOT, ypos + 3 * row_height) ;
    MSetColor(C_CR_MENU);
    CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_TestMenu_AF1,7,3,row_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

    offset = C_OFF_TEMP + 50;
    MCopyProtectedMemory( testrange, (void*)offset, 8, PM_COPY_READ);
    testrange[1] = MGetAmpStdMax()/2;
    para_xpos = xpos + 7 * C_CCHAR_HDOT;	/*д������λ��ˮƽ��������*/
    para_ypos = ypos + 4;

    while(retvalue == -1)
    {
        /*��� */
        xpos = para_xpos;
        ypos = para_ypos + row_height *0 ;
        Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[0],4,1,0);
        EMenuOut(xpos + 4 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);

        /*�߶�*/
        ypos = para_ypos + row_height *1 ;
        Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,testrange[1],2,0,0);
        EMenuOut(xpos + 2 * C_ECHAR_VDOT,ypos+8,"% ",2,1,8);

        keycode = MenuKeyReturn(2,3);	/*���ֲ˵�ʱ����������keymax�ļ����أ�mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч*/
        ypos = para_ypos + row_height * (keycode - 1);

        switch (keycode)
        {
        case 1:
            deci_len = 1;
            number = 0;
            if ( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                testrange[0] = number;
            break;
        case 2:
            deci_len = 0;
            number = 0;
            if ( Input_Number(xpos,ypos,&number,2, &deci_len,0) == 1)
                testrange[1] = number;
            break;
        case C_KEYCOD_RETURN:
            retvalue = 0;
            break;
        case C_KEYCOD_CONFIRM:
            retvalue = 1;
            offset = C_OFF_TEMP + 20;
            MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
            break;
        }
    }

    if(retvalue == 1)
    {
        while(1)
        {
            if (TestSensitivity(testrange[0],testrange[1], 0 ) == C_FALSE)return C_FALSE;
            if( DisplayQuery(12) == 1) return C_TRUE;
        }
    }
    return C_FALSE;
}
int TestSensitivity(uint32_t depth/*���������*/, uint32_t stdamp/*��׼���߶�*/, int mode/*0�ɵ����棬1���ɵ�*/)
{

    int gain ,range;//, angle, speed;
    //uint32_t echotime;
    uint32_t gateamp;

    uint32_t char_len,row_number,crow_height;
    //int number,deci_len;
    int xpos,ypos;
    uint32_t preElapsedtime;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;
    int diameter = 0;

    uint32_t testrange0 = depth * 10000 / MGetAngle(3) - diameter/2;

    MSetAcquisition(1);

    MKeyRlx();
    MSetScaleMode(0,0);

    if( mode == 0)gain = 600;
    else gain = MGetBaseGain();
    range = testrange0 * 100/80;		//���ڵ�8��

    MSetPara(gain,0,0,0,range,MGetOffset(),0,MGetAngle(0),MGetSpeed() );
	MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,120,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MChannelRenovate();	//��ͨ����Ļ����ˢ��

    char_len = 7;
    row_number = 3;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT-17;
    ypos = C_COORVPOSI + 1;
    MSetColor(C_CR_MENU);
    EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 6, ypos + crow_height * row_number) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos+4, row_number+2*MGetLanguage(), 17,crow_height-12*MGetLanguage(), (uint8_t*)_TestMenu_AB3[MGetLanguage()][2], 4);
    if (MGetLanguage())
    {
//        EMenuOut(xpos + 18*C_ECHAR_HDOT,ypos+4 * (crow_height-12*MGetLanguage()),_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    else
    {
//        EMenuOut(xpos + 9*C_ECHAR_HDOT,ypos+2 * crow_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
    }
    WriteLongness(xpos + 11 * C_ECHAR_HDOT-8*MGetLanguage(), ypos  + 4+29*MGetLanguage() ,depth,4/*�ܳ�*/,0/*�Ƿ�д��λ*/);
    if( MGetUnitType() > 0)EMenuOut(xpos + 11 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 12+20*MGetLanguage(),"in",2,1,8);
    else EMenuOut(xpos + 11 * C_ECHAR_HDOT+40*MGetLanguage(), ypos  + 12+20*MGetLanguage(),"mm",2,1,8);
///	Write_Number(xpos + 11 * C_ECHAR_HDOT,ypos  + 4 ,depth,4,1,0);

    GateMax.Amp = 0;
    xpos = 0;
    ypos = 0;
    ClearCursor(2);

    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

    ///MSetSystemMode(1,0);		//���ó��Զ�����״̬

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    DisplayPrompt(15);
    MSetColor(C_CR_MENU);
    //CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,24);
    TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][5]+2,4);
    GateMax.Amp = 0;	//��߲��ó�ֵΪ0

    while (1)
    {
        if( mode == 0)
        {
            if ( MAdjustGain(0,0,stdamp*2,stdamp*2) )
            {
                GateMax.Amp = 0;	//�����Զ����ڹ�
            }
        }
        MParaRenovate(1);
        gateamp = MGetAmpMax(gatetype);
        if ( MGetGateParaMax( gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
        }
        keycode = MGetKeyCode(0);

        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            MKeyRlx();

            MSetEquivalentDays(2,1);//��������,mode = 0�������ܣ�1���/K/dac/avg,2������
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
//				SystemParaRestore();
//				ChannelParaRestore();

                break;
            }
        }
        else if(mode == 0 && keycode == C_KEYCOD_BASEGAIN && GetSameKeyTime() > 20)MAdjustGain(0,0,stdamp*2,stdamp*2);
        else if( keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_RANGE )
        {
            KeyManage(keycode,1);
            MKeyRlx();
            DisplayPrompt(15);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0,_TestMenu_A1+6*5+1,5,1,24);
            TextOut(0,0,1,11,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][5]+2,4);
        }
    }
    MKeyRlx();
    return retvalue;

}
int ManualMonthTest(void)//�˹�������У��
{

    uint32_t gain,range,offset,delay,speed;
    int retvalue = C_FALSE;
    uint32_t echotime;
//	int i,j;
    int keycode;
    uint32_t gateamp;

    uint32_t xpos,ypos;//,menu_xpos,menu_ypos;
    uint32_t char_len,row_number,crow_height;//,row_height;
    uint32_t gatetype = 0;

    uint16_t _Prompt_TB1[] =
    {
        CH(5039),CH(3887),CH(4852),CH(3660),CH(4254),CH(2215),CH(1866),CH(4252)
    };//	���������ֶ�����
    MSetProbeMode(0,C_SETMODE_SETSAVE);

    gain = 300;
    range = 1250;
    offset = 0;
    delay = 0;
    speed = 5960;

    MSetPara(gain,0,0,2,range,offset,delay,0,speed);

    MSetEchoMode(0,C_SETMODE_SAVE);
    MSetPulseMode(1,C_SETMODE_SAVE);
    MSetBandMode(0,C_SETMODE_SAVE);
    MSetGatePara(C_COORHORIUNIT * 3.8,C_COORHORIUNIT * 1.2,120,0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 1.5,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MSetAcquisition(1);	/*���ò������0������1����*/

    MChannelRenovate();	//��ͨ����Ļ����ˢ��

    char_len = 11;
    row_number = 2;
    crow_height = 48-14*MGetLanguage();

    xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*3)*C_CCHAR_HDOT;
    ypos = C_COORVPOSI+1;
//	EraseDrawRectangle(xpos, ypos, 280, ypos+crow_height * row_number) ;
    MSetColor(C_CR_MENU);
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,char_len,row_number,crow_height);

    /* if (MGetLanguage())
     {
         EraseDrawRectangle(C_COORWIDTH-(14+1)*C_CCHAR_HDOT, ypos, C_COORWIDTH, ypos+15 * 3) ;
         EMenuOut(C_COORWIDTH-(14+1)*C_CCHAR_HDOT + C_ECHAR_HDOT,ypos + 4, _Menu_Power1E ,28,3,15);
     }
     else
     {
         EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+24 * 2) ;
         CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,11,2,24);
     }*/
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+32 * (2+MGetLanguage())+MGetLanguage()*8) ;
    TextOut(xpos + C_ECHAR_HDOT,ypos + 8,2+MGetLanguage()*2,22,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][0],4);

    CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);

    GateMax.Amp = 0;	//
    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    while (1)
    {
        if (MAdjustGain(0,2,120,120) )
        {
            GateMax.Amp = 0;	//�����Զ����ڹ�
        }
        MParaRenovate(1);
        keycode = MGetKeyCode(10);
        gateamp = MGetAmpMax(gatetype);
        if ( MGetGateParaMax(gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
        }
        if(keycode == C_KEYCOD_CONFIRM)
        {
            ClearCursor(2);
            echotime = MGetAmpTime(gatetype);

            MGateDraw();
            MSetGatePara(C_COORHORIUNIT * 7.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);//�����Ƶ���8��
            MGateDraw();
            MKeyRlx();
            MAdjustGain(0,0,80,160);
            ExpendTime(10);
            offset = 2 * echotime - MGetAmpTime(gatetype);
            MSetOffset(offset,C_SETMODE_SETSAVE);
            speed = 500*C_SAMPLE_FREQ/(echotime - offset);
            MSetSpeed(speed,C_SETMODE_SETSAVE);

            range = 2500;
            MSetRange(range,C_SETMODE_SAVE);
            MSetSystem();
            MChannelRenovate();	//��ͨ����Ļ����ˢ��
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                MSetAcquisition(0);
                return retvalue;
            }
        }
    }
    keycode = C_KEYCODMAX;
    MSetColor(C_CR_MENU);
    CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);

    while(true)
    {
        if(keycode == C_KEYCOD_GATE || keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN )
        {
            keycode = KeyManage(keycode,1);
        }
        else if( keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                MSetAcquisition(0);
                break;
            }
        }
        else
        {
            keycode = MGetKeyCode(0);
            MSetColor(C_CR_MENU);
            CEMenuOut(0,0, _Prompt_TB1 ,8,1,16);
        }
        MParaRenovate(1);
    }
    return 1;
}

extern const uint32_t c_crPara[];
extern const uint32_t Echo_crPara[];

extern uint8_t crPara[];
int MenuMonthTest(void)//������У��
{
    POWER power;
    uint32_t table[] = {1000, 794, 631, 501, 398, 316, 251, 200, 158, 125, 100, 79, 63, 50, 39};

    int gain_temp;
    int number2 = 0;

    uint8_t tempcrPara[C_CR_MAX+12];
    uint32_t gain,range,offset,delay,speed;
    int retvalue = C_FALSE;
    uint32_t echotime;
    int number;//,deci_len;
    int i,j;
    int keycode;
    uint32_t gateamp;
    uint8_t *sampbuff;

    uint32_t xpos,ypos;//,menu_xpos,menu_ypos;
    uint32_t char_len,row_number,crow_height,row_height;
    uint32_t gatetype = 0;
    const char _Prompt_B[C_LANGUAGE][6][50] =
    {
        {
            "ˮƽ���Բ��ϸ��ز⣡  ",
            "��ֱ���Բ��ϸ��ز⣡  ",
            "�ֱ������ϸ��ز⣡    ",
            "��̬��Χ���ϸ��ز⣡  ",
            "�������������ϸ��ز⣡",
            "�鿴ǰ�β��Խ�����٣���",
        },
        {
            "Horizontal linearity     disqualification,redo!   ",
            "Vertical linearity       disqualification,redo!   ",
            "Resolution               disqualification,redo!   ",
            "Dynamic range            disqualification,redo!   ",
            "Surplus sensitivity      disqualification,redo!   ",
            "Detector performance     before? Y/N              ",
        }
    };

    AllParaStore();
    MSetUnitType(0);
	MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetTestStatus(C_TEST_ALL,0);	//���ò���״̬,DAC��
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetBandMode(0,C_SETMODE_SAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetReject(0,C_SETMODE_SETSAVE);
    MSetGatePara(MGetGatePara(1,0),MGetGatePara(1,1),0,1,C_SETMODE_SETSAVE);
    MSetDelay(0,C_SETMODE_SETSAVE);		//���þ���50.0mm����ʱ
    MSetStdMode(0,C_SETMODE_SAVE);
    offset = C_OFF_POWER;
    MCopyProtectedMemory(&power, (void*)offset, sizeof(POWER), PM_COPY_READ);
    MSetProbeMode(0,C_SETMODE_SETSAVE);
//	int elapsedtime = GetElapsedTime() + 1000;
    short begin = 0;
    retvalue = true;
    while (retvalue)
    {
        /* set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2 */
        if( begin == 0)
        {
            if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][5],24,MB_YESNO) == MB_NO )retvalue = true;
            else retvalue = false;
            begin = 1;
        }
        //ˮƽ����
        MSetPulseMode(0,C_SETMODE_SAVE);
        while(retvalue)
        {
            gain = 300;
            range = 1250;
            offset = 0;
            delay = 0;
            speed = 5920;


            MSetPara(gain,0,0,2,range,offset,delay,0,speed);

            MSetGatePara(C_COORHORIUNIT * 3.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);

            MSetSystem();
            MSetAcquisition(1);	/*���ò������0������1����*/

            MChannelRenovate();	//��ͨ����Ļ����ˢ��

            char_len = 11;
            row_number = 2;
            crow_height = 48-14*MGetLanguage();

            xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*4)*C_CCHAR_HDOT;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,char_len,row_number,crow_height);
            /*   if (MGetLanguage())
               {
                   EraseDrawRectangle(C_COORWIDTH-(14+1)*C_CCHAR_HDOT, ypos, C_COORWIDTH, ypos+15 * 3) ;
                   EMenuOut(C_COORWIDTH-(14+1)*C_CCHAR_HDOT + C_ECHAR_HDOT,ypos + 4, _Menu_Power1E ,28,3,15);
               }
               else
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos+24 * 2) ;
                   CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power1 ,11,2,24);
               }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (2+MGetLanguage())+10+MGetLanguage()*8) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,2+MGetLanguage()*2,22,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][0],4);

            MEraseWindow(0, 0, 24*C_ECHAR_HDOT,17 ) ;
            //	CMenuOut(0,0, _Power_prompt + 1 * 6, 6, 1, 24);
            TextOut(0,0,1,20,24,(char *)_Power_prompt[MGetLanguage()][0],4);

            GateMax.Amp = 0;	//
            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            ExpendTime(50);
            while (1)
            {
                if(MAdjustGain(0,1,120,120))
                {
                    MSetGateParaInit();	//������߲����ֵ
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }

                if(keycode == C_KEYCOD_CONFIRM)
                {
                    MSetDelay((255 * C_SAMPLE_FREQ + MGetSpeed()/2 )/(int)MGetSpeed(),C_SETMODE_SETSAVE);
                    ClearCursor(2);
                    echotime = MGetAmpTime(gatetype);

                    MGateDraw();
                    MSetGatePara(C_COORHORIUNIT * 7.8,C_COORHORIUNIT * 1.5,120,0,C_SETMODE_SETSAVE);//�����Ƶ���8��
                    MGateDraw();
                    MKeyRlx();
                    MAdjustGain(0,0,160,160);
                    ExpendTime(50);
                    offset = 2 * echotime - MGetAmpTime(gatetype);
                    MSetOffset(offset,C_SETMODE_SETSAVE);
                    speed = 500*C_SAMPLE_FREQ/(echotime - offset);
                    MSetSpeed(speed,C_SETMODE_SETSAVE);

                    range = 1250;
                    MSetRange(range,C_SETMODE_SAVE);
                    MSetSystem();
                    MChannelRenovate();	//��ͨ����Ļ����ˢ��
                    MSetColor(C_CR_MENU);
                    //	CMenuOut(0,0, _Power_prompt + 1 * 6, 6, 1, 24);
                    MEraseWindow(0, 0,C_CCHAR_HDOT*10,C_CCHAR_VDOT ) ;
                    TextOut(0,0,1,20,24,(char *)_Power_prompt[MGetLanguage()][0],4);

                    power.Horiline = 0;
                    MParaRenovate(1);


                    for(i = 0; i < 4; i++)
                    {

                        MGateDraw();
                        MSetGatePara(C_COORHORIUNIT * (2*i+1.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);
                        MGateDraw();
                        //if( i == 4)MSetDelay(50,C_SETMODE_SETSAVE);		//���һ�β�������ʱ,������ȿ���
                        for(j = 0 ; j < 10; j++)
                        {
                            ExpendTime(10);
                            MAdjustGain(0,0,100,120);
                            AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                            AdjustTime.time = AdjustTime.time2 = 0;
                            gateamp = MGetAmpMax(0) ;
                            if( gateamp < 105 && gateamp > 95)break;
                        }
                        if( i == 0 )power.gain[0] = MGetBaseGain();
                        MParaRenovate(1);

                        number = MGetAmpDist1(gatetype);
//Write_Number(10,30,number,4,0,0);
                        number = Mabsi(number - (i+1)*250);
                        if( number > 200)number -= 250;
//Write_Number(100,30,number,4,0,0);
                        //��ǰ����25���������Ĳ�

                        if( number > 4)number -= 4;
                        if(number > power.Horiline)	power.Horiline = number;
                        ExpendTime(50);

                    }
                    break;

                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            MKeyRlx();
            if( power.Horiline > c_SystemPowerStd[0] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][0],24,MB_OKCANCEL) == MB_CANCEL )
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }
        //�ֱ���
        //range = 850;
        //delay = range - 75*3;//���ز�����7.5������
        //MSetScaleDelay(delay,C_SETMODE_SETSAVE);		//���þ���50.0mm����ʱ

        //range = 100 * 3;
        MSetDelay(0,C_SETMODE_SETSAVE);		//���þ���0.0mm����ʱ
        MSetPulseMode(2,C_SETMODE_SAVE);
        while(retvalue)
        {
            range = 2500;
            MSetRange(range,C_SETMODE_SAVE);

            gain = 600;
            MSetBaseGain(gain,C_SETMODE_SAVE);
            MSetGatePara(C_COORHORIUNIT * 3,C_COORHORIUNIT*5/6,100,0,C_SETMODE_SETSAVE);
            MSetSystem();
            MChannelRenovate();	//��ͨ����Ļ����ˢ��
            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();

            xpos = C_COORWIDTH-(char_len+1+MGetLanguage()*3)*C_CCHAR_HDOT;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /*   if (MGetLanguage())
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH-6*8, ypos+12 * 3) ;
                   EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3E ,26,3,12);
               }
               else
               {
                   EraseDrawRectangle(xpos, ypos, C_COORWIDTH-9*8, ypos+24 * 3) ;
                   CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3 ,11,3,24);
               }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * 3+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3,22,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][2],4);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power3,char_len,row_number,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
            //	CMenuOut(0,0, _Power_prompt + 3 * 6, 6, 1, 24);

            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][2],4);


            GateMax.Amp = 0;	//�����Զ����ڹ�
            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            //*
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//������߲����ֵ
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();

                    MAdjustGain(0,0,200,200);
                    power.ResolvingPower = MGetSysGain();
                    power.gain[2] = MGetBaseGain();

                    sampbuff = GetSampleBuffer() ;
//					uint8_t temp;

                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    sampbuff = GetSampleBuffer() ;

                    for(i = C_COORWIDTH*85/100, gateamp = 200; i <= C_COORWIDTH*91/100 +1; i++)
                    {
                        if( ( *(sampbuff+i) ) < gateamp )
                        {
                            //����Ͳ�
                            gateamp = ( *(sampbuff+i) );
                        }
                    }
                    // Write_Number(10,100,gateamp,4,0,0);
                    // MAnyKeyReturn();
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
//Write_Number(10,30,gateamp,4,0,0);
//ExpendTime(200);
                    if( gateamp <= 1)power.ResolvingPower = 400;
                    else power.ResolvingPower = 200*log10(100.0/gateamp)+40;

                    // Write_Number(20,10,power.ResolvingPower,4,1,0);
                    // MAnyKeyReturn();

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }

#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            /*/
            while (1)
            {
            	if (MAdjustGain(0,0,100,100) )
            	{
            		GateMax.Amp = 0;	//�����Զ����ڹ�
            	}
            	MParaRenovate(1);
            	keycode = MGetKeyCode(2);
            	gateamp = MGetAmpMax(gatetype);
            	if ( MGetGateParaMax(gatetype) == C_TRUE )
            	{
            		ClearCursor(2);
            		xpos = GateMax.Pos;
            		ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp;
            		if(GateMax.Amp < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
            	}
            	if(keycode == C_KEYCOD_CONFIRM)
            	{
            		ClearCursor(2);
            		MKeyRlx();

            		MAdjustGain(0,0,40,40);
            		power.ResolvingPower = MGetSysGain();
            		power.gain[2] = MGetBaseGain();

            		sampbuff = GetSampleBuffer() ;
            		uint8_t temp;

            		while(true)
            		{
            			BaseGainPlus(1);
            			WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
            			ExpendTime(5);
            			sampbuff = GetSampleBuffer() ;

            			for(i = 280*85/100, gateamp = 200; i <= 280*91/100 ; i++)
            			{
            				if( ( *(sampbuff+i) ) < gateamp )
            				{//����Ͳ�
            					gateamp = ( *(sampbuff+i) );
            				}
            			}
            			if( gateamp >= 100 || MGetSysGain() >= C_MAX_SYSGAIN)break;
            		}
            		power.ResolvingPower = MGetSysGain() -	power.ResolvingPower;

            		//Write_Number(20,10,power.ResolvingPower,4,1,0);
            		//MAnyKeyReturn();

            		break;
            	}
            	else if(keycode == C_KEYCOD_RETURN)
            	{
            		MSetAcquisition(0);
            		return retvalue;
            	}
            }
            //*/
            MKeyRlx();
            if(retvalue == false)break;

            if( power.ResolvingPower < c_SystemPowerStd[2] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][2],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }


        //��ֱ���ԡ���̬��Χ
        while(retvalue)
        {
            range = 2500;	//��8��
            MSetRange(range,C_SETMODE_SETSAVE);
            MSetGatePara((int)(C_COORHORIUNIT * 7.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);

            MChannelRenovate();	//��ͨ����Ļ����ˢ��
            power.vertline = 0;

            gain = 400;
            MSetBaseGain(gain,C_SETMODE_SETSAVE);
            MSetSystem();
            MChannelRenovate();	//��ͨ����Ļ����ˢ��

            GateMax.Amp = 0;	//�����Զ����ڹ�
            // set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2
            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();
            xpos = C_COORWIDTH-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-10;
            ypos = C_COORVPOSI+1;
            //		EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /*  if (MGetLanguage())
              {
                  EraseDrawRectangle(xpos, ypos, C_COORWIDTH-7*8, ypos+12 * 3) ;
                  EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4E ,26,3,12);
              }
              else
              {
                  EraseDrawRectangle(xpos, ypos, C_COORWIDTH-10*8, ypos+24 * 3) ;
                  CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,11,3,24);
              }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (3+MGetLanguage())+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3+MGetLanguage(),21,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][3],4);

//			CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,char_len,row_number,crow_height);
            //	CMenuOut(0, 0, _Power_prompt + 2 * 6, 6, 1, 24);
            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][1],4);
            MKeyRlx();

            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//������߲����ֵ
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    MSetBaseGain(MGetBaseGain()+ 60,C_SETMODE_SETSAVE);

                    while(MGetAmpMax(0) < 200)
                    {
                        MSetBaseGain( MGetBaseGain() + 1 , C_SETMODE_SETSAVE);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(20);
                    }
                    while(MGetAmpMax(0) > 202)
                    {
                        MSetBaseGain( MGetBaseGain() - 1 , C_SETMODE_SETSAVE);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(20);
                    }

                    gain_temp = MGetBaseGain();
                    power.gain[4] = power.gain[1] = MGetBaseGain();

                    i = 0;
                    number2 = 0;
                    power.vertline = 0;
                    MParaRenovate(1);
                    gateamp = 0 ;
                    for( i = 0; i < 20 && MGetSysGain() > 0; i++)
                    {
                        for( number = 0 ; number < 20; number++)
                        {
                            ExpendTime(10);
                            MParaRenovate(0);
                            if( Mabsi(gateamp - MGetAmpMax(0) ) < 2 )
                            {
                                break;
                            }
                            gateamp = MGetAmpMax(0);
                        }
                        if( gateamp > 0)power.Dynamic = MGetBaseGain();
                        number = gateamp*5 - table[i];
                        if(i < 13)
                        {
                            //���㴹ֱ����
                            if (number >= 0)
                            {
                                if(number > power.vertline) power.vertline = number;
                            }
                            else
                            {
                                if( number < number2) number2 = number;
                            }
                            MSetBaseGain( MGetBaseGain() - 20 , C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            if( gateamp <= 1)break;
                            MSetBaseGain( MGetBaseGain() - 5 , C_SETMODE_SETSAVE);
                        }

//Write_Number(200,30,number,4,0,-1);
//Write_Number(200,40,power.vertline,4,0,-1);
//Write_Number(200,50,number2,4,0,-1);
                        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                        AdjustTime.amp_total = AdjustTime.amp_total2 = 0;
                        AdjustTime.time = AdjustTime.time2 = 0;
                        ExpendTime(50);
                    }

                    power.vertline -= number2;//��ȥ��ֵ
                    MSetColor(C_CR_MENU);
                    //	CMenuOut(0,0, _Power_prompt + 4 * 6, 6, 1, 24);
                    MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
                    TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][3],4);

                    power.Dynamic = gain_temp - power.Dynamic + 10;
                    MSetBaseGain( gain_temp - 60, C_SETMODE_SETSAVE);
                    //Write_Number(20,10,power.Dynamic,4,1,0);
                    //MAnyKeyReturn();
                    //MKeyRlx();

                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1

                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            MKeyRlx();
            if(retvalue == false)break;
            if( power.vertline > c_SystemPowerStd[1] )
            {
                if( CMessageBox( 0, 0, (char *)_Prompt_B[MGetLanguage()][1],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else if( power.Dynamic < c_SystemPowerStd[3] )
            {
                if( CMessageBox( 0, 0,(char *) _Prompt_B[MGetLanguage()][3],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }


        while(retvalue)
        {
            //����������
            range = 2000 *100/80;	//��8��
            MSetRange(range,C_SETMODE_SETSAVE);
            MSetGatePara((int)(C_COORHORIUNIT * 7.5),C_COORHORIUNIT * 1,100,0,C_SETMODE_SETSAVE);

            MChannelRenovate();	//��ͨ����Ļ����ˢ��


            MKeyRlx();

            MSetSystem();
            MChannelRenovate();	//��ͨ����Ļ����ˢ��

            GateMax.Amp = 0;	//�����Զ����ڹ�

            char_len = 11;
            row_number = 3;
            crow_height = 48-14*MGetLanguage();
            xpos = C_COORWIDTH-(char_len+2+MGetLanguage()*2)*C_CCHAR_HDOT-10;
            ypos = C_COORVPOSI+1;
            //	EraseDrawRectangle(xpos, ypos, C_COORHORIUNIT * 7, ypos+crow_height * row_number) ;
            MSetColor(C_CR_MENU);
            /* if (MGetLanguage())
             {
                 EraseDrawRectangle(xpos, ypos, C_COORWIDTH-7*8, ypos+12 * 3) ;
                 EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4E ,26,3,12);
             }
             else
             {
                 EraseDrawRectangle(xpos, ypos, C_COORWIDTH-9*8, ypos+24 * 3) ;
                 CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,11,3,24);
             }*/

            EraseDrawRectangle(xpos, ypos, C_COORWIDTH+18, ypos+32 * (3+MGetLanguage())+10) ;
            TextOut(xpos + C_ECHAR_HDOT,ypos + 8,3+MGetLanguage(),21,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][3],4);
            //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power4 ,char_len,row_number,crow_height);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0, _Power_prompt + 5 * 6, 6, 1, 24);
            MEraseWindow(0, 0, 23*C_ECHAR_HDOT,17 ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][4],4);


            MKeyRlx();

            xpos = 0;
            ypos = 0;
            ClearCursor(2);
            while (retvalue)
            {
                if(MAdjustGain(0,0,100,100))
                {
                    MSetGateParaInit();	//������߲����ֵ
                }
                keycode = MGetKeyCode(0);
                MParaRenovate(1);
                gateamp = MGetAmpMax(gatetype);
                if(MGetGateParaMax(gatetype) == C_TRUE)
                {
                    ClearCursor(2);
                    xpos = GateMax.Pos;
                    ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                    if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
                }
                if(keycode == C_KEYCOD_CONFIRM)
                {
                    ClearCursor(2);
                    MKeyRlx();
                    MAdjustGain(0,0,100,100);
                    power.Sensitivity = MGetBaseGain();
                    power.gain[3] = MGetBaseGain();
                    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
                    break;
                }
                else if(keycode == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }
#if C_ACQU == 1
                if (GetElapsedTime() >= elapsedtime)
                {
                    elapsedtime = GetElapsedTime() + 1000;
                    MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
                }
#endif
            }
            xpos = 0;
            ypos = C_COORVPOSI;

            range = 40000 *100/80;	//��8��
            MSetRange(range,C_SETMODE_SETSAVE);
            MChannelRenovate();
            //	EraseDrawRectangle(xpos + 2, ypos, xpos+20*C_ECHAR_HDOT, ypos+24) ;
            MSetColor(C_CR_MENU);
            /*  if (MGetLanguage())
              {
                  EraseDrawRectangle(xpos + 2, ypos, xpos+34*C_ECHAR_HDOT, ypos+18) ;
                  EMenuOut(xpos + C_ECHAR_HDOT,ypos + 4, _Menu_Power5E ,33,1,12);
              }
              else
              {
                  EraseDrawRectangle(xpos + 2, ypos, xpos+20*C_ECHAR_HDOT, ypos+24) ;
                  CEMenuOut(xpos + 2 + C_ECHAR_HDOT,ypos + 4, _Menu_Power5 ,9,1,crow_height);
              }*/

            EraseDrawRectangle(xpos+2, ypos, xpos+(20+MGetLanguage()*11)*C_ECHAR_HDOT, ypos+32+10) ;
            TextOut(xpos+2 + C_ECHAR_HDOT,ypos + 8,1,18+MGetLanguage()*11,32-MGetLanguage()*6,(uint8_t*)_Menu_Power[MGetLanguage()][4],4);
//			CEMenuOut(xpos + 2 + C_ECHAR_HDOT,ypos + 4, _Menu_Power5 ,9,1,crow_height);
            MSetColor(C_CR_MENU);
            //	CMenuOut(0,0, _Power_prompt + 5 * 6, 6, 1, 24);
            MEraseWindow(0, 0,C_CCHAR_HDOT*8,C_CCHAR_VDOT ) ;
            TextOut(0,0,1,20,16,(char *)_Power_prompt[MGetLanguage()][4],4);

            while(1)
            {
                if(MGetKeyCode(2) == C_KEYCOD_CONFIRM)
                {
                    int i = 0;
                    do
                    {
                        BaseGainPlus(2);
                        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                        ExpendTime(5);
                        if( MGetAmpMax(gatetype) > 20 )i++;
                        else i = 0;
                    }
                    while( MGetBaseGain() < C_MAX_SYSGAIN && i < 3 );

                    power.Sensitivity = MGetBaseGain() - power.Sensitivity;

                    break;
                }
                else if(MGetKeyCode(2) == C_KEYCOD_RETURN)
                {
                    if( DisplayQuery(4) == 1)
                    {
                        retvalue = false;
                        break;
                    }
                }

            }
            MKeyRlx();
            if(retvalue == false)break;
            if( power.Sensitivity < c_SystemPowerStd[4] )
            {
                if( CMessageBox( 0, 0, (char *)_Prompt_B[MGetLanguage()][4],24,MB_OKCANCEL) == MB_CANCEL)
                {
                    retvalue = true;
                    break;
                }
            }
            else break;
        }
        for (i=0; i<C_CR_MAX+12; i++)
        {
            tempcrPara[i]=crPara[i];
        }
        while(true)
        {

            MSaveDefualtColor(1);
            MSetAcquisition(0);
            power.test = 1;

            offset =  C_OFF_POWER;
            MCopyProtectedMemory((void*)offset, &power, sizeof(POWER), PM_COPY_WRITE);

            //������ʾ���Խ��
            MFclearScreen();

            xpos = 40;
            ypos = 1;
            char_len = 6;
            row_number = 6;
            row_height = 48;

            MSetColor(C_CR_MENU);
            EraseDrawRectangle(xpos, ypos, xpos+10*C_CCHAR_HDOT + C_ECHAR_VDOT+9*C_ECHAR_VDOT*MGetLanguage(), ypos+row_height*6) ;

//			EraseDrawRectangle(xpos, ypos, xpos+9*C_CCHAR_HDOT + C_ECHAR_VDOT, ypos+row_height*6) ;

            //	CMenuOut(xpos+4, ypos+4, _Power_prompt, char_len, row_number, row_height);

            TextOut(xpos+4,ypos+4,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][5],4);
            TextOut(xpos+4,ypos+4+row_height,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][0],4);
            TextOut(xpos+4,ypos+4+row_height*2,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][1],4);
            TextOut(xpos+4,ypos+4+row_height*3,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][2],4);
            TextOut(xpos+4,ypos+4+row_height*4,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][3],4);
            TextOut(xpos+4,ypos+4+row_height*5,1,14+MGetLanguage()*6,row_height,(char *)_Power_prompt[MGetLanguage()][4],4);

            xpos = 40;
            ypos = ypos+row_height*6+12;
            char_len = 3;
            row_number = 2;
            DrawRectangle(xpos, ypos, xpos+(11+MGetLanguage())*C_CCHAR_HDOT+MGetLanguage()*100, ypos+row_height*3);
            MSetColor(C_CR_MENU);

            //  if(MGetLanguage()==1)	//English
            {
                //CMenuOut(xpos+10, ypos+4, _Power_prompt3, 1, row_number, row_height);
                //       EMenuOut(xpos+10+16, ypos+12,"1. Print",8,1,8);
                //       EMenuOut(xpos+10+16, ypos+12+row_height,"# Redo ",8,1,8);
                //       EMenuOut(xpos+10+16, ypos+12+row_height*2,"�� Exit ",8,1,8);
            }
            //   else
            {
                TextOut(xpos+10,ypos+4,row_number,8,row_height,(char *)_Power_prompt[MGetLanguage()][6],4);
				TextOut(xpos+10,ypos+row_height*2+4,1,18+MGetLanguage()*5,row_height,(char *)_Power_prompt[MGetLanguage()][7],4);
//                EMenuOut(xpos+10,ypos+4+2*row_height,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

                // CMenuOut(xpos+10, ypos+4, _Power_prompt2, char_len, row_number, row_height);
            }

            xpos = 180+14*C_ECHAR_HDOT*MGetLanguage();
            ypos = 8;
            ypos += row_height;
            Write_Number(xpos,ypos,power.Horiline,4,1,0);
            Write_Ascii(xpos+2*C_CCHAR_HDOT, ypos,'%');

            ypos += row_height;
            Write_Number(xpos,ypos,power.vertline,4,1,0);
            Write_Ascii(xpos+2*C_CCHAR_HDOT, ypos,'%');

            ypos += row_height;
            Write_Number(xpos,ypos,power.ResolvingPower,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            ypos += row_height;
            Write_Number(xpos,ypos,power.Dynamic,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            ypos += row_height;
            Write_Number(xpos,ypos,power.Sensitivity,4,1,0);
            EMenuOut(xpos + 2*C_CCHAR_HDOT+16,ypos,"dB",2,1,8);

            do
            {
                keycode = MAnyKeyReturn();
            }
            while( keycode != 1 && keycode != C_KEYCOD_SAVE&& keycode != C_KEYCOD_CONFIRM && keycode!= C_KEYCOD_RETURN);

            if (keycode == 1)
            {
                //	retvalue = MGetColorMode();
                if( TestPrinterReady() != 0)
                {
                    //	MSaveDefualtColor(0);
                    PrinterInitialize();		//��ӡ����ʼ��
                    xpos = 0;
                    ypos = 0;

                    for(i = 0; i< 156/12 && ypos <= 156; i++)
                    {
                        ScrcpyInit(40);
                        ScrcpyInvert(xpos,ypos,40);
                        ypos += 12;
                    }
                    PrintEnd();
                    //	MSaveDefualtColor(retvalue);
                }
            }
            else if (keycode == C_KEYCOD_RETURN)
            {
                offset = C_OFF_TEMP+60;	//��ɫ�洢λ��
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                MFclearScreen();
                MKeyRlx();
                retvalue = false;
                break;
            }
            else if (keycode == C_KEYCOD_CONFIRM)
            {
                offset = C_OFF_TEMP+60;	//��ɫ�洢λ��
                MCopyProtectedMemory( (void*)offset, tempcrPara, C_CR_MAX+12, PM_COPY_WRITE);
                MCopyProtectedMemory( crPara, (void*)offset, C_CR_MAX+12, PM_COPY_READ);
                MSetColor(C_CR_DEFAULT);
                SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
                MSetEquivalentDays(0,1);//��������,mode = 0�������ܣ�1���/K/dac/avg,2������
                retvalue = true;
                break;
            }
			else if (keycode == C_KEYCOD_SAVE )
            {
				// MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                // MSetSavePosition(1,C_SETMODE_SETSAVE);
				WaveSave1(1);
                retvalue = true;
                break;
            }
        }
        if( retvalue )break;
        retvalue = true;
    }
    AllParaRestore();
    return retvalue;
}



int WeldSetup(void)
{
    uint32_t char_len = 6,row_number = 9,crow_height = 44;
    int number,deci_len;
    int xpos,ypos;
    //int x1,y1,x2,y2,r;
    //double rate;//ʱ�䳤�ȵ�λmm����Ļ�����ı���
    int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 4;
    //int weld_xpos,weld_ypos;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2, page = 0;
    int row;
    //unsigned int offset;
    uint16_t* cpText = 0;
    //uint8_t* epText = 0;
    int i;
    //uint32_t elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;



    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);


    while(1)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);

        if( page < 1)page = 1;
        else if(page > page_max )page = page_max;

        TextOut(C_ECHAR_HDOT,4,1,25,16,(uint8_t*)_Weld_A1[MGetLanguage()][0],4);
        //	CEMenuOut(C_ECHAR_HDOT,4, _Weld_A1,12,1,crow_height);
        Write_Number(C_ECHAR_HDOT + (10+MGetLanguage()*2) * C_ECHAR_HDOT, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 25 * C_ECHAR_HDOT+4, 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 17 * C_ECHAR_HDOT, 4,page_max,1,0,0);
        }


        menu_xpos = 0;
        menu_ypos = crow_height-4;
        para_xpos = menu_xpos+6*C_CCHAR_HDOT;
        para_ypos = menu_ypos + 4;
        //	CEMenuOut(menu_xpos+C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�

        if(page == 1)
        {
            row_number = 9;
            if( stWeldPara.type == 3)//T��
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(uint8_t*)_Weld_B1[MGetLanguage()][i],4);
                }
            }
            else
            {
                for (i=0; i<row_number; i++)
                {

                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(uint8_t*)_Weld_B2[MGetLanguage()][i],4);
                }
            }
        }
        else if (page == 2)
        {
            row_number = 4;     //7
            if( stWeldPara.type == 3)//T��
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(uint8_t*)_Weld_B12[MGetLanguage()][i],4);
                }
            }
            else
            {
                for (i=0; i<row_number; i++)
                {
                    TextOut(menu_xpos+C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,12+12*MGetLanguage(),(uint8_t*)_Weld_B22[MGetLanguage()][i],4);

                }
            }
        }

        MSetColor(C_CR_PARA);
        if( stWeldPara.type > 0 )
        {
            if( stWeldPara.thick1 < 10)
            {
                stWeldPara.face = 0;
                stWeldPara.thick1 = 200;
                stWeldPara.wide = 250;
                stWeldPara.thick2 = 200;
                stWeldPara.high1 = 30;
                stWeldPara.high2 = 20;
                stWeldPara.radius = 0;
                stWeldPara.angle1 = 600;
                stWeldPara.angle2 = 300;
                stWeldPara.deep = 200;
                stWeldPara.blunt = 0;
                stWeldPara.space = 20;
                stWeldPara.probe = 200;

                stWeldPara.angle3=0;	//���ڽǶȣ�< 180;
                stWeldPara.angle4=0;	//���ڽǶȣ�< 180;
                stWeldPara.deep3=0;	//������ȣ�< 100mm;
            }
        }
        if( stWeldPara.type < 3 && stWeldPara.face > 1 )
        {
            stWeldPara.face = 0;
        }

        if( stWeldPara.type == 3  )
        {
            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep;
        }
        else
        {
            //�۱߼��¿�����ڰ��
            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
            if( stWeldPara.thick1 < stWeldPara.deep + stWeldPara.blunt )
            {
                if( stWeldPara.type == 1 )
                {
                    stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep;
                }
                else stWeldPara.blunt = 0;
            }
        }

        if( stWeldPara.type < 3 && stWeldPara.thick2 > stWeldPara.thick1  )
        {
            //����/˫
            stWeldPara.thick2 = stWeldPara.thick1 ;
        }
        if( stWeldPara.type < 3 )
        {
            //����/˫
            if( stWeldPara.angle1 > 1500  )	stWeldPara.angle1 = 1500;
            if( stWeldPara.angle1 < stWeldPara.angle2 )stWeldPara.angle2 = stWeldPara.angle1/2;
            if( stWeldPara.angle3 > 1500  )	stWeldPara.angle3 = 1500;
            if( stWeldPara.angle3 < stWeldPara.angle4 )stWeldPara.angle4 = stWeldPara.angle3/2;

            ///	if( stWeldPara.thick2 < stWeldPara.deep )stWeldPara.thick2 = stWeldPara.deep ;
        }
        else if(stWeldPara.angle1 > 800  )
        {
            stWeldPara.angle1 = 800 ;
        }
        if( stWeldPara.type == 3 && stWeldPara.angle2 > 800  )
        {
            stWeldPara.angle1 = 800;
        }

        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT-1;
        }
        switch(page)
        {
        case 1://��һҳ
            //��������
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (uint16_t*)_Weld_C1 + stWeldPara.type * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
            TextOut(xpos,ypos,1,8,16,(uint8_t*)_Weld_C1[MGetLanguage()][stWeldPara.type]+2,4);


            //̽����
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (uint16_t*)_Weld_D1 + stWeldPara.face * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
            TextOut(xpos,ypos,1,2,16,(uint8_t*)_Weld_D1[MGetLanguage()][stWeldPara.face]+2+8*MGetLanguage(),4);

            //�������
            row = 2;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //Write_Number(xpos,ypos,stWeldPara.thick1 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.thick1,4,1);

            //��Ч���
            row = 3;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.thick2 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.thick2,4,1);

            //������
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.wide  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.wide,4,1);

            //�����뾶
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.radius  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.radius,4,1);


            //�϶����
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.high1  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.high1,4,1);

            //�¶����
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.high2 ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.high2,4,1);

            //�¿ڽǶ�
            row = 8;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle1 ,4,1,0);

            break;
        case 2://�ڶ�ҳ
            //�¿����
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle2 ,4,1,0);

            //�¿����
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.deep  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.deep,4,1);

            //�۱߸߶�
            row = 2;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.blunt  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.blunt,4,1);

            //������϶
            row = 3;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
//			Write_Number(xpos,ypos,stWeldPara.space  ,4,1,0);
            WriteLongness(xpos,ypos,stWeldPara.space,4,1);

            //�¿ڽǶ�
            row = 4;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle3 ,4,1,0);

            //�¿ڽǶ�
            row = 5;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,stWeldPara.angle4 ,4,1,0);

            //�¿ڽǶ�
            row = 6;
            if(row>(row_number-1))break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            WriteLongness(xpos,ypos,stWeldPara.deep3,4,1);

            break;
        }
        //������

        stWeldPara.probe = 0;
        WELD_DRAW( &stWeldPara,300, 600, 120, 250,MGetAngle(0), 0);

        MKeyRlx();		//ֻ�а������ͷŲ��˳�
        do
        {
            ///		if( GetElapsedTime() > elapsedtime1 + 200 )
            ///		{
            ///			InvertWords(27*C_ECHAR_HDOT,190+4, 2); //������ʾ�����ֵ�����
            ///			elapsedtime1 = GetElapsedTime() ;
            ///		}
            keycode = MGetKeyCode(10);
#if C_DEVTYPE == 14
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS || keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
#else
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
#endif
            {
                MKeyRlx();
                break;
            }
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(true);

        if (keycode == C_KEYCOD_RETURN)
        {
            MKeyRlx();
            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {
            SystemParaStore();
            ChannelParaStore();
            MSetSystem();

            retvalue = C_TRUE;
            break;
        }
#if C_DEVTYPE == 14
        if (keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_PLUS )
#else
        if (keycode == C_KEYCOD_RIGHT)
#endif
        {
            page++;
            if(page > page_max)page = 1;
            continue;
        }
#if C_DEVTYPE == 14
        else if (keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_MINUS )
#else
        else if (keycode == C_KEYCOD_LEFT)
#endif
        {
            page--;
            if(page == 0)page = page_max;
            continue;
        }


        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(menu_xpos + C_ECHAR_HDOT,ypos-MGetLanguage()*C_ECHAR_VDOT, 1); //������ʾ�����ֵ�����

        if( MGetUnitType() > 0)deci_len = 3;
        else deci_len = 1;//����1С��
        number = 100;
        if( page == 1)//�������
        {
            switch (keycode)
            {
            case 1:		//��������
                //	cpText = (uint16_t*)_Weld_C1 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                for (i=0; i<4; i++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,6+2*MGetLanguage(),16,(uint8_t*)_Weld_C1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    {
                        MKeyRlx();
                        break;
                    }
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        stWeldPara.type = keycode-1;
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//̽����
//				cpText = (uint16_t*)_Weld_D1 ;
                MPushWindow(xpos, ypos, xpos + (6+2*MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 3+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	//��ָ��λ�ø���ÿ���ַ������������и�д�˵�
                for (i=0; i<3; i++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,8+3*MGetLanguage(),16,(uint8_t*)_Weld_D1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    {
                        MKeyRlx();
                        break;
                    }
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        stWeldPara.face = keycode-1;
                        break;
                    }
                }
                MPopWindow();
                break;
            case 3://�������
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.thick1 = number;
                        break;
                    }
                }
                break;
            case 4://��Ч���
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1 && stWeldPara.type < 3 )
                    {
                        stWeldPara.thick2 = number;
                        break;
                    }
                    else
                    {
                        stWeldPara.thick2 = number;
                        break;
                    }

                }
                break;
            case 5://������
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.wide = number;
                        break;
                    }
                }
                break;
            case 6://�����뾶
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.radius = number;
                        break;
                    }
                }
                break;
            case 7://�϶����
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.high1 = number;
                        break;
                    }
                }
                break;
            case 8://�¶����
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.high2 = number;
                        break;
                    }
                }

                break;
            case 9://�¿ڽǶ�
                deci_len = 1;//����1С��
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle1 = number;
                        if( stWeldPara.type < 3 )stWeldPara.angle2 = stWeldPara.angle1/2;
                        break;
                    }
                }
                break;
            }
        }
        else if(page == 2)
        {
            switch( keycode )
            {
                //�ڶ�ҳ
            case 1://�¿����
                deci_len = 1;//����1С��
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle2 = number;
                        if( number > stWeldPara.angle1 && stWeldPara.type < 2 )
                        {
                            stWeldPara.angle1 = number;
                        }
                        break;
                    }
                }
                break;
            case 2://�¿����
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1)
                    {
                        stWeldPara.deep = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep;
                        }
                        else
                        {
                            //�۱߼��¿�����ڰ��
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep;
                            }
                        }

                        break;
                    }
                }
                break;
            case 3://�۱߸߶�
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else //if( number < stWeldPara.thick1 )
                    {
                        stWeldPara.blunt = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.deep = stWeldPara.thick2 - stWeldPara.blunt;
                        }
                        else
                        {
                            //�۱߼��¿�����ڰ��
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.deep = stWeldPara.thick1 - stWeldPara.blunt;
                            }
                        }


                        break;
                    }
                }
                break;
            case 4://������϶
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.space = number;
                        break;
                    }
                }
                break;

            case 5://�¿ڽǶ�
                deci_len = 1;//����1С��
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle3 = number;
                        if( stWeldPara.type < 3 )stWeldPara.angle4 = stWeldPara.angle3/2;
                        break;
                    }
                }
                break;

            case 6://�¿����
                deci_len = 1;//����1С��
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        stWeldPara.angle4 = number;
                        if( number > stWeldPara.angle3 && stWeldPara.type < 2 )
                        {
                            stWeldPara.angle3 = number;
                        }
                        break;
                    }
                }
                break;

            case 7://�¿����
                while(true)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else if( number <= stWeldPara.thick1)
                    {
                        stWeldPara.deep3 = number;
                        if( stWeldPara.type == 3  )
                        {
                            if( stWeldPara.thick2 < stWeldPara.deep + stWeldPara.blunt)
                                stWeldPara.blunt = stWeldPara.thick2 - stWeldPara.deep3;
                        }
                        else
                        {
                            //�۱߼��¿�����ڰ��
                            if( stWeldPara.thick1 < stWeldPara.deep)stWeldPara.deep3 = stWeldPara.thick1;
                            if( stWeldPara.type < 2 )
                            {
                                stWeldPara.blunt = stWeldPara.thick1 - stWeldPara.deep3;
                            }
                        }

                        break;
                    }
                }
                break;
            }
        }

    }
    return retvalue;
}


int ShowWeld(short lxpos,short lypos,short width,short height,short angle,int dist)
{

    int keycode;
    short xpos,ypos;
    short cypos,size;
//	int number;
    int probe;

    uint8_t echo[C_LEN_SAMP];
    probe = C_OFF_SAMP;
    MCopyProtectedMemory( echo, (void*)probe, C_LEN_SAMP, PM_COPY_READ);

    xpos = ( C_COORWIDTH - C_CCHAR_HDOT * 15 ) / 8 * 8-2*8*MGetLanguage();
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH+24+9*8*MGetLanguage(), ypos + 60) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_Weld_A2,12,1,24);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,28,32,(uint8_t*)_Weld_A2[MGetLanguage()][0],4);

    if( stWeldPara.type == 3)
    {
        cypos = lypos +height/2+5;
        size = 30;
    }
    else
    {
        cypos = lypos +height/2+5;
        size = 50;
    }

    xpos += C_CCHAR_HDOT * 12+5*8*MGetLanguage();
    ypos += 4;
    probe = stWeldPara.probe - MGetForward();
    keycode = 1;
    probe = 0;
    while(true)
    {
        if( keycode > 0)
        {
            stWeldPara.probe = probe + MGetForward();
            EraseDrawRectangle(lxpos, lypos, lxpos+500, lypos+height) ;
            WELD_DRAW( &stWeldPara,lxpos+10, lxpos+480, cypos, size,angle, dist);
        }
        else
        {
            keycode = 1;
            break;
        }
        Write_Number( xpos,ypos+MGetLanguage()*C_ECHAR_VDOT,probe,5,1,0);
        if( MGetUnitType() > 0)keycode = 3;//��λinch
        else keycode = 1;//��ʱ����С��λ��
        keycode = Input_Number(xpos-MGetLanguage()*C_ECHAR_HDOT*6,ypos+24,&probe,3, &keycode,0) ;
        if( keycode == 0)break;//���˷��ؼ�
    }
    probe = C_OFF_SAMP;
    MCopyProtectedMemory( (void*)probe, echo, C_LEN_SAMP, PM_COPY_WRITE); //
    return keycode;
}
int ShowWeldA(short lxpos,short lypos,short width,short height,short angle,int dist)
{
    short xpos,ypos;
    short cypos,size;

    if( stWeldPara.type == 3)
    {
        cypos = lypos +height/2+5;
        size = 30;
    }
    else
    {
        cypos = lypos +height/2+5;
        size = 50;
    }

    xpos = ( C_COORWIDTH - C_CCHAR_HDOT * 14) / 8 * 8;
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, C_COORWIDTH, ypos + 40) ;
//	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_Weld_A2,12,1,24);
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,28,24,(uint8_t*)_Weld_A2[MGetLanguage()][0],4);
    xpos += C_CCHAR_HDOT * 12;
    ypos += 4;
    EraseDrawRectangle(lxpos, lypos, lxpos+500, lypos+height) ;
    WELD_DRAW( &stWeldPara,lxpos+10, lxpos+480, cypos, size,angle, dist);
    Write_Number( xpos,ypos + 9,stWeldPara.probe - MGetForward(),5,1,0);
    while(true)
    {
        if( MAnyKeyReturn() == C_KEYCOD_RETURN)break;//���˷��ؼ�
    }
    return stWeldPara.probe;
}



int tspLine(POINT *PosIn, POINT *PosOut, int n, int ch, float tx1, float tx2, float ty1, float ty2)
{
    const int MAX_NUM = 32;
    float x[MAX_NUM], y[MAX_NUM];
    int num = 0, tt;
    float a[MAX_NUM], b[MAX_NUM], c[MAX_NUM], dx[MAX_NUM], dy[MAX_NUM];
    float qx[MAX_NUM], qy[MAX_NUM];
    float bx3, bx4, by3, by4;
    float cx, cy, t[MAX_NUM+1], px[MAX_NUM], py[MAX_NUM];
//	float u[3], v[3];
    short i;

    for (i=0; i<n; i++)
    {
        x[i] = PosIn[i].x;
        y[i] = PosIn[i].y;
    }
    for (i=0; i<n; i++)
        t[i] = 73;//hypot(x[i+1] - x[i], y[i+1] - y[i]);
    t[n] = 1.0e+8;//Ϊ�Ժ�ļ����ʼ��t[n]��t[0]����
    switch(ch)
    {
    case 0: //��������
        a[0] = 2;
        c[0] = 1;
        dx[0] = 3 * (x[1] - x[0]) / t[1];
        dy[0] = 3 * (y[1] - y[0]) / t[1];
        a[n] = 2;
        b[n] = 1;
        dx[n] = 3 * (x[n] - x[n-1]) / t[n];
        dy[n] = 3 * (y[n] - y[n-1]) / t[n];
        break;
    case 1: //�г�����
        a[0] = 1;
        c[0] = 0;
        dx[0] = tx1;
        dy[0] = ty1;
        a[n] = 1;
        b[n] = 0;
        dx[n] = tx2;
        dy[n] = ty2;
        break;
    default:
        return false;
    }
    //���㷽����ϵ����ͳ�����
    for (i=1; i<=n-1; i++)
    {
        a[i] = 2 * (t[i] + t[i+1]);
        b[i] = t[i+1];
        c[i] = t[i];
        dx[i] = 3 * (t[i] * (x[i+1] - x[i]) / t[i+1] + t[i+1] * (x[i] - x[i-1]) / t[i]);
        dy[i] = 3 * (t[i] * (y[i+1] - y[i]) / t[i+1] + t[i+1] * (y[i] - y[i-1]) / t[i]);
    }
    //����׷�Ϸ��ⷽ����
    c[0] = c[0] / a[0];
    for (i=1; i<=n-1; i++)
    {
        a[i] = a[i] - b[i] * c[i-1];
        c[i] = c[i] / a[i];
    }
    a[n] = a[n] - b[n] * c[i-1];
    qx[0] = dx[0] / a[0];
    qy[0] = dy[0] / a[0];

    for (i=1; i<=n; i++)
    {
        qx[i] = (dx[i] - b[i] * qx[i-1]) / a[i];
        qy[i] = (dy[i] - b[i] * qy[i-1]) / a[i];
    }
    px[n] = qx[n];
    py[n] = qy[n];

    for (i=n-1; i>=0; i--)
    {
        px[i] = qx[i] - c[i] * px[i+1];
        py[i] = qy[i] - c[i] * py[i+1];
    }
    //���������ϵ������

    for (i=0; i<n-1; i++)
    {
        bx3 = (3 * (x[i+1] - x[i]) / t[i+1] - 2 * px[i] - px[i+1]) / t[i+1];
        bx4 = ((2 * (x[i] - x[i+1]) / t[i+1] + px[i] + px[i+1]) / t[i+1]) / t[i+1];
        by3 = (3 * (y[i+1] - y[i]) / t[i+1] - 2 * py[i] - py[i+1]) / t[i+1];
        by4 = ((2 * (y[i] - y[i+1]) / t[i+1] + py[i] + py[i+1]) / t[i+1]) / t[i+1];
        tt = 0;
        while (tt < t[i+1])
        {
            cx = x[i] + (px[i] + (bx3 + bx4 * tt) * tt) * tt;
            cy = y[i] + (py[i] + (by3 + by4 * tt) * tt) * tt;
            //if ((cx <= 10)) Beep(1000,2);
            PosOut[num].x = (short)cx;
            PosOut[num++].y = (short)cy;
            tt++;
        }
    }
    PosOut[num].x = PosIn[n-1].x;
    PosOut[num++].y = PosIn[n-1].y;
    return num;
}
