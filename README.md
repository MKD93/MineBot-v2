# MineBot-v2

## A Minesweeper solver written in C++. Supports solving and "cheating" on Windows XP Minesweeper games.

## Instructions:
#### Download source code and compile, or download executable files. Run one or more instances of Minesweeper from the provided executable file. Run the solve or cheat executables to solve or cheat on active instances of Minesweeper.

## Notes:
#### The solve and cheat executables will terminate if there are no active instances of Minesweeper. It is possible to solve or cheat on newly created instances of Minesweeper while the solve or cheat executables are already running. After the Minesweeper game has terminated, the user must manually reset the game in order to continue solving or cheating on that game.

## How it works:
#### The MineBot program searches for active processes in memory that are instances of Minesweeper. After collecting process information, game state data (i.e. known values of tiles) is read directly from memory. If "cheating" is enabled, then moves are chosen based off of where there is no mine. Otherwise, the game state solver attempts to determine the most viable moves based off of what is visible to the player. The game state solver first attempts to use patern recognition to determine definite safe moves. If there are no known safe moves, then the game state solver quantifies the risks of each move, and chooses the move with the lowest risks. If there is not enough data to know risk statistics, then a move is chosen pseudo-randomly. After a move has been selected, user input is simulated by sending messages directly to active Minesweeper processes that indicate a "mouse click" has been made.

## Sources:
#### For single and pair neighbor pattern solving: http://www.minesweeper.info/wiki/Strategy
#### For advanced guessing: http://nothings.org/games/minesweeper/
