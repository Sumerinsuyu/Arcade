cmake_minimum_required(VERSION 3.10)

project(arcade)

string(ASCII 27 Esc)
set(BoldBlue    "${Esc}[1;34m")
set(ColourReset "${Esc}[m")

message("${BoldBlue}
  ___  ______  _____   ___ ______ _____ 
 / _ \\ | ___ \\/  __ \\ / _ \\|  _  \\  ___|
/ /_\\ \\| |_/ /| /  \\// /_\\ \\ | | | |__  
|  _  ||    / | |    |  _  | | | |  __| 
| | | || |\\ \\ | \\__/\\| | | | |/ /| |___ 
\\_| |_/\\_| \\_| \\____/\\_| |_/___/ \\____/
${ColourReset}")


include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(GameLib)
add_subdirectory(GraphLib)
set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/Core.cpp
    ${CMAKE_SOURCE_DIR}/src/LibLoader.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
    -g
)

add_custom_target(build ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:arcade> ${CMAKE_SOURCE_DIR}/arcade
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/lib ${CMAKE_SOURCE_DIR}/lib

    DEPENDS arcade my_ncurses my_sdl2 my_sfml my_caca menu snake minesweeper pacman
)
