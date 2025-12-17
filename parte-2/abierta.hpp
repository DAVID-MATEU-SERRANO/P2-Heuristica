#ifndef ABIERTA_HPP
#define ABIERTA_HPP

#include <vector>
#include <list>

struct ElementoAbierta {
    int id;
    int f;
    int g;
};

class Abierta {
    public:
        // El modulo debe ser > max_coste_arista para Dijkstra
        Abierta(int modulo) : modulo(modulo), num_elementos(0), current_min(-1) {
            buckets.resize(modulo);
        }

        void insertar(ElementoAbierta elemento);
        ElementoAbierta extraer_minimo();
        bool empty() const { return num_elementos == 0; }

    private:
        // Usamos list o vector para los buckets
        std::vector<std::list<ElementoAbierta>> buckets;
        int modulo;
        int num_elementos;
        int current_min; 
};

#endif