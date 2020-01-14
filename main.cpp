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
    nop,
    set_resource_type,
    print_resource_type
};

/*
    restituisce il numero associato al comando

    ritorna il valore di un comando se la stringa è valida
    altrimenti ritorna -1
*/
int parse_command(string cmd)
{
    if(cmd == "SET_VALUE" || cmd == "SET" || cmd == "S")
        return insert_single_value;
    else if (cmd == "SET_RESOURCE")
        return set_resource_type;
    else if (cmd == "RESOURCE")
        return print_resource_type;
    else if (cmd == "SET_VALUE_MULTI" || cmd == "SET_MULTI" || cmd == "SM")
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
    cout << "velocita' di propagazione(tra 0 e 1): ";
    float k;
    cin >> k; cout << endl;
    cout << "tolleranza sulla velocita' di propagazione(tra 0 e 1): ";
    float e;
    cin >> e; cout << endl;
    ValueMap m = ValueMap(dim);
    if(k>=0 && k<=1 && e<=k && e>=0 && e<=1)
        m.new_resource_type(k, e);  
    else
        cout << "ERRORE: dati non validi." << endl;
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
        << "SET_RESOURCE : definisci un nuovo tipo di risorsa, rimpiazzando quello precedentemente definito" << endl
        << "RESOURCE : visualizza le statistiche della risorsa attualmente usata dal simulatore" << endl
        << "SET_VALUE o SET o S : modifica il valore di una cella" << endl
        << "SET_VALUE_MULTI o SET_MULTI o SM: modifica i valori di un certo numero di celle" << endl
        << "UPDATE : aggiornamento per un ciclo di clock" << endl
        << "UPDATE_CK : ripeti l'aggiornamento per un certo ciclo di clock" << endl
        << "CK : stampa l'attuale valore del clock" << endl
        << "PRINT_MAP o PRINT o P : stampa la matrice" << endl
        << "UPDATE_N_PRINT oppure UP : esegui l'aggiornamento per un ciclo e stampa al termine" << endl
        << "HELP : stampa questa guida dei comandi" << endl
        << "VERIFY : verifica se il valore della risorsa e' uguale a quello previsto" << endl
        << "RES_DIFF : mostra la differenza tra il valore previsto di risorsa e quello calcolato dalla somma di tutte le celle della mappa" << endl
        << "EXIT : chiudi il simulatore" << endl << endl;
}




int main()
{   
    cout << "Francesco Ganci, Lorenzo Terranova" << endl
        << " UNTOLD GAMES - value spread throughout the basecamp - PROTOTIPO ALGORITMO DI PROPAGAZIONE" << endl << endl;

    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);

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
        Resource res;

        switch(cmd)
        {
            case set_resource_type:
                float k, e;
                cout << "fattore di propagazione: ";
                cin >> k; cout << endl;
                cout << "tolleranza sul fattore di propagazione: ";
                cin >> e; cout << endl;
                if(!m.new_resource_type(k, e))
                    cout << "ERRORE: dati immessi non validi! k:" << k << " e:" << e << endl;
            break;
            case print_resource_type:
                res = m.get_resource_stats();
                cout << "coefficiente di propagazione=" << res.get_k() << "\ntolleranza=" << res.get_tollerance() << "\nseed=" << res.get_seed() << endl;
            break;
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