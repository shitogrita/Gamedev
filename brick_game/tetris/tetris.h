/**
 * @file tetris.h
 * @brief Заголовочный файл с основными структурами и функциями для логики игры
 * Tetris.
 *
 * Этот файл содержит определения констант, структур и функций,
 * обеспечивающих работу движка Tetris, включая генерацию фигур,
 * обработку перемещений, проверку столкновений и очистку линий.
 */

#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "api.h"

/**
 * @brief Ширина игрового поля в клетках.
 */
#define WIDTH 10

/**
 * @brief Высота игрового поля в клетках.
 */
#define HEIGHT 20

/**
 * @brief Структура, описывающая тетромино (фигуру в Tetris).
 *
 * Содержит форму фигуры, её позицию на поле и тип (вид фигуры).
 */
typedef struct {
  int shape[4][4]; /**< Матрица формы фигуры (4x4). */
  int x;           /**< Позиция фигуры по горизонтали. */
  int y;           /**< Позиция фигуры по вертикали. */
  int type;        /**< Тип фигуры (0–6). */
} Tetromino;

/**
 * @brief Игровое поле, представленное в виде двумерного массива.
 *
 * field[y][x] = 0 — пустая клетка,
 * field[y][x] = 1 — занятая клетка.
 */
extern int field[HEIGHT][WIDTH];

/**
 * @brief Текущая активная фигура.
 */
extern Tetromino current;

/**
 * @brief Следующая фигура.
 */
extern Tetromino next;

void start_game();                    /**< Начало игры. */
Tetromino create_tetromino(int type); /**< Создание фигуры. */
Tetromino create_tetromino_with_move(int type, int x,
                                     int y); /**< Создание фигуры. */
void rotate_current();                       /**< Поворот фигуры. */
void move_piece(char direction);             /**< Движение фигуры. */
bool tick_down();                            /**< Опускание фигуры. */
void drop();                                 /**< Мгновенный сброс фигуры. */
bool check_collision(Tetromino* t);          /**< Проверка столкновения. */
bool is_game_over();                         /**< Проверка конца игры. */
void tetris_reset();                         /**< Сброс игры. */
void tetris_clear_lines();                   /**< Очистка линий. */
void tetris_lock_piece();                    /**< Закрепление фигуры. */
bool tetris_can_move_down(); /**< Фигура может двагаться вниз. */

#define HIGHSCORE_FILE "highscore.txt" /**< Файл рекорда. */

void add_score(int lines); /**< Добавление очков. */