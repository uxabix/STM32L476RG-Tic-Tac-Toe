/**
 * @file minimax.c
 * @brief Contains the implementation of the Minimax algorithm for the computer's moves.
 *
 * This file contains the implementation of the Minimax algorithm, used to calculate the best possible 
 * move for the computer player in a Tic-Tac-Toe-like game. The computer analyzes all possible moves, 
 * evaluates them using the Minimax algorithm, and selects the move that maximizes its chances of winning.
 *
 * The file includes functions to determine available moves on the board, the evaluation of game states,
 * and the decision-making process for the computer player.
 */
#include <stdbool.h>
#include "minimax.h"
#include "winChecks.h"

/**
 * @brief Retrieves all available moves for the current player.
 *
 * This function scans the game board for empty spaces and stores the coordinates of 
 * available moves in the provided 'moves' array. The number of available moves is returned through 
 * the 'moveCount' parameter.
 * 
 * @param board The current game board.
 * @param size The size of the game board.
 * @param moves A 2D array to store the available move coordinates.
 * @param moveCount The total number of available moves.
 */
void GetAvailableMoves(char board[maxFieldSize][maxFieldSize], int size,
		int moves[][2], int *moveCount) {
	*moveCount = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == '\0') {
				moves[*moveCount][0] = i;
				moves[*moveCount][1] = j;
				(*moveCount)++;
			}
		}
	}
}

/**
 * @brief The Minimax algorithm for evaluating the best move.
 *
 * This function recursively evaluates the game board and assigns scores to different possible moves
 * based on whether they lead to a win for the computer or the player. The function uses alpha-beta 
 * pruning to optimize the search.
 *
 * The 'isMaximizing' parameter indicates whether the current turn is for the computer (maximizing) 
 * or the player (minimizing). The function returns a score based on the best move found.
 *
 * @param board The current game board.
 * @param size The size of the game board.
 * @param depth The current search depth in the recursion.
 * @param isMaximizing A flag indicating whether we are maximizing or minimizing.
 * @param alpha The best score found so far for the maximizing player.
 * @param beta The best score found so far for the minimizing player.
 * @return The best score for the current state.
 */
int Minimax(char board[maxFieldSize][maxFieldSize], int size, int depth,
		bool isMaximizing, int alpha, int beta) {
	char gameStatus = checkWin(board, size);
	int score;
	if (gameStatus == secondPlayerChar) {
		score = 10;
	} else if (gameStatus == firstPlayerChar) {
		score = -10;
	} else {
		score = 0;
	}

	if (score == 10 || score == -10)
		return score - depth;
	if (depth == 0 || isDraw(board, size))
		return 0;

	int moves[maxFieldSize * maxFieldSize][2], moveCount;
	GetAvailableMoves(board, size, moves, &moveCount);

	if (isMaximizing) {
		int bestScore = -1000;
		for (int i = 0; i < moveCount; i++) {
			int x = moves[i][0], y = moves[i][1];
			board[x][y] = secondPlayerChar;
			bestScore = max(bestScore,
					Minimax(board, size, depth - 1, false, alpha, beta));
			board[x][y] = '\0';

			alpha = max(alpha, bestScore);
			if (beta <= alpha)
				break;
		}
		return bestScore;
	} else {
		int bestScore = 1000;
		for (int i = 0; i < moveCount; i++) {
			int x = moves[i][0], y = moves[i][1];
			board[x][y] = firstPlayerChar;
			bestScore = min(bestScore,
					Minimax(board, size, depth - 1, true, alpha, beta));
			board[x][y] = '\0';

			beta = min(beta, bestScore);
			if (beta <= alpha)
				break;
		}
		return bestScore;
	}
}

/**
 * @brief Makes the computer's move based on the best possible choice.
 *
 * This function calculates the best possible move for the computer player using the Minimax algorithm
 * and then makes the move on the game board. The computer always tries to maximize its chances of winning.
 * 
 * The 'maxDepth' is determined based on the size of the game board (larger boards may have a smaller depth 
 * for optimization reasons).
 *
 * @param board The current game board.
 * @param size The size of the game board.
 */
void ComputerMove(char board[maxFieldSize][maxFieldSize], int size) {
	int bestScore = -1000;
	int bestMove[2] = { -1, -1 };

	int moves[maxFieldSize * maxFieldSize][2], moveCount;
	GetAvailableMoves(board, size, moves, &moveCount);

	int maxDepth = (size <= 3) ? 9 : (size <= 4 ? 4 : 3);

	for (int i = 0; i < moveCount; i++) {
		int x = moves[i][0], y = moves[i][1];
		board[x][y] = secondPlayerChar;

		int moveScore = Minimax(board, size, maxDepth, false, -1000, 1000);

		board[x][y] = '\0';

		if (moveScore > bestScore) {
			bestScore = moveScore;
			bestMove[0] = x;
			bestMove[1] = y;
		}
	}

	board[bestMove[0]][bestMove[1]] = secondPlayerChar;
}
