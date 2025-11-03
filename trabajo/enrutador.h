#ifndef ENRUTADOR_H
#define ENRUTADOR_H
#pragma once
#include <string>
#include <unordered_map>
#include <iosfwd>      // ostream
#include <climits>
#include "EntradaTabla.h"

using namespace std;

class Enrutador {
private:
    // ===== Atributos =====
    string id_;                                          // identificador del enrutador
    unordered_map<string, int> adyacentes_;              // vecino -> costo directo
    unordered_map<string, EntradaTabla> tabla_;          // destino -> entrada

public:
    // ===== Ctor =====
    explicit Enrutador(const string& id);

    // ===== Identidad =====
    const string& obtenerId() const noexcept;

    // ===== Vecindad (enlaces directos) =====
    void agregarEnlace(const string& vecino, int costo); // crea/actualiza
    void eliminarEnlace(const string& vecino);
    bool tieneEnlaceCon(const string& vecino) const noexcept;

    // Acceso a la lista de vecinos
    const unordered_map<string,int>& adyacentes() const noexcept;
    unordered_map<string,int>& adyacentes()       noexcept;

    // ===== Tabla de enrutamiento =====
    void establecerTabla(const unordered_map<string, EntradaTabla>& nuevaTabla);
    const unordered_map<string, EntradaTabla>& tabla() const noexcept;

    // ===== Utilidad =====
    void imprimirTabla(ostream& os) const;
};


#endif // ENRUTADOR_H
