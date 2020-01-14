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
    Tile() = default; Tile(int x, int y);

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

        NOTA BENE: nessun controllo sulla validità delle informazioni inserite!
    */
    Resource() = default; Resource(float propagFactor, float toll); 
    
    /*
        COSTRUTTORE con seed
        propagFactor : velocità di propagazione della risorsa tra celle vicine, in percentuale
        toll : di quanto il propagationFactor reale può discostarsi da quello nominale
        seed : seme per il generatore casuale
            seed >= 0 : seed col valore dato
            seed < 0 : seed in base al tempo di sistema

        NOTA BENE: nessun controllo sulla validità delle informazioni inserite!
    */
    Resource(float propagFactor, float toll, int seed);

    //ritorna il propagationFactor nominale
    float get_k();

    //ritorna il propagationFactor reale, calcolato in base alla tolleranza
    float get_real_k();

    /*
        seed del generatore casuale
    */
    void seed_real_k(); void seed_real_k(int seed);

    //valore del seed usato per generare i valori della tolleranza
    int get_seed();

    /*
        modifica le statistiche della risorsa
        ritorna il nuovo valore della statistica indicata

        NOTA BENE: nessun controllo sulla validità delle informazioni inserite!
    */
    float set_k(float new_k); float set_toll(float new_toll);

    private:

    float propagationFactor;
    float tollerance;

    int seed;

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
    ValueMap() = default; ValueMap(int dim);

    /*----------------------OPERAZIONI DALL'ESTERNO----------------------*/

    /*
        somma un certo valore a quello attuale di una certa cella
        (x, y) : la posizione della cella nella mappa
        increment : il valore da sommare a quello attuale

        ritorna TRUE se è stato possibile effettuare l'aggiornamento, altrimenti FALSE.
    */
    bool set_actual_value_at(int x, int y, float increment);

    /*
        ridefinisci le statistiche della risorsa
        ritorna TRUE se i dati inseriti sono corretti, altrimenti FALSE
    */
    bool new_resource_type(float k, float toll);


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
        PER VERIFICARE LA VALIDITA' DEL METODO DI AGGIORNAMENTO
        ritorna la differenza tra il valore memorizzato durante le aggiunte, e la somma di tutte le celle
        se l'algoritmo funziona correttamente, allora il valore restituito è zero.
    */
    float get_resource_diff();

    /*
        stampa su console la matrice
    */
    void print_map();

    /*
        ritorna il valore di clock attuale
    */
    int get_clock();


    private:

    //clock, per la simulazione
    int clock;

    //la matrice
    Tile* **map; int map_size;

    //totale di risorsa nella mappa
    float map_tot_resource;

    //il tipo di risorsa
    Resource r;
    
    //coordinate dell'origine nella matrice
    int origin_position[2];
    
    //coordinate dei vertici
    //sono V1 in alto a destra
    //e V3 in basso a sinistra
    int vertex[2][2];

    //il buffer
    Tile* *buffer; int buffer_size;

    //indici della coda
    int buffer_first, buffer_last;

    /*----------------------OPERAZIONI SULLA MATRICE----------------------*/

    /*
        somma i valori di risorsa di tutte le celle nella matrice
        ritorna il risultato della somma
    */
    float sum_all_tiles();
    
    /*
        ritorna il puntatore alla cella in base alle coordinate reali, se le coordinate sono valide
        altrimenti ritorna NULL
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
        esegue l'operazione di modulo per l'accesso all'array buffer
        tiene anche conto del segno

        ritorna
        un indice >= 0 se i è valido
        -1 altrimenti
    */
    int idx(int i);

    /*
        verifica se quel tile è contenuto o no nel buffer
        ritorna TRUE se lo contiene
        altrimenti FALSE
    */
    bool buffer_contains(Tile* tl);
    
    /*
        ritorna l'elemento numero tot nel buffer
        il metodo tiene conto automaticamente della struttura ad array della coda

        ritorna NULL nel caso l'indice sia non valido

        è possibile scorrere il buffer in due direzioni:
        index positivo: muovi dal primo elemento all'ultimo
        index negativo: muovi l'indica dall'ultimo elemento al primo
    */
    Tile* get_from_buffer(int index);

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

    /*----------------------ALGORITMO DI AGGIORNAMENTO----------------------*/

    /*
        FASE 1 dell'algoritmo di aggiornamento
    */
    void update_step_1();

    /*
        FASE 2 dell'algoritmo di aggiornamento
    */
    void update_step_2();

};




