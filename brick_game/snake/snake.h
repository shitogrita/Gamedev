/**
* @file snake.h
 * @brief Логика змейки: движение, рост, столкновения.
 */

#pragma once

#include <ctime>
#include <random>
#include <utility>
#include <vector>

#include "apple.h"

namespace s21 {

 /**
 * @brief Направление движения змейки.
 */
enum Direction { None, Up, Down, Left, Right };
 /**
  * @brief Класс змейки.
  *
  * Хранит сегменты тела, направление движения, состояние (жив/мёртв/выиграл),
  * а также проверяет столкновения и рост при поедании яблока.
  */
class snake {
 public:
  struct Segment {
    int x;
    int y;
  };
  snake();

  bool move(int apple_x, int apple_y);

  const std::vector<Segment>& body() const { return body_; }

  bool isDead() const { return dead_; }
  bool isWin() const { return win_; }  // size == 200
  void turn(Direction dir);  // dir = -1 (left) / dir = +1 (right) // dir for
                             // keyPass // хуй

  int length() const;

 private:
  void rotateLeft();
  void rotateRight();
  void rotateDown();
  void rotateUp();

  static constexpr int width_ = 10;
  static constexpr int height_ = 20;
  std::vector<Segment> body_;
  bool collides(int x, int y) const;

  int dx_{1};
  int dy_{0};

  bool dead_{false};
  bool win_{false};
};
}  // namespace s21
