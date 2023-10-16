
#pragma once

#define USB_HOST_MODE     0 // Set USB Controller to Host mode
#define USB_SLAVE_MODE    1 // Set USB Controller to Slave mode
#define TRUE              1
#define FALSE             0

#define BUFFER_LENGTH     32768
#define GATE_PEAK_SIZE    300 // 24
#define ECHO_PACKAGE_SIZE 500 // 300

#define C_WRITE_TO_UDISK  1
#define C_WRITE_INTERVAL  1000

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _UDISK_INFO {
    uint32_t   IsAttached;   // U���Ƿ�����
    uint32_t   filelen;      // �ļ���С
    uint32_t   operatormode; // 1 -- Write ; 2 --Read
    uint32_t   writemode;    // 0 ,1, 2
    uint32_t   sample_num;
    uint32_t   transfer_surplus;
    uint32_t   FreeSpaceLowPart;   // U�����ɿռ�����ֽ�
    uint32_t   FreeSpaceHighPart;  // U�����ɿռ�����ֽ�
    uint32_t   TotalSpaceLowPart;  // U���ܿռ�����ֽ�
    uint32_t   TotalSpaceHighPart; // U���ܿռ�����ֽ�
    uint32_t   DiskIsFull;         // U������־
    uint32_t   DataHeaderMark;     // 0,�������䣻1,д����
    uint32_t   last_tick_count;
    uint32_t   TotalWriteTimes;
    uint32_t   LastFileLength; // ���һ�δ�ĳ���
    char       filename[8];
    TIME_CLOCK system_time;
    char      *pEchoDataBuffer;
    char      *pEchoWriteBuffer;

    uint8_t DataBuffer[2][BUFFER_LENGTH * 16]; // �ɼ�����BUFFER [8]
    uint8_t UDiskBuffer[BUFFER_LENGTH * 8];    // U�̽���BUFFER [3]
} UDISK_INFO;

#define ShowProblemLine()                                  \
    do {                                                   \
        char msg[200];                                     \
        memset(msg, 0, 200);                               \
        strcat(msg, "ERROR:");                             \
        strcat(msg, __FILE__);                             \
        char temp[10];                                     \
        memset(temp, 0, 10);                               \
        DecimalToAscii(temp, __LINE__, 3);                 \
        strcat(msg, temp);                                 \
        strcat(msg, __FUNCTION__);                         \
        FTextOut(10, 200, msg, strlen(msg), COLOR_YELLOW); \
        RDBuzzerOn(10);                                    \
    } while (0)

#define BCDtoDecimal(x) (((x & 0xf0) >> 4) * 10 + (x & 0x0f))

extern UDISK_INFO g_UDiskInfo;

int  EchoDataStorageFunc(int keycode, int mode);
void StorageData(int keycode);
int  FileClose(int mode);
void Write_UDisk_Len(int xpos, int ypos, int SpacePart, int Cluster, int filelen);
int  SaveToUDisk(int file_no);

#ifdef __cplusplus
}
#endif
