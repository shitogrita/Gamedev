/**
* @file gameManager.h
 * @brief Класс-менеджер игры "Змейка".
 */


#pragma once  // FSM
#include <fstream>
#include <utility>
#include <vector>

#include "apple.h"
#include "snake.h"

namespace s21 {

  /**
 * @brief Управляет состоянием игры "Змейка".
 *
 * Отвечает за:
 * - запуск/паузу/сброс игры;
 * - движение змейки и спавн яблок;
 * - подсчёт очков и уровней;
 * - сохранение/загрузку рекорда (topScore.txt).
 */
class GameManager {
 public:
  enum class State { Start, Running, Pause, GameOver, Win };

  GameManager() {
    std::vector<std::pair<int, int>> occupied;
    for (const auto& s : snake_.body()) {
      occupied.emplace_back(s.x, s.y);
    }
    apple_.spawn(occupied);
    state_ = State::Start;
    score_ = 0;
    handleTopScore(false);
  }

  void Reset() {
    snake_ = snake();

    std::vector<std::pair<int, int>> occupied;
    for (const auto& s : snake_.body()) {
      occupied.emplace_back(s.x, s.y);
    }
    apple_.spawn(occupied);
    state_ = State::Start;
    score_ = 0;
    apple_eatan = 0;
    level_ = 0;
    handleTopScore(false);
  }

  void Start() {
    if (state_ == State::Start || state_ == State::Pause)
      state_ = State::Running;
  }

  void Pause() {
    if (state_ == State::Running) state_ = State::Pause;
  }

  void Resume() {
    if (state_ == State::Pause) state_ = State::Running;
  }

  void TogglePause() {
    if (state_ == State::Running)
      Pause();
    else if (state_ == State::Pause)
      Resume();
  }

  void Tick(bool accelerate = false) {
    if (state_ == State::Start) state_ = State::Running;
    if (state_ != State::Running) return;

    bool ate = snake_.move(appleX(), appleY());

    if (snake_.isDead()) {
      state_ = State::GameOver;
      return;
    }
    if (snake_.isWin()) {
      state_ = State::Win;
      return;
    }

    if (ate) {
      std::vector<std::pair<int, int>>
          occ;  // занятые клетки провряка для спавна яблок
      for (auto& s : snake_.body()) occ.emplace_back(s.x, s.y);
      apple_.spawn(occ);
      apple_eatan += 1;
      score_ += 1;
      if (score_ > top_score_) {
        top_score_ = score_;
        handleTopScore(true);
      }
      int new_level = score_ / 5 + 1;
      if (new_level > 10) new_level = 10;
      level_ = new_level;
    }
  }

  void handleTopScore(bool save = false) {
    const char* filename = "topScore.txt";

    if (save) {
      std::ofstream file(filename);
      if (file.is_open()) {
        file << top_score_;
        file.close();
      }
    } else {
      std::ifstream file(filename);
      if (file.is_open()) {
        file >> top_score_;
        file.close();
      } else {
        top_score_ = 0;
      }
    }
  }

  void TurnLeft() { snake_.turn(Left); }
  void TurnRight() { snake_.turn(Right); }
  void TurnUp() { snake_.turn(Up); }
  void TurnDown() { snake_.turn(Down); }

  // for Qt
  const snake& Snake() const { return snake_; }
  int appleX() const { return apple_.x(); }
  int appleY() const { return apple_.y(); }
  State state() const { return state_; }
  int score() const { return score_; }
  int topScore() const { return top_score_; }
  int level() const { return level_; }
  int appleEaten() const { return apple_eatan; }

 private:
  snake snake_;
  apple apple_;
  State state_{State::Start};
  int score_{0};
  int apple_eatan{0};
  int top_score_{0};
  int level_{0};
};
}  // namespace s21
