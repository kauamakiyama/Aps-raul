# Pigglet – Linguagem de Programação para a VM Cofrinho  

## Contextualização  

O **Pigglet** é uma linguagem de programação de alto nível desenvolvida para interagir com a **VM Cofrinho**.  
Ela simula um cofrinho inteligente, permitindo operações de **depósito, saque e consulta**, além de funcionalidades adicionais como:  

- **Definição de metas de economia** (`definir_meta`)  
- **Quebrar o cofrinho** para reiniciar os valores (`quebrar`)  
- **Mensagens personalizadas** (`imprimir`)  
- **Categorias de poupança** para guardar valores separados (ex.: `viagem`, `festa`, `estudos`)  

A linguagem foi projetada para explorar os principais conceitos de programação:  
- **Variáveis** (`:=`)  
- **Estruturas condicionais** (`if/else`)  
- **Laços de repetição** (`while`)  
- **Comandos específicos da VM Cofrinho**  

Com isso, é possível simular diferentes estratégias de economia e programar regras personalizadas para atingir metas.  

---

## Pigglet especificada em EBNF  

```ebnf
letter       = "A" | "B" | ... | "Z"
             | "a" | "b" | ... | "z" ;

digit        = "0" | "1" | "2" | "3" | "4"
             | "5" | "6" | "7" | "8" | "9" ;

character    = letter | digit | "_" | " " ;

identifier   = letter , { letter | digit | "_" } ;

number       = digit , { digit } ;

string       = '"' , { character } , '"' ;

------------------------------------------------

program      = { statement } ;

statement    = assignment
             | conditional
             | loop
             | action ;

assignment   = identifier , ":=" , expression , ";" ;

conditional  = "if" , "(" , condition , ")" , block ,
               [ "else" , block ] ;

loop         = "while" , "(" , condition , ")" , block ;

block        = "{" , { statement } , "}" ;

------------------------------------------------

action       = coin_command | sensor_command ;

coin_command = "depositar" , "(" , expression , [ "," , identifier ] , ")" , ";"
             | "sacar" , "(" , expression , [ "," , identifier ] , ")" , ";"
             | "saldo" , [ "(" , identifier , ")" ] , ";"
             | "definir_meta" , "(" , expression , ")" , ";"
             | "quebrar" , ";"
             | "imprimir" , "(" , string , ")" , ";"
             | "finalizar" , ";" ;

sensor_command = "obter" , "(" , sensor , ")" , "=>" , identifier , ";" ;

------------------------------------------------

expression   = term , { ("+" | "-" | "*" | "/") , term } ;
term         = number | identifier | "(" , expression , ")" ;

condition    = expression , comparator , expression ;
comparator   = "=" | "!=" | "<" | ">" | "<=" | ">=" ;

sensor       = "valor" | "quantidade" | "meta" ;
```



## Exemplo de uso

```pigglet
definir_meta(200);

obter(meta) => m;
obter(valor) => s;

while (s < m) {
    depositar(50, viagem);
    depositar(20, festa);
    saldo(viagem);
    saldo(festa);
    obter(valor) => s;
}

if (s >= m) {
    imprimir("Parabéns! Você atingiu sua meta!");
} else {
    imprimir("Ainda falta juntar mais...");
}

quebrar;
finalizar;
```




## Explicação do código 

1 - Define uma meta de 200 unidades.

2 - Enquanto o valor total for menor que a meta:

 - Deposita 50 na categoria viagem.
 - Deposita 20 na categoria festa.
 - Mostra os saldos parciais.

3 - Quando atingir ou ultrapassar a meta, imprime uma mensagem de sucesso.

4 - Quebra o cofrinho (resetando os saldos).

5 - Finaliza o programa.



## Possiveis usos da linguagem

 - Simulação de economia pessoal.

 - Ensino de estruturas básicas de programação (variáveis, loops e condicionais).

 - Demonstração de compiladores e máquinas virtuais simples em sala de aula.

 - Linguagem didática para mostrar abstração de máquina.

---

## Como compilar

### No Windows:

1. **Instale as ferramentas necessárias:**
   - **MinGW-w64** (para gcc)
   - **Bison** e **Flex** (ou use WSL)

2. **Use um dos scripts de build:**
   ```cmd
   # Opção 1: Script Batch
   build.bat
   
   # Opção 2: Script PowerShell
   powershell -ExecutionPolicy Bypass -File build.ps1
   ```

3. **Ou use WSL (recomendado):**
   ```bash
   # No WSL/Linux
   make
   ```

### No Linux/WSL:

```bash
make
```

### Arquivos gerados:
- `pigglet.tab.c` e `pigglet.tab.h` (parser)
- `lex.yy.c` (lexer)
- `pigglet.exe` (executável final)

### Limpeza:
```bash
make clean
```

---

## Arquitetura da VM

A VM Pigglet foi construída seguindo o padrão do **MicrowaveVM**, com:

### Componentes principais:

1. **Sistema de Valores (`Value`)**
   - Tipos: `int`, `bool`, `string`
   - Conversão automática entre tipos
   - Gerenciamento de memória

2. **Pilha de Execução (`Stack`)**
   - Operações push/pop
   - Suporte a operações aritméticas
   - Gerenciamento dinâmico de capacidade

3. **Tabela de Símbolos (`Variables`)**
   - Armazenamento de variáveis
   - Busca por nome
   - Atualização dinâmica

4. **Categorias de Poupança (`Categories`)**
   - Múltiplas categorias
   - Saldo por categoria
   - Gerenciamento de memória

5. **Sistema de Bytecode (Preparado)**
   - Estrutura para instruções
   - Interpretador de bytecode
   - Opcodes definidos

### Estrutura da VM:

```c
typedef struct {
    Variable *variables;
    Category *categories;
    int total_balance;
    int meta;
    Value *stack;
    Instruction *program;
} VM;
```

### Funcionalidades implementadas:

- **Gerenciamento de memória**: Alocação e liberação automática
- **Pilha de execução**: Operações push/pop
- **Tabela de símbolos**: Variáveis dinâmicas
- **Categorias**: Múltiplas categorias de poupança
- **Sensores**: Leitura de valores da VM
- **Comandos**: Depositar, sacar, saldo, etc.
- **Estado da VM**: Visualização completa do estado

---

## Como usar a linguagem

### 1. Compilar o projeto:
```bash
make
```

### 2. Executar um programa:
```bash
./pigglet < exemplo.pigglet
```

### 3. Teste interativo:
```bash
echo "depositar(100); saldo(); quebrar; finalizar;" | ./pigglet
```

### Regra importante:
**Todos os programas Pigglet devem terminar com:**
```pigglet
quebrar;
finalizar;
```

Essa é uma regra obrigatória da linguagem!


É preciso terminar o programa usando o finalizar()!!!