#include <iostream>
//#include <cstdlib>
#include <string>
#include <cstring>
//#include <istream>
#include "ValueMap.h"
using namespace std;

enum commands 
{
    insert_single_value,
    insert_multi,
    update,
    update_clock,
    print_clock,
    print_map,
    update_and_print,
    print_command_help,
    verify,
    resource_diff,
    logout,
    nop
};

/*
    restituisce il numero associato al comando

    ritorna il valore di un comando se la stringa è valida
    altrimenti ritorna -1
*/
int parse_command(string cmd)
{
    if(cmd == "SET_VALUE")
        return insert_single_value;
    else if (cmd == "SET_VALUE_MULTI")
        return insert_multi;
    else if(cmd == "UPDATE")
        return update;
    else if(cmd == "UPDATE_CK")
        return update_clock;
    else if(cmd == "CK")
        return print_clock;
    else if(cmd == "PRINT_MAP" || cmd == "PRINT" || cmd == "P")
        return print_map;
    else if(cmd == "UPDATE_N_PRINT" || cmd == "UP")
        return update_and_print;
    else if(cmd == "HELP")
        return print_command_help;
    else if(cmd == "EXIT")
        return logout;
    else if(cmd == "VERIFY")
        return verify;
    else if(cmd == "RES_DIFF")
        return resource_diff;
    else if(cmd == "")
        return nop;
    else
        return -1;
}




ValueMap init_map()
{
    cout << "\n" << "dimensioni della mappa: " << endl;
    int dim;
    cin >> dim; cout << endl;
    cout << "velocità di propagazione(tra 0 e 1): ";
    float k;
    cin >> k; cout << endl;
    cout << "tolleranza sulla velocità di propagazione(tra 0 e 1): ";
    float e;
    cin >> e; cout << endl;
    ValueMap m = ValueMap(dim);
    m.new_resource_type(k, e);
    return m;
}




void insert_single_value_f(ValueMap &m)
{
    int x, y; float incr;
    cout << " x : "; 
    cin >> x;
    cout << " y : ";
    cin >> y;
    cout << " valore : ";
    cin >> incr; 
    cout << endl;
    if(!m.set_actual_value_at(x, y, incr))
        cout << "ERRORE: dati non validi." << endl;
}




void print_help_f()
{
    cout << "PROTOTIPO ALGORITMO DI PROPAGAZIONE DI UN VALORE IN UNA MATRICE" << endl
        << "Ecco i comandi:" << endl 
        << "SET_VALUE : modifica il valore di una cella" << endl
        << "SET_VALUE_MULTI : modifica i valori di un certo numero di celle" << endl
        << "UPDATE : aggiornamento per un ciclo di clock" << endl
        << "UPDATE_CK : ripeti l'aggiornamento per un certo ciclo di clock" << endl
        << "CK : stampa l'attuale valore del clock" << endl
        << "PRINT_MAP o PRINT o P : stampa la matrice" << endl
        << "UPDATE_N_PRINT oppure UP : esegui l'aggiornamento per un ciclo e stampa al termine" << endl
        << "HELP : stampa questa guida dei comandi" << endl
        << "VERIFY : verifica se il valore della risorsa è uguale a quello previsto" << endl
        << "RES_DIFF : mostra la differenza tra il valore previsto di risorsa e quello calcolato dalla somma di tutte le celle della mappa" << endl
        << "EXIT : chiudi il simulatore" << endl;
}




int main()
{   
    //inizializzazione mappa
    ValueMap m = init_map();
    bool exit_cmd = false;

    string command_string = "";

    while(!exit_cmd)
    {
        if(command_string == "") cout << "->\t";
        getline(cin, command_string);
        cout << endl;
        
        int cmd = parse_command(command_string);

        switch(cmd)
        {
            case insert_single_value:
                insert_single_value_f(m);
            break;
            case insert_multi:
                cout << "quanti valori? ";
                int n;
                cin >> n;
                for(int i=0; i<n; i++)
                {
                    cout << "inserimento numero " << i << endl;
                    insert_single_value_f(m);
                }
            break;
            case update:
                m.update_map();
                cout << "fatto!" << endl;
            break;
            case update_clock:
                int clk;
                cout << "aggiornamento per quanti cicli di clock? ";
                cin >> clk; cout << endl;
                m.update_map_clock(clk);
            break;
            case print_clock:
                cout << "valore di clock: " << m.get_clock() << endl;
            break;
            case print_map:
                m.print_map();
            break;
            case update_and_print:
                m.update_map();
                m.print_map();
            break;
            case print_command_help:
                print_help_f();
            break;
            case verify:
                if(m.test_tot_resource())
                    cout << "la risorsa si e' conservata." << endl;
                else
                    cout << "Il valore di risorsa non si e' conservato!" << endl
                        << "differenza tra risorsa prevista e risorsa nella mappa: " << m.get_resource_diff() << endl;     
            break;
            case resource_diff:
                cout << "differenza tra valore di risorsa previsto e valore nella mappa: " << m.get_resource_diff() << endl;
            break;
            case logout:
                exit_cmd = true;
            break;
            case nop:
                //do nothing!
            break;
            default:
                cout <<"ERRORE: comando '" << command_string << "' non valido!" << endl
                    << "digita 'HELP' per la lista dei comandi" << endl;
        }
        command_string = "";
    }

    cout << "chiusura..." << endl;
    return 0;
}