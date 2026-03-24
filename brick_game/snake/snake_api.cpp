#include "snake_api.h"

#include <ctime>
#include <vector>

#include "apple.h"
#include "gameManager.h"
#include "snake.h"

static const int SNAKE_WIDTH = 10;
static const int SNAKE_HEIGHT = 20;

static int field_storage[SNAKE_HEIGHT][SNAKE_WIDTH];
static int* field_ptrs[SNAKE_HEIGHT];
static bool inited_ptrs = false;

static s21::GameManager g_manager;

static void fillFieldFromManager() {
  for (int y = 0; y < SNAKE_HEIGHT; ++y) {
    for (int x = 0; x < SNAKE_WIDTH; ++x) {
      field_storage[y][x] = 0;
    }
  }

  const auto& body = g_manager.Snake().body();
  for (size_t i = 0; i < body.size(); ++i) {
    int x = body[i].x;
    int y = body[i].y;
    if (x >= 0 && x < SNAKE_WIDTH && y >= 0 && y < SNAKE_HEIGHT) {
      field_storage[y][x] = 1;
    }
  }

  int ax = g_manager.appleX();
  int ay = g_manager.appleY();
  if (ax >= 0 && ax < SNAKE_WIDTH && ay >= 0 && ay < SNAKE_HEIGHT) {
    field_storage[ay][ax] = 2;
  }
}

extern "C" {

void snakeUserInput(SnakeAction_t action) {
  switch (action) {
    case SNAKE_ACTION_UP:
      g_manager.TurnUp();
      break;
    case SNAKE_ACTION_DOWN:
      g_manager.TurnDown();
      break;
    case SNAKE_ACTION_LEFT:
      g_manager.TurnLeft();
      break;
    case SNAKE_ACTION_RIGHT:
      g_manager.TurnRight();
      break;
    case SNAKE_ACTION_PAUSE:
      g_manager.TogglePause();
      break;
    case SNAKE_ACTION_RESET:
      g_manager.Reset();
      break;
    case SNAKE_ACTION_NONE:
    default:
      break;
  }
}

SnakeInfo_t snakeUpdateState() {
  if (!inited_ptrs) {
    for (int y = 0; y < SNAKE_HEIGHT; ++y) field_ptrs[y] = field_storage[y];
    inited_ptrs = true;
    g_manager.Reset();
  }

  g_manager.Tick();

  fillFieldFromManager();

  SnakeInfo_t info{};
  info.width = SNAKE_WIDTH;
  info.height = SNAKE_HEIGHT;
  info.field = field_ptrs;
  info.score = g_manager.score();
  info.high_score = g_manager.topScore();
  info.level = g_manager.level();
  info.apples = g_manager.appleEaten();
  info.game_over =
      (g_manager.state() == s21::GameManager::State::GameOver) ? 1 : 0;
  info.win = (g_manager.state() == s21::GameManager::State::Win) ? 1 : 0;
  info.pause = (g_manager.state() == s21::GameManager::State::Pause) ? 1 : 0;

  return info;
}
}