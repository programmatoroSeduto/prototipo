/*IMPLEMENTAZIONE DELLA CLASSE TILE*/

#include <cstdlib>
#include <iostream>
#include "ValueMap.h"
using namespace std;

Tile::Tile(int x, int y)
{
    //posizione della cella nella mappa
    this->x = x;
    this->y = y;

    //stato attuale della cella
    actualValue = 0; delta = 0; 
    marked = false;
}




int Tile::get_x() { return x; }

int Tile::get_y() { return y; }

float Tile::get_actualValue() { return actualValue; }

float Tile::get_delta() { return delta; }




float Tile::update_actualValue()
{
    actualValue += delta;
    delta = 0;
    return actualValue;
}

float Tile::set_delta(float increment)
{
    delta += increment;
    return delta;
}




void Tile::mark() { marked = true; }

void Tile::unmark() { marked = false; }

bool Tile::is_marked() { return marked; }