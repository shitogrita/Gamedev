#include "tetris_cli.h"

#include <curses.h>

#include "../../brick_game/tetris/tetris.h"
#include "interface.h"

TetrisLoopResult run_tetris_cli() {
  init_interface();
  userInput(Start, false);

  while (true) {
    int ch = getch();
    switch (ch) {
      case 'a':
        userInput(Left, false);
        break;
      case 'd':
        userInput(Right, false);
        break;
      case 's':
        userInput(Down, false);
        break;
      case ' ':
        userInput(Up, false);
        break;
      case 'p':
        userInput(Pause, false);
        break;

      case 'g':
        shutdown_interface();
        return TETRIS_LOOP_TO_SNAKE;

      case 'q':
        shutdown_interface();
        return TETRIS_LOOP_QUIT;
    }

    GameInfo_t info = updateCurrentState();
    render(info.field);
  }
}
