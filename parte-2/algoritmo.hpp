#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include "grafo.hpp"
#include <fstream>

class Algoritmo {

    public:
        struct Resultado {
            int coste_total;
            std::vector<int> camino;
            int nodos_expandidos;
        };

        Algoritmo() {}

        Resultado busqueda(int nodo_origen, int nodo_destino, Grafo &grafo, bool heuristica);
        int haversine(int nodo_origen, int nodo_destino, Grafo &grafo);
    
    private:
        
};

#endif