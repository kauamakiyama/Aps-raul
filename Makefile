# Compilador e ferramentas
CC = gcc
LEX = flex
YACC = bison -d
CFLAGS = -Wall

# Nomes dos arquivos
TARGET = pigglet
LEX_FILE = pigglet.l
YACC_FILE = pigglet.y

# Regras
all: $(TARGET)

$(TARGET): pigglet.tab.c lex.yy.c
	$(CC) $(CFLAGS) pigglet.tab.c lex.yy.c -o $(TARGET)

pigglet.tab.c pigglet.tab.h: $(YACC_FILE)
	$(YACC) $(YACC_FILE)

lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)

clean:
	rm -f lex.yy.c pigglet.tab.c pigglet.tab.h $(TARGET)
