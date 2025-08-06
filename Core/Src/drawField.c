/**
 * @file drawField.c
 * @brief Functions to render the game field and its elements on the screen.
 * 
 * This file contains the implementation of functions used to draw the game grid and the player moves 
 * on the screen using the SSD1306 OLED display. It includes functions to draw the grid of the 
 * game board, highlight the active cell, and render the 'X' and 'O' characters for each player. 
 * Additionally, it supports the ability to invert the color scheme for the display based on the 
 * 'invertThemeFlag', allowing for a dark or light theme.
 */
#include <stdbool.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "gameConfiguration.h"
#include "drawField.h"

/**
 * @brief Draws the grid of the game field on the screen.
 * 
 * This function draws the lines that create the visual structure of the game board. It divides the display
 * area into cells based on the 'fieldSize' and uses the 'invert' parameter to adjust the color scheme.
 * 
 * @param field A 2D array representing the game board (not directly used in this function, but required for grid).
 * @param fieldSize The size of the game field (e.g., 3x3, 4x4, etc.).
 * @param invert A flag indicating whether to use inverted colors (black and white).
 */
void drawCells(char field[maxFieldSize][maxFieldSize], int fieldSize, bool invert) {
	int height = HEIGHT - playerTextCorrection;
	int cellSize = height / fieldSize;
	int xMargin = (WIDTH - cellSize * fieldSize) / 2;
	for (int i = 0; i < fieldSize + 1; i++) {
		ssd1306_Line(xMargin + cellSize * i, playerTextCorrection,
				xMargin + cellSize * i, HEIGHT, invert ? Black : White);
	}

	for (int i = 0; i < fieldSize + 1; i++) {
		ssd1306_Line(xMargin, playerTextCorrection + cellSize * i,
		WIDTH - xMargin, playerTextCorrection + cellSize * i, invert ? Black : White);
	}
}

/**
 * @brief Draws 'X' and 'O' symbols in the game cells on the screen.
 *
 * This function displays 'X' and 'O' in their respective grid positions based on the game state. It also highlights
 * the currently active cell (where the player can make a move) with the appropriate colors based on the 'invert' flag.
 * 
 * @param field A 2D array representing the game board, with 'X' and 'O' characters indicating player moves.
 * @param fieldSize The size of the game field (e.g., 3x3, 4x4, etc.).
 * @param activeCellX The X-coordinate of the currently active cell (for player interaction).
 * @param activeCellY The Y-coordinate of the currently active cell.
 * @param invert A flag indicating whether to use inverted colors (black and white).
 */
void drawXO(char field[maxFieldSize][maxFieldSize], int fieldSize, int activeCellX, int activeCellY, bool invert) {
	int height = HEIGHT - playerTextCorrection;
	int cellSize = height / fieldSize;
	int xMargin = (WIDTH - cellSize * fieldSize) / 2;
	for (int y = 0; y < fieldSize; y++) {
		for (int x = 0; x < fieldSize; x++) {
			bool isActive = activeCellX == x && activeCellY == y;
			int selectionBorderX = xMargin + cellSize * x;
			int selectionBorderY = playerTextCorrection + cellSize * y;
			ssd1306_FillRectangle(selectionBorderX + 1, selectionBorderY + 1,
								selectionBorderX + cellSize - 1,
								selectionBorderY + cellSize - 1, isActive ? (invert ? Black : White) : (invert ? White : Black));
			ssd1306_SetCursor(
					xMargin + cellSize * x + (cellSize - xoWidth) / 2 + 1,
					playerTextCorrection + cellSize * y
							+ (cellSize - xoHeight) / 2);
			ssd1306_WriteChar(field[y][x], Font_6x8, isActive ? (invert ? White : Black) : (invert ? Black : White));
		}
	}
}

/**
 * @brief Draws the complete game field on the screen, including the grid and player moves.
 *
 * This function combines both drawing the grid and the player moves. It also handles the display
 * inversion theme based on the provided 'invertThemeFlag' parameter. The function uses 'invert' to determine 
 * the theme.
 * 
 * @param field A 2D array representing the game board, with 'X' and 'O' characters indicating player moves.
 * @param fieldSize The size of the game field (e.g., 3x3, 4x4, etc.).
 * @param activeCellX The X-coordinate of the currently active cell (for player interaction).
 * @param activeCellY The Y-coordinate of the currently active cell.
 * @param invert A flag indicating whether to use inverted colors (black and white).
 * @param invertThemeFlag A flag determining whether the theme (light/dark) should be inverted.
 */
void drawField(char field[maxFieldSize][maxFieldSize], int fieldSize, int activeCellX, int activeCellY, bool invert, bool invertThemeFlag){
	invert = invertThemeFlag ? !invert : invert;
	if (invert){
		ssd1306_Fill(White);
	}
	drawCells(field, fieldSize, invert);
	drawXO(field, fieldSize, activeCellX, activeCellY, invert);
}

