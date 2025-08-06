/**
 * @file fieldController.c
 * @brief This file contains functions responsible for managing and controlling the game field.
 *
 * The game field is represented as a 2D array, and this file provides functions to clear the field,
 * place characters (such as 'X' or 'O') on the field, and check the state of the cells. The functions
 * in this file are essential for the game logic, allowing the players to interact with the game board.
 */
#include "fieldController.h"

/**
 * @brief The game field represented as a 2D array.
 *
 * The field is used to store the game state, with each cell holding either 
 * a character ('X' or 'O') or being empty ('\0').
 */
char field[maxFieldSize][maxFieldSize];
/**
 * @brief The size of the game field.
 *
 * The field size is defined by the constant `maxFieldSize`, which is used 
 * to allocate the field array.
 */
int fieldSize = maxFieldSize;

/**
 * @brief Clears the game field.
 *
 * This function resets the game field by setting all cells to '\0'. It is used 
 * at the start of a new game or when restarting the current game.
 */
void clearField() {
	for (int i = 0; i < maxFieldSize; i++) {
		for (int j = 0; j < maxFieldSize; j++) {
			field[i][j] = '\0';
		}
	}
}

/**
 * @brief Places a character ('X' or 'O') on the game field.
 *
 * This function attempts to place the given character at the specified 
 * (x, y) coordinates on the game field. If the cell at the specified 
 * coordinates is empty ('\0'), the character is placed, and the function 
 * returns 'true'. If the cell is already occupied, the function returns 'false'.
 * 
 * The function also ensures that the game board is not modified in any way 
 * when trying to place a character in an already occupied cell.
 *
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param toPlace The character to place, typically 'X' or 'O'.
 * @return true if the character was successfully placed, false if the cell is occupied.
 */
bool placeChar(int x, int y, char toPlace) {
	bool isFree = field[y][x] == '\0';
	field[y][x] = isFree ? toPlace : field[y][x];

	return isFree;
}
