# Tic-Tac-Toe on STM32L476RG

This project is a team-based implementation of the classic Tic-Tac-Toe game developed for the STM32L476RG microcontroller using the C programming language and the STM32 HAL library. It was created in January 2025 as part of the **Computer Architecture** (*Architektura Komputerów*) course during the **3rd semester of studies at Politechnika Białostocka**.

The game features a graphical interface rendered on an OLED display and is controlled through a computer keyboard using **USART** serial communication.

## 👥 Team Members

- **Kiryl Alishkevich**
- **Igor Kondratowicz**
- **Konrad Łupiński**
- **Paweł Rutkowski**

## 🎮 Features

- **Game Modes:** 
  - Player vs Player (2P)
  - Player vs AI (1P)
- **Board Sizes:** 
  - 3x3, 4x4, 5x5 grids
- **Graphical Display:** 
  - OLED screen with **SSD1306** or **SSD1309** driver (I2C)
- **USART Keyboard Control:** 
  - Game input via a PC keyboard through a serial terminal
- **AI Opponent:** 
  - Powered by the **Minimax algorithm**
- **Custom Rule Mode:** 
  - Randomly removes pieces during the game for added challenge
- **Visual Themes:** 
  - Light and Dark modes

## 🛠️ Hardware Requirements

- **Microcontroller:** STM32L476RG Nucleo-64 development board
- **Display:** OLED screen with SSD1306/SSD1309 driver via I2C
- **Computer:** Required for serial input and power

## 📁 Project Structure
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

## 🧩 Setup and Installation

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
  
    
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
This work was created as part of coursework at Bialystok University of Technology for educational and research purposes. When using or referencing this work, please provide appropriate attribution.
