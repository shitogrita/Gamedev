add_library(GameManager STATIC gameManager.cpp gameManager.h)
target_include_directories(GameManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Snake STATIC snake.cpp snake.h)
target_include_directories(Snake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Apple STATIC apple.cpp apple.h)
target_include_directories(Apple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(SnakeAPI STATIC snake_api.cpp snake_api.h)
target_include_directories(SnakeAPI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

