//test.h
#ifndef _TEST_H_


typedef struct
{
    int speed;		//Ԥ������
    int range[2];	//��һ����/�ڶ�����
    int rangemode;	//��������0���룬1��ȣ�2�������β�������

    int offset;		//���
    unsigned short *pPmt1;	//��ʾ1
    unsigned short *pPmt2;	//��ʾ2

    int len1;	//��ʾ1��ÿ�г���
    int row1;	//��ʾ1������
    int len2;	//��ʾ2��ÿ�г���
    int row2;	//��ʾ2������

} TEST_OFFSET;

TEST_OFFSET stTestOffset;
int TestOffset(TEST_OFFSET *stTestOffset)
{
    /*
    t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
    */

    int i,j;
    int testrange[2];
    int gain= 400, range, offset , offset0, delay = 0, speed = stTestOffset->speed;

    int amp_low,amp_high;
    int gateposi,gatewide;
    int rangemode = stTestOffset->rangemode;

    int echotime;
    int gateamp;

    int len,row,crow_height;

    //int number,deci_len;
    int xpos,ypos,menu_xpos,menu_ypos;
    int confirm_num = 0;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;

    unsigned short _Pmt[] = { CH(1866),CH(3367),CH(2167),CH(4189),CH(4357),CH(100),CH(100),CH(100),CH(100),CH(100),CH(100)};
    //int i;

    for( i = 0; i < 2; i++)testrange[i] = stTestOffset->range[i];

    MKeyRlx();
    MSetScaleMode(2,0);

    if( MGetProbeMode() == C_DPROBEZ||MGetProbeMode() == C_DPROBEA )	offset0 = 120;
    MSetOffset(offset0,C_SETMODE_SAVE);

    range = testrange[0] * 100/40;		//���ڵ�4��
    MSetPara(gain,0,0,0,range,offset0,delay,0,speed);

    amp_low = MGetAmpStdMax()-1;
    amp_high = MGetAmpStdMax() +1;
    gateposi = C_COORHORIUNIT * 4;
    if( MGetProbeMode() == C_DPROBEZ||MGetProbeMode() == C_DPROBEA )gatewide = C_COORHORIUNIT * 3;
    else gatewide = C_COORHORIUNIT * 2;
    MSetGatePara(gateposi,gatewide,120,0,C_SETMODE_SETSAVE);

    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);

    MSetSystem();
    MChannelRenovate();	//��ͨ����Ļ����ˢ��
    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetAcquisition(1);

    len = stTestOffset->len1;
    row = stTestOffset->row1;
    crow_height = 24;

    menu_xpos = C_COORHORIUNIT * 10-(len+1)*C_CCHAR_HDOT;
    menu_ypos = C_COORVPOSI + 1;
    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + crow_height * row_number) ;
    CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, stTestOffset->pPmt1 ,len,row,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
    //Write_Number(menu_xpos + 11 * C_ECHAR_HDOT,menu_ypos  + 4 ,testrange0,4,1,0);

    xpos = 0;
    ypos = 0;
    ClearCursor(2);
    MSetGateParaInit();	//������߲����ֵ

    preElapsedtime = GetElapsedTime();	//��ʼ����ʱ��

    MSetSystemMode(1,0);		//���ó��Զ�����״̬
    CMenuOut(0,0,_Pmt,6,1,24);
    while (1)
    {

        if (MAdjustGain(0,1,amp_low, amp_high ) )
        {
            MSetGateParaInit();	//������߲����ֵ
        }

        MParaRenovate(0);
        gateamp = MGetAmpMax(gatetype);
        if(MGetGateParaMax(gatetype) == C_TRUE )
        {
            ClearCursor(2);
            xpos = GateMax.Pos;
            ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
            if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
        }
        keycode = MGetKeyCode(0);

        if(keycode == C_KEYCOD_CONFIRM )
        {
            //t = ( R2*T1 - R1 * T2)/( R2 - R1) = T1 - (T2-T1)R1/(R2-R1)
            if( confirm == 0)
            {
                confirm_num++;
                ClearCursor(2);
                echotime = MGetAmpTime(gatetype);

                offset = echotime - testrange[0]*C_SAMPLE_FREQ/speed;//�Ȱ�Ԥ�����ٺ�һ�β�ʱ��������

                if( testrange[1] <= testrange[0] )
                {
                    //�����������С��һ�����̻�Ϊ0����ֻ��һ��
                    confirm_num++;
                    break;
                }
                else
                {
                    if( testrange[1] > testrange[0] * 2)//�������̴���һ�ε�����
                    {
                        range = testrange[1] * 100/80;		//���ڵ�8��
                    }
                    gateposi = C_COORHORIUNIT * 8-7;
                    if( MGetProbeMode() == C_DPROBEZ||MGetProbeMode() == C_DPROBEA )gatewide = C_COORHORIUNIT * 3;
                    else gatewide = C_COORHORIUNIT * 1.5;
                    MSetGatePara(gateposi, gatewide, 120,0,C_SETMODE_SETSAVE);
                    gain = MGetBaseGain()+120;
                    MSetRange(range,C_SETMODE_SETSAVE);
                    MSetBaseGain(gain,C_SETMODE_SETSAVE);

                    MChannelRenovate();	//��ͨ����Ļ����ˢ��

                    MSetGateParaInit();	//������߲����ֵ


                    len = stTestOffset->len2;
                    row = stTestOffset->row2;
                    crow_height = 24;

                    menu_xpos = C_COORHORIUNIT * 8-(len+1)*C_CCHAR_HDOT;
                    menu_ypos = C_COORVPOSI + 1;
                    EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + crow_height * row_number) ;
                    CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, stTestOffset->pPmt2 ,len,row,crow_height);	/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/

                    //				xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT;
                    //				ypos = C_COORVPOSI + 1;
                    Write_Number(xpos + 11 * C_ECHAR_HDOT,ypos  + 4 ,testrange0,4,1,0);
                }

            }
            else
            {
                confirm_num++;
                offset = MGetAmpTime(gatetype) - echotime ;	//T2 - T1
                offset = echotime - testrange[0] * offset / (testrange[1] - testrange[0]);

                speed = testrange[0] * C_SAMPLE_FREQ/(echotime - offset);
                MSetSpeed(speed,C_SETMODE_SETSAVE);

                break;
            }
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                retvalue = C_FALSE;
                SystemParaRestore();
                ChannelParaRestore();
                break;
            }
        }
        else if(keycode == C_KEYCOD_BASEGAIN)
        {
            if (GetSameKeyTime() > 20)
            {
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            }
            else
            {
                GainChange();
                //	CMenuOut(0,0,_TestMenu_A1+1,5,1,crow_height);
                TextOut(0,0,1,11,crow_height,(u_char*)_TestMenu_A1[MGetLanguage()][0]+2,4);
            }
        }
        else if (keycode == C_KEYCOD_GATE)
        {
            GateChange();
        }
        else if (keycode == C_KEYCOD_RANGE)
        {
            RangeChange();
        }
        else if (keycode == C_KEYCOD_OFFSET)
        {
            OffsetChange();
        }
        else continue;
        DisplayPrompt(15);
        CMenuOut(0,0,_Pmt,6,1,24);
        ExpendTime(10);
    }
    if( confirm_num > 1)
    {
        MSetOffset(offset + offset0,C_SETMODE_SETSAVE);
        if( MGetProbeMode() == C_APROBE )
        {
            //б̽ͷ
            len = 8;
            row = 2;
            crow_height = 24;

            menu_xpos = C_COORHORIUNIT * 6-(char_len+1)*C_CCHAR_HDOT;
            menu_ypos = C_COORVPOSI + 1;
            EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height * row) ;
            TextOut(menu_xpos + C_ECHAR_HDOT,menu_ypos+2, row+1*MGetLanguage(), 17,crow_height-12*MGetLanguage(), (u_char*)_TestMenu_AB3[MGetLanguage()][1], 4);

//			EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 6, menu_ypos + row * crow_height) ;
//			CEMenuOut(menu_xpos + C_ECHAR_HDOT,menu_ypos + 4, _TestMenu_AB7,len,row,crow_height);
            int number,deci_len;

            deci_len = 1;
            number = testrange1;
            menu_xpos = menu_xpos+10*C_ECHAR_HDOT+8*MGetLanguage();
            menu_ypos = C_COORVPOSI + 37-4*MGetLanguage();

            while(1)
            {
                if( Input_Number(menu_xpos, menu_ypos,&number,2, &deci_len,0) != 1)break;
                else
                {
                    Write_Number(menu_xpos, menu_ypos, number, 2, 1, 0);
                    MSetForward(testrange1-number,C_SETMODE_SAVE);
                    break;
                }
            }
        }
        //MSetTestStatus(C_TEST_OFFSET,1);	//���ò���״̬,����Ѳ�

        MSetGatePara(C_COORHORIUNIT * 3, C_COORHORIUNIT * 2 ,120,0,C_SETMODE_SETSAVE);

        //	MChannelRenovate();	//��ͨ����Ļ����ˢ��

        SystemParaStore();
        ChannelParaStore();

        retvalue = C_TRUE;
    }

    MKeyRlx();
    range = testrange1 * 100/40;		//���ڵ�6.5��
    MSetRange(range,C_SETMODE_SETSAVE);
    return retvalue;
}

int TestMenuZero(int iBlock/*�Կ�����*/,bool bBlockModify,int iProbemode/*̽ͷ����*/, bool bProbeModify, int iSoundmode/*��������*/, bool bSoundModify, int iRange1/*����1*/, bool bRangeModify1, int iRange2/*����2*/, bool bRangeModify2);
//�����˵�����bool��Ϊ����޸���Ӧ��
int TestZero(int iProbemode/*̽ͷ����*/, int iSpeed/*Ĭ������*/, int iRange1/*����1*/, int iRange2/*����2*/,int iForward/*�Ƿ��ǰ��*/);


int TestMenuAngle( int iAngle/*�������*/, bool bAngleModify, int iDiameter/*��ֱ��*/, bool bDiameterModify, int iDepth/*����*/, bool bDepthModify,int iContact/*�Ӵ���*/,bool bContact,int iOutRandius/*��뾶*/);
int TestAngle( int iAngle/*�������*/, int iDiameter/*��ֱ��*/, int iDepth/*����*/, int iContact/*�Ӵ���*/,int iOutRandius,int iInnerRandius);

int TestSensitivity(int mode,int iRange,bool bParaAdjust);//��������


int TestMenuOffset(void);
int TestOffset(u_int testrang1,u_int testrange2,u_int mode);
//��1���̣���2���� mode = 0����⣬=1��� =2 ����
//int TestMenuAngle(void);
//int TestAngle(u_int diameter,u_int depth);//ֱ�������

int TestMenuDac(void);
int TestDac(u_int depthmax,u_int diameter,u_int length);//�����ȡ�������ֱ��������
int CalcuDac(void);	//����DAC,
int DrawDac(int mode);		//���ͻ����DAC����,mode=0������=1��
int GetLine_TwoDot(u_short* line,int dist1,int dist2);
int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist);	//�õ��м�ֵ

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);


#define _TEST_H_
#endif

