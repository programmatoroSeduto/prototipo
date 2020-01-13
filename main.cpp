#include <iostream>
#include <cstdlib>
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
    logout
};

/*
    restituisce il numero associato al comando

    ritorna il valore di un comando se la stringa è valida
    altrimenti ritorna -1
*/
int parse_command(char* cmd)
{
    if(strncmp(cmd, "SET_VALUE", 9))
        return insert_single_value;
    else if (strncmp(cmd, "SET_VALUE_MULTI", 15))
        return insert_multi;
    else if(strncmp(cmd, "UPDATE", 6))
        return update;
    else if(strncmp(cmd, "UPDATE_CK", 9))
        return update_clock;
    else if(strncmp(cmd, "CK", 2))
        return print_clock;
    else if(strncmp(cmd, "PRINT_MAP", 9))
        return print_map;
    else if(strncmp(cmd, "UPDATE_N_PRINT", 14))
        return update_and_print;
    else if(strncmp(cmd, "HELP", 4))
        return print_command_help;
    else if(strncmp(cmd, "EXIT", 4))
        return logout;
    else
        return -1;
}




ValueMap init_map()
{
    cout << "\n" << "dimensioni della mappa: " << endl;
    int dim;
    cin >> dim; cout << endl;
    return ValueMap(dim);
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
        << "PRINT_MAP : stampa la matrice" << endl
        << "UPDATE_N_PRINT : esegui l'aggiornamento per un ciclo e stampa al termine" << endl
        << "HELP : stampa questa guida dei comandi" << endl
        << "EXIT : chiudi il simulatore" << endl;
}




int main()
{
    //inizializzazione mappa
    ValueMap m = init_map();
    bool exit_cmd = false;

    while(!exit_cmd)
    {
        char* command_string;
        cin.getline(command_string, 50);
        int cmd = parse_command(command_string);

        switch(cmd)
        {
            case insert_single_value:
                insert_single_value_f(m);
            break;
            case insert_multi:
                cout << "quanti valori! ";
                int n = 0;
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
            case logout:
                exit_cmd = true;
            break;
            default:
                cout <<"ERRORE: comando " << command_string << "non valido!" << endl
                    << "digita 'HELP' per la lista dei comandi" << endl;
        }
    }

    cout << "chiusura..." << endl;
    return 0;
}