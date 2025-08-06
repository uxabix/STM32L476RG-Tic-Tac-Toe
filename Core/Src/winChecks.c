/**
 * @file winChecks.c
 * @brief Implements functions for checking winning conditions and draw state in a game.
 *
 * This file provides the implementation of functions used to check if there is a winner in the game
 * or if the game has resulted in a draw. It checks for horizontal, vertical, and diagonal wins, as well 
 * as the draw state where no player has won and all cells are filled.
 *
 * The functions are used to evaluate the current state of the board after each move and determine if the game 
 * should continue, has been won, or is a draw.
 */
#include <stdbool.h>
#include "winChecks.h"

/**
 * @brief Checks if there is a winning condition horizontally.
 * 
 * This function checks each row in the game field to determine if there are 'toWin' consecutive marks
 * of the player represented by 'toCheck' in any of the rows.
 *
 * @param field The current game field.
 * @param toCheck The character to check for (either 'X' or 'O').
 * @param toWin The number of consecutive marks required for a win.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if there is a horizontal win, false otherwise.
 */
bool checkWinHorizontally(char field[maxFieldSize][maxFieldSize], char toCheck, int toWin, int fieldSize) {
	for (int y = 0; y < fieldSize; y++) {
		int inRow = 0;
		for (int x = 0; x < fieldSize; x++) {
			if (field[y][x] == toCheck) {
				inRow++;
			} else {
				inRow = 0;
			}
			if (inRow == toWin) {
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief Checks if there is a winning condition vertically.
 *
 * This function checks each column in the game field to determine if there are 'toWin' consecutive marks
 * of the player represented by 'toCheck' in any of the columns.
 *
 * @param field The current game field.
 * @param toCheck The character to check for (either 'X' or 'O').
 * @param toWin The number of consecutive marks required for a win.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if there is a vertical win, false otherwise.
 */
bool checkWinVerticaly(char field[maxFieldSize][maxFieldSize], char toCheck, int toWin, int fieldSize) {
	for (int x = 0; x < fieldSize; x++) {
		int inRow = 0;
		for (int y = 0; y < fieldSize; y++) {
			if (field[y][x] == toCheck) {
				inRow++;
			} else {
				inRow = 0;
			}
			if (inRow == toWin) {
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief Checks for a left-to-right diagonal win.
 *
 * This function checks each possible left-to-right diagonal on the board to see if the player represented 
 * by 'toCheck' has a winning condition, which means having 'toWin' consecutive marks.
 *
 * @param field The current game field.
 * @param yStart The starting row for the diagonal check.
 * @param toCheck The character to check for (either 'X' or 'O').
 * @param toWin The number of consecutive marks required for a win.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if there is a left-to-right diagonal win, false otherwise.
 */
bool leftToRightDiagonally(char field[maxFieldSize][maxFieldSize], int yStart, char toCheck, int toWin, int fieldSize) {
	for (int xStart = 0; xStart < fieldSize; xStart++) {
		int inRow = 0;
		for (int adder = 0; adder < fieldSize; adder++) {
			if (xStart + adder >= fieldSize || yStart + adder >= fieldSize) {
				break;
			}
			if (field[yStart + adder][xStart + adder] == toCheck) {
				inRow++;
			} else {
				inRow = 0;
			}
			if (inRow == toWin) {
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief Checks for a right-to-left diagonal win.
 *
 * This function checks each possible right-to-left diagonal on the board to see if the player represented 
 * by 'toCheck' has a winning condition, which means having 'toWin' consecutive marks.
 *
 * @param field The current game field.
 * @param yStart The starting row for the diagonal check.
 * @param toCheck The character to check for (either 'X' or 'O').
 * @param toWin The number of consecutive marks required for a win.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if there is a right-to-left diagonal win, false otherwise.
 */
bool rightToLeftDiagonally(char field[maxFieldSize][maxFieldSize], int yStart, char toCheck, int toWin, int fieldSize) {
	for (int xStart = fieldSize - 1; xStart >= 0; xStart--) {
		int inRow = 0;
		for (int adder = 0; adder < fieldSize; adder++) {
			if (xStart - adder < 0 || yStart + adder >= fieldSize) {
				break;
			}
			if (field[yStart + adder][xStart - adder] == toCheck) {
				inRow++;
			} else {
				inRow = 0;
			}
			if (inRow == toWin) {
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief Checks for a diagonal win.
 *
 * This function checks all diagonals for a win condition by calling the respective diagonal check functions.
 * It checks if the player represented by 'toCheck' has 'toWin' consecutive marks in any diagonal direction.
 *
 * @param field The current game field.
 * @param toCheck The character to check for (either 'X' or 'O').
 * @param toWin The number of consecutive marks required for a win.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if there is a diagonal win, false otherwise.
 */
bool checkWinDiagonaly(char field[maxFieldSize][maxFieldSize], char toCheck, int toWin, int fieldSize) {
	for (int yStart = 0; yStart < fieldSize; yStart++) {
		bool result = false;
		result += leftToRightDiagonally(field, yStart, toCheck, toWin, fieldSize);
		result += rightToLeftDiagonally(field, yStart, toCheck, toWin, fieldSize);
		if (result)
			return result;
	}

	return false;
}

/**
 * @brief Checks if the game has resulted in a draw.
 *
 * This function checks if all cells on the board are filled with either 'X' or 'O', and if there is no winner.
 * If all cells are filled and no winner exists, the game is considered a draw.
 *
 * @param field The current game field.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return True if the game is a draw, false otherwise.
 */
bool isDraw(char field[maxFieldSize][maxFieldSize], int fieldSize) {
	for (int y = 0; y < fieldSize; y++) {
		for (int x = 0; x < fieldSize; x++) {
			if (field[y][x] != firstPlayerChar
					&& field[y][x] != secondPlayerChar)
				return false;
		}
	}

	return true;
}

/**
 * @brief Checks if there is a winner or if the game has ended in a draw.
 *
 * This function checks for horizontal, vertical, and diagonal wins, as well as a draw condition. 
 * It returns the winner's character ('X' or 'O') or 'd' for a draw. If there is no winner and no draw, 
 * it returns '\0'.
 *
 * @param field The current game field.
 * @param fieldSize The size of the game field (3x3, 4x4, etc.).
 * @return The character of the winner ('X' or 'O'). Returns 'd' for a draw or '\0' if no result.
 */
char checkWin(char field[maxFieldSize][maxFieldSize], int fieldSize) {
	char toCheck[2] = { firstPlayerChar, secondPlayerChar };
	int toWin = fieldSize > 3 ? fieldSize - 1 : 3;
	for (int i = 0; i < 2; i++) {
		bool result = false;
		result += checkWinHorizontally(field, toCheck[i], toWin, fieldSize);
		result += checkWinVerticaly(field, toCheck[i], toWin, fieldSize);
		result += checkWinDiagonaly(field, toCheck[i], toWin, fieldSize);
		if (result) {
			return toCheck[i];
		}
	}
	if (isDraw(field, fieldSize))
		return 'd';

	return '\0';
}
