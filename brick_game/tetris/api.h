/**
 * @file api.h
 * @brief API игры Tetris.
 */

#pragma once
#include <stdbool.h>

/** @brief Файл для хранения рекорда. */

#define HIGHSCORE_FILE "highscore.txt"

/**
 * @brief Действия пользователя.
 */

typedef enum {
  Start,     /**< Запуск игры. */
  Pause,     /**< Пауза. */
  Terminate, /**< Завершение игры. */
  Left,      /**< Движение влево. */
  Right,     /**< Движение вправо. */
  Up,        /**< Поворот фигуры. */
  Down,      /**< Ускоренное падение. */
  Action,
  Cheats /**< Чит-код. */
} UserAction_t;

/**
 * @brief Текущее состояние игры.
 */

typedef struct {
  int** field;    /**< Игровое поле. */
  int** next;     /**< Следующая фигура. */
  int score;      /**< Текущий счёт. */
  int high_score; /**< Рекорд. */
  int level;      /**< Уровень. */
  int speed;      /**< Скорость игры. */
  int pause;      /**< Флаг паузы. */
  int game_over;  /**< Флаг конца игры. */
} GameInfo_t;

/**
 * @brief Глобальная структура с информацией об игре.
 */
extern GameInfo_t game_info;

/**
 * @brief Обрабатывает ввод пользователя.
 * @param action Действие пользователя.
 * @param hold true, если кнопка удерживается.
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Обновляет и возвращает текущее состояние игры.
 * @return Структура состояния.
 */

GameInfo_t updateCurrentState();

/** @brief Загружает рекорд из файла. */
void load_high_score();

/** @brief Сохраняет рекорд в файл. */
void save_high_score();

/** @brief Задержка падения фигуры (мс). */
extern int drop_delay_ms;

/**
 * @brief Обновляет задержку падения в зависимости от скорости.
 * @param speed Скорость игры.
 */
void update_drop_delay(int speed);

/**
 * @brief Устанавливает флаг инициализации API.
 * @param value true — API инициализировано, false — API не инициализировано.
 */
void set_inited_ptr(bool value);

/**
 * @brief Устанавливает время последнего тика игры.
 * @param ms Время в миллисекундах.
 */
void set_last_tick_ms(long ms);