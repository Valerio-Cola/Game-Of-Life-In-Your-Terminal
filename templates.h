#ifndef TEMPLATES_H
#define TEMPLATES_H

#define TEMPLATE_SIZE 10

// Struttura per rappresentare le coordinate di un punto nella griglia
typedef struct {
    int x, y;  
} Point;

// Struttura per rappresentare un template, ovvero un insieme di punti
typedef struct {
    const char* name;
    const Point* points; 
    int num_points;
} Template;

// Glider (3x3)
const Point glider_points[] = {
    {0,0},{0,1},{0,2},{-1,2},{-2,1}
};

const Template glider = {
    "Glider",
    glider_points,
    5 
};

// Toad (2x4)
const Point toad_points[] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -2}, {0, -1}, {0, 0}
};

const Template toad = {
    "Toad",
    toad_points,
    6  // numero di punti
};

// LWSS (4x5)
const Point lwss_points[] = {
    {-2, -1}, {-2, 2},
    {-1, 3},
    {0, -1}, {0, 3},
    {1, 0}, {1, 1}, {1, 2}, {1, 3}
};
const Template lwss = {
    "Lightweight Spaceship",
    lwss_points,
    9
};

// MWSS (4x6)
const Point mwss_points[] = {
    {-2, -2}, {-2, 2},
    {-1, 3},
    {0, -2}, {0, 3},
    {1, -1}, {1, 0}, {1, 1}, {1, 2}, {1, 3}
};
const Template mwss = {
    "Middleweight Spaceship",
    mwss_points,
    10
};

// HWSS (4x7)
const Point hwss_points[] = {
    {-2, -2}, {-2, 3},
    {-1, 4},
    {0, -2}, {0, 4},
    {1, -1}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}
};
const Template hwss = {
    "Heavyweight Spaceship",
    hwss_points,
    12
};

// Pulsar (13x13)
const Point pulsar_points[] = {
    // Parte superiore
    {-6, -2}, {-6, -1}, {-6, 0}, {-6, 2}, {-6, 3}, {-6, 4},
    {-4, -4}, {-4, 1}, {-4, 3}, {-4, 6},
    {-3, -4}, {-3, -3}, {-3, -2}, {-3, 2}, {-3, 3}, {-3, 4},
    // Parte centrale
    {-1, -4}, {-1, -3}, {-1, -2}, {-1, 2}, {-1, 3}, {-1, 4},
    {1, -4}, {1, -3}, {1, -2}, {1, 2}, {1, 3}, {1, 4},
    // Parte inferiore
    {3, -4}, {3, -3}, {3, -2}, {3, 2}, {3, 3}, {3, 4},
    {4, -4}, {4, 1}, {4, 3}, {4, 6},
    {6, -2}, {6, -1}, {6, 0}, {6, 2}, {6, 3}, {6, 4}
};
const Template pulsar = {
    "Pulsar",
    pulsar_points,
    48
};

// Penta-decathlon 
const Point pentadecathlon_points[] = {
    {0,1},{0,2},{0,0},{0,3},{0,-2},{0,-3},{0,5},{0,6},{1,4},{-1,4},{-1,-1},{1,-1}
};

const Template pentadecathlon = {
    "Penta-decathlon",
    pentadecathlon_points,
    12
};

// Glider Gun (9x36)
const Point h_glider_gun_points[] = {
    {0,0},{-1,0},{-1,1},{-1,-1},{-2,2},{-2,-2},{-3,0},{-4,3},{-4,-3},{-5,3},{-5,-3},{-6,2},{-6,-2},{-7,1},{-7,0},{-7,-1}, 
    {-16,0},{-17,0},{-16,-1},{-17,-1},
    {17,-2},{17,-3},{18,-2},{18,-3},
    {3,-1},{3,-2},{3,-3},{4,-1},{4,-2},{4,-3},{5,0},{5,-4},{7,0},{7,1},{7,-4},{7,-5}
};

const Template h_glider_gun = {
    "Glider Gun",
    h_glider_gun_points,
    36 
};


const Point spaceship_25P3H1V0_points[] ={
    {0,0},{1,-1},{2,-1},{3,0},{2,-2},{3,-2},{4,-2},{4,-3},{6,-2},{7,-2},{8,-2},{9,-1},{8,0},{-3,1},{-2,1},{0,3},{-3,1},{-5,1},{-4,1},{-4,0},{-6,2},{-5,3},{-2,2},{-3,3},{-1,4}
};

const Template spaceship_25P3H1V0 = {
    "Spaceship 25P3H1V0",
    spaceship_25P3H1V0_points,
    26
};

const Point blockedp4_points[] = {
    {0,0},{-3,0},{-2,0},{-1,0},{1,0},{2,0},{3,0},{-2,-3},{0,-3},{2,-3},
    {0,3},{-1,4},{1,4},{0,5},{5,0},{5,-1},{5,-2},{5,3},{5,4},{6,4},{6,-3},
    {7,-2},{7,-1},{7,0},{7,1},{7,2},{7,3},{9,0},{10,0},{9,1},{10,1},
    {-5,-2},{-5,-1},{-5,0},{-5,3},{-5,4},{-6,4},{-6,-3},{-7,-2},{-7,-1},{-7,0},
    {-7,1},{-7,2},{-7,3},{-10,0},{-9,0},{-10,1},{-9,1}
};

const Template blockedp4 = {
    "blockedp4-3",
    blockedp4_points,
    48
};

// Cell pattern (3x3)
const Point cell_points[] = {
    {0, -1},        // Punto superiore
    {-1, 0}, {0, 0},  // Punti centrali
    {0, 1}, {1, 1}    // Punti inferiori
};

const Template cell = {
    "Cell",
    cell_points,
    5  
};

// Funzione per caricare un template in una griglia di qualsiasi dimensione
void load_template_centered(const Template* tmpl, int** grid, int height, int width) {
    // Calcola il centro della griglia
    int center_y = height / 2;
    int center_x = width / 2;
    
    // Imposta tutte le celle a non attive (0)
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            grid[i][j] = 0;
        }
    }
    
    // Posiziona i punti del template, poichè i template hanno come punto centrale la coordinata (0,0) per evitare che venga disegnato fuori dalla griglia
    // in alto a sinistra, si somma il centro della griglia + la coordinata del punto del template
    for(int i = 0; i < tmpl->num_points; i++) {
        int y = center_y + tmpl->points[i].y;
        int x = center_x + tmpl->points[i].x;
        
        // Verifica che il punto sia dentro i limiti
        if(y >= 0 && y < height && x >= 0 && x < width) {
            grid[y][x] = 1;
        }
    }
}

// Array di tutti i template disponibili
const Template* templates[] = {
    &glider, 
    &toad, 
    &lwss, 
    &mwss, 
    &hwss, 
    &pulsar, 
    &pentadecathlon,
    &spaceship_25P3H1V0,
    &h_glider_gun,
    &cell,
    &blockedp4
};

// Numero di template disponibili
const int num_templates = sizeof(templates) / sizeof(templates[0]);

#endif