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




/*----------------------OPERAZIONI SULLA MATRICE----------------------*/

float ValueMap::sum_all_tiles()
{
    float sum = 0;

    for(int j = 0; j < map_size; j++)
        for(int k = 0; k < map_size; k++)
            sum += map[j][k]->get_actualValue();

    return sum;
}




Tile* ValueMap::get_tile_at(int x, int y)
{
    //se viene richiesto un numero al di fuori dello spazio rappresentabile, ritorna true
    if( ( (x < vertex[1][0]) || (x > vertex[0][0]) ) || ( (y < vertex[1][1]) || (y > vertex[0][0]) ) )
        return NULL;

    int real_x = x + origin_position[0];
    int real_y = y + origin_position[1];

    return map[real_x][real_y];
}




Tile* ValueMap::next_nearby_tile(int base_x, int base_y, int index)
{
    /*
        CONVENZIONE:
        0   ->  alto
        1   ->  destra
        2   ->  basso
        3   ->  sinistra
        >3, <0  NULL
    */
    switch(index)
    {
        case 0:
            Tile* t = get_tile_at(base_x, base_y + 1);
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 1);
            else
                return t;
        break;
        case 1:
            Tile* t = get_tile_at(base_x + 1, base_y);
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 2);
            else
                return t;
        break;
        case 2:
            Tile* t = get_tile_at(base_x, base_y - 1);
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 3);
            else
                return t;
        break;
        case 3:
            Tile* t = get_tile_at(base_x - 1, base_y);
            if(t == NULL)
                return NULL;
            else
                return t;
        break;
        default:
            return NULL;
    }
}