%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

void yyerror(const char *s);
int yylex(void);

extern VM *vm;
extern char *yytext;
%}

%union {
    int num;
    char* str;
    int expr_val;
}

%token <num> NUMBER
%token <str> IDENTIFIER STRING

%type <expr_val> expression term
%type <str> opt_id opt_paren_id sensor

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
    IDENTIFIER ATRIB expression PONTOVIRG {
        Value val = int_value($3);
        vm_set_variable(vm, $1, val);
    }
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
      DEPOSITAR ABREPAR expression opt_id FECHAPAR PONTOVIRG {
        vm_depositar(vm, $3, $4);
      }
    | SACAR ABREPAR expression opt_id FECHAPAR PONTOVIRG {
        vm_sacar(vm, $3, $4);
      }
    | SALDO opt_paren_id PONTOVIRG {
        vm_saldo(vm, $2);
      }
    | DEFINIR_META ABREPAR expression FECHAPAR PONTOVIRG {
        vm_definir_meta(vm, $3);
      }
    | QUEBRAR PONTOVIRG {
        vm_quebrar(vm);
      }
    | IMPRIMIR ABREPAR STRING FECHAPAR PONTOVIRG {
        vm_imprimir($3);
      }
    | FINALIZAR PONTOVIRG {
      }
    ;

opt_id:
    /* vazio */ { $$ = NULL; }
  | VIRG IDENTIFIER { $$ = $2; }
    ;

opt_paren_id:
    /* vazio */ { $$ = NULL; }
  | ABREPAR IDENTIFIER FECHAPAR { $$ = $2; }
    ;

sensor_command:
    OBTER ABREPAR sensor FECHAPAR SETA IDENTIFIER PONTOVIRG {
        int valor = 0;
        if (strcmp($3, "valor") == 0) {
            valor = vm_obter_valor(vm);
        } else if (strcmp($3, "quantidade") == 0) {
            valor = vm_obter_quantidade(vm);
        } else if (strcmp($3, "meta") == 0) {
            valor = vm_obter_meta(vm);
        }
        Value val = int_value(valor);
        vm_set_variable(vm, $6, val);
    }
    ;

expression:
    term { $$ = $1; }
  | expression '+' term { $$ = $1 + $3; }
  | expression '-' term { $$ = $1 - $3; }
  | expression '*' term { $$ = $1 * $3; }
  | expression '/' term { 
        if ($3 == 0) {
            fprintf(stderr, "Erro: divisão por zero\n");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
  ;

term:
      NUMBER { $$ = $1; }
    | IDENTIFIER { 
        Value *var = vm_get_variable(vm, $1);
        if (var != NULL) {
            $$ = value_to_int(*var);
        } else {
            fprintf(stderr, "Erro: variável '%s' não declarada\n", $1);
            $$ = 0;
        }
    }
    | ABREPAR expression FECHAPAR { $$ = $2; }
    ;

condition:
    expression comparator expression {
    }
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
      VALOR { $$ = strdup("valor"); }
    | QUANTIDADE { $$ = strdup("quantidade"); }
    | META { $$ = strdup("meta"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}

VM *vm = NULL;

int main(void) {
    printf("Iniciando analise da VM Pigglet...\n");
    printf("VM criada (estilo MicrowaveVM)\n");
    vm = vm_create();
    yyparse();
    vm_print_state(vm);
    printf("Analise concluida com sucesso.\n");
    vm_destroy(vm);
    return 0;
}
