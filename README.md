# Tic-Tac-Toe on STM32L476RG

This project is an implementation of the classic Tic-Tac-Toe game on an STM32L476RG microcontroller. It was developed in January 2025 as a team project for the Computer Architecture (Architektura Komputerów) course during the 3rd semester at Politechnika Białostocka.

The game features a graphical interface on an OLED display and is controlled via a computer keyboard using USART communication.

## Features

-   **Game Modes:** Play against another player (2P) or against the AI (1P).
-   **Variable Board Size:** Choose between 3x3, 4x4, and 5x5 game boards.
-   **Graphical Display:** Utilizes an OLED display (SSD1306/SSD1309) to render the game board and menus.
-   **Keyboard Control:** Game input is handled through a standard computer keyboard via a serial terminal.
-   **AI Opponent:** The single-player mode features a computer opponent powered by the Minimax algorithm.
-   **Custom Rules:** An optional game mode where game pieces are randomly removed from the board, adding an extra challenge.
-   **Themes:** Switch between light and dark themes for visual preference.

## Hardware Requirements

-   **Microcontroller:** STM32L476RG Nucleo-64 board.
-   **Display:** OLED Display with an SSD1306 or SSD1309 driver (connected via I2C).
-   **PC/Laptop:** A computer to connect to the Nucleo board for power and serial communication.

## Project Structure

The project is organized into the following main directories:

-   `Core/`: Contains the core application source code.
    -   `Inc/`: Header files (`.h`) for the application logic, drivers, and configuration.
    -   `Src/`: Source files (`.c`) implementing the game logic, display rendering, and hardware interaction.
-   `Drivers/`: STM32 HAL library files and CMSIS drivers.

### Key Files in `Core/Src/`:
-   `main.c`: The main application entry point, containing the main game loop and state machine.
-   `drawField.c`: Handles all rendering of the game board and pieces on the OLED display.
-   `fieldController.c`: Manages the state of the game board array.
-   `minimax.c`: Implements the AI logic for the single-player mode.
-   `winChecks.c`: Contains functions to check for win, lose, or draw conditions.

## Setup and Installation

1.  **Clone the Repository:**
    ```sh
    git clone <repository-url>
    ```
2.  **Open in STM32CubeIDE:**
    -   Launch STM32CubeIDE.
    -   Select `File > Import...`.
    -   Choose `General > Existing Projects into Workspace` and click `Next`.
    -   Browse to the cloned repository's root directory.
    -   Ensure the project is selected and click `Finish`.
3.  **Build the Project:**
    -   Right-click on the project in the Project Explorer.
    -   Select `Build Project`.
4.  **Flash the Microcontroller:**
    -   Connect the STM32 Nucleo board to your computer via USB.
    -   Click the `Run` button in STM32CubeIDE (or `Run > Run`) to flash the built program to the board.

## Usage

1.  **Connect a Serial Terminal:**
    -   Connect to the ST-Link's Virtual COM Port from your computer using a terminal emulator (e.g., PuTTY, Tera Term, minicom).
    -   Use the following settings:
        -   **Baud Rate:** 115200
        -   **Data Bits:** 8
        -   **Parity:** None
        -   **Stop Bits:** 1
2.  **Game Controls:**
    -   `w`: Move cursor up
    -   `s`: Move cursor down
    -   `a`: Move cursor left
    -   `d`: Move cursor right
    -   `Enter`: Confirm selection / Place a piece
    -   `q`: Quit the current game and return to the main menu
