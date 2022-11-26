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

#ifndef YY_DEF_HOME_IGOR_GITHUB_WORKS_PARSERTEST_DEFTESTS_H_INCLUDED
# define YY_DEF_HOME_IGOR_GITHUB_WORKS_PARSERTEST_DEFTESTS_H_INCLUDED
/* Debug traces.  */
#ifndef DEFDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define DEFDEBUG 1
#  else
#   define DEFDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define DEFDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DEFDEBUG */
#if DEFDEBUG
extern int defdebug;
#endif
/* "%code requires" blocks.  */
#line 14 "/home/igor/GitHub/Works/ParserTest/DefTests.y"

#include <string>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

#line 65 "/home/igor/GitHub/Works/ParserTest/DefTests.h"

/* Token kinds.  */
#ifndef DEFTOKENTYPE
# define DEFTOKENTYPE
  enum deftokentype
  {
    DEFEMPTY = -2,
    DEFEOF = 0,                    /* "end of file"  */
    DEFerror = 256,                /* error  */
    DEFUNDEF = 257,                /* "invalid token"  */
    ID_VAR = 258,                  /* ID_VAR  */
    NUM_VAR = 259,                 /* NUM_VAR  */
    SET_ID = 260,                  /* SET_ID  */
    SLEEP_ID = 261,                /* SLEEP_ID  */
    MEASURE_VAR = 262,             /* MEASURE_VAR  */
    TEST_VAR = 263,                /* TEST_VAR  */
    DEFERROR = 264                 /* DEFERROR  */
  };
  typedef enum deftokentype deftoken_kind_t;
#endif

/* Value type.  */
#if ! defined DEFSTYPE && ! defined DEFSTYPE_IS_DECLARED
union DEFSTYPE
{
#line 29 "/home/igor/GitHub/Works/ParserTest/DefTests.y"

	char* IdString;
	char OpSymbol;

#line 96 "/home/igor/GitHub/Works/ParserTest/DefTests.h"

};
typedef union DEFSTYPE DEFSTYPE;
# define DEFSTYPE_IS_TRIVIAL 1
# define DEFSTYPE_IS_DECLARED 1
#endif




int defparse (yyscan_t scanner);


#endif /* !YY_DEF_HOME_IGOR_GITHUB_WORKS_PARSERTEST_DEFTESTS_H_INCLUDED  */
