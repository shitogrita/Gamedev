/**
* @file apple.cpp
 * @brief Реализация класса яблока для игры "Змейка".
 */


#include "apple.h"
#include <iostream>
namespace s21 {
apple::apple() { rng_.seed(static_cast<unsigned>(std::time(nullptr))); }

bool apple::busy(int x, int y,
                 const std::vector<std::pair<int, int>>& occupied) const {
  for (const auto& cell : occupied) {
    if (cell.first == x && cell.second == y) return true;
  }
  return false;
}

void apple::spawn(const std::vector<std::pair<int, int>>& occupied) {
  std::uniform_int_distribution<int> dx(0, 9);
  std::uniform_int_distribution<int> dy(0, 19);

  for (int tries = 0; tries < 128; ++tries) {  // check for cell for new rand
    int xx = dx(rng_);
    int yy = dy(rng_);
    if (!busy(xx, yy, occupied)) {
      x_ = xx;
      y_ = yy;
      return;
    }
  }
  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 10; ++x) {
      if (!busy(x, y, occupied)) {
        x_ = x;
        y_ = y;
        return;
      }
    }
  }
}

}  // namespace s21
