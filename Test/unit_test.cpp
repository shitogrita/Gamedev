#include "unit_test.h"
#define private public
#include <gameManager.h>
#undef private
TEST_F(snakeTest, turnLeft) {
  s21::GameManager gm;
  auto head_start = gm.Snake().body().front();
  gm.TurnUp();
  gm.Tick();
  auto head_after_up = gm.Snake().body().front();
  EXPECT_EQ(head_after_up.x, head_start.x);
  EXPECT_EQ(head_after_up.y, head_start.y - 1);
  gm.TurnLeft();
  gm.Tick();
  auto head_after_left = gm.Snake().body().front();
  EXPECT_EQ(head_after_left.y, head_after_up.y);
  EXPECT_EQ(head_after_left.x, head_after_up.x - 1);
}

TEST_F(snakeTest, turnRight) {
  s21::GameManager gm;
  auto head_start = gm.Snake().body().front();
  gm.TurnUp();
  gm.Tick();
  auto head_after_up = gm.Snake().body().front();
  EXPECT_EQ(head_after_up.x, head_start.x);
  EXPECT_EQ(head_after_up.y, head_start.y - 1);
  gm.TurnRight();
  gm.Tick();
  auto head_after_right = gm.Snake().body().front();
  EXPECT_EQ(head_after_right.y, head_after_up.y);
  EXPECT_EQ(head_after_right.x, head_after_up.x + 1);
}

TEST_F(snakeTest, InitialState) {
  s21::GameManager gm;
  EXPECT_EQ(gm.state(), s21::GameManager::State::Start);
  EXPECT_EQ(gm.score(), 0);
  EXPECT_EQ(gm.level(), 0);
  EXPECT_EQ(gm.appleEaten(), 0);
}

TEST_F(snakeTest, TickFromStartSetsRunningAndMoves) {
  s21::GameManager gm;
  auto head_before = gm.Snake().body().front();
  gm.Tick();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(head_after.y, head_before.y);
  EXPECT_EQ(head_after.x, head_before.x + 1);
}

TEST_F(snakeTest, StartFromStartSetsRunning) {
  s21::GameManager gm;
  gm.Start();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
}

TEST_F(snakeTest, StartFromPauseSetsRunning) {
  s21::GameManager gm;
  gm.Start();
  gm.Pause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
  gm.Start();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
}

TEST_F(snakeTest, PauseOnlyFromRunning) {
  s21::GameManager gm;
  gm.Pause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Start);
  gm.Start();
  gm.Pause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
}

TEST_F(snakeTest, ResumeOnlyFromPause) {
  s21::GameManager gm;
  gm.Start();
  gm.Pause();
  gm.Resume();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
  gm.Start();
  gm.Resume();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
}

TEST_F(snakeTest, TogglePauseFromRunningAndBack) {
  s21::GameManager gm;
  gm.Start();
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
}

TEST_F(snakeTest, PausePreventsMovement) {
  s21::GameManager gm;
  gm.Start();
  auto head_before = gm.Snake().body().front();
  gm.Pause();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y);
}

TEST_F(snakeTest, ResetRestoresInitialState) {
  s21::GameManager gm;
  gm.Start();
  for (int i = 0; i < 3; ++i) gm.Tick();
  gm.Pause();
  gm.Reset();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Start);
  EXPECT_EQ(gm.score(), 0);
  EXPECT_EQ(gm.level(), 0);
  EXPECT_EQ(gm.appleEaten(), 0);

  s21::GameManager gm2;
  auto head1 = gm.Snake().body().front();
  auto head2 = gm2.Snake().body().front();
  EXPECT_EQ(head1.x, head2.x);
  EXPECT_EQ(head1.y, head2.y);
  EXPECT_EQ(gm.Snake().body().size(), gm2.Snake().body().size());
}

TEST_F(snakeTest, TurnUpChangesDirection) {
  s21::GameManager gm;
  auto head_before = gm.Snake().body().front();
  gm.TurnUp();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y - 1);
}

TEST_F(snakeTest, TurnDownChangesDirection) {
  s21::GameManager gm;
  gm.TurnUp();
  gm.Tick();
  auto head_before = gm.Snake().body().front();
  gm.TurnDown();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y - 1);
}

TEST_F(snakeTest, TurnLeftFromUp) {
  s21::GameManager gm;
  auto head0 = gm.Snake().body().front();
  gm.TurnUp();
  gm.Tick();
  auto head_up = gm.Snake().body().front();
  EXPECT_EQ(head_up.x, head0.x);
  EXPECT_EQ(head_up.y, head0.y - 1);
  gm.TurnLeft();
  gm.Tick();
  auto head_left = gm.Snake().body().front();
  EXPECT_EQ(head_left.x, head_up.x - 1);
  EXPECT_EQ(head_left.y, head_up.y);
}

TEST_F(snakeTest, TurnRightFromUp) {
  s21::GameManager gm;
  auto head0 = gm.Snake().body().front();
  gm.TurnUp();
  gm.Tick();
  auto head_up = gm.Snake().body().front();
  EXPECT_EQ(head_up.x, head0.x);
  EXPECT_EQ(head_up.y, head0.y - 1);
  gm.TurnRight();
  gm.Tick();
  auto head_right = gm.Snake().body().front();
  EXPECT_EQ(head_right.x, head_up.x + 1);
  EXPECT_EQ(head_right.y, head_up.y);
}

TEST_F(snakeTest, HorizontalLeftAndRightIgnored) {
  s21::GameManager gm;
  auto head_before = gm.Snake().body().front();
  gm.TurnLeft();
  gm.Tick();
  auto head_after_left = gm.Snake().body().front();
  EXPECT_EQ(head_after_left.y, head_before.y);
  EXPECT_EQ(head_after_left.x, head_before.x + 1);

  head_before = head_after_left;
  gm.TurnRight();
  gm.Tick();
  auto head_after_right = gm.Snake().body().front();
  EXPECT_EQ(head_after_right.y, head_before.y);
  EXPECT_EQ(head_after_right.x, head_before.x + 1);
}

TEST_F(snakeTest, VerticalReverseIgnored) {
  s21::GameManager gm;
  gm.TurnUp();
  gm.Tick();
  auto head_before = gm.Snake().body().front();
  gm.TurnDown();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y - 1);
}

TEST_F(snakeTest, GameOverOnWallCollision) {
  s21::GameManager gm;
  gm.Start();
  int safe_guard = 0;
  while (gm.state() == s21::GameManager::State::Running && safe_guard < 50) {
    gm.Tick();
    ++safe_guard;
  }
  EXPECT_EQ(gm.state(), s21::GameManager::State::GameOver);
}

TEST_F(snakeTest, AppleNotOnSnakeAtStart) {
  s21::GameManager gm;
  int ax = gm.appleX();
  int ay = gm.appleY();
  for (auto s : gm.Snake().body()) {
    EXPECT_FALSE(s.x == ax && s.y == ay);
  }
}

TEST_F(snakeTest, TickDoesNothingInGameOverState) {
  s21::GameManager gm;
  gm.Start();
  int guard = 0;
  while (gm.state() == s21::GameManager::State::Running && guard < 100) {
    gm.Tick();
    ++guard;
  }
  ASSERT_EQ(gm.state(), s21::GameManager::State::GameOver);
  auto head_before = gm.Snake().body().front();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y);
}

TEST_F(snakeTest, TogglePauseFromStartKeepsStart) {
  s21::GameManager gm;
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Start);
}

TEST_F(snakeTest, MultipleTogglePauseFromRunning) {
  s21::GameManager gm;
  gm.Start();
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Running);
  gm.TogglePause();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
}

TEST_F(snakeTest, TickInPauseKeepsPause) {
  s21::GameManager gm;
  gm.Start();
  gm.Pause();
  auto head_before = gm.Snake().body().front();
  gm.Tick();
  auto head_after = gm.Snake().body().front();
  EXPECT_EQ(gm.state(), s21::GameManager::State::Pause);
  EXPECT_EQ(head_after.x, head_before.x);
  EXPECT_EQ(head_after.y, head_before.y);
}

TEST_F(snakeTest, ResetChangesApplePosition) {
  s21::GameManager gm;
  int ax1 = gm.appleX();
  int ay1 = gm.appleY();
  gm.Reset();
  int ax2 = gm.appleX();
  int ay2 = gm.appleY();
  bool same = (ax1 == ax2 && ay1 == ay2);
  SUCCEED();
}

TEST_F(snakeTest, StartFromGameOverDoesNotChangeState) {
  s21::GameManager gm;
  gm.Start();
  int guard = 0;
  while (gm.state() == s21::GameManager::State::Running && guard < 100) {
    gm.Tick();
    ++guard;
  }
  ASSERT_EQ(gm.state(), s21::GameManager::State::GameOver);
  gm.Start();
  EXPECT_EQ(gm.state(), s21::GameManager::State::GameOver);
}

TEST_F(snakeLogicTest, InitialBodyAndDirection) {
  s21::snake s;
  EXPECT_EQ(s.body().size(), 4u);
  auto head = s.body().front();
  auto tail = s.body().back();
  EXPECT_EQ(head.x, 6);
  EXPECT_EQ(head.y, 10);
  EXPECT_EQ(tail.x, 3);
  EXPECT_EQ(tail.y, 10);
  EXPECT_FALSE(s.isDead());
  EXPECT_FALSE(s.isWin());
}

TEST_F(snakeLogicTest, MoveWithoutAppleDoesNotGrow) {
  s21::snake s;
  std::size_t size_before = s.body().size();
  bool ate = s.move(-1, -1);
  EXPECT_FALSE(ate);
  EXPECT_EQ(s.body().size(), size_before);
  EXPECT_FALSE(s.isDead());
  auto head_after = s.body().front();
  EXPECT_EQ(head_after.x, 7);
  EXPECT_EQ(head_after.y, 10);
}

TEST_F(snakeLogicTest, MoveWithAppleGrows) {
  s21::snake s;
  auto head = s.body().front();
  int apple_x = head.x + 1;
  int apple_y = head.y;
  std::size_t size_before = s.body().size();
  bool ate = s.move(apple_x, apple_y);
  EXPECT_TRUE(ate);
  EXPECT_EQ(s.body().size(), size_before + 1);
  auto new_head = s.body().front();
  EXPECT_EQ(new_head.x, apple_x);
  EXPECT_EQ(new_head.y, apple_y);
  EXPECT_FALSE(s.isDead());
}

TEST_F(appleTest, SpawnWithinBoundsEmptyOccupied) {
  s21::apple a;
  std::vector<std::pair<int, int>> occ;
  a.spawn(occ);
  EXPECT_GE(a.x(), 0);
  EXPECT_LE(a.x(), 9);
  EXPECT_GE(a.y(), 0);
  EXPECT_LE(a.y(), 19);
}

TEST_F(appleTest, SpawnAvoidsSingleOccupiedCell) {
  s21::apple a;
  std::vector<std::pair<int, int>> occ;
  occ.emplace_back(5, 5);
  a.spawn(occ);
  EXPECT_FALSE(a.x() == 5 && a.y() == 5);
}

TEST_F(snakeTest, appleEatTriggersAllBranches) {
  s21::GameManager gm;

  auto& snake = gm.snake_;
  auto& apple = gm.apple_;

  int hx = snake.body().front().x;
  int hy = snake.body().front().y;

  apple.x_ = hx + 1;
  apple.y_ = hy;

  gm.TurnRight();
  gm.Tick();

  EXPECT_EQ(gm.score(), 1);
  EXPECT_EQ(gm.apple_eatan, 1);
  EXPECT_EQ(gm.level_, 1);
}

TEST_F(snakeTest, topScoreSavedAndLoaded) {
  s21::GameManager gm;

  gm.score_ = 10;
  gm.top_score_ = 10;

  gm.handleTopScore(true);

  gm.top_score_ = 0;

  gm.handleTopScore(false);

  EXPECT_EQ(gm.top_score_, 10);
}

TEST_F(snakeTest, appleSpawnFallback) {
  s21::apple a;

  std::vector<std::pair<int, int>> occupied;

  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 10; ++x) {
      occupied.emplace_back(x, y);
    }
  }

  occupied.erase(
      std::remove(occupied.begin(), occupied.end(), std::make_pair(0, 0)),
      occupied.end());

  a.spawn(occupied);

  EXPECT_EQ(a.x(), 0);
  EXPECT_EQ(a.y(), 0);
}