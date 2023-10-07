
#include "M10Era.h"
#include "sdcard.h"

#define USB_HOST_MODE  0 // Set USB Controller to Host mode
#define USB_SLAVE_MODE 1 // Set USB Controller to Slave mode

static char *cTips[] = {
    /*0*/ "USB Host Mode",
    /*1*/ "Waiting for  USB connect signal",
    /*2*/ "Recv Data...[IN]",
    /*3*/ "Send Data...[OUT]",
    /*4*/ "Usb device connected successfully!",
    /*5*/ "Press any key to Send Channel Status...[Out]",
    /*6*/ "Freq[Hz]: ",
    /*7*/ "Dirctory Content",
    /*8*/ "Press any key to Receive Freq...[Out]",
    /*9*/ "Bulk OUT ERROR",
    /*10*/ "Bulk IN ERROR",

};
static int Y_POS[] = {
    0,
    0x08,
    0x10,
    0x18,
    0x20,
    0x28,
    0x30,
    0x38,
};

void RDDelay(uint32_t time) {
    uint32_t time1, time2;
    time1 = GetElapsedTime();
    while (1) {
        time2 = GetElapsedTime();
        if (time2 - time1 >= time)
            break;
    }
}

void FTextOut(int x, int y, char *pzText, int iSectionLen, int color) {
    EraseWindow(x, y, SCREEN_WIDTH - 1, y + 7);
    if (iSectionLen > 40)
        iSectionLen = 40;
    SetDisplayColor(color);
    pzText[iSectionLen] = '\0';
    ETextOut(x, y, pzText, iSectionLen);
    return;
}

void HexTextOut(int x, int y, char *pzText, int iSectionLen, int color1, int color2) {
    char outText[160 + 1];
    int  i;
    char CHAR_TABLE[16] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                           0x41, 0x42, 0x43, 0x44, 0x45, 0x46};

    EraseWindow(x, y, SCREEN_WIDTH - 1, y + 7);

    //
    // 当前只支持40个字符
    //
    if (iSectionLen > 40)
        iSectionLen = 40;

    outText[iSectionLen * 2 + 1] = '\0';
    //
    // Decimal to Hex
    //
    for (i = 0; i < iSectionLen; i++) {
        outText[2 * i]     = ((uint8_t)pzText[i] / 16);
        outText[2 * i + 1] = ((uint8_t)pzText[i] % 16);
    }

    for (i = 0; i < iSectionLen * 2; i++)
        outText[i] = CHAR_TABLE[(int)outText[i]];
    //
    // Output string
    //
    for (i = 0; i < iSectionLen; i++) {
        // SetDisplayColor((i%2)==0?COLOR_RED:COLOR_CYAN);
        SetDisplayColor((i % 2) == 0 ? color1 : color2);
        ETextOut(x + 16 * i, y, outText + i * 2, 2);
    }

    return;
}

void FormatPrint(int x, int y, int value, int iSectionLen, int mode) {
    return;
}

int GetCh() {
    int key_index;
    for (;;) {
        key_index = GetKeyCode() & 0xff;
        if (key_index < 0)
            break;
    }
    return abs(key_index);
}

int RDDelayFixedTime(uint32_t delay_second) {
#define KEY_EXIT 12 // “返回”

    int iRtn = 0;

    uint32_t iKeyPress = 0;
    //	char cBuf[10];

    uint32_t elapsedtime = GetElapsedTime() + 1000 * delay_second;

    while ((GetElapsedTime() < elapsedtime) || (iKeyPress != 0)) // 5sec 后或有按键退出，进入下一道流程
    {
        if (iKeyPress == 0) {
            if (RDKbhit() != 0)
                iKeyPress++;
        } else {
            if (RDKbhit() == KEY_EXIT) // 按下了返回键
                break;
        }
    }

    return iRtn;
}

int RDKbhit() {
#define KEY_PRESS_TIME 2

    int iKeyCode = GetKeyCode() & 0xff;
    if (iKeyCode >= 0)
        return 0;
    iKeyCode = abs(iKeyCode);
    RDBuzzerOn(KEY_PRESS_TIME);
    return iKeyCode;
}

void RDBuzzerOn(uint32_t time) {
    SwitchBuzzerOn(1);
    RDDelay(time * 100);
    SwitchBuzzerOn(0);
}

void DebugIntData(int x, int y, char *cTips, int ii) {
    char cBuf[40 + 1], cText[40 + 1];

    int iLen;
    memset(cBuf, 0, 41);
    if (cTips == NULL)
        iLen = 0;
    else {
        iLen = strlen(cTips);
        if (iLen > 40 - 8)
            iLen = 40 - 8;
        memcpy(cBuf, cTips, iLen);
    }

    DecimalToAscii(cText, ii, 0x01010008);
    strcat(cBuf, cText);
    EraseWindow(x, y, x + strlen(cBuf) * 8, y + 7);
    ETextOut(x, y, cBuf, strlen(cBuf));
}
