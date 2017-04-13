cmake_minimum_required(VERSION 3.5)

project(websocket)
#include_directories(src)

#set (SOURCE_EXE main.cpp src/handler.cpp src/handler.h)
set(SOURCE_EXE main.c++)
#set(SOURCE_LIB src/server/server.cpp)
#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -levent")
set(CMAKE_CXX_FLAGS "-std=c++11 -levent")
#set(HEADER_FILES src/handler.h src/server/server.hpp)


find_package(Boost COMPONENTS system filesystem REQUIRED)

file(GLOB_RECURSE SRC "src/*.c++")
file(GLOB_RECURSE HEADERS "src/*.hpp" "src/*hpp")


#add_library(lib src/handler.cpp src/server/server.cpp ${HEADER_FILES})
#add_library(lib ${SOURCE_LIB})
#add_executable(${PROJECT_NAME} ${SOURCE_EXE} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} ${SOURCE_EXE} ${SRC} ${HEADER_FILE})
add_executable(${PROJECT_NAME} ${SOURCE_EXE} ${SRC} ${HEADERS})


#target_link_libraries(${PROJECT_NAME}
#                      ${Boost_FILESYSTEM_LIBRARY}
#                      ${Boost_SYSTEM_LIBRARY}
#                      lib)
target_link_libraries(${PROJECT_NAME}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY})
