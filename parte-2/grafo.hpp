#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <utility>
#include <string>


struct Nodo {
    int lat;
    int lon;
    std::vector<std::pair<int,int>> vecinos;
    std::vector<int> padres;
};

class Grafo {
    public:
        Grafo() {}

        void generar_grafo(std::string coordenadas, std::string distancias);
        int get_coste_arista(int u, int v);
        
    public:
        std::vector<Nodo> nodos;
        int num_nodos;
        int num_aristas;
        int max_cost = -1;
};

#endif
