/**
 * @file fieldController.h
 * @brief Defines the interface for controlling the game field.
 *
 * This header file declares functions to manipulate the game field, including
 * clearing the field and placing player characters ("X" or "O"). The field 
 * is represented as a 2D array, and the functions provide the necessary 
 * operations for updating and interacting with the game board.
 */
#ifndef SRC_FIELDCONTROLLER_H_
#define SRC_FIELDCONTROLLER_H_

#pragma once

#include <stdbool.h>
#include "gameConfiguration.h"

/**
 * @brief The game field represented as a 2D array.
 *
 * The field is used to store the game state, with each cell holding either 
 * a character ('X' or 'O') or being empty ('\0').
 */
extern char field[maxFieldSize][maxFieldSize];
/**
 * @brief The size of the game field.
 *
 * The field size is defined by the constant `maxFieldSize`, which is used 
 * to allocate the field array.
 */
extern int fieldSize;

/**
 * @brief Clears the game field by resetting all cells.
 *
 * This function sets all cells in the game field to '\0', effectively 
 * clearing the board for a new game.
 */
void clearField();

/**
 * @brief Places a character on the game field.
 *
 * The function attempts to place a specified character ('X' or 'O') 
 * at the given (x, y) coordinates on the field. If the cell is free , 
 * the character is placed, and the function returns true. If the cell 
 * is already occupied, the function returns false.
 *
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param toPlace The character ('X' or 'O') to place on the field.
 * @return true if the character was placed, false if the cell was already occupied.
 */
bool placeChar(int x, int y, char toPlace);

#endif
