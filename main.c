//including necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

//defines ANSI escape codes for color formatting.
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE     "\x1b[34m"
#define MAGENTA    "\x1B[35m"
#define TABLE_SIZE 8
#define NUM_SHIPS 5


// Global constants and variables
const int NumShip = 5; // Number of ships
const int TableSize = 8; // Size of the game table

int wins;
int losses;
int totalGames;

bool shipsSet = false;  // Flag to indicate whether ships have been set


//Represents a cell on the game table 
struct Cell {
    char column;
    int integerColumn;
    int row;
};


// All function
int translateColumn(char columnLetter);

int randomMove(int min, int max);

bool isDuplicate(struct Cell ships[], int currentShipIndex);

bool isTooClose(struct Cell ships[], int currentShipIndex);

bool isAppropriate(struct Cell ships[], int currentShipIndex);

void displayGameTable(char** table, struct Cell ships[], int numShips, bool isComputer, bool** alreadyShot);

void setShips(char** table, struct Cell ships[], bool isComputer, FILE* file);

bool isHit(struct Cell ships[], int numShips, int row, int column);

void makeShoot(struct Cell ships[], int numShips, bool** alreadyShot, bool isComputer, FILE* file);

// Prototypes for functions related to loading saved games and console display.

void listSavedGames();

void showBoardSavedGame();

//Function to erase the lines in console
void eraseLines(int numLines);


// Function to start a new game
void newGame();


//Main function that displays a menu and handles user input for starting a new game, loading a game, or exiting the program.
int main() {
    // Code for reading and updating game data from a file

    // Read the current values from the file 
    FILE* file = fopen("allGames.txt", "r+");
    if (file == NULL) {
        //if the file does not exist it will be created, and program will ask for another Run
        FILE* file = fopen("allGames.txt", "w");


        fprintf(file, "Total Games: %d\nWins: %d\nLosses: %d\n", totalGames, wins, losses);

        fclose(file);


        printf("Create a file for game data.\n");
        printf("Please Run the code once again for start.\n");
    }

    // Read the first three lines and update the variables
    fscanf(file, "Total Games: %d\nWins: %d\nLosses: %d\n", &totalGames, &wins, &losses);


    // Close the file
    fclose(file);


    printf("Welcome to the battleship game!\n");

    while (1) {
        // Displaying menu options...
        printf("Choose the option:\n");
        printf(GREEN"1. Play a new game\n"RESET);
        printf("2. Load game\n");
        printf(RED"3. Exit the program\n"RESET);

        // for main menu
        int Option;
        scanf("%d", &Option);

        // Clear previous lines in the console for a cleaner display.
        eraseLines(6);

        // for submenu
        int loadGamesOption;
        switch (Option) {
        case 1:
            printf(GREEN"Let's get game started!\n"RESET);
            //Save point
            printf(RED"Enter S0 if you want to save and exit the game.\n"RESET);
            newGame();
            break;
        case 2:
            //Enters the submenu for loading games
            // allowing to see the list of all saved games or show the board of a specific saved game.
            do {
                printf(MAGENTA"Load Game Menu:\n"RESET);
                printf("1. List all saved games\n");
                printf("2. Show the board of one of the saved games\n");
                printf("4. Return to the main menu\n");
                printf("Choose an option: \n");

                scanf("%d", &loadGamesOption);
                switch (loadGamesOption) {
                case 1:
                    listSavedGames();
                    break;
                case 2:
                    showBoardSavedGame();
                    break;

                case 4:
                    printf("Returning to the main menu.\n");
                    break;
                default:
                    printf("Invalid option. Try another one.\n");
                    break;
                }
            } while (loadGamesOption != 4);
            break;

        case 3:
            printf(BLUE"Exiting the program. Goodbye!\n"RESET);
            exit(0);
            break;

        default:
            printf("Not possible option, try another one :D\n");

        }
    }

    return 0;
}



// Function to erase lines in the console.
void eraseLines(int numLines) {
    for (int i = 0; i < numLines; i++) {
        printf("\033[K");  // ANSI escape code to clear the current line

        // If it's not the last line to be cleared, move the cursor up one line.
        if (i < numLines - 1) {
            printf("\033[F");  // ANSI escape code to move the cursor up one line
        }
    }
}

//Function used for loadGame();

// Function to list all saved games.
void listSavedGames() {
    //Opens the file "allGames.txt" for reading.
    FILE* file = fopen("allGames.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    //Declares a character array to store each line read from the file.
    char line[100]; // Assuming that a line in the file won't exceed 100 characters
    int gameNumber = 0;

    //Header
    printf(YELLOW"List of saved games:\n"RESET);

    // Read the file line by line and print game numbers
    while (fgets(line, sizeof(line), file) != NULL) {
        // Checks if each line starts with the characters 'Game'.
        if (line[0] == 'G' && line[1] == 'a' && line[2] == 'm' && line[3] == 'e') {
            // If a saved game entry is found, it increments the gameNumber and prints the game number along with the line.
            printf("%d. %s", ++gameNumber, line);
        }
    }
    // Close the file
    fclose(file);

    if (gameNumber == 0) {
        printf("No saved games found.\n");
    }
}

// Function to show the board of one of the saved games.
void showBoardSavedGame() {
    listSavedGames(); // Display the list of saved games first

    int gameChoice;
    printf("Enter the game number to show its board: ");
    scanf("%d", &gameChoice);

    // opens the file "allGames.txt" for reading.
    FILE* file = fopen("allGames.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[100];
    int currentGame = 0;

    // Read the file line by line and find the chosen game
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'G' && line[1] == 'a' && line[2] == 'm' && line[3] == 'e') {
            //counting the number of game entries('Game') to determine the current game.
            currentGame++;
        }

        if (currentGame == gameChoice) {
            //Consider the game number with the user's choice and prints the board (if exist).
            printf("Board for Game %d:\n", gameChoice);
            while (fgets(line, sizeof(line), file) != NULL && line[0] != '\n') {
                printf("%s", line);
            }
            fclose(file);
            return;
        }
    }
    //close the file
    fclose(file);
    printf("Invalid game number.\n");
}


// Function used for newGame();


// Returns the integer value of the column.
int translateColumn(char columnLetter) {
    if ((columnLetter >= 'A' && columnLetter <= 'H') || (columnLetter >= 'a' && columnLetter <= 'h')) {
        return (columnLetter >= 'A' && columnLetter <= 'H') ? columnLetter - 'A' + 1 : columnLetter - 'a' + 1; // Returns the integer value of the column.
    }
    else {
        printf(RED"Invalid input. Please enter a letter between A and H."RESET"\n");
        return -1; // Return a sentinel value to indicate an error
    }
}

// Function to generate a random move for the computer.
int randomMove(int min, int max) {
    //Generates a random move within the specified range.
    return rand() % (max - min + 1) + min; //Returns a random integer.
}

// Function to check for duplicate ships in the array.
bool isDuplicate(struct Cell ships[], int currentShipIndex) {
    for (int i = 0; i < currentShipIndex; i++) {
        // Checks for duplicate ship placements in the array.
        if (ships[i].row == ships[currentShipIndex].row && ships[i].integerColumn == ships[currentShipIndex].integerColumn) {
            return true; // Duplicate found
        }
    }
    return false; // No duplicate
}

// Function to check if a ship is too close to another ship.
bool isTooClose(struct Cell ships[], int currentShipIndex) {
    for (int i = 0; i < currentShipIndex; i++) { // calculate the destance between ships
        int rowDifference = abs(ships[i].row - ships[currentShipIndex].row);
        int columnDifference = abs(ships[i].integerColumn - ships[currentShipIndex].integerColumn);
        if (rowDifference <= 1 && columnDifference <= 1) {
            return true; // Too close to another ship
        }
    }
    return false; // Appropriate distance to another ship
}

// Function to check if a ship placement is appropriate.
bool isAppropriate(struct Cell ships[], int currentShipIndex) { // verify if the placement of ships is settled correct
    return !isDuplicate(ships, currentShipIndex) && !isTooClose(ships, currentShipIndex);
}

// Function to display the game table.
void displayGameTable(char** table, struct Cell ships[], int numShips, bool isComputer, bool** alreadyShot) {
    // Print the column headers in green.
    printf(GREEN"  A B C D E F G H \n"RESET);

    // Loop through each row of the game table.
    for (int i = 0; i < TableSize; i++) {
        // Print the row number.
        printf(GREEN"%d "RESET, i + 1);

        // Loop through each column of the game table.
        for (int j = 0; j < TableSize; j++) {
            char cellContent = table[i][j];

            // Check if there's a ship at the current cell.
            for (int k = 0; k < numShips; k++) {
                if (ships[k].row == i + 1 && ships[k].integerColumn == j + 1) {
                    if (!alreadyShot[i][j]) { // // Check if the cell has not been shot.

                        // If it's the computer's table, hide the ship; otherwise, show 'S'.
                        cellContent = isComputer ? '-' : 'S';
                    }
                }
            }

            // Check if the cell has been shot and if it contains a ship.
            if (alreadyShot[i][j] && cellContent != 'S') {
                cellContent = 'O'; // Display 'O' for a missed shot
            }
            else if (alreadyShot[i][j] && cellContent == 'S') {
                cellContent = 'X'; // Display 'X' for a hit shot
            }
            // Print the content of the current cell.
            printf("%c ", cellContent);
        }
        // Move to the next line for the next row.
        printf("\n");
    }
}

// Function to set the ships on the game table.
void setShips(char** table, struct Cell ships[], bool isComputer, FILE* file) {
    srand(time(NULL));

    // Loop through each ship to set its position on the table.
    for (int i = 0; i < NumShip; i++) {

        /*
        Random Ship Placement (Computer's Turn):
        If it's the computer's turn (isComputer is true),
        generates random column and row values for the ship until a valid position is found.
        It ensures the generated position is appropriate using the isAppropriate function.
        */
        if (isComputer) {
            do {
                ships[i].integerColumn = randomMove(1, TableSize);
                ships[i].row = randomMove(1, TableSize);
            } while (!isAppropriate(ships, i)); // Repeat if the generated position is not appropriate.

        }
        // The Player's turn to set ships
        /*
        Asking the user to enter the column and row for each ship.
        It continues prompting until a valid position is entered,
        checking both the input format and the appropriateness of the position.
        */
        else {
            // Keep prompting the user until a valid position is entered.
            while (1) {
                printf("Enter the column and row of ship %d: ", i + 1);
                scanf(" %c %d", &ships[i].column, &ships[i].row);
                ships[i].integerColumn = translateColumn(ships[i].column);

                // Check if the entered position is valid.
                if ((ships[i].row > 0) && (ships[i].integerColumn > 0)
                    && (ships[i].row <= TableSize) && isAppropriate(ships, i)) {
                    break; // Break out of the loop if the position is valid.
                }

            }

        }
        // Set the ship symbol on the game table.
        table[ships[i].row - 1][ships[i].integerColumn - 1] = 'S'; //convert it from 1-based indexing to 0-based indexing.

    }
    // After setting all ships, record the ship coordinates in the file.
    for (int i = 0; i < NumShip; i++) {
        fprintf(file, "Coordinates of ship %d: %d %d\n", i + 1, ships[i].integerColumn, ships[i].row);
    }



}

// Function to check if a shot hits a ship.
bool isHit(struct Cell ships[], int numShips, int row, int column) {
    for (int i = 0; i < numShips; i++) {
        if (ships[i].row == row && ships[i].integerColumn == column) {
            //Hit!
            // If a ship is found at the specified location, mark it as hit by setting its coordinates to -1.
            ships[i].row = -1;
            ships[i].integerColumn = -1; // Mark as hit
            return true; // Indicate a successful hit.
        }
    }
    //Miss!
    // If no ship is found at the specified location after checking all ships, it's a miss.
    return false; // Missed the ship
}

// Function for a player to make a shot.
void makeShoot(struct Cell ships[], int numShips, bool** alreadyShot, bool isComputer, FILE* file) {
    int row;
    char column;

    // Computer's turn to shoot
    if (isComputer) {
        do {
            // Generate random coordinates for the computer's shot within the table bounds.
            row = randomMove(1, TableSize);
            column = randomMove(1, TableSize);

        } while (alreadyShot[row - 1][column - 1]);

        // Display computer's move and update the file with the move.
        printf(BLUE"Computer's turn!\n"RESET);
        printf("Computer shoots at %c%d\n", 'A' + column - 1, row);  // Display the computer's move
        fprintf(file, "Computer shoots at %c%d\n", 'A' + column - 1, row);  // Update the file with the computer's move

        // Check if the computer hit a ship and display the result.
        if (isHit(ships, numShips, row, column)) {
            printf(RED"Computer hit your ship!"RESET"\n");
        }
        else {
            printf("Computer missed.\n");
        }

        alreadyShot[row - 1][column - 1] = true;  // Mark the cell as already shot

    }

    // The player's turn to shoot
    else {
        bool isMakeAShot = false;
        while (!isMakeAShot) {
            for (int i = 0; i < 1; i++) {
                printf(GREEN"Your turn!\n"RESET);
                printf("Enter the column and row to shoot: ");
                scanf(" %c %d", &column, &row);


                // Check if the user wants to exit
                if ((column == 'S' || column == 's') && row == 0) {
                    printf("Save your game\n");
                    fprintf(file, "You left the game\n");
                   
                    fclose(file);
                    printf(BLUE"Exiting the game. Goodbye!\n"RESET);
                    exit(1);
                }


                int integerColumn = translateColumn(column);

                // Check for valid input(within table bounds and not already shot).
                if (row < 1 || row > TableSize || integerColumn == -1) {
                    printf(RED"Invalid input. Please enter valid values for column and row."RESET"\n");
                    continue;
                }

                if (alreadyShot[row - 1][integerColumn - 1]) {
                    printf(RED"Caution: You've already shot in this cell. Choose another one!"RESET"\n");
                    continue;
                }

                // Update the file with the user's move.
                fprintf(file, "User shoots at %c%d\n", column, row);

                // Mark the cell as already shot.
                alreadyShot[row - 1][integerColumn - 1] = true;

                // Check if the player hit a computer's ship and display the result.
                if (isHit(ships, numShips, row, integerColumn)) {
                    printf(RED"You hit computer's ship!"RESET"\n");
                }
                else {
                    printf("You missed.\n");
                }

                isMakeAShot = true; // Exit the loop as the player has made a valid shot.

            }
        }


    }



}

// Function to update the game statistics in the file.
void statusReadandChange() {

    // Open the file "allGames.txt" for reading and writing.
    FILE* file = fopen("allGames.txt", "r+");

    // Check if the file is successfully opened.
    if (file == NULL) {
        printf("No such file was found");
    }

    // Skip the first three lines
    for (int i = 0; i < 3; ++i) {
        while (fgetc(file) != '\n') {}
    }

    // Move back to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Calculate the total number of games (wins and losses).
    int totalGames = wins + losses;

    // Write the new values (total games, wins, losses) to the file.
    fprintf(file, "Total Games: %d\nWins: %d\nLosses: %d\n", totalGames, wins, losses);

    // Move to the end of the file.
    fseek(file, 0, SEEK_END);

    // Close the file
    fclose(file);
};

// Function to start a new game.
void newGame() {


    // Update the game statistics from the file.
    statusReadandChange();

    // Open the file "allGames.txt" for appending.
    FILE* file = fopen("allGames.txt", "a");
    if (file == NULL) {
        printf("Error in opening file");
    }

    // Print the current game number to the file.
    fprintf(file, "Game %d\n", totalGames + 1);

    // Initialize variables for computer and human tables, ships, and shot tracking.
    char** computerTable;
    char** humanTable;
    struct Cell* computerShips;
    struct Cell* humanShips;
    bool** alreadyShotHuman;
    bool** alreadyShotComputer;


    // Dynamically allocate memory
    // for computerTable[][]
    computerTable = (char**)malloc(TableSize * sizeof(char*));
    for (int i = 0; i < TableSize; i++) {
        computerTable[i] = (char*)malloc(TableSize * sizeof(char));
    }

    // for humanTable[][]
    humanTable = (char**)malloc(TableSize * sizeof(char*));
    for (int i = 0; i < TableSize; i++) {
        humanTable[i] = (char*)malloc(TableSize * sizeof(char));
    }

    // for computerShips[] and humanShips[]
    computerShips = (struct Cell*)malloc(NumShip * sizeof(struct Cell));
    humanShips = (struct Cell*)malloc(NumShip * sizeof(struct Cell));

    // Dynamically allocate memory for alreadyShotHuman[] and alreadyShotComputer[]
    alreadyShotHuman = (bool**)calloc(TableSize, sizeof(bool*)); // using calloc for initializes the allocated memory to zero
    for (int i = 0; i < TableSize; i++) {
        alreadyShotHuman[i] = (bool*)calloc(TableSize, sizeof(bool));
    }

    alreadyShotComputer = (bool**)calloc(TableSize, sizeof(bool*));
    for (int i = 0; i < TableSize; i++) {
        alreadyShotComputer[i] = (bool*)calloc(TableSize, sizeof(bool));
    }



    // Initialize tables and shot tracking for both computer and human.
    // Set all cells to default values.
    for (int i = 0; i < TableSize; i++) {
        for (int j = 0; j < TableSize; j++) {
            computerTable[i][j] = '-';
            humanTable[i][j] = '-';
        }
    }

    for (int i = 0; i < TableSize; i++) {
        for (int j = 0; j < TableSize; j++) {
            alreadyShotHuman[i][j] = false;
            alreadyShotComputer[i][j] = false;
        }
    }



    // Display current game statistics.
    printf("Total Games: %d\n", totalGames);
    printf("Wins: %d\n", wins);
    printf("Losses: %d\n", losses);


    // Set computer's ships and display its table.
    fprintf(file, "Computer's ships coordinates:\n");
    setShips(computerTable, computerShips, true, file);
    printf("Computer's Table:\n");
    displayGameTable(computerTable, computerShips, NumShip, true, alreadyShotComputer);

    // Prompt the user to set their ships and display their table.
    printf("Enter column in letter and row in integer: e.g. A2 \n");
    printf("Notice: keep the distance between ships minimum in 1 cell \n");
    fprintf(file, "User's ships coordinates:\n");
    setShips(humanTable, humanShips, false, file);

    printf("User's Table:\n");
    displayGameTable(humanTable, humanShips, NumShip, false, alreadyShotHuman);

    int userTurn = 0; // flag of the game turn
    int round = 1; // quantity of rounds


    // Main game loop.
    while (1) {
        // Computer's turn.
        if (userTurn == 0) {

            makeShoot(computerShips, NumShip, alreadyShotComputer, false, file);

            // Display the computer's board.
            displayGameTable(computerTable, computerShips, NumShip, true, alreadyShotComputer);
            userTurn = 1;
        }
        else {
            // User's turn.
            round++;
            printf("Round: %d\n", round);
            makeShoot(humanShips, NumShip, alreadyShotHuman, true, file);

            // Display the user's board.
            displayGameTable(humanTable, humanShips, NumShip, false, alreadyShotHuman);
            userTurn = 0;

        }

        // Check remaining ships.
        int computerShipsLeft = 0;
        int humanShipsLeft = 0;

        for (int i = 0; i < NumShip; i++) {
            if (computerShips[i].row != -1 || computerShips[i].integerColumn != -1) {
                computerShipsLeft++;
            }

            if (humanShips[i].row != -1 || humanShips[i].integerColumn != -1) {
                humanShipsLeft++;
            }
        }

        // Check and display game outcome.
        if (computerShipsLeft == 0) {
            printf(GREEN"Congratulations! You have sunk all of the computer's ships. You win!"RESET"\n");
            wins++;
            break;  // Exit the loop as the game is over
        }
        else if (humanShipsLeft == 0) {
            printf(GREEN"Game over! The computer has sunk all of your ships. Better luck next time!"RESET"\n");
            losses++;
            break;  // Exit the loop as the game is over
        }




    }
    // Close the file.
    fclose(file);

    // Update and display the final game statistics.
    statusReadandChange();



    // Free dynamically allocated memory at the end of the game
    for (int i = 0; i < TableSize; i++) {
        free(computerTable[i]);
        free(humanTable[i]);
        free(alreadyShotHuman[i]);
        free(alreadyShotComputer[i]);
    }

    free(computerTable);
    free(humanTable);
    free(computerShips);
    free(humanShips);
    free(alreadyShotHuman);
    free(alreadyShotComputer);
}

