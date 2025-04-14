add_library(pacman SHARED
    Pacman.cpp
    ${CMAKE_SOURCE_DIR}/src/AGame.cpp
)

target_compile_features(pacman PRIVATE cxx_std_20)
set_target_properties(pacman PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(pacman PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "pacman")

set_target_properties(pacman PROPERTIES PREFIX "")
set_target_properties(pacman PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(pacman PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
