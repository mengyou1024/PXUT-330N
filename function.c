//�빦���йصĺ���


int GateParaMax,GateParaValue;
int SearchPeak(u_char * sampbuff,u_int curr_pos,u_int direction)	//������߲�����ˮƽ��������Ϊ������Ϊ����߲�
{
    //Ѱ��sampbuffָ���δ洢������߲���curr_posΪ��ǰλ��,directionΪ����:C_KEYCOD_LEFT��C_KEYCOD_RIGHT��
    u_char preamp = *(sampbuff+curr_pos);	//ǰһ����ֵ
    u_char amp = *(sampbuff+curr_pos);	//��ǰ����ֵ

    u_short search_time = 0;	//Ѱ�Ҵ�����������1
    short retvalue = 0;

    if(curr_pos <= 5 )
    {
        curr_pos = C_COORWIDTH-1;
    }
    if(direction == C_KEYCOD_RIGHT)	//����
    {
        while(search_time <= 1)
        {
            preamp = amp;
            curr_pos++;
            if(curr_pos >= C_COORWIDTH )
            {
                curr_pos = 14;
                search_time ++;
            }
            amp = *(sampbuff+curr_pos);	//��ǰ����ֵ
            if( amp <= 5 || amp >= 200) continue;
            if( amp <= preamp)continue;
            if( amp < *(sampbuff+curr_pos+1) )continue;
            if( amp < *(sampbuff+curr_pos+2) )continue;
            retvalue = curr_pos;
            break;
        }
    }
    else if(direction == C_KEYCOD_LEFT)	//����
    {
        while(search_time <= 1)
        {
            preamp = amp;
            curr_pos--;
            if(curr_pos <= 5 )
            {
                curr_pos = C_COORWIDTH-1;
                search_time ++;
            }
            amp = *(sampbuff+curr_pos);	//��ǰ����ֵ
            if( amp <= 5 || amp >= 200) continue;
            if( amp <= preamp)continue;
            if( amp < *(sampbuff+curr_pos-1) )continue;
            if( amp < *(sampbuff+curr_pos-2) )continue;
            retvalue = curr_pos;
            break;
        }
    }
    if(retvalue >= 5)
    {
        unsigned short dist_dire = (int)( (double)(curr_pos * MGetRange(3)+MGetRange(3)/2) / (double)C_COORWIDTH)+MGetDelay(3);
        unsigned short dist_hori = dist_dire * MGetAngle(2) / 10000;
        unsigned short dist_vert = dist_dire * MGetAngle(3) / 10000;
        short db;

        db = Amp2db( curr_pos,amp );//-MGetSurfGain() ;
        switch( MGetEquivalentMode() )	//mode1=0B��ʧ��1������mode2<0����.>0dB��
        {
        case 1:
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
			if( MGetProbeMode()==0 )
			{
				MSetColor(C_CR_MENU);
				TextOut(340, 28,1,10,20,"��",0);
				MSetColor(C_CR_PARA);
				if( MGetUnitType() > 0 )
				{
					Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)*100/25.4  + 0.5), 5,3,0);
				}
				else
				{
					Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-VertOffsetScreen-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)  + 0.5), 3,1,0);
				}
			}
            db -=  MGetLineGain( MGetEquivalentMode()-2 );
            break;
        case 10:
        case 11://��ʾ�׾�
            if( MGetUnitType() > 0 )
            {
				Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)*100/25.4  + 0.5), 5,3,0);
            }
            else
            {
				Write_Number(23*C_ECHAR_HDOT,C_COORVPOSI - C_ECHAR_VDOT-8,(int)( 10*powf( 10,( db + 120 ) / 400.0)  + 0.5), 3,1,0);
            }
            //	Write_Number(23*8,C_COORVPOSI - 10,(int)( 10*powf( 10,( db + 120 ) / 400.0)  + 0.5), 3,1,0);
            db -= MGetLineGain( 2 );
            break;
        default:
            db = amp * 5;
            break;
        }
        if (dist_hori >= MGetForward())
        {
            dist_hori -= MGetForward();
        }
        if( MGetThick() > 0 && MGetDiameter() == 0 && MGetSystemMode() == 0)
        {
            if( (dist_vert / MGetThick() ) % 2 == 0) dist_vert = dist_vert % MGetThick();
            else dist_vert = MGetThick() - (dist_vert % MGetThick() );
        }
        WritePeakInfo(0,MGetEquivalentMode(),dist_dire,dist_hori,dist_vert,db);
    }
    return retvalue;
}

int g_iMaxAmp;
int g_iPos;
u_char sampbuffs[C_LEN_SAMP+1];
u_int PeakMemory(void)		//��ֵ����
{
    u_int gatetype = 0 ;
    u_int retvalue ;
    u_char sampbuff[C_LEN_SAMP+1] ;
    u_char *samp ;
    u_int  offset = C_OFF_SAMP ;
//	u_int left = 0,right= 0 ;
    int i;

    if(MGetFunctionMode(C_ECHO_ENVELOPE) || MGetFunctionMode(C_ECHOMAX_MEMORY))
    {
        GateParaMax =1;
    }
    else
        GateParaMax =0;

    if( !MGetFunctionMode(C_ECHOMAX_MEMORY) )return 0;
    retvalue = MGetGateParaMax(gatetype);
    MSetColor(C_CR_ENVELOPE);
    if(retvalue == 1)
    {
        offset = C_OFF_PEAK;
        MCopyProtectedMemory( (void*)offset, &GatePeak, sizeof(GATE_PEAK), PM_COPY_WRITE);//�Ų���
        offset = C_OFF_SAMP ;
        MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
        DrawWave(sampbuff,0,C_LEN_SAMP);

        samp = GetSampleBuffer();
        for (i=0; i<C_COORWIDTH; i++)
        {
       //     Write_Number(10+16*4*(i%10),10+24*(i/10),samp[i],4,0,0);
            if (samp[i]>=200)
            {
                samp[i]=200;
            }
        }
         //   Write_Number(10,10,2,4,0,0);
        //    MAnyKeyReturn();
/*#if C_DEVTYPE != 3 && C_DEVTYPE != 9 && C_DEVTYPE != 11

        if( MGetSystemMode() )
#endif
        {
            int left,right,mid;
            for(i = 0; i < C_LEN_SAMP; i++)	sampbuff[i] = 0;
            left = GateMax.Pos;
            right = GateMax.Pos;
            mid = right;

            while(samp[left - 1] <= samp[left] && samp[left] > 0) left-- ;
            while(samp[right] >= samp[right+1] && samp[right] > 0) right++ ;

//			for( i = left; i <= right; i++)
            {
//				sampbuff[i] = samp[i];
            }
            for( i = left; i <= mid; i++)
            {
                sampbuff[i-1] = samp[i]+1;
            }
            sampbuff[mid] = samp[mid]+2;
            for( i = mid; i <= right; i++)
            {
                sampbuff[i+1] = samp[i]+1;
            }
        }*/
#if C_DEVTYPE != 3 && C_DEVTYPE != 9 && C_DEVTYPE != 11

        //else
        {
            for(i = 0; i < C_LEN_SAMP; i++)	sampbuff[i] = samp[i];
        }
#endif

        MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);
		memset(sampbuffs, 0, sizeof(sampbuffs));
		for(i = 0; i < C_LEN_SAMP; i++)
			sampbuffs[i] = sampbuff[i];
        DrawWave(sampbuff,0,C_LEN_SAMP);

		g_iMaxAmp=0;
		g_iPos=0;
		for(i=MGetGatePara(0,0); i<(MGetGatePara(0,0)+MGetGatePara(0,1)); i++)
        {
            if(sampbuff[i]>=g_iMaxAmp)
            {
                g_iMaxAmp=sampbuff[i];
                g_iPos=i;
            }
        }
    }
    MSetColor(C_CR_UNDO);

    GateParaValue=retvalue;
    return retvalue;
}

int DepthCompensate(void)		//��Ȳ���
{
    /*	if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 0 ) return -1;
    	if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0 )return 0;

    	int pos = MGetAmpPos(0) ;

    	if( MGetAmpMax(0) < 5 )
    	{
    		pos =  MGetGatePara(0,0) + MGetGatePara(0,1)/2 ;	//��A���ڲ�̫��,��A�����м����Ϊ������
    	}
    	MSetBaseGain( MGetBaseGain() + DACDB[pos/2] - DACDB[0] , C_SETMODE_SET );
    */
    return 1;
}

int EchoEnvelope(void)	//�ز�����12:02
{
    u_int gatetype = 0 ;
    u_int retvalue ;
    u_char sampbuff[C_LEN_SAMP+1] ;
    //u_char *samp ;
    int  offset;
    int high;
    int pos;	//�����
    int start,end;	//���Ŀ�ʼ��
    int dot;	//���ĵ���
    int k;	//б��
    int i,j;
    //u_char preamp;
    u_char amp;

    if(MGetFunctionMode(C_ECHO_ENVELOPE) || MGetFunctionMode(C_ECHOMAX_MEMORY))
    {
        GateParaMax =1;
    }
    else
        GateParaMax =0;
    if( MGetFunctionMode(C_ECHO_ENVELOPE) != 1 || MGetAcquisitionEnable() != 1 )return -1;
    ///if( MGetSaveMode()==1 && MGetSaveStatus()==1 )return -1;

    pos = MGetAmpPos(gatetype);	/*�õ�ָ���ţ�A or B)�ڻز�ˮƽ������*/
    amp = MGetAmpMax(gatetype);

    MSetColor(C_CR_ENVELOPE);
    offset = C_OFF_SAMP;
    MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
    ExpendTime(10);///
    if(amp > sampbuff[pos] && amp > 20)
    {
        retvalue=MGetGateParaMax(gatetype);
        if(retvalue== C_TRUE)
        {
            offset = C_OFF_PEAK;
            MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//�Ų���
        }

        GateParaValue=retvalue;
        DrawWave(sampbuff,0,C_LEN_SAMP);

        sampbuff[pos] = amp;
        //&& sampbuff[start] > 0
        start = MGetGatePara(0, 0);
        end = start + MGetGatePara(0, 1);
        if( end >= C_COORWIDTH-1)end = C_COORWIDTH - 2;

        if( amp > sampbuff[pos-1] )
        {
            for( i = pos - 1, dot = pos , j = 0; i >= start && j < 4 ; i--, j++ )   // ����ֵΪ�Ų���
            {
                high = sampbuff[i];
                if( high <= 1 )
                {
                    if( dot == pos)dot = i;
                    continue;
                }
                //��һ����ֵ
                if( high >= sampbuff[i-1] && high > sampbuff[i+1] )break;
                if( high > sampbuff[i-1] && high >= sampbuff[i+1] )break;
                if( high > amp)break;
            }
            if( i > start)dot = i;//�ҵ�һ����ֵ
            //else if( i == pos - 1)dot--;//����ȫΪ0
            high = sampbuff[dot];
            k = 1000* ( amp - high )/(pos -dot) ;

            for( i = 1; dot + i < pos; i++)
            {
                sampbuff[dot + i] = (u_char)( high + ( i * k + 0)/1000 );
            }
        }

        if( amp > sampbuff[pos+1] )
        {
            for( i = pos + 1,dot = pos, j = 0; i <= end && j < 4; i++,j++ )
            {
                high = sampbuff[i];
                if( high <= 1 )
                {
                    if( dot == pos)dot = i;
                    continue;
                }
                //��һ����ֵ
                if( high > sampbuff[i-1] && high >= sampbuff[i+1] ) break;
                if( high >= sampbuff[i-1] && high > sampbuff[i+1] ) break;
                if( high > amp)break;
            }
            if( i < end)dot = i;//�ҵ�һ����ֵ

            k = 1000 * ( sampbuff[dot] - amp  )/(dot - pos) ;

            for( i = 1; pos + i < dot; i++)
            {
                sampbuff[pos + i] = (u_char) ( amp + ( i * k + 0)/1000 );
            }
        }


        offset = C_OFF_SAMP ;
        MCopyProtectedMemory( (void*)offset, sampbuff , end, PM_COPY_WRITE);

        DrawWave(sampbuff,0,C_LEN_SAMP);


        MSetColor(C_CR_UNDO);
        return 1;
    }
    MSetColor(C_CR_UNDO);
    return 0;
}
void ClearEnvelope(void)		//���������ֵ
{
    int offset = C_OFF_SAMP;
    u_char sampbuff[C_LEN_SAMP+1];
    int i,start;


    GateMax.Amp = 0;	//��߲��ó�ֵΪ0
    MSetColor(C_CR_ENVELOPE);
//	start = 0;
//	dots = C_LEN_SAMP;
    MCopyProtectedMemory( sampbuff, (void*)offset, C_LEN_SAMP, PM_COPY_READ);
    for( start = 0 ; start <= C_LEN_SAMP; start++)
        if( sampbuff[start] > 0)break;
    if(start < C_LEN_SAMP )
    {
        //�а�����ֵ����ʱ������Ļ
//		start--;
//		dots = C_LEN_SAMP - start;
        if( MGetFunctionMode(C_ECHO_ENVELOPE) )//DrawEnvelope(sampbuff,0,C_LEN_SAMP);
            DrawWave(sampbuff,0,C_LEN_SAMP);		//���������ֵ
        else if( MGetFunctionMode(C_ECHOMAX_MEMORY) ) DrawWave(sampbuff,0,C_LEN_SAMP);		//���������ֵ
//		DrawWave(sampbuff,start,dots);		//���������ֵ
    }
    for( i = 0 ; i <= C_LEN_SAMP; i++)sampbuff[i] = 0;
    MSetColor(C_CR_UNDO);
    MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE);
     ClearCursor(2);
}

int GateWaveExpend(int mode/*=0չ��1����*/)		//����չ��
{
    int range = MGetRange(-2);//��ǰ�����ֵ
    int delay = MGetDelay(-2);
    int gateposi ;
    int gatewide ;
    if( mode == 0)
    {
        //չ��
        gateposi = MGetGatePara( 0,0);
        gatewide = MGetGatePara( 0,1);
        MSetRange(range,-1);
        MSetDelay(delay,-1);
        delay = delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6);
        range = range * gatewide/C_COORWIDTH;
        MSetRange( range, -2);
        MSetDelay( delay, -2);
        MSetGatePara( 0, C_COORWIDTH, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
    }
    else if( mode == 1)
    {
        gatewide = range * C_COORWIDTH/MGetRange(-1);
        range = MGetRange(-1);
        gateposi = (int)( (float)(delay - MGetDelay(-1) )* (float)MGetSpeed()/(float)( C_SAMPLE_FREQ * range) * (float)C_COORWIDTH + 0.6);
        delay = MGetDelay(-1);
        MSetRange( range, -2);
        MSetDelay( delay, -2);
        MSetGatePara( gateposi, gatewide, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
    }
    return 0;
}
int MSetCurveAmend(CURVE_AMEND *stCurveAmend)//������ȷ�ɼ��㷵��1�����򷵻�0��
{
    double alpha, beta;
    double R = (double)stCurveAmend->Diameter /20.0;//����������ֱ��,�Ҵ�һλС��
    double thick = (double)stCurveAmend->Thick/10.0;
    double r = R - thick;
    double angle = (double)stCurveAmend->Angle/10.0;
    double half_len_bow, half_len_arc;
    /*
    Write_Number(70,30,stCurveAmend->Diameter,5,0,0);
    Write_Number(120,30,stCurveAmend->Thick,5,0,0);
    Write_Number(170,30,10*r,5,0,0);
    Write_Number(220,30,stCurveAmend->Angle,5,0,0);
    MAnyKeyReturn();
    //*/
    if( r < 0  )
    {
        half_len_bow  = 0;
        half_len_arc = 0;
        beta = 0;
    }
    else
    {
        alpha = (double)angle * C_PI/180.0;//ת��Ϊ���ȣ�
		if( R * sin(alpha) < r )
        {
            //�ز����ڱ�
            	beta = acos( R * sin(alpha) / r );
			half_len_bow = R * cos(alpha) -  r * sin(beta) ;//���ҳ�
            beta = C_PI/2 - alpha - beta;
        }
        else
        {
			half_len_bow = R * cos(alpha) ;//���ҳ�
            //		beta = acos( (R * R + r * r - half_len_bow * half_len_bow )/( 2 * R * r) ) ;
            beta = C_PI/2 - alpha ;
        }
        half_len_arc = beta * R ;//beta�ǻ��ȣ���2*Pi*R*(angle/360)=2*Pi*R*(beta/2*Pi);
    }
    stCurveAmend->Beta = (u_short)( beta*10000+0.5);//���ҳ���Ӧ�ĽǶ�
    stCurveAmend->Half_Len_Bow = ( u_short)(half_len_bow*10+0.5);//���ҳ�
    stCurveAmend->Half_len_Arc = ( u_short)(half_len_arc*10+0.5);//���ҳ���Ӧ�Ļ���
    /*
    Write_Number(20,40,beta*10000,5,4,0);
    Write_Number(100,40,half_len_bow*10,5,1,0);
    Write_Number(150,40,half_len_arc*10,5,1,0);
    MAnyKeyReturn();
    //*/
    return 1;
}
int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend)//���ؾ��������ˮƽ�ʹ�ֱ
{
    double beta = (double)stCurveAmend->Beta /10000.0;//��Ϊ���ȣ�
    double R = (double)stCurveAmend->Diameter /20.0;//����������ֱ��,�Ҵ�һλС��
    double thick = (double)stCurveAmend->Thick/10.0;
//	double r = R - thick;
    double angle = (double)stCurveAmend->Angle * C_PI/1800.0;//ת��Ϊ���ȣ�
    double half_len_bow = (double)stCurveAmend->Half_Len_Bow /10.0;//���ҳ�
//	double half_len_arc = (double)stCurveAmend->Half_len_Arc / 10.0;//�뻡��
    double temp_dist;//,temp_hori,temp_vert;
    double arc, alpha, bow_hori,bow_vert = 0;
    int i;

		temp_dist = (double)(*dist)/10.0 ;//ת��Ϊ������
		double a , b , c , angB ;

		a = R ;
		b = temp_dist ;
		c = sqrt( pow( fabs( temp_dist - (long)a * MGetAngle(3) / 10000 ) , 2 ) + pow( (long)a * MGetAngle(2) / 10000 , 2 ) ) ;
		angB = acos((a*a+c*c-b*b)/(2*a*c));

		bow_vert = a - c ;//��̤��
		// if(bow_vert > thick)
		// {
			 // bow_vert = 2*thick - bow_vert;
		// }
		 // while(bow_vert > 2*thick && thick > 0)
        // {
            // bow_vert -= 2*thick;
        // }
		arc = a * angB;//����

   
    *hori = (int)( arc * 10 + 0.5);
    *vert = (int)( bow_vert * 10+0.5);
    return (int)(alpha*10+0.5);

} 

/* int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend)//���ؾ��������ˮƽ�ʹ�ֱ
{
    double beta = (double)stCurveAmend->Beta /10000.0;//��Ϊ���ȣ�
    double R = (double)stCurveAmend->Diameter /20.0;//����������ֱ��,�Ҵ�һλС��
    double thick = (double)stCurveAmend->Thick/10.0;
//	double r = R - thick;
    double angle = (double)stCurveAmend->Angle * C_PI/1800.0;//ת��Ϊ���ȣ�
    double half_len_bow = (double)stCurveAmend->Half_Len_Bow /10.0;//���ҳ�
//	double half_len_arc = (double)stCurveAmend->Half_len_Arc / 10.0;//�뻡��
    double temp_dist;//,temp_hori,temp_vert;
    double arc, alpha, bow_hori,bow_vert = 0;
    int i;

    temp_dist = (double)(*dist)/10.0 ;//ת��Ϊ������
    arc = 0;
    alpha = 0;
    i = 0;

    if( half_len_bow < 1)
    {
        //���ҳ�Ϊ0
        arc = temp_dist * MGetAngle(2) / 10000;	//ˮƽ
        bow_vert = temp_dist * MGetAngle(3) / 10000;	//��ֱ
        while(bow_vert > 2*thick && thick > 0)
        {
            bow_vert -= 2*thick;
        }
        if( bow_vert > thick)
        {
            bow_vert = 2*thick - bow_vert;
        }
        alpha = 0;
    }
    else
    {
        for(i = 0; i < 100 ; i++)
        {
            if( temp_dist > half_len_bow)
            {
                //������ڰ��ҳ�
                //	arc += half_len_arc;
                alpha += beta;
                temp_dist -= half_len_bow;
//	Write_Number(20,60,alpha*10,5,0,0);
//	Write_Number(70,60,temp_dist*10,5,0,0);
                continue;
            }
            else
            {
                if( i % 2 == 0)
                {
                    //����������
                    bow_hori = temp_dist;
					bow_vert = sqrt( R*R + bow_hori*bow_hori - 2*R*bow_hori*cos(angle) );
                    //alpha += acos(  (R*R + bow_vert*bow_vert - bow_hori*bow_hori)/(2*R*bow_vert) );
                    	alpha += asin(bow_hori * sin(angle)/bow_vert);
                }
                else
                {
                    //��ż������
                    bow_hori = half_len_bow - temp_dist;
					bow_vert = sqrt( R*R + bow_hori*bow_hori - 2*R*bow_hori*cos(angle) );
                    alpha += beta;
                    //alpha -= acos(  (R*R + bow_vert*bow_vert - bow_hori*bow_hori)/(2*R*bow_vert) );
                    	alpha -= asin(bow_hori * sin(angle)/bow_vert);
                }

                bow_vert = R - bow_vert;
                arc = alpha * R;
                break;
            }
        }
    }
    *hori = (int)( arc * 10 + 0.5);
    *vert = (int)( bow_vert * 10+0.5);
    return (int)(alpha*10+0.5);

}

 */

