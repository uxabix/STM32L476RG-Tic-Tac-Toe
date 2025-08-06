/**
 * @file winChecks.h
 * @brief Contains the declaration of the function for checking winning conditions in the game.
 *
 * This file includes the declaration of the 'checkWin' function, which is responsible for checking
 * if there is a winning condition on the game board, for either player. It evaluates rows, columns, 
 * and diagonals to determine if a player has achieved a win.
 * 
 * The winning condition is checked for both 'X' (first player) and 'O' (second player), and the result 
 * is returned accordingly.
 */
#ifndef SRC_WINCHECKS_H_
#define SRC_WINCHECKS_H_

#include "gameConfiguration.h"

/**
 * @brief Checks if there is a winning condition on the board.
 *
 * This function checks all rows, columns, and diagonals of the game field to determine if there is 
 * a winner. It returns the character of the player who has won, or '\0' if there is no winner.
 * 
 * @param field The current game field.
 * @param fieldSize The size of the game field (3x3, 4x4, 5x5, etc.).
 * @return The character representing the winner ('X' or 'O'). If there is no winner, it returns '\0'.
 */
char checkWin(char field[maxFieldSize][maxFieldSize], int fieldSize);

#endif
