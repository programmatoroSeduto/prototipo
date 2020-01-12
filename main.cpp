#include <iostream>
#include <cstdlib>
#include "map.h"
using namespace std;
const int dMap = 10;

enum commands
{
    insert_value,
    next,
};

int main()
{
    Map m = Map(dMap);

    while(1)
    {
        char cmd;
        cout << "->\t"; cin >> cmd; cout << endl;

        bool exit_cmd = false;

        switch(cmd)
        {
            case 'i':
                int x, y;
                float val;
                cout << "posizione x: "; cin >> x; cout << endl;
                cout << "posizione y: "; cin >> y; cout << endl;
                cout << "valore: "; cin >> val; cout << endl;

                //aggiornamento 
                if(m.position(x, y)){
                    cout << "VALORE AGGIORNATO: " << m.value(val) << endl;
                    m.addStartPoint(x, y);
                }
                else{
                    cout << "ERRORE: punto non compreso nella mappa" << endl;
                    continue;
                }
            break;
            case 'n':
                //avanti...
            break;
            case 'x':
                cout << "chiusura...";
                exit_cmd = true;
            break;
            default :
                cout << "comando non valido!\n";
                continue;
        }

        if(exit_cmd) break;

        

    }

    return 0;
}