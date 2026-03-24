/**
* @file game_widget.h
 * @brief Qt-виджет визуализации игры "Змейка".
 */


#pragma once

#include <../../brick_game/snake/apple.h>
#include <../../brick_game/snake/snake.h>

#include <QColor>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/gameManager.h"

/**
 * @brief Отложенное направление поворота змейки.
 *
 * Используется для очереди поворотов, чтобы не терять быстрые нажатия.
 */
enum class PendingDir { None, Up, Down, Left, Right };


/**
 * @brief Виджет отрисовки и управления игрой "Змейка" в Qt.
 *
 * Отвечает за:
 * - отрисовку поля, змейки, яблока и HUD;
 * - обработку нажатий клавиш;
 * - таймер кадров и игрового шага.
 */
class GameWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 public slots:
  void startNewGame();

 signals:
  void switchToTetrisRequested();

 private slots:
  void onFrame();

  void drawBackground(QPainter& p, int field_width, int field_height,
                      int cell_size, int ox, int oy);
  void drawSnake(QPainter& p, int cell_size, int ox, int oy);
  void drawApple(QPainter& p, int cell_size, int ox, int oy);
  void drawHub(QPainter& p);
  void drawWin(QPainter& p, int cell_size);
  void drawOver(QPainter& p, int cell_size);

 private:
  QTimer* timer_{nullptr};
  QElapsedTimer elapsed_;  // таймер
  // double step_ms_ = 150.0;
  s21::GameManager manager_;
  double acc_ms_{0.0};      // счет шага змейки
  bool accelerate_{false};  // ускорение

  std::deque<PendingDir> pending_directions;
};