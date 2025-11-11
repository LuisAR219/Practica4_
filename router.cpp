#include "router.h"
#include <iostream>
#include <algorithm>

using namespace std;

Router::Router(): id("") {}

Router::Router(string id): id(id){}

void Router::agregarVecino(const string& vecino, int costo){

    for (auto& par : vecinos) {
        if (par.first == vecino) {
            par.second = costo;
            return;
        }
    }
    vecinos.push_back({vecino, costo});

}

void Router::eliminarVecino(const string &vecino){

    for (auto it = vecinos.begin(); it != vecinos.end(); ) {
        if (it->first == vecino) {
            it = vecinos.erase(it);
        } else {
            ++it;
        }
    }

}

void Router::mostrarTabla()const{

    cout<<"Tabla de enrutamiento de "<<id<<":\n";
    cout<<"\nDestino\tCosto\tSiguiente\n";

    for (const auto& [destino,info]: tabla){
        cout<<destino<<"\t"<<info.costo<<"\t"<<(info.siguiente.empty()?"-":info.siguiente)<<endl;
    }
    cout<<endl;

}
