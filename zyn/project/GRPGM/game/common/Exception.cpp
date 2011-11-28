
#include "Exception.h"


CException::CException(int e)
{
        this->eInt = e;
}

CException::CException(char * e)
{
        this->eStr = e;
}

