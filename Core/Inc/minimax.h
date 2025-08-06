/**
 * @file minimax.h
 * @brief Header file providing the minimax algorithm for computer moves.
 *
 * This file contains the declaration of the function, which enables the computer 
 * to make a move on the game board by selecting the best possible move according to the minimax algorithm.
 * The function analyzes the game state and chooses the optimal move based on the current board configuration.
 */
#pragma once
#ifndef SRC_MINIMAX_H_
#define SRC_MINIMAX_H_

#include "gameConfiguration.h"

/**
 * @brief Calculates and performs the best move for the computer player using the minimax algorithm.
 *
 * This function evaluates the game board and uses the minimax algorithm to determine the optimal move
 * for the computer. It modifies the provided board by placing the computer's move at the calculated position.
 * 
 * @param board The current game board represented as a 2D array.
 * @param size The size of the game board (e.g., 3x3, 4x4, or 5x5).
 */
void ComputerMove(char board[maxFieldSize][maxFieldSize], int size);


#endif /* SRC_MINIMAX_H_ */
