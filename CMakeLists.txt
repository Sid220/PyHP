cmake_minimum_required(VERSION 3.25)
project(PyHP C)

find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(PyHP src/main.c src/pyhp.c src/runner.c include/settings.h src/log.c include/main.h src/cJSON.c src/conf.c)
target_compile_definitions(PyHP PUBLIC IS_FAST_CGI="0")
target_link_libraries(PyHP PRIVATE ${Python_LIBRARIES})
target_include_directories(PyHP PRIVATE ${Python_INCLUDE_DIRS})

add_executable(PyHPCGI src/socket.c src/pyhp.c src/runner.c include/settings.h src/log.c src/conf.c)
target_compile_definitions(PyHPCGI PUBLIC FAST_CGI IS_FAST_CGI="1")
target_link_libraries(PyHPCGI PRIVATE fcgi ${Python_LIBRARIES})
target_include_directories(PyHPCGI PRIVATE ${Python_INCLUDE_DIRS})

