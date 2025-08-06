/**
 * @file findRandom.c
 * @brief Implementation of functions related to random selection of positions on the board.
 *
 * This file contains functions that allow random selection of positions on the board
 * where certain symbols are present. This is useful in various parts of the game 
 * where random elements are required, such as determining initial moves in the game.
 */
#include "findRandom.h"

/**
 * @brief Finds a random position on the board that contains the specified symbol.
 *
 * This function scans the board for all positions containing the specified symbol,
 * then randomly selects one of these positions and stores its coordinates in the provided 'x' and 'y' pointers.
 *
 * @param board The game board represented as a 2D array.
 * @param size The size of the board (e.g. 3x3, 4x4, 5x5).
 * @param x Pointer to the variable where the x-coordinate of the random position will be stored.
 * @param y Pointer to the variable where the y-coordinate of the random position will be stored.
 * @param toFind The symbol to be searched for on the board (e.g. 'X' or 'O').
 * @return Returns 'true' if at least one position with the symbol 'toFind' is found, otherwise 'false'.
 */
bool findRandom(char board[maxFieldSize][maxFieldSize], int size, int *x, int *y, char toFind) {
    int positions[size * size][2];
    int count = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == toFind) {
                positions[count][0] = i;
                positions[count][1] = j;
                count++;
            }
        }
    }

    if (count == 0) return false;

    int randomIndex = rand() % count;
    *x = positions[randomIndex][0];
    *y = positions[randomIndex][1];
    return true;
}

/**
 * @brief Finds random positions for both players on the board.
 *
 * This function calls 'findRandom' twice: once for player 1, and once for player 2.
 * If random positions are found for both players, the function returns 'true', otherwise it returns 'false'.
 *
 * @param board The game board represented as a 2D array.
 * @param size The size of the board (e.g. 3x3, 4x4, 5x5).
 * @param x1 Pointer to the variable where the x-coordinate of player 1's random position will be stored.
 * @param y1 Pointer to the variable where the y-coordinate of player 1's random position will be stored.
 * @param x2 Pointer to the variable where the x-coordinate of player 2's random position will be stored.
 * @param y2 Pointer to the variable where the y-coordinate of player 2's random position will be stored.
 * @return Returns 'true' if random positions were found for both players, otherwise 'false'.
 */
bool findRandomXO(char board[maxFieldSize][maxFieldSize], int size, int *x1, int *y1, int *x2, int *y2){
	bool res1 = findRandom(board, size, x1, y1, firstPlayerChar);
	bool res2 = findRandom(board, size, x2, y2, secondPlayerChar);

	return res1 && res2;
}
