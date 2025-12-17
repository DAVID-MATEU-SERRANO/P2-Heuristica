#include "abierta.hpp"

void Abierta::insertar(ElementoAbierta elemento) {
    // Si es el primer elemento, inicializamos current_min
    if (num_elementos == 0 || elemento.f < current_min) {
        current_min = elemento.f;
    }
    
    int idx = elemento.f % modulo;
    buckets[idx].push_back(elemento);
    num_elementos++;
}

ElementoAbierta Abierta::extraer_minimo() {
    if (num_elementos == 0) {
        return {-1, -1, -1};
    }
    int idx = current_min % modulo;

    // Buscamos el siguiente bucket con contenido
    while (buckets[idx].empty()) {
        current_min++;
        idx = current_min % modulo;
    }

    // Extraemos usando la eficiencia de deque
    ElementoAbierta res = buckets[idx].front();
    buckets[idx].pop_front();
    num_elementos--;
    
    return res;
}

