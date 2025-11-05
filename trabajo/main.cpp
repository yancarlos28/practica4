#include "Red.h"
#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <climits>

using namespace std;

// ================= Utilidades de entrada =================
int leerInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida. Intenta de nuevo.\n";
    }
}

double leerDouble(const string& prompt) {
    while (true) {
        cout << prompt;
        double v;
        if (cin >> v) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida. Intenta de nuevo.\n";
    }
}

string leerStr(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// ================= Menús =================
void menuEnrutadores(Red& red) {
    while (true) {
        cout << "\n--- Enrutadores ---\n"
                "1) Agregar enrutador\n"
                "2) Eliminar enrutador\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (op == 1) {
                string id = leerStr("ID del enrutador: ");
                red.agregarEnrutador(id);
                cout << "OK\n";
            } else if (op == 2) {
                string id = leerStr("ID a eliminar: ");
                red.eliminarEnrutador(id);
                cout << "OK\n";
            } else if (op == 0) return;
            else cout << "Opción inválida.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void menuEnlaces(Red& red) {
    while (true) {
        cout << "\n--- Enlaces ---\n"
                "1) Agregar enlace (A-B costo)\n"
                "2) Actualizar costo\n"
                "3) Eliminar enlace\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (op == 1) {
                string a = leerStr("ID A: ");
                string b = leerStr("ID B: ");
                int c = leerInt("Costo (entero > 0): ");
                red.agregarEnlace(a, b, c);
                cout << "OK\n";
            } else if (op == 2) {
                string a = leerStr("ID A: ");
                string b = leerStr("ID B: ");
                int c = leerInt("Nuevo costo: ");
                red.actualizarCosto(a, b, c);
                cout << "OK\n";
            } else if (op == 3) {
                string a = leerStr("ID A: ");
                string b = leerStr("ID B: ");
                red.eliminarEnlace(a, b);
                cout << "OK\n";
            } else if (op == 0) return;
            else cout << "Opción inválida.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void menuRutas(Red& red) {
    while (true) {
        cout << "\n--- Rutas ---\n"
                "1) Recalcular todas las tablas (Dijkstra)\n"
                "2) Consultar ruta óptima ORIGEN->DESTINO\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (op == 1) {
                red.recalcularTablas();
                cout << "Tablas recalculadas.\n";
            } else if (op == 2) {
                string o = leerStr("ORIGEN: ");
                string d = leerStr("DESTINO: ");
                auto e = red.rutaOptima(o, d);
                if (e.costoTotal == INT_MAX || e.camino.empty()) {
                    cout << "Inalcanzable\n";
                } else {
                    cout << "Costo: " << e.costoTotal << " | Camino: ";
                    for (size_t i = 0; i < e.camino.size(); ++i) {
                        cout << e.camino[i] << (i + 1 < e.camino.size() ? " -> " : "");
                    }
                    cout << "\n";
                }
            } else if (op == 0) return;
            else cout << "Opción inválida.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void menuArchivos(Red& red) {
    while (true) {
        cout << "\n--- Archivos ---\n"
                "1) Cargar topología desde archivo\n"
                "2) Guardar topología a archivo\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (op == 1) {
                string ruta = leerStr("Archivo a cargar: ");
                red.cargarDesdeArchivo(ruta);
                cout << "OK. Topología cargada.\n";
            } else if (op == 2) {
                string ruta = leerStr("Archivo de salida: ");
                red.guardarEnArchivo(ruta);
                cout << "OK. Topología guardada.\n";
            } else if (op == 0) return;
            else cout << "Opción inválida.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void menuAleatoria(Red& red) {
    while (true) {
        cout << "\n--- Generación Aleatoria ---\n"
                "1) Generar red aleatoria (reemplaza la actual)\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (op == 1) {
            int N = leerInt("N (cantidad de enrutadores): ");
            double p = leerDouble("p (probabilidad de enlace 0..1): ");
            int cMax = leerInt("costoMax (1..): ");
            // Semilla para reproducibilidad (opcional)
            string usaSeed = leerStr("¿Usar semilla fija? (s/n): ");
            uint32_t seed = (usaSeed == "s" || usaSeed == "S")
                                ? static_cast<uint32_t>(leerInt("Semilla (entero): "))
                                : random_device{}();

            // Reiniciar red y generar
            red = Red{};
            vector<string> ids; ids.reserve(N);
            for (int i = 1; i <= N; ++i) {
                string id = "R" + to_string(i);
                ids.push_back(id);
                red.agregarEnrutador(id);
            }
            mt19937 rng(seed);
            uniform_real_distribution<double> U(0.0, 1.0);
            uniform_int_distribution<int> C(1, cMax);

            for (int i = 0; i < N; ++i) {
                for (int j = i + 1; j < N; ++j) {
                    if (U(rng) <= p) {
                        red.agregarEnlace(ids[i], ids[j], C(rng));
                    }
                }
            }
            cout << "OK. Red aleatoria generada (N=" << N << ", p=" << p
                 << ", costoMax=" << cMax << ", seed=" << seed << ").\n";
        } else if (op == 0) {
            return;
        } else {
            cout << "Opción inválida.\n";
        }
    }
}

void menuVer(Red& red) {
    while (true) {
        cout << "\n--- Ver/Imprimir ---\n"
                "1) Ver enlaces (red)\n"
                "2) Ver todas las tablas de enrutamiento\n"
                "0) Volver\n> ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (op == 1) {
            red.imprimirRed(cout);
        } else if (op == 2) {
            red.imprimirTablas(cout);
        } else if (op == 0) {
            return;
        } else {
            cout << "Opción inválida.\n";
        }
    }
}

// ================= Programa principal =================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(&cout); // <- vuelve a atar cin a cout

    Red red;
    cout << "=== Simulador de Red (Menú por secciones) ===\n";

    while (true) {
        cout << "\n=== Menú principal ===\n"
                "1) Enrutadores\n"
                "2) Enlaces\n"
                "3) Rutas\n"
                "4) Archivos\n"
                "5) Generación aleatoria\n"
                "6) Ver/Imprimir\n"
                "0) Salir\n> ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (op) {
        case 1: menuEnrutadores(red); break;
        case 2: menuEnlaces(red); break;
        case 3: menuRutas(red); break;
        case 4: menuArchivos(red); break;
        case 5: menuAleatoria(red); break;
        case 6: menuVer(red); break;
        case 0: cout << "Adiós.\n"; return 0;
        default: cout << "Opción inválida.\n"; break;
        }
    }
}
