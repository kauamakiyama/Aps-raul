# 🐷 Pigglet – Linguagem de Programação para a VM Cofrinho  

## 📌 Contextualização  

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

## 📜 Pigglet especificada em EBNF  

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
    depositar(50, viagem);       // guarda 50 na categoria viagem
    depositar(20, festa);        // guarda 20 na categoria festa
    saldo(viagem);               // mostra quanto já tem na viagem
    saldo(festa);                // mostra quanto já tem na festa
    obter(valor) => s;           // atualiza o valor total
}

if (s >= m) {
    imprimir("Parabéns! Você atingiu sua meta!");
} else {
    imprimir("Ainda falta juntar mais...");
}

quebrar;     // reseta o cofrinho (zerando saldo e categorias)
finalizar;   // fim da execução
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