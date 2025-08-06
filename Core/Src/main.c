/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Tic-Tac-Toe game for two players or against a computer, controlled via UART
 * 
 * @external_files
 * - **ssd1306.h**: Library for controlling the SSD1306 OLED display, used for rendering the game interface.
 * - **ssd1306_tests.h**: Contains test functions for verifying the functionality of the SSD1306 display.
 * - **ssd1306_fonts.h**: Includes font definitions for rendering text on the SSD1306 OLED display.
 * - **sx1509.h**: Header for the SX1509 I/O expander, used for managing additional inputs/outputs.
 * - **gameConfiguration.h**: Defines game settings and parameters, such as board size and player settings.
 * - **fieldController.h**: Manages the state of the game board (e.g., placing pieces, checking for a win).
 * - **minimax.h**: Contains the Minimax algorithm used for the computer's moves in the single-player mode.
 * - **winChecks.h**: Contains functions for checking if a player has won the game by evaluating horizontal, vertical, and diagonal lines of pieces on the board.
 * - **findRandom.h**: Provides functions for finding random coordinates on the game field.
 * - **drawField.h**: Responsible for rendering the game field on the display.
 * 
 * 
 * This program implements the classic Tic-Tac-Toe game with options for single-player or 
 * two-player modes. The game runs on the STM32L476RG microcontroller and uses the SSD1306 OLED 
 * display along with UART for user interaction.
 * 
 * The functionalities of the program include:
 * - **Board Size Selection**: Allows the player to choose from three different board sizes:
 *   - 3x3
 *   - 4x4
 *   - 5x5
 * - **Custom Mode**: In this mode, at random points during the game, 1 circle and 1 cross are removed from the board, adding an extra challenge.
 * - **Two-Player Mode**: Two players can compete against each other on the same board, taking turns to make their moves.
 * - **Single-Player Mode**: The player can play against the computer, which makes moves based on the Minimax algorithm.
 * - **Minimax Algorithm**: The computer analyzes possible moves, evaluates them, and selects the optimal move to increase its chances of winning.
 * - **Exit Game**: The player can exit the game without finishing it. The game remembers the state and can be resumed later.
 * - **Resume Game**: The player can resume the previous game from where it was left off.
 * - **View Last Game Results**: After a game ends, the player can view the result, showing who won or if it was a draw.
 * - **Switch Themes**: The player can toggle between a light and dark theme for display preferences.
 * 
 * The project uses the following technologies:
 * - STM32L476RG Microcontroller
 * - SSD1306 OLED Display
 * - I2C and SPI Interfaces
 * - UART Interface for User Communication
 * 
 * @author Kiryl Alishkevich, Igor Kondratowicz, Konrad Lupinski, Paweł Rutkowski
 * @date 2025-01-30
 * 
 * @note This file is part of a larger system controlling the Tic-Tac-Toe game.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 * 
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include "sx1509.h"
#include <stdbool.h>

#include "gameConfiguration.h"
#include "fieldController.h"
#include "minimax.h"
#include "winChecks.h"
#include "findRandom.h"
#include "drawField.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * @brief Structure for managing the I2C interface.
 *
 * The variable 'hi2c1' holds the configuration and state information for the I2C interface.
 * It is used in functions related to data transmission via I2C.
 */
I2C_HandleTypeDef hi2c1;

/**
 * @brief Structure for managing the SPI interface.
 *
 * The variable 'hspi1' holds the configuration and state information for the SPI interface.
 * It is used for communication with devices using the SPI protocol.
 */
SPI_HandleTypeDef hspi1;

/**
 * @brief Structure for managing the UART interface.
 *
 * The variable 'huart2' holds configuration and state data for the UART interface.
 * It is used for serial communication (UART) with external devices.
 */
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief Function reads a 16-bit value from the INA device via I2C.
 *
 * The function sends a read request to the specified register of the INA device
 * using the I2C protocol, and then receives a two-byte response, which it interprets
 * as a 16-bit value. In case of an error, it returns the value 0xFFFF.
 *
 * @param reg The register from which the value should be read
 * @return The value read from the register as a 16-bit number, 
 *         or 0xFFFF in case of an error
 */
static uint16_t readWordINA(uint8_t reg) {
	HAL_StatusTypeDef ret;
	uint8_t buf[2] = { reg };
	ret = HAL_I2C_Master_Transmit(&SX1509_I2C_PORT, INAAddress, buf, 1,
	i2CTimeout);
	if (ret == HAL_OK) {
		ret = HAL_I2C_Master_Receive(&SX1509_I2C_PORT, INAAddress, buf, 2,
		i2CTimeout);
		if (ret == HAL_OK) {
			uint16_t tmp = buf[0];
			tmp <<= 8;
			tmp |= buf[1];
			return tmp;
		}
	}
	return 0xFFFF;
}

/**
 * @brief UART data reception buffer.
 *
 * The 'uart_rx_buffer' variable stores the received data from the UART port. It is used
 * in the function that handles interrupts or data reception via UART.
 */
uint8_t uart_rx_buffer;


/**
 * @brief Flag indicating whether the screen needs to be updated.
 *
 * This variable is set to 'true' when the screen needs to be refreshed, e.g., as a result
 * of changes in the game or menu state.
 */
bool isNeedToUpdate = true;
/**
 * @brief Flag indicating whether the game should continue.
 *
 * The variable is set to 'true' if the user wants to continue the game
 * or false when starting a new game.
 */
bool isGameToContinue = false;
/**
 * @brief Flag indicating whether single-player mode is enabled.
 *
 * This variable is set to 'true' if the game is being played in single-player mode
 * or 'false' if it's a two-player game.
 */
bool singlePlayerFlag = false;
/**
 * @brief Flag indicating whether the game is in custom mode.
 *
 * The variable is set to 'true' when the game is in custom mode.
 */
bool customModeFlag = false;
/**
 * @brief Number of moves until the next pair of cells will be removed.
 *
 * The variable stores the number of moves after which random cells
 * will be removed from the board. If it is set to '-1', cell removal is disabled.
 */
int movesToNextRemoval = -1;
/**
 * @brief Flag indicating whether the color theme should be inverted.
 *
 * The variable is set to 'true' if the color theme of the game
 * should be inverted, which might be useful in low-light conditions.
 */
bool invertThemeFlag = false;

/**
 * @brief Game states.
 *
 * The enum defines the different states in which the game can be.
 */
enum gameStates {
	mainMenu, game, options, gameResult,
};
/**
 * @brief Current game state.
 *
 * The variable holds the current state of the game, determining
 * which screen or mode is currently active.
 */
enum gameStates currentState = mainMenu;

char lastWinner;

/**
 * @brief X coordinate of the active cell.
 *
 * The variable stores the current position of the active cell on the board
 * along the X axis. This helps track which cell is currently selected by the player.
 */
int activeCellX = 0;
/**
 * @brief Y coordinate of the active cell.
 *
 * The variable stores the current position of the active cell on the board
 * along the Y axis. This helps track which cell is currently selected by the player.
 */
int activeCellY = 0;

// Main Menu
/**
 * @brief Active button in the main menu.
 *
 * The variable stores the index of the currently active button in 
 * the main menu. Its value can range from 1 to 4, depending on 
 * the selected button.
 */
int activeMainButton = 2;
/**
 * @brief Number of buttons in the main menu.
 *
 * The variable stores the number of buttons in the main menu. This helps 
 * manage displaying and selecting the buttons by the user.
 */
int mainMenuButtonsCount = 4;
/**
 * @brief Array of button labels in the main menu.
 *
 * The variable holds the text associated with each button 
 * in the main menu. 
 */
char mainMenuButtons[][WIDTH / mainFontWidth] = { "-Continue", "New 2p",
		"New 1p", "Options" };

// Options Menu
/**
 * @brief Active button in the options menu.
 *
 * The variable stores the index of the currently active button in the
 * options menu.
 */
int activeOptionButton = 1;
/**
 * @brief Number of buttons in the options menu.
 *
 * The variable stores the number of buttons in the options
 * menu, allowing easy management of the user's option selection.
 */
int optionsButtonsCount = 5;
/**
 * @brief Number of buttons related to board size in the options menu.
 *
 * The variable defines the number of buttons related to the
 * board size selection.
 */
int sizeButtonsCount = 3;
/**
 * @brief Array of button labels in the options menu.
 *
 * The variable holds the text associated with each button in the options menu.
 */
char optionsButtons[][WIDTH / mainFontWidth] = { "+5x5", "-4x4", "-3x3",
		"-Custom Rules", "-Light theme" };

/**
 * @brief Displays a menu on the OLED screen with the active button highlighted.
 *
 * The function renders a menu on the OLED display, where each
 * button is centered on the screen. The active button is highlighted,
 * and the other buttons are displayed as text. The highlighting of the active
 * button involves drawing a rectangle around it.
 *
 * @param buttons An array containing the text of the buttons in the menu
 * @param buttonsCount The number of buttons in the menu
 * @param activeButton The index of the active button
 */
void displayMenu(char buttons[][WIDTH / mainFontWidth], int buttonsCount,
		int activeButton) {
	if (invertThemeFlag) ssd1306_Fill(White);
	int gap = (HEIGHT - buttonsCount * mainFontHeight) / (buttonsCount + 1);
	int selectionY = gap * activeButton
			+ mainFontHeight * (activeButton - 1)- selectionPadding;

	ssd1306_DrawRectangle(15, selectionY, 113,
			selectionY + mainFontHeight + selectionPadding, invertThemeFlag ? Black : White);
	for (int i = 0; i < buttonsCount; i++) {
		int startX = (WIDTH - strlen(buttons[i]) * mainFontWidth) / 2;
		ssd1306_SetCursor(startX, gap * (i + 1) + mainFontHeight * i);
		ssd1306_WriteString(buttons[i], Font_7x10, invertThemeFlag ? Black : White);
	}
}

bool playerFlag = true; // true = player 1, false = player 2

/**
 * @brief Function sets the game's continuation state.
 *
 * The function changes the value of the `isGameToContinue` variable and updates 
 * the first button in the main menu to either the "+" symbol or the "-" symbol, 
 * indicating whether the game is to continue.
 *
 * @param state The game's state, indicating whether the game should continue (true) or not (false).
 */
void setGameToContinue(bool state){
	isGameToContinue = state;
	mainMenuButtons[0][0] = state ? '+' : '-';
}

/**
 * @brief Function initializes the settings for a new game.
 *
 * The function resets all relevant variables and prepares the game to start. 
 * The game field is cleared, the game continuation state is set to "false", 
 * the player flag is initialized to true, and the coordinates of the active cell 
 * are set to (0, 0).
 */
void setupNewGame() {
	clearField();
	setGameToContinue(false);
	playerFlag = true;
	activeCellX = 0;
	activeCellY = 0;
}

/**
 * @brief Function displays the end screen after the game, showing the winner.
 *
 * The function sets the active cell coordinates to -1, redraws the game field in
 * its final state, and then displays an appropriate message on the screen 
 * indicating the result of the game (a draw or the victory of one player).
 *
 * @param winner A character indicating the game result:
 *               'd' means a draw, 
 *               'P1' means player 1 won, 
 *               'P2' means player 2 won, 
 *               and '\0' means no winner.
 */
void winScreen(char winner) {
	activeCellX = -1;
	activeCellY = -1;
	drawField(field, fieldSize, activeCellX, activeCellY, true, invertThemeFlag);
	ssd1306_SetCursor(1, 1);
	if (winner == 'd') {
		ssd1306_WriteString("Draw!", Font_6x8, invertThemeFlag ? White : Black);
		return;
	}
	if (winner != '\0') {
		ssd1306_WriteString(winner == firstPlayerChar ? "P1 Won!" : "P2 Won!",
				Font_6x8, invertThemeFlag ? White : Black);
		return;
	}
}

/**
 * @brief Function draws the game field and checks if the game has ended.
 *
 * The function checks the game result, and if a player has won, it calls the end screen 
 * with the appropriate winner message. If the game is not finished, it redraws the game field 
 * and displays whose turn it is.
 */
void drawGame() {
	char winner = checkWin(field, fieldSize);
	if (winner != '\0'){
		lastWinner = winner;
		currentState = gameResult;
		return winScreen(winner);
	}
	drawField(field, fieldSize, activeCellX, activeCellY, false, invertThemeFlag);
	ssd1306_SetCursor(1, 1);
	ssd1306_WriteString(playerFlag ? "Player 1" : "Player 2", Font_6x8, invertThemeFlag ? Black : White);
}

/**
 * @brief Function makes the computer's move in the game.
 *
 * This function toggles the player flag, redraws the game field, and then makes the computer's move. 
 * After the computer makes its move, the player flag is toggled again to indicate it is the player's turn.
 */
void computersTurn() {
	playerFlag = !playerFlag;
	ssd1306_Fill(Black);
	drawGame();
	ssd1306_UpdateScreen();
	ComputerMove(field, fieldSize);
	playerFlag = !playerFlag;
}

/**
 * @brief Function handles input events from the main menu.
 *
 * The function responds to input events from the main menu, such as pressing buttons 
 * and changes the active button in the menu.
 * */
void mainMenuInputEvent() {
	switch (uart_rx_buffer) {
	case 'w':
		activeMainButton--;
		if (activeMainButton < 1) {
			activeMainButton = 1;
			isNeedToUpdate = false;
		}
		break;
	case 's':
		activeMainButton++;
		if (activeMainButton > mainMenuButtonsCount) {
			activeMainButton = mainMenuButtonsCount;
			isNeedToUpdate = false;
		}
		break;
	case '\n':
	case '\r':
		switch (activeMainButton) {
		case 1:
			if (isGameToContinue) {
				currentState = game;
			}
			break;
		case 2:
			setupNewGame();
			singlePlayerFlag = false;
			currentState = game;
			break;
		case 3:
			setupNewGame();
			singlePlayerFlag = true;
			currentState = game;
			break;
		case 4:
			currentState = options;
		}
		break;
	}
}

/**
 * @brief Function handles input events during the game.
 *
 * The function responds to player input events for movement and for confirming a move by placing 
 * a symbol on the game field. It also handles custom mode logic, where after a certain number of moves,
 * random symbols may be removed from the field. In single-player mode, it triggers the computer's 
 * turn after the player's move.
 */
void gameInputEvent() {
	switch (uart_rx_buffer) {
	case 'a':
		activeCellX--;
		if (activeCellX < 0) {
			activeCellX = 0;
		}
		break;
	case 'd':
		activeCellX++;
		if (activeCellX > fieldSize - 1) {
			activeCellX = fieldSize - 1;
		}
		break;
	case 'w':
		activeCellY--;
		if (activeCellY < 0) {
			activeCellY = 0;
		}
		break;
	case 's':
		activeCellY++;
		if (activeCellY > fieldSize - 1) {
			activeCellY = fieldSize - 1;
		}
		break;
	case '\n':
	case '\r':
		setGameToContinue(true);
		bool isFree = placeChar(activeCellX, activeCellY,
				playerFlag ? firstPlayerChar : secondPlayerChar);
		if (isFree && customModeFlag){
			if (movesToNextRemoval <= -1){
				movesToNextRemoval = singlePlayerFlag ? rand() % 3 + 2 : (rand() % 3 + 2) * 2;

			} else if (movesToNextRemoval == 0) {
				int x1, x2, y1, y2;
				if (findRandomXO(field, fieldSize, &x1, &y1, &x2, &y2)){
					field[y1][x1] = '\0';
					field[y2][x2] = '\0';
				}
			}
			movesToNextRemoval--;
		}
		if (singlePlayerFlag && isFree) {
			computersTurn();
		} else {
			playerFlag = isFree ? !playerFlag : playerFlag;
		}
		break;
	case 'q':
		currentState = mainMenu;
		break;
	}
}

/**
 * @brief Function handles input events in the options menu.
 *
 * The function responds to input events related to navigation in the options menu
 * and updating the game settings. It changes the field size, toggles custom mode,
 * and switches the color theme based on the selected option.
 */
void optionsInputEvent() {
	switch (uart_rx_buffer) {
	case 'w':
		activeOptionButton--;
		if (activeOptionButton < 1) {
			activeOptionButton = 1;
			isNeedToUpdate = false;
		}
		break;
	case 's':
		activeOptionButton++;
		if (activeOptionButton > optionsButtonsCount) {
			activeOptionButton = optionsButtonsCount;
			isNeedToUpdate = false;
		}
		break;
	case '\n':
	case '\r':
		if (activeOptionButton <= sizeButtonsCount) {
			setupNewGame();
			for (int i = 0; i < sizeButtonsCount; i++)
				optionsButtons[i][0] = '-';
			fieldSize = optionsButtons[activeOptionButton - 1][1] - '0';

			optionsButtons[activeOptionButton - 1][0] = '+';
		} else if (activeOptionButton == optionsButtonsCount - 1) {
			optionsButtons[optionsButtonsCount - 2][0] =
					optionsButtons[optionsButtonsCount - 2][0] == '+' ?
							'-' : '+';
			customModeFlag = !customModeFlag;
		} else {
			optionsButtons[optionsButtonsCount - 1][0] =
								optionsButtons[optionsButtonsCount - 1][0] == '+' ?
										'-' : '+';
			invertThemeFlag = !invertThemeFlag;
		}
		break;
	case 'q':
		currentState = mainMenu;
		break;
	}
}

/**
 * @brief Function handles input events on the game result screen.
 *
 * The function responds to input events where the player can choose to return to the 
 * main menu after the game ends by pressing the 'q' key.
 */
void gameResultInputEvent(){
	switch (uart_rx_buffer) {
	case 'q':
		currentState = mainMenu;
		break;
	}
}

/**
 * @brief UART receive complete callback function.
 *
 * The function is called when data reception via UART is completed. Depending on the 
 * current game state, it triggers the appropriate input handling function for 
 * the main menu, game, options, or game result screens.
 *
 * @param huart Pointer to the UART handle structure that indicates which UART has completed the transmission.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) {
		isNeedToUpdate = true;
		switch (currentState) {
		case mainMenu:
			mainMenuInputEvent();
			break;
		case game:
			gameInputEvent();
			break;
		case options:
			optionsInputEvent();
			break;
		case gameResult:
			gameResultInputEvent();
			break;
		}
	}
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	uint16_t reg = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	ssd1306_Init();
	srand(time(NULL));

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		if (isNeedToUpdate) {
			isNeedToUpdate = false;
			ssd1306_Fill(Black);
			switch (currentState) {
			case mainMenu:
				displayMenu(mainMenuButtons, mainMenuButtonsCount,
						activeMainButton);
				break;
			case game:
				drawGame();
				break;
			case options:
				displayMenu(optionsButtons, optionsButtonsCount,
						activeOptionButton);
				break;
			case gameResult:
				winScreen(lastWinner);
				break;
			}
			ssd1306_UpdateScreen();
		}
		HAL_UART_Receive_IT(&huart2, &uart_rx_buffer, 1);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
	/** Initializes the peripherals clock  */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart2ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	/* Enable Peripheral clock */
	__HAL_RCC_I2C1_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**USART2 GPIO Configuration
	 PA2     ------> USART2_TX
	 PA3     ------> USART2_RX
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x10D19CE4;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
