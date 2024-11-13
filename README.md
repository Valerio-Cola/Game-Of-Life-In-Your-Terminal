- [Italiano](#Italiano-)
- [English](#English-)

# English 🇬🇧

# 🎮 Game Of Life in Your Terminal

A version of Conway's Game of Life in your terminal written entirely in C, with graphics made possible by the ncurses library.

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/Valerio-Cola/Game-Of-Life-In-Your-Terminal)
You can run the program online with GitPod. You need to log in with GitHub, select **VSCODE Browser** as the editor, set the class to **Large**, and click continue. Once vscode web is open, expand the terminal and execute ```gcc -Ofast -march=native -fomit-frame-pointer -funroll-loops main.c -o game_of_life -lncurses```

## 📹 Demonstration Video

<p align="center">
    <img src="vid.gif">
</p>

## 📑 Table of Contents

- [How it Works](#how-it-works-) 🔧
- [How to Play](#how-to-play-) 🎮
- [Conway's Game Rules](#conways-game-rules-) 📋
- [Bugs](#bugs-) 🐛 
- [License](#license-) 📜

## How it Works 🔧

For a more detailed description of the code, see [`main.c`](main.c).

The algorithm operates as follows:

- The **`main`** function is responsible for calling the main functions:
        - Initialization of the ncurses library.
        - Disabling key echo to prevent input from being displayed on the screen.
        - Cursor initialization.
        - Starting the game menu.
        - Closing ncurses and terminating the program.

- **`show_menu`** initializes custom colors for the game screen, prints the title, commands, and various templates to choose from. It is also possible to build a grid with custom dimensions that cannot exceed the current terminal size.

- After selecting the choice, two grids are allocated in memory: the current one to be displayed and the next one. This allows the current state of the grid to be displayed while calculating the next state.

- If the custom grid is chosen, the user will be prompted to enter the dimensions.

- In both cases, **✏️ `edit_mode`** is entered, and then the simulation starts with **`game_loop`**.

- In **✏️ `edit_mode`**, through an infinite `while` loop and a `switch` for commands, it is possible to:
        - Move the cursor in the grid.
        - Activate and deactivate cells.
        - Return to the menu.
        - Change the game speed.
        - Start the `game_loop`.

- In the **🔄 `game_loop`**, similar to `edit_mode`, it is possible to:
        - Change the game speed.
        - Return to the menu.
        - Terminate the program.
        - Pause the game.
        - Restart the simulation.

A support variable is used to save the grid configuration before starting the simulation. If the simulation is paused, it is possible to move to the next frame.

- If the simulation is not paused, it will calculate the next grid considering all the rules of the original Game and then draw the grid.

## How to Play 🎮

### Compile the Program 🛠️

```bash
gcc main.c -o game_of_life -lncurses
```

### Run the Program with the Following Command

```bash
./game_of_life
```

**Note**: The program might temporarily change the terminal colors once finished. Just open a new terminal.

**Note**: The positions of the text in the menu have been optimized to be as centered as possible without overlapping. If the menu screen is not similar to this, close the program, enlarge the terminal, or execute the command `ctrl + -` and restart it:

![show image img.png](img.png)

**Note**: If the terminal is resized during the program execution, graphical bugs may occur, and it will be necessary to terminate and restart the process.

### Game Commands

**Menu:**

- `UP` `DOWN`: Move cursor$SELECTION_PLACEHOLDER$- `ENTER`: Select scenario

**Edit Mode:**

- `UP` `DOWN` `←` `→`: Move cursor
- `SPACE`: Activate/Deactivate cell
- `+`/`-`: Increase/Decrease speed
- `ENTER`: Start simulation
- `m`: Return to menu

**During Simulation:**

- `p`: Pause/Play
- `n`: Next frame (when paused)
- `r`: Restart
- `m`: Main menu
- `q`: Quit
- `+`/`-`: Increase/Decrease speed

In edit mode, the cursor `"X"` will be visible, but not during the simulation.

In the top left corner, various information will be displayed:

- Generation number
- Population
- Game speed (expressed in ms between generations)
- Two types of coordinates: the cursor position with the center `(0,0)` at the top left and exactly centered in the grid.

## Conway's Game Rules 📋

Quoting Wikipedia: [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

> It is actually a game without players, meaning its evolution is determined by its initial state, without the need for any input from human players. It takes place on a grid of square cells that extends infinitely in all directions; this grid is called the world. Each cell has 8 neighbors, which are the cells adjacent to it, including those diagonally. Each cell can be in two states: alive or dead (or on and off). The state of the grid evolves in discrete time steps, meaning it progresses in clear intervals. The states of all cells at a given time are used to calculate the state of the cells at the next time step. All cells in the world are updated simultaneously in the transition from one time step to the next: thus, a generation passes.
>
>The transitions depend solely on the state of neighboring cells in that generation:
>
>1. **Underpopulation**: Any live cell with fewer than two live neighbors dies, as if by isolation.
>2. **Survival**: Any live cell with two or three live neighbors survives to the next generation.
>3. **Overpopulation**: Any live cell with more than three live neighbors dies, as if by overpopulation.
>4. **Reproduction**: Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

## Bugs 🐛

- The cursor `"X"` is not visible after a restart.
- If a template is loaded, the menu is returned to, and a custom grid is selected, the program terminates.

## License 📜

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.



# Italiano 🇮🇹

# 🎮 Game Of Life nel Tuo Terminale

Una versione di Conway's Game of Life nel proprio terminale scritta interamente in C, grafica realizzata grazie alla libreria ncurses.

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/Valerio-Cola/Game-Of-Life-In-Your-Terminal)
È possibile eseguire il programma online con GitPod. È necessario effettuare il login con github, selezionare come editor **VSCODE Browser** impostare la classe su **Large** e cliccare su continue. Una volta aperto vscode web espandi il terminale ed esegui  ```gcc -Ofast -march=native -fomit-frame-pointer -funroll-loops main.c -o game_of_life -lncurses```

## 📹 Video dimostrativo

<p align="center">
  <img src="vid.gif">
</p>

## 📑 Indice

- [Funzionamento](#funzionamento-) 🔧
- [Come giocare](#come-giocare-) 🎮
- [Regole del Gioco di Conway](#regole-del-gioco-di-conway-) 📋
- [Bug](#bug-) 🐛
- [Licenza](#licenza-) 📜

## Funzionamento 🔧

Per una descrizione più dettagliata del codice vedere [`main.c`](main.c).

L'algoritmo opera nel seguente modo:

-  La funzione **`main`** si occupa di chiamare le funzioni principali:
    -  Inizializzazione della libreria ncurses.
    -  Disabilitazione dell'echo dei tasti per evitare che venga mostrato a schermo l'input.
    -  Inizializzazione del cursore.
    -  Avvio del menu di gioco.
    -  Chiusura di ncurses con terminazione del programma.

-  **`show_menu`** inizializza i colori personalizzati per la schermata di gioco, stampa a schermo il titolo, i comandi e i vari template tra cui scegliere. È possibile scegliere anche di costruire una griglia con dimensioni personalizzate che non potranno sforare le dimensioni attuali del terminale.

-  Dopo aver selezionato la propria scelta vengono allocate due griglie in memoria: quella attuale che verrà mostrata e la successiva. Ciò permette di far visualizzare a schermo lo stato attuale della griglia e nel mentre calcolare lo stato successivo.

-  Se è stata scelta la griglia personalizzata verrà richiesto all'utente di inserire le dimensioni.

- In entrambi i casi si entrerà in **✏️ `edit_mode`** e poi si avvierà la simulazione con il **`game_loop`**.

- In **✏️ `edit_mode`**, mediante un ciclo `while` che non termina e uno `switch` per i comandi, è possibile:
    -  Spostare il cursore nella griglia.
    -  Attivare e disattivare le celle.
    -  Tornare al menu.
    -  Modificare la velocità del gioco.
    -  Avviare il `game_loop`.

- Nel **🔄 `game_loop`**, in modo simile all'`edit_mode`, è possibile:
    -  Modificare la velocità di gioco.
    -  Tornare al menu.
    -  Terminare il programma.
    -  Mettere in pausa.
    -  Fare il restart della simulazione.

    Per fare ciò è utilizzata una variabile di supporto per salvare la configurazione della griglia prima di avviare la simulazione. Se la simulazione è in pausa è possibile passare al frame successivo.

- Se la simulazione non è in pausa, calcolerà la griglia successiva tenendo conto di tutte le regole del Gioco originale e successivamente disegna la griglia.

## Come giocare 🎮

### Compila il programma 🛠️

```bash
gcc main.c -o game_of_life -lncurses
```

### Esegui il programma con il seguente comando

```bash
./game_of_life
```

**Nota**: Il programma potrebbe modificare momentaneamente anche i colori del terminale una volta terminato. Basterà aprire un nuovo terminale.

**Nota**: Le posizioni delle scritte nel menu sono state ottimizzate per poter essere più centrate possibile senza accavallarsi. Se la schermata del menu non è simile a questa allora chiudere il programma, ingrandire il terminale oppure eseguire il comando `ctrl + -` e riavviarlo:

![mostra l'immagine img.png](img.png)

**Nota**: Se verrà ridimensionato il terminale durante l'esecuzione del programma potrebbero presentarsi bug grafici, sarà necessario terminare e rieseguire il processo.

### Comandi di gioco

**Menu:**

- `INVIO`: Seleziona lo scenario

**Modalità edit:**

- `UP` `DOWN` `←` `→`: Muovi cursore
- `SPAZIO`: Attiva/Disattiva cella
- `+`/`-`: Aumenta/Diminuisce velocità
- `INVIO`: Avvia la simulazione
- `m`: Torna al menu

**Durante la simulazione:**

- `p`: Pausa/Play
- `n`: Prossimo frame (in pausa)
- `r`: Restart
- `m`: Menu principale
- `q`: Esci
- `+`/`-`: Aumenta/Diminuisce velocità

Nella modalità edit sarà visibile il cursore `"X"`, durante la simulazione no.

In alto a sinistra saranno mostrate varie informazioni relative a:

- Numero di generazione
- Popolazione
- Velocità di gioco (espressa in ms tra una generazione e la successiva)
- Due tipologie di coordinate: la posizione del cursore con centro `(0,0)` in alto a sinistra e esattamente centrata nella griglia.

## Regole del Gioco di Conway 📋

Citando Wikipedia: [Gioco della vita](https://it.wikipedia.org/wiki/Gioco_della_vita)

> Si tratta in realtà di un gioco senza giocatori, intendendo che la sua evoluzione è determinata dal suo stato iniziale, senza necessità di alcun input da parte di giocatori umani. Si svolge su una griglia di caselle quadrate (celle) che si estende all'infinito in tutte le direzioni; questa griglia è detta mondo. Ogni cella ha 8 vicini, che sono le celle ad essa adiacenti, includendo quelle in senso diagonale. Ogni cella può trovarsi in due stati: viva o morta (o accesa e spenta, on e off). Lo stato della griglia evolve in intervalli di tempo discreti, cioè scanditi in maniera netta. Gli stati di tutte le celle in un dato istante sono usati per calcolare lo stato delle celle all'istante successivo. Tutte le celle del mondo vengono quindi aggiornate simultaneamente nel passaggio da un istante a quello successivo: passa così una generazione.
>
>Le transizioni dipendono unicamente dallo stato delle celle vicine in quella generazione:
>
>1. **Solitari**: Qualsiasi cella viva con meno di due celle vive adiacenti muore, come per effetto d'isolamento.
>2. **Sopravvivenza**: Qualsiasi cella viva con due o tre celle vive adiacenti sopravvive alla generazione successiva.
>3. **Sovrappopolazione**: Qualsiasi cella viva con più di tre celle vive adiacenti muore, come per effetto di sovrappopolazione.
>4. **Riproduzione**: Qualsiasi cella morta con esattamente tre celle vive adiacenti diventa una cella viva, come per effetto di riproduzione.

## Bug 🐛

- Il cursore `"X"` dopo il restart non è visibile.
- Se si carica un template, si torna al menu e si seleziona griglia personalizzata il programma termina.

## Licenza 📜

Questo progetto è concesso in licenza sotto la Licenza MIT. Vedi il file [LICENSE](LICENSE) per i dettagli.
