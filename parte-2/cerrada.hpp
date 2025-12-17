#ifndef CERRADA_HPP
#define CERRADA_HPP

#include <vector>

class Cerrada {
    public:
        Cerrada(int n_nodos) {
           cerrada.resize(n_nodos, false);
        }   

        void insertar(int nodo) {
            cerrada[nodo-1] = true;
        }

        bool contiene(int nodo) {
            return cerrada[nodo-1];
        }

    private:
        std::vector<bool> cerrada;
};

#endif