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

    std::cout << "# vertices: " << grafo.num_nodos << std::endl;
    std::cout << "# arcos : " << grafo.num_aristas << std::endl;
    std::cout << "# arcos max: " << grafo.max_cost << std::endl;

    // 2. Ejecutar búsqueda y cronometrar
    Algoritmo algoritmo;
    auto t_inicio = std::chrono::high_resolution_clock::now();
    
    // Ejecutamos A* (usa_h = true)
    Algoritmo::Resultado res = algoritmo.busqueda(origen, destino, grafo, true);
    
    auto t_final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = t_final - t_inicio;

    // 3. Mostrar resultados por pantalla (lo capturará el terminal)
    if (res.coste_total != -1) {
        std::cout << "Solucion optima encontrada con coste " << res.coste_total << std::endl;
        std::cout << "Tiempo de ejecucion: " << std::fixed << std::setprecision(2) << tiempo.count() << " segundos" << std::endl;
        
        double nodos_sec = (tiempo.count() > 0) ? res.nodos_expandidos / tiempo.count() : 0;
        std::cout << "# expansiones : " << res.nodos_expandidos << " (" << std::fixed << std::setprecision(2) << nodos_sec << " nodes/sec)" << std::endl;

        // 4. Guardar en el archivo de salida con el formato de guiones solicitado
        std::ofstream out(argv[5]);
        if (out.is_open()) {
            for (size_t i = 0; i < res.camino.size(); ++i) {
                out << res.camino[i];
                if (i < res.camino.size() - 1) {
                    // Consultamos el coste del arco real en el grafo
                    int u = res.camino[i];
                    int v = res.camino[i+1];
                    int coste_arco = grafo.get_coste_arista(u, v);
                    out << " - (" << coste_arco << ") - ";
                }
            }
            out << std::endl;
            out.close();
        }
    } else {
        std::cout << "No se encontro solucion optima." << std::endl;
    }

    return 0;
}