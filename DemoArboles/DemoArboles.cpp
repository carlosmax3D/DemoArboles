#include <iostream>
#include <string>

using namespace std;

struct PERSONA {
    char nombre[255] = {'\0'};
    int edad = 0;
    float peso = 0;
    int idPersona = 0;
    PERSONA* siguiente = NULL;
    PERSONA* anterior = NULL;
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
        nuevo->siguiente = NULL;
        nuevo->anterior = NULL;
        return nuevo;
    }
    void imprimirPersona() {
        PERSONA* persona = this;
        if (persona->anterior == NULL)
            cout << "NULL<-";
        else {
            cout << "(" << persona->anterior->getNombre() << ", " << persona->anterior->edad << ", " << persona->anterior->peso << ")";
            if (persona->anterior->siguiente == persona)
                cout << " <-> ";
            else cout << " - ";
        }
        cout << "(" << persona->getNombre() << ", " << persona->edad << ", " << persona->peso << ")";
        if (persona->siguiente == NULL)
            cout << " -> NULL";
        else {
            if (persona->siguiente->anterior == persona)
                cout << " <-> ";
            else cout << " - ";
            cout << "(" << persona->siguiente->getNombre() << ", " << persona->siguiente->edad << ", " << persona->siguiente->peso << ")";
        }
    }
    void imprimirPersonaId() {
        cout << "(" << idPersona << ", " << getNombre() << ", " << edad << ", " << peso << ")";
    }
};
struct PERSONAS {
    PERSONA* ORIGEN = NULL;
    PERSONA* FIN = NULL;
    static int maxId;
    void agregarPersona(string nombre, int edad, float peso) {
        agregarPersona(nombre, edad, peso, 0);
    }
    void agregarPersona(string nombre, int edad, float peso, int idPersona) {
        PERSONA* nuevo = PERSONA::nuevaPersona(nombre, edad, peso);
        if (nuevo == NULL) {
            exit(-1);
        }
        agregarPersona(nuevo, idPersona);
    }
    void agregarPersona(PERSONA* nuevo) {
        agregarPersona(nuevo, 0);
    }
    void agregarPersona(PERSONA* nuevo, int idPersona) {
        if (idPersona == 0) {
            PERSONAS::maxId++;
            nuevo->idPersona = PERSONAS::maxId;
        } else {
            nuevo->idPersona = idPersona;
            PERSONAS::maxId = PERSONAS::maxId < idPersona ? idPersona : PERSONAS::maxId;
        }
        if (ORIGEN == NULL) {
            ORIGEN = nuevo;
            FIN = nuevo;
        } else {
            FIN->siguiente = nuevo;
            nuevo->anterior = FIN;
            FIN = nuevo;
        }
    }
    PERSONA* buscarPersona(string nombre) {
        PERSONA* indice = ORIGEN;
        while (indice != NULL) {
            if (_stricmp(nombre.c_str(),indice->nombre) == 0)
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
    bool eliminarPersona(string nombreBuscar) {
        PERSONA* persona = buscarPersona(nombreBuscar);
        if (persona != NULL) {
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
                PERSONA* anterior = persona->anterior,
                       *siguiente = persona->siguiente;
                anterior->siguiente = siguiente;
                siguiente->anterior = anterior;
                delete persona;
            }
            return true;
        }
        return false;
    }
    void imprimirPersonas() {
        PERSONA* indice = this->ORIGEN;
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
        PERSONA* indice = ORIGEN;
        while (indice != NULL) {
            PERSONA* actual = indice;
            indice = indice->siguiente;
            delete actual;
        }
        ORIGEN = NULL;
        FIN = NULL;
    }
}listaPersonas;
int PERSONAS::maxId = 0;

struct NODOPERSONA {
    NODOPERSONA* PADRE = NULL;
    NODOPERSONA* IZQUIERDO = NULL;
    NODOPERSONA* DERECHO = NULL;
    PERSONA* PERSONA = NULL;
    int balance;
    static NODOPERSONA* nuevoNodo(struct PERSONA* info) {
        NODOPERSONA* nuevo = new NODOPERSONA;
        if (nuevo == NULL)
            exit(-1);
        nuevo->PERSONA = info;
        nuevo->PADRE = NULL;
        nuevo->IZQUIERDO = NULL;
        nuevo->DERECHO = NULL;
        return nuevo;
    }
};
struct ARBOLPERSONAS {
    NODOPERSONA* RAIZ = NULL;
    bool balanceo = false;
    /* Rotación Izquierda *
    *  A           B
    * / \         / \
    * a  B ==>   A   c
    *   / \     / \
    *   b  c    a  b
    * Sólo cambian los factores de balance de los nodos A y B
    * Los factores de balance de los sub-árboles no cambian. */
    NODOPERSONA* rotacionIzquierda(NODOPERSONA* t) {
        NODOPERSONA* temp, *padre = t->PADRE;
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

    /* Rotación derecha
    *
    *   A         B
    *  / \       / \
    *  B  c ==> a   A
    * / \          / \
    * a  b        b   c
    *
    */
    NODOPERSONA* rotacionDerecha(NODOPERSONA* t) {
        NODOPERSONA* temp = t, * padre = t->PADRE;
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
    int calcularEquilibrios(NODOPERSONA* t) {
        if (t == NULL)
            return 0;
        int eIzquierdo = calcularEquilibrios(t->IZQUIERDO) + 1;
        int eDerecho = calcularEquilibrios(t->DERECHO) + 1;
        t->balance = eDerecho - eIzquierdo;
        return eIzquierdo > eDerecho ? eIzquierdo : eDerecho;
    }
    NODOPERSONA* buscarNodo(NODOPERSONA* nodo, PERSONA* persona) {
        if (nodo == NULL)
            return NULL;
        if (nodo->PERSONA->idPersona > persona->idPersona)
            return buscarNodo(nodo->IZQUIERDO, persona);
        else if (nodo->PERSONA->idPersona < persona->idPersona)
            return buscarNodo(nodo->DERECHO, persona);
        return nodo;
    }
    NODOPERSONA* buscarPersona(PERSONA* persona) {
        return buscarNodo(RAIZ, persona);
    }
    NODOPERSONA* buscarPersona(int idPersona) {
        PERSONA personaVacia;
        personaVacia.idPersona = idPersona;
        return buscarNodo(RAIZ, &personaVacia);
    }
    void eliminarNodo(NODOPERSONA** nodo, NODOPERSONA* padre, NODOPERSONA* eliminar) {
        NODOPERSONA* elemento = *nodo;
        if (elemento == NULL){
            this->balanceo = false;
        } else if (elemento->PERSONA->idPersona < eliminar->PERSONA->idPersona) {
            eliminarNodo(&(elemento->DERECHO), elemento, eliminar);
            elemento->balance -= 1;
        } else if (elemento->PERSONA->idPersona > eliminar->PERSONA->idPersona) {
            eliminarNodo(&(elemento->IZQUIERDO), elemento, eliminar);
            elemento->balance += 1;
        } else {
            if (elemento->IZQUIERDO == NULL) {
                NODOPERSONA* tmp = elemento;
                (*nodo) = tmp->DERECHO;
                delete tmp;
                this->balanceo = true;
                return;
            } else if (elemento->DERECHO == NULL) {
                NODOPERSONA* tmp = elemento;
                (*nodo) = tmp->IZQUIERDO;
                delete tmp;
                this->balanceo = true;
                return;
            } else   /* Tiene dos hijos */ {
                if (elemento->balance < 0) {
                    /* Si cargado a la izquierda, elimina mayor descendiente hijo izq */
                    NODOPERSONA* tmp = elemento;
                    tmp = (*nodo)->IZQUIERDO;
                    while (tmp->DERECHO != NULL) tmp = tmp->DERECHO;
                    (*nodo)->PERSONA = tmp->PERSONA;
                    eliminarNodo(&((*nodo)->DERECHO), (*nodo), tmp);
                    elemento = (*nodo);
                    elemento->balance += 1;
                } else {
                    /* Si cargado a la derecha, elimina menor descendiente hijo der */
                    NODOPERSONA* tmp = elemento;
                    tmp = (*nodo)->IZQUIERDO;
                    while (tmp->IZQUIERDO != NULL) tmp = tmp->IZQUIERDO;
                    (*nodo)->PERSONA = tmp->PERSONA;
                    eliminarNodo(&((*nodo)->IZQUIERDO), (*nodo), tmp);
                    elemento = (*nodo);
                    elemento->balance -= 1;
                }
            }
        }
        /* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por descarte */
        if (this->balanceo == true) {
            /* Hay que revisar factores de balance en el ascenso*/
            if (elemento->balance < -1)  /* Si quedó desbalanceado por la izquierda y dejó de ser AVL */ {
                if (elemento->IZQUIERDO->balance > 0)  /*espejos casos c, d y e */ {
                    /* Si el hijo izquierdo está cargado a la derecha */
                    elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
                    this->balanceo = true; /*Continuar revisando factores */
                } else if (elemento->IZQUIERDO->balance == 0)
                    this->balanceo = false; /*No debe seguir el rebalance */
                else
                    this->balanceo = true;/* Debe seguir revisando factores de balance */
                (*nodo) = rotacionDerecha(*nodo);
            } else if (elemento->balance > 1)  /* Si quedó desbalaceado por la derecha */ {
                if (elemento->DERECHO->balance < 0) {
                    /* Si hijo derecho está cargado a la izquierda */
                    (*nodo)->DERECHO = rotacionDerecha((*nodo)->DERECHO);
                    this->balanceo = true; //debe seguir revisando. Caso d.
                } else if (elemento->DERECHO->balance == 0)
                    this->balanceo = false; /* No debe seguir el rebalance. Caso c. */
                else //positivo
                    this->balanceo = true;/* Debe seguir revisando factores de balance. Caso e. */
                (*nodo) = rotacionIzquierda(*nodo);
            } else if (elemento->balance == 0) /* Si estaba en +1 ó -1 y queda en cero */
                this->balanceo = true; /* Debe seguir corrigiendo. Caso b.*/
            else /* Si estaba en cero y queda en -1 ó +1 */
                this->balanceo = false; /* No debe seguir rebalanceando. Caso a.*/
        }
    }
    void borrarNodo(PERSONA* eliminar) {
        NODOPERSONA* nodo = buscarPersona(eliminar);
        if (nodo != NULL) {
            calcularEquilibrios(RAIZ);
            eliminarNodo(&RAIZ, NULL, nodo);
        }
    }
    void insertarNodo(NODOPERSONA** nodo, NODOPERSONA* padre, NODOPERSONA* nuevo) {
        NODOPERSONA* elemento = *nodo;
        if (elemento == NULL) {
            *nodo = nuevo;
            elemento = nuevo;
            nuevo->PADRE = padre;
            nuevo->balance = 0;
            balanceo = true;
            return;
        } else {
            if (elemento->PERSONA->idPersona > nuevo->PERSONA->idPersona) {
                insertarNodo(&(elemento->IZQUIERDO), elemento, nuevo);
                elemento->balance -= 1;
            } else {
                insertarNodo(&(elemento->DERECHO), elemento, nuevo);
                elemento->balance += 1;
            }
        }
        if (this->balanceo == true) {
        /*El código a continuación es el costo adicional para mantener propiedad AVL */
        /* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por inserción */
            if (elemento->balance < -1) {
                /* Quedó desbalanceado por la izquierda. Espejos Casos c y d.*/
                if (elemento->IZQUIERDO->balance > 0) {
                    /* Si hijo izquierdo está cargado a la derecha */
                    elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
                    elemento->IZQUIERDO->PADRE = elemento;
                    if (elemento->IZQUIERDO->DERECHO != NULL)
                        elemento->IZQUIERDO->DERECHO->PADRE = elemento->IZQUIERDO;
                    if (elemento->IZQUIERDO->IZQUIERDO != NULL)
                        elemento->IZQUIERDO->IZQUIERDO->PADRE = elemento->IZQUIERDO;
                }
                *nodo = rotacionDerecha(elemento);
                this->balanceo = false; /* El subárbol no aumenta su altura */
                (*nodo)->PADRE = padre;
                if ((*nodo)->IZQUIERDO != NULL)
                    (*nodo)->IZQUIERDO->PADRE = (*nodo);
                if ((*nodo)->DERECHO != NULL)
                    (*nodo)->DERECHO->PADRE = (*nodo);
            } else if (elemento->balance > 1) {
                /* Si quedó desbalanceado por la derecha: Casos c y d.*/
                if (elemento->DERECHO->balance < 0) {
                    /* Si hijo derecho está cargado a la izquierda Caso d.*/
                    elemento->DERECHO = rotacionDerecha(elemento->DERECHO);
                    elemento->DERECHO->PADRE = elemento;
                    if (elemento->DERECHO->IZQUIERDO != NULL)
                        elemento->DERECHO->IZQUIERDO->PADRE = elemento->DERECHO;
                    if (elemento->DERECHO->DERECHO != NULL)
                        elemento->DERECHO->DERECHO->PADRE = elemento->DERECHO;
                }
                *nodo = rotacionIzquierda(elemento); /* Caso c.*/
                this->balanceo = false; /* El subárbol no aumenta su altura */
                (*nodo)->PADRE = padre;
                if ((*nodo)->IZQUIERDO != NULL)
                    (*nodo)->IZQUIERDO->PADRE = (*nodo);
                if ((*nodo)->DERECHO != NULL)
                    (*nodo)->DERECHO->PADRE = (*nodo);
            } else if (elemento->balance == 0)/* La inserción lo balanceo */
                this->balanceo = false; /* El subárbol no aumenta su altura. Caso a*/
            else /* Quedó desbalanceado con -1 ó +1 Caso b */
                this->balanceo = true; /* Propaga ascendentemente la necesidad de rebalancear */
        }
    }
    void agregarNodo(PERSONA* info) {
        NODOPERSONA* nuevo = NODOPERSONA::nuevoNodo(info);
        insertarNodo(&RAIZ, NULL, nuevo);
        calcularEquilibrios(RAIZ);
    }
    void lista2Arbol(PERSONA* listaDoble) {
        PERSONA* persona = listaDoble;
        while (persona != NULL) {
            agregarNodo(persona);
            persona = persona->siguiente;
        }
    }
    void eliminarArbolNodos(NODOPERSONA* nodo) {
        if (nodo == NULL) {
            return;
        } else {
            eliminarArbolNodos(nodo->IZQUIERDO);
            eliminarArbolNodos(nodo->DERECHO);
            delete(nodo);
        }
    }
    void eliminarArbol() {
        eliminarArbolNodos(RAIZ);
        RAIZ = NULL;
    }
    void imprimirNodos(NODOPERSONA* nodo, int espacios) {
        if (nodo == NULL)
            return;
        string espacio = "";
        for (int i = 0; i < espacios; i++)
            espacio.append(" ");
        cout << espacio << "+";
        nodo->PERSONA->imprimirPersonaId();
        if (nodo->IZQUIERDO != NULL || nodo->DERECHO != NULL) {
            cout << endl << espacio << " |\n";
            imprimirNodos(nodo->IZQUIERDO, espacios + 2);
            imprimirNodos(nodo->DERECHO, espacios + 2);
            cout << espacio << " -\n";
        } else cout << endl;
    }
    void imprimirArbol() {
        cout << "\n\t--INICIO--" << endl;
        imprimirNodos(RAIZ, 0);
        cout << "\t--FIN--" << endl;
    }
} arbolPersonas;

int main(int arg, char **argv){
    listaPersonas.agregarPersona("Jose", 18, 63, 5);
    listaPersonas.agregarPersona("Edna", 21, 63, 6);
    listaPersonas.agregarPersona("Monse", 20, 50, 7);
    listaPersonas.agregarPersona("Carlos", 27, 70, 1);
    listaPersonas.agregarPersona("Manuel", 20, 65, 2);
    listaPersonas.agregarPersona("Adriana", 18, 50, 3);
    listaPersonas.agregarPersona("Alondra", 19, 50, 4);
    arbolPersonas.lista2Arbol(listaPersonas.ORIGEN);
    listaPersonas.imprimirPersonas();
    arbolPersonas.imprimirArbol();

    arbolPersonas.borrarNodo(listaPersonas.buscarPersona("Manuel"));

    arbolPersonas.imprimirArbol();
    arbolPersonas.eliminarArbol();
    listaPersonas.eliminarTodo();
}
