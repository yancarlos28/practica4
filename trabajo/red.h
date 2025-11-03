#ifndef RED_H
#define RED_H
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <iosfwd>          // ostream
#include "enrutador.h"
#include "EntradaTabla.h"
using namespace std;

// Orquesta nodos, enlaces y cálculo de rutas.
class Red {
private:
    // ===== Atributos =====
    unordered_map<string, Enrutador> enrutadores_;   // id -> Enrutador

    // ===== Helpers (algoritmo) =====
    void dijkstra(
        const string& origen,
        unordered_map<string, int>& dist,
        unordered_map<string, string>& padre
        ) const;

    static vector<string> reconstruirCamino(
        const string& origen,
        const string& destino,
        const unordered_map<string, string>& padre
        );

public:
    // ===== Gestión de enrutadores =====
    bool existe(const string& id) const noexcept;
    void agregarEnrutador(const string& id);
    void eliminarEnrutador(const string& id);

    // ===== Gestión de enlaces (no dirigidos) =====
    void agregarEnlace(const string& a, const string& b, int costo);
    void actualizarCosto(const string& a, const string& b, int nuevoCosto);
    void eliminarEnlace(const string& a, const string& b);

    // ===== Cálculo de tablas =====
    void recalcularTablas();

    // ===== Consulta puntual =====
    EntradaTabla rutaOptima(const string& origen, const string& destino) const;

    // ===== I/O de topología simple =====
    void cargarDesdeArchivo(const string& rutaArchivo);
    void guardarEnArchivo(const string& rutaArchivo) const;

    // ===== Visualización =====
    void imprimirRed(ostream& os) const;
    void imprimirTablas(ostream& os) const;
};
#endif // RED_H
