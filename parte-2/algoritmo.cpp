#include "algoritmo.hpp"
#include "abierta.hpp"
#include "grafo.hpp"
#include <algorithm>

#include "algoritmo.hpp"
#include "abierta.hpp"
#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>

/**
 * Ejecuta la búsqueda A* o Dijkstra y escribe el camino óptimo en 'out'.
 * Retorna el coste total (g) del camino, o -1 si no hay conexión.
 */
Algoritmo::Resultado Algoritmo::busqueda(int origen, int destino, Grafo &grafo, bool usa_h) {
    int n = grafo.num_nodos;
    Abierta abierta(2 * grafo.max_cost + 1);
    std::vector<bool> cerrada(n, false);
    std::vector<int> g_minimos(n, std::numeric_limits<int>::max());
    std::vector<int> padres(n, 0); // Usamos 0 para indicar sin padre (los IDs son 1..N)
    int n_expansiones = 0;
    
    int h_ini = usa_h ? haversine(origen, destino, grafo) : 0;
    g_minimos[origen-1] =  0;
    ElementoAbierta e = {origen, h_ini, 0};
    abierta.insertar(e);

    ElementoAbierta actual, hijo;
    while (!abierta.empty()) {
        actual = abierta.extraer_minimo();
        if (cerrada[actual.id-1]) continue;
        
        n_expansiones++;
        // 4. META ENCONTRADA
        if (actual.id == destino) {
            // Reconstrucción del camino mediante backtracking (hacia atrás)
            std::vector<int> camino;
            for (int v = destino; v != 0; v = padres[v-1]) {
                camino.push_back(v);
            }
            std::reverse(camino.begin(), camino.end());

            return Resultado{actual.g, camino, n_expansiones};
        }

        cerrada[actual.id-1] = true;

        for (const auto& arista : grafo.nodos[actual.id-1].vecinos) {
            int v = arista.first;       // ID del vecino
            int peso = arista.second;   // Peso de la arista (distancia real)
            int nuevo_g = actual.g + peso;

            // RELAJACIÓN: ¿Es este camino mejor que el mejor encontrado antes?
            if (nuevo_g < g_minimos[v-1]) {
                g_minimos[v-1] = nuevo_g;
                padres[v-1] = actual.id; // Guardamos el rastro localmente

                int h_v = usa_h ? haversine(v, destino, grafo) : 0;

                hijo = {v, nuevo_g + h_v, nuevo_g};
                abierta.insertar(hijo);
            }
        }
    }

    // Si salimos del bucle sin encontrar el destino
    return Resultado{-1, {}, n_expansiones};
}

int Algoritmo::haversine(int nodo_origen, int nodo_destino, Grafo &grafo) {
    // 1. Obtener coordenadas y convertir de millonésimas de grado a grados (double)
    // El formato DIMACS es: v ID LON LAT
    double lon1 = grafo.nodos[nodo_origen-1].lon / 1000000.0;
    double lat1 = grafo.nodos[nodo_origen-1].lat / 1000000.0;
    double lon2 = grafo.nodos[nodo_destino-1].lon / 1000000.0;
    double lat2 = grafo.nodos[nodo_destino-1].lat / 1000000.0;

    // 2. Convertir grados a radianes
    const double PI = 3.14159265358979323846;
    double rad_lat1 = lat1 * PI / 180.0;
    double rad_lat2 = lat2 * PI / 180.0;
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;

    // 3. Fórmula de Haversine
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(rad_lat1) * std::cos(rad_lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    // 4. Radio de la Tierra en metros
    const double R = 6371000.0;
    double distancia = R * c;

    // 5. Retornar floor para mantener la admisibilidad en A*
    return static_cast<int>(std::floor(distancia));
}