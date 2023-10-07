#pragma once

#define SAMPLE_FREQ                 100  // MHz
#define MAX_GAIN_DB                 1200 // 0.1dB

#define SCREEN_WIDTH                640
#define SCREEN_HEIGHT               480

#define ASK_PARAMETER               0x100

#define AMP_BAND_NARROW_1           0x110
#define AMP_BAND_NARROW_2           (AMP_BAND_NARROW_1 + 1)
#define AMP_BAND_NARROW_3           (AMP_BAND_NARROW_2 + 1)
#define AMP_BAND_WIDE               (AMP_BAND_NARROW_3 + 1)

#define FULL_WAVE                   0x120
#define POSI_HALF_WAVE              (FULL_WAVE + 1)
#define NEGA_HALF_WAVE              (POSI_HALF_WAVE + 1)
#define RF_WAVE                     (NEGA_HALF_WAVE + 1)

#define LOW_DAMPING                 0x130
#define HIGH_DAMPING                (LOW_DAMPING + 1)

#define T_T_SINGLE                  0x140
#define T_R_DUAL                    (T_T_SINGLE + 1)

#define LOW_PULSE                   0x150            /*  50V */
#define MED1_PULSE                  (LOW_PULSE + 1)  /* 100V */
#define MED2_PULSE                  (MED1_PULSE + 1) /* 200V */
#define MED3_PULSE                  (MED2_PULSE + 1) /* 300V */
#define MED4_PULSE                  (MED3_PULSE + 1) /* 400V */
#define MED5_PULSE                  (MED4_PULSE + 1) /* 500V */
#define HIGH_PULSE                  (MED5_PULSE + 1) /* 600V */

#define PM_COPY_READ                0x210
#define PM_COPY_WRITE               (PM_COPY_READ + 1)

#define USB_PRINTER_GET_DEVICE_ID   1
#define USB_PRINTER_GET_PORT_STATUS 2
#define USB_PRINTER_SOFT_RESET      3

#define BLUE_B0                     0x0001
#define BLUE_B1                     0x0002
#define BLUE_B2                     0x0004
#define BLUE_B3                     0x0008
#define BLUE_B4                     0x0010
#define GREEN_B0                    0x0020
#define GREEN_B1                    0x0040
#define GREEN_B2                    0x0080
#define GREEN_B3                    0x0100
#define GREEN_B4                    0x0200
#define GREEN_B5                    0x0400
#define RED_B0                      0x0800
#define RED_B1                      0x1000
#define RED_B2                      0x2000
#define RED_B3                      0x4000
#define RED_B4                      0x8000

#define COLOR_RED                   (RED_B0 | RED_B1 | RED_B2 | RED_B3 | RED_B4)
#define COLOR_RED_LIGHT             (RED_B4)
#define COLOR_GREEN                 (GREEN_B0 | GREEN_B1 | GREEN_B2 | GREEN_B3 | GREEN_B4 | GREEN_B5)
#define COLOR_GREEN_LIGHT           (GREEN_B5)
#define COLOR_BLUE                  (BLUE_B0 | BLUE_B1 | BLUE_B2 | BLUE_B3 | BLUE_B4)
#define COLOR_BLUE_LIGHT            (BLUE_B4)
#define COLOR_YELLOW                (COLOR_RED | COLOR_GREEN)
#define COLOR_PINK                  (COLOR_RED | COLOR_BLUE)
#define COLOR_CYAN                  (COLOR_GREEN | COLOR_BLUE)
#define COLOR_WHITE                 (COLOR_RED | COLOR_GREEN | COLOR_BLUE)
#define COLOR_GREY                  (COLOR_RED_LIGHT | COLOR_GREEN_LIGHT | COLOR_BLUE_LIGHT)
#define COLOR_BLACK                 0x0000

#define ECHO_RED_B0                 0x01
#define ECHO_RED_B1                 0x02
#define ECHO_RED_B2                 0x04
#define ECHO_GREEN_B0               0x08
#define ECHO_GREEN_B1               0x10
#define ECHO_GREEN_B2               0x20
#define ECHO_BLUE_B0                0x40
#define ECHO_BLUE_B1                0x80

#define ECHO_COLOR_RED              (ECHO_RED_B0 | ECHO_RED_B1 | ECHO_RED_B2)
#define ECHO_COLOR_GREEN            (ECHO_GREEN_B0 | ECHO_GREEN_B1 | ECHO_GREEN_B2)
#define ECHO_COLOR_BLUE             (ECHO_BLUE_B0 | ECHO_BLUE_B1)
#define ECHO_COLOR_YELLOW           (ECHO_COLOR_RED | ECHO_COLOR_GREEN)
#define ECHO_COLOR_PINK             (ECHO_COLOR_RED | ECHO_COLOR_BLUE)
#define ECHO_COLOR_CYAN             (ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_WHITE            (ECHO_COLOR_RED | ECHO_COLOR_GREEN | ECHO_COLOR_BLUE)
#define ECHO_COLOR_BLACK            0x000

#ifdef __cplusplus
extern "C" {
    #include <cstdint>
#else
    #include <stdint.h>
#endif /*__cplusplus*/

#include <stdint.h>

typedef struct
{
    uint32_t Gate1PeakSampPoints;
    uint32_t Gate2PeakSampPoints;
    uint32_t Gate3PeakSampPoints;
    uint16_t Gate1PeakPos;
    uint16_t Gate2PeakPos;
    uint16_t Gate3PeakPos;
    uint16_t Gate1PeakAmp;
    uint16_t Gate2PeakAmp;
    uint16_t Gate3PeakAmp;
} GATE_PEAK;

typedef struct
{
    uint16_t Position;
    uint16_t Width;
    uint16_t Height;
    uint8_t  Type;
    uint8_t  AlarmEnable;
} GATE_PARA;

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t date;
    uint8_t month;
    uint8_t day;
    uint8_t yearl;
    uint8_t yearh;
} TIME_CLOCK;

typedef struct _WavHeader {
    /// Contains the letters "RIFF" in ASCII form.
    unsigned int chunkID;
    /// Size of the rest of the chunk following this number.
    unsigned int chunkSize;
    /// Contains the letters "WAVE".
    unsigned int format;
    /// Contains the letters "fmt ".
    unsigned int subchunk1ID;
    /// 16 for PCM.  This is the size of the rest of the Subchunk
    /// which follows this number.
    unsigned int subchunk1Size;
    /// PCM = 1 (i.e. Linear quantization). Values other than 1 indicate some
    /// form of compression.
    unsigned short audioFormat;
    /// Mono = 1, Stereo = 2, etc.
    unsigned short numChannels;
    /// 8000, 44100, etc.
    unsigned int sampleRate;
    /// SampleRate * NumChannels * BitsPerSample/8
    unsigned int byteRate;
    /// NumChannels * BitsPerSample/8
    unsigned short blockAlign;
    /// 8 bits = 8, 16 bits = 16, etc.
    unsigned short bitsPerSample;
    /// Contains the letters "data".
    unsigned int subchunk2ID;
    /// Number of bytes in the data.
    unsigned int subchunk2Size;
} WavHeader;

/* System Initializaion, must be called at the very beginning */
extern void SystemInit(uint32_t TotalEchoPoints, uint32_t RangeEchoPoints);
/* get the base address of the display buffer */
extern uint16_t* GetDisplayBufferBase();
/* set repeat rate of data acquisition in Hz(100, 1000)
   return value: previous rate */
extern void SetRepeatRate(uint32_t rate);
/* ; mode_fill = 1 -> echo fill display, mode_rf = 1 -> rf display */
extern void SetEchoDisplayMode(uint32_t mode_fill, uint32_t mode_rf);
/* switch on-off echo display , ena = 1 -> on */
extern void EnableEchoDisplay(uint32_t ena);
/* set screen background color */
extern void SetBackgroundColor(uint32_t color);
/* set screen render color */
extern void SetDisplayColor(uint32_t color);
/* mode_xor: 1 -> read-xor-write; 0 -> overwrite */
extern void SetDisplayMode(uint32_t mode_xor);
/* buffer echo color data, pos_b: echo point start position(<508)
   pos_e: echo point end position(<508) , rgb(b0-b7): echo display */
extern void SetEchoColor(uint32_t pos_s, uint32_t pos_e, uint32_t rgb);
/* validate echo color */
extern void UpdateEchoColor();
/* set echo dc level */
extern void SetEchoBaseLine(uint32_t level);
/* h_start: horizontal start(>1), : h_end: horizontal end(>h_start),
   v_start: vertical offset, v_half: vertical half screen display */
extern void SetEchoLayout(uint32_t h_start, uint32_t h_end, uint32_t v_start, uint32_t v_half);
/* size_xy: (b15-b0) -> x-dot size; (b31-b16) -> y-dot size */
extern void UserDotsOut(uint32_t x, uint32_t y, uint8_t* pDots, uint32_t size_xy);
/* display chinese character string pointed to (pzText) with length (len) at (x,y) */
extern void CTextOut(uint32_t x, uint32_t y, uint16_t* pzText, uint32_t len);
/* display english character string pointed to (pzText) with length (len) at (x,y) */
extern void ETextOut(uint32_t x, uint32_t y, char* pzText, uint32_t len);
/* draw a line from point(x1,y1) to (x2,y2), including both points */
extern void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
/* draw a point at (x,y) */
extern void DrawPixel(uint32_t x, uint32_t y);
/* get one screen pixel color value from (x,y) to (x,y) */
extern uint32_t GetPixel(uint32_t x, uint32_t y);
/* erase screen window */
extern void EraseWindow(uint32_t left, uint32_t top, uint32_t width, uint32_t height);
/* invert screen window */
extern void InvertWindow(uint32_t left, uint32_t top, uint32_t width, uint32_t height);
/* left_sd : b0-b15  -> screen window left-top corner x,
             b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
extern void PushWindow(uint32_t left_sd, uint32_t top_sd, uint32_t width, uint32_t height);
/* left_sd : b0-b15  -> screen window left-top corner x,
             b16-b31 -> backup window left-top corner x,
   top_sd  : b0-b15  -> screen window left-top corner y,
             b16-b31 -> backup window left-top corner y  */
extern void PopWindow(uint32_t left_sd, uint32_t top_sd, uint32_t width, uint32_t height);
/* convert integer decimal to ascii string according to the assigned Format */
/* Format: (b0-b7) is total digit number(including sign and point) ,
           (b8-b15) is decimal digit number(excluding point) ,
           (b16-b19)=0, 1, 2 -> no align, front align, back align
           (b20-b23)=0 -> no positive sign, 1 -> add positive sign
            b24 : determines if the null character is appended */
extern int DecimalToAscii(char* pzText, int Para, uint32_t Format);
/* convert float to integer decimal, ten_radix: digit number after point */
extern int FloatToDecimal(float fpar, uint32_t ten_radix);
/* set the system gain of flaw detector, gain >=0 and <=MAX_GAIN_DB with precision 0.1dB */
extern uint32_t SetAmplifierGain(uint32_t gain);
/* pGain: address pointer to the start of the dynamic gain curve which deferred by DAC curve and must have
          a length of TotalEchoPoints set by SystemInit(uint32_t TotalEchoPoints, uint32_t RangeEchoPoints) */
extern void SetAmplifierDynamicGain(uint32_t* pGain);
/* switch dynamic gain on(1) and off(0) */
extern void AmplifierDynamicGainOnOff(uint32_t on);
/* set the system frequency band of flaw detector,
   (band) must be in {AMP_BAND_NARROW_1, AMP_BAND_NARROW_2, AMP_BAND_WIDE} */
extern uint32_t SetAmplifierBand(uint32_t band);
/* set echo amplitude rejection, reject >=0 and <=800 with precision 0.1% */
extern uint32_t SetEchoReject(uint32_t reject);
/* set echo acquisition mode ,
   (mode) must be in {FULL_WAVE, POSI_HALF_WAVE, NEGA_HALF_WAVE, RF_WAVE} */
extern uint32_t SetEchoMode(uint32_t mode);
/* set ultrasonic inspection range in sampling points ,  */
extern uint32_t SetEchoRange(uint32_t range);
/* set inspection delay in sampling points */
extern uint32_t SetEchoDelay(uint32_t edelay);
/* set the gate structure for peak maximum, gate_index 0 for gate 1 and 1 for gate 2 */
extern void SetPeakGate(GATE_PARA* pGate, int gate_index);
/* set the connection mode of transmitting and receiving probes,
   (mode) must be in {T_T_SINGLE, T_R_DUAL} */
extern void SetProbeMode(uint32_t mode);
/* set the damping for the transmitting probe,
   (damp) must be in {LOW_DAMPING, HIGH_DAMPING} */
extern void SetProbeDamping(int damp);
/* set the pulse strength for the transmitting ptobe,
   (pulse) must be in {LOW_PULSE, MEDIUM1_PULSE, MEDIUM2_PULSE, HIGH_PULSE} */
extern void SetTransmitterVoltage(uint32_t pulse);
/* width: 1-127 */
extern void SetPulseWidth(uint32_t width);
/* bEnable = 1 for activating data acquisition ,
   bEnable = 0 for disabling data acquisition */
extern uint32_t EnableDataAcquisition(uint32_t bEnable);
extern uint32_t IfDataAcquisitionActivated();
extern uint32_t ActivateEchoEnvelope(int act);
/* get the pointer to kernel gate peak buffer, don't modify! */
GATE_PEAK* GetGatePeakBuffer();
/* get the pointer to kernel echo data buffer, don't modify! */
extern uint8_t* GetSampleBuffer();
extern uint8_t* GetEchoEnvelopeBuffer();
/* get the pointer to kernel dac data buffer, don't modify! */
/* transfer data to/from/within the system protected memory,
   the address of the protected memory start from 0,
   copymode must be in {PM_COPY_READ, PM_COPY_WRITE} ,
   it's better to check the return value to see if succeeded(>0) or failed(0) */
extern int CopyProtectedMemory(void* memdes, void* memsrc, uint32_t bytesize, uint32_t copymode);
/* noise reduction , grade: 0-4 */
extern void DeNoise(uint32_t grade);
/* get the elapsed time from power-on, resolution is 1 ms */
extern uint32_t GetElapsedTime();
/* fill the assigned structure TIME_CLOCK, (tm) must be already declared */
extern void GetSystemTime(TIME_CLOCK* tm);
/* return value > 0 , denoting some key has been and is being pressed,
   return value < 0 , denoting some key was pressed and just released,
   =0 , no key is pressed */
extern uint32_t GetKeyCode(void);
/* idx = 0,1 ->encoder 1 and 2, en -> enable, rst -> reset, res( >=1 ) -> resolution */
extern void SetScanRotaryEncoder(uint32_t idx, uint32_t en, uint32_t rst, uint32_t res);
/**/
extern uint32_t GetScanRotaryValue(uint32_t idx);
/* return value: b0 -> power source, b1 -> charge done */
extern uint32_t GetPowerStatus();
/* bEnable = 0 -> buzzer off, bEnable > 0 -> buzzer on */
extern void SwitchBuzzerOn(uint32_t bEnable);
/* bEnable = 0 -> alarm led off, bEnable > 0 -> alarm led on */
extern void SwitchAlarmOn(uint32_t bEnable);
/* adjust lcd brightness, 0 <= grade <= 31 */
extern void SetLCDBrightness(uint32_t grade);
/**/
extern uint32_t CharToKeypad(uint32_t uchar);
/* addr: (b7-b0) -> command , (b15-b8) -> bus address */
extern uint32_t BatteryPackWrite(uint32_t addr, uint32_t len, void* pBuf);
/**/
extern uint32_t BatteryPackRead(uint32_t addr, uint32_t len, void* pBuf);

/* return value: 0x01 -> USB connected ,
                 0x02 -> Host Mode ,
                 0x10 -> Storage attached in Host Mode ,
                 0x20 -> Printer attached in Host Mode */
extern uint32_t UsbGetStatus();
/**/
extern uint32_t UsbDevDataOut(char* pDataBuf, uint32_t length);
/**/
extern uint32_t UsbDevDataIn(char* pDataBuf, uint32_t length);
/**/
extern uint32_t UsbPrinterWrite(char* pDataBuf, uint32_t length);
/**/
extern uint32_t UsbPrinterRead(char* pDataBuf, uint32_t length);
/*	req_type: (USB_PRINTER_GET_DEVICE_ID, USB_PRINTER_GET_PORT_STATUS, USB_PRINTER_SOFT_RESET)
    pDataBuf: buffer */
extern uint32_t UsbPrinterRequest(uint32_t req_type, void* pDataBuf);
/**/
extern uint32_t UsbStorageBlockWrite(uint32_t blkofs, uint32_t blknum, char* pDataBuf);
/**/
extern uint32_t UsbStorageBlockRead(uint32_t blkofs, uint32_t blknum, char* pDataBuf);

/* return value: 0x01 -> inserted ,
                 0x02 -> write protection ,
                 0x04 -> initialized */
extern uint32_t SDCardGetStatus();
/**/
extern uint32_t SDCardInit();
/**/
extern void SDCardStopTransfer();
/**/
extern uint32_t SDCardBlockWrite(uint32_t blkofs, uint32_t blknum, char* pDataBuf);
/**/
extern uint32_t SDCardBlockRead(uint32_t blkofs, uint32_t blknum, char* pDataBuf);
/**/
extern uint32_t EthernetInit(uint8_t* pMacAddr, uint8_t* pIpAddr);
/**/
extern uint32_t EthernetTransmit(uint8_t* buf, uint32_t len);
/**/
extern uint32_t EthernetReceive(uint8_t* buf, uint32_t len);
/**/
extern uint32_t EthernetStop(void);
/**/
extern void WavePlayback(WavHeader* pWaveStream, uint32_t vol);
/* vol: 0-100, 0 -> mute */
extern void WaveVolume(uint32_t vol);
/* */
extern uint32_t GetRotaryEncoder(uint32_t enc_idx);

#ifdef __cplusplus
}
#endif /*__cplusplug*/
