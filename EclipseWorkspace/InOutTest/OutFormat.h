/*
 *  OutFormat.h
 *
 *  Created on: 2018-01-01
 *  Author: Igor Bender
 *
 */
#ifndef OUTFORMAT_H
#define OUTFORMAT_H

#include <string>

namespace ColorForground
{
    const std::string Default("\033[39m");
    const std::string Black("\033[30m");
    const std::string Red("\033[31m");
    const std::string Green("\033[32m");
    const std::string Yellow("\033[33m");
    const std::string Blue("\033[34m");
    const std::string Magenta("\033[35m");
    const std::string Cyan("\033[36m");
    const std::string LightGray("\033[37m");
    const std::string DarkGray("\033[90m");
    const std::string LightRed("\033[91m");
    const std::string LightGreen("\033[92m");
    const std::string LightYellow("\033[93m");
    const std::string LightBlue("\033[94m");
    const std::string LightMagenta("\033[95m");
    const std::string LightCyan("\033[96m");
    const std::string White("\033[97m");
}

namespace ColorBackground
{
    const std::string Default("\033[49m");
    const std::string Black("\033[40m");
    const std::string Red("\033[41m");
    const std::string Green("\033[42m");
    const std::string Yellow("\033[43m");
    const std::string Blue("\033[44m");
    const std::string Magenta("\033[45m");
    const std::string Cyan("\033[46m");
    const std::string LightGray("\033[47m");
    const std::string DarkGray("\033[100m");
    const std::string LightRed("\033[101m");
    const std::string LightGreen("\033[102m");
    const std::string LightYellow("\033[103m");
    const std::string LightBlue("\033[104m");
    const std::string LightMagenta("\033[105m");
    const std::string LightCyan("\033[106m");
    const std::string White("\033[107m");
}

namespace BoldFont
{
    const std::string Default = "\033[0m";
    const std::string Bold = "\033[1m";
}

#endif // OUTFORMAT_H
