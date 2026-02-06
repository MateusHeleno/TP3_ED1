# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
TARGET = exe

# Arquivos objeto
OBJS = filaprocessos.o tp.o

# Alvo principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para filaprocessos.o
filaprocessos.o: filaprocessos.c filaprocessos.h
	$(CC) $(CFLAGS) -c filaprocessos.c

# Regra para tp.o
tp.o: tp.c filaprocessos.h
	$(CC) $(CFLAGS) -c tp.c

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)

# Limpeza completa
purge: clean
	rm -f output.out

.PHONY: clean purge