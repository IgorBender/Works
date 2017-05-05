#include <ParserInterface.h>


#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

#include <Parser.h>
#include <DefTests.h>
//#include <DefTestsLex.h>
struct yy_buffer_state;
#define YY_EXTRA_TYPE ParserInterface*
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


void ParserInterface::printString(std::string s)
{
    cout << s;
}

void ParserInterface::printChar(char c)
{
    cout << c;
}

double ParserInterface::getMeasuredValue(std::string s)
{
    MeasureResultsMapType::iterator i = MeasureResultsMap.find(s);
    if(MeasureResultsMap.end() == i)
        return std::numeric_limits<double>::min();
    return MeasureResultsMap[s];
}

ParserInterface::BitResultType ParserInterface::getTestValue(std::string s)
{
    TestResultsMapType::iterator i = TestResultsMap.find(s);
    if(TestResultsMap.end() == i)
        return ParserInterface::BitResultType::BitResultNotTested;
    return TestResultsMap[s];
}

ParserInterface::BitResultType ParserInterface::andOperation(ParserInterface::BitResultType a, ParserInterface::BitResultType b)
{
    if(ParserInterface::BitResultType::BitResultNotTested == a || ParserInterface::BitResultType::BitResultNotTested == b)
        return ParserInterface::BitResultType::BitResultNotTested;
    if(ParserInterface::BitResultType::BitResultFailed == a && ParserInterface::BitResultType::BitResultFailed == b)
        return ParserInterface::BitResultType::BitResultFailed;
    return ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::orOperation(ParserInterface::BitResultType a, ParserInterface::BitResultType b)
{
    if(ParserInterface::BitResultType::BitResultNotTested == a || ParserInterface::BitResultType::BitResultNotTested == b)
        return ParserInterface::BitResultType::BitResultNotTested;
    if(ParserInterface::BitResultType::BitResultFailed == a || ParserInterface::BitResultType::BitResultFailed == b)
        return ParserInterface::BitResultType::BitResultFailed;
    return ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::notOperation(ParserInterface::BitResultType a)
{
    if(ParserInterface::BitResultType::BitResultNotTested == a)
        return ParserInterface::BitResultType::BitResultNotTested;
    if(ParserInterface::BitResultType::BitResultOk == a)
        return ParserInterface::BitResultType::BitResultFailed;
    return ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::lessOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 < Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::moreOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 > Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::lessEqualOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 <= Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::moreEqualOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 >= Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::equalOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 == Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

ParserInterface::BitResultType ParserInterface::notEqualOperation(double Value1, double Value2)
{
    if(std::numeric_limits<double>::min() == Value1 || std::numeric_limits<double>::min() == Value2)
    {
        return ParserInterface::BitResultType::BitResultNotTested;
    }
    return Value1 != Value2 ? ParserInterface::BitResultType::BitResultFailed : ParserInterface::BitResultType::BitResultOk;
}

void ParserInterface::addTestVariable(std::string s)
{
    TestResultsMap[s] = BitResultType::BitResultNotTested;
}

void ParserInterface::addMeasureVariable(std::string s)
{
    MeasureResultsMap[s] = std::numeric_limits<double>::min();
}

bool ParserInterface::setTestResult(std::string Name, BitResultType Result)
{
    TestResultsMapType::iterator i = TestResultsMap.find(Name);
    if(TestResultsMap.end() == i)
        return false;
    TestResultsMap[Name] = Result;
    return true;
}

bool ParserInterface::setMesuredResult(std::string Name, double Result)
{
    MeasureResultsMapType::iterator i = MeasureResultsMap.find(Name);
    if(MeasureResultsMap.end() == i)
        return false;
    MeasureResultsMap[Name] = Result;
    return true;
}

bool ParserInterface::parseResultString(std::string s, ParserInterface* Parser)
{
    void* pp = NULL;
    bitlex_init_extra(Parser, &pp);
    yy_buffer_state* p = bit_scan_string(const_cast<char*>(s.c_str()), pp);
    int Res = bitparse(pp);
    bit_delete_buffer(p, pp);
    bitlex_destroy(pp);
    return (0 == Res) ? true : false;
}

bool ParserInterface::parseDefinitionsString(std::string s, ParserInterface* Parser)
{
    void* pp = NULL;
    deflex_init_extra(Parser, &pp);
    yy_buffer_state* p = def_scan_string(const_cast<const char*>(s.c_str()), pp);
    int Res = defparse(pp);
    def_delete_buffer(p, pp);
    deflex_destroy(pp);
    return (0 == Res) ? true : false;
}

std::ostream& operator<<(std::ostream& out, ParserInterface::BitResultType BitResult)
{
    switch(BitResult)
    {
    case ParserInterface::BitResultType::BitResultNotTested:
        out << "NOT TESTED";
        break;

    case ParserInterface::BitResultType::BitResultOk:
        out << "OK";
        break;

    case ParserInterface::BitResultType::BitResultFailed:
        out << "Failed";
        break;

    default:
        out << "Unknown";
    }
    return out;
}
