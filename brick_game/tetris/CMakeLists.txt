# brick_game/tetris/CMakeLists.txt

add_library(TetrisLogic STATIC
        api.c
        tetris.c
        fsm.c
)

target_include_directories(TetrisLogic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
