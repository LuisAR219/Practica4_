#include "Network.h"
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <random>  // Para std::random_device, mt19937_64, etc.

using namespace std;

void Network::dijkstra(const string& origen,
                       map<string, int>& dist,
                       map<string, string>& previo) const {
    dist.clear(); previo.clear();
    set<string> visitados;

    for (auto& [id, r] : routers) dist[id] = INT_MAX;
    if (!routers.count(origen)) return;
    dist[origen] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, origen});

    while (!pq.empty()) {
        auto [costo, actual] = pq.top(); pq.pop();
        if (visitados.count(actual)) continue;
        visitados.insert(actual);

        const auto& rActual = routers.at(actual);
        for (auto& [vecino, costoEnlace] : rActual.vecinos) {
            if (!routers.count(vecino)) continue;
            int nuevoCosto = costo + costoEnlace;
            if (nuevoCosto < dist[vecino]) {
                dist[vecino] = nuevoCosto;
                previo[vecino] = actual;
                pq.push({nuevoCosto, vecino});
            }
        }
    }
}

vector<string> Network::reconstruirCamino(const string& origen, const string& destino,
                                          const map<string, string>& previo) const {
    vector<string> camino;
    if (!routers.count(origen) || !routers.count(destino)) return camino;

    string at = destino;
    while (true) {
        camino.push_back(at);
        if (at == origen) break;
        auto it = previo.find(at);
        if (it == previo.end()) { // no hay camino
            camino.clear();
            return camino;
        }
        at = it->second;
    }
    reverse(camino.begin(), camino.end());
    return camino;
}

void Network::agregarRouter(const string& id) {
    routers.emplace(id, Router(id));
}

bool Network::eliminarRouter(const string& id) {
    if (!routers.count(id)) return false;
    // quitar referencias en vecinos de otros routers
    for (auto& [rid, r] : routers) {
        if (rid == id) continue;
        r.eliminarVecino(id);
    }
    routers.erase(id);
    // tras cambios, actualizar tablas
    actualizarTablas();
    return true;
}

void Network::conectar(const string& id1, const string& id2, int costo) {
    if (!routers.count(id1)) agregarRouter(id1);
    if (!routers.count(id2)) agregarRouter(id2);
    routers[id1].agregarVecino(id2, costo);
    routers[id2].agregarVecino(id1, costo);
    actualizarTablas();
}

bool Network::desconectar(const string& id1, const string& id2) {
    if (!routers.count(id1) || !routers.count(id2)) return false;
    routers[id1].eliminarVecino(id2);
    routers[id2].eliminarVecino(id1);
    actualizarTablas();
    return true;
}

bool Network::establecerCostoEnlace(const string& id1, const string& id2, int nuevoCosto) {
    if (!routers.count(id1) || !routers.count(id2)) return false;
    routers[id1].agregarVecino(id2, nuevoCosto);
    routers[id2].agregarVecino(id1, nuevoCosto);
    actualizarTablas();
    return true;
}

void Network::actualizarTablas() {
    for (auto& [id, router] : routers) {
        map<string, int> dist;
        map<string, string> previo;
        dijkstra(id, dist, previo);

        router.limpiarTabla();

        for (const auto& [dest, costo] : dist) {
            if (costo == INT_MAX) continue; // inalcanzable
            string siguienteHop;
            if (dest == id) {
                siguienteHop = ""; // self
            } else {
                // encontrar el first hop: desde 'id' hacia 'dest'
                auto camino = reconstruirCamino(id, dest, previo);
                if (camino.size() >= 2) siguienteHop = camino[1];
            }
            router.tabla[dest] = RutaInfo{costo, siguienteHop};
        }
    }
}

int Network::costoMinimo(const string& origen, const string& destino) const {
    if (!routers.count(origen) || !routers.count(destino)) return INT_MAX;
    const auto& tabla = routers.at(origen).tabla;
    auto it = tabla.find(destino);
    return (it == tabla.end()) ? INT_MAX : it->second.costo;
}

vector<string> Network::caminoMinimo(const string& origen, const string& destino) const {
    if (!routers.count(origen) || !routers.count(destino)) return {};
    // podemos reconstruir directamente vía Dijkstra para obtener camino completo
    map<string, int> dist;
    map<string, string> previo;
    dijkstra(origen, dist, previo);
    return reconstruirCamino(origen, destino, previo);
}

bool Network::cargarDesdeArchivo(const string& rutaArchivo) {
    ifstream in(rutaArchivo);
    if (!in.is_open()) return false;

    // limpiar red actual
    routers.clear();

    string linea;
    while (getline(in, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string token; ss >> token;

        if (token == "NODES") {
            string id;
            while (ss >> id) agregarRouter(id);
        } else if (token == "LINK") {
            string a, b; int c;
            ss >> a >> b >> c;
            if (ss) conectar(a, b, c); // conectar ya actualiza tablas
        }
    }

    // asegurar tablas consistentes
    actualizarTablas();
    return true;
}

void Network::generarAleatoria(int n, double densidad, int maxCosto, std::uint64_t seed) {
    routers.clear();
    if (n <= 0 || maxCosto <= 0) return;

    // Si seed es 0, usar random_device
    if (seed == 0) {
        std::random_device rd;
        seed = rd();
    }

    // crear routers con ids "A","B","C",...
    for (int i = 0; i < n; ++i) {
        string id(1, char('A' + (i % 26)));
        if (i >= 26) id += to_string(i / 26); // evita duplicados tras Z
        agregarRouter(id);
    }

    vector<string> ids;
    ids.reserve(routers.size());
    for (auto& [id, r] : routers) ids.push_back(id);

    std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> costoDist(1, maxCosto);

    for (size_t i = 1; i < ids.size(); ++i) {
        std::uniform_int_distribution<size_t> previoDist(0, i - 1);
        string otro = ids[previoDist(gen)];
        int c = costoDist(gen);
        conectar(ids[i], otro, c);
    }

    // conectar pares según densidad
    for (size_t i = 0; i < ids.size(); ++i) {
        for (size_t j = i + 1; j < ids.size(); ++j) {
            if (prob(gen) <= densidad) {
                int c = costoDist(gen);
                conectar(ids[i], ids[j], c);
            }
        }
    }

    actualizarTablas();
}

void Network::mostrarRed() const {
    cout << "Topologia:\n";
    for (const auto& [id, r] : routers) {
        cout << id << ": ";
        for (const auto& [v, c] : r.vecinos) cout << "(" << v << "," << c << ") ";
        cout << "\n";
    }
    cout << endl;
}

void Network::mostrarTablas() const {
    for (const auto& [id, r] : routers) r.mostrarTabla();
}
