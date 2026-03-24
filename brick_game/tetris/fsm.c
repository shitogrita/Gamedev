#include "fsm.h"

#include "tetris.h"

GameState game_state = STATE_START;

void tetris_update() {
  switch (game_state) {
    case STATE_FALLING:
      if (!tick_down()) {
        game_state = STATE_LOCK;
      }
      break;
    case STATE_START:
      current = create_tetromino(rand() % 7);
      next = create_tetromino(rand() % 7);
      game_state = STATE_FALLING;
      break;
    case STATE_SPAWN:
      current = next;
      next = create_tetromino(rand() % 7);
      if (check_collision(&current)) {
        game_state = STATE_GAME_OVER;
      } else {
        game_state = STATE_FALLING;
      }
      break;
    case STATE_LOCK:
      tetris_lock_piece();
      game_state = STATE_CLEAR_LINES;
      break;

    case STATE_CLEAR_LINES:
      tetris_clear_lines();
      game_state = STATE_SPAWN;
      break;

    case STATE_GAME_OVER:
      break;

    default:
      break;
  }
}
