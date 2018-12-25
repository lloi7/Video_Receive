PROJECT(video_transmit)

add_compile_options(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

