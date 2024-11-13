// gcc main.c -o game_of_life -lncurses

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "templates.h"

#define MIN_SIZE 10
#define MAX_SIZE 50

#define DELAY 200000  

int  width, height;
// Variabili per la griglia attuale e quella successiva, utilizzata per calcolare la prossima generazione senza modificare la griglia attuale
// Finita la generazione, la griglia successiva diventa la griglia attuale
int **grid, **next_grid;

// Variabili globali per la posizione iniziale del cursore  
int cursor_x = 0, cursor_y = 0;

// Flag per controllare se il gioco è in esecuzione o in pausa
int is_running = 0;
int is_paused = 0;

// Variabile per salvare lo stato iniziale della griglia
int **initial_state;

// Variabile per tenere traccia del numero di generazioni
unsigned long generation = 0;

// Delay iniziale in microsecondi tra un frame e l'altro
int current_delay = 200000;  

void show_menu(void);   

// Funzioni di allocazione e liberazione memoria
int** allocate_grid(int rows, int cols) {
    int **grid = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++) {
        grid[i] = (int*)calloc(cols, sizeof(int));
    }
    return grid;
}

void free_grid(int **grid) {
    for(int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);
}

// Funzione per contare le celle attive
int count_active_cells() {
    int count = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(grid[i][j]) count++;
        }
    }
    return count;
}


// Funzione per disegnare la griglia
void draw_grid() {

    // Pulisce il terminale
    clear();
    // Calcola la posizione di partenza per centrare la griglia
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width * 2) / 2;
    
    int center_x = cursor_x - (width / 2);
    int center_y = (height / 2) - cursor_y;
    
    // Stampa le informazioni in alto a sinistra
    mvprintw(0, 0, "Generazione: %lu | Popolazione: %d | Velocità: %dms | Posizione: (%d,%d) | Centro: (%d,%d)", 
             generation,
             count_active_cells(),
             current_delay/1000, 
             cursor_x, cursor_y,
             center_x, -center_y);

    // Disegna la griglia
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            // Se le coordinate corrispondono a quelle del cursore, stampa "X"
            if(!is_running && i == cursor_y && j == cursor_x) {
                mvprintw(start_y + i, start_x + j * 2, "X ");
            // Altrimenti, se la cella è attiva, stampa "O"
            } else if(grid[i][j]) {
                mvprintw(start_y + i, start_x + j * 2, "O ");
            // Altrimenti, stampa due spazi, cella non attiva
            } else {
                mvprintw(start_y + i, start_x + j * 2, "  ");
            }
        }
    }
    refresh();
}

// Funzione per contare i vicini di una cella
int count_neighbors(int y, int x) {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            
            // Per le coordinate che vanno oltre la griglia utilizzo il modulo per calcolare la posizione "opposta"
            int ny = (y + i + height) % height;  
            int nx = (x + j + width) % width;    
            
            count += grid[ny][nx];
        }
    }
    return count;
}

// Calcola la prossima generazione
void calculate_next_generation() {
    // Per ogni cella della griglia calcola il numero di vicini
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            int neighbors = count_neighbors(i, j);
            // Applica le regole, se la cella è attiva e ha 2 o 3 vicini, rimane attiva
            if(grid[i][j]) {
                next_grid[i][j] = (neighbors == 2 || neighbors == 3);
            // Se la cella è inattiva e ha esattamente 3 vicini, diventa attiva
            } else {
                next_grid[i][j] = (neighbors == 3);
            }
        }
    }

    // Scambia le griglie, imposto la griglia successiva come griglia attuale
    int **temp = grid;
    grid = next_grid;
    next_grid = temp;
}

// Funzione per resettare la griglia impostando tutte le celle a 0
void reset_grid() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            grid[i][j] = 0;
        }
    }
}

// Funzione per salvare lo stato iniziale
void save_initial_state() {
    initial_state = allocate_grid(height, width);
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            initial_state[i][j] = grid[i][j];
        }
    }
}

// Funzione per ripristinare lo stato iniziale
void restart_game() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            grid[i][j] = initial_state[i][j];
        }
    }
}

// Funzione per la modalità edit
void edit_mode() {

    int ch;
    nodelay(stdscr, FALSE);  
    draw_grid(); 
    
    // Cicla finché non si preme invio, ogni volta viene fatta una draw_grid() per aggiornare la posizione del cursore e le celle
    while(1) {
        ch = getch();
        switch(ch) {
            // Movimento del cursore in alto, in basso, a sinistra e a destra
            case KEY_UP:
                cursor_y = (cursor_y - 1 + height) % height;
                draw_grid();
                break;
            case KEY_DOWN:
                cursor_y = (cursor_y + 1) % height;
                draw_grid();
                break;
            case KEY_LEFT:
                cursor_x = (cursor_x - 1 + width) % width;
                draw_grid();
                break;
            case KEY_RIGHT:
                cursor_x = (cursor_x + 1) % width;
                draw_grid();
                break;
            // Attiva/disattiva la cella della stessa posizione del cursore
            case ' ':
                grid[cursor_y][cursor_x] = !grid[cursor_y][cursor_x];
                draw_grid();
                break;
            // Aumenta/Diminuisce la velocità di esecuzione
            case '+':
            case '=':
                current_delay = (current_delay > 50000) ? current_delay - 50000 : current_delay;
                draw_grid();
                break;
            case '-':
                current_delay = (current_delay < 500000) ? current_delay + 50000 : current_delay;
                draw_grid();
                break;
            // Torna al menu principale
            case 'm':
                is_running = 0;
                show_menu();
                return;
            // Avvia la simulazione
            case '\n':
                // Viene salvato lo stato iniziae della griglia nel caso si voglia ripristinare con "r"
                save_initial_state();
                nodelay(stdscr, TRUE);
                return;
        }
    }
}

// Loop principale del gioco
void game_loop() {

    int ch;
    is_running = 1;
    nodelay(stdscr, TRUE);
    
    // Quando la simulazione è in esecuzione, il gioco cicla costantemente
    while(is_running) {
        if((ch = getch()) != ERR) {
            switch(ch) {

                // Aumenta la velocità diminuendo il delay tra un frame e un altro
                case '+':
                case '=':
                    current_delay = (current_delay > 50000) ? current_delay - 50000 : current_delay;
                    draw_grid();  
                    break;
                // Diminuisce la velocità aumentando il delay tra un frame e un altro
                case '-':
                    current_delay = (current_delay < 500000) ? current_delay + 50000 : current_delay;
                    draw_grid();
                    break;
                // Torna al menu principale
                case 'm':
                    is_running = 0;
                    show_menu();
                    return;
                // Esci dal gioco
                case 'q':
                    is_running = 0;
                    break;
                // Pausa/Play
                case 'p':
                    is_paused = !is_paused;
                    break;
                // Reset della griglia alle condizioni iniziali
                case 'r':
                    restart_game();
                    generation = 0;
                    // Reset posizione cursore
                    cursor_x = width / 2;    
                    cursor_y = height / 2;
                    // Ridisegna la griglia e torna alla modalità edit
                    draw_grid();            
                    edit_mode();           
                    break;
                // Solo se il gioco è in pausa si può skippare al prossimo frame/step
                case 'n':
                    if(is_paused) {
                        calculate_next_generation();
                        generation++;
                        draw_grid();
                    }
                    break;
            }
        }
        // Se il gioco non è in pausa, calcola la prossima generazione e disegna la griglia
        if(!is_paused) {
            calculate_next_generation();
            generation++;
            draw_grid();
            usleep(current_delay);  
        }
    }
    // Disabilita il nodelay per evitare che il terminale si blocchi con l'input da tastiera
    nodelay(stdscr, FALSE);
}

void show_menu() {

    // Inizializzazione colori personalizzati per il terminale
    start_color();

    // Definizione colori personalizzati 
    init_color(7, 51, 98, 153);           // #0d1927 - Blu scuro (sfondo)
    init_color(8, 494, 243, 624);         // #7e3e9f - Viola 
    init_color(9, 773, 255, 204);         // #c54134 - Rosso 
    init_color(10, 80, 400, 380);         // #1d857a - Verde 
    init_color(11, 36, 478, 820);         // #247ad1 - Blu chiaro
    init_color(12, 784, 800, 804);        // #c8cccd - Grigio chiaro

    // Coppie di colori
    init_pair(1, 9, 7);                   // Rosso su blu scuro
    init_pair(2, 8, 7);                   // Viola su blu scuro
    init_pair(3, 12, 7);                  // Grigio chiaro su blu scuro
    init_pair(4, 10, 7);                  // Verde su blu scuro
    init_pair(5, 11, 7);                  // Blu chiaro su blu scuro

    // Imposta il colore dello sfondo
    bkgd(COLOR_PAIR(3));  

    // Calcola posizione centrale per il menu
    int menu_x = COLS/2 - 20;
    int menu_y = LINES/2 - 2;

    // Libera la memoria delle griglie esistenti nel caso si ritorna al menu dopo una simulazione
    if(grid != NULL) {
        free_grid(grid);
        free_grid(next_grid);
        if(initial_state != NULL) {
            free_grid(initial_state);
        }
    }

    // Reset delle variabili globali
    generation = 0;
    is_paused = 0;
    cursor_x = cursor_y = 0;
    
    // Pulizia terminale
    clear();
    
    // Lunghezza del titolo e calcolo della posizione per centrarlo
    int text_width = 105; 
    int start_x = (COLS - text_width) / 2;

    // Stampa del titolo in grassetto e colorato (viola)
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(LINES/6-4, start_x, " ______     ______     __    __     ______        ______     ______        __         __     ______   ______");
    mvprintw(LINES/6-3, start_x, "/\\  ___\\   /\\  __ \\   /\\ \"-./  \\   /\\  ___\\      /\\  __ \\   /\\  ___\\      /\\ \\       /\\ \\   /\\  ___\\ /\\  ___\\  ");
    mvprintw(LINES/6-2, start_x, "\\ \\ \\__ \\  \\ \\  __ \\  \\ \\ \\-./\\ \\  \\ \\  __\\      \\ \\ \\/\\ \\  \\ \\  __\\      \\ \\ \\____  \\ \\ \\  \\ \\  __\\ \\ \\  __\\  ");
    mvprintw(LINES/6-1, start_x, " \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\ \\ \\_\\  \\ \\_____\\     \\ \\_____\\  \\ \\_\\         \\ \\_____\\  \\ \\_\\  \\ \\_\\    \\ \\_____\\");
    mvprintw(LINES/6, start_x, "  \\/_____/   \\/_/\\/_/   \\/_/  \\/_/   \\/_____/      \\/_____/   \\/_/          \\/_____/   \\/_/   \\/_/     \\/_____/");
    attroff(COLOR_PAIR(2) | A_BOLD);

    /*
     ______     ______     __    __     ______        ______     ______        __         __     ______   ______   
    /\  ___\   /\  __ \   /\ "-./  \   /\  ___\      /\  __ \   /\  ___\      /\ \       /\ \   /\  ___\ /\  ___\  
    \ \ \__ \  \ \  __ \  \ \ \-./\ \  \ \  __\      \ \ \/\ \  \ \  __\      \ \ \____  \ \ \  \ \  __\ \ \  __\  
     \ \_____\  \ \_\ \_\  \ \_\ \ \_\  \ \_____\     \ \_____\  \ \_\         \ \_____\  \ \_\  \ \_\    \ \_____\
      \/_____/   \/_/\/_/   \/_/  \/_/   \/_____/      \/_____/   \/_/          \/_____/   \/_/   \/_/     \/_____/
                                                                        
    */

    // Stampa dei vari template disponibili
    for(int i = 0; i < num_templates; i++) {
        mvprintw(LINES/4 + i, menu_x, " %s", templates[i]->name);
    }
    
    // Stampa dell'opzione per la griglia personalizzata
    mvprintw(LINES/4 + num_templates, menu_x, " Griglia personalizzata");
    
    // Griglia dei comandi sulla sinistra con titoli in grassetto, sottolineati e colorati (rosso)
    int commands_x = 7;
    int commands_y = LINES/4 ;

    attron(COLOR_PAIR(1) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 1, commands_x, "COMANDI DI GIOCO:");
    attroff(COLOR_PAIR(1) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 2, commands_x, "Modalità Edit:");
    mvprintw(commands_y + 3, commands_x, "UP DOWN -> <-: Muovi cursore");
    mvprintw(commands_y + 4, commands_x, "SPAZIO: Attiva/Disattiva cella");
    mvprintw(commands_y + 5, commands_x, "+/-: Aumenta/Diminuisce velocità");
    mvprintw(commands_y + 6, commands_x, "INVIO: Avvia simulazione");
    
    attroff(COLOR_PAIR(1) | A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(1) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 8, commands_x, "DURANTE IL GIOCO:");
    attroff(COLOR_PAIR(1) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 9, commands_x, "p: Pausa/Play");
    mvprintw(commands_y + 10, commands_x, "n: Prossimo frame (in pausa)");
    mvprintw(commands_y + 11, commands_x, "r: Restart");
    mvprintw(commands_y + 12, commands_x, "m: Menu principale");
    mvprintw(commands_y + 13, commands_x, "q: Esci");
    mvprintw(commands_y + 14, commands_x, "+/-: Aumenta/Diminuisce velocità");
    attron(COLOR_PAIR(5) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 16, commands_x, "Dimensione terminale:");
    attroff(COLOR_PAIR(5) | A_BOLD | A_UNDERLINE);
    mvprintw(commands_y + 16, commands_x + 21," %d x %d ", COLS/2, LINES-2);
    
    // Aggiorna il terminale
    refresh();
    

    // Variabile per la scelta del template
    int choice = 0;

    // Posizione iniziale del cursore
    int cursor_pos = 1; 
    // Il while cicla finché non si preme invio e gestisce lo spoiler del cursore "X" per selezionare il template/griglia personalizzata
    while (1) {

        // Pulisce i precedenti elementi del menu
        for(int i = 0; i < num_templates + 1; i++) {
            mvprintw(LINES/4 + i, menu_x - 2, " ");
        }

        // Mostra il cursore alla posizione corrente in grassetto e colorato (verde)
        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(LINES/4 + (cursor_pos - 1), menu_x - 2, "X");
        attroff(COLOR_PAIR(4) | A_BOLD);
        refresh();

        // Ottiene l'input da tastiera
        int key = getch();

        // Gestione del movimento del cursore e della selezione
        // Se si preme UP il cursore si sposta verso l'alto, se si preme DOWN il cursore si sposta verso il basso
        // Lo spostamento accompagna anche l'incremento o il decremento della variabile cursor_pos che sarà la scelta finale choice
        // Il codice tiene anche conto di rimanere nei limiti tra 1-numero_templates
        if (key == KEY_UP) {
            cursor_pos = (cursor_pos > 1) ? cursor_pos - 1 : cursor_pos;
        } else if (key == KEY_DOWN) {
            cursor_pos = (cursor_pos < num_templates + 1) ? cursor_pos + 1 : cursor_pos;
        } else if (key == '\n') {
            choice = cursor_pos;
            break;
        }
    }

    // Se la scelta è minore o uguale al numero di template disponibili è stato selezionato un template
    if(choice <= num_templates) {

        // Ottieni le dimensioni esatte del terminale facendo spazio in altezza alla barra delle informazioni in gioco
        // Dimezza la larghezza del terminale per avere una perfettamente centrata con la visuale del terminale
        height = LINES - 2;
        width = COLS/2;

        // Alloca le griglie
        grid = allocate_grid(height, width);
        next_grid = allocate_grid(height, width);

        // Chiama la funzione in templates.h per caricare il template, il quale è un insieme di coordinate.
        // Ogni coordinata è un punto della griglia che verrà attivato tutto il resto sarà di default disattivato
        load_template_centered(templates[choice - 1], grid, height, width);

    } else {

        // Altrimenti se è stata scelta la griglia personalzzata
        // Pulisce il terminale e abilita l'echo per la lettura delle dimensioni
        clear();
        echo();
        char input[32];
        mvprintw(LINES/4, COLS/2 - 20, "Larghezza griglia (%d-%d): ", MIN_SIZE, COLS/2);
        refresh();
        // Legge la larghezza della griglia
        getstr(input);
        width = atoi(input);
        
        mvprintw(LINES/4 + 2, COLS/2 - 20, "Altezza griglia (%d-%d): ", MIN_SIZE, LINES-2);
        refresh();
        // Legge l'altezza della griglia
        getstr(input);
        height = atoi(input);

        // Disabilita l'echo degli input
        noecho();
        
        // Se le dimensioni non sono valide termina il gioco
        // Le dimensioni devono essere comprese tra MIN_SIZE (10) e i limiti del terminale
        if(width < MIN_SIZE || width > COLS/2 || height < MIN_SIZE || height > LINES-2) {
            endwin();
            printf("Dimensioni non valide!\n");
            exit(1);
        }
        
        // Alloca le griglie 
        grid = allocate_grid(height, width);
        next_grid = allocate_grid(height, width);
    }
    
    // In entrambi i casi si passa alla modalità edit e successivamente al game loop
    edit_mode();    
    game_loop();
}

int main() {
    // Inizializzazione esistente
    // Inizializzazione ncurses
    initscr();
    // Disabilita il buffering della riga di input
    cbreak();
    // Disabilita l'echo dei caratteri digitati
    noecho();
    // Abilita l'uso dei tasti funzione come le frecce
    keypad(stdscr, TRUE);
    // Nasconde il cursore
    curs_set(0);
        
    // Avvio menu del gioco
    show_menu();
    
    // Termina il gioco
    endwin();
    return 0;
}