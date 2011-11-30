
#include "Render.h"
#include "Surface.h"
#include "Tools.h"
#include "Animation.h"
#include <algorithm>
#include <cstring>
using namespace std;

CRender *       CRender::lastRender = NULL;

CRender::CRender(int width, int height):
        nWidth(width),
        nHeight(height)
{
        this->mainSurface = new CSurface(nWidth, nHeight);
        this->mainSurface->CreateEmptyBitmap();
        lastRender = this;
}

CRender::CRender():
        nWidth(640),
        nHeight(480)
{
        this->mainSurface = new CSurface(nWidth, nHeight);
        this->mainSurface->CreateEmptyBitmap();
        lastRender = this;
}

CRender::~CRender()
{
        list<CSurface *>::iterator      iter;
        CSurface        *               surface;
        for(iter = lsSurface.begin(); iter != lsSurface.end(); iter++)
        {
                surface = *iter;
                delete surface;
        }
        lsSurface.clear();

}
        
void    CRender::Attach(CSurface * surface)
{
        lsSurface.push_back(surface);
}

void    CRender::Detach(CSurface * surface)
{
        list<CSurface *>::iterator      iter;
        iter = find(lsSurface.begin(), lsSurface.end(), surface);
        if(iter == lsSurface.end())
        {
                Debug() << "No Such Surface!" << endl;
                //throw("No Such Surface!");
                return;
        }
        lsSurface.erase(iter);
}

void    CRender::Attach(CAnimation *     ani)
{
        lsAnimation.push_back(ani);
}

void    CRender::Detach(CAnimation *     ani)
{
        list<CAnimation *>::iterator      iter;
        iter = find(lsAnimation.begin(), lsAnimation.end(), ani);
        if(iter == lsAnimation.end())
        {
                Debug() << "No Such Animation!" << endl;
                //throw("No Such Surface!");
                return;
        }
        lsAnimation.erase(iter);
}

const char *    CRender::RenderScene(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader)
{
        list<CSurface *>::iterator      iter;
        CSurface *                      surface;

        list<CAnimation *>::iterator    ita;
        CAnimation *                    ani;
        for(ita = lsAnimation.begin(); ita != lsAnimation.end(); ita++)
        {
                ani = *ita;
                ani->RenderScene();
        }

        for(iter = lsSurface.begin(); iter != lsSurface.end(); iter++)
        {
                surface = *iter;
                mainSurface->Blt(surface);
        }
        memcpy(&bmpHeader, &mainSurface->bmpHeader, sizeof(BMPHEADER));
        memcpy(&biHeader, &mainSurface->biHeader, sizeof(BMPINFOHEADER));
        return (const char *)mainSurface->imgbuffer;
}

CRender *       CRender::GetLastRender(void)
{
        return lastRender;
}

