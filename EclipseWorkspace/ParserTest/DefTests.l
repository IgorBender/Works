 /* Definition of output files. */
%option outfile="DefTestsLex.cpp" header-file="DefTestsLex.h"

 /* Options to generate reentrant code used in bison parser. */
%option reentrant bison-bridge

 /* No use of global variables noyywrap nounput noinput. */
%option noyywrap nounput noinput

 /* Prefix of generated functions names. */
%option prefix="def"

 /* Definition of extra parameter type. */
%option extra-type="BitParserInterface*"

%{
/*#include <io.h>*/
#include <BitParserInterface.h>
#include "DefTests.h"
#define YYSTYPE         DEFSTYPE
#define YY_EXTRA_TYPE BitParserInterface*
%}

 /* Identifier may consist of exactly one 't' OR 'T' and any number of digits. */ 
TEST_ID        [tT][0-9]+

/* Identifier may consist of exactly one 'm' OR 'M' and any number of digits. */ 
MEASURE_ID     [mM][0-9]+
 /* Space, EOL and TAB are considered as WhiteSpaces. */
WS        [ \n\t]
%%

{TEST_ID}    {
		yylval->IdString = strdup(yytext);
		return TEST_VAR;
	    }
{MEASURE_ID} {
		yylval->IdString = strdup(yytext);
		return MEASURE_VAR;
	    }
{WS}    {};
.       { return DEFERROR; }

%%
