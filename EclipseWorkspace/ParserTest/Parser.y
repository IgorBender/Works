%output  "Parser.cpp"
%defines "Parser.h"
%define api.pure full
%define api.prefix {bit}
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }

%code requires {
#include <ParserInterface.h>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif
}

%code {
#include <ParserInterface.h>
#include "ParserLex.h"
void yyerror(yyscan_t scanner, const char *s);
}

%union {
	char *IdString;
	char OpSymbol;
	ParserInterface::BitResultType NumType;
	double RealType;
}

%token <IdString> VAR LESS_EQUAL MORE_EQUAL EQUAL_OPER NOT_EQ_OPER REAL_VAL REAL_VAR
%token <OpSymbol> NOT_OPER AND_OPER OR_OPER PAR_OPEN PAR_CLOSE ERROR LESS_OPER MORE_OPER
%type <RealType> real_val
%type <NumType> exp comparison

%right PAR_CLOSE
%left LESS_OPER MORE_OPER LESS_EQUAL MORE_EQUAL EQUAL_OPER NOT_EQ_OPER
%left AND_OPER OR_OPER
%left NOT_OPER
%left PAR_OPEN

%%
result: exp { bitget_extra(scanner)->setResult($1); };
exp: 	VAR { $$ = bitget_extra(scanner)->getTestValue($1);
/*				if(-1 == $$) 
				{
					yyerror("No defined name");
					YYABORT;
				} */
			}
	| comparison
	| exp AND_OPER exp { $$ = bitget_extra(scanner)->andOperation(ParserInterface::BitResultType($1), ParserInterface::BitResultType($3)); }
	| exp OR_OPER exp { $$ = bitget_extra(scanner)->orOperation($1, $3); }
	| NOT_OPER exp { $$ = bitget_extra(scanner)->notOperation($2); }
	| PAR_OPEN exp PAR_CLOSE { $$ = $2; }
	| ERROR { yyerror(NULL, "Forbidden symbol"); };
comparison: real_val LESS_OPER real_val { $$ = bitget_extra(scanner)->lessOperation($1, $3); }
    | real_val MORE_OPER real_val { $$ = bitget_extra(scanner)->moreOperation($1, $3); }
    | real_val LESS_EQUAL real_val { $$ = bitget_extra(scanner)->lessEqualOperation($1, $3); }
    | real_val MORE_EQUAL real_val { $$ = bitget_extra(scanner)->moreEqualOperation($1, $3); }
    | real_val EQUAL_OPER real_val { $$ = bitget_extra(scanner)->equalOperation($1, $3); }
    | real_val NOT_EQ_OPER real_val { $$ = bitget_extra(scanner)->notEqualOperation($1, $3); }
real_val: REAL_VAL { std::stringstream ConvertStream;
		ConvertStream << $1 << std::ends;
		double d = 0.0;
		ConvertStream >> d;
		$$ = d; }
	| REAL_VAR { $$ = bitget_extra(scanner)->getMeasuredValue($1); }

%%

void yyerror(yyscan_t scanner, const char *s) {
	 printf("BIT, parse error!  Message: %s\n", s);
}

