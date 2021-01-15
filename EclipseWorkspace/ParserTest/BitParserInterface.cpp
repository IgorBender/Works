#include <BitParserInterface.h>


#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

#include <Parser.h>
#include <DefTests.h>
//#include <DefTestsLex.h>
struct yy_buffer_state;
#define YY_EXTRA_TYPE BitParserInterface*
//extern struct yy_buffer_state*  bit__scan_string(char * str);
//extern void bit__delete_buffer(yy_buffer_state* buffer);
extern struct yy_buffer_state*  def_scan_string(const char * str, yyscan_t ptr_yy_globals);
extern void def_delete_buffer(yy_buffer_state* buffer, yyscan_t ptr_yy_globals);
//extern int deflex_init(yyscan_t* ptr_yy_globals);
extern int deflex_init_extra(YY_EXTRA_TYPE user_defined,yyscan_t* scanner);
extern int deflex_destroy(yyscan_t yyscanner);
extern struct yy_buffer_state*  bit_scan_string(const char * str, yyscan_t ptr_yy_globals);
extern void bit_delete_buffer(yy_buffer_state* buffer, yyscan_t ptr_yy_globals);
//extern int bitlex_init(yyscan_t* ptr_yy_globals);
extern int bitlex_init_extra(YY_EXTRA_TYPE user_defined,yyscan_t* scanner);
extern int bitlex_destroy(yyscan_t yyscanner);


//void BitParserInterface::printString(std::string s)
//{
//    cout << s;
//}

//void BitParserInterface::printChar(char c)
//{
//    cout << c;
//}

double BitParserInterface::getMeasuredValue(std::string s)
{
    MeasureResultsMapType::iterator i = m_MeasureResultsMap.find(s);
    if(m_MeasureResultsMap.end() == i)
        return std::numeric_limits<double>::min();
    return m_MeasureResultsMap[s];
}

BitParserInterface::BitResultType BitParserInterface::getTestValue(std::string s)
{
    TestResultsMapType::iterator i = m_TestResultsMap.find(s);
    if(m_TestResultsMap.end() == i)
        return BitParserInterface::BitResultType::BitResultNotTested;
    return m_TestResultsMap[s];
}

BitParserInterface::BitResultType BitParserInterface::andOperation(BitParserInterface::BitResultType a, BitParserInterface::BitResultType b)
{
    if(BitParserInterface::BitResultType::BitResultNotTested == a || BitParserInterface::BitResultType::BitResultNotTested == b)
        return BitParserInterface::BitResultType::BitResultNotTested;
    if(BitParserInterface::BitResultType::BitResultFailed == a && BitParserInterface::BitResultType::BitResultFailed == b)
        return BitParserInterface::BitResultType::BitResultFailed;
    return BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::orOperation(BitParserInterface::BitResultType a, BitParserInterface::BitResultType b)
{
    if(BitParserInterface::BitResultType::BitResultNotTested == a || BitParserInterface::BitResultType::BitResultNotTested == b)
        return BitParserInterface::BitResultType::BitResultNotTested;
    if(BitParserInterface::BitResultType::BitResultFailed == a || BitParserInterface::BitResultType::BitResultFailed == b)
        return BitParserInterface::BitResultType::BitResultFailed;
    return BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::notOperation(BitParserInterface::BitResultType a)
{
    if(BitParserInterface::BitResultType::BitResultNotTested == a)
        return BitParserInterface::BitResultType::BitResultNotTested;
    if(BitParserInterface::BitResultType::BitResultOk == a)
        return BitParserInterface::BitResultType::BitResultFailed;
    return BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::lessOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 < Value2 ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::moreOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 > Value2 ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::lessEqualOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 <= Value2 ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::moreEqualOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 >= Value2 ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::equalOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return std::abs(Value1 - Value2) < std::numeric_limits<double>::epsilon() ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

BitParserInterface::BitResultType BitParserInterface::notEqualOperation(double Value1, double Value2)
{
    if(std::abs(Value1 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon() ||
            std::abs(Value2 - std::numeric_limits<double>::min()) <  std::numeric_limits<double>::epsilon())
    {
        return BitParserInterface::BitResultType::BitResultNotTested;
    }
    return std::abs(Value1 - Value2) > std::numeric_limits<double>::epsilon() ? BitParserInterface::BitResultType::BitResultFailed : BitParserInterface::BitResultType::BitResultOk;
}

void BitParserInterface::addTestVariable(std::string s)
{
    m_TestResultsMap[s] = BitResultType::BitResultNotTested;
}

void BitParserInterface::addMeasureVariable(std::string s)
{
    m_MeasureResultsMap[s] = std::numeric_limits<double>::min();
}

bool BitParserInterface::setTestResult(std::string Name, BitResultType Result)
{
    TestResultsMapType::iterator i = m_TestResultsMap.find(Name);
    if(m_TestResultsMap.end() == i)
        return false;
    m_TestResultsMap[Name] = Result;
    return true;
}

bool BitParserInterface::setMesuredResult(std::string Name, double Result)
{
    MeasureResultsMapType::iterator i = m_MeasureResultsMap.find(Name);
    if(m_MeasureResultsMap.end() == i)
        return false;
    m_MeasureResultsMap[Name] = Result;
    return true;
}

bool BitParserInterface::parseResultString(std::string s, BitParserInterface* Parser)
{
    void* pp = nullptr;
    bitlex_init_extra(Parser, &pp);
    yy_buffer_state* p = bit_scan_string(const_cast<char*>(s.c_str()), pp);
    int Res = bitparse(pp);
    bit_delete_buffer(p, pp);
    bitlex_destroy(pp);
    return (0 == Res) ? true : false;
}

bool BitParserInterface::parseDefinitionsString(std::string s, BitParserInterface* Parser)
{
    void* pp = nullptr;
    deflex_init_extra(Parser, &pp);
    yy_buffer_state* p = def_scan_string(const_cast<const char*>(s.c_str()), pp);
    int Res = defparse(pp);
    def_delete_buffer(p, pp);
    deflex_destroy(pp);
    return (0 == Res) ? true : false;
}

bool BitParserInterface::performSetComand(std::string SetName, std::string Value)
{
    // Chack if set ID is defined
    // Future check
    if("id_1" != SetName)
        return false;
    stringstream Convert;
    Convert << Value;
    uint32_t IntVal = 0;
    Convert >> IntVal;
    if(!Convert)
        return false;
    // Perform real set operation here
    // For now print only
    cout << "Set " << SetName << " to " << IntVal << endl;
    return true;
}

bool BitParserInterface::performSleepComand(std::string Value)
{
    stringstream Convert;
    Convert << Value;
    uint32_t IntVal = 0;
    Convert >> IntVal;
    if(!Convert)
        return false;
    cout << "Perform sleep for " << IntVal << " milliseconds" << endl;
    timespec Time;
    Time.tv_sec = IntVal / 1000;
    Time.tv_nsec = (IntVal % 1000) * 1000 * 1000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &Time, nullptr);
    return true;
}

std::ostream& operator<<(std::ostream& out, BitParserInterface::BitResultType BitResult)
{
    switch(BitResult)
    {
    case BitParserInterface::BitResultType::BitResultNotTested:
        out << "NOT TESTED";
        break;

    case BitParserInterface::BitResultType::BitResultOk:
        out << "OK";
        break;

    case BitParserInterface::BitResultType::BitResultFailed:
        out << "Failed";
        break;

//    default:
//        out << "Unknown";
    }
    return out;
}

