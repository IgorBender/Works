#include <GenException.h>

ostream& operator<< (ostream& out, const GenException& Except)
{
    if(0 == Except.getErrCode())
    {
        out << "File : " << Except.getFile() << ", Line : " << Except.getLine()
            << " : " << Except.getMessage();
    }
    else
    {
        out << "File : " << Except.getFile() << ", Line : " << Except.getLine()
            << ", Error : " << Except.getErrCode();
    }
    return out;
}
