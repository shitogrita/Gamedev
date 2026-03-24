// api.c
#include "api.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "fsm.h"
#include "tetris.h"

static int field_storage[HEIGHT][WIDTH];
static int next_storage[4][4];
static int* field_ptrs[HEIGHT];
static int* next_ptrs[4];

static long last_tick_ms = 0;
static int inited_ptrs = 0;

int drop_delay_ms = 500;
static long now_ms() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void userInput(UserAction_t action, bool hold) {
  if (action == Pause) {
    game_info.pause = !game_info.pause;
    return;
  }

  if (game_info.pause || game_state == STATE_GAME_OVER ||
      game_state == STATE_LOCK)
    return;
  if (action == Start) {
    tetris_reset();
    game_info.score = 0;
    game_info.level = 1;
    game_info.speed = 1;
    game_info.pause = 0;
    load_high_score();
    game_state = STATE_SPAWN;
    last_tick_ms = now_ms();
    return;
  }
  if (action == Cheats) {
    game_info.score = 5000;
    game_info.level = 10;
  }

  if (action == Terminate) {
    game_state = STATE_GAME_OVER;
    return;
  }

  if (game_info.pause || game_state == STATE_GAME_OVER) return;

  switch (action) {
    case Left:
      move_piece('A');
      break;
    case Right:
      move_piece('D');
      break;
    case Down:
      drop();
      break;
    case Up:
    case Action:
      rotate_current();
      break;
    default:
      break;
  }
}

GameInfo_t updateCurrentState() {
  if (!inited_ptrs) {
    for (int y = 0; y < HEIGHT; ++y) field_ptrs[y] = field_storage[y];
    for (int i = 0; i < 4; ++i) next_ptrs[i] = next_storage[i];
    inited_ptrs = 1;
    last_tick_ms = now_ms();
  }

  long now = now_ms();
  if (!game_info.pause && game_state != STATE_GAME_OVER &&
      now - last_tick_ms >= drop_delay_ms) {
    tetris_update();
    last_tick_ms = now;
  }

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      field_storage[y][x] = field[y][x];
    }
  }

  if (game_state != STATE_LOCK && game_state != STATE_GAME_OVER) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (current.shape[i][j]) {
          int tx = current.x + j;
          int ty = current.y + i;
          if (tx >= 0 && tx < WIDTH && ty >= 0 && ty < HEIGHT) {
            field_storage[ty][tx] = current.shape[i][j];
          }
        }
      }
    }
  }

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) next_storage[i][j] = 0;

  game_info.field = field_ptrs;
  game_info.next = next_ptrs;
  game_info.game_over = (game_state == STATE_GAME_OVER) ? 1 : 0;

  return game_info;
}

// part 2, 3

void load_high_score() {
  FILE* f = fopen(HIGHSCORE_FILE, "r");
  if (f) {
    fscanf(f, "%d", &game_info.high_score);
    fclose(f);
  } else {
    game_info.high_score = 0;
  }
}

void save_high_score() {
  FILE* f = fopen(HIGHSCORE_FILE, "w");
  if (f) {
    fprintf(f, "%d", game_info.high_score);
    fclose(f);
  }
}

void update_drop_delay(int speed) {
  drop_delay_ms = 50 * (10 - speed);
  if (drop_delay_ms < 50) drop_delay_ms = 50;
}

/* TESTS ONLY FUNCTIONS */

void set_inited_ptr(bool value) { inited_ptrs = value; }

void set_last_tick_ms(long ms) { last_tick_ms = ms; }