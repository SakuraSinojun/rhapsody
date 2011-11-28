
#pragma once

#include <list>
#include "type.h"


class CSurface;
class CAnimation;

class CRender
{
public:
        CRender();
        CRender(int width, int height);
        ~CRender();
        
        void            Attach(CSurface * surface);
        void            Detach(CSurface * surface);
        void            Attach(CAnimation *     ani);
        void            Detach(CAnimation *     ani);
        const char *    RenderScene(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader);

        static CRender * GetLastRender(void);

private:
        int                     nWidth, nHeight;
        CSurface *              mainSurface;
        std::list<CSurface *>   lsSurface;
        std::list<CAnimation *> lsAnimation;

        static CRender *        lastRender;
};
