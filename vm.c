#include "vm.h"

#define INITIAL_STACK_SIZE 1024
#define INITIAL_PROGRAM_SIZE 1024

VM* vm_create(void) {
    VM *vm = (VM*)malloc(sizeof(VM));
    vm->variables = NULL;
    vm->categories = NULL;
    vm->total_balance = 0;
    vm->meta = 0;
    vm->quantidade_moedas = 0;
    vm->stack_capacity = INITIAL_STACK_SIZE;
    vm->stack_size = 0;
    vm->stack_pointer = -1;
    vm->stack = (Value*)malloc(sizeof(Value) * vm->stack_capacity);
    vm->program_capacity = INITIAL_PROGRAM_SIZE;
    vm->program_size = 0;
    vm->pc = 0;
    vm->program = (Instruction*)malloc(sizeof(Instruction) * vm->program_capacity);
    vm->initialized = true;
    vm->running = false;
    return vm;
}

void vm_destroy(VM *vm) {
    Variable *var = vm->variables;
    while (var != NULL) {
        Variable *next = var->next;
        if (var->value.type == VAL_STRING && var->value.v.str_val) {
            free(var->value.v.str_val);
        }
        free(var->name);
        free(var);
        var = next;
    }
    Category *cat = vm->categories;
    while (cat != NULL) {
        Category *next = cat->next;
        free(cat->name);
        free(cat);
        cat = next;
    }
    for (int i = 0; i <= vm->stack_pointer; i++) {
        if (vm->stack[i].type == VAL_STRING && vm->stack[i].v.str_val) {
            free(vm->stack[i].v.str_val);
        }
    }
    free(vm->stack);
    for (int i = 0; i < vm->program_size; i++) {
        if (vm->program[i].str_operand) {
            free(vm->program[i].str_operand);
        }
    }
    free(vm->program);
    free(vm);
}

void vm_reset(VM *vm) {
    vm->total_balance = 0;
    vm->meta = 0;
    vm->quantidade_moedas = 0;
    vm->stack_pointer = -1;
    vm->pc = 0;
    vm->running = false;
}


Value int_value(int val) {
    Value v;
    v.type = VAL_INT;
    v.v.int_val = val;
    return v;
}

Value bool_value(bool val) {
    Value v;
    v.type = VAL_BOOL;
    v.v.bool_val = val;
    return v;
}

Value string_value(const char *str) {
    Value v;
    v.type = VAL_STRING;
    v.v.str_val = strdup(str);
    return v;
}

int value_to_int(Value v) {
    if (v.type == VAL_INT) {
        return v.v.int_val;
    } else if (v.type == VAL_BOOL) {
        return v.v.bool_val ? 1 : 0;
    }
    return 0;
}

bool value_to_bool(Value v) {
    if (v.type == VAL_BOOL) {
        return v.v.bool_val;
    } else if (v.type == VAL_INT) {
        return v.v.int_val != 0;
    }
    return false;
}

const char* value_to_string(Value v) {
    static char buffer[256];
    if (v.type == VAL_STRING) {
        return v.v.str_val;
    } else if (v.type == VAL_INT) {
        sprintf(buffer, "%d", v.v.int_val);
        return buffer;
    } else if (v.type == VAL_BOOL) {
        return v.v.bool_val ? "true" : "false";
    }
    return "";
}

void value_free(Value v) {
}


void vm_stack_push(VM *vm, Value value) {
    if (vm->stack_pointer >= vm->stack_capacity - 1) {
        vm->stack_capacity *= 2;
        vm->stack = (Value*)realloc(vm->stack, sizeof(Value) * vm->stack_capacity);
    }
    vm->stack_pointer++;
    vm->stack[vm->stack_pointer] = value;
}

Value vm_stack_pop(VM *vm) {
    if (vm->stack_pointer < 0) {
        fprintf(stderr, "Erro: pilha vazia\n");
        return int_value(0);
    }
    Value v = vm->stack[vm->stack_pointer];
    vm->stack_pointer--;
    return v;
}

Value vm_stack_peek(VM *vm, int offset) {
    int index = vm->stack_pointer - offset;
    if (index < 0 || index > vm->stack_pointer) {
        fprintf(stderr, "Erro: índice inválido na pilha\n");
        return int_value(0);
    }
    return vm->stack[index];
}


void vm_set_variable(VM *vm, const char *name, Value value) {
    Variable *var = vm->variables;
    while (var != NULL) {
        if (strcmp(var->name, name) == 0) {
            if (var->value.type == VAL_STRING && var->value.v.str_val) {
                free(var->value.v.str_val);
            }
            var->value = value;
            return;
        }
        var = var->next;
    }
    var = (Variable*)malloc(sizeof(Variable));
    var->name = strdup(name);
    var->value = value;
    var->next = vm->variables;
    vm->variables = var;
}

Value* vm_get_variable(VM *vm, const char *name) {
    Variable *var = vm->variables;
    while (var != NULL) {
        if (strcmp(var->name, name) == 0) {
            return &var->value;
        }
        var = var->next;
    }
    return NULL;
}

bool vm_variable_exists(VM *vm, const char *name) {
    return vm_get_variable(vm, name) != NULL;
}


Category* vm_get_category(VM *vm, const char *name) {
    if (name == NULL) return NULL;
    
    Category *cat = vm->categories;
    while (cat != NULL) {
        if (strcmp(cat->name, name) == 0) {
            return cat;
        }
        cat = cat->next;
    }
    return NULL;
}

void vm_create_category(VM *vm, const char *name) {
    if (vm_get_category(vm, name) != NULL) {
        return;
    }
    
    Category *cat = (Category*)malloc(sizeof(Category));
    cat->name = strdup(name);
    cat->balance = 0;
    cat->next = vm->categories;
    vm->categories = cat;
}


void vm_depositar(VM *vm, int valor, const char *categoria) {
    if (categoria != NULL) {
        vm_create_category(vm, categoria);
        Category *cat = vm_get_category(vm, categoria);
        cat->balance += valor;
    }
    vm->total_balance += valor;
    vm->quantidade_moedas++;
    printf("Depositado: %d", valor);
    if (categoria != NULL) {
        printf(" na categoria '%s'", categoria);
    }
    printf("\n");
}

void vm_sacar(VM *vm, int valor, const char *categoria) {
    if (categoria != NULL) {
        Category *cat = vm_get_category(vm, categoria);
        if (cat != NULL && cat->balance >= valor) {
            cat->balance -= valor;
            vm->total_balance -= valor;
            printf("Sacado: %d da categoria '%s'\n", valor, categoria);
        } else {
            printf("Erro: Saldo insuficiente na categoria '%s'\n", categoria);
        }
    } else {
        if (vm->total_balance >= valor) {
            vm->total_balance -= valor;
            printf("Sacado: %d\n", valor);
        } else {
            printf("Erro: Saldo insuficiente\n");
        }
    }
}

void vm_saldo(VM *vm, const char *categoria) {
    if (categoria != NULL) {
        Category *cat = vm_get_category(vm, categoria);
        if (cat != NULL) {
            printf("Saldo da categoria '%s': %d\n", categoria, cat->balance);
        } else {
            printf("Saldo da categoria '%s': 0\n", categoria);
        }
    } else {
        printf("Saldo total: %d\n", vm->total_balance);
    }
}

void vm_definir_meta(VM *vm, int meta) {
    vm->meta = meta;
    printf("Meta definida: %d\n", meta);
}

void vm_quebrar(VM *vm) {
    vm->total_balance = 0;
    vm->quantidade_moedas = 0;
    vm->meta = 0;
    
    Category *cat = vm->categories;
    while (cat != NULL) {
        cat->balance = 0;
        cat = cat->next;
    }
    
    printf("Cofrinho quebrado! Todos os valores foram resetados.\n");
}

void vm_imprimir(const char *mensagem) {
    if (mensagem && mensagem[0] == '"' && mensagem[strlen(mensagem)-1] == '"') {
        for (int i = 1; i < (int)strlen(mensagem)-1; i++) {
            printf("%c", mensagem[i]);
        }
        printf("\n");
    } else {
        printf("%s\n", mensagem ? mensagem : "");
    }
}

int vm_obter_valor(VM *vm) {
    return vm->total_balance;
}

int vm_obter_quantidade(VM *vm) {
    return vm->quantidade_moedas;
}

int vm_obter_meta(VM *vm) {
    return vm->meta;
}


void vm_add_instruction(VM *vm, OpCode opcode, Value operand, const char *str_operand) {
    if (vm->program_size >= vm->program_capacity) {
        vm->program_capacity *= 2;
        vm->program = (Instruction*)realloc(vm->program, sizeof(Instruction) * vm->program_capacity);
    }
    
    Instruction *instr = &vm->program[vm->program_size];
    instr->opcode = opcode;
    instr->operand = operand;
    instr->str_operand = str_operand ? strdup(str_operand) : NULL;
    vm->program_size++;
}

void vm_execute_instruction(VM *vm, Instruction *instr) {
    Value a, b, result;
    
    switch (instr->opcode) {
        case OP_NOP:
            break;
            
        case OP_PUSH_INT:
            vm_stack_push(vm, instr->operand);
            break;
            
        case OP_PUSH_VAR:
            {
                Value *var = vm_get_variable(vm, instr->str_operand);
                if (var != NULL) {
                    vm_stack_push(vm, *var);
                } else {
                    fprintf(stderr, "Erro: variável '%s' não declarada\n", instr->str_operand);
                    vm_stack_push(vm, int_value(0));
                }
            }
            break;
            
        case OP_ADD:
            b = vm_stack_pop(vm);
            a = vm_stack_pop(vm);
            result = int_value(value_to_int(a) + value_to_int(b));
            vm_stack_push(vm, result);
            break;
            
        case OP_SUB:
            b = vm_stack_pop(vm);
            a = vm_stack_pop(vm);
            result = int_value(value_to_int(a) - value_to_int(b));
            vm_stack_push(vm, result);
            break;
            
        case OP_MUL:
            b = vm_stack_pop(vm);
            a = vm_stack_pop(vm);
            result = int_value(value_to_int(a) * value_to_int(b));
            vm_stack_push(vm, result);
            break;
            
        case OP_DIV:
            b = vm_stack_pop(vm);
            a = vm_stack_pop(vm);
            if (value_to_int(b) == 0) {
                fprintf(stderr, "Erro: divisão por zero\n");
                vm_stack_push(vm, int_value(0));
            } else {
                result = int_value(value_to_int(a) / value_to_int(b));
                vm_stack_push(vm, result);
            }
            break;
            
        case OP_STORE:
            {
                Value val = vm_stack_pop(vm);
                vm_set_variable(vm, instr->str_operand, val);
            }
            break;
            
        case OP_DEPOSITAR:
            {
                Value valor = vm_stack_pop(vm);
                vm_depositar(vm, value_to_int(valor), instr->str_operand);
            }
            break;
            
        case OP_SACAR:
            {
                Value valor = vm_stack_pop(vm);
                vm_sacar(vm, value_to_int(valor), instr->str_operand);
            }
            break;
            
        case OP_SALDO:
            vm_saldo(vm, instr->str_operand);
            break;
            
        case OP_DEFINIR_META:
            {
                Value meta = vm_stack_pop(vm);
                vm_definir_meta(vm, value_to_int(meta));
            }
            break;
            
        case OP_QUEBRAR:
            vm_quebrar(vm);
            break;
            
        case OP_IMPRIMIR:
            {
                Value msg = vm_stack_pop(vm);
                vm_imprimir(value_to_string(msg));
            }
            break;
            
        case OP_OBTER_VALOR:
            vm_stack_push(vm, int_value(vm_obter_valor(vm)));
            break;
            
        case OP_OBTER_QUANTIDADE:
            vm_stack_push(vm, int_value(vm_obter_quantidade(vm)));
            break;
            
        case OP_OBTER_META:
            vm_stack_push(vm, int_value(vm_obter_meta(vm)));
            break;
            
        case OP_HALT:
            vm->running = false;
            break;
            
        default:
            fprintf(stderr, "Erro: instrução desconhecida %d\n", instr->opcode);
            break;
    }
}

void vm_execute(VM *vm) {
    vm->running = true;
    vm->pc = 0;
    
    while (vm->running && vm->pc < vm->program_size) {
        Instruction *instr = &vm->program[vm->pc];
        vm_execute_instruction(vm, instr);
        vm->pc++;
    }
}


void vm_print_state(VM *vm) {
    printf("\nEstado da VM Cofrinho:\n");
    printf("  Saldo total: %d\n", vm->total_balance);
    printf("  Quantidade de moedas: %d\n", vm->quantidade_moedas);
    printf("  Meta: %d\n", vm->meta);
    
    if (vm->categories != NULL) {
        printf("  Categorias:\n");
        Category *cat = vm->categories;
        while (cat != NULL) {
            printf("    %s: %d\n", cat->name, cat->balance);
            cat = cat->next;
        }
    }
    
    if (vm->variables != NULL) {
        printf("  Variáveis:\n");
        Variable *var = vm->variables;
        while (var != NULL) {
            printf("    %s = %s\n", var->name, value_to_string(var->value));
            var = var->next;
        }
    }
    printf("\n");
}

void vm_print_stack(VM *vm) {
    printf("Stack (pointer=%d): [", vm->stack_pointer);
    for (int i = 0; i <= vm->stack_pointer; i++) {
        printf("%s", value_to_string(vm->stack[i]));
        if (i < vm->stack_pointer) printf(", ");
    }
    printf("]\n");
}