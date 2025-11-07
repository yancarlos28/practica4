#include "Red.h"
#include <queue>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <climits>
#include <iostream>

using namespace std;

bool Red::existe(const string& id) const noexcept {
    return enrutadores_.count(id) > 0;
}

void Red::agregarEnrutador(const string& id) {
    if (existe(id)) return;
    enrutadores_.emplace(id, Enrutador{id});
    recalcularTablas();
}

void Red::eliminarEnrutador(const string& id) {
    if (!existe(id)) return;
    for (auto& [_, r] : enrutadores_) {
        r.eliminarEnlace(id);
        recalcularTablas();
    }
    enrutadores_.erase(id);
}

void Red::agregarEnlace(const string& a, const string& b, int costo) {
    if (a == b) throw invalid_argument("No se permite enlace A-A");
    if (costo < 0) throw invalid_argument("Costo negativo no permitido");
    if (!existe(a)) agregarEnrutador(a);
    if (!existe(b)) agregarEnrutador(b);
    enrutadores_.at(a).agregarEnlace(b, costo);
    enrutadores_.at(b).agregarEnlace(a, costo);
    recalcularTablas();
}

void Red::actualizarCosto(const string& a, const string& b, int nuevoCosto) {
    if (nuevoCosto < 0) throw invalid_argument("Costo negativo no permitido");
    if (!existe(a) || !existe(b)) throw runtime_error("Enrutador inexistente");
    if (!enrutadores_.at(a).tieneEnlaceCon(b) || !enrutadores_.at(b).tieneEnlaceCon(a))
        throw runtime_error("El enlace no existe");
    enrutadores_.at(a).agregarEnlace(b, nuevoCosto);
    enrutadores_.at(b).agregarEnlace(a, nuevoCosto);
    recalcularTablas();
}

void Red::eliminarEnlace(const string& a, const string& b) {
    if (!existe(a) || !existe(b)) return;
    enrutadores_.at(a).eliminarEnlace(b);
    enrutadores_.at(b).eliminarEnlace(a);
}
//algoritmo para el camino más eficiente
void Red::dijkstra(
    const string& origen,
    unordered_map<string, int>& dist,
    unordered_map<string, string>& padre
    ) const {
    dist.clear();
    padre.clear();
    for (const auto& [id, _] : enrutadores_) dist[id] = INT_MAX;
    dist[origen] = 0;

    using Par = pair<int, string>;
    auto cmp = [](const Par& a, const Par& b)
    { return a.first > b.first;
    };
    priority_queue<Par, vector<Par>, decltype(cmp)> pq(cmp);
    pq.push({0, origen});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;

        const auto& ru = enrutadores_.at(u);
        for (const auto& [v, w] : ru.adyacentes()) {
            if (w < 0) throw runtime_error("Se detecto costo negativo");
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                padre[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

vector<string> Red::reconstruirCamino(
    const string& origen,
    const string& destino,
    const unordered_map<string, string>& padre
    ) {
    vector<string> camino;
    if (origen == destino) {
        camino.push_back(origen); return camino; }
    string cur = destino;

    // Si destino no tiene padre y no es el origen, está desconectado
    if (padre.find(cur) == padre.end()) return camino;

    while (cur != origen) {
        camino.push_back(cur);
        auto it = padre.find(cur);
        if (it == padre.end()) {
            camino.clear(); return camino; }
        cur = it->second;
    }
    camino.push_back(origen);
    reverse(camino.begin(), camino.end());
    return camino;
}

void Red::recalcularTablas() {
    for (auto& [idOrigen, rOrigen] : enrutadores_) {
        unordered_map<string,int> dist;
        unordered_map<string,string> padre;
        dijkstra(idOrigen, dist, padre);

        unordered_map<string, EntradaTabla> t;
        for (const auto& [idDestino, _] : enrutadores_) {
            if (idDestino == idOrigen) continue;
            EntradaTabla e;
            e.costoTotal = dist[idDestino];
            e.camino = reconstruirCamino(idOrigen, idDestino, padre);
            t[idDestino] = move(e);
        }
        rOrigen.establecerTabla(t);
    }
}

EntradaTabla Red::rutaOptima(const string& origen, const string& destino) const {
    if (!existe(origen) || !existe(destino))
        throw runtime_error("Origen o destino inexistente");

    const auto& tabla = enrutadores_.at(origen).tabla();
    auto it = tabla.find(destino);
    if (it != tabla.end()) return it->second;

    // Cálculo al vuelo si la tabla no está precalculada
    unordered_map<string,int> dist;
    unordered_map<string,string> padre;
    dijkstra(origen, dist, padre);

    EntradaTabla e;
    e.costoTotal = dist[destino];
    e.camino = reconstruirCamino(origen, destino, padre);
    return e;
}

void Red::cargarDesdeArchivo(const string& rutaArchivo) {
    ifstream in(rutaArchivo);
    if (!in) throw runtime_error("No se pudo abrir el archivo");

    enrutadores_.clear();
    string linea;

    int n = 0;
    if (!getline(in, linea)) throw runtime_error("Archivo vacio");
    {
        stringstream ss(linea);
        if (!(ss >> n)) throw runtime_error("Primera linea debe ser un entero (número de enrutadores)");
    }

    if (!getline(in, linea)) throw runtime_error("Falta linea de ids");
    {
        stringstream ss(linea);
        for (int i = 0; i < n; ++i) {
            string id;
            if (!(ss >> id)) throw runtime_error("Cantidad de ids menor a n");
            agregarEnrutador(id);
        }
    }

    while (getline(in, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        stringstream ss(linea);
        string a, b; int c;
        if (ss >> a >> b >> c) {
            agregarEnlace(a, b, c);
        }
    }
}

void Red::guardarEnArchivo(const string& rutaArchivo) const {
    ofstream out(rutaArchivo);
    if (!out) throw runtime_error("No se pudo abrir el archivo para escribir");

    out << enrutadores_.size() << "\n";

    bool primero = true;
    for (const auto& [id, _] : enrutadores_) {
        if (!primero) out << ' ';
        out << id; primero = false;
    }
    out << "\n";

    set<pair<string,string>> impreso;
    for (const auto& [a, ra] : enrutadores_) {
        for (const auto& [b, costo] : ra.adyacentes()) {
            auto par = minmax(a,b);
            if (impreso.insert(par).second) {
                out << par.first << ' ' << par.second << ' ' << costo << "\n";
            }
        }
    }
}

void Red::imprimirRed(ostream& os) const {
    os << "=== Red (enlaces) ===\n";
    set<pair<string,string>> visto;
    for (const auto& [a, ra] : enrutadores_) {
        for (const auto& [b, c] : ra.adyacentes()) {
            auto par = minmax(a,b);
            if (visto.insert(par).second) {
                os << par.first << " --" << c << "-- " << par.second << "\n";
            }
        }
    }
}

void Red::imprimirTablas(ostream& os) const {
    for (const auto& [_, r] : enrutadores_) {
        r.imprimirTabla(os);
        os << '\n';
    }
}
