
#define _CRT_SECURE_NO_WARNINGS

#include "CWnd.h"
#include "Tools.h"
#include "Render.h"
#include "Surface.h"
#include "Animation.h"
#undef __STRICT_ANSI__
#include <cstdio>

static  CRender *  m_render = NULL;
static  CSurface * surface1 = NULL;
static  CSurface * surface2 = NULL;
static  CAnimation * ani    = NULL;

static  int     level = 0;
static  int     step  = 8;

static const char * render(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader)
{  
        surface2->SetAlpha(level);
	
        level += step;
	if(level>256)
	{
		step = -8;
		level = 256;
	}
	if(level<0)
	{
		step = 8;
		level = 0;
	}
        return m_render->RenderScene(bmpHeader, biHeader);
}

static int control(int key, int state, int x, int y)
{
        if(state != 0)
                return 0;
        if(key == 'P')
        {
                if(ani->IsRunning())
                {
                        ani->Pause();
                }
                else
                {
                        ani->Start();
                }
        }else if(key == 'S'){
                ani->Stop();
        }else if(key == 'R'){
                ani->Reset();
        }
        return 0;

}

int main(int argc, char * argv[])
{
        CWnd *  window = new CWnd();
        window->Create(640, 480, render, control);

        Debug() << "Hello" << endl;

        m_render = new CRender(640, 480);

        surface1 = new CSurface(640, 480);
        surface2 = new CSurface(640, 480);
       
        char    file1[1024];
        char    file2[1024];
        sprintf(file1, "%s\\data\\bg1.bmp", CTools::Get()->AppPath());
        sprintf(file2, "%s\\data\\char1.bmp", CTools::Get()->AppPath());
        
        surface1->Show();
        surface2->Show();
        // surface2->MoveTo(100, 100);

        surface1->Load(CTools::Get()->ReadAsBitmap(file1));
        surface2->Load(CTools::Get()->ReadAsBitmap(file2));
        surface2->SetColorKey(true);
        surface2->SetAlpha(196);

        // surface2->SetSrcPos(100, 100);

        char    file3[1024];
        sprintf(file3, "%s\\data\\Explosion.bmp", CTools::Get()->AppPath());

        int     i;
        ani = new CAnimation();
        for(i=0; i<11; i++)
        {
                CSurface  *     f = new CSurface(67, 100);
                f->Load(CTools::Get()->ReadAsBitmap(file3));
                f->SetColorKey(true);
                f->SetSrcPos(i * 67, 0);
                ani->AddFrame(f);
                ani->FrameInterval(100);
                ani->TrackInterval(1000);
                ani->AddWayPoint(i * 64, i * 48);
        }
        ani->Start();
        window->Loop();
        return 0;
}



