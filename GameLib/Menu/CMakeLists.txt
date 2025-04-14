add_library(menu SHARED
    Menu.cpp
    ${CMAKE_SOURCE_DIR}/src/AGame.cpp
)

target_compile_features(menu PRIVATE cxx_std_20)
set_target_properties(menu PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(menu PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "menu")

set_target_properties(menu PROPERTIES PREFIX "")
set_target_properties(menu PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(menu PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
