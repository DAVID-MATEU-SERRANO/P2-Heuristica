#!/usr/bin/env python3
import subprocess
import os
import argparse

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("nodo_origen", type = int)
    parser.add_argument("nodo_destino", type = int)
    parser.add_argument("mapa", type = str)
    parser.add_argument("output_file", type = str)

    args = parser.parse_args()

    nodo_origen = args.nodo_origen
    nodo_destino = args.nodo_destino
    coordenadas = os.path.abspath(args.mapa + ".co")
    distancias = os.path.abspath(args.mapa + ".gr")
    output_file = os.path.abspath(args.output_file)

    if os.path.exists(coordenadas) and os.path.exists(distancias):
        # Compilar el c++
        subprocess.run([
            "g++", "-O3", "-std=c++17",
            "main.cpp", "grafo.cpp", "abierta.cpp", "algoritmo.cpp",
            "-o", "search"
        ], check=True)

        # Ejecutar el c++
        subprocess.run([
            "./search", str(nodo_origen), str(nodo_destino), coordenadas, distancias, output_file
        ], check=True)

    else:
        print("Error: No se ha podido acceder a los archivos del grafo")

if __name__ == "__main__":
    main()
