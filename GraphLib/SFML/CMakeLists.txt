find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_library(my_sfml SHARED
    Sfml.cpp
    ${CMAKE_SOURCE_DIR}/src/ADisplay.cpp
)

target_compile_features(my_sfml PRIVATE cxx_std_20)
set_target_properties(my_sfml PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(my_sfml PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "sfml")

target_link_libraries(my_sfml sfml-system sfml-window sfml-graphics)
set_target_properties(my_sfml PROPERTIES PREFIX "")
set_target_properties(my_sfml PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(my_sfml PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
