#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  TETRIS_LOOP_QUIT = 0,
  TETRIS_LOOP_TO_SNAKE = 1
} TetrisLoopResult;

TetrisLoopResult run_tetris_cli();

#ifdef __cplusplus
}
#endif
