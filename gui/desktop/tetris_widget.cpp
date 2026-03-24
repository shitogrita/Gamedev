#include "tetris_widget.h"

#include <algorithm>
TetrisWidget::TetrisWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("BrickGame - Tetris");
  setFocusPolicy(Qt::StrongFocus);
  setFocus();

  resize(400, 800);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &TetrisWidget::onFrame);
  timer_->start(16);

  elapsed_.start();

  startNewGame();
}

void TetrisWidget::startNewGame() {
  userInput(Start, false);
  info_ = updateCurrentState();
  update();
}

void TetrisWidget::keyPressEvent(QKeyEvent* event) {
  UserAction_t action = Action;

  bool handled = true;
  switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
      action = Left;
      break;
    case Qt::Key_Right:
    case Qt::Key_D:
      action = Right;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Up:
    case Qt::Key_Space:
      action = Up;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_C:
      action = Cheats;
      break;
    case Qt::Key_Q:
      action = Terminate;
      break;
    case Qt::Key_S:
      emit switchToSnakeRequested();
      handled = false;
      break;
    default:
      handled = false;
      break;
  }

  if (handled) {
    userInput(action, false);
    info_ = updateCurrentState();

    if (action == Terminate || info_.game_over) {
      close();
    } else {
      update();
    }
  } else {
    QWidget::keyPressEvent(event);
  }
}

void TetrisWidget::onFrame() {
  info_ = updateCurrentState();

  if (info_.game_over) {
    update();
    return;
  }

  update();
}

void TetrisWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  int field_width = WIDTH;
  int field_height = HEIGHT;

  int cell_size = std::min(width() / field_width, height() / field_height);
  int ox = (width() - cell_size * field_width) / 2;
  int oy = (height() - cell_size * field_height) / 2;

  p.fillRect(rect(), QColor(20, 20, 40));

  drawBackground(p, cell_size, ox, oy);
  drawField(p, cell_size, ox, oy);
  drawHud(p);
  drawNext(p, cell_size, ox, oy);

  if (info_.pause) {
    drawOverlay(p, "PAUSE");
  } else if (info_.game_over) {
    drawOverlay(p, "GAME OVER");
  }
}

void TetrisWidget::drawBackground(QPainter& p, int cell_size, int ox, int oy) {
  p.save();
  p.setPen(QColor(80, 80, 120));

  for (int x = 0; x <= WIDTH; ++x) {
    int px = ox + x * cell_size;
    p.drawLine(px, oy, px, oy + HEIGHT * cell_size);
  }
  for (int y = 0; y <= HEIGHT; ++y) {
    int py = oy + y * cell_size;
    p.drawLine(ox, py, ox + WIDTH * cell_size, py);
  }

  p.restore();
}

void TetrisWidget::drawField(QPainter& p, int cell_size, int ox, int oy) {
  if (!info_.field) return;

  p.save();
  p.setPen(Qt::NoPen);

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (info_.field[y][x]) {
        int px = ox + x * cell_size;
        int py = oy + y * cell_size;
        p.setBrush(QColor(0, 200, 255));
        p.drawRect(px + 1, py + 1, cell_size - 2, cell_size - 2);
      }
    }
  }

  p.restore();
}

void TetrisWidget::drawNext(QPainter& p, int cell_size, int ox, int oy) {
  p.save();

  int start_x = ox + WIDTH * cell_size + 20;
  int start_y = oy + 20;

  p.setPen(Qt::white);
  p.drawText(start_x, start_y, "Next:");

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (next.shape[i][j]) {
        int px = start_x + j * cell_size;
        int py = start_y + 20 + i * cell_size;
        p.setBrush(QColor(0, 200, 255));
        p.setPen(Qt::NoPen);
        p.drawRect(px + 1, py + 1, cell_size - 2, cell_size - 2);
      }
    }
  }

  p.restore();
}

void TetrisWidget::drawHud(QPainter& p) {
  p.save();

  p.setBrush(QColor(0, 0, 0, 180));
  p.setPen(Qt::NoPen);
  p.drawRect(5, 5, 180, 90);

  p.setPen(Qt::white);
  QFont f = p.font();
  f.setPointSize(10);
  p.setFont(f);

  int y = 20;
  p.drawText(10, y, QString("Score: %1").arg(info_.score));
  p.drawText(10, y + 15, QString("High: %1").arg(info_.high_score));
  p.drawText(10, y + 30, QString("Level: %1").arg(info_.level));
  p.drawText(10, y + 45, QString("Speed: %1").arg(info_.speed));

  p.restore();
}

void TetrisWidget::drawOverlay(QPainter& p, const QString& text) {
  p.save();
  p.fillRect(rect(), QColor(0, 0, 0, 120));

  QFont f = p.font();
  f.setPointSize(24);
  f.setBold(true);
  p.setFont(f);
  p.setPen(Qt::white);

  p.drawText(rect(), Qt::AlignCenter, text);

  p.restore();
}
