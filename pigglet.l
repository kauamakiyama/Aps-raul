%{
#include "pigglet.tab.h"
#include <stdio.h>
#include <string.h>

void yyerror(const char *s);
char* yylval_string;
%}

%option noyywrap nounput noinput

digit       [0-9]
letter      [A-Za-z]
identifier  {letter}({letter}|{digit}|_)*
number      {digit}+
string      \"([^\\"]|\\.)*\"

%%

"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }

"depositar"         { return DEPOSITAR; }
"sacar"             { return SACAR; }
"saldo"             { return SALDO; }
"definir_meta"      { return DEFINIR_META; }
"quebrar"           { return QUEBRAR; }
"imprimir"          { return IMPRIMIR; }
"finalizar"         { return FINALIZAR; }
"obter"             { return OBTER; }

"valor"             { return VALOR; }
"quantidade"        { return QUANTIDADE; }
"meta"              { return META; }

":="                { return ATRIB; }
"("                 { return ABREPAR; }
")"                 { return FECHAPAR; }
"{"                 { return ABRECH; }
"}"                 { return FECHACH; }
","                 { return VIRG; }
";"                 { return PONTOVIRG; }
"=>"                { return SETA; }

"="                 { return IGUAL; }
"!="                { return DIF; }
"<="                { return MENIG; }
">="                { return MAIORIG; }
"<"                 { return MENOR; }
">"                 { return MAIOR; }

{number}            { yylval.num = atoi(yytext); return NUMBER; }
{identifier}        { yylval.str = strdup(yytext); return IDENTIFIER; }
{string}            { yylval.str = strdup(yytext); return STRING; }

[ \t\n\r]+          { /* ignora espaços e quebras de linha */ }
.                   { printf("Caractere desconhecido: %s\n", yytext); }

%%
