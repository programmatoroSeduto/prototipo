#include <iostream>
#include <cstdlib>
#include "ValueMap.h"
using namespace std;

ValueMap::ValueMap(int dim)
{
    //clock
    this->clock = 0;
    
    //creazione della matrice
    map_size = dim;
    map = new Tile**[dim];
    for(int i = 0; i < dim; i++)
        map[i] = new Tile*[dim];

    map_tot_resource = 0;

    r = Resource(0, 0);

    //posizione dell'origine nella mappa, nelle coordinate reali
    origin_position[0] = origin_position[1] = dim/2;

    //coordinate dei vertici della mappa
    //vertice V1
    vertex[0][0] = dim - origin_position[0];
    vertex[0][1] = dim - origin_position[1];

    //vertice V3
    vertex[1][0] = origin_position[0] - dim;
    vertex[1][1] = origin_position[1] - dim;

    //creazione del buffer
    buffer_size = dim*dim + dim/2;
    buffer = new Tile*[buffer_size];
    buffer_first = buffer_last = 0;
    first_marked = -1;
}