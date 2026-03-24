#include "gui/cli/snake_cli.h"
#include "gui/cli/tetris_cli.h"

int main() {
  int current = 0;
  int running = 1;

  while (running) {
    if (current == 0) {
      TetrisLoopResult res = run_tetris_cli();
      if (res == TETRIS_LOOP_TO_SNAKE) {
        current = 1;
      } else {
        running = 0;
      }
    } else {
      run_snake_cli();
      current = 0;
    }
  }

  return 0;
}
