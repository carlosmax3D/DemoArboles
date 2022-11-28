#include <iostream>
#include <string>
#include <cstring>

using namespace std;

//#define strcpy_s(x,y,z) strcpy(x,z)
//#define _stricmp(x,y) strcmp(x,y)

struct PERSONA {
    char nombre[255] = { '\0' };
    int edad = 0;
    float peso = 0;
    int idPersona = 0;
    void setNombre(string nombre) {
        strcpy_s(this->nombre, 255, nombre.c_str());
        if (nombre.size() > 255)
            this->nombre[254] = '\0';
    }
    string getNombre() {
        string nombre(this->nombre);
        return nombre;
    }
    static PERSONA* nuevaPersona(string nombre, int edad, float peso) {
        PERSONA* nuevo = new PERSONA;
        if (nuevo == NULL) {
            cout << "Error, no se pudo crear la persona en memoria" << endl;
            return NULL;
        }
        nuevo->setNombre(nombre);
        nuevo->edad = edad;
        nuevo->peso = peso;
        return nuevo;
    }
    void imprimirPersonaId() {
        cout << "(" << idPersona << ", " << getNombre() << ", " << edad << ", " << peso << ")";
    }
    string getDataPersona() {
        return "ID Persona: " + to_string(idPersona) + " Nombre: " + getNombre() + "  Edad: " + to_string(edad) + " Peso: " + to_string(peso);
    }
};
struct NODOPERSONAS {
    PERSONA* dato = NULL;
    NODOPERSONAS* anterior = NULL;
    NODOPERSONAS* siguiente = NULL;
    static NODOPERSONAS* nuevoNodoPersona(string nombre, int edad, float peso) {
        PERSONA* nuevo = PERSONA::nuevaPersona(nombre, edad, peso);
        return nuevoNodoPersona(nuevo);
    }
    static NODOPERSONAS* nuevoNodoPersona(PERSONA* nuevo) {
        NODOPERSONAS* nodo = new NODOPERSONAS;
        nodo->dato = nuevo;
        nodo->siguiente = NULL;
        nodo->anterior = NULL;
        return nodo;
    }
    void imprimirPersona() {
        NODOPERSONAS* persona = this;
        if (persona->anterior == NULL)
            cout << "NULL<-";
        else {
            cout << "(" << persona->anterior->dato->getNombre() << ", " << persona->anterior->dato->edad << ", " << persona->anterior->dato->peso << ")";
            if (persona->anterior->siguiente == persona)
                cout << " <-> ";
            else cout << " - ";
        }
        cout << "(" << persona->dato->getNombre() << ", " << persona->dato->edad << ", " << persona->dato->peso << ")";
        if (persona->siguiente == NULL)
            cout << " -> NULL";
        else {
            if (persona->siguiente->anterior == persona)
                cout << " <-> ";
            else cout << " - ";
            cout << "(" << persona->siguiente->dato->getNombre() << ", " << persona->siguiente->dato->edad << ", " << persona->siguiente->dato->peso << ")";
        }
    }
};
struct PERSONAS {
    NODOPERSONAS* ORIGEN = NULL;
    NODOPERSONAS* FIN = NULL;
    static int maxId;
    void agregarPersona(string nombre, int edad, float peso) {
        agregarPersona(nombre, edad, peso, 0);
    }
    void agregarPersona(string nombre, int edad, float peso, int idPersona) {
        NODOPERSONAS* nuevo = NODOPERSONAS::nuevoNodoPersona(nombre, edad, peso);
        if (nuevo == NULL) {
            exit(-1);
        }
        agregarPersona(nuevo, idPersona);
    }
    void agregarPersona(NODOPERSONAS* nuevo) {
        agregarPersona(nuevo, 0);
    }
    void agregarPersona(NODOPERSONAS* nuevo, int idPersona) {
        if (idPersona == 0) {
            PERSONAS::maxId++;
            nuevo->dato->idPersona = PERSONAS::maxId;
        }
        else {
            nuevo->dato->idPersona = idPersona;
            PERSONAS::maxId = PERSONAS::maxId < idPersona ? idPersona : PERSONAS::maxId;
        }
        if (ORIGEN == NULL) {
            ORIGEN = nuevo;
            FIN = nuevo;
            nuevo->siguiente = NULL;
            nuevo->anterior = NULL;
        }
        else {
            FIN->siguiente = nuevo;
            nuevo->anterior = FIN;
            FIN = nuevo;
            nuevo->siguiente = NULL;
        }
    }
    PERSONA* buscarPersona(string nombre) {
        NODOPERSONAS* indice = ORIGEN;
        while (indice != NULL) {
            if (_stricmp(nombre.c_str(), indice->dato->nombre) == 0)
                return indice->dato;
            indice = indice->siguiente;
        }
        return NULL;
    }
    NODOPERSONAS* buscarNodoPersona(string nombre) {
        NODOPERSONAS* indice = ORIGEN;
        while (indice != NULL) {
            if (_stricmp(nombre.c_str(), indice->dato->nombre) == 0)
                return indice;
            indice = indice->siguiente;
        }
        return indice;
    }
    bool modificarPersona(string nombreBuscar, string nombre, int edad, float peso) {
        PERSONA* persona = buscarPersona(nombreBuscar);
        return modificarPersona(persona, nombre, edad, peso);
    }
    bool modificarPersona(PERSONA* persona, string nombre, int edad, float peso) {
        if (persona != NULL) {
            persona->setNombre(nombre);
            persona->edad = edad;
            persona->peso = peso;
            return true;
        }
        return false;
    }
    PERSONA *eliminarPersona(string nombreBuscar) {
        NODOPERSONAS* persona = buscarNodoPersona(nombreBuscar);
        PERSONA *per = NULL;
        if (persona != NULL) {
            per = persona->dato;
            if (persona->anterior == NULL) {
                ORIGEN = persona->siguiente;
                if (persona == FIN) FIN = NULL;
                else ORIGEN->anterior = NULL;
                delete persona;
            }if (persona->siguiente == NULL) {
                FIN = persona->anterior;
                if (FIN != NULL)
                    FIN->siguiente = NULL;
                delete persona;
            } else {
                NODOPERSONAS* anterior = persona->anterior,
                    * siguiente = persona->siguiente;
                anterior->siguiente = siguiente;
                siguiente->anterior = anterior;
                delete persona;
            }
        }
        return per;
    }
    void imprimirPersonas() {
        NODOPERSONAS* indice = this->ORIGEN;
        cout << "\n\t--INICIO--" << endl;
        if (indice == NULL)
            cout << "NULL" << endl;
        while (indice != NULL) {
            indice->imprimirPersona();
            cout << endl;
            indice = indice->siguiente;
        }
        cout << "\t--FIN--" << endl;
    }
    void eliminarTodo() {
        NODOPERSONAS* indice = ORIGEN;
        while (indice != NULL) {
            NODOPERSONAS* actual = indice;
            indice = indice->siguiente;
            delete actual;
        }
        ORIGEN = NULL;
        FIN = NULL;
    }
}listaPersonas, listaHeap;
int PERSONAS::maxId = 0;

struct NODOARBOLPERSONA {
    NODOARBOLPERSONA* PADRE = NULL;
    NODOARBOLPERSONA* IZQUIERDO = NULL;
    NODOARBOLPERSONA* DERECHO = NULL;
    PERSONA* PER = NULL;
    int balance;
    static NODOARBOLPERSONA* nuevoNodo(struct PERSONA* info) {
        NODOARBOLPERSONA* nuevo = new NODOARBOLPERSONA;
        if (nuevo == NULL)
            exit(-1);
        nuevo->PER = info;
        nuevo->PADRE = NULL;
        nuevo->IZQUIERDO = NULL;
        nuevo->DERECHO = NULL;
        return nuevo;
    }
};
struct ARBOLPERSONAS {
    NODOARBOLPERSONA* RAIZ = NULL;
    bool balanceo = false;
    /* Rotaci�n Izquierda *
    *  A           B
    * / \         / \
    * a  B ==>   A   c
    *   / \     / \
    *   b  c    a  b
    * S�lo cambian los factores de balance de los nodos A y B
    * Los factores de balance de los sub-�rboles no cambian. */
    NODOARBOLPERSONA* rotacionIzquierda(NODOARBOLPERSONA* t) {
        NODOARBOLPERSONA* temp, * padre = t->PADRE;
        int x, y;
        temp = t;
        t = t->DERECHO;
        temp->DERECHO = t->IZQUIERDO;
        t->IZQUIERDO = temp;
        //Recalcula factores de balance de los dos nodos
        x = temp->balance; // oldbal(A)
        y = t->balance; // oldbal(B)
        temp->balance = x - 1 - max(y, 0); // nA
        t->balance = min(x - 2 + min(y, 0), y - 1); // nB

        return t;
    }

    /* Rotaci�n derecha
    *
    *   A         B
    *  / \       / \
    *  B  c ==> a   A
    * / \          / \
    * a  b        b   c
    *
    */
    NODOARBOLPERSONA* rotacionDerecha(NODOARBOLPERSONA* t) {
        NODOARBOLPERSONA* temp = t, * padre = t->PADRE;
        int x, y;
        t = t->IZQUIERDO;
        temp->IZQUIERDO = t->DERECHO;
        t->DERECHO = temp;
        x = temp->balance; // oldbal(A)
        y = t->balance;    // oldbal(B)
        temp->balance = x + 1 - min(y, 0);  // nA
        t->balance = max(x + 2 + max(y, 0), y + 1); //nB
        return t;
    }
    int calcularEquilibrios(NODOARBOLPERSONA* t) {
        if (t == NULL)
            return 0;
        int eIzquierdo = calcularEquilibrios(t->IZQUIERDO) + 1;
        int eDerecho = calcularEquilibrios(t->DERECHO) + 1;
        t->balance = eDerecho - eIzquierdo;
        return eIzquierdo > eDerecho ? eIzquierdo : eDerecho;
    }
    NODOARBOLPERSONA* buscarNodo(NODOARBOLPERSONA* nodo, PERSONA* persona) {
        if (nodo == NULL)
            return NULL;
        if (nodo->PER->idPersona > persona->idPersona)
            return buscarNodo(nodo->IZQUIERDO, persona);
        else if (nodo->PER->idPersona < persona->idPersona)
            return buscarNodo(nodo->DERECHO, persona);
        return nodo;
    }
    NODOARBOLPERSONA* buscarPersona(PERSONA* persona) {
        return buscarNodo(RAIZ, persona);
    }
    NODOARBOLPERSONA* buscarPersona(int idPersona) {
        PERSONA personaVacia;
        personaVacia.idPersona = idPersona;
        return buscarNodo(RAIZ, &personaVacia);
    }
    void eliminarNodo(NODOARBOLPERSONA** nodo, NODOARBOLPERSONA* padre, NODOARBOLPERSONA* eliminar) {
        NODOARBOLPERSONA* elemento = *nodo;
        if (elemento == NULL) {
            this->balanceo = false;
        }
        else if (elemento->PER->idPersona < eliminar->PER->idPersona) {
            eliminarNodo(&(elemento->DERECHO), elemento, eliminar);
            elemento->balance -= 1;
        }
        else if (elemento->PER->idPersona > eliminar->PER->idPersona) {
            eliminarNodo(&(elemento->IZQUIERDO), elemento, eliminar);
            elemento->balance += 1;
        }
        else {
            if (elemento->IZQUIERDO == NULL) {
                NODOARBOLPERSONA* tmp = elemento;
                (*nodo) = tmp->DERECHO;
                delete tmp;
                this->balanceo = true;
                return;
            }
            else if (elemento->DERECHO == NULL) {
                NODOARBOLPERSONA* tmp = elemento;
                (*nodo) = tmp->IZQUIERDO;
                delete tmp;
                this->balanceo = true;
                return;
            }
            else   /* Tiene dos hijos */ {
                if (elemento->balance < 0) {
                    /* Si cargado a la izquierda, elimina mayor descendiente hijo izq */
                    NODOARBOLPERSONA* tmp = elemento;
                    tmp = (*nodo)->IZQUIERDO;
                    while (tmp->DERECHO != NULL) tmp = tmp->DERECHO;
                    (*nodo)->PER = tmp->PER;
                    eliminarNodo(&((*nodo)->DERECHO), (*nodo), tmp);
                    elemento = (*nodo);
                    elemento->balance += 1;
                }
                else {
                    /* Si cargado a la derecha, elimina menor descendiente hijo der */
                    NODOARBOLPERSONA* tmp = elemento;
                    tmp = (*nodo)->IZQUIERDO;
                    while (tmp->IZQUIERDO != NULL) tmp = tmp->IZQUIERDO;
                    (*nodo)->PER = tmp->PER;
                    eliminarNodo(&((*nodo)->IZQUIERDO), (*nodo), tmp);
                    elemento = (*nodo);
                    elemento->balance -= 1;
                }
            }
        }
        /* Mantiene �rbol balanceado avl. S�lo una o dos rotaciones por descarte */
        if (this->balanceo == true) {
            /* Hay que revisar factores de balance en el ascenso*/
            if (elemento->balance < -1)  /* Si qued� desbalanceado por la izquierda y dej� de ser AVL */ {
                if (elemento->IZQUIERDO->balance > 0)  /*espejos casos c, d y e */ {
                    /* Si el hijo izquierdo est� cargado a la derecha */
                    elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
                    this->balanceo = true; /*Continuar revisando factores */
                }
                else if (elemento->IZQUIERDO->balance == 0)
                    this->balanceo = false; /*No debe seguir el rebalance */
                else
                    this->balanceo = true;/* Debe seguir revisando factores de balance */
                (*nodo) = rotacionDerecha(*nodo);
            }
            else if (elemento->balance > 1)  /* Si qued� desbalaceado por la derecha */ {
                if (elemento->DERECHO->balance < 0) {
                    /* Si hijo derecho est� cargado a la izquierda */
                    (*nodo)->DERECHO = rotacionDerecha((*nodo)->DERECHO);
                    this->balanceo = true; //debe seguir revisando. Caso d.
                }
                else if (elemento->DERECHO->balance == 0)
                    this->balanceo = false; /* No debe seguir el rebalance. Caso c. */
                else //positivo
                    this->balanceo = true;/* Debe seguir revisando factores de balance. Caso e. */
                (*nodo) = rotacionIzquierda(*nodo);
            }
            else if (elemento->balance == 0) /* Si estaba en +1 � -1 y queda en cero */
                this->balanceo = true; /* Debe seguir corrigiendo. Caso b.*/
            else /* Si estaba en cero y queda en -1 � +1 */
                this->balanceo = false; /* No debe seguir rebalanceando. Caso a.*/
        }
    }
    void borrarNodo(PERSONA* eliminar) {
        NODOARBOLPERSONA* nodo = buscarPersona(eliminar);
        if (nodo != NULL) {
            calcularEquilibrios(RAIZ);
            eliminarNodo(&RAIZ, NULL, nodo);
        }
    }
    void insertarNodo(NODOARBOLPERSONA** nodo, NODOARBOLPERSONA* padre, NODOARBOLPERSONA* nuevo) {
        NODOARBOLPERSONA* elemento = *nodo;
        if (elemento == NULL) {
            *nodo = nuevo;
            elemento = nuevo;
            nuevo->PADRE = padre;
            nuevo->balance = 0;
            balanceo = true;
            return;
        }
        else {
            if (elemento->PER->idPersona > nuevo->PER->idPersona) {
                insertarNodo(&(elemento->IZQUIERDO), elemento, nuevo);
                elemento->balance -= 1;
            }
            else {
                insertarNodo(&(elemento->DERECHO), elemento, nuevo);
                elemento->balance += 1;
            }
        }
        if (this->balanceo == true) {
            /*El c�digo a continuaci�n es el costo adicional para mantener propiedad AVL */
            /* Mantiene �rbol balanceado avl. S�lo una o dos rotaciones por inserci�n */
            if (elemento->balance < -1) {
                /* Qued� desbalanceado por la izquierda. Espejos Casos c y d.*/
                if (elemento->IZQUIERDO->balance > 0) {
                    /* Si hijo izquierdo est� cargado a la derecha */
                    elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
                    elemento->IZQUIERDO->PADRE = elemento;
                    if (elemento->IZQUIERDO->DERECHO != NULL)
                        elemento->IZQUIERDO->DERECHO->PADRE = elemento->IZQUIERDO;
                    if (elemento->IZQUIERDO->IZQUIERDO != NULL)
                        elemento->IZQUIERDO->IZQUIERDO->PADRE = elemento->IZQUIERDO;
                }
                *nodo = rotacionDerecha(elemento);
                this->balanceo = false; /* El sub�rbol no aumenta su altura */
                (*nodo)->PADRE = padre;
                if ((*nodo)->IZQUIERDO != NULL)
                    (*nodo)->IZQUIERDO->PADRE = (*nodo);
                if ((*nodo)->DERECHO != NULL)
                    (*nodo)->DERECHO->PADRE = (*nodo);
            }
            else if (elemento->balance > 1) {
                /* Si qued� desbalanceado por la derecha: Casos c y d.*/
                if (elemento->DERECHO->balance < 0) {
                    /* Si hijo derecho est� cargado a la izquierda Caso d.*/
                    elemento->DERECHO = rotacionDerecha(elemento->DERECHO);
                    elemento->DERECHO->PADRE = elemento;
                    if (elemento->DERECHO->IZQUIERDO != NULL)
                        elemento->DERECHO->IZQUIERDO->PADRE = elemento->DERECHO;
                    if (elemento->DERECHO->DERECHO != NULL)
                        elemento->DERECHO->DERECHO->PADRE = elemento->DERECHO;
                }
                *nodo = rotacionIzquierda(elemento); /* Caso c.*/
                this->balanceo = false; /* El sub�rbol no aumenta su altura */
                (*nodo)->PADRE = padre;
                if ((*nodo)->IZQUIERDO != NULL)
                    (*nodo)->IZQUIERDO->PADRE = (*nodo);
                if ((*nodo)->DERECHO != NULL)
                    (*nodo)->DERECHO->PADRE = (*nodo);
            }
            else if (elemento->balance == 0)/* La inserci�n lo balanceo */
                this->balanceo = false; /* El sub�rbol no aumenta su altura. Caso a*/
            else /* Qued� desbalanceado con -1 � +1 Caso b */
                this->balanceo = true; /* Propaga ascendentemente la necesidad de rebalancear */
        }
    }
    void agregarNodo(PERSONA* info) {
        NODOARBOLPERSONA* nuevo = NODOARBOLPERSONA::nuevoNodo(info);
        insertarNodo(&RAIZ, NULL, nuevo);
        calcularEquilibrios(RAIZ);
    }
    void lista2Arbol(NODOPERSONAS* listaDoble) {
        eliminarArbol();
        NODOPERSONAS* persona = listaDoble;
        while (persona != NULL) {
            agregarNodo(persona->dato);
            persona = persona->siguiente;
        }
    }
    void eliminarArbolNodos(NODOARBOLPERSONA* nodo) {
        if (nodo == NULL) {
            return;
        }
        else {
            eliminarArbolNodos(nodo->IZQUIERDO);
            eliminarArbolNodos(nodo->DERECHO);
            delete(nodo);
        }
    }
    void eliminarArbol() {
        eliminarArbolNodos(RAIZ);
        RAIZ = NULL;
    }
    void imprimirNodos(NODOARBOLPERSONA* nodo, int espacios) {
        if (nodo == NULL)
            return;
        string espacio = "";
        for (int i = 0; i < espacios; i++)
            espacio.append(" ");
        cout << espacio << "+";
        nodo->PER->imprimirPersonaId();
        if (nodo->IZQUIERDO != NULL || nodo->DERECHO != NULL) {
            cout << endl << espacio << " |\n";
            imprimirNodos(nodo->IZQUIERDO, espacios + 2);
            imprimirNodos(nodo->DERECHO, espacios + 2);
            cout << espacio << " -\n";
        }
        else cout << endl;
    }
    void imprimirArbol() {
        cout << "\n\t--INICIO--" << endl;
        imprimirNodos(RAIZ, 0);
        cout << "\t--FIN--" << endl;
    }
    string recorridoInfijoReporteHeap(NODOARBOLPERSONA* nodo) {
        if (nodo == NULL)
            return "";
        string texto = nodo->PER->getDataPersona() + " \n\r\n\r";
        if (nodo->IZQUIERDO != NULL || nodo->DERECHO != NULL) {
            texto = recorridoInfijoReporteHeap(nodo->IZQUIERDO) + texto;
            texto = texto + recorridoInfijoReporteHeap(nodo->DERECHO);
        }
        return texto;
    }
    void recorridoInfijo2Lista(NODOARBOLPERSONA* nodo, PERSONAS* listaHeap) {
        if (nodo == NULL)
            return;
        if (nodo->IZQUIERDO != NULL || nodo->DERECHO != NULL) {
            recorridoInfijo2Lista(nodo->IZQUIERDO, listaHeap);
            listaHeap->agregarPersona(NODOPERSONAS::nuevoNodoPersona(nodo->PER), nodo->PER->idPersona);
            recorridoInfijo2Lista(nodo->DERECHO , listaHeap);
        }
        else
            listaHeap->agregarPersona(NODOPERSONAS::nuevoNodoPersona(nodo->PER), nodo->PER->idPersona);
    }

}arbolPersonas;

int main(int arg, char** argv) {
    listaPersonas.agregarPersona("Jose", 18, 63, 5);
    listaPersonas.agregarPersona("Edna", 21, 63, 6);
    listaPersonas.agregarPersona("Monse", 20, 50, 7);
    listaPersonas.agregarPersona("Carlos", 27, 70, 1);
    listaPersonas.agregarPersona("Manuel", 20, 65, 2);
    listaPersonas.agregarPersona("Adriana", 18, 50, 3);
    listaPersonas.agregarPersona("Alondra", 19, 50, 4);
    arbolPersonas.lista2Arbol(listaPersonas.ORIGEN);
    cout << endl << endl;
    cout << arbolPersonas.recorridoInfijoReporteHeap(arbolPersonas.RAIZ);
    arbolPersonas.recorridoInfijo2Lista(arbolPersonas.RAIZ, &listaHeap);
    listaHeap.imprimirPersonas();
    cout << endl << endl;
    listaPersonas.imprimirPersonas();
    arbolPersonas.imprimirArbol();

    arbolPersonas.borrarNodo(listaPersonas.buscarPersona("Manuel"));

    arbolPersonas.imprimirArbol();
    arbolPersonas.eliminarArbol();
    listaHeap.eliminarTodo();
    NODOPERSONAS* indice = listaPersonas.ORIGEN;
    while (indice != NULL) {
        PERSONA* p = indice->dato;
        NODOPERSONAS* temp = indice;
        if (p != NULL)
            delete p;
        indice = indice->siguiente;
        delete temp;
    }
}