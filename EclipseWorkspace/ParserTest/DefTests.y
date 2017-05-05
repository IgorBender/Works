 /* Definition of output files. */
%output  "DefTests.cpp"
%defines "DefTests.h"

 /* Make reentrant. */
%define api.pure full

 /* Define prefix. */
%define api.prefix "def"
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }

 /* The code for DefTests.h. */
%code requires {
#include <string>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif
}

 /* The code for DefTests.cpp. */
%code {
#include <ParserInterface.h>
#include "DefTestsLex.h"
void yyerror(yyscan_t scanner, const char *s);
}

%union {
	char* IdString;
	char OpSymbol;
}


%token <IdString> TEST_VAR MEASURE_VAR
%token <IdString> DEFERROR
%type <IdString> exp

%%
result: exp {}; 
exp: 	TEST_VAR { defget_extra(scanner)->addTestVariable($1); }
	| MEASURE_VAR { defget_extra(scanner)->addMeasureVariable($1); }
	| exp TEST_VAR { defget_extra(scanner)->addTestVariable($2); }
	| exp MEASURE_VAR { defget_extra(scanner)->addMeasureVariable($2); }
	| DEFERROR { yyerror(scanner, "Forbidden symbol"); };

%%


void yyerror(yyscan_t scanner, const char *s) {
	 printf("DEF, parse error!  Message: %s\n", s);
}

