add_library(snake SHARED
    Snake.cpp
    ${CMAKE_SOURCE_DIR}/src/AGame.cpp
)

target_compile_features(snake PRIVATE cxx_std_20)
set_target_properties(snake PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(snake PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "snake")

set_target_properties(snake PROPERTIES PREFIX "")
set_target_properties(snake PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(snake PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
