/**
 * @file drawField.h
 * @brief Header for drawing the game field on the screen.
 *
 * This file contains the declaration of the 'drawField' function, which is responsible
 * for rendering the game field on the screen. The function handles the display of the game
 * grid with the current state of the game, including the active cell and support for inverting
 * the display theme if needed.
 */
#ifndef SRC_DRAWFIELD_H_
#define SRC_DRAWFIELD_H_

#pragma once

/**
 * @brief Draws the game field on the screen.
 *
 * This function updates the display to show the current state of the game field, including the 
 * positions of 'X' and 'O' characters, the active cell, and optionally inverts the display theme 
 * depending on the game settings.
 *
 * @param field A 2D array representing the game board, where each element is a character ('x', 'o', or empty space).
 * @param fieldSize The size of the game field (e.g., 3x3, 4x4, etc.).
 * @param activeCellX The X-coordinate of the currently active cell that the user is interacting with.
 * @param activeCellY The Y-coordinate of the currently active cell that the user is interacting with.
 * @param invert A flag to indicate whether the display should be inverted (for accessibility or design purposes).
 * @param invertThemeFlag A flag indicating if the theme of the game should be dark or light.
 */
void drawField(char field[maxFieldSize][maxFieldSize], int fieldSize, int activeCellX, int activeCellY, bool invert, bool invertThemeFlag);

#endif
