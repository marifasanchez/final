#include <stdio.h>
#include "grafo.h"
#include "ciclos.h"
#include "ordenamiento.h"

#define LONGITUD_MINIMA_CICLO 3  /* ciclos de 1-2 nodos no son sospechosos */

static int cargar_transacciones(const char* ruta, Grafo* g) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo: %s\n", ruta);
        return 0;
    }

    int numCuentas;
    if (fscanf(archivo, "%d", &numCuentas) != 1) {
        fprintf(stderr, "Formato invalido: falta el numero de cuentas\n");
        fclose(archivo);
        return 0;
    }
    grafo_inicializar(g, numCuentas);

    int origen, destino;
    float monto;
    while (fscanf(archivo, "%d %d %f", &origen, &destino, &monto) == 3) {
        grafo_agregar_transaccion(g, origen, destino, monto);
    }

    fclose(archivo);
    return 1;
}

static void imprimir_ciclos(const ListaCiclos* lista) {
    if (lista->cantidad == 0) {
        printf("\nNo se detectaron ciclos sospechosos (longitud >= %d).\n",
               LONGITUD_MINIMA_CICLO);
        return;
    }

    printf("\n=== %d ciclo(s) sospechoso(s) detectado(s), ordenados por monto ===\n",
           lista->cantidad);
    for (int i = 0; i < lista->cantidad; i++) {
        const Ciclo* c = &lista->items[i];
        printf("Ciclo #%d | %d cuentas | monto total: $%.2f\n  Ruta: ",
               i + 1, c->longitud, c->montoTotal);
        for (int j = 0; j < c->longitud; j++) {
            printf("%d -> ", c->nodos[j]);
        }
        printf("%d (vuelve al origen)\n", c->nodos[0]);
    }
}

int main(int argc, char* argv[]) {
    const char* ruta = (argc > 1) ? argv[1] : "data/transacciones.txt";

    Grafo g;
    if (!cargar_transacciones(ruta, &g)) {
        return 1;
    }

    grafo_imprimir(&g);

    ListaCiclos ciclos;
    detectar_ciclos(&g, &ciclos, LONGITUD_MINIMA_CICLO);
    ordenar_ciclos_por_monto(&ciclos);
    imprimir_ciclos(&ciclos);

    grafo_liberar(&g);
    return 0;
}
