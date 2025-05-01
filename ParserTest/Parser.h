/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_BIT_HOME_IGOR_GITHUB_WORKS_PARSERTEST_PARSER_H_INCLUDED
# define YY_BIT_HOME_IGOR_GITHUB_WORKS_PARSERTEST_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef BITDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define BITDEBUG 1
#  else
#   define BITDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define BITDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined BITDEBUG */
#if BITDEBUG
extern int bitdebug;
#endif
/* "%code requires" blocks.  */
#line 8 "/home/igor/GitHub/Works/ParserTest/Parser.y"

#include <BitParserInterface.h>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

#line 65 "/home/igor/GitHub/Works/ParserTest/Parser.h"

/* Token kinds.  */
#ifndef BITTOKENTYPE
# define BITTOKENTYPE
  enum bittokentype
  {
    BITEMPTY = -2,
    BITEOF = 0,                    /* "end of file"  */
    BITerror = 256,                /* error  */
    BITUNDEF = 257,                /* "invalid token"  */
    VAR = 258,                     /* VAR  */
    LESS_EQUAL = 259,              /* LESS_EQUAL  */
    MORE_EQUAL = 260,              /* MORE_EQUAL  */
    EQUAL_OPER = 261,              /* EQUAL_OPER  */
    NOT_EQ_OPER = 262,             /* NOT_EQ_OPER  */
    REAL_VAL = 263,                /* REAL_VAL  */
    REAL_VAR = 264,                /* REAL_VAR  */
    NOT_OPER = 265,                /* NOT_OPER  */
    AND_OPER = 266,                /* AND_OPER  */
    OR_OPER = 267,                 /* OR_OPER  */
    PAR_OPEN = 268,                /* PAR_OPEN  */
    PAR_CLOSE = 269,               /* PAR_CLOSE  */
    ERROR = 270,                   /* ERROR  */
    LESS_OPER = 271,               /* LESS_OPER  */
    MORE_OPER = 272                /* MORE_OPER  */
  };
  typedef enum bittokentype bittoken_kind_t;
#endif

/* Value type.  */
#if ! defined BITSTYPE && ! defined BITSTYPE_IS_DECLARED
union BITSTYPE
{
#line 22 "/home/igor/GitHub/Works/ParserTest/Parser.y"

	char *IdString;
	char OpSymbol;
    BitParserInterface::BitResultType NumType;
	double RealType;

#line 106 "/home/igor/GitHub/Works/ParserTest/Parser.h"

};
typedef union BITSTYPE BITSTYPE;
# define BITSTYPE_IS_TRIVIAL 1
# define BITSTYPE_IS_DECLARED 1
#endif




int bitparse (yyscan_t scanner);


#endif /* !YY_BIT_HOME_IGOR_GITHUB_WORKS_PARSERTEST_PARSER_H_INCLUDED  */
