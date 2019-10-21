#ifndef _GENEXCEPTION_H
#define _GENEXCEPTION_H

#include <iostream>
#include <string.h>
using namespace std;

#define EXCEPT_PARAMS __FILE__, __LINE__
#define THROW_GEN(x) throw(GenException(__FILE__, __LINE__, \
                     x))

class GenException { 
    public:
        GenException(const char* File, int Line, const char* Message) :
            m_Line(Line), m_ErrCode(0)
        {
            strcpy(m_File, File);
            strcpy(m_Message, Message);
        }
        GenException(const char* File, int Line, long ErrCode) :
            m_Line(Line), m_ErrCode(ErrCode)
        {
            strcpy(m_File, File);
        }
        virtual ~GenException() {}

        const char* getFile() const { return m_File; }
        const char* getMessage() const { return m_Message; }
        int getLine() const { return m_Line; }
        int getErrCode() const { return m_ErrCode; }

    protected:
        char m_File[256];
        char m_Message[256];
        int m_Line;
        long m_ErrCode;
};

ostream& operator<< (ostream& out, const GenException& Except);

#endif /* _GENEXCEPTION_H */
