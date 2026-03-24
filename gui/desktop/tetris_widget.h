#pragma once

#include <QElapsedTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

extern "C" {
#include "../../brick_game/tetris/api.h"
#include "../../brick_game/tetris/tetris.h"
}

class TetrisWidget : public QWidget {
  Q_OBJECT
 public:
  explicit TetrisWidget(QWidget* parent = nullptr);

 public slots:
  void startNewGame();

 protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

 private:
  void drawBackground(QPainter& p, int cell_size, int ox, int oy);
  void drawField(QPainter& p, int cell_size, int ox, int oy);
  void drawNext(QPainter& p, int cell_size, int ox, int oy);
  void drawHud(QPainter& p);
  void drawOverlay(QPainter& p, const QString& text);

 signals:
  void switchToSnakeRequested();

 private slots:
  void onFrame();

 private:
  QTimer* timer_{nullptr};
  QElapsedTimer elapsed_;

  GameInfo_t info_{};
};
