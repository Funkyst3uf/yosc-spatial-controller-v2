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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    YJUMP = 258,                   /* YJUMP  */
    YMOVE = 259,                   /* YMOVE  */
    YMUTE = 260,                   /* YMUTE  */
    YNEWLINE = 261,                /* YNEWLINE  */
    YSEMI = 262,                   /* YSEMI  */
    YSET = 263,                    /* YSET  */
    YPONG = 264,                   /* YPONG  */
    YSWAP = 265,                   /* YSWAP  */
    YSTATUS = 266,                 /* YSTATUS  */
    YQUIT = 267,                   /* YQUIT  */
    YINT = 268,                    /* YINT  */
    YFLOAT = 269,                  /* YFLOAT  */
    YLABEL = 270                   /* YLABEL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define YJUMP 258
#define YMOVE 259
#define YMUTE 260
#define YNEWLINE 261
#define YSEMI 262
#define YSET 263
#define YPONG 264
#define YSWAP 265
#define YSTATUS 266
#define YQUIT 267
#define YINT 268
#define YFLOAT 269
#define YLABEL 270

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "yosc.y"
  // Type des valeurs attachées aux nœuds
    int i;          /**< Entier : ID des objets, états binaires, itérations */
    float f;        /**< Flottant : coordonnées, vitesses */
    char* s;        /**< Chaîne de caractères (labels) */
    Point3D p;      /**< Structure pour les coordonnées 3D */

#line 104 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
