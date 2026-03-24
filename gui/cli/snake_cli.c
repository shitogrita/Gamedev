#include "snake_cli.h"

#include <ncurses.h>

#include "../../brick_game/snake/snake_api.h"

static void snake_init_interface() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(150);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
}

static void snake_shutdown_interface() { endwin(); }

static void snake_draw_border(int width, int height, int ox, int oy) {
  for (int y = 0; y <= height; y++) {
    mvprintw(oy + y, ox - 2, "||");
    mvprintw(oy + y, ox + width * 2, "||");
  }
  for (int x = -1; x <= width; x++) {
    mvprintw(oy + height, ox + x * 2, "--");
  }
}

static void snake_draw_field(SnakeInfo_t* info, int ox, int oy) {
  if (!info || !info->field) return;

  for (int y = 0; y < info->height; ++y) {
    for (int x = 0; x < info->width; ++x) {
      int cell = info->field[y][x];
      if (cell == 1) {
        attron(COLOR_PAIR(1));
        mvprintw(oy + y, ox + x * 2, "[]");
        attroff(COLOR_PAIR(1));
      } else if (cell == 2) {
        attron(COLOR_PAIR(2));
        mvprintw(oy + y, ox + x * 2, "()");
        attroff(COLOR_PAIR(2));
      } else {
        mvprintw(oy + y, ox + x * 2, "  ");
      }
    }
  }
}

static void snake_draw_info(SnakeInfo_t* info, int ox, int oy) {
  int info_x = ox + info->width * 2 + 5;
  mvprintw(1, info_x, "Snake C Game");
  mvprintw(3, info_x, "a/Left    - move left");
  mvprintw(4, info_x, "d/Right   - move right");
  mvprintw(5, info_x, "w/Up      - move up");
  mvprintw(6, info_x, "s/Down    - move down");
  mvprintw(7, info_x, "p         - pause");
  mvprintw(8, info_x, "r         - restart");
  mvprintw(9, info_x, "g         - go to Tetris");
  mvprintw(10, info_x, "q         - quit");

  mvprintw(12, info_x, "Score: %d", info->score);
  mvprintw(13, info_x, "High:  %d", info->high_score);
  mvprintw(14, info_x, "Level: %d", info->level);
  mvprintw(15, info_x, "Apples:%d", info->apples);

  if (info->pause) {
    mvprintw(17, info_x, "[PAUSE]");
  }
  if (info->game_over) {
    mvprintw(18, info_x, "[GAME OVER]");
  }
}

typedef enum { SNAKE_LOOP_QUIT = 0, SNAKE_LOOP_TO_TETRIS = 1 } SnakeLoopResult;

static SnakeLoopResult snake_loop() {
  SnakeInfo_t info = snakeUpdateState();

  int ox = 5;
  int oy = 2;

  int running = 1;
  while (running) {
    clear();
    snake_draw_border(info.width, info.height, ox, oy);
    snake_draw_field(&info, ox, oy);
    snake_draw_info(&info, ox, oy);
    refresh();

    int ch = getch();
    switch (ch) {
      case 'a':
      case 'A':
      case KEY_LEFT:
        snakeUserInput(SNAKE_ACTION_LEFT);
        break;
      case 'd':
      case 'D':
      case KEY_RIGHT:
        snakeUserInput(SNAKE_ACTION_RIGHT);
        break;
      case 'w':
      case 'W':
      case KEY_UP:
        snakeUserInput(SNAKE_ACTION_UP);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        snakeUserInput(SNAKE_ACTION_DOWN);
        break;
      case 'p':
      case 'P':
        snakeUserInput(SNAKE_ACTION_PAUSE);
        break;
      case 'r':
      case 'R':
        snakeUserInput(SNAKE_ACTION_RESET);
        break;
      case 'g':
      case 'G':
        return SNAKE_LOOP_TO_TETRIS;
      case 'q':
      case 'Q':
        return SNAKE_LOOP_QUIT;
      default:
        break;
    }

    info = snakeUpdateState();

    if (info.game_over || info.win) {
    }
  }

  return SNAKE_LOOP_QUIT;
}

void run_snake_cli() {
  snake_init_interface();
  SnakeLoopResult res = snake_loop();
  snake_shutdown_interface();
}
