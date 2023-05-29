cmake_minimum_required(VERSION 3.25)
project(pyhp C)

find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(pyhp src/main.c src/pyhp.c src/runner.c include/settings.h src/log.c include/main.h src/cJSON.c src/conf.c)
target_compile_definitions(pyhp PUBLIC IS_FAST_CGI="0")
target_link_libraries(pyhp PRIVATE ${Python_LIBRARIES})
target_include_directories(pyhp PRIVATE ${Python_INCLUDE_DIRS})

add_executable(pyhp-cgi src/socket.c src/pyhp.c src/runner.c include/settings.h src/log.c src/cJSON.c src/conf.c)
target_compile_definitions(pyhp-cgi PUBLIC FAST_CGI IS_FAST_CGI="1")
target_link_libraries(pyhp-cgi PRIVATE fcgi ${Python_LIBRARIES})
target_include_directories(pyhp-cgi PRIVATE ${Python_INCLUDE_DIRS})

add_executable(pyhp-cgi-start src/start-socket.c src/cJSON.c src/conf.c src/log.c)


#install(TARGETS pyhp
#        COMPONENT pyhp
#        CONFIGURATIONS Release
#        RUNTIME DESTINATION "/home/"
#        LIBRARY DESTINATION "/home/"
#        DESTINATION "/home/"
#        )
install(TARGETS pyhp pyhp-cgi pyhp-cgi-start RUNTIME DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets/root/" DESTINATION "/")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_SOURCE_DIR}/assets/postinst")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sidney Trzepacz") #required
set(CPACK_DEBIAN_PACKAGE_DEPENDS libpython3.10 libfcgi libfcgi-bin)
set(CPACK_RPM_PACKAGE_REQUIRES libpython3.10 libfcgi libfcgi-bin)
include(CPack)

