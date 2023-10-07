#pragma once

#include "globe.h"

#ifdef __cplusplus
extern "C" {
#endif

// ���ú���
// ���ߺ���
void InitPara(void); // ��ʼ��

// void MSetAmplifierGain(uint16_t gain);

void InvertBytes(uint32_t XPos, uint32_t YPos, uint32_t bytes); /*������ʾ�����ֽڵ�����*/

void InvertWords(uint32_t XPos, uint32_t YPos, uint32_t words); /*������ʾ�����ֵ�����*/

int CharStr2Number(char str[], int number); // number������
int ShortStr2Number(short str[], int number);

uint32_t Bcd2Hex(uint32_t val);
uint32_t Hex2Bcd(uint32_t val);

uint32_t GetNumberLen(uint32_t number); /*�õ�����λ��*/
void     Swap(void *x, void *y);        /*��������*/
void     ExpendTime(int time);          /*����ʱ�䣬��10msΪ��λ*/
void     BuzzerOn(uint32_t time);       /*����0.1 * time ��*/
void     AlarmOn(void);
void     MSetAlarmStatus(int on);
int      MGetAlarmSatus();

uint32_t TestSameKeyTime(uint32_t Prekeycode, uint32_t PreElapsedTime, uint32_t MaxTime);
/*���԰���Prekeycode�Ѱ�ʱ����PreElapsedTimeΪ����ʱ��,MaxTimeΪ�ʱ�䣬
    ��ʱ�䵽���в�ͬ�����˳�,����ֵΪͬһ��������ʱ��,��Prekeycode = C_KEYCODEMAX ��õ�
    ǰһ�ε��ô˺����ķ���ֵ*/
uint32_t GetSameKeyTime(void);
/*�õ�������������*/
uint32_t GetNoKeyTime(void); // �õ�����ǰ�ΰ�����Ŀǰֹ��ʱ������λs,��������

uint32_t CheckDate(uint32_t year, uint32_t month, uint32_t date);
// ����ꡢ�¡����Ƿ���Ϲ��򣬷���ֵ0���ϣ���0�����ϣ�1��year,2:month,4:date
// 1+2:year+moth,1+4:year+date,2+4:month+date,1+2+4:year+month+date

void MPush(int mint); // ѹջ
int  MPop(int mode);  // ��ջ,mode=0ջָ��Ϊ0

int      test_fft(void);
void     FFT(short dataI[], short w[], int mode); // ͨ��ת����dataI�е���������ת��dataR��
uint32_t Random();
float    Mexpf(float ex, uint32_t len); // ָ���������õ�e^ex,λ��������len��8
int      Mabsi(int x);
float    Mabsf(float x);
double   Mabsd(double x);
short    Mabss(short x);
char     Mabsc(char x);

// �Ӱ����˴������ݵı����з�����ָ��λ�õı���
uint32_t BitLoad(uint32_t des /*�������ݵı���*/, uint32_t bits /*�ֽ���*/, uint32_t cx /*��ʼ��λ��*/);
// ������ָ��λ�ð����˴������ݵı���
uint32_t BitSave(uint32_t des /*��Ҫ�����������ݵı���*/, uint32_t bits /*�ֽ���*/, uint32_t cx /*��ʼ��λ��*/, uint32_t var /*����ı���*/);

void     UGetSystemTime(TIME_CLOCK *tm);
void     MSettimetoi2c(TIME_CLOCK TimeClock);
uint32_t MSetSystemTime(void);
// ������йصĺ���
int MGetRotary(int rotary /*0��һ����ť1�ڶ�������1ȫ������2�ж��Ƿ�����ť*/);

// �õ���ֵ��time��ʾ�õ���ֵ����֮ǰ����Сʱ��
int  MGetKey();
int  MGetKey2();
int  MGetKeyCode(uint32_t time);
void MKeyRlx(void);                       /*ֻ�а������ͷŲ��˳�*/
int  MAnyKeyReturn(void);                 /*ֱ���ͷŵ�ǰ���������µİ������˳�*/
int  MAnyTimeReturn(uint32_t time);       /*ֱ���а�����ʱ�䵽���˳� */
int  MGetAnyKey(uint32_t time);           /*�õ�������������Ǳ��ΰ���Ҳ������ǰ�ΰ���δ�ͷ�*/
int  MenuKeyReturn(int keymax, int mode); /*���ֲ˵�ʱ����������keymax�ļ����أ�
      mode=0��������Ч��=1ȷ�ϼ���Ч��=2���ؼ���Ч��=3ȷ�Ϸ��ؼ���Ч,keymaxΪ����ɷ���0*/

void     MFclearScreen(void);
int      MCopyProtectedMemory(void *memdes, void *memsrc, uint32_t bytesize, uint32_t copymode);
int      CopyMemoryAllSystem(uint32_t copymode);
int      CopyMemoryToSystem(void *memsrc, uint32_t bytesize, uint32_t copymode);
void     MCTextOut(uint32_t x, uint32_t y, uint16_t *pzText, uint32_t len);
void     METextOut(uint32_t x, uint32_t y, char *pzText, uint32_t len);
void     MDrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t BGColor);
void     MDrawPixel(int x, int y, int mode);
uint32_t MGetPixels(uint32_t x, uint32_t y, uint32_t len);
int      MPushWindow(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);
int      MPopWindow(void);
void     MEraseWindow(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);
void     MInvertWindow(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);

/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void MActivateDac(int bOn);
/* peak amplitude dac adjusting */
void MDacGatePeak();

void MSetEchoColor(uint32_t pos_s, uint32_t pos_e, uint32_t rgb);
void MSetDisplayColor(uint32_t rgb);

uint32_t MSetEchoRange(uint32_t range);
void     MGetGatePeakBuffer();

int DrawCurver(short *buff, int startposi, int dotnumber);     // ������
int DrawEnvelope(uint8_t *buff, int startposi, int dotnumber); // ������
int DrawGateCurver(short *buff, int startposi, int dotnumber, int mode);
int DrawWave(uint8_t *buff, int start, int dots); // ������

uint32_t CheckDrawLine(int x1, int y1, int x2, int y2);
void     DrawCircle(uint32_t HCenter, uint32_t VCenter, uint32_t CircleRadius);
void     DrawArc1(int x1, int y1, int x2, int y2, int r); // ���ӵ�(x1,y1)����(x2,y2)�뾶Ϊr�Ļ���

uint32_t DrawMultiEdge(int xpos, int ypos, uint32_t edge, float angle, int len);
/*��ָ��λ��Ϊ���ģ�������=edge����б��=angle�����㵽���ľ���Ϊlen�Ķ����*/
void DrawCursor(uint32_t xpos, uint32_t ypos, uint32_t mode); /*����꣬mode=0Ӣ��1����2бʮ��*/
void ClearCursor(uint32_t mode);                              /*���꣬mode=0Ӣ��1����*/

/* ������ */
void DrawRectangle(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);
/* ����������� */
void EraseDrawRectangle(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);

void CMenuOut(uint32_t xpos, uint32_t ypos, const uint16_t pzText[], uint32_t char_len, uint32_t row_number, uint32_t row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void EMenuOut(uint32_t xpos, uint32_t ypos, const uint8_t *pzText, uint32_t char_len, uint32_t row_number, uint32_t row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void CEMenuOut(uint32_t xpos, uint32_t ypos, const uint16_t *pzText, uint32_t char_len, uint32_t row_number, uint32_t row_height);
/*��ָ��λ�ø���ÿ���ַ������������и�д�˵�*/
void CEMenuOut2(uint32_t xpos, uint32_t ypos, const uint16_t *pzText, uint32_t char_len, uint32_t row_number, uint32_t row_height);

void Write_Space(uint32_t xpos, uint32_t ypos, uint32_t len, uint32_t mode);
/*д�ո�mode=0Ӣ����8x8,=1������16x16*/
int CETextOut(uint32_t xpos, uint32_t ypos, uint32_t lpos, uint32_t rpos, const uint16_t *pzText, uint32_t char_max, uint32_t row_height, uint32_t type);
/*����lpos����rpos����ʼ(xpos,ypos)����д�ַ���char_max,ÿ�е������ı�����type=0Ӣ1��*/
int CETextOut2(int xpos, int ypos, const uint16_t *pzText, int col_max, int row_number, int row_height, int serial, int align);
/*��ʼ(xpos,ypos)��ÿ������ַ���col_max(Ӣ��),����row_number,�и�row_height,���Ǳ��serial=0��1�У����뷽ʽalign��0��1��2�м�3����*/
int Write_UNumber(uint32_t xpos, uint32_t ypos, uint32_t number, uint32_t total_len, uint32_t deci_len);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩д�޷�������total_lenΪ�ܳ��ȣ��������ŵ�����С������deci_lenΪС��λ����*/

int Write_Number(uint32_t xpos, uint32_t ypos, int number, uint32_t total_len, uint32_t deci_len, uint32_t sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩д�з�������total_lenΪ�ܳ��ȣ��������ŵ�����С������deci_lenΪС��λ����,sign=0�޷��ţ�=1�з���*/

void Write_Ascii(uint32_t xpos, uint32_t ypos, uint8_t AscChar);
/*��ָ��λ��дһ��ASCII�ַ�*/

void Write_Crystal(uint32_t xpos, uint32_t ypos, uint32_t crystal_l, uint32_t crystal_w); /*д��Ƭ�ߴ�*/

int Input_Crystal(uint32_t xpos1, uint32_t ypos1, int *number_l, int *number_w); /*���뾧Ƭ�ߴ�*/

int Input_Number(uint32_t xpos, uint32_t ypos, int *number, uint32_t int_len, int *deci_len, uint32_t sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩�����з�������number>0 && <= 9ʱ����Ϊ��ʼֵ����
int_lenΪ����λ��󳤶ȣ�deci_lenΪС��λ��󳤶�
sign=0�޷��ţ�=1�з���*/
int Input_Float(uint32_t xpos, uint32_t ypos, float *number, uint32_t int_len, uint32_t deci_len, uint32_t sign);
/*��ָ��λ��(xpos,yposΪ��Ļ�ϵĵ㣩���븡������int_lenΪ����λ��󳤶ȣ�deci_lenΪС��λ��󳤶�*/
int  InputPwd(int x, int y, int total_len /*�ܳ���*/); // ���ظ�ֵ��ʾ�Լ��˳�����ֵ������
void WritePwd(int x, int y, int number);

int CMessageBox(int x, int y, uint8_t *epText, int len, int mode);
int EMessageBox(int x, int y, uint8_t *epText, int len, int mode);
int EMessageBox2(int x, int y, uint8_t *epText, int len, int row, int mode);

/* ��ʾ��ʾ��Ϣ*/
void DisplayPrompt(uint32_t num); // ��ʾ��ʾ
int  DisplayQuery(int index);     // ��ʾѯ��
int  DisplayQuery1(int index);    // ��ʾѯ��

void WriteFileDate(uint32_t xpos, uint32_t ypos, uint32_t date); // date = year * 65536 + month*256+day
void WriteFileDate1(uint32_t xpos, uint32_t ypos, uint32_t date);

void WriteDate(uint32_t xpos, uint32_t ypos, TIME_CLOCK *tm);
void WriteTime(uint32_t xpos, uint32_t ypos, TIME_CLOCK *tm);
void WriteTime1(uint32_t xpos, uint32_t ypos, uint32_t tm);

uint32_t InputDate(uint32_t xpos, uint32_t ypos, TIME_CLOCK *tm);
uint32_t InputTime(uint32_t xpos, uint32_t ypos, TIME_CLOCK *tm);
uint32_t InputWeek(uint32_t xpos, uint32_t ypos, TIME_CLOCK *tm);
uint32_t date2int(TIME_CLOCK *tm);  // 2004/05/06
uint32_t date2int2(TIME_CLOCK *tm); // ��Ϊ��2000��1��1������
void     int2date(TIME_CLOCK *tm, uint32_t date);
void     int2date2(TIME_CLOCK *tm, uint32_t date); // �ӵ�2000��1��1����������tm

//

// �빦���йصĺ���
int SearchPeak(uint8_t *sampbuff, uint32_t curr_pos, uint32_t direction); // ������߲�����ˮƽ��������Ϊ������Ϊ����߲�
// Ѱ��sampbuffָ���δ洢������߲���curr_posΪ��ǰλ��,directionΪ����:C_KEYCOD_LEFT��C_KEYCOD_RIGHT��
uint32_t PeakMemory(void);      // ��ֵ����
int      EchoEnvelope(void);    // �ز�����
int      DepthCompensate(void); // ��Ȳ���
void     ClearEnvelope(void);   // ���������ֵ

int GateWaveExpend(int mode); // ����չ��
// ������ϵͳ̽ͷ�йصĺ���
// ontime ʱ�䣬//ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
uint32_t MLoadStartTime(void); // ����
void     MSaveStartTime(int time);

uint32_t MLoadCurrentTime(void); // ����

uint32_t MLoadSystemElapsedTime(int mode); // ʱ�䵥λ��MILLION�����룬SECOND:�룬minute:����
void     MSaveSystemElapsedTime(uint32_t nElapsedTime, int mode);

// mode = 0�꣬1�£�2���գ�3�ܼ���4Сʱ��5���ӣ�6�룬
//-1������,	-2��2000���ڼ��գ�-3�ڼ��ܣ�-4����ڼ��գ�-5�ڼ���,��6���쿪ʼ����ٷ���,��7���쿪ʼ�������
uint32_t MGetSystemTime(int mode);

uint32_t MGetCurrentYearTimeCount(int date, int time, int mode); // �õ���ǰʱ��൱��1��1��0��Ĳ�࣬mode = 2���գ�3�ܣ�4Сʱ��5���ӣ�6��
uint32_t MGetYearDateCount(int year1, int year2);                // �õ���year1����year2֮�������������2003��2004��365��

uint32_t MGet2kDays(int mode);      // mode=0�õ��洢������������ʵ������
void     MSet2kDays(uint32_t days); // ���õõ���2000���������

uint32_t MGetDenoise();
void     MSetDenoise(uint32_t denoise, int mode /*mode = 0\1\2*/);
uint32_t MGetRepeatRate();
void     MSetRepeatRate(uint32_t repeat, int mode /*mode = 0\1\2*/);
uint32_t MGetPulseWidth();
void     MSetPulseWidth(uint32_t pulsewidth, int mode /*mode = 0\1\2*/);
// ��ͨ��״̬�йصĺ���
uint32_t MLoadChannelStatus(int mode); // ȡͨ��״̬�е�ĳ������,mode=0-n,����ĸ˳����ϵ���
void     MSaveChannelStatus(int mode, uint32_t para);

uint32_t MGetBandMode(void); /*��������Ƶ����ȣ�2.5\5.0\10\wide	*/
void     MSetBandMode(uint32_t iBand, int mode /*mode = 0\1\2*/);

uint32_t MGetEchoMode(void); /*�������ü첨��ʽ��ȫ��\���벨\���벨\��Ƶ  	*/
void     MSetEchoMode(uint32_t Wave, int mode /*mode = 0\1\2*/);

uint32_t MGetDampMode(void); /*��������̽ͷ���᣺50Q\400Q 	*/
void     MSetDampMode(uint32_t iDamp, int mode /*mode = 0\1\2*/);

uint32_t MGetPulseMode(void); /*�������÷����ѹ��200V\400V  	*/
void     MSetPulseMode(uint32_t Pulse, int mode /*mode = 0\1\2*/);

uint32_t MGetProbeMode(void);                                       /*����ֵΪ��ǰ̽ͷģʽ1\2\3\4=ֱ\б\˫��*/
void     MSetProbeMode(uint32_t iProbe, int mode /*mode = 0\1\2*/); /*����̽ͷģʽ*/

uint32_t MGetStdMode(void); /*�������ñ�׼	*/
void     MSetStdMode(uint32_t iStd, int mode /*mode = 0*/);

uint32_t MGetFunctionMode(uint32_t type);                // �õ�type��ָ���ܵĵ�ǰ״̬,���й���ֻ��������ر�����״̬
void     MSetFunctionMode(uint32_t func, uint32_t type); // ����type��ָ���ܵ�״̬

int MSetCurveAmend(CURVE_AMEND *stCurveAmend);                                  // ������ȷ�ɼ��㷵��1�����򷵻�0��
int MGetCurveAmend(int *dist, int *hori, int *vert, CURVE_AMEND *stCurveAmend); // ���ؾ��������ˮƽ�ʹ�ֱ

uint32_t MGetDacMode(void); /*�������õ�����׼��ĸ��\�з�\����\����	*/
void     MSetDacMode(uint32_t iDac, int mode /*mode = 0*/);

uint32_t MGetAvgMode(void); /*�������õ�����ʾ��dBֵ\�׾�	*/
void     MSetAvgMode(uint32_t iAvg, int mode /*mode = 0*/);
int      MGetEquivalentMode(); // ������ʾ��ʽ��<0���ߣ�1ĸ��2RL3SL4EL,5AVG-db,6avg-�׾�

int  MGetScaleMode(void); /*�������ñ�ȣ���ֱ\ˮƽ\����*/
void MSetScaleMode(int scale, int mode /*mode = 0*/);

int  MGetParaLock(int mode, int type); // ���ز�������״��0δ��1����,mode=1����ʾ
void MSetParaLock(int status, int type);

int  MGetBGateMode();
void MSetBGateMode(int bgate, int mode); // ����B�ţ�bgate=0ʧ��1����

int ParaStepChange(uint32_t paraindex);  // ���ô����ڲ�������
int MGetParaStep(uint32_t paraindex);    // �����ڲ�������
int MGetParaSort(int index);             // �����ڲ�����(��A/��B),ϵͳ����/��������
int MSetParaSort(int status, int index); // �����ڲ�����(��A/��B),ϵͳ����/��������

// cgain<1000�Ͳ��䣬����С��0�Ͳ��䣬���뱣֤bgain+sgain+cgain����0С���������������������Ҳ�����ڷ�Χ֮��
void MSetPara(int bgain /*��������*/, int cgain /*��������*/, int sgain /*���油��*/, int scale /*���*/, int range /*����*/, int offset /*���*/, int delay /*��ʱ*/, int angle /*�����*/, int speed /*����*/);

int  MGetCurveNumber(void);       // �õ����ߵ���Ŀ
void MSetCurveNumber(int number); // �������ߵ���Ŀ

int  MGetBlock(void);      // �õ��Կ�����
void MSetBlock(int block); // �����Կ�����

int  MGetEquivalentDays(int mode);           // ��������,mode = 0�������ܣ�1���/K/dac/avg,2������
void MSetEquivalentDays(int mode, int days); // ��������,mode = 0�������ܣ�1���/K/dac/avg,2������

int  MGetTestStatus(uint32_t index);             // ���ز���״̬0=δ��1=�Ѳ⣬
void MSetTestStatus(uint32_t index, int status); // ���ò���״̬

int  MGetAmpStdMax(void);    // 0-200
void MSetAmpStdMax(int amp); // �����Զ�����ʱ��׼����,AMP=0-100

// ͨ����ʼ��
int ChannelParaInit(int mode); /*������ʼ����mode=0��ǰһ��ͨ����mode=1����ͨ��,2�����û�*/

// �����йصĺ���
void MSetGatePara(uint16_t posi, uint16_t wide, uint16_t high, int gatetype, int mode);
/*�����Ų�����gatetype=0A��1B��*/
uint16_t MGetGatePara(int gatetype, int paratype); /* ����ֵΪ�Ų��� */

short MGetGatedB(void);
void  MSetGatedB(short gatedB, int mode /*mode = 0*/);

void DACGatedB(void); // DAC��dB��
// void DACGate(void);
void DacGateInit(void);
int  DacGate_dBHigh(int dB, int dac_dB); // ����ĸ�ߵ�dB�DACdBֵ
int  MGetDacDb(void);

// ��ͨ�������йصĺ���

uint16_t MGetThick(void);           // �õ��������
void     MSetThick(uint16_t thick); // ���ù������

uint16_t MGetDiameter(void);              // �õ�����ֱ��
void     MSetDiameter(uint16_t Diameter); // ���ù���ֱ��

uint16_t MGetOffset(void);                                       /*����ֵΪ��ǰͨ��	*/
void     MSetOffset(uint32_t Offset, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint16_t MGetSpeed(void);                                      /*����ֵΪ��ǰͨ���Լ����٣�	��С��	*/
void     MSetSpeed(uint16_t Speed, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

int  MGetDepth();
void MSetDepth(int depth, int mode);

uint32_t MGetAngle(uint32_t mode);                               /*����ֵΪ��ǰͨ�������0�Ƕ�1����2����3����	�Ƕ�һλС��������4λ*/
void     MSetAngle(uint16_t Angle, int mode /*mode = 0\1\2*/);   /*���õ�ǰͨ��	*/
uint16_t MGetKvalue(void);                                       /*����ֵΪ��ǰͨ��Kֵ ��λС��	*/
void     MSetKvalue(uint16_t Kvalue, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint16_t MGetForward(void);                                    /*����ֵΪ��ǰͨ��̽ͷǰ�س��� һλС��	*/
void     MSetForward(uint16_t Forward, int mode /*mode = 0*/); /*���õ�ǰͨ��	*/

uint16_t MGetFrequence(void);                                      /*����ֵΪ��ǰͨ��̽ͷƵ�� һλС��	*/
void     MSetFrequence(uint16_t Frequence, int mode /*mode = 0*/); /*���õ�ǰͨ��	*/

uint16_t MGetCrystal_l(void);                                    /*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
uint16_t MGetCrystal_w(void);                                    /*����ֵΪ��ǰͨ����Ƭ�ߴ� a*256+b	*/
void     MSetCrystal_l(uint16_t Crystal, int mode /*mode = 0*/); /*���õ�ǰͨ��	*/
void     MSetCrystal_w(uint16_t Crystal, int mode /*mode = 0*/); /*���õ�ǰͨ��	*/

int   MGetNearField(double multi); // �õ�����������
float MGetAttenuate(int depth);    // ����˥��ϵ��

uint32_t MGetRange(int mode);                             /*����ֵΪmode=0ֱ�Ӿ��룬��0Ϊ��ǰ����µľ���	*/
void     MSetRange(int Range, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint32_t MGetDelay(int mode);                                  /*����ֵΪmode=0ʱ�䣬1��ǰ����µ�����,3��ʱ����	*/
void     MSetDelay(int Delay, int mode /*mode = 0\1\2*/);      /*���õ�ǰͨ��	*/
void     MSetScaleDelay(int Delay, int mode /*mode = 0\1\2*/); /*���õ�ǰ����µ���ʱ����	*/

uint16_t MGetReject(void);                                       /*����ֵΪ��ǰͨ��	*/
void     MSetReject(uint16_t Reject, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint16_t MGetSysGain(void);                                          // �õ���������
uint16_t MGetSurfGain(void);                                         /*����ֵΪ��ǰͨ�����油������ һλС��	*/
int      MSetSurfGain(uint16_t SurfGain, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint16_t MGetBaseGain(void);                                      /*����ֵΪ��ǰͨ���������� һλС��	*/
int      MSetBaseGain(short BaseGain, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

short MGetCompGain(void);                                      /*����ֵΪ��ǰͨ���������� һλС��	*/
int   MSetCompGain(short CompGain, int mode /*mode = 0\1\2*/); /*���õ�ǰͨ��	*/

uint16_t MGetDepthGain(void);
void     MSetDepthGain(uint16_t gain); // ������DAC/AVGʱ��Ȳ���ֵ

short MGetLineGain(int line);             // �õ����ߵ�ƫ����
void  MSetLineGain(int line, short gain); // �������ߵ�ƫ����

void MSetSystemGain(void);   /*���õ�ǰ���� */
void MSetSystemRange(void);  /*���õ�ǰ���� */
void MSetSystemDelay(void);  /*���õ�ǰ��ʱ����� */
void MSetSystemBright(void); /*���õ�ǰ��Ļ���� */
int  MSetSystemProbe(void);  /*���õ�ǰ̽ͷģʽ*/
void MSetSystem(void);       /*���õ�ǰϵͳ��״̬*/

// ���û������йصĺ���
int  UserManager(void);
int  UserStore(void);
int  UserRestore(void);
int  GetUserNumber();           // �õ���ǰ���õ��û�����
void SetUserNumber(int number); // ���õ�ǰ�û�����
int  GetCurrentUser();          // �õ���ǰ�û�����ţ�0��������1
void SetCurrentUser(int user);  // ���õ�ǰ�û�����ţ�0��������1
void UserMove(int user);

int  GetUserName(uint16_t name[], int nUserNumber); // �õ�����û�������,����1����ȡ�ã�0�д�
int  CheckPwd(int x, int y, int pwd);
int  InputPwd(int x, int y, int total_len /*�ܳ���*/); // ���ظ�ֵ��ʾ�Լ��˳�����ֵ������
void WritePwd(int x, int y, int number);
int  GetPwd(int nUserNumber); // �õ�����û�������

void SysPwd(int mode);

// �����뷨�йصĺ���
int MInputChar(uint32_t xpos, uint32_t ypos, uint32_t type, uint16_t *InputChar, uint16_t len_char_max, uint16_t char_max);
/*�����ַ���type = 0ֻ��Ӣ�ģ�1��Ӣ����,len_char_maxÿ�п�д�����Ӣ���ַ�����char_max�ܹ���д����Ӣ���ַ���*/
int      MGetCChar(uint32_t xpos, uint32_t ypos, uint16_t association); /*�õ��ַ�ֵ��Ӣ��Ϊascii�룬����Ϊ��λ��*/
int      MGetQuwei(uint32_t xpos, uint32_t ypos, uint16_t association);
uint16_t MGetEChar(uint32_t xpos, uint32_t ypos, uint16_t *pzText, uint32_t mode); /*�õ�ASCII�ַ�*/
char     MGetSymbol(void);                                                         /*�õ�8x8������*/

// ��ز������йصĺ���
void MGetGatePeakBuffer();
int  MGetGateParaMax(int gatetype);
// �õ�������߻ز��Ĵ���ʱ�䡢ˮƽ�㡢���롢����,gatetype = 0��A�ţ�=1��B��
// ����C_TRUE��ʾ���µ���߲�,���в�����ʷ��߲���ͬһ�������0.1s
void MSetGateParaInit(void);              // ���ֵ
int  MGetAmpMax(int gatetype);            /*�õ�ָ���ţ�0=A or 1=B)�ڻز��߶ȣ�*/
int  MGetAmpPos(int gatetype);            /*�õ�ָ���ţ�A or B)�ڻز�ˮƽ������*/
int  MGetAmpPos1(int gatetype, int mode); /*�õ�ָ�����ڻز�mode=0��ʼ/1���/2����3/ǰ�����ཻ/4������ཻ�ĵ�����*/

int MGetAmpTime(int gatetype);  /*�õ�ָ���ţ�A or B)�ڻز�����ʱ�䣬*/
int MGetAmpDist(int gatetype);  /*�õ�ָ���ţ�A or B)�ڻز��������̣�*/
int MGetAmpDist1(int gatetype); /*�õ�ָ���ţ�A or B)�ڻز��������̣�*/
int Amp2db(int pos, int amp);   // ���ݻز�λ�úͲ��ߵõ�dBֵ

// ����ϵͳ״̬����
uint32_t MLoadSystemStatus(int mode); // ȡϵͳ״̬�е�ĳ������,mode=0-n,����ĸ˳����ϵ���
void     MSaveSystemStatus(int mode, uint32_t para);

uint32_t MGetPrinterMode(void);                                     /*����ֵΪ��ǰ��ӡ������1\2\3\4=LQ\HP\EPSON C/P*/
void     MSetPrinterMode(uint32_t iPrinter, int mode /*mode = 0*/); /*���õ�ǰ��ӡ������*/

uint32_t MGetInputMode(void);                                   /*����ֵΪ��ǰ���뷨����1\2\3\4=����\��д\Сд*/
void     MSetInputMode(uint32_t iInput, int mode /*mode = 0*/); /*���õ�ǰ���뷨����*/

uint32_t MGetSystemMode(void);                                    /*����ֵΪ��ǰϵͳ״̬0\1=����\����*/
void     MSetSystemMode(uint32_t iSystem, int mode /*mode = 0*/); /*���õ�ǰϵͳ״̬*/

uint32_t MGetAcquisitionEnable(void); /*�Ƿ��������d5*/
void     MSetAcquisitionEnable(uint32_t acquisition, int mode /*mode = 0\1\2*/);
void     MSetAcquisition(uint32_t mode); /*���ò������0������1����*/
uint32_t MGetFill(void);                 // ��ʾӲ������ʵ�Ļ��ǿ���
void     MSetFill(uint32_t fill, int mode);
uint32_t MGetHardEchoShow(void); // ��ʾӲ������
void     MSetHardEchoShow(uint32_t show, int mode);

uint32_t MGetSoftEchoShow(void); // ��ʾ�������
void     MSetSoftEchoShow(uint32_t show, int mode);

uint32_t MGetCursorEnable(void); // �Ƿ������й��
void     MSetCursorEnable(uint32_t cursor, int mode);

uint32_t MGetCursorShow(void); // �Ƿ���ʾ���
void     MSetCursorShow(uint32_t cursor, int mode);
uint32_t MGetColorMode(void); //
void     MSetColorMode(uint32_t item, int mode);
uint32_t MGetChannel(void); /*���ط�����ǰͨ����*/
int      MSetChannel(int channel, int mode /*mode = 0*/);

uint32_t MGetSoundAlarm(int mode);            // mode = 0������1����
void     MSetSoundAlarm(int mode, int alarm); // mode = 0������1����

uint32_t MGetScreenBrightness(void);                                            /*����ֵΪ��ǰ��Ļ����*/
void     MSetScreenBrightness(uint32_t iBrightness, int mode /*mode = 0\1\2*/); /**/

int  MGetInputEnable(int type);
void MSetInputEnable(int bEnable, int type); // ����ĳ�����뷨�Ƿ��ѡ

int MGetSaveMode(void); // �洢ģʽ0��1����
int MSetSaveMode(int style, int mode);
int MGetSaveStatus(void); // �洢״̬0���ڴ棬1���ڴ�
int MSetSaveStatus(int status, int mode);
int MGetSaveTime(void); // �õ���������ʱ��
int MSetSaveTime(int time, int mode);
int MGetSavePosition(void); // �õ��洢λ��
int MSetSavePosition(int time, int mode);

int  MGetSaveFrequency();
void MSetSaveFrequency(int bSave, int mode); // �Ƿ�洢Ƶ��

int  MGetCraft(); // �õ����պ�
void MSetCraft(int craft, int modee);

int  MGetDacShowMode(void); // ����dac��ʾģʽ��0������1dB
void MSetDacShowMode(int show, int mode);

int  MGetKeyLock();
void MSetKeyLock(int lock, int mode); // ������,1��,0��

void MSetAmp2Gate(int gatetype, int gate); // ���ڻز��Ƿ�����Ÿ�
int  MGetAmp2Gate(int gatetype);

int  MGetGateEchoColor(int gatetype);
void MSetGateEchoColor(int gatetype, int gate); // ���ڻز���ɫ�Ƿ�Ҫ��

void MSetReadForeland(int read); // �Ƿ�ǰ�ض���
int  MGetReadForeland();

// �õ�����
int MGetLanguage(void);
// ��������
void MSetLanguage(int language);
// �õ�������λ
void MSetUnitType(int unit);
// ���ü�����λ
int MGetUnitType(void);

/*
�����ƶ������ơ���ʼ��
*/

int ChannelParaStore(void);   /*ͨ�������浽������*/
int ChannelParaRestore(void); /*ͨ�������ӱ�����ȡ*/

int SystemParaStore(void);   /*ϵͳ�����浽������*/
int SystemParaRestore(void); /*ϵͳ�����ӱ�����ȡ*/

int AllParaStore(void);
int AllParaRestore(void); // ȫ���������ݺͻָ�
// ���ӡ�йصĺ���
int mSendByteToPrinter(uint32_t pdata);

uint32_t TestPrinterReady();  // ����ӡ���Ƿ�׼���ã���û�г�����ʾ����3�Σ�����1Ϊ׼����
void     PrinterInitialize(); // ��ӡ����ʼ��
void     PrintEnd(void);      // ��ӡ����
void     PrinterFormFeed();   // ��ӡ����ֽ
void     SetPrnPosi_AbHori_EpC61(uint8_t nl, uint8_t nh);
// Set absolute horizontial print positon,Epson C61 Para:Ah=nH,Al=nl	ESC $ nL nH;
void SetPrnPosi_ReVert_EpC61(uint8_t ml, uint8_t mh);
// ���ô�ֱ��Դ�ӡλ�ã�Para:Al=mL,Ah=mH  ESC ( v nl nh mL mH,
void SetPrn_Raster_EpC61(uint16_t PrintHoriBytes); // Para:Print_HoriBytes,����ӡ�����ֽ���������40����80
// ESC . c v h m nl nh d1...dk(d1...dk���¶������
void     LQ12To24(uint32_t xpos, uint32_t ypos, uint8_t *PrnBuff); // ���ֱ12���Ϊ24��
uint16_t Byte2Word(uint8_t Byte);                                  // һ���ֽڱ�Ϊһ����8-16
uint32_t Byte2DWord(uint8_t Byte);                                 // һ���ֽڱ�Ϊһ��˫��8-32
void     ScrcpyInit(uint16_t PrintHoriBytes);
void     ScrcpyInvert(uint32_t xpos, uint32_t ypos, uint16_t PrintHoriBytes);
uint32_t ScreenCopy(void);                  // ����
void     ReportPrint(void);                 // �����ӡ
int      ReportText(int mode, int file_no); // �����ı�

// ���þ��Դ�ֱ��ӡλ�ã��Զ����հ�Ϊ��׼
void SetPrnPosi_AbVert(int dots /*�ƶ�������ֻ������*/);
// ������Դ�ֱ��ӡλ�ã��Ե�ǰλ��Ϊ��׼
void SetPrnPosi_ReVert(int dots /*�ƶ������������ɸ�*/);
// ���þ���ˮƽ��ӡλ�ã����󲿿հ�Ϊ��׼
void SetPrnPosi_AbHori(int dots /*�ƶ�������ֻ������*/);
// �������ˮƽ��ӡλ�ã��Ե�ǰλ��Ϊ��׼
void SetPrnPosi_ReHori(int dots /*�ƶ������������ɸ�*/);

// void ScreenStore();//������Ļ����
void DebugInfo(int num); // C_DEBUG == 1ʱ���ڣ�10��30����ʾ����

#if C_DEVLIB == 4
void     SwitchBuzzerOn(uint32_t bEnable);
uint8_t *GetDacBuffer();
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void ActivateDac(int bOn);
/* peak amplitude dac adjusting */
void     DacGatePeak();
void     UpdateEchoColor();
void     EnableEchoDisplay(uint32_t ena, uint32_t mode_fill);
void     NotExtendEchoDisplay(uint32_t notext);
uint32_t SerialPortByteOut(uint32_t dout);
uint32_t SerialPortByteIn();
/* noise reduction , level: 0->no, 1->2 average, 2->4 average, 3->8 average */
// void 	DeNoise(uint32_t level) ;

#elif C_DEVLIB == 23 || C_DEVLIB == 24

uint32_t SerialPortByteOut(uint32_t dout);
uint32_t SerialPortByteIn();
uint8_t *GetDacBuffer();
/* activate/deactivate DAC funtion , bOn != 0 -> activate else deactivate */
void ActivateDac(int bOn);
/* peak amplitude dac adjusting */
void DacGatePeak();

#endif

void m_DrawLine(int x1, int y1, int x2, int y2);
void m_DrawDot(int x, int y); // ��pDC��ָ���豸�ϴӻ���(x,y)
void m_DrawCircle(int x, int y, int radius);
// ��pDC��ָ���豸���Ե�(x,y)ΪԲ��,radiusΪ�뾶��Բ��,start_angleΪ��ʼ�Ƕ�,ˮƽx����Ϊ0��
// angleΪ����ʼֵ˳ʱ�뷽����ת�Ƕ�ֵ,
void m_DrawArc2(int x, int y, int radius, double start_angle, double angle);
// ��pDC��ָ���豸�ϴӵ�(x1,y1)/(x2,y2)��Բ��,mode=0ֱ��1����͹��1���°�
// ��������Բ���İ뾶
int m_DrawArc1(int x1, int y1, int x2, int y2, int mode);

// void WELD_DRAW( CDC *pDC);
void WELD_DRAW(WELD_PARA *stWeld, int wlpos, int wrpos, int wypos, int maxsize, int ProbeBeta, int dist_dire);
void SINGALWELD_DRAW(WELD_PARA *stWeld, WELD_POSI *stWPosi);
void DOUBLEWELD_DRAW(WELD_PARA *stWeld, WELD_POSI *stWPosi);
void TWELD_DRAW(WELD_PARA *stWeld, WELD_POSI *stWPosi);
void WELD_BUGLINE(WELD_PARA *stWeld, WELD_POSI *stWPosi, int ProbeBeta, int dist_dire);

#ifdef __cplusplus
}
#endif
