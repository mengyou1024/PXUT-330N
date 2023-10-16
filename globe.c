
// main.c
GATE_PEAK         GatePeak;
GATE_PARA         AGatePara;     /*8*/
GATE_PARA         BGatePara;     /*8*/
GATE_MAX          GateMax;       // 门内出现的最高波
FUNCTION          Function;      // 功能
SYSTEM_STATUS     SystemStatus;  /*4*/
CHANNEL_STATUS    ChannelStatus; /*4*/
CHANNEL_PARAMETER ChannelPara;   /*15*2*/
NOTES             Notes;
CACHE             cache;
FAT               fat;
CURVE_AMEND       stCurveAmend;
uint8_t           bCOM2PC;
WELD_PARA         stWeldPara;
ADJUST_TIME       AdjustTime;
DAC               Dac;
uint8_t          *dactable;
int               HoriOffsetScreen; // 所有写在屏幕上的内容，水平方向开始点
int               VertOffsetScreen; // 所有写在屏幕上的内容，水平方向开始点
double            dac_coe;          // 系数，与80%的差距
int               SecondTime;


// keyio.c
int g_iNum;

// display.c
uint8_t crPara[C_CR_MAX+12];