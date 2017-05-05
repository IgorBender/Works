/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_BIT_PARSER_H_INCLUDED
# define YY_BIT_PARSER_H_INCLUDED
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
#line 8 "Parser.y" /* yacc.c:1909  */

#include <ParserInterface.h>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

#line 60 "Parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef BITTOKENTYPE
# define BITTOKENTYPE
  enum bittokentype
  {
    VAR = 258,
    LESS_EQUAL = 259,
    MORE_EQUAL = 260,
    EQUAL_OPER = 261,
    NOT_EQ_OPER = 262,
    REAL_VAL = 263,
    REAL_VAR = 264,
    NOT_OPER = 265,
    AND_OPER = 266,
    OR_OPER = 267,
    PAR_OPEN = 268,
    PAR_CLOSE = 269,
    ERROR = 270,
    LESS_OPER = 271,
    MORE_OPER = 272
  };
#endif

/* Value type.  */
#if ! defined BITSTYPE && ! defined BITSTYPE_IS_DECLARED

union BITSTYPE
{
#line 22 "Parser.y" /* yacc.c:1909  */

	char *IdString;
	char OpSymbol;
	ParserInterface::BitResultType NumType;
	double RealType;

#line 97 "Parser.h" /* yacc.c:1909  */
};

typedef union BITSTYPE BITSTYPE;
# define BITSTYPE_IS_TRIVIAL 1
# define BITSTYPE_IS_DECLARED 1
#endif



int bitparse (yyscan_t scanner);

#endif /* !YY_BIT_PARSER_H_INCLUDED  */
