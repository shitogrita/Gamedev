#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int width;
  int height;
  int** field;
  int score;
  int high_score;
  int level;
  int apples;
  int game_over;
  int win;
  int pause;
} SnakeInfo_t;

typedef enum {
  SNAKE_ACTION_NONE = 0,
  SNAKE_ACTION_UP,
  SNAKE_ACTION_DOWN,
  SNAKE_ACTION_LEFT,
  SNAKE_ACTION_RIGHT,
  SNAKE_ACTION_PAUSE,
  SNAKE_ACTION_RESET,
} SnakeAction_t;

/**
 * @brief Обработать ввод пользователя (из консоли или ещё откуда).
 */
void snakeUserInput(SnakeAction_t action);

/**
 * @brief Обновить состояние змейки (шаг игры).
 * @return Текущая информация об игре.
 */
SnakeInfo_t snakeUpdateState();

#ifdef __cplusplus
}
#endif
