#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>

using namespace std;

const int table_size = 9;

// Function to print Sudoku grid
void printSudokuGrid(int grid[9][9], int cur_score) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j] << " ";
            if ((j + 1) % 3 == 0 && j < 8) {
                cout << "| ";
            }
        }
        cout << endl;
        if ((i + 1) % 3 == 0 && i < 8) {
            cout << "----------------\n";
        }
    }
    cout << "\n-------- current score = " << cur_score << " --------\n";
}

// Function to check if a number is valid in a row
bool isValidInRow(int table[table_size][table_size], int row, int num) {
    for (int col = 0; col < table_size; col++) {
        if (table[row][col] == num) {
            return false;
        }
    }
    return true;
}

// Function to check if a number is valid in a column
bool isValidInColumn(int table[table_size][table_size], int col, int num) {
    for (int row = 0; row < table_size; row++) {
        if (table[row][col] == num) {
            return false;
        }
    }
    return true;
}

const int grid_size = 3; // Assuming Sudoku grid size is 9x9

// Function to check if a number is valid in a 3x3 grid
bool isValidInGrid(int table[table_size][table_size], int startRow, int startCol, int num) {
    // Determine the boundaries of the current 3x3 grid
    int endRow = startRow + grid_size;
    int endCol = startCol + grid_size;

    // Iterate over the current 3x3 grid
    for (int i = startRow; i < endRow; i++) {
        for (int j = startCol; j < endCol; j++) {
            if (table[i][j] == num) {
                return false; // If the number already exists in the grid, return false
            }
        }
    }
    return true; // If the number does not exist in the grid, return true
}

// Function to generate a Sudoku grid
void generateSudoku(int table[table_size][table_size]) {
    srand(static_cast<unsigned int>(time(0)));
    // Fill the board with zeros
    for (int i = 0; i < table_size; i++) {
        for (int j = 0; j < table_size; j++) {
            table[i][j] = 0;
        }
    }
    // Generate random numbers to fill some of the cells of the board
    int count = 0;
    while (count < 40) { // Adjust the count according to the desired difficulty level
        int row = rand() % table_size;
        int col = rand() % table_size;
        int num = rand() % 9 + 1;
        if (table[row][col] == 0) {
            if (isValidInRow(table, row, num) && isValidInColumn(table, col, num)) {
                table[row][col] = num;
                count++;
            }
        }
    }
}

// Function to check if there are any zeros in the Sudoku grid
bool hasZeros(int grid[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Function to display credits
void credits() {
    cout << "The game is created by the following persons: " << endl;
    cout << "  =====     Usman Ashfaq     ===== " << endl;
    cout << "=====     Muhammad Sumair      =====" << endl;
}

// Function to display instructions
void instructions() {
    cout << "Instructions: " << endl;
    cout << " 1: Players input numbers from 1 to 9 into the grid to solve the puzzle." << endl;
    cout << " 2: Each row, column, and 3x3 subgrid must contain all the numbers from 1 to 9 without repetition." << endl;
    cout << " 3: The game will check your solution after each move and highlight any incorrect cells in red." << endl;
    cout << " 4: The game is won when all of the empty cells are filled with the correct digits." << endl;
    cout << " 5: You can access the credits, instructions, and save_score at any time by selecting options 2, 3, or 4." << endl;
}

// Function to save the score
void save_score(int count) {
    string playerName;
    cout << "saving your score. Enter Name: ";
    cin >> playerName;
    ifstream infile("scores.txt");
    int scores[3] = { 0 }; // Stores the top 3 scores
    string players[3]; // Stores the corresponding player names
    if (infile.is_open()) {
        for (int i = 0; i < 3 && infile >> scores[i] >> players[i]; i++);
        infile.close();
    }

    // Sorting to keep only top 3 scores in descending order
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (scores[j] > scores[i]) {
                int tempScore = scores[i];
                string tempPlayer = players[i];
                scores[i] = scores[j];
                players[i] = players[j];
                scores[j] = tempScore;
                players[j] = tempPlayer;
            }
        }
    }

    // Update scores array with the new score
    if (count > scores[2]) {
        scores[2] = count;
        players[2] = playerName;
        for (int i = 2; i > 0 && scores[i] > scores[i - 1]; i--) {
            int tempScore = scores[i];
            string tempPlayer = players[i];
            scores[i] = scores[i - 1];
            players[i] = players[i - 1];
            scores[i - 1] = tempScore;
            players[i - 1] = tempPlayer;
        }
    }

    ofstream outfile("scores.txt");
    for (int i = 0; i < 3; i++) {
        outfile << scores[i] << " " << players[i] << endl;
    }
    outfile.close();

    // Display current score
    cout << "Your score: " << count << endl;

    // Check if current score is highest
    if (count >= scores[0]) {
        cout << "Congratulations! You achieved the high score!" << endl;
    }
}

// Function to start the game
void start_game(int sudokuGrid[9][9], int& curr_score) {
    generateSudoku(sudokuGrid);
    printSudokuGrid(sudokuGrid, curr_score);
    cout << endl;
    // Loop until all zeros are filled
    int wrong_attempts = 0;
    while (hasZeros(sudokuGrid) && wrong_attempts < 3) {
        int row, col, num;
        cout << "Enter the row, column, and number: ";
        cin >> row >> col >> num; // input as 1-based index
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            cout << "Invalid input! > (1-9)" << endl;
            continue;
        }
        if (sudokuGrid[row - 1][col - 1] != 0) {
            wrong_attempts++;
            cout << "Cell is already filled!" << endl;
        }
        else if (!isValidInRow(sudokuGrid, row - 1, num) || !isValidInColumn(sudokuGrid, col - 1, num) ||
            !isValidInGrid(sudokuGrid, (row - 1) / 3 * 3, (col - 1) / 3 * 3, num)) {
            wrong_attempts++;
            cout << "Invalid move!" << endl;
        }
        else {
            // Add 50 to the score for each correct guess
            curr_score += 50;
            sudokuGrid[row - 1][col - 1] = num;
            printSudokuGrid(sudokuGrid, curr_score);
        }
        if (wrong_attempts == 3) {
            save_score(curr_score);
            cout << "Exiting the game. Goodbye!" << endl;
        }
    }
}

int main() {
    system("color A5");

    ifstream scorefile;
    int choice;
    int curr_score = 0; // Initialize score here

    int sudokuGrid[9][9];

    do {
        cout << setw(20) << "     || ********   *******   ||\n" << setw(10);
        cout << setw(0) << "     ||                                                    ||\n" << setw(10);
        cout << setw(20) << "     ||                                                    ||\n" << setw(10);

        cout << setw(20) << "  ===============\t WELCOME TO THE SUDOKU GAME\t ================" << endl;
        cout << setw(20) << "     ||                                                    ||\n" << setw(10);
        cout << setw(20) << "     ||                                                    ||\n" << setw(10);
        cout << setw(20) << "     ||  ********   *********  ||\n" << setw(10);

        cout << setw(10) << " 1:  To Play  Game  " << endl;
        cout << setw(10) << " 2:  Credits        " << endl;
        cout << setw(10) << " 3:  Instructions   " << endl;
        cout << setw(10) << " 4:  Score          " << endl;
        cout << setw(10) << " 5:  To stop it          " << endl;
        cout << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << endl;
            system("color A5");
            start_game(sudokuGrid, curr_score);
            cout << endl;
            break;
        case 2:
            cout << endl;
            system("color B5");
            credits();
            cout << endl;
            break;
        case 3:
            cout << endl;
            system("color A5");
            instructions();
            cout << endl;
            break;
        case 4:
            scorefile.open("scores.txt");
            cout << "High Scores:" << endl;
            int currentPlayer;
            int currentScore;
            while (scorefile >> currentScore >> currentPlayer) {
                cout << currentPlayer << ": " << currentScore << endl;
            }
            scorefile.close();
            break;
        case 5:
            cout << "Exiting the game. Goodbye!" << endl;
            break;
        default:
            cout << "You entered the wrong choice!" << endl;
        }

    } while (choice != 5);

    system("pause");
    return 0;
}
