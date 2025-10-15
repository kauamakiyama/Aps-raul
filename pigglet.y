%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

%}

%union {
    int num;
    char* str;
}

%token <num> NUMBER
%token <str> IDENTIFIER STRING

%token IF ELSE WHILE
%token DEPOSITAR SACAR SALDO DEFINIR_META QUEBRAR IMPRIMIR FINALIZAR OBTER
%token VALOR QUANTIDADE META

%token ATRIB ABREPAR FECHAPAR ABRECH FECHACH VIRG PONTOVIRG SETA
%token IGUAL DIF MENOR MAIOR MENIG MAIORIG

%left '+' '-'
%left '*' '/'

%start program

%%

program:
    /* vazio */
  | program statement
  ;

statement:
      assignment
    | conditional
    | loop
    | action
    ;

assignment:
    IDENTIFIER ATRIB expression PONTOVIRG
    ;

conditional:
    IF ABREPAR condition FECHAPAR block
    | IF ABREPAR condition FECHAPAR block ELSE block
    ;

loop:
    WHILE ABREPAR condition FECHAPAR block
    ;

block:
    ABRECH program FECHACH
    ;

action:
      coin_command
    | sensor_command
    ;

coin_command:
      DEPOSITAR ABREPAR expression opt_id FECHAPAR PONTOVIRG
    | SACAR ABREPAR expression opt_id FECHAPAR PONTOVIRG
    | SALDO opt_paren_id PONTOVIRG
    | DEFINIR_META ABREPAR expression FECHAPAR PONTOVIRG
    | QUEBRAR PONTOVIRG
    | IMPRIMIR ABREPAR STRING FECHAPAR PONTOVIRG
    | FINALIZAR PONTOVIRG
    ;

opt_id:
    /* vazio */
  | VIRG IDENTIFIER
  ;

opt_paren_id:
    /* vazio */
  | ABREPAR IDENTIFIER FECHAPAR
  ;

sensor_command:
    OBTER ABREPAR sensor FECHAPAR SETA IDENTIFIER PONTOVIRG
    ;

expression:
    term
  | expression '+' term
  | expression '-' term
  | expression '*' term
  | expression '/' term
  ;

term:
      NUMBER
    | IDENTIFIER
    | ABREPAR expression FECHAPAR
    ;

condition:
    expression comparator expression
    ;

comparator:
      IGUAL
    | DIF
    | MENOR
    | MAIOR
    | MENIG
    | MAIORIG
    ;

sensor:
      VALOR
    | QUANTIDADE
    | META
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}

int main(void) {
    printf("🐷 Iniciando análise da VM Pigglet...\n");
    yyparse();
    printf("✅ Análise concluída com sucesso.\n");
    return 0;
}
