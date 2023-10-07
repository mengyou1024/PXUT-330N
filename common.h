//
#include "globe.h"

//#include "user.h"
#include "sysdef.h"


//���ú���
//���ߺ���
void InitPara(void);		//��ʼ��

//void MSetAmplifierGain(u_short gain);

void InvertBytes(u_int XPos, u_int YPos, u_int bytes) ;/*������ʾ�����ֽڵ�����*/

void InvertWords(u_int XPos, u_int YPos, u_int words) ;/*������ʾ�����ֵ�����*/

int CharStr2Number( char str[],int number);//number������
int ShortStr2Number( short str[],int number);

u_int Bcd2Hex(u_int val);
u_int Hex2Bcd(u_int val);

u_int GetNumberLen(u_int number);	/*�õ�����λ��*/
void Swap(void *x,void *y);		/*��������*/
void ExpendTime(int time);	/*����ʱ�䣬��10msΪ��λ*/
void BuzzerOn(u_int time); 		/*����0.1 * time ��*/
void AlarmOn(void);
void MSetAlarmStatus(int on);
int MGetAlarmSatus();

u_int TestSameKeyTime(u_int Prekeycode,u_int PreElapsedTime,u_int MaxTime);
/*���԰���Prekeycode�Ѱ�ʱ����PreElapsedTimeΪ����ʱ��,MaxTimeΪ�ʱ�䣬
	��ʱ�䵽���в�ͬ�����˳�,����ֵΪͬһ��������ʱ��,��Prekeycode = C_KEYCODEMAX ��õ�
	ǰһ�ε��ô˺����ķ���ֵ*/
u_int GetSameKeyTime(void);
/*�õ�������������*/
u_int GetNoKeyTime(void);	//�õ�����ǰ�ΰ�����Ŀǰֹ��ʱ������λs,��������

u_int CheckDate(u_int year,u_int month,u_int date);
//����ꡢ�¡����Ƿ���Ϲ��򣬷���ֵ0���ϣ���0�����ϣ�1��year,2:month,4:date
//1+2:year+moth,1+4:year+date,2+4:month+date,1+2+4:year+month+date

void MPush(int mint);		//ѹջ
int  MPop(int mode);			//��ջ,mode=0ջָ��Ϊ0

int test_fft(void);
void FFT(short dataI[],short w[],int mode);		//ͨ��ת����dataI�е���������ת��dataR��
u_int Random();
float Mexpf(float ex, u_int len);//ָ���������õ�e^ex,λ��������len��8
int Mabsi( int x);
float Mabsf( float x);
double Mabsd( double x);
short Mabss( short x);
char Mabsc( char x);

//�Ӱ����˴������ݵı����з�����ָ��λ�õı���
u_int BitLoad(u_int des/*�������ݵı���*/, u_int bits/*�ֽ���*/, u_int cx/*��ʼ��λ��*/);
//������ָ��λ�ð����˴������ݵı���
u_int BitSave(u_int des/*��Ҫ�����������ݵı���*/, u_int bits/*�ֽ���*/, u_int cx/*��ʼ��λ��*/,u_int var/*����ı���*/);

void UGetSystemTime(TIME_CLOCK* tm);
void MSettimetoi2c(TIME_CLOCK TimeClock);
u_int MSetSystemTime(void);
//������йصĺ���
int MGetRotary(int rotary/*0��һ����ť1�ڶ�������1ȫ������2�ж��Ƿ�����ť*/);

//�õ���ֵ��time��ʾ�õ���ֵ����֮ǰ����Сʱ��
int MGetKey();
int MGetKey2();
int MGetKeyCode(u_int time);
void MKeyRlx(void);		/*ֻ�а������ͷŲ��˳�*/
int MAnyKeyReturn(void);		/*ֱ���ͷŵ�ǰ���������µİ������˳�*/
int MAnyTimeReturn(u_int time);		/*ֱ���а�����ʱ�䵽���˳� */
int MGetAnyKey(u_int time);			/*�õ�������������Ǳ��ΰ���Ҳ������ǰ�ΰ���δ�ͷ�*/
int MenuKeyReturn(int keymax,int mode);	/*���ֲ˵�ʱ����������keymax�ļ����أ�
	mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч,keymaxΪ����ɷ���0*/

void MFclearScreen(void);
int MCopyProtectedMemory(void* memdes, void* memsrc, u_int bytesize, u_int copymode) ;
int CopyMemoryAllSystem(u_int copymode);
int CopyMemoryToSystem(void* memsrc, u_int bytesize,u_int copymode);
void MCTextOut(u_int x, u_int y, u_short* pzText, u_int len) ;
void METextOut(u_int x, u_int y, char* pzText, u_int len) ;
void MDrawLine(u_int x1, u_int y1, u_int x2, u_int y2,u_int BGColor) ;
void MDrawPixel(int x, int y, int mode) ;
u_int MGetPixels(u_int x, u_int y, u_int len) ;
int MPushWindow(u_int left, u_int top, u_int right, u_int bottom) ;
int MPopWindow(void) ;
void MEraseWindow(u_int left, u_int top, u_int right, u_int bottom) ;
void MInvertWindow(u_int left, u_int top, u_int right, u_int bottom) ;

/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	MActivateDac(int bOn) ;
/* peak amplitude dac adjusting */
void    MDacGatePeak() ;

void	MSetEchoColor(u_int pos_s, u_int pos_e, u_int rgb) ;
void 	MSetDisplayColor(u_int rgb) ;

u_int 	MSetEchoRange(u_int range) ;
void MGetGatePeakBuffer() ;


int DrawCurver( short * buff,int startposi,int dotnumber);			//������
int DrawEnvelope( u_char * buff,int startposi,int dotnumber);			//������
int DrawGateCurver( short * buff,int startposi,int dotnumber,int mode);
int DrawWave( u_char* buff,int start,int dots);		//������

u_int CheckDrawLine(int x1,int y1,int x2,int y2);
void DrawCircle(u_int HCenter, u_int VCenter, u_int CircleRadius);
void DrawArc1(int x1,int y1,int x2,int y2,int r);//���ӵ�(x1,y1)����(x2,y2)�뾶Ϊr�Ļ���

u_int DrawMultiEdge(int xpos,int ypos,u_int edge,float angle,int len);
/*��ָ��λ��Ϊ���ģ�������=edge����б��=angle�����㵽���ľ���Ϊlen�Ķ����*/
void DrawCursor(u_int xpos,u_int ypos,u_int mode);	/*����꣬mode=0Ӣ��1����2бʮ��*/
void ClearCursor(u_int mode);	/*���꣬mode=0Ӣ��1����*/

/* ������ */
void DrawRectangle(u_int left, u_int top, u_int right, u_int bottom) ;
/* ����������� */
void EraseDrawRectangle(u_int left, u_int top, u_int right, u_int bottom) ;


void CMenuOut(u_int xpos,u_int ypos,const u_short pzText[],u_int char_len,u_int row_number,u_int row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void EMenuOut(u_int xpos,u_int ypos,const u_char* pzText,u_int char_len,u_int row_number,u_int row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void CEMenuOut(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void CEMenuOut2(u_int xpos,u_int ypos,const u_short* pzText,u_int char_len,u_int row_number,u_int row_height);

void Write_Space(u_int xpos ,u_int ypos,u_int len,u_int mode);
/*д�ո�mode=0Ӣ����8x8,=1������16x16*/
int CETextOut(u_int xpos,u_int ypos,u_int lpos,u_int rpos,const u_short* pzText,u_int char_max,u_int row_height,u_int type);
/*����lpos����rpos����ʼ(xpos,ypos)����д�ַ���char_max,ÿ�е������ı�����type=0Ӣ1��*/
int CETextOut2(int xpos,int ypos,const u_short* pzText,int col_max,int row_number,int row_height,int serial,int align);
/*��ʼ(xpos,ypos)��ÿ������ַ���col_max(Ӣ��),����row_number,�и�row_height,���Ǳ��serial=0��1�У����뷽ʽalign��0��1��2�м�3����*/
int Write_UNumber(u_int xpos,u_int ypos,u_int number,u_int total_len,u_int deci_len);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩д�޷�������total_lenΪ�ܳ��ȣ��������ŵ�����С������deci_lenΪС��λ����*/

int Write_Number(u_int xpos,u_int ypos,int number,u_int total_len,u_int deci_len,u_int sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩д�з�������total_lenΪ�ܳ��ȣ��������ŵ�����С������deci_lenΪС��λ����,sign=0�޷��ţ�=1�з���*/

void Write_Ascii(u_int xpos,u_int ypos,u_char AscChar);
/*��ָ��λ��дһ��ASCII�ַ�*/

void Write_Crystal(u_int xpos,u_int ypos,u_int crystal_l,u_int crystal_w);		/*д��Ƭ�ߴ�*/

int Input_Crystal(u_int xpos1,u_int ypos1,int *number_l,int *number_w);			/*���뾧Ƭ�ߴ�*/

int Input_Number(u_int xpos,u_int ypos,int *number,u_int int_len,int *deci_len,u_int sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩�����з�������number>0 && <= 9ʱ����Ϊ��ʼֵ����
int_lenΪ����λ��󳤶ȣ�deci_lenΪС��λ��󳤶�
sign=0�޷��ţ�=1�з���*/
int Input_Float(u_int xpos,u_int ypos,float *number,u_int int_len,u_int deci_len,u_int sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩���븡������int_lenΪ����λ��󳤶ȣ�deci_lenΪС��λ��󳤶�*/
int InputPwd(int x, int y,int total_len/*�ܳ���*/);//���ظ�ֵ��ʾ�Լ��˳�����ֵ������
void WritePwd(int x, int y, int number);

int CMessageBox( int x,int y, u_char * epText, int len, int mode );
int EMessageBox( int x,int y, u_char * epText, int len, int mode );
int EMessageBox2( int x,int y, u_char * epText,int len, int row,int mode );

/* ��ʾ��ʾ��Ϣ*/
void DisplayPrompt(u_int num);//��ʾ��ʾ
int DisplayQuery(int index);//��ʾѯ��
int DisplayQuery1(int index);//��ʾѯ��

void WriteFileDate(u_int xpos, u_int ypos, u_int date);//date = year * 65536 + month*256+day
void WriteFileDate1(u_int xpos, u_int ypos, u_int date);

void WriteDate(u_int xpos, u_int ypos, TIME_CLOCK *tm);
void WriteTime(u_int xpos, u_int ypos, TIME_CLOCK *tm);
void WriteTime1(u_int xpos, u_int ypos, u_int tm);

u_int InputDate(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int InputTime(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int InputWeek(u_int xpos, u_int ypos, TIME_CLOCK *tm);
u_int date2int(TIME_CLOCK *tm);//2004/05/06
u_int date2int2(TIME_CLOCK *tm);//��Ϊ��2000��1��1������
void int2date(TIME_CLOCK *tm,u_int date);
void int2date2(TIME_CLOCK *tm,u_int date);//�ӵ�2000��1��1����������tm

//

//�빦���йصĺ���
int SearchPeak(u_char * sampbuff,u_int curr_pos,u_int direction);		//������߲�����ˮƽ��������Ϊ������Ϊ����߲�
//Ѱ��sampbuffָ���δ洢������߲���curr_posΪ��ǰλ��,directionΪ����:C_KEYCOD_LEFT��C_KEYCOD_RIGHT��
u_int PeakMemory(void);		//��ֵ����
int EchoEnvelope(void);	//�ز�����
int DepthCompensate(void);		//��Ȳ���
void ClearEnvelope(void);		//���������ֵ

int GateWaveExpend( int mode);		//����չ��
//������ϵͳ̽ͷ�йصĺ���
//ontime ʱ�䣬//ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
u_int MLoadStartTime(void);//����
void MSaveStartTime(int time);

u_int MLoadCurrentTime(void);//����

u_int MLoadSystemElapsedTime( int mode);//ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
void MSaveSystemElapsedTime( u_int nElapsedTime ,int mode);

//mode = 0�꣬1�£�2���գ�3�ܼ���4Сʱ��5���ӣ�6�룬
//-1������,	-2��2000���ڼ��գ�-3�ڼ��ܣ�-4����ڼ��գ�-5�ڼ���,��6���쿪ʼ����ٷ���,��7���쿪ʼ�������
u_int MGetSystemTime(int mode);

u_int MGetCurrentYearTimeCount(int date, int time,int mode);//�õ���ǰʱ��൱��1��1��0��Ĳ�࣬mode = 2���գ�3�ܣ�4Сʱ��5���ӣ�6��
u_int MGetYearDateCount( int year1, int year2);//�õ���year1����year2֮�������������2003��2004��365��

u_int MGet2kDays(int mode);//mode=0�õ��洢������������ʵ������
void MSet2kDays(u_int days);//���õõ���2000���������



u_int MGetDenoise();
void MSetDenoise(u_int denoise,int mode/*mode = 0\1\2*/);
u_int MGetRepeatRate();
void MSetRepeatRate(u_int repeat,int mode/*mode = 0\1\2*/);
u_int MGetPulseWidth();
void MSetPulseWidth(u_int pulsewidth,int mode/*mode = 0\1\2*/);
//��ͨ��״̬�йصĺ���
u_int MLoadChannelStatus( int mode);//ȡͨ��״̬�е�ĳ������,mode=0-n,����ĸ˳����ϵ���
void MSaveChannelStatus( int mode, u_int para);


u_int MGetBandMode(void);	/*��������Ƶ����ȣ�2.5\5.0\10\wide	*/
void MSetBandMode(u_int iBand,int mode/*mode = 0\1\2*/);

u_int MGetEchoMode(void);	/*�������ü첨��ʽ��ȫ��\���벨\���벨\��Ƶ  	*/
void MSetEchoMode(u_int Wave,int mode/*mode = 0\1\2*/);

u_int MGetDampMode(void);	/*��������̽ͷ���᣺50Q\400Q 	*/
void MSetDampMode(u_int iDamp,int mode/*mode = 0\1\2*/);

u_int MGetPulseMode(void);	/*�������÷����ѹ��200V\400V  	*/
void MSetPulseMode(u_int Pulse,int mode/*mode = 0\1\2*/);

u_int MGetProbeMode(void);	/*����ֵΪ��ǰ̽ͷģʽ1\2\3\4=ֱ\б\˫��*/
void MSetProbeMode(u_int iProbe,int mode/*mode = 0\1\2*/);	/*����̽ͷģʽ*/

u_int MGetStdMode(void);	/*�������ñ�׼	*/
void MSetStdMode(u_int iStd,int mode/*mode = 0*/);

u_int MGetFunctionMode(u_int type);	//�õ�type��ָ���ܵĵ�ǰ״̬,���й���ֻ��������ر�����״̬
void MSetFunctionMode(u_int func,u_int type);	//����type��ָ���ܵ�״̬

int MSetCurveAmend(CURVE_AMEND *stCurveAmend);//������ȷ�ɼ��㷵��1�����򷵻�0��
int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend);//���ؾ��������ˮƽ�ʹ�ֱ

u_int MGetDacMode(void);	/*�������õ�����׼��ĸ��\�з�\����\����	*/
void MSetDacMode(u_int iDac,int mode/*mode = 0*/);

u_int MGetAvgMode(void);	/*�������õ�����ʾ��dBֵ\�׾�	*/
void MSetAvgMode(u_int iAvg,int mode/*mode = 0*/);
int MGetEquivalentMode();		//������ʾ��ʽ��<0���ߣ�1ĸ��2RL3SL4EL,5AVG-db,6avg-�׾�

int MGetScaleMode(void);	/*�������ñ�ȣ���ֱ\ˮƽ\����*/
void MSetScaleMode(int scale,int mode/*mode = 0*/);

int MGetParaLock(int mode,int type);	//���ز�������״��0δ��1����,mode=1����ʾ
void MSetParaLock(int status, int type);

int MGetBGateMode();
void MSetBGateMode(int bgate,int mode);	//����B�ţ�bgate=0ʧ��1����

int ParaStepChange(u_int paraindex);	//���ô����ڲ�������
int MGetParaStep(u_int paraindex);	//�����ڲ�������
int MGetParaSort(int index);		//�����ڲ�����(��A/��B),ϵͳ����/��������
int MSetParaSort(int status,int index);		//�����ڲ�����(��A/��B),ϵͳ����/��������

//cgain<1000�Ͳ��䣬����С��0�Ͳ��䣬���뱣֤bgain+sgain+cgain����0С���������������������Ҳ�����ڷ�Χ֮��
void MSetPara(int bgain/*��������*/,int cgain/*��������*/,int sgain/*���油��*/,int scale/*���*/,int range/*����*/,int offset/*���*/,int delay/*��ʱ*/,int angle/*�����*/,int speed/*����*/);

int MGetCurveNumber(void);	//�õ����ߵ���Ŀ
void MSetCurveNumber(int number);//�������ߵ���Ŀ

int MGetBlock(void);//�õ��Կ�����
void MSetBlock(int block);//�����Կ�����


int  MGetEquivalentDays(int mode);//��������,mode = 0�������ܣ�1���/K/dac/avg,2������
void MSetEquivalentDays(int mode,int days);//��������,mode = 0�������ܣ�1���/K/dac/avg,2������

int MGetTestStatus(u_int index);	//���ز���״̬0=δ��1=�Ѳ⣬
void MSetTestStatus(u_int index,int status);	//���ò���״̬

int MGetAmpStdMax(void);//0-200
void MSetAmpStdMax(int amp);//�����Զ�����ʱ��׼����,AMP=0-100


//ͨ����ʼ��
int ChannelParaInit(int mode);	/*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��,2�����û�*/

//�����йصĺ���
void MSetGatePara(u_short posi,u_short wide,u_short high,int gatetype,int mode);
/*�����Ų�����gatetype=0A��1B��*/
u_short MGetGatePara(int gatetype, int paratype);   /* ����ֵΪ�Ų��� */

short MGetGatedB(void);
void MSetGatedB(short gatedB, int mode/*mode = 0*/);

void DACGatedB(void);	//DAC��dB��
//void DACGate(void);
void DacGateInit(void);
int DacGate_dBHigh(int dB,int dac_dB);//����ĸ�ߵ�dB�DACdBֵ
int MGetDacDb(void);

//��ͨ�������йصĺ���

u_short MGetThick(void);//�õ��������
void MSetThick(u_short thick);//���ù������

u_short MGetDiameter(void);//�õ�����ֱ��
void MSetDiameter(u_short Diameter);//���ù���ֱ��

u_short MGetOffset(void);			/*����ֵΪ��ǰͨ��	*/
void MSetOffset(u_int Offset,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_short MGetSpeed(void);			/*����ֵΪ��ǰͨ���Լ����٣�	��С��	*/
void MSetSpeed(u_short Speed,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

int MGetDepth();
void MSetDepth(int depth,int mode);

u_int MGetAngle(u_int mode);			/*����ֵΪ��ǰͨ�������0�Ƕ�1����2����3����	�Ƕ�һλС��������4λ*/
void MSetAngle(u_short Angle,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/
u_short MGetKvalue(void);			/*����ֵΪ��ǰͨ��Kֵ ��λС��	*/
void MSetKvalue(u_short Kvalue,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_short MGetForward(void);			/*����ֵΪ��ǰͨ��̽ͷǰ�س��� һλС��	*/
void MSetForward(u_short Forward,int mode/*mode = 0*/);	/*���õ�ǰͨ��	*/

u_short MGetFrequence(void);			/*����ֵΪ��ǰͨ��̽ͷƵ�� һλС��	*/
void MSetFrequence(u_short Frequence,int mode/*mode = 0*/);	/*���õ�ǰͨ��	*/

u_short MGetCrystal_l(void);			/*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
u_short MGetCrystal_w(void);			/*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
void MSetCrystal_l(u_short Crystal,int mode/*mode = 0*/);	/*���õ�ǰͨ��	*/
void MSetCrystal_w(u_short Crystal,int mode/*mode = 0*/);	/*���õ�ǰͨ��	*/

int MGetNearField(double multi);		//�õ�����������
float MGetAttenuate(int depth);		//����˥��ϵ��

u_int MGetRange(int mode);			/*����ֵΪmode=0ֱ�Ӿ��룬��0Ϊ��ǰ����µľ���	*/
void MSetRange(int Range,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_int MGetDelay(int mode);			/*����ֵΪmode=0ʱ�䣬1��ǰ����µ�����,3��ʱ����	*/
void MSetDelay(int Delay,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/
void MSetScaleDelay(int Delay,int mode/*mode = 0\1\2*/);	/*���õ�ǰ����µ���ʱ����	*/

u_short MGetReject(void);			/*����ֵΪ��ǰͨ��	*/
void MSetReject(u_short Reject,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_short MGetSysGain(void);	//�õ���������
u_short MGetSurfGain(void);			/*����ֵΪ��ǰͨ�����油������ һλС��	*/
int MSetSurfGain(u_short SurfGain,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_short MGetBaseGain(void);			/*����ֵΪ��ǰͨ���������� һλС��	*/
int MSetBaseGain(short BaseGain,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

short MGetCompGain(void);			/*����ֵΪ��ǰͨ���������� һλС��	*/
int MSetCompGain(short CompGain,int mode/*mode = 0\1\2*/);	/*���õ�ǰͨ��	*/

u_short MGetDepthGain(void);
void MSetDepthGain(u_short gain);//������DAC/AVGʱ��Ȳ���ֵ

short MGetLineGain(int line);//�õ����ߵ�ƫ����
void MSetLineGain(int line,short gain);//�������ߵ�ƫ����

void MSetSystemGain(void);	/*���õ�ǰ���� */
void MSetSystemRange(void);	/*���õ�ǰ���� */
void MSetSystemDelay(void);	/*���õ�ǰ��ʱ����� */
void MSetSystemBright(void);/*���õ�ǰ��Ļ���� */
int MSetSystemProbe(void);	/*���õ�ǰ̽ͷģʽ*/
void MSetSystem(void);		/*���õ�ǰϵͳ��״̬*/


//���û������йصĺ���
int UserManager(void);
int UserStore(void);
int UserRestore(void);
int GetUserNumber();//�õ���ǰ���õ��û�����
void SetUserNumber( int number);//���õ�ǰ�û�����
int GetCurrentUser();//�õ���ǰ�û�����ţ�0��������1
void SetCurrentUser( int user);//���õ�ǰ�û�����ţ�0��������1
void UserMove(int user);

int GetUserName( u_short name[],int nUserNumber);//�õ�����û�������,����1����ȡ�ã�0�д�
int CheckPwd(int x, int y, int pwd);
int InputPwd(int x, int y,int total_len/*�ܳ���*/);//���ظ�ֵ��ʾ�Լ��˳�����ֵ������
void WritePwd(int x, int y, int number);
int GetPwd(int nUserNumber);//�õ�����û�������

void SysPwd(int mode);

//�����뷨�йصĺ���
int MInputChar(u_int xpos,u_int ypos,u_int type,u_short* InputChar,u_short len_char_max,u_short char_max);
/*�����ַ���type = 0ֻ��Ӣ�ģ�1��Ӣ����,len_char_maxÿ�п�д�����Ӣ���ַ�����char_max�ܹ���д����Ӣ���ַ���*/
int  MGetCChar(u_int xpos,u_int ypos,u_short association);		/*�õ��ַ�ֵ��Ӣ��Ϊascii�룬����Ϊ��λ��*/
int MGetQuwei(u_int xpos,u_int ypos,u_short association);
u_short MGetEChar(u_int xpos,u_int ypos,u_short* pzText,u_int mode);		/*�õ�ASCII�ַ�*/
char MGetSymbol(void);	/*�õ�8x8������*/

//��ز������йصĺ���
void MGetGatePeakBuffer() ;
int MGetGateParaMax(int gatetype);
//�õ�������߻ز��Ĵ���ʱ�䡢ˮƽ�㡢���롢����,gatetype = 0��A�ţ�=1��B��
//����C_TRUE��ʾ���µ���߲�,���в�����ʷ��߲���ͬһ�������0.1s
void MSetGateParaInit(void);	//���ֵ
int MGetAmpMax(int gatetype);	/*�õ�ָ���ţ�0=A or 1=B)�ڻز��߶ȣ�*/
int MGetAmpPos(int gatetype);	/*�õ�ָ���ţ�A or B)�ڻز�ˮƽ������*/
int MGetAmpPos1(int gatetype,int mode);	/*�õ�ָ�����ڻز�mode=0��ʼ/1���/2����3/ǰ�����ཻ/4������ཻ�ĵ�����*/

int MGetAmpTime(int gatetype);	/*�õ�ָ���ţ�A or B)�ڻز�����ʱ�䣬*/
int MGetAmpDist(int gatetype);	/*�õ�ָ���ţ�A or B)�ڻز��������̣�*/
int MGetAmpDist1(int gatetype);	/*�õ�ָ���ţ�A or B)�ڻز��������̣�*/
int Amp2db(int pos,int amp);		//���ݻز�λ�úͲ��ߵõ�dBֵ


//����ϵͳ״̬����
u_int MLoadSystemStatus( int mode);//ȡϵͳ״̬�е�ĳ������,mode=0-n,����ĸ˳����ϵ���
void MSaveSystemStatus( int mode, u_int para);

u_int MGetPrinterMode(void);	/*����ֵΪ��ǰ��ӡ������1\2\3\4=LQ\HP\EPSON C/P*/
void MSetPrinterMode(u_int iPrinter,int mode/*mode = 0*/);	/*���õ�ǰ��ӡ������*/

u_int MGetInputMode(void);	/*����ֵΪ��ǰ���뷨����1\2\3\4=����\��д\Сд*/
void MSetInputMode(u_int iInput,int mode/*mode = 0*/);	/*���õ�ǰ���뷨����*/

u_int MGetSystemMode(void);	/*����ֵΪ��ǰϵͳ״̬0\1=����\����*/
void MSetSystemMode(u_int iSystem ,int mode/*mode = 0*/);	/*���õ�ǰϵͳ״̬*/

u_int MGetAcquisitionEnable(void);	/*�Ƿ��������d5*/
void MSetAcquisitionEnable(u_int acquisition,int mode/*mode = 0\1\2*/);
void MSetAcquisition(u_int mode);	/*���ò������0������1����*/
u_int MGetFill(void);//��ʾӲ������ʵ�Ļ��ǿ���
void MSetFill(u_int fill,int mode);
u_int MGetHardEchoShow(void);//��ʾӲ������
void MSetHardEchoShow(u_int show,int mode);

u_int MGetSoftEchoShow(void);//��ʾ�������
void MSetSoftEchoShow(u_int show,int mode);

u_int MGetCursorEnable(void);//�Ƿ������й��
void MSetCursorEnable(u_int cursor,int mode);

u_int MGetCursorShow(void);//�Ƿ���ʾ���
void MSetCursorShow(u_int cursor,int mode);
u_int MGetColorMode(void);//
void MSetColorMode(u_int item,int mode);
u_int MGetChannel(void);	/*���ط�����ǰͨ����*/
int MSetChannel(int channel,int mode/*mode = 0*/);

u_int MGetSoundAlarm(int mode);//mode = 0������1����
void MSetSoundAlarm( int mode, int alarm );//mode = 0������1����

u_int MGetScreenBrightness(void);	/*����ֵΪ��ǰ��Ļ����*/
void MSetScreenBrightness(u_int iBrightness,int mode/*mode = 0\1\2*/);		/**/

int MGetInputEnable(int type);
void MSetInputEnable(int bEnable, int type);//����ĳ�����뷨�Ƿ��ѡ

int MGetSaveMode(void);		//�洢ģʽ0��1����
int MSetSaveMode(int style,int mode);
int MGetSaveStatus(void);	//�洢״̬0���ڴ棬1���ڴ�
int MSetSaveStatus(int status,int mode);
int MGetSaveTime(void);		//�õ���������ʱ��
int MSetSaveTime(int time,int mode);
int MGetSavePosition(void);		//�õ��洢λ��
int MSetSavePosition(int time,int mode);

int MGetSaveFrequency();
void MSetSaveFrequency(int bSave,int mode);//�Ƿ�洢Ƶ��

int MGetCraft();	//�õ����պ�
void MSetCraft(int craft,int modee);

int MGetDacShowMode(void);//����dac��ʾģʽ��0������1dB
void MSetDacShowMode(int show,int mode);

int MGetKeyLock();
void MSetKeyLock(int lock,int mode);//������,1��,0��

void MSetAmp2Gate( int gatetype,int gate);//���ڻز��Ƿ�����Ÿ�
int MGetAmp2Gate( int gatetype);

int MGetGateEchoColor( int gatetype);
void MSetGateEchoColor( int gatetype,int gate);//���ڻز���ɫ�Ƿ�Ҫ��

void MSetReadForeland( int read);//�Ƿ�ǰ�ض���
int MGetReadForeland( );

// �õ�����
int MGetLanguage(void);
//��������
void MSetLanguage(int language);
// �õ�������λ
void MSetUnitType(int unit);
// ���ü�����λ
int MGetUnitType(void);


/*
�����ƶ������ơ���ʼ��
*/

int ChannelParaStore(void);		/*ͨ�������浽������*/
int ChannelParaRestore(void);		/*ͨ�������ӱ�����ȡ*/

int SystemParaStore(void);		/*ϵͳ�����浽������*/
int SystemParaRestore(void);		/*ϵͳ�����ӱ�����ȡ*/

int AllParaStore(void);
int AllParaRestore(void);//ȫ���������ݺͻָ�
//���ӡ�йصĺ���
int mSendByteToPrinter(u_int pdata) ;

u_int TestPrinterReady();	//����ӡ���Ƿ�׼���ã���û�г�����ʾ����3�Σ�����1Ϊ׼����
void PrinterInitialize();		//��ӡ����ʼ��
void PrintEnd(void);	//��ӡ����
void PrinterFormFeed();		//��ӡ����ֽ
void SetPrnPosi_AbHori_EpC61(u_char nl,u_char nh);
//Set absolute horizontial print positon,Epson C61 Para:Ah=nH,Al=nl	ESC $ nL nH;
void SetPrnPosi_ReVert_EpC61(u_char ml,u_char mh);
//���ô�ֱ��Դ�ӡλ�ã�Para:Al=mL,Ah=mH  ESC ( v nl nh mL mH,
void SetPrn_Raster_EpC61(u_short PrintHoriBytes);	//Para:Print_HoriBytes,����ӡ�����ֽ���������40����80
//ESC . c v h m nl nh d1...dk(d1...dk���¶������
void LQ12To24(u_int xpos,u_int ypos,u_char* PrnBuff);	//���ֱ12���Ϊ24��
u_short Byte2Word(u_char Byte);		//һ���ֽڱ�Ϊһ����8-16
u_int Byte2DWord(u_char Byte);		//һ���ֽڱ�Ϊһ��˫��8-32
void ScrcpyInit(u_short PrintHoriBytes);
void ScrcpyInvert(u_int xpos,u_int ypos,u_short PrintHoriBytes);
u_int ScreenCopy(void);		//����
void ReportPrint(void);		//�����ӡ
int ReportText(int mode, int file_no);		//�����ı�


//���þ��Դ�ֱ��ӡλ�ã��Զ����հ�Ϊ��׼
void SetPrnPosi_AbVert(int dots/*�ƶ�������ֻ������*/);
//������Դ�ֱ��ӡλ�ã��Ե�ǰλ��Ϊ��׼
void SetPrnPosi_ReVert(int dots/*�ƶ������������ɸ�*/);
//���þ���ˮƽ��ӡλ�ã����󲿿հ�Ϊ��׼
void SetPrnPosi_AbHori(int dots/*�ƶ�������ֻ������*/);
//�������ˮƽ��ӡλ�ã��Ե�ǰλ��Ϊ��׼
void SetPrnPosi_ReHori(int dots/*�ƶ������������ɸ�*/);

//void ScreenStore();//������Ļ����
void DebugInfo(int num);//C_DEBUG == 1ʱ���ڣ�10��30����ʾ����


#if C_DEVLIB == 4
void	SwitchBuzzerOn(u_int bEnable) ;
u_char* GetDacBuffer() ;
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	ActivateDac(int bOn) ;
/* peak amplitude dac adjusting */
void    DacGatePeak() ;
void 	UpdateEchoColor() ;
void 	EnableEchoDisplay(u_int ena, u_int mode_fill);
void 	NotExtendEchoDisplay(u_int notext) ;
u_int 	SerialPortByteOut(u_int dout) ;
u_int 	SerialPortByteIn() ;
/* noise reduction , level: 0->no, 1->2 average, 2->4 average, 3->8 average */
//void 	DeNoise(u_int level) ;

#elif C_DEVLIB == 23 || C_DEVLIB == 24

u_int 	SerialPortByteOut(u_int dout) ;
u_int 	SerialPortByteIn() ;
u_char* GetDacBuffer();
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void	ActivateDac(int bOn);
/* peak amplitude dac adjusting */
void    DacGatePeak() ;

#endif


void m_DrawLine(  int x1,int y1,int x2,int y2);
void m_DrawDot(  int x,int y);//��pDC��ָ���豸�ϴӻ���(x,y)
void m_DrawCircle(int x,int y,int radius);
//��pDC��ָ���豸���Ե�(x,y)ΪԲ��,radiusΪ�뾶��Բ��,start_angleΪ��ʼ�Ƕ�,ˮƽx����Ϊ0��
//angleΪ����ʼֵ˳ʱ�뷽����ת�Ƕ�ֵ,
void m_DrawArc2(int x,int y,int radius,double start_angle,double angle);
//��pDC��ָ���豸�ϴӵ�(x1,y1)/(x2,y2)��Բ��,mode=0ֱ��1����͹��1���°�
//��������Բ���İ뾶
int m_DrawArc1( int x1,int y1,int x2,int y2,int mode);

//void WELD_DRAW( CDC *pDC);
void WELD_DRAW( WELD_PARA *stWeld,int wlpos, int wrpos,int wypos,int maxsize,int ProbeBeta, int dist_dire);
void SINGALWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void DOUBLEWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void TWELD_DRAW( WELD_PARA *stWeld,WELD_POSI *stWPosi);
void WELD_BUGLINE(WELD_PARA *stWeld,WELD_POSI *stWPosi,int ProbeBeta, int dist_dire);

