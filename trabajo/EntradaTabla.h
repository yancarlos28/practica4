#ifndef ENTRADATABLA_H
#define ENTRADATABLA_H
#pragma once
#include <string>
#include <vector>
#include <climits>

using namespace std;

struct EntradaTabla {
    int costoTotal = INT_MAX;     // distancia acumulada
    vector<string> camino;        // ruta completa origen..destino
};


#endif // ENTRADATABLA_H
