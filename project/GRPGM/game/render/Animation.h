

#pragma once

#include <vector>

class CSurface;
class CRender;
class CAnimation
{
public:
        CAnimation();
        ~CAnimation();

        void            AddFrame(CSurface *     surface);
        void            DelFrame(CSurface *     surface);
        void            AddWayPoint(int x, int y);
        void            DelWay(void);
        void            FrameInterval(unsigned int   dwMilliseconds);
        unsigned int    FrameInterval(void);
        void            TrackInterval(unsigned int   dwMilliseconds);
        unsigned int    TrackInterval(void);
        void            Start(void);
        void            Pause(void);
        void            Stop(void);
        void            Reset(void);
        bool            IsRunning(void);
        void            RenderScene(void);
        CSurface *      Surface(void);

        int             AttachTo(CRender *      render);
        int             Detach();
private:
        bool                            running;
        unsigned int                    dwFrameInterval;
        unsigned int                    dwTrackInterval;
        double                          dfStartTime;
        double                          dfLastWPTime;
        CRender *                       render;
        std::vector<CSurface *>         vecSurface;
        std::vector<CSurface *>::iterator       iterCurrentSurface;
        std::vector<std::pair<int, int> >       vecWP;
        std::vector<std::pair<int, int> >::iterator       iterCurrentWayPoint;
};

