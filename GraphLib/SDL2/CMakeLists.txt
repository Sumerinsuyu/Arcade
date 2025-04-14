add_library(my_sdl2 SHARED
    Sdl2.cpp
    ${CMAKE_SOURCE_DIR}/src/ADisplay.cpp
)

target_compile_features(my_sdl2 PRIVATE cxx_std_20)
set_target_properties(my_sdl2 PROPERTIES POSITION_INDEPENDENT_CODE ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_compile_options(my_sdl2 PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "sdl2")

target_include_directories(my_sdl2 PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(my_sdl2 PRIVATE
    SDL2
    SDL2_ttf
)

set_target_properties(my_sdl2 PROPERTIES PREFIX "")
set_target_properties(my_sdl2 PROPERTIES OUTPUT_NAME "arcade_${LNAME}")
set_target_properties(my_sdl2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
