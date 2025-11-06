#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <map>
#include <vector>
#include <cstdint>  // Para uint64_t
#include "Router.h"

class Network {
private:
    std::map<std::string, Router> routers;

    void dijkstra(const std::string& origen,
                  std::map<std::string, int>& dist,
                  std::map<std::string, std::string>& previo) const;

    std::vector<std::string> reconstruirCamino(const std::string& origen, const std::string& destino,
                                               const std::map<std::string, std::string>& previo) const;

public:
    void agregarRouter(const std::string& id);
    void eliminarRouter(const std::string& id);
    void conectar(const std::string& id1, const std::string& id2, int costo);
    void desconectar(const std::string& id1, const std::string& id2);
    void establecerCostoEnlace(const std::string& id1, const std::string& id2, int nuevoCosto);
    void actualizarTablas();
    int costoMinimo(const std::string& origen, const std::string& destino) const;
    std::vector<std::string> caminoMinimo(const std::string& origen, const std::string& destino) const;
    bool cargarDesdeArchivo(const std::string& rutaArchivo);
    void generarAleatoria(int n, double densidad, int maxCosto, std::uint64_t seed = 0);
    void mostrarRed() const;
    void mostrarTablas() const;
};

#endif
