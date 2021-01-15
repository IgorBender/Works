/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_DEF_HOME_IGOR_GITHUB_WORKS_ECLIPSEWORKSPACE_PARSERTEST_DEFTESTS_H_INCLUDED
# define YY_DEF_HOME_IGOR_GITHUB_WORKS_ECLIPSEWORKSPACE_PARSERTEST_DEFTESTS_H_INCLUDED
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
#line 14 "/home/igor/GitHub/Works/EclipseWorkspace/ParserTest/DefTests.y"

#include <string>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

#line 64 "/home/igor/GitHub/Works/EclipseWorkspace/ParserTest/DefTests.h"

/* Token type.  */
#ifndef DEFTOKENTYPE
# define DEFTOKENTYPE
  enum deftokentype
  {
    ID_VAR = 258,
    NUM_VAR = 259,
    SET_ID = 260,
    SLEEP_ID = 261,
    MEASURE_VAR = 262,
    TEST_VAR = 263,
    DEFERROR = 264
  };
#endif

/* Value type.  */
#if ! defined DEFSTYPE && ! defined DEFSTYPE_IS_DECLARED
union DEFSTYPE
{
#line 29 "/home/igor/GitHub/Works/EclipseWorkspace/ParserTest/DefTests.y"

	char* IdString;
	char OpSymbol;

#line 90 "/home/igor/GitHub/Works/EclipseWorkspace/ParserTest/DefTests.h"

};
typedef union DEFSTYPE DEFSTYPE;
# define DEFSTYPE_IS_TRIVIAL 1
# define DEFSTYPE_IS_DECLARED 1
#endif



int defparse (yyscan_t scanner);

#endif /* !YY_DEF_HOME_IGOR_GITHUB_WORKS_ECLIPSEWORKSPACE_PARSERTEST_DEFTESTS_H_INCLUDED  */
