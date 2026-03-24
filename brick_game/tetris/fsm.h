/**
 * @file fsm.h
 * @brief Состояния конечного автомата игры Tetris.
 */

#pragma once
#include "tetris.h"

/**
 * @brief Состояния игры.
 */

typedef enum {
  STATE_START,
  STATE_SPAWN,       /**< Появление новой фигуры. */
  STATE_FALLING,     /**< Падение фигуры. */
  STATE_LOCK,        /**< Закрепление фигуры. */
  STATE_CLEAR_LINES, /**< Очистка заполненных линий. */
  STATE_GAME_OVER    /**< Конец игры. */
} GameState;

extern GameState game_state; /**< Текущее состояние игры. */

/**
 * @brief Выполняет один шаг обновления состояния игры.
 */
void tetris_update();
