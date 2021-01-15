 /* Definition of output files. */
/*%output  "DefTests.cpp"
%defines "DefTests.h"*/

 /* Make reentrant. */
%define api.pure full

 /* Define prefix. */
%define api.prefix {def}
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
#include <BitParserInterface.h>
#include "DefTestsLex.h"
void yyerror(yyscan_t scanner, const char *s);
}

%union {
	char* IdString;
	char OpSymbol;
}


%token <IdString> ID_VAR NUM_VAR SET_ID SLEEP_ID MEASURE_VAR TEST_VAR
%token <IdString> DEFERROR
%type <IdString> exp

%%
result: exp {};
exp: 	SET_ID ID_VAR NUM_VAR { if(!defget_extra(scanner)->performSetComand($2, $3))
                                yyerror(scanner, "Forbidden symbol"); }
    | SLEEP_ID NUM_VAR { if(!defget_extra(scanner)->performSleepComand($2))
        yyerror(scanner, "Forbidden symbol"); }
    | MEASURE_VAR { defget_extra(scanner)->addMeasureVariable($1); }
    | exp SET_ID ID_VAR NUM_VAR { if(!defget_extra(scanner)->performSetComand($3, $4))
        yyerror(scanner, "Forbidden symbol"); }
    | exp SLEEP_ID NUM_VAR { if(!defget_extra(scanner)->performSleepComand($3))
        yyerror(scanner, "Forbidden symbol"); }
    | exp TEST_VAR { defget_extra(scanner)->addTestVariable($2); }
    | exp MEASURE_VAR { defget_extra(scanner)->addMeasureVariable($2); }
    | DEFERROR { yyerror(scanner, "Forbidden symbol"); };

%%

/* For warning elimination */
static yyscan_t scan;
void yyerror(yyscan_t scanner, const char *s) {
    scan = scanner; /* To eliminate warning about unused variable. */
	 printf("DEF, parse error!  Message: %s\n", s);
}

