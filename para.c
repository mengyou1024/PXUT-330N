//����ֲ����йصĺ���
extern uint16_t dac_line[];
extern FUNCTION             Function;   //����
extern int nFlag;

int ChannelParaInit(int mode)	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��,2�����û�*/
{
    int retvalue = 1;
    //int offset = C_OFF_CHANNEL;
    //uint8_t i=0;
    //int probe,speed;
//	int speed;
    int i;
    uint8_t Channel;
    int stdmode=MGetStdMode();
    int nCurrentUser = GetCurrentUser();
	int frequence = MGetFrequence();
//	int nUserNumber = GetUserNumber();

	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;

    int testrange[3];
    int offset;
    int ChMax = 0, UserMax, temp_ch ;

    Dac.num = 0;

    Channel = MGetChannel();
    if( Channel < C_CHMAX)temp_ch = Channel;
    else temp_ch = 0;
    UserMax = nCurrentUser + 1;
    if(mode == 0)
    {
        ChMax = Channel + 1;
    }
    else if (mode > 0)
    {
    	nFlag = 0;
		SystemStatus.prepare[2] = 0;
        MSaveDefualtColor(1);//����Ĭ����ɫ
        SystemStatus.option = 0;
        SystemStatus.sys = 0;
        Function.funcMenu[0]=1;
        Function.funcMenu[1]=2;
        Function.funcMenu[2]=7;
        Function.funcMenu[3]=0;

        offset = C_OFF_KEYFUNC;
        MCopyProtectedMemory( (void*)offset, &Function, C_LEN_FUNCTION, PM_COPY_WRITE);
        CopyMemoryToSystem( (void*)offset,C_LEN_FUNCTION,PM_COPY_WRITE);

        /*����ֵΪ��ǰ��ӡ������1\2\3\4=LQ\HP\EPSON C/P*/
        MSetPrinterMode(0,0/*mode = 0*/);	/*���õ�ǰ��ӡ������*/
        /*����ֵΪ��ǰ���뷨����1\2\3\4=����\��д\Сд*/
        MSetInputMode(C_INPUT_DIGITAL,0/*mode = 0*/);	/*���õ�ǰ���뷨����*/

        /*����ֵΪ��ǰϵͳ״̬1\2\3\4=����\����\���԰�\�����*/
        MSetSystemMode(0 ,0/*mode = 0*/);	/*���õ�ǰϵͳ״̬*/
        /*����ֵΪ��ǰ��Ļ����*/
        MSetScreenBrightness(3,C_SETMODE_SETSAVE/*mode = 0*/);		/**/

        MSetSaveMode(0,C_SETMODE_SETSAVE);//�ر�������¼
        MSetSaveStatus( 0,C_SETMODE_SETSAVE);
        MSetSavePosition(0,C_SETMODE_SETSAVE);

        MSetFill(0,C_SETMODE_SETSAVE);
        MSetDenoise(0,C_SETMODE_SETSAVE);
        MSetRepeatRate(100,C_SETMODE_SETSAVE);
        if(extend==0)
        {
            MSetPulseWidthInput(C_PULSE_MIN, C_SETMODE_SET);
        }
        else
        {
            MSetPulseWidth(0,C_SETMODE_SETSAVE);
        }

        MSetUnitType(0);

        ChMax = C_CHMAX ;
        Channel = 0;
        if( mode > 1)
        {
            nCurrentUser = 0;
            UserMax = C_USERMAX;
            SetUserNumber( C_USERMAX);
        }
    }
	
	if(mode>1)
	{
		UserInfo.pzPurchaser[0] = 1;
		UserInfo.pzPurchaser[1] = 12;
		UserInfo.pzPurchaser[2] = 250;
		UserInfo.pzPurchaser[3] = 13000;
		UserInfo.pzPurchaser[4] = 13000;
		UserInfo.pzPurchaser[5] = 2;
		UserInfo.pzPurchaser[6] = 500;
		UserInfo.pzPurchaser[7] = 20000;
		UserInfo.pzPurchaser[8] = 500;
		UserInfo.pzPurchaser[9] = 20000;
        UserInfo.pzPurchaser[10] = 1;
		
		UserInfo.pzSerial[0] = 0;
		UserInfo.pzSerial[1] = 0;
		UserInfo.pzSerial[2] = 1000;
		UserInfo.pzSerial[3] = 0;
		UserInfo.pzSerial[4] = 500;
		UserInfo.pzSerial[5] = 300;
		UserInfo.pzSerial[6] = 800;
		UserInfo.pzSerial[7] = 20;
		UserInfo.pzSerial[8] = 400;
		UserInfo.pzSerial[9] = 2000;
		UserInfo.pzSerial[10] = 20;
		UserInfo.pzSerial[11] = 0;
		UserInfo.pzSerial[12] = 2000;
		UserInfo.pzSerial[13] = 20;
		UserInfo.pzSerial[14] = 0;
		UserInfo.pzSerial[15] = 200;
        UserInfo.pzSerial[16] = 1000;
        UserInfo.pzSerial[17] = 0;
        UserInfo.pzSerial[18] = 1000;
        UserInfo.pzSerial[19] = 0;

        UserInfo.pzGovernor[0] = 0;
        UserInfo.pzGovernor[1] = 0;
        UserInfo.pzGovernor[2] = 0;
        UserInfo.pzGovernor[3] = 0;
        
		MCopyProtectedMemory((void*)offsets, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
		CopyMemoryToSystem((void*)offsets, C_OFF_BATTERY-C_OFF_USER_INFO, PM_COPY_WRITE);
	}



    for( ; nCurrentUser < UserMax; nCurrentUser++)
    {
        if( mode > 0 )
        {
            SetCurrentUser(nCurrentUser);
            SystemParaStore();
            Channel = 0;
        }

        for( ; Channel < ChMax; Channel++)
        {
			MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);

            ExpendTime(50);
//Write_Number(0,0,Channel,4,0,0);
            ChannelStatus.option = 0;
            ChannelStatus.status = 0;
            ChannelStatus.sys = 0;

            SystemStatus.displaymode = 1;
            SystemStatus.language = 0;
            SystemStatus.unit = 0;

            stWeldPara.type = 0;
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

            MSetAmpStdMax(80);//�ز�����ʱ�ı�׼�߶ȣ�Ĭ��80��


            /*����ֵΪ��ǰ��������*/
            ///MSetScrsaveMode(1,0/*mode = 0*/);	/*���õ�ǰ��������*/

            /*����ֵΪ��ǰ������ʱ,������30����*/
            ///MSetScrsaveTime(1,0/*mode = 0*/);	/*���õ�ǰ������ʱ,������30����*/

            /*����ֵΪ��ǰ���1\2\3=��ֱ\ˮƽ\����*/
            MSetScaleMode(0,2/*mode = 0\1\2*/);	/*���õ�ǰ���*/

            /*�������õ�����׼��ĸ��\�з�\����\����	*/
            MSetDacMode(2,0/*mode = 0*/);

            /*�������õ�����ʾ��dBֵ\�׾�	*/
            MSetAvgMode(0,0/*mode = 0*/);

            /*��������Ƶ����ȣ�2.5\5.0\10\wide	*/
#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 12
            MSetBandMode(3,0/*mode = 0\1\2*/);
#else
			MSetBandMode(0,0/*mode = 0\1\2*/);
#endif
            /*��������̽ͷ���᣺50Q\400Q 	*/
            MSetDampMode(1,0/*mode = 0\1\2*/);

            /*�������÷����ѹ��200V\400V  	*/
			if(UserInfo.pzPurchaser[0]==1)
            	MSetPulseMode(2,0/*mode = 0\1\2*/);
			else
				MSetPulseMode(UserInfo.pzPurchaser[5],0/*mode = 0\1\2*/);

            /*�������ü첨��ʽ��ȫ��\���벨\���벨\��Ƶ  	*/
            MSetEchoMode(0,0/*mode = 0\1\2*/);


            /*����ֵΪ��ǰ̽ͷģʽ0\1\2\3=ֱ\б\˫��*/
            if(UserInfo.pzPurchaser[0]==1)
                MSetProbeMode(C_APROBE,0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/
            else
                MSetProbeMode(UserInfo.pzPurchaser[10],0/*mode = 0\1\2*/);	/*����̽ͷģʽ*/

            /*����ֵΪ��ǰ����״̬0\1\2\=δ��\AVG\DAC\AVG*/
            MSetTestStatus(C_TEST_ALL,0);

            /*
            ����Ϊ���ֲ����Ļ��������
            */
            MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
            MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,0,1,C_SETMODE_SETSAVE);
            MSetGatedB(21, 0);

            //������

            /*����ֵΪ��ǰͨ���Լ����٣�	��С��	*/
//			if( MGetProbeMode() == C_APROBE)speed = 3230;
//			else speed = 5900;
//			MSetSpeed(speed,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/
            /*����ֵΪ��ǰͨ���Լ����٣�	��С��	*/
            if(MGetProbeMode() != 1)	//��б̽ͷ
            {
                if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//��λinch
                else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
            }
            else
            {
                if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//��λinch
                else MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);

            }

            MSetDepth(0,0);
            /*����ֵΪ��ǰͨ�������	һλС��*/
            MSetAngle(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


            /*����ֵΪ��ǰͨ��̽ͷǰ�س��� һλС��	*/
            MSetForward(0,0/*mode = 0*/);	/*���õ�ǰͨ��	*/

            /*����ֵΪ��ǰͨ��̽ͷƵ�� һλС��	*/
			if(UserInfo.pzPurchaser[0]==1)
			{
				if(mode>0)
					MSetFrequence(500,0/*mode = 0*/);	/*���õ�ǰͨ��	*/
				else
	            	MSetFrequence(frequence,0/*mode = 0*/);	/*���õ�ǰͨ��	*/
			}
			else
			{
	            if(MGetProbeMode() == 0)
					MSetFrequence(UserInfo.pzPurchaser[6],0);
				else if(MGetProbeMode() == 1)
					MSetFrequence(UserInfo.pzPurchaser[2],0);
				else if(MGetProbeMode() == 2)
					MSetFrequence(UserInfo.pzPurchaser[8],0);
			}

            /*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
			if(UserInfo.pzPurchaser[0]==1)
			{
				MSetCrystal_l(13000,0/*mode = 0*/); /*���õ�ǰͨ��	*/
				MSetCrystal_w(13000,0/*mode = 0*/); /*���õ�ǰͨ��	*/
			}
			else
			{
				if(MGetProbeMode() == 1)
				{
		            MSetCrystal_l(UserInfo.pzPurchaser[3],0/*mode = 0*/);	/*���õ�ǰͨ��	*/
		            MSetCrystal_w(UserInfo.pzPurchaser[4],0/*mode = 0*/);	/*���õ�ǰͨ��	*/
				}
				else if(MGetProbeMode() == 0)
				{
					MSetCrystal_l(UserInfo.pzPurchaser[7],0/*mode = 0*/);
					MSetCrystal_w(0,C_SETMODE_SETSAVE);
				}
				else if(MGetProbeMode() == 2)
				{
					MSetCrystal_l(UserInfo.pzPurchaser[9],0/*mode = 0*/);
					MSetCrystal_w(0,C_SETMODE_SETSAVE);
				}
			}
            /*����ֵΪ��ǰͨ���������� һλС��	*/
            MSetBaseGain(450,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

            /*����ֵΪ��ǰͨ�����油������ һλС��	*/
            MSetSurfGain(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

            /*����ֵΪ��ǰͨ���������� һλС��	*/
            MSetCompGain(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


            /*����ֵΪ��ǰͨ��	*/
            for(i = 0; i < 6 ; i++)MSetLineGain(i,0);

            /*����ֵΪ��ǰͨ��	*/

            if( MGetUnitType() > 0)MSetRange(5000,C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/
            else MSetRange(2000,C_SETMODE_SETSAVE/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/


            /*����ֵΪ��ǰͨ��	*/
            MSetDelay(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

            MSetDepthGain(0);


            MSetReject(0,0);

            /*����ֵΪ��ǰͨ��	*/
            MSetOffset(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

            MSetFunctionMode(0,C_FUNCTION_ALL);
            /*�������ñ�׼	*/
			if(UserInfo.pzPurchaser[0]==1)
				MSetStdMode(12,0/*mode = 0*/);
			else
				MSetStdMode(UserInfo.pzPurchaser[1],0/*mode = 0*/);
			
            //MSetStdMode(1,C_SETMODE_SAVE);
            MSetCurveNumber(3);
            MSetBlock(0);
            MSetThick(0);
            MSetDiameter(0);

            MSetChannel(Channel,0);

            ChannelParaStore();

            retvalue = C_TRUE;
        }
    }

//Write_Number(0,10,1,4,0,0);
    if( mode > 1)
    {
        SetCurrentUser(0);
        SetUserNumber( 1 );
    }

//Write_Number(0,20,2,4,0,0);
    MSetChannel(temp_ch,C_SETMODE_SETSAVE);
    ChannelParaRestore();
//Write_Number(0,20,3,4,0,0);
    MSetSurfGain(0,0/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/
    ChannelParaStore();
//Write_Number(0,20,4,4,0,0);
    ExpendTime(20);
    MSetSystem();		/*���õ�ǰϵͳ��״̬*/

	if(UserInfo.pzPurchaser[0]==0)
	{
		MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(UserInfo), PM_COPY_READ);
		if(MGetProbeMode() != 1)
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
		else if (MGetStdMode()==12)	//JB/T47013
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
	}

    return retvalue;
}

//���йص�
void MSetGatePara(uint16_t Posi,uint16_t Wide,uint16_t High,int gatetype,int mode)
/*�����Ų�����gatetype=0A��1B��*/
{
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        if(Posi > C_HORIDOT_SCREEN )Posi = C_HORIDOT_SCREEN - 1;
        if(Posi + Wide > C_HORIDOT_SCREEN )Wide = C_HORIDOT_SCREEN - 1 - Posi ;
        if(High > C_COORHEIGHT )High = C_COORHEIGHT ;
        if( gatetype == 0)
        {
            AGatePara.Position = Posi ;
            AGatePara.Width = Wide ;
            AGatePara.Height = High ;
        }
        else if( gatetype == 1)
        {
            BGatePara.Position = Posi ;
            BGatePara.Width = Wide ;
            BGatePara.Height = High ;
        }
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        GATE_PARA	GatePara;
#if C_DEVLIB == 1
        Posi = Posi + HoriOffsetScreen;
#endif
        if( MGetEchoMode() == C_RF_WAVE)
        {
            Posi = ( (Posi+1)/2 ) * 2;	//��Ƶ����λ����Ϊ˫
            Wide = ( (Wide+1)/2 ) * 2;
            High = ( (High+1)/2 ) * 2;
        }
        GatePara.Position = Posi ;

        GatePara.Width = Wide ;
        GatePara.Height = High ;
        //if( (MGetBGateMode() == 1 && gatetype == 1) || gatetype == 0 )
#if C_DEVLIB == 4
        SetPeakGate(MGetChannel(),&GatePara, gatetype) ;
#else
        SetPeakGate(&GatePara, gatetype) ;
#endif
        MSetColor(C_CR_WAVE);
        //if( MGetBGateMode() != 0 && gatetype == 1)
        if( gatetype == 1)
        {
            //ExpendTime(10);
            MSetGatePara(MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SET);
        }
    }
}


uint16_t MGetGatePara(int gatetype, int paratype)   /* ����ֵΪ�Ų��� */
{
    if(gatetype == 0 && paratype == 0)
    {
        if( AGatePara.Position >= C_COORWIDTH )AGatePara.Position = C_COORWIDTH;
        return AGatePara.Position;
    }

    if(gatetype == 0 && paratype == 1)
    {
        if( AGatePara.Position > C_COORWIDTH )AGatePara.Position = C_COORWIDTH;
        if( AGatePara.Position + AGatePara.Width >= C_GATEPOSIMAX )AGatePara.Width = C_GATEPOSIMAX - AGatePara.Position;
        return AGatePara.Width;
    }
    if(gatetype == 0 && paratype == 2)
    {
        if( AGatePara.Height > C_COORHEIGHT )AGatePara.Height = C_COORHEIGHT;
        return AGatePara.Height;
    }
    if(gatetype == 1 && paratype == 0)
    {
        if( BGatePara.Position >= C_COORWIDTH )BGatePara.Position = C_COORWIDTH;
        return BGatePara.Position;
    }

    if(gatetype == 1 && paratype == 1)
    {
        if( BGatePara.Position > C_COORWIDTH )BGatePara.Position = C_COORWIDTH;
        if( BGatePara.Position + BGatePara.Width >= C_GATEPOSIMAX )BGatePara.Width = C_GATEPOSIMAX - BGatePara.Position;
        return BGatePara.Width;
    }

    if(gatetype == 1 && paratype == 2)
    {
        if( BGatePara.Height > C_COORHEIGHT )BGatePara.Height = C_COORHEIGHT;
        return BGatePara.Height;
    }

    return 0;
}


//DAC gate
int MGetDacDb(void)
{
    int offset;
    uint16_t dac_dB;
    int basegain;

    basegain = MGetBaseGain() + 481.31;	//��׼����
    offset = C_OFF_DACDB + MGetGatePara(0,0)*C_SIZE_SHORT ;	//��λ
    MCopyProtectedMemory(&dac_dB, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
    //dacdB����DACĸ����λ����dBֵ
    dac_dB = basegain - dac_dB;
    return dac_dB;
}

int DacGate_dBHigh(int dB,int dac_dB)//����ĸ�ߵ�dB�DACdBֵ
{
    //int offset;
    int gatehigh;

    MSetGatedB(dB, 0);
	gatehigh = (int)pow(10,(float)(dB+dac_dB)/200.0);
    if(gatehigh >= C_COORHEIGHT ) gatehigh = C_COORHEIGHT - 1 ;
    return gatehigh;
}

void DacGateInit(void)
{
    int offset;
    int dac_dB,gate_dB;
    int basegain;
    int gatehigh;
    int i;
    int dist1,dist2;
    int step = 14;


    dac_dB = MGetDacDb();	//dacdB����DACĸ����Ϊ����dBֵ
    gate_dB = MGetGatedB();

    gatehigh = DacGate_dBHigh(gate_dB,dac_dB);//����ĸ�ߵ�dB�DACdBֵ
    MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),gatehigh,0,C_SETMODE_SETSAVE);

    basegain = MGetBaseGain() + 481.31 + gate_dB;	//��׼����
    offset = C_OFF_DACDB;
    MCopyProtectedMemory( dac_line, (void*)offset, C_LEN_DACDB, PM_COPY_READ);
    dist2 = 0;
	dac_line[dist2] = (int)pow( 10,( basegain - dac_line[dist2] )/200.0) ;
    while(dist2 <= C_COORWIDTH)
    {
        dist1 = dist2;
        dist2 += step;
		dac_line[dist2] = (int)pow( 10,( basegain - dac_line[dist2] )/200.0) ;
        GetLine_TwoDot(dac_line,dist1,dist2);
    }
    for(i = 0 ; i < C_SCREEN_WIDTH; i ++ )
    {
        if(dac_line[i] >= C_COORHEIGHT)dac_line[i] = C_COORHEIGHT - 1;
    }
    offset = C_OFF_DACGATE;
    MCopyProtectedMemory((void*)offset, dac_line, C_LEN_DACDB, PM_COPY_WRITE);
}

void DACGatedB(void)	//DAC��dB��
{
    int offset;
    int basegain;
    uint16_t Height;
    uint16_t gatedB;

    basegain = MGetBaseGain() + 481.31;	//��׼����
    offset = C_OFF_DACDB + MGetGatePara(0,0);	//��λ
    MCopyProtectedMemory(&gatedB, (void*)offset, C_SIZE_SHORT, PM_COPY_READ);
	Height = (int) pow(10,(basegain - gatedB)/200.0) / pow(10, MGetGatedB()/200.0) ;
}


//����ͨ�������йصĺ���

void MSetPara(int bgain,int cgain,int sgain,int scale,int range,int offset,int delay,int angle,int speed)
{
    int Min_range=MGetSpeed()*2.5/1000+1;
    Min_range=Min_range*10;
    int mode = C_SETMODE_SETSAVE;
    if( bgain >= 0 && bgain < C_MAX_BASEGAIN)MSetBaseGain(bgain,mode);
    else MSetBaseGain(CD_BASEGAIN,mode);
    if( cgain >= 0)MSetCompGain(cgain,mode);
    else MSetCompGain(CD_COMPGAIN,mode);
    if( sgain >= 0)MSetSurfGain(sgain,mode);
    else MSetSurfGain(CD_SURFGAIN,mode);
    if( scale >= 0 && scale < 3)MSetScaleMode(scale,mode);
    else MSetScaleMode(0,mode);
    if( angle >= 0 && angle < C_MAX_ANGLE)MSetAngle(angle,mode);
    else MSetAngle(0,mode);
    if( offset >= 0 && offset < C_MAX_OFFSET)MSetOffset(offset,mode);
    else MSetOffset(0,mode);
    if (MGetUnitType()>0)
    {
        if( speed >= C_MIN_SPEED*100/25.4 && speed < C_MAX_SPEED*100/25.4)MSetSpeed(speed,mode);
        else if( MGetAngle(0) > 200 )MSetSpeed(CD_SPEED_1*100/25.4,mode);//�Შ
        else MSetSpeed(CD_SPEED_2*100/25.4,mode);//�ݲ�
        if( range >= Min_range/0.254 && range < C_MAX_RANGE/0.254 )MSetRange(range,mode);
        else MSetRange(CD_RANGE/0.254,mode);
        if( delay >= 0 && delay < C_MAX_DELAY/0.254)MSetDelay(delay,mode);
        else MSetDelay(0,mode);
    }
    else
    {
        if( speed >= C_MIN_SPEED && speed < C_MAX_SPEED)MSetSpeed(speed,mode);
        else if( MGetAngle(0) > 200 )MSetSpeed(CD_SPEED_1,mode);//�Შ
        else MSetSpeed(CD_SPEED_2,mode);//�ݲ�
        if( range >= Min_range && range < C_MAX_RANGE )MSetRange(range,mode);
        else MSetRange(CD_RANGE,mode);
        if( delay >= 0 && delay < C_MAX_DELAY)MSetDelay(delay,mode);
        else MSetDelay(0,mode);
    }
}

uint16_t MGetThick(void)//�õ��������
{
    return ChannelPara.Thick;
}
void MSetThick(uint16_t thick)//���ù������
{
    ChannelPara.Thick = thick;
}

uint16_t MGetDiameter(void)//�õ�����ֱ��
{
    return ChannelPara.Diameter;
}
void MSetDiameter(uint16_t Diameter)//���ù���ֱ��
{
    ChannelPara.Diameter = Diameter;
    if( Diameter >= 2 * MGetThick() )
    {
        MSetFunctionMode( 1,C_CURVER_AMEND);
    }
    else
    {
        MSetFunctionMode( 0,C_CURVER_AMEND);
    }
}

uint16_t MGetOffset(void)			/*����ֵΪ��ǰͨ��	*/
{
    if( ChannelPara.Offset > C_MAX_OFFSET)MSetOffset(C_MAX_OFFSET,C_SETMODE_SAVE);
    return ChannelPara.Offset;
}

void MSetOffset(uint32_t Offset,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Offset = Offset;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        Offset = ( Offset + MGetDelay(0) )/2;
        if( MGetProbeMode() == C_P_TRANSMISSION)Offset /= 2;
#if C_DEVLIB == 4
        SetEchoDelay(MGetChannel(),  Offset) ;
#else
        SetEchoDelay( Offset ) ;
#endif

    }
}

uint16_t MGetSpeed(void)			/*����ֵΪ��ǰͨ���Լ����٣�	��С��	*/
{
    if( MGetUnitType() > 0)
    {
        //��λinch
        if( ChannelPara.Speed > (C_MAX_SPEED*100/25.4+0.5))ChannelPara.Speed = C_MAX_SPEED*100/25.4+0.5;
        else if( ChannelPara.Speed < (C_MIN_SPEED*100/25.4+0.5))ChannelPara.Speed = C_MIN_SPEED*100/25.4+0.5;
    }
    else
    {
        //��λmm
        if( ChannelPara.Speed > C_MAX_SPEED)ChannelPara.Speed = C_MAX_SPEED;
        else if( ChannelPara.Speed < C_MIN_SPEED)ChannelPara.Speed = C_MIN_SPEED;
    }
    return ChannelPara.Speed;
}

void MSetSpeed(uint16_t Speed,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    int Scale=MGetScaleMode();
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetScaleMode(2,C_SETMODE_SETSAVE);
        float range = (float)(MGetRange(0) * Speed )/ (C_SAMPLE_FREQ);
        MSetRange( (int)(range+0.5),C_SETMODE_SAVE);
        MSetScaleMode(Scale,C_SETMODE_SETSAVE);
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Speed = Speed;
    }
}

uint32_t MGetAngle(uint32_t mode)			/*����ֵΪ��ǰͨ�������0�Ƕ�1����2����3����	һλС��*/
{
    uint32_t retvalue = ChannelPara.Angle;
    float angle = (float)retvalue * C_PI/1800.0;	//�ǶȻ�Ϊ����
    switch (mode)
    {
    case 1://kֵ
		retvalue = (int)(tanf(angle) * 10000.0 + 0.5);
        break;
    case 2://ˮƽ
		retvalue = (int)(sinf(angle) * 10000.0 + 0.5);
        break;
    case 3://��ֱ
		retvalue = (int)(cosf(angle) * 10000.0 + 0.5);
        break;
    case 4://kֵ
		retvalue = (int)(sinf(angle) * 10000.0/cosf(angle) + 0.5);
        break;
    }
    return retvalue;
}
void MSetAngle(uint16_t Angle,int mode)	/*���õ�ǰͨ��	*/
{
    ChannelPara.Angle = Angle;
}

int MGetDepth()
{
    return ChannelPara.depth;
}
void MSetDepth(int depth,int mode)
{
    ChannelPara.depth = depth;
}
uint16_t MGetKvalue(void)			/*����ֵΪ��ǰͨ��Kֵ ��λС��	*/
{
    float Angle = ChannelPara.Angle;
    if( Angle > 810 ) return 999;
    else
    {
        Angle = (ChannelPara.Angle)*C_PI/1800.0;
		return (int)(tan(Angle)*100+0.5);
    }
}
void MSetKvalue(uint16_t Kvalue,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
	ChannelPara.Angle = (int)( atan( (float)Kvalue/100.0) *1800/C_PI + 0.5);
}

uint16_t MGetForward(void)			/*����ֵΪ��ǰͨ��̽ͷǰ�س��� һλС��	*/
{
    return ChannelPara.Forward;
}
void MSetForward(uint16_t Forward,int mode/*mode = 0*/)	/*���õ�ǰͨ��	*/
{
    ChannelPara.Forward = Forward;
}

uint16_t MGetFrequence(void)			/*����ֵΪ��ǰͨ��̽ͷƵ�� һλС��	*/
{
    if( ChannelPara.Frequence > 0)
        return ChannelPara.Frequence;
    else return 5;
}
void MSetFrequence(uint16_t Frequence,int mode/*mode = 0*/)	/*���õ�ǰͨ��	*/
{
    ChannelPara.Frequence = Frequence;
    if( C_DEVLIB != 3)
    {
        if( Frequence < 400 )MSetBandMode(0,C_SETMODE_SETSAVE);
        else if( Frequence < 800 )MSetBandMode(1,C_SETMODE_SETSAVE);
        else if( Frequence < 1200 )MSetBandMode(2,C_SETMODE_SETSAVE);
        else MSetBandMode(3,C_SETMODE_SETSAVE);
    }
}

uint16_t MGetCrystal_l(void)			/*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
{
    return ChannelPara.Crystal_l;
}
uint16_t MGetCrystal_w(void)			/*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
{
    return ChannelPara.Crystal_w;
}

void MSetCrystal_l(uint16_t Crystal,int mode/*mode = 0*/)	/*���õ�ǰͨ��	*/
{
    ChannelPara.Crystal_l = Crystal;
}
void MSetCrystal_w(uint16_t Crystal,int mode/*mode = 0*/)	/*���õ�ǰͨ��	*/
{
    ChannelPara.Crystal_w = Crystal;
}
int MGetNearField(double multi)		//�õ�����������
{
    short number;
    int Near;

    if( MGetUnitType() > 0)
    {
        //��λinch
        number= MGetCrystal_l()*25.4/1000+0.5;	//��Ƭ�ߴ�ĵ�8λ����Ϊ0����ֱ̽ͷ����ֱ������
        if( number == 0)number = MGetCrystal_w()*25.4/1000+0.5;

        if( MGetCrystal_w() == 0)number *= MGetCrystal_l()*25.4 /1000+0.5;
        else number *= MGetCrystal_w ()*25.4/1000+0.5;
        if( number == 0)number = 25;
        multi *= number;
        Near = ( 100 * multi * MGetFrequence())/(4 * (MGetSpeed()*254/1000.0+0.5));
        return  (Near);
    }
    else
    {
        number= MGetCrystal_l()/1000;	//��Ƭ�ߴ�ĵ�8λ����Ϊ0����ֱ̽ͷ����ֱ������
        if( number == 0)number = MGetCrystal_w()/1000;

        if( MGetCrystal_w() <100)number *= MGetCrystal_l()/1000 ;
        else number *= MGetCrystal_w ()/1000;
        if( number == 0)number = 25;

        multi *= number;
        Near = ( 100 * multi * MGetFrequence())/(4 * MGetSpeed()+0.5 );
        return  ( Near);
    }
}
float MGetAttenuate(int depth)
{
    float att = 0;

    /*	if((MGetCrystal()/256)<15 && depth>1200)
    	{
    		att = 0.024;
    	}

    Write_Number(10,80,MGetCrystal()/256,8,0,0);
    Write_Number(10,90,depth,8,0,0);
    Write_Number(10,100,att,8,0,0);*/
    return att;
}

uint32_t MGetRange(int mode)			/*����ֵΪ��ǰ����µ����̷�Χ	*/
{
    if( ChannelPara.Range > C_MAX_RANGE)MSetRange(1000,C_SETMODE_SAVE);
    uint32_t retvalue = ChannelPara.Range ;	//
    if( mode == -1)
    {
        //չ��ǰ����
        retvalue = ChannelPara.OldRange ;
    }
    else if( mode == -2)
    {
        //չ��ǰ����
        retvalue = ChannelPara.Range ;
    }

    if(mode > 0)	//���ص�ǰ���������
    {
        if( mode == 3)return retvalue;	//���ؾ�������
        else if( mode == 4)return (retvalue * MGetAngle(3))/10000;		//����,��ֱ
        else if( mode == 5)return (retvalue * MGetAngle(2))/10000;		//����,ˮƽ
        switch ( MGetScaleMode() )
        {
        case 0:		//��ֱ
            retvalue = (retvalue * MGetAngle(3))/10000;		//����
            break;
        case 1:		//ˮƽ
            retvalue = (retvalue * MGetAngle(2))/10000;		//����
            break;
        case 2:		//����
            break;
        }
    }
    else if( mode == 0)
    {
        //�������̵�
        int offset =  C_OFF_TEMP+42;
        MCopyProtectedMemory( &retvalue, (void*)offset, C_SIZE_INT, PM_COPY_READ);
    }

    if( MGetUnitType() > 0)
    {
        //��λinch
///		retvalue = (uint32_t)( retvalue*100/25.4+0.5);
    }
    return retvalue;
}

void MSetRange(int Range,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if( mode == -1)
    {
        //չ��ǰ����
        ChannelPara.OldRange = Range;
        return;
    }
    else if( mode == -2)
    {
        ChannelPara.Range = Range;
        return;
    }
    switch ( MGetScaleMode() )
    {
    case 0:		//��ֱ
        Range = (uint32_t)( Range * 10000.0 / MGetAngle(3) + 0.5);		//����
        break;
    case 1:		//ˮƽ
        Range = (uint32_t)( Range * 10000.0 / MGetAngle(2) + 0.5);		//����
        break;
    case 2:		//����
        break;
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        if(Range >= C_MAX_RANGE) Range = C_MAX_RANGE;
        ChannelPara.Range = Range;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        uint32_t speed = MGetSpeed();

        uint32_t rm = C_MAX_RANGE*90/100;
        if( MGetSpeed() < CD_SPEED_2 * 110/100 && MGetProbeMode() == C_APROBE)rm /= 2;
        if(Range >= rm) Range = rm + (Range - rm)*60/100;
        Range = (C_SAMPLE_FREQ * Range + speed/2)/speed;
        if( MGetProbeMode() == C_P_TRANSMISSION)Range >>= 1;

        MSetEchoRange(Range);




        MSetColor(C_CR_WAVE);
        int offset =  C_OFF_TEMP+42;
//        int size = C_SIZE_INT;

        MCopyProtectedMemory( (void*)offset, &Range, C_SIZE_INT, PM_COPY_WRITE);

    }
}

uint32_t MGetDelay(int mode)			/*����ֵΪ��ǰͨ��	*/
{
    if( ChannelPara.Delay > C_MAX_DELAY ) MSetDelay(0,C_SETMODE_SAVE);
    uint32_t retvalue = ChannelPara.Delay ;	//��ʱʱ�䣬��λ1/80 000 ms

    if( mode == -1)return ChannelPara.OldDelay;//չ��ǰ��ʱ
    else if( mode == -2)return ChannelPara.Delay;
    if(mode != 0)	//���ص�ǰ�������ʱ����
    {
        retvalue = retvalue * MGetSpeed() /( C_SAMPLE_FREQ );	//��λΪmm����һλС��
        if(mode == 3)return retvalue;	//��ģʽ��ֱ�ӷ�����ʱ����
        switch ( MGetScaleMode() )
        {
        case 0:		//��ֱ
            retvalue = retvalue * MGetAngle(3)/10000;		//����
            break;
        case 1:		//ˮƽ
            retvalue = retvalue * MGetAngle(2)/10000;		//����
            break;
        case 2:		//����
            break;
        }
    }

    return retvalue;
}
void MSetDelay(int Delay,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if( mode == -1 )
    {
        //չ��ǰ��ʱ
        ChannelPara.OldDelay = Delay;
        return;
    }
    else if( mode == -2)
    {
        ChannelPara.Delay = Delay;
        return;
    }
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Delay = Delay;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemDelay();
    }
}
void MSetScaleDelay(int Delay,int mode/*mode = 0\1\2*/)	/*���õ�ǰ����µ���ʱ����	*/
{

    switch ( MGetScaleMode() )
    {
    case 0:		//��ֱ
        Delay = ( Delay * 10000 + MGetAngle(3)/2 ) / MGetAngle(3);		//����
        break;
    case 1:		//ˮƽ
        Delay = ( Delay * 10000 + MGetAngle(2)/2 ) / MGetAngle(2);		//����
        break;
    case 2:		//����
        break;
    }

    //if( Delay + MGetRange(3) > C_MAX_RANGE) Delay = C_MAX_RANGE - MGetRange(3);
    Delay = ( Delay * C_SAMPLE_FREQ + MGetSpeed()/2 )/(int)MGetSpeed();

    MSetDelay(Delay,mode);
}

uint16_t MGetReject(void)			/*����ֵΪ��ǰͨ��	*/
{
    if( ChannelPara.Reject < C_MAX_REJECT)return ChannelPara.Reject;
    else return C_MAX_REJECT;
}
void MSetReject(uint16_t Reject,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if( Reject > 800 )Reject = 800;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.Reject = Reject;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
#if C_DEVLIB == 4
        SetEchoReject(MGetChannel(), Reject);
#else
        SetEchoReject(Reject);
#endif
    }
}

uint16_t MGetSysGain(void)	//�õ���������
{
    if( ChannelPara.SurfGain + ChannelPara.BaseGain + ChannelPara.CompGain > C_MAX_SYSGAIN)
    {
        ChannelPara.SurfGain = ChannelPara.CompGain = 0;
        ChannelPara.BaseGain = 450;
    }
    return ( ChannelPara.SurfGain + ChannelPara.BaseGain + ChannelPara.CompGain );
}

uint16_t MGetSurfGain(void)			/*����ֵΪ��ǰͨ�����油������ һλС��	*/
{
    if (ChannelPara.SurfGain > C_MAX_SURFGAIN)ChannelPara.SurfGain = 0;
    return ChannelPara.SurfGain;
}

int MSetSurfGain(uint16_t SurfGain,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if (SurfGain > C_MAX_SURFGAIN)
    {
        ChannelPara.SurfGain = 0;
        return C_FALSE;
    }
    if ( (MGetBaseGain() + MGetCompGain() + SurfGain ) > C_MAX_BASEGAIN)return C_FALSE;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.SurfGain = SurfGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemGain();
    }
    return C_TRUE;
}

uint16_t MGetBaseGain(void)			/*����ֵΪ��ǰͨ���������� һλС��	*/
{
    return ( ( ChannelPara.BaseGain > C_MAX_BASEGAIN)? C_MAX_BASEGAIN : ChannelPara.BaseGain );
}

int MSetBaseGain(short BaseGain,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if( BaseGain < 0 )BaseGain = 0;
    if( BaseGain > C_MAX_BASEGAIN )BaseGain = C_MAX_BASEGAIN ;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.BaseGain = BaseGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        uint16_t gain = BaseGain + MGetSurfGain() + MGetCompGain() ;
        if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
//		if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//		if (gain > 900)gain = 900 + (gain - 900)/2;
//		if (gain > MAX_GAIN_DB)gain = MAX_GAIN_DB;
        MSetAmplifierGain(gain);
    }
    return C_TRUE;
}

short MGetGatedB(void)	/*����ֵΪ��ǰͨ��DAC�� һλС��*/
{
    return ChannelPara.gatedB;
}

void MSetGatedB(short gatedB, int mode/*mode = 0*/) //DAC��dB��
{
    ChannelPara.gatedB = gatedB;
}

short MGetCompGain(void)			/*����ֵΪ��ǰͨ���������� һλС��	*/
{
    return ( ( ChannelPara.CompGain > C_MAX_BASEGAIN)? C_MAX_BASEGAIN : ChannelPara.CompGain );
}

int MSetCompGain(short CompGain,int mode/*mode = 0\1\2*/)	/*���õ�ǰͨ��	*/
{
    if( CompGain > C_MAX_BASEGAIN)CompGain = C_MAX_BASEGAIN;
    if (mode == C_SETMODE_SAVE || mode == C_SETMODE_SETSAVE)
    {
        ChannelPara.CompGain = CompGain;
    }
    if (mode == C_SETMODE_SETSAVE || mode == C_SETMODE_SET)
    {
        MSetSystemGain();
        uint16_t gain = CompGain + MGetSurfGain() + MGetBaseGain();
        if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
//		if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//		if (gain > 900)gain = 900 + (gain - 900)/2;
//		if (gain > MAX_GAIN_DB)gain = MAX_GAIN_DB;
        MSetAmplifierGain(gain);
    }

    return 0;
}

uint16_t MGetDepthGain(void)
{
    return	ChannelPara.DepthGain;
}
void MSetDepthGain(uint16_t gain)//������DAC/AVGʱ��Ȳ���ֵ
{
    if( gain > 300 )gain = 300;
    ChannelPara.DepthGain = gain;
}

short MGetLineGain(int line)//�õ����ߵ�ƫ����
{
    line %= 6;
    return ChannelPara.lineGain[line]	;
}


void MSetLineGain(int line,short gain)//�������ߵ�ƫ����
{
    int i;
    line %= 6;
    ChannelPara.lineGain[line] = gain ;
    for( i = line+1; i < 6 ; i++)
    {
        if( ChannelPara.lineGain[i] > gain ) ChannelPara.lineGain[i] = gain;
    }
    for( i = line-1; i >= 0 ; i--)
    {
        if( ChannelPara.lineGain[i] < gain ) ChannelPara.lineGain[i] = gain;
    }
}

void MSetSystemGain(void)	/*���õ�ǰ���� */
{
    uint16_t gain = MGetBaseGain() + MGetSurfGain() + MGetCompGain() ;
//;	if (gain > 1100)gain = 1100 + (gain - 1100)/2;
//;	if (gain > 1000)gain = 1000 + (gain - 1000)/2;
//;	if (gain > C_MAX_BASEGAIN)gain = C_MAX_BASEGAIN;
    MSetAmplifierGain(gain);
}

void MSetSystemRange(void)	/*���õ�ǰ���� */
{
    /*SAMPLE_FREQ*1000*RANGE/SPEED */
    uint32_t range = MGetRange(3);
    uint32_t speed = MGetSpeed();
    uint32_t rm = C_MAX_RANGE*90/100;
    if( MGetSpeed() < CD_SPEED_2 * 110/100 && MGetProbeMode() == C_APROBE)rm /= 2;
    if(range >= rm) range = rm + (range - rm)*60/100;

    range = (C_SAMPLE_FREQ * range + speed/2)/speed;
    if( MGetProbeMode() == C_P_TRANSMISSION)range >>= 1;
    MSetEchoRange( range ) ;
    MSetColor(C_CR_WAVE);
}

void MSetSystemDelay(void)	/*���õ�ǰ��ʱ����� */
{
    uint32_t edelay = ( MGetDelay(0) + MGetOffset() ) /2;
    if( MGetProbeMode() == C_P_TRANSMISSION)edelay /= 2;
#if C_DEVLIB == 4
    SetEchoDelay(MGetChannel(), edelay ) ;
#else
    SetEchoDelay( edelay ) ;
#endif
}

void MSetSystemBright(void)	/*���õ�ǰ��Ļ����*/
{
    uint32_t cur_bright = 0,obj_bright = C_BRIGHTNESS_COM;
    int mode;
    cur_bright = 0;
    obj_bright = C_BRIGHTNESS_COM;
    mode = 0;
#if C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
    switch ( MGetScreenBrightness() )
    {
    case 0:
        obj_bright = C_BRIGHTNESS_MIN;
        break;
    case 1:
        obj_bright = C_BRIGHTNESS_COM;
        break;
    case 2:
        obj_bright = C_BRIGHTNESS_MID;
        break;
    case 3:
        obj_bright = C_BRIGHTNESS_MAX;
        break;
    }
#endif
#if C_DEVLIB == 1
    do
    {
        cur_bright = AdjustBrightness(0) ;/*�õ���ǰ����*/
        mode = obj_bright - cur_bright;
        AdjustBrightness(mode) ;
    }
    while( mode != 0 );
#elif C_DEVLIB == 3 || C_DEVLIB == 23
//Write_Number(10,30,1,4,0,0);

    SetLCDBrightness(obj_bright) ;
#elif C_DEVLIB == 24
    SetELBrightness(obj_bright) ;
#endif

}

int MSetSystemProbe(void)
{
    int Ax = MGetProbeMode() -0 ;
    int i;
    if (Ax < C_DPROBEZ)
    {
        Ax = T_T_SINGLE;
        for(i=0; i<=120; i++)GainTable[i] = c_gaintable[0][i]; //���Ի������

    }
    else
    {
        Ax = T_R_DUAL;
        for(i=0; i<=120; i++)GainTable[i] = c_gaintable[1][i]; //���Ի������

    }
#if C_DEVLIB == 4
    SetProbeMode( MGetChannel(), Ax) ;
#else
    SetProbeMode(Ax) ;
#endif


    return C_TRUE;
}


void MSetSystem(void)		/*���õ�ǰϵͳ��״̬*/
{

#if C_DEVLIB == 4
    SetEchoReject(MGetChannel(), MGetReject());
#else
    SetEchoReject(MGetReject());
#endif

#if C_DEVLIB != 3
#if C_DEVLIB == 4
    SetAmplifierBand( MGetChannel(), MGetBandMode() + AMP_BAND_NARROW_1);
#else
    SetAmplifierBand( MGetBandMode() + AMP_BAND_NARROW_1);
#endif
    MSetPulseMode( MGetPulseMode(), C_SETMODE_SET);
    //SetTransmitterVoltage( MGetPulseMode() + LOW_PULSE ) ;
#else
    if( MGetPulseMode() > 0)SetTransmitterVoltage(HIGH_PULSE) ;
    else SetTransmitterVoltage(LOW_PULSE) ;
#endif

#if C_DEVLIB == 4
    SetProbeDamping( MGetChannel(), MGetDampMode() + LOW_DAMPING) ;
#else
    MSetDampMode( MGetDampMode()  ,C_SETMODE_SET) ;
#endif

#if C_DEVLIB == 4
    SetEchoMode( MGetChannel(), MGetEchoMode() + FULL_WAVE ) ;
#elif C_DEVLIB == 3
    if( MGetEchoMode() == 0 )SetEchoMode( FULL_WAVE ) ;
    else SetEchoMode( RF_WAVE ) ;
#else
    SetEchoMode( MGetEchoMode() + FULL_WAVE ) ;
#if C_DEVLIB == 23 || C_DEVLIB == 24
//    if( MGetEchoMode()%4 == 3)SetEchoBaseLine( c_RFBaseLine);
//    else SetEchoBaseLine( c_FWBaseLine);
#endif
#endif

    MSetGatePara( MGetGatePara(1,0),MGetGatePara(1,1),MGetGatePara(1,2),1,C_SETMODE_SETSAVE);
    MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),MGetGatePara(0,2),0,C_SETMODE_SETSAVE);

    MSetSystemGain();
    MSetSystemRange();
    MSetSystemDelay();
    MSetSystemBright();
    MSetSystemProbe();
}


/*

*/


