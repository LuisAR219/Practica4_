#include "network.h"



void Network::dijkstra(const string& origen,
                       map<string, int>& dist,
                       map<string, string>& previo) const {
    dist.clear(); previo.clear();
    set<string> visitados;
    for (const auto& [id, _] : routers) dist[id] = INT_MAX;
    if (!routers.count(origen)) return;
    dist[origen] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, origen});

    while (!pq.empty()) {
        auto [costo, actual] = pq.top(); pq.pop();
        if (visitados.count(actual)) continue;
        visitados.insert(actual);

        for (const auto& [vecino, c] : routers.at(actual).vecinos) {
            int nuevoCosto = costo + c;
            if (nuevoCosto < dist[vecino]) {
                dist[vecino] = nuevoCosto;
                previo[vecino] = actual;
                pq.push({nuevoCosto, vecino});
            }
        }
    }
}

vector<string> Network::reconstruirCamino(const string& origen,
                                          const string& destino,
                                          const map<string, string>& previo) const {
    vector<string> camino;
    if (!routers.count(origen) || !routers.count(destino)) return camino;

    string at = destino;
    while (true) {
        camino.push_back(at);
        if (at == origen) break;
        auto it = previo.find(at);
        if (it == previo.end()) {
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

void Network::eliminarRouter(const string& id) {
    if (!routers.count(id)) return;
    for (auto& [rid, r] : routers) {
        if (rid != id) r.eliminarVecino(id);
    }
    routers.erase(id);
    actualizarTablas();
}

void Network::conectar(const string& id1, const string& id2, int costo) {
    if (!routers.count(id1)) agregarRouter(id1);
    if (!routers.count(id2)) agregarRouter(id2);
    routers[id1].agregarVecino(id2, costo);
    routers[id2].agregarVecino(id1, costo);
    actualizarTablas();
}

void Network::desconectar(const string& id1, const string& id2) {
    if (!routers.count(id1) || !routers.count(id2)) return;
    routers[id1].eliminarVecino(id2);
    routers[id2].eliminarVecino(id1);
    actualizarTablas();
}

void Network::establecerCostoEnlace(const string& id1, const string& id2, int nuevoCosto) {
    if (!routers.count(id1) || !routers.count(id2)) return;
    routers[id1].agregarVecino(id2, nuevoCosto);
    routers[id2].agregarVecino(id1, nuevoCosto);
    actualizarTablas();
}

void Network::actualizarTablas() {
    for (auto& [id, router] : routers) {
        map<string, int> dist;
        map<string, string> previo;
        dijkstra(id, dist, previo);
        router.limpiarTabla();

        for (const auto& [dest, costo] : dist) {
            if (costo == INT_MAX) continue;
            string siguienteHop;
            if (dest == id) {
                siguienteHop = "";
            } else {
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
