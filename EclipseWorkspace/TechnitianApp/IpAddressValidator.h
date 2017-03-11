#ifndef IPADDRESSVALIDATOR_H
#define IPADDRESSVALIDATOR_H

#include <QValidator>

class IpAddressValidator : public QValidator
{
public:
    IpAddressValidator();

    virtual State validate(QString & input, int & pos) const;

};

#endif // IPADDRESSVALIDATOR_H
