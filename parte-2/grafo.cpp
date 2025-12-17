#include "grafo.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


void Grafo::generar_grafo(std::string coordenadas, std::string distancias) {
    std::ifstream co_file(coordenadas);
    std::ifstream gr_file(distancias);

    if (!co_file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de coordenadas: " << coordenadas << "\n";
        return;
    }
    if (!gr_file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de distancias: " << distancias << "\n";
        return;
    }

    std::string line;
    // --- Leer arcos / distancias ---
    while (std::getline(gr_file, line)) {
        if (line.empty() || line[0] == 'c') continue;

        if (line[0] == 'p') {
            std::istringstream iss(line);
            char p;
            std::string tipo;
            int num_nodos_file, num_aristas_file;
            iss >> p >> tipo >> num_nodos_file >> num_aristas_file;

            num_nodos = num_nodos_file;
            num_aristas = num_aristas_file;
            nodos.resize(num_nodos);
        }

        if (line[0] == 'a') {
            std::istringstream iss(line);
            char a;
            int from, to, cost;
            iss >> a >> from >> to >> cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
            // Ajustar indices a base 0 para el acceso al vector, pero guardar el ID del vecino tal cual (1-indexed)
            nodos[from-1].vecinos.push_back({to, cost});
        }
    }
    // --- Leer coordenadas ---
    while (std::getline(co_file, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;

        if (line[0] == 'v') {
            std::istringstream iss(line);
            char v;
            int id, lon, lat;
            iss >> v >> id >> lon >> lat;

            // Los vectores se indexan desde 0
            nodos[id-1].lat = lat;
            nodos[id-1].lon = lon;
        }
    }

    co_file.close();
    gr_file.close();
}

int Grafo::get_coste_arista(int u, int v) {
    for (std::pair<int, int> const& arista : nodos[u-1].vecinos) {
        if (arista.first == v) {
            return arista.second;
        }
    }
    return 0;
}