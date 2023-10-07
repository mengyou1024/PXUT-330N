
#ifndef _LIMITS_H___
#include <limits.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#include <stdint.h>
#include <stdbool.h>

/* int:   signed 32-bit integer , uint32_t:   unsigned 32-bit integer */
/* short: signed 16-bit integer , uint16_t: unsigned 16-bit integer */
/* char:  signed  8-bit integer , uint8_t:  unsigned  8-bit integer */

#define SAMPLE_FREQ			100		// MHz
#define MAX_GAIN_DB			1200	// 0.1dB

#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		240
#define FULL_ECHO_POINTS	308
#define FULL_ECHO_HEIGHT	200
#define EFFE_ECHO_POINTS	280

#define ASK_PARAMETER		0x100

#define AMP_BAND_NARROW_1	0x110
#define AMP_BAND_NARROW_2	(AMP_BAND_NARROW_1 + 1)
#define AMP_BAND_WIDE		(AMP_BAND_NARROW_2 + 1)

#define FULL_WAVE			0x120
#define POSI_HALF_WAVE		(FULL_WAVE + 1)
#define NEGA_HALF_WAVE		(POSI_HALF_WAVE + 1)
#define RF_WAVE				(NEGA_HALF_WAVE + 1)

#define LOW_DAMPING			0x130
#define HIGH_DAMPING		(LOW_DAMPING + 1)

#define T_T_SINGLE			0x140
#define T_R_DUAL			(T_T_SINGLE + 1)

#define LOW_PULSE			0x150					/*  50V */
#define MED1_PULSE			(LOW_PULSE + 1)			/* 100V */
#define MED2_PULSE			(MED1_PULSE + 1)		/* 200V */
#define MED3_PULSE			(MED2_PULSE + 1)		/* 300V */
#define MED4_PULSE			(MED3_PULSE + 1)		/* 400V */
#define MED5_PULSE			(MED4_PULSE + 1)		/* 500V */
#define HIGH_PULSE			(MED5_PULSE + 1)		/* 600V */

#define PM_COPY_READ		0x210
#define PM_COPY_WRITE		(PM_COPY_READ + 1)
#define PM_COPY_MOVE		(PM_COPY_WRITE + 1)

#define USB_DATA_TRANSFER_OK		0
#define USB_DATA_TRANSFER_BUSY		1
#define USB_DATA_TRANSFER_ERROR		2
#define USB_DATA_TRANSFER_DECLINED	3
#define USB_DATA_TRANSFER_NOT_READY 4

#define RED_B0				0x01
#define RED_B1				0x02
#define RED_B2				0x04
#define GREEN_B0			0x08
#define GREEN_B1			0x10
#define GREEN_B2			0x20
#define BLUE_B0				0x40
#define BLUE_B1				0x80

#define COLOR_RED			(RED_B0 | RED_B1 | RED_B2)
#define COLOR_RED_L			(RED_B2)
#define COLOR_GREEN			(GREEN_B0 | GREEN_B1 | GREEN_B2)
#define COLOR_GREEN_L		(GREEN_B2)
#define COLOR_BLUE			(BLUE_B0 | BLUE_B1)
#define COLOR_BLUE_L		(BLUE_B1)
#define COLOR_YELLOW		(COLOR_RED | COLOR_GREEN)
#define COLOR_PINK			(COLOR_RED | COLOR_BLUE)
#define COLOR_CYAN			(COLOR_GREEN | COLOR_BLUE)
#define COLOR_WHITE			(COLOR_RED | COLOR_GREEN | COLOR_BLUE)
#define COLOR_GREY			(COLOR_RED_L | COLOR_GREEN_L | COLOR_BLUE_L)
#define COLOR_BLACK			0x000

#define ECHO_RED_B0			0x01
#define ECHO_RED_B1			0x02
#define ECHO_RED_B2			0x04
#define ECHO_GREEN_B0		0x08
#define ECHO_GREEN_B1		0x10
#define ECHO_GREEN_B2		0x20
#define ECHO_BLUE_B0		0x40
#define ECHO_BLUE_B1		0x80

#define ECHO_COLOR_RED		(ECHO_RED_B0 | ECHO_RED_B1 | ECHO_RED_B2)
#define ECHO_COLOR_GREEN	(ECHO_GREEN_B0 | ECHO_GREEN_B1 | ECHO_GREEN_B2)
#define ECHO_COLOR_BLUE		(ECHO_BLUE_B0 | ECHO_BLUE_B1)
#define ECHO_COLOR_YELLOW	(ECHO_COLOR_RED | ECHO_COLOR_GREEN)
#define ECHO_COLOR_PINK		(ECHO_COLOR_RED | ECHO_COLOR_BLUE)
#define ECHO_COLOR_CYAN		(ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_WHITE	(ECHO_COLOR_RED | ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_BLACK	0x000

typedef struct
{
    uint32_t 	Gate1PeakSampPoints ;
    uint32_t 	Gate2PeakSampPoints ;
    uint32_t 	Gate3PeakSampPoints ;
    uint16_t Gate1PeakPos ;
    uint16_t Gate2PeakPos ;
    uint16_t Gate3PeakPos ;
    uint16_t Gate1PeakAmp ;
    uint16_t Gate2PeakAmp ;
    uint16_t Gate3PeakAmp ;
} GATE_PEAK;

typedef struct
{
    uint16_t Position ;//位置
    uint16_t Width ;//宽
    uint16_t Height ;//高
    uint8_t  Type ;//种类A/B
    uint8_t  AlarmEnable ;//警报
} GATE_PARA;

typedef struct
{
    uint8_t	second ;
    uint8_t	minute ;
    uint8_t	hour ;
    uint8_t	date ;
    uint8_t	month ;
    uint8_t	day ;
    uint8_t	yearl ;
    uint8_t  yearh ;
} TIME_CLOCK;

/* set repeat rate of data acquisition in Hz(100, 1000)
   return value: previous rate */
void	SetRepeatRate(uint32_t rate) ;
/* switch on-off echo display , ena = 1 -> on ; mode_fill = 1 -> echo fill display */
void 	EnableEchoDisplay(uint32_t ena, uint32_t mode_fill) ;
/* set screen background color */
void	SetBackgroundColor(uint32_t color) ;
/* set screen render color */
void 	SetDisplayColor(uint32_t color) ;
/* mode_xor: 1 -> read-xor-write; 0 -> overwrite */
void 	SetDisplayMode(uint32_t mode_xor) ;
/* buffer echo color data, pos_b: echo point start position(<508)
   pos_e: echo point end position(<508) , rgb(b0-b7): echo display */
void	SetEchoColor(uint32_t pos_s, uint32_t pos_e, uint32_t rgb) ;
/* validate echo color */
void 	UpdateEchoColor() ;
/* set echo dc level */
void 	SetEchoBaseLine(uint32_t level) ;
/* h_start: horizontal start(>1), : h_end: horizontal end(>h_start),
   v_start: vertical offset, v_half: vertical half screen display */
void 	SetEchoLayout(uint32_t h_start, uint32_t h_end, uint32_t v_start, uint32_t v_half) ;
/* size_xy: (b15-b0) -> x-dot size; (b31-b16) -> y-dot size */
void 	UserDotsOut(uint32_t x, uint32_t y, uint16_t* pDots, uint32_t size_xy) ;
/* display chinese character string pointed to (pzText) with length (len) at (x,y) */
void 	CTextOut(uint32_t x, uint32_t y, uint16_t* pzText, uint32_t len) ;
/* display english character string pointed to (pzText) with length (len) at (x,y) */
void 	ETextOut(uint32_t x, uint32_t y, char* pzText, uint32_t len) ;
/* draw a line from point(x1,y1) to (x2,y2), including both points */
void 	DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) ;
/* draw a point at (x,y) */
void 	DrawPixel(uint32_t x, uint32_t y) ;
/* get one screen pixel color value from (x,y) to (x,y) */
uint32_t 	GetPixel(uint32_t x, uint32_t y) ;
/* erase screen window */
void 	EraseWindow(uint32_t left, uint32_t top, uint32_t width, uint32_t height) ;
/* invert screen window */
void 	InvertWindow(uint32_t left, uint32_t top, uint32_t width, uint32_t height) ;
/* left_sd : b0-b15  -> screen window left-top corner x,
			 b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
void 	PushWindow(uint32_t left_sd, uint32_t top_sd, uint32_t width, uint32_t height) ;
/* left_sd : b0-b15  -> screen window left-top corner x,
			 b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
void	PopWindow(uint32_t left_sd, uint32_t top_sd, uint32_t width, uint32_t height) ;
/* convert integer decimal to ascii string according to the assigned Format */
/* Format: (b0-b7) is total digit number(including sign and point) ,
	   	   (b8-b15) is decimal digit number(excluding point) ,
	       (b16-b19)=0, 1, 2 -> no align, front align, back align
	       (b20-b23)=0 -> no positive sign, 1 -> add positive sign
	        b24 : determines if the null character is appended */
int 	DecimalToAscii(char* pzText, int Para, uint32_t Format) ;
/* convert float to integer decimal, ten_radix: digit number after point */
int 	FloatToDecimal(float fpar, uint32_t ten_radix) ;
/* set the system gain of flaw detector, gain >=0 and <=MAX_GAIN_DB with precision 0.1dB */
uint32_t 	SetAmplifierGain(uint32_t gain) ;
/* set the system frequency band of flaw detector,
   (band) must be in {AMP_BAND_NARROW_1, AMP_BAND_NARROW_2, AMP_BAND_WIDE} */
uint32_t 	SetAmplifierBand(uint32_t band) ;
/* set echo amplitude rejection, reject >=0 and <=800 with precision 0.1% */
uint32_t 	SetEchoReject(uint32_t reject) ;
/* set echo acquisition mode ,
   (mode) must be in {FULL_WAVE, POSI_HALF_WAVE, NEGA_HALF_WAVE, RF_WAVE} */
uint32_t 	SetEchoMode(uint32_t mode) ;
/* set ultrasonic inspection range in sampling points ,  */
uint32_t 	SetEchoRange(uint32_t range) ;
/* set inspection delay in sampling points */
uint32_t 	SetEchoDelay(uint32_t edelay) ;
/* set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2 */
void 	SetPeakGate(GATE_PARA* pGate , int gate_index) ;
/* set the connection mode of transmitting and receiving probes,
   (mode) must be in {T_T_SINGLE, T_R_DUAL} */
void 	SetProbeMode(uint32_t mode) ;
/* set the damping for the transmitting probe,
   (damp) must be in {LOW_DAMPING, HIGH_DAMPING} */
void 	SetProbeDamping(int damp) ;
/* set the pulse strength for the transmitting ptobe,
   (pulse) must be in {LOW_PULSE, MEDIUM1_PULSE, MEDIUM2_PULSE, HIGH_PULSE} */
void 	SetTransmitterVoltage(uint32_t pulse) ;
/* width: 1-127 */
void SetPulseWidth(uint32_t width) ;
/* bEnable = 1 for activating data acquisition ,
   bEnable = 0 for disabling data acquisition */
uint32_t 	EnableDataAcquisition(uint32_t bEnable, uint32_t bWait) ;
uint32_t 	IfDataAcquisitionActivated() ;
uint32_t 	ActivateEchoEnvelope(int act) ;
/* get the pointer to kernel gate peak buffer, don't modify! */
GATE_PEAK* GetGatePeakBuffer() ;
/* get the pointer to kernel echo data buffer, don't modify! */
uint8_t* GetSampleBuffer() ;
uint8_t* GetEchoEnvelopeBuffer() ;
/* get the pointer to kernel dac data buffer, don't modify! */
/* transfer data to/from/within the system protected memory,
   the address of the protected memory start from 0,
   copymode must be in {PM_COPY_READ, PM_COPY_WRITE, PM_COPY_MOVE} ,
   it's better to check the return value to see if succeeded(>0) or failed(0) */
int 	CopyProtectedMemory(void* memdes, void* memsrc, uint32_t bytesize, uint32_t copymode) ;
/* noise reduction , grade: 0-4 */
void 	DeNoise(uint32_t grade) ;
/* get the elapsed time from power-on, resolution is 1 ms */
uint32_t 	GetElapsedTime() ;
/* fill the assigned structure TIME_CLOCK, (tm) must be already declared */
void 	GetSystemTime(TIME_CLOCK* tm) ;
/* return value = 0, the printer is not ready, otherwise ready */
uint32_t 	IsPrinterReady() ;
/* send data to printer, pdata: the least byte is effective */
uint32_t 	SendByteToPrinter(uint32_t pdata) ;
/* return value > 0 , denoting some key has been and is being pressed,
   return value < 0 , denoting some key was pressed and just released,
   =0 , no key is pressed */
int 	GetKeyIndex(void) ;
/* increment: b6-b0, decrement: b8-b14, switch pressed: b15 = 1 */
uint32_t 	GetRotaryValue() ;
/* get the return value from battery management
   the return vaule: b0 -> battery charge fast(1) or trickle(0) ;
   		             b1 -> battery charge on(1) or off(0) ;
   		             b2 -> battery connected(1) or disconnected(0) ;
   		             b3 -> DC adapter connected(1) or disconnected(0) ;
   		             b8-b15 -> battery capacity value < 32 */
uint32_t 	GetBatteryStatus() ;
/* bEnable = 0 -> buzzer off, bEnable > 0 -> buzzer on */
void 	SwitchBuzzerOn(uint32_t bEnable) ;
/* bEnable = 0 -> alarm led off, bEnable > 0 -> alarm led on */
void 	SwitchAlarmOn(uint32_t bEnable) ;
/* adjust lcd brightness, 0 <= grade <= 31 */
void 	SetLCDBrightness(uint32_t grade) ;
/* set USB mode, 0 -> HOST, 1 -> SLAVE */
uint32_t	SetUsbMode(uint32_t mode) ;
/* turn off power to usb device */
void 	SwitchUsbDevicePowerOff() ;
/* 1 -> Inserted Or Removed, 0 -> no action */
uint32_t 	IsUsbDeviceInsertedOrRemoved() ;
/* 1 -> connected, 0 -> disconnected */
uint32_t 	IsUsbDeviceAttached() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
uint32_t	UsbHostDataOutStatus() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
uint32_t	UsbHostDataInStatus() ;
/* usb host data out setting, 1 -> success, 0 -> fail */
uint32_t 	UsbHostDataOut(char* pAddr, uint32_t length) ;
void 	UsbHostDataOutToggle() ;
void 	UsbHostOutAbort() ;
/* usb host data in setting, 1 -> success, 0 -> fail */
uint32_t 	UsbHostDataIn(char* pAddr, uint32_t length) ;
void 	UsbHostDataInToggle() ;
void 	UsbHostInAbort() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
uint32_t	UsbDevDataOutStatus() ;
/* return value: USB_DATA_TRANSFER_OK,
				 USB_DATA_TRANSFER_BUSY(previous transfer not complete),
				 USB_DATA_TRANSFER_ERROR(transfer corrupt),
				 USB_DATA_TRANSFER_DECLINED(usb not connectted)*/
uint32_t	UsbDevDataInStatus() ;
/* usb data out setting */
void 	UsbDevDataOut(char* pAddr, uint32_t length) ;
/* usb data in setting  */
void    UsbDevDataIn(char* pAddr, uint32_t length) ;
/* clear usb transfer error, dir: 0-> in; 1-> out,
   return value: 0-> unsuccessful, 1-> done */
uint32_t	UsbDevClearError(uint32_t dir) ;
