/*IMPLEMENTAZIONE DELLA CLASSE RESOURCE*/

//#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
//#include <ctime>
#include "ValueMap.h"
using namespace std;

Resource::Resource(float propagFactor, float toll)
{
    this->propagationFactor = propagFactor;
    this->tollerance = toll;

    if(toll > propagFactor)
        tollerance = toll - propagFactor;

    this->seed = 0;
    this->n_ops = 0;
}




Resource::Resource(float propagFactor, float toll, int seed)
{
    this->propagationFactor = propagFactor;
    this->tollerance = toll;
    /*
    if(seed >= 0)
        seed_real_k(seed);
    else
        seed_real_k();
    */
    this->seed = seed;
    this->n_ops = 0;
}




float Resource::get_k() { return propagationFactor; }

float Resource::get_real_k() 
{ 
    float tll = generate_tollerance_value();
    float ttt = propagationFactor + tll;
    //cout << "GET_REAL_K real propagation factor=" << ttt << endl;
    return ttt;
}

void Resource::seed_real_k() { /*srand((this->seed = time(NULL)));*/ return; }

void Resource::seed_real_k(int seed) { srand((this->seed = seed)); }

int Resource::get_seed() { return this->seed; }

float Resource::get_tollerance() { return this->tollerance; }




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
    /*
    //schifo!!!!
    for(int i = 0; i < 25; i++){
    //float ttt = ( ( 2 * rand() ) / RAND_MAX - 1 ) * tollerance;
    float ttt = ( rand()%2 == 0 ? 1 : -1 ) * ( ( rand() ) / RAND_MAX ) * tollerance;
    cout << "RESOURCE(test " << i << ") randomic tollerance=" << ttt << endl;
    }
    float ttt = ( rand()%2 == 0 ? 1 : -1 ) * ( static_cast<float>( rand()  / RAND_MAX ) * tollerance );
    cout << "RESOURCE final randomic tollerance=" << ttt << endl;
    return ttt;
    */
    /*
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(std::random_device()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-tollerance, tollerance);
    float generated_value = (float) (dis(gen) * 0.98);
    cout << "RESOURCE generated randomic tollerance=" << generated_value << endl;
    */
    n_ops++;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    double generated_value;
    for(int i = 0; i < n_ops; i++)
        generated_value = dis(gen) * (double) tollerance;
    //cout << "RESOURCE generated randomic tollerance=" << generated_value << endl;
    return (float) generated_value;
}