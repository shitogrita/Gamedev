add_library(SnakeCli STATIC
        snake_cli.c
)

target_include_directories(SnakeCli PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../brick_game/snake
)

target_link_libraries(SnakeCli PRIVATE
        ${CURSES_LIBRARIES}
        GameManager Snake Apple SnakeAPI
)

add_library(TetrisCli STATIC tetris_cli.c interface.c debug_tools.c)

target_include_directories(TetrisCli PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../brick_game/tetris
)
target_link_libraries(TetrisCli PRIVATE
        ${CURSES_LIBRARIES}
        TetrisLogic
)