/**
 * @file interface.h
 * @brief CLI-интерфейс для игры Tetris с использованием ncurses.
 */

#pragma once
#include "../../brick_game/tetris/tetris.h"

/** @brief Смещение поля по X. */
#define OFFSET_X 5
/** @brief Смещение поля по Y. */
#define OFFSET_Y 2

/**
 * @brief Код клавиши "стрелка вниз" в ncurses.
 */
#define KEY_DOWN 0402

/**
 * @brief Код клавиши "стрелка вверх" в ncurses.
 */
#define KEY_UP 0403

/**
 * @brief Код клавиши "стрелка влево" в ncurses.
 */
#define KEY_LEFT 0404

/**
 * @brief Код клавиши "стрелка вправо" в ncurses.
 */
#define KEY_RIGHT 0405

/** @brief Инициализирует интерфейс. */
void init_interface();

/** @brief Отображает следующию фигуру. */
void draw_next_piece();

/** @brief Завершает работу интерфейса. */
void shutdown_interface();

/** @brief Отображает статус игры (счёт, уровень и т. д.). */
void draw_status();

/**
 * @brief Отрисовывает игровое поле.
 * @param field Указатель на массив поля.
 */
void render(int** field);
