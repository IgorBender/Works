#include "IpAddressValidator.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


IpAddressValidator::IpAddressValidator()
{

}

IpAddressValidator::State IpAddressValidator::validate(QString & input, int & pos) const
{
    if(7 > pos)
    {
        return Intermediate;
    }
    std::string AddrString = input.toStdString();
    if(3 != std::count(AddrString.begin(), AddrString.end(), '.'))
    {
        return Intermediate;
    }
    if(INADDR_NONE == inet_addr(AddrString.c_str()))
    {
        return Intermediate;
    }
    return Acceptable;
}
