#include "Red.h"
#include <iostream>
#include <climits>

using namespace std;

int main() {
    Red red;
    red.agregarEnrutador("A");
    red.agregarEnrutador("B");
    red.agregarEnrutador("C");
    red.agregarEnlace("A","B",4);
    red.agregarEnlace("B","C",3);
    red.agregarEnlace("A","C",10);

    red.recalcularTablas();

    red.imprimirRed(cout);
    cout << "\n";
    red.imprimirTablas(cout);

    auto r = red.rutaOptima("A","C");
    cout << "Ruta A->C | Costo: " << (r.costoTotal==INT_MAX? -1 : r.costoTotal) << " | Camino: ";
    for (size_t i=0;i<r.camino.size();++i) {
        cout << r.camino[i] << (i+1<r.camino.size()? " -> " : "");
    }
    cout << "\n";
    return 0;
}
