
#pragma once

#include "type.h"

class CRender;
class CSurface
{
        friend  class CRender;
public:
        CSurface();
        CSurface(int width, int height);
        virtual ~CSurface();
        
        int     AttachTo(CRender* render);
        int     Detach(void);
        int     Load(const unsigned char * imgbuf);
        int     SetColorKey(bool bKey, bool bUseFirstColor = true, unsigned int uKeyColor = 0x00000000);
        int     Show(bool bShow = true);
        int     SetAlpha(int    Alpha = -1);    // 0~256
        int     SetSrcPos(int left, int top);
        int     MoveTo(int x, int y);
        int     SetDrawPos(int x, int y);
        int     X(){return ox;}
        int     Y(){return oy;}
        int     DX(){return x;}
        int     DY(){return y;}
        int     ResetPos(void);
        int     Blt(CSurface * surface);
        int     Blt(CSurface * surface, int x, int y);
        int     Clear(void);
        int     CreateEmptyBitmap(void);

protected:
        int                     nWidth, nHeight;
        int                     nSrcLeft, nSrcTop;
        bool                    bColorKey, bUseFirstColor;
        unsigned int            uKeyColor;
        int                     nAlpha;
        int                     bShow;
        unsigned char *         imgbuffer;
        BMPHEADER               bmpHeader;
        BMPINFOHEADER           biHeader;
        CRender *               render;
        int                     x, y, ox, oy;
};
