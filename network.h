#ifndef NETWORK_H
#include "Network.h"
#include "Router.h"
#include "RutaInfo.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <random>
#include <fstream>
#include <sstream>
#include <climits>
#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

class Network {
private:
    map<string, Router> routers;

    void dijkstra(const string& origen, map<string,int>& dist, map<string,string>& previo) const;
    vector<string> reconstruirCamino(const string& origen, const string& destino, const map<string,string>& previo) const;

public:
    void agregarRouter(const string& id);
    void eliminarRouter(const string& id);
    void conectar(const string& id1, const string& id2, int costo);
    void desconectar(const string& id1, const string& id2);
    void establecerCostoEnlace(const string& id1, const string& id2, int nuevoCosto);
    void actualizarTablas();

    int costoMinimo(const string& origen, const string& destino) const;
    vector<string> caminoMinimo(const string& origen, const string& destino) const;

    bool cargarDesdeArchivo(const string& rutaArchivo);
    void generarAleatoria(int n, double densidad, int maxCosto, uint64_t seed = std::random_device{}());

    void mostrarRed() const;
    void mostrarTablas() const;
};

#endif

