#ifndef PARSERINTERFACE_H
#define PARSERINTERFACE_H

#include <string>
#include <map>
#include <stdint.h>
#include <sstream>
#include <iostream>

class ParserInterface
{
public:
    // Old style enumeration for compatibility with pre-C++0x compilers
//    enum BitResultType
//    {
//        BitResultNotTested = -1,
//        BitResultOk = 0,
//        BitResultFailed = 1
//    };
    enum class BitResultType
    {
        BitResultNotTested = -1,
        BitResultOk = 0,
        BitResultFailed = 1
    };

    ParserInterface() : m_Result(BitResultType::BitResultNotTested) {}
    ~ParserInterface() {}

    void printString(std::string s);
    void printChar(char c);
    double  getMeasuredValue(std::string s);
    BitResultType getTestValue(std::string s);
    BitResultType andOperation(BitResultType a, BitResultType b);
    BitResultType orOperation(BitResultType a, BitResultType b);
    BitResultType lessOperation(double Value1, double Value2);
    BitResultType moreOperation(double Value1, double Value2);
    BitResultType lessEqualOperation(double Value1, double Value2);
    BitResultType moreEqualOperation(double Value1, double Value2);
    BitResultType equalOperation(double Value1, double Value2);
    BitResultType notEqualOperation(double Value1, double Value2);
    BitResultType notOperation(BitResultType a);
    void addTestVariable(std::string s);
    void addMeasureVariable(std::string s);
    void setResult(BitResultType r)
    {
        m_Result = r;
    }
    BitResultType getResult()
    {
        return m_Result;
    }
    bool setTestResult(std::string Name, BitResultType Result);
    bool setMesuredResult(std::string Name, double Result);
    static bool parseResultString(std::string s, ParserInterface* Parser);
    static bool parseDefinitionsString(std::string s, ParserInterface* Parser);

private:
    typedef std::map<std::string, BitResultType> TestResultsMapType;
    typedef std::map<std::string, double> MeasureResultsMapType;
    TestResultsMapType TestResultsMap;
    MeasureResultsMapType MeasureResultsMap;
    BitResultType m_Result;
};

std::ostream& operator<<(std::ostream& out, ParserInterface::BitResultType BitResult);

#endif
