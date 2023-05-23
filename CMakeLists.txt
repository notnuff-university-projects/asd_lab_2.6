cmake_minimum_required(VERSION 3.25)
project(2_6 C)

set(CMAKE_C_STANDARD 11)

add_executable(2_6 main.c "src/c files/matrix_math.c" "src/c files/graph_draw.c" "src/c files/vec_math_lib.c" "src/c files/X_routines.c" src/graph_functions.h "src/c files/graph_functions.c" "src/graph_functions.h" "src/c files/graph_functions.c" "src/c files/prime`s_algorithm.c" "src/prime`s_algorithm.h")
target_link_libraries(2_6 X11)
target_link_libraries(2_6 m)
