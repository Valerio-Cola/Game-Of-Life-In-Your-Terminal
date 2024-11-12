// gcc main.c -o game_of_life -lncurses

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "templates.h"

#define MIN_SIZE 10
#define MAX_SIZE 50

#define DELAY 200000  // Aumentato da 100000 a 200000 (200ms tra i frame)

int  width, height;
int **grid, **next_grid;
int cursor_x = 0, cursor_y = 0;
int is_running = 0;
int is_paused = 0;

// Aggiungi questa funzione per salvare lo stato iniziale
int **initial_state;

// Aggiungi dopo le altre variabili globali
unsigned long generation = 0;

// Aggiungi dopo le altre variabili globali
int current_delay = 200000;  // Delay iniziale in microsecondi

// All'inizio del file, dopo gli #include e le definizioni globali
void show_menu(void);  // Prototipo della funzione

// Funzioni di allocazione e gestione memoria
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

// Modifica della funzione draw_grid
void draw_grid() {
    clear();
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width * 2) / 2;
    
    int center_x = cursor_x - (width / 2);
    int center_y = (height / 2) - cursor_y;
    
    // Aggiunto contatore popolazione
    mvprintw(0, 0, "Generazione: %lu | Popolazione: %d | Velocità: %dms | Posizione: (%d,%d) | Centro: (%d,%d)", 
             generation,
             count_active_cells(),  // Nuovo contatore
             current_delay/1000, 
             cursor_x, cursor_y,
             center_x, -center_y);

    // Resto del codice invariato
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(!is_running && i == cursor_y && j == cursor_x) {
                mvprintw(start_y + i, start_x + j * 2, "X ");
            } else if(grid[i][j]) {
                mvprintw(start_y + i, start_x + j * 2, "O ");
            } else {
                mvprintw(start_y + i, start_x + j * 2, "  ");
            }
        }
    }
    refresh();
}

// Modifica la funzione count_neighbors per implementare il wrapping
int count_neighbors(int y, int x) {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            
            // Calcola le coordinate con wrapping
            int ny = (y + i + height) % height;  // Wrapping verticale
            int nx = (x + j + width) % width;    // Wrapping orizzontale
            
            count += grid[ny][nx];
        }
    }
    return count;
}

// Calcola la prossima generazione
void calculate_next_generation() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            int neighbors = count_neighbors(i, j);
            if(grid[i][j]) {
                next_grid[i][j] = (neighbors == 2 || neighbors == 3);
            } else {
                next_grid[i][j] = (neighbors == 3);
            }
        }
    }

    // Scambia le griglie
    int **temp = grid;
    grid = next_grid;
    next_grid = temp;
}

// Aggiungi questa funzione dopo le altre funzioni di gestione griglia
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

// Modifica edit_mode()
void edit_mode() {
    int ch;
    nodelay(stdscr, FALSE);  // Imposta input bloccante
    draw_grid();  // Disegna una volta all'inizio
    
    while(1) {
        ch = getch();
        switch(ch) {
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
            case ' ':
                grid[cursor_y][cursor_x] = !grid[cursor_y][cursor_x];
                draw_grid();
                break;
            case '+':
            case '=':
                current_delay = (current_delay > 50000) ? current_delay - 50000 : current_delay;
                draw_grid();
                break;
            case '-':
                current_delay = (current_delay < 500000) ? current_delay + 50000 : current_delay;
                draw_grid();
                break;
            case '\n':
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
    
    while(is_running) {
        if((ch = getch()) != ERR) {
            switch(ch) {
                case '+':
                case '=':
                    current_delay = (current_delay > 50000) ? current_delay - 50000 : current_delay;
                    draw_grid();  // Forza l'aggiornamento
                    break;
                case '-':
                    current_delay = (current_delay < 500000) ? current_delay + 50000 : current_delay;
                    draw_grid();  // Forza l'aggiornamento
                    break;
                case 'm':
                    is_running = 0;
                    show_menu();
                    return;
                case 'q':
                    is_running = 0;
                    break;
                case 'p':
                    is_paused = !is_paused;
                    break;
                case 'r':
                    // Reset della griglia alle condizioni iniziali
                    restart_game();
                    generation = 0;
                    cursor_x = width / 2;    // Reset posizione cursore
                    cursor_y = height / 2;
                    draw_grid();            // Ridisegna la griglia
                    edit_mode();           // Torna alla modalità edit
                    break;
                case 'n':
                    if(is_paused) {
                        calculate_next_generation();
                        generation++;
                        draw_grid();
                    }
                    break;
            }
        }
        
        if(!is_paused) {
            calculate_next_generation();
            generation++;
            draw_grid();
            usleep(current_delay);  // Usa il delay variabile
        }
    }
    nodelay(stdscr, FALSE);
}

void show_menu() {

    start_color();

    // Definizione colori personalizzati usando indici custom
    init_color(7, 51, 98, 153);           // #0d1927 - Blu scuro (sfondo)
    init_color(8, 494, 243, 624);         // #7e3e9f - Viola 
    init_color(9, 773, 255, 204);         // #c54134 - Rosso 
    init_color(10, 80, 400, 380);         // #1d857a - Verde 
    init_color(11, 36, 478, 820);         // #247ad1 - Blu chiaro
    init_color(12, 784, 800, 804);        // #c8cccd - Grigio chiaro

    // Coppie di colori aggiornate con i nuovi indici
    init_pair(1, 9, 7);                   // Rosso su blu scuro
    init_pair(2, 8, 7);                   // Viola su blu scuro
    init_pair(3, 12, 7);                  // Grigio chiaro su blu scuro
    init_pair(4, 10, 7);                  // Verde su blu scuro
    init_pair(5, 11, 7);                  // Blu chiaro su blu scuro

    bkgd(COLOR_PAIR(3));  

    // Calcola posizione centrale per il menu
    int menu_x = COLS/2 - 20;
    int menu_y = LINES/2 - 2;

    // Libera la memoria delle griglie esistenti
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
    
    // Mostra il menu e gestisci la selezione
    //int choice = select_mode();
    clear();
    
    // Titolo del menu

    int text_width = 105;  // Width of the ASCII art text
    int start_x = (COLS - text_width) / 2;  // Calculate starting x position to center

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

    // Opzioni di gioco
    for(int i = 0; i < num_templates; i++) {
        mvprintw(LINES/4 + i, menu_x, " %s", templates[i]->name);
    }
    
    mvprintw(LINES/4 + num_templates, menu_x, " Griglia personalizzata");
    // Comandi sulla sinistra
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
    
    refresh();
    
    int choice = 0;
    int cursor_pos = 1;  // Initial cursor position
    while (1) {
        // Clear previous menu items
        for(int i = 0; i < num_templates + 1; i++) {
            mvprintw(LINES/4 + i, menu_x - 2, " ");
        }

        // Show cursor at current position
        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(LINES/4 + (cursor_pos - 1), menu_x - 2, "X");
        attroff(COLOR_PAIR(4) | A_BOLD);
        refresh();

        int key = getch();
        if (key == KEY_UP) {
            cursor_pos = (cursor_pos > 1) ? cursor_pos - 1 : cursor_pos;
        } else if (key == KEY_DOWN) {
            cursor_pos = (cursor_pos < num_templates + 1) ? cursor_pos + 1 : cursor_pos;
        } else if (key == '\n') {
            choice = cursor_pos;
            break;
        }
    }

    if(choice <= num_templates) {
        // Ottieni le dimensioni esatte del terminale

        // Se non hai bisogno di spazio extra per l'interfaccia, non sottrarre nulla
        // Se hai elementi aggiuntivi, sottrai lo spazio necessario
        // Esempio: per una barra di stato in fondo allo schermo

        height = LINES - 2;
        width = COLS/2;

        // Alloca le griglie utilizzando le dimensioni esatte
        grid = allocate_grid(height, width);
        next_grid = allocate_grid(height, width);

        // Carica il template centrato nella griglia
        load_template_centered(templates[choice - 1], grid, height, width);

    } else {
        // Solo per griglia personalizzata chiedi dimensioni
        clear();
        echo();
        char input[32];
        mvprintw(LINES/4, COLS/2 - 20, "Larghezza griglia (%d-%d): ", MIN_SIZE, COLS/2);
        refresh();
        getstr(input);
        width = atoi(input);
        
        mvprintw(LINES/4 + 2, COLS/2 - 20, "Altezza griglia (%d-%d): ", MIN_SIZE, LINES-2);
        refresh();
        getstr(input);
        height = atoi(input);
        noecho();
        
        if(width < MIN_SIZE || width > COLS/2 || height < MIN_SIZE || height > LINES-2) {
            endwin();
            printf("Dimensioni non valide!\n");
            exit(1);
        }
        
        grid = allocate_grid(height, width);
        next_grid = allocate_grid(height, width);
    }
    
    // Modalità editing
    edit_mode();
    
    // Avvia il gioco
    game_loop();
}


// Modifica main() per registrare il gestore
int main() {
    // Inizializzazione esistente
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
        
    // Resto del codice main() invariato
    show_menu();
    
    endwin();
    return 0;
}