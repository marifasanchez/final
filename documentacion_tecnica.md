# Documentación Técnica | detector-fraude-bancario
## Detector de Fraude en Transacciones Bancarias mediante Análisis de Ciclos en Grafos

## 1. Descripción general

El programa recibe una lista de transacciones bancarias (cuenta origen, cuenta
destino, monto) y construye un grafo dirigido donde cada cuenta es un nodo y
cada transacción es una arista con peso (el monto). Luego busca **ciclos**
dentro de ese grafo: secuencias de cuentas donde el dinero, tras pasar por
varias transacciones, regresa a la cuenta de origen. Estos ciclos se reportan
ordenados por el monto total involucrado, de mayor a menor, para priorizar la
revisión de los casos más significativos.

Este enfoque está inspirado directamente en Bodaghi & Teimourpour (2019),
*"The detection of professional fraud in automobile insurance using social
network analysis"*, quienes demuestran que buscar ciclos en un grafo es más
eficiente y preciso que buscar comunidades densas para detectar fraude
organizado, ya que las comunidades incluyen muchas entidades no fraudulentas
mientras que los ciclos son estructuras mucho más específicas.

## 2. Estructuras de datos (TAD)

### 2.1 Grafo (lista de adyacencia)

```c
typedef struct NodoAdy {
    int destino;
    float monto;
    struct NodoAdy* siguiente;
} NodoAdy;

typedef struct {
    NodoAdy* cabeza[MAX_CUENTAS];
    int numCuentas;
} Grafo;
```

**Justificación:** se eligió lista de adyacencia en vez de matriz de
adyacencia porque en una red de transacciones reales cada cuenta se relaciona
con relativamente pocas otras cuentas (el grafo es disperso). La matriz
ocuparía O(V²) memoria sin importar cuántas transacciones existan realmente;
la lista de adyacencia ocupa O(V + E), proporcional a las transacciones reales.

### 2.2 Ciclo y ListaCiclos

```c
typedef struct {
    int nodos[MAX_LONGITUD_CICLO];
    int longitud;
    float montoTotal;
} Ciclo;
```

Estructura simple (arreglo estático) para almacenar la secuencia de cuentas de
cada ciclo detectado y su monto acumulado, suficiente dado que el número de
ciclos y su longitud están acotados en este dominio de aplicación.

## 3. Algoritmos y complejidad

### 3.1 Detección de ciclos (DFS)

Se recorre el grafo con búsqueda en profundidad (DFS), coloreando cada nodo:

- **Blanco**: no visitado.
- **Gris**: en la pila de recursión actual (forma parte del camino activo).
- **Negro**: ya se procesó por completo.

Cuando durante el recorrido se encuentra una arista hacia un nodo **gris**,
significa que ese nodo ya está en el camino actual, es decir, se cerró un
ciclo. Se reconstruye el ciclo tomando el segmento de la pila desde ese nodo
hasta el nodo actual, y se suman los montos de las aristas involucradas.

**Complejidad:**
- Temporal: **O(V + E)**, donde V es el número de cuentas y E el número de
  transacciones, ya que cada nodo y cada arista se visitan a lo sumo una vez
  en el recorrido DFS estándar.
- Espacial: **O(V)** para los arreglos de color y de pila (además del propio
  grafo, que ya ocupa O(V+E)).

### 3.2 Ordenamiento de ciclos (Quick Sort)

Los ciclos detectados se ordenan de mayor a menor monto total mediante Quick
Sort (partición tipo Lomuto), para que los casos con mayor monto involucrado
—los potencialmente más costosos si son fraude real— se revisen primero.

**Complejidad:**
- Temporal: **O(n log n)** en el caso promedio; **O(n²)** en el peor caso
  (poco probable aquí porque el número de ciclos detectados suele ser
  pequeño en comparación con el número de cuentas).
- Espacial: **O(log n)** por la pila de recursión.

## 4. Manejo de memoria dinámica

Cada transacción agregada al grafo reserva memoria dinámica (`malloc`) para un
nodo de la lista de adyacencia. Toda esa memoria se libera explícitamente al
finalizar el programa mediante `grafo_liberar()`, que recorre cada lista de
adyacencia y libera cada nodo con `free()`. No se usa memoria dinámica en la
detección de ciclos ni en el ordenamiento (se usan arreglos estáticos
acotados), lo que simplifica el manejo de memoria y evita fugas.

## 5. Limitaciones conocidas

- El número máximo de cuentas y de ciclos detectables está acotado por
  constantes (`MAX_CUENTAS`, `MAX_CICLOS`) para mantener el código simple;
  esto es suficiente para el propósito académico del proyecto pero no
  escalaría a redes bancarias reales sin usar estructuras dinámicas (p. ej.
  tablas hash para mapear cuentas, arreglos dinámicos para los ciclos).
- El algoritmo puede reportar el mismo ciclo lógico más de una vez si es
  alcanzable desde distintos puntos de entrada del grafo; para el alcance de
  este proyecto no se implementó deduplicación exhaustiva de ciclos
  isomorfos, siguiendo la simplificación de enfocarnos en la lógica central
  de detección.

## 6. Cómo compilar y ejecutar

```bash
make          # compila el proyecto
make run      # compila (si hace falta) y ejecuta con data/transacciones.txt
make clean    # elimina el binario compilado
```

Para usar un archivo de transacciones distinto:

```bash
./detector_fraude ruta/a/otro_archivo.txt
```

### Formato del archivo de entrada

```
<numero_de_cuentas>
<cuenta_origen> <cuenta_destino> <monto>
<cuenta_origen> <cuenta_destino> <monto>
...
```

## 7. Evidencia de pruebas realizadas

Se probó el programa con el archivo de ejemplo `data/transacciones.txt`
(10 cuentas, 10 transacciones). Resultado obtenido:

\```
--- Grafo de transacciones (10 cuentas) ---
Cuenta 0 -> [6, $400.00] [1, $1500.50]
Cuenta 1 -> [2, $2300.00]
Cuenta 2 -> [0, $900.25]
Cuenta 3 -> [4, $500.00]
Cuenta 4 -> [5, $700.00]
Cuenta 5 -> [3, $300.00]
Cuenta 6 -> [7, $200.00]
Cuenta 7 -> [8, $150.00]
Cuenta 8 -> [6, $1200.00]

=== 3 ciclo(s) sospechoso(s) detectado(s), ordenados por monto ===
Ciclo #1 | 3 cuentas | monto total: $4700.75
  Ruta: 0 -> 1 -> 2 -> 0 (vuelve al origen)
Ciclo #2 | 3 cuentas | monto total: $1550.00
  Ruta: 6 -> 7 -> 8 -> 6 (vuelve al origen)
Ciclo #3 | 3 cuentas | monto total: $1500.00
  Ruta: 3 -> 4 -> 5 -> 3 (vuelve al origen)
\```

Se verificó que los 3 ciclos detectados corresponden efectivamente a
subconjuntos de cuentas donde el dinero regresa a su origen, y que se
ordenan correctamente de mayor a menor monto total. El programa compiló
sin errores ni warnings con `gcc -Wall -Wextra`.