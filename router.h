#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <list>
#include "rutainfo.h"

using namespace std;

class Router{

public:

    string id;
    list<pair<string,int>> vecinos;
    map<string,RutaInfo> tabla;

    Router();
    Router(string id);
    void agregarVecino(const string& vecino, int costo);
    void eliminarVecino(const string& vecino);
    void limpiarTabla(){tabla.clear();}
    void mostrarTabla()const;

};

#endif // ROUTER_H
