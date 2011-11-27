
#pragma once

#include <cstdint>

#pragma pack(push)
#pragma pack(1)

typedef uint8_t         _BYTE;
typedef uint16_t        _WORD;
typedef uint32_t        _DWORD;
typedef int32_t         _LONG;

typedef struct
{
        _WORD    bfType;
        _DWORD   bfSize;
        _WORD    bfReserved1;
        _WORD    bfReserved2;
        _DWORD   bfOffBits;
} BMPHEADER;

typedef struct 
{
        _DWORD      biSize;
        _LONG       biWidth;
        _LONG       biHeight;
        _WORD       biPlanes;
        _WORD       biBitCount;
        _DWORD      biCompression;
        _DWORD      biSizeImage;
        _LONG       biXPelsPerMeter;
        _LONG       biYPelsPerMeter;
        _DWORD      biClrUsed;
        _DWORD      biClrImportant;
} BMPINFOHEADER;

typedef struct 
 {
        _BYTE    rgbBlue;
        _BYTE    rgbGreen;
        _BYTE    rgbRed;
        _BYTE    rgbAlpha;
}RGBQ;

#pragma pack(pop)

enum 
{
        STATE_KEYDOWN = 0,
        STATE_KEYUP,
};

enum
{
        KEY_BEFOREPAINT = -2,
        KEY_AFTERPAINT  = -1,
};

