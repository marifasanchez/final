#include "ordenamiento.h"

static void intercambiar(Ciclo* a, Ciclo* b) {
    Ciclo temp = *a;
    *a = *b;
    *b = temp;
}

/* Particion tipo Lomuto, orden DESCENDENTE por montoTotal */
static int particionar(Ciclo arr[], int bajo, int alto) {
    float pivote = arr[alto].montoTotal;
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (arr[j].montoTotal > pivote) {
            i++;
            intercambiar(&arr[i], &arr[j]);
        }
    }
    intercambiar(&arr[i + 1], &arr[alto]);
    return i + 1;
}

static void quicksort(Ciclo arr[], int bajo, int alto) {
    if (bajo < alto) {
        int p = particionar(arr, bajo, alto);
        quicksort(arr, bajo, p - 1);
        quicksort(arr, p + 1, alto);
    }
}

void ordenar_ciclos_por_monto(ListaCiclos* lista) {
    if (lista->cantidad > 1) {
        quicksort(lista->items, 0, lista->cantidad - 1);
    }
}
