add_library(minesweeper SHARED
    Minesweeper.cpp
    ${CMAKE_SOURCE_DIR}/src/AGame.cpp
)

target_compile_features(minesweeper PRIVATE cxx_std_20)
set_target_properties(minesweeper PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(minesweeper PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "minesweeper")

set_target_properties(minesweeper PROPERTIES PREFIX "")
set_target_properties(minesweeper PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(minesweeper PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
