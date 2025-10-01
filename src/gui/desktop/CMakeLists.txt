cmake_minimum_required(VERSION 3.19)
project(desktop LANGUAGES C CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "")

qt_add_executable(desktop
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    snake_game.h snake_game.cpp snake_game.ui
    tetris_game.h tetris_game.cpp tetris_game.ui
    ../../brick_game/snake/backend_s.hpp
    ../../brick_game/snake/backend_s.cpp
    ../../brick_game/tetris/backend_t.h
    ../../brick_game/tetris/backend_t.c
    exit.h exit.cpp exit.ui
)

find_package(Qt6 REQUIRED COMPONENTS Core)

target_link_libraries(desktop
    PRIVATE
        Qt::Core
        Qt6::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS desktop
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET desktop
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
