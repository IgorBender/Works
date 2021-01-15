#ifndef BITPARSERINTERFACE_H
#define BITPARSERINTERFACE_H

#include <string>
#include <map>
#include <stdint.h>
#include <sstream>
#include <iostream>

class BitParserInterface
{
public:
    enum class BitResultType
    {
        BitResultNotTested = -1,
        BitResultOk = 0,
        BitResultFailed = 1
    };
    typedef std::map<std::string, BitResultType> TestResultsMapType;
    typedef std::map<std::string, double> MeasureResultsMapType;

    BitParserInterface() : m_Result(BitResultType::BitResultNotTested) {}
    ~BitParserInterface() {}

//    void printString(std::string s);
//    void printChar(char c);
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
    bool performSetComand(std::string SetName, std::string Value);
    bool performSleepComand(std::string Value);
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
    static bool parseResultString(std::string s, BitParserInterface* Parser);
    static bool parseDefinitionsString(std::string s, BitParserInterface* Parser);
    TestResultsMapType& getTests()
    {
        return m_TestResultsMap;
    }
    MeasureResultsMapType& getMeasures()
    {
        return m_MeasureResultsMap;
    }

private:
    TestResultsMapType m_TestResultsMap;
    MeasureResultsMapType m_MeasureResultsMap;
    BitResultType m_Result;
};

std::ostream& operator<<(std::ostream& out, BitParserInterface::BitResultType BitResult);

#endif
