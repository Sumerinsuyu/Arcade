add_library(my_caca SHARED
    Caca.cpp
    ${CMAKE_SOURCE_DIR}/src/ADisplay.cpp
)

target_compile_features(my_caca PRIVATE cxx_std_20)
set_target_properties(my_caca PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(my_caca PRIVATE
    -Wall
    -Wextra
    -fno-gnu-unique
)

set(LNAME "caca")

find_package(PkgConfig REQUIRED)
pkg_check_modules(CACA REQUIRED caca)

target_include_directories(my_caca PRIVATE ${CACA_INCLUDE_DIRS})
target_link_libraries(my_caca PRIVATE ${CACA_LIBRARIES})
set_target_properties(my_caca PROPERTIES PREFIX "")
set_target_properties(my_caca PROPERTIES OUTPUT_NAME "arcade_${LNAME}")

set_target_properties(my_caca PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
