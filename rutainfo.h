#ifndef RUTAINFO_H
#define RUTAINFO_H

#include <string>

struct RutaInfo {
    int costo;           // costo total hasta destino
    std::string siguiente;    // next-hop desde el router actual
};

#endif
