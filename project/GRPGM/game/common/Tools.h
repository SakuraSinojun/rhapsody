


#pragma once
#undef __STRICT_ANSI__
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
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
        ostream&                OutPRT(const char * file, int line);
public:
//        void                    Debug(const char * fmt, ...);
//#define Debug(fmt, ...)         DebugPRT(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

private:
        CTools();
        
        static CTools *                         m_instance;
        map<string, const unsigned char *>      bmpMap;
        char *                                  apppath;

};

#define Debug()   CTools::Get()->OutPRT(__FILE__, __LINE__)


template <class T>
class DumpBase
{
public:
	void dump()
	{
		char     *	p = reinterpret_cast<char *>(this);
		int		i;
                int             j;
                int             t;

                for(i=0; i<sizeof(T); i+=16)
                {
                        t = 16;
                        if(i + 16 > sizeof(T))
                                t = sizeof(T) - i;
                        for(j=0; j<t; j++)
                        {
			        printf("%02x ", p[i+j] & 0xff);
                        }
                        for(j=t; j<16; j++)
                        {
                                printf("   ");
                        }
                        printf("\t");
                        for(j=0; j<t; j++)
                        {
                                if(p[i+j] >= 36 && p[i+j] <= 126)
                                        printf("%c", p[i+j] & 0xff);
                                else
                                        printf(".");
                        }
                        printf("\n");
		}
	}
};







