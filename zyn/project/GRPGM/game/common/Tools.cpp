
#define _CRT_SECURE_NO_WARNINGS
#include "Tools.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;


#include <Windows.h>


CTools * CTools::m_instance = NULL;

CTools::CTools()
{
        char            temp[1024];
        GetModuleFileName(GetModuleHandleA(NULL), temp, 1024);
        *(strrchr(temp, '\\')) = '\0';
        apppath = new char[strlen(temp) + 2];
        strcpy(apppath, temp);
        strcat(apppath, "\\");
}

CTools::~CTools()
{
        map<string, const unsigned char *>::iterator     iter;
        for(iter = bmpMap.begin(); iter != bmpMap.end(); iter ++)
        {
                delete[] iter->second;
        }
        bmpMap.clear();
}

CTools * CTools::Get(void)
{
        if(m_instance == NULL)
        {
                m_instance = new CTools();
        }
        if(m_instance == NULL)
        {
                throw("malloc failed\n");
        }
        return m_instance;
}

const unsigned char *  CTools::ReadAsBitmap32(const char * filename)
{
        // TODO...
        map<string, const unsigned char *>::iterator     iter;
        iter = bmpMap.find(filename);
        if(iter != bmpMap.end())
                return iter->second;

        unsigned char * buffer = NULL;
        streamoff       len;
        std::ifstream   file(filename, ios::in | ios::binary);
        if(file.fail())
        {
                Debug("No Such File! %s\n", filename);
                return NULL;
        }
        file.seekg(0, ios::end);
        len = file.tellg();
        file.seekg(0, ios::beg);
        buffer = new unsigned char[static_cast<unsigned int>(len)];
        file.read((char *)buffer, len);
        file.close();
        Debug("read %s\n", filename);
        bmpMap.insert(map<string, const unsigned char *>::value_type(filename, buffer));
        return buffer;
}

const unsigned char *  CTools::ReadAsBitmap24(const char * filename)
{
        return ReadAsBitmap32(filename);
}

void CTools::DebugPRT(const char * file, int line, const char * str, ...)
{
        va_list arglist;
        if(str == NULL || file == NULL)
        {
                throw "Debug! NULL string.\n";
                return;
        }
        const char *    p = strrchr(file, '\\');
        if(p == NULL)
        {
                p = file;
        }else{
                p ++;
        }
        time_t  t = time(NULL);
        struct  tm *    ctm = localtime(&t);
        
        printf("[%02d:%02d:%02d] [%s:%05d] ", ctm->tm_hour, ctm->tm_min,
                ctm->tm_sec, p, line);
        va_start(arglist, str);
        vfprintf(stdout, str, arglist);
        va_end(arglist);
        
}

const char * CTools::AppPath(void)
{
        return apppath;
}

double CTools::CurrentTime(void)
{
        LARGE_INTEGER   litmp;
        LONGLONG        qt;
        double          dft;
        double          dff;
        
        QueryPerformanceFrequency(&litmp);
        dff = (double)litmp.QuadPart;

        QueryPerformanceCounter(&litmp);
        qt = litmp.QuadPart;

        dft = qt / dff;
        return dft;
}

#undef Debug
void    CTools::Debug(const char * fmt, ...)
{
        va_list arglist;
        va_start(arglist, fmt);
        vfprintf(stdout, fmt, arglist);
        va_end(arglist);
}

