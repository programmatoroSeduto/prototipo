#include <iostream>
//#include <cstdlib>
#include "ValueMap.h"
using namespace std;

ValueMap::ValueMap(int dim)
{
    //clock
    clock = 0;
    
    //creazione della matrice
    map_size = dim;
    map = new Tile**[dim];
    for(int i = 0; i < dim; i++)
        map[i] = new Tile*[dim];

    map_tot_resource = 0;

    r = Resource(0, 0, -1);

    //posizione dell'origine nella mappa, nelle coordinate reali
    origin_position[0] = origin_position[1] = dim/2;

    //cout << "origin(x:" << origin_position[0] << " ,y:" << origin_position[1] <<")" << endl;

    //istanziazione dei tiles nella mappa
    for(int x = 0; x < dim; x++)
        for(int y = 0; y < dim; y++)
        {
            map[x][y] = new Tile(x - origin_position[0], y - origin_position[1]);
            //cout << "tile=" << map[x][y] << " x=" << map[x][y]->get_x() << " y=" << map[x][y]->get_y() << endl;
        }

    //coordinate dei vertici della mappa
    //vertice V1
    vertex[0][0] = dim - origin_position[0] - 1;
    vertex[0][1] = dim - origin_position[1] - 1;

    //cout << "V1(x:" << vertex[0][0] << " ,y:" << vertex[0][1] <<")" << endl;

    //vertice V3
    vertex[1][0] = origin_position[0] - dim + 1;
    vertex[1][1] = origin_position[1] - dim + 1;

    //cout << "V3(x:" << vertex[1][0] << " ,y:" << vertex[1][1] <<")" << endl;

    //creazione del buffer
    buffer_size = dim*dim + dim/2;
    buffer = new Tile*[buffer_size];
    for(int i = 0; i < buffer_size; i++) buffer[i] = NULL;
    buffer_first = buffer_last = 0;

    //cout << "buffer_first=" << buffer_first << " buffer_last=" << buffer_last << endl;
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
        t->update_actualValue();
        map_tot_resource += increment;
        cout << "risposta: " << enqueue(t) << endl ;
        return true;
    }
}




bool ValueMap::new_resource_type(float k, float toll)
{
    if(k<0 || k>1)
        return false;
    else if (toll<0 || toll>1 || k-toll<0)
        return false;
    else
    {
        r = Resource(k, toll);
        //cout << "NEW RESOURCE TYPE k=" << r.get_k() << " e=" << toll << endl;
        return true;
    }
}




/*----------------------AGGIORNAMENTO----------------------*/

void ValueMap::update_map()
{
    if(length_of_buffer() == 0)
    {
        //cout << "nessuna cella da aggiornare." << endl;
        return;
    }

    update_step_1();
    update_step_2();
    clock++;
}




void ValueMap::update_map_clock(int ck)
{
    if(length_of_buffer() == 0)
    {
        //cout << "nessuna cella da aggiornare." << endl;
        return;
    }

    for(int i = 0; i < ck; i++)
    {
        update_step_1();
        update_step_2();
        clock++;
    }
}




/*----------------------VISUALIZZAZIONE DOPO L'AGGIORNAMENTO----------------------*/

bool ValueMap::test_tot_resource()
{
    if(map_tot_resource == sum_all_tiles())
        return true;
    else
        return false;
}




float ValueMap::get_resource_diff() { return map_tot_resource - sum_all_tiles(); }

int ValueMap::get_clock() { return clock; }




void ValueMap::print_map()
{
    cout << endl;
    
    //stampa la matrice
    for(int i=0; i<map_size; i++)
    {
        for(int j=0; j<map_size; j++)
            cout << map[i][j]->get_actualValue() << "\t";
        cout <<endl;
    }

    cout << endl;
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

    //cout << "GET_LINE_AT position in array x=" << real_x << " y=" << real_y << endl;

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

    Tile* t = NULL;

    switch(index)
    {
        case 0:
            t = get_tile_at(base_x, base_y + 1);
            /*
            cout << "NEXT NEARBY TILE up tile=" << t;
            if(t != NULL)
                cout << " x=" << t->get_x() << " y=" << t->get_y() << endl;
            else
                cout << endl;
            */
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 1);
            else
                return t;
        break;
        case 1:
            t = get_tile_at(base_x + 1, base_y);
            /*
            cout << "NEXT NEARBY TILE right tile= << t";
            if(t != NULL)
                cout << " x=" << t->get_x() << " y=" << t->get_y() << endl;
            else
                cout << endl;
            */
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 2);
            else
                return t;
        break;
        case 2:
            t = get_tile_at(base_x, base_y - 1);
            /*
            cout << "NEXT NEARBY TILE down tile=" << t;
            if(t != NULL)
                cout << " x=" << t->get_x() << " y=" << t->get_y() << endl;
            else
                cout << endl;
            */
            if(t == NULL)
                return next_nearby_tile(base_x, base_y, 3);
            else
                return t;
        break;
        case 3:
            t = get_tile_at(base_x - 1, base_y);
            /*
            cout << "NEXT NEARBY TILE left tile=" << t;
            if(t != NULL)
                cout << " x=" << t->get_x() << " y=" << t->get_y() << endl;
            else
                cout << endl;
            */
            if(t == NULL)
            {
                //cout << "NEXT NEARBY TILE returning NULL..."<< endl;
                return NULL;
            }
            else
                return t;
        break;
        default:
            //cout << "NEXT NEARBY TILE returning NULL..."<< endl;
            return NULL;
    }
    return NULL;
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




bool ValueMap::buffer_contains(Tile* tl)
{
    for(int i = 0; i < length_of_buffer(); ++i)
        if(buffer[idx(i)] == tl)
            return true;

    return false;
}




Tile* ValueMap::get_from_buffer(int index)
{
    int i = idx(index);
    //cout << "GET_FROM_BUFFER i=" << i << endl;

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
    if(buffer_first == ((buffer_last + 1) % buffer_size))
    {
        //cout << "ENQUEUE condition full queue true" << endl;
        return false;
    }

    if(buffer_contains(t))
    {
        //cout << "ENQUEUE condition contains..." << endl;
        return false;
    }
    else
    {
        //cout << "ENQUEUE success" << endl;
        buffer[buffer_last] = t;
        buffer_last = (buffer_last + 1) % buffer_size;
        return true;
    }
}




bool ValueMap::dequeue_all_marked()
{
    //cout << "DEQUEUE starting..." << endl;

    //prima verifico che la coda non sia vuota
    if(buffer_first == buffer_last)
    {
        //cout << "DEQUEUE condition empty queue" << endl;
        return false;
    }

    for(int i = buffer_first; i < buffer_last; i++)
    {
        if(buffer[i]->is_marked())
        {
            buffer[i] = NULL;
            buffer_first = (buffer_first + 1) % buffer_size;
            //cout << "DEQUEUE dequeue buffer_first=" << buffer_first << " buffer_last=" << buffer_last << " buffer_length=" << length_of_buffer() << endl;
        }
        else
            break;
    }

    return true;
}




/*----------------------ALGORITMO DI AGGIORNAMENTO----------------------*/

void ValueMap::update_step_1()
{
    int length_buf = length_of_buffer();

    //cout << "here! UPDATE STEP 1 length_buf=" << length_buf << " start update..." << endl;

    for(int i=0; i < length_buf; i++)
    {
        //recupera la cella e marcala
        Tile* t = get_from_buffer(i);
        //cout << "UPDATE STEP 1 tile=" << t << " base_x=" << t->get_x() << " base_y=" << t->get_y() << endl;
        t->mark();

        //prendi i valori delle celle vicine(e conta anche quante celle vicine ci sono effettivamente)
        int n = 1;
        float res_mean = 0.0;
        for(int j=0; j<4; j++)
        {
            Tile* temp = next_nearby_tile(t->get_x(), t->get_y(), j);
            //cout << "UPDATE STEP 1(calculating mean...) temp=" << temp << endl;
            if(temp == NULL)
                break;
            else
            {
                n++;
                res_mean += temp->get_actualValue();
            }
        }
        res_mean += t->get_actualValue();
        res_mean /= n;
        //cout << "UPDATE STEP 1 mean=" << res_mean << endl;

        //programma aggiornamento cella presente
        float delta_value = (r.get_real_k())*(res_mean - t->get_actualValue());
        //cout << "UPDATE STEP 1 delta=" << delta_value << endl;
        t->set_delta(delta_value);

        //nota: se il delta è a zero, non procedere nell'aggiornamento

        //cout << "UPDATE STEP 1 starting delta assignment..." << endl;

        //programma aggiornamento celle vicine
        n--;
        for(int j=0; j<4; j++)
        {
            Tile* temp = next_nearby_tile(t->get_x(), t->get_y(), j);
            //cout << "UPDATE STEP 1 nearby cell=" << temp << endl;
            if(temp == NULL)
                break;
            else
            {
                //cout << "UPDATE STEP 1 set delta on cell=" << temp << endl;
                temp->set_delta(-(delta_value/static_cast<double>(n)));
                enqueue(temp);
            }
        }

        //cout << "new buffer_length=" << length_of_buffer() << " endinf update step 1..." << endl;
    }
}




void ValueMap::update_step_2()
{
    //cout << "UPDATE step 2 starting..." << endl;
    for(int i=0; i<length_of_buffer();i++)
    {
        //cout << "UPDATE STEP 2 i=" << i << " tile=" << get_from_buffer(i) << endl;
        get_from_buffer(i)->update_actualValue();
    }
    dequeue_all_marked();
    //cout << "UPDATE step 2 ending..." << endl;
}