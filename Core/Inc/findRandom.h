/**
 * @file findRandom.h
 * @brief Header file containing the declaration of the 'findRandomXO' function.
 *
 * This header file provides the declaration of the 'findRandomXO' function, which is used to find two random 
 * empty positions on the game board. These positions can be used to place game pieces in a random order.
 *
 * This function is typically used in scenarios where the game needs a random selection of positions, such as 
 * when generating random moves or placements for the game.
 */
#ifndef SRC_FINDRANDOM_H_
#define SRC_FINDRANDOM_H_

#pragma once

#include <stdbool.h>
#include "gameConfiguration.h"

/**
 * @brief Finds two random empty positions on the game board.
 *
 * This function searches the game board for two random empty spots. The coordinates of these 
 * positions are stored in the provided pointers. The function returns true if two 
 * empty spots are found and false if not.
 *
 * @param board The current game board.
 * @param size The size of the game board (3x3, 4x4, etc.).
 * @param x1 Pointer to the first x-coordinate of a random empty spot.
 * @param y1 Pointer to the first y-coordinate of a random empty spot.
 * @param x2 Pointer to the second x-coordinate of a random empty spot.
 * @param y2 Pointer to the second y-coordinate of a random empty spot.
 * @return True if two random empty positions are found, false otherwise.
 */
bool findRandomXO(char board[maxFieldSize][maxFieldSize], int size, int *x1, int *y1, int *x2, int *y2);

#endif
