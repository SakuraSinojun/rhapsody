


#pragma once

#include <map>
#include <string>
using namespace std;

class CTools
{
public:
        ~CTools();
        static CTools *         Get(void);
        const unsigned char *   ReadAsBitmap32(const char * filename);
        const unsigned char *   ReadAsBitmap24(const char * filename);
#define ReadAsBitmap            ReadAsBitmap32

        void                    DebugPRT(const char * file, int line, const char * str, ...);
        const char *            AppPath(void);

        double                  CurrentTime(void);

public:
        void                    Debug(const char * fmt, ...);
#define Debug(fmt, ...)         DebugPRT(__FILE__, __LINE__, fmt, __VA_ARGS__)

private:
        CTools();
        
        static CTools *                         m_instance;
        map<string, const unsigned char *>      bmpMap;
        char *                                  apppath;

};

