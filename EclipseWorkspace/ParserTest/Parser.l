 /* Definition of output files. */
%option outfile="ParserLex.cpp" header-file="ParserLex.h"

 /* Options to generate reentrant code used in bison parser. */
%option reentrant bison-bridge

 /* No use of global variables noyywrap nounput noinput. */
%option noyywrap nounput noinput

 /* Prefix of generated functions names. */
%option prefix="bit"

 /* Definition of extra parameter type. */
%option extra-type="ParserInterface*"

%{
#include <stdio.h>
/*#include <io.h>*/
#include <ParserInterface.h>
#include "Parser.h"
#define YYSTYPE         BITSTYPE
#define YY_EXTRA_TYPE ParserInterface*
%}

TESTID     [tT][0-9]+
NUMID      [mM][0-9]+
REAL       [0-9]*"."?[0-9]*
OP_AND     [&]
OP_OR      [|]
OP_NOT     [~]
OP_LESS    [<]
OP_MORE    [>]
OP_LESS_EQ [<][=]
OP_MORE_EQ [>][=]
OP_EQUAL   [=][=]
OP_NOT_EQ  [!][=]
PAR_OPEN   [(]
PAR_CLOSE  [)]
WS         [ \n\t]

%%

{TESTID}     {
		yylval->IdString = strdup(yytext);
		return VAR;
	 }
{NUMID}     {
		yylval->IdString = strdup(yytext);
		return REAL_VAR;
	 }
{REAL}      {
		yylval->IdString = strdup(yytext);
		return REAL_VAL;
     }
{OP_AND}     {
		yylval->OpSymbol = yytext[0];
		return AND_OPER;
	 }
{OP_OR}     {
		yylval->OpSymbol = yytext[0];
		return OR_OPER;
	 }
{OP_NOT}     {
		yylval->OpSymbol = yytext[0];
		return NOT_OPER;
	 }
{PAR_OPEN}    {
		yylval->OpSymbol = yytext[0];
		return PAR_OPEN;
	 }
{PAR_CLOSE}    {
		yylval->OpSymbol = yytext[0];
		return PAR_CLOSE;
	 }
{OP_LESS}    {
		yylval->OpSymbol = yytext[0];
		return LESS_OPER;
     }
{OP_MORE}    {
		yylval->OpSymbol = yytext[0];
		return MORE_OPER;
     }
{OP_LESS_EQ}    {
		yylval->IdString = strdup(yytext);
		return LESS_EQUAL;
     }
{OP_MORE_EQ}    {
		yylval->IdString = strdup(yytext);
		return MORE_EQUAL;
     }
{OP_EQUAL}      {
		yylval->IdString = strdup(yytext);
		return EQUAL_OPER;
     }
{OP_NOT_EQ}      {
		yylval->IdString = strdup(yytext);
		return NOT_EQ_OPER;
     }
{WS} ;
. { return ERROR; }

%%
