add_library(my_ncurses SHARED
    Ncurses.cpp
    ${CMAKE_SOURCE_DIR}/src/ADisplay.cpp
)

target_compile_features(my_ncurses PRIVATE cxx_std_20)
set_target_properties(my_ncurses PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(my_ncurses PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "ncurses")

target_link_libraries(my_ncurses ncurses)
set_target_properties(my_ncurses PROPERTIES PREFIX "")
set_target_properties(my_ncurses PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(my_ncurses PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
