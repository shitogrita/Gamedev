/**
* @file snake.cpp
 * @brief Реализация логики змейки.
 */

#include "snake.h"
#include <algorithm>
#include <iostream>

namespace s21 {

snake::snake() {
  body_.clear();
  int y = 10;
  int start_x = 3;
  for (int i = 0; i < 4; ++i) {
    body_.insert(body_.begin(), Segment{start_x + i, y});
  }
  dx_ = 1;
  dy_ = 0;
  dead_ = false;
  win_ = false;
}

/*
       (0,-1)
(-1,0)   |    (1,0)
_________|_________
         |
         |
       (0,1)
*/

void snake::rotateDown() {
  if (dy_ == 0) {
    dy_ = 1;
    dx_ = 0;
  }
}

void snake::rotateUp() {
  if (dy_ == 0) {
    dy_ = -1;
    dx_ = 0;
  }
}

void snake::rotateLeft() {
  if (dx_ == 0) {
    dx_ = -1;
    dy_ = 0;
  }
}

void snake::rotateRight() {
  if (dx_ == 0) {
    dx_ = 1;
    dy_ = 0;
  }
}

int snake::length() const { return static_cast<int>(body_.size()); }

void snake::turn(Direction dir) {
  switch (dir) {
    case Up:
      rotateUp();
      break;
    case Down:
      rotateDown();
      break;
    case Right:
      rotateRight();
      break;
    case Left:
      rotateLeft();
      break;
  }
}

bool snake::collides(int x, int y) const {
  if (x < 0 || x >= width_ || y < 0 || y >= height_)
    return true;  // проверка стен и просто проход по полю
  for (const auto& s : body_) {
    if (s.x == x && s.y == y) {
      return true;
    }
  }
  return false;
}

bool snake::move(int apple_x, int apple_y) {
  if (dead_ || win_) return false;

  Segment head = body_.front();
  int newX = head.x + dx_;
  int newY = head.y + dy_;
  if (collides(newX, newY)) {
    dead_ = true;
    return false;
  }

  body_.insert(body_.begin(), Segment{newX, newY});

  if (newX == apple_x && newY == apple_y) {
    if (static_cast<int>(body_.size()) >= 200) {
      win_ = true;
    }
    return true;
  }

  body_.pop_back();

  return false;
}
}  // namespace s21
