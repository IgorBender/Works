 /* Definition of output files. */
/* %option outfile="DefTestsLex.cpp" header-file="DefTestsLex.h" */

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

ANY_ID        [Ii][Dd]_[a-zA-Z0-9_]+
NUM_VAL       [0-9][0-9]*
SLEEP_CMD     [Ss][Ll][Ee][Ee][Pp]
SET_CMD       [Ss][Ee][Tt]
MEASURE_ID    [mM][0-9]+
TEST_ID       [tT][0-9]+

/* Space, EOL and TAB are considered as WhiteSpaces. */
WS        [ \n\t,]
%%

{NUM_VAL}    {
        yylval->IdString = strdup(yytext);
        return NUM_VAR;
        }
{ANY_ID}    {
        yylval->IdString = strdup(yytext);
        return ID_VAR;
        }
{SLEEP_CMD}    {
        yylval->IdString = strdup("SLEEP");
        return SLEEP_ID;
        }
{SET_CMD}    {
       yylval->IdString = strdup("SET");
       return SET_ID;
       }
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
