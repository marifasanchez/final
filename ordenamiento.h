#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "ciclos.h"

/* Ordena los ciclos de mayor a menor monto total usando Quick Sort,
   para priorizar cuales revisar primero (idea tomada del paper: los
   ciclos se ordenan por "fraud probability" para priorizar la revision) */
void ordenar_ciclos_por_monto(ListaCiclos* lista);

#endif
