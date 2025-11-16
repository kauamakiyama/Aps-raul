#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    VAL_INT,
    VAL_BOOL,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        bool bool_val;
        char *str_val;
    } v;
} Value;

typedef enum {
    OP_NOP,
    OP_PUSH_INT,
    OP_PUSH_VAR,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_STORE,
    OP_DEPOSITAR,
    OP_SACAR,
    OP_SALDO,
    OP_DEFINIR_META,
    OP_QUEBRAR,
    OP_IMPRIMIR,
    OP_OBTER_VALOR,
    OP_OBTER_QUANTIDADE,
    OP_OBTER_META,
    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_JMP_IF_TRUE,
    OP_HALT
} OpCode;

typedef struct {
    OpCode opcode;
    Value operand;
    char *str_operand;
} Instruction;

typedef struct Variable {
    char *name;
    Value value;
    struct Variable *next;
} Variable;

typedef struct Category {
    char *name;
    int balance;
    struct Category *next;
} Category;

typedef struct {
    Variable *variables;
    Category *categories;
    int total_balance;
    int meta;
    int quantidade_moedas;
    Value *stack;
    int stack_size;
    int stack_capacity;
    int stack_pointer;
    Instruction *program;
    int program_size;
    int program_capacity;
    int pc;
    bool initialized;
    bool running;
} VM;
VM* vm_create(void);
void vm_destroy(VM *vm);
void vm_reset(VM *vm);

void vm_stack_push(VM *vm, Value value);
Value vm_stack_pop(VM *vm);
Value vm_stack_peek(VM *vm, int offset);

void vm_set_variable(VM *vm, const char *name, Value value);
Value* vm_get_variable(VM *vm, const char *name);
bool vm_variable_exists(VM *vm, const char *name);

Category* vm_get_category(VM *vm, const char *name);
void vm_create_category(VM *vm, const char *name);

void vm_depositar(VM *vm, int valor, const char *categoria);
void vm_sacar(VM *vm, int valor, const char *categoria);
void vm_saldo(VM *vm, const char *categoria);
void vm_definir_meta(VM *vm, int meta);
void vm_quebrar(VM *vm);
void vm_imprimir(const char *mensagem);
int vm_obter_valor(VM *vm);
int vm_obter_quantidade(VM *vm);
int vm_obter_meta(VM *vm);

void vm_add_instruction(VM *vm, OpCode opcode, Value operand, const char *str_operand);
void vm_execute(VM *vm);
void vm_execute_instruction(VM *vm, Instruction *instr);

void vm_print_state(VM *vm);
void vm_print_stack(VM *vm);

Value int_value(int val);
Value bool_value(bool val);
Value string_value(const char *str);
int value_to_int(Value v);
bool value_to_bool(Value v);
const char* value_to_string(Value v);
void value_free(Value v);

#endif