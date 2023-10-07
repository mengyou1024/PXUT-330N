//main.h
#ifndef _MAIN_H_

void MainInitialize(void);	/*������ʼ��*/
void AppMain(void);			/*��ѭ��*/
void SystemClear(void);			/*�������ʼ���Ĳ����ڴ˺����н���*/
void TestGainTable();
void TestGainTable1();
void AdjustGainTable();


void DisplayBMP(const u_char * picture,const u_long * Palette, int xpos, int ypos,int length,int width);
//���Ժ���
int TestMenuOffset(void);
int TestOffset(u_int testrang1,u_int testrange2,u_int mode);
//��1���̣���2���� mode = 0����⣬=1��� =2 ����
int TestMenuAngle(void);
int TestAngle(u_int diameter,u_int depth);//ֱ�������

int TestMenuDac(void);
int TestDac(u_int depthmax,u_int diameter,u_int length, short test_num);//�����ȡ�������ֱ��������
int CalcuDac(void);	//����DAC,
int DrawDac(int mode);		//���ͻ����DAC����,mode=0������=1��
int DrawDac2(int mode);		//���ͻ����DAC����,mode=0������=1��
int GetLine_TwoDot(u_short* line,int dist1,int dist2);
int GetMiddleValue(int dist1,int db1,int dist2,int db2,int dist);	//�õ��м�ֵ

int TestFlawHigh(void);		//���

int TestMenuStandard(void);
int TestMenuThick(void);
int TestMenuSystem(void);
int TestMenuFft(void);

void BScan(void);

int ManualMonthTest(void);//�˹�������У��

int TestMenuSensitivity(void);//������У��
int TestSensitivity(u_int depth/*���������*/, u_int stdamp/*��׼���߶�*/, int mode/*0�ɵ����棬1���ɵ�*/);

int MenuMonthTest(void);//��������У��
//�˵�����
int KeyManage(int keycode,int mode);		//��������,mode=1renovate
int MenuChoice(void);
int TestChoice(int Test);
int TestMenu(void);
int ParaMenu1(int page);//���ֵڼ�ҳ�Ĳ����˵�
int ParaMenu(int page);//���ֵڼ�ҳ�Ĳ����˵�
int ChannelMenu(void);
int DataMenu(int mode);	//mode > 0 Ϊ��������ֱ�ӽ����¼�˵�
int FuncMenu(void);
int ColorMenu(void);

int STDMenu(void);
int OptionMenu(int page);
void InitPara(void)	;//��ʼ��

//��洢�йص�
int WaveSave(int mode);
int DataProcedure();	//���ݴ���
int DataProceduresd();	//���ݴ���
int DataDisplaysd(int language);	//��ʾ�洢������
int DataDisplay(int file_no,int language);
int DataSave();		//����1��ʾ�Ѵ棬0δ��
int DataStore(int file_no);
int DataRestore(int file_no);
int GetDataPosi(int file_no);	//�õ��� x �����ݴ洢λ��
int DataDelete(int file_no);	//ɾ��һ�����ݣ�����Ϊ��������ɾ������ʣ�������������������󷵻أ�1
int FileDelete(int file_no,unsigned int off_total/*�ļ������洢λ��*/, unsigned off_fat/*�ļ�ͷ��ʼ*/,int len_fat/*�ļ�ͷ��*/, unsigned int off_file/*�ļ��忪ʼ*/, int len_file/*�ļ��峤*/);

int AutoSave(int mode);
int AutoSavePlay(int file_no,int file_total);		//�ӵ�ǰ���ݿ�ʼ���������洢�Ĳ���
int SeekDate(int file_total);
int SeekName(int file_total);

#define _MAIN_H_
#endif
