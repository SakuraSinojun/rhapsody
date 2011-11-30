
#include "Surface.h"
#include "type.h"
#include "Tools.h"
#include "Render.h"
#include <algorithm>
#include <cstring>


CSurface::CSurface():
        nWidth(640),
        nHeight(480),
        x(0),
        y(0),
        imgbuffer(NULL),
        render(NULL),
        bShow(false),
        nAlpha(-1),
        bColorKey(false),
        uKeyColor(0),
        bUseFirstColor(false),
        nSrcLeft(0),
        nSrcTop(0),
        ox(0),
        oy(0)
{
        CRender *       lastRender = CRender::GetLastRender();
        if(lastRender != NULL)
                this->AttachTo(lastRender);
}

CSurface::CSurface(int width, int height):
        nWidth(width),
        nHeight(height),
        x(0),
        y(0),
        imgbuffer(NULL),
        render(NULL),
        bShow(false),
        nAlpha(-1),
        bColorKey(false),
        uKeyColor(0),
        bUseFirstColor(false),
        nSrcLeft(0),
        nSrcTop(0)
{
        CRender *       lastRender = CRender::GetLastRender();
        if(lastRender != NULL)
                this->AttachTo(lastRender);
}

CSurface::~CSurface()
{
        if(imgbuffer != NULL)
                delete [] imgbuffer;
}
      
int     CSurface::AttachTo(CRender* render)
{
        if(render == NULL)
                return 0;
        render->Attach(this);
        this->render = render;
        return 0;
}

int     CSurface::Detach(void)
{
        if(this->render == NULL)
                return 0;
        this->render->Detach(this);
        return 0;
}

int     CSurface::Load(const unsigned char * imgbuf)
{
        int     width, height;
        if(imgbuf == NULL)
        {
                CTools::Get()->Debug("Null Buffer!\n");
                this->CreateEmptyBitmap();
                return -1;
        }

        if(imgbuffer != NULL)
        {
                delete imgbuffer;
                imgbuffer = NULL;
        }

        // 取位图头
        const unsigned char *    p;
        memcpy(&bmpHeader, imgbuf, sizeof(BMPHEADER));
        p = imgbuf + sizeof(BMPHEADER);
        memcpy(&biHeader, p, sizeof(BMPINFOHEADER));
        p += biHeader.biSize;

        width = biHeader.biWidth;
        height = (biHeader.biHeight > 0) ? biHeader.biHeight : ( - biHeader.biHeight);

        biHeader.biSizeImage = width * height * biHeader.biBitCount / 8;

        if(biHeader.biWidth < this->nWidth)
                this->nWidth = biHeader.biWidth;
        if(height < this->nHeight)
                this->nHeight = height;
        nSrcLeft = 0;
        nSrcTop  = 0;
        imgbuffer = new unsigned char[width * height * 4];

        // 转化为32位位图
        int             nDepth = biHeader.biBitCount;
        int             i, j, l;
        int             bytes_per_line;

        switch(nDepth)
        {
        case 24:
                if(biHeader.biHeight > 0)
                {
                        bytes_per_line = ((width * 3 + 3) / 4) * 4;
                        for(j=0; j<biHeader.biHeight; j++)
                        {
                                for(i=0, l=0; i<width * 4; i+=4, l+=3)
                                {
                                        imgbuffer[j * width * 4 + i + 0] = p[j * bytes_per_line + l + 0];
                                        imgbuffer[j * width * 4 + i + 1] = p[j * bytes_per_line + l + 1];
                                        imgbuffer[j * width * 4 + i + 2] = p[j * bytes_per_line + l + 2];
                                        imgbuffer[j * width * 4 + i + 3] = 0;
                                }
                        }
                }else{
                        bytes_per_line = ((width * 3 + 3) / 4) * 4;
                        for(j=0; j<height; j++)
                        {
                                for(i=0, l=0; i<width * 4; i+=4, l+=3)
                                {
                                        imgbuffer[j * width * 4 + i + 0] = p[(height - j - 1) * bytes_per_line + l + 0];
                                        imgbuffer[j * width * 4 + i + 1] = p[(height - j - 1) * bytes_per_line + l + 1];
                                        imgbuffer[j * width * 4 + i + 2] = p[(height - j - 1) * bytes_per_line + l + 2];
                                        imgbuffer[j * width * 4 + i + 3] = 0;
                                }
                        }
                }
                memset(&bmpHeader, 0, sizeof(BMPHEADER));
	        memset(&biHeader, 0, sizeof(BMPINFOHEADER));
	        bmpHeader.bfType 	= 0x4d42;               // "BM"
	        bmpHeader.bfReserved1 	= 0;
	        bmpHeader.bfReserved2	= 0;
	        bmpHeader.bfSize 	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER) + width * height * 4;
	        bmpHeader.bfOffBits	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
	        biHeader.biSize		= sizeof(BMPINFOHEADER);
	        biHeader.biWidth	= width;
	        biHeader.biHeight	= height;
	        biHeader.biPlanes	= 0;
	        biHeader.biBitCount	= 32;
	        biHeader.biCompression	= 0;
	        biHeader.biSizeImage	= width * height * 4;
                break;
        case 32:
                memcpy((void *)imgbuffer, (void *)p, biHeader.biSizeImage);
                break;
        default:
                CTools::Get()->Debug("Bitmap Not Support. depth:%d\n", nDepth);
                throw "Bitmap Not Support!";
                return -1;
                break;
        }

        return 0;
}

int     CSurface::SetColorKey(bool bKey, bool bUseFirstColor, unsigned int uKeyColor)
{
        this->bColorKey = bKey;
        this->bUseFirstColor = bUseFirstColor;
        this->uKeyColor = uKeyColor;
        return 0;
}

int     CSurface::Show(bool bShow)
{
        this->bShow = bShow;
        return 0;
}

int     CSurface::SetAlpha(int Alpha)
{
        if(Alpha < -1)
                Alpha = -1;
        if(Alpha > 256)
                Alpha = 256;
        this->nAlpha = Alpha;
        return 0;
}

int     CSurface::SetSrcPos(int left, int top)
{
        if(biHeader.biWidth - left < nWidth)
                left = biHeader.biWidth - nWidth;
        if(biHeader.biHeight - top < nHeight)
                top = biHeader.biHeight - nHeight;
        if(left < 0)
                left = 0;
        if(top < 0)
                top = 0;

        this->nSrcLeft = left;
        this->nSrcTop = top;
        return 0;
}

int     CSurface::MoveTo(int x, int y)
{
        this->ox = x;
        this->oy = y;
        ResetPos();
        return 0;
}

int     CSurface::SetDrawPos(int dx, int dy)
{
        this->x = dx;
        this->y = dy;
        return 0;
}

int     CSurface::ResetPos(void)
{
        this->x = this->ox;
        this->y = this->oy;
        return 0;
}

int     CSurface::Clear(void)
{
        memset(imgbuffer, 0, nWidth * nHeight * 4);
        return 0;
}

int     CSurface::CreateEmptyBitmap(void)
{
        if(imgbuffer != NULL)
                delete [] imgbuffer;
        this->imgbuffer = new unsigned char [nWidth * nHeight * 4];
        Clear();

        memset(&bmpHeader, 0, sizeof(BMPHEADER));
	memset(&biHeader, 0, sizeof(BMPINFOHEADER));
	bmpHeader.bfType 	= 0x4d42;               // "BM"
	bmpHeader.bfReserved1 	= 0;
	bmpHeader.bfReserved2	= 0;
	bmpHeader.bfSize 	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER) + nWidth * nHeight * 4;
	bmpHeader.bfOffBits	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
	biHeader.biSize		= sizeof(BMPINFOHEADER);
	biHeader.biWidth	= nWidth;
	biHeader.biHeight	= nHeight;
	biHeader.biPlanes	= 0;
	biHeader.biBitCount	= 32;
	biHeader.biCompression	= 0;
	biHeader.biSizeImage	= nWidth * nHeight * 4;
        
        return 0;
}

int     CSurface::Blt(CSurface * surface)
{
        unsigned int    tcolor, color;
        int             x0, y0;
        int             left, top, right, bottom;
        unsigned char * p0;
        unsigned char * p1;

        if(!surface->bShow)
        {
                return 0;
        }

        // 先求相交矩形
        if(this->x + this->nWidth < surface->x 
           ||   this->x > surface->x + surface->nWidth
           ||   this->y + this->nHeight < surface->y
           ||   this->y > surface->y + surface->nHeight
        )
        {
                return 0;
        }

        left = std::max<int>(this->x, surface->x);
        right = std::min<int>(this->x + this->nWidth, surface->x + surface->nWidth);
        top = std::max<int>(this->y, surface->y);
        bottom = std::min<int>(this->y + this->nHeight, surface->y + surface->nHeight);

        
        if(surface->bColorKey)
        {
                if(surface->bUseFirstColor)
                        tcolor = *(unsigned __int32 *)surface->imgbuffer & 0x00ffffff;
                else
                        tcolor = surface->uKeyColor;
                // CTools::Get()->Debug("transparent color is %#x\n", tcolor);
        }

        // 出于效率原因， 透明色与半透明的四种组合分开来做。
        if(surface->nAlpha > 0 && surface->nAlpha < 256)        // 半透明
        {
                if(surface->bColorKey)                          // 最慢的情况， 同时存在ColorKey和透明色。。。
                {
                        for(y0 = 0; y0 < bottom - top; y0 ++)
                        {
                                for(x0 = 0; x0 < right - left; x0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                        color = *(unsigned int *)p1 & 0x00FFFFFF;
                                        if(color == tcolor)
                                        {
                                                continue;
                                        }
                                        p0[0] = p0[0] + (p1[0] - p0[0]) * surface->nAlpha / 256;
                                        p0[1] = p0[1] + (p1[1] - p0[1]) * surface->nAlpha / 256;
                                        p0[2] = p0[2] + (p1[2] - p0[2]) * surface->nAlpha / 256;
                                }
                        }
                }
                else                                            // 可以考虑用MMX来做。
                {
                        for(y0 = 0; y0 < bottom - top; y0 ++)
                        {
                                for(x0 = 0; x0 < right - left; x0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                        p0[0] = p0[0] + (p1[0] - p0[0]) * surface->nAlpha / 256;
                                        p0[1] = p0[1] + (p1[1] - p0[1]) * surface->nAlpha / 256;
                                        p0[2] = p0[2] + (p1[2] - p0[2]) * surface->nAlpha / 256;
                                }
                        }
                }
        }
        else                                                    // 不半透明
        {
                if(surface->nAlpha == 0)                        // 显示背景
                {
                        return 0;
                }
                else                                            // 显示前景
                {
                        if(surface->bColorKey)                  // 存在透明色
                        {
                                for(y0 = 0; y0 < bottom - top; y0 ++)
                                {
                                        for(x0 = 0; x0 < right - left; x0 ++)
                                        {
                                                p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                                p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                                color = *(unsigned int *)p1 & 0x00FFFFFF;
                                                if(color == tcolor)
                                                {
                                                        continue;
                                                }
                                                p0[0] = p1[0];
                                                p0[1] = p1[1];
                                                p0[2] = p1[2];
                                        }
                                }

                        }
                        else                                    // 简单位图复制 
                        {
                                for(y0 = 0; y0 < bottom - top; y0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (left - surface->x + surface->nSrcLeft) * 4;
                                        memcpy(p0, p1, (right-left) * 4);
                                }
                        }
                }
        }

        return 0;
}

int     CSurface::Blt(CSurface * surface, int x, int y)
{
        int     ox, oy;
        ox = this->x;
        oy = this->y;
        this->MoveTo(x, y);
        this->Blt(surface);
        this->MoveTo(ox, oy);
        return 0;
}

