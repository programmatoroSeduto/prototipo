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

    r = Resource(0, 0, -1);

    //posizione dell'origine nella mappa, nelle coordinate reali
    origin_position[0] = origin_position[1] = dim/2;

    //istanziazione dei tiles nella mappa
    for(int x = 0; x < dim; x++)
        for(int y = 0; y < dim; y++)
            map[x][y] = new Tile(x - origin_position[0], y - origin_position[1]);

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
    for(int i = 0; i < buffer_size; i++) buffer[i] = NULL;
    buffer_first = buffer_last = 0;
}




/*----------------------OPERAZIONI DALL'ESTERNO----------------------*/

bool ValueMap::set_actual_value_at(int x, int y, float increment)
{
    Tile* t = get_tile_at(x, y);

    if(t == NULL)
        return false;
    else
    {
        t->set_delta(increment);
        return true;
    }
}




bool ValueMap::new_resource_type(float k, float toll)
{
    if(k<0 || k>1)
        return false;
    else if (toll<0 || toll>1 || toll-k<0)
        return false;
    else
    {
        r = Resource(k, toll);
        return true;
    }
}




/*----------------------AGGIORNAMENTO----------------------*/

void ValueMap::update_map()
{
    update_step_1();
    update_step_2();
    clock++;
}




void ValueMap::update_map_clock(int ck)
{
    for(int i = 0; i < ck; i++)
    {
        update_step_1();
        update_step_2();
        clock++;
    }
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




/*----------------------OPERAZIONI SUL BUFFER----------------------*/

int ValueMap::idx(int i)
{
    if( (i >= (buffer_size - 1)) || (i <= (1 - buffer_size)))
        return -1;

    if(i == 0)
        return buffer_first;
    else if(i < 0)
        return (buffer_last + i) % buffer_size;
    else
        return (buffer_first + i) % buffer_size;
}




Tile* ValueMap::get_from_buffer(int index)
{
    int i = idx(index);

    if(i >= 0)
        return buffer[i];
    else
        return NULL;
}




Tile* ValueMap::get_from_buffer_if_unmarked(int index)
{
    Tile* t = get_from_buffer(index);

    if(t == NULL)
        return NULL;
    else if (t->is_marked())
        return NULL;
    else
        return t;
}




int ValueMap::length_of_buffer()
{
    if(buffer_first == buffer_last)
        return 0;
    else if( buffer_first < buffer_last )
        return buffer_last - buffer_first;
    else
        return buffer_size - ( buffer_first - buffer_last );
    
}




bool ValueMap::enqueue(Tile* t)
{
    //prima verifico che la coda non sia piena
    if(buffer_last == ((buffer_first + 1) % buffer_size))
        return false;

    buffer[buffer_last] = t;
    buffer_last = idx(buffer_last + 1);

    return true;
}




bool ValueMap::dequeue_all_marked()
{
    //prma verifico che la coda non sia vuota
    if(buffer_first == buffer_last)
        return false;

    for(int i = buffer_first; i < buffer_last; i++)
    {
        if(buffer[i]->is_marked())
            buffer[i] = NULL;
        else
            break;
    }

    return true;
}




/*----------------------ALGORITMO DI AGGIORNAMENTO----------------------*/

void ValueMap::update_step_1()
{
    int length_buf = length_of_buffer();

    for(int i=0; i < length_buf; i++)
    {
        //recupera la cella e marcala
        Tile* t = get_from_buffer(i);
        t->mark();

        //prendi i valori delle celle vicine(e conta anche quante celle vicine ci sono effettivamente)
        int n = 1;
        float res_mean = 0;
        for(int j=0; j<4; j++)
        {
            Tile* temp = next_nearby_tile(t->get_x(), t->get_y(), i);
            if(t == NULL)
                break;
            else
            {
                n++;
                res_mean += temp->get_actualValue();
            }
        }
        res_mean += t->get_actualValue();
        res_mean /= n;

        //programma aggiornamento cella presente
        float delta_value = r.get_real_k()*(res_mean - t->get_actualValue());
        t->set_delta(delta_value);

        //programma aggiornamento celle vicine
        for(int j=0; j<4; j++)
        {
            Tile* temp = next_nearby_tile(t->get_x(), t->get_y(), i);
            if(t == NULL)
                break;
            else
            {
                temp->set_delta(-(delta_value/static_cast<double>(n)));
                enqueue(temp);
            }
        }
    }
}




void ValueMap::update_step_2()
{
    for(int i=0; i<length_of_buffer();i++)
    {
        get_from_buffer(i)->update_actualValue();
    }
    dequeue_all_marked();
}