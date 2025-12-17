#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include "grafo.hpp"
#include "algoritmo.hpp"

int main(int argc, char * argv[]) {
    // Recogemos los argumentos según el orden del script de Python
    int origen        = std::stoi(argv[1]);
    int destino       = std::stoi(argv[2]);

    // 1. Cargar el Grafo
    Grafo grafo;
    grafo.generar_grafo(argv[3], argv[4]);

    std::cout << "# vertices: " << grafo.get_num_nodos() << std::endl;
    std::cout << "# arcos : " << grafo.get_num_aristas() << std::endl;

    // 2. Ejecutar búsqueda y cronometrar
    Algoritmo algoritmo;
    auto t_inicio_h = std::chrono::high_resolution_clock::now();
    
    // Ejecutamos A* (usa_h = true)
    Resultado res_h = algoritmo.busqueda(origen, destino, grafo, true);
    
    auto t_final_h = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo_h = t_final_h - t_inicio_h;

    auto t_inicio_d = std::chrono::high_resolution_clock::now();
    
    // Ejecutamos Dijkstra (usa_h = false)
    Resultado res_d = algoritmo.busqueda(origen, destino, grafo, false);
    
    auto t_final_d = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo_d = t_final_d - t_inicio_d;

    // 3. Mostrar resultados por pantalla (lo capturará el terminal)
    if (res_h.coste_total != -1) {
        // Mostramos el resultado de la búsqueda con heurística
        std::cout << "Solucion optima usando A* encontrada con coste " << res_h.coste_total << std::endl;
        std::cout << "Tiempo de ejecucion: " << std::fixed << std::setprecision(6) << tiempo_h.count() << " segundos" << std::endl;
        double nodos_sec_h = (tiempo_h.count() > 0) ? res_h.nodos_expandidos / tiempo_h.count() : 0;
        std::cout << "# expansiones : " << res_h.nodos_expandidos << " (" << std::fixed << std::setprecision(2) << nodos_sec_h << " nodes/sec)" << std::endl;

        // Mostramos el resultado de la búsqueda sin heurística
        std::cout << "Solucion optima usando Dijkstra encontrada con coste " << res_d.coste_total << std::endl;
        std::cout << "Tiempo de ejecucion: " << std::fixed << std::setprecision(6) << tiempo_d.count() << " segundos" << std::endl;
        double nodos_sec_d = (tiempo_d.count() > 0) ? res_d.nodos_expandidos / tiempo_d.count() : 0;
        std::cout << "# expansiones : " << res_d.nodos_expandidos << " (" << std::fixed << std::setprecision(2) << nodos_sec_d << " nodes/sec)" << std::endl;

        // 4. Guardar en el archivo de salida 
        std::ofstream out(argv[5]);
        if (out.is_open()) {
            for (size_t i = 0; i < res_h.camino.size(); ++i) {
                out << res_h.camino[i].first;
                if (i < res_h.camino.size() - 1) {
                    // El coste del arco entre el nodo i y el i+1 está guardado en el nodo i+1
                    int coste_arco = res_h.camino[i+1].second;
                    out << " - (" << coste_arco << ") - ";
                }
            }
            out << std::endl;
            out.close();
        }
    } else {
        std::cout << "No se encontro solucion optima." << std::endl;
        return 1;
    }

    return 0;
}