/*menu.c*/
#include <string.h>
#include <stdarg.h>
#include "Media.h"
#include "ff.h"
//#include "ding.c"
//#include "Music_alice.c"
//#include "Music_lee.c"
//#include "Music_byebye.c"

extern GATE_MAX				GateMax;		//门内出现的最高波
extern FUNCTION             Function;   //功能
extern CACHE	cache;
extern int signFlag ;

extern int g_iMaxAmp;
extern int g_iPos;
extern uint8_t sampbuffs[C_LEN_SAMP+1];

int NumHigh;
int KeyManage(int keycode,int mode)		//按键管理,mode=1renovate
{


    uint32_t SameKeyTime = 0;
    int mode1;			//mode1 =1长按键0短按
    int xpos,ypos;
    uint8_t* sampbuffer ;
    char funcNumber=10;
    int prestatus=0;
    /*
        Function.funcMenu[0]=8;
        Function.funcMenu[1]=5;
        Function.funcMenu[2]=6;
        Function.funcMenu[3]=7;
    */

    xpos = 2;

    if(keycode != C_KEYCODMAX)
    {
        SameKeyTime = TestSameKeyTime(keycode,GetElapsedTime(),C_SAMEKEYTIME);
        //   Write_Number(10,50,SameKeyTime,5,0,0);
    }
    else if( MGetSystemMode() == 0 )
    {
#if( C_ROTARY > 0 )
        keycode = MGetRotary(0);//左旋
        if( keycode > 0)
        {
            CompGainPlus(10);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        }
        else if( keycode < 0)
        {
            CompGainMinus(10);
            WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        }
        else
        {
            keycode = MGetRotary(1);//右旋
            if( keycode > 0)
            {
                //	MGatePosi(C_KEYCOD_PLUS,0,keycode);
            }
            else if( keycode < 0)
            {
                //	MGatePosi(C_KEYCOD_MINUS,0,-1*keycode);
            }
        }
        return -1;
#endif
        SameKeyTime = 0;
    }
    //同一按键时间
    if(SameKeyTime >= C_SAMEKEYTIME)mode1 = 1;	//mode1 =1长按键0短按
    else mode1 = 0;

    if( keycode != C_KEYCODMAX && MGetKeyLock() > 0 && !(keycode == C_KEYCOD_RETURN && mode1 == 1) )
    {
        int tm1;
        tm1 = GetElapsedTime() + 1000 *2;
        //	MPushWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
        //	MEraseWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
        if (MGetLanguage())
        {
            EMenuOut( 0, 4, _KEYLOCK_A3_E,16,1,12);
        }
        else
        {
            CEMenuOut( 0, 0,_KEYLOCK_A3,6,1,12);
        }
        while( GetElapsedTime() < tm1 )
        {
        }
        DisplayPrompt(16);
        //	MPopWindow();
        return -1;//键盘锁返回-1
    }


//;	MSetColor(C_CR_WAVE);
    ///MParaRenovate(0);

//	SystemParaStore();
//	ChannelParaStore();

    switch (keycode)
    {
    case C_KEYCOD_CHANNEL:
        if( MGetSystemMode() != 1)
        {
        	int xpos,ypos;
			int keycode,rotary;
			int number,deci_len;
        	uint16_t Channel, cmax;
			
			xpos = C_CHANNEL_HPOSI;
			ypos = C_CHANNEL_VPOSI;
			if(ChannelMax>10)
				cmax=1;
			else
				cmax=0;
			
            if( mode1 == 1)
			{
				number=0;
				deci_len=0;
				Channel=MGetChannel();
				MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30) ;
                MEraseWindow(C_CHANNEL_HPOSI-8,C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30);
                while(1)
                {
                    if( Input_Number(xpos-8,ypos+4,&number,2,&deci_len,0) != 1)
                    {
                        number=Channel;
                        break;
                    }
                    else
                    {
                        if( number < ChannelMax)
                        {
                            break;
                        }
                        else
                        	DisplayPrompt(4);
                    }
                }
				Channel = number;
                MSetChannel( Channel,C_SETMODE_SETSAVE);
                g_UDiskInfo.DataHeaderMark = 1;
                SystemParaStore();
                ChannelParaRestore();
                MSetAcquisitionEnable(1,C_SETMODE_SET);
                MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) );
                MSetSystem();
                ScreenRenovate();	//屏幕刷新
                MSaveSystemElapsedTime( GetElapsedTime() / (1000*60), C_TIME_MINUTE );
                DrawDac(0);
                ClearEnvelope();
                //MInvertWindow(C_CHANNEL_HPOSI-8, C_CHANNEL_VPOSI+1, C_CHANNEL_HPOSI+C_CCHAR_HDOT-8+cmax*C_ECHAR_HDOT, C_CHANNEL_VPOSI+30);
            }
            else
            {
				keycode = ChannelChange();
			}
        }
		keycode = -1;
        break;
    case C_KEYCOD_BASEGAIN:
#ifndef C_KEYCOD_AUTOGAIN
        if( (MGetAmpMax(0) > 150 && MGetAmpMax(0) < 160) || (MGetAmpMax(1) > 150 && MGetAmpMax(1) < 160) )
            keycode = GainChange();
        else if( MGetSystemMode() == 0 )		//波高在80-160即40-80%之间，不进行自动增益
        {
            //if( MGetFunctionMode(C_AUTOADJUST_GAIN) == 1 || mode1 == 1)
            if( mode1 == 1 )
            {
#if C_DEVTYPE!=20
                DisplayPrompt(18);
                DrawDac(1);
                MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
                DrawDac(0);
                DisplayPrompt(16);
                ClearEnvelope();	//清包络或峰值
                keycode = -1;
#endif
            }
            else
                keycode = GainChange();
        }
        else 		//波高在80-160即40-80%之间，不进行自动增益
            keycode = GainChange();
#else
        if( MGetSystemMode() == 0&&mode1 == 1 )		///长按输入数据
        {

            MEraseWindow(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI,C_BASEGAIN_HPOSI+4*16,C_BASEGAIN_VPOSI+24);
            InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI,1);
            int keycode1;
            int gain1=MGetBaseGain();
            int gain2=MGetCompGain();
            int number1=0;
            int dec_len1=1;
            int iflag=0;
            while(true)
            {
                if(Input_Gain(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI,&number1,3,&dec_len1,1,&iflag))
                {

                    //Write_Number(100, 200,signFlag,1,0,0);
                    //Write_Number(100, 240,number1,4,1,0);
                    //Write_Number(100, 280,iflag,1,0,0);
                    //ExpendTime(100);

                    if(iflag==1)//-号
                    {
                        if(abs(number1)>gain1)
                        {
                            DisplayPrompt(4);
                            MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            MSetBaseGain(gain1-abs(number1),C_SETMODE_SETSAVE);
                            break;
                        }
                    }
                    else if(iflag==2)//+
                    {
                        if((number1+gain1+gain2)>C_MAX_BASEGAIN)
                        {
                            DisplayPrompt(4);
                            MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                        }

                        else
                        {
                            MSetBaseGain(number1+gain1,C_SETMODE_SETSAVE);
                            break;
                        }
                    }
                    else if(iflag==0)
                    {
                        if((number1+gain2)>C_MAX_BASEGAIN)
                        {
                            DisplayPrompt(4);
                            MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                        }

                        else
                        {
                            MSetBaseGain(number1,C_SETMODE_SETSAVE);
                            break;
                        }
                    }
                }
                else
                {
                	MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                	break;
                }
            }
            /*while(1)
            {
                keycode=MGetKeyCode(0);
                //InvertBytes(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI,1);
                if(keycode==C_KEYCOD_PLUS)
                {

                    TextOut(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI-2,1,1,24,"+",4);
                    while(1)
                    {
                        if(Input_Number(C_BASEGAIN_HPOSI+16, C_BASEGAIN_VPOSI,&number1,3,&dec_len1,0))
                        {
                            if(number1+gain1+gain2>C_MAX_BASEGAIN)
                            {
                                DisplayPrompt(4);
                                MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                            }

                            else
                            {
                                MSetBaseGain(number1+gain1,C_SETMODE_SETSAVE);
                                break;
                            }

                        }
                        else
                        {
                            MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                            break;
                        }
                    }
                    break;
                }
                else if(keycode==C_KEYCOD_MINUS)
                {

                    TextOut(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI-2,1,1,24,"-",4);
                    while(1)
                    {
                        if(Input_Number(C_BASEGAIN_HPOSI+16, C_BASEGAIN_VPOSI,&number1,3,&dec_len1,0))
                        {
                            if(number1>gain1)
                            {
                                DisplayPrompt(4);
                                MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                            }

                            else
                            {
                                MSetBaseGain(gain1-number1,C_SETMODE_SETSAVE);
                                break;
                            }

                        }
                        else
                        {
                            MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                            break;
                        }
                    }
                    break;
                }
                else if(keycode>=1&&keycode<=9)
                {

                    Write_Number(C_BASEGAIN_HPOSI, C_BASEGAIN_VPOSI,keycode,1,0,0);
                    while(1)
                    {


                        if(Input_Number(C_BASEGAIN_HPOSI+16, C_BASEGAIN_VPOSI,&number1,2,&dec_len1,0))
                        {
                            if(number1==0)
                            {
                                MSetBaseGain(keycode*100,C_SETMODE_SETSAVE);
                                WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                break;
                            }
                            else if(number1>=100)
                            {
                                if(keycode*1000+number1+gain2>C_MAX_BASEGAIN)
                                {
                                    DisplayPrompt(4);
                                    MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                }
                                else
                                {
                                    MSetBaseGain(keycode*1000+number1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                    break;
                                }
                            }
                            else
                            {
                                if(keycode*100+number1+gain2>C_MAX_BASEGAIN)
                                {
                                    DisplayPrompt(4);
                                    MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                }
                                else
                                {
                                    MSetBaseGain(keycode*100+number1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                    break;
                                }
                            }

                        }

                        if(Input_Gain(C_BASEGAIN_HPOSI+16, C_BASEGAIN_VPOSI,&number1,2,&dec_len1,0,flag))
                        {

                            else if(number1==0)
                            {
                                MSetBaseGain(keycode*100,C_SETMODE_SETSAVE);
                                WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                break;
                            }
                            else if(number1>=100)
                            {
                                if(keycode*1000+number1+gain2>C_MAX_BASEGAIN)
                                {
                                    DisplayPrompt(4);
                                    MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                }
                                else
                                {
                                    MSetBaseGain(keycode*1000+number1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                    break;
                                }
                            }
                            else
                            {
                                if(keycode*100+number1*10+gain2>C_MAX_BASEGAIN)
                                {
                                    DisplayPrompt(4);
                                    MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                }
                                else
                                {
                                    MSetBaseGain(keycode*100+number1*10,C_SETMODE_SETSAVE);
                                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                    break;
                                }
                            }

                        }

                        else
                        {

                            if(keycode*10+gain2>C_MAX_BASEGAIN)
                            {
                                DisplayPrompt(4);
                                MSetBaseGain(gain1,C_SETMODE_SETSAVE);
                                WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                            }
                            else

                            {
                                MSetBaseGain(keycode*10,C_SETMODE_SETSAVE);
                                WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                                break;
                            }


                        }

                    }
                    break;
                }
                else if(keycode==C_KEYCOD_RETURN)
                {
                    MSetBaseGain(MGetBaseGain(),C_SETMODE_SETSAVE);
                    WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
                    break;
                }
            }*/


            MChannelRenovate();
            DisplayPrompt(16);///探伤中
            DrawDac(0);
            keycode = -1;
        }
        else keycode = GainChange();
#endif
        break;
#ifdef C_KEYCOD_AUTOGAIN
    case C_KEYCOD_AUTOGAIN:
        if( mode1 == 1)
        {

            int NumHigh=80;
            int declen=0;
            MEraseWindow(0,0,24*7+16*4,4+24);
            TextOut(0,0,1,18,24,"请输入标准波高(%):",0);

            if(Input_Number(24*7+16*4,4,&NumHigh,2,&declen,0))
                MSetAmpStdMax(NumHigh);
            ChannelParaStore();
            ScreenRenovate();
            DrawDac(0);
        }
        else
        {
            DisplayPrompt(18);
            DrawDac(1);
            MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
            if(MGetStdMode()==10||MGetStdMode()==11)
            {
                MChannelRenovate();
                g_iNum=0;
            }
            DrawDac(0);
            DisplayPrompt(16);
            ClearEnvelope();	//清包络或峰值
        }

        keycode = -1;
        break;
#endif
    case C_KEYCOD_OFFSET:
#if C_DEVTYPE == 3 || C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 8  || C_DEVTYPE == 9   || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == TestMenu();
            keycode = -1;
        }
        else keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 2 || C_DEVTYPE == 10
        if( mode1 == 1)
        {
            keycode == RejectChange();
            keycode = -1;
        }
        else keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 14
        keycode = OffsetChange();
        break;
#elif C_DEVTYPE == 15 || C_DEVTYPE == 1
//
        keycode = TestChoice(C_TEST_OFFSET);
        break;
#endif

#if C_DEVTYPE == 2 || C_DEVTYPE == 10  || C_DEVTYPE == 14
    case C_KEYCOD_TEST:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        keycode == TestMenu();
        keycode = -1;
        break;
#endif
    case C_KEYCOD_SPEED:
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == STDMenu();
            keycode = -1;
        }
        else keycode = SpeedChange();
        break;
    case C_KEYCOD_RANGE:
        if( MGetSystemMode() == 0&&mode1 == 1 )		///非测试模式下 长按输入数据
        {
            MEraseWindow(C_RANGE_HPOSI, C_RANGE_VPOSI,C_RANGE_HPOSI+4*16,C_RANGE_VPOSI+24);
            int range1=MGetRange(1);
            int number1=0;
            int dec_len1=1;
            while(1)
            {

                if(Input_Number(C_RANGE_HPOSI, C_RANGE_VPOSI,&number1,4,&dec_len1,0))
                {
                    /*if((number1>C_MAX_RANGE/2&&MGetProbeMode()==1)\
                            ||(number1>C_MAX_RANGE&&MGetProbeMode()!=1))
                    {
                        DisplayPrompt(4);
                        MSetRange(range1,C_SETMODE_SETSAVE);
                    }

                    else if((number1<=C_MAX_RANGE&&MGetProbeMode()!=1)\
                            ||(number1<=C_MAX_RANGE/2&&MGetProbeMode()==1))
                    {
                        MSetRange(number1,C_SETMODE_SETSAVE);
                        break;
                    }*/
                    if(  ((number1<90||number1>C_MAX_RANGE/2)&&MGetProbeMode()==1)\
                            ||((number1<150||number1>C_MAX_RANGE)&&MGetProbeMode()!=1))
                    {
                        DisplayPrompt(4);
                        MSetRange(range1,C_SETMODE_SETSAVE);
                    }

                    else if((number1>=150&&number1<=C_MAX_RANGE&&MGetProbeMode()!=1)\
                            ||(number1>=90&&number1<=C_MAX_RANGE/2&&MGetProbeMode()==1))
                    {
                        MSetRange(number1,C_SETMODE_SETSAVE);
                        break;
                    }

                }
                else
                {
                    MSetRange(range1,C_SETMODE_SETSAVE);
                    break;
                }
            }
            MChannelRenovate();
            DisplayPrompt(16);///探伤中
            DrawDac(0);

            keycode = -1;
        }
        else keycode = RangeChange();
        break;
    case C_KEYCOD_ANGLE:
#if C_DEVTYPE == 3 || C_DEVTYPE == 11
        keycode = AngleChange();
        break;
#elif C_DEVTYPE == 2 || C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5 || C_DEVTYPE == 8 || C_DEVTYPE == 10  || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14 || C_DEVTYPE == 9
        if( mode1 == 1)
        {
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            keycode == ColorMenu();//ParaMenu(1);
            keycode = -1;
        }
        else keycode = AngleChange();
        break;
#elif C_DEVTYPE == 15 || C_DEVTYPE == 1
        keycode = TestChoice(C_TEST_ANGLE);
        break;
#endif

#if C_DEVTYPE==15 || C_DEVTYPE == 1
    case C_KEYCOD_DAC:
        keycode = TestChoice(C_TEST_DAC);
        break;
    case C_KEYCOD_SETUP:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        MenuChoice();
        keycode = -1;
        break;
#endif
#if C_DEVTYPE == 20
    case C_KEYCOD_SETUP:

        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        if( mode1 == 1)
        {
            //长按减号键，将到参数菜单的第二页，查看设置基准波高
            ParaMenu1(2);
        }
        else
        {
            ParaMenu1(1);
        }
        keycode = -1;
        break;

#elif C_DEVTYPE == 2 || C_DEVTYPE == 3 || C_DEVTYPE == 4 || C_DEVTYPE == 5 || C_DEVTYPE == 8  || C_DEVTYPE == 9  || C_DEVTYPE == 10  || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14
    case C_KEYCOD_SETUP:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        if( mode1 == 1)
        {
            //长按减号键，将到参数菜单的第二页，查看设置基准波高
            ParaMenu(2);
        }
        else ParaMenu(1);
        keycode = -1;
        break;
#endif
    case C_KEYCOD_GATE:
        if( MGetFunctionMode( C_WAVE_EXPEND	) && MGetGatePara(0,0) > 1)
        {
            /*			GateWaveExpend(0);
            			MSetSystemRange();	//设置当前声程
            			MSetSystemDelay();
            			MChannelRenovate();
            			MDrawGate(-1,0,-1,0);
            			DrawDac(0);
            			//WriteRange(C_RANGE_HPOSI,C_RANGE_VPOSI);
            			//WriteScale();
            			MKeyRlx();
            			while(true)
            			{
            				MParaRenovate(0);
            				if( MGetKeyCode(10) == C_KEYCOD_GATE) break;
            			}
            			GateWaveExpend(1);
            			MSetSystemRange();	//设置当前声程
            			MSetSystemDelay();
            */
            int range = MGetRange(-2);//当前所存的值
            int delay = MGetDelay(-2);
            int gateposi ;
            int gatewide ;
            int elapsedtime= GetElapsedTime();
            ChannelParaStore();
            gateposi = MGetGatePara( 0,0);
            gatewide = MGetGatePara( 0,1);
            MSetDelay( delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6), -2);
            MSetRange( range * gatewide/C_COORWIDTH, -2);
            MSetGatePara( 5, C_COORWIDTH-10, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
            MSetSystemRange();	//设置当前声程
            MSetSystemDelay();
            MChannelRenovate();
            MDrawGate(-1,0,-1,0);
            DrawDac(0);
            MKeyRlx();
            g_UDiskInfo.DataHeaderMark = 1;
            while(true)
            {
                keycode=MGetKeyCode(0);
                StorageData(C_KEYCODMAX);
                //	if (GetElapsedTime() >= elapsedtime)
                {
                    //	elapsedtime = GetElapsedTime() + 200;
                    MParaRenovate(1);
                }
                if( keycode == C_KEYCOD_GATE) break;
            }

//			MSetRange( range, -2);
//			MSetDelay( delay, -2);
//			MSetGatePara( gateposi, gatewide, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
//			MSetSystemRange();	//设置当前声程
//			MSetSystemDelay();
            g_UDiskInfo.DataHeaderMark = 1;
            ChannelParaRestore();
            MSetSystem();
            MChannelRenovate();
            DrawDac(0);
            DisplayPrompt(16);
            MKeyRlx();
            keycode = -1;
        }
        //*/

        else
        {
            MSetParaSort( MGetParaSort(C_AGATEPOSI) %3 ,C_AGATEPOSI) ;
            keycode = GateChange();
        }
        break;
#if C_DEVTYPE == 2 || C_DEVTYPE == 10  || C_DEVTYPE == 14
    case C_KEYCOD_GATEB:
        MSetParaSort( 4, C_BGATEPOSI);
        keycode = GateChange();
        break;
#endif
    case C_KEYCOD_RETURN:

        ClearEnvelope();	//清包络或峰值

//SetScanRotaryEncoder(1, 1, 0, 1) ;
//SetScanRotaryEncoder(0, 1, 0, 1) ;

        if(0)
        {
            int tm1,tm2;
            MKeyRlx();
            ClearEnvelope();
            tm1 = GetElapsedTime() + 1000 *3;
            tm2 = MGetKeyLock();
            //	MPushWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
            //	MEraseWindow(0,0,C_BASEGAIN_HPOSI-2,C_COORVPOSI-10);
            if( MGetKeyLock() > 0)
            {
                if (MGetLanguage())
                {
                    EMenuOut( 0, 4, _KEYLOCK_A2_E,13,1,12);
                }
                else
                {
                    CEMenuOut( 0, 0,_KEYLOCK_A2,6,1,12);
                }
            }
            else
            {
                if (MGetLanguage())
                {
                    EMenuOut( 0, 4, _KEYLOCK_A1_E,13,1,12);
                }
                else
                {
                    CEMenuOut( 0, 0,_KEYLOCK_A1,6,1,12);
                }
            }
            while( GetElapsedTime() < tm1 )
            {
                keycode = MGetKeyCode(10);
                if(keycode == C_KEYCOD_POINT)
                {
                    //短按确定键后在约定时间内按了点键
                    MSetKeyLock( tm2 + 1,C_SETMODE_SAVE);
                    break;
                }
            }
            DisplayPrompt(16);
            ClearEnvelope();
            //	MPopWindow();
            if ( tm2 != MGetKeyLock() )
            {
                MChannelRenovate();
                DrawDac(1);
            }

            MKeyRlx();


        }
        else if( stWeldPara.type > 0 )//长按返回键,且有焊缝设置
        {
            ShowWeld(20,58+240,520,160,MGetAngle(0), MGetAmpDist1(0) );
            ScreenRenovate();
            DrawDac(0);
        }
        ExpendTime(50);
        keycode = -1;
        break;

    case C_FUNC_1:  //功能快捷键F1-F4
//SetScanRotaryEncoder(1, 0, 0, 1) ;
//SetScanRotaryEncoder(0, 0, 0, 1) ;
        funcNumber=0;
        keycode = -1;
        break;
    case C_FUNC_2:
//    WavePlayback((WavHeader*) byebye, 100) ;
//    WaveVolume(80) ;
        funcNumber=1;
        keycode = -1;
        break;
    case C_FUNC_3:
//    WavePlayback((WavHeader*) lee, 100) ;
//    WaveVolume(100) ;
        funcNumber=2;
        keycode = -1;
        break;
    case C_FUNC_4:
//    WavePlayback((WavHeader*) ding, 100) ;
//    WaveVolume(10) ;
        funcNumber=3;
        keycode = -1;
        break;
    case C_KEYCOD_CONFIRM:

        //WavePlayback((WavHeader*) ding, 30) ;
//    WavePlayback((WavHeader*) alice, 100) ;
//    WaveVolume(50) ;
        // SetScanRotaryEncoder(1, 0, 1, 1) ;
        // SetScanRotaryEncoder(0, 0, 1, 1) ;
#if C_DEVTYPE == 4 || C_DEVTYPE == 20
        keycode = -1;
        break;
#endif
        if(mode1==1&&(MGetStdMode()==10||MGetStdMode()==11))
        {
        	int nflag=0;
			
            uint8_t szPingji[2][4][19]=
            {
                {
                    "不可验收           ",
                    "超评定等级         ",
                    "超记录等级         ",
                    "未超评定线         ",
                },
                {
                    "Unacceptable       ",
                    "Exceed assess grade",
                    "Exceed record grade",
                    "Uexceed assess line",
                }
            };

            if(MGetTestStatus(C_TEST_DAC)==1&&(MGetStdMode()==10||MGetStdMode()==11)&&MGetThick()!=0&&MGetBlock()==0)///11345
            {
                int iGain=MGetBaseGain();
                int iret;
                int iThick=MGetThick()/10;
                int i=0, j=0;
                short linegain[6]={0};
                xpos=1;
                ypos=56;

                MPushWindow(xpos,ypos,xpos+24*7+16*10+8,ypos+24*2+8);
                EraseDrawRectangle(xpos+1,ypos+1,xpos+24*7+16*10+8-1,ypos+24*2+8-1);
                int number=0;
                int dec_len=1;
                if(MGetLanguage()==0)
					TextOut(xpos+4,ypos+4,1,22,24,"请输入缺陷长度:     mm",0);
                else
                	TextOut(xpos+4,ypos+4,1,21,24,"input len:        mm",0);
				
                MEraseWindow(xpos+4+24*7+16,ypos+4,xpos+4+24*7+16+16*5,ypos+4+24);

				while(1)
				{
					if(Input_Number(xpos+4+24*7+16,ypos+4,&number,3,&dec_len,0))
					{
						MKeyRlx();
						if(number>=5)
							break;
						else
						{
							number=0;
							DisplayPrompt(4);
							continue;
						}
					}

					if(MGetKeyCode(0)==C_KEYCOD_RETURN)
					{
						nflag=1;
						break;
					}
				}

				if(nflag)
				{
					nflag=0;
					break;
				}
				
				MKeyRlx();
				Write_Number(xpos+4+24*7+16,ypos+4,number,5,1,0);
				int num=MGetThick()/2*10;
				int a=num%10;
				if(a>5)
					num=num/10+1;
				else
					num=num/10;

                if(number<=MGetThick()*0.5&&MGetStdMode()==10)
                {
                    MSetBlock(2);
                }
                else if(number<=MGetThick()&&MGetStdMode()==10)
                {
                    MSetBlock(3);
                }
                else if(number>MGetThick()&&MGetStdMode()==10)
                {
                    MSetBlock(4);
                }
                else if(number<=MGetThick()*0.5&&MGetStdMode()==11)
                {
                    MSetBlock(2);
                }
                else if(number<=MGetThick()&&MGetStdMode()==11)
                {
                    MSetBlock(3);
                }
                else if(number>MGetThick()&&MGetStdMode()==11)
                {
                    MSetBlock(4);
                }

                iret=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                MSetCurveNumber(c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM]);
                for(i=0;i<iret;i++)
                {
                    if(iThick>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i])
						continue;

                    for(j=0;j<MGetCurveNumber();j++)
                    {
                        MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                    }
                    break;
                }

                ScreenRenovate();
                DrawDac(0);
                int para4=0;
                int iMaxAmp=0;
                int iPos=0;
                if(MGetFunctionMode(C_ECHOMAX_MEMORY))
                {
                    uint8_t sampbuff[C_LEN_SAMP+1]={0};
                    int iOffset = C_OFF_SAMP;
                    iPos=0;
                    iMaxAmp=0;
                    MCopyProtectedMemory( sampbuff, (void*)iOffset, C_LEN_SAMP, PM_COPY_READ);
                    for(i=MGetGatePara(0,0);i<(MGetGatePara(0,0)+MGetGatePara(0,1));i++)
                    {
                        if(sampbuff[i]>=iMaxAmp)
                        {
                            iMaxAmp=sampbuff[i];
                            iPos=i;
                        }
                    }
                    para4= Amp2db( g_iPos,g_iMaxAmp);
					MSetColor(C_CR_ENVELOPE);
					DrawWave(sampbuffs,0,C_LEN_SAMP);
					MSetColor(C_CR_UNDO);
                }
                else
                	para4=Amp2db(MGetAmpPos(0),MGetAmpMax(0));

                MPushWindow(xpos+6*24-4,ypos+4+24-4,xpos+6*24+16*19+4,ypos+4+24+24+4);
                EraseDrawRectangle(xpos+6*24-4,ypos+4+24-4,xpos+6*24+16*19+4,ypos+4+24+24+4);
                if(MGetStdMode()==10)
                {
                    if(MGetThick()<150)
                    {
                        if(para4>=-40)
                            TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-80&&para4<-40)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,(uint8_t*)szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-80)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-140&&para4<-100)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                    else if(MGetThick()>=150&&MGetThick()<1000)
                    {
                        if(para4>=0)
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-40&&para4<0)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-40)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num&&number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-140&&para4<-100)
                        {
                            if(number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                }
                else if(MGetStdMode()==11)
                {
                    if(MGetThick()<150)
                    {
                        if(para4>=0)
                            TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-40&&para4<0)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-40)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                    else if(MGetThick()>=150&&MGetThick()<1000)
                    {
                        if(para4>=40)
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=0&&para4<40)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-20&&para4<0)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-20)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num&&number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                }

				MKeyRlx();
				while(MGetKeyCode(0)!=C_KEYCOD_RETURN);

				MPopWindow();
				MSetFunctionMode(MGetFunctionMode(C_ECHOMAX_MEMORY),C_ECHOMAX_MEMORY);
				MSetBlock(0);
	            MSetCurveNumber(c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM]);

	            for( i = 0 ; i < iret; i++)
	            {

	                if( iThick>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;
	                for( j = 0; j < MGetCurveNumber(); j++)
	                {
	                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
	                }
	                break;

	            }
            }

            if(MGetTestStatus(C_TEST_DAC) == 1)
            {
                MSetDelay(0,C_SETMODE_SETSAVE);
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
                DrawDac(0);		//新画DAC

                if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
                    else if (MGetStdMode()==12||MGetStdMode()==9)	//JB47013
                    {
                        MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);

                    }
                    else if (MGetStdMode()==7||MGetStdMode()==4)	//CBT3559-94
                    {
                        //MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/160.0),C_SETMODE_SETSAVE);
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
                MSetSystem();
                SystemParaStore();
                ChannelParaStore();
                ScreenRenovate();
                DrawDac(0);
            }
        }
        else//短按
        {
            /*uint8_t szPingji[2][4][19]=
            {
                {
                    "不可验收           ",
                    "超评定等级         ",
                    "超记录等级         ",
                    "未超评定线         ",
                },
                {
                    "Unacceptable       ",
                    "Exceed assess grade",
                    "Exceed record grade",
                    "Uexceed assess line",
                }
            };

            if(MGetTestStatus(C_TEST_DAC)==1&&(MGetStdMode()==10||MGetStdMode()==11)&&MGetThick()!=0&&MGetBlock()==0)///11345
            {
                int iGain=MGetBaseGain();
                int iret;
                int iThick=MGetThick()/10;
                int i=0, j=0;
                short linegain[6]={0};
                xpos=1;
                ypos=56;

                MPushWindow(xpos,ypos,xpos+24*7+16*10+8,ypos+24*2+8);
                EraseDrawRectangle(xpos+1,ypos+1,xpos+24*7+16*10+8-1,ypos+24*2+8-1);
                int number=0;
                int dec_len=1;
                if(MGetLanguage()==0)
					TextOut(xpos+4,ypos+4,1,22,24,"请输入缺陷长度:     mm",0);
                else
                	TextOut(xpos+4,ypos+4,1,21,24,"input len:        mm",0);
				
                MEraseWindow(xpos+4+24*7+16,ypos+4,xpos+4+24*7+16+16*5,ypos+4+24);
				
				if(Input_Number(xpos+4+24*7+16,ypos+4,&number,3,&dec_len,0))
				{
					MKeyRlx();
					if(number>=5)
						break;
					else
					{
						number=0;
						DisplayPrompt(4);
					}
				}

				if(MGetKeyCode(0)==C_KEYCOD_RETURN)
					break;
				MKeyRlx();
				Write_Number(xpos+4+24*7+16,ypos+4,number,5,1,0);
				int num=MGetThick()/2*10;
				int a=num%10;
				if(a>5)
					num=num/10+1;
				else
					num=num/10;

                if(number<=MGetThick()*0.5&&MGetStdMode()==10)
                {
                    MSetBlock(2);
                }
                else if(number<=MGetThick()&&MGetStdMode()==10)
                {
                    MSetBlock(3);
                }
                else if(number>MGetThick()&&MGetStdMode()==10)
                {
                    MSetBlock(4);
                }
                else if(number<=MGetThick()*0.5&&MGetStdMode()==11)
                {
                    MSetBlock(2);
                }
                else if(number<=MGetThick()&&MGetStdMode()==11)
                {
                    MSetBlock(3);
                }
                else if(number>MGetThick()&&MGetStdMode()==11)
                {
                    MSetBlock(4);
                }

                iret=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                MSetCurveNumber(c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM]);
                for(i=0;i<iret;i++)
                {
                    if(iThick>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i])
						continue;

                    for(j=0;j<MGetCurveNumber();j++)
                    {
                        MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                    }
                    break;
                }

                ScreenRenovate();
                DrawDac(0);
                int para4=0;
                int iMaxAmp=0;
                int iPos=0;
                if(MGetFunctionMode(C_ECHOMAX_MEMORY))
                {
                    uint8_t sampbuff[C_LEN_SAMP+1]={0};
                    int iOffset = C_OFF_SAMP;
                    iPos=0;
                    iMaxAmp=0;
                    MCopyProtectedMemory( sampbuff, (void*)iOffset, C_LEN_SAMP, PM_COPY_READ);
                    for(i=MGetGatePara(0,0);i<(MGetGatePara(0,0)+MGetGatePara(0,1));i++)
                    {
                        if(sampbuff[i]>=iMaxAmp)
                        {
                            iMaxAmp=sampbuff[i];
                            iPos=i;
                        }
                    }
                    para4= Amp2db( g_iPos,g_iMaxAmp);
					MSetColor(C_CR_ENVELOPE);
					DrawWave(sampbuffs,0,C_LEN_SAMP);
					MSetColor(C_CR_UNDO);
                }
                else
                	para4=Amp2db(MGetAmpPos(0),MGetAmpMax(0));

                MPushWindow(xpos+6*24-4,ypos+4+24-4,xpos+6*24+16*19+4,ypos+4+24+24+4);
                EraseDrawRectangle(xpos+6*24-4,ypos+4+24-4,xpos+6*24+16*19+4,ypos+4+24+24+4);
                if(MGetStdMode()==10)
                {
                    if(MGetThick()<150)
                    {
                        if(para4>=-40)
                            TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-80&&para4<-40)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,(uint8_t*)szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-80)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-140&&para4<-100)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                    else if(MGetThick()>=150&&MGetThick()<1000)
                    {
                        if(para4>=0)
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-40&&para4<0)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-40)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num&&number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-140&&para4<-100)
                        {
                            if(number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                }
                else if(MGetStdMode()==11)
                {
                    if(MGetThick()<150)
                    {
                        if(para4>=0)
                            TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=-40&&para4<0)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-40)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=MGetThick())  TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick()) TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                    else if(MGetThick()>=150&&MGetThick()<1000)
                    {
                        if(para4>=40)
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);
                        else if(para4>=0&&para4<40)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-20&&para4<0)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-60&&para4<-20)
                        {
                            if(number<=num)
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>num&&number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][0],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"不可验收",0);
                        }
                        else if(para4>=-100&&para4<-60)
                        {
                            if(number<=MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][1],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超评定等级",0);
                            else if(number>MGetThick())
                                TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][2],0);//TextOut(xpos+6*24,ypos+4+24,1,10,24,"超记录等级",0);
                        }
						else
							TextOut(xpos+6*24,ypos+4+24,1,19,24,szPingji[MGetLanguage()][3],0);
                    }
                }

				MKeyRlx();
				while(MGetKeyCode(0)!=C_KEYCOD_RETURN);

				MPopWindow();
				MSetFunctionMode(MGetFunctionMode(C_ECHOMAX_MEMORY),C_ECHOMAX_MEMORY);
				MSetBlock(0);
	            MSetCurveNumber(c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM]);

	            for( i = 0 ; i < iret; i++)
	            {

	                if( iThick>=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )    continue;
	                for( j = 0; j < MGetCurveNumber(); j++)
	                {
	                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
	                }
	                break;

	            }
            }*/

            if(MGetTestStatus(C_TEST_DAC) == 1)
            {
                MSetDelay(0,C_SETMODE_SETSAVE);
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
                DrawDac(0);		//新画DAC

                if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
                    else if (MGetStdMode()==12||MGetStdMode()==9)	//JB47013
                    {
                        MSetBaseGain( MGetBaseGain()-200*log10( dac_line[MGetThick()*2*C_COORWIDTH/MGetRange(4)]/80.0),C_SETMODE_SETSAVE);

                    }
                    else if (MGetStdMode()==7||MGetStdMode()==4)	//CBT3559-94
                    {
                        //MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/160.0),C_SETMODE_SETSAVE);
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
                MSetSystem();
                SystemParaStore();
                ChannelParaStore();
                ScreenRenovate();
                DrawDac(0);
            }
        }
        keycode = -1;
        break;
    case C_KEYCOD_SAVE:
        if( MGetSystemMode() != 1)
        {
            if( stWeldPara.type > 0 && !MGetSaveMode())
            {
                //有焊缝/无连续存
                if( ShowWeld(20,58+240,520,160,MGetAngle(0), MGetAmpDist1(0) ) == 0)
                {
                    //按了返回键
                    keycode = -1;
                    break;
                }
            }

            GetDispEchoPara();/*得到指定门（0=A or 1=B)内回波的位置大小并写在屏幕上*/
            MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
            WaveSave(mode1);
            MSetAcquisition(1);
            DisplayPrompt(16);
        }
        //if( MGetSystemMode() != 1)DataMenu(1);
        keycode = -1;
        break;
    case C_KEYCOD_DELAY:
        keycode = DelayChange();
        break;
#if  !(C_DEVTYPE == 14 ||C_DEVTYPE == 15||C_DEVTYPE == 1)
    case C_KEYCOD_PLUS:
//	case C_KEYCOD_RIGHT:
        g_UDiskInfo.DataHeaderMark = 1;
        StorageData(keycode);
        ClearEnvelope();	//清包络或峰值
        DisplayPrompt(18);
        DrawDac(1);
        MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
        DrawDac(0);
        DisplayPrompt(16);
        keycode = -1;
        break;
#endif
#if C_DEVTYPE == 14
    case C_KEYCOD_MINUS:
#endif
    case C_KEYCOD_LEFT:
        keycode = -1;
        if(extend==0)
        {
            int key;
            int nPulseWidth = 0;

            //   SystemParaRestore();
            //   ChannelParaRestore();
            //   MSetSystemMode(0,0);	//设置成一般状态
            //   MSetBaseGain(400,C_SETMODE_SETSAVE);

            //   MSetSystem();
            //   MSetAcquisition(1);

            //   ScreenRenovate();	/*屏幕刷新*/

            nPulseWidth = MGetPulseWidthNumber();	//33
            ETextOut(320,10,"PulseWidth:  ",11);
            Write_Number(320+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
            ExpendTime(20);
            MKeyRlx();
            while(true)
            {

                MParaRenovate(1);
                key = MGetKeyCode(0);
                if( key == C_KEYCODMAX)continue;
                if( key == C_KEYCOD_RETURN)
                {
                    ScreenRenovate();	/*屏幕刷新*/
                    break;
                }
                if( key == C_KEYCOD_PLUS || key == C_KEYCOD_MINUS)
                {
                    if( key == C_KEYCOD_PLUS )nPulseWidth++;
                    else if( key == C_KEYCOD_MINUS )nPulseWidth--;
                    SetPulseWidth(nPulseWidth);
                    MSetPulseWidthInput(nPulseWidth, C_SETMODE_SETSAVE);
                    if( nPulseWidth >= 127)nPulseWidth = 0;
                    else if( nPulseWidth < 0) nPulseWidth = 127;
                    ETextOut(320,0,"                  ",16);
                    ETextOut(320,10,"PulseWidth:       ",16);
                    Write_Number(320+C_ECHAR_HDOT*11,10,nPulseWidth,4,0,0);
                    ExpendTime(20);
                    continue;
                }

                if( key == C_KEYCOD_BASEGAIN || key == C_KEYCOD_GATE || key == C_KEYCOD_RANGE)
                {
                    KeyManage(key,1);
                    ExpendTime(20);
                }
                MKeyRlx();
            }

        }
        break;
        if( mode1 == 1)
        {
            //长按加号键，调节亮度
#if C_LIGHT == 1
#if C_DEVLIB == 1  || C_DEVLIB == 3|| C_DEVLIB == 24 || C_DEVLIB == 23
            int bright;
            //	CEMenuOut(0,0,_Bright_A1,7,1,16);
            TextOut(0,0,1,18,16,(uint8_t*)_Bright_A1[MGetLanguage()][0],4);

            MKeyRlx();
            bright = 0;
            while( true)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RIGHT )
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    MSetScreenBrightness( (MGetScreenBrightness()+1)%4,C_SETMODE_SETSAVE);
                    ExpendTime(20);
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                }
                else if( keycode == C_KEYCOD_LEFT )
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    MSetScreenBrightness( (MGetScreenBrightness()-1)%4,C_SETMODE_SETSAVE);
                    ExpendTime(20);
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                }
                else if( keycode != C_KEYCODMAX)break;
                //	MKeyRlx();
            }
            DisplayPrompt(16);
#else  // C_DEVLIB == 23
            int bright;
            //	CEMenuOut(0,0,_Bright_A1,7,1,16);
            TextOut(0,0,1,18,16,(uint8_t*)_Bright_A1[MGetLanguage()][0],4);
            MKeyRlx();
            bright = 0;
            while( true)
            {
                keycode = MGetKeyCode(0);

                if( keycode == C_KEYCOD_RIGHT || keycode == C_KEYCOD_LEFT )
                {
                    if (MGetScreenBrightness()==0)
                    {
                        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                        MSetScreenBrightness(3,C_SETMODE_SETSAVE);
                        ExpendTime(20);
                        MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    }
                    else
                    {
                        MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                        MSetScreenBrightness(0,C_SETMODE_SETSAVE);
                        ExpendTime(20);
                        MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    }
                }
                //
                else if( keycode != C_KEYCODMAX)break;

                //MKeyRlx();
                ExpendTime(20);
            }
            DisplayPrompt(16);
#endif
#endif
        }
        else
        {
            //短按加号键，调节扫查增益使波到80％
            ///	MAdjustGain(1,0,MGetAmpStdMax(),MGetAmpStdMax());
#if C_FILL == 1
            //   MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
#endif
        }
        keycode = -1;
        break;///暂不考虑下面的测试增益表的语句
        MKeyRlx();
        ExpendTime(100);
        keycode = MGetKeyCode(10);
        MKeyRlx();
        if( keycode == C_KEYCOD_RIGHT)
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				TestGainTable();
            }
        }
        else if( keycode == 7 )
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				TestGainTable1();
            }
        }
        else
        {
            ExpendTime(100);
            if( MGetKeyCode(10) == C_KEYCOD_LEFT)
            {
///				if( keycode == 0 )keycode = 10;
///				TestGain(keycode);
            }
        }

        keycode = -1;
        break;
    case C_KEYCODMAX:
        keycode = C_KEYCODMAX;
        break;
    default :
        keycode = -1;
        break;
    }
/////////////可设置功能
    /*#define C_KEYCOD_FUNCTION	16     //换用峰值记忆
    #define C_KEYCOD_ENVELOPE	1      //回波包络
    #define C_KEYCOD_FREEZ	    2      //回波包络
    #define C_KEYCOD_DATA		3*/
    if(funcNumber<4)
    {
        switch (Function.funcMenu[funcNumber])
        {
        case C_FUNC_INIT:             //初始化

            MSetAcquisition(0);
            if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
            {
                break;
            }
            InitPara();//初始化
            MSetAcquisition(1);
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);		//新画DAC
            break;
        case C_FUNC_ENVELOPE:	      //回波包络

            DisplayPrompt(16);
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            ClearEnvelope();
            if(MGetFunctionMode(C_ECHO_ENVELOPE) == 0)
            {
                MSetFunctionMode(1,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                MSetSaveFrequency(0,0);
            }
            else if (MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;
        case C_FUNC_ECHOMAX:	          //峰值记忆
            DisplayPrompt(16);
            g_UDiskInfo.DataHeaderMark = 1;
            StorageData(keycode);
            ClearEnvelope();
            if(MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_GATE_ALARM);
                MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                MSetSaveFrequency(0,0);
            }
            else if (MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;

        case C_FUNC_DACGATE:	          //DAC门

            prestatus = MGetFunctionMode(C_DAC_GATE);	//原状态
            MSetFunctionMode( prestatus+1,C_DAC_GATE);
            if( MGetFunctionMode(C_DAC_GATE) == 1 )
            {
                if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0)
                {
                    MSetFunctionMode(0,C_DAC_GATE);
                }
                if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 1 && MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    //  InvertWords(inv_xpos, inv_ypos + (C_DEPTH_COMPENSATE + 1)* crow_height-2,1 ); /*反白显示数字*/
                    MSetFunctionMode(0,C_DEPTH_COMPENSATE);
                }
                if( MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    int dac_dB;

                    dac_dB = MGetDacDb();	//dacdB：在DAC母线门为处的dB值
                    MSetGatedB(11, 0);
                    DacGateInit();
                    //MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),DacGate_dBHigh(11,dac_dB),0,C_SETMODE_SETSAVE);
                }
            }
            else if( prestatus == 1)
            {
                //从DAC门改成普通门
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);
            break;
        case C_FUNC_DACADJUST:	      //深度补偿

            if( MGetFunctionMode(C_DAC_GATE) == 1 && prestatus == 0)
            {
                //   InvertWords(inv_xpos, inv_ypos + (C_DAC_GATE + 1)* crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_DAC_GATE);

                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            if( MGetEquivalentDays(1) <= 0 || MGetEchoMode() == C_RF_WAVE)	//返回测试状态<0=未测>=0已测，
            {
                MSetFunctionMode( 0,C_DEPTH_COMPENSATE);
            }
            else
            {
                prestatus = MGetFunctionMode(C_DEPTH_COMPENSATE);	//原状态
                MSetFunctionMode( prestatus+1,C_DEPTH_COMPENSATE);
            }
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);
            break;
        case C_FUNC_GATEEXPEND:        //门内展宽

            if( MGetGatePara(0,0) > 1)
            {
                prestatus = MGetFunctionMode(C_WAVE_EXPEND);	//原状态
                MSetFunctionMode( prestatus+1,C_WAVE_EXPEND);
                int range = MGetRange(-2);//当前所存的值
                int delay = MGetDelay(-2);
                int gateposi ;
                int gatewide ;
                int elapsedtime= GetElapsedTime();
                ChannelParaStore();
                gateposi = MGetGatePara( 0,0);
                gatewide = MGetGatePara( 0,1);
                MSetDelay( delay + (int)( (float)( C_SAMPLE_FREQ * range )/(float)MGetSpeed() * (float)gateposi/ (float)C_COORWIDTH + 0.6), -2);
                MSetRange( range * gatewide/C_COORWIDTH, -2);
                MSetGatePara( 5, C_COORWIDTH-10, MGetGatePara(0,2), 0, C_SETMODE_SETSAVE);
                MSetSystemRange();	//设置当前声程
                MSetSystemDelay();
                MChannelRenovate();
                MDrawGate(-1,0,-1,0);
                DrawDac(0);
                MKeyRlx();
                g_UDiskInfo.DataHeaderMark = 1;
                while(true)
                {
                    keycode=MGetKeyCode(0);
                    StorageData(C_KEYCODMAX);
                    MParaRenovate(1);
                    if( keycode == C_KEYCOD_GATE) break;
                    else if(keycode == C_KEYCOD_RETURN || (funcNumber==0&&keycode==C_FUNC_1) ||(funcNumber==1&&keycode==C_FUNC_2)
                            ||(funcNumber==2&&keycode==C_FUNC_3) ||(funcNumber==3&&keycode==C_FUNC_4) )
                    {
                        break;
                    }
                }
                g_UDiskInfo.DataHeaderMark = 1;
                ChannelParaRestore();
                MSetSystem();
                MChannelRenovate();
                DrawDac(0);
                // InvertBytes(400+24+32*funcNumber,7, 1);
                // MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                DisplayPrompt(16);
                keycode = -1;
                MKeyRlx();
            }
            break;
        case C_FUNC_GATEALARM:	      //门内报警
            prestatus = MGetFunctionMode(C_GATE_ALARM);	//原状态
            MSetFunctionMode( prestatus+1,C_GATE_ALARM);

            if(MGetFunctionMode(C_GATE_ALARM) == 1)
            {
                ClearEnvelope();
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            SysParaDisp();
            break;
        case C_FUNC_DATA:		      //数据处理
            if (MGetSaveStatus() <= 0)
            {
                // InvertBytes(400+24+32*funcNumber,7, 1);
                //MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                if( MGetSystemMode() != 1)
                {
                    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                    DataMenu(mode1);
                    MSetSystem();
                    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                    DisplayPrompt(16);
                }
                //if( MGetSystemMode() != 1)DataMenu(1);
                MKeyRlx();
                ExpendTime(20);
            }
            break;
        case C_FUNC_FREEZ:	          //波形冻结
            if( MGetAmpMax(0) >= 5)
            {
                // InvertBytes(400+24+32*funcNumber,7, 1);
                // MInvertWindow( 512+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-32,400+24+32*funcNumber+24,7+24) ;
                MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                mode1 = MGetBGateMode();
                MSetBGateMode(0,C_SETMODE_SETSAVE);//先设B门为失波,并保存原来状态
                // MChannelRenovate();
                // DrawDac(1);
                ClearEnvelope();
                MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
                sampbuffer = GetSampleBuffer();
                ClearCursor(2);
                if( keycode != C_KEYCOD_CONFIRM )
                {
                    keycode = MGetKeyCode(10);
                    if(keycode == C_KEYCODMAX)
                    {
                        keycode = MGetRotary(-1);
                        if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                        else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                        else keycode = C_KEYCODMAX;
                    }
                }
                else
                {
                    xpos = GateMax.Pos;
                    keycode = C_KEYCOD_RIGHT;
                }
                if(keycode == C_KEYCOD_LEFT)xpos++;
                else if(keycode == C_KEYCOD_RIGHT)xpos--;	//使得当前最高波能第一次被选中


                do
                {
                    if(keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    {
                        ClearCursor(2);
                        xpos = SearchPeak(sampbuffer,xpos,keycode);
                        if( xpos < 0)break ;
                        ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (*(sampbuffer + xpos))*2;
                        DrawCursor(xpos,ypos,2);
                    }
                    else if(keycode == C_KEYCOD_RETURN || (funcNumber==0&&keycode==C_FUNC_1) ||(funcNumber==1&&keycode==C_FUNC_2)
                            ||(funcNumber==2&&keycode==C_FUNC_3) ||(funcNumber==3&&keycode==C_FUNC_4) )
                    {
                        // InvertBytes(400+24+32*funcNumber,7, 1);
                        MInvertWindow( 504+32*funcNumber,C_COORVPOSI + C_COORHEIGHT-35,504+32*funcNumber+24,C_COORVPOSI + C_COORHEIGHT-35+24) ;
                        ClearCursor(2);
                        break;
                    }
                    keycode = MGetKeyCode(10);
                    if(keycode == C_KEYCODMAX)
                    {
                        keycode = MGetRotary(-1);
                        if( keycode > 0 )keycode = C_KEYCOD_LEFT;
                        else if( keycode < 0)keycode = C_KEYCOD_RIGHT;
                        else keycode = C_KEYCODMAX;
                    }
                    ExpendTime(20);
                }
                while( xpos > 0 );	//当SearchPeak返回-1时表示未能找到最高波
                MSetGateParaInit();	//门内最高波设初值
                MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
                MSetBGateMode(mode1,C_SETMODE_SETSAVE);//恢复B门类型
                // DrawDac(1);
                keycode = -1;
            }
            break;
        case C_FUNC_FILL:	          //实心波形
            prestatus = MGetFill();	//原状态
            // MSetFunctionMode( prestatus+1,C_WAVE_FILL);
            MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
            SysParaDisp();
            break;
        default:
            break;
        }

    }



/////////////
    if( MGetStdMode() == C_STD_AWS_NUM + 5)//AWS
    {
        MSetBGateMode(0, C_SETMODE_SAVE);
        //MSetEquivalentMode();	//mode1=0B门失波1进波，mode2<0波高.>0dB数
    }

    if( keycode == C_KEYCOD_RETURN )
    {
        keycode = -1 ;
    }
//	if( keycode == -1 )ExpendTime(20);
//Write_Number(10,300,1,1,0,0);
    if(MGetSystemMode() == 0 && keycode != C_KEYCODMAX)
//	if( MGetSystemMode() == 0 && keycode == -1 )
    {
//Write_Number(10,300,2,1,0,0);
        /*int i,j,rev=0;
        int FileMax=(C_OFF_FILE+4095)/4096*4096;
        char c[FileMax];*/
        SystemParaStore();
        ChannelParaStore();

        /* for(i=0;i<FileMax;i++)
         {
             c[i]=MEMORY_STORAGE[i];
         }*/
         CopyMemoryAllSystem(PM_COPY_WRITE);

        /*      CopyMemoryAllSystem(PM_COPY_READ);
                          for(i=0,j=0;i<FileMax;i++)
                          {
                              if(c[i]!=MEMORY_STORAGE[i]){
                                  j++;
                              }
                          }
                              Write_Number(30,0,j,4,0,0);
        */
        /*        MEMORY_STORAGE[C_OFF_BACKUP]=12;
                            for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,1,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();
                rev=CopyMemoryAllSystem(PM_COPY_WRITE);

                            for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,2,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();

                MEMORY_STORAGE[C_OFF_BACKUP]=13;
                rev=CopyMemoryAllSystem(PM_COPY_READ);

                           for(i=0,j=0;i<145;i++)
                            {
                                if(i%5==0 && i>0)j++;
                                Write_Number((i%10)*112,10+j*16,MEMORY_STORAGE[C_OFF_BACKUP-i],7,0,0);
                            }
                         Write_Number(10,0,3,3,0,0);
                         Write_Number(10,20,rev,3,0,0);
                         MAnyKeyReturn();
            */
    }
    //if(mode == 1 && keycode == C_KEYCOD_RETURN)MChannelRenovate();
    return keycode;
}
int ParaMenu1(int page)//出现第几页的参数菜单
{
    uint32_t char_len = 6,row_number = 8,crow_height = 48,erow_height =16;
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 0;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
//	int offset;
    uint16_t* cpText = 0;
    uint8_t* epText = 0;
    uint8_t* epText1 = 0;
    int i;
    uint32_t elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;

    MSetColor(C_CR_MENU);

    MSetAcquisition(0);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

#if C_DEVTYPE==20
    MSetDacMode(2,0/*mode = 0*/);
#endif

    while(1)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);

        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                if( MGetTestStatus(C_TEST_AVG) != 1)
                {
                    epText1 =  (uint8_t *)_ParaMenu_A6[MGetLanguage()][i];
                    epText =  (uint8_t*)_ParaMenu_B6;
                }
                else
                {
                    epText1 =  (uint8_t *)_ParaMenu_A6A1[MGetLanguage()][i];
                    epText =  (uint8_t*)_ParaMenu_B6A1;
                }
                row_number = 8;
                break;
            case 2:
                epText1 =(uint8_t *)_ParaMenu_A3[MGetLanguage()][i];
                epText =  (uint8_t*)_ParaMenu_B3;
                row_number = 2;
                break;

            }
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,11,crow_height,epText1,0);
            if (MGetLanguage())
            {
                TextOut(C_ECHAR_HDOT,para_ypos+10+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        TextOut(C_ECHAR_HDOT,4,1,25,16,(uint8_t*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8,7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1)*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);

        MSetColor(C_CR_PARA);

        xpos = para_xpos;
        //声程标度
        switch(page)
        {
        case 1:
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (uint16_t*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos ,1,7,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //表面补偿
            row = 1;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);

            row = 2;//工件厚度
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            WriteLongness(xpos , ypos  ,MGetThick(),4/*总长*/,1/*是否写单位*/);
            //Write_Number(xpos,ypos,MGetThick(),4,1,0);

            row = 3;//打印机
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            epText = (uint8_t*)_ParaMenu_AB3 + ( (MGetPrinterMode()-0)*15+2 );
            EMenuOut(xpos,ypos , epText ,13,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            //3．	当量标准：母线\判废\定量\评定
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);

            //7．	判废线RL：DAC+0.0dB
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(0),5,1,1);

            //8．	定量线SL：DAC+0.0dB
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(1),5,1,1);

            //9．	评定线EL：DAC+0.0dB
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.6) ,4,1,0);
            }
            else Write_Number(xpos,ypos,MGetLineGain(2),5,1,1);

            break;

        case 2:

            //1．	记录方式：单幅\连续
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                //	cpText = (uint16_t*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos,ypos ,1,6,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);
            }
            //2．	记录间隔：10sec-120sec	;如为单幅则此参量为0，选连续时自动设为30sec
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC7[MGetLanguage()][MGetSavePosition()]+2,4);

                //Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);
            }
            break;
        }

        MKeyRlx();		/*只有按键已释放才退出*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

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

        if (keycode == C_KEYCOD_RIGHT)
        {
            page++;
            if(page > page_max)page = 1;
            continue;
        }
        else if (keycode == C_KEYCOD_LEFT)
        {
            page--;
            if(page == 0)page = page_max;
            continue;
        }

        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*反白显示几个字的区域*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//标度
                //	cpText = (uint16_t*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;

            case 2:	//表面补偿
                deci_len = 1;
                number = 100;
                while(true)
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;
            case 3:	//
                if( MGetUnitType() > 0)deci_len = 3;//单位inch
                else deci_len = 1;
                number = 0;
                while( true )//如未设置连续存则不能输入
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }

                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                break;
            case 4:	//打印机
                epText = (uint8_t*)_ParaMenu_AB3;
                //ypos -= erow_height * 4 ;
                MPushWindow(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 5) ;
                EraseDrawRectangle(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 5) ;
                EMenuOut(xpos+4,ypos+4, epText ,15,4,15);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetPrinterMode(keycode-1,0/*mode = 0\1\2*/);	/*设置探头模式*/
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//当量标准
#if C_DEVTYPE==20
                break;
#else
                //	cpText = (uint16_t*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 ) ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
#endif
            case 6:		//Rl
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if( MGetTestStatus(C_TEST_AVG) == 1) sign = 0;
                else sign = 1;

                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if( number >= 5 && number < 100)
                            {
                                number = (int)( 400 * log10( number/10)-120 );
                            }
                            else break;
                        }
                        else if(number < -300 || number > 400)break;
                        MSetLineGain(0, number);
                        break;
                    }
                }

                break;
            case 7:
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            //AVG
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10) - 120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(1, number);
                        break;
                    }
                }

                break;
            case 8:	//El
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;
                while( 1 )
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(2, number);
                        break;
                    }
                }
                break;
            }
        }
        else if( page == 2)
        {
#if C_UDISK == 1
            switch (keycode)
            {
            case 1:	//存储模式
                //*
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveMode(keycode-1,C_SETMODE_SETSAVE);
                        if (MGetSaveMode())
                        {
                            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                            MSetSavePosition(1,C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            MSetSavePosition(0,C_SETMODE_SETSAVE);
                        }
                        /*	if( MGetSaveTime() > 600 || MGetSaveTime() < 1)
                        {
                        MSetSaveTime(10,C_SETMODE_SAVE);
                        offset = C_OFF_STORE +	34;		//已自动存数据个数
                        number = 0;
                        MCopyProtectedMemory( (void*)offset, &number,4, PM_COPY_WRITE);
                        					}*/
                        break;
                    }
                }
                MPopWindow();
                //*/
                break;

            case 2:	//存储位置
                break;
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                ypos -= crow_height * 1;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,10,crow_height,(uint8_t*)_ParaMenu_AC7[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSavePosition(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;

            }
            ExpendTime(20);
#endif
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);
    MSetAcquisition(1);
    ClearEnvelope();	//清包络或峰值

    MSetColor(C_CR_UNDO);
    ScreenRenovate();	/*屏幕刷新*/
    if(MGetStdMode()==10||MGetStdMode()==11) MSetCurveNumber(3);
    DrawDac(0);		//新画DAC
    return 1;
}

int ParaMenu(int page)//出现第几页的参数菜单
{
    uint32_t char_len = 6,row_number = 9,crow_height = 48,erow_height = 30;
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height ;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
    int stdswitch=0;
    uint16_t* cpText = 0;
    uint8_t* epText = 0;
    uint8_t* epText1 = 0;
    int i;
    uint32_t elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;
    int old_unit = MGetUnitType();
    int old_unit1 = MGetUnitType();

#if C_DEVTYPE == 9 || C_DEVTYPE == 10 || C_DEVTYPE == 13 || C_DEVTYPE == 14  ||C_DEVTYPE == 15
    page_max = 3;	//3
#else
    page_max = 2;
#endif

    i = 0;
    if( page < 1)page = 1;
    else if(page > page_max )page = page_max;

    MSetColor(C_CR_MENU);

    MSetAcquisition(0);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

    xpos = 0;
    while(1)
    {
        para_ypos = crow_height-4 ;
        MFclearScreen();
        MSetColor(C_CR_MENU);

        TextOut(C_ECHAR_HDOT,4,1,25,16,(uint8_t*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+16, 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    epText1 =(char *)_ParaMenu_A22[MGetLanguage()][i];
                    epText =  (uint8_t*)_ParaMenu_B2A1;
                }
                else
                {
                    epText1 = (char *)_ParaMenu_A2[MGetLanguage()][i];
                    epText =  (uint8_t*)_ParaMenu_B2;
                }
                if((MGetStdMode()==10||MGetStdMode()==11||MGetStdMode()==14)&& (page == 1)&&MGetTestStatus(C_TEST_DAC))
                {
                    row_number = 6;
                }
                else if((MGetStdMode()==10||MGetStdMode()==11||MGetStdMode()==14)&& (page == 1)&&MGetTestStatus(C_TEST_AVG))
				{
                    row_number = 6;
                }
                else
                {
                    row_number = 9;
                }
                break;
            case 2:
                epText1 =(char *)_ParaMenu_A3[MGetLanguage()][i];
                epText =  (uint8_t*)_ParaMenu_B3;
#if C_DEVTYPE == 1
                row_number = 9;
#else
                row_number = 9;
#endif

                break;
            case 3:
                epText1 =	(char *)_ParaMenu_C1[MGetLanguage()][i];
                epText =  (uint8_t*)_ParaMenu_D1;
#if C_DEVTYPE == 1
                row_number = 2;
#else
                row_number = 2;
#endif
                break;
            }

            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,crow_height,epText1,0);
            //   if (MGetLanguage())
            {
                //      TextOut(C_ECHAR_HDOT,para_ypos+C_ECHAR_VDOT+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos+C_ECHAR_VDOT*MGetLanguage() , epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        xpos = para_xpos;
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }

        switch(page)
        {
        case 1:
            //声程标度
            row = 0;
            ypos = para_ypos + crow_height *row;
            //	cpText = (uint16_t*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos ,1,7,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //表面补偿
            row = 1;
            ypos = para_ypos + crow_height *row ;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);
            row = 2;//工件厚度
            ypos = para_ypos + crow_height *row ;
            WriteLongness(xpos,ypos,MGetThick(),5,1);

#if C_DEVTYPE == 4 || C_DEVTYPE == 20
            break;
#endif
            row = 3;//工件外径
            ypos = para_ypos + crow_height *row ;
            WriteLongness(xpos,ypos,MGetDiameter(),5,1);
            ///Write_Number(xpos,ypos,MGetDiameter(),4,1,0);

            //3．	当量标准：母线\判废\定量\评定
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
			if(MGetStdMode()==14)
			{
				TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][2]+2,4);
			}
			else 
			{
				TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);
			}

            //4．	当量显示：dB值\孔径
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;

			// if( MGetTestStatus(C_TEST_AVG) == 1)
            {
                TextOut(xpos,ypos ,1,6,crow_height,(uint8_t*)_ParaMenu_AC3A[MGetLanguage()][MGetAvgMode()]+2,4);
            }
			// else
            {
				// cpText = (uint16_t*)_ParaMenu_AC3 + (MGetAvgMode()-0) * 4 + 1;		//由于DB算为两个字
				// CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);
            }
			if((MGetStdMode()==10||MGetStdMode()==11||MGetStdMode()==14)&&MGetTestStatus(C_TEST_DAC)==1)  break;///11345退出
			if((MGetStdMode()==10||MGetStdMode()==11||MGetStdMode()==14)&&MGetTestStatus(C_TEST_AVG)==1)  break;///11345退出
            ///if(MGetStdMode() !=10&&MGetStdMode() !=11&&MGetTestStatus(C_TEST_DAC)==0)
            {
                //7．	判废线RL：DAC+0.0dB
                row = 6;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    //WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);

                    //				//Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(0) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(0),5,1,1);

                //8．	定量线SL：DAC+0.0dB
                row = 7;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    // WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);
                    //Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(1) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(1),5,1,1);

                //9．	评定线EL：DAC+0.0dB
                row = 8;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                if( MGetTestStatus(C_TEST_AVG) == 1)
                {
                    number = (int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.6);
                    if(MGetUnitType() > 0)
                    {
                        number = (UINT)(number * 1000/254.0+0.5);
                    }
                    //  WriteLongness(xpos,ypos,number,5,1);
                    Write_Number(xpos,ypos,number,5,1,1);
                    //Write_Number(xpos,ypos,(int)( 10*powf( 10,( MGetLineGain(2) + 120 ) / 400.0)  + 0.5) ,4,1,0);
                }
                else Write_Number(xpos,ypos,MGetLineGain(2),5,1,1);
            }
            break;

        case 2:

//#if C_UDISK == 1
            //1．	记录方式：单幅\连续
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                //	cpText = (uint16_t*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);
                TextOut(xpos,ypos ,1,6,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);
            }
            //2．	记录间隔：10sec-120sec	;如为单幅则此参量为0，选连续时自动设为30sec
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC7[MGetLanguage()][MGetSavePosition()]+2,4);

                //Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);
            }
            /*#else
            			row = 0;//计量单位
            			if( row >= row_number)break;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            			epText = (uint8_t*)_ParaMenu_CA1 + MGetUnitType() * 6 + 2;
            			EMenuOut(xpos,ypos , epText ,4,1,crow_height);

            			row = 1;//语言
            			if( row >= row_number)break;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            			epText = (uint8_t *)_MainMenu_C1A11[MGetLanguage()][MGetLanguage()];
            			TextOut(xpos,ypos,1,8,crow_height,epText+2,8);
            #endif*/

            //位置方式,前沿峰值
            row = 2;
            ypos = para_ypos + crow_height *row ;
            TextOut(xpos,ypos ,1,6,crow_height,(uint8_t*)_ParaMenu_AC6[MGetLanguage()][MGetReadForeland()]+2,4);

            xpos = para_xpos;

            row = 3;//语言
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row;
            epText = (uint8_t *)_MainMenu_C1A11[MGetLanguage()][MGetLanguage()];
            TextOut(xpos,ypos,1,8,crow_height,epText+2,8);


            //6．	B门用途：失波\进波
            row = 4;
            ypos = para_ypos + crow_height *row;
            TextOut(xpos,ypos,1,6+2*MGetLanguage(),crow_height,(uint8_t*)_ParaMenu_AC5[MGetLanguage()][MGetBGateMode()]+2,4);

            row = 5;//按键声音开关
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            //	cpText = (uint16_t*)_ParaMenu_AD8 + MGetSoundAlarm(0) * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos,1,4,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][MGetSoundAlarm(0)]+2,4);

            //5．	屏幕亮度：低亮\一般\中亮\高亮
#if C_LIGHT == 1
#if C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 23 || C_DEVLIB == 24
            row = 6;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][MGetScreenBrightness()]+2,4);
#endif
#endif
            //7．
            row = 7;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            Write_Number(xpos,ypos,MGetAmpStdMax()*5,4,1,0);

            //焊缝图示
            row = 8;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                //	cpText = (uint16_t*)_Weld_C1 + stWeldPara.type  * 3 + 1;
                //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos,ypos,1,8,16,(uint8_t*)_Weld_C1[MGetLanguage()][stWeldPara.type]+2,4);
            }

            //8．	屏保延时：  min		；无屏保时为0，设为有时自动设为5min
            /*				row = 7;
            			xpos = para_xpos;
            			ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            */
            break;
        case 3:

            row = 0;//打印机
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row;
            TextOut(xpos,ypos,1,3,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][MGetSaveFrequency()]+2,4);
            // row = 1;//存储频谱
            // if( row >= row_number)break;
            // ypos = para_ypos + crow_height *row ;
            // TextOut(xpos,ypos,1,3,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][MGetSaveFrequency()]+2,4);
            //6．	参量保护：全部\部分
            row = 1;
            if( row >= row_number)break;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row ;
            uint16_t paratemp=0;
            int j;
            for(j = 0; j < C_LOCK_ALL; j++)
            {
                if(MGetParaLock(0,j) == 1)paratemp++;
            }
            epText = (uint8_t *)_ParaMenu_AD6[MGetLanguage()][paratemp];
            TextOut(xpos,ypos,1,4,crow_height,epText,4);

            row = 3;//计量单位
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row;
            epText = (uint8_t*)_ParaMenu_CA1 + MGetUnitType() * 6 + 2;
            EMenuOut(xpos,ypos , epText ,4,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/

            break;

        }

        MKeyRlx();		/*只有按键已释放才退出*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if ( (keycode > 0 && keycode <= row_number)
                    || ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
               )
            {
                //if(  !((MGetStdMode()==10||MGetStdMode() ==11)&&keycode == 5)  )
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
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {

            if( old_unit1 != old_unit )//长度单位有改变
            {
                if( old_unit1 == 0)
                {
                    //新单位inch
                    MSetThick( (uint32_t)(MGetThick()/0.254+0.5) );
                    MSetDiameter( (uint32_t)(MGetDiameter()/0.254+0.5) );

                    stWeldPara.face = stWeldPara.face/0.254+0.5;
                    stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                    stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                    stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                    stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                    stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                    stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                    stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                    stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                    stWeldPara.space = stWeldPara.space/0.254+0.5;
                    stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                }
                else
                {
                    MSetThick( (uint32_t)(MGetThick()*25.4/100+0.5) );
                    MSetDiameter( (uint32_t)(MGetDiameter()*25.4/100+0.5) );

                    stWeldPara.face = stWeldPara.face*0.254+0.5;
                    stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                    stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                    stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                    stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                    stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                    stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                    stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                    stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                    stWeldPara.space = stWeldPara.space*0.254+0.5;
                    stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                }
                old_unit1=MGetUnitType();
            }
            SystemParaStore();
            ChannelParaStore();
            deci_len = MGetChannel();//现在通道
            if( old_unit != MGetUnitType() )//长度单位有改变
            {
                int keycodetemp;
                if( old_unit == 0)
                {
                    //新单位inch
                    keycodetemp = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测零点的三个值
                    keycodetemp = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测K值的三个值
                    keycodetemp = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number * 100/25.4+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测DAC的三个值
                }
                else
                {
                    keycodetemp = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测零点的三个值
                    keycodetemp = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测K值的三个值
                    keycodetemp = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    keycodetemp += 4;
                    MCopyProtectedMemory( &number, (void*)keycodetemp, 4, PM_COPY_READ);
                    number = (uint32_t)(number*25.4/100+0.5);
                    MCopyProtectedMemory( (void*)keycodetemp, &number, 4, PM_COPY_WRITE);
                    //以上为测DAC的三个值
                }

                for( sign = 0; sign < C_CHMAX; sign++)
                {
                    MSetChannel(sign,C_SETMODE_SETSAVE);
                    SystemParaStore();
                    ChannelParaRestore();
                    if( old_unit == 0)
                    {
                        //新单位inch
                        MSetSpeed( (uint32_t)(ChannelPara.Speed/0.254+0.5) ,C_SETMODE_SAVE);
                        MSetThick( (uint32_t)(MGetThick()/0.254+0.5) );
                        MSetDiameter( (uint32_t)(MGetDiameter()/0.254+0.5) );
                        MSetCrystal_l(ChannelPara.Crystal_l/25.4+0.5,C_SETMODE_SETSAVE);
                        MSetCrystal_w(ChannelPara.Crystal_w/25.4+0.5,C_SETMODE_SETSAVE);
                        MSetForward((uint32_t)( MGetForward()/0.254+0.5),C_SETMODE_SAVE);
                        MSetRange( (uint32_t)( MGetRange(1)/0.254+0.5 ),C_SETMODE_SAVE);

                        stWeldPara.face = stWeldPara.face/0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                        stWeldPara.space = stWeldPara.space/0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                        //////
                    }
                    else
                    {
                        MSetSpeed( (uint32_t)(ChannelPara.Speed*0.254+0.5) ,C_SETMODE_SAVE);
                        MSetThick( (uint32_t)(MGetThick()*25.4/100+0.5) );
                        MSetDiameter( (uint32_t)(MGetDiameter()*25.4/100+0.5) );
                        MSetCrystal_l((ChannelPara.Crystal_l)*25.4+0.5,C_SETMODE_SETSAVE);
                        if (ChannelPara.Crystal_w>0)
                        {
                            MSetCrystal_w((ChannelPara.Crystal_w)*25.4+0.5,C_SETMODE_SETSAVE);
                        }
                        MSetForward((uint32_t)( MGetForward()*25.4/100+0.5),C_SETMODE_SAVE);
                        MSetRange( (uint32_t)( MGetRange(1)*0.254+0.5 ),C_SETMODE_SAVE);						//////

                        stWeldPara.face = stWeldPara.face*0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                        stWeldPara.space = stWeldPara.space*0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                    }

                    ChannelParaStore();
                }

            }
            MSetChannel(deci_len,C_SETMODE_SETSAVE);
            SystemParaStore();
            ChannelParaRestore();
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

        InvertWords(C_ECHAR_HDOT,ypos-MGetLanguage()*C_CCHAR_VDOT, 1); /*反白显示几个字的区域*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//标度
                //	cpText = (uint16_t*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 3+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//表面补偿
                deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;


            case 3:	//工件厚度
                if( MGetUnitType() > 0)deci_len = 3;//单位inch
                else deci_len = 1;
                number = 100;
                while( true )//如未设置连续存则不能输入
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }

                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                int j;
                if(MGetStdMode()==12)//判断厚度是否在范围内
                {

                    if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                            ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                            ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                            ||(MGetBlock()==3&&MGetThick()<40)    )

                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }

                }

                else if(MGetStdMode()==2)//判断厚度是否在范围内
                {
                    if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                            ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                            ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                            ||(MGetBlock()==3&&MGetThick()<40) )
                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }
                }
                else if(MGetStdMode()==4)//判断厚度是否在范围内
                {
                    if( MGetThick()<50)
                    {
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, 0 );
                        }
                    }
                }

                stdswitch=1;
                break;
            case 4:
                if( MGetUnitType() > 0)deci_len = 3;//单位inch
                else deci_len = 1;
                number = 100;
                while( true )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4, &deci_len,0) != 1)
                        break;
                    else if( number < 50000)
                    {
                        MSetDiameter(number);
                        break;
                    }
                }
                break;

            case 5:	//当量标准
            	if(MGetStdMode() == 10 || MGetStdMode() == 11)
				{
					break;
				}
				else if(MGetStdMode()==14)
				{
					MSetDacMode(3,C_SETMODE_SAVE);//10559标准第三根线定量线B为0线
					break;
				}
                //cpText = (uint16_t*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * ( MGetCurveNumber()+1 )+1 ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:	//当量显示
                //	if( MGetTestStatus(C_TEST_AVG) == 1)
                //		cpText = (uint16_t*)_ParaMenu_AC3A ;
                //	else cpText = (uint16_t*)_ParaMenu_AC3 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(uint8_t*)_ParaMenu_AC3A[MGetLanguage()][0],4);
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
                break;
            case 7:		//Rl
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if( MGetTestStatus(C_TEST_AVG) == 1) sign = 0;
                else sign = 1;


                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;

                        }
                        else if(number < -300 || number > 400)break;
                        MSetLineGain(0, number);
                        break;
                    }
                }

                break;
            case 8:
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;

                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            //AVG
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(1, number);
                        break;
                    }
                }

                /*		while( 1 )//如未设置连续存则不能输入
                		{
                			if( Input_Number(xpos,ypos,&number,3, &deci_len,1) != 1)break;
                			else if(number >= -300 && number <= 400)
                			{
                				MSetSlGain(number,C_SETMODE_SAVE);
                				break;
                			}
                		}
                */
                break;
            case 9:	//El
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;

                if(MGetTestStatus(C_TEST_AVG) == 1 && MGetUnitType() > 0)deci_len = 3;
                else deci_len = 1;
                number = 0;
                if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
                else sign = 1;

                while( 1 )
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                    else
                    {
                        if(MGetTestStatus(C_TEST_AVG) == 1)
                        {
                            if(MGetUnitType() > 0)
                            {
                                number = (UINT)(number * 254/1000.0+0.5);
                            }
                            if( number >= 5 && number < 100)
                                number = (int)( 400 * log10( number/10.0) -120);
                            else break;
                        }
                        else if(number < -300 || number > 400)break;

                        MSetLineGain(2, number);
                        break;
                    }
                }
                break;
            }
        }
        else if( page == 2)
        {
            switch (keycode)
            {

#if C_UDISK == 1
            case 1:	//                //*
                if (MGetSaveStatus() > 0)
                {
                    break;
                }
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveMode(keycode-1,C_SETMODE_SETSAVE);
                        if (MGetSaveMode())
                        {
                            MSetSaveStatus( 0,C_SETMODE_SETSAVE);
                            MSetSavePosition(1,C_SETMODE_SETSAVE);
                        }
                        else
                        {
                            MSetSavePosition(0,C_SETMODE_SETSAVE);
                        }
                        /*	if( MGetSaveTime() > 600 || MGetSaveTime() < 1)
                        	{
                        		MSetSaveTime(10,C_SETMODE_SAVE);
                        		offset = C_OFF_STORE +	34;		//已自动存数据个数
                        		number = 0;
                        		MCopyProtectedMemory( (void*)offset, &number,4, PM_COPY_WRITE);
                        	}*/
                        break;
                    }
                }
                MPopWindow();
                //*/
                break;

            case 2:	//存储位置
                // break;
                if (MGetSaveStatus() > 0||MGetSaveMode()>0)
                {
                    break;
                }
                ypos -= crow_height * 1;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+31, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+30, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,10,crow_height,(uint8_t*)_ParaMenu_AC7[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSavePosition(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
                /*#else
                			case 1://计量单位
                				epText = (uint8_t*)_ParaMenu_CA1 ;
                				MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + 12 * 3) ;
                				EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + 12 * 3) ;
                				EMenuOut(xpos+4,ypos+8 , epText ,6,2,12);
                				number = MGetUnitType();
                				while(1)
                				{
                					keycode = MAnyKeyReturn();
                					if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                						break;
                					else if(keycode >= 1 && keycode <= 2)
                					{
                						MSetUnitType(keycode-1);
                						break;
                					}
                				}
                				if( old_unit1 != MGetUnitType() )//长度单位有改变
                				{
                					if( old_unit1 == 0)
                					{//新单位inch
                						MSetThick( (uint32_t)(MGetThick()/0.254+0.5) );
                						MSetDiameter( (uint32_t)(MGetDiameter()/0.254+0.5) );

                						stWeldPara.face = stWeldPara.face/0.254+0.5;
                						stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                						stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                						stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                						stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                						stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                						stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                						stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                						stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                						stWeldPara.space = stWeldPara.space/0.254+0.5;
                						stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                					}
                					else
                					{
                						MSetThick( (uint32_t)(MGetThick()*25.4/100+0.5) );
                						MSetDiameter( (uint32_t)(MGetDiameter()*25.4/100+0.5) );

                						stWeldPara.face = stWeldPara.face*0.254+0.5;
                						stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                						stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                						stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                						stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                						stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                						stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                						stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                						stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                						stWeldPara.space = stWeldPara.space*0.254+0.5;
                						stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                					}
                					old_unit1=MGetUnitType();
                				}
                				MPopWindow();

                				break;
                			case 2://语言
                				MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + 16 * 3) ;
                				EraseDrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + 16 * 3) ;

                				TextOut(xpos+4,ypos+8,1,10,16,(char *)_MainMenu_C1A11[MGetLanguage()][0],4);
                				TextOut(xpos+4,ypos+8+20,1,10,16,(char *)_MainMenu_C1A11[MGetLanguage()][1],4);
                				while(1)
                				{
                					keycode = MAnyKeyReturn();
                					if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                						break;
                					else if(keycode >= 1 && keycode <= 2)
                					{
                						MSetLanguage(keycode-1);
                						break;
                					}
                				}
                				MPopWindow();

                				break;*/
#endif
            case 3:	//位置显示
                //	cpText = (uint16_t*)_ParaMenu_AC6 ;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4,2,8,crow_height,(uint8_t*)_ParaMenu_AC6[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetReadForeland(keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;


            case 4://语言
                MPushWindow(xpos, ypos, xpos + 7*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + 7*C_CCHAR_HDOT, ypos + crow_height * 2) ;

                TextOut(xpos+4,ypos+8,1,10,crow_height,(char *)_MainMenu_C1A11[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+8+30,1,10,crow_height,(char *)_MainMenu_C1A11[MGetLanguage()][1],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetLanguage(keycode-1);
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//B门类型
                if( MGetFunctionMode(C_CURVER_AMEND ) )break;
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage()*3)*C_CCHAR_HDOT+1, ypos + crow_height * 2+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4,1,8+2*MGetLanguage(),crow_height,(uint8_t*)_ParaMenu_AC5[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+4+crow_height,1,8+2*MGetLanguage(),crow_height,(uint8_t*)_ParaMenu_AC5[MGetLanguage()][1],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetBGateMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:
                //	cpText = (uint16_t*)_ParaMenu_AD8 ;
                MPushWindow(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT+2, ypos + crow_height * 2+2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4,2,6,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSoundAlarm(0, keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 7://屏幕亮度
#if C_LIGHT == 1
#if	C_DEVLIB == 1 || C_DEVLIB == 3 || C_DEVLIB == 24 ||C_DEVLIB == 23
                ypos-=C_CCHAR_HDOT+30;
                //	cpText = (uint16_t*)_ParaMenu_AC4 ;
                MPushWindow(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                TextOut(xpos+4,ypos+4 ,4,10,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetScreenBrightness(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#else		//C_DEVLIB == 23		//两项，低亮，高亮
                //	cpText = (uint16_t*)_ParaMenu_AC4 ;
                break;
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,1,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                //	CEMenuOut(xpos+4,ypos+4+crow_height , cpText+3 ,1,1,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                //	CEMenuOut(xpos+4+16,ypos+4+crow_height , cpText+3*3+1 ,2,1,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                TextOut(xpos+4,ypos+4 ,1,10,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+4+crow_height ,1,10,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][4],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode == 1)
                    {
                        MSetScreenBrightness(0,C_SETMODE_SETSAVE);
                        break;
                    }
                    else if(keycode == 2)
                    {
                        MSetScreenBrightness(3,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#endif
#endif
                break;
            case 8:
                deci_len = 0;
                NumHigh = 100;
                while( true )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&NumHigh,2, &deci_len,0) != 1)
                        break;
                    else if(NumHigh >= 40 && NumHigh <= 90)
                    {
                        MSetAmpStdMax( NumHigh);
                        break;
                    }
                }
                break;
            case 9:			//焊缝图示
                if( MGetTestStatus(C_TEST_AVG) == 1)break;

                //	cpText = (uint16_t*)_Weld_C1 ;
                ypos -= crow_height * 3+8+24*MGetLanguage();
                MPushWindow(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+11, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5+MGetLanguage())*C_CCHAR_HDOT+10, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);
                for(i = 0; i < 4; i ++)
                {
                    TextOut(xpos+4,ypos+4+crow_height*i,1,8,16,(uint8_t*)_Weld_C1[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        stWeldPara.type = keycode -1;
                        break;
                    }
                }
                MPopWindow();
                if( ( keycode > 1 && keycode <= 4) || keycode == C_KEYCOD_CONFIRM)
                {
                    //新选中了焊缝类型
                    int i,j,k;
                    if( stWeldPara.type == 3 && stWeldPara.face == 2)
                    {
                        stWeldPara.thick2 = MGetThick();
                    }
                    else
                    {
                        stWeldPara.thick1 = MGetThick();
                    }
                    WeldSetup();
                    if( stWeldPara.type > 0)
                    {
                        if( stWeldPara.type == 3 && stWeldPara.face ==2)
                        {
                            MSetThick( stWeldPara.thick2 );
                            number = stWeldPara.thick2;
                        }
                        else
                        {
                            MSetThick( stWeldPara.thick1 );
                            number = stWeldPara.thick1;
                        }
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                }
                break;

                /*	case 9:	//连续存间隔时间

                	if (MGetSaveStatus() > 0)
                	{
                		break;
                	}
                	deci_len = 1;
                	number = 0;
                	while( MGetSaveMode() )//如未设置连续存则不能输入
                	{
                		if( Input_Number(xpos,ypos,&number,3, &deci_len,0) != 1)
                			break;
                		else if(number >= 1 && number <= 250)
                		{
                			MSetSaveTime(number,C_SETMODE_SAVE);
                			break;
                		}
                	}

                	break;*/
            }
            ExpendTime(20);
        }
        else if( page == 3)
        {
            switch (keycode)
            {

            case 1:	//打印机
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4,2,5,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveFrequency(keycode-1,0);
                        break;
                    }
                }
                if (MGetFrequence() != 0)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MPopWindow();
                break;
            // case 2://是否存频谱
                // MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                // EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                // TextOut(xpos+4,ypos+4,2,5,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
                // while(1)
                // {
                    // keycode = MAnyKeyReturn();
                    // if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        // break;
                    // else if(keycode >= 1 && keycode <= 2)
                    // {
                        // MSetSaveFrequency(keycode-1,0);
                        // break;
                    // }
                // }
                // if (MGetFrequence() != 0)
                // {
                    // MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    // MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                // }
                // MPopWindow();
                // break;
            case 2:
                ypos -= 24;
                //	cpText = (uint16_t*)_ParaProtect1 ;//4-6去掉
                MPushWindow(xpos, ypos, xpos+ (12+7*MGetLanguage())*C_CCHAR_HDOT+1, ypos + 6 * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ (12+7*MGetLanguage())*C_CCHAR_HDOT, ypos + 6 * crow_height) ;
                //	CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,6,5,crow_height);

                int i = 0;
                for (i=0; i<6; i++)
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+i*crow_height,1,12+9*MGetLanguage(),crow_height,(uint8_t*)_ParaProtect1[MGetLanguage()][i],4);
                }
                // if (MGetLanguage())
                // {
                    // TextOut(xpos+ C_ECHAR_HDOT,ypos+4+4*crow_height-8,1,12,8,(uint8_t*)_ParaProtect1[MGetLanguage()][3]+12,0);
                // }
                int ypos1;

                xpos += (6+6*MGetLanguage())*C_CCHAR_HDOT+MGetLanguage();

                for(i = 0; i < C_LOCK_ALL; i ++)
                {
                    ypos1 = ypos + crow_height * (i +2);
                    //	cpText = (uint16_t*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                    //	CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);

                    MEraseWindow(xpos+ C_ECHAR_HDOT-1,ypos1, xpos+ C_ECHAR_HDOT+8*C_ECHAR_HDOT,ypos1+C_ECHAR_VDOT) ;
                    TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(uint8_t*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);

                }

                do
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                        break;

                    if (keycode == 0)
                    {
                        if(MGetParaLock(0,0)==1 || MGetParaLock(0,1)==1 || MGetParaLock(0,2)==1)
                        {
                            MSetParaLock(0, C_LOCK_ALL);	//所有都被保护
                        }
                        else
                        {
                            MSetParaLock(1, C_LOCK_ALL);	//所有都被保护
                        }


                    }
                    else if(keycode >0 && keycode <= C_LOCK_ALL)
                    {
                        MSetParaLock(MGetParaLock(0,keycode-1)+1, keycode-1);	//
                    }
                    else continue;
                    for(i = 0; i < C_LOCK_ALL; i ++)
                    {
                        ypos1 = ypos + crow_height * (i +2);
                        //	cpText = (uint16_t*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                        //	CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                        MEraseWindow(xpos+ C_ECHAR_HDOT-1,ypos1, xpos+ C_ECHAR_HDOT+8*C_ECHAR_HDOT,ypos1+C_ECHAR_VDOT) ;
                        TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(uint8_t*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                    }
                }
                while(1);
                MPopWindow();
                break;
            case 4://计量单位
                epText = (uint8_t*)_ParaMenu_CA1 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT+8*MGetLanguage(), ypos + erow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT+8*MGetLanguage(), ypos + erow_height* 3) ;
                EMenuOut(xpos+4,ypos+8 , epText ,6,2,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                number = MGetUnitType();
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetUnitType(keycode-1);
                        break;
                    }
                }
                if( old_unit1 != MGetUnitType() )//长度单位有改变
                {
                    if( old_unit1 == 0)
                    {
                        //新单位inch
                        MSetThick( (uint32_t)(MGetThick()/0.254+0.5) );
                        MSetDiameter( (uint32_t)(MGetDiameter()/0.254+0.5) );

//                        stWeldPara.face = stWeldPara.face/0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1/0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide/0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2/0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1/0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2/0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius/0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep/0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt/0.254+0.5;
                        stWeldPara.space = stWeldPara.space/0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe/0.254+0.5;
                    }
                    else
                    {
                        MSetThick( (uint32_t)(MGetThick()*25.4/100+0.5) );
                        MSetDiameter( (uint32_t)(MGetDiameter()*25.4/100+0.5) );

//                        stWeldPara.face = stWeldPara.face*0.254+0.5;
                        stWeldPara.thick1 = stWeldPara.thick1*0.254+0.5;
                        stWeldPara.wide = stWeldPara.wide*0.254+0.5;
                        stWeldPara.thick2 = stWeldPara.thick2*0.254+0.5;
                        stWeldPara.high1 = stWeldPara.high1*0.254+0.5;
                        stWeldPara.high2 = stWeldPara.high2*0.254+0.5;
                        stWeldPara.radius = stWeldPara.radius*0.254+0.5;
                        stWeldPara.deep = stWeldPara.deep*0.254+0.5;
                        stWeldPara.blunt = stWeldPara.blunt*0.254+0.5;
                        stWeldPara.space = stWeldPara.space*0.254+0.5;
                        stWeldPara.probe = stWeldPara.probe*0.254+0.5;
                    }
                    old_unit1=MGetUnitType();
                }
                MPopWindow();

                break;
            }
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);

    SetRepeatRate(g_nRepeat);
    if(extend==0)
    {
        MSetPulseWidthInput(MGetPulseWidthNumber(), C_SETMODE_SETSAVE);
    }
    else
    {
        MSetPulseWidth(MGetPulseWidth(), C_SETMODE_SETSAVE);
    }
    g_nDenoise = 0;
    g_nAutoDenoise = 0;
    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetColor(C_CR_WAVE);
    ScreenRenovate();	/*屏幕刷新*/
    ClearEnvelope();	//清包络或峰值
    MSetColor(C_CR_UNDO);
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1 && stdswitch==1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        DrawDac(2);		//新画DAC
        if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==12||MGetStdMode()==9)	//47013
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

        if (MGetThick()>=30 && stdswitch ==1)
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
        MSetSystem();
        MSetColor(C_CR_PARA);
        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
        WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    DrawDac(0);		//新画DAC

    return 1;
}

int ChannelMenu(void)
{
    int FileMax=(C_OFF_FAT+3)/4;//C_OFF_FILEMAX/4;//
    int offset = 0;
	
	int nMode = MGetStdMode();
	USER_INFO UserInfo;
	int offsets = C_OFF_USER_INFO;
	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
	
	float angle;
	uint16_t angles;
	short pages = 1, page_maxs = 4, pagemaxs = 0;
	short row_numbers = 10;
	int numbers, deci_lens, number1s;

    uint16_t char_len = 6,row_number = 10,crow_height = 48,erow_height =20;
    int number,deci_len;//,sign;
    short xpos = 0,ypos = 0;
    //int menu_xpos = 0,menu_ypos = 0;
    short para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    short page_max = 1, page = 1,pagemax=0;
    int row;
    uint16_t* cpText = 0;
    uint8_t* epText = 0;
    uint8_t* epText1 = 0;
    int i,j,k;
    int stdflag=0;
    int testrange[3];

    uint32_t elapsedtime1 = GetElapsedTime();// ,elapsedtime2;
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();



    DisplayPrompt(15);
    para_xpos = C_CCHAR_HDOT * ( char_len + 1) ;
    short row_number_page1, para_ypos_page1;
    short row_number_page2=5, para_ypos_page2=48;
#if  C_DEVTYPE == 3 || C_DEVTYPE == 5
    page_max = 2;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 2;
#elif  C_DEVTYPE == 4
    page_max = 1;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 0;
#elif  C_DEVTYPE == 20
    page_max = 1;
    row_number_page1 = 8;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 0;
#elif  C_DEVTYPE == 12
    row_number_page1 = 6;
    para_ypos_page1 =  56;
#elif C_DEVTYPE == 8  || C_DEVTYPE == 1
    page_max = 2;
    row_number_page1 = 9;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 3;
#elif C_DEVTYPE == 9  || C_DEVTYPE == 14 ||C_DEVTYPE == 15
    page_max = 2;
    row_number_page1 = 9;
    para_ypos_page1 =para_ypos_page2=  48;
    row_number_page2 = 5;
#else
    row_number_page1 = 9;
    para_ypos_page1 =  4;
#endif

	// if(MGetTestStatus(C_TEST_DAC) == 1)
	// {
		// MSetParaLock(1, C_LOCK_PROBE);
	// }
	// else
	// {
		// MSetParaLock(0, C_LOCK_PROBE);
	// }

    while(true)
    {
        MFclearScreen();


        MSetColor(C_CR_MENU);

        TextOut(C_ECHAR_HDOT,4,1,25,24,(uint8_t*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8+6*MGetLanguage(), 7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_max,1,0,0);
        }

        //CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        for (i=0; i<row_number; i++)
        {
            switch(page)
            {
            case 1:
                epText1 = (char *)_ChannelMenu_A1[MGetLanguage()][i];
                epText =  (uint8_t*)_ChannelMenu_B1;
                row_number = row_number_page1;
                para_ypos =  para_ypos_page1-8*MGetLanguage();
                break;
            case 2:
                epText1 =  (char *)_ChannelMenu_A2[MGetLanguage()][i];
                epText =  (uint8_t*)_ChannelMenu_B2;
                row_number = row_number_page2;
                para_ypos =  para_ypos_page2-8*MGetLanguage();
                break;
            }
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,22,crow_height,epText1,0);
            //      if (MGetLanguage())
            {
                //         TextOut(C_ECHAR_HDOT,para_ypos+C_ECHAR_VDOT+crow_height*i,1,11,crow_height,epText1+11,2);
            }
        }
        if(MGetLanguage())
        {
            para_ypos+=C_ECHAR_VDOT;
        }
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos, epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);

        switch(page)
        {
        case 1:
            //	默认标准
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                epText = (uint8_t*)_STDMenu_AB1 + ( MGetStdMode()*C_STD_LEN+2 );
                EMenuOut(xpos,ypos , epText ,C_STD_LEN-2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            }


            /*探头类型 */
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            epText = (char *)_ParaMenu_AB2[MGetLanguage()][MGetProbeMode()]+2;
            TextOut(xpos,ypos,1,11,16,epText,4);
            //	cpText = (uint16_t*)_ParaMenu_AB2 + ( (MGetProbeMode()-0)*4+1 );
            //	CEMenuOut(xpos,ypos , cpText ,3,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            //探头频率
            row = 2;
            ypos = para_ypos + crow_height *row ;
            Write_Number(xpos,ypos,MGetFrequence(),5,2,0);

            //晶片尺寸
            row = 3;
            ypos = para_ypos + crow_height *row ;
            Write_Crystal(xpos,ypos,MGetCrystal_l(),MGetCrystal_w());
            //	EMenuOut(xpos + 5*8,ypos,"mm",2,1,12);

            //折射角
            row = 4;
            ypos = para_ypos + crow_height *row ;
            if (MGetAngle(0)>=C_MAX_ANGLE)
            {
                TextOut(xpos,ypos-6,1,3,10,(char *)_infinity[MGetLanguage()][0],0);
            }
            else
            {
                Write_Number(xpos,ypos,( MGetAngle(1) + 50)/100,4,2,0);	//K value
            }
            Write_Ascii(xpos + 4 * C_ECHAR_HDOT,ypos,'/');
            Write_Number(xpos + 5 * C_ECHAR_HDOT,ypos ,MGetAngle(0),4,1,0);	//angle
            Write_Ascii(xpos + 9 * C_ECHAR_HDOT,ypos ,ANGLE);

            //声速
            row = 5;
            ypos = para_ypos + crow_height *row ;
            ///Write_Number(xpos,ypos,MGetSpeed(),5,0,0);
            WriteSpeed(xpos,ypos );

            //探头前沿
            row = 6;
            if( row < row_number)
            {
                ypos = para_ypos + crow_height *row ;
                ///		Write_Number(xpos,ypos,MGetForward(),4,1,0);
                WriteLongness(xpos,ypos,MGetForward(),5,1);
            }

            //3．	频带宽度：2.5\5.0\wide
            row = 7;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                epText = _ParaMenu_AD3[MGetLanguage()][MGetBandMode()]+2;
                TextOut(xpos,ypos, 1, 4,erow_height, epText, 4);
                //	epText = (uint8_t*)_ParaMenu_AD3 + ( MGetBandMode()*9+3 );
                //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,6,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            }
            //  2．  检波方式：全波\正半波\负半波\射频
            row = 8;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
                epText = (char *)_ParaMenu_AD2[MGetLanguage()][MGetEchoMode()]+2;
                TextOut(xpos,ypos+1,1,8,16,epText,4);
                //	cpText = (uint16_t*)_ParaMenu_AD2 + ( MGetEchoMode()*6+1 );
                //	CEMenuOut(xpos,ypos+1,cpText,4,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            }

            break;
        case 2:
            //4．	探头阻尼：50Q\400Q
            row = 0;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row;
                epText = (uint8_t*)_ParaMenu_AD4 + ( MGetDampMode()*10+3);
                EMenuOut(xpos,ypos+1,epText,7,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            }
            //5．	发射电压：200V\400V
            row = 1;
            if( row < row_number)
            {
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                //	epText = (uint8_t*)_ParaMenu_AD5 + ( MGetPulseMode()*10+3 );
                //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                epText = (char *)_ParaMenu_AD5[MGetLanguage()][MGetPulseMode()]+2;
                TextOut(xpos,ypos,1,6,16,epText,4);
            }

            row = 2;//重复频率
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            //	epText = (uint8_t*)_ParaMenu_CD1 + ( (MGetRepeatRate()/100) - 1) * 8 + 2;
            //	EMenuOut(xpos,ypos , epText ,6,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/
            Write_Number(xpos,ypos,MGetRepeatRate(),4,0,0);

            row = 3;//降噪
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            epText = (uint8_t*)_ParaMenu_CC1 + MGetDenoise() * 6 + 2;
            EMenuOut(xpos,ypos , epText ,4,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/

            row = 4;//脉冲宽度
            if( row >= row_number)break;
            ypos = para_ypos + crow_height *row ;
            if(extend==0)
            {
                Write_Number(xpos,ypos,MGetPulseWidthNumber(),4,0,0);
            }
            else
            {
                epText = (char *)_MainMenu_C1A3[MGetLanguage()][MGetPulseWidth()];
                TextOut(xpos,ypos,1,8,16,epText,4);
            }
            //	cpText = (uint16_t*)_ParaMenu_CE1 +  MGetPulseWidth() * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/
			
			row = 5;//用户参数
			if( row >= row_number)break;
			ypos = para_ypos + crow_height *row ;
			TextOut(xpos,ypos,1,4,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][UserInfo.pzPurchaser[0]]+2,4);
            
			break;
        }////////////
        // keycode = MAnyKeyReturn();

        MKeyRlx();		/*只有按键已释放才退出*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(0);
            if ( (keycode > 0 && keycode <= row_number)|| (keycode == 0 && 10 == row_number)
                    || ( ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) && page_max > 1 )
                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
                    || ( keycode == C_KEYCOD_DEL && extend == 0 
					|| (keycode == 6)|| (keycode == 7))
               )
                break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );



        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();

            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();
            SystemParaStore();
            ChannelParaStore();
            //SaveDefPara(MGetChannel());///3.26

            //  CopyMemoryAllSystem(PM_COPY_WRITE);
			if(nMode!=MGetStdMode())
			{
				UserInfo.pzPurchaser[0] = 1;
				MCopyProtectedMemory((void*)offsets, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
				CopyMemoryToSystem((void*)offsets, C_OFF_BATTERY-C_OFF_USER_INFO, PM_COPY_WRITE);
			}
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
        else if( keycode == C_KEYCOD_DEL)
        {
            if(extend == 0)
            {
                page_max = 2;
                row_number_page1 = 9;
                para_ypos_page1 =para_ypos_page2=  36;
                row_number_page2 = 5;
                continue;
            }
        }


        if(keycode == 0)keycode = 10;
        row = keycode - 1;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-24;

        InvertWords(C_ECHAR_HDOT,ypos+24*(1-MGetLanguage()), 1); /*反白显示几个字的区域*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 2:		/*探头类型 */
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                MPushWindow(xpos, ypos, xpos + (7+2*MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 4+1) ;
                EraseDrawRectangle(xpos, ypos, xpos + (7+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 5) ;

                for (i=0; i<5; i++)
                {
                    epText = (char *)_ParaMenu_AB2[MGetLanguage()][i];
                    TextOut(xpos+4,ypos+4+i*crow_height,1,12,16,epText,4);
                }

                int offset;
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 5)
                    {
                        MSetProbeMode(keycode-1,C_SETMODE_SAVE);	/*设置探头模式*/
                       if(MGetProbeMode() != 1 && MGetProbeMode() != 2 && MGetProbeMode() != 4)	//不是斜探头不是双晶斜
                        {
                            if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//单位inch
                            else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
							if(UserInfo.pzPurchaser[0]==0)
							{
								if(MGetProbeMode() == 0)
								{
									MSetFrequence(UserInfo.pzPurchaser[6],0);
									MSetCrystal_l(UserInfo.pzPurchaser[7],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								else if(MGetProbeMode() == 2)
								{
									MSetFrequence(UserInfo.pzPurchaser[8],0);
									MSetCrystal_l(UserInfo.pzPurchaser[9],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								MSetAngle(0,C_SETMODE_SAVE);
								MSetForward(0,C_SETMODE_SETSAVE);
							}
							else
							{
								if( MGetUnitType() > 0)MSetCrystal_l(20000/25.4,C_SETMODE_SETSAVE);//单位inch
								else MSetCrystal_l(20000,C_SETMODE_SETSAVE);
								MSetCrystal_w(0,C_SETMODE_SETSAVE);
								MSetAngle(0,C_SETMODE_SAVE);
								MSetForward(0,C_SETMODE_SETSAVE);
								MSetFrequence(250,2);
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
								else if (MGetStdMode()==12)	//JB/T47013-2005
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
						else if(MGetProbeMode() == 2 )	//双晶直
                        {
                            if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//单位inch
                            else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
							if(UserInfo.pzPurchaser[0]==0)
							{
								if(MGetProbeMode() == 0)
								{
									MSetFrequence(UserInfo.pzPurchaser[6],0);
									MSetCrystal_l(UserInfo.pzPurchaser[7],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								else if(MGetProbeMode() == 2)
								{
									MSetFrequence(UserInfo.pzPurchaser[8],0);
									MSetCrystal_l(UserInfo.pzPurchaser[9],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								MSetAngle(0,C_SETMODE_SAVE);
								MSetForward(0,C_SETMODE_SETSAVE);
							}
							else
							{
								if( MGetUnitType() > 0)MSetCrystal_l(20000/25.4,C_SETMODE_SETSAVE);//单位inch
								else MSetCrystal_l(20000,C_SETMODE_SETSAVE);
								MSetCrystal_w(0,C_SETMODE_SETSAVE);
								MSetAngle(0,C_SETMODE_SAVE);
								MSetForward(0,C_SETMODE_SETSAVE);
								MSetFrequence(500,2);
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
								else if (MGetStdMode()==12)	//JB/T47013-2005
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
                        else if(MGetProbeMode() == 1)	//斜探头
                        {
							if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//单位inch
								else MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);
							if(UserInfo.pzPurchaser[0]==0)
                        	{
                        		MSetFrequence(UserInfo.pzPurchaser[2],0);
                        		MSetCrystal_l(UserInfo.pzPurchaser[3],0/*mode = 0*/);
								MSetCrystal_w(UserInfo.pzPurchaser[4],0/*mode = 0*/);
								if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_2*1000/254,C_SETMODE_SAVE);//单位inch
	                            else MSetSpeed(CD_SPEED_2,C_SETMODE_SAVE);
	                            MSetScaleMode(2,0);
	                            if(MGetRange(1)>C_MAX_RANGE/2- MGetDelay(3))
	                            {
	                                MSetRange(C_MAX_RANGE/2 - MGetDelay(3), C_SETMODE_SETSAVE) ;

	                            }
	                            MSetScaleMode(MGetScaleMode(),0);

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
								if(MGetStdMode() ==12)
									MSetFrequence(500,2);
								else
									MSetFrequence(250,2);
								if( MGetUnitType() > 0)
								{
									MSetCrystal_l(13000/25.4,C_SETMODE_SETSAVE);
									MSetCrystal_w(13000/25.4,C_SETMODE_SETSAVE);
								}
								else
								{
									MSetCrystal_l(13000,C_SETMODE_SETSAVE);
									MSetCrystal_w(13000,C_SETMODE_SETSAVE);
								}
								MSetScaleMode(2,0);
								if(MGetRange(1)>C_MAX_RANGE/2- MGetDelay(3))
								{
									MSetRange(C_MAX_RANGE/2 - MGetDelay(3), C_SETMODE_SETSAVE) ;

								}
								MSetScaleMode(MGetScaleMode(),0);

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
								else if (MGetStdMode()==12)	//JB/T47013-2005
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
                        else if(MGetProbeMode() == 4 )	//双晶斜
                        {
							if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_3*1000/254,C_SETMODE_SAVE);//单位inch
								else MSetSpeed(CD_SPEED_3,C_SETMODE_SAVE);
							if(UserInfo.pzPurchaser[0]==0)
							{
								if(MGetProbeMode() == 0)
								{
									MSetFrequence(UserInfo.pzPurchaser[6],0);
									MSetCrystal_l(UserInfo.pzPurchaser[7],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								else if(MGetProbeMode() == 2)
								{
									MSetFrequence(UserInfo.pzPurchaser[8],0);
									MSetCrystal_l(UserInfo.pzPurchaser[9],0/*mode = 0*/);
									MSetCrystal_w(0,C_SETMODE_SETSAVE);
									MSetAngle(0,C_SETMODE_SAVE);
	                            	MSetForward(0,C_SETMODE_SETSAVE);
									
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
								MSetAngle(0,C_SETMODE_SAVE);
								MSetForward(0,C_SETMODE_SETSAVE);
							}
							else
							{
								MSetFrequence(500,2);
								if( MGetUnitType() > 0)
								{
									MSetCrystal_l(8000/25.4,C_SETMODE_SETSAVE);
									MSetCrystal_w(12000/25.4,C_SETMODE_SETSAVE);
								}
								else
								{
									MSetCrystal_l(8000,C_SETMODE_SETSAVE);
									MSetCrystal_w(12000,C_SETMODE_SETSAVE);
								}
								MSetAngle(0,C_SETMODE_SETSAVE);
								MSetScaleMode(2,0);
								if(MGetRange(1)>C_MAX_RANGE/2- MGetDelay(3))
								{
									MSetRange(C_MAX_RANGE/2 - MGetDelay(3), C_SETMODE_SETSAVE) ;

								}
								MSetScaleMode(MGetScaleMode(),0);

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
								else if (MGetStdMode()==12)	//JB/T47013-2005
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
						MSetScaleMode(0,C_SETMODE_SAVE);

                        break;
                    }
                }
                MPopWindow();
                break;
            case 3:	//探头频率
                if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_DACAVG) == 1||MGetParaLock(1,C_LOCK_DAC) == 1)break;
                deci_len = 2;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number > 0 && number <= 2000)
                        {
                            MSetFrequence(number,C_SETMODE_SAVE);
                            if( C_DEVLIB != 3)
                            {
                                if(number < 100 || number > 750) MSetBandMode(2,C_SETMODE_SAVE);
                                else if(number >= 100 && number <= 350) MSetBandMode(0,C_SETMODE_SAVE);
                                else if(number <= 750) MSetBandMode(1,C_SETMODE_SAVE);
                                else if(number <= 1250) MSetBandMode(2,C_SETMODE_SAVE);
                            }

                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 4:	//晶片尺寸
                // if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                deci_len = 1;
                number = 100;
                int number1=100;
                while(1)
                {
                    if( Input_Crystal(xpos,ypos,&number,&number1) != 1)break;
                    else
                    {

                        if( number <= 50000 && number1 <= 50000)
                        {
                            MSetCrystal_l(number,C_SETMODE_SAVE);
                            MSetCrystal_w(number1,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 5:	//折射角
                if( MGetParaLock(1,C_LOCK_OFFSET) == 1||MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                number = 0;
                deci_len = 2;
                if ( Input_Number(xpos,ypos,&number,2, &deci_len,0) == 1)
                {
                    if(number <= 500)
                    {
                        /*斜探头输入K值*/
                        MSetKvalue(number,C_SETMODE_SAVE);
                    }
                    else if(number <= C_MAX_ANGLE*10)
                    {
                        number /=10;
                        MSetAngle(number,C_SETMODE_SAVE);	/*设置当前通道	*/
                    }
                }
                break;

            case 6:	//声速
                if( MGetParaLock(1,C_LOCK_OFFSET) == 1||MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                if( MGetUnitType() > 0)deci_len = 2;//单位inch
                else deci_len = 0;
                number = 100;
                int MaxSpeed = C_MAX_SPEED,MinSpeed = C_MIN_SPEED;
                if (MGetUnitType())
                {
                    MaxSpeed = C_MAX_SPEED*100/25.4+0.5;
                    MinSpeed = C_MIN_SPEED*100/25.4+0.5;
                }
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,5, &deci_len,0) != 1)break;
                    else
                    {
                        if(number >= MinSpeed && number <= MaxSpeed)
                        {
                            MSetSpeed(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 7:	//前沿
				if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_OFFSET) == 1)break;
                if( MGetUnitType() > 0)deci_len = 3;//单位inch
                else deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if(( number <= 3000 && MGetUnitType()==0)||( number <= 11000 && MGetUnitType()==1))
                        {
                            MSetForward(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }
                break;
            case 1:		//默认标准
                if( MGetTestStatus(C_TEST_AVG) == 1)break;//已做AVG则不显示标准菜单
                if( MGetParaLock(1,C_LOCK_DACAVG) == 1)break;	//参量保护
                ///ypos -= 4 * crow_height;
                MPushWindow(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT+1, ypos + (C_STD_MAX+1) * erow_height+1) ;
                //EraseDrawRectangle(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT, ypos + (C_STD_MAX+1) * erow_height) ;
                //EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_STDMenu_AB1,C_STD_LEN,C_STD_MAX,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                /*
                while(1)
                {
                    //keycode = MenuKeyReturn(row,2);	//出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效
                    keycode = MAnyKeyReturn();	//出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效
                    stdflag=1;
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 0 && keycode <= C_STD_MAX|| keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS)
                    {
                        if (keycode==0)
                        {
                            keycode=10;
                        }
                		if (keycode == C_KEYCOD_PLUS)//change
                		{
                			keycode=11;
                		}
                		if (keycode == C_KEYCOD_MINUS)//change
                		{
                			keycode=12;
                		}
                        MSetStdMode(keycode-1,C_SETMODE_SAVE);
                        MSetBlock(0);
                		if(keycode == 11 || keycode == 12)
                		MSetDacMode(0,0);
                		else
                        MSetDacMode(2,0);

                        MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                        number = MGetThick()/10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        for( i = 0 ; i < k; i++)
                        {
                			if(MGetStdMode()==0)break;
                            if( number >=  c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }

                        break;
                    }
                }*/

                stdflag=1;
                while(1)
                {
                    EraseDrawRectangle(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT, ypos + 12 * erow_height) ;

                    if(pagemax==0)
                        EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1+9*C_STD_LEN*pagemax,C_STD_LEN,9,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                    else
                        EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1+9*C_STD_LEN*pagemax,C_STD_LEN,7,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                    Write_Number(xpos + 11* C_ECHAR_HDOT, ypos+10*erow_height+6,pagemax+1,1,0,0);
                    TextOut(xpos+12*C_ECHAR_HDOT,ypos+10*erow_height+4,1,3,24,"/2",4);

                    while(1)
                    {
                        keycode=MAnyKeyReturn();
                        if( (keycode>=1&&keycode<=9)||(keycode==C_KEYCOD_PLUS)||(keycode==C_KEYCOD_MINUS)||(keycode==C_KEYCOD_CONFIRM)||(keycode==C_KEYCOD_RETURN) )
                            break;

                    }

                    if(keycode>=1&&keycode<=9&&(pagemax==0||pagemax==1))
                    {
                        if(pagemax==0)
                        {
                            if(keycode==2) keycode=11;
                            else if(keycode==3) keycode=12;
                            else if(keycode==4) keycode=13;
                            else if(keycode==5) keycode=8;
                            else if(keycode==6) keycode=2;
                            else if(keycode==7) keycode=4;
                            else if(keycode==8) keycode=5;
                            else if(keycode==9) keycode=6;
                            MSetStdMode(keycode-1,C_SETMODE_SAVE);
                            MSetBlock(0);
                            //MSetDacMode(2,0);
                            if(keycode==11||keycode==12)
                                MSetDacMode(0,0);
                            else
                                MSetDacMode(2,0);

                            MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                            k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                            number = MGetThick()/10;
                            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                            for( i = 0 ; i < k; i++)
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

                                for( j = 0; j < MGetCurveNumber(); j++)
                                {
                                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                                }
                                break;
                            }

                        }
                        else
                        {
                            if(keycode==1) keycode=7;
                            else if(keycode==2) keycode=9;
                            else if(keycode==3) keycode=10;
							else if(keycode==4) keycode=3;
							else if(keycode==5) keycode=14;
							else if(keycode==6) keycode=15;
							else if(keycode==7) keycode=16;
                            MSetStdMode(keycode-1,C_SETMODE_SAVE);
                            MSetBlock(0);

                            MSetDacMode(2,0);

                            MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                            k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                            number = MGetThick()/10;
                            if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                            for( i = 0 ; i < k; i++)
                            {

                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                                for( j = 0; j < MGetCurveNumber(); j++)
                                {
                                    MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                                }
                                break;
                            }
                        }
                        if(MGetStdMode()==12)//47013
                        {
                            int testrange[3];
                            int offset;
                            testrange[0] = 800;
                            testrange[1] = 20;
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
                        else if(MGetStdMode()==2)//4730
                        {
                            int testrange[3];
                            int offset;
                            testrange[0] = 500;
                            testrange[1] = 10;
                            testrange[2] = 60;
                            offset = C_OFF_TEMP + 30;
                            MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                        }

                        break;
                    }
                    else if(keycode==C_KEYCOD_PLUS)
                    {
                        pagemax++;
                        if(pagemax>1) pagemax=0;
                        continue;
                    }
                    else if(keycode==C_KEYCOD_MINUS)
                    {
                        pagemax--;
                        if(pagemax<0)  pagemax=1;
                        continue;
                    }
                    else if(keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                        break;
                }
				if( MGetParaLock(0,C_LOCK_PROBE) == 1||MGetParaLock(0,C_LOCK_DAC) == 1)
				{
				}
				else 
				{
					if(MGetStdMode()==12&&MGetProbeMode()==1)
						MSetFrequence(500,2);
					else if(MGetProbeMode()==2||MGetProbeMode()==4)
						MSetFrequence(500,2);
					else
						MSetFrequence(250,2);
				}
                MPopWindow();
                break;
			case 8://滤波
                if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_DACAVG) == 1||MGetParaLock(1,C_LOCK_DAC) == 1)break;
                ypos-=crow_height*2;
                MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT+1, ypos + 4 * crow_height+5+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 4 * crow_height+5) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_ParaMenu_AD3,9,3,15);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                TextOut(xpos + 4,ypos+4, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][0], 4);
                TextOut(xpos + 4,ypos+4+crow_height, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][1], 4);
                TextOut(xpos + 4,ypos+4+crow_height*2, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][2], 4);
                TextOut(xpos + 4,ypos+4+crow_height*3, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][3], 4);

                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 5)
                    {
                        MSetBandMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 9:
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                //cpText = (uint16_t*)_ParaMenu_AD2 ;
                ypos -= 3 * crow_height;
#if C_DEVLIB != 3
                deci_len = 4;
#else
                deci_len =2;
#endif
                MPushWindow(xpos, ypos, xpos+ 6*C_CCHAR_HDOT+1, ypos + deci_len * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + deci_len * crow_height) ;
                for (i=0; i<deci_len; i++)
                {
                    TextOut(xpos+C_ECHAR_HDOT,ypos+ i * crow_height+4,1,8,16,_ParaMenu_AD2[MGetLanguage()][i],4);
                }
                //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,5,deci_len,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= deci_len)
                    {
                        MSetEchoMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            }
        }
        else if( page == 2)
        {
            switch(keycode)
            {

            case 1:	//探头阻尼
                if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_DACAVG) == 1)break;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT+1, ypos + 3 * erow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;

                EMenuOut(xpos + C_ECHAR_HDOT,ypos+6,_ParaMenu_AD4,10,2,20);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetDampMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//发射电压
                if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_DACAVG) == 1)break;
#if C_DEVLIB == 23 || C_DEVLIB == 24
                //	ypos -= 5 * erow_height;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT+1, ypos + 4 * crow_height+1) ;
                EraseDrawRectangle(xpos, ypos , xpos+ 12*C_ECHAR_HDOT, ypos + 4 * crow_height) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 ,_ParaMenu_AD5,10,4,17);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][0],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][1],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*2 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][2],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*3 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][3],4);
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SAVE);
                        //SetTransmitterVoltage(LOW_PULSE+keycode-1) ;
                        break;
                    }
                }
                MPopWindow();
#else

                //	ypos -= 3 * erow_height;
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EraseDrawRectangle(xpos, ypos , xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 ,_ParaMenu_AD5,10,2,17);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#endif

                break;

            case 3://重复频率
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                deci_len = 0;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                    else
                    {
                        if(number >= 10 && number <= 1000)
                        {
                            MSetRepeatRate(number,C_SETMODE_SAVE);
                            break;
                        }
                    }
                }
                break;
            case 4://降噪
                if( MGetParaLock(1,C_LOCK_PROBE) == 1)break;
                epText = (uint8_t*)_ParaMenu_CC1 ;
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + erow_height * 6) ;
                EraseDrawRectangle(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + erow_height * 6) ;
                EMenuOut(xpos+4,ypos+8 , epText ,6,5,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 0 && keycode <= 4)
                    {
                        g_nDenoise = keycode;
                        if( g_nRepeat < 100 * g_nDenoise + 100)g_nRepeat = 100 * g_nDenoise + 100;
                        MSetDenoise(g_nDenoise, C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5://脉冲宽度
                if( MGetParaLock(1,C_LOCK_PROBE) == 1||MGetParaLock(1,C_LOCK_DACAVG) == 1)break;
                if(extend==0)
                {
                    deci_len = 0;
                    number = 0;
                    while(1)
                    {
                        if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)break;
                        else
                        {
                            if(number >= 1 && number <= 100)
                            {
                                MSetPulseWidthInput(number, C_SETMODE_SETSAVE);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    MSetPulseWidth(MGetPulseWidth()+1,C_SETMODE_SETSAVE);
                }
                break;
			case 6://用户参数
			    MSetColor(C_CR_MENU);
			    para_ypos = 48-8*MGetLanguage();
			    TextOut(C_ECHAR_HDOT,para_ypos+crow_height*5,1,22,crow_height, (char *)_ChannelMenu_A2[MGetLanguage()][5],0);
				MPushWindow(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT+2, ypos + crow_height * 2+2);
				EraseDrawRectangle(xpos, ypos, xpos + (4+MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 2);
				TextOut(xpos+4,ypos+4,2,6,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][0],4);
                if(UserInfo.pzPurchaser[0]==0)
                    InvertWords(xpos+4,270,1);
                else
                    InvertWords(xpos+4,315,1);
				while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode == 1)
                    {
						while(1)
						{
							MFclearScreen();
							MSetColor(C_CR_MENU);

							TextOut(C_ECHAR_HDOT,4,1,25,24,(uint8_t*)_ParaMenu_H1[MGetLanguage()][0],4);
							Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4, 4+3*MGetLanguage(),pages,1,0,0);

							if (MGetLanguage())
					        {
					            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8+6*MGetLanguage(), 7,page_maxs,1,0,0);
					        }
					        else
					        {
					            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4, 4,page_maxs,1,0,0);
					        }

							for (i=0; i<row_numbers; i++)
					        {
					            switch(pages)
					            {
					            case 1:
					                epText = (char *)_ChannelMenu_A1S[MGetLanguage()][i];
									row_numbers = 9;
					                para_ypos = para_ypos_page1-8*MGetLanguage();
					                break;
					            case 2:
									epText = (char *)_ChannelMenu_A2S[MGetLanguage()][i];
									row_numbers = 8;
									para_ypos = para_ypos_page1-8*MGetLanguage();
					                break;
								case 3:
									epText = (char *)_ChannelMenu_A3S[MGetLanguage()][i];
									row_numbers = 7;
									para_ypos = para_ypos_page1-8*MGetLanguage();
                                    break;
                                case 4:
									epText = (char *)_ChannelMenu_A4S[MGetLanguage()][i];
									row_numbers = 9;
									para_ypos = para_ypos_page1-8*MGetLanguage();
                                    break;
					            }
					            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,40,crow_height,epText,0);	
					        }

							if (pages==1)
							{
						        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12));
						        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
						        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
							}

							MSetColor(C_CR_PARA);

							switch(pages)
							{
					        case 1:
					            //默认标准
					            row = 0;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								epText = (uint8_t*)_STDMenu_AB1 + (UserInfo.pzPurchaser[1]*C_STD_LEN+2);
								EMenuOut(xpos,ypos , epText ,C_STD_LEN-2,1,crow_height);

								//发射电压
								row = 1;
					            xpos = para_xpos;
					            ypos = para_ypos + crow_height *row;
								epText = (char *)_ParaMenu_AD5[MGetLanguage()][UserInfo.pzPurchaser[5]]+2;
								TextOut(xpos,ypos,1,6,16,epText,4);

                                //默认探头类型
                                row = 2;
					            xpos = para_xpos;
					            ypos = para_ypos + crow_height *row;
                                epText = (char *)_ParaMenu_AB2[MGetLanguage()][UserInfo.pzPurchaser[10]]+2;
                                TextOut(xpos+50,ypos,1,8,16,epText,4);
                                
					            //斜探头默认频率
					            row = 3;
					            xpos = para_xpos;
					            ypos = para_ypos + crow_height *row;
								Write_Number(xpos+70,ypos,UserInfo.pzPurchaser[2],5,2,0);

					            //斜探头默认晶片尺寸
					            row = 4;
					            ypos = para_ypos + crow_height *row;
					            Write_Crystals(xpos+120,ypos,UserInfo.pzPurchaser[3],UserInfo.pzPurchaser[4]);

					            //直探头默认频率
					            row = 5;
					            ypos = para_ypos + crow_height *row ;
								Write_Number(xpos+70,ypos,UserInfo.pzPurchaser[6],5,2,0);

					            //直探头默认晶片尺寸
					            row = 6;
					            ypos = para_ypos + crow_height *row ;
								angle = (float)retvalue * C_PI/1800.0;
								Write_Crystals(xpos+120,ypos,UserInfo.pzPurchaser[7],0);

					            //双晶直探头默认频率
					            row = 7;
								ypos = para_ypos + crow_height *row ;
								Write_Number(xpos+120,ypos,UserInfo.pzPurchaser[8],5,2,0);

					            //双晶直探头默认晶片尺寸
					            row = 8;
								ypos = para_ypos + crow_height *row ;
								Write_Crystals(xpos+160,ypos,UserInfo.pzPurchaser[9],0);
								
					            break;
					        case 2:
					            //调试零点、角度时，波峰记忆自动开启
					            row = 0;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								TextOut(xpos+300,ypos,1,3,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][UserInfo.pzSerial[0]]+2,4);
								
					            //调试零点、角度时，回波包络自动开启
					            row = 1;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								TextOut(xpos+300,ypos,1,3,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][UserInfo.pzSerial[1]]+2,4);
								
								//斜探头测零点一次回波默认
					            row = 2;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+180,ypos,UserInfo.pzSerial[2],5,1,0);

								//斜探头测零点二次回波默认0
					            row = 3;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+180,ypos,UserInfo.pzSerial[3],5,1,0);
                                
								//直探头测零点一次回波默认
					            row = 4;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+180,ypos,UserInfo.pzSerial[16],5,1,0);

								//直探头测零点二次回波默认0
					            row = 5;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+180,ypos,UserInfo.pzSerial[17],5,1,0);
                                
                                //双晶直探头测零点一次回波默认
					            row = 6;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+230,ypos,UserInfo.pzSerial[18],5,1,0);

								//双晶直探头测零点二次回波默认0
					            row = 7;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+230,ypos,UserInfo.pzSerial[19],5,1,0);
                                
					            break;
                            case 3:
                                //测角度默认反射体直径
								row = 0;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+130,ypos,UserInfo.pzSerial[4],5,1,0);
								
								//测角度默认反射体中心深度
								row = 1;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+180,ypos,UserInfo.pzSerial[5],5,1,0);

								//标称K值折射角
								row = 2;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								if(UserInfo.pzSerial[15]<=500)	
								{
									angles = (int)( atan( (float)UserInfo.pzSerial[15]/100.0) *1800/C_PI + 0.5);
								}
								else if(UserInfo.pzSerial[15]<= C_MAX_ANGLE*10)
								{
									angles = UserInfo.pzSerial[15]/10;
								}
								Write_Number(xpos + 8 * C_ECHAR_HDOT,ypos,angles,4,1,0);//angle
								float angless = (float)angles * C_PI/1800.0;
								Write_Number(xpos+60,ypos,((int)(tanf(angless) * 10000.0 + 0.5) + 50)/100,4,2,0);//K value
								Write_Ascii(xpos + 7 * C_ECHAR_HDOT,ypos,'/');
								Write_Ascii(xpos + 12 * C_ECHAR_HDOT,ypos ,ANGLE);

								//斜探头制作DAC曲线表面补偿
								row = 3;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+210,ypos,UserInfo.pzGovernor[1],5,1,0);

								//直探头制作DAC曲线表面补偿
								row = 4;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+210,ypos,UserInfo.pzGovernor[2],5,1,0);

								//双晶直探头制作DAC曲线表面补偿
								row = 5;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+250,ypos,UserInfo.pzGovernor[3],5,1,0);

								//DAC曲线末端直线显示
								row = 6;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								TextOut(xpos+130,ypos,1,3,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][UserInfo.pzGovernor[0]]+2,4);
                                
                                break;
							case 4:
								//斜探头制作DAC曲线默认最大深度
								row = 0;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+250,ypos,UserInfo.pzSerial[6],5,1,0);
								
								//斜探头制作DAC曲线默认发射体直径
								row = 1;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+270,ypos,UserInfo.pzSerial[7],5,1,0);

								//斜探头制作DAC曲线默认发射体长度
								row = 2;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+270,ypos,UserInfo.pzSerial[8],5,1,0);

								//直探头制作DAC曲线默认最大深度
								row = 3;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+250,ypos,UserInfo.pzSerial[9],5,1,0);

								//直探头制作DAC曲线默认发射体直径
								row = 4;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+270,ypos,UserInfo.pzSerial[10],5,1,0);

								//直探头制作DAC曲线默认发射体长度
								row = 5;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+270,ypos,UserInfo.pzSerial[11],5,1,0);

								//双晶直探头制作DAC曲线默认最大深度
								row = 6;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+300,ypos,UserInfo.pzSerial[12],5,1,0);

								//双晶直探头制作DAC曲线默认发射体直径
								row = 7;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+320,ypos,UserInfo.pzSerial[13],5,1,0);

								//双晶直探头制作DAC曲线默认发射体长度
								row = 8;
								xpos = para_xpos;
								ypos = para_ypos + crow_height *row;
								Write_Number(xpos+320,ypos,UserInfo.pzSerial[14],5,1,0);

								break;
					        }

							MKeyRlx();		/*只有按键已释放才退出*/
					        do
					        {
					        	if(pages==1)
					        	{
						            if( GetElapsedTime() > elapsedtime1 + 200 )
						            {
						                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
						                elapsedtime1 = GetElapsedTime() ;
						            }
					        	}
					            keycode = MGetKeyCode(0);
					            if ( (keycode > 0 && keycode <= row_numbers)|| (keycode == 0 && 10 == row_numbers)
					                    || ( ( keycode == C_KEYCOD_PLUS || keycode == C_KEYCOD_MINUS) && page_maxs > 1 )
					                    || ( keycode == C_KEYCOD_LEFT || keycode == C_KEYCOD_RIGHT)
					                    || ( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN )
					                    || ( keycode == C_KEYCOD_DEL && extend == 0 )
					               )
					                break;
					            else
					            {
					                keycode = MGetRotary(-1);
					                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
					                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
					                else continue;
					                break;
					            }
					        }
					        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );

							if (keycode == C_KEYCOD_RETURN)
					        {
					        	MCopyProtectedMemory(&UserInfo, (void*)offsets, sizeof(USER_INFO), PM_COPY_READ);
					            retvalue = C_FALSE;
					            break;
					        }
							else if (keycode == C_KEYCOD_CONFIRM)
							{
								UserInfo.pzPurchaser[0] = 0;
								MCopyProtectedMemory((void*)offsets, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
								CopyMemoryToSystem((void*)offsets, C_OFF_BATTERY-C_OFF_USER_INFO, PM_COPY_WRITE);
								retvalue = C_TRUE;
								break;
							}
							if (keycode == C_KEYCOD_RIGHT)
							{
								pages++;
								if(pages > page_maxs)pages = 1;
								continue;
							}
							else if (keycode == C_KEYCOD_LEFT)
							{
								pages--;
								if(pages == 0)pages = page_maxs;
								continue;
							}

							if(keycode == 0)keycode = 10;
							row = keycode - 1;
							xpos = para_xpos;
							ypos = para_ypos + crow_height * row-24;
							InvertWords(C_ECHAR_HDOT,ypos+24*(1-MGetLanguage()), 1); /*反白显示几个字的区域*/

							if(pages == 1)
							{
								switch(keycode)
								{
									case 1://默认标准
										while(1)
										{
											EraseDrawRectangle(xpos, ypos, xpos+ (C_STD_LEN+2)*C_ECHAR_HDOT, ypos + 12 * erow_height) ;
											if(pagemaxs==0)
												EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1+9*C_STD_LEN*pagemaxs,C_STD_LEN,9,erow_height);
											else
												EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1+9*C_STD_LEN*pagemaxs,C_STD_LEN,4,erow_height);
											Write_Number(xpos + 11* C_ECHAR_HDOT, ypos+10*erow_height+6,pagemaxs+1,1,0,0);
											TextOut(xpos+12*C_ECHAR_HDOT,ypos+10*erow_height+4,1,3,24,"/2",4);

											while(1)
											{
												keycode=MAnyKeyReturn();
												if( (keycode>=1&&keycode<=9)||(keycode==C_KEYCOD_PLUS)||(keycode==C_KEYCOD_MINUS)||(keycode==C_KEYCOD_CONFIRM)||(keycode==C_KEYCOD_RETURN) )
													break;
											}

											if(keycode>=1&&keycode<=9&&(pagemaxs==0||pagemaxs==1))
											{
												if(pagemax==0)
												{
													if(keycode==2) keycode=11;
		                    						else if(keycode==3) keycode=12;
		                    						else if(keycode==4) keycode=13;
		                    						else if(keycode==5) keycode=8;
		                    						else if(keycode==6) keycode=2;
		                    						else if(keycode==7) keycode=4;
		                    						else if(keycode==8) keycode=5;
		                    						else if(keycode==9) keycode=6;
												}
												else
												{
													if(keycode==1) keycode=7;
													else if(keycode==2) keycode=9;
													else if(keycode==3) keycode=10;
													else if(keycode==4) keycode=3;
												}

												if(keycode==11||keycode==12)
													UserInfo.pzSerial[7] = 30;
												else
													UserInfo.pzSerial[7] = 20;
												UserInfo.pzPurchaser[1] = keycode -1;
												
												break;
											}
											else if(keycode==C_KEYCOD_PLUS)
											{
												pagemaxs++;
												if(pagemaxs>1)
													pagemaxs=0;
												continue;
											}
											else if(keycode==C_KEYCOD_MINUS)
											{
												pagemaxs--;
												if(pagemaxs<0)
													pagemaxs=1;
												continue;
											}
											else if(keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
												break;
										}
										
										break;
									case 2://发射电压
										MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT+1, ypos + 4 * crow_height+1) ;
										EraseDrawRectangle(xpos, ypos , xpos+ 12*C_ECHAR_HDOT, ypos + 4 * crow_height) ;
										TextOut(xpos + C_ECHAR_HDOT,ypos + 6 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][0],4);
										TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][1],4);
										TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*2 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][2],4);
										TextOut(xpos + C_ECHAR_HDOT,ypos + 6+crow_height*3 ,4,6,crow_height,(char *)_ParaMenu_AD5[MGetLanguage()][3],4);
										while(1)
						                {
						                    keycode = MenuKeyReturn(4,2);
						                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
						                        break;
						                    else if(keycode >= 1 && keycode <= 4)
						                    {
						                    	UserInfo.pzPurchaser[5] = keycode-1;
						                        break;
						                    }
										}
										break;
                                    case 3:
                                        MPushWindow(xpos, ypos, xpos + (7+2*MGetLanguage())*C_CCHAR_HDOT+1, ypos + crow_height * 4+1) ;
                                        EraseDrawRectangle(xpos, ypos, xpos + (7+2*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * 5) ;
                                        for (i=0; i<5; i++)
                                        {
                                            epText = (char *)_ParaMenu_AB2[MGetLanguage()][i];
                                            TextOut(xpos+4,ypos+4+i*crow_height,1,12,16,epText,4);
                                        }
                                        while(1)
                                        {
                                            keycode = MAnyKeyReturn();
                                            if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                                                break;
                                            else if(keycode >= 1 && keycode <= 5)
                                                UserInfo.pzPurchaser[10] = keycode-1;
                                            break;
                                        }
                                        break;
									case 4://斜探头默认频率
										deci_lens = 2;
										numbers = 100;
										if(Input_Number(xpos+70,ypos,&numbers,2,&deci_lens,0) != 1)break;
										else
										{
											if( numbers > 0 && numbers <= 2000)
												UserInfo.pzPurchaser[2] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 5://斜探头默认晶片尺寸
										numbers = 100;
										number1s = 100;
										if(Input_Crystal(xpos+120,ypos,&numbers,&number1s) != 1)break;
										else
										{
											if( numbers <= 50000 && number1s <= 50000)
											{
												UserInfo.pzPurchaser[3] = numbers;
												UserInfo.pzPurchaser[4] = number1s;
											}
											else
												DisplayPrompt(4);
										}
										break;
									case 6://直探头默认频率
										deci_lens = 2;
										numbers = 100;
										if(Input_Number(xpos+70,ypos,&numbers,2,&deci_lens,0) != 1)break;
										else
										{
											if( numbers > 0 && numbers <= 2000)
												UserInfo.pzPurchaser[6] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 7://直探头默认晶片尺寸
										numbers = 100;
										number1s = 100;
										if(Input_Crystal(xpos+120,ypos,&numbers,&number1s) != 1)break;
										else
										{
											if( numbers <= 50000 && number1s <= 50000)
												UserInfo.pzPurchaser[7] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 8://双晶直探头默认频率
										deci_lens = 2;
										numbers = 100;
										if(Input_Number(xpos+120,ypos,&numbers,2, &deci_lens,0) != 1)break;
										else
										{
											if( numbers > 0 && numbers <= 2000)
												UserInfo.pzPurchaser[8] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 9://双晶直探头默认晶片尺寸
										numbers = 100;
										number1s = 100;
										if(Input_Crystal(xpos+160,ypos,&numbers,&number1s) != 1)break;
										else
										{
											if( numbers <= 50000 && number1s <= 50000)
												UserInfo.pzPurchaser[9] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
								}
							}
							else if(pages == 2)
							{
								switch(keycode)
								{
									case 1:
										EraseDrawRectangle(xpos+300, ypos, xpos + 4*C_CCHAR_HDOT+300, ypos + crow_height * 2);
										TextOut(xpos+304,ypos+4,2,5,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
										while(1)
										{
											keycode = MAnyKeyReturn();
											if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
												break;
											else if(keycode >= 1 && keycode <= 2)
											{
												UserInfo.pzSerial[0] = keycode-1;
												if(keycode==2)
													UserInfo.pzSerial[1] = 0;
												break;
											}
										}
										break;
									case 2:
										EraseDrawRectangle(xpos+300, ypos, xpos + 4*C_CCHAR_HDOT+300, ypos + crow_height * 2);
										TextOut(xpos+304,ypos+4,2,5,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
										while(1)
										{
											keycode = MAnyKeyReturn();
											if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
												break;
											else if(keycode >= 1 && keycode <= 2)
											{
												UserInfo.pzSerial[1] = keycode-1;
												if(keycode==2)
													UserInfo.pzSerial[0] = 0;
												break;
											}
										}
										break;
									case 3:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+180,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[2] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 4:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+180,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[3] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 5:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+180,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[16] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 6:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+180,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[17] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 7:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+230,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[18] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
                                    case 8:
                                        deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+230,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[19] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
								}
							}
                            else if(pages == 3)
                            {
                                switch(keycode)
                                {
                            		case 1:
        								deci_lens = 1;
        								numbers = 0;
        								if( Input_Number(xpos+130,ypos,&numbers,4,&deci_lens,0) != 1)break;
        								else
        								{
        									if(numbers <= 30000)
        										UserInfo.pzSerial[4] = numbers;
        									else
        										DisplayPrompt(4);
        								}
        								break;
        							case 2:
        								deci_lens = 1;
        								numbers = 0;
        								if( Input_Number(xpos+180,ypos,&numbers,4,&deci_lens,0) != 1)break;
        								else
        								{
        									if(numbers <= 30000)
        										UserInfo.pzSerial[5] = numbers;
        									else
        										DisplayPrompt(4);
        								}
        								break;
        							case 3:
        								deci_lens = 2;
        								numbers = 0;
        								if ( Input_Number(xpos+60,ypos,&numbers,2,&deci_lens,0) == 1)
        				                {
        				                    if(numbers <= 500)
        				                    {
        				                        /*斜探头输入K值*/
        				                        UserInfo.pzSerial[15] = numbers;
        				                    }
        				                    else if(numbers <= C_MAX_ANGLE*10)
        				                    {
        				                        // numbers /=10;
        				                        UserInfo.pzSerial[15] = numbers;
        				                    }
        				                }
        				                break;
                                    case 4:
                                        deci_lens = 1;
        								numbers = 0;
        								if( Input_Number(xpos+210,ypos,&numbers,4,&deci_lens,0) != 1)break;
        								else
        								{
        									if(numbers <= 30000)
        										UserInfo.pzGovernor[1] = numbers;
        									else
        										DisplayPrompt(4);
        								}
        								break;
                                    case 5:
                                        deci_lens = 1;
        								numbers = 0;
        								if( Input_Number(xpos+210,ypos,&numbers,4,&deci_lens,0) != 1)break;
        								else
        								{
        									if(numbers <= 30000)
        										UserInfo.pzGovernor[2] = numbers;
        									else
        										DisplayPrompt(4);
        								}
        								break;
                                    case 6:
                                        deci_lens = 1;
        								numbers = 0;
        								if( Input_Number(xpos+250,ypos,&numbers,4,&deci_lens,0) != 1)break;
        								else
        								{
        									if(numbers <= 30000)
        										UserInfo.pzGovernor[3] = numbers;
        									else
        										DisplayPrompt(4);
        								}
        								break;
                                    case 7:
										EraseDrawRectangle(xpos+130, ypos, xpos + 4*C_CCHAR_HDOT+130, ypos + crow_height * 2);
										TextOut(xpos+134,ypos+4,2,5,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
										while(1)
										{
											keycode = MAnyKeyReturn();
											if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
												break;
											else if(keycode >= 1 && keycode <= 2)
											{
												UserInfo.pzGovernor[0] = keycode-1;
                                                break;
											}
										}
										break;
                                }
                            }
							else if(pages == 4)
							{
								switch(keycode)
								{
									case 1:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+250,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[6] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 2:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+270,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[7] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 3:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+270,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[8] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 4:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+250,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[9] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 5:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+270,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[10] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 6:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+270,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[11] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 7:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+300,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[12] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 8:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+320,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[13] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
									case 9:
										deci_lens = 1;
										numbers = 0;
										if( Input_Number(xpos+320,ypos,&numbers,4,&deci_lens,0) != 1)break;
										else
										{
											if(numbers <= 30000)
												UserInfo.pzSerial[14] = numbers;
											else
												DisplayPrompt(4);
										}
										break;
								}
							}
						}
						break;
                    }
					else if(keycode == 2)
					{
						UserInfo.pzPurchaser[0] = 1;
						MCopyProtectedMemory((void*)offsets, &UserInfo, sizeof(USER_INFO), PM_COPY_WRITE);
						CopyMemoryToSystem((void*)offsets, C_OFF_BATTERY-C_OFF_USER_INFO, PM_COPY_WRITE);
						break;
					}
                }
				break;
			case 7://参数纠错
				while(1)
                {
                    MFclearScreen();
					MSetColor(C_CR_MENU);
					offset = C_OFF_KEYFUNC;
					MCopyProtectedMemory( & Function, (void*)offset, C_LEN_FUNCTION, PM_COPY_READ);
					Write_Number(50,100,Function.funcMenu[0],2,0,0);
					Write_Number(150,100,Function.funcMenu[1],2,0,0);
					Write_Number(250,100,Function.funcMenu[2],2,0,0);
					Write_Number(350,100,Function.funcMenu[3],2,0,0);
					TextOut( 400, 400, 1, 170, 70, "返回键退出", 4 );
					keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN)
                        break;            
				}
				break;			
            }
        }
    }
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1 && stdflag==1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
        DrawDac(0);		//新画DAC
        if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==7)	//CBT3559-94
            {
                MSetBaseGain( MGetBaseGain()-200*log10( dac_line[2*C_COORWIDTH/MGetRange(4)]/160.0),C_SETMODE_SETSAVE);
            }
            else if (MGetStdMode()==12||MGetStdMode()==9)	//47013
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
        MSetSystem();
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    MSetColor(C_CR_UNDO);
    MSetSystem();
    ClearEnvelope();	//清包络或峰值
    MSetRepeatRate(MGetRepeatRate(), C_SETMODE_SET);
    if(extend==0)
    {
        MSetPulseWidthInput(MGetPulseWidthNumber(), C_SETMODE_SETSAVE);
    }
    else
    {
        MSetPulseWidth(MGetPulseWidth(), C_SETMODE_SETSAVE);
    }
    g_nAutoDenoise = 0;
    MSetBaseGain(MGetBaseGain(), C_SETMODE_SETSAVE);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);		//新画DAC
    return 1;
}



int OptionMenu(int page)//出现第几页的参数菜单
{
    uint32_t char_len = 6,row_number = 9,crow_height = 48,erow_height =30;
    int number,deci_len;//,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos = crow_height + 4;
    int retvalue = C_TRUE;
    int keycode;
    int page_max = 2;
    int row;
    uint16_t* cpText = 0;
    uint8_t* epText = 0;
    int i;
    uint32_t elapsedtime1 = GetElapsedTime() ;//,elapsedtime2;

    if( page < 1)page = 1;
    else if(page > page_max )page = page_max;

    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();
    DisplayPrompt(15);

    while(1)
    {
        MFclearScreen();

        switch(page)
        {
        case 1:
            cpText =  (uint16_t*)_OptionMenu_B1;
            epText =  (uint8_t*)_OptionMenu_C1;
            break;
        case 2:
            cpText =  (uint16_t*)_ParaMenu_A3;
            epText =  (uint8_t*)_ParaMenu_B3;
            break;
        }

        MSetColor(C_CR_MENU);
        TextOut(C_ECHAR_HDOT,4,1,25,16,(uint8_t*)_ParaMenu_H1[MGetLanguage()][0],4);
        Write_Number(C_ECHAR_HDOT + 12 * C_ECHAR_HDOT+4,4+3*MGetLanguage(),page,1,0,0);
        if (MGetLanguage())
        {
            Write_Number(C_ECHAR_HDOT + 24 * C_ECHAR_HDOT+8,7,page_max,1,0,0);
        }
        else
        {
            Write_Number(C_ECHAR_HDOT + 19 * C_ECHAR_HDOT+4,4,page_max,1,0,0);
        }
        CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1)*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
//		CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_PARA);
        switch(page)
        {
        case 1:
            //声程标度
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row;
            //	cpText = (uint16_t*)_ParaMenu_AB1 + (MGetScaleMode()-0) * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos ,1,10,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][MGetScaleMode()]+2,4);

            //表面补偿
            row = 1;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            Write_Number(xpos,ypos,MGetSurfGain(),4,1,0);

            //5．	屏幕亮度：低亮\一般\中亮\高亮
#if C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
            row = 2;//工件厚度
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetThick(),4,1,0);
#elif C_COLOR != 1
            row = 2;//屏幕亮度
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][MGetScreenBrightness()]+2,4);
#endif
            //6．打印机
            row = 3;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT;
            epText = (uint8_t*)_ParaMenu_AB3 + ( (MGetPrinterMode()-0)*15+2 );
            EMenuOut(xpos,ypos , epText ,13,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            //3．	当量标准：母线\判废\定量\评定
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos ,1,8,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][MGetDacMode()]+2,4);
            //4．	当量显示：dB值\孔径
            //2．	检波方式：全波\正半波\负半波\射频
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            epText = (char *)_ParaMenu_AD2[MGetLanguage()][MGetEchoMode()]+2;
            TextOut(xpos,ypos+1,1,8,16,epText,4);
            //	cpText = (uint16_t*)_ParaMenu_AD2 + ( MGetEchoMode()*6+1 );
            //	CEMenuOut(xpos,ypos+1,cpText,4,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            //3．	频带宽度：2.5\5.0\10\wide
            row = 6;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	epText = (uint8_t*)_ParaMenu_AD3 + ( MGetBandMode()*9+3 );
            //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,4,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            epText = _ParaMenu_AD3[MGetLanguage()][MGetBandMode()]+2;
            TextOut(xpos,ypos, 1, 4,erow_height, epText, 4);
            //4．	探头阻尼：50Q\400Q
            row = 7;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            epText = (uint8_t*)_ParaMenu_AD4 + ( MGetDampMode()*10+3 );
            EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

            //5．	发射电压：200V\400V
            row = 8;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	epText = (uint8_t*)_ParaMenu_AD5 + ( MGetPulseMode()*10+3 );
            //	EMenuOut(xpos,ypos+C_ECHAR_VDOT+1,epText,7,1,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            epText = (char *)_ParaMenu_AD5[MGetLanguage()][MGetPulseMode()]+2;
            TextOut(xpos,ypos,1,6,16,epText,4);
            break;

        case 2:
            //1．	记录方式：单幅\连续
            row = 0;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (uint16_t*)_ParaMenu_AC1 + MGetSaveMode() * 3 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos ,1,6,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][MGetSaveMode()]+2,4);

            //2．	记录间隔：10sec-120sec	;如为单幅则此参量为0，选连续时自动设为30sec
            row = 1;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetSaveTime(),4,1,0);


            row = 2;//	B门用途：失波\进波
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            TextOut(xpos,ypos,1,6+2*MGetLanguage(),crow_height,(uint8_t*)_ParaMenu_AC5[MGetLanguage()][MGetBGateMode()]+2,4);

            row = 3;//按键声音开关
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (uint16_t*)_ParaMenu_AD8 + MGetSoundAlarm(0) * 2 + 1;
            //CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos,1,4,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][MGetSaveFrequency()]+2,4);

            //6．	参量保护：全部\部分
            row = 4;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            uint16_t paratemp=0;
            int j;
            for(j = 0; j < C_LOCK_ALL; j++)
            {
                if(MGetParaLock(0,j) == 1)paratemp++;
            }
            epText = (uint8_t *)_ParaMenu_AD6[MGetLanguage()][paratemp];
            TextOut(xpos,ypos,1,4,crow_height,epText,4);

            //7．
            row = 5;
            xpos = para_xpos;
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetAmpStdMax()*5,4,1,0);

            row = 6;//存储频谱
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 0;
            //	cpText = (uint16_t*)_ParaMenu_AD9 + MGetSaveFrequency() * 2 + 1;
            //	CEMenuOut(xpos,ypos , cpText ,1,1,crow_height);	 /*在指定位置根据每行字符数、行数、行高写菜单*/
            TextOut(xpos,ypos,1,4,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][MGetSaveFrequency()]+2,4);

            row = 7;//工件厚度
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetThick(),4,1,0);

            row = 8;//工件外径
            ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            Write_Number(xpos,ypos,MGetDiameter(),4,1,0);


            //8．	屏保延时：  min		；无屏保时为0，设为有时自动设为5min
            /*				row = 7;
            				xpos = para_xpos;
            				ypos = para_ypos + crow_height *row + C_ECHAR_HDOT * 1;
            */
            break;
        }

        MKeyRlx();		/*只有按键已释放才退出*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if (keycode >= 0 && keycode < C_KEYCODMAX)break;
            else
            {
                keycode = MGetRotary(-1);
                if( keycode > 0 )keycode = C_KEYCOD_PLUS;
                else if( keycode < 0)keycode = C_KEYCOD_MINUS;
                else continue;
                break;
            }
        }
        while(1);

        //keycode = MAnyKeyReturn();

        if (keycode == C_KEYCOD_RETURN)
        {

            retvalue = C_FALSE;
            break;
        }
        if (keycode == C_KEYCOD_CONFIRM)
        {
            SystemParaStore();
            ChannelParaStore();

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

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*反白显示几个字的区域*/

        if( page == 1)
        {
            switch (keycode)
            {
            case 1:		//标度
                //	cpText = (uint16_t*)_ParaMenu_AB1 ;
                MPushWindow(xpos, ypos, xpos + (4)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                EraseDrawRectangle(xpos, ypos, xpos + (4)*C_CCHAR_HDOT, ypos + crow_height * 3) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,3,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,3,10,crow_height,(uint8_t*)_ParaMenu_AB1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetScaleMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 2:	//表面补偿
                deci_len = 1;
                number = 100;
                while(1)
                {
                    if( Input_Number(xpos,ypos,&number,2, &deci_len,0) != 1)break;
                    else
                    {
                        if( number <= C_MAX_SURFGAIN)
                        {
                            MSetSurfGain(number,C_SETMODE_SAVE);
                            break;
                        }
                        else	DisplayPrompt(4);
                    }
                }

                break;
            case 3:	//屏幕亮度
#if C_DEVTYPE != 4
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * 4) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,3,4,crow_height);	//在指定位置根据每行字符数、行数、行高写菜单
                TextOut(xpos+4,ypos+4 ,4,10,crow_height,(uint8_t*)_ParaMenu_AC4[MGetLanguage()][0],4);

                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetScreenBrightness(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
#elif C_COLOR == 1
                deci_len = 1;
                number = 0;
                int k,j;
                while( true )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
#endif
                break;
            case 4://打印机
                epText = (uint8_t*)_ParaMenu_AB3;
                //ypos -= erow_height * 4 ;
                MPushWindow(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 4) ;
                EraseDrawRectangle(xpos, ypos, xpos + 8*C_CCHAR_HDOT, ypos + erow_height * 4) ;
                EMenuOut(xpos+4,ypos+4, epText ,15,3,15);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 3)
                    {
                        MSetPrinterMode(keycode-1,0/*mode = 0\1\2*/);	/*设置探头模式*/
                        break;
                    }
                }
                MPopWindow();

                break;
            case 5:	//当量标准
                //	cpText = (uint16_t*)_ParaMenu_AC2 ;
                if( MGetCurveNumber() > 3)ypos -= crow_height * ( MGetCurveNumber()-3 );
                MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 ) ) ;
                EraseDrawRectangle(xpos, ypos, xpos + (6)*C_CCHAR_HDOT, ypos + crow_height * ( MGetCurveNumber()+1 )) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText, 4,MGetCurveNumber()+1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4 ,MGetCurveNumber()+1,10,crow_height,(uint8_t*)_ParaMenu_AC2[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode > 0 && keycode <= MGetCurveNumber()+1 )
                    {
                        MSetDacMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 6:
                //	cpText = (uint16_t*)_ParaMenu_AD2 ;
                MPushWindow(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + 4 * crow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 6*C_CCHAR_HDOT, ypos + 4 * crow_height) ;
                //	CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,5,4,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                for (i=0; i<deci_len; i++)
                {
                    TextOut(xpos+C_ECHAR_HDOT,ypos+ i * crow_height+4,1,8,16,_ParaMenu_AD2[MGetLanguage()][i],4);
                }
                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetEchoMode(keycode-1,C_SETMODE_SETSAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 7:
                ypos-=crow_height*2;
                MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 3 * crow_height+5) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + 3 * crow_height+5) ;
                //	EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_ParaMenu_AD3,9,3,15);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                TextOut(xpos + C_ECHAR_HDOT,ypos+4, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][0], 4);
                TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][1], 4);
                TextOut(xpos + C_ECHAR_HDOT,ypos+4+crow_height*2, 1, 10,crow_height, (uint8_t*)_ParaMenu_AD3[MGetLanguage()][2], 4);

                while(1)
                {
                    keycode = MenuKeyReturn(4,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
                    {
                        MSetBandMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 8:
                MPushWindow(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height+1) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 12*C_ECHAR_HDOT, ypos + 3 * erow_height) ;
                EMenuOut(xpos + C_ECHAR_HDOT,ypos+6,_ParaMenu_AD4,10,2,17);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                while(1)
                {
                    keycode = MenuKeyReturn(2,2);	/*按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetDampMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 9:
                MPushWindow(xpos, ypos - erow_height, xpos+ 12*C_ECHAR_HDOT, ypos+ 7 * erow_height) ;
                EraseDrawRectangle(xpos, ypos - erow_height, xpos+ 12*C_ECHAR_HDOT, ypos + 7 * erow_height) ;
                //EMenuOut(xpos + C_ECHAR_HDOT,ypos + 6 - erow_height,_ParaMenu_AD5,10,2,17);	/*在指定位置根据每行字符数、行数、行高写菜单*/

                TextOut(xpos + C_ECHAR_HDOT,ypos + 6 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][0],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][1],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17*2 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][2],4);
                TextOut(xpos + C_ECHAR_HDOT,ypos + 6+17*3 ,4,6,17,(char *)_ParaMenu_AD5[MGetLanguage()][3],4);
                while(1)
                {
#if C_DEVLIB == 23 || C_DEVLIB == 24
                    keycode = MenuKeyReturn(4,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 4)
#else
                    keycode = MenuKeyReturn(2,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
#endif
                    {
                        MSetPulseMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            }
        }
        else if( page == 2)
        {
            switch (keycode)
            {
#if C_UDISK == 1

            case 1:	//存储模式
                //*
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT+1, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4 ,2,8,crow_height,(uint8_t*)_ParaMenu_AC1[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;

                    /*	else if(keycode >= 1 && keycode <= 2)
                    	{
                    		MSetSaveMode(keycode-1,C_SETMODE_SAVE);
                    		if( MGetSaveTime() > 600 )MSetSaveTime(10,C_SETMODE_SAVE);
                    		break;
                    	}*/
                }
                MPopWindow();
                //*/
                break;
            case 2:	//连续存间隔时间
                //*
                deci_len = 1;
                number = 100;
                while( MGetSaveMode() )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos,&number,3, &deci_len,0) != 1)
                        break;
                    else if(number >= 1 && number <= 250)
                    {
                        MSetSaveTime(number,C_SETMODE_SAVE);
                        break;
                    }
                }
                //*/
                break;

#endif
            case 3:	//B门类型
                MPushWindow(xpos, ypos, xpos + 5*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + (5)*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                TextOut(xpos+4,ypos+4,2,8+2*MGetLanguage(),crow_height,(uint8_t*)_ParaMenu_AC5[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetBGateMode(keycode-1,C_SETMODE_SAVE);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 4:
                //	cpText = (uint16_t*)_ParaMenu_AD8 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos+4,ypos+4,2,6,crow_height,(uint8_t*)_ParaMenu_AD8[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSoundAlarm(0, keycode-1);
                        break;
                    }
                }
                MPopWindow();
                break;
            case 5:
                ypos -= 10;
                MPushWindow(xpos, ypos, xpos+ 11*C_CCHAR_HDOT, ypos + 6 * crow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ 11*C_CCHAR_HDOT, ypos + 6 * crow_height) ;
                //CMenuOut(xpos + C_ECHAR_HDOT,ypos+4,cpText,6,5,crow_height);
                int i = 0;
                int ypos1;
                for (i=0; i<6; i++)
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+i*crow_height,1,12,crow_height,(uint8_t*)_ParaProtect1[MGetLanguage()][i],4);
                }
                if (MGetLanguage())
                {
                    TextOut(xpos+ C_ECHAR_HDOT,ypos+4+4*crow_height-8,1,12,crow_height,(uint8_t*)_ParaProtect1[MGetLanguage()][3]+12,4);
                }

                xpos += 6*C_CCHAR_HDOT;

                for(i = 0; i < C_LOCK_ALL; i ++)
                {
                    ypos1 = ypos + crow_height * (i +2)+4;
                    //	cpText = (uint16_t*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                    //CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                    TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(uint8_t*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                }

                do
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
                        break;

                    if (keycode == 0)
                    {
                        MSetParaLock(1, C_LOCK_ALL);	//所有都被保护
                    }
                    else if(keycode >0 && keycode <= C_LOCK_ALL)
                    {
                        MSetParaLock(MGetParaLock(0,keycode-1)+1, keycode-1);	//
                    }
                    else continue;
                    for(i = 0; i < C_LOCK_ALL; i ++)
                    {
                        ypos1 = ypos + crow_height * (i +2)+4;
                        //	cpText = (uint16_t*)_ParaProtect2 + MGetParaLock(0,i) * 2 ;
                        //CMenuOut(xpos + C_ECHAR_HDOT,ypos1,cpText,2,1,crow_height);
                        TextOut(xpos+ C_ECHAR_HDOT,ypos1,1,8,crow_height,(uint8_t*)_ParaProtect2[MGetLanguage()][MGetParaLock(0,i)],0);
                    }
                }
                while(1);
                MPopWindow();
                break;
            case 6:
               deci_len = 0;
                NumHigh = 100;
                while( true )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos,&NumHigh,2, &deci_len,0) != 1)
                        break;
                    else if(NumHigh >= 40 && NumHigh <= 90)
                    {
                        MSetAmpStdMax(NumHigh);
                        break;
                    }
                }
                break;
            case 7:
                //	cpText = (uint16_t*)_ParaMenu_AD9 ;
                MPushWindow(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                EraseDrawRectangle(xpos, ypos, xpos + 4*C_CCHAR_HDOT, ypos + crow_height * 2) ;
                //	CEMenuOut(xpos+4,ypos+4 , cpText ,2,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                TextOut(xpos,ypos,2,6,crow_height,(uint8_t*)_ParaMenu_AD9[MGetLanguage()][0],4);
                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                        break;
                    else if(keycode >= 1 && keycode <= 2)
                    {
                        MSetSaveFrequency(keycode-1,0);
                        break;
                    }
                }
                if (MGetFrequence() != 0)
                {
                    MSetFunctionMode(0,C_ECHO_ENVELOPE);
                    MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                }
                MPopWindow();
                break;
            case 8:
                deci_len = 1;
                number = 0;
                while( true )//如未设置连续存则不能输入
                {
                    int i,j,k;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        break;
                    }
                }
                break;
            case 9:
                deci_len = 1;
                number = 0;
                while( true )//如未设置连续存则不能输入
                {
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) != 1)
                        break;
                    else
                    {
                        MSetDiameter(number);
                        break;
                    }
                }
                break;

            }
            ExpendTime(20);
        }
    }
    SystemParaRestore();
    ChannelParaRestore();
    MSetScreenBrightness( MGetScreenBrightness(),C_SETMODE_SETSAVE);
    ClearEnvelope();	//清包络或峰值
    MSetSystem();
    MSetColor(C_CR_MENU);
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
//	ScreenRenovate();	/*屏幕刷新*/
//	DrawDac(0);		//新画DAC
    return 1;
}
extern int nFlag;
int FuncMenu(void)
{
    uint32_t char_len = 5,row_number = 9,crow_height = 40;
    int xpos,ypos,inv_xpos = 0,inv_ypos = 0;
    int keycode;
    uint16_t *cpText;
    uint8_t *epText;
    int i;
    int prestatus=0;
    uint32_t elapsedtime1 = GetElapsedTime() ;
    int deci_len ,number ,row;
    int para_xpos ,para_ypos;

#if C_DEVTYPE == 1
    row_number = 10;
    crow_height = 40;
#elif C_DEVTYPE == 15
    row_number = 11;
    crow_height = 36;
#endif

    MSetColor(C_CR_MENU);
//	MGateDraw();
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
    ypos = C_COORVPOSI + 1;
//	MPushWindow(xpos, ypos, xpos + (char_len+1)*C_CCHAR_HDOT, ypos + crow_height * (row_number) ) ;
    EraseDrawRectangle(xpos, ypos, xpos + (char_len+1+6*MGetLanguage())*C_CCHAR_HDOT, ypos + crow_height * (row_number)+10) ;
    inv_xpos = xpos + 4;
    inv_ypos = ypos + crow_height + 4;
    for (i=0; i<row_number; i++)
    {
        epText = (uint8_t*) _FuncMenu_A1[MGetLanguage()][i];
        TextOut(xpos+4,ypos+3+crow_height*i,1,11+5*MGetLanguage(),crow_height,epText,0);
        // if (MGetLanguage())
        // {
        //     TextOut(xpos+4,ypos+3+C_ECHAR_VDOT+crow_height*i,1,11,8,epText+11,0);
        // }
    }
//	CEMenuOut(xpos+4,ypos+4 , cpText ,char_len,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
    InvertWords(xpos+4+6*MGetLanguage()*C_ECHAR_HDOT, ypos+2, 2+1*MGetLanguage()); /*反白显示几个字的区域*/

/*#if C_FILL == 1
    if(MGetFill() == 1)
        InvertWords(inv_xpos, inv_ypos + (8 )* crow_height-2,1 );
    MSetFunctionMode(0,C_WAVE_FILL);
    //	InvertWords(inv_xpos, inv_ypos + (9)* crow_height,1 );
#endif*/
#if C_DEVTYPE == 1
    for(i = 0; i < row_number-2; i++)
#elif C_DEVTYPE == 15
    for(i = 0; i < row_number-3; i++)
#endif
    {
        if( MGetFunctionMode(i) == 1 )InvertWords(inv_xpos, inv_ypos + (i + 1)* crow_height-2,1 ); /*反白显示数字*/
    }
    DisplayPrompt(15);
    while(1)
    {
        MKeyRlx();		/*只有按键已释放才退出*/
        xpos = C_COORWIDTH - ( char_len + 1+5*MGetLanguage() ) * C_CCHAR_HDOT;
        ypos = C_COORVPOSI + 1;
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(xpos+4+6*MGetLanguage()*C_ECHAR_HDOT, ypos+2, 2+1*MGetLanguage()); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN)break;
            if (keycode >= 0 && keycode < row_number - 1)break;
        }
        while(1);

//		keycode = MAnyKeyReturn();
        if(keycode == 0 )
        {
            //MChannelRenovate();

            if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
            {
                continue;
            }
            InitPara();//初始化
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);		//新画DAC
            return 1;
        }
#if C_DEVTYPE == 2 || C_DEVTYPE == 9 || C_DEVTYPE == 10 || C_DEVTYPE == 11 || C_DEVTYPE == 13 || C_DEVTYPE == 14 ||C_DEVTYPE == 15
        else if(keycode == C_BSCAN + 1)//B扫描
        {
            BScan();
            MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
            MSetHardEchoShow(1,C_SETMODE_SETSAVE);
            ScreenRenovate();	/*屏幕刷新*/
            DrawDac(0);		//新画DAC
            return 1;
        }
//		else if(keycode == C_FLAW_HEIGHT);//裂纹测高
#endif
        else if(keycode == C_KEYCOD_CONFIRM)
        {
            MActivateDac(MGetFunctionMode(C_DEPTH_COMPENSATE) ) ;

            //设置了门内展宽，则B门不能为进波
            if( MGetFunctionMode(C_CURVER_AMEND) == 1 )MSetBGateMode(0, C_SETMODE_SETSAVE);
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();
            SystemParaStore();
            ChannelParaStore();

            //MGateDraw();
            break;
        }
        else if(keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            //MGateDraw();
            break;
        }

        keycode--;		//1-9 -> 0-8
        prestatus = MGetFunctionMode(keycode);	//原状态
        if(keycode!=7)
        {
        	if(keycode <= C_FUNCTION_MAX)
				MSetFunctionMode( prestatus+1,keycode);
        }
        switch (keycode)
        {
        case C_ECHO_ENVELOPE:
            if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHOMAX_MEMORY + 1) * crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            if (MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                MSetSaveFrequency(0,0);
            }
            break;
        case C_ECHOMAX_MEMORY:		//峰值记忆
            if( MGetFunctionMode(C_ECHO_ENVELOPE) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHO_ENVELOPE + 1) * crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_ECHO_ENVELOPE);
            }
            if(MGetFunctionMode(C_GATE_ALARM) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_GATE_ALARM + 1) * crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_GATE_ALARM);

            }
            if (MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                MSetSaveFrequency(0,0);
            }
            break;
        case C_GATE_ALARM:
            if(MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
            {
                InvertWords(inv_xpos, inv_ypos + (C_ECHOMAX_MEMORY + 1) * crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_ECHOMAX_MEMORY);
            }
            break;
        case C_DAC_GATE:
//Write_Number(10,30,MGetFunctionMode(C_DEPTH_COMPENSATE),4,0,0);
//Write_Number(10,40,MGetFunctionMode(C_DAC_GATE),4,0,0);
//Write_Number(10,40,MGetFunctionMode(C_DAC_GATE),4,0,0);
            if( MGetFunctionMode(C_DAC_GATE) == 1 )
            {
                if( MGetTestStatus(C_TEST_DAC) == 0 && MGetTestStatus(C_TEST_AVG) == 0)
                {
                    MSetFunctionMode(0,C_DAC_GATE);
                }
                if( MGetFunctionMode(C_DEPTH_COMPENSATE) == 1 && MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    InvertWords(inv_xpos, inv_ypos + (C_DEPTH_COMPENSATE + 1)* crow_height-2,1 ); /*反白显示数字*/
                    MSetFunctionMode(0,C_DEPTH_COMPENSATE);
                }
                if( MGetFunctionMode(C_DAC_GATE) == 1)
                {
                    int dac_dB;

                    dac_dB = MGetDacDb();	//dacdB：在DAC母线门为处的dB值
                    MSetGatedB(11, 0);
                    DacGateInit();
                    //MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),DacGate_dBHigh(11,dac_dB),0,C_SETMODE_SETSAVE);
                }
            }
            else if( prestatus == 1)
            {
                //从DAC门改成普通门
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }

            break;
        case C_DEPTH_COMPENSATE:
            if( MGetFunctionMode(C_DAC_GATE) == 1 && prestatus == 0)
            {
                InvertWords(inv_xpos, inv_ypos + (C_DAC_GATE + 1)* crow_height-2,1 ); /*反白显示数字*/
                MSetFunctionMode(0,C_DAC_GATE);
                MSetGatePara( MGetGatePara(0,0),MGetGatePara(0,1),160,0,C_SETMODE_SETSAVE);
            }
            if( MGetEquivalentDays(1) <= 0 || MGetEchoMode() == C_RF_WAVE)	//返回测试状态<0=未测>=0已测，
            {
                MSetFunctionMode( 0,C_DEPTH_COMPENSATE);
            }
            break;
        case C_CURVER_AMEND:
            keycode = C_CURVER_AMEND;
            if( MGetFunctionMode(keycode) == 0)break;
            /*
            if( MGetFunctionMode(C_CURVER_AMEND) != 1)break;
            else if( MGetAngle(0) == 0)
            {
            	MSetFunctionMode( 0,C_CURVER_AMEND);
            	break;
            }
            //*/
            cpText = (uint16_t*)_ParaMenu_A5 ;
            xpos = inv_xpos - 5* C_CCHAR_HDOT;
            ypos = inv_ypos + (C_CURVER_AMEND + 1) * crow_height;
            para_xpos = xpos + C_CCHAR_HDOT * 6 ;
            para_ypos = ypos + 8;
            if(MGetLanguage())
            {
                MPushWindow(xpos-32, ypos, xpos +13*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                //   MPushWindow(xpos + 6*C_CCHAR_HDOT+10, ypos, xpos + 9*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                EraseDrawRectangle(xpos-32, ypos, xpos + 13*C_CCHAR_HDOT+10, ypos + crow_height * 2+7) ;

                TextOut(xpos+4-32,ypos+4,1,15,16,(uint8_t*)_ParaMenu_A5[MGetLanguage()][0],4);
                TextOut(xpos+4-32,ypos+4+crow_height,1,15,16,(uint8_t*)_ParaMenu_A5[MGetLanguage()][1],4);
            }
            else
            {
                MPushWindow(xpos, ypos, xpos + 10*C_CCHAR_HDOT+11, ypos + crow_height * 2+8) ;
                EraseDrawRectangle(xpos, ypos, xpos + 10*C_CCHAR_HDOT+10, ypos + crow_height * 2+7) ;
                TextOut(xpos+4,ypos+8,1,14,32,(uint8_t*)_ParaMenu_A5[MGetLanguage()][0],4);
                TextOut(xpos+4,ypos+8+crow_height,1,14,32,(uint8_t*)_ParaMenu_A5[MGetLanguage()][1],4);
            }
            //	MPushWindow(xpos, ypos, xpos + 10*C_CCHAR_HDOT+1, ypos + crow_height * 3) ;
            //	EraseDrawRectangle(xpos, ypos, xpos + 10*C_CCHAR_HDOT, ypos + crow_height * 3) ;
            //	CEMenuOut(xpos+4,ypos+8 , cpText ,6,2,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            while(1)
            {
                row = 0;//工件厚度
                xpos = para_xpos+C_CCHAR_HDOT * 3 *MGetLanguage();
                ypos = para_ypos + crow_height *row;
                Write_Number(xpos,ypos,MGetThick(),4,1,0);

                row = 1;//工件外径
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetDiameter(),4,1,0);

                while(true)
                {
                    keycode = MAnyKeyReturn();
                    if (keycode == 1 || keycode == 2 || keycode == C_KEYCOD_CONFIRM || keycode == C_KEYCOD_RETURN)break;
                }

                if (keycode == C_KEYCOD_RETURN)
                {
                    MSetFunctionMode( 0,C_CURVER_AMEND);
                    break;
                }
                if (keycode == C_KEYCOD_CONFIRM)
                {
                    break;
                }
                row = keycode - 1;
                xpos = para_xpos+C_CCHAR_HDOT * 3 *MGetLanguage();
                ypos = para_ypos + crow_height * row-1;
                if( keycode == 1)
                {
                    int i,j,k;
                    deci_len = 1;
                    number = 0;
                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                }
                else if( keycode == 2)
                {
                    deci_len = 1;
                    number = 0;

                    if( Input_Number(xpos,ypos,&number,4, &deci_len,0) == 1)
                    {
                        //	if( number >= 2 * MGetThick() )
                        MSetDiameter(number);
                    }
                }
            }
            MPopWindow();
            keycode = C_CURVER_AMEND;
            break;

#if C_FILL == 1
        case C_WAVE_FILL:
            //MSetFill(MGetFill()+1,C_SETMODE_SETSAVE);
            //InvertWords(inv_xpos, inv_ypos + (6 + 1)* crow_height,1 ); /*反白显示数字*/
            if(SystemStatus.prepare[2])
            {
				SystemStatus.prepare[2] = 0;
				nFlag = 0;
				CharToKeypad(BACKLIGHT_OFF);
				SystemParaStore();
				MSetSystem();
            }
			else
			{
				SystemStatus.prepare[2] = 1;
				nFlag = 1;
				CharToKeypad(BACKLIGHT_ON);
				SystemParaStore();
				MSetSystem();
			}
			break;
#endif
        case C_WAVE_EXPEND:		/*门内展宽*/
            break;

        case C_AUTOADJUST_GAIN:
            break;
        }

        if( keycode <= C_FUNCTION_MAX && prestatus != MGetFunctionMode(keycode) )//状态已改变
            InvertWords(inv_xpos, inv_ypos + (keycode + 1)* crow_height-2,1 ); /*反白显示数字*/
    }

    MSetColor(C_CR_UNDO);
    ClearEnvelope();	//清包络或峰值
    SystemParaRestore();
    ChannelParaRestore();

    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    MSetSystem();
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);

    return 1;
}

extern uint8_t crPara[];
extern const uint32_t c_crPara[] ;
extern const uint32_t Echo_crPara[] ;

const uint8_t _COLOR_Menu[C_LANGUAGE][9][15] =
{
    {
        "０自定义",
        "⒈传统黑",
        "⒉海洋蓝",
        "⒊梅子青",
        "⒋火焰红",
        "⒌香槟粉",
        "⒍香草绿",
        "⒎经典白",
        "⒏经典黑",
    },
    {
        "0.Custom     ",
        "1.Tradition  ",
        "2.Blue       ",
        "3.Cyan       ",
        "4.Red        ",
        "5.Pink       ",
        "6.Green      ",
        "7.White      ",
        "8.Black      ",
    }
};
const uint8_t _COLOR_A1[C_LANGUAGE][10][15] =
{
    {
        "０屏幕底色:",
        "⒈波形区域:",
        "⒉奇次回波:",
        "⒊偶次回波:",
        "⒋提示菜单:",
        "⒌定量曲线:",
        "⒍坐标参量:",
        "⒎波门Ａ  :",
        "⒏波门Ｂ  :",
        "⒐峰值包络:",
    },
    {
        "0.Background:",
        "1.WaveGround:",
        "2.ODD wave  :",
        "3.EVEN wave :",
        "4.TIP menu  :",
        "5.Curve     :",
        "6.Param     :",
        "7.Gate A    :",
        "8.Gate B    :",
        "9.Envelope  :",
    }
};
const uint8_t _COLOR_B1[C_LANGUAGE][9][8] =
{
    {
        "⒈红",
        "⒉粉",
        "⒊黄",
        "⒋绿",
        "⒌青",
        "⒍兰",
        "⒎灰",
        "⒏白",
        "⒐黑",
    },
    {
        "1.Red   ",
        "2.Pink  ",
        "3.Yellow",
        "4.Green ",
        "5.Cyan  ",
        "6.Blue  ",
        "7.Gray  ",
        "8.White ",
        "9.Black ",
    }
};
/*


0。颜色设置：自定义/默认设置1/默认设置2/默认设置3
0. 恢复默认值
1. 奇次回波：
2。偶次回波：
3。提示菜单：
4。定量曲线：
5. 坐标参量
6。波门A：
7。波门B：
8。峰值包络：
9.屏幕底色

*/
int ColorMenu(void)
{
#if C_COLOR == 1 && C_DEVTYPE != 20
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

    uint8_t crTemp[C_CR_MAX+12];
    for( i = 0 ; i < C_CR_MAX+12 ; i++)crTemp[i] = crPara[i]; //保存原设置
    crnum = 9;
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(uint8_t*)_COLOR_Menu[MGetLanguage()][i],4);
        }

        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_UNDO);

        /*    for( i = 0,row = 0; i < crnum ; i++,row++)
            {
                xpos = para_xpos + C_ECHAR_HDOT+8*MGetLanguage();
                ypos = para_ypos + crow_height *row ;
                MSetColor( i ) ;
                TextOut(xpos,ypos,1,6,crow_height,(uint8_t*)_COLOR_B1[MGetLanguage()][MLoadColor(i)]+2,4);
                MSetColor(C_CR_UNDO);
            }*/
        MKeyRlx();		/*只有按键已释放才退出*/

        xpos = para_xpos;
        ypos = para_ypos + crow_height *MGetColorMode();
        InvertWords(C_ECHAR_HDOT,ypos, 1); /*反白显示几个字的区域*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode < crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );


        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();
            for( i = 0 ; i < C_CR_MAX+12 ; i++)crPara[i] = crTemp[i] ; //恢复原设置
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            offset = C_OFF_TEMP+60;	//颜色存储位置
            MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        /*   else if( keycode == C_KEYCOD_COLOR)
           {
               MSaveDefualtColor(MGetColorMode()+1);//设置默认颜色
               continue;
           }*/
        else if( keycode > 0)
        {
//;			MSaveDefualtColor(0);//设置默认颜色
            MSaveDefualtColor(keycode);//设置默认颜色
            continue;
        }
        else if( keycode == 0)
        {
            MSaveDefualtColor(keycode);//设置默认颜色
            CustomizeColorMenu();
            int offset = C_OFF_TEMP+60;	//颜色存储位置
            MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
            CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);

        }

    }
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
    MSetColor(C_CR_UNDO);
    MSetColor(C_CR_WAVE);
    MSetSystem();
    MSetAcquisition(1);
    ClearEnvelope();	//清包络或峰值
    ScreenRenovate();	/*屏幕刷新*/
    DrawDac(0);		//新画DAC
#endif
    return 1;
}
int CustomizeColorMenu(void)
{
#if C_COLOR == 1 && C_DEVTYPE != 20
    uint32_t char_len = 6, crow_height = 48;//,row_number = 10,  erow_height = 12;
    //int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 3*MGetLanguage()) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    //int page_no = 1,page_max = 3;
    int row;
    uint16_t* cpText = 0;
    //uint8_t* epText = 0;
    int i;//,j,k;
    int len;//,line;
    int item;
    uint32_t elapsedtime1 = GetElapsedTime();// ,elapsedtime2;
    int offset;
    int crnum = 0;

//#if C_DEVTYPE == 20
//	return 0;
//#endif
    uint8_t crTemp[C_CR_MAX+12];
    for( i = 0 ; i < C_CR_MAX+12 ; i++)crTemp[i] = crPara[i]; //保存原设置

#if C_DEVTYPE == 1 || C_DEVTYPE == 8  || C_DEVTYPE == 9  || C_DEVTYPE == 10  || C_DEVTYPE == 11 || C_DEVTYPE == 12 || C_DEVTYPE == 13 || C_DEVTYPE == 14 ||C_DEVTYPE == 15
    crnum = 10;
#elif C_DEVTYPE == 4 || C_DEVTYPE == 20 || C_DEVTYPE == 5
    crnum = 8;
#endif
    MSetColor(C_CR_MENU);
    MSetAcquisition(0);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();

        //cpText =  (uint16_t*)_COLOR_A1;
        //epText =  (uint8_t*)_STDMenu_B1;
        MSetColor(C_CR_MENU);
        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (6+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12));
        MSetColor(C_CR_MENU);
        for (i=0; i<crnum; i++)
        {
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i,1,16,24,(uint8_t*)_COLOR_A1[MGetLanguage()][i],4);
        }
        //CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,crnum,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        //EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        //CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        MSetColor(C_CR_UNDO);
        //MSetColor(C_CR_PARA);

        //
        for( i = 0,row = 0; i < crnum ; i++,row++)
        {
            xpos = para_xpos + C_ECHAR_HDOT+8*MGetLanguage();
            ypos = para_ypos + crow_height *row ;
            MSetColor( i ) ;
            TextOut(xpos,ypos,1,6,crow_height,(uint8_t*)_COLOR_B1[MGetLanguage()][MLoadColor(i)]+2,4);
            MSetColor(C_CR_UNDO);
        }
        MKeyRlx();		/*只有按键已释放才退出*/

        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode < crnum )break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM && keycode != C_KEYCOD_COLOR );

        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();
            for( i = 0 ; i < C_CR_MAX+12 ; i++)crPara[i] = crTemp[i] ; //恢复原设置
            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            MSetSystem();

            offset = C_OFF_TEMP+60;	//颜色存储位置
            MCopyProtectedMemory( (void*)offset, crPara, C_CR_MAX+12, PM_COPY_WRITE);
//            CopyMemoryToSystem( (void*)offset,C_CR_MAX+12,PM_COPY_WRITE);
            SystemParaStore();
            ChannelParaStore();

            retvalue = C_TRUE;
            break;
        }
        else if( keycode == C_KEYCOD_COLOR)
        {
            MSaveDefualtColor(MGetColorMode()+1);//设置默认颜色
            continue;
        }
        /*  else if( keycode == 0)
          {
        //;			MSaveDefualtColor(0);//设置默认颜色
              MSaveDefualtColor(MGetColorMode()+1);//设置默认颜色
              continue;
          }*/

        row = item = keycode ;
        //item--;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos, 1); /*反白显示几个字的区域*/
        ypos = para_ypos + crow_height * 0;

        len = 2;
        row = 9;
        MPushWindow(xpos, ypos, xpos+ (len+5+3*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        EraseDrawRectangle(xpos, ypos, xpos+ (len+5+3*MGetLanguage())*C_ECHAR_HDOT, ypos + (row+0) * crow_height) ;
        for( i = 0; i < row ; i++)
        {
#if( C_COLOR == 1)
            if( i == row - 1)SetDisplayColor( RED_B0|GREEN_B0|BLUE_B0 );
            else SetDisplayColor( c_crPara[ i ] );
#endif
            TextOut(xpos + C_ECHAR_HDOT,ypos+8+i*crow_height,1,3+5*MGetLanguage(),crow_height,(uint8_t*)_COLOR_B1[MGetLanguage()][i],4);
        }
        while(1)
        {
            /*  MSetColor(C_CR_MENU);
              Write_Number(100,50,MLoadColor(0),1,0,0);
              Write_Number(100,70,MLoadColor(1),1,0,0);
              Write_Number(100,90,item,1,0,0);
              MAnyKeyReturn();*/
            keycode = MenuKeyReturn(row,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/

            if(MLoadColor(1) == (keycode-1) && (item==2||item==3||item==5||item==7||item==8||item==9))  //与波形区不重色
                continue;
            if(MLoadColor(0) == (keycode-1)&& (item==4||item==6))  //与底色区不重色
                continue;
            /*   for(i=0;i<row;i++)
               {
                   if(item==0 && MLoadColor(i) ==  (keycode-1))  //底色区与其他不重色
                   {
                       i=250;
                       break;
                   }
                   if(item==1 && MLoadColor(i) ==  (keycode-1))  //波形区与其他不重色
                   {
                       i=250;
                       break;
                   }
               }
               if(i==250)continue;*/

            if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                break;
            else if(keycode >= 1 && keycode <= row)
            {
                MSaveColor(item,keycode-1 );
                //Write_Number( 200,30,item,4,0,0);
                //Write_Number( 200,40,keycode,4,0,0);
                //MAnyKeyReturn();
                break;
            }
        }
        MPopWindow();
    }
    SetBackgroundColor(c_crPara[crPara[ C_CR_BACK] ]);
    MSetColor(C_CR_UNDO);
    MSetColor(C_CR_WAVE);
    MSetSystem();
//    MSetAcquisition(1);
//    ClearEnvelope();	//清包络或峰值
//    ScreenRenovate();	/*屏幕刷新*/
//    DrawDac(0);		//新画DAC
#endif
    return 1;
}

void InitPara(void)
{
    int xpos, ypos ;
    uint16_t* cpText ;
    int keycode ;
    int offset=0,offmax;
    int Zero;
    int row_number;
    int i;
    int crow_height = 48,erow_height =20;
    int language= MGetLanguage();
    int Unit = MGetUnitType();
    if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
    {
        return;
    }

    SysPwd(0);
    MSetColor(C_CR_DEFAULT);

    offset = C_OFF_INNER_INFO;
    INNER_INFO InnerInfo;
    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);

    xpos = C_COORWIDTH - 6 * C_CCHAR_HDOT-16*MGetLanguage();
    ypos = C_COORVPOSI + 1 ;

    MFclearScreen();
    //MSetColor(C_CR_MENU);
    row_number = 6;
    MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT+2, ypos + crow_height *row_number+1 ) ;
    MEraseWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT, ypos + crow_height * row_number+1) ; /* 清除窗体 */
    for (i=0; i<row_number; i++)
// @todo (union#1#): Now Here
    {
        TextOut(xpos+4, ypos+4+i*crow_height,  1, 17,24, (uint8_t*)_FuncMenu_A2[MGetLanguage()][i], 4);
    }
    DrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*4*C_CCHAR_HDOT, ypos + crow_height * row_number ) ; /* 画矩形 */

    while(1)
    {
        keycode = MAnyKeyReturn();
        if (keycode == C_KEYCOD_RETURN || keycode == C_KEYCOD_CONFIRM)
            break;
        else if( (keycode >= 1 && keycode <row_number) || keycode == 9)
        {
            //if( DisplayQuery(3) == 0)break;

            if( keycode == 1)
            {
                offset = C_OFF_CHANNEL;
                // if( DisplayQuery(8) == 0)break;
                DisplayPrompt(17);//显示数据正在清除
                ChannelParaInit(0);	/*参数初始化，mode=0当前一个通道，mode=1所有通道*/
            }
            else if (keycode == 2 && MGetSaveStatus()==0)	//连续记录下仅可清当前通道
            {
                offset = C_OFF_SYSTEM;
                if( DisplayQuery1(9) == 0)break;

                DisplayPrompt(17);//显示数据正在清除
                MSetChannel(0,0);
                ChannelParaInit(1);	/*参数初始化，mode=0当前一个通道，mode=1所有通道*/
            }
            else if (keycode == 3 && MGetSaveStatus()==0)
            {
                offset = C_OFF_FILEINFO;
                int file_total;

                if( DisplayQuery1(10) == 0)break;

                for(i=0; i<C_FILE_NUMBER; i++)
                {
                    cache.cache[i]=i;
                }
                offset = C_OFF_CACHE;
                MCopyProtectedMemory( (void*)offset, &cache, sizeof(CACHE), PM_COPY_WRITE);

                offset = C_OFF_FILEINFO;
                file_total = 0;		//清空存储的探伤数据
                MCopyProtectedMemory( (void*)offset, &file_total, C_SIZE_INT, PM_COPY_WRITE);
                CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

            }
            else if (keycode == 5)
            {
                MSetSystemTime() ;
            }
            else if (keycode == 6)
            {
                if((SDCardGetStatus()&0x01)==1)
                {
                    DisplayPrompt(19);	//正在连接U盘
                    // U盘初始化
                    if(SDCard_Init())
                    {
                        // DisplayPrompt(20);	//U盘连接成功
                        DisplayPrompt(29);	//SD正在格式化
                        if(SdCard_Format())
                        {
                            DisplayPrompt(30);	//SD格式化成功
                            ExpendTime(100);
                            DisplayPrompt(24);	//正在安全断开
                            FileClose(0);
                            DisplayPrompt(23);	//U盘断开连接
                        }
                        else
                        {
                            DisplayPrompt(21);	//U盘连接失败
                        }
                    }

                }
                else
                {
                    DisplayPrompt(21);	//U盘连接失败
                }


            }
            else if ((keycode == 4 || (keycode == 9 && GetElapsedTime() < 1000 * 30) ) && MGetSaveStatus()==0)//开机30秒内在初始化中按9可清全部数据
            {
				MFclearScreen();
                int number,deci_len;
                number = 0;
                deci_len = 0;
                EMenuOut( 0,0,"Pwd:",4,1,8);
                if( Input_Number(C_ECHAR_HDOT*4,0,&number,8,&deci_len,0) < 0)
                {
                    number = C_DEFPWD - 1;
                }

                if(number != 1234)
                    break;
                MFclearScreen();
				
                if( DisplayQuery1(11) == 0)break;
                int mode,i;

                if( keycode == 9 )
                {
                    //开机时间约定时长
                    mode = 2;
                    for( i = 0; i < 5; i++)
                    {
                        offset = C_OFF_INIT + i * C_SIZE_INT;
                        MCopyProtectedMemory( &Zero, (void*)offset, C_SIZE_INT, PM_COPY_READ);
                        if( Zero != i)break;
                    }
                    if( i < 5 )//说明是第一次初始化
                    {
                        for( i = 0; i < 5; i++)
                        {
                            offset = C_OFF_INIT + i * C_SIZE_INT;
                            MCopyProtectedMemory( (void*)offset, &i,C_SIZE_INT, PM_COPY_WRITE);
                        }
                        offset = C_OFF_PWD;
                    }
                    else offset = C_OFF_STORE;
                }
                else
                {
                    mode = 3;
                    offset = C_OFF_DAC;
                }

                //数据清除
                Zero = 0;
                offmax = C_OFF_FILEMAX;
                DisplayPrompt(17);//显示数据正在清除
                uint32_t time = GetElapsedTime();
                uint32_t elapsedtime = 400;
                int xpos = (6+4*MGetLanguage())* C_CCHAR_HDOT;
                MSetColor(C_CR_MENU);
                for( ; offset < offmax; offset += C_SIZE_INT)
                {
                    MCopyProtectedMemory( (void*)offset, &Zero, C_SIZE_INT, PM_COPY_WRITE);	//写0
                    if( time + elapsedtime < GetElapsedTime() )
                    {
                        Write_Ascii(xpos,7,'.');
                        time = GetElapsedTime();
                        xpos += 8;
                    }
                }
                ExpendTime(100);
                Write_Ascii(xpos,7,'.');
                MSetChannel(0,0);




                ChannelParaInit(mode);	/*参数初始化，mode=0当前通道，mode=1所有通道,2所有用户*/

                if(MGetStdMode()==2)
                {
                    int testrange[3];
                    testrange[1] = 1000;
                    testrange[2] = 0;
                    offset = C_OFF_TEMP + 0;
                    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    testrange[0] = 500;
                    testrange[1] = 300;
                    offset = C_OFF_TEMP + 20;
                    MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
                    testrange[0] = 800;
                    testrange[1] = 10;
                    testrange[2] = 60;
                    offset = C_OFF_TEMP + 30;
                    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);


                }

                else if(MGetStdMode()==12)
                {
                    int testrange[3];
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

                if( keycode == 9)
                {

                    language=0;	//按9初始化恢复语言
                    Unit = 0;

                }

                for(i=0; i<C_FILE_NUMBER; i++)
                {
                    cache.cache[i]=i;
                }
                offset = C_OFF_CACHE;
                MCopyProtectedMemory( (void*)offset, &cache, sizeof(CACHE), PM_COPY_WRITE);

                offset = C_OFF_INIT;
                CopyMemoryToSystem((void*)offset, C_OFF_BATTERY-offset,PM_COPY_WRITE);
                CopyMemoryAllSystem(PM_COPY_WRITE); //全部初始化了
                offset = C_OFF_FILEINFO;
                CopyMemoryToSystem((void*)offset, C_OFF_FAT-offset,PM_COPY_WRITE);

                offset = C_OFF_F_BACKUP;
                CopyMemoryToSystem((void*)offset, C_OFF_FILEMAX-offset,PM_COPY_WRITE);
            }

            //ChannelParaRestore();
            MSetSurfGain(0,2);
            break;
        }
    }
    MPopWindow() ;
    MSetColor( C_CR_MENU);
    MSetColor( C_CR_WAVE);
    MSetLanguage(language);
    MSetUnitType(Unit);
}

void InitParaTemp(void)
{
    int xpos, ypos ;
    uint16_t* cpText ;
    int keycode ;
    int offset,offmax;
    int Zero;
    int row_number;
    int i;
    int crow_height = 48,erow_height =20;
    int language= MGetLanguage();
    int Unit = MGetUnitType();
    if (MGetSaveMode()==1 && MGetSaveStatus()==1 )
    {
        return;
    }
    MSetColor(C_CR_DEFAULT);

    xpos = C_COORWIDTH - 6 * C_CCHAR_HDOT-16*MGetLanguage();
    ypos = C_COORVPOSI + 1 ;

    offset = C_OFF_INNER_INFO;
    INNER_INFO InnerInfo;
    MCopyProtectedMemory(&InnerInfo, (void*)offset, sizeof(INNER_INFO), PM_COPY_READ);

    MFclearScreen();
    MSetColor(C_CR_MENU);
    row_number = 5;
    MPushWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT+2, ypos + crow_height *row_number ) ;
    MEraseWindow(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT, ypos + crow_height * row_number) ; /* 清除窗体 */
    for (i=0; i<row_number; i++)
// @todo (union#1#): Now Here
    {
        TextOut(xpos+4, ypos+4+i*crow_height,  1, 17,24, (uint8_t*)_FuncMenu_A2[MGetLanguage()][i], 4);
    }
    DrawRectangle(xpos, ypos, xpos + 6*C_CCHAR_HDOT+MGetLanguage()*3*C_CCHAR_HDOT, ypos + crow_height * row_number ) ; /* 画矩形 */

    int mode;

    mode = 2;
    for( i = 0; i < 5; i++)
    {
        offset = C_OFF_INIT + i * C_SIZE_INT;
        MCopyProtectedMemory( &Zero, (void*)offset, C_SIZE_INT, PM_COPY_READ);
        if( Zero != i)break;
    }
    if( i < 5 )//说明是第一次初始化
    {
        for( i = 0; i < 5; i++)
        {
            offset = C_OFF_INIT + i * C_SIZE_INT;
            MCopyProtectedMemory( (void*)offset, &i,C_SIZE_INT, PM_COPY_WRITE);
        }
        offset = C_OFF_PWD;
    }
    else offset = C_OFF_STORE;


    //数据清除
    Zero = 0;
    offmax = C_OFF_FILEMAX;
    DisplayPrompt(17);//显示数据正在清除
    uint32_t time = GetElapsedTime();
    uint32_t elapsedtime = 400;
    xpos = 6* C_CCHAR_HDOT;
    MSetColor(C_CR_MENU);
    for( ; offset < offmax; offset += C_SIZE_INT)
    {
        MCopyProtectedMemory( (void*)offset, &Zero, C_SIZE_INT, PM_COPY_WRITE);	//写0
        if( time + elapsedtime < GetElapsedTime() )
        {
            Write_Ascii(xpos,7,'.');
            time = GetElapsedTime();
            xpos += 8;
        }
    }
    ExpendTime(100);
    Write_Ascii(xpos,7,'.');
    MSetChannel(0,0);




    ChannelParaInit(mode);	/*参数初始化，mode=0当前一个通道，mode=1所有通道,2所有用户*/


    int testrange[3];
    testrange[1] = 1000;
    testrange[2] = 0;
    offset = C_OFF_TEMP + 0;
    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
    testrange[0] = 500;
    testrange[1] = 300;
    offset = C_OFF_TEMP + 20;
    MCopyProtectedMemory( (void*)offset, testrange, 8, PM_COPY_WRITE);
    testrange[0] = 800;
    testrange[1] = 10;
    testrange[2] = 60;
    offset = C_OFF_TEMP + 30;
    MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);


    language=0;	//按9初始化恢复语言
    Unit = 0;

    //ChannelParaRestore();
    MSetSurfGain(0,2);

    MPopWindow() ;

    MSetColor( C_CR_MENU);
    MSetColor( C_CR_WAVE);
    MSetLanguage(language);
    MSetUnitType(Unit);

}



int STDMenu(void)//标准菜单
{
    uint32_t char_len = 6,crow_height = 48,erow_height = 30;//row_number = 10,
    int number,deci_len,sign;
    int xpos,ypos;
    //int menu_xpos = 0,menu_ypos = 0;
    int para_xpos = C_CCHAR_HDOT * ( char_len + 1) ,para_ypos =  4;
    int retvalue = C_TRUE;
    int keycode;
    //int page_no = 1,page_max = 3;
    int row;
    uint16_t* cpText = 0;
    uint8_t* epText = 0;
    int i,j,k;
    int len,line;
    uint32_t elapsedtime1 = GetElapsedTime() ;
    int basegain1=MGetBaseGain();
    int basegain2;
#if C_DEVTYPE == 20
    return 0;
#endif
    if( MGetTestStatus(C_TEST_AVG) == 1)return 0;//已做AVG则不显示标准菜单
    if( MGetParaLock(1,C_LOCK_DACAVG) == 1)return 0;	//参量保护
    MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
    MSetHardEchoShow(0,C_SETMODE_SETSAVE);
    ClearEnvelope();
    SystemParaStore();
    ChannelParaStore();

    DisplayPrompt(15);
    while(true)
    {
        MFclearScreen();
        para_ypos =  4;
        //	cpText =  (uint16_t*)_STDMenu_A1;
        //epText =  (uint8_t*)_STDMenu_B1;

        MSetColor(C_CR_MENU);
        if(MGetStdMode() == 10||MGetStdMode() == 11)//if(MGetStdMode() >= C_STD_11345_2013_NUM -1)
        {
            if(MGetBlock() > 1)
            {
                for (i=0; i<MGetCurveNumber()+4; i++)
                {
                    TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A2[MGetLanguage()][i], 4);
                }
            }
            else if(MGetBlock() == 1)
            {
                if(MGetThick()/10 <15)
                {
                    for (i=0; i<MGetCurveNumber()+4 - 1; i++)
                    {
                        TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A3[MGetLanguage()][i], 4);
                    }
                }
                else
                {
                    for (i=0; i<MGetCurveNumber()+4; i++)
                    {
                        TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A4[MGetLanguage()][i], 4);
                    }
                }
            }
            else if(MGetBlock() == 0)
            {
                for (i=0; i<MGetCurveNumber()+4; i++)
                {
                    TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A5[MGetLanguage()][i], 4);
                }
            }
        }
        else
        {
            for (i=0; i<MGetCurveNumber()+4; i++)
            {
                TextOut(C_ECHAR_HDOT,para_ypos+crow_height*i, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A1[MGetLanguage()][i], 4);
            }
        }
        if(MGetStdMode()==1 || MGetStdMode()==3 || MGetStdMode()==8 || MGetStdMode()==9 || MGetStdMode()==13 || MGetStdMode()==14 )
        {
            MEraseWindow(C_ECHAR_HDOT,para_ypos+crow_height*2,C_ECHAR_HDOT+C_CCHAR_HDOT*6,para_ypos+crow_height*2+C_CCHAR_VDOT);
            TextOut(C_ECHAR_HDOT,para_ypos+crow_height*2, 1, 13+MGetLanguage()*2,16, (uint8_t*)_STDMenu_A1[MGetLanguage()][10], 4);
        }
        //  if(MGetLanguage())
        {
            //      TextOut(C_ECHAR_HDOT,para_ypos+11, 1, 8,10, (uint8_t*)_STDMenu_A1[MGetLanguage()][0]+13, 4);
            //      TextOut(C_ECHAR_HDOT,para_ypos+crow_height+11, 1, 8,10, (uint8_t*)_STDMenu_A1[MGetLanguage()][1]+13, 4);
        }
        //	CEMenuOut(C_ECHAR_HDOT,para_ypos, cpText ,char_len,MGetCurveNumber()+4,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        //EMenuOut(para_xpos+C_ECHAR_HDOT*5,para_ypos + 8, epText ,3,row_number,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+1+1*MGetLanguage())*C_CCHAR_HDOT, 380 + 2 * (C_CCHAR_VDOT+12)) ;
        //	CMenuOut(27*C_ECHAR_HDOT,190+4,_MenuPrompt_B1,5,2,24);	/*在指定位置根据每行字符数、行数、行高写菜单*/
        TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0],4);
        TextOut(27*C_ECHAR_HDOT,380+4+36,1,10,24,(uint8_t*)_MenuPrompt_B1[MGetLanguage()][0]+10,4);
//        EMenuOut(27*C_ECHAR_HDOT,380+4,_TestMenu_Enter,1,1,C_ECHAR_VDOT);
        if(MGetLanguage())
        {
            para_ypos+= C_ECHAR_VDOT;
        }

        MSetColor(C_CR_PARA);
        /*标准类型 */
        row = 0;
        xpos = para_xpos;
        ypos = para_ypos + crow_height *row ;
        epText = (uint8_t*)_STDMenu_AB1 + ( MGetStdMode()*C_STD_LEN+2 );
        EMenuOut(xpos,ypos , epText ,C_STD_LEN-2,1,crow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        //曲线数
        row = 1;
        ypos = para_ypos + crow_height *row ;
        if((MGetStdMode() == 10||MGetStdMode() == 11)&&(MGetBlock() == 1)&&(MGetThick()/10 <15))
        {
            Write_Number(xpos,ypos,MGetCurveNumber()-1,2,0,0);
        }
        else
            Write_Number(xpos,ypos,MGetCurveNumber(),2,0,0);

        //试块类型
        row = 2;
        ypos = para_ypos + crow_height *row ;
        epText = (uint8_t*)_STDMenu_B1[MGetStdMode()]+1 + ( MGetBlock()*C_BLOCK_LEN+2 ) ;
        EMenuOut(xpos, ypos, epText ,C_BLOCK_LEN-2, 1, erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/

        //工件厚度
        row = 3;
        ypos = para_ypos + crow_height *row ;
        WriteLongness(xpos,ypos,MGetThick(),5,1);
        ///Write_Number(xpos,ypos,MGetThick(),4,1,0);

        //第1一6根线
        if((MGetStdMode() == 10||MGetStdMode() == 11) &&(MGetBlock() == 1)&&(MGetThick()/10 <15))
        {
            for( i = 0; i < (MGetCurveNumber()-1); i++)
            {
                row++;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetLineGain(i),5,1,1);
            }
        }
        else
        {
            for( i = 0; i < MGetCurveNumber(); i++)
            {
                row++;
                xpos = para_xpos;
                ypos = para_ypos + crow_height *row ;
                Write_Number(xpos,ypos,MGetLineGain(i),5,1,1);
            }
        }
        MKeyRlx();		/*只有按键已释放才退出*/
        do
        {
            if( GetElapsedTime() > elapsedtime1 + 200 )
            {
                InvertWords(27*C_ECHAR_HDOT,380+4, 1); /*反白显示几个字的区域*/
                elapsedtime1 = GetElapsedTime() ;
            }
            keycode = MGetKeyCode(10);
            if( keycode >= 0 && keycode <= MGetCurveNumber() + 4)break;
        }
        while( keycode != C_KEYCOD_RETURN && keycode != C_KEYCOD_CONFIRM );

        if (keycode == C_KEYCOD_RETURN)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif
            SystemParaRestore();
            ChannelParaRestore();

            retvalue = C_FALSE;
            break;
        }
        else if (keycode == C_KEYCOD_CONFIRM)
        {
#if C_ACQU == 1
            MSetAcquisitionEnable(MGetAcquisitionEnable(),C_SETMODE_SETSAVE);
#endif

            retvalue = C_TRUE;
            break;
        }

        if(keycode == 0)keycode = 10;
        keycode--;//把1－10变为0－9
        row = keycode ;
        xpos = para_xpos;
        ypos = para_ypos + crow_height * row-1;

        InvertWords(C_ECHAR_HDOT,ypos-MGetLanguage()*C_ECHAR_VDOT, 1); /*反白显示几个字的区域*/

        switch (keycode)
        {
        case 0:		//标准
            len = C_STD_LEN;
            row = 9;
            short pagemax1=0;
            while(1)
            {
                MPushWindow(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+2) * erow_height) ;
                EraseDrawRectangle(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+2) * erow_height) ;
                if(pagemax1==0)
                    EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1,len,row,erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
                else
                    EMenuOut(xpos + C_ECHAR_HDOT,ypos+4,ZSTDMenu_AB1+9*C_STD_LEN,C_STD_LEN,7,erow_height);


                Write_Number(xpos + 11* C_ECHAR_HDOT, ypos+10*erow_height+6,pagemax1+1,1,0,0);
                TextOut(xpos+12*C_ECHAR_HDOT,ypos+10*erow_height+4,1,3,24,"/2",4);


                while(1)
                {
                    keycode = MAnyKeyReturn();
                    if((keycode>=1&&keycode<=9&&pagemax1==0)\
                            ||(keycode>=1&&keycode<=7&&pagemax1==1)\
                            ||keycode==C_KEYCOD_PLUS||keycode==C_KEYCOD_MINUS||keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                        break;

                }
                if(keycode==C_KEYCOD_PLUS)
                {

                    pagemax1++;
                    if(pagemax1>1) pagemax1=0;
                    continue;

                }

                else if(keycode==C_KEYCOD_MINUS)
                {

                    pagemax1--;
                    if(pagemax1<0) pagemax1=1;
                    continue;

                }
                else if(keycode>=1&&keycode<=9&&pagemax1==0)
                {

                    if(keycode==2) keycode=11;
                    else if(keycode==3) keycode=12;
                    else if(keycode==4) keycode=13;
                    else if(keycode==5) keycode=8;
                    else if(keycode==6) keycode=2;
                    else if(keycode==7) keycode=4;
                    else if(keycode==8) keycode=5;
                    else if(keycode==9) keycode=6;

                    MSetStdMode(keycode-1,C_SETMODE_SAVE);
                    if (MGetStdMode() == 1)
                    {
                        MSetBlock(1);
                    }
                    else
                    {
                        MSetBlock(0);
                    }
                    if(MGetStdMode()==10||MGetStdMode()==11)
                        MSetDacMode(0,0);
                    else
                        MSetDacMode(2,0);

                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                    for( i = 0 ; i < k; i++)
                    {
                        if(MGetStdMode()==0)break;
                        else if(MGetStdMode()==10||MGetStdMode()==11)
                        {
                            if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                        else
                        {
                            if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }

                    if(MGetStdMode()==2)
                    {
                        int testrange[3];
                        int offset;
                        testrange[0] = 500;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    if(MGetStdMode()==12)//47013
                    {
                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 20;
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

                    break;
                }
                else if(keycode>=1&&keycode<=7&&pagemax1==1)
                {

                    if(keycode==1) keycode=7;
                    else if(keycode==2) keycode=9;
                    else if(keycode==3) keycode=10;
					else if(keycode==4) keycode=3;
					else if(keycode==5) keycode=14;
					else if(keycode==6) keycode=15;
					else if(keycode==7) keycode=16;
                    MSetStdMode(keycode-1,C_SETMODE_SAVE);

                    MSetBlock(0);

                    MSetDacMode(2,0);

                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );

                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                    for( i = 0 ; i < k; i++)
                    {
                        if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                        }
                        break;
                    }

                    break;
                }
                else if(keycode==C_KEYCOD_CONFIRM||keycode==C_KEYCOD_RETURN)
                    break;
            }
			if( MGetParaLock(0,C_LOCK_PROBE) == 1||MGetParaLock(0,C_LOCK_DAC) == 1)
			{
				
			}
			else 
			{
				if(MGetStdMode()==12&&MGetProbeMode()==1)
					MSetFrequence(500,2);
				else
					MSetFrequence(250,2);
			}
			
            MPopWindow();
            break;
        case 1:	//曲线数
            deci_len = 0;
            number = 100;
            while(1)
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,2, &deci_len,0) != 1)break;
                else
                {
                    if( number > 0 && number <= 6)
                    {
                        MSetCurveNumber(number);

                        break;
                    }
                    else	DisplayPrompt(4);
                }
            }
            break;
        case 2:	//试块类型
            //break;
            row =  _STDMenu_B1[MGetStdMode()][0]- '0' ;


            len = C_BLOCK_LEN;
            epText = (uint8_t*)( _STDMenu_B1[MGetStdMode()]+1 );
            MPushWindow(xpos, ypos, xpos+ 11*C_ECHAR_HDOT, ypos + (row+1) * erow_height) ;
            EraseDrawRectangle(xpos, ypos, xpos+ (len+2)*C_ECHAR_HDOT, ypos + (row+1) * erow_height) ;
            EMenuOut(xpos + C_ECHAR_HDOT, ypos+4, epText ,len, row, erow_height);	/*在指定位置根据每行字符数、行数、行高写菜单*/
            while(1)
            {
                keycode = MenuKeyReturn(row,2);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
                if (keycode == C_KEYCOD_RETURN||keycode == C_KEYCOD_CONFIRM)
                    break;
                else if(keycode >= 1 && keycode <= row)
                {
                    MSetBlock(keycode-1);
                    MSetCurveNumber( c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_LINENUM] );
                    k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];//试块种类
                    number = MGetThick()/10;
                    if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                    for( i = 0 ; i < k; i++)
                    {
                        if(MGetStdMode()==0)break;

                        if(MGetStdMode()==10||MGetStdMode()==11)
                        {
                            if(MGetBlock()!=0)
                                MSet11345CurveNum(1);
                            if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                        }
                        else
                        {
                            if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                        }

                        for( j = 0; j < MGetCurveNumber(); j++)
                        {
                            MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                        }
                        break;
                    }


                    if(MGetBlock()==1&&MGetStdMode()==12)
                    {
                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    else if(MGetBlock()==0&&MGetStdMode()==12)
                    {

                        int testrange[3];
                        int offset;
                        testrange[0] = 800;
                        testrange[1] = 20;
                        testrange[2] = 400;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    else if(MGetStdMode()==2)
                    {

                        int testrange[3];
                        int offset;
                        testrange[0] = 500;
                        testrange[1] = 10;
                        testrange[2] = 60;
                        offset = C_OFF_TEMP + 30;
                        MCopyProtectedMemory( (void*)offset, testrange, 12, PM_COPY_WRITE);
                    }
                    if(MGetStdMode()==12)//判断厚度是否在范围内
                    {

                        if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                                ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                                ||(MGetBlock()==3&&MGetThick()<40)    )

                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }

                    }

                    else if(MGetStdMode()==2)//判断厚度是否在范围内
                    {
                        if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                                ||(MGetBlock()==3&&MGetThick()<40) )
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    else if(MGetStdMode()==4)//判断厚度是否在范围内
                    {
                        if( MGetThick()<50)
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }

                    break;
                }
            }
            MPopWindow();
            break;
        case 3:	//工件厚度
            //break;
            if( MGetUnitType() > 0)deci_len = 3;//单位inch
            else deci_len = 1;
            number = 0;
            while(1)
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,4,&deci_len,0) != 1)break;

                else
                {
                    if(( number < 50000 && MGetUnitType()==0)||( number < (50000/0.254+0.5) && MGetUnitType()==1))
                    {
                        MSetThick(number);
                        number /= 10;
                        if( MGetUnitType() > 0)number = (UINT)(number * 254/1000.0+0.5);//单位inch
                        k = c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THNUM];
                        for( i = 0 ; i < k; i++)
                        {
                            if(MGetStdMode()==0)break;
                            if(MGetStdMode()==10||MGetStdMode()==11)
                            {
                                if( number >=c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;
                            }
                            else
                            {
                                if( number >c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i] )continue;

                            }
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, c_DACLine[MGetStdMode()][MGetBlock()][C_STD_POS_THICK+7*i+j+1] );
                            }
                            break;
                        }
                    }
                    if(MGetStdMode()==12)//判断厚度是否在范围内
                    {

                        if( (MGetBlock()==0&&(MGetThick()<60||MGetThick()>2000))\
                                ||(MGetBlock()==1&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=2000||MGetThick()>5000))\
                                ||(MGetBlock()==3&&MGetThick()<40) )

                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }

                    }

                    else if(MGetStdMode()==2)//判断厚度是否在范围内
                    {
                        if( (MGetBlock()==0&&(MGetThick()<80||MGetThick()>1200))\
                                ||(MGetBlock()==1&&(MGetThick()<60||MGetThick()>1200))\
                                ||(MGetBlock()==2&&(MGetThick()<=1200||MGetThick()>4000))\
                                || (MGetBlock()==3&&MGetThick()<40)   )
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    else if(MGetStdMode()==4)//判断厚度是否在范围内
                    {
                        if( MGetThick()<50)
                        {
                            for( j = 0; j < MGetCurveNumber(); j++)
                            {
                                MSetLineGain(j, 0 );
                            }
                        }
                    }
                    break;
                }
            }
            break;
        case 4:	//第一根线
        case 5:	//第2根线
        case 6:
        case 7:
        case 8:
        case 9:
            deci_len = 1;
            number = 0;
            if(MGetTestStatus(C_TEST_AVG) == 1)sign = 0;
            else sign = 1;
            line = keycode - 4;
            if( MGetCurveNumber() < line)break;
            while( 1 )
            {
                if( Input_Number(xpos,ypos-C_ECHAR_VDOT,&number,3, &deci_len,sign) < 1)break;
                else
                {
                    if(MGetTestStatus(C_TEST_AVG) == 1)
                    {
                        if( number >= 5 && number < 300)
                            number = (int)( 400 * log10( number/10.0) -120);
                        else break;
                    }
                    else if(number < -300 || number > 400)break;

                    MSetLineGain(line,number);
                    break;
                }
            }

            if(MGetStdMode()==10||MGetStdMode()==11)  MSet11345CurveNum(1);
            break;
        }
    }
    MSetColor(C_CR_UNDO);
    ScreenRenovate();	/*屏幕刷新*/
    MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
    MSetHardEchoShow(1,C_SETMODE_SETSAVE);
    ClearEnvelope();	//清包络或峰值
    if( retvalue == C_TRUE && MGetTestStatus(C_TEST_DAC) == 1)
    {
        MSetDelay(0,C_SETMODE_SETSAVE);
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
        DrawDac(2);		//新画DAC
        DrawDac(0);
        if( MGetThick()*2 <= MGetRange(4) /*&& MGetStdMode()!=11*/ )//if( MGetThick()*2 <= MGetRange(4) && MGetStdMode()<C_STD_MAX-1 )
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
            else if (MGetStdMode()==12||MGetStdMode()==9)	//47013
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

        MSetSystem();
        MSetColor(C_CR_PARA);
        WriteBaseGain(C_BASEGAIN_HPOSI,C_BASEGAIN_VPOSI);
        WriteCompGain(C_COMPGAIN_HPOSI,C_COMPGAIN_VPOSI);
        WriteSurfGain(C_SURFGAIN_HPOSI,C_SURFGAIN_VPOSI);
        //  WriteSurfGain(C_BASEGAIN_HPOSI + 9 * C_ECHAR_HDOT,C_BASEGAIN_VPOSI);
        SystemParaStore();
        ChannelParaStore();
        ScreenRenovate();
    }
    DrawDac(0);		//新画DAC

    return 1;
}


int DataCall(int file_no,int SaveMode)	//调用已存储的数据并覆盖到当前通道
{
    //显示提示:通道X的数据将被覆盖,Y/N?

    MEraseWindow(0,0,C_CCHAR_HDOT*12,25);
    if(CMessageBox( 0,0,(char *) _DataMenu_E1[MGetLanguage()][0],22,MB_YESNO) == MB_YES)
    {
        MSetChannel(MGetChannel(),C_SETMODE_SETSAVE);
        //	MSetSaveMode(0,0);
        MSetSaveMode(SaveMode,C_SETMODE_SETSAVE);
        MSetSaveFrequency(0,0);
        ChannelParaStore();
        SystemParaStore();	//将存储通道数据和系统数据调入存储区
        MSetSystem();
        return 1;

    }
    else return 0;
}
int TestFlawHigh(void)		//测高
{

    uint16_t xpos,ypos,menu_xpos,menu_ypos;
    short keycode;
    int high = 0, high1=0,high2=0,i;
    char num, dac_num = Dac.num;
    uint8_t len;//,row;
    uint8_t *samp;
    uint8_t sampbuff[C_LEN_SAMP+1] ;


    AllParaStore();
    Dac.num = 0;

    MSetFunctionMode(0, C_DAC_GATE);
    MSetTestStatus(C_TEST_ALL,0);	//设置测试状态,DAC作
    MSetAcquisition(1);
    ScreenRenovate();
    len = 11;
    menu_xpos = 10;
    menu_ypos = C_COORVPOSI+2;
    MSetGatePara(C_COORHORIUNIT * 6,C_COORHORIUNIT * 3,0,1,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);


    for( keycode = 0 ; keycode < C_LEN_SAMP+1; keycode++)sampbuff[keycode] = 0;
    num = 0;
    MSetDepth(0,0);
    while(true)
    {
        if( num == 0)
        {
            MSetGatePara(C_COORHORIUNIT * 4,C_COORHORIUNIT * 5,120,0,C_SETMODE_SETSAVE);
        }
        else
        {
            MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 7,120,0,C_SETMODE_SETSAVE);
        }
        MChannelRenovate();
        DisplayPrompt(15);
        //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//写：测缺陷高度
        TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
        EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+1+5*MGetLanguage())*C_CCHAR_HDOT, menu_ypos + 2 * 32+6+8*MGetLanguage()) ;
        if( num == 0)
        {
            //	CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG2 ,len,2,24);	//写：测缺陷下端
            TextOut(menu_xpos+7, menu_ypos+4,2+MGetLanguage(),23+MGetLanguage(),32-10*MGetLanguage(),(char *)_TestMenu_AG2[MGetLanguage()][0],4-3*MGetLanguage());

        }
        else
        {
            //	CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG3 ,len,2,24);	//写：测缺陷上端
            TextOut(menu_xpos+7, menu_ypos+4,2+MGetLanguage(),23+MGetLanguage(),32-10*MGetLanguage(),(char *)_TestMenu_AG2[MGetLanguage()][1],4-3*MGetLanguage());
        }
        DrawWave(sampbuff,0,C_LEN_SAMP);
        while(true)
        {


            if (!MGetFunctionMode(C_ECHOMAX_MEMORY))
            {
                if(MAdjustGain(0,1,MGetAmpStdMax()/2,MGetAmpStdMax()))
                {
                    MSetGateParaInit();	//门内最高波设初值
                    //	g_UDiskInfo.DataHeaderMark = 1;
                }
            }
            keycode = MGetKeyCode(0);
            //	StorageData(keycode);
            MParaRenovate(1);
            // gateamp = MGetAmpMax(gatetype);
            if(!MGetFunctionMode(C_ECHOMAX_MEMORY))
            {
                if(MGetGateParaMax(0) == C_TRUE)
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
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - (GateMax.Amp*2) ;
            }
            /*          if (MAdjustGain(0,0,MGetAmpStdMax()/2,MGetAmpStdMax()))
                      {
                          GateMax.Amp = 0;	//增益自动调节过
                      }
                      MParaRenovate(1);
                      keycode = MGetKeyCode(0);//Code(0);
                      if ( MGetGateParaMax(0) == C_TRUE )
                      {
                          ClearCursor(2);
                          xpos = GateMax.Pos;
                          ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2;
                          if((GateMax.Amp*2) < C_COORHEIGHT)DrawCursor(xpos,ypos,2);
                      }
            */
            if( keycode == C_KEYCOD_CONFIRM)
            {
                short left,right;
                left = right = MGetAmpPos(0);
                MSetAcquisitionEnable(0,C_SETMODE_SET);
                samp = GetSampleBuffer();
                while(samp[left - 1] <= samp[left] && samp[left] > 0) left-- ;
                while(samp[right] >= samp[right+1] && samp[right] > 0) right++ ;

                for( keycode = left-1; keycode <= right+1; keycode++)
                {
                    sampbuff[keycode] = samp[keycode];
                }
                MSetAcquisitionEnable(1,C_SETMODE_SET);

                num++;
                if( num == 1)
                {
                    high = MGetAmpTime(0) ;
                    high1 = (int)( (double)(high * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//距离
                    high1 = high1 * MGetAngle(3) / 10000;	//垂直
                }
                else if( num == 2)
                {
                    high2 = MGetAmpTime(0);
                    if( high > high2 )	high -= high2;
                    else high = high2 - high;
                    high2 = (int)( (double)(high2 * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5) ;	//距离
                    high2 = high2 * MGetAngle(3) / 10000;	//垂直
                }
                break;
            }
            else if( keycode == C_KEYCOD_RETURN)
            {
                if( DisplayQuery(4) != 1)continue;
                num = 10;//用来表示按下返回键
                break;
            }
            //else if( num == 0 && (keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN) )
            else if(  (keycode == C_KEYCOD_RANGE || keycode == C_KEYCOD_BASEGAIN || keycode == C_KEYCOD_GATE ) )
            {
                KeyManage(keycode,1);
                DisplayPrompt(15);
                //			CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//写：测缺陷高度
                TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
            }
#if C_DEVTYPE == 15 || C_DEVTYPE == 1
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
                        MSetFunctionMode(0,C_ECHOMAX_MEMORY);

                    }
                    MKeyRlx();
                    keycode =-1;
                    KeyManage(keycode,1);
                    DisplayPrompt(15);
                    TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
                    //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//写：测缺陷高度

                }
                else if(Function.funcMenu[i]==C_ECHOMAX_MEMORY+1)
                {
                    ClearEnvelope();
                    if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 1)
                    {
                        MSetFunctionMode(0,C_ECHOMAX_MEMORY);
                    }
                    else if( MGetFunctionMode(C_ECHOMAX_MEMORY) == 0)
                    {
                        MSetFunctionMode(1,C_ECHOMAX_MEMORY);
                        MSetFunctionMode(0,C_ECHO_ENVELOPE);
                        MSetSaveFrequency(0,0);
                    }
                    MKeyRlx();
                    keycode =-1;
                    KeyManage(keycode,1);
                    DisplayPrompt(15);
                    TextOut(0 , 0,1,16,32,(char *)_TestMenu_AG1[MGetLanguage()][0],4);
                    //	CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//写：测缺陷高度

                }
            }
#endif
            //else if( keycode == C_KEYCOD_SPEED || keycode == C_KEYCOD_OFFSET || keycode == C_KEYCOD_ANGLE);
            /*
            			else if( keycode == C_KEYCOD_GATE)
            			{
            				KeyManage(keycode,1);
            				DisplayPrompt(15);
            				CEMenuOut(0 , 0,  _TestMenu_AG1 ,5,1,24);	//写：测缺陷高度
            			}
            //*/
            //MKeyRlx();
        }
        MKeyRlx();

        if( num == 2)
        {
            //已确认两次，显示高度值
            MChannelRenovate();
            EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+2+3*MGetLanguage())*C_CCHAR_HDOT+8, menu_ypos + 2 * 32+6) ;
            TextOut(menu_xpos+7, menu_ypos+4,2,23,32,(char *)_TestMenu_AG2[MGetLanguage()][2],4-3*MGetLanguage());
//			EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + (len+1)*C_CCHAR_HDOT, menu_ypos + 2 * 24) ;
//			CEMenuOut( menu_xpos+8, menu_ypos+4,  _TestMenu_AG4 ,len,2,24);	//写：测缺陷高度
            if( MGetUnitType() > 0)EMenuOut(menu_xpos + 16 + (len-1) * C_CCHAR_HDOT, menu_ypos+4,"in",2,1,8);
            else EMenuOut(menu_xpos +16+ (len-1+3*MGetLanguage()) * C_CCHAR_HDOT, menu_ypos+4,"mm",2,1,8);

            DrawWave(sampbuff,0,C_LEN_SAMP);
            MSetAcquisition(0);
            high = (int)( (double)(high * MGetSpeed() )/(double)(C_SAMPLE_FREQ) +0.5)* MGetAngle(3) / 10000 ;
            Write_Number( menu_xpos+16 + (7+3*MGetLanguage())*C_CCHAR_HDOT,menu_ypos+4, high,4,1,0);
            MKeyRlx();
            while(true)
            {
                keycode = MGetKeyCode(0);
                if( keycode == C_KEYCOD_YES)
                {
                    //存储
                    int offset;
                    MChannelRenovate();
                    MSetDepth(high1 - high2,0);


                    DrawWave(sampbuff,0,C_LEN_SAMP);
                    offset = C_OFF_PEAK;
                    MCopyProtectedMemory( (void*)offset, &GatePeak, C_LEN_PEAK, PM_COPY_WRITE);//门参数
                    offset = C_OFF_SAMP;
                    MCopyProtectedMemory( (void*)offset, sampbuff, C_LEN_SAMP, PM_COPY_WRITE); //以上为波形
                    MSetSystemMode(0,0);		//设置成自动调节状态
                    ChannelParaStore();
                    SystemParaStore();
                    DataSave();
                    break;
                }
                else if( keycode == C_KEYCOD_NO)break;
            }
            break;
        }
        if( num == 10)break;//返回
    }

    Dac.num = dac_num;
    AllParaRestore();
    MSetDepth(0,0);
    MSetSystem();
    return 1;
}


int TestForging(void)
{
    GateMax.Amp = 0;	//最高波置初值为0

    int gain = 400,range = 1000,offset = 0, offset0 = 0, delay = 0,speed = MGetSpeed();
    uint32_t echotime = 0;
    short gateamp;
    int i;
    uint32_t char_len,row_number,crow_height;
    int number,deci_len,number1,deci_len1;
    int xpos,ypos,menu_xpos,menu_ypos;
    int confirm_num = 0;
    uint32_t preElapsedtime;
    int retvalue = C_TRUE;
    int keycode;
    int gatetype = 0;
	int Thicktemp;
	int BG,x,h,D1,D2,D3,L,X,Y,N,BF;
	double dB1,a,b,dB,dB2,c,dB3;




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
	MChannelRenovate();	//本通道屏幕参数刷表

	xpos = 0;
    ypos = 0;
    // EraseDrawRectangle(xpos, ypos, 590, 24) ;
    MSetColor(C_CR_MENU);
    TextOut(xpos, ypos,1,40+MGetLanguage()*12,20,(uint8_t*)_Menu_FJ[MGetLanguage()][0],0);
    number = 10000;
    deci_len = 2;
	while(1)
	{
		if ( Input_Number(xpos + (17+MGetLanguage()*16)*C_ECHAR_HDOT,ypos,&number,2,&deci_len,0) )
		{
			
			D3 =MGetCrystal_l()/1000;
			a = (MGetSpeed()*100/MGetFrequence()) ;
			
			N= 3*(D3*D3*10000)/(4*a/10);
			if(number<N)
			{
				DisplayPrompt(31);
				continue;
			}
			else
			{
			}
			
				range=number/4;
				MSetEchoMode(0,C_SETMODE_SAVE);
				MSetProbeMode(0,C_SETMODE_SAVE);
				MSetGatePara(C_COORHORIUNIT * 3-2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
				MSetPara(gain,0,0,2,range,MGetOffset(),delay,MGetAngle(0),MGetSpeed());
				MSetSystem();
				MChannelRenovate();	//本通道屏幕参数刷表
				MSetFunctionMode(0,C_FUNCTION_ALL);
				MSetAcquisition(1);
				MSetRange(range,C_SETMODE_SETSAVE);


				char_len = 7;
				row_number = 3;
				crow_height = 48-14*MGetLanguage();

				menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-30*MGetLanguage()+C_COORHORIUNIT-23;
				menu_ypos = C_COORVPOSI + 1;
				MSetColor(C_CR_MENU);
				EraseDrawRectangle(menu_xpos-30-15*MGetLanguage(), menu_ypos, C_COORHORIUNIT * 10+17+MGetLanguage()*5, menu_ypos + crow_height *(row_number+MGetLanguage())) ;
				TextOut(menu_xpos + C_ECHAR_HDOT-40-15*MGetLanguage(),menu_ypos+4, row_number+2*MGetLanguage(), 17+MGetLanguage()*2,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][0], 4);
				WriteLongness(menu_xpos -25+ 1 * C_ECHAR_HDOT+170*MGetLanguage(), menu_ypos  + 3-30*MGetLanguage()+100 ,800,2/*总长*/,0/*是否写单位*/);
				EMenuOut(menu_xpos+25 + 1 * C_ECHAR_HDOT+170*MGetLanguage(), menu_ypos  + 20-30*MGetLanguage()+80,"%",2,1,8);
				MSetColor(C_CR_UNDO);




				xpos = 0;
				ypos = 0;
				ClearCursor(2);
				MSetGateParaInit();	//门内最高波设初值
				// DisplayPrompt(15);
				MSetColor(C_CR_MENU);
				TextOut(0,0,1,100,24,(uint8_t*)_TestMenu_A1[MGetLanguage()][7]+2,4);
				while (1)
				{
					{
						if(MAdjustGain(0,1,0,MGetAmpStdMax()))
						{
							MSetGateParaInit();	//门内最高波设初值
						}

					}
					keycode = MGetKeyCode(0);
					// MParaRenovate(1);
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

					if(keycode == C_KEYCOD_CONFIRM && confirm_num == 0)
					{
						confirm_num++;
						ClearCursor(2);
						BG = MGetBaseGain();
						x = number/100;
						D1 = 20;
						a = (MGetSpeed()*100/MGetFrequence()) ;
						b = (2*a*x)/(PI*D1*D1);
						dB1= (200*log10(b/10)*10+5)/10;
						dB = dB1+BG;
						MSetColor(C_CR_MENU);
						MSetAcquisition(0);
						MFclearScreen();
						// D3 =MGetCrystal_l()/1000;

						TextOut(5, 150,1,40+MGetLanguage()*12,20,(uint8_t*)_Menu_FJ[MGetLanguage()][1],0);
						TextOut(5, 180,1,40+MGetLanguage()*12,20,(uint8_t*)_Menu_FJ[MGetLanguage()][2],0);
						TextOut(5, 210,1,40+MGetLanguage()*12,20,(uint8_t*)_Menu_FJ[MGetLanguage()][3],0);
						TextOut(5, 240,1,40+MGetLanguage()*12,20,(uint8_t*)_Menu_FJ[MGetLanguage()][4],0);
						EraseDrawRectangle(26*C_ECHAR_HDOT, 380, 26*C_ECHAR_HDOT + (5+2)*C_CCHAR_HDOT, 380 + 1 * (C_CCHAR_VDOT+12)) ;
						TextOut(27*C_ECHAR_HDOT,380+4,1,10,24,(uint8_t*)_MenuPrompt_P1[MGetLanguage()][0],4);

						MSetColor(C_CR_PARA);
						Write_Number(340+MGetLanguage()*180,150,BG,4,1,0);
						Write_Number(150+MGetLanguage()*210,180,number/10,5,1,0);
						// Write_Number(340,150,BG,4,1,0);
						// Write_Number(150,180,number/10,5,1,0);
						if(dB1<10||dB1>500)
						{
							Write_Number(390,210,0,4,1,0);
							Write_Number(260,240,0,4,1,0);
						}
						else
						{
							Write_Number(390+MGetLanguage()*180,210,dB1,4,1,0);
							Write_Number(260+MGetLanguage()*290,240,dB,4,1,0);
						} 
						// Write_Number(260,270,a,4,0,0);
						// Write_Number(260,300,D3,4,0,0);
						// Write_Number(260,330,N,6,0,0);

					}
					else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 1)
					{				
						confirm_num++;
						MSetBaseGain(dB,C_SETMODE_SAVE);
						MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 5.5,120,0,C_SETMODE_SETSAVE);
						MSetPara(dB,0,0,2,number/8,MGetOffset(),delay,MGetAngle(0),MGetSpeed());
						MSetSystem();
						MChannelRenovate();	//本通道屏幕参数刷表
						MSetFunctionMode(0,C_FUNCTION_ALL);
						MSetAcquisition(1);
						MSetRange(number/8,C_SETMODE_SETSAVE);
						
						char_len = 7;
						row_number = 3;
						crow_height = 48-14*MGetLanguage();

						menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-30*MGetLanguage()+C_COORHORIUNIT-33;
						menu_ypos = C_COORVPOSI + 1;
						MSetColor(C_CR_MENU);
						if(MGetLanguage()==0)
						{
							EraseDrawRectangle(menu_xpos, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())-60) ;
							TextOut(menu_xpos + C_ECHAR_HDOT-10-12*MGetLanguage(),menu_ypos+4, row_number+2*MGetLanguage(), 16+MGetLanguage()*20,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][1], 4);
							TextOut(menu_xpos + C_ECHAR_HDOT-10-50*MGetLanguage(),menu_ypos+28, row_number+2*MGetLanguage(), 16+MGetLanguage()*20,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][2], 4);
							TextOut(menu_xpos + C_ECHAR_HDOT-10-12*MGetLanguage(),menu_ypos+52, row_number+2*MGetLanguage(), 16+MGetLanguage()*20,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][3], 4);
							WriteLongness(menu_xpos+25 + 1 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage()+50 ,800,2/*总长*/,0/*是否写单位*/);
							EMenuOut(menu_xpos+65 + 1 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage()+32,"%",2,1,8);
						}
						else 
						{
							EraseDrawRectangle(menu_xpos + C_ECHAR_HDOT-20-100*MGetLanguage(), menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())-30) ;
							TextOut(menu_xpos + C_ECHAR_HDOT-10-100*MGetLanguage(),menu_ypos+4, row_number+1*MGetLanguage(), 16+MGetLanguage()*6,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][1], 4);
							WriteLongness(menu_xpos+25 + 1 * C_ECHAR_HDOT+16*MGetLanguage()-100*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage()+20 ,800,2/*总长*/,0/*是否写单位*/);
							EMenuOut(menu_xpos+45 + 1 * C_ECHAR_HDOT+40*MGetLanguage()-100*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage()+32,"%",2,1,8);
						}
						MSetColor(C_CR_UNDO);
						
						xpos = 0;
						ypos = 0;
						ClearCursor(2);
						MSetGateParaInit();	//门内最高波设初值
						DisplayPrompt(15);
						MSetColor(C_CR_MENU);
						// MEraseWindow(0,ypos,C_CCHAR_HDOT*16,C_CCHAR_VDOT+ypos);
						TextOut(0,0,1,38,26,(uint8_t*)_Forging_A2[MGetLanguage()][0],4);


					}
					else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 2)
					{				
						confirm_num++;
						dB2 = MGetBaseGain();
						h= MGetAmpTime(0) * MGetSpeed() / C_SAMPLE_FREQ;
						
						
						// MSetBaseGain(dB,C_SETMODE_SAVE);
						MSetGatePara(C_COORHORIUNIT * 2,C_COORHORIUNIT * 7,120,0,C_SETMODE_SETSAVE);
						// MSetPara(dB,0,0,2,number/8,MGetOffset(),delay,MGetAngle(0),MGetSpeed());
						MSetSystem();
						MChannelRenovate();	//本通道屏幕参数刷表
						MSetFunctionMode(0,C_FUNCTION_ALL);
						MSetAcquisition(1);
						// MSetRange(number/8,C_SETMODE_SETSAVE);
						
						char_len = 7;
						row_number = 3;
						crow_height = 48-14*MGetLanguage();

						menu_xpos = C_COORHORIUNIT * 10-(char_len+2+MGetLanguage())*C_CCHAR_HDOT-30*MGetLanguage()+C_COORHORIUNIT-33;
						menu_ypos = C_COORVPOSI + 1;
						MSetColor(C_CR_MENU);
						if(MGetLanguage()==0)
						{
							EraseDrawRectangle(menu_xpos-24, menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())-60) ;
							TextOut(menu_xpos + C_ECHAR_HDOT-34,menu_ypos+4, row_number+2*MGetLanguage(), 18+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][4], 4);
							TextOut(menu_xpos + C_ECHAR_HDOT-34,menu_ypos+28, row_number+2*MGetLanguage(), 18+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][5], 4);
							TextOut(menu_xpos + C_ECHAR_HDOT-34,menu_ypos+52, row_number+2*MGetLanguage(), 18+MGetLanguage(),crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][6], 4);
							WriteLongness(menu_xpos+40 + 1 * C_ECHAR_HDOT+16*MGetLanguage(), menu_ypos  + 3+(crow_height+13)*1*MGetLanguage()+50 ,800,2/*总长*/,0/*是否写单位*/);
							EMenuOut(menu_xpos+75 + 1 * C_ECHAR_HDOT+40*MGetLanguage(), menu_ypos  + 20+20*MGetLanguage()+32,"%",2,1,8);
						}
						else 
						{
							EraseDrawRectangle(menu_xpos + C_ECHAR_HDOT-20-100*MGetLanguage(), menu_ypos, C_COORHORIUNIT * 10+17, menu_ypos + crow_height *(row_number+MGetLanguage())-30) ;
							TextOut(menu_xpos + C_ECHAR_HDOT-10-100*MGetLanguage(),menu_ypos+4, row_number+1*MGetLanguage(), 16+MGetLanguage()*6,crow_height-12*MGetLanguage(), (uint8_t*)_TestForging[MGetLanguage()][4], 4);
							WriteLongness(menu_xpos+25 + 1 * C_ECHAR_HDOT+146*MGetLanguage(), menu_ypos +20+1*MGetLanguage()+30 ,800,2/*总长*/,0/*是否写单位*/);
							EMenuOut(menu_xpos+45 + 1 * C_ECHAR_HDOT+170*MGetLanguage(), menu_ypos  + 20+1*MGetLanguage()+30,"%",2,1,8);
						}
						MSetColor(C_CR_UNDO);						
					}
					else if((keycode == C_KEYCOD_CONFIRM && confirm_num == 3)||(keycode == C_KEYCOD_RETURN && confirm_num == 4))
					{
						int row;
						X=0;
						Y=0;
						if(confirm_num == 3)
						{
							confirm_num++;
						}
						BF = MGetBaseGain();
						
						MChannelRenovate();	//本通道屏幕参数刷表
						char_len = 7;
						row_number = 3;
						crow_height = 48;

						menu_xpos = C_COORHORIUNIT * 10-(char_len+2)*C_CCHAR_HDOT+C_COORHORIUNIT-260;
						menu_ypos = C_COORVPOSI + 1;
						MSetColor(C_CR_MENU);
					
						EraseDrawRectangle(menu_xpos, menu_ypos, menu_xpos + 16*C_CCHAR_HDOT+30, menu_ypos + crow_height * 4) ;
						TextOut(menu_xpos+4,menu_ypos+8,1,34,32,(uint8_t*)_Forging_A1[MGetLanguage()][0],4);
						TextOut(menu_xpos+4,menu_ypos+8+crow_height,1,34,32,(uint8_t*)_Forging_A1[MGetLanguage()][1],4);
						TextOut( menu_xpos+4,menu_ypos+4+crow_height*2,1,34,16, (uint8_t*)_MenuPrompt_P3[MGetLanguage()][0], 24 );
						TextOut( menu_xpos+4,menu_ypos+4+crow_height*3,1,34,16, (uint8_t*)_MenuPrompt_P4[MGetLanguage()][0], 24 );
						
						MSetColor(C_CR_UNDO);	
						row = 0;

						Write_Number(menu_xpos+270,menu_ypos+10,X,6,1,1);

						row = 1;
						Write_Number(menu_xpos+270,menu_ypos+10+crow_height,Y,6,1,1);



						while(1)
						{
							
							row = 0;

							Write_Number(menu_xpos+270,menu_ypos+10,X,6,1,1);
							deci_len1 = 1;
							number1 = 0;
							row = 1;
							Write_Number(menu_xpos+270,menu_ypos+10+crow_height,Y,6,1,1);
							if( Input_Number(menu_xpos+270,menu_ypos+10,&number1,2, &deci_len1,1) )
							{
								X=number1;
								row = 0;

								Write_Number(menu_xpos+270,menu_ypos+10,X,6,1,1);
								if(Input_Number(menu_xpos+270,menu_ypos+10+crow_height,&number1,2, &deci_len1,1))
								{
									Y=number1;
									row = 1;
									Write_Number(menu_xpos+270,menu_ypos+10+crow_height,Y,6,1,1);
									break;
								}
								// break;
							}
						}
					}
					else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 4)
					{
						// confirm_num++;
						confirm_num = 1;
						ClearCursor(2);
						// dB2 = MGetBaseGain();
						// h= MGetAmpTime(0) * MGetSpeed() / C_SAMPLE_FREQ;
						D1 = 20;
						b = (2*a*x)/(PI*D1*D1);
						dB1= (200*log10(b/10)*10+5)/10;
						dB = dB1+BG;
						c=pow(pow(10,(dB1+BG-dB2)/4),0.01);
						D2=(c*D1*h)/(x*10);
						dB3=(40*(200-log10(D2*100/40)*100)+5)/10;
						MSetColor(C_CR_MENU);
						MSetAcquisition(0);
						MFclearScreen();

						TextOut(5, 120,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][5],0);
						TextOut(5, 150,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][6],0);
						TextOut(5, 180,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][7],0);
						TextOut(5, 210,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][8],0);
						TextOut(5, 240,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][9],0);
						TextOut(5, 270,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][10],0);
						TextOut(5, 300,1,40+MGetLanguage()*0,20,(uint8_t*)_Menu_FJ[MGetLanguage()][11],0);
						EraseDrawRectangle(26*C_ECHAR_HDOT-200, 380, 26*C_ECHAR_HDOT + (8+1)*C_CCHAR_HDOT, 380 + 1 * (C_CCHAR_VDOT+12)) ;
						TextOut(27*C_ECHAR_HDOT-200,380+4,1,30,50,(uint8_t*)_MenuPrompt_P2[MGetLanguage()][0],4);

						MSetColor(C_CR_PARA);

						Write_Number(280+190*MGetLanguage(),120,dB2,4,1,0);
						Write_Number(150+110*MGetLanguage(),150,h,5,1,0);
						Write_Number(170+160*MGetLanguage(),180,D2,3,1,0);
						Write_Number(170+110*MGetLanguage(),210,X,6,1,1);
						Write_Number(170+110*MGetLanguage(),240,Y,6,1,1);
						
						if(D2<40&&D2>0)
						{
							EMenuOut(310+170*MGetLanguage(),270,"-",1,1,8);
							dB3=(40*(200-log10(D2*100/40)*100)+5)/10;
						}
						else if(D2>=40)
						{
							EMenuOut(310+170*MGetLanguage(),270,"+",1,1,8);
							dB3=(40*(log10(D2*100/40)*100-200)+5)/10;
						}
						else
						{
							dB3=0;
						}
						Write_Number(330+170*MGetLanguage(),270,dB3,4,1,0);
						Write_Number(290+220*MGetLanguage(),300,BF,4,1,0);
						

					}
					// else if(keycode == C_KEYCOD_CONFIRM && confirm_num == 4)
					// {
						// confirm_num = 1;
					// }
					else if(keycode == C_KEYCOD_RETURN)
					{
						if( DisplayQuery(4) == 1)
						{
							retvalue = C_FALSE;
								MSetRange(2000,C_SETMODE_SETSAVE);
								MSetBaseGain(450,C_SETMODE_SAVE);
							MSetGatePara(C_COORHORIUNIT * 2-2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
							break;
						}
						// break;
					}
					else if(keycode == C_KEYCOD_AUTOGAIN )
					{
						MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax());
						MSetGateParaInit();	//门内最高波设初值
						MKeyRlx();
					}
					else if(keycode == C_KEYCOD_BASEGAIN)
					{
						GainChange();
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
					else continue;
				}
				retvalue = C_TRUE;

		}
		break;
    }
	MKeyRlx();
	// if(dB==0||range==0)
	// {
		MSetRange(2000,C_SETMODE_SETSAVE);
		MSetBaseGain(450,C_SETMODE_SAVE);
								MSetSystem();
						// MChannelRenovate();
	// }
	// else
	// {
		// MSetRange(range,C_SETMODE_SETSAVE);
		// MSetBaseGain(dB,C_SETMODE_SAVE);
	// }
	MSetGatePara(C_COORHORIUNIT * 2-2,C_COORHORIUNIT * 3,120,0,C_SETMODE_SETSAVE);
    MSetFunctionMode(0,C_ECHOMAX_MEMORY);

    return retvalue;
}




/*
端点6dB法
调参量移探头使缺陷左端
最高波在进波门内en确认

左移探头当左端点回波降
为40％时作标记并en确认


调参量移探头使缺陷右端
最高波在进波门内en确认

右移探头当右端点回波降
为40％时作标记并en确认

将量出的两标记间长度在
此输入：    mm，en确认

6dB法
调参量移探头使缺陷
最高波在进波门内en确认


分别左右移探头当回波降
为40％时作标记并en确认

将量出的两标记间长度在
此输入：    mm，en确认

*/
#define C_LEN_BUFF 32
#define C_BASE		0
#define C_WAVENUM	2
#define C_REJ	10

void BScan(void)//B扫描
{
    int gain ,range;//, angle, speed;
    uint32_t amp,nextamp;
//	int gatetype = 0;
    uint32_t gateamp;
//	uint32_t char_len,row_number,crow_height;
    //int number,deci_len;
//	int xposPeak,yposPeak,
    int xpos,ypos,x0,y0;
    uint32_t preElapsedtime;
    int keycode;
    uint8_t* sampbuffer;
    int i, j, k,l;
    int tm ;
    uint8_t bug[C_LEN_BUFF][4][2];
    int ip,jp,kp,Unit;//,lp;
    int flag;//=0表示前一个点无底波无回波，1有底波无回波，2有回波无底波，3有回波有底波
    int preypos=0;
    int high=100;

    AllParaStore();
    MSetTestStatus(C_TEST_DAC,0);	//设置测试状态,DAC作
    MSetTestStatus(C_TEST_AVG,0);	//设置测试状态,AVG作
    MSetAcquisition(0);
    ScreenRenovate();
    MKeyRlx();
    //设置成直探头
    MSetProbeMode(0,C_SETMODE_SAVE);
    if( MGetUnitType() > 0)MSetSpeed(CD_SPEED_1*1000/254,C_SETMODE_SAVE);//单位inch
    else MSetSpeed(CD_SPEED_1,C_SETMODE_SAVE);
    if (MGetUnitType() > 0)
    {
        MSetCrystal_l(787,C_SETMODE_SETSAVE);
    }
    else
    {
        MSetCrystal_l(20000,C_SETMODE_SETSAVE);
    }

    MSetCrystal_w(0,C_SETMODE_SETSAVE);
    MSetAngle(0,C_SETMODE_SAVE);
    MSetForward(0,C_SETMODE_SAVE);
    if( DisplayQuery(1) == 1)
    {
        if( TestMenuOffset() == C_FALSE)
        {
            MSetAcquisition(1);
            AllParaRestore();
            MSetSystem();
            ScreenRenovate();
            DrawDac(0);
            return ;
        }
    }
    ScreenRenovate();
    tm = 32;//临时用作行高
    xpos = ( C_HORIDOT_SCREEN - C_CCHAR_HDOT * 6 - C_ECHAR_HDOT * 15) / 8 * 8;
    ypos = C_COORVPOSI;
    EraseDrawRectangle(xpos, ypos, xpos+ (13+MGetLanguage()*3)*C_CCHAR_HDOT, ypos + 1 * tm+8) ;
    /* if (MGetLanguage())
     {
         EMenuOut(xpos + 2,ypos+4,"1.Thickness:",12,1,16);
     }
     else
     {
         CEMenuOut(xpos + C_ECHAR_HDOT,ypos+4,_BSCAN_B2,6,1,tm);
     }
     */
    TextOut(xpos + C_ECHAR_HDOT,ypos+4,1,12,32,(char *)_Bscan[MGetLanguage()][3],4);
    x0 = xpos + (7+MGetLanguage()*2) * C_CCHAR_HDOT;	/*写参数，位置水平方向右移*/
    y0 = ypos + 4;
    if( MGetUnitType() > 0)
    {
        Unit = 3;//单位inch
        range = 3973;
    }
    else
    {
        Unit = 1;
        range = 1000;
    }
    while(true)
    {
        xpos = x0;
        ypos = y0 + tm *0 ;
        WriteLongness(xpos,ypos+1 ,range,5,1);
        //	Write_Number(xpos,ypos+C_ECHAR_VDOT+1 ,range,4,1,0);
        //	EMenuOut(xpos + 5 * C_ECHAR_VDOT,ypos+8,"mm",2,1,8);

        keycode = MenuKeyReturn(3,3);	/*出现菜单时，按不大于keymax的键返回，mode=0其它键无效，=1确认键有效，=2返回键有效，=3确认返回键有效*/
        ypos = y0 + tm * (keycode - 1);

        if (keycode == 1)
        {
            if ( Input_Number(xpos,ypos,&range,5, &Unit,0) != 1)
            {
                if(range < C_MAX_RANGE && MGetUnitType()==0)
                {
                    range = 1000;

                }
                else if( range < (C_MAX_RANGE/0.254+0.5) && MGetUnitType()==1)
                {
                    range = 3973;
                }
            }
            //	break;
        }
        else if( keycode == C_KEYCOD_RETURN )
        {
            //	range = 1000;
            break;
        }
        else if( keycode == C_KEYCOD_CONFIRM )
        {
            break;
        }
    }
    gain = MGetBaseGain();
//	Write_Number(10,50,range,10,0,0);
//	MAnyKeyReturn();
    range = range * 10/8;
//	Write_Number(10,60,range,10,0,0);
//	MAnyKeyReturn();
    MSetPara(gain,0,0,0,range,MGetOffset(),0,MGetAngle(0),MGetSpeed() );

    MSetFunctionMode(0,C_FUNCTION_ALL);
    MSetEchoMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7,C_COORHORIUNIT * 2,0,0,C_SETMODE_SETSAVE);
    MSetBGateMode(0,C_SETMODE_SETSAVE);
    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,0,1,C_SETMODE_SETSAVE);

    MSetSystem();

    MSetAcquisition(1);
    ScreenRenovate();
    xpos = 16;
    ypos = C_COORVPOSI+1;
    EraseDrawRectangle(xpos, ypos, xpos+ (12+MGetLanguage()*4)*C_CCHAR_HDOT, ypos + 1 * 38) ;
//	CEMenuOut(xpos + 4,ypos+2,_BSCAN_B3,9,1,16);
    TextOut(xpos + 4,ypos+2,1,21,32,(char *)_Bscan[MGetLanguage()][1],2);

    MSetGatePara(C_COORHORIUNIT * 7.5,C_COORHORIUNIT * 1,140,0,C_SETMODE_SETSAVE);
    MDrawGate(-1,0,-1,0);
    while(true)
    {
        if(MAdjustGain(0,1,80,160))
        {
            MSetGateParaInit();	//门内最高波设初值
        }
        gateamp = MGetAmpMax(0);
        if(!MGetFunctionMode(C_ECHOMAX_MEMORY))
        {
            if(MGetGateParaMax(0) == C_TRUE)
            {
                ClearCursor(2);
                xpos = GateMax.Pos;
                ypos = C_COORVPOSI + C_COORHEIGHT - 2 - GateMax.Amp*2 ;
                if((GateMax.Amp*2) < C_COORHEIGHT)	DrawCursor(xpos,ypos,2);
            }

        }

        MParaRenovate(0);

        keycode = MGetKeyCode(0);
        if( keycode == C_KEYCOD_RETURN )
        {
            break;
        }
        else if( keycode == C_KEYCOD_CONFIRM )
        {
            ExpendTime(30);
            //	MAdjustGain(0,1,160,160 );
            //	MSetBaseGain( MGetBaseGain()+ 60 * range/500,C_SETMODE_SETSAVE);
            break;
        }
    }


    xpos = 0;
    ypos = 0;
    MSetColor(C_CR_MENU);
    MKeyRlx();


    //MAdjustGain(0,0,MGetAmpStdMax(),MGetAmpStdMax() );
    MFclearScreen();
    MSetColor(C_CR_PARA);
    MCoorDraw(C_COORHPOSI ,C_COORVPOSI , C_COORHEIGHT , C_COORWIDTH );	/*画指定位置和大小的波形坐标*/
//	CEMenuOut(0,0,_BSCAN_B4,9,1,16);
    TextOut(0,0,1,18,32,(char *)_Bscan[MGetLanguage()][2],4);
//	WriteScale();	//标度
    MSetColor(C_CR_MENU);
    for( i = 0 ; i < C_LEN_BUFF; i++)
    {
        for( j = 0; j < 4; j++)
        {
            bug[i][j][0] = 0;
            bug[i][j][1] = 0;
        }
    }
    x0 = 0;
    preypos = y0 = C_COORVPOSI +30;//C_COORVPOSI + C_COORHEIGHT;
    tm = 30;//30;//间隔时间长度
    preElapsedtime = GetElapsedTime();	//开始测试时间

    for( xpos = 0,ip = 0, flag = 1; xpos < C_COORWIDTH; xpos++)
    {
        keycode = MGetKeyCode(0);
        if(keycode == C_KEYCOD_RETURN)
        {
            if( DisplayQuery(4) == 1)
            {
                break;
            }
        }
//		MParaRenovate(0);
//;		MSetAcquisitionEnable(0,C_SETMODE_SETSAVE);
        sampbuffer = GetSampleBuffer();

        for( j = 0; j < 4; j++)
        {
            bug[ip][j][0] = 0;
            bug[ip][j][1] = 0;
        }
        k = C_COORWIDTH*80/100 ;
        for( i = k+5,amp = 0; i >= k - 5 ; i--)
        {
            //从波形水平0－80中找峰波
            nextamp = *(sampbuffer+i);
//			if( nextamp > C_REJ )
            {
                //波高必须大于5，表示一个波形开始
                for( l = i-1  ; l >= k - 5 && nextamp >= C_REJ ; l--)//波高小于2表示一个波形结束
                {
                    nextamp = *(sampbuffer+l);	//当前波峰值
                    if( nextamp > amp )
                    {
                        //目前的最高波
                        amp = nextamp;
                        i = l;
                    }
                }
            }
        }
        bug[ip][0][0] = C_COORWIDTH*80/100;
        bug[ip][0][1] = (amp>>2)<<2;//保存底波的位置和高度

        k = C_COORWIDTH*80/100 - 5;
        i = 28;
        for( amp = *(sampbuffer+i) ; i <= 56 ; i++)
        {
            //从波形水平0－80中找峰波
            if( amp <= C_REJ )break;
            amp = *(sampbuffer+i);
        }//去除始波

        for( amp = *(sampbuffer+i), jp = 1 ; i <= k ; i++)
        {
            //从波形水平0－80中找峰波
            if( amp > C_REJ )
            {
                //波高必须大于5，表示一个波形开始
                for( l = i+1 ,nextamp = amp; l < k && nextamp >= 2 ; l++)//波高小于2表示一个波形结束
                {
                    nextamp = *(sampbuffer+l);	//当前波峰值
                    if( nextamp > amp )
                    {
                        //目前的最高波
                        amp = nextamp;
                        i = l;
                    }
                }
                bug[ip][jp][0] = i;//(i>>2)<<2;
                bug[ip][jp][1] = (amp>>2)<<2;//保存一个峰波的位置和高度

                ///	break;//如果只找一个缺陷波，则在此跳出，否则改行去除

                jp++;
                if( jp > C_WAVENUM + 1)//含底波已有4个峰波,[0]底波[1][2][3]
                {
                    break;
                    if( bug[ip][1][1] <= bug[ip][2][1] && bug[ip][1][1] <= bug[ip][3][1])
                    {
                        //第一个最低
                        bug[ip][1][1] = bug[ip][2][1];
                        bug[ip][1][0] = bug[ip][2][0];
                        bug[ip][2][1] = bug[ip][3][1];
                        bug[ip][2][0] = bug[ip][3][0];
                    }
                    else if( bug[ip][2][1] <= bug[ip][3][1] )
                    {
                        //第二个最低
                        bug[ip][2][1] = bug[ip][3][1];
                        bug[ip][2][0] = bug[ip][3][0];
                    }
                    jp--;
                }
                i = l + 5;
            }
            nextamp = *(sampbuffer+i);	//当前波峰值
            amp = nextamp;
        }
        /*
        		if( bug[ip][0][1] > 32)
        		{//有底波,且较高
        			if( bug[ip][1][1] * bug[ip][2][0] * 2< bug[ip][2][1] * bug[ip][1][1])
        			{
        				 bug[ip][1][1] = bug[ip][2][1] ;
        				 bug[ip][1][0] = bug[ip][2][0] ;
        			}
        		}
        		else
        		{
        		}
        		if( bug[ip][0][1] == 0)
        		{//无底波
        			for( jp = 1; jp < 4; jp++)
        			{

        				if( bug[ip][jp][1] > 0)
        				{//无底波
        					for( j = jp ; j < 4; j++)
        					{
        //						 bug[ip][j-jp][1] = bug[ip][j][1] ;
        //						 bug[ip][j-jp][0] = bug[ip][j][0] ;
        					}
        					break;
        				}
        			}
        		}
        /*/
        if( bug[ip][1][1] > 16 || bug[ip][1][1] * 8 > bug[ip][0][1] )
        {
            //无底波
            bug[ip][0][1] = bug[ip][1][1] ;
            bug[ip][0][0] = bug[ip][1][0] ;
        }
//*/
//;		MSetAcquisitionEnable(1,C_SETMODE_SETSAVE);
        if( GetElapsedTime() <= preElapsedtime + tm)
        {
            xpos--;//间隔时间
            continue;
        }
        else preElapsedtime = GetElapsedTime();

        for( kp = 0 ; kp < C_LEN_BUFF; kp++)
        {
            for( jp = 0; jp < 4; jp++)
            {
//		Write_Number( kp*20,30+19*jp,bug[kp][jp][0]/4,2,0,0);
//		Write_Number( kp*20,30+19*jp+9,bug[kp][jp][1]/4,2,0,0);
            }
        }

        ip++;
        if( ip >= C_LEN_BUFF)
        {
            //缓冲区已满
            ip = C_LEN_BUFF - 1;
            jp = C_LEN_BUFF/2;
            //flag;//=0表示前一个点无底波无回波，1有底波无回波，2有回波无底波，3有回波有底波
//Write_Number(0,0,bug[jp][0][1],4,0,0);
//Write_Number(0,10,bug[jp][0][0],4,0,0);
//Write_Number(40,0,bug[jp][1][1],4,0,0);
//Write_Number(40,10,bug[jp][1][0],4,0,0);
//Write_Number(80,0,bug[jp][2][1],4,0,0);
//Write_Number(80,10,bug[jp][2][0],4,0,0);
            if( bug[jp][0][1] > 32  && bug[jp][0][1] *2> bug[jp][1][1])
            {
                //有底波,且有一定的高度
                if( bug[jp][1][1] < 64 && bug[jp][1][1] * 2 < bug[jp][0][1] && bug[jp][0][1] > 128 )
                {
                    //画底波
                    kp =  bug[jp][C_BASE][0];//位置
                    amp = bug[jp][C_BASE][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
#if C_DEVLIB == 23
                    MDrawPixel( x0, ypos ,0);
#else
                    MDrawPixel( x0, ypos , DP_SET);
#endif
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 1;
                }
                else if( bug[jp][1][1] > 64 ||
                         (bug[jp][1][1] > 32 && bug[jp][1][1] * 2 > bug[jp][0][1] ) )
                {
                    //缺陷波高于底波的1/2,或者底波小于80
                    kp =  bug[jp][1][0];//位置
                    amp = bug[jp][1][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
#if C_DEVLIB == 23
                    MDrawPixel( x0, ypos ,0);
#else
                    MDrawPixel( x0, ypos , DP_SET);
#endif

                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
                else// if( bug[jp][1][1] > 32 && bug[jp][0][1] > 128+64 && bug[jp][1][1]*8 >  bug[jp][0][1])
                {
                    //缺陷波底波都有一定高度，
                    kp =  bug[jp][1][0];//位置
                    amp = bug[jp][1][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);

                    kp =  bug[jp][C_BASE][0];//位置
                    amp = bug[jp][C_BASE][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    flag = 1;
                }



            }
            else//无底波
            {
                if( bug[jp][1][1] > 0  && bug[jp][2][1] > 0 )//有两个缺陷波
                {
                    if( ( bug[jp][1][1] < 64 && bug[jp][1][1] * 4 < bug[jp][2][1] ) ||
                            bug[jp][2][1] > 128 + 32)
                    {
                        //缺陷波高于底波的1/2,或者底波小于80
                        kp =  bug[jp][2][0];//位置
                        amp = bug[jp][2][1];//波高
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                        y0 = ypos;
                        flag = 2;
                    }
                    else if( ( bug[jp][1][1] * 4 > bug[jp][2][1] && bug[jp][2][1] < 128 + 64 ) ||
                             (  bug[jp][1][1] > 16 && bug[jp][1][1] * 2 > bug[jp][2][1]  ) )
                    {
                        //无波
                        kp =  bug[jp][1][0];//位置
                        amp = bug[jp][1][1];//波高
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                        y0 = ypos;
                        flag = 2;
                    }
                    else// if( bug[jp][2][1] > 32 && bug[jp][1][1] > 128+64 && bug[jp][2][1]*8 >  bug[jp][1][1])
                    {
                        //缺陷波底波都有一定高度，
                        kp =  bug[jp][2][0];//位置
                        amp = bug[jp][2][1];//波高
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);

                        kp =  bug[jp][1][0];//位置
                        amp = bug[jp][1][1];//波高
                        ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                        ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                        x0 = xpos-C_LEN_BUFF/2;
                        MDrawPixel( x0, preypos ,DP_SET);
                        MDrawPixel( x0, ypos , DP_SET);
                        flag = 1;
                    }

                }
                else if( bug[jp][1][1] > 0 )
                {
                    kp =  bug[jp][1][0];//位置
                    amp = bug[jp][1][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
                else
                {
                    kp =  bug[jp][0][0];//位置
                    amp = bug[jp][0][1];//波高
                    ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*80)/(C_COORWIDTH*100)+high;
                    ///MSetDisplayColor( c_crPara[(amp>>6)%8] );
                    x0 = xpos-C_LEN_BUFF/2;
                    MDrawPixel( x0, preypos ,DP_SET);
                    MDrawPixel( x0, ypos , DP_SET);
                    if( y0 != ypos )MDrawLine(x0,y0,x0,ypos,C_CR_WAVEBACK);
                    y0 = ypos;
                    flag = 2;
                }
            }

            /*
            			if( bug[jp][C_BASE][1] > 0)
            			{//有底波
            				if( bug[jp][C_BASE+1][1] == 0)//无其他波
            				{
            					kp =  bug[jp][C_BASE][0];//位置
            					amp = bug[jp][C_BASE][1];//波高
            					ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            					MSetDisplayColor( c_crPara[(amp>>6)%8] );
            					x0 = xpos-C_LEN_BUFF/2;
            					MDrawPixel( x0, ypos , DP_SET);
            					if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 1;
            				}
            				else //有其他波
            				{
            					for( kp = jp+1; kp < C_LEN_BUFF ; kp++)
            					{//向后看有波
            						if( bug[kp][C_BASE][1] > 0 && bug[kp][C_BASE+1][1] > 0)continue;
            						break;
            					}
            					for( lp = jp-1; lp >= 0; lp--)
            					{//向前看
            						if( bug[lp][C_BASE][1] > 0 && bug[lp][C_BASE+1][1] > 0)continue;
            						break;
            					}
            					if( jp - lp > kp - jp )
            					{//前面同时存在的波比后面多
            						if( lp < 0 && kp < C_LEN_BUFF)
            						{//前面一直都有回波
            							if( bug[jp][C_BASE+1][1] > bug[jp][C_BASE][1]*2 || ( bug[kp][C_BASE][1] <= 0 && kp - jp > C_LEN_BUFF/4) )
            							{//回波高于底波双倍，或者后面底波先于回波消失且回波出现时间较长
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}

            						}
            						else //if( kp < C_LEN_BUFF )//前面不都有回波
            						{//两边都有回波，但都不到边
            							if(  ( bug[kp][C_BASE+1][1] <= 0 && kp - jp > C_LEN_BUFF/8) &&
            								( bug[lp][C_BASE+1][1] <= 0 && jp - lp > C_LEN_BUFF/8) &&
            								(bug[jp][C_BASE][1] < bug[jp][C_BASE+1][1]*4 ) )
            							{//后面回波先于底波消失,且出现时间不长,或者前面回波先于底波消失且时间不长
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            							}
            							else if( bug[kp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4 )
            							{//底波先消失且回波已出现一段时间
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            					}
            					else
            					{//后面同时存在的波比前面多
            						if( lp >= 0 && kp >= C_LEN_BUFF)
            						{//后面一直都有回波
            							if( bug[jp][C_BASE+1][1] > bug[jp][C_BASE][1]*2 ||
            								( bug[lp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4) )
            							{//回波高于底波双倍，或者前面底波先于回波消失且回波出现时间较长
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            						else //if( kp < C_LEN_BUFF )//前面不都有回波
            						{//两边都有回波，但都不到边
            							if(  ( bug[kp][C_BASE+1][1] <= 0 && kp - jp > C_LEN_BUFF/8) &&
            								( bug[lp][C_BASE+1][1] <= 0 && jp - lp > C_LEN_BUFF/8) &&
            								( bug[jp][C_BASE][1] < bug[jp][C_BASE+1][1]*4 ) )
            							{//后面回波先于底波消失,且出现时间不长,或者前面回波先于底波消失且时间不长
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								MDrawPixel( xpos - C_LEN_BUFF/2, ypos , DP_SET);
            							}
            							else if( bug[kp][C_BASE][1] <= 0 && jp - lp > C_LEN_BUFF/4 )
            							{//底波先消失且回波已出现一段时间
            								kp =  bug[jp][C_BASE+1][0];//位置
            								amp = bug[jp][C_BASE+1][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 2;
            							}
            							else
            							{
            								kp =  bug[jp][C_BASE][0];//位置
            								amp = bug[jp][C_BASE][1];//波高
            								ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            								MSetDisplayColor( c_crPara[(amp>>6)%8] );
            								x0 = xpos-C_LEN_BUFF/2;
            								MDrawPixel( x0, ypos , DP_SET);
            								if( flag == 2 )MDrawLine(x0,y0,x0,ypos);
            								y0 = ypos;
            								flag = 1;
            							}
            						}
            					}
            				}
            			}
            			else
            			{//无底波
            				if( bug[jp][C_BASE+1][1] <= 2)//无其他波
            				{
            					x0 = xpos-C_LEN_BUFF/2;
            					ypos = C_COORVPOSI;
            			//		MDrawPixel( x0, ypos , DP_SET);
            			//		if( flag != 2 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 2;
            				}
            				else //有其他波
            				{
            					kp =  bug[jp][C_BASE+1][0];//位置
            					amp = bug[jp][C_BASE+1][1];//波高
            					ypos = C_COORVPOSI + ( (kp-0)*C_COORHEIGHT*100)/(C_COORWIDTH*80);
            					MSetDisplayColor( c_crPara[(amp>>6)%8] );
            					x0 = xpos-C_LEN_BUFF/2;
            					MDrawPixel( x0, ypos , DP_SET);
            					if( flag == 1 )MDrawLine(x0,y0,x0,ypos);
            					y0 = ypos;
            					flag = 2;
            				}
            			}
            //*/
            for( jp = 1 ; jp < C_LEN_BUFF; jp++)
            {
                for( kp = 0; kp < 4; kp++)
                {
                    bug[jp-1][kp][1] = bug[jp][kp][1];
                    bug[jp-1][kp][0] = bug[jp][kp][0];
                }
            }
        }
    }
    MKeyRlx();

    if( xpos >= C_COORWIDTH )
        MDrawLine(x0,preypos,x0,ypos,C_CR_WAVEBACK);
    DisplayPrompt(15);
    //	CEMenuOut(0,0,_Bscan,10,1,24);
    TextOut(0,0,1,32,16,(char *)_Bscan[MGetLanguage()][0],4);
    MAnyKeyReturn();

    AllParaRestore();
    MSetSystem();
//	ScreenRenovate();
//	DrawDac(0);

}

