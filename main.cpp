#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include "network.h"

using namespace std;

string aMayusculas(const string& str) {
    string resultado = str;
    transform(resultado.begin(), resultado.end(), resultado.begin(),
              [](unsigned char c) { return toupper(c); });
    return resultado;
}

int leerEntero(const string& mensaje, int min=0, int max=10000) {
    int valor;
    while (true) {
        cout<<mensaje;
        if (cin >> valor && valor>=min && valor<=max) {
            return valor;
        } else {
            cout << "Entrada inválida, debe ser un numero. Intente de nuevo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int leerDouble(const string& mensaje) {
    double valor;
    while (true) {
        cout<<mensaje;
        if (cin >> valor && valor>0.0 && valor<1.0) {
            return valor;
        } else {
            cout << "Entrada invalida. Intente de nuevo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main(){

    Network net;
    int opcion;

    do{
        cout<<"\n================= RED DE ENRUTADORES =================\n";
        cout<<"1. Cargar red desde archivo\n";
        cout<<"2. Generar red aleatoria\n";
        cout<<"3. Agregar router\n";
        cout<<"4. Eliminar router\n";
        cout<<"5. Conectar routers\n";
        cout<<"6. Desconectar routers\n";
        cout<<"7. Consultar costo de enlace\n";
        cout<<"8. Consultar mejor camino entre ruters\n";
        cout<<"9. Modificar costo de enlace\n";
        cout<<"10. Mostrar red\n";
        cout<<"11. Mostrar tablas de conexiones\n";
        cout<<"12. Salir del sistema\n\n";

        opcion=leerEntero("Opcion: ");

        if (opcion==1){
            cout<<"Ingrese la ruta del archivo que contiene la red: ";
            string ruta;
            cin>>ruta;
            net.cargarDesdeArchivo(ruta);
            cout<<"Red cargada\n";
        }
        else if (opcion==2){
            int n=leerEntero("Numero de routers que tendra la red (min 2): ",2);
            double densidad=leerDouble("Densidad de conexioes (entre 0.0 y 1.0): ");
            int costo=leerEntero("Costo maximo que podria tener una conexion(min $1): ",1);
            net.generarAleatoria(n,densidad,costo);
            cout<<"Red generada\n";
        }
        else if (opcion==3){
            cout<<"Ingrese el ID del nuevo router: ";
            string id;
            cin>>id;
            id = aMayusculas(id);
            net.agregarRouter(id);
            cout<<"Router agregado\n";
        }
        else if (opcion==4){
            cout<<"Ingrese el ID del router a eliminar: ";
            string id;
            cin>>id;
            id = aMayusculas(id);
            if (net.eliminarRouter(id)){cout<<"Router eliminado\n";}
            else{cout<<"El router no exite en la red";}
        }
        else if (opcion==5){
            cout<<"Ingrese el ID del primer router: ";
            string id1;
            cin>>id1;
            id1 = aMayusculas(id1);
            cout<<"Ingrese el ID del segundo router: ";
            string id2;
            cin>>id2;
            id2 = aMayusculas(id2);
            int costo=leerEntero("Ingrese el costo de la conexion(min 1): ",1);
            net.conectar(id1,id2,costo);
            cout<<"Routers conectados\n";
        }
        else if (opcion==6){
            cout<<"Ingrese el ID del primer router: ";
            string id1;
            cin>>id1;
            id1 = aMayusculas(id1);
            cout<<"Ingrese el ID del segundo router: ";
            string id2;
            cin>>id2;
            id2 = aMayusculas(id2);
            if (net.desconectar(id1,id2)){cout<<"Routers desconectados\n";}
            else{cout<<"Routers no existentes en la red\n";}
        }
        else if (opcion==7){
            cout<<"Ingrese el ID del router origen: ";
            string id1;
            cin>>id1;
            id1 = aMayusculas(id1);
            cout<<"Ingrese el ID del router destino: ";
            string id2;
            cin>>id2;
            id2 = aMayusculas(id2);
            int costo=net.costoMinimo(id1,id2);
            if (costo==INT_MAX){cout<<"Routers no conectados\n";}
            else{cout<<"Costo: "<<costo<<endl;}
        }
        else if (opcion==8){
            cout<<"Ingrese el ID del router origen: ";
            string id1;
            cin>>id1;
            id1 = aMayusculas(id1);
            cout<<"Ingrese el ID del router destino: ";
            string id2;
            cin>>id2;
            id2 = aMayusculas(id2);
            auto camino=net.caminoMinimo(id1,id2);
            if (camino.empty()){cout<<"Routers no conectados\n";}
            else {
                cout << "Camino: ";
                for (size_t i = 0; i < camino.size(); ++i) {
                    cout << camino[i];
                    if (i < camino.size() - 1) cout << "->";
                }
                cout << endl;
            }
        }
        else if (opcion==9){
            cout<<"Ingrese el ID del primer router: ";
            string id1;
            cin>>id1;
            id1 = aMayusculas(id1);
            cout<<"Ingrese el ID del segundo router: ";
            string id2;
            cin>>id2;
            id2 = aMayusculas(id2);
            int costo=leerEntero("Ingrese el nuevo costo de la conexion(min $1): ",1);
            if (net.establecerCostoEnlace(id1,id2,costo)){cout<<"Nuevo costo establecido\n";}
            else{cout<<"Routers no existentes en la red\n";}
        }
        else if (opcion==10){
            net.mostrarRed();
        }
        else if (opcion==11){
            net.mostrarTablas();
        }
        else{cout<<"Opcion invalida, intente de nuevo.";}

    }while (opcion!=12);

}
