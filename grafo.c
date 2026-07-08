#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

void grafo_inicializar(Grafo* g, int numCuentas) {
    g->numCuentas = numCuentas;
    for (int i = 0; i < MAX_CUENTAS; i++) {
        g->cabeza[i] = NULL;
    }
}

void grafo_agregar_transaccion(Grafo* g, int origen, int destino, float monto) {
    if (origen < 0 || origen >= MAX_CUENTAS || destino < 0 || destino >= MAX_CUENTAS) {
        fprintf(stderr, "Cuenta fuera de rango: %d -> %d\n", origen, destino);
        return;
    }

    NodoAdy* nuevo = (NodoAdy*) malloc(sizeof(NodoAdy));
    if (nuevo == NULL) {
        fprintf(stderr, "Error de memoria al agregar transaccion\n");
        exit(1);
    }
    nuevo->destino = destino;
    nuevo->monto = monto;
    nuevo->siguiente = g->cabeza[origen]; /* insercion al inicio: O(1) */
    g->cabeza[origen] = nuevo;
}

void grafo_imprimir(const Grafo* g) {
    printf("\n--- Grafo de transacciones (%d cuentas) ---\n", g->numCuentas);
    for (int i = 0; i < g->numCuentas; i++) {
        NodoAdy* actual = g->cabeza[i];
        if (actual == NULL) continue;
        printf("Cuenta %d ->", i);
        while (actual != NULL) {
            printf(" [%d, $%.2f]", actual->destino, actual->monto);
            actual = actual->siguiente;
        }
        printf("\n");
    }
}

void grafo_liberar(Grafo* g) {
    for (int i = 0; i < MAX_CUENTAS; i++) {
        NodoAdy* actual = g->cabeza[i];
        while (actual != NULL) {
            NodoAdy* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
        g->cabeza[i] = NULL;
    }
}
