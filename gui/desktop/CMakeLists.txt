add_library(GameWidget STATIC game_widget.cpp game_widget.h
        tetris_widget.cpp
        tetris_widget.h)
target_include_directories(GameWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(GameWidget PUBLIC Qt6::Gui Qt6::Widgets)
