/**
 * @file gameConfiguration.h
 * @brief Contains game-related constants and macros for configuration.
 *
 * This file defines constants and macros that are used throughout the game, including
 * screen dimensions, character definitions, and helper macros for mathematical operations.
 */
#pragma once

#define HEIGHT 64 /**< The height of the display in pixels */
#define WIDTH 128 /**< The width of the display in pixels */
#define mainFontHeight 10  /**< Height of the main font in pixels */
#define mainFontWidth 7  /**< Width of the main font in pixels */
#define selectionPadding 2  /**< Padding around the selected item */
#define xoWidth 6 /**< Width of the "X" or "O" character in pixels */
#define xoHeight 8  /**< Height of the "X" or "O" character in pixels */
#define firstPlayerChar 'x'  /**< Character representing the first player */
#define secondPlayerChar 'o'  /**< Character representing the second player */
#define maxFieldSize 5 /**< Maximum field size for the Tic-Tac-Toe board (5x5) */
#define playerTextCorrection 8 /**< Horizontal text correction for player names */

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; }) /**< Macro to return the maximum value between a and b */

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; }) /**< Macro to return the minimum value between a and b */
