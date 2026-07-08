#ifndef GRAFO_H
#define GRAFO_H

#define MAX_CUENTAS 200

/* Nodo de la lista de adyacencia: representa una transaccion
   saliente hacia otra cuenta */
typedef struct NodoAdy {
    int destino;
    float monto;
    struct NodoAdy* siguiente;
} NodoAdy;

/* TAD Grafo: arreglo de listas de adyacencia (grafo dirigido y ponderado) */
typedef struct {
    NodoAdy* cabeza[MAX_CUENTAS];
    int numCuentas;
} Grafo;

void grafo_inicializar(Grafo* g, int numCuentas);
void grafo_agregar_transaccion(Grafo* g, int origen, int destino, float monto);
void grafo_imprimir(const Grafo* g);
void grafo_liberar(Grafo* g);

#endif
