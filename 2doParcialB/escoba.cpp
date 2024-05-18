#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstring>
using namespace std;
typedef char str6[7];

//STRUCTS
struct sMazo{
    short carta;
    bool usado;
};
struct sCarta{
  short valor;
  str6 palo;
  short numRef;
};
struct Nodo{
  sCarta dato;
  Nodo* siguiente;
};
struct sPuntCJ{
    short cantOros,
          totCartas,
          cantSetentas;
    bool sO;
};
struct sVecCJ{
    sCarta cartasCJ[3];
};



//PROTOTIPOS
void turnoJug(Nodo*&, sCarta[], short[], short , Nodo*& , bool&);
void turnoBot(Nodo*&, sCarta[], short[], short, Nodo*&, bool&, int);
void finMano(short, Nodo*[], short[], bool&);
void levantaMesa(Nodo*&, Nodo*&);
void inicMazo(sMazo[][10]);
int cartasRestantesMazo(sMazo[][10]);
void reinicMazo(sMazo[][10]);
short eligeCarta(sMazo[][10],short&,short&);
void printMazo(sMazo[][10]);
void repartir(sMazo[][10],sVecCJ[],short&,short&,short);
void inicCJ(sCarta[]);
void mostrarCJ(sCarta[]);
void leerMazoSec(Nodo*);
void definePalo(short, str6);
void InicioPartida(sMazo[][10], short&, short&, Nodo*& );
void insertarEnListaCir(Nodo*&,sCarta);
void push(sCarta, Nodo*&);
void mostrarListado(Nodo *);
bool eliminarCarta(Nodo*&, sCarta);
void Interfaz(Nodo*&, sCarta[], Nodo*&,bool&);
void casoB(Nodo*&,int , sCarta[]);
void mazovacio(Nodo*&, sCarta[]);
void mazovacioBot(Nodo*&, sCarta[]);
sCarta pop(Nodo*&);
int tamanioMesa(Nodo*);
bool levSumaAcum(sCarta, Nodo*[], short, Nodo*&, Nodo*&);
void levAvanza(Nodo*[], short, short);
bool levAvanzaFin(Nodo*[], short&, short, int,Nodo*);
void levanta(Nodo*&, sCarta, Nodo*&, bool&);
void puntosMazo(Nodo*&,sPuntCJ&);
void inicPuntCJ(sPuntCJ[]);
short eligeMayor(short, short, short);
void puntaje(Nodo*[], short, short[]);


int main(){
    bool band2 = false,
         juegoTermina = false;
    short filaCarta = 0,
          colCarta = 0,
          puntos[3] = {0,0,0},
          cantJugs,
          cantBots,
          ultimoLevan;

    sMazo mazo[4][10];
    Nodo*inicListaCarta = NULL;


    do {
        cout << "Bienvenidos, cuantas personas seran? 1, 2 o 3? ";
        cin >> cantJugs;
    } while (cantJugs < 1 || cantJugs > 3);
    do {
        cout << "Contra cuantos bots? (la suma entre jugadores y bots debe ser 3 o 2) ";
        cin >> cantBots;
    } while (cantBots + cantJugs != 3 && cantBots + cantJugs != 2);

 	Nodo* punteroMazoSecJ[cantJugs + cantBots] ;
    sVecCJ vecCJ[cantJugs + cantBots];
    inicMazo(mazo);
    for (short i = 0; i < (cantJugs + cantBots); i++ ){
        inicCJ(vecCJ[i].cartasCJ);
        punteroMazoSecJ[i] = NULL;
    }
    repartir(mazo,vecCJ,filaCarta,colCarta, cantJugs+cantBots);

    cout << endl <<"Empezar el Juego con: "<< endl;
    short i, j;
    for (i = 0; i < cantJugs; i++){
        cout << "Jugador " << i+1 << ": Persona"<< endl;
    }
    for (j = 0; j < cantBots; j++){
        cout << "Jugador " << i+1 << ": Bot"<< endl;
        i++;
    }

    system("pause");
    system("cls");
    InicioPartida(mazo,filaCarta, colCarta, inicListaCarta);
    cout << endl  <<"Se reparten 3 cartas a cada uno..." << endl;
    cout << "Quedan "<< cartasRestantesMazo(mazo) << " cartas en el mazo" << endl;

    while (!juegoTermina){
        for(int i=0;i<3;i++){
            for(int j = 0; j < cantJugs; j++){
                turnoJug(inicListaCarta, vecCJ[j].cartasCJ, puntos, j, punteroMazoSecJ[j],band2);
                if(band2){
                    ultimoLevan = j;
                }
            }
            for(int j = 0; j < cantBots; j++){
                turnoBot(inicListaCarta, vecCJ[j+cantJugs].cartasCJ, puntos, j+cantJugs, punteroMazoSecJ[j+cantJugs],band2, 0);
                if(band2){
                    ultimoLevan = j+cantJugs;
                }
            }
        }
        system("pause");
        system("cls");

        if(cartasRestantesMazo(mazo)<= 0){
            cout << "Jugador " << ultimoLevan+1 << " fue el ultimo en levantar! se queda con las cartas de la Mesa!"<< endl;
            levantaMesa(inicListaCarta, punteroMazoSecJ[ultimoLevan]);
            finMano(cantJugs, punteroMazoSecJ, puntos, juegoTermina);
            if (!juegoTermina){
                cout << endl <<"El mazo se ha quedado sin cartas suficientes, reiniciando mazo y repartiendo cartas a mesa..." <<endl;
            }
            reinicMazo(mazo);
            InicioPartida(mazo, filaCarta, colCarta, inicListaCarta);
        }
        repartir(mazo,vecCJ,filaCarta,colCarta, cantJugs);
        if (!juegoTermina){
            cout << "Se reparten 3 cartas a cada uno..." << endl;
            cout << "Quedan "<< cartasRestantesMazo(mazo) << " cartas en el mazo" << endl;
        }else{
            cout << endl <<"Presione cualquier tecla para cerrar el juego"<< endl;
        }
        system("pause");
        system("cls");

    }
    return 0;
}

// ---------------------------------------------------------------------------------------
void turnoJug(Nodo*& inicListaCarta, sCarta CJ[], short puntos[], short numJugador, Nodo*& puntMazoSec, bool& band2){

    cout << endl << "Pase computadora a Jugador " << numJugador+1 << ")" << endl;
    system("pause");
    system("cls");
    if (tamanioMesa(inicListaCarta) == 0){
        mazovacio(inicListaCarta, CJ);
    }else{
        cout << "La mesa tiene " << endl;
        mostrarListado(inicListaCarta);
        cout << "Jugador " << numJugador+1 << ")" << endl;
        Interfaz(puntMazoSec, CJ, inicListaCarta, band2);
    }
    if (tamanioMesa(inicListaCarta) == 0){
        cout << "Hiciste una Escoba! Ganas 1 punto!" << endl;
        puntos[numJugador]++;
    }

}

void turnoBot(Nodo*& inicListaCarta, sCarta CJ[], short puntos[], short numJugador, Nodo*& puntMazoSec, bool& band2, int posCarta){

    int posCartaAux = posCarta; //Contiene la carta inicial elegida, en caso de que no pueda levantar nada
    cout << endl << "Turno de Jugador " << numJugador+1 << ") [BOT] " << endl;
    system("pause");
    system("cls");
    if (tamanioMesa(inicListaCarta) == 0){
        mazovacioBot(inicListaCarta, CJ);
    }else{
        cout << "La mesa tiene " << endl;
        mostrarListado(inicListaCarta);
        while (posCarta < 3 && CJ[posCarta].numRef == 0 ){
            posCarta++;
        }
        levanta(puntMazoSec, CJ[posCarta], inicListaCarta,band2);
            if (!band2){
                posCarta++;
                if (posCarta > 2){
                    cout<< "El Bot no pudo levantar nada, por lo tanto tira su carta a la mesa"<< endl;
                    casoB(inicListaCarta,posCartaAux,CJ);
                }
                else{
                    turnoBot(inicListaCarta, CJ, puntos, numJugador, puntMazoSec, band2, posCarta);
                }
            }else{
                cout << "Bot utilizo la carta: " << CJ[posCarta].numRef << " " << CJ[posCarta].palo << endl;
                CJ[posCarta].numRef = 0;

            }
    }
    if (tamanioMesa(inicListaCarta) == 0){
        cout << "Hiciste una Escoba! Ganas 1 punto!" << endl;
        puntos[numJugador]++;
    }

}

void finMano(short cantJugs, Nodo* punteroMazoSecJ[], short puntos[], bool& juegoTermina){//CAMBIE
    short aux1,
          aux2;
    cout << endl << "Mazo de Cartas Levantadas: ";
    for (short i = 0; i < cantJugs; i++ ){
        cout <<endl <<"Jugador " <<i+1 <<") " << endl;
        leerMazoSec(punteroMazoSecJ[i]);
    };
    cout << endl << "Puntaje" << endl;
    puntaje(punteroMazoSecJ, cantJugs, puntos);
    for (short i = 0; i < cantJugs; i++ ){
        if (puntos[i] >= 15) {
            if (cantJugs == 2){
                aux1 = 0;
            }else {
                aux1 = puntos[2];
            }
            aux2 = eligeMayor(puntos[0], puntos[1], aux1);
            if (aux2 > 2){
                cout<< endl << "Los ganadores tienen la misma cantidad de puntos, Es un Empate!"<< endl;
            }else{
                cout << endl <<"Felicidades Jugador "<< aux2+1 <<"!!";
                cout << endl<< "Has ganado la partida con " << puntos[aux2] << " puntos!" << endl;
            }
            juegoTermina = true;
            i = cantJugs;
        }
    }
}

void levantaMesa(Nodo* &inicListaCarta, Nodo*& punteroMasoSec){
    while(inicListaCarta != NULL){
        push(inicListaCarta->dato, punteroMasoSec);
        eliminarCarta(inicListaCarta, inicListaCarta->dato);
    }
}

void inicMazo(sMazo mazo[][10]){
    short i,
          j;
    for (i = 0; i < 4; i++){
        for (j = 0; j < 10; j++){
            if ( j < 7){
                mazo[i][j].carta = j+1;
                mazo[i][j].usado = false;
            }
            else{   // Hice esta division para que saltear los numeros 8 y 9, y aun asi usar los espacios 7 y 8 de la matriz
            mazo[i][j].carta = j+3;
            mazo[i][j].usado = false;
            }
        }
    };
}

int cartasRestantesMazo(sMazo mazo[][10]){
    int cantRest = 0;

    for (short i = 0; i < 4; i++){
        for(short j = 0; j < 10; j++){
            if (!(mazo[i][j].usado)){
                cantRest++;
            }
        }
    }
    return cantRest;
}

void reinicMazo(sMazo mazo[][10]){

    for (short i = 0; i < 4; i++){
        for(short j = 0; j < 10; j++){
            mazo[i][j].usado = false;
        }
    }
}

short eligeCarta(sMazo mazo[][10],short& filaCarta, short& colCarta){
    short i = 0;
    srand(time(0));
    colCarta= rand()%10;
    filaCarta= rand()%4;

    if (cartasRestantesMazo(mazo) <= 0){
        cout << endl <<"El mazo se ha quedado sin cartas suficientes, reiniciando mazo..." <<endl;
        reinicMazo(mazo);
    }

    while (mazo[filaCarta][colCarta].usado){
        if (i <= 5){ //agregue esto para que Si a la quinta vez que hace random, no le sale un numero util, que saque el siguiente de la lista
            colCarta = rand()%10;
            filaCarta= rand()%4;
            i++;
        }else{
            colCarta++;
            if (colCarta >= 10){
                colCarta = 0;
                filaCarta++;

                if (filaCarta >= 4){
                    filaCarta = 0;
                }
            }
        }
    }

    mazo[filaCarta][colCarta].usado = true;
    return mazo[filaCarta][colCarta].carta;
}

void printMazo(sMazo mazo[][10]){ //Funcion auxiliar, usada unicamente para la ayuda de correccion de errores
    short i,
          j;

    for (i = 0; i < 4; i++){
        for (j = 0; j < 10; j++){
            cout << mazo[i][j].carta << " ";
        }
        cout << endl;
    }

    cout << endl <<"Fue usado?" << endl;
    for (i = 0; i < 4; i++){
        for (j = 0; j < 10; j++){
            cout <<j+1 + (i*10) << ") "<< mazo[i][j].usado << "  ";
        }
        cout << endl;
    }
}

void repartir(sMazo mazo[][10],sVecCJ vecCJ[],short& filaCarta, short& colCarta, short cantJugs){
    int i=0,
        j;

    for (i; i <cantJugs; i++){
        for(j = 0; j<3 ; j++){
            eligeCarta(mazo,filaCarta,colCarta);
            vecCJ[i].cartasCJ[j].numRef= mazo[filaCarta][colCarta].carta;
            definePalo(filaCarta, vecCJ[i].cartasCJ[j].palo);
            vecCJ[i].cartasCJ[j].valor = colCarta+1;
        }
  }
}

void inicCJ(sCarta C[]){
  int i=0;
  for (i; i< 3; i++){
    C[i].valor=0;
    strcpy(C[i].palo, "Vacio");
    C[i].numRef=0;
  }
}

void mostrarCJ(sCarta C[]){
  int i=0;
  for (i; i<3 ; i++){
    if (C[i].numRef != 0){
            cout<< i+1 <<") "<<C[i].numRef<< " ";
            cout<<C[i].palo<< endl;
    }
  }
}

void leerMazoSec(Nodo* mazoSec){
    short i = 1;
    if(mazoSec == NULL){
        cout << "Mazo Vacio..." << endl;
    }
    while (mazoSec != NULL){
        cout << i <<") "<< mazoSec->dato.numRef << " ";
        cout << mazoSec->dato.palo << endl;
        mazoSec = mazoSec->siguiente;
        i++;
    }

}

void definePalo(short palo, str6 paloCarta){ //Le das el numero de la FILA y la VARIABLE donde GUARDAR el nombre del palo

    switch (palo){
        case 0:
            strcpy(paloCarta, "oro");
            break;
        case 1:
            strcpy(paloCarta, "copa");
            break;
        case 2:
            strcpy(paloCarta, "espada");
            break;
        case 3:
            strcpy(paloCarta, "basto");
            break;
        default:
            cout << "ERROR PALO INVALIDO, FILA NO ES NI 0, 1, 2 O 3";
    }
}

void InicioPartida(sMazo mazo[][10],short& filaCarta, short& colCarta,Nodo*& inicListaCarta){
   sCarta CartaActual;
    for (int i=0; i<4 ; i++){
    eligeCarta(mazo,filaCarta,colCarta);
    CartaActual.numRef= mazo[filaCarta][colCarta].carta;
    definePalo(filaCarta, CartaActual.palo);
    CartaActual.valor = colCarta+1;
    insertarEnListaCir(inicListaCarta,CartaActual);
    }
}

void insertarEnListaCir(Nodo *&inicio, sCarta CartaActual){//insertaOrdenado

    Nodo *nuevo = new Nodo, *aux;
    nuevo->dato = CartaActual;
    aux = inicio;

    if (inicio == NULL){
        inicio = nuevo;
        nuevo->siguiente = inicio;
    }
    else{
        while (aux->siguiente != inicio){
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
        nuevo-> siguiente = inicio;
    }
}

void push(sCarta datoNuevo, Nodo*&mazoSec){
    Nodo *nuevo = new Nodo();
    nuevo->dato = datoNuevo;

    nuevo->siguiente = mazoSec;
    mazoSec = nuevo;
}

void mostrarListado(Nodo *inicListaCarta){
    Nodo *aux = inicListaCarta;
    short i = 1;
    do{
        cout << i << ") "<<aux->dato.numRef<< " " << aux->dato.palo << "   ";
        i++;
        aux = aux->siguiente;
    }while (aux != inicListaCarta);
    cout << endl << endl;
}

bool eliminarCarta(Nodo*& inicListaCarta, sCarta cartaBuscada) {
    Nodo* aux, *antecesor;
    aux = inicListaCarta;
    antecesor = aux;

    while (aux->siguiente != inicListaCarta && (aux->dato.numRef != cartaBuscada.numRef || strcmp(aux->dato.palo, cartaBuscada.palo) != 0)) {
        antecesor = aux;
        aux = aux->siguiente;
    }

    if ( aux->dato.numRef != cartaBuscada.numRef && strcmp(aux->dato.palo, cartaBuscada.palo) != 0){
        cout << endl << "ERROR CARTA A ELIMINAR NO ENCONTRADA"<< endl;
        return false;
    }

    if (aux == inicListaCarta) {
            if (aux->siguiente == aux){
                inicListaCarta = NULL;
                delete aux;
                return true;
            }
            else{
                while (antecesor->siguiente != inicListaCarta){
                    antecesor = antecesor->siguiente;
                }
                antecesor ->siguiente = aux->siguiente;
                inicListaCarta = aux->siguiente;
                delete aux;
                return true;
            }
    }else{
        antecesor ->siguiente = aux->siguiente;
        inicListaCarta = aux->siguiente;
        delete aux;
        return true;
    }
}

void Interfaz(Nodo*& mazoSecundario, sCarta CJ[], Nodo*& inicListaCarta, bool& band2){
    int posCarta;
    char accion;
    mostrarCJ(CJ);

    do{
        cout << "Seleccione una Carta segun la posicion" << endl;
        cin >> posCarta;
        posCarta -= 1;
        if ( 0> posCarta || posCarta > 2) {
            cout << "El jugador puede tener hasta tres cartas, no existe esa posicion"<< endl;
        }else if (CJ[posCarta].numRef == 0){
            cout << "Carta ya usada, elija de vuelta"<< endl;
        }
    } while (0> posCarta || posCarta > 2 || CJ[posCarta].numRef == 0);
    system("cls");
    mostrarListado(inicListaCarta);
    cout<< "elegiste la carta " << CJ[posCarta].numRef << " " <<CJ[posCarta].palo << endl;
    cout << "Que operacion desea realizar con esta carta?" <<endl;
    do{
    cout << "Si desea levantar inserte A o si desea tirar la carta inserte B" << endl;
    cin >> accion;
    accion = toupper(accion);
    }while (accion != 'A' && accion != 'B');
    switch (accion){
        case 'A':
            levanta(mazoSecundario, CJ[posCarta], inicListaCarta,band2);
            if (!band2){
                cout<< "La carta que elegiste no puede levantar nada por lo que queda tirada en la mesa"<< endl;
                casoB(inicListaCarta,posCarta,CJ);
            }else{
                CJ[posCarta].numRef = 0;

            }
            break;
        case 'B':
            casoB(inicListaCarta,posCarta,CJ);
            cout << "Carta tirada en la Mesa..." << endl;
            break;
        default:
            cout << "SELECCION INVALIDA, PIERDE TURNO"<<endl;
            break;
  }
}

void casoB(Nodo*& inicListaCarta,int posCarta, sCarta CJ[]){
	insertarEnListaCir(inicListaCarta,CJ[posCarta]);
	CJ[posCarta].numRef=0;
}

void mazovacio (Nodo*& inicListaCarta, sCarta CJ[]){
    int posCarta;

    do{
        cout << "La mesa no tiene cartas" << endl;
        mostrarCJ(CJ);
        cout << "Seleccione la posicion de la carta a tirar a la mesa: " << endl;
        cin >> posCarta;
        posCarta -= 1;
        if ( 0> posCarta || posCarta > 2) {
            cout << "El jugador puede tener hasta tres cartas, no existe esa posicion"<< endl;
            system("cls");
        }
    } while (0> posCarta || posCarta > 2);

    casoB(inicListaCarta, posCarta, CJ);

}

void mazovacioBot (Nodo*& inicListaCarta, sCarta CJ[]){
    int posCarta = 0;

    while (posCarta < 3 && CJ[posCarta].numRef == 0 ){
            posCarta++;
    }
    cout << "La mesa no tiene cartas" <<endl << "El Bot suelta la carta: " <<endl;
    cout << CJ[posCarta].numRef << " " << CJ[posCarta].palo << endl;
    casoB(inicListaCarta, posCarta, CJ);

}

sCarta pop(Nodo*& pLista){
    Nodo *aux = pLista;
    sCarta info = aux->dato;
    pLista = aux->siguiente;
    delete aux;
    return info;
}

int tamanioMesa(Nodo* mesa){
    if (mesa == NULL){
        return 0;
    }

    Nodo* aux = mesa->siguiente;
    int i = 1;
    while (aux != mesa){
        aux = aux->siguiente;
        i++;
    }
    return i;
}
//--------------------INICIO COMBINATORIA (Levanta de Mesa)-----------------------
bool levSumaAcum(sCarta carJug, Nodo*p[], short i, Nodo*&mazoSec, Nodo*& mazoMesa){
    short acum = carJug.valor,
          j;
    for (j = 0 ; j <= i; j++){
        acum += p[j]->dato.valor;
    }

    if(acum == 15){
        push(carJug, mazoSec);
        cout << "Carta/s de la mesa levantada/s: ";
        j = 0; //lo pongo afuera del for para que quede bien esteticamente
        cout << p[j]->dato.numRef << " " << p[j]->dato.palo;
        push(p[j]->dato, mazoSec);
        eliminarCarta(mazoMesa, p[j]->dato);
        for (j = 1 ; j <= i; j++){
            cout << ", "<<p[j]->dato.numRef << " " << p[j]->dato.palo;
            push(p[j]->dato, mazoSec);
            eliminarCarta(mazoMesa, p[j]->dato);
        }
        cout << endl;
        system("pause");
        return false;
    }
    else {
        return true;
    }

}

void levAvanza(Nodo*p[] , short curr, short i){
    bool band = true;
    short j;
    while (band){
        band = false;
        for (j = 0; j < i; j++){
            if (p[curr] == p[j] && curr != j){
                band = true;
                p[curr] = p[curr]->siguiente;
            }
        }
    }
}

bool levAvanzaFin(Nodo*p[], short& i, short j, int tamMesa, Nodo*mazoMesa){
    if (i >= tamMesa-1){ //si hay mas punteros que cartas en mesa, terminar proceso
        return true;
    }
    else if (p[j-1]->siguiente != p[j]) { //si el puntero anterior puede avanzar, que avance
        p[j-1] = p[j-1]->siguiente;
        return false;
    }
    else {// Si el puntero NO puede avanzar...
        if (p[j-1] != p[0]){ //Y no es el cero, se repite este modulo (recursivo)
            levAvanzaFin(p, i, j-1 , tamMesa, mazoMesa);
            return false;
        }

        else{ // Si es el cero, entonces se posicionan todos los punteros al comienzo y se agrega un puntero
            p[0] = mazoMesa;
            levAvanza(p, 0, i);
            while (p[j]->siguiente != p[0]){
                p[j] = p[j-1]->siguiente;
                j++;
            };
            p[j]=p[j-1]-> siguiente;
            i++;
            return false;
        }
    }
}


void levanta(Nodo*& mazoSecundario, sCarta carJug, Nodo*& mazoMesa, bool& band2 ){
    short i = 0;
    bool band1 = true;
    Nodo**p = new Nodo*[tamanioMesa(mazoMesa)];

    band2 = true;
    for (short j = 0; j < tamanioMesa(mazoMesa); j++){
        p[j] = NULL;
    }

    p[i] = mazoMesa;
    while(band1 && band2){
        if (i >= tamanioMesa(mazoMesa)){
            band1 = false;
            band2 = false;
		}else {
           levAvanza(p, i, i);
           if (i>0){
           }
           band1 = levSumaAcum(carJug, p, i, mazoSecundario, mazoMesa);
		}
        if (band1 && p[i]->siguiente != mazoMesa){
            p[i] = p[i]->siguiente;
        }else if (band1){
            if (i == 0){
                p[0]=p[0]->siguiente;
                i++;
                p[i]= mazoMesa;
            }else{
                if (levAvanzaFin(p, i, i, tamanioMesa(mazoMesa),mazoMesa)){
                    band2 = false;
                }
                else{
                     p[i]= mazoMesa;
                }
            }
        }
    }
}
//-----------------------------FIN COMBINATORIA-----------------------------------

//------------------------------PROCESAMIENTO-------------------------------------
void puntosMazo(Nodo*&mazoSec, sPuntCJ &puntCJ) {
    if(mazoSec==NULL){
        return;
    }
    sCarta dato;

    while(mazoSec->siguiente!=NULL){

        dato = pop(mazoSec);

        if(dato.valor==7){
            puntCJ.cantSetentas++;
        }
        if(strcmp(dato.palo,"oro")==0){
            puntCJ.cantOros++;
        }
        if(dato.valor==7 && (strcmp(dato.palo,"oro")==0)){
            puntCJ.sO=true;
        }

        puntCJ.totCartas++;
    }
    dato = pop(mazoSec);
    if(dato.valor==7){
        puntCJ.cantSetentas++;
    }
    if(strcmp(dato.palo,"oro")==0){
        puntCJ.cantOros++;
    }
    if(dato.valor==7 && (strcmp(dato.palo,"oro")==0)){
        puntCJ.sO=true;
    }
    puntCJ.totCartas++;

    return;
}

void inicPuntCJ(sPuntCJ puntCJ[]){
    for (short i = 0; i < 3; i++){
        puntCJ[i].cantOros = 0;
        puntCJ[i].cantSetentas = 0;
        puntCJ[i].totCartas = 0;
        puntCJ[i].sO = false;
    }
}

short eligeMayor(short v1, short v2, short v3){ // 4 si los dos mayores son iguales, 0 si v1 es mayor, 1 si v2 es mayor, 2 si v3 es mayor
    if(v1 > v2 && v1 > v3){
        return 0;
    }else if (v2 > v1 && v2 > v3){
        return 1;
    }else if(v3 > v1 && v3 > v2){
        return 2;
    }else{
        return 4;
    }

}

void puntaje(Nodo* mazoJ[], short cantJugs, short puntos[]) {
    short aux,
          i;
    sPuntCJ puntCJ[3];
    inicPuntCJ(puntCJ);
    for (i = 0; i < cantJugs; i++){
        puntosMazo(mazoJ[i],puntCJ[i]);
    }

    aux = eligeMayor(puntCJ[0].totCartas, puntCJ[1].totCartas, puntCJ[2].totCartas);
    if (aux < 3){
        puntos[aux]++;
        cout << "Jugador " << aux+1 << " Gana 1 punto por mayoria de cartas!" << endl;
    }else {
        cout << "Todos Jugadores tienen la misma cantidad de cartas, Ninguno gana Puntos!"<< endl;
    }

    aux = eligeMayor(puntCJ[0].cantOros, puntCJ[1].cantOros, puntCJ[2].cantOros);
    if (aux < 3){
        puntos[aux]++;
        cout << "Jugador " << aux+1 << "  Gana 1 punto por mayoria de Oros!" << endl;
    }else {
        cout << "Todos Jugadores tienen la misma cantidad de Oros, Ninguno gana Puntos!"<< endl;
    }

    aux = eligeMayor(puntCJ[0].cantSetentas, puntCJ[1].cantSetentas, puntCJ[2].cantSetentas);
    if (aux < 3){
        puntos[aux]++;
        cout << "Jugador " << aux+1 << " Gana 1 punto por mayoria de Sietes!" << endl;
    } else {
        cout << "Todos los Jugadores tienen la misma cantidad de Sietes, Ninguno gana Puntos!"<< endl;
    }

    for (i = 0; i < cantJugs; i++){
        if(puntCJ[i].sO){
            puntos[i]++;
            cout << "Jugador " << i+1 << " Gana 1 punto por tener el Siete de Oro!" << endl;
        }
    }
    cout <<endl;
    for (i = 0; i < cantJugs; i++){
        cout << "El jugador " << i+1 <<" tiene " << puntos[i] << " puntos" << endl;
    }

    return;
}

