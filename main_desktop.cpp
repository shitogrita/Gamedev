#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>

#include "gui/desktop/game_widget.h"
#include "gui/desktop/tetris_widget.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  mainWindow.setWindowTitle("BrickGame v2.0");

  auto stack = new QStackedWidget(&mainWindow);
  auto snake = new GameWidget(stack);
  auto tetris = new TetrisWidget(stack);

  stack->addWidget(snake);
  stack->addWidget(tetris);

  stack->setCurrentWidget(snake);
  mainWindow.setCentralWidget(stack);
  mainWindow.resize(400, 800);
  mainWindow.show();

  QObject::connect(snake, &GameWidget::switchToTetrisRequested, [&]() {
    tetris->startNewGame();
    stack->setCurrentWidget(tetris);
    tetris->setFocus();
  });

  QObject::connect(tetris, &TetrisWidget::switchToSnakeRequested, [&]() {
    snake->startNewGame();
    stack->setCurrentWidget(snake);
    snake->setFocus();
  });

  return app.exec();
}
