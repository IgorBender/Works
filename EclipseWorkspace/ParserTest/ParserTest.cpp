/*
 *  ParserTest.cpp
 *
 *  Created on: 2017-05-01
 *  Author: Igor Bender
 *
 */

/*
 * Unit test for definitions and analysis of BIT results.
 * ParserInterface is class wrapping two definitive languages :
 *  - definition language providing rules for build in tests execution,
 *  - analysis language providing rules for build in tests results analysis.
 *  Each language is implemented as a set of two files for lexical analyzer (flex)
 *  and parser (bison).
 *
 * Definition language defines naming for tests no be executed. Tests should be of
 * two predefined types :
 *   - test predicates - ending with boolean result (OK/Failed). Name starts with 'T" or 't' with following numbers.
 *   - measurements - ending with measured value. Name starts with 'M' or 'm' with following numbers.
 * The definition language sentence is string containing test and measurement names delimited by
 * spaces, tabs or end of line symbols. The definition language sentence is input parameter of
 * ParserInterface::parseDefinitionsString method and constructs internal tests and measurements
 * data structures initialized with NOT TESTED value for test predicates and 0.0 for measurement values.
 *
 * Analysis language has same naming conventions as the definition language with addition of numerical constants
 * in form of real number (examples: 1, 2., .3, 4.5). The analysis language sentence is string containing set of
 * possible logical operations on test and measurement names delimited by spaces, tabs or end of line symbols.
 * Possible logical operations are (in order of precedence levels) :
 *  Grouping level
 *   - (...) - regular braces grouping.
 *  Comparison level (each comparison results in OK/Fail)
 *   - < - less than comparison, possible on measurements values and numeric constants only.
 *   - > - more than comparison, possible on measurements values and numeric constants only.
 *   - <= - less than or equal to comparison, possible on measurements values and numeric constants only.
 *   - >= - more than or equal to comparison, possible on measurements values and numeric constants only.
 *  Binary logical level
 *   - & - logical and, possible on test only (including comparison results).
 *   - | - logical or, possible on test only (including comparison results).
 *  Unary logical level
 *   - ~ - logical negation, possible on test only (including comparison results).
 *
 * Attempt to get not defined measure value results in 0.0.
 * Attempt to get not defined test results in NOT TESTED.
 * Any logical operation on any operand of NOT TESTED value results in NOT TESTED.
 *
 */
#include <iostream>
#include "BitParserInterface.h"

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    // Perform following tests :
    //   - test t1
    //   - test t2
    //   - test t3
    //   - measurement m1
    //   - measurement m2
    // Test results of not defined tests will be set as NOT TESTED
    // Values of not defined measurements will be set as 0.0
//    std::string Definitions = "t1 t2 t3 m1 m2";
    std::string Definitions = "sleep 2000 set id_1 2 sleep 3000 m1" ;

    BitParserInterface Parser;

    if(!BitParserInterface::parseDefinitionsString(Definitions, &Parser))
    {
        std::cout << "Wrong definitions." << std::endl;
        return 1;
    }


    BitParserInterface::TestResultsMapType& Tests = Parser.getTests();
    BitParserInterface::MeasureResultsMapType& Measures = Parser.getMeasures();
    cout << "Tests to be performed ";
    for(auto t : Tests)
        // Perform test procedure in accordance to test, for now print only.
        cout << t.first << " ";
    cout << endl;
    cout << "Measures to be measured ";
    for(auto m : Measures)
        // Perform measure procedure in accordance to measure, for now print only.
        cout << m.first << " ";
    cout << endl;

    // Set tests and measures results after performed procedures, for now set some values for test purposes.
    bool SetResult = Parser.setTestResult("t1", BitParserInterface::BitResultType::BitResultFailed);
    if(!SetResult)
        cout << R"(Wrong test type "t1")" << endl;
    SetResult = Parser.setTestResult("t2", BitParserInterface::BitResultType::BitResultFailed);
    if(!SetResult)
        cout << R"(Wrong test type "t2")" << endl;
    SetResult = Parser.setTestResult("t3", BitParserInterface::BitResultType::BitResultFailed);
    if(!SetResult)
        cout << R"(Wrong test type "t3")" << endl;
    SetResult = Parser.setMesuredResult("m1", 2.45);
    if(!SetResult)
        cout << R"(Wrong measure type "m1")" << endl;
    // Measure m2 is not set so its value will be 0.

    // Attempt to set not defined mesasure
    SetResult = Parser.setMesuredResult("m5", 2.45);
    if(!SetResult)
        cout << R"(Wrong measure type "m5")" << endl;

    for(auto t : Tests)
        cout << t.first << " result : " << t.second << endl;
    for(auto m : Measures)
        cout << m.first << " value : " << m.second << endl;;

    // Failure is when t1 is OK and t2 is Failed or t3 is failed and m1 is less than or equal to 2.45.
    // Result should be Failed
    std::string Analysis = "~t1 & t2 | t3 & m1 <= 2.45";
    cout << endl << "Result of ~t1 & t2 | t3 & m1 <= 2.45 : ";
    if(BitParserInterface::parseResultString(Analysis, &Parser))
    {
        std::cout << Parser.getResult() << std::endl<< std::endl;
    }
    else
    {
        std::cout << std::endl << "Wrong results sentence." << std::endl;
    }

    // Failure is when t1 is OK and t2 is Failed and t3 is failed and m1 is less than or equal to 2.45.
    // Result should be OK
    Analysis = "~t1 & t2 & t3 & m1 <= 2.45";
    cout << "Result of ~t1 & t2 & t3 & m1 <= 2.45 : ";
    if(BitParserInterface::parseResultString(Analysis, &Parser))
    {
        std::cout << Parser.getResult() << std::endl << std::endl;
    }
    else
    {
        std::cout << std::endl << "Wrong results sentence." << std::endl;
    }
}
