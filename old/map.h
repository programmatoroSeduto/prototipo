/*
METODI:
    modifica posizione del puntatore
    incrementa il valore della cella
    output su console
*/
#include <iostream>
#include <cstdlib>
using namespace std;

class Map {
    
    public:

    Map(int dim);
    /*
        costrutture della matrice dando le dimensioni dell'array
        istanzia la matrice e il buffer
    */

   bool addStartPoint(int x, int y);
   /*
       aggiungi la coordinata al buffer degli inserimenti
       nota: il buffer è una coda
   */

    void nextStartPoint();
    /*
        richiama il prossimo punto dal buffer per l'aggiornamento
    */

    int pendingUpdates();
    /*
            quanti aggiornamenti rimangono da fare? conta quanti ce ne sono nella coda
    */
    
    bool position(int x, int y);
    /*
        ritorna
        TRUE se è stato possibile raggiungere quella posizione
        FALSE altrimenti
    */

   int getx();
   int gety();
   /*
        coordinate del puntatore nella mappa
   */

   float value(float val);
   /*
        ritorna il nuovo valore associato alla cella
   */

    void output();
    /*
        stampa la matrice
    */

    private:

    //la propria posizione nella mappa
    int x, y;
    int dim;
    float *arrayMap;
    int *buffer;
};