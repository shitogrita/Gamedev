/**
* @file game_widget.cpp
 * @brief Реализация Qt-виджета для игры "Змейка".
 */


#include "game_widget.h"

#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

GameWidget::GameWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Game Snake");
  setFocusPolicy(Qt::StrongFocus);  // tab
  setFocus();
  resize(400, 800);  // (10,20) * 40
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &GameWidget::onFrame);
  timer_->start(16);  // 60 fps
  std::srand(std::time({}));
  elapsed_.start();
}

void GameWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  const int field_width = 10;
  const int field_height = 20;
  int cell_size = std::min(width() / field_width, height() / field_height);
  int ox = (width() - cell_size * field_width) / 2;
  int oy = (height() - cell_size * field_height) / 2;

  p.fillRect(rect(), QColor(255, 255, 204));

  drawBackground(p, field_width, field_height, cell_size, ox, oy);
  drawSnake(p, cell_size, ox, oy);
  drawApple(p, cell_size, ox, oy);
  drawHub(p);
  drawOver(p, cell_size);
  drawWin(p, cell_size);
}

void GameWidget::drawBackground(QPainter& p, int field_width, int field_height,
                                int cell_size, int ox, int oy) {
  p.fillRect(rect(), QColor(255, 255, 204));

  p.setPen(QColor(204, 204, 0));
  for (int x = 0; x <= field_width; ++x) {
    int px = ox + x * cell_size;
    p.drawLine(px, oy, px, oy + field_height * cell_size);
  }
  for (int y = 0; y <= field_height; ++y) {
    int py = oy + y * cell_size;
    p.drawLine(ox, py, ox + field_width * cell_size, py);
  }
}

void GameWidget::drawApple(QPainter& p, int cell_size, int ox, int oy) {
  int ax = manager_.appleX();
  int ay = manager_.appleY();
  int px = ox + ax * cell_size;
  int py = oy + ay * cell_size;

  p.save();
  p.setBrush(Qt::red);
  p.setPen(Qt::NoPen);
  p.drawEllipse(px + 2, py + 2, cell_size - 4, cell_size - 4);
  p.restore();
}

void GameWidget::drawSnake(QPainter& p, int cell_size, int ox, int oy) {
  const auto& body = manager_.Snake().body();

  p.save();
  for (size_t i = 0; i < body.size(); ++i) {
    int x = body[i].x;
    int y = body[i].y;
    int px = ox + x * cell_size;
    int py = oy + y * cell_size;

    p.setBrush(i == 0 ? QColor(0, 102, 0) : QColor(0, 153, 0));
    p.setPen(Qt::NoPen);
    p.drawRect(px + 1, py + 1, cell_size - 2, cell_size - 2);
  }
  p.restore();
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
  PendingDir dir = PendingDir::None;
  switch (event->key()) {
    case Qt::Key_Left:
      dir = PendingDir::Left;
      break;
    case Qt::Key_Right:
      dir = PendingDir::Right;
      break;
    case Qt::Key_Up:
      dir = PendingDir::Up;
      break;
    case Qt::Key_Down:
      dir = PendingDir::Down;
      break;
    case Qt::Key_P:
      manager_.TogglePause();
      break;
    case Qt::Key_R:
      manager_.Reset();
      acc_ms_ = 0.0;
      pending_directions.clear();
      update();
      break;

    case Qt::Key_Space:
      accelerate_ = true;
      break;

    case Qt::Key_T:
      emit switchToTetrisRequested();
      startNewGame();
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
  if (dir != PendingDir::None) {
    if (pending_directions.size() < 3) {
      pending_directions.push_back(dir);
    }
  }
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Space) {
    accelerate_ = false;
  } else {
    QWidget::keyReleaseEvent(event);
  }
}

void GameWidget::startNewGame() {
  manager_.Reset();
  acc_ms_ = 0.0;
  pending_directions.clear();
  accelerate_ = false;
  update();
}

void GameWidget::drawWin(QPainter& p, int cell_size) {
  if (manager_.state() != s21::GameManager::State::Win) return;
  p.save();
  p.fillRect(rect(), QColor(0, 0, 0, 180));

  QFont f = p.font();
  f.setPointSize(25);
  f.setBold(true);
  p.setFont(f);
  p.setPen(Qt::white);
  p.drawText(QRect(100, 350, 200, 100), Qt::AlignCenter, "GAME WIN");

  p.restore();
}

void GameWidget::drawOver(QPainter& p, int cell_size) {
  if (manager_.state() != s21::GameManager::State::GameOver) return;

  p.save();
  p.fillRect(rect(), QColor(0, 0, 0, 180));

  QFont f = p.font();
  f.setPointSize(25);
  f.setBold(true);  // жирный шрифт
  p.setFont(f);     // обновление шрифта
  p.setPen(Qt::white);
  p.drawText(QRect(100, 350, 200, 100), Qt::AlignCenter, "GAME OVER");

  p.restore();
}

void GameWidget::drawHub(QPainter& p) {
  p.save();

  p.setBrush(QColor(255, 255, 255, 180));
  p.setPen(Qt::NoPen);
  p.drawRect(5, 5, 150, 90);

  p.setPen(Qt::black);
  QFont f = p.font();
  f.setPointSize(10);
  p.setFont(f);

  int score = manager_.score();
  int top = manager_.topScore();
  int apples = manager_.appleEaten();
  int level = manager_.level();

  int y = 20;
  p.drawText(10, y, QString("Score: %1").arg(manager_.score()));
  p.drawText(10, y + 15, QString("Top: %1").arg(manager_.topScore()));
  p.drawText(10, y + 30, QString("Apples: %1").arg(manager_.appleEaten()));
  p.drawText(10, y + 45, QString("Level: %1").arg(manager_.level()));

  if (accelerate_) {
    p.setPen(QColor(200, 0, 0));
    QFont f = p.font();
    f.setBold(true);
    p.setFont(f);
    p.drawText(width() - 120, 20, "BOOST!");
  }

  p.restore();
}

void GameWidget::onFrame() {
  static qint64 last = elapsed_.elapsed();
  qint64 now = elapsed_.elapsed();
  double dt = double(now - last);
  last = now;
  acc_ms_ += dt;
  double base_step_ms = 230.0;

  int lvl = manager_.level();
  double step_ms = base_step_ms - (lvl - 1) * 15.00;
  if (step_ms < 70.0) step_ms = 70.0;

  if (accelerate_) {
    step_ms *= 0.5;
  }
  if (step_ms <= 40.0) step_ms = 40.0;

  // std::cout << step_ms << std::endl;

  while (acc_ms_ >= step_ms) {
    while (!pending_directions.empty()) {
      auto d = pending_directions.front();
      pending_directions.pop_front();
      switch (d) {
        case PendingDir::Left:
          manager_.TurnLeft();
          break;
        case PendingDir::Right:
          manager_.TurnRight();
          break;
        case PendingDir::Up:
          manager_.TurnUp();
          break;
        case PendingDir::Down:
          manager_.TurnDown();
          break;
        default:
          break;
      }
      break;
    }
    manager_.Tick();
    acc_ms_ -= step_ms;
  }
  update();
}
