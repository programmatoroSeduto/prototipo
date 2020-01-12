/*
METODI:
    modifica posizione del puntatore
    incrementa il valore della cella
    output su console
*/
#include <iostream>
#include <cstdlib>
using namespace std;

class map {
    
    public:

    map(int dim);
    
    bool position(int x, int y);
    /*
        ritorna
        TRUE se è stato possibile raggiungere quella posizione
        FALSE altrimenti
    */

   int value(float val);
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
}