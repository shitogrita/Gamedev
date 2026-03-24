#include "tetris.h"

#include <unistd.h>

#include "../../gui/cli/debug_tools.h"
#include "../../gui/cli/interface.h"
#include "fsm.h"

int field[HEIGHT][WIDTH];
Tetromino current;
Tetromino next;

GameInfo_t game_info;

const int tetromino_shape[7][4][4] = {{// I
                                       {0, 0, 0, 0},
                                       {1, 1, 1, 1},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0}},
                                      {// O
                                       {0, 0, 0, 0},
                                       {0, 1, 1, 0},
                                       {0, 1, 1, 0},
                                       {0, 0, 0, 0}},
                                      {// T
                                       {0, 1, 0, 0},
                                       {0, 1, 1, 0},
                                       {0, 1, 0, 0},
                                       {0, 0, 0, 0}},
                                      {// S
                                       {0, 1, 1, 0},
                                       {1, 1, 0, 0},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0}},
                                      {// Z
                                       {0, 0, 0, 0},
                                       {1, 1, 0, 0},
                                       {0, 1, 1, 0},
                                       {0, 0, 0, 0}},
                                      {// J
                                       {1, 0, 0, 0},
                                       {1, 1, 1, 0},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0}},
                                      {// L
                                       {0, 0, 1, 0},
                                       {1, 1, 1, 0},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0}}};

Tetromino create_tetromino(int type) {
  Tetromino t;
  t.type = type;
  t.x = 3;
  t.y = 0;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) t.shape[i][j] = tetromino_shape[type][i][j];
  return t;
}

Tetromino create_tetromino_with_move(int type, int x, int y) {
  Tetromino t;
  t.type = type;
  t.x = x;
  t.y = y;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) t.shape[i][j] = tetromino_shape[type][i][j];
  return t;
}

void start_game() {
  memset(field, 0, sizeof(field));
  current = create_tetromino(rand() % 7);
  next = create_tetromino(rand() % 7);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (next.shape[i][j]) exit(1);
    }
  }
}

void tetris_spawn_new_piece() {
  next.x = 3;
  next.y = 0;
  current = next;
  next = create_tetromino(rand() % 7);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (next.shape[i][j]) exit(1);
    }
  }
}

void rotate_with_center(Tetromino* t) {
  int before_center_x = 0;
  int before_center_y = 0;
  int count = 0;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (t->shape[i][j]) {
        before_center_y += i;
        before_center_x += j;
        count++;
      }
    }
  }
  if (count == 0) return;

  before_center_x /= count;
  before_center_y /= count;

  int rotated[4][4] = {0};
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) rotated[j][3 - i] = t->shape[i][j];

  int after_center_x = 0;
  int after_center_y = 0;
  count = 0;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (rotated[i][j]) {
        after_center_y += i;
        after_center_x += j;
        count++;
      }
    }
  }

  after_center_x /= count;
  after_center_y /= count;

  int dx = before_center_x - after_center_x;
  int dy = before_center_y - after_center_y;

  Tetromino temp = *t;
  memcpy(temp.shape, rotated, sizeof(rotated));
  temp.x += dx;
  temp.y += dy;

  if (!check_collision(&temp)) {
    *t = temp;
  }
}

void rotate_current() { rotate_with_center(&current); }

bool tick_down() {
  Tetromino temp = current;
  temp.y += 1;

  if (!check_collision(&temp)) {
    current.y += 1;
    return true;
  }
  return false;
}

bool check_collision(Tetromino* t) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (t->shape[i][j]) {
        int nx = t->x + j;
        int ny = t->y + i;
        if (nx < 0 || nx >= WIDTH || ny >= HEIGHT) return true;
        if (ny >= 0 && field[ny][nx]) return true;
      }
    }
  }
  return false;
}

void move_piece(char direction) {
  Tetromino temp = current;
  switch (direction) {
    case 'D':
      temp.x += 1;
      break;
    case 'A':
      temp.x -= 1;
      break;
    case 'S':
      temp.y += 1;
      break;
  }
  if (!check_collision(&temp)) current = temp;
}

void drop() {
  if (current.y < 2) {
    return;
  }
  int limit = HEIGHT;
  while (tick_down() && limit--) {
  }
  game_state = STATE_LOCK;
  tetris_update();
}

bool is_game_over() { return check_collision(&current); }

void tetris_reset() {
  memset(field, 0, sizeof(field));
  current = create_tetromino(rand() % 7);
  next = create_tetromino(rand() % 7);
}

bool tetris_can_move_down() {
  Tetromino temp = current;
  temp.y++;
  return !check_collision(&temp);
}

void tetris_clear_lines() {
  int lines_cleared = 0;
  int write_row = HEIGHT - 1;

  for (int read_row = HEIGHT - 1; read_row >= 0; read_row--) {
    bool full = true;
    for (int x = 0; x < WIDTH; x++) {
      if (!field[read_row][x]) {
        full = false;
        break;
      }
    }
    if (!full) {
      memcpy(field[write_row], field[read_row], sizeof(field[read_row]));
      write_row--;
    } else {
      lines_cleared++;
    }
  }
  for (int y = write_row; y >= 0; y--) {
    memset(field[y], 0, sizeof(field[y]));
  }
  if (lines_cleared > 0) {
    add_score(lines_cleared);
  }
}

void tetris_lock_piece() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (current.shape[i][j]) {
        int x = current.x + j;
        int y = current.y + i;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
          field[y][x] = 1;
        }
      }
    }
  }
  current = next;
}

// part 2, 3

void add_score(int lines) {
  switch (lines) {
    case 1:
      game_info.score += 100;
      break;
    case 2:
      game_info.score += 300;
      break;
    case 3:
      game_info.score += 700;
      break;
    case 4:
      game_info.score += 1500;
      break;
  }

  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    save_high_score();
  }

  int new_level = game_info.score / 600 + 1;
  if (new_level > game_info.level && new_level <= 10) {
    game_info.level = new_level;
    if (game_info.speed < 10) {
      game_info.speed++;
      update_drop_delay(game_info.speed);
    }
  }
}
