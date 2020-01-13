/*
    rappresenta la singola cella
*/
class Tile 
{
    public:

    /*
        COSTRUTTORE
        (x, y) posizione del tile
    */
    Tile(int x, int y);

    //le coordinate della cella
    int get_x(); int get_y();

    //ritorna un valori della cella
    float get_actualValue(); float get_delta();
    
    //somma ad actualValue il valore di delta
    //ritorna il nuovo valore attuale
    float update_actualValue();

    //somma al delta il valore increment
    //ritorna il nuovo valore
    float set_delta(float increment);

    //modifica del valore marked
    void mark(); void unmark();

    //verifica se la cella è stata marcata
    bool is_marked();

    private:

    //posizione della cella
    int x, y;

    //valore attuale, e incremento al prossimo colpo di clock
    float actualValue, delta;
    
    //per l'aggiornamento
    bool marked;
};






/*
    contiene le statistiche della risorsa che dev'essere distribuita all'interno della mappa
*/
class Resource
{
    public:

    /*
        COSTRUTTORE
        propagFactor : velocità di propagazione della risorsa tra celle vicine, in percentuale
        toll : di quanto il propagationFactor reale può discostarsi da quello nominale
    */
    Resource(float propagFactor, float toll);

    //ritorna il propagationFactor nominale
    float get_k();

    //ritorna il propagationFactor reale, calcolato in base alla tolleranza
    float get_real_k();

    //modifica le statistiche della risorsa
    //ritorna il nuovo valore della statistica indicata
    float set_k(float new_k); float set_toll(float new_toll);

    private:

    float propagationFactor;
    float tollerance;

    float generate_tollerance_value();
};






/*
    rappresenta la mappa che contiene le risorse distribuite
*/
class ValueMap
{
    public:

    /*
        istanzia una matrice quadrata di lato dim
        e inizializza tutti i valori utili per la lavorazione
    */
    ValueMap(int dim);

    /*----------------------OPERAZIONI DALL'ESTERNO----------------------*/

    /*
        somma un certo valore a quello attuale di una certa cella
        (x, y) : la posizione della cella nella mappa
        increment : il valore da sommare a quello attuale

        ritorna TRUE se è stato possibile effettuare l'aggiornamento, altrimenti FALSE.
    */
    bool set_actual_value_at(int x, int y, float increment);


    /*----------------------AGGIORNAMENTO----------------------*/

    /*
        ALGORITMO DI AGGIORNAMENTO prima versione
    */
    void update_map();

    /*
        variante: propaga il valore di risorsa nella mappa per ck cicli di clock
    */
    void update_map_clock(int ck);

    /*----------------------VISUALIZZAZIONE DOPO L'AGGIORNAMENTO----------------------*/

    /*
        PER VERIFICARE LA VALIDITA' DEL METODO DI AGGIORNAMENTO
        verifica se la riorsa si è conservata durante gli aggiornamenti
        ovvero verifica se la somma delle risorse calcolata durante gli inserimenti è uguale a quella calcolata con la somma di tutti gli elementi della matrice
    */
    bool test_tot_resource();

    /*
        stampa su console la matrice
    */
    void print_map();


    private:

    //clock, per la simulazione
    int clock;

    //la matrice
    Tile* map; int map_size;

    //totale di risorsa nella mappa
    float map_tot_resource;

    //il tipo di risorsa
    Resource r;
    
    //coordinate dell'origine nella matrice
    int origin_position[2];
    
    //coordinate dei vertici
    int vertex[4][2];

    //il buffer
    Tile* buffer; 
    int buffer_size;

    //indici della coda
    int buffer_first, buffer_last;
    //puntatore utilizzato durante la fase 2 dell'aggiornamento
    int first_marked;

    /*----------------------OPERAZIONI SULLA MATRICE----------------------*/

    /*
        somma i valori di risorsa di tutte le celle nella matrice
        ritorna il risultato della somma
    */
    float sum_all_tiles();
    
    /*
        ritorna il puntatore alla cella in base alle coordinate reali
    */
    Tile* get_tile_at(int x, int y);

    /*
        individua la prossima adiacenza in base al valore di indice e alla propria posizione
        (base_x, base_y) : la posizione della cella che richiede quella vicina
        index : il valore dell'indice durante lo scorrimento

        ritorna il puntatore ad una cella vicina, qualunque essa sia
        NOTA BENE: il metodo tiene conto automaticamente delle celle raggiungibili
    */
    Tile* next_nearby_tile(int base_x, int base_y, int index);

    /*----------------------OPERAZIONI SUL BUFFER----------------------*/
    
    /*
        ritorna l'elemento numero tot nel buffer
        il metodo tiene conto automaticamente della struttura ad array della coda

        ritorna null nel caso l'indice sia troppo grande

        è possibile scorrere il buffer in due direzioni:
        index positivo: muovi dal primo elemento all'ultimo
        index negativo: muovi l'indica dall'ultimo elemento al primo
    */
    Tile *get_from_buffer(int index);

    /*
        ritorna l'elemento numero tot nel buffer
        il metodo tiene conto automaticamente della struttura ad array della coda

        è possibile scorrere il buffer in due direzioni:
        index positivo: muovi dal primo elemento all'ultimo
        index negativo: muovi l'indica dall'ultimo elemento al primo

        se l'elemento non è unmarked, ritorna null anzichè il tile.
        ritorna null anche se l'indice dovesse sforare i limiti.
    */
    Tile* get_from_buffer_if_unmarked(int index);

    /*
        ritorna quanti elementi ci sono nel buffer
    */
    int length_of_buffer();

    /*
        metti in coda un tile
    */
    bool enqueue(Tile* t);

    /*
        elimina dalla coda tutti i tile marcati
    */
    bool dequeue_all_marked();

};




