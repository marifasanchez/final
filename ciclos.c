#include <stdio.h>
#include "ciclos.h"

/* Colores para el DFS (idea explicada en el paper: se recorre el arbol
   DFS de cada nodo y se compara con el grafo inducido para hallar ciclos).
   BLANCO = no visitado, GRIS = en la pila de recursion actual (path activo),
   NEGRO = ya se termino de procesar por completo. */
#define BLANCO 0
#define GRIS 1
#define NEGRO 2

static void guardar_ciclo(const int pila[], const float pilaMontoEntrada[],
                           int desde, int hasta, float montoCierre,
                           ListaCiclos* resultado, int longitudMinima) {
    int longitud = (hasta - desde) + 1;
    if (longitud < longitudMinima) return;               /* descarta ciclos triviales */
    if (resultado->cantidad >= MAX_CICLOS) return;        /* limite del arreglo */
    if (longitud > MAX_LONGITUD_CICLO) return;

    Ciclo* c = &resultado->items[resultado->cantidad];
    float total = montoCierre;
    for (int i = desde; i <= hasta; i++) {
        c->nodos[i - desde] = pila[i];
        if (i > desde) total += pilaMontoEntrada[i];
    }
    c->longitud = longitud;
    c->montoTotal = total;
    resultado->cantidad++;
}

static void dfs(const Grafo* g, int u, int color[], int pila[],
                 float pilaMontoEntrada[], int profundidad,
                 ListaCiclos* resultado, int longitudMinima) {
    color[u] = GRIS;
    pila[profundidad] = u;

    NodoAdy* arista = g->cabeza[u];
    while (arista != NULL) {
        int v = arista->destino;

        if (color[v] == BLANCO) {
            pilaMontoEntrada[profundidad + 1] = arista->monto;
            dfs(g, v, color, pila, pilaMontoEntrada, profundidad + 1, resultado, longitudMinima);
        } else if (color[v] == GRIS) {
            /* v esta en el camino actual -> se cerro un ciclo */
            int desde = -1;
            for (int i = 0; i <= profundidad; i++) {
                if (pila[i] == v) { desde = i; break; }
            }
            if (desde != -1) {
                guardar_ciclo(pila, pilaMontoEntrada, desde, profundidad,
                              arista->monto, resultado, longitudMinima);
            }
        }
        /* si color[v] == NEGRO, ya fue procesado por completo: se ignora */

        arista = arista->siguiente;
    }

    color[u] = NEGRO;
}

void detectar_ciclos(const Grafo* g, ListaCiclos* resultado, int longitudMinima) {
    int color[MAX_CUENTAS];
    int pila[MAX_LONGITUD_CICLO];
    float pilaMontoEntrada[MAX_LONGITUD_CICLO];

    for (int i = 0; i < g->numCuentas; i++) color[i] = BLANCO;
    resultado->cantidad = 0;

    for (int i = 0; i < g->numCuentas; i++) {
        if (color[i] == BLANCO) {
            dfs(g, i, color, pila, pilaMontoEntrada, 0, resultado, longitudMinima);
        }
    }
}
