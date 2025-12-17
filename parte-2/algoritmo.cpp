#include "algoritmo.hpp"
#include "abierta.hpp"
#include "grafo.hpp"
#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>

constexpr double PI = 3.14159265358979323846;
constexpr double EARTH_R = 6371000.0;

// --------------------------------------------------
// Precalcular datos del destino (UNA SOLA VEZ)
// --------------------------------------------------
void Algoritmo::inicializar_destino(int nodo_destino, const Grafo &grafo) {
    const auto &n = grafo.get_nodos()[nodo_destino - 1];

    double lat = n.lat / 1e6;
    double lon = n.lon / 1e6;

    destino_cache.lat_rad = lat * PI / 180.0;
    destino_cache.lon_rad = lon * PI / 180.0;
    destino_cache.cos_lat = std::cos(destino_cache.lat_rad);

    destino_inicializado = true;
}


// Dependiendo del valor de usa_h, se ejecuta A* o Dijkstra (heurística == 0)
Resultado Algoritmo::busqueda(int origen, int destino, Grafo &grafo, bool usa_h) {
    if (usa_h && !destino_inicializado) {
        inicializar_destino(destino, grafo);
    }
    // Inicializaciones importantes
    int n = grafo.get_num_nodos();
    Abierta abierta(2 * grafo.get_max_cost() + 1); // En la memoria queda explicado el por que de este módulo
    std::vector<bool> cerrada(n, false);
    std::vector<int> g_minimos(n, std::numeric_limits<int>::max()); // Se usará para ir actualizando los mejores valores de g
    std::vector<int> padres(n, 0); // Se usa para reconstruir el camino de la solución
    int n_expansiones = 0; // Para mostrarlo como información
    
    // Insertamos el primer nodo en abierta
    int h_ini = 0;
    if (usa_h) {
        h_ini = haversine(origen, grafo);
    }
    g_minimos[origen-1] =  0; // Iniciamos el valor de g del origen
    ElementoAbierta e = {origen, h_ini, 0};
    abierta.insertar(e);

    ElementoAbierta actual, hijo;

    while (true) {
        actual = abierta.extraer_minimo();
        if (actual.id == -1) {
            // Lista abierta vacía
            break;
        }

        if (cerrada[actual.id-1]) continue; // Si el nodo ya está en cerrada, lo saltamos 
        
        n_expansiones++;
        
        // Meta encontrada
        if (actual.id == destino) {
            // Reconstrucción del camino mediante backtracking (hacia atrás)
            std::vector<std::pair<int, int>> camino; // En el camino guardamos el ID del nodo y el coste de la arista para llegar a él
            for (int v = destino; v != 0; v = padres[v-1]) {
                int p = padres[v-1];
                if (p != 0) {
                    camino.push_back({v, grafo.get_coste_arista(p, v)});
                } else {
                    camino.push_back({v, 0}); // El origen no tiene arista de entrada
                }
            }
            std::reverse(camino.begin(), camino.end());

            return Resultado{actual.g, camino, n_expansiones};
        }

        cerrada[actual.id-1] = true;

        for (const auto& arista : grafo.get_nodos()[actual.id-1].vecinos) {
            int v = arista.first;       // ID del vecino
            if (cerrada[v-1]) continue; // Si el vecino ya está en cerrada, lo saltamos
            int peso = arista.second;   // Peso de la arista 
            int nuevo_g = actual.g + peso; // Coste acumulado

            // Si ya hemos encontrado un camino mejor, lo saltamos, nos ahorramos meterlo en la lista abierta
            if (nuevo_g < g_minimos[v-1]) {
                g_minimos[v-1] = nuevo_g;
                padres[v-1] = actual.id; // Guardamos el rastro localmente

                int h_v = 0;
                if (usa_h) {
                    h_v = haversine(v, grafo);
                }

                hijo = {v, nuevo_g + h_v, nuevo_g};
                abierta.insertar(hijo);
            }
        }
    }
    // Si salimos del bucle sin encontrar el destino
    return Resultado{-1, {}, n_expansiones};
}

// --------------------------------------------------
// Heurística Haversine (DESTINO FIJO)
// --------------------------------------------------
inline int Algoritmo::haversine(int nodo_origen, const Grafo &grafo) {

    const auto &n = grafo.get_nodos()[nodo_origen - 1];

    double lat = n.lat / 1e6;
    double lon = n.lon / 1e6;

    double lat_rad = lat * PI / 180.0;
    double lon_rad = lon * PI / 180.0;

    double dLat = destino_cache.lat_rad - lat_rad;
    double dLon = destino_cache.lon_rad - lon_rad;

    double sin_dLat = std::sin(dLat * 0.5);
    double sin_dLon = std::sin(dLon * 0.5);

    double a = sin_dLat * sin_dLat +
               std::cos(lat_rad) * destino_cache.cos_lat *
               sin_dLon * sin_dLon;

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return static_cast<int>(EARTH_R * c);
}