CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/grafo.c src/ciclos.c src/ordenamiento.c
BIN = detector_fraude

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN) data/transacciones.txt

clean:
	rm -f $(BIN)

.PHONY: all run clean
