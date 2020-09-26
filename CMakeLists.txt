cmake_minimum_required(VERSION 3.13)
project(TSP-Solver)


add_executable(TSP-Solver main.cpp Graph.cpp ExactAlgorithms.cpp LocalSearchAlgorithms.cpp Ant.cpp ACO.cpp)

target_link_directories(TSP-Solver PUBLIC
        "$(PROJECT_BINARY_DIR)"
        )
target_link_libraries(TSP-Solver PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        )