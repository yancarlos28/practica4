#include "Enrutador.h"
#include <climits>
#include <iostream>

using namespace std;

Enrutador::Enrutador(const string& id) : id_(id) {}

const string& Enrutador::obtenerId() const noexcept {
    return id_; }

void Enrutador::agregarEnlace(const string& vecino, int costo) {
    adyacentes_[vecino] = costo;
}

void Enrutador::eliminarEnlace(const string& vecino) {
    adyacentes_.erase(vecino);
}

bool Enrutador::tieneEnlaceCon(const string& vecino) const noexcept {
    return adyacentes_.count(vecino) > 0;
}

const unordered_map<string,int>& Enrutador::adyacentes() const noexcept {
    return adyacentes_;
}

unordered_map<string,int>& Enrutador::adyacentes() noexcept {
    return adyacentes_;
}

void Enrutador::establecerTabla(const unordered_map<string, EntradaTabla>& nuevaTabla) {
    tabla_ = nuevaTabla;
}

const unordered_map<string, EntradaTabla>& Enrutador::tabla() const noexcept {
    return tabla_;
}

void Enrutador::imprimirTabla(ostream& os) const {
    os << "Tabla de enrutamiento de " << id_ << ":\n";
    for (const auto& [dest, entrada] : tabla_) {
        os << "  -> " << dest << " | Costo: ";
        if (entrada.costoTotal == INT_MAX) os << "INF";
        else os << entrada.costoTotal;
        os << " | Camino: ";
        for (size_t i = 0; i < entrada.camino.size(); ++i) {
            os << entrada.camino[i];
            if (i + 1 < entrada.camino.size()) os << " -> ";
        }
        os << '\n';
    }
}
