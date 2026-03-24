#include "debug_tools.h"

void draw_debug(int info) {
  mvprintw(15, 15, "%d\n\n", info);
  refresh();
}

void draw_debug_piece(Tetromino piece) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      mvprintw(17 + i, 45 + j, " %d %d %d", piece.shape[i][j], i, j);
    }
  }
}
