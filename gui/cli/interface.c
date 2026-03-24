#include "interface.h"

#include <ncurses.h>

void init_interface() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(200);
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
}

void shutdown_interface() { endwin(); }

void draw_next_piece() {
  int start_y = 15;
  int start_x = OFFSET_X + WIDTH * 2 + 5;
  mvprintw(start_y, start_x, "Next:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (next.shape[i][j]) {
        mvprintw(start_y + 1 + i, start_x + j * 2, "[]");
      } else {
        mvprintw(start_y + 1 + i, start_x + j * 2, "  ");
      }
    }
  }
  attroff(COLOR_PAIR(1));
}

static void draw_border() {
  for (int y = 0; y <= HEIGHT; y++) {
    mvprintw(OFFSET_Y + y, OFFSET_X - 2, "||");
    mvprintw(OFFSET_Y + y, OFFSET_X + WIDTH * 2, "||");
  }
  for (int x = -1; x <= WIDTH; x++) {
    mvprintw(OFFSET_Y + HEIGHT, OFFSET_X + x * 2, "--");
  }
}

static void draw_field(int** field) {
  attron(COLOR_PAIR(1));
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (field[y][x])
        mvprintw(OFFSET_Y + y, OFFSET_X + x * 2, "[]");
      else
        mvprintw(OFFSET_Y + y, OFFSET_X + x * 2, "  ");
    }
  }
  attroff(COLOR_PAIR(1));
}

static void draw_info() {
  mvprintw(1, OFFSET_X + WIDTH * 2 + 5, "Tetris C Game");
  mvprintw(3, OFFSET_X + WIDTH * 2 + 5, "a/Left    - move left");
  mvprintw(4, OFFSET_X + WIDTH * 2 + 5, "d/Right   - move right");
  mvprintw(5, OFFSET_X + WIDTH * 2 + 5, "s/Down    - drop");
  mvprintw(6, OFFSET_X + WIDTH * 2 + 5, "Space/Up  - rotate");
  mvprintw(7, OFFSET_X + WIDTH * 2 + 5, "p         - pause");
  mvprintw(8, OFFSET_X + WIDTH * 2 + 5, "q         - quit");
}

void draw_status() {
  mvprintw(9, OFFSET_X + WIDTH * 2 + 5, "Level: %d", game_info.level);
  mvprintw(10, OFFSET_X + WIDTH * 2 + 5, "Score: %d", game_info.score);
  mvprintw(11, OFFSET_X + WIDTH * 2 + 5, "High score: %d",
           game_info.high_score);
  mvprintw(12, OFFSET_X + WIDTH * 2 + 5, "Speed: %d", game_info.speed);
}

void render(int** field) {
  clear();
  draw_border();
  draw_field(field);
  draw_next_piece();
  draw_info();
  draw_status();
  refresh();
}
