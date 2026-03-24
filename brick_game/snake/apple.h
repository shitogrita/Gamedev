/**
* @file apple.h
 * @brief Класс яблока для игры "Змейка".
 */

#pragma once
#include <ctime>
#include <random>
#include <utility>
#include <vector>

#include "snake.h"

namespace s21 {
 /**
 * @brief Объект яблока на игровом поле.
 *
 * Хранит координаты яблока и умеет случайно спавниться на свободной клетке,
 * не занимая клетки, которые уже заняты змейкой.
 */
class apple {
 public:
  apple();

  void spawn(const std::vector<std::pair<int, int>>& occupied);

  int x() const { return x_; }
  int y() const { return y_; }

 private:
  bool busy(int x, int y,
            const std::vector<std::pair<int, int>>& occupied) const;
  int x_{0};
  int y_{0};
  std::mt19937 rng_;
};
}  // namespace s21
