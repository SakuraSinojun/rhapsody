
#pragma once

class CException
{
public:
        CException(int e);
        CException(char * e);
private:
        int     eInt;
        char *  eStr;
};

