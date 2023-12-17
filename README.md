# Battleship_game
Battleship game in C (user & computer mode) 
 
The provided C program is an implementation of a battleship game with various features, including saving and loading games, dynamic memory allocation, and game statistics tracking. Here's a summary of the main aspects and functionalities: 

 

User Interface: 

The user interface includes colored text for better visibility. 

Users are prompted to input ship coordinates for better way of interaction with game, and choose menu options. 

 

Functions: 

Various functions are declared and defined for translating columns, generating random moves, checking ship placements, displaying the game table, setting ships, and handling shots. 

Functions are also provided for listing saved games, showing the board of a saved game, erasing lines in the console, updating game statistics, and starting a new game. 

  

Main Function: 

The main function initializes the game data file (allGames.txt) and reads the current values (total games, wins, losses). 

It displays a menu for playing a new game, loading a game, or exiting the program. 

Submenus are implemented for loading games, listing saved games, and showing the board of a saved game. 

  

File Handling: 

The program uses file operations to read and update game data in the `allGames.txt` file. 

Saved game information, including ship coordinates and user/computer shots, is recorded in the file. 

  

Game Flow: 

The game flow is controlled through loops, including the main menu loop and submenus for loading games. 

The main game loop alternates between the computer's and user's turns, updating game tables and checking for remaining ships. 

  

Dynamic Memory Allocation: 

Memory is dynamically allocated for game tables, ship arrays, and boolean arrays to track shot history and is freed at the end of the game to prevent memory leaks. 

  

Game Outcome: 

The program determines and displays the outcome of the game (win or loss) based on the remaining ships. 

  

Hope you enjoy the game :) 
