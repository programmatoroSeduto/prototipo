/*IMPLEMENTAZIONE DELLA CLASSE RESOURCE*/

#include <cstdlib>
#include <cmath>
#include <ctime>
#include "ValueMap.h"
using namespace std;

Resource::Resource(float propagFactor, float toll)
{
    this->propagationFactor = propagFactor;
    this->tollerance = toll;

    this->seed = 0;
}




Resource::Resource(float propagFactor, float toll, int seed)
{
    this->propagationFactor = propagFactor;
    this->tollerance = toll;

    if(seed >= 0)
        seed_real_k(seed);
    else
        seed_real_k();
    
}




float Resource::get_k() { return propagationFactor; }

float Resource::get_real_k() { return propagationFactor + generate_tollerance_value(); }

void Resource::seed_real_k() { srand((this->seed = time(NULL))); }

void Resource::seed_real_k(int seed) { srand((this->seed = seed)); }

int Resource::get_seed() { return this->seed; }




float Resource::set_k(float new_k) 
{
    propagationFactor = new_k;
    return propagationFactor;
}

float Resource::set_toll(float new_toll) 
{
    tollerance = new_toll;
    return tollerance;
}



float Resource::generate_tollerance_value() 
{
    return ( ( 2 * rand() ) / RAND_MAX - 1 ) * tollerance;
}