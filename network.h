#ifndef NETWORK_H
#define NETWORK_H
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <string>

using namespace  std;

class Network {
private:
    std::map<std::string, Router> routers;

    void dijkstra(const std::string& origen,
                  std::map<std::string, int>& dist,
                  std::map<std::string, std::string>& previo) const;

    std::vector<std::string> reconstruirCamino(const std::string& origen,
                                               const std::string& destino,
                                               const std::map<std::string, std::string>& previo) const;

public:
    void agregarRouter(const std::string& id);
    void eliminarRouter(const std::string& id);
    void conectar(const std::string& id1, const std::string& id2, int costo);
    void desconectar(const std::string& id1, const std::string& id2);
    void establecerCostoEnlace(const std::string& id1, const std::string& id2, int nuevoCosto);
    void actualizarTablas();
    int costoMinimo(const std::string& origen, const std::string& destino) const;

};

#endif // NETWORK_H
