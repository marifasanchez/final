#ifndef CICLOS_H
#define CICLOS_H

#include "grafo.h"

#define MAX_CICLOS 100
#define MAX_LONGITUD_CICLO 50

/* Representa un ciclo sospechoso: secuencia de cuentas + su monto total */
typedef struct {
    int nodos[MAX_LONGITUD_CICLO];
    int longitud;
    float montoTotal;
} Ciclo;

typedef struct {
    Ciclo items[MAX_CICLOS];
    int cantidad;
} ListaCiclos;

/* Recorre el grafo con DFS y llena resultado con los ciclos encontrados.
   Filtra ciclos triviales (longitud <= 2) igual que en el paper (n>3
   se descartan los ciclos "normales" de pocos nodos). */
void detectar_ciclos(const Grafo* g, ListaCiclos* resultado, int longitudMinima);

#endif
